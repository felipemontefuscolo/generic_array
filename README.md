generic_array
=============

A lightweight generic N-dimensional array library


This library is only a single header file (array.hpp) written
in C++. Although it is supposed to be generic, it supports
at most 10-dimensional arrays. However, there is c++11 version
which supports any rank.


Features:

- generic array dimension (at most 10 for c++03 standard);
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



