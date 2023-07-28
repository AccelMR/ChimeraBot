#pragma once

#include "chPrerequisitesUtilities.h"
#include "Command.h"

namespace chBot {

class PingCommand : public BaseCommand {
  InteractionCallback
  getCallback() const override;

  virtual dpp::slashcommand_contextmenu_type
  getType() const override;

  // Inherited via BaseCommand
  virtual std::vector<dpp::command_option>
  getOptions() const override;

  virtual chEngineSDK::String
  getName() const override;

  virtual chEngineSDK::String
  getDescription() const override;

  virtual chEngineSDK::Vector<chEngineSDK::String>
  getExclusiveGuilds() const override;

 public: 
  static const chEngineSDK::String NAME;
  static const chEngineSDK::String DESCRIPTION;
};
}