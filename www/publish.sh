#!/bin/bash

echo $1
FILE=${1%.md}

# get title block
head -n 4 $FILE.md > temp.md

# if not homepage, add a link back to the homepage
if [ "${FILE}" != "./index" ]; then
        RETURN="<p><a href='/index.md'>Go back to homepage</a></p>"
fi

# read rest of file
tail -n +5 $FILE.md >> temp.md

# add license notice where applicable
# TODO: make this less intrusive
#if [[ ${FILE} != "docs*" ]] ; then
#    cat license.md >> temp.md
#fi

# change out .md -> .html
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*)/.html\1)/g'

# work around issue #2872
TOC=$(grep -q "^x-toc-enable: true$" temp.md && echo "--toc")

# work around heterogenous pandoc versions
SMART=$(pandoc -v | grep -q '2\.0' || echo "--smart")

# chuck through pandoc
pandoc $SMART temp.md -s --css /global.css --section-divs -T Libreboot $TOC --template=template.html --metadata "return=$RETURN" > $FILE.html
