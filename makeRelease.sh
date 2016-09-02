echo "old version=`git describe`"
echo '#define CLASSDESC_VERSION "'$1'"' >classdescVersion.h
echo "Version $1" >doc/version.tex
echo "PROJECT_NUMBER=$1" >Doxyversion
git commit -a -m "Release $1"
git tag -a -m "" $1
