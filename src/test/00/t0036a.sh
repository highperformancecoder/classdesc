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
int main()
{
  classdesc::xdr_pack p;
  p.packraw("hello",6);
  if (p.size()<6) return 1;
  char foo[10];
  p.unpackraw(foo,6);
  if (p.pos()<6) return 1;
  return 0;
}
EOF

if uname|grep CYGWIN; then pass; fi

$CC -I$here -DXDR_PACK test.cc -DTR1 $here/xdr_pack.o
if test $? -ne 0; then fail; fi
./a.out
if test $? -ne 0; then fail; fi

pass
