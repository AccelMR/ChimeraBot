// Bot.cpp : Defines the entry point for the application.
//

#include "Bot.h"
#include <dpp/dpp.h>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Please provide the bot token as an argument." << std::endl;
    return 1;
  }

  std::string BOT_TOKEN = argv[1];

  dpp::cluster bot(BOT_TOKEN);

  bot.on_log(dpp::utility::cout_logger());

  bot.on_slashcommand([](const dpp::slashcommand_t& event) {
    if (event.command.get_command_name() == "ping") {
      event.reply("Pong!");
    }
    });

  bot.on_ready([&bot](const dpp::ready_t& event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(
        dpp::slashcommand("ping", "Ping pong!", bot.me.id)
      );
    }
    });

  bot.start(dpp::st_wait);

  return 0;
}
