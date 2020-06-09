# Master makefile

# Project root directory path (containing this Makefile)
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_SRC := $(PATH_ROOT)/src

# Object files
OBJ := $(PATH_ROOT)/*.o $(PATH_SRC)/*.o
# Executable files
EXE := $(PATH_ROOT)/game

# Compiler
CC := g++
# Compiler flags
CFLAGS := -g -Wall

# --------------------------------------------------
# Default target

default: game

# --------------------------------------------------
# Main executable

game: $(PATH_SRC)/main.o $(PATH_SRC)/game.o $(PATH_SRC)/game_state.o \
$(PATH_SRC)/bomb.o
	@echo "Building main executable.."
	@$(CC) -o $@ $(PATH_SRC)/main.o $(PATH_SRC)/game.o $(PATH_SRC)/game_state.o \
$(PATH_SRC)/bomb.o -pthread
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
