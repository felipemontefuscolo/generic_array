#ifndef MULTI_ARRAY_HPP
#define MULTI_ARRAY_HPP

#include <vector>
#include <stdexcept>
#include <iostream>
using namespace std;

#define MA_MAX_RANK 9

namespace marray {

enum Options {

  RowMajor = 1 << 1,
  ColMajor = 1 << 2

};


namespace internal
{

template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

#define MA_STATIC_CHECK(expr, msg) \
    { internal::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 
 
// to expand int i0, int i1, int i2, ... 
 
#define MA_EXPAND_ARGS1(type_)                         type_ i0
#define MA_EXPAND_ARGS2(type_) MA_EXPAND_ARGS1(type_), type_ i1
#define MA_EXPAND_ARGS3(type_) MA_EXPAND_ARGS2(type_), type_ i2
#define MA_EXPAND_ARGS4(type_) MA_EXPAND_ARGS3(type_), type_ i3
#define MA_EXPAND_ARGS5(type_) MA_EXPAND_ARGS4(type_), type_ i4
#define MA_EXPAND_ARGS6(type_) MA_EXPAND_ARGS5(type_), type_ i5
#define MA_EXPAND_ARGS7(type_) MA_EXPAND_ARGS6(type_), type_ i6
#define MA_EXPAND_ARGS8(type_) MA_EXPAND_ARGS7(type_), type_ i7
#define MA_EXPAND_ARGS9(type_) MA_EXPAND_ARGS8(type_), type_ i8

#define MA_EXPAND_ARGS_(N, type_) MA_EXPAND_ARGS##N(type_)
#define MA_EXPAND_ARGS(N, type_) MA_EXPAND_ARGS_(N, type_)


// to expand i0, i1, i2, ...

#define MA_EXPAND_SEQ1                 i0
#define MA_EXPAND_SEQ2 MA_EXPAND_SEQ1, i1
#define MA_EXPAND_SEQ3 MA_EXPAND_SEQ2, i2
#define MA_EXPAND_SEQ4 MA_EXPAND_SEQ3, i3
#define MA_EXPAND_SEQ5 MA_EXPAND_SEQ4, i4
#define MA_EXPAND_SEQ6 MA_EXPAND_SEQ5, i5
#define MA_EXPAND_SEQ7 MA_EXPAND_SEQ6, i6
#define MA_EXPAND_SEQ8 MA_EXPAND_SEQ7, i7
#define MA_EXPAND_SEQ9 MA_EXPAND_SEQ8, i8

#define MA_EXPAND_SEQ_(N) MA_EXPAND_SEQ##N
#define MA_EXPAND_SEQ(N) MA_EXPAND_SEQ_(N) 
 
 
  template<class T> class Traits;

  template<int Rank>
  struct RowMajIdxComputer
  {
    static int idx(int const*const sizes, int const*const indices)
    {
      return RowMajIdxComputer<Rank-1>::idx(sizes, indices) * sizes[Rank-1]  + indices[Rank-1];
    }
  };
  template<>
  struct RowMajIdxComputer<1>
  {
    static int idx(int const*, int const* indices)
    { return indices[0]; }
  };

  template<int Rank>
  struct ColMajIdxComputer
  {
    static int idx(int const* sizes, int const* indices)
    {
      return indices[0] + sizes[0]*ColMajIdxComputer<Rank-1>::idx(sizes+1, indices+1);
    }
  };
  template<>
  struct ColMajIdxComputer<1>
  {
    static int idx(int const*const, int const*const indices)
    { return indices[0]; }
  };


  template<int Rank, bool isRowMajor>
  struct IdxCompTraits;

  template<int Rank>
  struct IdxCompTraits<Rank, true>
  {
    typedef RowMajIdxComputer<Rank> type;
  };

  template<int Rank>
  struct IdxCompTraits<Rank, false>
  {
    typedef ColMajIdxComputer<Rank> type;
  };

#ifdef DEBUG
  template<int Rank>
  struct BoundCheck
  {
    static void check(int const dims[], int const args[])
    {
      if (args[0] >= dims[0])
        throw std::out_of_range ("ERROR: Array<>: invalid index");
      BoundCheck<Rank-1>::check(dims+1, args +1);
    }
  };

  template<>
  struct BoundCheck<1>
  {
    static void check(int const dims[], int const args[])
    {
      if (args[0] >= dims[0])
        throw std::out_of_range ("ERROR: Array<>: invalid index");
    }
  };

  // assert i < j
  inline void assertLess(int i, int j, const char* msg)
  {
    if (i >= j)
      throw std::out_of_range (msg);
  }


  inline void assertTrue(bool b, const char* msg)
  {
    if (!b)
      throw std::out_of_range (msg);
  }

#else
  template<int Rank>
  struct BoundCheck
  {
    static void check(...) { }
  };

  // assert i < j
  inline void assertLess(... )
  { }

  inline void assertTrue(...)
  { }

#endif



} // end `internal` namespace

//template<typename P_type>
//class MemoryBlock
//{
//public:
//
//  typedef P_type UserT;
//  typedef int    IndexT;
//
//protected:
//  typedef vector<UserT> StorageT;
//  StorageT m_data;
//
//public:
//
//  MemoryBlock() : m_data() {}
//
//  MemoryBlock(MemoryBlock const& b) = delete;
//  MemoryBlock& operator=(MemoryBlock const& b) = delete;
//
//  IndexT size() const
//  { return m_data.size(); }
//
//  UserT* data()
//  { return m_data.data(); }
//
//  UserT const* data() const
//  { return m_data.data(); }
//
//protected:
//
//  void resize(int n)
//  {
//  }
//
//};


template<typename Derived>
class ArrayBase
{
#if !defined(THIS) && !defined(CONST_THIS)
  #define THIS static_cast<Derived*>(this)
  #define CONST_THIS static_cast<const Derived*>(this)
#endif

  typedef typename internal::Traits<Derived>::UserT UserT;
  static const int Rank = internal::Traits<Derived>::Rank;
  static const bool isRowMajor = internal::Traits<Derived>::isRowMajor;
  typedef typename internal::Traits<Derived>::MemBlockT MemBlockT;

public:

  typedef          UserT&                       reference;
  typedef          UserT const&                 const_reference;
  typedef          UserT*                       iterator;
  typedef          UserT const*                 const_iterator;
  typedef          int                          size_type;
  typedef          std::ptrdiff_t               difference_type;
  typedef          UserT*                       pointer;
  typedef          UserT const*                 const_pointer;


  iterator begin()
  { return THIS->data(); }

  const_iterator begin() const
  { return THIS->data(); }

  iterator end()
  { return THIS->data() + THIS->size(); }

  const_iterator end() const
  { return THIS->data() + THIS->size(); }

  reference back()
  { return *(--end()); }

  const_reference back() const
  { return *(--end()); }

  reference front()
  { return *begin(); }

  const_reference front() const
  { return *begin(); }

// define call operator
#define MA_DEF_CALL_OP(N_ARGS)                                                      \
  reference operator() (MA_EXPAND_ARGS(N_ARGS, int))                                \
  {                                                                                 \
    int const indices[] = {MA_EXPAND_SEQ(N_ARGS)};                                  \
    MA_STATIC_CHECK(Rank==sizeof(indices)/sizeof(int), INVALID_NUMBER_OF_ARGS_IN_CALL_OP); \
    internal::BoundCheck<Rank>::check(THIS->rdims(), indices);                      \
    typedef typename internal::IdxCompTraits<Rank, isRowMajor>::type ToRelative;    \
    return THIS->operator[] ( ToRelative::idx(THIS->rdims(), indices) );            \
  }

  // define call operators
  MA_DEF_CALL_OP(1)
  MA_DEF_CALL_OP(2)
  MA_DEF_CALL_OP(3)
  MA_DEF_CALL_OP(4)
  MA_DEF_CALL_OP(5)  
  MA_DEF_CALL_OP(6)
  MA_DEF_CALL_OP(7)
  MA_DEF_CALL_OP(8)
  MA_DEF_CALL_OP(9)
#undef MA_DEF_CALL_OP


// define const call operator
#define MA_DEF_CONST_CALL_OP(N_ARGS)                                                \
  const_reference operator() (MA_EXPAND_ARGS(N_ARGS, int)) const                    \
  {                                                                                 \
    int const indices[] = {MA_EXPAND_SEQ(N_ARGS)};                                  \
    MA_STATIC_CHECK(Rank==sizeof(indices)/sizeof(int), INVALID_NUMBER_OF_ARGS_IN_CALL_OP); \
    internal::BoundCheck<Rank>::check(THIS->rdims(), indices);                      \
    typedef typename internal::IdxCompTraits<Rank, isRowMajor>::type ToRelative;    \
    return THIS->operator[] ( ToRelative::idx(THIS->rdims(), indices) );            \
  }

  // define call operators
  MA_DEF_CONST_CALL_OP(1)
  MA_DEF_CONST_CALL_OP(2)
  MA_DEF_CONST_CALL_OP(3)
  MA_DEF_CONST_CALL_OP(4)
  MA_DEF_CONST_CALL_OP(5)  
  MA_DEF_CONST_CALL_OP(6)
  MA_DEF_CONST_CALL_OP(7)
  MA_DEF_CONST_CALL_OP(8)
  MA_DEF_CONST_CALL_OP(9)
#undef MA_DEF_CONST_CALL_OP




  int maxDim() const
  {
    int m = 0;
    for (int i = 0; i < Rank; ++i)
      if ( CONST_THIS->dim(i) > m)
        m = CONST_THIS->dim(i);
    return m;
  }


  // Const Version -----------



#undef THIS
#undef CONST_THIS
};

template<typename P_type, int P_rank, Options P_opts = RowMajor, typename P_MemBlock = std::vector<P_type> >
class Array : public P_MemBlock, public ArrayBase<Array<P_type,P_rank,P_opts,P_MemBlock> >
{

  typedef P_MemBlock       Base1;
  typedef ArrayBase<Array> Base2;

  typedef typename Base2::reference reference;
  typedef typename Base2::const_reference const_reference;

  friend class ArrayBase<Array>;

//  using P_MemBlock::m_data;

public:

  typedef P_type UserT;
  static const int Rank = P_rank;
  static const bool isRowMajor = P_opts & RowMajor;

  using Base1::resize;

private:
    int m_rdims[Rank];   // size of each rank


public:

  Array() : Base1(), Base2(), m_rdims() {};
  Array(Array const& ) = default;
  Array& operator= (Array const&) = default;

  template<class ...Args>
  Array(Args...args)
  {
    reshape(args...);
  }

  int rank() const
  { return Rank; }

  int dim(int r) const
  {
    internal::assertTrue(r < Rank, "**ERROR**: Array<>: invalid index in function `size()`");
    return m_rdims[r];
  }
  int size() const
  { return Base1::size(); }

  template<class ...Args>
  void reshape(Args...args)
  {
    //static_assert(sizeof...(Args) == Rank, "**ERROR**: Array<>: too few arguments in the `reshape`");
    int const new_dims[] = { args... };

    int new_size = 1;
    for (int i = 0; i < Rank; ++i)
    {
      internal::assertTrue(new_dims[i] > 0, "**ERROR**: Array<>: dimension must be greater than 0");
      m_rdims[i] = new_dims[i];
      new_size *= new_dims[i];
    }

    resize(new_size);
  }

protected:

  // vecotr with rank sizes
  int* rdims()
  { return m_rdims; }

  int const* rdims() const
  { return m_rdims; }

};


namespace internal
{

template<class T, int A, Options O, class M>
struct Traits<Array<T,A,O,M> > {
  typedef T UserT;
  static const int Rank = A;
  static const bool isRowMajor = O & RowMajor;
  typedef M MemBlockT;
};

} // end namespace internal





} // end namespace

#endif
