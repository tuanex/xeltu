CC := gcc

CFLAGS := -g -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-parameter

SRCDIR := src
OBJDIR := obj

TARGET := xeltu

SOURCES := $(wildcard $(SRCDIR)/*.c)

OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

.PHONY: all
all: $(OBJDIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET)
