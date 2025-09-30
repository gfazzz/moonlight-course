# Episode 09: Network Basics - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = net_analyzer
SOLUTION_DIR = solution
ARTIFACTS_DIR = artifacts

# Source files
STARTER = starter.c
SOLUTION = $(SOLUTION_DIR)/net_analyzer_solution.c

# Default target: build starter
all: $(TARGET)

$(TARGET): $(STARTER)
	$(CC) $(CFLAGS) -o $(TARGET) $(STARTER)
	@echo "✅ Network analyzer built successfully!"

# Build solution version
solution: $(SOLUTION)
	$(CC) $(CFLAGS) -o $(ARTIFACTS_DIR)/net_analyzer_solution $(SOLUTION)
	@echo "✅ Solution built!"

# Generate test data
generate: addresses.txt
	@echo "✅ Test data already exists"

addresses.txt:
	@echo "# MOONLIGHT Network Map" > addresses.txt
	@echo "192.168.1.100:8080" >> addresses.txt
	@echo "10.0.0.5:22" >> addresses.txt
	@echo "172.16.0.10:443" >> addresses.txt
	@echo "8.8.8.8:53" >> addresses.txt
	@echo "127.0.0.1:80" >> addresses.txt
	@echo "203.0.113.42:31337" >> addresses.txt
	@echo "1.1.1.1:53" >> addresses.txt
	@echo "192.168.100.1:3306" >> addresses.txt
	@echo "✅ Generated addresses.txt

"

# Run tests
test: $(TARGET) generate
	@echo "🧪 Running tests..."
	@bash tests/test.sh

# Test solution
test-solution: solution generate
	@echo "🧪 Testing solution..."
	@./$(ARTIFACTS_DIR)/net_analyzer_solution addresses.txt

# Clean build artifacts
clean:
	rm -f $(TARGET)
	rm -f $(ARTIFACTS_DIR)/net_analyzer_solution
	rm -f addresses.txt
	@echo "🧹 Cleaned!"

# Help
help:
	@echo "Available targets:"
	@echo "  make           - Build net_analyzer from starter.c"
	@echo "  make solution  - Build solution version"
	@echo "  make generate  - Generate test data file"
	@echo "  make test      - Run automated tests"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make help      - Show this help"

.PHONY: all solution generate test test-solution clean help
