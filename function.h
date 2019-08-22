/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/**\file
  \brief Metaprogramming support for processing functions of multiple arguments
*/

#ifndef FUNCTION_H
#define FUNCTION_H
#include "classdesc.h"
#include <string>

namespace classdesc
{

  /**
     \namespace classdesc::functional \brief contains code generated by
     functiondb.sh that defines functional attributes.
  */
  namespace functional
  {

    // these classes have either a const static member V, or a type T
    /// \c Arity::V (or ::value) is the number of arguments of
    //function object \a F
    template <class F> struct Arity;
    /// \c Return::T (or ::type) is the return type of \a F
    template <class F> struct Return;


    
    /** \c Arg<F,i> is the type of argument \a i of \a F, i=1..Arity<F> */
    template <class F, size_t> struct Arg;

    /** \c ClassOf<F> returns the class type that F is a member function of. Returns F if F is not a class */
    template <class F> struct ClassOf
    {typedef F T; typedef F type;};
    template <class C, class U> struct ClassOf<U C::*>
    {typedef C T; typedef C type;};

    /// \c is_member_function_ptr::value is true if \a F is a member function pointer
    // note - this basically duplicates std::is_member_function_pointer
    template <class F> struct is_member_function_ptr
    {static const bool value=false;};

    /// \c is_const_method::value is true if F is a pointer to a const method
    template <class F> struct is_const_method
    {static const bool value=false;};

    /// \c is_nonmember_function_ptr::value is true if \a F is an ordinary function pointer (maybe a member)
    template <class F> struct is_nonmember_function_ptr
    {static const bool value=false;};
    /// is_member_function_ptr<F>||is_nonmember_function_ptr<F>
    /// note this is semantically different from std::is_function
    template <class F> struct is_function
    {
      static const bool value=is_member_function_ptr<F>::value||
      is_nonmember_function_ptr<F>::value;
    };


    

    /// @{ apply metaprogramming predicate to all arguments of a
    /// functional, and reduce by && or ||
    template <class F, template<class> class Pred, int arity=Arity<F>::value> struct AllArgs;
//    template <class F, template<class> class Pred> struct AnyArg
//    {
//      static const bool value=!AllArgs<F,Not<Pred> >::value;
//    };
   /// @}

    template <class C, class M, class R=typename Return<M>::T> class bound_method;

    template <class O, class M>
    bound_method<O,M> bindMethod(O& o, M m)
    {return bound_method<O,M>(o,m);}


    template <class T> struct Fdummy {Fdummy(int) {} };


    /**
       apply function f to A arguments. Called from \c apply

       Function definitions below left for documentation purposes, but
       commented out to cause hard compilation failures when F is not
       supported (eg has lvalue arguments)

    template <class F, class Args> inline
    typename Return<F>::T 
    apply_nonvoid_fn(F f, const Args& args, Fdummy<F> dum=0);

    template <class F, class Args>  inline
    void apply_void_fn(F f, const Args& args, Fdummy<F> dum=0);
    */

#if defined(__cplusplus) && __cplusplus>=201103L
    //#if 0
    template <class... Args> struct ArityArgs;
    
    template <class A, class... Args>
    struct ArityArgs<A, Args...>
    {
      static const size_t value=ArityArgs<Args...>::value+1;
    };

    template <>
    struct ArityArgs<>
    {
      static const size_t value=0;
    };

    template <size_t N, class A, class... Args>
    struct ArgOf
    {
      typedef typename ArgOf<N-1,Args...>::T T;
    };

    template <class A, class... Args>
    struct ArgOf<1,A,Args...>
    {
      typedef A T;
    };

    template <template<class> class P, class... Args> struct AllArgsHelper;
    template <template<class> class P, class A, class... Args> struct AllArgsHelper<P,A,Args...>
    {
      static const bool value=P<A>::value && AllArgsHelper<P,Args...>::value;
    };

    template <template<class> class P> struct AllArgsHelper<P>
    {
      static const bool value=true;
    };
    
    template <class R, class... Args> struct FunctionalHelper
    {
      static const size_t arity=ArityArgs<Args...>::value;
      typedef R Return;
      template <size_t N> struct Arg
      {
        typedef typename ArgOf<N,Args...>::T T;
      };
      template <template<class> class P> struct AllArgs
      {
        typedef AllArgsHelper<P,Args...> T;
      };
    };

    /// argumentless specialisation
    template <class R> struct FunctionalHelper<R>
    {
      static const size_t arity=0;
      typedef R Return;
      template <size_t N> struct Arg
      {
        typedef void T;
      };
      template <template<class> class P> struct AllArgs
      {
        typedef AllArgsHelper<P> T;
      };
    };

    template <class F> struct FunctionalHelperFor;
    // raw functions
    template <class R, class... Args> struct FunctionalHelperFor<R(Args...)>
    {
      typedef FunctionalHelper<R,Args...> T;
    };
    // function pointers
    template <class R, class... Args> struct FunctionalHelperFor<R(*)(Args...)>
    {
      typedef FunctionalHelper<R,Args...> T;
    };
    // method pointers
    template <class R, class C, class... Args> struct FunctionalHelperFor<R(C::*)(Args...)>
    {
      typedef FunctionalHelper<R,Args...> T;
    };
    template <class R, class C, class... Args> struct FunctionalHelperFor<R(*C::*)(Args...)>
    {
      typedef FunctionalHelper<R,Args...> T;
    };
    template <class R, class C, class... Args> struct FunctionalHelperFor<R(C::*)(Args...) const>
    {
      typedef FunctionalHelper<R,Args...> T;
    };
    /// member object pointers
    template <class R, class C> struct FunctionalHelperFor<R(C::*)>
    {
      typedef FunctionalHelper<R> T;
    };

    // doesn't seem to work, alas...
    //    template <class F> struct FunctionalHelperFor:
    //      public FunctionalHelperFor<decltype(F::operator())> {};
    
      

    // lambdas
//    template <class R, class... Args> struct FunctionalHelperFor<decltype([](Args...)->R)>
//    {
//      typedef typename FunctionalHelper<R,Args...>::T T;
//    };

    template <class F> struct Arity {
      typedef typename FunctionalHelperFor<F>::T Helper;
      static const size_t V=Helper::arity;
      static const size_t value=Helper::arity;
    };

    template <class F> struct Return
    {
      typedef typename FunctionalHelperFor<F>::T Helper;
      typedef typename Helper::Return T;
      typedef T type;
    };
    template <class F, size_t N> struct Arg
    {
      typedef typename FunctionalHelperFor<F>::T Helper;
      typedef typename Helper::template Arg<N>::T T;
      typedef T type;
    };

    template <class R, class C, class... Args>
    struct ClassOf<R (C::*)(Args...)>
    {
      typedef C T;
      typedef C type;
    };

    template <class R, class C, class... Args>
    struct ClassOf<R (*C::*)(Args...)>
    {
      typedef C T;
      typedef C type;
    };

    template <class R, class C, class... Args>
    struct ClassOf<R (C::*)(Args...) const>
    {
      typedef C T;
      typedef C type;
    };

    template <class C, class R, class... Args> 
    struct is_member_function_ptr<R (C::*)(Args...)>
    {
      static const bool value=true;
    };

    template <class C, class R, class... Args> 
    struct is_member_function_ptr<R (C::*)(Args...) const>
    {
      static const bool value=true;
    };

    template <class C, class R, class... Args> 
    struct is_const_method<R (C::*)(Args...) const>
    {
      static const bool value=true;
    };

    template <class R, class... Args> 
    struct is_nonmember_function_ptr<R (*)(Args...)>
    {
      static const bool value=true;
    };

    template <class C, class R, class... Args> 
    struct is_nonmember_function_ptr<R (*C::*)(Args...)>
    {
      static const bool value=true;
    };

    template <class C, class M, class R>
    class bound_method
    {
      C* obj;
      M method;
    public:
      bound_method(C& obj, M method): obj(&obj), method(method) {}
      template <class... Args>
      R operator()(Args... args) const {return (obj->*method)(args...);}
      void rebind(C& newObj) {obj=&newObj;}
    };

    template <class C, class M>
    class bound_method<C, M, void>
    {
      C* obj;
      M method;
    public:
      bound_method(C& obj, M method): obj(&obj), method(method) {}
      template <class... Args>
      void operator()(Args... args) const {(obj->*method)(args...);}
      void rebind(C& newObj) {obj=&newObj;}
    };

    template <class C, class F> struct FunctionalHelperFor<bound_method<C,F>>
    {
      typedef typename FunctionalHelperFor<F>::T T;
    };

    template <class F, template<class> class P, int N> /*N not actually used anywhere...*/
    struct AllArgs
    {
      typedef typename FunctionalHelperFor<F>::T Helper;
      typedef typename Helper::template AllArgs<P>::T AllArgsHelper;
      static const bool value=AllArgsHelper::value;
    };
    
    template <class F, class ArgVector, size_t N=Arity<F>::value>
    struct CurryLastNonVoid;

    template <class F, class ArgVector, size_t N>
    struct CurryLastNonVoid
    {
      F f;
      ArgVector& a;
      CurryLastNonVoid(F f, ArgVector& a): f(f), a(a) {} 
      template <class... Args>
      typename Return<F>::T operator()(Args... args) const {
        return f(args...,a[Arity<F>::value-1]);
      }
      
      typename Return<F>::T apply()
      {return CurryLastNonVoid<CurryLastNonVoid, ArgVector>(*this, a).apply();}
    };
      
    template <class F, class ArgVector>
    struct CurryLastNonVoid<F,ArgVector,0>
    {
      F f;
      ArgVector& a;
      CurryLastNonVoid(F f, ArgVector& a): f(f), a(a) {} 
      typename Return<F>::T operator()() const {return f();}
      typename Return<F>::T apply() const {return f();}
    };
      
    template <class F, class ArgVector,size_t N>
    struct Return<CurryLastNonVoid<F,ArgVector,N>>
    {
      typedef typename Return<F>::T T;
    };
    
    template <class F, class ArgVector, size_t N>
    struct Arity<CurryLastNonVoid<F,ArgVector,N>>
    {
      static const size_t value=Arity<F>::value-1;
    };
    
    template <class F, class ArgVector, size_t N=Arity<F>::value>
    struct CurryLastVoid
    {
      F f;
      ArgVector& a;
      CurryLastVoid(F f, ArgVector& a): f(f), a(a) {} 
      template <class... Args>
      void operator()(Args... args) const {
        f(args...,a[Arity<F>::value-1]);
      }
      
      void apply()
      {CurryLastVoid<CurryLastVoid, ArgVector>(*this, a).apply();}
    };
      
    template <class F, class ArgVector>
    struct CurryLastVoid<F,ArgVector,0>
    {
      F f;
      ArgVector& a;
      CurryLastVoid(F f, ArgVector& a): f(f), a(a) {} 
      void operator()() const {f();}
      void apply() const {f();}
    };
      
    template <class F, class ArgVector>
    struct Arity<CurryLastVoid<F,ArgVector>>
    {
      static const size_t value=Arity<F>::value-1;
    };
    
    template <class F, class Args> 
    typename enable_if<AllArgs<F, is_rvalue>, typename Return<F>::T>::T
    apply_nonvoid_fn(F f, Args& a)
    {
      return CurryLastNonVoid<F,Args>(f,a).apply();
    }


    template <class F, class Args> 
    typename enable_if<AllArgs<F, is_rvalue>, typename Return<F>::T>::T
    apply_void_fn(F f, Args& a)
    {
      CurryLastVoid<F,Args>(f,a).apply();
    }

    /** 
        helper classes to serialise/deserialise a function call to
        assist with command patterns 

        Use like
        pack_t buf;
        PackFunctor(buf)(a, b);
        ...
        Functor object;
        UnpackAndCall(buf)(object); 

*/
    template <class F, class R=typename Return<F>::T,
              class A=typename Arg<F,1>::T>
    class CurryFirst;

    template <class F, class R, class A, size_t I>
    struct Arg<CurryFirst<F,R,A>,I>
    {
      typedef typename Arg<F,I+1>::T T;
      typedef T type;
    };

    template <class F, class R, class A>
    struct Return<CurryFirst<F,R,A> >
    {
      typedef R T;
      typedef T type;
    };
    
    template <class F, class R, class A>
    struct Arity<CurryFirst<F,R,A> >
    {
      const int V=Arity<F>::V-1;
      const int value=V;
    };

    
    template <class F, class R, class A>
    class CurryFirst
    {
      F f;
      A& a;
    public:
      CurryFirst(F f, A& a): f(f), a(a) {}
      template <class... Args>
      R operator()(Args... args) {
        return f(std::forward<A>(a),std::forward<Args>(args)...);
      }
    };

    template <class F, class A>
    class CurryFirst<F,void,A>
    {
      F f;
      A& a;
    public:
      CurryFirst(F f, A& a): f(f), a(a) {}
      template <class... Args>
      void operator()(Args... args) {
        f(std::forward<A>(a),std::forward<Args>(args)...);
      }
    };

    template <class F, class R, class A>
    struct FunctionalHelperFor<CurryFirst<F,R,A>>:
      public FunctionalHelperFor<F>
    {
      template <size_t N> struct Arg: public functional::Arg<F,N+1> {};
    };

    /// polymorphic type representing the result of functor call
    template <class R> struct Result;
    struct ResultBase
    {
      virtual ~ResultBase() {}
      template <class R>
      R getResult() const {
        if (is_void<R>::value)
          throw std::runtime_error("void result");          
        if (auto s=dynamic_cast<const Result<R>*>(this))
          return **s;
        else
          throw std::runtime_error("Incorrect result type");
      }
        
    };
    template <class R> struct Result: public ResultBase
    {
      R r;
      Result(const R& r): r(r) {}
      const R& operator*() const {return r;}
      R&& move() {return std::move(r);}
    };
    template <> struct Result<void>: public ResultBase
    {};

    template <class Buffer, class F, class R=typename Return<F>::T,
              int N=Arity<F>::value> class CallOnBuffer;

    /// extract an argument from buffer \a b, and run functional f on it 
    template <class F, class A, class R, class B>
    typename enable_if<is_default_constructible<A>, R>::T
    eval(F f, B& b)
    {
      A a;
      b>>a;
      return f(a);
    }
    
    template <class F, class A, class R, class B>
    typename enable_if<Not<is_default_constructible<A>>, R>::T
    eval(F f, B& b)
    {
      throw std::runtime_error("unable to unpack into "+typeName<A>());
    }

    template <class F, class A, class B>
    typename enable_if<is_default_constructible<A>, void>::T
    evalVoid(F f, B& b)
    {
      A a;
      b>>a;
      f(a);
    }
    
    template <class F, class A, class B>
    typename enable_if<Not<is_default_constructible<A>>, void>::T
    evalVoid(F f, B& b)
    {eval<F,A,void,B>(f,b);}
    
    template <class Buffer, class F, class R, int N>
    class CallOnBuffer
    {
      Buffer& buffer;
      F f;
      typedef typename remove_const
          <typename remove_reference
           <typename Arg<F,1>::T>::type>::type A1;
    public:
      CallOnBuffer(Buffer& buffer, F f): buffer(buffer), f(f) {}
      Result<R> operator()() {
        auto ff=[&](A1& a){return CallOnBuffer<Buffer, CurryFirst<F>, R, N-1>
            (buffer, CurryFirst<F>(f,a))();};
        return std::move(eval<decltype(ff),A1,Result<R>,Buffer>(ff, buffer));
       }
     };

    template <class Buffer, class F, class R>
    class CallOnBuffer<Buffer,F,R,0>
    {
      F f;
    public:
      CallOnBuffer(Buffer& buffer, F f): f(f) {}
      Result<R> operator()() {return f();}
    };


    
    template <class Buffer, class F, int N>
    class CallOnBuffer<Buffer,F,void,N>
    {
      Buffer& buffer;
      F f;
      typedef typename remove_const
          <typename remove_reference
           <typename Arg<F,1>::T>::type>::type A1;
    public:
      CallOnBuffer(Buffer& buffer, F f): buffer(buffer), f(f) {}
      Result<void> operator()() {
        auto ff=[&](A1& a)
                {CallOnBuffer<Buffer, CurryFirst<F>, void, N-1>
                    (buffer, CurryFirst<F>(f,a))();};
        evalVoid<decltype(ff),A1,Buffer>(ff,buffer);
        return {};
      }
    };

    template <class Buffer, class F>
    class CallOnBuffer<Buffer,F,void,1>
    {
      Buffer& buffer;
      F f;
      typedef typename remove_const
          <typename remove_reference
           <typename Arg<F,1>::T>::type>::type A1;
    public:
      CallOnBuffer(Buffer& buffer, F f): buffer(buffer), f(f) {}
      Result<void> operator()() {
        evalVoid<F,A1,Buffer>(f, buffer);
        return {};
      }
    };

    template <class Buffer, class F>
    class CallOnBuffer<Buffer,F,void,0>
    {
      F f;
    public:
      CallOnBuffer(Buffer& buffer, F f): f(f) {}
      Result<void> operator()() {
        f();
        return Result<void>();        
      }
    };

    template <class Buffer>
    class PackFunctor: public Buffer
    {
    public:
      /// a Buffer needs to support << operations for arbitrary rhs types
      template <class... Args> PackFunctor(Args... args):
        Buffer(std::forward<Args>(args)...) {}

      template <class F, class... Args>
      void pack(Args... args)
      {packArg<F,1>(args...);}
      /// overload that accepts argument list as is, rather than casting to a signature
      template <class... Args>
      void pack(Args... args)
      {packArg<void(Args...),1>(args...);}
      template <class F, int N, class A, class... Args>
      void packArg(A a, Args... args)
      {
        (*this) << typename Arg<F,N>::T(a);
        packArg<F,N+1>(args...);
      }
      template <class F, int N>
      void packArg() {}

      template <class F>
      typename enable_if<Not<is_void<typename Return<F>::T>>,
                         typename remove_reference<typename Return<F>::T>::type>::T
      call(F f) {
        return CallOnBuffer<Buffer, F>(*this,f)().move();
      }
      
      template <class F>
      typename enable_if<is_void<typename Return<F>::T>,void>::T
      call(F f) {
        CallOnBuffer<Buffer, F>(*this,f)();
      }
      
    };

    template <class Class, class Buffer>
    class CallMethodOnBufferBase
    {
    public:
      virtual std::unique_ptr<ResultBase> call(Buffer&, Class&)=0;
      template <class M> static
      std::unique_ptr<CallMethodOnBufferBase> makeCallMethodOnBuffer(M m);
    };

    template <class Class, class Buffer, class M>
    class CallMethodOnBuffer: public CallMethodOnBufferBase<Class,Buffer>
    {
      M m;
      typedef typename Return<M>::T R;
    public:
      CallMethodOnBuffer(M m): m(m) {}
      std::unique_ptr<ResultBase> call(Buffer& buf, Class& o) {
        auto bm=bindMethod(o,m);
        return make_unique<Result<R>>(CallOnBuffer<Buffer,decltype(bm)>(buf,bm)());
      }
    };
    
    template <class Class, class Buffer, class M> static
    std::unique_ptr<CallMethodOnBufferBase<Class,Buffer>>
    makeCallMethodOnBuffer(M m)
    {return new CallMethodOnBuffer<Class,Buffer,M>(m);}

    /// return a binary array object that represents a method pointer
    typedef void (is_array::*MethodPtrExemplar)();
    typedef std::array<char,sizeof(MethodPtrExemplar)> MethodBin;
    template <class M>
    MethodBin& methodBin(M& m) {
      static_assert(sizeof(MethodBin)==sizeof(M),"");
      return reinterpret_cast<MethodBin&>(m);
    }
    
    template <class Class, class Buffer>
    class CallMethods: public std::vector<std::unique_ptr<CallMethodOnBufferBase<Class,Buffer>>>
    {
      std::map<MethodBin,size_t> indexMap;
      template <class M, class... Args>
      void init(M m, Args... args) {
        indexMap.emplace(methodBin(m),this->size());
        this->emplace_back(new CallMethodOnBuffer<Class,Buffer,M>(m));
        init(args...);
      }
      void init() {}
    public:
      template <class A, class... Args>
      CallMethods(A a, Args... args)  {init(a,args...);}
      template <class M>
      size_t indexOf(M m) {
        auto i=indexMap.find(methodBin(m));
        if (i==indexMap.end())
          { //TODO should we be adding this, or just throwing?
            i=indexMap.emplace(methodBin(m),this->size()).first;
            this->emplace_back(new CallMethodOnBuffer<Class,Buffer,M>(m));
          }
        return i->second;
      }
    };

    template <class Class, class Buffer>
    struct MethodPackFunctor: public PackFunctor<Buffer>
    {
      CallMethods<Class,Buffer> methods;
      template <class... Args>
      MethodPackFunctor(Args... args): methods(args...) {}
      template <class M, class... Args>
      void operator()(M m, Args... args) {
        (*this)<<methods.indexOf(m);
        this->template pack<M,Args...>(args...);
      }
      std::unique_ptr<ResultBase> invoke(Class& object)
      {
        size_t methodIdx;
        (*this)>>methodIdx;
        if (methodIdx>=methods.size())
          throw std::runtime_error("invalid method");
        return methods[methodIdx]->call(*this,object);
      }
    };
    
#else
    /// legacy code supporting pre-modern C++ compilers.
    
    /// overloaded for member object pointers
    template <class R, class C> struct Return<R (C::*)>
    {typedef R T; typedef R type;};

    template <class C, class M> 
    struct Arity<bound_method<C,M> >
    {
      static const int V=Arity<M>::V;
      static const int value=V;
    };

    template <class C, class M> 
    struct Return<bound_method<C,M> >
    {
      typedef typename Return<M>::T T;
      typedef T type;
    };

    template <class C, class M, size_t i> 
    struct Arg<bound_method<C,M>,i>
    {
      typedef typename Arg<M,i>::T T;
      typedef T type;
    };

#include "functiondb.h"
#endif
        
    template <class F, class Args>
    typename enable_if< 
      Not<is_void<typename Return<F>::T> >,
      void
    >::T
    apply(typename Return<F>::T* r, F f, const Args& args, dummy<0> d=0)
    {*r=apply_nonvoid_fn(f,args);}
  
    template <class F, class Args>
    typename enable_if<is_void<typename Return<F>::T>, void>::T
    apply(void* r, F f, Args& args, dummy<1> d=0)
    {apply_void_fn(f,args);}
  }

}
#endif
