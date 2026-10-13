# ---------------------------------------------------------------------------
# Compiler + standard selection
#
# Prefers the newest GCC installed (gcc-14 over the system gcc) and picks the
# newest C standard that compiler fully supports:
#   GCC >= 14 -> gnu23   (final C23)
#   GCC 13    -> gnu2x   (draft C23, most features)
#   older     -> gnu17
# The gnu* variants keep POSIX/GNU extras visible (M_PI, strdup, getline...),
# which strict -std=c23 hides.
#
# Override any of these on the command line, e.g.
#   make hash_map_test CC=gcc CSTD=gnu17
# Run `make clean` after changing compiler or standard so objects aren't mixed.
# ---------------------------------------------------------------------------
ifneq ($(filter default undefined,$(origin CC)),)
CC := $(firstword $(foreach c,gcc-16 gcc-15 gcc-14 gcc,$(if $(shell command -v $(c) 2>/dev/null),$(c))))
endif
CC_MAJOR := $(firstword $(subst ., ,$(shell $(CC) -dumpversion 2>/dev/null)))
CSTD ?= $(shell if [ "$(CC_MAJOR)" -ge 14 ] 2>/dev/null; then echo gnu23; \
                elif [ "$(CC_MAJOR)" -ge 13 ] 2>/dev/null; then echo gnu2x; \
                else echo gnu17; fi)

SRC_DIR       := src
HDR_DIR       := $(SRC_DIR)/headers
BIN_DIR       := bin
OBJ_DIR       := $(BIN_DIR)/obj
LIB_DIR       := ../lib

CFLAGS   := -Wall -Wextra -g -std=$(CSTD) -I$(HDR_DIR) -I$(LIB_DIR)
CFLAGS   += $(EXTRA_CFLAGS)
DEPFLAGS := -MMD -MP

# -lm so <math.h> functions (sin, sqrt, ...) link; glibc keeps them in libm.
LDLIBS   := -lm
LDLIBS   += $(EXTRA_LDLIBS)

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

# Disable make's built-in implicit rules/variables, and silence the
# "Entering/Leaving directory" chatter from the recursive $(MAKE) -C calls.
MAKEFLAGS += -r -R --no-print-directory

# `make lib <name>`: compile a single lib/*/<name>.c file as a syntax check.
# <name> can be "hash_map", "hash_map.c", or "containers/hash_map".
# `make newlib <name>` scaffolds lib/<name>/<name>.{c,h}, and
# `make newlib <dir>/<name>` adds <name>.{c,h} to lib/<dir>/ (new or existing).
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
# top level: forward into any one project dir, since lib/ is a shared
# sibling of all of them and ../lib resolves the same from any project.
lib:
ifeq ($(LIB_ARG),)
	$(error Usage: make lib <name>  e.g. make lib hash_map or make lib containers/hash_map)
endif
	@$(MAKE) -C $(firstword $(PROJECTS)) -f $(MAKEFILE_PATH) lib $(LIB_ARG)

newlib:
ifeq ($(NEWLIB_ARG),)
	$(error Usage: make newlib <name> or make newlib <dir>/<name>  e.g. make newlib containers/hash_map)
endif
	@$(MAKE) -C $(firstword $(PROJECTS)) -f $(MAKEFILE_PATH) newlib $(NEWLIB_ARG)
else
# nested (inside a project dir): resolve the file and compile it via the
# $(OBJ_DIR)/lib/%.o pattern rule. "name" is searched in every lib/*/,
# "dir/name" is taken as an exact path.
LIB_REL := $(basename $(LIB_ARG))
ifeq ($(findstring /,$(LIB_REL)),)
LIB_REL := $(patsubst $(LIB_DIR)/%.c,%,$(firstword $(wildcard $(LIB_DIR)/*/$(LIB_REL).c)))
else
LIB_REL := $(if $(wildcard $(LIB_DIR)/$(LIB_REL).c),$(LIB_REL))
endif

lib: $(if $(LIB_REL),$(OBJ_DIR)/lib/$(LIB_REL).o)
ifeq ($(LIB_ARG),)
	$(error Usage: make lib <name>  e.g. make lib hash_map or make lib containers/hash_map)
endif
ifeq ($(LIB_REL),)
	$(error No source file found matching '$(LIB_ARG)' under $(LIB_DIR)/ -- use make newlib $(basename $(LIB_ARG)) to create it)
endif
	@echo "compiled $(LIB_DIR)/$(LIB_REL).c -> $(OBJ_DIR)/lib/$(LIB_REL).o"

# `make newlib hash_map`            -> lib/hash_map/hash_map.{c,h}
# `make newlib containers/hash_map` -> lib/containers/hash_map.{c,h}
# Existing files are never overwritten.
NEWLIB_NAME := $(basename $(NEWLIB_ARG))
ifeq ($(findstring /,$(NEWLIB_NAME)),)
NEWLIB_DIR  := $(NEWLIB_NAME)
NEWLIB_FILE := $(NEWLIB_NAME)
else
NEWLIB_DIR  := $(patsubst %/,%,$(dir $(NEWLIB_NAME)))
NEWLIB_FILE := $(notdir $(NEWLIB_NAME))
endif

newlib:
ifeq ($(NEWLIB_ARG),)
	$(error Usage: make newlib <name> or make newlib <dir>/<name>  e.g. make newlib containers/hash_map)
endif
	@mkdir -p $(LIB_DIR)/$(NEWLIB_DIR)
	@for ext in c h; do \
	  f=$(LIB_DIR)/$(NEWLIB_DIR)/$(NEWLIB_FILE).$$ext; \
	  if [ -e "$$f" ]; then echo "skipped $$f (already exists)"; \
	  else printf '/*\n    author: Henry Centeno\n    description:\n*/\n' > "$$f"; echo "created $$f"; fi; \
	done
	code $(LIB_DIR)/$(NEWLIB_DIR)/$(NEWLIB_FILE).h
endif

# `make listlib`: enumerate every lib/*/*.c file that `make lib <name>`
# could build, without compiling anything.
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
# Every header the program pulls in, directly or through other headers,
# as reported by the compiler itself.
DEPS_$(1) := $$(shell $(CC) $(CFLAGS) -MM $(SRC_DIR)/$(1).c 2>/dev/null \
               | tr -d '\\' | tr ' ' '\n')

HDRS_$(1) := $$(filter $(HDR_DIR)/%.h,$$(DEPS_$(1)))

# Link every lib/<dir>/ that any of those headers lives in -- including
# libraries only reached through another library's header -- and nothing
# else, so an unrelated broken library can't break this build.
USED_LIBS_$(1) := $$(sort $$(foreach h,$$(filter $(LIB_DIR)/%,$$(DEPS_$(1))), \
    $$(firstword $$(subst /, ,$$(patsubst $(LIB_DIR)/%,%,$$(h))))))

LIB_OBJS_$(1) := $$(foreach l,$$(USED_LIBS_$(1)),$$(patsubst $(LIB_DIR)/%.c,$(OBJ_DIR)/lib/%.o,$$(wildcard $(LIB_DIR)/$$(l)/*.c)))

OBJS_$(1) := $(OBJ_DIR)/$(1).o $$(patsubst $(HDR_DIR)/%.c,$(OBJ_DIR)/headers/%.o,$$(wildcard $$(HDRS_$(1):.h=.c))) $$(LIB_OBJS_$(1))

$(BIN_DIR)/$(1): $$(OBJS_$(1)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $$^ $(LDLIBS) -o $$@

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
else ifneq ($(PROJECTS),)
	@for p in $(PROJECTS); do rm -rf $$p/bin; echo "cleaned $$p/bin"; done
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
.PHONY: HELP
HELP:
	@echo "C Makefile -- available targets  (compiler: $(CC) $(CC_MAJOR), -std=$(CSTD))"
	@echo ""
	@echo "Top level (run from C/):"
	@echo "  make <project>                   to build a project."
	@echo "  make clean [proj ...]            to remove the bin directory for the named projects. If no project was given, will remove each bin directory in all the project directories."
	@echo "  make list                        list out every project"
	@echo "  make project <name>              create a new project dir"
	@echo "  make lib <name>|<dir>/<name>     compile the given lib source file for a syntax check, does not link"
	@echo "  make newlib <name>|<dir>/<name>  create a lib source/header pair (new dir, or add to an existing one)"
	@echo "  make listlib                     list every library available under the lib directory"
	@echo "  make HELP                        show this message"
	@echo ""
	@echo "Overrides: CC=<compiler> CSTD=<std> EXTRA_CFLAGS=... EXTRA_LDLIBS=..."
ifneq ($(PROJECTS),)
	@echo ""
	@echo "Projects found here:"
	@for p in $(PROJECTS); do echo "  $$p"; done
else ifneq ($(PROGRAMS),)
	@echo ""
	@echo "Programs found here (src/*.c):"
	@for p in $(PROGRAMS); do echo "  $$p"; done
endif