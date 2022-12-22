#ifndef CLASSDESC_FUNCTIONDB8
#define CLASSDESC_FUNCTIONDB8
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 1> 
{
  typedef A1 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 2> 
{
  typedef A2 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 2> 
{
  typedef A2 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 3> 
{
  typedef A3 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 3> 
{
  typedef A3 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 4> 
{
  typedef A4 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 4> 
{
  typedef A4 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 5> 
{
  typedef A5 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 5> 
{
  typedef A5 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 6> 
{
  typedef A6 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 6> 
{
  typedef A6 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 7> 
{
  typedef A7 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 7> 
{
  typedef A7 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8), 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8), 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const, 8> 
{
  typedef A8 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept, 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept, 8> 
{
  typedef A8 T;
  typedef T type;
};
#endif
template <class F, template<class> class P>
struct AllArgs<F,P,8>
{
   static const bool value=true  && P<typename Arg<F,1>::T>::value  && P<typename Arg<F,2>::T>::value  && P<typename Arg<F,3>::T>::value  && P<typename Arg<F,4>::T>::value  && P<typename Arg<F,5>::T>::value  && P<typename Arg<F,6>::T>::value  && P<typename Arg<F,7>::T>::value   && P<typename Arg<F,8>::T>::value;
};


template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    static const int V=8;
    static const int value=8;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    static const int V=8;
    static const int value=8;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    static const int V=8;
    static const int value=8;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const> 
{
    static const int V=8;
    static const int value=8;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct ClassOf<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_const_method<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const>
{
   static const bool value=true;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_nonmember_function_ptr<R (*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_nonmember_function_ptr<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
   static const bool value=true;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    C* obj;
    M method;
    public:
    static const int arity=8;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {return (obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    C* obj;
    M method;
    public:
    static const int arity=8;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {(obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<const C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    const C* obj;
    M method;
    public:
    static const int arity=8;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<const C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8)>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    const C* obj;
    M method;
    public:
    static const int arity=8;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) const>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;
    C& obj;
    M method;
    public:
    static const int arity=8;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {return (obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    static const bool is_const=true;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) const>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;
    C& obj;
    M method;
    public:
    static const int arity=8;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {(obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    static const bool is_const=true;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    static const int V=8;
    static const int value=8;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    static const int V=8;
    static const int value=8;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    static const int V=8;
    static const int value=8;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept> 
{
    static const int V=8;
    static const int value=8;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct ClassOf<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_const_method<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept>
{
   static const bool value=true;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_nonmember_function_ptr<R (*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> 
struct is_nonmember_function_ptr<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
   static const bool value=true;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    C* obj;
    M method;
    public:
    static const int arity=8;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {return (obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    C* obj;
    M method;
    public:
    static const int arity=8;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {(obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<const C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    const C* obj;
    M method;
    public:
    static const int arity=8;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<const C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) noexcept>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8);
    const C* obj;
    M method;
    public:
    static const int arity=8;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;
    C& obj;
    M method;
    public:
    static const int arity=8;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {return (obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    static const bool is_const=true;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8) const noexcept>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8) const;
    C& obj;
    M method;
    public:
    static const int arity=8;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8) const {(obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8);}
    static const bool is_const=true;
};
#endif

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 8> >, 
   typename Return<F>::T>::T
apply_nonvoid_fn(F f, Args& a, Fdummy<F> dum=0)
{
  return f(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
}

/*
 TODO: if any of the arguments to f are lvalues, we need to construct temporaries,
 which require C++-11 ability to test for the existence of a copy constructor. 
 If the return type is not copy constructable, the user must arrange for the return value to be discarded
*/

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 8> >,
    void>::T
apply_void_fn(F f, Args& a, Fdummy<F> dum=0)
{
  f(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
}

template <class Buffer, class F>
typename enable_if<Eq<Arity<F>::value, 8>, typename Return<F>::T>::T
callOnBuffer(Buffer& buffer, F f)
{
  typename remove_const<typename remove_reference<typename Arg<F,1>::T>::type>::type a1;
  buffer>>a1;
  typename remove_const<typename remove_reference<typename Arg<F,2>::T>::type>::type a2;
  buffer>>a2;
  typename remove_const<typename remove_reference<typename Arg<F,3>::T>::type>::type a3;
  buffer>>a3;
  typename remove_const<typename remove_reference<typename Arg<F,4>::T>::type>::type a4;
  buffer>>a4;
  typename remove_const<typename remove_reference<typename Arg<F,5>::T>::type>::type a5;
  buffer>>a5;
  typename remove_const<typename remove_reference<typename Arg<F,6>::T>::type>::type a6;
  buffer>>a6;
  typename remove_const<typename remove_reference<typename Arg<F,7>::T>::type>::type a7;
  buffer>>a7;
  typename remove_const<typename remove_reference<typename Arg<F,8>::T>::type>::type a8;
  buffer>>a8;
  return f(
a1
,
a2
,
a3
,
a4
,
a5
,
a6
,
a7
,
a8
  );
}
#endif
