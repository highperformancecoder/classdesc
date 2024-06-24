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
#include <map>
#include <stdexcept>

#ifndef REST_PROCESS_BUFFER
#define REST_PROCESS_BUFFER json_pack_t
#endif

namespace classdesc
{
  /// interface for the REST processor
  class RESTProcessBase
  {
  public:
    virtual ~RESTProcessBase() {}
    /// perform the REST operation, with \a remainder being the query string and \a arguments as body text
    virtual REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments)=0;
    /// return signature(s) of the operations
    virtual REST_PROCESS_BUFFER signature() const=0;
    /// return list of subcommands to this
    virtual REST_PROCESS_BUFFER list() const=0;
    /// return type name of this
    virtual REST_PROCESS_BUFFER type() const=0;
    /// return signature for a function type F
    template <class F> REST_PROCESS_BUFFER functionSignature() const;
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
    virtual unsigned matchScore(const REST_PROCESS_BUFFER& arguments) const=0;
  };

  inline void convert(char& y, const string& x)
  {
    if (x.size()!=1)
      throw std::runtime_error("can only assign a single character string to a character variable");
    y=x[0];
  }
  
  inline void convert(const char* y, const string& x)
  {
    y=x.c_str();
  }
   
  template <class X, class Y>
  typename enable_if<And<is_assignable<Y,X>,Not<is_const<Y>>>, void>::T
  convert(Y& y, const X& x)
  {y=x;}

  template <class X, class Y>
  typename enable_if<And<Not<is_assignable<Y,X>>, is_convertible<X,Y>, std::is_move_assignable<Y>, Not<is_const<Y>>>, void>::T
  convert(Y& y, const X& x)
  {y=std::move(Y(x));}

  template <class X, class Y>
  typename enable_if<And<
                       Not<is_assignable<Y,X>>,
                       Not<And<is_convertible<X,Y>, std::is_move_assignable<Y>>>,
                       Not<is_const<Y>>,
                       Not<is_enum<Y>>
                       >, void>::T
  convert(Y& y, const X& x)
  {throw std::runtime_error(typeName<X>()+" cannot be converted to "+typeName<Y>());}

  template <class X, class Y>
  void convert(const Y&, const X&) 
  {throw std::runtime_error("attempt to alter a const variable");}
  
  template <class X>
  typename enable_if<
    And<
      Not<is_sequence<X>>,
      Not<is_associative_container<X>>,
      Not<is_const<X>>,
      Not<is_enum<X>>
      >, void>::T
  convert(X& x, const REST_PROCESS_BUFFER& j)
  {
    switch (j.type())
      {
      case RESTProcessType::object:
        j>>x;
        break;
      case RESTProcessType::array:
        {
          auto arr=j.array();
          if (arr.size()>0)
            REST_PROCESS_BUFFER(arr[0])>>x;
        }
        break;
      case RESTProcessType::string:
        {
          string tmp;
          j>>tmp;
          convert(x,tmp);
        }
        break;
      case RESTProcessType::boolean:
        {
          bool tmp{};
          j>>tmp;
          convert(x,tmp);
        }
        break;
      case RESTProcessType::int_number:
        {
          int64_t tmp{};
          j>>tmp;
          convert(x,tmp);
        }
        break;
      case RESTProcessType::float_number:
        {
          double tmp{};
          j>>tmp;
          convert(x,tmp);
        }
        break;
      case RESTProcessType::null:
        break;
      }
  }

  template <class X>
  typename enable_if<And<is_sequence<X>,Not<is_const<X>>>, void>::T
  convert(X& x, const REST_PROCESS_BUFFER& j)
  {
    if (j.type()==RESTProcessType::array)
      {
        auto arr=j.array();
        resize(x, arr.size());
        auto xi=x.begin();
        for (auto& ai: arr)
          {
            if (xi==x.end()) break;
            REST_PROCESS_BUFFER(ai) >> *xi++;
          }
      }
  }
  
  template <class X>
  typename enable_if<And<is_associative_container<X>,Not<is_const<X>>>, void>::T
  convert(X& x, const REST_PROCESS_BUFFER& j)
  {
    switch (j.type())
      {
      case RESTProcessType::array:
        {
          auto arr=j.array();
          x.clear();
          for (auto& ai: arr)
            {
              typename X::value_type v;
              REST_PROCESS_BUFFER(ai) >> v;
              x.insert(v);
            }
        }
        break;
      case RESTProcessType::object: // special case for StringKeys 
        j>>x;
        break;
      default: break;
      }
  }

  template <class X>
  void convert(std::shared_ptr<X>& x, const REST_PROCESS_BUFFER& j)
  {
    if (x) convert(*x,j);
  }

  template <class X>
  void convert(std::weak_ptr<X>& x, const REST_PROCESS_BUFFER& j)
  {
    if (auto s=x.lock()) convert(*s,j);
  }
  
  template <class E>
  typename enable_if<is_enum<E>,void>::T
  convert(E& x, const REST_PROCESS_BUFFER& j)
  {
    string tmp; j>>tmp;
    x=enum_keys<E>()(tmp);
  }
  
  template <class X>
  void convert(const X* x, const REST_PROCESS_BUFFER& j)
  {}

  template <class F, int N> struct DefineFunctionArgTypes;

  /// REST processor registry 
  struct RESTProcess_t: public std::multimap<std::string, std::unique_ptr<RESTProcessBase> >
  {
    RESTProcess_t()=default;
    /// populate this map with the members of \a obj. T must be a
    /// classdescified type, otherwise this is equivalent to a default
    /// contructor
    template <class T> RESTProcess_t(T& obj);
    /// ownership of \a rp is passed
    void add(string d, RESTProcessBase* rp)
    {
      // for objects, ensure any previous entries of this key are deleted
      erase(d);
      emplace(d,std::unique_ptr<RESTProcessBase>(rp));
    }
    /// ownership of \a rp is passed
    void add(string d, RESTProcessFunctionBase* rp)
    {
      // for overloadable functions, allow multiple entries for this key
      emplace(d,std::unique_ptr<RESTProcessBase>(rp));
    }
  
    REST_PROCESS_BUFFER process(const std::string& query, const REST_PROCESS_BUFFER& jin);

      /// define all arguments of \a F
    template <class F> void defineFunctionArgTypes()
    {
      DefineFunctionArgTypes<F,functional::Arity<F>::value>::define(*this);
    }
  };

  
  template <class T>
  inline REST_PROCESS_BUFFER mapAndProcess(const string& query, const REST_PROCESS_BUFFER& arguments, T& a)
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

  template <class T>
  inline typename enable_if<is_default_constructible<T>,REST_PROCESS_BUFFER>::T
  mapAndProcessDummy(const string& query, const REST_PROCESS_BUFFER& arguments)
  {
    T dummy{};
    return mapAndProcess(query,arguments,dummy);
  }

  template <class T>
  inline typename enable_if<Not<is_default_constructible<T>>,REST_PROCESS_BUFFER>::T
  mapAndProcessDummy(const string& query, const REST_PROCESS_BUFFER& arguments)
  {
    throw std::runtime_error(typeName<T>()+" is not default constructible, but requested element doesn't exist");
  }

  /// handle setting and getting of objects
  template <class T> struct RESTProcessObject: public RESTProcessBase
  {
    T& obj;
    RESTProcessObject(T& obj): obj(obj) {}
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      REST_PROCESS_BUFFER r;
      if (remainder.empty())
        {
          switch (arguments.type())
            {
            case RESTProcessType::null: break;
            case RESTProcessType::array:
              {
                auto& arr=arguments.array();
                if (!arr.empty()) convert(obj, REST_PROCESS_BUFFER(arr[0]));
                break;
              }
            default:
              convert(obj, arguments);
              break;
            }
          return r<<obj;
        }
      return mapAndProcess(remainder, arguments, obj);
    }
    REST_PROCESS_BUFFER signature() const override;
    REST_PROCESS_BUFFER list() const override;
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<T>());}
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
    insert(U& o, const REST_PROCESS_BUFFER& j) {
      typename U::value_type v;
      convert(v,j);
      o.push_back(v);
    }

    template <class U>
    typename enable_if<Not<Insertable<U> >, void>::T insert(U&, const REST_PROCESS_BUFFER&)
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
    erase(U& seq, const REST_PROCESS_BUFFER& j)
    {
      size_t idx{}; convert(idx,j);
      if (idx<seq.size())
        {
          auto i=seq.begin();
          std::advance(i, idx);
          seq.erase(i);
        }
    }
    
    template <class U>
    typename enable_if<Not<Erasable<U>>,void>::T
    erase(U& seq, const REST_PROCESS_BUFFER& j)
    {
      throw std::runtime_error("cannot erase from this sequence");
    }
    
  public:
    RESTProcessSequence(T& obj): obj(obj) {}
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      REST_PROCESS_BUFFER r;
      if (remainder.empty())
          switch (arguments.type())
            {
            case RESTProcessType::null: break;
            case RESTProcessType::array:
              {
                auto& arr=arguments.array();
                if (!arr.empty()) convert(obj, REST_PROCESS_BUFFER(arr[0]));
                break;
              }
            default:
              convert(obj, arguments);
              break;
            }
      else if (startsWith(remainder,".@elem"))
        {
          // extract idx
          auto idxStart=find(remainder.begin()+1, remainder.end(), '.');
          if (idxStart==remainder.end())
            throw std::runtime_error("no index");
          auto idxEnd=find(idxStart+1, remainder.end(), '.');
          size_t idx=stoi(string(idxStart+1, idxEnd));
          if (idx>=obj.size())
            {
              if (startsWith(remainder,".@elemNoThrow"))
                return mapAndProcessDummy<typename T::value_type>(string(idxEnd,remainder.end()), arguments);
              else
                throw std::runtime_error("idx out of bounds");
            }
          auto i=obj.begin();
          std::advance(i, idx);
          return mapAndProcess(string(idxEnd,remainder.end()), arguments, *i);
        }
      else if (startsWith(remainder,".@insert"))
        insert(obj, arguments);
      else if (startsWith(remainder,".@erase"))
        erase(obj, arguments);
      else if (startsWith(remainder,".@size"))
        return r<<obj.size();
      else
        return RESTProcess_t(obj).process(remainder,arguments); // treat as an object, not container
      return r<<obj;
    }
    REST_PROCESS_BUFFER signature() const override;
    REST_PROCESS_BUFFER list() const override {
      std::vector<string> array{".@elem",".@elemNoThrow",".@insert",".@erase",".@size"};
      return REST_PROCESS_BUFFER(array);
    }
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<T>());}
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
  void RPAC_insert(T& obj, const REST_PROCESS_BUFFER& arguments)
  {
    typename MutableValueType<typename T::value_type>::type v;
    convert(v,arguments);
    if (!obj.insert(v).second)
      throw std::runtime_error("key already exists, not inserted");
  }

  /// insert element into map
  template <class T>
  void RPAC_insert(const T&, const REST_PROCESS_BUFFER& argument)
  {
    throw std::runtime_error("cannot insert data into a constant container");
  }

  template <class T> 
  void RPAC_erase(T& obj, const REST_PROCESS_BUFFER& arguments)
  {
    typename T::key_type k;
    convert(k,arguments);
    obj.erase(k);
  }

  /// insert element into map
  template <class T>
  void RPAC_erase(const T&, const REST_PROCESS_BUFFER& argument)
  {
    throw std::runtime_error("cannot erase data from a constant container");
  }

  template <class U>
  inline typename enable_if<is_fundamental<U>, void>::T
  assignRawStringToKey(U& key, const std::string& x)
  {
    std::istringstream is(x); is>>key;
  }
    
  template <class U>
  inline typename enable_if<Not<is_fundamental<U>>, void>::T
  assignRawStringToKey(U& key, const std::string& x)
  {
    throw std::runtime_error("key "+x+" needs to be JSON encoded");
  }
    
  template <>
  inline void assignRawStringToKey(std::string& key, const std::string& x)
  {
    key=x;
  }

    /// assign \a x if T is a map
  template <class T,class K>
  typename enable_if<
    And<
      Not<is_const<T>>,
      is_pair<typename T::value_type>
      >, void>::T
  assignElem(T& obj, const K& k, const REST_PROCESS_BUFFER& x)
  {
    auto iter=obj.emplace(k, typename T::mapped_type()).first;
    convert(iter->second,x);
  }

  /// assign \a x if T is a set
  template <class T,class K>
  typename enable_if<
    And<
      Not<is_const<T>>,
      Not<is_pair<typename T::value_type>>
      >, void>::T
  assignElem(T& obj, const K& k,const REST_PROCESS_BUFFER& x)
  {
    bool v; x>>v;
    if (v)
      obj.insert(k);
    else
      obj.erase(k);
  }

  template <class T,class K>
  typename enable_if<is_const<T>, void>::T
  assignElem(T& obj, const K& k,const REST_PROCESS_BUFFER& x) {}

  
  template <class T> class RESTProcessAssociativeContainer: public RESTProcessWrapperBase
  {
    T& obj;

    /// get element if a map
    template <class I>
    typename enable_if<is_pair<typename std::iterator_traits<I>::value_type>, typename std::iterator_traits<I>::value_type::second_type>::T
    elem_of(const I& i) {return i->second;}

    /// get element if a set
    template <class I>
    typename enable_if<Not<is_pair<typename std::iterator_traits<I>::value_type>>, typename std::iterator_traits<I>::value_type>::T
    elem_of(const I& i) {return *i;}
    
  public:
    RESTProcessAssociativeContainer(T& obj): obj(obj) {}
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      REST_PROCESS_BUFFER r;
      if (remainder.empty())
        {
          convert(obj, arguments);
        }
      else if (startsWith(remainder,".@elem"))
        {
          // extract key
          auto keyStart=find(remainder.begin()+1, remainder.end(), '.');
          if (keyStart!=remainder.end())
            {
              ++keyStart;
              auto keyEnd=keyStart;
              typename T::key_type key;
              if (strchr("\"'{[",*keyStart)) // JSON leadin
                {
                  json_pack_t jsonKey;
                  read_range(keyEnd,remainder.end(),static_cast<json5_parser::mValue&>(jsonKey));
                  jsonKey>>key;
                }
              else
                {
                  keyEnd=find(keyStart, remainder.end(), '.');
                  assignRawStringToKey(key, std::string(keyStart, keyEnd));
                }

              string tail(keyEnd,remainder.end());
              if (tail.empty() && arguments.type()!=RESTProcessType::null)
                assignElem(obj, key, arguments);
              auto i=obj.find(key);
              if (i==obj.end())
                {
                  if (startsWith(remainder,".@elemNoThrow"))
                    return mapAndProcessDummy<typename T::value_type>(tail, arguments);
                  else
                    throw std::runtime_error("key "+std::string(keyStart, keyEnd)+" not found");
                }
              else if (tail.empty())
                return r<<elem_of(i);
              auto eoi=elem_of(i);
              return mapAndProcess(tail, arguments, eoi);
            }
        }
      else if (startsWith(remainder,".@insert"))
        RPAC_insert(obj,arguments);
      else if (startsWith(remainder,".@erase"))
        RPAC_erase(obj,arguments);
      else if (startsWith(remainder,".@size"))
        return r<<obj.size();
      else if (startsWith(remainder,".@keys"))
        {
          std::vector<typename T::key_type> keys;
          for (auto& i: obj)
            keys.push_back(keyOf(i));
          return r<<keys;
        }
      else
        return RESTProcess_t(obj).process(remainder,arguments); // treat as an object, not container
      return r<<obj;
    }
    REST_PROCESS_BUFFER signature() const override;
    REST_PROCESS_BUFFER list() const override {
      std::vector<string> array{".@elem",".@elemNoThrow",".@insert",".@erase",".@size",".@keys"};
      return REST_PROCESS_BUFFER(array);
    }
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<T>());}
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
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    REST_PROCESS_BUFFER signature() const override;
    REST_PROCESS_BUFFER list() const override {
      if (ptr)
        return const_cast<RESTProcessPtr<T>*>(this)->process(".@list",{});
      else
        return REST_PROCESS_BUFFER(json5_parser::mArray());
    }
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<T>());}
  };

  template <class T>
  struct RESTProcessWeakPtr: public RESTProcessWrapperBase
  {
    T& ptr;
    RESTProcessWeakPtr(T& ptr): ptr(ptr) {}
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    REST_PROCESS_BUFFER signature() const override;
    REST_PROCESS_BUFFER list() const override {
      if (auto p=ptr.lock())
        return RESTProcessObject<typename T::element_type>(*p).list();
      else return REST_PROCESS_BUFFER(json5_parser::mArray());
    }
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<std::weak_ptr<T> >());}
  };

  
  template <class T>
  struct RESTProcessPtr<classdesc::weak_ptr<T>>: public RESTProcessWeakPtr<classdesc::weak_ptr<T>>
  {RESTProcessPtr(classdesc::weak_ptr<T>& x): RESTProcessWeakPtr<classdesc::weak_ptr<T>>(x) {}};
  
  template <class T>
  struct RESTProcessPtr<const classdesc::weak_ptr<T>>: public RESTProcessWeakPtr<const classdesc::weak_ptr<T>>
  {RESTProcessPtr(const classdesc::weak_ptr<T>& x): RESTProcessWeakPtr<const classdesc::weak_ptr<T>>(x) {}};
  
  
  // buffer adaptor for a vector of REST_PROCESS_BUFFER objects 
  class JSONBuffer
  {
    std::vector<REST_PROCESS_BUFFER> values;
    std::vector<REST_PROCESS_BUFFER>::iterator it;
  public:
    JSONBuffer(const REST_PROCESS_BUFFER& j) {
      if (j.type()==RESTProcessType::array)
        for (auto& i: j.array())
          values.push_back(REST_PROCESS_BUFFER(i));
      else if (j.type()!=RESTProcessType::null)
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
        is_reference<typename functional::Return<F>::T>>,
    REST_PROCESS_BUFFER>::T
  callFunction(const string& remainder, const REST_PROCESS_BUFFER& arguments, F f)
  {
    JSONBuffer argBuf(arguments);
    auto& r=functional::callOnBuffer(argBuf,f);
    if (remainder.empty())
      {
        // if there are arguments left over, assign the first of the
        // remaining arguments to the result
        switch (arguments.type())
          {
          case RESTProcessType::null: break;
          case RESTProcessType::array:
            {
              auto arr=arguments.array();
              if (arr.size()>functional::Arity<F>::value)
                convert(r,REST_PROCESS_BUFFER(arr[functional::Arity<F>::value]));
              break;
            }
          default:
            if (functional::Arity<F>::value==0)
              convert(r,arguments);
            break;
          }
        REST_PROCESS_BUFFER rj;
        return rj<<r;
      }
    RESTProcess_t map;
    RESTProcess(map,"",r);
    return map.process(remainder, arguments);
  }

  template <class F>
  typename enable_if<
    And<functional::AllArgs<F, functional::ArgAcceptable>,
        And<
          Not<is_void<typename functional::Return<F>::T>>,
        Not<is_reference<typename functional::Return<F>::T>>>>,
    REST_PROCESS_BUFFER>::T
  callFunction(const string& remainder, const REST_PROCESS_BUFFER& arguments, F f)
  {
    JSONBuffer argBuf(arguments);
    auto r=functional::callOnBuffer(argBuf,f);
    if (remainder.empty())
      {
        REST_PROCESS_BUFFER rj;
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
    REST_PROCESS_BUFFER>::T
  callFunction(const string& remainder, const REST_PROCESS_BUFFER& arguments, F f)
  {
    JSONBuffer argBuf(arguments);
    functional::callOnBuffer(argBuf,f);
    return {};
  }

  
  // don't do anything if we cannot create or copy an argument
  template <class F>
  typename enable_if<Not<functional::AllArgs<F, functional::ArgAcceptable>>, REST_PROCESS_BUFFER>::T
  callFunction(const string& remainder, const REST_PROCESS_BUFFER& arguments, F f)
  {throw std::runtime_error("cannot call this function");}
  
  /// @{
  /// return whether \a arg matches a C++ type T for a function call argument
  
  template <class T, class V>
  typename enable_if<And<is_pointer<T>, Not<is_same<T,const char*> > >,bool>::T
  matches(const V& x)
  {return false;}

  template <class T, class V>
  typename enable_if<is_same<T,bool>,bool>::T
  matches(const V& x)
  {return x.type()==RESTProcessType::boolean;}

  template <class T>
  typename enable_if<is_same<T,string>,bool>::T matches(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::string;}

  template <class T>
  typename enable_if<is_same<T,const char*>, bool>::T
  matches(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::string;}

  template <class T>
  typename enable_if<And<is_integral<T>,Not<is_same<T,bool>>>, bool>::T matches(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::int_number;}

  template <class T>
  typename enable_if<is_floating_point<T>, bool>::T matches(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::float_number;}
  
  template <class T>
  typename enable_if<is_enum<T>, bool>::T matches(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::string;}
  
  template <class T>
  typename enable_if<
    And<
    And<
      And<is_class<T>, is_default_constructible<T> >,
      Not<is_same<T,string>>
      >,
      Not<is_container<T>>
      >, bool>::T
  matches(const REST_PROCESS_BUFFER& x)
  {
    if (x.type()!=RESTProcessType::object) return false;
    try // to convert the json object to a T
      {
        T test;
        REST_PROCESS_BUFFER(x)>>test;
      }
    catch(const std::exception&)
      {return false;}
    return true;
  }

  template <class T>
  typename enable_if<is_container<T>, bool>::T matches(const REST_PROCESS_BUFFER& x)
  {
    if (x.type()==RESTProcessType::array)
      {
        auto arr=x.array();
        bool r=true;
        for (auto& i: arr) r &= matches<typename T::value_type>(REST_PROCESS_BUFFER(i));
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
  matches(const REST_PROCESS_BUFFER&) {return false;}

  template <class T>
  typename enable_if<
    And<is_const<typename remove_reference<T>::type>, is_reference<T>>, bool>::T
  matches(const REST_PROCESS_BUFFER& x)
  {return matches<typename remove_const_ref<T>::type>(x);}

  template <class T>
  typename enable_if<
    And<
    is_object<T>,
      Not<is_default_constructible<typename remove_reference<T>::type>>
      >, bool>::T
  matches(const REST_PROCESS_BUFFER& x)
  {return false;}

  template <class T>
  struct isNoMatch
  {
    static const bool value = !is_integral<T>::value && !is_floating_point<T>::value &&
      !is_container<T>::value && !is_object<T>::value && !is_pointer<T>::value;
  };

  template <class T>
  typename enable_if<And<Not<is_reference<T>>,isNoMatch<typename remove_const<T>::type>>, bool>::T
  matches(const REST_PROCESS_BUFFER&) {return false;}

  template <class T>
  typename enable_if<And<is_reference<T>, Not<is_const<typename remove_reference<T>::type>>>, bool>::T
  matches(const REST_PROCESS_BUFFER&) {return false;}

  /// @{ testing for not quite so good matches between json type and C++ type
  //template <class T> bool partiallyMatchable(const json5_parser::mValue& x);

  template <class T>
  typename enable_if<is_floating_point<typename remove_reference<T>::type>, bool>::T partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::float_number||x.type()==RESTProcessType::int_number;}

  template <class T>
  typename enable_if<is_container<T>, bool>::T partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {
    if (x.type()==RESTProcessType::array)
      {
        auto arr=x.array();
        bool r;
        for (auto& i: arr) r &= partiallyMatchable<typename T::value_type>(i);
        return r;
      }
    return partiallyMatchable<typename T::value_type>(x); // treat a single json object as a single element sequence
  }

  template <class T>
  typename enable_if<And<Not<is_floating_point<typename remove_reference<T>::type> >, Not<is_container<T> > >, bool>::T
  partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {return matches<T>(x);}


  template <class T> unsigned argMatchScore(const REST_PROCESS_BUFFER& x)
  {
    if (matches<T>(x)) return 0;
    if (partiallyMatchable<T>(x)) return 1;
    return RESTProcessFunctionBase::maxMatchScore;
  }
  
  
  template <class F, int N, int NN=N>
  struct MatchScore
  {
    static unsigned score(const REST_PROCESS_BUFFER& x)
    {
      if (x.type()!=RESTProcessType::array) return RESTProcessFunctionBase::maxMatchScore;
      auto arr=x.array();
      if (arr.size()<N) return RESTProcessFunctionBase::maxMatchScore;
      return  argMatchScore<typename functional::Arg<F,N>::T>(REST_PROCESS_BUFFER(arr[N-1])) +
        MatchScore<F,N-1,NN>::score(x);
    }
  };
  
  template <class F, int NN>
  struct MatchScore<F,2,NN>
  {
    static unsigned score(const REST_PROCESS_BUFFER& x)
    {
      if (x.type()!=RESTProcessType::array) return RESTProcessFunctionBase::maxMatchScore;
      auto arr=x.array();
      if (arr.size()<2) return RESTProcessFunctionBase::maxMatchScore;
      return argMatchScore<typename functional::Arg<F,1>::T>(REST_PROCESS_BUFFER(arr[0])) +
        argMatchScore<typename functional::Arg<F,2>::T>(REST_PROCESS_BUFFER(arr[1]))+
        10*(arr.size()-NN); // penalize for supplying more arguments than needed
    }
  };

  template <class F,int NN>
  struct MatchScore<F,1,NN>
  {
    static unsigned score(const REST_PROCESS_BUFFER& x)
    {
      switch (x.type())
        {
        case RESTProcessType::null:
          return RESTProcessFunctionBase::maxMatchScore;
        case RESTProcessType::array:
          {
            auto arr=x.array();
            if (arr.empty()) return RESTProcessFunctionBase::maxMatchScore;
            return argMatchScore<typename functional::Arg<F,1>::T>(REST_PROCESS_BUFFER(arr[0]))+
              10*(arr.size()-NN); // penalize for supplying more arguments than needed
          }
        default:
          return argMatchScore<typename functional::Arg<F,1>::T>(x);
        }
    }
  };
  
  template <class F,int NN>
  struct MatchScore<F,0,NN>
  {
    static unsigned score(const REST_PROCESS_BUFFER& x)
    {
      switch (x.type())
        {
        case RESTProcessType::null:
          return 0;
        case RESTProcessType::array:
          {
            auto arr=x.array();
            return 10*arr.size()-NN; // penalize for supplying more arguments than needed
          }
        default:
          return 10; // penalize for supplying more arguments than needed
        }
    }
  };
  
  template <class F, int N=functional::Arity<F>::value>
  unsigned matchScore(const REST_PROCESS_BUFFER& x)
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
    

    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {return callFunction(remainder, arguments, f);}
    
    REST_PROCESS_BUFFER signature() const override {return functionSignature<F>();}
    unsigned matchScore(const REST_PROCESS_BUFFER& arguments) const override
    {return classdesc::matchScore<F>(arguments);}
    REST_PROCESS_BUFFER list() const override {return REST_PROCESS_BUFFER(RESTProcessType::array);}
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER("function");}
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
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      throw std::runtime_error("currently unable to call functions returning unique_ptr");
    }
    REST_PROCESS_BUFFER signature() const override {return functionSignature<F>();}
    unsigned matchScore(const REST_PROCESS_BUFFER& arguments) const override
    {return classdesc::matchScore<F>(arguments);}
    REST_PROCESS_BUFFER list() const override {return REST_PROCESS_BUFFER(json5_parser::mArray());}
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<F>());}
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
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      REST_PROCESS_BUFFER r;
      if (remainder=="@type")
        return r<<typeName<E>();
      else if (remainder=="@signature")
        return signature();
      else if (arguments.type()==RESTProcessType::string)
        {
          string tmp; arguments>>tmp;
          convert(e, enum_keys<E>()(tmp));
        }
      else if (arguments.type()==RESTProcessType::array && !arguments.array().empty())
        {
          string tmp; json_pack_t(arguments.array()[0])>>tmp;
          convert(e, enum_keys<E>()(tmp));
        }
      return r<<enum_keys<E>()(e);
    }
    REST_PROCESS_BUFFER signature() const override;
    REST_PROCESS_BUFFER list() const override {return REST_PROCESS_BUFFER(json5_parser::mArray());}
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER(typeName<E>());}
  };

  template <class E>
  class EnumerateEnumerators: public RESTProcessBase
  {
    REST_PROCESS_BUFFER process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    REST_PROCESS_BUFFER signature() const override {return REST_PROCESS_BUFFER("{ret: \"vector<string>\", args: []}");}
    REST_PROCESS_BUFFER list() const override {return REST_PROCESS_BUFFER(json5_parser::mArray());}
    REST_PROCESS_BUFFER type() const override {return REST_PROCESS_BUFFER("function");}
  };

  /// @{ define type dependent information in repository
  template <class E>
  typename enable_if<is_enum<E>, void>::T
  defineType(RESTProcess_t& r)
  {r.add("@enum."+typeName<E>(), new EnumerateEnumerators<E>());}

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


  template <class... Args> struct NumArgs
  {
    static const size_t value=functional::Arity<void(*)(Args...)>::value;
  };

//  template <class A, class... Args>
//  struct NumArgs<A,Args...>
//  {
//    static const size_t value=1+NumArgs<Args...>::value;
//  };
//
//  template <> struct NumArgs<>
//  {
//    static const size_t value=0;
//  };
  
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
