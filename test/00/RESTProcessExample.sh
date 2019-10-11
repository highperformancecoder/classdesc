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

cat >expected.dat <<EOF
/root
{"bar":{"barfoo":"eb","base___Foo":{"a":0.10000000000000001,"af":0.20000000298023224,"b":3,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]},"f":20,"vFoo":[{"a":0.10000000000000001,"af":0.20000000298023224,"b":1,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]},{"a":0.10000000000000001,"af":0.20000000298023224,"b":1,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]},{"a":0.10000000000000001,"af":0.20000000298023224,"b":1,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]}]},"bar1":{"barfoo":"ec","f":{"a":0.10000000000000001,"af":0.20000000298023224,"b":2,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]},"fp":{"a":0,"af":0,"b":0,"bf":false,"bt":false,"c":"","c1":[],"ch":0,"d":[0,0,0],"d1":[[0,0],[0,0],[0,0]],"ef":"ea","h":[],"l":[],"llex":[],"m":[],"sef":{},"sm":{},"vs":[]},"g":2,"vFoo":[{"a":0.10000000000000001,"af":0.20000000298023224,"b":1,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]},{"a":0.10000000000000001,"af":0.20000000298023224,"b":1,"bf":false,"bt":true,"c":"\r hello & 123 ","c1":["\r","\r"],"ch":77,"d":[0,1,2],"d1":[[0,1],[2,3],[4,5]],"ef":"ea","h":[2,2,2],"l":[2,2,2],"llex":[["hello","hello"],["hello","hello"]],"m":[{"first":0,"second":5},{"first":3,"second":2}],"sef":{},"sm":{},"vs":[" hello"," hello"]}]},"defaultless":{"base___Abstract":{}}}
/root/bar/a
0.10000000000000001
/root/bar/a
0.20000000000000001
/root/bar/a
0.20000000000000001
/root/bar/af
0.20000000298023224
/root/bar/af
0.30000001192092896
/root/bar/af
0.30000001192092896
/root/bar/b
3
/root/bar/b
1
/root/bar/b
1
EOF

$here/RESTProcessExample/RESTProcess <$here/RESTProcessExample/RESTProcessExample.txt >output.dat

diff output.dat expected.dat
if test $? -ne 0; then fail; fi

pass
