#include <iostream>

#include "ErrorCodes.h"
#include "chCommandArguments.h"
#include "BotApplication.h"
#include "BotManager.h"
#include "chDebug.h"

using namespace chEngineSDK;
int main(int argc, char* argv[]) {

  CmdParser::getInstance().parse(argc, argv);

  const chEngineSDK::String BOT_TOKEN = CmdParser::getInstance().getParam("token");

  if (BOT_TOKEN.empty()) {
    LOG_ERROR("No Bot Token was provided.");
    return NO_BOT_TOKEN;
  }

  chBot::BotApp::startUp();
  chBot::BotManager::startUp();
  chBot::BotApp::instance().init(BOT_TOKEN);

  chBot::BotApp::instance().run();

  return 0;
}
