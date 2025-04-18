CC = gcc
CFLAGS = -Wall -Wextra -Iutilities
OUTDIR = outputs

SRC = main.c shell.c $(wildcard utilities/*.c)
OBJ = $(patsubst %.c, $(OUTDIR)/%.o, $(SRC))
EXEC = turtle

# Default target
all: $(EXEC)

# Build executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Compile .c to .o inside outputs/
$(OUTDIR)/%.o: %.c | $(OUTDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure outputs/ directory exists
$(OUTDIR):
	mkdir -p $(OUTDIR)

# Clean target
clean:
	rm -rf $(OUTDIR)

.PHONY: all clean
