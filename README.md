generic_array
=============

A lightweight generic N-dimensional array library



This library is only a single header file (array.hpp) written
in C++. It uses some features of the new standard C++11 like
variadic templates and deleted/default functions. However, I
was able to compile this lib with gcc4.4 without any compiler
flags (specially the -std=c++0x) at the price of some
annoying warnings. If you use -std=c++0x flag, all warnings
go away.


Features:

- generic array dimension;
- can be chosen row or col major order;
- there is a wrapper to a pre-existing data;


This library has/is

- very simple API;
- lightweight;
- tested


Check `test.cpp` file to learn how to use it.

Some comments:

- You can change the numbering order defining the macro variable MA_DEFAULT_MAJOR:
  \#define MA_DEFAULT_MAJOR RowMajor
or 
  \#define MA_DEFAULT_MAJOR ColMajor
before the library include.

- For debug version (with range cheking, etc.) define the macro variable DEBUG;



