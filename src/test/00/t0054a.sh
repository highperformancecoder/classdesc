#! /bin/sh

# checks the namespace wrapped class wrapped enum case

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
namespace foo
{
  struct bar
  {
    enum foobar {one, two};
  };
}
EOF

$here/classdesc -typeName pack <test.h >test.cd
if test $? -ne 0; then fail; fi

cat >test.cc <<EOF
#include "classdesc.h"
#include "test.h"
#include "test.cd"
#include "classdesc_epilogue.h"
#include <iostream>
using namespace classdesc;

int main()
{
  foo::bar::foobar x= foo::bar::two;
  if (string(Enum_handle<foo::bar::foobar>(x))!="two")
    return 1;
}
EOF

$CC -I$here -DTR1 test.cc
if test $? -ne 0; then fail; fi

./a.out
if test $? -ne 0; then fail; fi

pass
