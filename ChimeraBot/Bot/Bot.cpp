#include "Bot.h"

namespace chBot {

Bot::Bot(const dpp::snowflake& snowflake): m_id(snowflake) {}
Bot::~Bot(){}

}