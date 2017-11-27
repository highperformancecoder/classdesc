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
#include "pack_stl.h"
using namespace classdesc;
struct foo
{
  unserialisable<std::vector<int>::iterator> i;
  std::vector<int> bar;
};

#include "test.cd"
#include "classdesc_epilogue.h"

void foobar(std::vector<int>::iterator x)
{}

int main()
{
  foo bar;
  pack_t b;
  pack(b,string(),bar);
  unpack(b,string(),bar);
  for (bar.i=bar.bar.begin(); bar.i!=bar.bar.end(); bar.i++)
    *bar.i++; 
  foobar(bar.i);
}
EOF

$here/classdesc pack unpack <test.cc >test.cd
if test $? -ne 0; then fail; fi
$CC -I$here -DTR1 -o a.out test.cc
if test $? -ne 0; then fail; fi
./a.out
if test $? -ne 0; then fail; fi

pass
