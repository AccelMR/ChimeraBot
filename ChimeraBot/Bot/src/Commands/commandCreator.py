import os

header_template = '''
#pragma once

#include "chPrerequisitesUtilities.h"
#include "Command.h"

namespace chBot {{

class {0}Command: public BaseCommand {{
 public:
  {0}(const dpp::interaction_create_t& event) 
    : m_event(event) {} //You should change this parameter if the data is different than dpp::interaction_create_t

  // Inherited via BaseCommand
  virtual dpp::slashcommand_contextmenu_type
  getType() const override;

  virtual 
  std::vector<dpp::command_option> getOptions() const override;

  virtual chEngineSDK::String 
  getName() const override;

  virtual chEngineSDK::String 
  getDescription() const override;

  virtual chEngineSDK::Vector<chEngineSDK::String> 
  getExclusiveGuilds() const override;

  virtual InteractionCallback 
  getCallback() const override;

 public:
  static const chEngineSDK::String NAME;
  static const chEngineSDK::String DESCRIPTION;
  static const chEngineSDK::Vector<chEngineSDK::String> EXCLUSIVE_GUILDS;
 private:
  dpp::interaction_create_t m_event;
}};

}}
'''

cpp_template = '''
#include "{0}Command.h"

namespace chBot {{

const chEngineSDK::String {0}Command::NAME = "{1}";
const chEngineSDK::String {0}Command::DESCRIPTION = "Description.";
const chEngineSDK::Vector<chEngineSDK::String> {0}Command::EXCLUSIVE_GUILDS = {{}};

/*
*/
dpp::slashcommand_contextmenu_type
PingCommand::getType() const {
  return dpp::slashcommand_contextmenu_type::ctxm_chat_input;
}

/*
*/
std::vector<dpp::command_option> 
{0}Command::getOptions() const {{
  return std::vector<dpp::command_option>();
}}

/*
*/
chEngineSDK::String 
{0}Command::getName() const {{
  return NAME;
}}

/*
*/
chEngineSDK::String {0}Command::getDescription() const {{
  return DESCRIPTION;
}}

/*
*/
chEngineSDK::Vector<chEngineSDK::String> 
{0}Command::getExclusiveGuilds() const {{
  return EXCLUSIVE_GUILDS;
}}

/*
*/
InteractionCallback
{0}Command::getCallback() const {{
  return std::function<void(const dpp::interaction_create_t&)>();
}}
}}
'''

def main():
    name = input('Enter command name: ')
    lower_name = name.lower()

    if not name.endswith('Command'):
        name = name + 'Command'

    with open(name + '.h', 'w') as f:
        f.write(header_template.format(name))

    with open(name + '.cpp', 'w') as f:
        f.write(cpp_template.format(name, lower_name))

if __name__ == '__main__':
    main()
