#include <testXMLcd.h>
#include <testXMLcd.cd>
#include <classdesc_epilogue.h>
using namespace classdesc;

int main()
{
  Root x, x1;
  random_init_t r;
  random_init(r,"",x);
  random_init(r,"",x1);
  assert(x.bar!=x1.bar);
  assert(x.bar1!=x1.bar1);
}
