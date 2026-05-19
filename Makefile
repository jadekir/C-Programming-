CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -pedantic -O2
BUILD_DIR := build
TARGETS := airline_reservation bit_tool word_count

.PHONY: all clean test

all: $(TARGETS:%=$(BUILD_DIR)/%)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

test: all
	./$(BUILD_DIR)/bit_tool 42
	./$(BUILD_DIR)/word_count README.md

clean:
	rm -rf $(BUILD_DIR)
