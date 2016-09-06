#ifndef CPPFEATURES_H
#define CPPFEATURES_H

struct Base
{
  virtual void foo()=0;
};

enum class Colour: int {red, green, blue};

template<class T> struct Twrap
{
  T t;
  Twrap() {}
  Twrap(const T& t): t(t) {}
  Twrap& operator=(const T& x) {t=x; return *this;}
  bool operator==(const Twrap& x) const {return t==x.t;}
};

struct CppFeatures: public Base
{
  int d{15};
  int d1=15;
  alignas(long long) int a; 
  long long ll;
  static constexpr int b=12;
  Colour c; 
  CppFeatures(): a{2}, c{Colour::red} {}
  CppFeatures(const CppFeatures&)=default;
  CppFeatures& operator=(const CppFeatures&)=delete;
  
  Twrap<Twrap<int>> esp; // see ticket #32

  [[noreturn]] void foo() override {throw 1;}
  void bar() noexcept {}

  template <class T> void barfoo(const T&) {}
  void barfoo(const int&)=delete;

  auto foo1()->int {return 0;}

  std::unordered_map<std::vector<int>,int> umap;

  using Foo=char;
  template <class T> using VFoo=std::vector<Foo>;
};

  template <typename C> using Elem_type=typename C::value_type;


  // test variadic template argument
  template <class T,class... Args>
  struct Join: public T, public Join<Args...> {};

  template <class T> struct Join<T>: public T {};

// test support of spaceless close angle brackets
  template <class T> struct allocator {};
  template <class T, class A=allocator<T>>
    struct vect {};

#include "c++Features.cd"
#endif
