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


$here/RESTProcessExample/RESTProcess <$here/RESTProcessExample/RESTProcessExample.txt &>output.txt

diff output.txt $here/RESTProcessExample/RESTProcessExpected.txt
if test $? -ne 0; then fail; fi

$here/RESTProcessExample/SimpleBuffer <$here/RESTProcessExample/RESTProcessExample.txt &>output.txt

diff output.txt $here/RESTProcessExample/RESTProcessExpected.txt
if test $? -ne 0; then fail; fi

pass
