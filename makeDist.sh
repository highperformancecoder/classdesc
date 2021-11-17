#!/usr/bin/bash
VERSION=`git describe`
git archive --format=tar --prefix=classdesc-$VERSION/ HEAD -o /tmp/classdesc-$VERSION.tar
# add submodules
cd json_spirit; git archive --format=tar --prefix=classdesc-$VERSION/json_spirit/ HEAD -o /tmp/$$.tar
tar Af /tmp/classdesc-$VERSION.tar /tmp/$$.tar
rm /tmp/$$.tar
gzip -f /tmp/classdesc-$VERSION.tar
