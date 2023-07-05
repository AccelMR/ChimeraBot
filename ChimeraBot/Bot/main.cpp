#include <iostream>

#include "BotApplication.h"
#include "BotManager.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Please provide the bot token as an argument." << std::endl;
    return 1;
  }

  std::string BOT_TOKEN = argv[1];

  chBot::BotApp::startUp();
  chBot::BotManager::startUp();
  chBot::BotApp::instance().init(BOT_TOKEN);

  return 0;
}
