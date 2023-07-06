#include "Command.h"

namespace chBot {
std::function<void(const dpp::slashcommand_t&)> 
PingCommand::getCallback() const {
  return [](const dpp::slashcommand_t& event) {
    if (event.command.get_command_name() == "ping") {
      event.reply("Pong!");
    }
  };
}

std::vector<dpp::command_option>
PingCommand::getOptions() const {
  return std::vector<dpp::command_option>();
}

chEngineSDK::String 
PingCommand::getName() const {
  return "Ping";
}

chEngineSDK::String 
PingCommand::getDescription() const {
  return "Check connection to the Bot.";
}

chEngineSDK::Vector<chEngineSDK::String> 
PingCommand::getExclusiveGuilds() const {
  return chEngineSDK::Vector<chEngineSDK::String>();
}

}