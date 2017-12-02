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
  TEST_ISA=`aefind -resolve $here/test -name test_isa.cc -print`
else #standalone test
  BL=.
  BL1=.
  TEST_ISA=$here/test/test_isa.cc
fi
CC=${CC:-g++}
$here/classdesc isa <$TEST_ISA >test_isa.cd
$CC -I. -I$here -I$BL -I$BL1 $TEST_ISA -DTR1 -o aout
./aout

if test $? -ne 0; then fail; fi

pass
