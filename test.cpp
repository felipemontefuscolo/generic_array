#include <iostream>
#include <cstdio>
#include <cassert>

#include <deque>

#include <Array/array.hpp>

using namespace std;
using namespace marray;

typedef std::size_t index;

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
  Array<double, 8>  I(2,3,4,3,2,1,2,3, /*value*/ 1.125);
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

  for (index i=0; i<I.size(); ++i)
	assert( I.at(i) == 1.125 );

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
  for (index i = 0; i < M.size(); ++i) \
  { \
    assert(M.access(i) == 0); \
    M.access(i) = i; \
    assert(M.access(i) == i); \
  }

  LOOP_TEST(A)
  LOOP_TEST(B)
  LOOP_TEST(C)
  LOOP_TEST(D)
  LOOP_TEST(E)
  LOOP_TEST(F)
  LOOP_TEST(G)
  LOOP_TEST(H)
  //LOOP_TEST(I)
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

#undef LOOP_TEST

}

void test_constructors_int()
{
  printf("test_constructors_int() ... ");

  Array<int, 3>  A;
  Array<int, 1>  B(5);
  Array<int, 2>  C(2,3);
  Array<int, 3>  D(2,3,4);
  Array<int, 4>  E(2,3,4,3);
  Array<int, 5>  F(2,3,4,3,2);
  Array<int, 6>  G(2,3,4,3,2,1);
  Array<int, 7>  H(2,3,4,3,2,1,2);
  Array<int, 8>  I(2,3,4,3,2,1,2,3, /*value*/ 99);
  Array<int, 9>  J(2,3,4,3,2,1,2,3,4);

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

  for (index i=0; i<I.size(); ++i)
	assert( I.at(i) == 99 );

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
  for (index i = 0; i < M.size(); ++i) \
  { \
    assert(M.access(i) == 0); \
    M.access(i) = i; \
    assert(M.access(i) == (int)i); \
  }

  LOOP_TEST(A)
  LOOP_TEST(B)
  LOOP_TEST(C)
  LOOP_TEST(D)
  LOOP_TEST(E)
  LOOP_TEST(F)
  LOOP_TEST(G)
  LOOP_TEST(H)
  //LOOP_TEST(I)
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


}

void test_RowMajor()
{
  printf("test_RowMajor() ... ");

  // 3x3 test
  {
    Array<double, 3>  A(2,3,4);  // RowMajor by default

    assert(A.size() == 24);

    for (index i = 0; i < A.size(); ++i)
      A.access(i) = i;

    int accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
      for (index j = 0; j < A.dim(1); ++j)
        for (index k = 0; k < A.dim(2); ++k)
        {
          assert( A(i,j,k)          ==  accum);
          assert( A[i][j][k]        ==  accum);
          assert( A(listify(i,j,k)) ==  accum++);
        }

    Array<double, 3> const B(A);  // RowMajor by default

    accum = 0;
    for (index i = 0; i < B.dim(0); ++i)
      for (index j = 0; j < B.dim(1); ++j)
        for (index k = 0; k < B.dim(2); ++k)
        {
          assert( B(i,j,k)   ==  accum);
          assert( B[i][j][k] ==  accum++);
        }

    accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
      for (index j = 0; j < A.dim(1); ++j)
        for (index k = 0; k < A.dim(2); ++k)
        {
          A[i][j][k] = accum--;
          assert( A[i][j][k] ==  A(i,j,k));
        }

    for (index i = 0; i < A.size(); ++i)
    {
      //printf("AQUIIIIIIII %f %d\n", A.access(i), -int(i));
      assert( A.access(i) == -int(i) );
    }

  }

  // 2x2 test
  {
    Array<double, 2>  A(2,3);  // RowMajor by default

    assert(A.size() == 6);

    for (index i = 0; i < A.size(); ++i)
      A.access(i) = i;

    int accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
      for (index j = 0; j < A.dim(1); ++j)
        {
          assert( A(i,j)   ==  accum);
          assert( A[i][j]  ==  accum++);
        }

    Array<double, 2> const B(A);  // RowMajor by default

    accum = 0;
    for (index i = 0; i < B.dim(0); ++i)
      for (index j = 0; j < B.dim(1); ++j)
        {
          assert( B(i,j)   ==  accum);
          assert( B[i][j]  ==  accum++);
        }

    accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
      for (index j = 0; j < A.dim(1); ++j)
        {
          A[i][j] = accum--;
          assert( A[i][j] ==  A(i,j));
        }

    for (index i = 0; i < A.size(); ++i)
      assert( A.access(i) == -int(i) );

  }

  // 1x1 test
  {
    Array<double, 1>  A(2);  // RowMajor by default

    assert(A.size() == 2);

    for (index i = 0; i < A.size(); ++i)
      A.access(i) = i;

    int accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
        {
          assert( A(i)   ==  accum);
          assert( A[i]  ==  accum++);
        }

    Array<double, 1> const B(A);  // RowMajor by default

    accum = 0;
    for (index i = 0; i < B.dim(0); ++i)
        {
          assert( B(i)   ==  accum);
          assert( B[i]  ==  accum++);
        }

    accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
        {
          A[i] = accum--;
          assert( A[i] ==  A(i));
        }

    for (index i = 0; i < A.size(); ++i)
      assert( A.access(i) == -int(i) );

  }


}

void test_ColMajor()
{
  printf("test_ColMajor() ... ");

  // 3x3 test
  {
    Array<double, 3, ColMajor>  A(2,3,4);  // RowMajor by default

    assert(A.size() == 24);

    for (index i = 0; i < A.size(); ++i)
      A.access(i) = i;

    int accum = 0;
    for (index k = 0; k < A.dim(2); ++k)
      for (index j = 0; j < A.dim(1); ++j)
        for (index i = 0; i < A.dim(0); ++i)
        {
          assert( A(i,j,k)   ==  accum);
          assert( A[i][j][k] ==  accum++);
        }

    Array<double, 3, ColMajor> const B(A);  // RowMajor by default

    accum = 0;
    for (index k = 0; k < B.dim(2); ++k)
      for (index j = 0; j < B.dim(1); ++j)
        for (index i = 0; i < B.dim(0); ++i)
        {
          assert( B(i,j,k)   ==  accum);
          assert( B[i][j][k] ==  accum++);
        }

    accum = 0;
    for (index k = 0; k < A.dim(2); ++k)
      for (index j = 0; j < A.dim(1); ++j)
        for (index i = 0; i < A.dim(0); ++i)
        {
          A[i][j][k] = accum--;
          assert( A[i][j][k] ==  A(i,j,k));
        }

    for (index i = 0; i < A.size(); ++i)
      assert( A.access(i) == -int(i) );

  }

  // 2x2 test
  {
    Array<double, 2, ColMajor>  A(2,3);  // RowMajor by default

    assert(A.size() == 6);

    for (index i = 0; i < A.size(); ++i)
      A.access(i) = i;

    int accum = 0;
    for (index j = 0; j < A.dim(1); ++j)
      for (index i = 0; i < A.dim(0); ++i)
        {
          assert( A(i,j)   ==  accum);
          assert( A[i][j]  ==  accum++);
        }

    Array<double, 2, ColMajor> const B(A);  // RowMajor by default

    accum = 0;
    for (index j = 0; j < B.dim(1); ++j)
      for (index i = 0; i < B.dim(0); ++i)
        {
          assert( B(i,j)   ==  accum);
          assert( B[i][j]  ==  accum++);
        }

    accum = 0;
    for (index j = 0; j < A.dim(1); ++j)
      for (index i = 0; i < A.dim(0); ++i)
        {
          A[i][j] = accum--;
          assert( A[i][j] ==  A(i,j));
        }

    for (index i = 0; i < A.size(); ++i)
      assert( A.access(i) == -int(i) );

  }

  // 1x1 test
  {
    Array<double, 1, ColMajor>  A(2);  // RowMajor by default

    assert(A.size() == 2);

    for (index i = 0; i < A.size(); ++i)
      A.access(i) = i;

    int accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
        {
          assert( A(i)  ==  accum);
          assert( A[i]  ==  accum++);
        }

    Array<double, 1, ColMajor> const B(A);  // RowMajor by default

    accum = 0;
    for (index i = 0; i < B.dim(0); ++i)
        {
          assert( B(i)  ==  accum);
          assert( B[i]  ==  accum++);
        }

    accum = 0;
    for (index i = 0; i < A.dim(0); ++i)
        {
          A[i] = accum--;
          assert( A[i] ==  A(i));
        }

    for (index i = 0; i < A.size(); ++i)
      assert( A.access(i) == -int(i) );

  }

}

void test_InitializerRowM()
{
  printf("test_InitializerRowM() ... ");

  Array<double, 3> A(2,3,4);

  A << 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  int accum = 0;
  for (index i = 0; i < A.dim(0); ++i)
    for (index j = 0; j < A.dim(1); ++j)
      for (index k = 0; k < A.dim(2); ++k)
        assert( A(i,j,k) ==  accum++);


}

void test_InitializerColM()
{
  printf("test_InitializerColM() ... ");

  Array<double, 3, ColMajor> B(2,3,4);

  B << 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  int accum = 0;
  for (index k = 0; k < B.dim(2); ++k)
    for (index j = 0; j < B.dim(1); ++j)
      for (index i = 0; i < B.dim(0); ++i)
        assert( B(i,j,k) ==  accum++);


}

void test_CopyConstructor()
{
  printf("test_CopyConstructor() ... ");

  Array<double, 3> B(2,3,4);

  B << 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  Array<double, 3> A(B);

  int accum = 0;
  for (index i = 0; i < A.dim(0); ++i)
    for (index j = 0; j < A.dim(1); ++j)
      for (index k = 0; k < A.dim(2); ++k)
        assert( A(i,j,k) ==  accum++);


}

void test_Assign()
{
  printf("test_Assign() ... ");

  Array<double, 3> B(2,3,4);

  B << 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  Array<double, 3> A;

  A = B;

  int accum = 0;
  for (index i = 0; i < A.dim(0); ++i)
    for (index j = 0; j < A.dim(1); ++j)
      for (index k = 0; k < A.dim(2); ++k)
      {
        assert( A(i,j,k) ==  accum++);
        A(i,j,k) = -1;
      }

  for (index i = 0; i < B.dim(0); ++i)
    for (index j = 0; j < B.dim(1); ++j)
      for (index k = 0; k < B.dim(2); ++k)
        assert( B(i,j,k) >= 0  );


}

void test_Reshape()
{
  printf("test_Reshape() ... ");

  Array<double, 3> B(2,3,4);

  B << 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  B.reshape(4,3,1);

  assert(B.dim(0) == 4);
  assert(B.dim(1) == 3);
  assert(B.dim(2) == 1);
  assert(B.rank() == 3);
  assert(B.size() == 12);

  int accum = 0;
  for (index i = 0; i < B.dim(0); ++i)
    for (index j = 0; j < B.dim(1); ++j)
      for (index k = 0; k < B.dim(2); ++k)
        assert( B(i,j,k) ==  accum++);

  B.reshape(4,4,4);

  assert(B.dim(0) == 4);
  assert(B.dim(1) == 4);
  assert(B.dim(2) == 4);

  assert(B.size() == 64);

  accum = 0;
  for (index i = 0; i < B.dim(0); ++i)
    for (index j = 0; j < B.dim(1); ++j)
      for (index k = 0; k < B.dim(2); ++k)
        B(i,j,k) =  accum++;

  for (index i = 0; i < B.size(); ++i)
    assert(B.access(i) == i);

  double const some_val = 1.25;
  B.reshape(8,4,4, some_val);
  for (index i = 0; i < B.dim(0); ++i)
    for (index j = 0; j < B.dim(1); ++j)
      for (index k = 0; k < B.dim(2); ++k)
      {
        if (i<4)
          // WARNING: it is not true for other dimensions because the elements
          // is stored in a contiguous memory block. It is not true
          // for ColMajor too.
          assert(B(i,j,k) == i*B.dim(1)*B.dim(2) + j*B.dim(2) + k);
        else
          assert(B(i,j,k) == some_val);
      }


}

void test_Amaps()
{
  printf("test_Amaps() ... ");

  Array<double, 3> B(2,3,4);

  B << 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23;

  Amaps<double, 2> A(B.data(), 6, 4);

  assert(A.size() == 24);
  assert(B.size() == 24);

  assert(A.dim(0) == 6);
  assert(A.dim(1) == 4);

  int acc = 0;
  for (index i = 0; i < A.dim(0); ++i)
    for (index j = 0; j < A.dim(1); ++j)
    {
      assert(A(i,j) == acc);
      assert(A[i][j] == acc);
      A[i][j] = 23 - acc;
      ++acc;
    }

  // check if reflects on B
  for (index i = 0; i < B.size(); ++i)
    assert( B.access(i) == 23-i );

  Amaps<double, 2> C(A);

  assert(C.size() == 24);

  assert(C.dim(0) == 6);
  assert(C.dim(1) == 4);

  acc = 23;
  for (index i = 0; i < C.dim(0); ++i)
    for (index j = 0; j < C.dim(1); ++j)
    {
      assert(C(i,j) == acc);
      C(i,j) = 23 - acc;
      --acc;
    }

  // check if reflects on B
  for (index i = 0; i < B.size(); ++i)
    assert( B.access(i) == i );

  new(&A) Amaps<double, 2>(B.data(), 12, 2);

  assert(A.size() == 24);
  assert(A.rank() == 2);
  assert(A.dim(0) == 12);
  assert(A.dim(1) == 2);

  A << -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1;

  for (index i = 0; i < B.size(); ++i)
    assert( B.access(i) == -1 );


}

void test_deque()
{
  printf("test_deque() ... ");

  Array<index, 3, RowMajor, std::deque<index> > B(600,3,10);

  for(index i=0; i<B.size(); ++i)
    B.access(i) = i;

  for(index i=0; i<B.dim(0); ++i)
    for (index j=0; j<B.dim(1); ++j)
      for (index k=0; k<B.dim(2); ++k)
        assert(B(i,j,k) == i*B.dim(2)*B.dim(1) + j*B.dim(2) + k);


}

void test_deque_iterators()
{
  typedef Array<index, 3, RowMajor, std::deque<index> > Array_t;

  Array_t B(600,3,10);
  Array_t::iterator it, end;

  int c = 0;
  for(it = B.begin(); it < B.end(); ++it)
    *it = c++;

  for(index i=0; i<B.dim(0); ++i)
    for (index j=0; j<B.dim(1); ++j)
      for (index k=0; k<B.dim(2); ++k)
        assert(B(i,j,k) == i*B.dim(2)*B.dim(1) + j*B.dim(2) + k);


}

#define TEST(fun_name) printf( #fun_name "() ... "); \
                       fun_name ();                  \
                       printf("OK\n");


int main()
{
  TEST(test_constructors    );
  TEST(test_constructors_int);
  TEST(test_RowMajor        );
  TEST(test_ColMajor        );
  TEST(test_InitializerRowM );
  TEST(test_InitializerColM );
  TEST(test_CopyConstructor );
  TEST(test_Assign          );
  TEST(test_Reshape         );
  TEST(test_Amaps           );
  TEST(test_deque           );
  TEST(test_deque_iterators );

  printf("Everything seems OK \n");
}



