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
    /// perform the REST operation, with \a remainder being the query string and \a arguments as body text
    virtual json_pack_t process(const string& remainder, const json_pack_t& arguments)=0;
    /// return signature(s) of the operations
    virtual json_pack_t signature() const=0;
    /// return list of subcommands to this
    virtual json_pack_t list() const=0;
    /// return type name of this
    virtual json_pack_t type() const=0;
    /// return signature for a function type F
    template <class F> json_pack_t functionSignature() const;
    /// returns a pointer to the underlying object if it is one of type T, otherwise null
    template <class T> T* getObject();
    /// true if this is an object, not a function
    virtual bool isObject() const {return false;}
    /// true if this is a const object, a const member function or static/free function
    virtual bool isConst() const {return false;}
    /// arity if this is a function, 0 otherwise
    virtual unsigned arity() const {return 0;}
  };

  /// marker for containers and pointers that wrap
  class RESTProcessWrapperBase: public RESTProcessBase {};
  
  // used to mark function types that can be overloaded 
  class RESTProcessFunctionBase: public RESTProcessBase
  {
  public:
    // match score if argument match impossible
    static const unsigned maxMatchScore=1000000;

    virtual ~RESTProcessFunctionBase() {}
    /// returns how good the match is with arguments, less is best
    virtual unsigned matchScore(const json_pack_t& arguments) const=0;
  };

  inline void convert(char& y, const string& x)
  {
    if (x.size()!=1)
      throw std::runtime_error("can only assign a single character string to a character variable");
    y=x[0];
  }
    
  template <class X, class Y>
  typename enable_if<And<is_convertible<X,Y>,Not<is_const<Y>>>, void>::T
  convert(Y& y, const X& x)
  {y=x;}

  template <class X, class Y>
  typename enable_if<And<And<Not<is_convertible<X,Y>>,Not<is_const<Y>>>,Not<is_enum<Y>>>, void>::T
  convert(Y& y, const X& x)
  {throw std::runtime_error(typeName<X>()+" cannot be converted to "+typeName<Y>());}

  template <class X, class Y>
  void convert(const Y&, const X&) 
  {throw std::runtime_error("attempt to alter a const variable");}
  
  template <class X>
  typename enable_if<And<And<Not<is_sequence<X>>,Not<is_const<X>>>,Not<is_enum<X>>>, void>::T
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
  void convert(std::shared_ptr<X>& x, const json_pack_t& j)
  {
    if (x) convert(*x,j);
  }

  template <class X>
  void convert(std::weak_ptr<X>& x, const json_pack_t& j)
  {
    if (auto s=x.lock()) convert(*s,j);
  }
  
  template <class E>
  typename enable_if<is_enum<E>,void>::T
  convert(E& x, const json_pack_t& j)
  {
    x=enum_keys<E>()(j.get_str());
  }
  
  template <class X>
  void convert(const X* x, const json_pack_t& j)
  {}

  template <class F, int N> struct DefineFunctionArgTypes;

  /// REST processor registry 
  struct RESTProcess_t: public std::multimap<std::string, std::unique_ptr<RESTProcessBase> >
  {
    /// ownership of \a rp is passed
    void add(string d, RESTProcessBase* rp)
    {
      std::replace(d.begin(),d.end(),'.','/');
      // for objects, ensure any previous entries of this key are deleted
      erase(d);
      emplace(d,std::unique_ptr<RESTProcessBase>(rp));
    }
    /// ownership of \a rp is passed
    void add(string d, RESTProcessFunctionBase* rp)
    {
      std::replace(d.begin(),d.end(),'.','/');
      // for overloadable functions, allow multiple entries for this key
      emplace(d,std::unique_ptr<RESTProcessBase>(rp));
    }

    json_pack_t process(const std::string& query, const json_pack_t& jin)
    {
      if (query[0]!='/') return {};
      string cmd=query;

      if (cmd=="/@enum/@list")
        {
          json_spirit::mArray r;
          for (auto& i: *this)
            if (i.first.find("/@enum")==0)
              r.push_back(i.first);
          return json_spirit::mValue(r);
        }
      
      for (auto cmdEnd=query.length(); ;
           cmdEnd=cmd.rfind('/'), cmd=cmd.substr(0,cmdEnd))
        {
          if (cmdEnd==string::npos)
            {
              cmdEnd=0;
              cmd="";
            }
          auto tail=query.substr(cmdEnd);
          switch (count(cmd))
            {
            case 0:
              if (cmdEnd)
                continue; // try next split
              else
                throw std::runtime_error("Command not found: "+query);
            case 1: // simple object or non overloaded function
              {
                auto r=find(cmd);
                if (tail=="/@signature")
                  return r->second->signature();
                else if (tail=="/@list")
                  return r->second->list();
                else if (tail=="/@type")
                  return r->second->type();
                else if (cmdEnd || dynamic_cast<RESTProcessWrapperBase*>(r->second.get()))
                  return r->second->process(tail, jin);
                else
                  throw std::runtime_error("Command not found: "+query);
              }
            default:
              {
                auto r=equal_range(cmd);
                if (tail=="/@signature")
                  {
                    json_spirit::mArray array;
                    for (; r.first!=r.second; ++r.first)
                      array.push_back(r.first->second->signature());
                    return json_pack_t(array);
                  }
                else if (tail=="/@list")
                  return json_pack_t(json_spirit::mArray());
                else if (tail=="/@type")
                  return json_pack_t("overloaded function");
                else
                  {
                    // sort function overloads by best match
                    auto cmp=[&](RESTProcessFunctionBase*x, RESTProcessFunctionBase*y)
                             {return x->matchScore(jin)<y->matchScore(jin);};
                    std::set<RESTProcessFunctionBase*, decltype(cmp)> sortedOverloads{cmp};
                    for (auto i=r.first; i!=r.second; ++i)
                      if (auto j=dynamic_cast<RESTProcessFunctionBase*>(i->second.get()))
                        sortedOverloads.insert(j);
                    auto& bestOverload=*sortedOverloads.begin();
                    if (bestOverload->matchScore(jin) >=
                        RESTProcessFunctionBase::maxMatchScore)
                      throw std::runtime_error("No suitable matching overload found");
                    if (sortedOverloads.size()>1)
                      { // ambiguous overload detection
                        auto i=sortedOverloads.begin(); i++;
                        if ((*i)->matchScore(jin)==bestOverload->matchScore(jin))
                          throw std::runtime_error("Ambiguous resolution of overloaded function");
                      }
                    return bestOverload->process(tail, jin);
                  }
                }
            }
        }
    }

    /// define all arguments of \a F
    template <class F> void defineFunctionArgTypes()
    {
      DefineFunctionArgTypes<F,functional::Arity<F>::value>::define(*this);
    }
  };

  
 template <class T>
  inline json_pack_t mapAndProcess(const string& query, const json_pack_t& arguments, T& a)
  {
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
  template <class T> struct RESTProcessObject: public RESTProcessBase
  {
    T& obj;
    RESTProcessObject(T& obj): obj(obj) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_pack_t r;
      if (remainder.empty())
        {
          if (!arguments.is_null())
            convert(obj, arguments);
          return r<<obj;
        }
      return mapAndProcess(remainder, arguments, obj);
    }
    json_pack_t signature() const override;
    json_pack_t list() const override {
      RESTProcess_t map;
      RESTProcess(map,"",obj);
      json_spirit::mArray array;
      for (auto& i:map)
        if (!i.first.empty())
          array.emplace_back(i.first);
      return json_pack_t(array);
    }
    json_pack_t type() const override {return json_pack_t(typeName<T>());}
    bool isObject() const override {return true;}
    bool isConst() const override {return std::is_const<T>::value;}
  };

  template <class T> T* RESTProcessBase::getObject()
  {
    if (auto p=dynamic_cast<RESTProcessObject<T>*>(this))
      return &p->obj;
    else
      return nullptr;
  }

  template <class T>
  struct is_classdescGenerated:
    public And<
    And<
      is_class<T>,
      Not<is_container<T>>
      >,
    Not<is_smart_ptr<T>>
    > {};
  
  /// descriptor for generating building REST processing registry
  template <class T>
  typename enable_if<Not<functional::is_nonmember_function_ptr<T>>, void>::T
  RESTProcess(RESTProcess_t& r, const string& d, T& a) {RESTProcessp(r,d,a);}

  
  template <class T>
  void RESTProcessp(RESTProcess_t& repo, const string& d, Exclude<T>& a)
  {}

  template <class T>
  typename enable_if<is_fundamental<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.add(d, new RESTProcessObject<T>(a));}

  template <class T>
  void RESTProcessp(RESTProcess_t& repo, const string& d, T* a)
  {/* pointers ignored */}

  template <class T>
  void RESTProcess(RESTProcess_t& repo, const string& d, is_const_static, T& a)
  {RESTProcess(repo,d,a);}

  template <class T>
  void RESTProcess(RESTProcess_t& r, const string& d, is_const_static, T* a)
  {RESTProcess(r,d,*a);}
    
  inline bool startsWith(const std::string& x, const std::string& prefix)
  {return x.size()>=prefix.size() && equal(prefix.begin(), prefix.end(), x.begin());}

  // sequences
  template <class T> class RESTProcessSequence: public RESTProcessWrapperBase
  {
    T& obj;

    template <class U>
    struct Insertable: public
    And<
      And<
      has_member_push_back<T,void (T::*)(const typename T::value_type&)>,
        is_default_constructible<typename T::value_type>>,
      Not<is_const<U>>> {};
    
    template <class U>
    typename enable_if<Insertable<U>, void>::T
    insert(U& o, const json_pack_t& j) {
      typename U::value_type v;
      j>>v;
      o.push_back(v);
    }

    template <class U>
    typename enable_if<Not<Insertable<U> >, void>::T insert(U&, const json_pack_t&)
    {
      throw std::runtime_error("cannot insert into this sequence");
    }

    template <class U>
    struct Erasable: public
    And<
      Or<
        has_member_erase<T,typename T::iterator (T::*)(typename T::iterator)>,
        has_member_erase<T,typename T::iterator (T::*)(typename T::const_iterator)>
        >,
      Not<is_const<U>>> {};

    
    template <class U>
    typename enable_if<Erasable<U>,void>::T
    erase(U& seq, const json_pack_t& j)
    {
      size_t idx; j>>idx;
      if (idx<seq.size())
        {
          auto i=seq.begin();
          std::advance(i, idx);
          seq.erase(i);
        }
    }
    
    template <class U>
    typename enable_if<Not<Erasable<U>>,void>::T
    erase(U& seq, const json_pack_t& j)
    {
      throw std::runtime_error("cannot erase from this sequence");
    }
    
  public:
    RESTProcessSequence(T& obj): obj(obj) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_pack_t r;
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
      else if (startsWith(remainder,"/@insert"))
        insert(obj, arguments);
      else if (startsWith(remainder,"/@erase"))
        erase(obj, arguments);
      else if (startsWith(remainder,"/@size"))
        return r<<obj.size();
      return r<<obj;
    }
    json_pack_t signature() const override;
    json_pack_t list() const override {
      json_spirit::mArray array{"/@elem","/@insert","/@erase","/@size"};
      return json_pack_t(array);
    }
    json_pack_t type() const override {return json_pack_t(typeName<T>());}
  };

  template <class T>
  typename enable_if<is_sequence<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.add(d, new RESTProcessSequence<T>(a));}

  /// used for removing const attributes of an associative container's value_type
  template <class T>
  struct MutableValueType
  {
    typedef typename std::remove_const<T>::type type;
  };
  
  template <class K, class V>
  struct MutableValueType<std::pair<const K, V> >
  {
    typedef std::pair<K, V> type;
  };
  
  /// insert element into an associative container
  template <class T> 
  void RPAC_insert(T& obj, const json_pack_t& arguments)
  {
    typename MutableValueType<typename T::value_type>::type v;
    arguments>>v;
    if (!obj.insert(v).second)
      throw std::runtime_error("key already exists, not inserted");
  }

  /// insert element into map
  template <class T>
  void RPAC_insert(const T&, const json_pack_t& argument)
  {
    throw std::runtime_error("cannot insert data into a constant container");
  }

  template <class T> 
  void RPAC_erase(T& obj, const json_pack_t& arguments)
  {
    typename T::key_type k;
    arguments>>k;
    obj.erase(k);
  }

  /// insert element into map
  template <class T>
  void RPAC_erase(const T&, const json_pack_t& argument)
  {
    throw std::runtime_error("cannot erase data from a constant container");
  }

  template <class T> class RESTProcessAssociativeContainer: public RESTProcessWrapperBase
  {
    T& obj;

    /// assign \a x if T is a map
    template <class K, class V, class C, class A>
    void assignIfMap(std::map<K,V,C,A>& m, const K& k, const json_pack_t& x)
    {
      V v;
      x>>v;
      m[k]=v;
    }
    template <class K, class V, class C, class A>
    void assignIfMap(std::unordered_map<K,V,C,A>& m, const K& k, const json_pack_t& x)
    {
      V v;
      x>>v;
      m[k]=v;
    }
    template <class U, class K> void assignIfMap(U&,const K&,const json_pack_t&) {}

    
  public:
    RESTProcessAssociativeContainer(T& obj): obj(obj) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_pack_t r;
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
              std::istringstream is(string(keyStart+1, keyEnd));
              is>>key;
              string tail(keyEnd,remainder.end());
              if (tail.empty() && arguments.type()!=json_spirit::null_type)
                assignIfMap(obj, key, arguments);
              auto i=obj.find(key);
              if (i==obj.end())
                throw std::runtime_error("key "+string(keyStart+1, keyEnd)+" not found");
              else if (tail.empty())
                return r<<*i;
              else
                return mapAndProcess(tail, arguments, *i);
            }
        }
      else if (startsWith(remainder,"/@insert"))
        {
          RPAC_insert(obj,arguments);
          return r;
        }
      else if (startsWith(remainder,"/@erase"))
        {
          RPAC_erase(obj,arguments);
          return r;
        }
      else if (startsWith(remainder,"/@size"))
        return r<<obj.size();
      else if (startsWith(remainder,"/@keys"))
        {
          std::vector<typename T::key_type> keys;
          for (auto& i: obj)
            keys.push_back(keyOf(i));
          return r<<keys;
        }
      return r<<obj;
    }
    json_pack_t signature() const override;
    json_pack_t list() const override {
      json_spirit::mArray array{"/@elem","/@insert","/@erase","/@size"};
      return json_pack_t(array);
    }
    json_pack_t type() const override {return json_pack_t(typeName<T>());}
  };

  template <class T>
  typename enable_if<is_associative_container<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {
    repo.add(d, new RESTProcessAssociativeContainer<T>(a));
  }
  
  template <class T>
  struct RESTProcessPtr: public RESTProcessWrapperBase
  {
    T& ptr;
    RESTProcessPtr(T& ptr): ptr(ptr) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override;
    json_pack_t signature() const override;
    json_pack_t list() const override {
      if (ptr)
        return const_cast<RESTProcessPtr<T>*>(this)->process("/@list",{});
      else
        return json_pack_t(json_spirit::mArray());
    }
    json_pack_t type() const override {return json_pack_t(typeName<T>());}
  };

  template <class T>
  struct RESTProcessPtr<std::weak_ptr<T>>: public RESTProcessWrapperBase
  {
    std::weak_ptr<T>& ptr;
    RESTProcessPtr(std::weak_ptr<T>& ptr): ptr(ptr) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override;
    json_pack_t signature() const override;
    json_pack_t list() const override {
      if (auto p=ptr.lock())
        return RESTProcessObject<T>(*p).list();
      else return json_pack_t(json_spirit::mArray());
    }
    json_pack_t type() const override {return json_spirit::mValue(typeName<std::weak_ptr<T> >());}
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
    JSONBuffer(const JSONBuffer& x): values(x.values), it(values.begin()+(x.it-x.values.begin())) {}
    JSONBuffer& operator=(const JSONBuffer& x) {
      values=x.values;
      it=values.begin()+(x.it-x.values.begin());
      return *this;
    }
    template <class T>
    JSONBuffer& operator>>(T& x) {
      if (it<values.end()) (*it++) >> x;
      return *this;
    }
    template <class T>
    JSONBuffer& operator>>(const T& x) {
      if (it<values.end()) ++it;
      return *this;
    }
  };

  template <class F>
  typename enable_if<
    And<functional::AllArgs<F, functional::ArgAcceptable>,
        Not<is_void<typename functional::Return<F>::T>>>,
    json_pack_t>::T
  callFunction(const string& remainder, const json_pack_t& arguments, F f)
  {
    functional::PackFunctor<JSONBuffer> argBuf(arguments);
    typename functional::Return<F>::T r=argBuf.call(f);
    if (remainder.empty())
      {
        // if there are arguments left over, assign the first of the
        // remaining arguments to the result
        switch (arguments.type())
          {
          case json_spirit::null_type: break;
          case json_spirit::array_type:
            {
              auto& arr=arguments.get_array();
              if (arr.size()>functional::Arity<F>::value)
                convert(r,json_pack_t(arr[functional::Arity<F>::value]));
              break;
            }
          default:
            if (functional::Arity<F>::value==0)
                convert(r,arguments);
            break;
          }
        json_pack_t rj;
        return rj<<r;
      }
    RESTProcess_t map;
    RESTProcess(map,"",r);
    return map.process(remainder, arguments);
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
  callFunction(const string& remainder, const json_pack_t& arguments, F f)
  {throw std::runtime_error("cannot call this function");}
  
  /// @{
  /// return whether \a arg matches a C++ type T for a function call argument
  //template <class T> bool matches(const json_spirit::mValue& arg);
  
  template <class T>
  typename enable_if<And<is_pointer<T>, Not<is_same<T,const char*> > >,bool>::T
  matches(const json_spirit::mValue& x)
  {return false;}

  template <class T>
  typename enable_if<is_same<T,bool>,bool>::T
  matches(const json_spirit::mValue& x)
  {return x.type()==json_spirit::bool_type;}

  template <class T>
  typename enable_if<is_same<T,string>,bool>::T matches(const json_spirit::mValue& x)
  {return x.type()==json_spirit::str_type;}

  template <class T>
  typename enable_if<is_same<T,const char*>, bool>::T
  matches(const json_spirit::mValue& x)
  {return x.type()==json_spirit::str_type;}

  template <class T>
  typename enable_if<And<is_integral<T>,Not<is_same<T,bool>>>, bool>::T matches(const json_spirit::mValue& x)
  {return x.type()==json_spirit::int_type;}

  template <class T>
  typename enable_if<is_floating_point<T>, bool>::T matches(const json_spirit::mValue& x)
  {return x.type()==json_spirit::real_type;}
  
  template <class T>
  typename enable_if<is_enum<T>, bool>::T matches(const json_spirit::mValue& x)
  {return x.type()==json_spirit::str_type;}
  
  template <class T>
  typename enable_if<
    And<
      And<
        And<is_class<T>, is_default_constructible<T> >,
        Not<is_same<T,string>>
        >,
      Not<is_container<T>>
      >, bool>::T
  matches(const json_spirit::mValue& x)
  {
    if (x.type()!=json_spirit::obj_type) return false;
    try // to convert the json object to a T
      {
        T test;
        x>>test;
      }
    catch(const std::exception&)
      {return false;}
    return true;
  }

  template <class T>
  typename enable_if<is_container<T>, bool>::T matches(const json_spirit::mValue& x)
  {
    if (x.type()==json_spirit::array_type)
      {
        auto& arr=x.get_array();
        bool r=true;
        for (auto& i: arr) r &= matches<typename T::value_type>(i);
        return r;
      }
    return matches<typename T::value_type>(x); // treat a single json object as a single element sequence
  }
  
  template <class T> struct remove_const_ref
  {
    using type=typename remove_const<typename remove_reference<T>::type>::type;
  };
  
  template <class T>
  typename enable_if<is_abstract<T>, bool>::T
  matches(const json_spirit::mValue&) {return false;}

  template <class T>
  typename enable_if<
    And<is_const<typename remove_reference<T>::type>, is_reference<T>>, bool>::T
  matches(const json_spirit::mValue& x)
  {return matches<typename remove_const_ref<T>::type>(x);}

  template <class T>
  typename enable_if<
    And<
      is_object<T>,
      Not<is_default_constructible<typename remove_reference<T>::type>>
      >, bool>::T
  matches(const json_spirit::mValue& x)
  {return false;}

  template <class T>
  struct isNoMatch
  {
    static const bool value = !is_integral<T>::value && !is_floating_point<T>::value &&
      !is_container<T>::value && !is_object<T>::value && !is_pointer<T>::value;
  };

  template <class T>
  typename enable_if<And<Not<is_reference<T>>,isNoMatch<typename remove_const<T>::type>>, bool>::T
  matches(const json_spirit::mValue&) {return false;}

  template <class T>
  typename enable_if<And<is_reference<T>, Not<is_const<typename remove_reference<T>::type>>>, bool>::T
  matches(const json_spirit::mValue&) {return false;}

  /// @{ testing for not quite so good matches between json type and C++ type
  //template <class T> bool partiallyMatchable(const json_spirit::mValue& x);

  template <class T>
  typename enable_if<is_floating_point<T>, bool>::T partiallyMatchable(const json_spirit::mValue& x)
  {return x.type()==json_spirit::int_type||x.type()==json_spirit::real_type;}

  template <class T>
  typename enable_if<is_container<T>, bool>::T partiallyMatchable(const json_spirit::mValue& x)
  {
    if (x.type()==json_spirit::array_type)
      {
        auto& arr=x.get_array();
        bool r;
        for (auto& i: arr) r &= partiallyMatchable<typename T::value_type>(i);
        return r;
      }
    return partiallyMatchable<typename T::value_type>(x); // treat a single json object as a single element sequence
  }

  template <class T>
  typename enable_if<And<Not<is_floating_point<T> >, Not<is_container<T> > >, bool>::T
                       partiallyMatchable(const json_spirit::mValue& x)
  {return matches<T>(x);}


  template <class T> unsigned argMatchScore(const json_spirit::mValue& x)
  {
    if (matches<T>(x)) return 0;
    if (partiallyMatchable<T>(x)) return 1;
    return RESTProcessFunctionBase::maxMatchScore;
  }
  
  
  template <class F, int N>
  struct MatchScore
  {
    static unsigned score(const json_spirit::mValue& x)
    {
      if (x.type()!=json_spirit::array_type) return RESTProcessFunctionBase::maxMatchScore;
      auto& arr=x.get_array();
      if (arr.size()<N) return RESTProcessFunctionBase::maxMatchScore;
      return  argMatchScore<typename functional::Arg<F,N>::T>(arr[N-1]) +
        MatchScore<F,N-1>::score(x);
    }
  };
  
  template <class F>
  struct MatchScore<F,2>
  {
    static unsigned score(const json_spirit::mValue& x)
    {
      if (x.type()!=json_spirit::array_type) return RESTProcessFunctionBase::maxMatchScore;
      auto& arr=x.get_array();
      if (arr.size()<2) return RESTProcessFunctionBase::maxMatchScore;
      return argMatchScore<typename functional::Arg<F,1>::T>(arr[0]) +
        argMatchScore<typename functional::Arg<F,2>::T>(arr[1])+
        10*(arr.size()-1); // penalize for supplying more arguments than needed
    }
  };

  template <class F>
  struct MatchScore<F,1>
  {
    static unsigned score(const json_spirit::mValue& x)
    {
      switch (x.type())
        {
        case json_spirit::null_type:
          return RESTProcessFunctionBase::maxMatchScore;
        case json_spirit::array_type:
          {
            auto& arr=x.get_array();
            if (arr.empty()) return RESTProcessFunctionBase::maxMatchScore;
            return argMatchScore<typename functional::Arg<F,1>::T>(arr[0])+
              10*(arr.size()-1); // penalize for supplying more arguments than needed
          }
        default:
          return argMatchScore<typename functional::Arg<F,1>::T>(x);
        }
    }
  };
  
  template <class F>
  struct MatchScore<F,0>
  {
    static unsigned score(const json_spirit::mValue& x)
    {
      switch (x.type())
        {
        case json_spirit::null_type:
          return 0;
        case json_spirit::array_type:
          {
            auto& arr=x.get_array();
            return 10*arr.size(); // penalize for supplying more arguments than needed
          }
        default:
          return 10; // penalize for supplying more arguments than needed
        }
    }
  };
  
  template <class F, int N=functional::Arity<F>::value>
  unsigned matchScore(const json_spirit::mValue& x)
  {return MatchScore<F,N>::score(x);}

  // static and free functions are const
  template <class F> struct FunctionalIsConst {static const bool value=true;};
  // only const bounrd methods are const
  template <class O, class M, class R>
  struct FunctionalIsConst<classdesc::functional::bound_method<O,M,R>>
  {static const bool value=functional::bound_method<O,M,R>::is_const;};
  
  // member functions
  template <class F, class R=typename functional::Return<F>::T>
  class RESTProcessFunction: public RESTProcessFunctionBase
  {
    F f;
  public:
    RESTProcessFunction(F f): f(f) {}
    

    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {return callFunction(remainder, arguments, f);}
    
    json_pack_t signature() const override {return functionSignature<F>();}
    unsigned matchScore(const json_pack_t& arguments) const override
    {return classdesc::matchScore<F>(arguments);}
    json_pack_t list() const override {return json_pack_t(json_spirit::mArray());}
    json_pack_t type() const override {return json_pack_t("function");}
    bool isObject() const override {return false;}
    bool isConst() const override {return FunctionalIsConst<F>::value;}
    unsigned arity() const override {return functional::Arity<F>::value;}
  };

  template <class F, class R>
  class RESTProcessFunction<F, std::unique_ptr<R>>: public RESTProcessFunctionBase
  {
    F f;
  public:
    RESTProcessFunction(F f): f(f) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      throw std::runtime_error("currently unable to call functions returning unique_ptr");
    }
    json_pack_t signature() const override {return functionSignature<F>();}
    unsigned matchScore(const json_pack_t& arguments) const override
    {return classdesc::matchScore<F>(arguments);}
    json_pack_t list() const override {return json_pack_t(json_spirit::mArray());}
    json_pack_t type() const override {return json_spirit::mValue(typeName<F>());}
 };

  template <class T, class F>
  typename enable_if<functional::is_member_function_ptr<F>, void>::T
  RESTProcess(RESTProcess_t& repo, const string& d, T& obj, F f)
  {
    auto bm=functional::bindMethod(obj,f);
    repo.add(d, new RESTProcessFunction<decltype(bm)>(bm));
    repo.defineFunctionArgTypes<F>();
  }

  template <class F>
  typename enable_if<functional::is_nonmember_function_ptr<F>, void>::T
  RESTProcess(RESTProcess_t& repo, const string& d, F f)
  {
    repo.add(d, new RESTProcessFunction<F>(f));
    repo.defineFunctionArgTypes<F>();
  }

  template <>
  inline void RESTProcess(RESTProcess_t& repo, const string& d, const char*& a)
  {repo.add(d,new RESTProcessObject<const char*>(a));}

  template <class E>
  class RESTProcessEnum: public RESTProcessBase
  {
    E& e;
  public:
    RESTProcessEnum(E& e): e(e) {}
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_pack_t r;
      if (remainder=="@type")
        return r<<typeName<E>();
      else if (remainder=="@signature")
        return signature();
      else if (arguments.type()==json_spirit::str_type)
        convert(e, enum_keys<E>()(arguments.get_str()));
      return r<<enum_keys<E>()(e);
    }
    json_pack_t signature() const override;
    json_pack_t list() const override {return json_pack_t(json_spirit::mArray());}
    json_pack_t type() const override {return json_pack_t(typeName<E>());}
  };

  template <class E>
  class EnumerateEnumerators: public RESTProcessBase
  {
    json_pack_t process(const string& remainder, const json_pack_t& arguments) override
    {
      json_spirit::mArray r;
      auto& enumerators=enum_keys<E>();
      for (auto i=enumerators.sbegin(); i!=enumerators.send(); ++i)
        r.push_back(*i);
      return json_spirit::mValue(r);
    }
    json_pack_t signature() const override {return "{ret: \"vector<string>\", args: []}";}
    json_pack_t list() const override {return json_pack_t(json_spirit::mArray());}
    json_pack_t type() const override {return "function";}
  };

  /// @{ define type dependent information in repository
  template <class E>
  typename enable_if<is_enum<E>, void>::T
  defineType(RESTProcess_t& r)
  {r.add("/@enum/"+typeName<E>(), new EnumerateEnumerators<E>());}

  template <class T>
  typename enable_if<Not<is_enum<T>>, void>::T
  defineType(RESTProcess_t&)
  {/* for now, we don't do anything with regular types */}
  /// @}
  
  template <class F, int N> struct DefineFunctionArgTypes
  {
    static void define(RESTProcess_t& r)
    {
      defineType<typename functional::Arg<F,N>::type>(r);
      DefineFunctionArgTypes<F,N-1>::define(r);
    }
  };

  template <class F> struct DefineFunctionArgTypes<F,0>
  {
    static void define(RESTProcess_t& r)
    {
      defineType<typename functional::Return<F>::type>(r);
    }
  };
  
   template <class E>
  typename enable_if<is_enum<E>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, E& e)
  {
    repo.add(d, new RESTProcessEnum<E>(e));
    defineType<E>(repo);
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

  template <class T>
  void RESTProcess(RESTProcess_t& r, const string& d, is_constructor, T& a)  {}

  template <class T>
  typename enable_if<is_smart_ptr<T>, void>::T 
  RESTProcessp(RESTProcess_t& repo, const std::string& d, T& a)
  {repo.add(d, new RESTProcessPtr<T>(a));}
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
