CXX=g++
CPPFLAGS=-Wall -std=c++98 -Wextra -DDEBUG -I. -pedantic

test: test.cpp Array/array.hpp

clean:
	rm -f test


