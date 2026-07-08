CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

SRC_DIR  := src
BIN_DIR  := bin

.PHONY: all clean list
.SECONDARY:          # <-- stop make from deleting intermediate bin/% files

# Usage: make <name>   (name = filename without .cpp, e.g. `make array`)
%: $(BIN_DIR)/%
	@:

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR)/%: %.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)/*

list:
	@echo "Buildable programs in $(SRC_DIR)/:"
	@ls $(SRC_DIR)/*.cpp | xargs -n1 basename | sed 's/\.cpp$$//'
	@echo "Buildable programs in current dir:"
	@ls *.cpp 2>/dev/null | xargs -n1 basename | sed 's/\.cpp$$//'