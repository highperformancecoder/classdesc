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
  CLASSDESC=`aefind -resolve $here -type file -name classdesc -print`
  if [ $here = $BL ]; then fail; fi
else #standalone test
  BL=.
  BL1=.
  CLASSDESC=$here/classdesc
fi
CC=${CC:-g++}

cat >test.cc <<EOF
enum colour {red,green,blue};

class fred
{
public:
  enum trueblue {one,two,three};
  trueblue x;
};

#include "test.h"
#include "classdesc_epilogue.h"
using namespace classdesc;

int main()
{
  pack_t p;
  colour a=green,b=red;
  p<<a; p>>b;
  if (a!=b) exit(1);
}
EOF

$here/classdesc -I $here pack unpack <test.cc >test.h
if test $? -ne 0; then fail; fi

$CC -I$here -I$BL -I$BL1 -DTR1 test.cc -o aout
if test $? -ne 0; then fail; fi

./aout
if test $? -ne 0; then fail; fi

pass
