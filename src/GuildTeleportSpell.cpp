#include "ScriptMgr.h"
#include "Player.h"
#include "Guild.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Chat.h"
#include "DatabaseEnv.h"

class GuildTeleportSpell : public SpellScriptLoader
{
public:
    GuildTeleportSpell() : SpellScriptLoader("GuildTeleportSpell") { }

    class GuildTeleportSpell_SpellScript : public SpellScript
    {
        PrepareSpellScript(GuildTeleportSpell_Script);

        void HandleTeleport(SpellEffIndex /*effIndex*/)
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return;

            Guild* guild = player->GetGuild();
            if (!guild)
            {
                ChatHandler(player->GetSession()).SendSysMessage("You are not in a guild.");
                return;
            }

            uint32 guildId = guild->GetId();
            QueryResult result = WorldDatabase.PQuery(
                "SELECT map, x, y, z, o FROM guild_teleport_locations WHERE guild_id = %u", guildId);

            if (!result)
            {
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

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(GuildTeleportSpell_Script::HandleTeleport, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
        }
    };

    SpellScript* AddSpellGuildTeleportScripts() const override
    {
        return new GuildTeleportSpell_Script();
    }
};