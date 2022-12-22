#ifndef CLASSDESC_FUNCTIONDB1
#define CLASSDESC_FUNCTIONDB1
template <class R, class A1> 
struct Arg<R (*)(A1), 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1> 
struct Arg<R (C::*)(A1), 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1> 
struct Arg<R (C::*)(A1) const, 1> 
{
  typedef A1 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1> 
struct Arg<R (*)(A1) noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1> 
struct Arg<R (C::*)(A1) noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1> 
struct Arg<R (C::*)(A1) const noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};
#endif
template <class F, template<class> class P>
struct AllArgs<F,P,1>
{
   static const bool value=true   && P<typename Arg<F,1>::T>::value;
};


template <class R, class A1> 
struct Arity<R (*)(A1)> 
{
    static const int V=1;
    static const int value=1;
};

template <class R, class A1> 
struct Return<R (*)(A1)> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R, class A1> 
struct Arity<R (* C::*)(A1)> 
{
    static const int V=1;
    static const int value=1;
};

template <class C,class R, class A1> 
struct Return<R (* C::*)(A1)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1> 
struct Arity<R (C::*)(A1)> 
{
    static const int V=1;
    static const int value=1;
};

template <class C, class R, class A1> 
struct Return<R (C::*)(A1)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1> 
struct Arity<R (C::*)(A1) const> 
{
    static const int V=1;
    static const int value=1;
};

template <class C, class R, class A1> 
struct Return<R (C::*)(A1) const> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1> 
struct ClassOf<R (C::*)(A1)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1> 
struct ClassOf<R (*C::*)(A1)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1> 
struct ClassOf<R (C::*)(A1) const> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1> 
struct is_member_function_ptr<R (C::*)(A1)>
{
   static const bool value=true;
};

template <class C, class R, class A1> 
struct is_member_function_ptr<R (C::*)(A1) const>
{
   static const bool value=true;
};

template <class C, class R, class A1> 
struct is_const_method<R (C::*)(A1) const>
{
   static const bool value=true;
};

template <class R, class A1> 
struct is_nonmember_function_ptr<R (*)(A1)>
{
   static const bool value=true;
};

template <class C, class R, class A1> 
struct is_nonmember_function_ptr<R (*C::*)(A1)>
{
   static const bool value=true;
};

template <class C, class D, class R, class A1>
class bound_method<C, R (D::*)(A1)>
{
    typedef R (D::*M)(A1);
    C* obj;
    M method;
    public:
    static const int arity=1;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()(A1 a1) const {return (obj->*method)(a1);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1>
class bound_method<const C, R (D::*)(A1)>
{
    typedef R (D::*M)(A1);
    const C* obj;
    M method;
    public:
    static const int arity=1;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()(A1 a1) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1>
class bound_method<C, R (D::*)(A1) const>
{
    typedef R (D::*M)(A1) const;
    C& obj;
    M method;
    public:
    static const int arity=1;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()(A1 a1) const {return (obj.*method)(a1);}
    static const bool is_const=true;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods

template <class R, class A1> 
struct Arity<R (*)(A1) noexcept> 
{
    static const int V=1;
    static const int value=1;
};

template <class R, class A1> 
struct Return<R (*)(A1) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R, class A1> 
struct Arity<R (* C::*)(A1) noexcept> 
{
    static const int V=1;
    static const int value=1;
};

template <class C,class R, class A1> 
struct Return<R (* C::*)(A1) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1> 
struct Arity<R (C::*)(A1) noexcept> 
{
    static const int V=1;
    static const int value=1;
};

template <class C, class R, class A1> 
struct Return<R (C::*)(A1) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1> 
struct Arity<R (C::*)(A1) const noexcept> 
{
    static const int V=1;
    static const int value=1;
};

template <class C, class R, class A1> 
struct Return<R (C::*)(A1) const noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1> 
struct ClassOf<R (C::*)(A1) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1> 
struct ClassOf<R (*C::*)(A1) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1> 
struct ClassOf<R (C::*)(A1) const noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1> 
struct is_member_function_ptr<R (C::*)(A1) noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1> 
struct is_member_function_ptr<R (C::*)(A1) const noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1> 
struct is_const_method<R (C::*)(A1) const noexcept>
{
   static const bool value=true;
};

template <class R, class A1> 
struct is_nonmember_function_ptr<R (*)(A1) noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1> 
struct is_nonmember_function_ptr<R (*C::*)(A1) noexcept>
{
   static const bool value=true;
};

template <class C, class D, class R, class A1>
class bound_method<C, R (D::*)(A1) noexcept>
{
    typedef R (D::*M)(A1);
    C* obj;
    M method;
    public:
    static const int arity=1;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()(A1 a1) const {return (obj->*method)(a1);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1>
class bound_method<C, void (D::*)(A1) noexcept>
{
    typedef void (D::*M)(A1);
    C* obj;
    M method;
    public:
    static const int arity=1;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1) const {(obj->*method)(a1);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1>
class bound_method<const C, R (D::*)(A1) noexcept>
{
    typedef R (D::*M)(A1);
    const C* obj;
    M method;
    public:
    static const int arity=1;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()(A1 a1) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1>
class bound_method<const C, void (D::*)(A1) noexcept>
{
    typedef void (D::*M)(A1);
    const C* obj;
    M method;
    public:
    static const int arity=1;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1>
class bound_method<C, R (D::*)(A1) const noexcept>
{
    typedef R (D::*M)(A1) const;
    C& obj;
    M method;
    public:
    static const int arity=1;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()(A1 a1) const {return (obj.*method)(a1);}
    static const bool is_const=true;
};

template <class C, class D, class A1>
class bound_method<C, void (D::*)(A1) const noexcept>
{
    typedef void (D::*M)(A1) const;
    C& obj;
    M method;
    public:
    static const int arity=1;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()(A1 a1) const {(obj.*method)(a1);}
    static const bool is_const=true;
};
#endif

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 1> >, 
   typename Return<F>::T>::T
apply_nonvoid_fn(F f, Args& a, Fdummy<F> dum=0)
{
  return f(a[0]);
}

/*
 TODO: if any of the arguments to f are lvalues, we need to construct temporaries,
 which require C++-11 ability to test for the existence of a copy constructor. 
 If the return type is not copy constructable, the user must arrange for the return value to be discarded
*/

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 1> >,
    void>::T
apply_void_fn(F f, Args& a, Fdummy<F> dum=0)
{
  f(a[0]);
}

template <class Buffer, class F>
typename enable_if<Eq<Arity<F>::value, 1>, typename Return<F>::T>::T
callOnBuffer(Buffer& buffer, F f)
{
  typename remove_const<typename remove_reference<typename Arg<F,1>::T>::type>::type a1;
  buffer>>a1;
  return f(
a1
  );
}
#endif
