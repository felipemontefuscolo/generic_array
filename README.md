generic_array
=============

A lightweight generic N-dimensional array library


This library is only a single header file (array.hpp) written
in C++. It uses some features of the new standard C++11 like
variadic templates and deleted/default functions. However, I
was able to compile this lib with gcc v4.4 and v4.7 without
any compiler flags (specially the -std=c++0x) at the price of some
annoying warnings. It's a good thing since you can link your
code with another libraries compiled in the old standard without
breaking ABI. If you use -std=c++0x flag to compile, all warnings
go away, but you only can link to another libraries in C++11
standard.

This library is only tested with gcc compiler.

Features:

- generic array dimension;
- can be chosen row or col major order;
- there are wrappers for pre-existing datas;


This library has/is

- very simple API;
- lightweight;
- tested


Check `test.cpp` file to learn how to use it.

Some comments:

- You can change the numbering order defining the macro MA_DEFAULT_MAJOR:
  `\#define MA_DEFAULT_MAJOR RowMajor`
or 
  `\#define MA_DEFAULT_MAJOR ColMajor`
before the including the library.

- For debug version (with range cheking, etc.) define the macro DEBUG before including the library;



