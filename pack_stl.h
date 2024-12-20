/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/** \file
\brief serialisation for standard containers
*/

#ifndef CLASSDESC_PACK_STL_H
#define CLASSDESC_PACK_STL_H

#include "pack_base.h"

#if defined(__cplusplus) && __cplusplus>=201103L
#include <array>
#endif
#include <iterator>
#include <string>
#include <vector>

namespace classdesc
{
  typedef unsigned long long uint64;
  
  // map<K,V>::value_type is pair<const K,V>, which causes problems
  template <class C> struct Value_Type 
  {typedef typename C::value_type value_type;};

  template <class K, class V, class C, class A> 
  struct Value_Type<std::map<K,V,C,A> > {typedef std::pair<K,V> value_type;};

  template <class K, class V, class C, class A> 
  struct Value_Type<std::multimap<K,V,C,A> > 
  {typedef std::pair<K,V> value_type;};


  /**
     Iterators are impossible to trap, as each compiler uses its own intrinsic
     type.

     Instead, we devise a replacement serialisable Iterator type that
     derives from the relevant container iterator, but can be
     serialised
  */
  template <class T>
  class Iterator: public T::iterator
  {
    // maintain a reference to the container for serialisation purposes
    T *container;
    friend struct classdesc_access::access_pack<Iterator>;
    friend struct classdesc_access::access_unpack<Iterator>;
  public:
    Iterator(): container(NULL) {} ///<serialisation is a nop if no container provided
    Iterator(T& container): container(&container) {}
    void operator=(const typename T::iterator& i) {T::iterator::operator=(i);}
    typename T::iterator& iter() {return *this;}
    const typename T::iterator& iter() const {return *this;}
  };

  // partial specialisation for const_iterators
  template <class T>
  class Iterator<const T>: public T::const_iterator
  {
    // maintain a reference to the container for serialisation purposes
    const T *container;
    friend struct classdesc_access::access_pack<Iterator>;
    friend struct classdesc_access::access_unpack<Iterator>;
  public:
    Iterator(): container(NULL) {} ///<serialisation is a nop if no container provided
    Iterator(const T& container): container(&container) {}
    void operator=(const typename T::const_iterator& i) {T::const_iterator::operator=(i);}
    typename T::const_iterator& iter() {return *this;}
    const typename T::const_iterator& iter() const {return *this;}
  };

  template <class T, class A>
  BinStream& BinStream::operator<<(const std::vector<T,A>& x)
  {
    (*this) << uint64(x.size());
    if (!x.empty())
      packer.packraw(reinterpret_cast<const char*>(x.data()), 
                     x.size()*sizeof(x[0]));
    return *this;
  }
  template <class T, class A>
  BinStream& BinStream::operator>>(std::vector<T,A>& x)
  {
    uint64 sz;
    (*this) >> sz;
    if (uint64(packer.size()-packer.pos())<sz*sizeof(T))
      throw pack_error("invalid size for data available");
    x.resize(sz);
    if (sz)
      packer.unpackraw(reinterpret_cast<char*>(x.data()), 
                       x.size()*sizeof(x[0]));
    return *this;
  }


  template <class T>
  typename enable_if<is_container<T>, void>::T
  pack(classdesc::pack_t& b, const classdesc::string&, const T& a)
  {
    b << uint64(a.size());
    for (typename T::const_iterator i=a.begin(); i!=a.end(); ++i)
      b << *i;
  }
#if defined(__GNUC__) && !defined(__ICC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

  template <class T>
  void unpackSequence(classdesc::pack_t& b, const classdesc::string& d, T& a)
  {
    uint64 sz=0, i=0;
    b >> sz;
    resize(a,0);
    for (typename T::iterator j=a.begin(); i<sz; ++i, ++j)
      {
        typename T::value_type x;
        b >> x;
        classdesc::push_back(a, x);
      }
  }
 
#if defined(__cplusplus) && __cplusplus>=201103L
  template <class T, size_t N>
  void unpackSequence(classdesc::pack_t& b, const classdesc::string& d, std::array<T,N>& a)
  {
    uint64 sz=0, i=0;
    b >> sz;
    for (size_t i=0; i<sz && i<N; ++i)
      b >> a[i];
  }
#endif
  
  template <class T>
  typename enable_if<is_sequence<T>,void>::T
  unpack(classdesc::pack_t& b, const classdesc::string& d, T& a)
  {unpackSequence(b,d,a);}
  
  template <class T>
  typename enable_if<is_sequence<T>, void>::T
  unpack(classdesc::pack_t& b, const classdesc::string&, const T& a)
  {
    uint64 sz;
    b >> sz; 
    typename T::value_type x;
    for (typename T::size_type i=0; i<sz; ++i) b>>x;
  }

#if defined(__GNUC__) && !defined(__ICC)
#pragma GCC diagnostic pop
#endif

  template <class T>
  typename enable_if<is_associative_container<T>, void>::T
  unpack(classdesc::pack_t& b, const classdesc::string&, T& a)
  {
    uint64 sz;
    b >> sz;
    a.clear();
    for (typename T::size_type i=0; i<sz; ++i)
      {
        typename classdesc::Value_Type<T>::value_type e;
        b >> e;
        a.insert(e);
      }
  }

  template <class T>
  typename enable_if<is_associative_container<T>, void>::T
  unpack(classdesc::pack_t& b, const classdesc::string&, const T& a)
  {
    uint64 sz;
    b >> sz;
    for (typename T::size_type i=0; i<sz; ++i)
      {
        typename classdesc::Value_Type<T>::value_type e;
        b >> e;
      }
  }

} 
using classdesc::pack;
using classdesc::unpack;

namespace classdesc_access
{

  template <class T>
  struct access_pack<classdesc::Iterator<T> >
  {
    template <class U>
    void operator()(classdesc::pack_t& b, const classdesc::string& d, U& a)
    {
      typename T::size_type i=a.container? 
        std::distance(a.container->begin(),a.iter()): 0;
      b << i;
    }
  };

  template <class T>
  struct access_unpack<classdesc::Iterator<T> >
  {
    template <class U>
    void operator()(classdesc::pack_t& b, const classdesc::string& d, U& a)
    {
      typename T::size_type i;
      b >> i;
      if (a.container)
        {
          a=a.container->begin();
          std::advance(a.iter(),i);
        }
    }
  };

/*
   strings
*/

#ifdef _CLASSDESC
#pragma omit pack std::char_traits
#pragma omit unpack std::char_traits
#pragma omit pack std::basic_string
#pragma omit unpack std::basic_string
#pragma omit pack std::string
#pragma omit unpack std::string
#endif

  template <class cT, class t, class A> 
  struct access_pack<std::basic_string<cT,t,A> >
  {
    template <class U>
    void operator()(classdesc::pack_t& targ, const classdesc::string& desc, U& arg) 
    {
      targ<<classdesc::uint64(arg.size());
      targ.packraw((const char*)arg.data(), sizeof(cT)*arg.size());
    }
  };

  template <class cT, class t, class A>
  struct access_unpack<std::basic_string<cT,t,A> >
  {
    typedef std::basic_string<cT,t,A> string;
    void asg(const string& x, const std::vector<cT>& b) {}
    void asg(string& x, const std::vector<cT>& b) 
    {
      if (!b.empty())
        x=string(b.data(), b.size()-1);
    }

    template <class U>
    void operator()(classdesc::pack_t& targ, const classdesc::string& desc, U& arg) 
    {
      classdesc::uint64 size=0; targ>>size;
      std::vector<cT> buf(size+1); //ensure buf[0] exists
      targ.unpackraw(buf.data(),sizeof(cT)*size);
      asg(arg, buf);
    }
  };
}

/*
   auxilliary types 
*/

#if defined(__GNUC__) && __GNUC__ < 3
#include <pair.h> // pair is part of <utility> now
#else
#include <utility>
#endif
#ifdef _CLASSDESC
#pragma omit pack std::pair
#pragma omit unpack std::pair
#endif

namespace classdesc_access
{
  template <class A, class B> struct access_pack<std::pair<A,B> >
  {
    template <class U>
    void operator()(classdesc::pack_t& targ, const classdesc::string& desc, U& arg) 
    { 
      ::pack(targ,desc,arg.first);
      ::pack(targ,desc,arg.second);
    }
  };

  template <class A, class B> struct access_unpack<std::pair<A,B> >
  {
    template <class U>
    void operator()(classdesc::pack_t& targ, const classdesc::string& desc, U& arg) 
    {
      ::unpack(targ,desc,arg.first);
      ::unpack(targ,desc,arg.second);
    }
  };
}
#ifdef _CLASSDESC
#pragma omit pack std::plus
#pragma omit unpack std::plus
#pragma omit pack std::minus
#pragma omit unpack std::minus
#pragma omit pack std::multiplies
#pragma omit unpack std::multiplies
#pragma omit pack std::divides
#pragma omit unpack std::divides
#pragma omit pack std::modulus
#pragma omit unpack std::modulus
#pragma omit pack negate
#pragma omit unpack negate

#pragma omit pack equal_to
#pragma omit unpack equal_to
#pragma omit pack not_equal_to
#pragma omit unpack not_equal_to
#pragma omit pack less
#pragma omit unpack less
#pragma omit pack greater
#pragma omit unpack greater
#pragma omit pack less_equal
#pragma omit unpack less_equal
#pragma omit pack greater_equal
#pragma omit unpack greater_equal

#pragma omit pack logical_and
#pragma omit unpack logical_and
#pragma omit pack logical_or
#pragma omit unpack logical_or
#pragma omit pack logical_not
#pragma omit unpack logical_not

#pragma omit pack identity
#pragma omit unpack identity
#pragma omit pack project1st
#pragma omit unpack project1st
#pragma omit pack project2nd
#pragma omit unpack project2nd
#pragma omit pack select1st
#pragma omit unpack select1st
#pragma omit pack select2nd
#pragma omit unpack select2nd


#pragma omit pack binary_function
#pragma omit unpack binary_function
#pragma omit pack unary_function
#pragma omit unpack unary_function

#endif

#if defined(__cplusplus) && __cplusplus<201103L
// note - these types were deprecated in C++11 and removed in C++17.
template <class A1, class A2, class R> 
void pack(classdesc::pack_t& targ, const classdesc::string& desc, std::binary_function<A1,A2,R>& arg) {}

template <class A1, class A2, class R> 
void unpack(classdesc::pack_t& targ, const classdesc::string& desc, std::binary_function<A1,A2,R>& arg) {}


template <class A, class R> 
void pack(classdesc::pack_t& targ, const classdesc::string& desc, std::unary_function<A,R>& arg) {}

template <class A, class R> 
void unpack(classdesc::pack_t& targ, const classdesc::string& desc, std::unary_function<A,R>& arg) {}
#endif

template <class C> typename
classdesc::enable_if<classdesc::is_container<C> >::T
pack(classdesc::pack_t& b, const classdesc::string& d, typename C::iterator& a) 
{}
template <class C> typename
classdesc::enable_if<classdesc::is_container<C> >::T
unpack(classdesc::pack_t& b, const classdesc::string& d, typename C::iterator& a) 
{}

#endif /* PACK_STL_H */
