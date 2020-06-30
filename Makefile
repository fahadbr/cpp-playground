
# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

cxx_flags = $(CXX_FLAGS) -std=c++17


default: main

bm1: bm1.cpp
	@mkdir -p bin
	clang++ $(cxx_flags) -O2 ./$? -o bin/$@ -lboost_regex
.PHONY: bm1

rocksdb%: rocksdb%.cpp
	clang++ $(cxx_flags) -lrocksdb -ldl ./$? -o bin/$@
.PHONY: rocksdb%

bin/%: %.cpp
	@mkdir -p bin
	clang++ $(cxx_flags) ./$? -o $@

clean:
	rm -Rf bin
.PHONY: clean
