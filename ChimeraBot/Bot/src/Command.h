#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"
#include <dpp/dpp.h>

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
  struct slashcommand_t;
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

}


