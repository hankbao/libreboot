#!/bin/bash
#
# Copyright (C) 2017 Alyssa Rosenzweig <alyssa@rosenzweig.io>
# Copyright (C) 2017 Leah Rowe <info@minifree.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

[ "x${DEBUG+set}" = 'xset' ] && set -v
set -e

echo $1
FILE=${1%.md}

# for link to the live editor
editlink="${FILE/.\//.\/docs\/}.md"
editlink="${editlink/.\/docs\/docs/docs}"
editlink="${editlink/.\/docs/www}EDIT"

cat $1 > temp.md

OPTS="-T Libreboot"

if [ "${FILE}" != "./index" ]; then
        if [[ $FILE == *suppliers ]]
        then
            RETURN=""
        else
            if [[ $FILE == *index ]]
            then
                DEST="../"
            else
                DEST="./"
            fi

            RETURN="<strong><a href='https://notabug.org/libreboot/libreboot/_edit/master/${editlink}'>Edit this page</a></strong> -- <a href='$DEST'>Back to previous index</a>"
            OPTS="-T Libreboot"
        fi
else
        OPTS="--css /headercenter.css"
fi

if [[ $FILE = *suppliers ]]; then
        printf "\n<strong><a href=\"https://notabug.org/libreboot/libreboot/_edit/master/${editlink}\">Edit this page</a></strong> -- <a href=\"../\">Back to previous page</a>\n" >> temp.md
fi

if [ "${FILE}" != "./docs/fdl-1.3" ] && [ "${FILE}" != "./conduct" ]; then
    printf "\n\n**[Edit this page](https://notabug.org/libreboot/libreboot/_edit/master/%s)** --\n" "${editlink}" >> temp.md
    cat footer.md >> temp.md
fi

# change out .md -> .html
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*)/.html\1)/g'
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*]/.html\1]/g'

# change out .md -> .html
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*)/.html\1)/g'
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*]/.html\1]/g'

# work around issue #2872
TOC=$(grep -q "^x-toc-enable: true$" temp.md && echo "--toc --toc-depth=2") || TOC=""

# work around heterogenous pandoc versions
SMART=$(pandoc -v | grep -q '2\.0' || echo "--smart") || SMART=""

# chuck through pandoc
pandoc $TOC $SMART temp.md -s --css /global.css $OPTS \
        --template template.html --metadata return="$RETURN"> $FILE.html

# additionally, produce bare file for RSS
pandoc $1 > $FILE.bare.html

# generate section title anchors as [link]
sed $FILE.html -i -e 's_^<h\([123]\) id="\(.*\)">\(.*\)</h\1>_<div class="h"><h\1 id="\2">\3</h\1><a aria-hidden="true" href="#\2">[link]</a></div>_'
sed -i -e 's/mdEDIT/md/g' $FILE.html
