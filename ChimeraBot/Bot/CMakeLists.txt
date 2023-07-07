# CMakeList.txt : CMake project for Bot, include source and define
# project specific logic here.

# Recursively include all .cpp and .h files in src and its subdirectories
file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h")

# Add source to this project's executable.
add_executable (Bot "main.cpp" ${SRC})

target_compile_definitions(Bot PRIVATE CH_UTILITY_EXPORTS)

find_package(dpp CONFIG REQUIRED)

target_link_libraries(Bot PRIVATE dpp::dpp)

if(WIN32)
  message(STATUS "dpp_DIR = ${dpp_DIR}")

  # Assuming the relative path from dpp_DIR to the include directory
  set(DPP_INCLUDE_DIR "${dpp_DIR}/../../include")

  message(STATUS "DPP_INCLUDE_DIR = ${DPP_INCLUDE_DIR}")
  # Add src and its subdirectories to the include directories
endif()

 target_include_directories(Bot PRIVATE ${DPP_INCLUDE_DIR} src src/Utilities src/Commands)
