# CMakeList.txt : CMake project for Bot, include source and define
# project specific logic here.
#

# Include all .cpp and .h files in utilities
file(GLOB UTILITIES "Utilities/*.cpp" "Utilities/*.h")

# Add source to this project's executable.
add_executable (Bot "main.cpp" ${UTILITIES} "BotManager.h" "BotManager.cpp" "BotApplication.h" "BotApplication.cpp" "Bot.cpp" "Bot.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Bot PROPERTY CXX_STANDARD 20)
endif()

find_package(dpp CONFIG REQUIRED)

target_link_libraries(Bot PRIVATE dpp::dpp)

if(WIN32)
  message(STATUS "dpp_DIR = ${dpp_DIR}")

  # Assuming the relative path from dpp_DIR to the include directory
  set(DPP_INCLUDE_DIR "${dpp_DIR}/../../include")

  message(STATUS "DPP_INCLUDE_DIR = ${DPP_INCLUDE_DIR}")
  target_include_directories(Bot PRIVATE ${DPP_INCLUDE_DIR} Utilities)
endif()