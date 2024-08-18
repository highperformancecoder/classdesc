/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/
#ifndef RESTPROCESS_EPILOGUE_H
#define RESTPROCESS_EPILOGUE_H
#include "RESTProcess_base.h"
#include "signature.h"
#include "multiArray.h"
#include "polyPackBase.h"
#include "polyRESTProcess.h"

namespace classdesc_access
{
  namespace classdesc=cd;

  template <class T>
  struct access_RESTProcess<cd::Exclude<T>>: public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <>
  struct access_RESTProcess<cd::string>
  {
    template <class U>
    void operator()(cd::RESTProcess_t& repo, const std::string& d, U& a)
    {repo.add(d, new cd::RESTProcessObject<U>(a));}
  };

  template <class F, class S>
  struct access_RESTProcess<std::pair<F,S>>
  {
    template <class U>
    void operator()(cd::RESTProcess_t& repo, const std::string& d, U& a)
    {
      RESTProcess(repo,d+".first", a.first);
      RESTProcess(repo,d+".second", a.second);
    }
  };

  template <>
  struct access_RESTProcess<cd::PolyPackBase>: 
    public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <class T>
  struct access_RESTProcess<cd::PolyPack<T>>: 
    public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <class T>
  struct access_RESTProcess<T, cd::void_t<typename std::iterator_traits<T>::value_type>>:
    public cd::NullDescriptor<cd::RESTProcess_t> {};
}

namespace classdesc
{
  template <class T> 
  std::vector<Signature> RESTProcessObject<T>::signature() const
  {
    auto tn=typeName<T>();
    return std::vector<Signature>{{tn,{}},{tn,{tn}}};
  }

  template <class T> 
  std::vector<Signature> RESTProcessPtr<T>::signature() const
  {
    auto tn=typeName<typename T::element_type>();
    return {{tn,{}},{tn,{tn}}};
  }
  
  template <class T> 
  std::vector<Signature> RESTProcessWeakPtr<T>::signature() const
  {
    auto tn=typeName<T>();
    return {{tn,{}},{tn,{tn}}};
  }

  template <class T> 
  std::vector<Signature> RESTProcessSequence<T>::signature() const
  {
    auto tn=typeName<T>();
    return {{tn,{}},{tn,{tn}}};
  }

  template <class T> 
  std::vector<Signature> RESTProcessAssociativeContainer<T>::signature() const
  {
    auto tn=typeName<T>();
    return {{tn,{}},{tn,{tn}}};
  }

  template <class E> 
  std::vector<Signature> RESTProcessEnum<E>::signature() const
  {
    return {{"std::string",{}},{"std::string",{"std::string"}}};
  }

  template <class E> 
  std::vector<Signature> EnumerateEnumerators<E>::signature() const
  {
    return {{"vector<string>",{}}};
  }
  
  template <class T> 
  RESTProcess_t RESTProcessObject<T>::list() const
  {
    RESTProcess_t map;
    RESTProcess(map,"",obj);
    return map;
  }
  
  inline RESTProcess_t RESTProcessOverloadedFunction::list() const {return {};}

  template <class T>
  RESTProcess_t RESTProcessSequence<T>::list() const {
    RESTProcess_t map;
    map.emplace(".@elem", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessSequence<T>::elem)));
    map.emplace(".@elemNoThrow", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessSequence<T>::elemNoThrow)));
    map.emplace(".@erase", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessSequence<T>::eraseElem)));
    map.emplace(".@insert", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessSequence<T>::pushBack)));
    map.emplace(".@size", makeRESTProcessFunction(functional::bindMethod(obj,&T::size)));
    return map;
  }
  
  template <class T>
  RESTProcess_t RESTProcessAssociativeContainer<T>::list() const {
    RESTProcess_t map;
    // duplicates here for backward compatibility
    map.emplace(".@elem", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessAssociativeContainer<T>::elem)));
    map.emplace(".@elemNoThrow", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessAssociativeContainer<T>::elem)));
    map.emplace(".@insert", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessAssociativeContainer<T>::elem)));
    map.emplace(".@erase", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessAssociativeContainer<T>::erase)));
    map.emplace(".@size", makeRESTProcessFunction(functional::bindMethod(obj,&T::size)));
    map.emplace(".@keys", makeRESTProcessFunction(functional::bindMethod(*this,&RESTProcessAssociativeContainer<T>::keys)));
    return map;
  }
  
  template <class T> 
  std::string RESTProcessObject<T>::type() const {return typeName<T>();}

  
  template <class F, int N=functional::Arity<F>::value >
  struct Args: public std::vector<string>
  {
    Args(): std::vector<string>(Args<F,N-1>()) {
      push_back(typeName<typename functional::Arg<F,N>::type>());
    }
  };

  template <class F> struct Args<F,0>: public std::vector<string> {};

  
  template <class F>
  Signature RESTProcessBase::functionSignature() const
  {
    return {typeName<typename functional::Return<F>::T>(), Args<F>()};
  }

  /// descriptor for generating building REST processing registry
  template <class T>
  typename enable_if<Not<functional::is_nonmember_function_ptr<T>>, void>::T
  RESTProcess(RESTProcess_t& r, const string& d, T& a) {RESTProcessp(r,d,a);}

  template <class T>
  typename enable_if<is_classdescGenerated<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, string d, T& obj)
  {
    classdesc_access::access_RESTProcess<typename remove_const<T>::type,void>()(repo,d,obj);
    repo.add(d, new RESTProcessObject<T>(obj));
  }

  inline RPPtr RESTProcessOverloadedFunction::process(const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    // sort function overloads by best match
    auto cmp=[&](RESTProcessFunctionBase*x, RESTProcessFunctionBase*y)
    {return x->matchScore(arguments)<y->matchScore(arguments);};
    std::set<RESTProcessFunctionBase*, decltype(cmp)> sortedOverloads{cmp};
    for (auto& i: overloadedFunctions)
      if (auto j=dynamic_cast<RESTProcessFunctionBase*>(i.get()))
        sortedOverloads.insert(j);
    auto& bestOverload=*sortedOverloads.begin();
    if (bestOverload->matchScore(arguments) >=
        RESTProcessFunctionBase::maxMatchScore)
      throw std::runtime_error("No suitable matching overload found");
    if (sortedOverloads.size()>1)
      { // ambiguous overload detection
        auto i=sortedOverloads.begin(); i++;
        if ((*i)->matchScore(arguments)==bestOverload->matchScore(arguments))
          throw std::runtime_error("Ambiguous resolution of overloaded function");
      }
    return bestOverload->process(remainder, arguments);
  }

  template <class T>
  RPPtr sequenceProcess(T& obj, const string& remainder, const REST_PROCESS_BUFFER& arguments)
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
          return makeRESTProcessRef(obj);
        }
      else if (startsWith(remainder,".@size"))
        return makeRESTProcessValueObject(obj.size());
      return RESTProcess_t(obj).process(remainder,arguments); // treat as an object, not container
    }

  template <class T, bool isConst>  struct IteratorSelector;
  template <class T> struct IteratorSelector<T, true>
  {
    using iterator=typename T::const_iterator;
  };
  template <class T> struct IteratorSelector<T, false>
  {
    using iterator=typename T::iterator;
  };
  
  
  template <class T> std::tuple<typename IteratorSelector<T,is_const<T>::value>::iterator, string::size_type>
  elemImpl(T& obj, const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    // extract idx
    assert(remainder.length()>2);
    auto idxStart=remainder.find('.',1);
    if (idxStart==string::npos)
      throw std::runtime_error("no index");
    auto idxEnd=remainder.find('.', idxStart+1);
    if (idxEnd==string::npos) idxEnd=remainder.length();
    size_t idx=stoi(remainder.substr(idxStart+1, idxEnd));
    if (idx>=obj.size())
      {
        // @elemNoThrow doesn't throw out of bounds error, but may throw invalid index
        if (startsWith(remainder,".@elemNoThrow"))
          return {obj.end(),string::npos};
        throw std::runtime_error("idx out of bounds");
      }
    auto i=obj.begin();
    std::advance(i, idx);
    return {i,idxEnd};
  }
  
  template <class T> RPPtr RESTProcessSequence<T>::process(const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
      if (startsWith(remainder,".@elem"))
        {
          auto [i, idxEnd]=elemImpl(obj,remainder,arguments);
          if (i==obj.end()) return std::make_shared<RESTProcessVoid>();
          return mapAndProcess(remainder.substr(idxEnd), arguments, *i);
        }
      if (startsWith(remainder,".@insert"))
        {
          insert(obj, arguments);
          return makeRESTProcessRef(obj);
        }
      if (startsWith(remainder,".@erase"))
        {
          erase(obj, arguments);
          return makeRESTProcessRef(obj);
        }
      return sequenceProcess(obj,remainder,arguments);
  }

  template <class T> RPPtr RESTProcessMultiArray<T>::process(const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    if (startsWith(remainder,".@elem"))
      {
        auto [i, idxEnd]=elemImpl(actual,remainder,arguments);
        if (i==actual.end()) return std::make_shared<RESTProcessVoid>();
        auto r=mapAndProcess(remainder.substr(idxEnd), arguments, *i);
        if (idxEnd==remainder.length() && T::rank>1)
          // create a copy of the MultiArray iterator before it goes out of scope
          return std::make_shared<RESTProcessMultiArray<typename T::iterator::value_type>>(*i);
        return r;
      }
    return sequenceProcess(actual,remainder,arguments);
  }

  template <class T>
  RPPtr RESTProcessAssociativeContainer<T>::process(const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
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
              return makeRESTProcessValueObject(elem_of(i));
            auto eoi=elem_of(i);
            return mapAndProcess(tail, arguments, eoi);
          }
      }
    else if (startsWith(remainder,".@insert"))
      RPAC_insert(obj,arguments);
    else if (startsWith(remainder,".@erase"))
      RPAC_erase(obj,arguments);
    else if (startsWith(remainder,".@size"))
      return makeRESTProcessValueObject(obj.size());
    else if (startsWith(remainder,".@keys"))
      {
        std::vector<typename T::key_type> keys;
        for (auto& i: obj)
          keys.push_back(keyOf(i));
        return makeRESTProcessValueObject(std::move(keys));
      }
    else
      return RESTProcess_t(obj).process(remainder,arguments); // treat as an object, not container
    return std::make_shared<RESTProcessAssociativeContainer>(obj);
  }

  template <class T>
  RPPtr RESTProcessPtr<T>::process(const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    if (ptr)
      return rProcess(*ptr, remainder, arguments);
    else
      return {};
  }

  template <class E>
  RPPtr EnumerateEnumerators<E>::process
  (const string& remainder, const REST_PROCESS_BUFFER& arguments)
    {
      auto& enumerators=enum_keys<E>();
      return makeRESTProcessValueObject(std::vector<string>(enumerators.sbegin(), enumerators.send()));
    }

  
  template <class T>
  RPPtr RESTProcessWeakPtr<T>::process
  (const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    if (auto p=ptr.lock())
      return rProcess(*p, remainder, arguments);
    else
      return {};
  }

  inline RPPtr RESTProcess_t::process(const std::string& query, const REST_PROCESS_BUFFER& jin)
  {
      string cmd=query;

      if (cmd=="@enum.@list")
        {
          std::vector<string> enums;
          for (auto& i: *this)
            if (i.first.find("@enum")==0)
              enums.push_back(i.first.substr(6));
          return makeRESTProcessValueObject(std::move(enums));
        }

      for (auto cmdEnd=query.length(); ;
           cmdEnd=cmd.rfind('.'), cmd=cmd.substr(0,cmdEnd))
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
                if (tail==".@signature")
                  return makeRESTProcessValueObject(r->second->signature());
                else if (tail==".@list")
                  {
                    auto map=r->second->list();
                    std::vector<string> array;
                    for (auto& i:map)
                      if (!i.first.empty())
                        array.emplace_back(i.first);
                    return makeRESTProcessValueObject(std::move(array));
                  }
                else if (tail==".@type")
                  return makeRESTProcessValueObject(r->second->type());
                else if (cmdEnd || dynamic_cast<RESTProcessWrapperBase*>(r->second.get()))
                  return r->second->process(tail, jin);
                else
                  throw std::runtime_error("Command not found: "+query);
              }
            default:
              {
                auto r=equal_range(cmd);
                if (tail==".@signature")
                  {
                    std::vector<Signature> array;
                    for (; r.first!=r.second; ++r.first)
                      {
                        auto sig=r.first->second->signature();
                        array.insert(array.end(), sig.begin(), sig.end());
                      }
                    return makeRESTProcessValueObject(std::move(array));
                  }
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
                if (tail==".@list")
                  {
                    auto map=bestOverload->list();
                    std::vector<std::string> list;
                    for (auto& i: map)
                      if (!i.first.empty())
                        list.push_back(i.first);
                    return makeRESTProcessValueObject(list);
                  }
                if (tail==".@type")
                  return makeRESTProcessValueObject(bestOverload->type());
                return bestOverload->process(tail, jin);
              }
            }
        }
    }

   template <class T>
  RPPtr mapAndProcess(const string& query, const REST_PROCESS_BUFFER& arguments, T& a)
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

 
  template <class T> typename enable_if<And<is_class<T>, Not<is_container<T>>>,void>::T
  populateFromObj(RESTProcess_t& r, T&obj)
  {classdesc_access::access_RESTProcess<T>()(r,"",obj);}
  
  template <class T> typename enable_if<Or<Not<is_class<T>>, is_container<T>>,void>::T
  populateFromObj(RESTProcess_t& r, T&obj)
  {}
  
  template <class T> RESTProcess_t::RESTProcess_t(T&obj) {populateFromObj(*this,obj);}

  template <class T, class...A> void RESTProcess_t::addFactory
  (const string& typeName,const std::function<void(const std::string& objName)>& callback)
  {
    std::function<void(const std::string& name, A... args)> factory=
      [this,callback](const std::string& name, A... args) {
      auto rp=make_unique<RESTProcessHeapObject<T>>();
      rp->obj=make_unique<T>(std::forward<A>(args)...);
      add(name,rp.release());
      callback(name);
    };
    
    add(typeName, new RESTProcessFunction<decltype(factory),void>(factory));
  }

  template <class T>
  typename enable_if<
    And<
      Not<is_container<T>>,
      Not<is_smart_ptr<T>>
      >, RPPtr>::T makeRESTProcessRef(T& obj)
  {return std::make_shared<RESTProcessObject<T>>(obj);}

  template <class T>
  typename enable_if<
    And<
      is_sequence<T>,
      Not<is_base_of<MultiArrayBase,T>>
      >,  RPPtr>::T
  makeRESTProcessRef(T& obj)
  {return std::make_shared<RESTProcessSequence<T>>(obj);}

  template <class T>
  typename enable_if<is_base_of<MultiArrayBase,T>, RPPtr>::T
  makeRESTProcessRef(T& obj)
  {return std::make_shared<RESTProcessMultiArray<T>>(obj);}

  template <class T>
  typename enable_if<is_smart_ptr<T>, RPPtr>::T
  makeRESTProcessRef(T& obj)
  {return std::make_shared<RESTProcessPtr<T>>(obj);}
}

namespace classdesc_access
{
  namespace cd=classdesc;
#if defined(__cplusplus) && __cplusplus>=201103L
  template <class F>
  struct access_RESTProcess<std::function<F>>
  {
    void operator()(cd::RESTProcess_t& r, const cd::string& d, const std::function<F>& a)
    {r.add(d, new cd::RESTProcessFunction<std::function<F>>(a));}
  };
#endif

#ifdef JSON_PACK_BASE_H
  template <>
  struct access_RESTProcess<cd::json_pack_t>: public cd::NullDescriptor<cd::RESTProcess_t> {};
  //  template <>
//  struct access_RESTProcess<cd::RESTProcessBase>: public cd::NullDescriptor<cd::RESTProcess_t> {};
//  template <>
//  struct access_json_pack<cd::RESTProcessBase>: public cd::NullDescriptor<cd::json_pack_t> {};
//  template <>
//  struct access_json_unpack<cd::RESTProcessBase>: public cd::NullDescriptor<cd::json_unpack_t> {};
#endif
  
#ifdef OBJECT_H
  template <>
  struct access_RESTProcess<cd::object>: public cd::NullDescriptor<cd::RESTProcess_t> {};
#endif
  
}
    

#ifdef CLASSDESC_MULTIARRAY_H
#include "multiArray-RESTProcess.cd"
#endif

#ifdef CLASSDESC_OBJECT_H
#include "object-RESTProcess.cd"
#endif

#ifdef CLASSDESC_POLY_BASE_H
#include "polyBase-RESTProcess.cd"
#endif

#ifdef CLASSDESC_POLYRESTPROCESSBASE_H
#include "polyRESTProcessBase-RESTProcess.cd"
#endif

#ifdef CLASSDESC_POLYRESTPROCESS_H
#include "polyRESTProcess-RESTProcess.cd"
#endif

#ifdef CLASSDESC_SIGNATURE_H
#include "signature-RESTProcess.cd"
#endif


#endif
