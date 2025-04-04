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

cat >test.h <<EOF
class foo
{
   Q_OBJECT
    Q_PROPERTY(Priority priority READ priority WRITE setPriority)
    Q_ENUMS(Priority)

public:
    MyClass(QObject *parent = 0);
    ~MyClass();

public slots:
    void mySlot();
signals:
    void mySignal1();
slots:
    void mySlot2();
};
EOF

cat >test.cd.expected <<EOF
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

#ifndef CLASSDESC_p___foo
#define CLASSDESC_p___foo
template <> struct access_p< class ::foo > {
template <class _CD_ARG_TYPE>
void operator()(classdesc::p_t& targ, const classdesc::string& desc,_CD_ARG_TYPE& arg)
{
::p(targ,desc+".mySlot",arg,&::foo::mySlot);
::p(targ,desc+".mySlot2",arg,&::foo::mySlot2);
}
template <class _CD_TYPE>
void type(classdesc::p_t& targ, const classdesc::string& desc)
{
::p_type<_CD_TYPE,class ::foo >(targ,desc+".mySlot",&::foo::mySlot);
::p_type<_CD_TYPE,class ::foo >(targ,desc+".mySlot2",&::foo::mySlot2);
}
};

#endif
}
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
EOF

$here/classdesc -qt p <test.h >test.cd
if test $? -ne 0; then fail; fi

diff test.cd test.cd.expected
if test $? -ne 0; then fail; fi

pass
