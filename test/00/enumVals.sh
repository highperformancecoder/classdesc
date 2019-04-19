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

PATH=$here:$BL:$BL1:$PATH

cat >test.h <<EOF
enum foo {bar=-1, foobar=0x10+2};
EOF

cat >test.cd.expected <<EOF
#include "classdesc.h"

#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
#ifndef CLASSDESC_TYPENAME___foo
#define CLASSDESC_TYPENAME___foo
namespace classdesc {
template <> inline std::string typeName< ::foo >()
  {return "::foo";}
namespace   {
template <> EnumKey enum_keysData< ::foo >::keysData[]=
 {
  {"bar",int(::bar)},
  {"foobar",int(::foobar)}
 };
template <> EnumKeys< ::foo > enum_keysData< ::foo >::keys(enum_keysData< ::foo >::keysData,sizeof(enum_keysData< ::foo >::keysData)/sizeof(enum_keysData< ::foo >::keysData[0]));
template <> int enumKey< ::foo >(const std::string& x){return int(enum_keysData< ::foo >::keys(x));}
template <> std::string enumKey< ::foo >(int x){return enum_keysData< ::foo >::keys(x);}
}
}
#endif
namespace classdesc_access {
}
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
EOF

classdesc -typeName <test.h >test.cd
if test $? -ne 0; then fail; fi

diff test.cd test.cd.expected
if test $? -ne 0; then fail; fi

pass
