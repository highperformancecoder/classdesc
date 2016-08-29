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

cat >input.cc <<EOF
#include <vector>
#include "pack_stl.h"
#include "classdesc_epilogue.h"
using std::vector;
class ProbCluster {
public:

struct Component {
Component() {}
Component(int a, double p, double probs)
: _unitID(a), _compProb(p), _elemProbabilities(probs) {}
int _unitID;
double _compProb;
double _elemProbabilities;
};

struct ComponentVect: public vector<Component>
{
};

typedef Component foobar;
struct foobarVect: public vector<foobar>
{
};


ComponentVect _components;
};
#include "input.cd"

main() {}
EOF

$here/classdesc pack <input.cc >input.cd
if test $? -ne 0; then fail; fi
$CC -I$here -DTR1 input.cc
if test $? -ne 0; then fail; fi

pass
