CC = clang
CFLAGS = -g -fsanitize=address
LIBS = -lraylib -lunistring -lm

# Target executable
TARGET = build/main

# Source files
MAIN_SRC = main.c
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
ALL_SRC = $(MAIN_SRC) $(SRC_FILES)

# Object files
OBJ_DIR = build
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(ALL_SRC)))

# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link the executable
$(TARGET): $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBS) -o $(TARGET)

# Compile main.c
$(OBJ_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o $(OBJ_DIR)/main.o

# Compile src/*.c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)

# Phony targets
.PHONY: all clean
