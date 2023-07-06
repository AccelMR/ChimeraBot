#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"
#include <dpp/dpp.h>

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
  class slashcommand_t;
  struct command_option;
}

namespace chBot {
class BaseCommand{
 public:
  BaseCommand() {}
  virtual ~BaseCommand() {}

  virtual std::vector<dpp::command_option>
  getOptions() const = 0;

  virtual chEngineSDK::String
  getName() const = 0;

  virtual chEngineSDK::String
  getDescription() const = 0;

  virtual chEngineSDK::Vector<chEngineSDK::String>
  getExclusiveGuilds() const = 0;

  virtual std::function<void(const dpp::slashcommand_t&)> 
  getCallback() const = 0;
};

class PingCommand : public BaseCommand {
  std::function<void(const dpp::slashcommand_t&)> 
  getCallback() const override;

  // Inherited via BaseCommand
  virtual std::vector<dpp::command_option> 
  getOptions() const override;

  virtual chEngineSDK::String 
  getName() const override;

  virtual chEngineSDK::String 
  getDescription() const override;

  virtual chEngineSDK::Vector<chEngineSDK::String> 
  getExclusiveGuilds() const override;
};

}

