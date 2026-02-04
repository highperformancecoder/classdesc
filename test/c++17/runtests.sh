#!/bin/bash
set -e
for i in $*; do
    echo $i;
    ./$i;
    done

# XSD generation test
xmllint --output /dev/null test1.xsd
xmllint --output /dev/null --schema ../XMLSchema.xsd test1.xsd
xmllint --output /dev/null --schema test1.xsd test1.xml
