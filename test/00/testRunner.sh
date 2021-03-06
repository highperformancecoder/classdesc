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

# runs all test executable with assertion statements
for i in $here/test/*.cc; do
    if grep assert $i >& null; then
        if [ $i = $here/test/testJavaClass.cc ]; then
            continue  # javaClass tested in t0043a.sh
        fi
        if [ $i = $here/test/testXML_base.cc ]; then
            continue  # XML_base tested in t0038a.sh
        fi
        if [ -f ${i%%.cc} ]; then
            echo $i
            ${i%%.cc} >& null
            if test $? -ne 0; then fail; fi
        fi
    fi
done


pass
