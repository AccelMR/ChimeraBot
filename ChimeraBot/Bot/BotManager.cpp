#include "BotManager.h"


namespace chBot {

BotManager::BotManager() {
}

BotManager::~BotManager() {
}

void 
BotManager::registerBot(const dpp::snowflake& snowflake) {
  m_botInstances[snowflake] = chEngineSDK::ch_shared_ptr_new<Bot>(snowflake);
}

chEngineSDK::SPtr<Bot>
BotManager::getBot(const dpp::snowflake& snowflake) {
  return m_botInstances[snowflake];
}

}