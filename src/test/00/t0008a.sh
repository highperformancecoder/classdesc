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

# force test to fail if run against baseline
if [ -n "$AEGIS_ARCH" ]; then
  BL=`aegis -cd -bl`
  if [ $here == $BL ]; then fail; fi
  GRAPHNODE=`aefind -resolve $here/examples -name graphnode -print`
else #standalone test
  BL=.
  BL1=.
  GRAPHNODE=$here/examples/graphnode
fi

# reduce stack size to ensure recursion limits are handled
ulimit -s 100
$GRAPHNODE
if test $? -ne 0; then fail; fi

$here/examples/ptrflag
if test $? -ne 0; then fail; fi

$here/examples/refnode
if test $? -ne 0; then fail; fi

pass
