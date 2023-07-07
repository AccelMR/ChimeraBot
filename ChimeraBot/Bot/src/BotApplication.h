#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"

#include <dpp/dpp.h>

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
  struct slashcommand_t;
  struct ready_t;
}

namespace chBot {
class BaseCommand;

typedef std::pair<std::unique_ptr<dpp::slashcommand_t>, chEngineSDK::SPtr<BaseCommand>> ResponseCommandPair;

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

  void
  loadCommands();

  void
  onClusterReady(const dpp::ready_t& event);

 private:
  chEngineSDK::SPtr<dpp::cluster> m_discordBot;
  bool m_isRunning = true;
  bool m_isDispatcherRunning = true;
  bool m_isDirty = false;

  std::thread m_commandDispatcher;
  std::mutex m_queueMutex; 
  std::queue<ResponseCommandPair> m_commandQueue;

};

}