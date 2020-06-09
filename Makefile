# Master makefile

# Project root directory path (containing this Makefile)
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_SRC := $(PATH_ROOT)/src
PATH_CORE := $(PATH_SRC)/core
PATH_GAME := $(PATH_SRC)/game

# Object files
OBJ := $(PATH_ROOT)/*.o $(PATH_SRC)/*.o
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

game: $(PATH_CORE)/main.o $(PATH_CORE)/game.o $(PATH_CORE)/game_state.o \
$(PATH_GAME)/board.o $(PATH_GAME)/character.o $(PATH_GAME)/bomb.o
	@echo "Building main executable.."
	@$(CC) -o $@ $(PATH_CORE)/main.o $(PATH_CORE)/game.o \
$(PATH_CORE)/game_state.o $(PATH_GAME)/board.o $(PATH_GAME)/character.o \
$(PATH_GAME)/bomb.o -pthread
	@echo "Complete."

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
