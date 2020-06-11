# CMake version
cmake_minimum_required(VERSION 3.17.2)

# Project name
project(Poptag)

# Generate executable in build root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Build executable game
add_executable(game src/core/main.cpp src/core/thread_core.cpp
src/core/thread_input.cpp src/core/thread_display.cpp src/core/game.cpp
src/core/data_packet.cpp src/game/game_state.cpp src/game/board.cpp
src/game/character.cpp src/game/bomb.cpp)

# Include path for executable game
target_include_directories(game PUBLIC src/core)
target_include_directories(game PUBLIC src/game)

# Threads library
find_package(Threads REQUIRED)
target_link_libraries(game Threads::Threads)

# Microsoft Visual C++
if(MSVC)
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
  target_compile_options(game PUBLIC /MT)
  target_link_options(game PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
endif(MSVC)
