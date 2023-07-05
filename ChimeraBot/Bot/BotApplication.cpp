#include "BotApplication.h"
#include "BotManager.h"

#include <dpp/dpp.h>


namespace chBot {
BotApp::BotApp() {}

BotApp::~BotApp() {}

void BotApp::init(chEngineSDK::String token) {
  m_discordBot = chEngineSDK::ch_shared_ptr_new<dpp::cluster>(token);

  m_discordBot->on_log(dpp::utility::cout_logger());

  //m_discordBot->on_slashcommand([](const dpp::slashcommand_t& event) {
  //  if (event.command.get_command_name() == "ping") {
  //    event.reply("Pong!");
  //  }
  //  });

  //m_discordBot->on_ready([this](const dpp::ready_t& event) {
  //  if (dpp::run_once<struct register_bot_commands>()) {
  //    m_discordBot->global_command_create(
  //      dpp::slashcommand("ping", "Ping pong!", m_discordBot->me.id)
  //    );
  //  }
  //  });

  m_discordBot->on_guild_create([](const dpp::guild_create_t& guild) {
    BotManager::instance().registerBot(guild.created->id);
  });

  m_discordBot->start(dpp::st_wait);
}
}