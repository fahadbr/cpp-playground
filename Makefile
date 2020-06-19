
# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

CXX_FLAGS = -std=c++17


default: main

bm1: bm1.cpp
	@mkdir -p bin
	clang++ $(CXX_FLAGS) -O2 ./$? -o bin/$@ -lboost_regex
.PHONY: bm1

rocksdb%: rocksdb%.cpp
	clang++ -std=c++17 -lrocksdb -ldl ./$? -o bin/$@
.PHONY: rocksdb%

bin/%: %.cpp
	@mkdir -p bin
	clang++ $(CXX_FLAGS) ./$? -o $@

clean:
	rm -Rf bin
.PHONY: clean
