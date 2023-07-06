#include "BotManager.h"

#include <dpp/dpp.h>
#include "Bot.h"
#include "chDebug.h"
#include "chStringUtils.h"

namespace chBot {
using namespace chEngineSDK;

std::string uint64_to_string(uint64 value) {
  std::ostringstream os;
  os << value;
  return os.str();
}

BotManager::BotManager() {
}

BotManager::~BotManager() {
}

void 
BotManager::registerBot(const dpp::snowflake& snowflake) {
  m_botInstances[snowflake] = ch_shared_ptr_new<Bot>(snowflake);
  LOG_INFO("Guild: [{" + uint64_to_string(snowflake) + "}] has a new instance of Bot.");
}

chEngineSDK::SPtr<Bot>
BotManager::getBot(const dpp::snowflake& snowflake) {
  return m_botInstances[snowflake];
}

}