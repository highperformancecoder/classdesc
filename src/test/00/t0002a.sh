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
  MANDELBROT=`aefind -resolve $here/mpi-examples -name mandelbrot -print`
  BL=`aegis -cd -bl`
  BL1=$BL/../../baseline
else #standalone test
  MANDELBROT=$here/mpi-examples/mandelbrot
  BL=.
  BL1=.
  # create a test directory under $here, and use that!
  cd $here
  rm -rf $tmp
  tmp=$here/$$
  mkdir $tmp
  cd $tmp
fi

if [ ! -z "$LAMHOME" ]; then
MPIHOME=$LAMHOME
MPILIB="-L$LAMHOME/lib -lmpi -llam"
else
MPIHOME=$HOME/usr
MPILIB="-L$MPIHOME/lib -lmpi"
fi

CC=${CC:-g++}

if [ $here = $BL ]; then fail; fi

if [ "$CC" = xlC ]; then CC=mpCC; fi

# exit with a pass if MPI is not there
if [ ! -f $MPIHOME/include/mpi.h ]; then pass; fi

if [ "$CC" = mpCC ]; then
 poe $MANDELBROT -procs 2 -hostfile $HOME/host.list
elif [ `uname` = OSF1 ]; then
 prun -n 2 a.out
elif [ ! -z "$LAMHOME" ]; then
    lrun -n 2 $MANDELBROT
else
 $MANDELBROT -np 2
fi

if test $? -ne 0; then fail; fi

pass
