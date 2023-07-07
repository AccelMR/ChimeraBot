#include "BotApplication.h"

#include <optional>

#include "Command.h"
#include "chCommandArguments.h"
#include "chDebug.h"
#include "PingCommand.h"
#include "CommandTypes.h"


namespace chBot {
using namespace chEngineSDK;

BotApp::BotApp() {}

BotApp::~BotApp() {
  m_isDispatcherRunning = false; // Inform the thread to stop looping
  if (m_commandDispatcher.joinable()) {
    m_commandDispatcher.join(); // Wait for the thread to finish
  }
}

void 
BotApp::init(const chEngineSDK::String& token) {

  m_discordBot = chEngineSDK::ch_shared_ptr_new<dpp::cluster>(token);

  //Bind all functions
  //TDOO: make own logger
  m_discordBot->on_log(dpp::utility::cout_logger());
  m_discordBot->on_guild_create([](const dpp::guild_create_t& guild) {});
  m_discordBot->on_ready(std::bind(&BotApp::onClusterReady, this, std::placeholders::_1));
  m_discordBot->on_slashcommand(std::bind(&BotApp::onSlashCommand, this, std::placeholders::_1));

  try {
    m_discordBot->start();
  }
  catch (const dpp::invalid_token_exception& exeption) {
    LOG_ERROR(exeption.what());
  }
}

void 
BotApp::run() {
  while (m_isRunning) {
  }
}

void 
BotApp::setDirty() {
  m_isDirty = true;
}

void
BotApp::notifyOwner() {
  const chEngineSDK::String OWNER_ID = CmdParser::getInstance().getParam("ownerid");
  if (OWNER_ID.empty()) {
    return;
  }

  m_discordBot->create_dm_channel(OWNER_ID, [this](const dpp::confirmation_callback_t& confirmation) {
    if (confirmation.is_error()) {
      LOG_ERROR(confirmation.get_error().message);
      return;
    }

    dpp::channel channel = std::get<dpp::channel>(confirmation.value);

    dpp::message msg(channel.id, "Bot is online!");
    m_discordBot->message_create(msg, [](const dpp::confirmation_callback_t& confirmation) {
      if (confirmation.is_error()) {
        LOG_ERROR(confirmation.get_error().message);
        return;
      }
    });
   });
}

void BotApp::onSlashCommand(const dpp::slashcommand_t& slashCommand) {
  chEngineSDK::String commandName = slashCommand.command.get_command_name();

  // Find the command in the map
  auto it = COMMAND_NAME_TO_TYPE.find(commandName);
  if (it == COMMAND_NAME_TO_TYPE.end()) {
    LOG_WARN("Command [" + commandName + "]" + " was not found.");
    return;
  }

  // Create a new instance of the command
  auto commandInstance = LOADED_COMMANDS[it->second]();

  // Command found, lock the mutex and push it to the queue
  std::lock_guard<std::mutex> guard(m_queueMutex);
  m_commandQueue.emplace(std::make_pair(std::make_unique<dpp::slashcommand_t>(slashCommand), commandInstance));
}

void BotApp::commandDispatcherThread() {
  while (m_isDispatcherRunning) {
    std::optional<ResponseCommandPair> commandEventPairOpt;
    {
      std::lock_guard<std::mutex> guard(m_queueMutex);
      try {
        commandEventPairOpt = std::make_optional(std::move(m_commandQueue.front()));
        m_commandQueue.pop();
      }
      catch (const std::exception& e) {
        LOG_ERROR(e.what());
      }
    }

    if (commandEventPairOpt) {
      auto& commandEventPair = *commandEventPairOpt; 
      commandEventPair.second->getCallback()(*commandEventPair.first);
    }

    //Clear pointer
    commandEventPairOpt.reset();
  }
}

void 
BotApp::loadCommands() {
  for (auto& [type, command] : LOADED_COMMANDS) {
    auto commandInstance = command();  // Call the CommandCreator function to create an instance
    auto exclusiveGuilds = commandInstance->getExclusiveGuilds();

    auto newCommand = dpp::slashcommand(commandInstance->getName(), commandInstance->getDescription(), m_discordBot->me.id);
    if (exclusiveGuilds.empty()) {
      m_discordBot->global_command_create(newCommand);
    }
    else {
      for (const auto& guildID : exclusiveGuilds) {
        m_discordBot->guild_command_create(newCommand, guildID);
      }
    }
  }
}

void 
BotApp::onClusterReady(const dpp::ready_t& event) {
  notifyOwner();

  //AS far as I know this on_ready should not be called more than once, but not sure. That's why I left this part.
  if (dpp::run_once<struct register_bot_commands>()) {
    loadCommands();
    //Execute Listener thread
    m_commandDispatcher = std::thread(&BotApp::commandDispatcherThread, this);
  }
}

}