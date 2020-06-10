
# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

CXX_FLAGS = "-std=c++17"


default: main

bin/%: %.cpp
	@mkdir -p bin
	clang++ $(CXX_FLAGS) ./$? -o $@ -lboost_regex

clean:
	rm -Rf bin
.PHONY: clean
