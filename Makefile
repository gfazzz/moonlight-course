# OPERATION MOONLIGHT - Main Makefile
# Root makefile for the entire course

.PHONY: help list status clean clean-all

# Default target
help:
	@echo "╔════════════════════════════════════════════════════╗"
	@echo "║     OPERATION MOONLIGHT - Interactive C Course    ║"
	@echo "╚════════════════════════════════════════════════════╝"
	@echo ""
	@echo "Available commands:"
	@echo ""
	@echo "  make help        - Show this help message"
	@echo "  make list        - List all seasons and episodes"
	@echo "  make status      - Show course progress"
	@echo "  make clean       - Clean all binaries and build artifacts"
	@echo "  make clean-all   - Same as clean"
	@echo ""
	@echo "To work on specific episode:"
	@echo "  cd season-X-name/episode-XX-name/"
	@echo "  make              # Build"
	@echo "  make run          # Run"
	@echo "  make test         # Test"
	@echo ""
	@echo "Quick start:"
	@echo "  cd season-1-foundations/episode-01-strange-message/"
	@echo "  cat README.md"
	@echo ""
	@echo "Good luck, agent! 🎯"

# List all seasons and episodes
list:
	@echo "=== MOONLIGHT Course Structure ==="
	@echo ""
	@echo "📁 Season 1: Foundations"
	@echo "   └─ Episode 01: Strange Message"
	@echo "   └─ Episode 02: Chain of Clues"
	@echo "   └─ Episode 03: Patterns in Time"
	@echo "   └─ Episode 04: First Tool"
	@echo ""
	@echo "📁 Season 2: Memory & Pointers"
	@echo "   └─ Episodes 05-08 (Coming soon)"
	@echo ""
	@echo "📁 Season 3: Networks"
	@echo "   └─ Episodes 09-12 (Coming soon)"
	@echo ""
	@echo "📁 Season 4: Crypto & Algorithms"
	@echo "   └─ Episodes 13-16 (Coming soon)"
	@echo ""
	@echo "📁 Season 5: Embedded & Arduino"
	@echo "   └─ Episodes 17-20 (Coming soon)"
	@echo ""
	@echo "📁 Season 6: Low-Voltage Systems"
	@echo "   └─ Episodes 21-24 (Coming soon)"
	@echo ""
	@echo "📁 Season 7: System Programming"
	@echo "   └─ Episodes 25-28 (Coming soon)"
	@echo ""
	@echo "📁 Season 8: AI & Data Science"
	@echo "   └─ Episodes 29-32 (Coming soon)"
	@echo ""
	@echo "📁 Final Operation"
	@echo "   └─ Final Project (Coming soon)"

# Show progress (simple version)
status:
	@echo "=== Course Progress ==="
	@echo ""
	@if [ -f .progress ]; then \
		cat .progress; \
	else \
		echo "No progress tracked yet."; \
		echo "Complete episodes and run 'make test' to track progress."; \
	fi
	@echo ""
	@echo "To update progress, complete episodes and run tests."

# Clean compiled binaries and intermediate files
clean:
	@echo "Cleaning compiled binaries and intermediate files..."
	@find . -type d -name "build" -exec rm -rf {} + 2>/dev/null || true
	@find . -type f -name "*.o" -delete
	@find . -type f -name "*.out" -delete
	@find . -type f -name "a.out" -delete
	@find . -type f -name "output.txt" -delete
	@echo "Removing episode executables..."
	@find season-*/episode-* -maxdepth 1 -type f -executable -delete 2>/dev/null || true
	@find season-*/episode-*/artifacts -maxdepth 1 -type f -executable -delete 2>/dev/null || true
	@echo "✓ All build artifacts and executables cleaned"

# Alias for clean
clean-all: clean

# Initialize course (create necessary directories)
init:
	@echo "Initializing MOONLIGHT course..."
	@mkdir -p season-1-foundations/episode-01-strange-message/artifacts
	@mkdir -p season-1-foundations/episode-02-chain-of-clues/artifacts
	@mkdir -p season-1-foundations/episode-03-patterns-in-time/artifacts
	@mkdir -p season-1-foundations/episode-04-first-tool/artifacts
	@touch .progress
	@echo "✓ Course initialized"
	@echo ""
	@echo "Start with: cd season-1-foundations/episode-01-strange-message/"
