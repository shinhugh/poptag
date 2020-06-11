# CMake version
cmake_minimum_required(VERSION 3.17)

# Project name
project(Poptag)

# Generate executable in build root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Target for executable game
add_executable(game glad/glad.c src/core/main.cpp src/core/thread_core.cpp
src/core/thread_input.cpp src/core/thread_display.cpp src/core/game.cpp
src/core/data_packet.cpp src/game/game_state.cpp src/game/board.cpp
src/game/character.cpp src/game/bomb.cpp)

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

# Microsoft Visual C++
# if(MSVC)
#   SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
#   target_compile_options(game PUBLIC /MT)
#   target_link_options(game PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
# endif(MSVC)
