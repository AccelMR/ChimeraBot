#pragma once

#include <dpp/dpp.h>

namespace chBot {
class Bot {
 public:
   Bot(const dpp::snowflake& snowflake);
   ~Bot();

 private:
   dpp::snowflake m_id;
};
}