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

cat $1 > temp.md

OPTS=

if [ "${FILE}" != "./index" ]; then
        if [[ $FILE == *index ]]
        then
            DEST="../"
        else
            DEST="./"
        fi

        RETURN="<a href='$DEST'>Back to previous index</a>"
        OPTS="--css /headerleft.css -T Libreboot"
fi

echo "" >> temp.md
printf "[License](/license.md) --\n" >> temp.md
printf "[Information about the Libreboot authors](/contrib.md) --\n" >> temp.md
printf "[Code of conduct](/conduct.md)\n" >> temp.md

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
