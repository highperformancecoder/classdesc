#include "referenceMember.h"
#include "classdesc_epilogue.h"
using namespace classdesc;

int main()
{
  Foo foo;
  pack_t buf;
  buf<<foo;
}
