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

$here/test/testXML_base <$here/test/test1.xml >out.dat
if test $? -ne 0; then fail; fi

cat >expected.dat <<EOF
[foo#0]=
  
  
  
  

[foo#0.array#0]=
    
    
    
  
[foo#0.array#0.elem#0]=1
[foo#0.array#0.elem#1]=2
[foo#0.array#0.elem#2]=3
[foo#0.bar#0]=1
[foo#0.dar#0]=foo&< >"'
[foo#0.dar#0.attr]=1
[foo#0.doo#0.attr]=3

Count[foo]=1
Count[foo#0.array]=1
Count[foo#0.array#0.elem]=3
Count[foo#0.bar]=1
Count[foo#0.dar]=1
Count[foo#0.doo]=1
EOF

diff out.dat expected.dat
if test $? -ne 0; then fail; fi

pass
