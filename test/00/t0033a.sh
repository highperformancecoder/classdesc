#! /bin/sh

here=`pwd`
if test $? -ne 0; then exit 2; fi
tmp=/tmp/$$
mkdir $tmp
if test $? -ne 0; then exit 2; fi
cd $tmp
if test $? -ne 0; then exit 2; fi

fail()
{
    echo "FAILED" 1>&2
    cd $here
    chmod -R u+w $tmp
    rm -rf $tmp
    exit 1
}

pass()
{
    echo "PASSED" 1>&2
    cd $here
    chmod -R u+w $tmp
    rm -rf $tmp
    exit 0
}

trap "fail" 1 2 3 15

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
# execute test here. PWD is temporary, refer to classdesc home directory 
# with $here

if [ -n "$AEGIS_ARCH" ]; then
  BL=`aegis -cd -bl`
  BL1=$BL/../../baseline
else #standalone test
  BL=.
  BL1=.
fi
CC=${CC:-g++}

cat >test.cc <<EOF
#include "pack_base.h"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace classdesc;

int main()
{
  pack_t p1, p2;
  xdr_pack x1, x2;
  double a=1, b=1;

  p1 << a << b;
  p1.swap(p2);
  p2 >> a >> b;
  assert(a==1 && b==1);

  x1 << a << b;
  x1.swap(x2);
  x2 >> a >> b;
  assert(a==1 && b==1);  

  p1 << a << b;
  p1 >> a;
  p1.swap(p2);
  p2 >> b;
  assert(a==1 && b==1);  

  x1 << a << b;
  x1 >> a;
  x1.swap(x2);
  x2 >> b;
  assert(a==1 && b==1);  

  try  {p1.swap(x1);}
  catch (pack_error& e) {return 0;}
  try {x1.swap(p1);}
  catch (pack_error& e) {return 0;}
  return 1;
}

EOF

# XDR tests don't mean anything on Cygwin
if uname|grep CYGWIN; then pass; fi

$CC -I$here -DXDR_PACK test.cc -DTR1 $here/xdr_pack.o `pkg-config --libs libtirpc`
if test $? -ne 0; then fail; fi

./a.out
if test $? -ne 0; then fail; fi

pass
