#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"

// Forward declaration of dpp::cluster
namespace dpp {
  class snowflake;
}

namespace chBot {
class Bot;

class BotManager: public chEngineSDK::Module<BotManager> {
 public:
  BotManager();
  ~BotManager();

  void
  registerBot(const dpp::snowflake& snowflake);

  chEngineSDK::SPtr<Bot>
  getBot(const dpp::snowflake& snowflake);

  void
  dispatchCommands();

 private:
  chEngineSDK::Map<dpp::snowflake, chEngineSDK::SPtr<Bot> > m_botInstances;
};
}