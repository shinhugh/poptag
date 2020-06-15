# CMake version
cmake_minimum_required(VERSION 3.17)

# Project name
project(Poptag)

# Generate executable in build root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Target for executable game
add_executable(game lib/glad.c src/core/main.cpp src/core/thread_core.cpp
src/core/thread_display.cpp src/core/game.cpp src/core/data_packet.cpp
src/game/game_state.cpp src/game/board.cpp src/game/character.cpp
src/game/bomb.cpp)

# Paths to search in for header files for executable game
target_include_directories(game PUBLIC src/core)
target_include_directories(game PUBLIC src/game)
target_include_directories(game PUBLIC include)

# Threads library for executable game
find_package(Threads REQUIRED)
target_link_libraries(game Threads::Threads)

# GLFW library for executable game
find_library(LIB_GLFW3 NAMES glfw3 HINTS lib)
target_link_libraries(game ${LIB_GLFW3})

# Visual Studio
if(MSVC)
  set_property(TARGET game PROPERTY MSVC_RUNTIME_LIBRARY
  "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif(MSVC)

# Linux
if(UNIX)
  target_link_libraries(game dl)
  target_link_libraries(game X11)
endif(UNIX)
