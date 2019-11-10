struct Foo
{
  int a;
  int& ra;
  Foo(): ra(a) {}
};
#include "referenceMember.cd"
