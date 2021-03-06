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

$here/test/testTypeName >log
if test $? -ne 0; then fail; fi

cat >dat <<EOF
::foo
::bar
::nn::dd
::nn::foo
::nn::bar< int > 
::foobar< int > 
std::vector<double>
std::deque<double>
std::list<double>
std::set<double>
std::map<int,double>
1 bb 6
EOF

diff -w log dat
if test $? -ne 0; then fail; fi

pass
