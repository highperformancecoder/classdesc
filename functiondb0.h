#ifndef CLASSDESC_FUNCTIONDB0
#define CLASSDESC_FUNCTIONDB0
template <class F, template<class> class P>
struct AllArgs<F,P,0>
{
   static const bool value=true ;
};


template <class R> 
struct Arity<R (*)()> 
{
    static const int V=0;
    static const int value=0;
};

template <class R> 
struct Return<R (*)()> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R> 
struct Arity<R (* C::*)()> 
{
    static const int V=0;
    static const int value=0;
};

template <class C,class R> 
struct Return<R (* C::*)()> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R> 
struct Arity<R (C::*)()> 
{
    static const int V=0;
    static const int value=0;
};

template <class C, class R> 
struct Return<R (C::*)()> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R> 
struct Arity<R (C::*)() const> 
{
    static const int V=0;
    static const int value=0;
};

template <class C, class R> 
struct Return<R (C::*)() const> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R> 
struct ClassOf<R (C::*)()> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R> 
struct ClassOf<R (*C::*)()> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R> 
struct ClassOf<R (C::*)() const> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R> 
struct is_member_function_ptr<R (C::*)()>
{
   static const bool value=true;
};

template <class C, class R> 
struct is_member_function_ptr<R (C::*)() const>
{
   static const bool value=true;
};

template <class C, class R> 
struct is_const_method<R (C::*)() const>
{
   static const bool value=true;
};

template <class R> 
struct is_nonmember_function_ptr<R (*)()>
{
   static const bool value=true;
};

template <class C, class R> 
struct is_nonmember_function_ptr<R (*C::*)()>
{
   static const bool value=true;
};

template <class C, class D, class R>
class bound_method<C, R (D::*)()>
{
    typedef R (D::*M)();
    C* obj;
    M method;
    public:
    static const int arity=0;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()() const {return (obj->*method)();}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D>
class bound_method<C, void (D::*)()>
{
    typedef void (D::*M)();
    C* obj;
    M method;
    public:
    static const int arity=0;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()() const {(obj->*method)();}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R>
class bound_method<const C, R (D::*)()>
{
    typedef R (D::*M)();
    const C* obj;
    M method;
    public:
    static const int arity=0;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()() const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D>
class bound_method<const C, void (D::*)()>
{
    typedef void (D::*M)();
    const C* obj;
    M method;
    public:
    static const int arity=0;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()() const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R>
class bound_method<C, R (D::*)() const>
{
    typedef R (D::*M)() const;
    C& obj;
    M method;
    public:
    static const int arity=0;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()() const {return (obj.*method)();}
    static const bool is_const=true;
};

template <class C, class D>
class bound_method<C, void (D::*)() const>
{
    typedef void (D::*M)() const;
    C& obj;
    M method;
    public:
    static const int arity=0;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()() const {(obj.*method)();}
    static const bool is_const=true;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods

template <class R> 
struct Arity<R (*)() noexcept> 
{
    static const int V=0;
    static const int value=0;
};

template <class R> 
struct Return<R (*)() noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R> 
struct Arity<R (* C::*)() noexcept> 
{
    static const int V=0;
    static const int value=0;
};

template <class C,class R> 
struct Return<R (* C::*)() noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R> 
struct Arity<R (C::*)() noexcept> 
{
    static const int V=0;
    static const int value=0;
};

template <class C, class R> 
struct Return<R (C::*)() noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R> 
struct Arity<R (C::*)() const noexcept> 
{
    static const int V=0;
    static const int value=0;
};

template <class C, class R> 
struct Return<R (C::*)() const noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R> 
struct ClassOf<R (C::*)() noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R> 
struct ClassOf<R (*C::*)() noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R> 
struct ClassOf<R (C::*)() const noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R> 
struct is_member_function_ptr<R (C::*)() noexcept>
{
   static const bool value=true;
};

template <class C, class R> 
struct is_member_function_ptr<R (C::*)() const noexcept>
{
   static const bool value=true;
};

template <class C, class R> 
struct is_const_method<R (C::*)() const noexcept>
{
   static const bool value=true;
};

template <class R> 
struct is_nonmember_function_ptr<R (*)() noexcept>
{
   static const bool value=true;
};

template <class C, class R> 
struct is_nonmember_function_ptr<R (*C::*)() noexcept>
{
   static const bool value=true;
};

template <class C, class D, class R>
class bound_method<C, R (D::*)() noexcept>
{
    typedef R (D::*M)();
    C* obj;
    M method;
    public:
    static const int arity=0;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()() const {return (obj->*method)();}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D>
class bound_method<C, void (D::*)() noexcept>
{
    typedef void (D::*M)();
    C* obj;
    M method;
    public:
    static const int arity=0;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()() const {(obj->*method)();}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R>
class bound_method<const C, R (D::*)() noexcept>
{
    typedef R (D::*M)();
    const C* obj;
    M method;
    public:
    static const int arity=0;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()() const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D>
class bound_method<const C, void (D::*)() noexcept>
{
    typedef void (D::*M)();
    const C* obj;
    M method;
    public:
    static const int arity=0;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()() const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R>
class bound_method<C, R (D::*)() const noexcept>
{
    typedef R (D::*M)() const;
    C& obj;
    M method;
    public:
    static const int arity=0;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()() const {return (obj.*method)();}
    static const bool is_const=true;
};

template <class C, class D>
class bound_method<C, void (D::*)() const noexcept>
{
    typedef void (D::*M)() const;
    C& obj;
    M method;
    public:
    static const int arity=0;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()() const {(obj.*method)();}
    static const bool is_const=true;
};
#endif

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 0> >, 
   typename Return<F>::T>::T
apply_nonvoid_fn(F f, Args& a, Fdummy<F> dum=0)
{
  return f();
}

/*
 TODO: if any of the arguments to f are lvalues, we need to construct temporaries,
 which require C++-11 ability to test for the existence of a copy constructor. 
 If the return type is not copy constructable, the user must arrange for the return value to be discarded
*/

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 0> >,
    void>::T
apply_void_fn(F f, Args& a, Fdummy<F> dum=0)
{
  f();
}

template <class Buffer, class F>
typename enable_if<Eq<Arity<F>::value, 0>, typename Return<F>::T>::T
callOnBuffer(Buffer& buffer, F f)
{
  return f(
  );
}
#endif
