# ---------------------------------------------------------------------------
# C++ Makefile -- same layout and targets as the C makefile:
#
#   CPP/
#     makefile
#     lib/<dir>/<name>.cpp + <name>.hpp     shared libraries
#     <project>/src/<project>.cpp           one project per directory
#     <project>/src/headers/*.hpp (+ .cpp)  project-local helpers
#     src/*.cpp, *.cpp                      loose practice programs (still supported)
#
# Compiler + standard selection: prefers the newest g++ installed (g++-14
# over the system g++) and builds with C++23, the newest finished standard.
# Every g++ from 11 up accepts -std=c++23, but the library side (std::print,
# std::expected, ranges additions) is only complete in newer versions --
# std::print, for example, needs g++ 14.
#
# External libraries: a program that includes raylib.h (directly or through
# any header it pulls in) is linked with -lraylib automatically. Add more
# in PROGRAM_template under EXTLIBS.
#
# Override anything on the command line, e.g.
#   make array CXX=g++ CXXSTD=c++20        (or CXXSTD=c++26 to try the next one)
# Run `make clean` after changing compiler or standard.
# ---------------------------------------------------------------------------
ifneq ($(filter default undefined,$(origin CXX)),)
CXX := $(firstword $(foreach c,g++-16 g++-15 g++-14 g++,$(if $(shell command -v $(c) 2>/dev/null),$(c))))
endif
CXX_MAJOR := $(firstword $(subst ., ,$(shell $(CXX) -dumpversion 2>/dev/null)))
CXXSTD   ?= c++23

MAKEFILE_PATH := $(abspath $(firstword $(MAKEFILE_LIST)))

SRC_DIR  := src
HDR_DIR  := $(SRC_DIR)/headers
BIN_DIR  := bin
OBJ_DIR  := $(BIN_DIR)/obj
# lib/ sits next to this makefile. Absolute, so it resolves the same from
# the top level and from inside any project dir.
LIB_DIR  := $(patsubst %/,%,$(dir $(MAKEFILE_PATH)))/lib

# Extensions used when scaffolding new files.
SRC_EXT  := cpp
HDR_EXT  := hpp

CXXFLAGS := -std=$(CXXSTD) -Wall -Wextra -g -I$(HDR_DIR) -I$(LIB_DIR)
CXXFLAGS += $(EXTRA_CXXFLAGS)
DEPFLAGS := -MMD -MP
LDLIBS   := $(EXTRA_LDLIBS)

# Top-level dispatch: `make <project>` / `make clean-<project>` from CPP/ itself.
PROJECTS := $(sort $(foreach f,$(wildcard */src/*.cpp),$(firstword $(subst /, ,$(f)))))

# Programs buildable in the current directory: src/<name>.cpp and ./<name>.cpp
# (src/ wins if both exist). Names that match a project dir go to the project.
SRC_PROGRAMS  := $(filter-out $(PROJECTS),$(basename $(notdir $(wildcard $(SRC_DIR)/*.cpp))))
ROOT_PROGRAMS := $(filter-out $(PROJECTS) $(SRC_PROGRAMS),$(basename $(wildcard *.cpp)))
PROGRAMS      := $(SRC_PROGRAMS) $(ROOT_PROGRAMS)

# Extra words after clean/project/lib/newlib are arguments, not targets --
# keep them out of the project/program rules so they don't clash.
ifneq ($(filter clean project lib newlib,$(firstword $(MAKECMDGOALS))),)
GOAL_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
.PHONY: $(GOAL_ARGS)
endif

define PROJECT_template
.PHONY: $(1) clean-$(1)
$(1):
	$$(MAKE) -C $(1) -f $$(MAKEFILE_PATH) $(1)

clean-$(1):
	$$(MAKE) -C $(1) -f $$(MAKEFILE_PATH) clean
endef

$(foreach proj,$(filter-out $(GOAL_ARGS),$(PROJECTS)),$(eval $(call PROJECT_template,$(proj))))

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
	@mkdir -p $(PROJECT_NAME)/src
	@f=$(PROJECT_NAME)/src/$(PROJECT_NAME).$(SRC_EXT); \
	if [ -e "$$f" ]; then echo "skipped $$f (already exists)"; \
	else printf '/*\n    author: Henry Centeno\n    description:\n*/\n' > "$$f"; echo "created $$f"; fi
	code $(PROJECT_NAME)/src/$(PROJECT_NAME).$(SRC_EXT)

# Disable make's built-in implicit rules/variables, and silence the
# "Entering/Leaving directory" chatter from the recursive $(MAKE) -C calls.
MAKEFLAGS += -r -R --no-print-directory

# `make lib <name>`: compile a single lib/*/<name>.cpp file as a syntax check.
# <name> can be "hash_map", "hash_map.cpp", or "containers/hash_map".
# `make newlib <name>` scaffolds lib/<name>/<name>.{cpp,hpp}, and
# `make newlib <dir>/<name>` adds <name>.{cpp,hpp} to lib/<dir>/ (new or existing).
# `make listlib` enumerates every buildable library. All three work from
# the top level or from inside a project dir. Extra word after
# "lib"/"newlib" is the file name, not a build target.
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

.PHONY: lib newlib listlib
ifneq ($(PROJECTS),)
# top level: compile inside one project dir so objects land in that
# project's bin/ rather than cluttering the top level.
lib:
ifeq ($(LIB_ARG),)
	$(error Usage: make lib <name>  e.g. make lib hash_map or make lib containers/hash_map)
endif
	@$(MAKE) -C $(firstword $(PROJECTS)) -f $(MAKEFILE_PATH) lib $(LIB_ARG)
else
# "name" is searched in every lib/*/, "dir/name" is taken as an exact path.
LIB_REL := $(basename $(LIB_ARG))
ifeq ($(findstring /,$(LIB_REL)),)
LIB_REL := $(patsubst $(LIB_DIR)/%.cpp,%,$(firstword $(wildcard $(LIB_DIR)/*/$(LIB_REL).cpp)))
else
LIB_REL := $(if $(wildcard $(LIB_DIR)/$(LIB_REL).cpp),$(LIB_REL))
endif

lib: $(if $(LIB_REL),$(OBJ_DIR)/lib/$(LIB_REL).o)
ifeq ($(LIB_ARG),)
	$(error Usage: make lib <name>  e.g. make lib hash_map or make lib containers/hash_map)
endif
ifeq ($(LIB_REL),)
	$(error No source file found matching '$(LIB_ARG)' under lib/ -- use make newlib $(basename $(LIB_ARG)) to create it)
endif
	@echo "compiled lib/$(LIB_REL).cpp -> $(OBJ_DIR)/lib/$(LIB_REL).o"
endif

# `make newlib hash_map`            -> lib/hash_map/hash_map.{cpp,hpp}
# `make newlib containers/hash_map` -> lib/containers/hash_map.{cpp,hpp}
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
	@for ext in $(SRC_EXT) $(HDR_EXT); do \
	  f=$(LIB_DIR)/$(NEWLIB_DIR)/$(NEWLIB_FILE).$$ext; \
	  if [ -e "$$f" ]; then echo "skipped lib/$(NEWLIB_DIR)/$(NEWLIB_FILE).$$ext (already exists)"; \
	  else printf '/*\n    author: Henry Centeno\n    description:\n*/\n' > "$$f"; echo "created lib/$(NEWLIB_DIR)/$(NEWLIB_FILE).$$ext"; fi; \
	done
	code $(LIB_DIR)/$(NEWLIB_DIR)/$(NEWLIB_FILE).$(HDR_EXT)

# `make listlib`: enumerate every lib/*/*.cpp file that `make lib <name>`
# could build, without compiling anything.
LIB_ALL := $(sort $(patsubst $(LIB_DIR)/%.cpp,%,$(wildcard $(LIB_DIR)/*/*.cpp)))
listlib:
	@echo "Libraries available under lib/:"
	@for l in $(LIB_ALL); do echo "  $$l"; done

.PHONY: all clean list HELP
ifneq ($(PROJECTS),)
all: $(PROJECTS) $(PROGRAMS)
else
all: $(PROGRAMS)
endif

define PROGRAM_template
# Every header the program pulls in, directly or through other headers,
# as reported by the compiler itself. -M (not -MM) so system headers such
# as /usr/local/include/raylib.h show up too.
DEPS_$(1) := $$(shell $(CXX) $(CXXFLAGS) -M $(2) 2>/dev/null \
               | tr -d '\\' | tr ' ' '\n')

# Project-local headers with a matching .cpp get that .cpp compiled + linked.
HDRS_$(1) := $$(filter $(HDR_DIR)/%,$$(DEPS_$(1)))
HDR_OBJS_$(1) := $$(patsubst $(HDR_DIR)/%.cpp,$(OBJ_DIR)/headers/%.o, \
                   $$(wildcard $$(addsuffix .cpp,$$(basename $$(HDRS_$(1))))))

# Link every lib/<dir>/ that any of those headers lives in -- including
# libraries only reached through another library's header -- and nothing
# else, so an unrelated broken library can't break this build.
USED_LIBS_$(1) := $$(sort $$(foreach h,$$(filter $(LIB_DIR)/%,$$(DEPS_$(1))), \
    $$(firstword $$(subst /, ,$$(patsubst $(LIB_DIR)/%,%,$$(h))))))

LIB_OBJS_$(1) := $$(foreach l,$$(USED_LIBS_$(1)),$$(patsubst $(LIB_DIR)/%.cpp,$(OBJ_DIR)/lib/%.o,$$(wildcard $(LIB_DIR)/$$(l)/*.cpp)))

# External libraries, linked only when the program (or any header it
# pulls in) includes the matching header. Add more with +=, e.g.
#   EXTLIBS_$(1) += $$(if $$(filter %/SDL.h,$$(DEPS_$(1))),-lSDL2)
EXTLIBS_$(1) := $$(if $$(filter %/raylib.h,$$(DEPS_$(1))),-lraylib)

OBJS_$(1) := $(OBJ_DIR)/$(1).o $$(HDR_OBJS_$(1)) $$(LIB_OBJS_$(1))

$(OBJ_DIR)/$(1).o: $(2) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $$< -o $$@

$(BIN_DIR)/$(1): $$(OBJS_$(1)) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $$^ $$(EXTLIBS_$(1)) $(LDLIBS) -o $$@

.PHONY: $(1)
$(1): $(BIN_DIR)/$(1)
	@:
endef

$(foreach p,$(filter-out $(GOAL_ARGS),$(SRC_PROGRAMS)),$(eval $(call PROGRAM_template,$(p),$(SRC_DIR)/$(p).cpp)))
$(foreach p,$(filter-out $(GOAL_ARGS),$(ROOT_PROGRAMS)),$(eval $(call PROGRAM_template,$(p),$(p).cpp)))

$(OBJ_DIR)/headers/%.o: $(HDR_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/lib/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Rebuild when any included header changes.
-include $(shell find $(OBJ_DIR) -name '*.d' 2>/dev/null)

clean:
ifneq ($(CLEAN_TARGETS),)
	@for p in $(CLEAN_TARGETS); do rm -rf $$p/bin; echo "cleaned $$p/bin"; done
else
	@for p in $(PROJECTS); do rm -rf $$p/bin; echo "cleaned $$p/bin"; done
	@if [ -d $(BIN_DIR) ]; then rm -rf $(BIN_DIR); echo "cleaned $(BIN_DIR)"; fi
endif

list:
	@for d in */; do \
	  d=$${d%/}; \
	  if ls $$d/src/*.cpp >/dev/null 2>&1; then \
	    echo "$$d:"; \
	    for f in $$d/src/*.cpp; do echo "  $$(basename "$$f" .cpp)"; done; \
	  fi; \
	done
ifneq ($(strip $(PROGRAMS)),)
	@echo "loose programs here:"
	@for p in $(PROGRAMS); do echo "  $$p"; done
endif

# `make HELP`: print a summary of every target this Makefile supports.
HELP:
	@echo "C++ Makefile -- available targets  (compiler: $(CXX) $(CXX_MAJOR), -std=$(CXXSTD))"
	@echo ""
	@echo "Top level (run from CPP/):"
	@echo "  make <project>|<program>         to build a project (or a loose src/*.cpp / *.cpp program)."
	@echo "  make all                         build everything."
	@echo "  make clean [proj ...]            to remove the bin directory for the named projects. If no project was given, will remove each bin directory in all the project directories."
	@echo "  make list                        list out every project"
	@echo "  make project <name>              create a new project dir"
	@echo "  make lib <name>|<dir>/<name>     compile the given lib source file for a syntax check, does not link"
	@echo "  make newlib <name>|<dir>/<name>  create a lib source/header pair (new dir, or add to an existing one)"
	@echo "  make listlib                     list every library available under the lib directory"
	@echo "  make HELP                        show this message"
	@echo ""
	@echo "Overrides: CXX=<compiler> CXXSTD=<std> EXTRA_CXXFLAGS=... EXTRA_LDLIBS=..."
	@echo "Auto-linked: -lraylib for programs that include raylib.h"
ifneq ($(PROJECTS),)
	@echo ""
	@echo "Projects found here:"
	@for p in $(PROJECTS); do echo "  $$p"; done
endif
ifneq ($(strip $(PROGRAMS)),)
	@echo ""
	@echo "Programs found here:"
	@for p in $(PROGRAMS); do echo "  $$p"; done
endif