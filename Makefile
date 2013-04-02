CXX=g++
CPPFLAGS=-Wall -Wextra -std=c++0x -DDEBUG -I. -pedantic

test: test.cpp Array/array.hpp

clean:
	rm -f test


