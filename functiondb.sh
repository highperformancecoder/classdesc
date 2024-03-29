:

# system shell is assumed to be bash.

max_arity=20
arity=0

# required to handle reference arguments
remove_reference()
{
    i=0
    while [ $i -lt $arity ]; do
        echo "  typename remove_reference<A$[$i+1]>::type a$i(a[$[$i]]);" >>functiondb$arity.h
        j=$[i++]
    done
}    

while [ $arity -le $max_arity ]; do
    cat <<EOF
#if !defined(CLASSDESC_ARITIES) || (CLASSDESC_ARITIES & (1<<$arity))
#include "functiondb$arity.h"
#endif
EOF
    # create the arity include file
    cat >functiondb$arity.h <<EOF
#ifndef CLASSDESC_FUNCTIONDB$arity
#define CLASSDESC_FUNCTIONDB$arity
EOF
    
    i=1
    template_args=
    arg_types=
    arg_decl=
    args=
    vec_args=
    type_and_args=
    while [ $i -lt $arity ]; do
        template_args="${template_args}, class A$i"
        arg_types="${arg_types}A$i,"
        arg_decl="${arg_decl}A$i a$i,"
        args="${args}a$i,"
        vec_args="${vec_args}a[$[$i-1]],"
#        vec_args="${vec_args}a$[$i-1],"
        type_and_args="${type_and_args} && P<typename Arg<F,$i>::T>::value "
        j=$[i++]
    done
    if [ $i -eq $arity ]; then
        template_args="${template_args}, class A$i"
        arg_types="${arg_types}A$i"
        arg_decl="${arg_decl}A$i a$i"
        args="${args}a$i"
        vec_args="${vec_args}a[$[$i-1]]"
#        vec_args="${vec_args}a$[$i-1]"
        type_and_args="${type_and_args}  && P<typename Arg<F,$i>::T>::value"
    fi

# definition of Arg type accessors
    arg=1
    while [ $arg -le $arity ]; do
cat >>functiondb$arity.h <<EOF
template <class R$template_args> 
struct Arg<R (*)($arg_types), $arg> 
{
  typedef A$arg T;
  typedef T type;
};

template <class C, class R$template_args> 
struct Arg<R (C::*)($arg_types), $arg> 
{
  typedef A$arg T;
  typedef T type;
};

template <class C, class R$template_args> 
struct Arg<R (C::*)($arg_types) const, $arg> 
{
  typedef A$arg T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R$template_args> 
struct Arg<R (*)($arg_types) noexcept, $arg> 
{
  typedef A$arg T;
  typedef T type;
};

template <class C, class R$template_args> 
struct Arg<R (C::*)($arg_types) noexcept, $arg> 
{
  typedef A$arg T;
  typedef T type;
};

template <class C, class R$template_args> 
struct Arg<R (C::*)($arg_types) const noexcept, $arg> 
{
  typedef A$arg T;
  typedef T type;
};
#endif
EOF
    j=$[arg++]
    done

    cat >>functiondb$arity.h <<EOF
template <class F, template<class> class P>
struct AllArgs<F,P,$arity>
{
   static const bool value=true $type_and_args;
};


template <class R$template_args> 
struct Arity<R (*)($arg_types)> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class R$template_args> 
struct Return<R (*)($arg_types)> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R$template_args> 
struct Arity<R (* C::*)($arg_types)> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class C,class R$template_args> 
struct Return<R (* C::*)($arg_types)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R$template_args> 
struct Arity<R (C::*)($arg_types)> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class C, class R$template_args> 
struct Return<R (C::*)($arg_types)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R$template_args> 
struct Arity<R (C::*)($arg_types) const> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class C, class R$template_args> 
struct Return<R (C::*)($arg_types) const> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R$template_args> 
struct ClassOf<R (C::*)($arg_types)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R$template_args> 
struct ClassOf<R (*C::*)($arg_types)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R$template_args> 
struct ClassOf<R (C::*)($arg_types) const> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R$template_args> 
struct is_member_function_ptr<R (C::*)($arg_types)>
{
   static const bool value=true;
};

template <class C, class R$template_args> 
struct is_member_function_ptr<R (C::*)($arg_types) const>
{
   static const bool value=true;
};

template <class C, class R$template_args> 
struct is_const_method<R (C::*)($arg_types) const>
{
   static const bool value=true;
};

template <class R$template_args> 
struct is_nonmember_function_ptr<R (*)($arg_types)>
{
   static const bool value=true;
};

template <class C, class R$template_args> 
struct is_nonmember_function_ptr<R (*C::*)($arg_types)>
{
   static const bool value=true;
};

template <class C, class D, class R$template_args>
class bound_method<C, R (D::*)($arg_types)>
{
    typedef R (D::*M)($arg_types);
    C* obj;
    M method;
    public:
    static const int arity=$arity;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()($arg_decl) const {return (obj->*method)($args);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R$template_args>
class bound_method<const C, R (D::*)($arg_types)>
{
    typedef R (D::*M)($arg_types);
    const C* obj;
    M method;
    public:
    static const int arity=$arity;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()($arg_decl) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R$template_args>
class bound_method<C, R (D::*)($arg_types) const>
{
    typedef R (D::*M)($arg_types) const;
    C& obj;
    M method;
    public:
    static const int arity=$arity;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()($arg_decl) const {return (obj.*method)($args);}
    static const bool is_const=true;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods

template <class R$template_args> 
struct Arity<R (*)($arg_types) noexcept> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class R$template_args> 
struct Return<R (*)($arg_types) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R$template_args> 
struct Arity<R (* C::*)($arg_types) noexcept> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class C,class R$template_args> 
struct Return<R (* C::*)($arg_types) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R$template_args> 
struct Arity<R (C::*)($arg_types) noexcept> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class C, class R$template_args> 
struct Return<R (C::*)($arg_types) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R$template_args> 
struct Arity<R (C::*)($arg_types) const noexcept> 
{
    static const int V=$arity;
    static const int value=$arity;
};

template <class C, class R$template_args> 
struct Return<R (C::*)($arg_types) const noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R$template_args> 
struct ClassOf<R (C::*)($arg_types) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R$template_args> 
struct ClassOf<R (*C::*)($arg_types) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R$template_args> 
struct ClassOf<R (C::*)($arg_types) const noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R$template_args> 
struct is_member_function_ptr<R (C::*)($arg_types) noexcept>
{
   static const bool value=true;
};

template <class C, class R$template_args> 
struct is_member_function_ptr<R (C::*)($arg_types) const noexcept>
{
   static const bool value=true;
};

template <class C, class R$template_args> 
struct is_const_method<R (C::*)($arg_types) const noexcept>
{
   static const bool value=true;
};

template <class R$template_args> 
struct is_nonmember_function_ptr<R (*)($arg_types) noexcept>
{
   static const bool value=true;
};

template <class C, class R$template_args> 
struct is_nonmember_function_ptr<R (*C::*)($arg_types) noexcept>
{
   static const bool value=true;
};

template <class C, class D, class R$template_args>
class bound_method<C, R (D::*)($arg_types) noexcept>
{
    typedef R (D::*M)($arg_types);
    C* obj;
    M method;
    public:
    static const int arity=$arity;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()($arg_decl) const {return (obj->*method)($args);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D$template_args>
class bound_method<C, void (D::*)($arg_types) noexcept>
{
    typedef void (D::*M)($arg_types);
    C* obj;
    M method;
    public:
    static const int arity=$arity;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()($arg_decl) const {(obj->*method)($args);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R$template_args>
class bound_method<const C, R (D::*)($arg_types) noexcept>
{
    typedef R (D::*M)($arg_types);
    const C* obj;
    M method;
    public:
    static const int arity=$arity;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()($arg_decl) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D$template_args>
class bound_method<const C, void (D::*)($arg_types) noexcept>
{
    typedef void (D::*M)($arg_types);
    const C* obj;
    M method;
    public:
    static const int arity=$arity;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()($arg_decl) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R$template_args>
class bound_method<C, R (D::*)($arg_types) const noexcept>
{
    typedef R (D::*M)($arg_types) const;
    C& obj;
    M method;
    public:
    static const int arity=$arity;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()($arg_decl) const {return (obj.*method)($args);}
    static const bool is_const=true;
};

template <class C, class D$template_args>
class bound_method<C, void (D::*)($arg_types) const noexcept>
{
    typedef void (D::*M)($arg_types) const;
    C& obj;
    M method;
    public:
    static const int arity=$arity;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()($arg_decl) const {(obj.*method)($args);}
    static const bool is_const=true;
};
#endif

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, $arity> >, 
   typename Return<F>::T>::T
apply_nonvoid_fn(F f, Args& a, Fdummy<F> dum=0)
{
  return f($vec_args);
}

/*
 TODO: if any of the arguments to f are lvalues, we need to construct temporaries,
 which require C++-11 ability to test for the existence of a copy constructor. 
 If the return type is not copy constructable, the user must arrange for the return value to be discarded
*/

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, $arity> >,
    void>::T
apply_void_fn(F f, Args& a, Fdummy<F> dum=0)
{
  f($vec_args);
}

template <class Buffer, class F>
typename enable_if<Eq<Arity<F>::value, $arity>, typename Return<F>::T>::T
callOnBuffer(Buffer& buffer, F f)
{
EOF
    arg=1
    while [ $arg -le $arity ]; do
        cat >>functiondb$arity.h <<EOF
  typename remove_const<typename remove_reference<typename Arg<F,$arg>::T>::type>::type a$arg;
  buffer>>a$arg;
EOF
        let $[arg++]
    done
    echo "  return f(" >>functiondb$arity.h
    arg=1
    while [ $arg -le $arity ]; do
        if [ $arg -gt 1 ]; then
            echo "," >>functiondb$arity.h
        fi
        echo "a$arg" >>functiondb$arity.h
        let $[arg++]
    done
    cat >>functiondb$arity.h <<EOF
  );
}
#endif
EOF
    j=$[arity++]
done
