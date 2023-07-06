#include "BotApplication.h"
#include "BotManager.h"
#include "chCommandArguments.h"

#include <dpp/dpp.h>


namespace chBot {
BotApp::BotApp() {}

BotApp::~BotApp() {}

void BotApp::init(chEngineSDK::String token) {
  m_discordBot = chEngineSDK::ch_shared_ptr_new<dpp::cluster>(token);

  //TDOO: make own logger
  m_discordBot->on_log(dpp::utility::cout_logger());

  m_discordBot->on_guild_create([](const dpp::guild_create_t& guild) {
    BotManager::instance().registerBot(guild.created->id);
  });

  m_discordBot->on_ready([this](const dpp::ready_t& event) {
    this->notifyOwner();
  });

  m_discordBot->start();
}

void BotApp::run() {
  while (m_isRunning) {
    if (!m_isDirty) {
      continue;
    }
  }
}

void BotApp::setDirty() {
  m_isDirty = true;
}

void BotApp::notifyOwner() {
  const chEngineSDK::String OWNER_ID = CmdParser::getInstance().getParam("ownerid");
  if (OWNER_ID.empty()) {
    return;
  }

  m_discordBot->create_dm_channel(OWNER_ID, [this](const dpp::confirmation_callback_t& confirmation) {
    if (confirmation.is_error()) {
      std::cout << confirmation.get_error().message << "\n";
      return;
    }

    dpp::channel channel = std::get<dpp::channel>(confirmation.value);

    dpp::message msg(channel.id, "Bot is online!");
    m_discordBot->message_create(msg, [](const dpp::confirmation_callback_t& confirmation) {
      if (confirmation.is_error()) {
        std::cout << confirmation.get_error().message << "\n";
        return;
      }
    });
   });
}

}