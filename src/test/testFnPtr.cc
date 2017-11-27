#include "testFnPtr.h"
#include "testFnPtr.cd"
#include "classdesc_epilogue.h"
using namespace classdesc;

int main() 
{
  pack_t b;
  Foo f;
  b<<f;
  return 0;
}
