# Master makefile

# Project root directory path (containing this Makefile)
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
PATH_SRC := $(PATH_ROOT)/src

# Object files
OBJ := $(PATH_ROOT)/*.o $(PATH_SRC)/*.o
# Executable files
EXE := $(PATH_ROOT)/game $(PATH_ROOT)/test

# Compiler
CC := g++
# Compiler flags
CFLAGS := -g -Wall -I $(PATH_ROOT)/include
# Linker flags
LFLAGS := -L $(PATH_ROOT)/lib

# --------------------------------------------------
# Default target

default: game

# --------------------------------------------------
# Main executable

game: $(PATH_SRC)/main.o $(PATH_SRC)/game_core.o $(PATH_SRC)/game_state.o \
$(PATH_SRC)/graphics.o
	@echo "Building main executable.."
	@$(CC) $(LFLAGS) -o $@ $(PATH_SRC)/main.o $(PATH_SRC)/game_core.o \
$(PATH_SRC)/game_state.o $(PATH_SRC)/graphics.o -pthread
	@echo "Complete."

# --------------------------------------------------
# Test executable

test: $(PATH_SRC)/test.o
	@echo "Building test executable.."
	@$(CC) $(LFLAGS) -o $@ $(PATH_SRC)/test.o -pthread -lglfw3 -lGL -ldl -lX11
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
