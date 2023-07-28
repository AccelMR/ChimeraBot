#pragma once

#include "chPrerequisitesUtilities.h"
#include "chModule.h"
#include <dpp/dpp.h>

// Forward declaration of dpp::cluster
namespace dpp {
  class cluster;
  struct interaction_create_t;
  struct command_option;
}

namespace chBot {
using InteractionCallback = std::function<void(const dpp::interaction_create_t&)>;
class BaseCommand{
 public:
  BaseCommand() {}
  virtual ~BaseCommand() {}

  virtual std::vector<dpp::command_option>
  getOptions() const = 0;

  virtual dpp::slashcommand_contextmenu_type
  getType() const = 0;

  virtual chEngineSDK::String
  getName() const = 0;

  virtual chEngineSDK::String
  getDescription() const = 0;

  virtual chEngineSDK::Vector<chEngineSDK::String>
  getExclusiveGuilds() const = 0;

  virtual InteractionCallback
  getCallback() const = 0;
};

}


