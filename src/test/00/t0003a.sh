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

if [ -n "$AEGIS_ARCH" ]; then
  BL=`aegis -cd -bl`
  BL1=$BL/../../baseline
  HEAT=`aefind -resolve $here/mpi-examples -name heat -print`
  ln -s `aefind -resolve $here/mpi-examples -name arr2D.h -print` .
else #standalone test
  BL=.
  BL1=.
  HEAT=$here/mpi-examples/heat
fi
CC=${CC:-g++}

if [ $here = $BL ]; then fail; fi
if [ "$CC" = xlC ]; then CC=mpCC; fi

if [ ! -z "$LAMHOME" ]; then
MPIHOME=$LAMHOME
MPILIB="-L$LAMHOME/lib -lmpi -llam"
else
MPIHOME=$HOME/usr
MPILIB="-L$MPIHOME/lib -lmpi"
fi


# exit with a pass if MPI is not there
if [ ! -f $MPIHOME/include/mpi.h ]; then pass; fi

if [ "$CC" = "mpCC" ]; then
    poe $HEAT  -procs 2 -hostfile $HOME/host.list >2.out
    if test $? -ne 0; then fail; fi
    poe $HEAT  -procs 1 -hostfile $HOME/host.list >1.out
    if test $? -ne 0; then fail; fi
elif [ `uname` = OSF1 ]; then
    prun -n 2 $HEAT  >2.out
    if test $? -ne 0; then fail; fi
    prun $HEAT >1.out
    if test $? -ne 0; then fail; fi
elif [ ! -z "$LAMHOME" ]; then
    lrun -n 2 $HEAT >2.out
    if test $? -ne 0; then fail; fi
    lrun -n 1 $HEAT >1.out
    if test $? -ne 0; then fail; fi
else
    $HEAT -np 2 >2.out
    if test $? -ne 0; then fail; fi

    $HEAT >1.out
    if test $? -ne 0; then fail; fi
fi

diff 2.out 1.out >/dev/null
if test $? -ne 0; then fail; fi


pass
