#include "ScriptMgr.h"
#include "Player.h"
#include "Guild.h"
#include "DatabaseEnv.h"
#include "Chat.h"
#include "World.h"

using namespace Acore::ChatCommands;

class guild_teleport_commandscript : public CommandScript {
    public:
        guildteleport_commandscript() : CommandScript("guild_teleport_commandscript") { }

        ChatCommandTable GetCommands() const override {
            static ChatCommandTable teleportTable = {
                { "", HandleGuildTeleportCommand, SEC_PLAYER, Console::No }
            };

            static ChatCommandTable commandTable = {
                { "guildteleport", teleportTable },
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

void AddSC_guild_teleport_commandscript() {
    new guild_teleport_commandscript();
}