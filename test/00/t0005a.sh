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
  if [ $here = "$BL" ]; then fail; fi
else #standalone test
  BL=.
  BL1=.
fi
CC=${CC:-mpcxx}
if [ "$CC" = xlC ]; then CC=mpCC; fi


cat >test1.cc <<EOF
#include <classdescMP.h>
classdesc::MPIbuf b;
main() {}
EOF

cat >test2.cc <<EOF
#include <classdescMP.h>
classdesc::MPIbuf c;
EOF

if [ ! -z "$LAMHOME" ]; then
MPIHOME=$LAMHOME
MPILIB="-L$LAMHOME/lib -lmpi -llam"
else
MPIHOME=$HOME/usr
MPILIB="-L$MPIHOME/lib -lmpich"
fi

# exit with a pass if MPI is not there
if [ ! -f $MPIHOME/include/mpi.h ]; then pass; fi


$CC -I$here -I$BL -I$BL1 -I/usr/local/include -L/usr/local/lib -DTR1 -I$MPIHOME/include  test1.cc test2.cc  $MPILIB 

if test $? -ne 0; then fail; fi

pass
