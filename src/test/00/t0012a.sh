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

# execute test here. PWD is temporary, refer to classdesc home directory 
# with $here

if [ -n "$AEGIS_ARCH" ]; then
  BL=`aegis -cd -bl`
  BL1=$BL/../../baseline
else #standalone test
  BL=.
  BL1=.
fi

# if MPICH2, then we need to run an mpd
if which mpd; then
    if [ -x `which mpd` ]; then
        mpd&
    fi
fi

cat >test.cc <<EOF
#include <classdescMP.h>
#include <pack_stream.h>
using namespace classdesc;

main(int argc, char**argv)
{
    MPISPMD p(argc,argv);
    MPIbuf b; int i=0, j=1;
    b << i << bcast(1) >> i;
    b << i << bcast(1) >> j;
    if (b.pos()!=sizeof(int)) return 1;
    if (i!=j) return 1;
}
EOF

# exit with a pass if MPI is not there
if [ ! -f $MPIHOME/include/mpi.h ]; then pass; fi

CC=${CC:-mpicxx}
echo "$CC -I$here -I$BL -I$BL1 -I$MPIHOME/include test.cc"
$CC -DTR1 -I$here -I$BL -I$BL1 -I$MPIHOME/include test.cc
if test $? -ne 0; then fail; fi

mpirun -np 2 a.out
if test $? -ne 0; then fail; fi

pass
