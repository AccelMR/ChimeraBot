#include "PingCommand.h"

namespace chBot {
const chEngineSDK::String PingCommand::NAME = "PING";
const chEngineSDK::String PingCommand::DESCRIPTION = "Check connection to the Bot.";

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
  return NAME;
}

chEngineSDK::String 
PingCommand::getDescription() const {
  return DESCRIPTION;
}

chEngineSDK::Vector<chEngineSDK::String> 
PingCommand::getExclusiveGuilds() const {
  return chEngineSDK::Vector<chEngineSDK::String>();
}

}