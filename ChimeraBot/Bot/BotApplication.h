#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"

#include <dpp/dpp.h>

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
  class slashcommand_t;
}

namespace chBot {

class BaseCommand;
class BotApp: public chEngineSDK::Module<BotApp> {
 public:
  BotApp();
  ~BotApp();

  void
  init(const chEngineSDK::String& token);

  void 
  run();

  void
  setDirty();

 private:

  void
  notifyOwner();

  void
  onSlashCommand(const dpp::slashcommand_t& event);

  void
  commandDispatcherThread();

 private:
  chEngineSDK::SPtr<dpp::cluster> m_discordBot;
  bool m_isRunning = true;
  bool m_isDispatcherRunning = true;
  bool m_isDirty = false;

  std::thread m_commandDispatcher;
  std::mutex m_queueMutex; 
  std::queue<std::pair<std::unique_ptr<dpp::slashcommand_t>, chEngineSDK::SPtr<BaseCommand>>> m_commandQueue;
  chEngineSDK::Map<chEngineSDK::String, std::function<chEngineSDK::SPtr<BaseCommand>()>> m_loadedCommands;

};

}