#include "BotApplication.h"
#include "BotManager.h"
#include "chCommandArguments.h"
#include "chDebug.h"
#include "Command.h"
#include <optional>


namespace chBot {
using namespace chEngineSDK;

BotApp::BotApp() {}

BotApp::~BotApp() {
  if (m_commandDispatcher.joinable()) {
    m_commandDispatcher.join();
  }
  chBot::BotManager::shutDown();
}

void 
BotApp::init(const chEngineSDK::String& token) {
  chBot::BotManager::startUp();

  m_discordBot = chEngineSDK::ch_shared_ptr_new<dpp::cluster>(token);

  //TDOO: make own logger
  m_discordBot->on_log(dpp::utility::cout_logger());

  m_discordBot->on_guild_create([](const dpp::guild_create_t& guild) {
    BotManager::instance().registerBot(guild.created->id);
  });

  m_discordBot->on_ready([this](const dpp::ready_t& event) {
    notifyOwner();

    if (dpp::run_once<struct register_bot_commands>()) {
      //SPtr<BaseCommand> pingCommnad = ch_shared_ptr_new<PingCommand>();
      //m_loadedCommands[pingCommnad->getName()] = pingCommnad;
      m_loadedCommands["ping"] = []() { return ch_shared_ptr_new<PingCommand>(); };

      m_discordBot->global_command_create(
        dpp::slashcommand("ping", "ping", m_discordBot->me.id)
      );
    }

    //Execute Listener
    m_commandDispatcher = std::thread(&BotApp::commandDispatcherThread, this);
  });

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
  auto it = m_loadedCommands.find(commandName);
  if (it != m_loadedCommands.end()) {
    // Create a new instance of the command
    auto commandInstance = it->second();

    // Command found, lock the mutex and push it to the queue
    std::lock_guard<std::mutex> guard(m_queueMutex);
    m_commandQueue.emplace(std::make_pair(std::make_unique<dpp::slashcommand_t>(slashCommand), commandInstance));
  }
  else {
    // Command not found, handle error
  }

}

void BotApp::commandDispatcherThread() {
  while (m_isDispatcherRunning) {
    std::optional<std::pair<std::unique_ptr<dpp::slashcommand_t>, chEngineSDK::SPtr<BaseCommand>>> commandEventPairOpt;
    {
      std::lock_guard<std::mutex> guard(m_queueMutex);
      if (!m_commandQueue.empty()) {
        commandEventPairOpt = std::make_optional(std::move(m_commandQueue.front()));
        m_commandQueue.pop();
      }
    }

    // the mutex is no longer locked at this point

    if (commandEventPairOpt) {
      // when using the pair
      auto& commandEventPair = *commandEventPairOpt; 
      commandEventPair.second->getCallback()(*commandEventPair.first);
    }

  }
}

}