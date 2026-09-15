CC       := gcc

SRC_DIR       := src
HDR_DIR       := $(SRC_DIR)/headers
BIN_DIR       := bin
OBJ_DIR       := $(BIN_DIR)/obj
LIB_DIR       := ../lib

CFLAGS   := -Wall -Wextra -g -I$(HDR_DIR) -I$(LIB_DIR)
DEPFLAGS := -MMD -MP

PROGRAMS := $(basename $(notdir $(wildcard $(SRC_DIR)/*.c)))

# Top-level dispatch: `make <project>` / `make clean-<project>` from C/ itself.
PROJECTS := $(sort $(foreach f,$(wildcard */src/*.c),$(firstword $(subst /, ,$(f)))))
MAKEFILE_PATH := $(abspath $(firstword $(MAKEFILE_LIST)))

define PROJECT_template
.PHONY: $(1) clean-$(1)
$(1):
	$$(MAKE) -C $(1) -f $$(MAKEFILE_PATH) $(1)

clean-$(1):
	$$(MAKE) -C $(1) -f $$(MAKEFILE_PATH) clean
endef

$(foreach proj,$(PROJECTS),$(eval $(call PROJECT_template,$(proj))))

# `make clean <project> [<project> ...]` at top level: extra words are
# targets to clean, not build targets.
ifeq (clean,$(firstword $(MAKECMDGOALS)))
CLEAN_TARGETS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
ifneq ($(CLEAN_TARGETS),)
$(eval $(CLEAN_TARGETS):;@:)
endif
endif

# `make project <name>`: scaffold a new project directory.
ifeq (project,$(firstword $(MAKECMDGOALS)))
PROJECT_NAME := $(word 2,$(MAKECMDGOALS))
ifneq ($(PROJECT_NAME),)
$(eval $(PROJECT_NAME):;@:)
endif
endif

.PHONY: project
project:
ifeq ($(PROJECT_NAME),)
	$(error Usage: make project <name>)
endif
	mkdir -p $(PROJECT_NAME)/src
	printf '/*\n    author: Henry Centeno\n    description:\n*/\n' > $(PROJECT_NAME)/src/$(PROJECT_NAME).c
	code $(PROJECT_NAME)/src/$(PROJECT_NAME).c

MAKEFLAGS += -r -R

# `make lib <name>`: compile a single lib/*/<name>.c file as a syntax check.
# `make newlib <name>` scaffolds a brand new lib/<name>/ directory instead.
# `make listlib` enumerates every buildable library. All three work from
# the top level (C/) or from inside a project dir. Extra word after
# "lib"/"newlib" is the file name, not a build target (same MAKECMDGOALS
# trick as `clean`/`project` above).
ifeq (lib,$(firstword $(MAKECMDGOALS)))
LIB_ARG := $(word 2,$(MAKECMDGOALS))
ifneq ($(LIB_ARG),)
$(eval $(LIB_ARG):;@:)
endif
endif

ifeq (newlib,$(firstword $(MAKECMDGOALS)))
NEWLIB_ARG := $(word 2,$(MAKECMDGOALS))
ifneq ($(NEWLIB_ARG),)
$(eval $(NEWLIB_ARG):;@:)
endif
endif

.PHONY: lib newlib
ifneq ($(PROJECTS),)
lib:
ifeq ($(LIB_ARG),)
	$(error Usage: make lib <name>  e.g. make lib hash_map)
endif
	@$(MAKE) -C $(firstword $(PROJECTS)) -f $(MAKEFILE_PATH) lib $(LIB_ARG)

newlib:
ifeq ($(NEWLIB_ARG),)
	$(error Usage: make newlib <name>  e.g. make newlib hash_map)
endif
	@$(MAKE) -C $(firstword $(PROJECTS)) -f $(MAKEFILE_PATH) newlib $(NEWLIB_ARG)
else
LIB_REL := $(basename $(LIB_ARG))
ifeq ($(findstring /,$(LIB_REL)),)
LIB_REL := $(patsubst $(LIB_DIR)/%.c,%,$(firstword $(wildcard $(LIB_DIR)/*/$(LIB_REL).c)))
endif

lib: $(if $(LIB_REL),$(OBJ_DIR)/lib/$(LIB_REL).o)
ifeq ($(LIB_ARG),)
	$(error Usage: make lib <name>  e.g. make lib hash_map)
endif
ifeq ($(LIB_REL),)
	$(error No source file found matching '$(LIB_ARG).c' under $(LIB_DIR)/*/ -- use make newlib $(LIB_ARG) to create one)
endif
	@echo "compiled $(LIB_DIR)/$(LIB_REL).c -> $(OBJ_DIR)/lib/$(LIB_REL).o"

newlib:
ifeq ($(NEWLIB_ARG),)
	$(error Usage: make newlib <name>  e.g. make newlib hash_map)
endif
	mkdir -p $(LIB_DIR)/$(NEWLIB_ARG)
	printf '/*\n    author: Henry Centeno\n    description:\n*/\n' > $(LIB_DIR)/$(NEWLIB_ARG)/$(NEWLIB_ARG).c
	printf '/*\n    author: Henry Centeno\n    description:\n*/\n' > $(LIB_DIR)/$(NEWLIB_ARG)/$(NEWLIB_ARG).h
	code $(LIB_DIR)/$(NEWLIB_ARG)/$(NEWLIB_ARG).h
	@echo "created $(LIB_DIR)/$(NEWLIB_ARG)/$(NEWLIB_ARG).c and $(LIB_DIR)/$(NEWLIB_ARG)/$(NEWLIB_ARG).h"
endif

# `make listlib`: enumerate every lib/*/*.c file that `make lib <name>`
# could build, without compiling anything. Separate single-word target so
# it never collides with `list` below or with MAKECMDGOALS parsing for `lib`.
.PHONY: listlib
ifneq ($(PROJECTS),)
listlib:
	@$(MAKE) -C $(firstword $(PROJECTS)) -f $(MAKEFILE_PATH) listlib
else
LIB_ALL := $(sort $(patsubst $(LIB_DIR)/%.c,%,$(wildcard $(LIB_DIR)/*/*.c)))
listlib:
	@echo "Libraries available under $(LIB_DIR)/:"
	@for l in $(LIB_ALL); do echo "  $$l"; done
endif

.PHONY: all clean list
all: $(addprefix $(BIN_DIR)/,$(PROGRAMS))

define PROGRAM_template
HDRS_$(1) := $$(shell $(CC) $(CFLAGS) -MM $(SRC_DIR)/$(1).c 2>/dev/null \
               | tr -d '\\' | tr ' ' '\n' \
               | grep -E '^$(HDR_DIR)/.*\.h$$$$')

# Scan the program's own source + its local headers for subfolder-qualified
# includes like #include "hash_map/hash_map.h", and only link the lib
# directories actually referenced -- unused libs are neither compiled
# nor linked for this program.
USED_LIBS_$(1) := $$(sort $$(filter $$(notdir $$(wildcard $(LIB_DIR)/*)), \
    $$(shell grep -ohE '#include *"[A-Za-z0-9_]+/' $(SRC_DIR)/$(1).c $$(HDRS_$(1)) 2>/dev/null \
        | sed -E 's/#include *"//; s#/##')))

LIB_OBJS_$(1) := $$(foreach l,$$(USED_LIBS_$(1)),$$(patsubst $(LIB_DIR)/%.c,$(OBJ_DIR)/lib/%.o,$$(wildcard $(LIB_DIR)/$$(l)/*.c)))

OBJS_$(1) := $(OBJ_DIR)/$(1).o $$(patsubst $(HDR_DIR)/%.h,$(OBJ_DIR)/headers/%.o,$$(HDRS_$(1))) $$(LIB_OBJS_$(1))

$(BIN_DIR)/$(1): $$(OBJS_$(1)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $$^ -o $$@

$(1): $(BIN_DIR)/$(1)
	@:
endef

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/headers/%.o: $(HDR_DIR)/%.c | $(OBJ_DIR)/headers
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/lib/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR) $(OBJ_DIR)/headers $(BIN_DIR):
	mkdir -p $@

-include $(wildcard $(OBJ_DIR)/*.d $(OBJ_DIR)/headers/*.d $(OBJ_DIR)/lib/*/*.d)

clean:
ifneq ($(CLEAN_TARGETS),)
	@for p in $(CLEAN_TARGETS); do rm -rf $$p/bin; echo "cleaned $$p/bin"; done
else
	rm -rf $(BIN_DIR)
endif

list:
	@for d in */; do \
	  d=$${d%/}; \
	  if ls $$d/src/*.c >/dev/null 2>&1; then \
	    echo "$$d:"; \
	    for f in $$d/src/*.c; do echo "  $$(basename "$$f" .c)"; done; \
	  fi; \
	done

# `make HELP`: print a summary of every target this Makefile supports.
# Works the same from the top level or inside a project dir; the last
# section lists whatever this directory's context actually contains
# (projects at the top level, programs inside a project dir).
.PHONY: HELP
HELP:
	@echo "C Makefile -- available targets"
	@echo ""
	@echo "Top level (run from C/):"
	@echo "  make <project>            to build a project."
	@echo "  make clean [proj ...]     to remove the bin directory for the named projects. If no project was given, will remove each bin directory in all the project directories."
	@echo "  make list                 list out every project"
	@echo "  make project <name>       create a new project dir"
	@echo "  make lib <name>           compile the given lib source file for a syntax check, does not link"
	@echo "  make newlib <name>        create a new lib directory with source and header files"
	@echo "  make listlib              list every library available under the lib directory"
	@echo "  make HELP                 show this message"
ifneq ($(PROJECTS),)
	@echo ""
	@echo "Projects found here:"
	@for p in $(PROJECTS); do echo "  $$p"; done
else ifneq ($(PROGRAMS),)
	@echo ""
	@echo "Programs found here (src/*.c):"
	@for p in $(PROGRAMS); do echo "  $$p"; done
endif