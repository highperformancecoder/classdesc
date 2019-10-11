/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef RESTPROCESS_H
#define RESTPROCESS_H
/// A classdesc descriptor to generate virtual xrap processing calls
#include "function.h"
#include "multiArray.h"
#include "json_pack_base.h"

#include <map>
#include <stdexcept>

namespace classdesc
{
  /// interface for the REST processor
  class RESTProcessBase
  {
  public:
    virtual ~RESTProcessBase() {}
    virtual json_pack_t process(const string& remainder, const json_pack_t& arguments)=0;
    virtual json_pack_t signature() const=0;
    /// return signature for a function type F
    template <class F> json_pack_t functionSignature() const;
  };

  //  template <> inline string typeName<RESTProcessBase>() {return "RESTProcessBase";}
  
  template <class X, class Y>
  typename enable_if<And<is_convertible<X,Y>,Not<is_const<Y>>>, void>::T
  convert(Y& y, const X& x)
  {y=x;}

  template <class X, class Y>
  typename enable_if<And<Not<is_convertible<X,Y>>,Not<is_const<Y>>>, void>::T
  convert(Y& y, const X& x)
  {throw std::runtime_error(typeName<X>()+" cannot be converted to "+typeName<Y>());}

  template <class X, class Y>
  void convert(const Y&, const X&) 
  {throw std::runtime_error("attempt to alter a const variable");}
  
  template <class X>
  typename enable_if<And<Not<is_sequence<X>>,Not<is_const<X>>>, void>::T
  convert(X& x, const json_pack_t& j)
  {
      switch (j.type())
        {
        case json_spirit::obj_type:
          j>>x;
          break;
        case json_spirit::array_type:
          {
            auto& arr=j.get_array();
            if (arr.size()>0)
              arr[0]>>x;
          }
          break;
        case json_spirit::str_type:
          convert(x,j.get_str());
          break;
        case json_spirit::bool_type:
          convert(x,j.get_bool());
          break;
        case json_spirit::int_type:
          convert(x,j.get_int());
          break;
        case json_spirit::real_type:
          convert(x,j.get_real());
          break;
        case json_spirit::null_type:
          break;
        }
  }

  template <class X>
  typename enable_if<And<is_sequence<X>,Not<is_const<X>>>, void>::T
  convert(X& x, const json_pack_t& j)
  {
    if (j.type()==json_spirit::array_type)
      {
        auto& arr=j.get_array();
        resize(x, arr.size());
        auto xi=x.begin();
        for (auto& ai: arr)
          {
            if (xi==x.end()) break;
            ai >> *xi++;
          }
      }
  }
  
  template <class X>
  void convert(const X* x, const json_pack_t& j)
  {}

  /// REST processor registry 
  struct RESTProcess_t: public std::map<std::string, std::unique_ptr<RESTProcessBase> >
  {
    /// ownership of \a rp is passed
    void add(string d, RESTProcessBase* rp)
    {
      std::replace(d.begin(),d.end(),'.','/');
      auto& i=(*this)[d];
      i.reset(rp);
    }

    json_pack_t process(const std::string& query, const json_pack_t& jin)
    {
      if (query[0]!='/') return {};
      string cmd=query;
      
      for (auto cmdEnd=query.length(); cmdEnd>0;
           cmdEnd=cmd.rfind('/'), cmd=cmd.substr(0,cmdEnd))
        {
          auto r=find(cmd);
          if (r!=end())
            {
              auto tail=query.substr(cmdEnd);
              if (tail=="/@signature")
                return r->second->signature();
              else
                return r->second->process(tail, jin);
            }
        }
      throw std::runtime_error("Command not found");
    }
  };
  
  template <class T>
  inline json_pack_t mapAndProcess(const string& query, const json_pack_t& arguments, T& a)
  {
//    if (query.empty())
//      {
//        convert(a, arguments); // set object if some args provided
////        json_pack_t r;
////        return r<<a;
//      }

    RESTProcess_t map;
    RESTProcess(map,"",a);
    if (query.empty())
      {
        auto i=map.find("");
        if (i!=map.end())
          return i->second->process("",arguments);
        else
          return {};
      }
    return map.process(query,arguments);
  }
  
  /// handle setting and getting of objects
  template <class T> class RESTProcessObject: public RESTProcessBase
  {
    T& obj;
  public:
    RESTProcessObject(T& obj): obj(obj) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_pack_t r;
      if (remainder=="@type")
        return r<<typeName<T>();
      else if (remainder=="@signature")
        return signature();
      else if (remainder.empty())
        {
          convert(obj, arguments);
          return r<<obj;
        }
      return mapAndProcess(remainder, arguments, obj);
    }
    json_pack_t signature() const override;
  };

  template <class T>
  struct is_classdescGenerated:
    public And<is_class<T>, Not<is_container<T>>> {};
  
  /// descriptor for generating building REST processing registry
  template <class T>
  //typename enable_if<is_classdescGenerated<T>, void>::T
  void RESTProcess(RESTProcess_t& r, const string& d, T& a) {RESTProcessp(r,d,a);}

  
  template <class T>
  void RESTProcessp(RESTProcess_t& repo, const string& d, Exclude<T>& a)
  {}

  template <class T>
  typename enable_if<is_fundamental<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.add(d, new RESTProcessObject<T>(a));}

  template <class T>
  void RESTProcess(RESTProcess_t& repo, const string& d, is_const_static, T& a)
  {RESTProcess(repo,d,a);}

  template <class T>
  void RESTProcess(RESTProcess_t& r, const string& d, is_const_static, T* a)
  {RESTProcess(r,d,*a);}
    
  inline bool startsWith(const std::string& x, const std::string& prefix)
  {return x.size()>=prefix.size() && equal(prefix.begin(), prefix.end(), x.begin());}
  
  // sequences
  template <class T> class RESTProcessSequence: public RESTProcessBase
  {
    T& obj;
  public:
    RESTProcessSequence(T& obj): obj(obj) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_pack_t r;
      // TODO @elem selector in remainder
      if (remainder.empty())
        convert(obj, arguments);
      else if (startsWith(remainder,"/@elem"))
        {
          // extract idx
          auto idxStart=find(remainder.begin()+1, remainder.end(), '/');
          if (idxStart==remainder.end())
            throw std::runtime_error("no index");
          auto idxEnd=find(idxStart+1, remainder.end(), '/');
          size_t idx=stoi(string(idxStart+1, idxEnd));
          if (idx>=obj.size())
            throw std::runtime_error("idx out of bounds");
          auto i=obj.begin();
          std::advance(i, idx);
          return mapAndProcess(string(idxEnd,remainder.end()), arguments, *i);
        }
      return r<<obj;
    }
    json_pack_t signature() const override;
  };

  template <class T>
  typename enable_if<is_sequence<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.add(d, new RESTProcessSequence<T>(a));}

  template <class T> class RESTProcessAssociativeContainer: public RESTProcessBase
  {
    T& obj;
  public:
    RESTProcessAssociativeContainer(T& obj): obj(obj) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      if (remainder.empty())
        convert(obj, arguments);
      else if (startsWith(remainder,"/@elem"))
        {
          // extract key
          auto keyStart=find(remainder.begin()+1, remainder.end(), '/');
          if (keyStart!=remainder.end())
            {
              auto keyEnd=find(keyStart+1, remainder.end(), '/');
              typename T::key_type key;
              convert(key, string(keyStart+1, keyEnd));
              auto i=obj.find(key);
              if (i==obj.end())
                throw std::runtime_error("key "+string(keyStart+1, keyEnd)+" not found");
              else
                return mapAndProcess(string(keyEnd,remainder.end()), arguments, *i);
            }
        }
      json_pack_t r;
      return r<<obj;
    }
    json_pack_t signature() const override;
  };

  template <class T>
  typename enable_if<is_associative_container<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {
    repo.add(d, new RESTProcessAssociativeContainer<T>(a));
  }

  template <class T>
  struct RESTProcessPtr: public RESTProcessBase
  {
    T& ptr;
    RESTProcessPtr(T& ptr): ptr(ptr) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      if (ptr)
        if (remainder.empty())
          return RESTProcessObject<typename T::element_type>(*ptr).process(remainder, arguments);
        else
          return mapAndProcess(remainder,arguments,*ptr);
      else
        return {};
    }
    json_pack_t signature() const override;
  };

  template <class T>
  struct RESTProcessPtr<std::weak_ptr<T>>: public RESTProcessBase
  {
    std::weak_ptr<T>& ptr;
    RESTProcessPtr(std::weak_ptr<T>& ptr): ptr(ptr) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      if (auto p=ptr.lock())
        return RESTProcessObject<T>(*p).process(remainder, arguments);
      else
        return {};
    }
    json_pack_t signature() const override;
  };

  

  
  
  // buffer adaptor for a vector of json_pack_t objects 
  class JSONBuffer
  {
    std::vector<json_pack_t> values;
    std::vector<json_pack_t>::iterator it;
  public:
    JSONBuffer(const json_pack_t& j) {
      if (j.type()==json_spirit::array_type)
        for (auto& i: j.get_array())
          values.push_back(i);
      else
        values.push_back(j);
      it=values.begin();
    }
    template <class T>
    JSONBuffer& operator>>(T& x) {
      if (it!=values.end()) (*it++) >> x;
      return *this;
    }
    template <class T>
    JSONBuffer& operator>>(const T& x) {++it; return *this;}
  };

  template <class F>
  typename enable_if<
    And<functional::AllArgs<F, functional::ArgAcceptable>,
        Not<is_void<typename functional::Return<F>::T>>>,
    json_pack_t>::T
  callFunction(const string& remainder, const json_pack_t& arguments, F f)
  {
    functional::PackFunctor<JSONBuffer> argBuf(arguments);
    json_pack_t r;
    return r<<argBuf.call(f);
  }

  template <class F>
  typename enable_if<
    And<functional::AllArgs<F, functional::ArgAcceptable>,
        is_void<typename functional::Return<F>::T>>,
    json_pack_t>::T
  callFunction(const string& remainder, const json_pack_t& arguments, F f)
  {
    functional::PackFunctor<JSONBuffer> argBuf(arguments);
    argBuf.call(f);
    return {};
  }

  
  // don't do anything if we cannot create or copy an argument
  template <class F>
  typename enable_if<Not<functional::AllArgs<F, functional::ArgAcceptable>>, json_pack_t>::T
  callFunction(const string& remainder, const json_pack_t& arguments, F f) {return {};}

  // member functions
  template <class F, class R=typename functional::Return<F>::T>
  class RESTProcessFunction: public RESTProcessBase
  {
    F f;
  public:
    RESTProcessFunction(F f): f(f) {}
    

    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {return callFunction(remainder, arguments, f);}
    
    json_pack_t signature() const override {return functionSignature<F>();}
  };

  template <class F, class R>
  class RESTProcessFunction<F, std::unique_ptr<R>>: public RESTProcessBase
  {
    F f;
  public:
    RESTProcessFunction(F f): f(f) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      throw std::runtime_error("currently unable to call functions returning unique_ptr");
    }
    json_pack_t signature() const override {return functionSignature<F>();}
  };

 
  
//  template <class F>
//  class RESTProcessFunction<F,void>: public RESTProcessBase
//  {
//    F f;
//  public:
//    RESTProcessFunction(F f): f(f) {}
//    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
//    {
//      functional::PackFunctor<JSONBuffer> argBuf(arguments);
//      argBuf.call(f);
//      return {};
//    }
//    json_pack_t signature() const override {return functionSignature<F>();}
//  };

  template <class T, class F>
  typename enable_if<functional::is_member_function_ptr<F>, void>::T
  RESTProcess(RESTProcess_t& repo, const string& d, T& obj, F f)
  {
    auto bm=functional::bindMethod(obj,f);
    repo.add(d, new RESTProcessFunction<decltype(bm)>(bm));
  }

  template <class F>
  typename enable_if<functional::is_nonmember_function_ptr<F>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, F f)
  {
    repo.add(d, new RESTProcessFunction<F>(f));
  }

  template <>
  inline void RESTProcess(RESTProcess_t& repo, const string& d, const char*& a)
  {repo.add(d,new RESTProcessObject<const char*>(a));}

  template <class E>
  typename enable_if<is_enum<E>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, E& e)
  {
    // TODO
    //repo.add(d, new RESTProcessEnum<E>(e);
  }

  template <class T>
  void RESTProcess_onbase(RESTProcess_t& r, const string& d, T& a)
  {RESTProcess(r,d,a);}

  template <class T, class F>
  void RESTProcess(RESTProcess_t&, const string&, T&, is_constructor, F)
  {}

  
  template <class... Args> struct NumArgs;

  template <class A, class... Args>
  struct NumArgs<A,Args...>
  {
    static const size_t value=1+NumArgs<Args...>::value;
  };

  template <> struct NumArgs<>
  {
    static const size_t value=0;
  };

  template <class T, class... Args>
  struct RESTProcessMultiArray:
    public MultiArray<T,NumArgs<Args...>::value>,
    public RESTProcessSequence<MultiArray<T,NumArgs<Args...>::value>>
  {
    static const size_t rank=NumArgs<Args...>::value;
    typedef MultiArray<T,rank> MA;
    RESTProcessMultiArray(T* data, Args... dims):
      MA(data,dims...), RESTProcessSequence<MA>(static_cast<MA&>(*this)) {}
  };
  
  template <class T, class... Args>
  void RESTProcess(RESTProcess_t& r, const string& d, is_array, T& a, int dims, Args... args)
  {
    r.add(d, new RESTProcessMultiArray<T,Args...>(&a, args...));
  }
}

namespace classdesc_access
{
  template <class T> struct access_RESTProcess;
}

namespace std
{
  template <class T, size_t N>
  void advance(typename classdesc::MultiArray<T,N>::iterator& i, size_t n)
  {i+=n;}
}

#include "use_mbr_pointers.h"
CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(RESTProcess);

//#ifdef _CLASSDESC
//#pragma omit RESTProcess classdesc::MultiArray 
//#pragma omit json_pack classdesc::MultiArray 
//#pragma omit json_unpack classdesc::MultiArray 
//#endif

using classdesc::RESTProcess;       
using classdesc::RESTProcess_onbase;       

#include "RESTProcess_base.cd"
#endif
