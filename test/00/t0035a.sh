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
struct foo
{
    int bar[5];
};

#include "test.cd"
#include "pack_stream.h"
#include "classdesc_epilogue.h"
using namespace classdesc;

int main()
{
  foo a,b;
  memset(a.bar,1,sizeof(a.bar));
  memset(b.bar,0,sizeof(b.bar));
  pack_t p;
  p<<a>>b;
  if (memcmp(a.bar,b.bar,sizeof(a.bar))==0)
      return 0;
  else
      return 1;
}

EOF

CC=${CC:-g++}

$here/classdesc pack unpack <test.cc >test.cd
if test $? -ne 0; then fail; fi

$CC -I. -I$here -I$BL -I$BL1 -DTR1 -o a.out test.cc
if test $? -ne 0; then fail; fi

./a.out
if test $? -ne 0; then fail; fi

pass
