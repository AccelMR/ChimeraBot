#include "BotApplication.h"
#include "BotManager.h"

#include <dpp/dpp.h>


namespace chBot {
BotApp::BotApp() {}

BotApp::~BotApp() {}

void BotApp::init(chEngineSDK::String token) {
  m_discordBot = chEngineSDK::ch_shared_ptr_new<dpp::cluster>(token);

  m_discordBot->on_log(dpp::utility::cout_logger());

  m_discordBot->on_guild_create([](const dpp::guild_create_t& guild) {
    BotManager::instance().registerBot(guild.created->id);
  });

  m_discordBot->start(dpp::st_wait);
}
}