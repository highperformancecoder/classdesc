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
if [ -n "$AEGIS_ARCH" ]; then
  BL=`aegis -cd -bl`
  BL1=$BL/../../baseline
  if [ $here = $BL ]; then fail; fi
  ls -s `aefind -resolve $here -name pack_base.h -print` unpack_base.h
  HEAT_C=`aefind -resolve $here/examples -name heat.cc -print`
else #standalone test
  BL=.
  BL1=.
  HEAT_C=$here/examples/heat.cc
fi
CC=${CC:-g++}

$CC -I. -I$here -I$BL -I$BL1 -I$here/examples -I$BL/examples  -I$BL1/examples -DTR1 $HEAT_C -o aout
if test $? -ne 0; then fail; fi

./aout 20 >norestart
if test $? -ne 0; then fail; fi

./aout 10 >/dev/null
if test $? -ne 0; then fail; fi

./aout 10 restart >restart
if test $? -ne 0; then fail; fi

diff restart norestart >/dev/null
if test $? -ne 0; then fail; fi

# not meaningful to test xdr version on Cygwin
if uname|grep CYGWIN; then pass; fi

# Now test the xdr pack version
$CC -DXDR_PACK -I. -I$here -I$BL -I$BL1 -I$here/examples -I$BL/examples  -I$BL1/examples $HEAT_C -DTR1 $here/xdr_pack.o `pkg-config --libs libtirpc` -o aout
if test $? -ne 0; then fail; fi

./aout 20 >norestart
if test $? -ne 0; then fail; fi

./aout 10 >/dev/null
if test $? -ne 0; then fail; fi

./aout 10 restart >restart
if test $? -ne 0; then fail; fi

diff restart norestart >/dev/null
if test $? -ne 0; then fail; fi

pass




