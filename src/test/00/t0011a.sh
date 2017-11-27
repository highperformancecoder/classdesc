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
  BL1=/home/ecolab/classdesc/branch.1/branch.1/baseline/../../baseline
else #standalone test
  BL=.
  BL1=.
fi
CC=${CC:-g++}

# do a checkpoint/restart test on few basic types
cat >test.cc <<EOF
#include "pack_base.h"
#include "classdesc_epilogue.h"
#include <stdio.h>
using namespace classdesc;

template <class T>
void test(const T& a)
{
    T b;
    pack_t b1;
    b1<<a; b1>>b; 
    if (a!=b) {puts("pack_t failure"); exit(1);}
    xdr_pack b2;
    b2<<a; b2>>b; 
    if (a!=b) {puts("xdr_pack failure"); exit(1);}
}    

main()
{
    test(1);
    test(1.0);
    test(string("hello"));
}
EOF

# Neither CYGWIN nor MacOS correctly implement XDR !
if [ `uname|cut -f1 -d_` != CYGWIN -a `uname` != Darwin ]; then XDRPACK=-DXDR_PACK; fi
$CC -I$here -I$BL -I$BL1 $XDRPACK -DTR1 test.cc $here/xdr_pack.o -o aout
if test $? -ne 0; then fail; fi

./aout 
if test $? -ne 0; then fail; fi

pass
