echo "old version=`git describe`"
echo '#define CLASSDESC_VERSION "'$1'"' >classdescVersion.h
git commit -a -m "Release $1"
git tag -a -m "" $1
