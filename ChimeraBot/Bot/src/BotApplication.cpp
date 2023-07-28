#include "BotApplication.h"

#include <optional>

#include "Command.h"
#include "chCommandArguments.h"
#include "chDebug.h"
#include "PingCommand.h"
#include "CommandTypes.h"


namespace chBot {
using namespace chEngineSDK;


SPtr<BaseCommand>
createCommandInstance(const CommandType& type) {
  // Create a new instance of the command
  return LOADED_COMMANDS[type]();
}

/*
*/
bool 
checkIfError(const dpp::confirmation_callback_t& confirmation) {
  if (confirmation.is_error()) {
    LOG_ERROR(confirmation.get_error().message);
    return true;
  }
  return false;
}


/*
*/
BotApp::BotApp() {}

/*
*/
BotApp::~BotApp() {
  m_isDispatcherRunning = false; // Inform the thread to stop looping
  if (m_commandDispatcher.joinable()) {
    m_commandDispatcher.join(); // Wait for the thread to finish
  }
}

/*
*/
void 
BotApp::init(const chEngineSDK::String& token) {

  m_discordBot = chEngineSDK::ch_shared_ptr_new<dpp::cluster>(token);

  //Bind all functions
  m_discordBot->on_log(dpp::utility::cout_logger()); //TDOO: make own logger
  m_discordBot->on_guild_create([](const dpp::guild_create_t& guild) {});
  m_discordBot->on_ready(std::bind(&BotApp::onClusterReady, this, std::placeholders::_1));
  m_discordBot->on_slashcommand(std::bind(&BotApp::onSlashCommand, this, std::placeholders::_1));
  m_discordBot->on_message_context_menu(std::bind(&BotApp::onMessageCommand, this, std::placeholders::_1));

  try {
    m_discordBot->start();
  }
  catch (const dpp::invalid_token_exception& exeption) {
    LOG_ERROR(exeption.what());
  }
}

/*
*/
void 
BotApp::run() {
  while (m_isRunning) {
  }
}

/*
*/
void 
BotApp::setDirty() {
  m_isDirty = true;
}

/*
*/
void
BotApp::notifyOwner() {
  const chEngineSDK::String OWNER_ID = CmdParser::getInstance().getParam("ownerid");
  if (OWNER_ID.empty()) { return; }

  m_discordBot->create_dm_channel(OWNER_ID, [this](const dpp::confirmation_callback_t& confirmation) {
    if (checkIfError(confirmation)) { return; }

    dpp::channel channel = std::get<dpp::channel>(confirmation.value);
    sendMessage(channel.id, "Bot is online!");
  });
}

template<typename T>
void BotApp::handleCommand(const T& command, std::queue<std::pair<T, chEngineSDK::SPtr<BaseCommand>>>& queue)
{
  chEngineSDK::String commandName = command.command.get_command_name();

  // Find the command in the map
  auto it = COMMAND_NAME_TO_TYPE.find(commandName);
  if (it == COMMAND_NAME_TO_TYPE.end()) {
    LOG_WARN("Command [" + commandName + "]" + " was not found.");
    return;
  }

  const CommandType commandType = it->second;
  SPtr<BaseCommand> commandInstance = createCommandInstance(commandType);

  // Command found, lock the mutex and push it to the queue
  std::lock_guard<std::mutex> guard(m_queueMutex);
  queue.emplace(std::make_pair(command, commandInstance));
}

/*
*/
void 
BotApp::onSlashCommand(const dpp::slashcommand_t& slashCommand) {
  handleCommand(slashCommand, m_commandQueue);
}

/*
*/
void 
BotApp::onMessageCommand(const dpp::message_context_menu_t& messageCommand) {
  handleCommand(messageCommand, m_commandMessageQueue);
}

/*
*/
void 
BotApp::commandDispatcherThread() {
  while (m_isDispatcherRunning) {
    dispatchMessageCommands();
    dispatchSlashCommands();
  }
}


/*
*/
void 
BotApp::loadCommands() {
  for (auto& [type, command] : LOADED_COMMANDS) {
    auto commandInstance = command();  // Call the CommandCreator function to create an instance
    auto exclusiveGuilds = commandInstance->getExclusiveGuilds();

    const String commandName = commandInstance->getName();
    const String  commandDesc = commandInstance->getDescription();
    const dpp::slashcommand_contextmenu_type  commandType = commandInstance->getType();
    LOG_INFO("Command loaded: " + commandName);
    LOG_INFO("Command descrption: " + commandDesc);

    auto newCommand = dpp::slashcommand(commandName, commandDesc, m_discordBot->me.id);
    newCommand.set_type(commandType);

    for (const auto& option : commandInstance->getOptions()) {
      newCommand.add_option(option);
    }

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

/*
*/
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

/*
*/
void 
BotApp::dispatchSlashCommands() {
  if (m_commandQueue.empty()) { return; }

  //Thread context starts
  std::lock_guard<std::mutex> guard(m_queueMutex);
  try {
    SlashCommandInteraction commandEventPair = std::move(m_commandQueue.front());
    m_commandQueue.pop();

    InteractionCallback callback = commandEventPair.second->getCallback();
    callback(commandEventPair.first);
  }
  catch (const std::exception& e) {
    LOG_ERROR(e.what());
    return;
  }
  //Thread context ends
}

/*
*/
void 
BotApp::dispatchMessageCommands() {
  if (m_commandMessageQueue.empty()) { return; }

  //Thread context starts
  std::lock_guard<std::mutex> guard(m_queueMutex);
  try {
    MessageContexCommandtInteraction commandEventPair = std::move(m_commandMessageQueue.front());
    m_commandMessageQueue.pop();

    InteractionCallback callback = commandEventPair.second->getCallback();
    callback(commandEventPair.first);
  }
  catch (const std::exception& e) {
    LOG_ERROR(e.what());
    return;
  }
  //Thread context ends
}

/*
*/
void
BotApp::sendMessage(const dpp::snowflake channel_id, const std::string& message) {
  std::lock_guard<std::mutex> lock(m_sendMessageMutex);
  dpp::message msg(channel_id, message);
  m_discordBot->message_create(msg, [](const dpp::confirmation_callback_t& confirmation) {
    if (checkIfError(confirmation)) { return; }
    });
}

/*
*/
void
BotApp::messageReact(const dpp::message& msg, const chEngineSDK::String& reaction) {
  std::lock_guard<std::mutex> lock(m_sendMessageMutex);
  m_discordBot->message_add_reaction(msg, reaction, [](const dpp::confirmation_callback_t& confirmation) {
    if (checkIfError(confirmation)) { return; }
  });
}


}