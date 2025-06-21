// Guild Teleport NPC AzerothCore Module

#include "ScriptMgr.h"
#include "Player.h"
#include "Guild.h"
#include "DatabaseEnv.h"
#include "Creature.h"
#include "Chat.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include <fmt/format.h>

class GuildTeleportSpell : public SpellScriptLoader {
    public:
        GuildTeleportSpell() : SpellScriptLoader("GuildTeleportSpell") { }

        class GuildTeleportSpell_SpellScript : public SpellScript {
            PrepareSpellScript(GuildTeleportSpell_SpellScript);

            void HandleTeleport(SpellEffIndex /*effIndex*/) {
                Player* player = GetCaster()->ToPlayer();
                if (!player)
                    return;

                Guild* guild = player->GetGuild();
                if (!guild) {
                    ChatHandler(player->GetSession()).SendSysMessage("You are not in a guild.");
                    return;
                }

                QueryResult result = WorldDatabase.Query(fmt::format(
                    "SELECT map, x, y, z, o FROM guild_teleport_locations WHERE guild_id = {}", guild->GetId()));

                if (!result) {
                    ChatHandler(player->GetSession()).SendSysMessage("Your guild has no teleport location set.");
                    return;
                }

                Field* fields = result->Fetch();
                uint32 map = fields[0].Get<uint32>();
                float x = fields[1].Get<float>();
                float y = fields[2].Get<float>();
                float z = fields[3].Get<float>();
                float o = fields[4].Get<float>();

                player->TeleportTo(map, x, y, z, o);
            }

            void Register() override {
                OnEffectHitTarget += SpellEffectFn(GuildTeleportSpell_SpellScript::HandleTeleport, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override {
            return new GuildTeleportSpell_SpellScript();
        }
};

class GuildTeleportNPC : public CreatureScript {
    public:
        GuildTeleportNPC() : CreatureScript("GuildTeleportNPC") { }

        bool OnGossipHello(Player* player, Creature* creature) override {
            Guild* guild = player->GetGuild();
            if (!guild) {
                ChatHandler(player->GetSession()).SendSysMessage("You must be in a guild to use this teleporter.");
                return true;
            }

            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(
                0, GOSSIP_ICON_CHAT, "Teleport to Guild Hall", 1, false);

            if (guild->GetLeaderGUID() == player->GetGUID()) {
                player->PlayerTalkClass->GetGossipMenu()->AddMenuItem(
                    0, GOSSIP_ICON_INTERACT_1, "Set Guild Hall location", 2, false);
            }

            player->PlayerTalkClass->SendGossipMenu(90001, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override {
            player->PlayerTalkClass->ClearMenus();

            Guild* guild = player->GetGuild();
            if (!guild) {
                ChatHandler(player->GetSession()).SendSysMessage("You must be in a guild to use this.");
                return true;
            }

            if (action == 1) {
                QueryResult result = WorldDatabase.Query(fmt::format(
                    "SELECT map, x, y, z, o FROM guild_teleport_locations WHERE guild_id = {}", guild->GetId()));

                if (!result) {
                    ChatHandler(player->GetSession()).SendSysMessage("Teleport location not configured for your guild.");
                    return true;
                }

                Field* fields = result->Fetch();
                uint32 mapId = fields[0].Get<uint32>();
                float x = fields[1].Get<float>();
                float y = fields[2].Get<float>();
                float z = fields[3].Get<float>();
                float o = fields[4].Get<float>();

                player->TeleportTo(mapId, x, y, z, o);
            } else if (action == 2) {
                if (guild->GetLeaderGUID() != player->GetGUID()) {
                    ChatHandler(player->GetSession()).SendSysMessage("Only the guild leader can set the guild teleport location.");
                    return true;
                }

                float x = creature->GetPositionX();
                float y = creature->GetPositionY();
                float z = creature->GetPositionZ();
                float o = creature->GetOrientation();
                uint32 mapId = creature->GetMapId();

                WorldDatabase.Execute(fmt::format(
                    "REPLACE INTO guild_teleport_locations (guild_id, map, x, y, z, o) VALUES ({}, {}, {}, {}, {}, {})",
                    guild->GetId(), mapId, x, y, z, o));

                ChatHandler(player->GetSession()).SendSysMessage("Guild teleport location has been set to this NPC's location.");
            }

            return true;
        }
};

class GuildTeleport_CommandScript : public CommandScript {
    public:
        GuildTeleport_CommandScript() : CommandScript("GuildTeleport_CommandScript") { }

        Acore::ChatCommands::ChatCommandTable GetCommands() const override {
            static Acore::ChatCommands::ChatCommandTable commandTable = {
                { "guildteleport", Acore::ChatCommands::HandleChatCommand(&HandleGuildTeleportCommand), SEC_PLAYER, Acore::ChatCommands::Console::No }
            };
            return commandTable;
        }

        static bool HandleGuildTeleportCommand(ChatHandler* handler, std::string_view /*args*/) {
            Player* player = handler->GetSession()->GetPlayer();

            if (!player)
                return false;

            if (!player->GetGuild()) {
                handler->SendSysMessage("You must be in a guild to use this command.");
                return true;
            }

            player->CastSpell(player, 100001, false); // trigger the custom teleport spell
            return true;
        }
};

void Addmod_guild_teleportScripts() {
    new GuildTeleportNPC();
    new GuildTeleportSpell();
    new GuildTeleport_CommandScript();
    printf(">> Guild Teleport Module Loaded!\n");
}