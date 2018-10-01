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

cat >input <<EOF
class foo
{
  template <class T> void a() {};
  void b() {};
}
EOF

cat >output <<EOF
#include "classdesc.h"

#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
#include "p_base.h"
namespace classdesc {
template <class C,class M>
void p_type(p_t&,const string&,M);
}
using classdesc::p_type;
namespace classdesc_access {
template <> struct access_p<class ::foo > {
template <class _CD_ARG_TYPE>
void operator()(classdesc::p_t& targ, const classdesc::string& desc,_CD_ARG_TYPE& arg)
{
::p(targ,desc+".b",arg,&foo::b);
}
void type(classdesc::p_t& targ, const classdesc::string& desc)
{
::p_type<class ::foo>(targ,desc+".b",&foo::b);
}
};
}
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
EOF

$here/classdesc p <input >tmp
if test $? -ne 0; then fail; fi
diff -b -B tmp output
if test $? -ne 0; then fail; fi

pass
