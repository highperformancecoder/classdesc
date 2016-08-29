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
#include "pack_graph.h"
#include "classdesc_epilogue.h"
using namespace classdesc;

int flag=0;

struct foo
{
  foo() {flag++;}
  ~foo() {flag++;}
};

namespace classdesc_access
{
  template <> struct access_pack<foo>: public NullDescriptor<pack_t>{};
  template <> struct access_unpack<foo>: public NullDescriptor<pack_t>{};
}

main()
{
  foo a;
  foo *f=&a;
  pack_t b;
  pack(b,"",is_treenode(),f);
  unpack(b,"",is_treenode(),f);
  b.alloced.clear();
  f=&a;
  if (flag!=3) abort();
  pack(b,"",is_graphnode(),f);
  unpack(b,"",is_graphnode(),f);
  b.alloced.clear();
  if (flag!=5) abort();
}

EOF

$CC -I$here -DTR1 -o a.out test.cc
if test $? -ne 0; then fail; fi

./a.out
if test $? -ne 0; then fail; fi

pass
