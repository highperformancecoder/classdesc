#include <classdesc_access.h>

enum {foobar=3};

class foo
{
  enum bar {one, two, three};
  bar bb;
  enum {b1,b2,b3} bb2;
  CLASSDESC_ACCESS(foo);
public:
  enum {c1,c2,c3} cc;
  int a;
};

