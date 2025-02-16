# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Library flags (example: -lm for math library)
LIBS = -lm

# Source files
SRCS = src/main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = crypto

# Library name
LIB_NAME = libcrypto.a

# Library source files
LIB_SRCS = $(wildcard lib/crypto/*.c)

# Library object files
LIB_OBJS = $(LIB_SRCS:.c=.o)

# Default target
all: $(LIB_NAME) $(EXEC)

# Build the library
$(LIB_NAME): $(LIB_OBJS)
	ar rcs $@ $^

# Link object files to create executable
$(EXEC): $(OBJS) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC) $(LIB_OBJS) $(LIB_NAME)

.PHONY: all clean