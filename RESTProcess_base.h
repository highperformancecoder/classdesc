/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef CLASSDESC_RESTPROCESS_BASE_H
#define CLASSDESC_RESTPROCESS_BASE_H
/// A classdesc descriptor to generate virtual xrap processing calls
#include "function.h"
#include "multiArray.h"
#include "object.h"
#include "polyRESTProcessBase.h"
#include "signature.h"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <assert.h>

#ifndef REST_PROCESS_BUFFER
#include "json_pack_base.h"
#define REST_PROCESS_BUFFER json_pack_t
#endif

namespace classdesc
{
  class RESTProcessBase;
  using RPPtr=std::shared_ptr<RESTProcessBase>;

  struct RESTProcess_t;

  template <class T>
  typename enable_if<Not<functional::is_nonmember_function_ptr<T>>, void>::T
  RESTProcess(RESTProcess_t&, const std::string&, T&);
  
  /// interface for the REST processor
  class RESTProcessBase
  {
  protected:
    /// implementation of upcasting to a classdesc::object
    template <class T> typename enable_if<is_base_of<object,T>, const object*>::T
    getClassdescObjectImpl(T& obj) {return &obj;}
    template <class T> typename enable_if<Not<is_base_of<object,T>>, const object*>::T
    getClassdescObjectImpl(T& obj) {return nullptr;}    
  public:
    virtual ~RESTProcessBase() {}
    /// perform the REST operation, with \a remainder being the query string and \a arguments as body text
    /// result of operation is returned as an object, and can be serialised into REST_PROCESS_BUFFER using asBuffer
    virtual RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments)=0;
    virtual REST_PROCESS_BUFFER asBuffer() const=0;
    /// return signature(s) of the operations
    virtual std::vector<Signature> signature() const=0;
    /// return list of subcommands to this
    virtual RESTProcess_t list() const=0;
    /// return type name of this
    virtual std::string type() const=0;
    /// populate \a map from the object wrapped by this, if any
    virtual void populate(RESTProcess_t& map) const {}
    /// return signature for a function type F
    template <class F> Signature functionSignature() const;
    /// returns a pointer to the underlying object if it is one of type T, otherwise null
    template <class T> T* getObject();
    /// @{ returns a classdesc object is referring to an object derived from classdesc::object
    virtual object* getClassdescObject() {return nullptr;}
    virtual const object* getConstClassdescObject() {return nullptr;}
    /// @}
    /// true if this is an object, not a function
    virtual bool isObject() const {return false;}
    /// true if this is a const object, a const member function or static/free function
    virtual bool isConst() const {return false;}
    /// arity if this is a function, 0 otherwise
    virtual unsigned arity() const {return 0;}
    /// size if this is a container, 0 otherwise
    virtual size_t size() const {return 0; }
    /// get element by position for sequences, by key for associative containers
    virtual RPPtr getElem(const REST_PROCESS_BUFFER&);
    /// sets element indexed/keyed by \a index to \a value
    /// @return updated element
    virtual RPPtr setElem(const REST_PROCESS_BUFFER& index, const REST_PROCESS_BUFFER& value)
    {return getElem(index);}
    /// append to end of a sequence, or inserts key into an associative container 
    virtual void insert(const REST_PROCESS_BUFFER& value) {}
    /// erase an element - by position for sequences, by key for associative containers
    virtual void erase(const REST_PROCESS_BUFFER& index) {}
    /// returns true if an associative container contains \a key
    virtual bool contains(const REST_PROCESS_BUFFER& key) const {return false;}
    /// returns a list of keys if this is an associative container, otherwise void
    virtual RPPtr keys() const;
  };


  /// @{ create an appropriate RESTProcess object referring to the argument.
  template <class T>
  typename enable_if<
    And<
      Not<is_container<T>>,
      Not<is_smart_ptr<T>>
      >, RPPtr>::T makeRESTProcessRef(T& obj);

  template <class T>
  typename enable_if<
    And<
      is_sequence<T>,
      Not<is_base_of<MultiArrayBase,T>>
      >,  RPPtr>::T
  makeRESTProcessRef(T& obj);

  template <class T>
  typename enable_if<is_base_of<MultiArrayBase,T>, RPPtr>::T
  makeRESTProcessRef(T& obj);

  template <class T>
  typename enable_if<is_smart_ptr<T>, RPPtr>::T
  makeRESTProcessRef(T& obj);
  /// @}
  
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
    REST_PROCESS_BUFFER asBuffer() const override {return {};}
  };

  struct RESTProcessOverloadedFunction: public RESTProcessBase
  {
    std::vector<std::shared_ptr<RESTProcessFunctionBase>> overloadedFunctions; 
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    REST_PROCESS_BUFFER asBuffer() const override {return {};}
    std::vector<Signature> signature() const override {
      std::vector<Signature> r;
      for (auto& i: overloadedFunctions) {
        auto s=i->signature();
        r.insert(r.end(),s.begin(),s.end());
      }
      return r;
    }
    RESTProcess_t list() const override;
    std::string type() const override {return "overloaded function";}
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
  typename enable_if<
    And<
      Not<is_base_of<MultiArrayBase,X>>,
      is_sequence<X>,
      Not<is_const<X>>
      >, void>::T
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
  
  template <class T,int R>
  void convert(MultiArray<T,R>& x, const REST_PROCESS_BUFFER& j)
  {
    if (j.type()==RESTProcessType::array)
      {
        auto arr=j.array();
        auto xi=x.begin();
        for (auto& ai: arr)
          {
            if (xi==x.end()) break;
            auto target=*xi++;
            convert(target, REST_PROCESS_BUFFER(ai));
          }
      }
  }

  template <class T>
  void convert(MultiArray<T,1>& x, const REST_PROCESS_BUFFER& j)
  {
    if (j.type()==RESTProcessType::array)
      {
        auto arr=j.array();
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
  typename enable_if<And<is_enum<E>,Not<is_const<E>>>, void>::T
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
  struct RESTProcess_t: public std::map<std::string, RPPtr >
  {
    RESTProcess_t()=default;
    /// populate this map with the members of \a obj. T must be a
    /// classdescified type, otherwise this is equivalent to a default
    /// contructor
    template <class T> RESTProcess_t(T& obj);
    /// Construct from an object that RPPPtr wraps
    RESTProcess_t(const RPPtr& p) {p->populate(*this);}
    /// ownership of \a rp is passed
    void add(string d, RESTProcessBase* rp)
    {
      // for objects, ensure any previous entries of this key are deleted
      erase(d);
      emplace(d,rp);
    }
    /// ownership of \a rp is passed
    void add(string d, RESTProcessFunctionBase* rp)
    {
      auto i=find(d);
      if (i==end())
        emplace(d,rp);
      else if (auto functions=dynamic_cast<RESTProcessOverloadedFunction*>(i->second.get()))
        {
          // replace if signature the same
          for (auto& f: functions->overloadedFunctions)
            if (f->signature()==rp->signature())
              {
                i->second.reset(rp);
                return;
              }
          functions->overloadedFunctions.emplace_back(rp);
        }
      else if (auto firstFunction=dynamic_pointer_cast<RESTProcessFunctionBase>(i->second))
        {
          // replace if signature the same
          if (firstFunction->signature()==rp->signature())
            {
              i->second.reset(rp);
              return;
            }
          auto functs=std::make_shared<RESTProcessOverloadedFunction>();
          if (firstFunction) functs->overloadedFunctions.push_back(std::move(firstFunction));
          functs->overloadedFunctions.emplace_back(rp);
          i->second=functs;
        }
      else  // overloading something that is not a function
        i->second.reset(rp);
    }
  
    inline RPPtr process(const std::string& query, const REST_PROCESS_BUFFER& jin);

    /// define all arguments of \a F
    template <class F> void defineFunctionArgTypes()
    {
      DefineFunctionArgTypes<F,functional::Arity<F>::value>::define(*this);
    }

    /// adds a factory function for creating objects of type \a
    /// T.
    /// @tparam T type of objects this factory will create
    /// @tparam Args are the types of arguments to be passed to T's constructor
    /// @param typeName alias for the factory - usually the unqualified type name
    /// @param callback an optional callback that is run whenever a new object is created
    /// The factory gets called with a string argument giving the new
    /// objects name in the registry, followed by the arguments for
    /// T's constructor, if any
    template <class T, class... Args> void addFactory
    (const std::string& typeName,const std::function<void(const std::string& objName)>& callback=nullptr);
  };

  template <class T>
  RPPtr mapAndProcess(const string& query, const REST_PROCESS_BUFFER& arguments, T& a);

  template <class T>
  inline typename enable_if<is_default_constructible<T>,RPPtr>::T
  mapAndProcessDummy(const string& query, const REST_PROCESS_BUFFER& arguments)
  {
    T dummy{};
    return mapAndProcess(query,arguments,dummy);
  }

  template <class T>
  inline typename enable_if<Not<is_default_constructible<T>>,RPPtr>::T
  mapAndProcessDummy(const string& query, const REST_PROCESS_BUFFER& arguments)
  {
    throw std::runtime_error(typeName<T>()+" is not default constructible, but requested element doesn't exist");
  }

  template <class T> struct RESTProcessObject;
  template <class T> inline
  typename enable_if<is_copy_constructible<T>,RPPtr>::T
  toObjectValueImpl(const RESTProcessObject<T>&);
  template <class T> inline
  typename enable_if<Not<is_copy_constructible<T>>,RPPtr>::T
  toObjectValueImpl(const RESTProcessObject<T>&) {return nullptr;}

  /// handle setting and getting of objects
  template <class T> struct RESTProcessObject: public RESTProcessBase
  {
    T& obj;
    RESTProcessObject(T& obj): obj(obj) {}
    std::shared_ptr<classdesc::RESTProcessBase> process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
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
          return std::make_shared<RESTProcessObject>(obj);
        }
      return mapAndProcess(remainder, arguments, obj);
    }
    REST_PROCESS_BUFFER asBuffer() const override {REST_PROCESS_BUFFER r; return r<<obj;}
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override;
    std::string type() const override;
    void populate(RESTProcess_t& map) const override {RESTProcess(map,"",obj);}
    object* getClassdescObject() override {
      if (is_const<T>::value) return nullptr;
      return const_cast<object*>(getClassdescObjectImpl(obj));
    }
    bool isObject() const override {return true;}
    const object* getConstClassdescObject() override {return getClassdescObjectImpl(obj);}
    bool isConst() const override {return std::is_const<T>::value;}
    size_t size() const override {
      if constexpr (is_class<T>::value)
        if constexpr (has_size<T>::value)
          return obj.size();
      return 0;
    }
    RPPtr getElem(const REST_PROCESS_BUFFER& b) override {
      if constexpr (is_class<T>::value)
        if constexpr (has_index_operator<T>::value)
           {
             size_t index; b>>index;
             return makeRESTProcessRef(obj[index]);
           }
      return RESTProcessBase::getElem(b);
    }
    /// sets element indexed/keyed by \a index to \a value
    /// @return updated element
    RPPtr setElem(const REST_PROCESS_BUFFER& b, const REST_PROCESS_BUFFER& value) override
    {
      if constexpr (is_class<T>::value && !is_const<T>::value)
        if constexpr (has_index_operator<T>::value)
          {
            size_t index; b>>index;
            value>>obj[index];
            return makeRESTProcessRef(obj[index]);
          }
      return RESTProcessBase::setElem(b,value);
    }
    

  };

  /// same as \a RESTProcessObject, but internally stores the object. T must be copy constructible or moveable
  template <class T> struct RESTProcessValueObject: public RESTProcessObject<T>
  {
    T actual;
  public:
    template <class... Args>
    RESTProcessValueObject(Args&&... args): RESTProcessObject<T>(actual), actual(std::forward<Args>(args)...) {}
  };

  template <class T> inline
  typename enable_if<is_copy_constructible<T>,RPPtr>::T
  toObjectValueImpl(const RESTProcessObject<T>& x) 
  {return std::make_shared<RESTProcessValueObject<T>>(x.obj);}
  
  template <class T>
  RPPtr makeRESTProcessValueObject(T&& obj)
  {return std::make_shared<RESTProcessValueObject<typename std::remove_reference<T>::type>>(std::forward<T>(obj));}
  // specialization for string and string vector to allow
  inline RPPtr makeRESTProcessValueObject(const char* s)
  {return std::make_shared<RESTProcessValueObject<std::string>>(s);}
  inline RPPtr makeRESTProcessValueObject(const std::initializer_list<std::string>& init)
  {return std::make_shared<RESTProcessValueObject<std::vector<std::string>>>(init);}
 
  /// class that represents the void, or null object
  struct RESTProcessVoid: public RESTProcessBase
  {
    std::shared_ptr<RESTProcessBase> process(const string&, const REST_PROCESS_BUFFER&) override
    {return std::make_shared<RESTProcessVoid>();}
    REST_PROCESS_BUFFER asBuffer() const override {return {};}
    std::vector<Signature> signature() const override {return {};}
    RESTProcess_t list() const override {return {};}
    std::string type() const override {return "void";}
    bool isConst() const override {return true;}
  };

  inline RPPtr RESTProcessBase::getElem(const REST_PROCESS_BUFFER&)
  {return std::make_shared<RESTProcessVoid>();}

  inline RPPtr RESTProcessBase::keys() const
  {return std::make_shared<RESTProcessVoid>();}
 
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
    is_class<T>,
    Not<is_container<T>>,
    Not<is_smart_ptr<T>>,
    Not<is_string<T>>,
    Not<is_excluded<T>>
    > {};
  
  
  template <class T>
  typename enable_if<is_excluded<T>,void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {}

  template <class T>
  typename enable_if<Or<is_fundamental<T>,is_string<T>>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.emplace(d, makeRESTProcessRef(a));}

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

  template <class U>
  typename enable_if<is_default_constructible<U>, U&>::T
  dummyRef() {
    static U dummy{};
    return dummy;
  }

  template <class U>
  typename enable_if<Not<is_default_constructible<U>>, U&>::T
  dummyRef() {
    throw std::runtime_error(typeName<U>()+" is not default constructible, but requested element doesn't exist");
  }

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
      push_back(o,v);
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

    // common element extraction code - assumes index is in bounds
    typename transfer_const<T, typename T::value_type>::type& elemCommon(size_t idx) {
      auto i=obj.begin();
      std::advance(i, idx);
      return *i;
    }

    typename transfer_const<T, typename T::value_type>::type& elemNoThrow(size_t idx) {
      if (idx<obj.size()) return elemCommon(idx);
      return dummyRef<typename T::value_type>();
    }
      
    void pushBack(const typename T::value_type& v) {
      push_back(obj, v);
    }

    template <class U>
    typename enable_if<has_member_erase<U,typename U::iterator(U::*)(typename U::const_iterator)>,void>::T
    erase(U& o,typename U::iterator i) {o.erase(i);}

    template <class U>
    typename enable_if<Not<is_same<typename U::iterator,typename U::const_iterator>>,void>::T
    erase(U& o,typename U::const_iterator i) {}
    
    template <class U>
    typename enable_if<Not<has_member_erase<U,typename U::iterator(U::*)(typename U::const_iterator)>>,void>::T
    erase(U& o,typename U::iterator i) {}
    
    
    void eraseElem(size_t idx) {
      if (idx>=size()) return;
      auto i=obj.begin();
      std::advance(i, idx);
      erase(obj,i);
    }


  protected:
    typename transfer_const<T, typename T::value_type>::type& elem(size_t idx) {
      if (idx<obj.size()) return elemCommon(idx);
      throw std::runtime_error("idx out of bounds");
    }

    
  public:
    RESTProcessSequence(T& obj): obj(obj) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override;
    std::string type() const override {return typeName<T>();}
    REST_PROCESS_BUFFER asBuffer() const override {REST_PROCESS_BUFFER r; return r<<obj;}
    bool isObject() const override {return true;}
    RPPtr getElem(const REST_PROCESS_BUFFER& index) override {
      size_t idx; index>>idx;
      return makeRESTProcessRef(elem(idx));
    }
    RPPtr setElem(const REST_PROCESS_BUFFER& index, const REST_PROCESS_BUFFER& value) override {
      size_t idx; index>>idx;
      auto& v=elem(idx);
      value>>v;
      return makeRESTProcessRef(v);
    }
    size_t size() const override {return obj.size();}
    void insert(const REST_PROCESS_BUFFER& value) override {insert(obj,value);}
    void erase(const REST_PROCESS_BUFFER& index) override {
      size_t idx; index>>idx;
      eraseElem(idx);
    }
  };

  template <class T> struct RESTProcessValueSequence: public RESTProcessSequence<T>
  {
    T actual;
  public:
    template <class... Args>
    RESTProcessValueSequence(Args&&... args): RESTProcessSequence<T>(actual), actual(std::forward<Args>(args)...) {}
  };

  template <class T, int R> RPPtr copyMultiArrayIterator(MultiArray<T,R>& x);
  template <class T, int R> RPPtr copyMultiArrayIterator(const MultiArray<T,R>& x);

  template <class T> RPPtr copyMultiArrayIterator(T& x)
  {return std::make_shared<RESTProcessObject<T>>(x);}
  
  template <class T> struct RESTProcessMultiArray: public RESTProcessBase
  {
    T actual;
  public:
    template <class... Args>
    RESTProcessMultiArray(Args&&... args): actual(std::forward<Args>(args)...) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    REST_PROCESS_BUFFER asBuffer() const override {
      REST_PROCESS_BUFFER r;
      return r<<actual;
    }
    std::vector<Signature> signature() const override {return {};}
    RESTProcess_t list() const override {return {};}
    std::string type() const override {return "typeName<T>()";}
    bool isObject() const override {return true;}
    RPPtr getElem(const REST_PROCESS_BUFFER& index) override {
      size_t idx; index>>idx;
      if (idx<actual.size())
        return copyMultiArrayIterator(actual[idx]);
      return std::make_shared<RESTProcessVoid>();
    }
    RPPtr setElem(const REST_PROCESS_BUFFER& index, const REST_PROCESS_BUFFER& value) override {
      size_t idx; index>>idx;
      if (idx<actual.size())
        {
          auto elem=actual[idx];
          convert(elem,value);
          return copyMultiArrayIterator(elem);
        }
      return std::make_shared<RESTProcessVoid>();
    }
    size_t size() const override {return actual.size();}
  };

  template <class T, int R> RPPtr copyMultiArrayIterator(MultiArray<T,R>& x)
  {return std::make_shared<RESTProcessMultiArray<MultiArray<T,R>>>(x);}
  template <class T, int R> RPPtr copyMultiArrayIterator(const MultiArray<T,R>& x)
  {return std::make_shared<RESTProcessMultiArray<MultiArray<T,R>>>(x);}

  template <class T> struct RESTProcessMultiArray<classdesc::MultiArray<T,1>>:
    public RESTProcessSequence<classdesc::MultiArray<T,1>>
  {
    MultiArray<T,1> actual;
  public:
    template <class... Args>
    RESTProcessMultiArray(Args&&... args): RESTProcessSequence<classdesc::MultiArray<T,1>>(actual), actual(std::forward<Args>(args)...) {}
    RPPtr getElem(const REST_PROCESS_BUFFER& index) {
      size_t idx; index>>idx;
      return std::make_shared<RESTProcessObject<T>>(this->elem(idx));
    }
    RPPtr setElem(const REST_PROCESS_BUFFER& index, const REST_PROCESS_BUFFER& value) {
      size_t idx; index>>idx;
      auto& v=this->elem(idx);
      value>>v;
      return std::make_shared<RESTProcessObject<T>>(v);
    }
    size_t size() const override {return actual.size();}
  };

  
  
  template <class T>
  typename enable_if<
    And<
      is_sequence<T>,
      Not<is_base_of<MultiArrayBase,T>>
      >, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.add(d, new RESTProcessSequence<T>(a));}

  template <class T>
  typename enable_if<is_base_of<MultiArrayBase,T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {repo.add(d, new RESTProcessValueSequence<T>(a));}

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

  template <class T, class Enable=void> struct MappedType;
  template <class T> struct MappedType // for maps
  <T, typename enable_if<is_pair<typename T::value_type>, void>::T>
  {
    using type=typename transfer_const<
      T,
      typename T::value_type::second_type,
      Or<is_const<T>, is_const<typename T::value_type>,
         is_const<typename T::value_type::second_type>>::value
      >::type;
  };

  template <class T> struct MappedType // for sets
  <T, typename enable_if<Not<is_pair<typename T::value_type>>, void>::T>
  {using type=const typename T::value_type;};


  template <class T>
  typename enable_if<is_pair<typename T::value_type>, typename T::value_type>::T
  makeElement(const typename T::key_type& k) {return {k,{}};}
  template <class T>
  typename enable_if<Not<is_pair<typename T::value_type>>, typename T::value_type>::T
  makeElement(const typename T::key_type& k) {return k;}
    
  template <class T> class RESTProcessAssociativeContainer: public RESTProcessWrapperBase
  {
    T& obj;

    /// get element if a map
    template <class I>
    typename enable_if<
      is_pair<typename std::iterator_traits<I>::value_type>,
      // in C++20, std::iterator_traits<I>::value_type has any const
      // attribute stripped off, hence this convoluted mouthful
      typename transfer_const<
        typename std::remove_reference<
          typename std::iterator_traits<I>::reference
          >::type,
        typename std::iterator_traits<I>::value_type::second_type
        >::type&
      >::T
    elem_of(const I& i) const {return i->second;}

    /// get element if a set
    template <class I>
    typename enable_if<Not<is_pair<typename std::iterator_traits<I>::value_type>>,
                       const typename std::iterator_traits<I>::value_type&>::T
    elem_of(const I& i) const {return *i;}

    template <class U>
    typename enable_if<Not<is_const<U>>, typename MappedType<U>::type&>::T elemImpl(const typename U::key_type& k) 
    {return elem_of(obj.emplace(makeElement<T>(k)).first);}
    
    template <class U>
    typename enable_if<is_const<U>, typename MappedType<U>::type&>::T elemImpl(const typename U::key_type& k) 
    {
      auto i=obj.find(k);
      if (i==obj.end()) return dummyRef<typename MappedType<U>::type>();
      return elem_of(i);
    }
    
    typename MappedType<T>::type& elem(const typename T::key_type& k)
    {return elemImpl<T>(k);}
    
    
  public:
    RESTProcessAssociativeContainer(T& obj): obj(obj) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override;
    std::string type() const override {return typeName<T>();}
    REST_PROCESS_BUFFER asBuffer() const override {REST_PROCESS_BUFFER r; return r<<obj;}
    bool isObject() const override {return true;}
    RPPtr getElem(const REST_PROCESS_BUFFER& index) override {
      typename T::key_type idx; index>>idx;
      return makeRESTProcessRef(elem(idx));
    }
    RPPtr setElem(const REST_PROCESS_BUFFER& index, const REST_PROCESS_BUFFER& value) override {
      typename T::key_type idx; index>>idx;
      auto& v=elem(idx);
      value>>v;
      return makeRESTProcessRef(v);
    }
    size_t size() const override {return obj.size();}
    void insert(const REST_PROCESS_BUFFER& value) override {RPAC_insert(obj,value);}
    void erase(const REST_PROCESS_BUFFER& index) override {RPAC_erase(obj,index);}
    bool contains(const REST_PROCESS_BUFFER& index) const override {
      typename T::key_type k; convert(k,index);
      return obj.count(k);
    }
    RPPtr keys() const override {
      std::vector<typename T::key_type> k;
      for (auto& i: obj)
        k.emplace_back(keyOf(i));
      return makeRESTProcessValueObject(std::move(k));
    }
  };

  template <class T> struct RESTProcessValueAssociativeContainer: public RESTProcessAssociativeContainer<T>
  {
    T actual;
  public:
    template <class... Args>
    RESTProcessValueAssociativeContainer(Args&&... args):
      RESTProcessAssociativeContainer<T>(actual), actual(std::forward<Args>(args)...) {}
  };

  template <class T>
  typename enable_if<is_associative_container<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, const string& d, T& a)
  {
    repo.add(d, new RESTProcessAssociativeContainer<T>(a));
  }

  template <class T>
  typename enable_if<is_base_of<PolyRESTProcessBase, T>, RESTProcess_t>::T
  rMap(T& x) {
    RESTProcess_t r;
    x.RESTProcess(r,"");
    return r;
  }
  
  template <class T>
  typename enable_if<Not<is_base_of<PolyRESTProcessBase, T>>, RESTProcess_t>::T
  rMap(T& x) {
    RESTProcess_t r;
    RESTProcess(r,"",x);
    return r;
  }
  
  template <class T>
  struct RESTProcessPtr: public RESTProcessWrapperBase
  {
    T& ptr;
    RESTProcessPtr(T& ptr): ptr(ptr) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override {
      if (ptr)
        return rMap(*ptr);
      else
        return {};
    }
    std::string type() const override {return typeName<T>();}
    object* getClassdescObject() override {
      if (!ptr || is_const<typename T::element_type>::value) return nullptr;
      return const_cast<object*>(getClassdescObjectImpl(*ptr));
    }
    const object* getConstClassdescObject() override {return ptr? getClassdescObjectImpl(*ptr): nullptr;}
    REST_PROCESS_BUFFER asBuffer() const override {
      REST_PROCESS_BUFFER r;
      return ptr? (r<<*ptr): r;
    }
    bool isObject() const override {return true;}
  };

  template <class T>
  struct RESTProcessWeakPtr: public RESTProcessWrapperBase
  {
    T& ptr;
    RESTProcessWeakPtr(T& ptr): ptr(ptr) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override {
      if (auto p=ptr.lock())
        return RESTProcessObject<typename T::element_type>(*p).list();
      return {};
    }
    std::string type() const override {return typeName<std::weak_ptr<T> >();}
    object* getClassdescObject() override {
      auto p=ptr.lock();
      if (!p || is_const<typename T::element_type>::value) return nullptr;
      return const_cast<object*>(getClassdescObjectImpl(*p));
    }
    const object* getConstClassdescObject() override  {
      auto p=ptr.lock();
      return p? getClassdescObjectImpl(*p): nullptr;
    }
    REST_PROCESS_BUFFER asBuffer() const override {
      REST_PROCESS_BUFFER r;
      auto p=ptr.lock();
      return p? (r<<*p): r;
    }
    bool isObject() const override {return true;}
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
    RPPtr>::T
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
        return std::make_shared<RESTProcessObject<typename functional::Return<F>::T>>(r);
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
    RPPtr>::T
  callFunction(const string& remainder, const REST_PROCESS_BUFFER& arguments, F f)
  {
    JSONBuffer argBuf(arguments);
    auto r=functional::callOnBuffer(argBuf,f);
    if (remainder.empty())
      return makeRESTProcessValueObject(std::move(r));
    RESTProcess_t map;
    RESTProcess(map,"",r);
    return map.process(remainder, arguments);
  }
  
  template <class F>
  typename enable_if<
    And<functional::AllArgs<F, functional::ArgAcceptable>,
        is_void<typename functional::Return<F>::T>>,
    RPPtr>::T
  callFunction(const string& remainder, const REST_PROCESS_BUFFER& arguments, F f)
  {
    JSONBuffer argBuf(arguments);
    functional::callOnBuffer(argBuf,f);
    return std::make_shared<RESTProcessVoid>();
  }

  
  // don't do anything if we cannot create or copy an argument
  template <class F>
  typename enable_if<Not<functional::AllArgs<F, functional::ArgAcceptable>>, RPPtr>::T
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
      Not<is_abstract<T>>,
      Not<is_default_constructible<typename remove_reference<T>::type>>,
      Not<is_container<T>>
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
  typename enable_if<is_floating_point<T>, bool>::T partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {return x.type()==RESTProcessType::float_number||x.type()==RESTProcessType::int_number||x.type()==RESTProcessType::null;}

  template <class T>
  typename enable_if<
    And<is_const<typename remove_reference<T>::type>, is_reference<T>>, bool>::T
  partiallyMatchable(const REST_PROCESS_BUFFER& x);

  template <class T>
  typename enable_if<
    And<
      Not<is_floating_point<T>>,
      Not<is_container<T>>,
      Not<And<is_reference<T>,is_const<typename remove_reference<T>::type>>>
      >, bool>::T
  partiallyMatchable(const REST_PROCESS_BUFFER& x);

  template <class T>
  typename enable_if<is_container<T>, bool>::T partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {
    if (x.type()==RESTProcessType::array)
      {
        auto arr=x.array();
        bool r=true;
        for (auto& i: arr) r &= partiallyMatchable<typename T::value_type>(REST_PROCESS_BUFFER(i));
        return r;
      }
    return partiallyMatchable<typename T::value_type>(x); // treat a single json object as a single element sequence
  }

  template <class T>
  typename enable_if<
    And<
      Not<is_floating_point<T>>,
      Not<is_container<T>>,
      Not<And<is_reference<T>,is_const<typename remove_reference<T>::type>>>
      >, bool>::T
  partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {return matches<T>(x);}

  template <class T>
  typename enable_if<
    And<is_const<typename remove_reference<T>::type>, is_reference<T>>, bool>::T
  partiallyMatchable(const REST_PROCESS_BUFFER& x)
  {return partiallyMatchable<typename remove_const_ref<T>::type>(x);}

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
  public:
    F f;
    RESTProcessFunction(F f): f(f) {}
    

    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {return callFunction(remainder, arguments, f);}
    
    std::vector<Signature> signature() const override {return {functionSignature<F>()};}
    
    unsigned matchScore(const REST_PROCESS_BUFFER& arguments) const override
    {return classdesc::matchScore<F>(arguments);}

    template <class U>
    typename enable_if<
      And<
        is_default_constructible<typename remove_reference<U>::type>,
        Not<is_void<U>>
        >,RESTProcess_t>::T
    slist() const {
      typename remove_const<typename remove_reference<U>::type>::type x;
      return RESTProcessObject<U>(x).list();
    }
    // for now, we cannot extract the lists of a non-default constructible return type
    template <class U>
    typename enable_if<
      Not<
        And<
          is_default_constructible<typename remove_reference<U>::type>,
          Not<is_void<U>>
          >>,RESTProcess_t>::T
    slist() const {return {};}
    
    RESTProcess_t list() const override {return slist<R>();}
    std::string type() const override {return typeName<R>();}
    bool isObject() const override {return false;}
    bool isConst() const override {return FunctionalIsConst<F>::value;}
    unsigned arity() const override {return functional::Arity<F>::value;}
  };

  template <class F, class R>
  class RESTProcessFunction<F, std::unique_ptr<R>>: public RESTProcessFunctionBase
  {
  public:
    F f;
    RESTProcessFunction(F f): f(f) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      throw std::runtime_error("currently unable to call functions returning unique_ptr");
    }
    std::vector<Signature> signature() const override
    {return {functionSignature<F>()};}
    unsigned matchScore(const REST_PROCESS_BUFFER& arguments) const override
    {return classdesc::matchScore<F>(arguments);}
    RESTProcess_t list() const override {return {};}
    std::string type() const override {return typeName<F>();}
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

  template <class F> std::shared_ptr<RESTProcessFunction<F>>
  makeRESTProcessFunction(F f)
  {return std::make_shared<RESTProcessFunction<F>>(f);}
  
  inline void RESTProcess(RESTProcess_t& repo, const string& d, const char*& a)
  {repo.add(d,new RESTProcessObject<const char*>(a));}

  template <class E>
  class RESTProcessEnum: public RESTProcessBase
  {
    E& e;
  public:
    RESTProcessEnum(E& e): e(e) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      if (remainder=="@type")
        return makeRESTProcessValueObject(typeName<E>());
      else if (remainder=="@signature")
        return makeRESTProcessValueObject(signature());
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
      return makeRESTProcessValueObject(enum_keys<E>()(e));
    }
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override {return {};}
    std::string type() const override {return typeName<E>();}
    REST_PROCESS_BUFFER asBuffer() const override {return REST_PROCESS_BUFFER(enum_keys<E>()(e));}
  };

  template <class E>
  class EnumerateEnumerators: public RESTProcessBase
  {
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override;
    std::vector<Signature> signature() const override;
    RESTProcess_t list() const override {return {};}
    std::string type() const override {return "function";}
    REST_PROCESS_BUFFER asBuffer() const override {return {};}
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

  
  template <class T, class... Args>
  struct RESTProcessMultiArrayFromC: public RESTProcessMultiArray<MultiArray<T,NumArgs<Args...>::value>>
  {
    static const size_t rank=NumArgs<Args...>::value;
    RESTProcessMultiArrayFromC(T* data, Args... dims):
      RESTProcessMultiArray<MultiArray<T,rank>>(data,dims...) {}
  };
  
  template <class T, class... Args>
  void RESTProcess(RESTProcess_t& r, const string& d, is_array, T& a, int dims, Args... args)
  {
    r.add(d, new RESTProcessMultiArrayFromC<T,Args...>(&a, args...));
  }

  template <class T>
  void RESTProcess(RESTProcess_t& r, const string& d, is_constructor, T& a)  {}

  template <class T>
  typename enable_if<is_smart_ptr<T>, void>::T 
  RESTProcessp(RESTProcess_t& repo, const std::string& d, T& a)
  {repo.add(d, new RESTProcessPtr<T>(a));}

  template <class T> struct RESTProcessHeapObject:  public RESTProcessPtr<std::unique_ptr<T>>
  {
    std::unique_ptr<T> obj;
    RESTProcessHeapObject(): RESTProcessPtr<std::unique_ptr<T>>(obj) {}
    RESTProcessHeapObject(std::unique_ptr<T>&& o): RESTProcessPtr<std::unique_ptr<T>>(obj), obj(std::move(o)) {}
    bool isObject() const override {return true;}
  };

  template <class T> std::shared_ptr<RESTProcessHeapObject<T>>
  makeRESTProcessHeapObject(std::unique_ptr<T>&& obj)
  {return std::make_shared<RESTProcessHeapObject<T>>(std::move(obj));}

}

namespace classdesc_access
{
  template <class T, class Enable=void> struct access_RESTProcess;
}

namespace std
{
  template <class T, size_t N>
  void advance(typename classdesc::MultiArray<T,N>::iterator& i, size_t n)
  {i+=n;}
}

#include "use_mbr_pointers.h"
CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(RESTProcess);

using classdesc::RESTProcess;       
using classdesc::RESTProcess_onbase;       

#endif
