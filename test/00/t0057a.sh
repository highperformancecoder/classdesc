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

# test XML schema generation
$here/test/testXSDgenerate 
if test $? -ne 0; then fail; fi

# test well-formedness
xmllint --output /dev/null test1.xsd
if test $? -ne 0; then fail; fi

# check against XMLSchema's official schema
xmllint --output /dev/null --schema $here/test/XMLSchema.xsd test1.xsd
if test $? -ne 0; then fail; fi

# now check the actual output against the schema
xmllint --output /dev/null --schema test1.xsd test1.xml
if test $? -ne 0; then fail; fi



pass
