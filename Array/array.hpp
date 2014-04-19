// This file is part of generic_array, A lightweight generic
// N-dimensional array library
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.


#ifndef MULTI_ARRAY_HPP
#define MULTI_ARRAY_HPP

#include <vector>
#include <stdexcept>
#include <tr1/type_traits>

namespace marray {

#ifndef MA_DEFAULT_MAJOR
#define MA_DEFAULT_MAJOR RowMajor
#endif

namespace marray_internal {
	template<bool, typename T = void>
	struct EnableIf {};

	template<typename T>
	struct EnableIf<true, T> {
	  typedef T type;
	};
}


enum Options {

  RowMajor = 1 << 1,
  ColMajor = 1 << 2

};


namespace internal
{
  template<class T> class Traits;

  template<int> struct CompileTimeError;
  template<> struct CompileTimeError<true> {};

  #define MA_STATIC_CHECK(expr, msg) \
      { internal::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; }


// to expand `int i0, int i1, int i2, ... `

#define MA_EXPAND_ARGS1(type_)                          type_ i0
#define MA_EXPAND_ARGS2(type_)  MA_EXPAND_ARGS1(type_), type_ i1
#define MA_EXPAND_ARGS3(type_)  MA_EXPAND_ARGS2(type_), type_ i2
#define MA_EXPAND_ARGS4(type_)  MA_EXPAND_ARGS3(type_), type_ i3
#define MA_EXPAND_ARGS5(type_)  MA_EXPAND_ARGS4(type_), type_ i4
#define MA_EXPAND_ARGS6(type_)  MA_EXPAND_ARGS5(type_), type_ i5
#define MA_EXPAND_ARGS7(type_)  MA_EXPAND_ARGS6(type_), type_ i6
#define MA_EXPAND_ARGS8(type_)  MA_EXPAND_ARGS7(type_), type_ i7
#define MA_EXPAND_ARGS9(type_)  MA_EXPAND_ARGS8(type_), type_ i8
#define MA_EXPAND_ARGS10(type_) MA_EXPAND_ARGS9(type_), type_ i9

#define MA_EXPAND_ARGS_(N, type_) MA_EXPAND_ARGS##N(type_)
#define MA_EXPAND_ARGS(N, type_) MA_EXPAND_ARGS_(N, type_)


// to expand `i0, i1, i2, ...`

#define MA_EXPAND_SEQ1                  i0
#define MA_EXPAND_SEQ2  MA_EXPAND_SEQ1, i1
#define MA_EXPAND_SEQ3  MA_EXPAND_SEQ2, i2
#define MA_EXPAND_SEQ4  MA_EXPAND_SEQ3, i3
#define MA_EXPAND_SEQ5  MA_EXPAND_SEQ4, i4
#define MA_EXPAND_SEQ6  MA_EXPAND_SEQ5, i5
#define MA_EXPAND_SEQ7  MA_EXPAND_SEQ6, i6
#define MA_EXPAND_SEQ8  MA_EXPAND_SEQ7, i7
#define MA_EXPAND_SEQ9  MA_EXPAND_SEQ8, i8
#define MA_EXPAND_SEQ10 MA_EXPAND_SEQ9, i9

#define MA_EXPAND_SEQ_(N) MA_EXPAND_SEQ##N
#define MA_EXPAND_SEQ(N) MA_EXPAND_SEQ_(N)


  // for syntax sugar initialization
  template<typename UserT, typename IteratorT>
  class ListInitializer {

  public:
    ListInitializer(IteratorT iter)
        : iter_(iter)
    { }

    ListInitializer<UserT, IteratorT> operator,(UserT const& x)
    {
      *iter_ = x;
      ++iter_;
      return ListInitializer<UserT, IteratorT>(iter_);
    }

  private:
    ListInitializer();

  protected:
    IteratorT iter_;
  };

  //template<typename ArrayT, typename IteratorT = typename ArrayT::UserT*>
  template<typename UserT, typename IteratorT>
  class ListInitializationSwitch {

  public:

    ListInitializationSwitch(const ListInitializationSwitch<UserT,IteratorT>& lis) : array_(lis.array_), value_(lis.value_)
    {  }

    ListInitializationSwitch(UserT* array, UserT const& value) : array_(array), value_(value)
    { }

    ListInitializer<UserT, IteratorT> operator,(UserT const& x)
    {
      IteratorT iter = array_;
      *iter = value_;
      ++iter;
      *iter = x;
      ++iter;
      return ListInitializer<UserT, IteratorT>(iter);
    }

  private:
    ListInitializationSwitch();

  protected:
    UserT* array_;
    UserT value_;
  };





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
  struct IdxComputationTraits;

  template<int Rank>
  struct IdxComputationTraits<Rank, true>
  {
    typedef RowMajIdxComputer<Rank> type;
  };

  template<int Rank>
  struct IdxComputationTraits<Rank, false>
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


  template<bool isGreaterThan0>
  struct CheckRank;

  template<>
  struct CheckRank<true> {};


} // end `internal` namespace



//              db                                                     88888888ba
//             d88b                                                    88      "8b
//            d8'`8b                                                   88      ,8P
//           d8'  `8b     8b,dPPYba, 8b,dPPYba, ,adPPYYba, 8b       d8 88aaaaaa8P' ,adPPYYba, ,adPPYba,  ,adPPYba,
//          d8YaaaaY8b    88P'   "Y8 88P'   "Y8 ""     `Y8 `8b     d8' 88""""""8b, ""     `Y8 I8[    "" a8P_____88
//         d8""""""""8b   88         88         ,adPPPPP88  `8b   d8'  88      `8b ,adPPPPP88  `"Y8ba,  8PP"""""""
//        d8'        `8b  88         88         88,    ,88   `8b,d8'   88      a8P 88,    ,88 aa    ]8I "8b,   ,aa
//       d8'          `8b 88         88         `"8bbdP"Y8     Y88'    88888888P"  `"8bbdP"Y8 `"YbbdP"'  `"Ybbd8"'
//                                                             d8'
//                                                            d8'

template<typename Derived, int P_rank>
class ArrayBase
{};

#if !defined(THIS) && !defined(CONST_THIS)
  #define THIS static_cast<Derived*>(this)
  #define CONST_THIS static_cast<const Derived*>(this)
#endif

#define IMPLEMENT_BASE(P_rank)                                                                \
template<typename Derived>                                                                    \
class ArrayBase<Derived, P_rank>                                                              \
{                                                                                             \
                                                                                              \
  typedef internal::Traits<Derived> Traits_Derived;                                           \
  typedef ArrayBase Self;                                                                     \
  typedef typename Traits_Derived::UserT UserT;                                               \
  static const int Rank = Traits_Derived::Rank;                                               \
  static const bool isRowMajor = Traits_Derived::isRowMajor;                                  \
                                                                                              \
public:                                                                                       \
                                                                                              \
  typedef typename Traits_Derived::reference        reference;                                \
  typedef typename Traits_Derived::const_reference  const_reference;                          \
  typedef typename Traits_Derived::iterator         iterator;                                 \
  typedef typename Traits_Derived::const_iterator   const_iterator;                           \
  typedef typename Traits_Derived::size_type        size_type;                                \
  typedef typename Traits_Derived::difference_type  difference_type;                          \
  typedef typename Traits_Derived::pointer          pointer;                                  \
  typedef typename Traits_Derived::const_pointer    const_pointer;                            \
                                                                                              \
                                                                                              \
  reference operator() (MA_EXPAND_ARGS(P_rank, int))                                          \
  {                                                                                           \
    MA_STATIC_CHECK(Rank==P_rank, INVALID_NUMBER_OF_ARGS_IN_CALL_OP);                         \
                                                                                              \
    int const indices[] = {MA_EXPAND_SEQ(P_rank)};                                            \
                                                                                              \
    internal::BoundCheck<Rank>::check(THIS->rdims(), indices);                                \
                                                                                              \
    typedef typename internal::IdxComputationTraits<Rank, isRowMajor>::type ToGlobal;         \
                                                                                              \
    return THIS->access( ToGlobal::idx(THIS->rdims(), indices) );                             \
  }                                                                                           \
                                                                                              \
  const_reference operator() (MA_EXPAND_ARGS(P_rank, int)) const                              \
  {                                                                                           \
    MA_STATIC_CHECK(Rank==P_rank, INVALID_NUMBER_OF_ARGS_IN_CALL_OP);                         \
                                                                                              \
    int const indices[] = {MA_EXPAND_SEQ(P_rank)};                                            \
                                                                                              \
    internal::BoundCheck<Rank>::check(CONST_THIS->rdims(), indices);                          \
                                                                                              \
    typedef typename internal::IdxComputationTraits<Rank, isRowMajor>::type ToGlobal;         \
                                                                                              \
    return CONST_THIS->access( ToGlobal::idx(CONST_THIS->rdims(), indices) );                 \
  };                                                                                          \
                                                                                              \
  reference get(MA_EXPAND_ARGS(P_rank, int))                                                  \
  {                                                                                           \
    MA_STATIC_CHECK(Rank==P_rank, INVALID_NUMBER_OF_ARGS_IN_CALL_OP);                         \
                                                                                              \
    int const indices[] = {MA_EXPAND_SEQ(P_rank)};                                            \
                                                                                              \
    internal::BoundCheck<Rank>::check(THIS->rdims(), indices);                                \
                                                                                              \
    typedef typename internal::IdxComputationTraits<Rank, isRowMajor>::type ToGlobal;         \
                                                                                              \
    return THIS->access_check( ToGlobal::idx(THIS->rdims(), indices) );                       \
  }                                                                                           \
                                                                                              \
  const_reference get(MA_EXPAND_ARGS(P_rank, int)) const                                      \
  {                                                                                           \
    MA_STATIC_CHECK(Rank==P_rank, INVALID_NUMBER_OF_ARGS_IN_CALL_OP);                         \
                                                                                              \
    int const indices[] = {MA_EXPAND_SEQ(P_rank)};                                            \
                                                                                              \
    internal::BoundCheck<Rank>::check(CONST_THIS->rdims(), indices);                          \
                                                                                              \
    typedef typename internal::IdxComputationTraits<Rank, isRowMajor>::type ToGlobal;         \
                                                                                              \
    return CONST_THIS->access_check( ToGlobal::idx(CONST_THIS->rdims(), indices) );           \
  };                                                                                          \
                                                                                              \
                                                                                              \
  int maxDim() const                                                                          \
  {                                                                                           \
    int m = 0;                                                                                \
    for (int i = 0; i < Rank; ++i)                                                            \
      if ( CONST_THIS->dim(i) > m)                                                            \
        m = CONST_THIS->dim(i);                                                               \
    return m;                                                                                 \
  }                                                                                           \
                                                                                              \
                                                                                              \
                                                                                              \
                                                                                              \
};


IMPLEMENT_BASE(1)
IMPLEMENT_BASE(2)
IMPLEMENT_BASE(3)
IMPLEMENT_BASE(4)
IMPLEMENT_BASE(5)
IMPLEMENT_BASE(6)
IMPLEMENT_BASE(7)
IMPLEMENT_BASE(8)
IMPLEMENT_BASE(9)
IMPLEMENT_BASE(10)

#undef IMPLEMENT_BASE


#undef THIS
#undef CONST_THIS

//              db
//             d88b
//            d8'`8b
//           d8'  `8b     8b,dPPYba, 8b,dPPYba, ,adPPYYba, 8b       d8
//          d8YaaaaY8b    88P'   "Y8 88P'   "Y8 ""     `Y8 `8b     d8'
//         d8""""""""8b   88         88         ,adPPPPP88  `8b   d8'
//        d8'        `8b  88         88         88,    ,88   `8b,d8'
//       d8'          `8b 88         88         `"8bbdP"Y8     Y88'
//                                                             d8'
//                                                            d8'

template<typename P_type, int P_rank, Options P_opts = MA_DEFAULT_MAJOR, typename P_MemBlock = std::vector<P_type> >
class GenericN : public P_MemBlock, public ArrayBase<GenericN<P_type,P_rank,P_opts,P_MemBlock>,P_rank>
{
protected:
  typedef P_MemBlock          Base1;
  typedef ArrayBase<GenericN,P_rank> Base2;

  typedef typename Base2::reference reference;
  typedef typename Base2::const_reference const_reference;
  typedef typename Base2::size_type size_type;

  friend class ArrayBase<GenericN,P_rank>;

//  using P_MemBlock::m_data;

public:

  typedef P_type UserT;
  static const int Rank = P_rank;
  static const bool isRowMajor = P_opts & RowMajor;

protected:
  int m_rdims[Rank];   // size of each rank

  // user can't use this
  using Base1::resize;

public:

  GenericN() : Base1(), Base2(), m_rdims() {};
  //Array(Array const& ) = default;
  //Array& operator<< (Array const&) = default;

  int rank() const
  { return Rank; }

  int dim(int r) const
  {
    internal::assertTrue(r < Rank, "**ERROR**: Array<>: invalid index in function `size()`");
    return m_rdims[r];
  }
  int size() const
  { return Base1::size(); }

protected:

  inline
  reference access(size_type i)
  { return P_MemBlock::operator[] (i);}

  inline
  const_reference access(size_type i) const
  { return P_MemBlock::operator[] (i);}

  inline
  reference access_check(size_type i)
  { return P_MemBlock::at(i);}

  inline
  const_reference access_check(size_type i) const
  { return P_MemBlock::at(i);}


  // vecotr with rank sizes
  int* rdims()
  { return m_rdims; }

  int const* rdims() const
  { return m_rdims; }

};




template<typename P_type, int P_rank, Options P_opts = MA_DEFAULT_MAJOR, typename P_MemBlock = std::vector<P_type> >
class Array : public GenericN<P_type,P_rank,P_opts,P_MemBlock>
{
};


#define IMPLEMENT_ARRAY(P_rank)                                                                          \
template<typename P_type, Options P_opts , typename P_MemBlock  >                                        \
class Array<P_type,P_rank,P_opts,P_MemBlock> : public GenericN<P_type,P_rank,P_opts,P_MemBlock>          \
{                                                                                                        \
  typedef GenericN<P_type,P_rank,P_opts,P_MemBlock> Base0;                                               \
                                                                                                         \
  typedef typename Base0::Base1  Base1;                                                                  \
  typedef typename Base0::Base2  Base2;                                                                  \
                                                                                                         \
                                                                                                         \
public:                                                                                                  \
                                                                                                         \
  typedef typename Base2::reference        reference;                                                    \
  typedef typename Base2::const_reference  const_reference;                                              \
  typedef typename Base2::iterator         iterator;                                                     \
  typedef typename Base2::const_iterator   const_iterator;                                               \
  typedef typename Base2::size_type        size_type;                                                    \
  typedef typename Base2::difference_type  difference_type;                                              \
  typedef typename Base2::pointer          pointer;                                                      \
  typedef typename Base2::const_pointer    const_pointer;                                                \
                                                                                                         \
  typedef P_type UserT;                                                                                  \
  static const int Rank = P_rank;                                                                        \
  static const bool isRowMajor = P_opts & RowMajor;                                                      \
                                                                                                         \
  Array() {};                                                                                            \
                                                                                                         \
  internal::ListInitializationSwitch<UserT, UserT*> operator<<(const_reference x)                        \
  {                                                                                                      \
    return internal::ListInitializationSwitch<UserT, UserT*>(this->data(), x);                           \
  }                                                                                                      \
                                                                                                         \
  Array(MA_EXPAND_ARGS(P_rank, int), UserT val = UserT())                                                \
  {                                                                                                      \
    reshape(MA_EXPAND_SEQ(P_rank), val);                                                                 \
  }                                                                                                      \
                                                                                                         \
  void reshape(MA_EXPAND_ARGS(P_rank, int), UserT val = UserT())                                         \
  {                                                                                                      \
    MA_STATIC_CHECK(P_rank == Rank, TOO_FEW_ARGUMENTS_IN_RESHAPE);                                       \
    int const new_dims[] = { MA_EXPAND_SEQ(P_rank) };                                                    \
                                                                                                         \
    int new_size = 1;                                                                                    \
    for (int i = 0; i < Rank; ++i)                                                                       \
    {                                                                                                    \
      internal::assertTrue(new_dims[i] > 0, "**ERROR**: Array<>: dimension must be greater than 0");     \
      Base0::m_rdims[i] = new_dims[i];                                                                   \
      new_size *= new_dims[i];                                                                           \
    }                                                                                                    \
                                                                                                         \
    this->resize(new_size, val);                                                                         \
  }                                                                                                      \
                                                                                                         \
};

IMPLEMENT_ARRAY(1)
IMPLEMENT_ARRAY(2)
IMPLEMENT_ARRAY(3)
IMPLEMENT_ARRAY(4)
IMPLEMENT_ARRAY(5)
IMPLEMENT_ARRAY(6)
IMPLEMENT_ARRAY(7)
IMPLEMENT_ARRAY(8)
IMPLEMENT_ARRAY(9)
IMPLEMENT_ARRAY(10)

#undef IMPLEMENT_ARRAY


//               db
//              d88b
//             d8'`8b
//            d8'  `8b     88,dPYba,,adPYba,  ,adPPYYba, 8b,dPPYba,  ,adPPYba,
//           d8YaaaaY8b    88P'   "88"    "8a ""     `Y8 88P'    "8a I8[    ""
//          d8""""""""8b   88      88      88 ,adPPPPP88 88       d8  `"Y8ba,
//         d8'        `8b  88      88      88 88,    ,88 88b,   ,a8" aa    ]8I
//        d8'          `8b 88      88      88 `"8bbdP"Y8 88`YbbdP"'  `"YbbdP"'
//                                                       88
//                                                       88

template<typename P_type, int P_rank, Options P_opts = MA_DEFAULT_MAJOR >
class Amaps : public ArrayBase<Amaps<P_type,P_rank,P_opts>,P_rank>
{

  typedef ArrayBase<Amaps,P_rank> Base;

  typedef typename Base::reference        reference;
  typedef typename Base::const_reference  const_reference;
  typedef typename Base::iterator         iterator;
  typedef typename Base::const_iterator   const_iterator;
  typedef typename Base::size_type        size_type;
  typedef typename Base::difference_type  difference_type;
  typedef typename Base::pointer          pointer;
  typedef typename Base::const_pointer    const_pointer;

  friend class ArrayBase<Amaps,P_rank>;

public:

  typedef P_type UserT;
  static const int Rank = P_rank;
  static const bool isRowMajor = P_opts & RowMajor;

private:
  UserT* m_data;
  int    m_size;
  int    m_rdims[Rank];

  Amaps();
public:
  //Amaps(Amaps const& ) = default;
  //Amaps& operator<< (Amaps const&) = default;

  internal::ListInitializationSwitch<UserT, UserT*> operator<<(const_reference x)
  {
    return internal::ListInitializationSwitch<UserT, UserT*>(this->data(), x);
  }

#define MA_AMAPS_CONSTRUCTOR(n_args)                                                                 \
  Amaps(UserT* mapped, MA_EXPAND_ARGS(n_args, int))                                                  \
  {                                                                                                  \
    MA_STATIC_CHECK(n_args == Rank, TOO_FEW_ARGUMENTS_IN_AMAPS_CONSTRUCTOR);                         \
                                                                                                     \
    int const new_dims[] = { MA_EXPAND_SEQ(n_args) };                                                \
    m_size = 1;                                                                                      \
    for (int i = 0; i < Rank; ++i)                                                                   \
    {                                                                                                \
      internal::assertTrue(new_dims[i] > 0, "**ERROR**: Amaps<>: dimension must be greater than 0"); \
      m_rdims[i] = new_dims[i];                                                                      \
      m_size *= new_dims[i];                                                                         \
    }                                                                                                \
                                                                                                     \
    if (mapped == NULL)                                                                              \
      throw std::runtime_error("**ERROR**: Amaps<>: null pointer");                                  \
    m_data = mapped;                                                                                 \
  }

  MA_AMAPS_CONSTRUCTOR( 1)
  MA_AMAPS_CONSTRUCTOR( 2)
  MA_AMAPS_CONSTRUCTOR( 3)
  MA_AMAPS_CONSTRUCTOR( 4)
  MA_AMAPS_CONSTRUCTOR( 5)
  MA_AMAPS_CONSTRUCTOR( 6)
  MA_AMAPS_CONSTRUCTOR( 7)
  MA_AMAPS_CONSTRUCTOR( 8)
  MA_AMAPS_CONSTRUCTOR( 9)
  MA_AMAPS_CONSTRUCTOR(10)
#undef MA_AMAPS_CONSTRUCTOR


  //internal::ListInitializationSwitch<Amaps, UserT*> operator<<(UserT const& x)
  //{
  //  return internal::ListInitializationSwitch<Amaps, UserT*>(*this, x);
  //}

  int rank() const
  { return Rank; }

  int dim(int r) const
  {
    internal::assertTrue(r < Rank, "**ERROR**: Amaps<>: invalid index in function `size()`");
    return m_rdims[r];
  }

  int size() const
  { return m_size; }

  UserT* data()
  {return m_data; }

  UserT const* data() const
  {return m_data; }

  UserT& operator[] (int i)
  { return m_data[i]; }

  UserT const& operator[] (int i) const
  { return m_data[i]; }

protected: // used by base class

  inline
  reference access(size_type i)
  { return m_data[i];}

  inline
  const_reference access(size_type i) const
  { return m_data[i];}

  inline
  reference access_check(size_type i)
  { return m_data[i];}

  inline
  const_reference access_check(size_type i) const
  { return m_data[i];}




  // vecotr with rank sizes
  int* rdims()
  { return m_rdims; }

  int const* rdims() const
  { return m_rdims; }
};

namespace internal
{

template<class T, int A, Options O, class M>
struct Traits<GenericN<T,A,O,M> > {
  typedef T UserT;
  static const int Rank = A;
  static const bool isRowMajor = O & RowMajor;
  typedef M MemBlockT;

  typedef typename MemBlockT::reference        reference;
  typedef typename MemBlockT::const_reference  const_reference;
  typedef typename MemBlockT::iterator         iterator;
  typedef typename MemBlockT::const_iterator   const_iterator;
  typedef typename MemBlockT::size_type        size_type;
  typedef typename MemBlockT::difference_type  difference_type;
  typedef typename MemBlockT::pointer          pointer;
  typedef typename MemBlockT::const_pointer    const_pointer;

};

template<class T, int A, Options O>
struct Traits<Amaps<T,A,O> > {
  typedef T UserT;
  static const int Rank = A;
  static const bool isRowMajor = O & RowMajor;

  typedef  UserT&          reference;
  typedef  UserT const&    const_reference;
  typedef  UserT*          iterator;
  typedef  UserT const*    const_iterator;
  typedef  int             size_type;
  typedef  std::ptrdiff_t  difference_type;
  typedef  UserT*          pointer;
  typedef  UserT const*    const_pointer;

};


}





} // endnamespace









#endif
