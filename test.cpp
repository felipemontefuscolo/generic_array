#include <iostream>
#include <cstdio>
#include <cassert>

#include <Array/array.hpp>

using namespace std;
using namespace marray;

#define KRONECKER2(i,j)   = ( i==j?1:0 )
#define KRONECKER3(i,j,k) = (i==j?(j==k?1:0):0)

void test_constructors()
{
  printf("test_constructors() ... ");
  
  Array<double, 3>  A;
  Array<double, 1>  B(5);
  Array<double, 2>  C(2,3);
  Array<double, 3>  D(2,3,4);
  Array<double, 4>  E(2,3,4,3);
  Array<double, 5>  F(2,3,4,3,2);
  Array<double, 6>  G(2,3,4,3,2,1);
  Array<double, 7>  H(2,3,4,3,2,1,2);
  Array<double, 8>  I(2,3,4,3,2,1,2,3);
  Array<double, 9>  J(2,3,4,3,2,1,2,3,4);
  
  assert( A.size() == 0    );
  assert( B.size() == 5    );
  assert( C.size() == 6    );
  assert( D.size() == 24   );
  assert( E.size() == 72   );
  assert( F.size() == 144  );
  assert( G.size() == 144  );
  assert( H.size() == 288  );
  assert( I.size() == 864  );
  assert( J.size() == 3456 );

#ifdef DEBUG
  // must throw
  try {
    A(1,2,3);
    printf("error ...\n");
    throw;
  }
  catch (...)
  { }
  
  // must not throw
  try {
    D(1,2,3);
  }
  catch (...)
  { printf("error ...\n"); throw; }
#endif


#define LOOP_TEST(M) \
  for (int i = 0; i < M.size(); ++i) \
  { \
    assert(M[i] == 0); \
    M[i] = i; \
    assert(M[i] == i); \
  }

  LOOP_TEST(A)
  LOOP_TEST(B)
  LOOP_TEST(C)
  LOOP_TEST(D)
  LOOP_TEST(E)
  LOOP_TEST(F)
  LOOP_TEST(G)
  LOOP_TEST(H)
  LOOP_TEST(I)
  LOOP_TEST(J)

  assert(A.dim(0) == 0);
  assert(A.dim(1) == 0);
  assert(A.dim(2) == 0);

  assert(J.dim(0) == 2);
  assert(J.dim(1) == 3);
  assert(J.dim(2) == 4);
  assert(J.dim(3) == 3);
  assert(J.dim(4) == 2);
  assert(J.dim(5) == 1);
  assert(J.dim(6) == 2);
  assert(J.dim(7) == 3);
  assert(J.dim(8) == 4);

  printf("OK\n");
}

void test_RowMajor()
{
  printf("test_RowMajor() ... ");
  
  Array<double, 3>  A(2,3,4);  // RowMajor by default
  
  assert(A.size() == 24);
  
  for (int i = 0; i < A.size(); ++i)
    A[i] = i;
  
  int accum = 0;
  for (int i = 0; i < A.dim(0); ++i)
    for (int j = 0; j < A.dim(1); ++j)
      for (int k = 0; k < A.dim(2); ++k)
        assert( A(i,j,k) ==  accum++);
  
  printf("OK\n");
}

void test_ColMajor()
{
  printf("test_ColMajor() ... ");
  
  Array<double, 3, ColMajor> A(2,3,4);
 // Array<double, 3, ColMajor> B(2,3,4);
  
  assert(A.size() == 24);
  
  for (int i = 0; i < A.size(); ++i)
    A[i] = i;  
  
  int accum = 0;
  for (int k = 0; k < A.dim(2); ++k)
    for (int j = 0; j < A.dim(1); ++j)
      for (int i = 0; i < A.dim(0); ++i)
        assert( A(i,j,k) ==  accum++);
  
  printf("OK\n");
}

void test_InitializerRowM()
{
  printf("test_InitializerRowM() ... ");
  
  Array<double, 3> A(2,3,4);
  
  A = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;
  
  int accum = 0;
  for (int i = 0; i < A.dim(0); ++i)
    for (int j = 0; j < A.dim(1); ++j)
      for (int k = 0; k < A.dim(2); ++k)
        assert( A(i,j,k) ==  accum++);  
  
  printf("OK\n");
}

void test_InitializerColM()
{
  printf("test_InitializerColM() ... ");
  
  Array<double, 3, ColMajor> B(2,3,4);
  
  B = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;
  
  int accum = 0;
  for (int k = 0; k < B.dim(2); ++k)
    for (int j = 0; j < B.dim(1); ++j)
      for (int i = 0; i < B.dim(0); ++i)
        assert( B(i,j,k) ==  accum++);  
  
  printf("OK\n");
}

void test_CopyConstructor()
{
  printf("test_CopyConstructor() ... ");
  
  Array<double, 3> B(2,3,4);
  
  B = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;
  
  Array<double, 3> A(B);
  
  int accum = 0;
  for (int i = 0; i < A.dim(0); ++i)
    for (int j = 0; j < A.dim(1); ++j)
      for (int k = 0; k < A.dim(2); ++k)
        assert( A(i,j,k) ==  accum++);    
  
  printf("OK\n");
}

void test_Assign()
{
  printf("test_Assign() ... ");
  
  Array<double, 3> B(2,3,4);
  
  B = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;
  
  Array<double, 3> A;
  
  A = B;
  
  int accum = 0;
  for (int i = 0; i < A.dim(0); ++i)
    for (int j = 0; j < A.dim(1); ++j)
      for (int k = 0; k < A.dim(2); ++k)
      {
        assert( A(i,j,k) ==  accum++);
        A(i,j,k) = -1;
      }

  for (int i = 0; i < B.dim(0); ++i)
    for (int j = 0; j < B.dim(1); ++j)
      for (int k = 0; k < B.dim(2); ++k)
        assert( B(i,j,k) > 0  );  
  
  printf("OK\n");
}

void test_Reshape()
{
  printf("test_Reshape() ... ");
  
  Array<double, 3> B(2,3,4);
  
  B = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  B.reshape(4,3,1);
  
  assert(B.dim(0) == 4);
  assert(B.dim(1) == 3);
  assert(B.dim(2) == 1);
  assert(B.rank() == 3);
  assert(B.size() == 12);
  
  int accum = 0;
  for (int i = 0; i < B.dim(0); ++i)
    for (int j = 0; j < B.dim(1); ++j)
      for (int k = 0; k < B.dim(2); ++k)
        assert( B(i,j,k) ==  accum++);     
  
  B.reshape(4,4,4);  
  
  assert(B.dim(0) == 4);
  assert(B.dim(1) == 4);
  assert(B.dim(2) == 4);  

  assert(B.size() == 64);
  
  accum = 0;
  for (int i = 0; i < B.dim(0); ++i)
    for (int j = 0; j < B.dim(1); ++j)
      for (int k = 0; k < B.dim(2); ++k)
        B(i,j,k) =  accum++;
  
  for (int i = 0; i < B.size(); ++i)
    assert(B[i] == i);
  
  printf("OK\n");
}

void test_Amaps()
{
  printf("test_Amaps() ... ");
  
  Array<double, 3> B(2,3,4);
  
  B = 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;
  
  Amaps<double, 2> A(B.data(), 6, 4);
  
  assert(A.size() == 24);
  assert(B.size() == 24);
  
  assert(A.dim(0) == 6);
  assert(A.dim(1) == 4);
  
  int acc = 0;
  for (int i = 0; i < A.dim(0); ++i)
    for (int j = 0; j < A.dim(1); ++j)
    {
      assert(A(i,j) == acc);
      A(i,j) = 23 - acc;
      ++acc;
    }
    
  // check if reflects on B
  for (int i = 0; i < B.size(); ++i)
    assert( B[i] == 23-i );
  
  Amaps<double, 2> C(A);

  assert(C.size() == 24);

  assert(C.dim(0) == 6);
  assert(C.dim(1) == 4);

  acc = 23;
  for (int i = 0; i < C.dim(0); ++i)
    for (int j = 0; j < C.dim(1); ++j)
    {
      assert(C(i,j) == acc);
      C(i,j) = 23 - acc;
      --acc;
    }

  // check if reflects on B
  for (int i = 0; i < B.size(); ++i)
    assert( B[i] == i );  
  
  new(&A) Amaps<double, 2>(B.data(), 12, 2);
  
  assert(A.size() == 24);
  assert(A.rank() == 2);
  assert(A.dim(0) == 12);
  assert(A.dim(1) == 2);  
  
  A = -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1;
  
  for (int i = 0; i < B.size(); ++i)
    assert( B[i] == -1 );
  
  printf("OK\n");
}

int main()
{
  test_constructors();
  test_RowMajor();  
  test_ColMajor();
  test_InitializerRowM();
  test_InitializerColM();
  test_Amaps();
  
  printf("Everything seems OK \n");
}



