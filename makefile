CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS =

# Define the source and object directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List of source files
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Generate list of object files
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Targets
all: folders $(BIN_DIR)/orchestrator $(BIN_DIR)/client

folders:
	@mkdir -p $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/orchestrator: $(OBJ_DIR)/orchestrator.o $(OBJ_DIR)/defs.o
	$(CC) $(LDFLAGS) $^ -o $@

$(BIN_DIR)/client: $(OBJ_DIR)/client.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

