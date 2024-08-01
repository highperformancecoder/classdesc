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
    SimpleBuffer(): Super(json_pack_t()) {}
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
    explicit SimpleBuffer(const json_pack_t& x) {operator=(x);}
    SimpleBuffer& operator=(const json_pack_t& x) {
      switch (x.type())
        {
        case RESTProcessType::boolean:
          Super::operator=(x.get_bool());
          break;
        case RESTProcessType::int_number:
          Super::operator=(x.get_int());
          break;
        case RESTProcessType::float_number:
          Super::operator=(x.get_real());
          break;
        case RESTProcessType::string:
          Super::operator=(x.get_str());
          break;
        case RESTProcessType::array:
          {
            auto& arr=x.get_array();
            std::deque<json_pack_t> v(arr.begin(),arr.end());
            Super::operator=(v);
          }
          break;
        default:
          Super::operator=(x);
        }
      return *this;
    }
    explicit SimpleBuffer(const std::deque<json_pack_t>& x): Super(x) {}
    template <class T> explicit SimpleBuffer(const T& x) {
      json_pack_t j;
      j<<x;
      operator=(j);
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

  const SimpleBuffer& operator>>(const SimpleBuffer& b, bool& x)
  {x=boost::get<bool>(b); return b;}

  SimpleBuffer& operator<<(SimpleBuffer& b, const bool& x)
  {b=x; return b;}

  const SimpleBuffer& operator>>(const SimpleBuffer& b, char& x)
  {x=boost::get<string>(b)[0]; return b;}

  SimpleBuffer& operator<<(SimpleBuffer& b, const char& x)
  {b=string{x}; return b;}

  // numbers
  template <class T>
  typename enable_if<
    And<is_arithmetic<T>,Not<is_same<T,bool>>,Not<is_const<T>>>,
    const SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {
    switch (b.type())
      {
      case RESTProcessType::float_number:
        x=boost::get<double>(b);
        break;
      case RESTProcessType::int_number:
        x=boost::get<int>(b);
        break;
      case RESTProcessType::object:
        boost::get<json_pack_t>(b)>>x;
        break;
      default:
        throw std::runtime_error("Invalid variant type");
      }
    return b;
  }

  template <class T>
  typename enable_if<
    And<is_arithmetic<T>,Not<is_same<T,bool>>>,
    SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {b=double(x); return b;}

  // strings
  template <class T>
  typename enable_if<And<is_string<T>,Not<is_const<T>>>, const SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {x=boost::get<string>(b); return b;}

  template <class T>
  typename enable_if<And<is_string<T>>, SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {b=string(x); return b;}

  // enums
  template <class T>
  typename enable_if<And<is_enum<T>,Not<is_const<T>>>, const SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {auto tmp=boost::get<string>(b); x=enum_keys<T>()(tmp); return b;}

  template <class T>
  typename enable_if<And<is_enum<T>>, SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {b=to_string(x); return b;}

  // sequences
  template <class T>
  typename enable_if<And<is_sequence<T>,Not<is_const<T>>>, const SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {
    resize(x,0);
    if (b.type()!=RESTProcessType::array) return b;
    auto& arr=b.array();
    for (auto& i: arr)
      {
        typename T::value_type e;
        i>>e;
        push_back(x,e);
      }
    return b;
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
    return b;
  }

  // const
  template <class T>
  typename enable_if<is_const<T>, const SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {return b;}


  const SimpleBuffer& operator>>(const SimpleBuffer& b, const char* x)
  {
    throw std::runtime_error("cannot unpack to char*, please use string instead");
  }
  
  // everything else
  template <class T>
  typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_string<T>>,
      Not<is_sequence<T>>,
      Not<is_enum<T>>,
      Not<is_const<T>>
      >, const SimpleBuffer&>::T
  operator>>(const SimpleBuffer& b, T& x)
  {
    boost::get<json_pack_t>(b)>>x;
    return b;
  }

  template <class T>
  typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_string<T>>,
      Not<is_sequence<T>>,
      Not<is_enum<T>>
      >, SimpleBuffer&>::T
  operator<<(SimpleBuffer& b, const T& x)
  {
    json_pack_t tmp; tmp<<x;
    b=tmp;
    return b;
  }
}

namespace classdesc_access
{
  namespace cd=classdesc;
  template <>
  struct access_json_unpack<cd::SimpleBuffer>
  {
  public:
    void operator()(cd::json_unpack_t& b, const cd::string& d, cd::SimpleBuffer& a)
    {a=b;}
  };
#endif
