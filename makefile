# Compiler and flags
CC = gcc
CFLAGS = -lm

# Source files
SRCS = main.c prompt.c hop.c log.c proclore.c seek.c reveal.c bgfg.c input.c alias.c activities.c ping.c redirection.c pipe.c neonate.c iman.c bg.c ifelse.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = ./a.out

# Default rule to build the executable
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to prevent conflicts with files of the same name
.PHONY: all clean
