# Master makefile

# Project root directory path (containing this Makefile)
PATH_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# Object files
OBJ := $(PATH_ROOT)/*.o
# Executable files
EXE := $(PATH_ROOT)/test

# Compiler
CC := g++
# Compiler flags
CFLAGS := -g -Wall -I $(PATH_ROOT)/include
# Linker flags
LFLAGS := -L $(PATH_ROOT)/lib

# --------------------------------------------------
# Default target

default: test

# --------------------------------------------------
# Test program

test: $(PATH_ROOT)/test.o
	@echo "Building test program.."
	@$(CC) $(LFLAGS) -o $@ $(PATH_ROOT)/test.o -pthread -lglfw3 -lGL -ldl -lX11
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
