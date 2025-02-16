# Define the compiler
CC = gcc

# Define the directories
SRC_DIR = src
LIB_DIR = $(SRC_DIR)/library
MAIN_SRC = $(SRC_DIR)/main.c

# Define the output executable
OUTPUT = main

CFLAGS=-arch arm64

# Find all source files in the library directory
LIB_SRCS = $(wildcard $(LIB_DIR)/*.c)

# Create object files for each source file
LIB_OBJS = $(LIB_SRCS:.c=.o)

# Default target
all: $(OUTPUT)

%.o: %.c %.h options.h
	$(CC) $(CFLAGS) -o $@ -c $<

# Build the main executable
$(OUTPUT): $(LIB_OBJS) $(MAIN_SRC)
	$(CC) -o $@ $^

# Compile library source files into object files
$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) -c -o $@ $<

# Clean up build files
clean:
	rm -f $(LIB_DIR)/*.o $(OUTPUT)

.PHONY: all clean