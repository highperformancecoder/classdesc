#include <string>

struct Foo: public std::string
{
  int bar;
};

#include "stringInheritance.cd"
