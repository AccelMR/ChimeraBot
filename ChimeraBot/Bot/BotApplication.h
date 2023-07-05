#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
}

namespace chBot {


class BotApp: public chEngineSDK::Module<BotApp> {
 public:
  BotApp();
  ~BotApp();

  void
  init(chEngineSDK::String token);

 private:
  chEngineSDK::SPtr<dpp::cluster> m_discordBot;
};

}