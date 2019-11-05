/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef MULTIARRAY_H
#define MULTIARRAY_H
#include <assert.h>

namespace classdesc
{
    template <class T, int Rank> class MultiArray;
  template <class T, int R> void advance(MultiArray<T,R>& a, std::ptrdiff_t n);

  /// base class of iterator for metaprogramming
  struct MultiArrayIterator{};
  
  // turn a pointer back into multidimensional array
  template <class T, int Rank>
  class MultiArray
  {
    T* m_data;
    size_t dim[Rank];
    size_t m_stride=1;

    friend void advance<T,Rank>(MultiArray<T,Rank>&,std::ptrdiff_t);
    
    void constructDims(size_t d, size_t d1)
    {
      assert(d==Rank-1);
      dim[d]=d1;
    }
    template <class... Args>
    void constructDims(size_t d, size_t d1, Args... args)
    {
      dim[d]=d1;
      m_stride*=d1;
      constructDims(d+1, args...);
    }

  public:
    typedef MultiArray<T,Rank-1> value_type;
    typedef size_t size_type;
    /// Create a MultiArray given data and dimensions passed as arguments
    /// 
    template <class... Args>
    MultiArray(T* data, Args... args): m_data(data)
    {
      constructDims(0, args...);
    }
    /// Create a MultiArray given data and vector of dimensions:
    /// @param must be of length \prod_i a_dim[i]
    /// @param a_dim must be of length Rank
    MultiArray(T* data, const size_t a_dim[]): m_data(data)
    {
      memcpy(dim, a_dim, sizeof(dim));
    }
    
    size_t size() const {return dim[Rank-1];}
    size_t stride() const {return m_stride;}
    const MultiArray<T,Rank-1> operator[](size_t i) const {
      return MultiArray<T,Rank-1>(m_data+i*m_stride, dim);
    }
    MultiArray<T,Rank-1> operator[](size_t i)  {
      return MultiArray<T,Rank-1>(m_data+i*m_stride, dim);
    }

    /// return if this refers to the same memory location as x
    bool same(const MultiArray& x) const {return m_data==x.m_data;}
    
    struct iterator: public MultiArrayIterator
    {
      using difference_type=std::ptrdiff_t;
      using value_type=MultiArray<T,Rank-1>;
      using pointer=MultiArray<T,Rank-1>*;
      using reference=MultiArray<T,Rank-1>&;
      using iterator_category=std::random_access_iterator_tag;
      using underlying_type=T;
      static const int rank=Rank;
      
      MultiArray<T,Rank-1> array;
      size_t stride;
      iterator(const MultiArray& array): array(array.m_data,array.dim),
                                         stride(array.stride()) {}
      iterator operator++() {advance(array,stride); return *this;}
      iterator operator++(int)
      {auto tmp=*this; advance(array,stride); return tmp;}
      iterator operator--() {advance(array,-stride); return *this;}
      iterator operator--(int)
      {auto tmp=*this; advance(array,-stride); return tmp;}
      iterator operator+=(size_t i) {advance(array,i*stride); return *this;}
      MultiArray<T,Rank-1>& operator*() {return array;}
      bool operator==(const iterator& x) const {return array.same(x.array);} 
      bool operator!=(const iterator& x) const {return !operator==(x);} 
    };
    struct const_iterator: public iterator
    {
      const_iterator(const MultiArray& array): iterator(array) {}
      const MultiArray<T,Rank-1>& operator*() {return iterator::operator*();}
    };
    
    iterator begin() {return MultiArray(*this);}
    iterator end() {auto tmp=begin(); tmp+=dim[Rank-1]; return tmp;}
    const_iterator begin() const {return MultiArray(*this);}
    const_iterator end() const {auto tmp=begin(); tmp+=dim[Rank-1]; return tmp;}
  };

  template <class T>
  class MultiArray<T, 1>
  {
    T* m_data;
    size_t m_size;
    friend void advance<T,1>(MultiArray<T,1>&,std::ptrdiff_t);
    
  public:
    typedef T value_type;
    typedef size_t size_type;
    static const int rank=1;
    MultiArray<T, 1>(T* data, size_t size): m_data(data), m_size(size) {}
    MultiArray<T, 1>(T* data, const size_t size[]): m_data(data), m_size(size[0]) {}
    T& operator[](size_t i) {return m_data[i];}
    const T& operator[](size_t i) const {return m_data[i];}
    typedef T* iterator;
    typedef const T* const_iterator;
    T* begin() {return m_data;}
    T* end() {return m_data+m_size;}
    const T* begin() const {return m_data;}
    const T* end() const {return m_data+m_size;}
    size_t size() const {return m_size;}
    bool same(const MultiArray& x) const {return m_data==x.m_data;}
  };

  template <class T, int N> struct is_sequence<MultiArray<T,N>>: public true_type {};
  
  /// friended advance function for use in iterators
  template <class T, int R>
  void advance(MultiArray<T,R>& a, std::ptrdiff_t n)
  {a.m_data+=n;}

  template <class T, int R>
  struct tn<MultiArray<T,R>>
  {
    static string name() {return "classdesc::MultiArray<"+typeName<T>()+","+std::to_string(R)+">";}
  };

  template <class T>
  struct tn<T, 
            typename enable_if<
              And<is_base_of<MultiArrayIterator,T>, Not<is_same<MultiArrayIterator,T>>>,
              void>::T>
  {
    static string name() {
      return typeName<typename T::value_type>()+"::iterator";
    }
  };

  

}

#endif
