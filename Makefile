CC = gcc
CFLAGS = -Wall -Wextra -Iutilities
OUTDIR = bin

SRC = main.c shell.c effects.c model.c
OBJ = $(patsubst %.c, $(OUTDIR)/%.o, $(SRC))
EXEC = turtle

# Default target
all: utils_subdir $(EXEC)

# Build executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Compile .c to .o inside bin/
$(OUTDIR)/%.o: %.c | $(OUTDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure bin/ directory exists
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Call make in utils directory
utils_subdir:
	$(MAKE) -C utilities

# Clean target
clean:
	rm -rf $(OUTDIR)
	$(MAKE) -C utils clean

.PHONY: all clean utils_subdir
