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

touch poly.cd

cat >should-fail.cc <<EOF
#include <ref.h>
#include "classdesc_epilogue.h"
using namespace classdesc;
struct foo1 {int f;};
struct foo2 {float f;};
int main()
{
    ref<foo1> a;
    ref<foo2> b;
    a OP b;
}
EOF

g++ -DOP="==" -DTR1  -I$here -I. should-fail.cc &>/dev/null
if test $? -eq 0; then fail; fi

g++ -DOP="!=" -DTR1  -I$here -I.  should-fail.cc &>/dev/null
if test $? -eq 0; then fail; fi

cat >should-fail.cc <<EOF
#include <poly.h>
#include "classdesc_epilogue.h"
using namespace classdesc;

struct polyType1: Object<polyType1,Eobject> {};
struct polyType2: Object<polyType2,Eobject> {};

int main()
{
    poly<polyType1> a;
    poly<polyType2> b;
    a OP b;
}
EOF

g++ -DOP="==" -DTR1  -I$here -I.  should-fail.cc
if test $? -eq 0; then fail; fi

g++ -DOP="!=" -DTR1 -I$here -I.  should-fail.cc
if test $? -eq 0; then fail; fi

cat >should-succeed.cc <<EOF
#include <ref.h>
#include <poly.h>
#include "classdesc_epilogue.h"
using namespace classdesc;

struct foo: public object
{
    int type() const{return 0;}
    object* clone() const {return new foo;}
    void pack(pack_t& b) const {}
    void unpack(pack_t& b) {}
};

int main()
{
    poly<object> a, b;
    a.addObject<foo>(); b.addObject<foo>();

    ref<foo> c, d;
    *c; *d;

    return !(a==a && a!=b && c==c && c!=d);
}
EOF

g++ -DTR1  -I$here  -I. -o a.out should-succeed.cc
if test $? -ne 0; then fail; fi

./a.out
if test $? -ne 0; then fail; fi
pass
