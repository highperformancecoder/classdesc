template <class T> struct Foo;

template <> struct Foo<long double> {};

struct Bar: public Foo<long double> {};

#include "explicitTemplateArg.cd"
