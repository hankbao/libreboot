#!/bin/sh

# see equivalent script in libreboot-website for more info

FILE=${1%.md}
echo $FILE
cat $FILE.md > temp.md
sed temp.md -i -e 's/.md\(#[a-z\-]*\)*)/.html\1)/g'
TOC=$(grep -q "^x-toc-enable: true$" temp.md && echo "--toc")
pandoc -t html temp.md -s --css global.css --section-divs -T Libreboot $TOC > $FILE.html
