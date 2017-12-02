#! /bin/sh

fail()
{
    echo "FAILED" 1>&2
    exit 1
}

pass()
{
    echo "PASSED" 1>&2
    exit 0
}

trap "fail" 1 2 3 15

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

cd test
./testJavaClass
if test $? -ne 0; then fail; fi

pass
