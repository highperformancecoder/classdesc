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

$here/classdesc p <$here/test/test_private.cc >tmp1
if test $? -ne 0; then fail; fi

sed -e '/^$/d' tmp1 >tmp2
diff -b tmp2 $here/test/test_private.out
if test $? -ne 0; then fail; fi

$here/classdesc -respect_private p <$here/test/test_private.cc >tmp1
if test $? -ne 0; then fail; fi

sed -e '/^$/d' tmp1 >tmp2
diff -b tmp2 $here/test/test_private.private.out
if test $? -ne 0; then fail; fi



pass
