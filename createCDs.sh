descriptor=$1
shift
for cdHeader in $*; do
    guard=CLASSDESC_`echo $cdHeader|tr [a-z] [A-Z]`_H 
    cat >>$descriptor-allCDs.h <<EOF
#ifdef $guard
#include "$cdHeader-$descriptor.cd"
#endif
EOF
done
