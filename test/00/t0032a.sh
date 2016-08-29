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

cat >test.h <<EOF 
struct foo
{
  const int a;
  int b;
  foo(int x): a(1), b(x) {}
  bool operator!=(const foo& x) const {return a!=x.a || b!=x.b;}
};
EOF

$here/classdesc pack unpack <test.h >test.cd
if test $? -ne 0; then fail; fi

cat >test.cc <<EOF
#include "test.h"
#include "test.cd"
#include "classdesc_epilogue.h"

int main()
{
  foo x(2), y(3);
  classdesc::pack_t b;
  b<<x>>y;
  return x!=y; 
}
EOF

$CC -I$here -DTR1 -o a.out test.cc
if test $? -ne 0; then fail; fi

./a.out
if test $? -ne 0; then fail; fi

pass
