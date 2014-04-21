#CXX=g++
CXX=clang++
CPPFLAGS=-Wall -std=c++98 -Wextra -DDEBUG -I. -pedantic

test: test.cpp Array/array.hpp
	$(CXX) $(CPPFLAGS) test.cpp -o test

clean:
	rm -f test


