# Makefile for Episode 04: First Tool

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
ARTIFACTS = artifacts
BUILD = build

# Object files
OBJS = $(BUILD)/crypto.o $(BUILD)/utils.o $(BUILD)/decoder.o

# Target executable
TARGET = $(BUILD)/decoder

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

# Compile crypto.o
$(BUILD)/crypto.o: $(ARTIFACTS)/crypto.c $(ARTIFACTS)/crypto.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(ARTIFACTS)/crypto.c -o $@

# Compile utils.o
$(BUILD)/utils.o: $(ARTIFACTS)/utils.c $(ARTIFACTS)/utils.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(ARTIFACTS)/utils.c -o $@

# Compile decoder.o
$(BUILD)/decoder.o: $(ARTIFACTS)/decoder.c $(ARTIFACTS)/crypto.h $(ARTIFACTS)/utils.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(ARTIFACTS)/decoder.c -o $@

# Link all objects
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	@echo "✓ decoder built successfully"

# Run decoder
run: $(TARGET)
	@echo "=== Running decoder ==="
	@$(TARGET) -h

# Run tests
test: $(TARGET)
	@echo "=== Running automated tests ==="
	@cd tests && ./test.sh

# Clean build files
clean:
	rm -rf $(BUILD)
	@echo "✓ Cleaned build directory"

# Help
help:
	@echo "Episode 04 - Makefile"
	@echo "===================="
	@echo ""
	@echo "Targets:"
	@echo "  make         - Compile all modules and link"
	@echo "  make run     - Compile and show help"
	@echo "  make test    - Run automated tests"
	@echo "  make clean   - Remove build files"
	@echo ""
	@echo "Files needed in artifacts/:"
	@echo "  - crypto.h, crypto.c"
	@echo "  - utils.h, utils.c"
	@echo "  - decoder.c"
	@echo ""
	@echo "Usage after build:"
	@echo "  ./build/decoder -i <input> -o <output> -k <key>"

.PHONY: all run test clean help
