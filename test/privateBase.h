#include <classdesc_access.h>

struct bar {};

class foo: private bar
{
  CLASSDESC_ACCESS(foo);
};

#include "privateBase.cd"
