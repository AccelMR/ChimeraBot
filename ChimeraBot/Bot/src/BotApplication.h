#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"

#include <dpp/dpp.h>

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
  struct slashcommand_t;
  struct message_context_menu_t;
  struct ready_t;
}

namespace chBot {
class BaseCommand;

using SlashCommandInteraction =  std::pair<dpp::slashcommand_t, chEngineSDK::SPtr<BaseCommand>>;
using MessageContexCommandtInteraction =  std::pair<dpp::message_context_menu_t, chEngineSDK::SPtr<BaseCommand>>;

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

  void
  sendMessage(const dpp::snowflake channel_id, const chEngineSDK::String& message);

  void
  messageReact(const dpp::message& msg, const chEngineSDK::String& reaction);

 private:
  template<typename T>
  void handleCommand(const T& command, std::queue<std::pair<T, chEngineSDK::SPtr<BaseCommand>>>& queue);

  void
  notifyOwner();

  void
  onSlashCommand(const dpp::slashcommand_t& event);


  void
  onMessageCommand(const dpp::message_context_menu_t& event);

  void
  commandDispatcherThread();

  void
  loadCommands();

  void
  onClusterReady(const dpp::ready_t& event);

  void
  dispatchSlashCommands();

  void
  dispatchMessageCommands();

 private:
  chEngineSDK::SPtr<dpp::cluster> m_discordBot;
  bool m_isRunning = true;
  bool m_isDispatcherRunning = true;
  bool m_isDirty = false;

  std::thread m_commandDispatcher;
  std::mutex m_queueMutex; 
  std::mutex m_sendMessageMutex;
  std::queue<SlashCommandInteraction> m_commandQueue;
  std::queue<MessageContexCommandtInteraction> m_commandMessageQueue;

};

}