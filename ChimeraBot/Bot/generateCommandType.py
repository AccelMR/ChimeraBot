import os
import re

command_types = []
command_creators = []
command_name_to_type = []

# Use regex to parse the command name and type
command_name_regex = re.compile(r'const\s+chEngineSDK::String\s+(\w+)::NAME\s+=\s+"(\w+)";')

def process_file(filename):
    # Extract command name from filename
    command_name = os.path.basename(filename)[:-4]  # Example: PingCommand
    command_type = command_name[:-7].upper()  # Example: PING

    # Add to the command types
    command_types.append(command_type)

    # Add to the command creators
    command_creators.append(f'  {{CommandType::{command_type}, [](){{ return std::make_shared<chBot::{command_name}>(); }} }},')

    with open(filename, 'r') as file:
        for line in file:
            # Check if line matches the regex
            match = command_name_regex.match(line.strip())
            if match:
                # If it does, extract the command type and name
                command_type = match.group(1)[:-7].upper()  # Remove "Command" and convert to upper case
                command_name = match.group(2).lower()

                # Add the mapping to the command name to type list
                command_name_to_type.append(f'  {{"{command_name}", CommandType::{command_type}}},')

# Traverse the commands directory
for root, _, files in os.walk('src/Commands'):
    for file in files:
        if file.endswith('.cpp'):
            process_file(os.path.join(root, file))

# Output to CommandTypes.h
with open('src/CommandTypes.h', 'w') as file:
    file.write('// This file is autogenerated\n\n')

    file.write('#pragma once\n\n')
    file.write('#include "chPrerequisitesUtilities.h"\n')
    
    # Include headers for each command type
    for command_type in command_types:
        file.write(f'#include "{command_type.capitalize()}Command.h"\n')

    file.write('\n')
    file.write('typedef std::function<std::shared_ptr<chBot::BaseCommand>()> CommandCreator;\n')
    file.write('enum class CommandType {\n')
    for command_type in command_types:
        file.write(f'  {command_type},\n')
    file.write('};\n\n')

    file.write('std::unordered_map<CommandType, CommandCreator> LOADED_COMMANDS = {\n')
    for command_creator in command_creators:
        file.write(command_creator + '\n')
    file.write('};\n\n')

    file.write('std::unordered_map<std::string, CommandType> COMMAND_NAME_TO_TYPE = {\n')
    for command_name_to_type_entry in command_name_to_type:
        file.write(command_name_to_type_entry + '\n')
    file.write('};\n')