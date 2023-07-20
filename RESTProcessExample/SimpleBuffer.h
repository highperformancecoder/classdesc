/*
  @copyright Russell Standish 2023
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/* A simple buffer implementation of the implied interface of this concept.
   classdesc::json_pack_t also implements this interface
*/

#ifndef CLASSDESC_SIMPLE_BUFFER_H
#define CLASSDESC_SIMPLE_BUFFER_H

#include <json_pack_base.h>
#include <boost/variant.hpp>
#include <deque>

namespace classdesc
{

  struct TypeVisitor: public boost::static_visitor<RESTProcessType::Type>, public RESTProcessType
  {
    Type operator()(bool) const {return boolean;}
    Type operator()(int) const {return int_number;}
    Type operator()(double) const {return float_number;}
    Type operator()(classdesc::string) const {return string;}
    Type operator()(const json_pack_t& x) const {return x.is_null()? null: object;}
    Type operator()(std::deque<json_pack_t>) const {return array;}
  };
  
  struct ToJsonVisitor: public boost::static_visitor<json_pack_t>
  {
    json_pack_t operator()(bool x) const {return json_pack_t(x);}
    json_pack_t operator()(int x) const {return json_pack_t(x);}
    json_pack_t operator()(double x) const {return json_pack_t(x);}
    json_pack_t operator()(const string& x) const {return json_pack_t(x);}
    json_pack_t operator()(const json_pack_t& x) const {return x;}
    json_pack_t operator()(const std::deque<json_pack_t>& x) const {
      json5_parser::mArray r(x.begin(),x.end());
      return json_pack_t(r);
    }
  };

  using SuperVariant=boost::variant<bool,int,double,string,json_pack_t,std::deque<json_pack_t>>;
  
  struct SimpleBuffer: public SuperVariant
  {
    using Super=SuperVariant;
    using Array=std::deque<json_pack_t>;
    SimpleBuffer() {}
    explicit SimpleBuffer(RESTProcessType::Type type) {
      switch (type)
        {
        case RESTProcessType::boolean: Super::operator=(false); return;
        case RESTProcessType::int_number: Super::operator=(0); return;
        case RESTProcessType::float_number: Super::operator=(0.0); return;
        case RESTProcessType::string: Super::operator=(""); return;
        case RESTProcessType::object: Super::operator=(json_pack_t()); return;
        case RESTProcessType::array: Super::operator=(std::deque<json_pack_t>()); return;
        }
    }
    explicit SimpleBuffer(bool x): Super(x) {}
    explicit SimpleBuffer(double x): Super(x) {}
    explicit SimpleBuffer(const string& x): Super(x) {}
    explicit SimpleBuffer(const json_pack_t& x): Super(x) {}
    explicit SimpleBuffer(const std::deque<json_pack_t>& x): Super(x) {}
    template <class T> explicit SimpleBuffer(const T& x) {
      json_pack_t j;
      j<<x;
      if (j.type()==RESTProcessType::array)
        {
          auto& arr=j.array();
          std::deque<json_pack_t> v(arr.begin(),arr.end());
          Super::operator=(v);
        }
      else
        Super::operator=(j);
    }

    template <class T>
    SimpleBuffer& operator=(const T& x) {Super::operator=(x); return *this;}
    
    /// return type conforms to the sequence concept
    const Array& array() const {
      return boost::get<Array>(*this);
    }
    RESTProcessType::Type type() const {
      return boost::apply_visitor(TypeVisitor(), *this);
    }
  };

   SimpleBuffer& operator>>(const SimpleBuffer& b, bool& x)
  {x=boost::get<bool>(b);}

  SimpleBuffer& operator<<(SimpleBuffer& b, const bool& x)
  {b=x;}

  // numbers
  template <class T>
  typename enable_if<
    And<is_arithmetic<T>,Not<is_same<T,bool>>,Not<is_const<T>>>,
    SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {x=boost::get<double>(b);}

  template <class T>
  typename enable_if<
    And<is_arithmetic<T>,Not<is_same<T,bool>>>,
    SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {b=double(x);}

  // strings
  template <class T>
  typename enable_if<And<is_string<T>,Not<is_const<T>>>, SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {x=boost::get<string>(b);}

  template <class T>
  typename enable_if<And<is_string<T>>, SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {b=string(x);}

  // sequences
  template <class T>
  typename enable_if<And<is_sequence<T>,Not<is_const<T>>>, SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {
    auto& arr=b.array();
    resize(x,0);
    for (auto& i: arr)
      {
        typename T::value_type e;
        i>>e;
        push_back(x,e);
      }
  }

  template <class T>
  typename enable_if<is_sequence<T>, SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {
    std::deque<json_pack_t> tmp;
    for (auto& i: x)
      {
        tmp.emplace_back();
        tmp.back()<<i;
      }
    b=SimpleBuffer(tmp);
  }

  // const
  template <class T>
  typename enable_if<is_const<T>, SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {}

  
  // everything else
  template <class T>
  typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_string<T>>,
      Not<is_sequence<T>>,
      Not<is_const<T>>
      >, SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {
    boost::get<json_pack_t>(b)>>x;
  }

  template <class T>
  typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_string<T>>,
      Not<is_sequence<T>>
      >, SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {
    json_pack_t tmp; tmp<<x;
    b=tmp;
  }
}
\

#endif
