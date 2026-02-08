descriptor=$1
shift
rm -f $descriptor-allCDs.h
cat >$descriptor-allCDs.h <<EOF
#ifndef CLASSDESC_${descriptor}_allCDS_H
#define CLASSDESC_${descriptor}_allCDS_H
EOF
if [ $descriptor = "json_pack" ]; then
    echo "#if defined(__cplusplus) && __cplusplus>=201103L" >>$descriptor-allCDs.h
fi
for cdHeader in $*; do
    guard=CLASSDESC_`echo $cdHeader|tr [a-z] [A-Z]`_H
    cat >>$descriptor-allCDs.h <<EOF
#ifdef $guard
#include "$cdHeader-$descriptor.cd"
#endif
EOF
done
if [ $descriptor = "json_pack" ]; then
    echo "#endif" >>$descriptor-allCDs.h
fi
echo "#endif" >>$descriptor-allCDs.h
