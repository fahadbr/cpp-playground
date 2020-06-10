
CXX_FLAGS = "-std=c++17"

default: main

%: %.cpp
	clang++ $(CXX_FLAGS) ./$@.cpp -o $@
