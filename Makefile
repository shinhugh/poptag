# Master makefile

# Project root directory path (containing this Makefile)
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_SRC := $(PATH_ROOT)/src
PATH_CORE := $(PATH_SRC)/core
PATH_GAME := $(PATH_SRC)/game

# Object files
OBJ := $(PATH_ROOT)/*.o $(PATH_SRC)/*.o $(PATH_CORE)/*.o $(PATH_GAME)/*.o
# Executable files
EXE := $(PATH_ROOT)/game

# Compiler
CC := g++
# Compiler flags
CFLAGS := -g -Wall -I $(PATH_SRC) -I $(PATH_CORE) -I $(PATH_GAME)

# --------------------------------------------------
# Default target

default: game

# --------------------------------------------------
# Main executable

game: \
$(PATH_CORE)/main.o \
$(PATH_CORE)/thread_core.h $(PATH_CORE)/thread_core.o \
$(PATH_CORE)/thread_input.h $(PATH_CORE)/thread_input.o \
$(PATH_CORE)/thread_display.h $(PATH_CORE)/thread_display.o \
$(PATH_CORE)/game.h $(PATH_CORE)/game.o \
$(PATH_CORE)/data_packet.h $(PATH_CORE)/data_packet.o \
$(PATH_GAME)/event_data.h \
$(PATH_GAME)/state_data.h \
$(PATH_GAME)/game_state.h $(PATH_GAME)/game_state.o \
$(PATH_GAME)/board.h $(PATH_GAME)/board.o \
$(PATH_GAME)/character.h $(PATH_GAME)/character.o \
$(PATH_GAME)/bomb.h $(PATH_GAME)/bomb.o
	@echo "Building main executable.."
	@$(CC) -o $@ $(PATH_CORE)/main.o \
$(PATH_CORE)/thread_core.o \
$(PATH_CORE)/thread_input.o \
$(PATH_CORE)/thread_display.o \
$(PATH_CORE)/game.o \
$(PATH_CORE)/data_packet.o \
$(PATH_GAME)/game_state.o \
$(PATH_GAME)/board.o \
$(PATH_GAME)/character.o \
$(PATH_GAME)/bomb.o \
-pthread
	@echo "Complete."

# --------------------------------------------------
# Targets aren't specified for individual object files
# Modifications in header files will not cause object files to be rebuilt
# Run 'make clean' and rebuild if a header file is modified

# --------------------------------------------------
# Build object file (.cpp)

%.o: %.cpp
	@echo "Building $@.."
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo "Complete."

# --------------------------------------------------
# Remove build

.PHONY: clean

clean:
	@echo "Removing build.."
	@rm -f $(OBJ) $(EXE)
	@echo "Complete."
