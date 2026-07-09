CC       := gcc
CFLAGS   := -std=c17 -Wall -Wextra -g
SRC_DIR  := src
BIN_DIR  := bin

.PHONY: all clean list
.SECONDARY:          # <-- stop make from deleting intermediate bin/% files

# Usage: make <name>   (name = filename without .c, e.g. `make array`)
%: $(BIN_DIR)/%
	@:

$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)/*

list:
	@echo "Buildable programs in $(SRC_DIR)/:"
	@ls $(SRC_DIR)/*.c 2>/dev/null | xargs -n1 basename | sed 's/\.c$$//'
	@echo "Buildable programs in current dir:"
	@ls *.c 2>/dev/null | xargs -n1 basename | sed 's/\.c$$//'