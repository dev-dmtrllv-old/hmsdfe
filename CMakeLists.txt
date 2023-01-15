cmake_minimum_required (VERSION 3.8)

project(game)

file(GLOB RECURSIVE_SOURCES CONFIGURE_DEPENDS "src/test-game/*.cpp" "src/tasky/*.cpp")
add_executable (game ${RECURSIVE_SOURCES})
target_precompile_headers(game PUBLIC include/test-game/pch.hpp)
set_property(TARGET game PROPERTY CXX_STANDARD 23)
include_directories(game include)