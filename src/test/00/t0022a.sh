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

if [ -f $here/classdesc ]; then
  CLASSDESC=$here/classdesc
else
  CLASSDESC=`aefind -resolve $here -name ./classdesc -print`
fi
echo $here
echo $CLASSDESC

cat >input.cc <<EOF
#include "pack_base.h"
#include "classdesc_access.h"
namespace foo
{
  class bar
  {
    int bar1;
    CLASSDESC_ACCESS(bar);
  };
}

#include "input.cd"
EOF

CC=${CC:-g++}

$CLASSDESC pack <input.cc >input.cd
if test $? -ne 0; then fail; fi

$CC -c -I$here -I$BL -I$BL1 -DTR1 input.cc -o aout
if test $? -ne 0; then fail; fi

pass
