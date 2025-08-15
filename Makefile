# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Werror -Wno-unused-function
TARGET = xeltu

# Source directory
SRCDIR = src
OBJDIR = .obj

# Source files (with path)
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/debug.c $(SRCDIR)/function.c $(SRCDIR)/lexer.c $(SRCDIR)/memory.c $(SRCDIR)/parser.c $(SRCDIR)/value.c

# Object files (automatically generated from source files)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Header files (for dependency tracking)
HEADERS = $(SRCDIR)/common.h $(SRCDIR)/debug.h $(SRCDIR)/function.h $(SRCDIR)/lexer.h $(SRCDIR)/memory.h $(SRCDIR)/parser.h $(SRCDIR)/value.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build with additional flags
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build with optimization
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Install (optional - adjust path as needed)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the calculator (default)"
	@echo "  clean    - Remove object files and executable"
	@echo "  rebuild  - Clean and build"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug flags"
	@echo "  release  - Build optimized release version"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  help     - Show this help message"

# Declare phony targets (targets that don't create files)
.PHONY: all clean rebuild run debug release install uninstall help
