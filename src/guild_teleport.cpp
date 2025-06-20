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

                uint32 guildId = guild->GetId();
                QueryResult result = WorldDatabase.Query(
                    "SELECT map, x, y, z, o FROM guild_teleport_locations WHERE guild_id = %u", guildId);

                if (!result) {
                    ChatHandler(player->GetSession()).SendSysMessage("Your guild has no teleport location set.");
                    return;
                }

                Field* fields = result->Fetch();
                uint32 map = fields[0].GetUInt32();
                float x = fields[1].GetFloat();
                float y = fields[2].GetFloat();
                float z = fields[3].GetFloat();
                float o = fields[4].GetFloat();

                player->TeleportTo(map, x, y, z, o);
            }

            void Register() override {
                OnEffectHitTarget += SpellEffectFn(GuildTeleportSpell_SpellScript::HandleTeleport, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
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

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to guild base", GOSSIP_SENDER_MAIN, 1);

            if (guild->GetLeaderGUID() == player->GetGUID())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Set this location as the guild base", GOSSIP_SENDER_MAIN, 2);

            player->SEND_GOSSIP_MENU(1, creature->GetGUID());
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
                QueryResult result = WorldDatabase.Query(
                    "SELECT map, x, y, z, o FROM guild_teleport_locations WHERE guild_id = %u",
                    guild->GetId());

                if (!result) {
                    ChatHandler(player->GetSession()).SendSysMessage("Teleport location not configured for your guild.");
                    return true;
                }

                Field* fields = result->Fetch();
                uint32 mapId = fields[0].GetUInt32();
                float x = fields[1].GetFloat();
                float y = fields[2].GetFloat();
                float z = fields[3].GetFloat();
                float o = fields[4].GetFloat();

                player->TeleportTo(mapId, x, y, z, o);
            }
            else if (action == 2) {
                if (guild->GetLeaderGUID() != player->GetGUID()) {
                    ChatHandler(player->GetSession()).SendSysMessage("Only the guild leader can set the guild teleport location.");
                    return true;
                }

                float x = creature->GetPositionX();
                float y = creature->GetPositionY();
                float z = creature->GetPositionZ();
                float o = creature->GetOrientation();
                uint32 mapId = creature->GetMapId();

                WorldDatabase.Execute(
                    "REPLACE INTO guild_teleport_locations (guild_id, map, x, y, z, o) VALUES (%u, %u, %f, %f, %f, %f)",
                    guild->GetId(), mapId, x, y, z, o);

                ChatHandler(player->GetSession()).SendSysMessage("Guild teleport location has been set to this NPC's location.");
            }

            return true;
        }
};

void AddGuildTeleportNPCScripts() {
    new GuildTeleportNPC();
}

void Addmod_guild_teleportScripts() {
    AddGuildTeleportNPCScripts();
    AddSpellGuildTeleportScripts();
}