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

# Test against regression of bug found in RESTProcess of overloaded
# functions where a partial override of overloaded base class occurs.

cat >source.h <<EOF
#include <string>
using std::string;
struct Base
{
  string m_x;
  virtual string x() const {return m_x;}
  virtual string x(const string& x) {return m_x=x;}  
};

struct Derived: public Base
{
  using Base::x;
  bool xCalled=false;
  string x(const string& x) override {xCalled=true; return m_x=x;}  
};
EOF

cat >source.cc <<EOF
#include "source.h"
#include "source.cd"
#include "pythonBuffer.h"
#include "classdesc_epilogue.h"

Derived derived;
CLASSDESC_ADD_GLOBAL(derived);
CLASSDESC_PYTHON_MODULE(module);
EOF

$here/classdesc -overload -typeName RESTProcess json_pack json_unpack <source.h >source.cd
if test $? -ne 0; then fail; fi

g++ -I$here -I$here/json5_parser/json5_parser `pkg-config --cflags python3` -fPIC -shared -o module.so source.cc `pkg-config --libs python3` -lboost_system 
if test $? -ne 0; then fail; fi

python3 <<EOF
from module import derived
derived.x("hello")
assert derived.xCalled()
assert derived.x()=="hello"
EOF
if test $? -ne 0; then fail; fi

pass
