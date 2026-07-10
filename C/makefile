CC       := gcc
CFLAGS   := -std=c17 -Wall -Wextra -g -Isrc/headers
DEPFLAGS := -MMD -MP

SRC_DIR  := src
HDR_DIR  := $(SRC_DIR)/headers
BIN_DIR  := bin
OBJ_DIR  := $(BIN_DIR)/obj

PROGRAMS := $(basename $(notdir $(wildcard $(SRC_DIR)/*.c)))

# Disable make's built-in implicit rules/variables -- they cause exactly
# the kind of silent fallback bug seen above.
MAKEFLAGS += -r -R

.PHONY: all clean list
all: $(addprefix $(BIN_DIR)/,$(PROGRAMS))

define PROGRAM_template
HDRS_$(1) := $$(shell $(CC) $(CFLAGS) -MM $(SRC_DIR)/$(1).c 2>/dev/null \
               | tr -d '\\' | tr ' ' '\n' \
               | grep -E '^$(HDR_DIR)/.*\.h$$$$')
OBJS_$(1) := $(OBJ_DIR)/$(1).o $$(patsubst $(HDR_DIR)/%.h,$(OBJ_DIR)/headers/%.o,$$(HDRS_$(1)))

$(BIN_DIR)/$(1): $$(OBJS_$(1)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $$^ -o $$@

# make hash_table
$(1): $(BIN_DIR)/$(1)
	@:

# make hash_table.c
$(1).c: $(BIN_DIR)/$(1)
	@:
endef

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/headers/%.o: $(HDR_DIR)/%.c | $(OBJ_DIR)/headers
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR) $(OBJ_DIR)/headers $(BIN_DIR):
	mkdir -p $@

-include $(wildcard $(OBJ_DIR)/*.d $(OBJ_DIR)/headers/*.d)

clean:
	rm -rf $(BIN_DIR)

list:
	@echo "Buildable programs:"
	@echo "$(PROGRAMS)" | tr ' ' '\n'