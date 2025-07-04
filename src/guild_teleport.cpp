// Guild Teleport NPC AzerothCore Module

#include "ScriptMgr.h"
#include "Player.h"
#include "Guild.h"
#include "DatabaseEnv.h"
#include "Creature.h"
#include "Chat.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ScriptedGossip.h"
#include "World.h"
#include "MapMgr.h"
#include <fmt/format.h>

#define ICON_GOSSIP_BALOON 0
#define ICON_GOSSIP_WING 2
#define ICON_GOSSIP_BOOK 3
#define ICON_GOSSIP_WHEEL1 4
#define ICON_GOSSIP_WHEEL2 5
#define ICON_GOSSIP_GOLD 6
#define ICON_GOSSIP_BALOONDOTS 7
#define ICON_GOSSIP_TABARD 8
#define ICON_GOSSIP_XSWORDS 9

#define GOSSIP_COUNT_MAX 10

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
                    player->RemoveSpellCooldown(100001, true);
                    return;
                }

                QueryResult result = WorldDatabase.Query(fmt::format(
                    "SELECT map, x, y, z FROM guild_teleport_locations WHERE guild_id = {}", guild->GetId()));

                if (!result) {
                    ChatHandler(player->GetSession()).SendSysMessage("Your guild has no teleport location set.");
                    player->RemoveSpellCooldown(100001, true);
                    return;
                }

                Field* fields = result->Fetch();
                uint32 mapId = fields[0].Get<uint32>();
                float x = fields[1].Get<float>();
                float y = fields[2].Get<float>();
                float z = fields[3].Get<float>();

                if (!MapMgr::IsValidMapCoord(mapId, x, y, z))
                    return;

                player->TeleportTo(mapId, x, y, z, 0.0f);
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

            player->PlayerTalkClass->ClearMenus();

            AddGossipItemFor(player, ICON_GOSSIP_BALOON, "Teleport to Guild Hall", GOSSIP_SENDER_MAIN, 1);

            if (guild->GetLeaderGUID() == player->GetGUID()) {
                AddGossipItemFor(player, ICON_GOSSIP_WHEEL1, "Set Guild Hall location", GOSSIP_SENDER_MAIN, 2);
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
                    "SELECT map, x, y, z FROM guild_teleport_locations WHERE guild_id = {}", guild->GetId()));

                if (!result) {
                    ChatHandler(player->GetSession()).SendSysMessage("Teleport location not configured for your guild.");
                    return true;
                }

                Field* fields = result->Fetch();
                uint32 mapId = fields[0].Get<uint32>();
                float x = fields[1].Get<float>();
                float y = fields[2].Get<float>();
                float z = fields[3].Get<float>();

                if (!MapMgr::IsValidMapCoord(mapId, x, y, z))
                    return true;

                player->TeleportTo(mapId, x, y, z, 0.0f);
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

void AddSC_guild_teleport() {
    new GuildTeleportNPC();
    new GuildTeleportSpell();
    printf(">> Guild Teleport Module Loaded!\n");
}