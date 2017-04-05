#!/bin/bash

echo $1
FILE=${1%.md}

cat $1 > temp.md

if [ "${FILE}" != "./index" ]; then
        if [[ $FILE == *index ]]
        then
            DEST=".."
        else
            DEST="index.html"
        fi

        RETURN="<a href='$DEST'>Back to previous index</a>"
fi

echo "" >> temp.md
echo "[License](license.md)" >> temp.md

# change out .md -> .html
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*)/.html\1)/g'

# work around issue #2872
TOC=$(grep -q "^x-toc-enable: true$" temp.md && echo "--toc --toc-depth=2")

# work around heterogenous pandoc versions
SMART=$(pandoc -v | grep -q '2\.0' || echo "--smart")

# chuck through pandoc
pandoc $TOC $SMART temp.md -s --css /global.css -T Libreboot --template template.html --metadata return="$RETURN"> $FILE.html
