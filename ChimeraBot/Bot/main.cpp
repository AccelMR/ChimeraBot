#include <iostream>

#include "chCommandArguments.h"
#include "BotApplication.h"
#include "BotManager.h"

int main(int argc, char* argv[]) {

  CmdParser::getInstance().parse(argc, argv);

  const chEngineSDK::String BOT_TOKEN = CmdParser::getInstance().getParam("token");


  chBot::BotApp::startUp();
  chBot::BotManager::startUp();
  chBot::BotApp::instance().init(BOT_TOKEN);

  chBot::BotApp::instance().run();

  return 0;
}
