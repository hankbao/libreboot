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
set -euo pipefail
IFS=$'\n\t'

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
echo "[License](/license.md)" >> temp.md

# change out .md -> .html
sed temp.md -i -e 's/\.md\(#[a-z\-]*\)*)/.html\1)/g'

# work around issue #2872
TOC=$(grep -q "^x-toc-enable: true$" temp.md && echo "--toc --toc-depth=2") || TOC=""

# work around heterogenous pandoc versions
SMART=$(pandoc -v | grep -q '2\.0' || echo "--smart") || SMART=""

# chuck through pandoc
pandoc $TOC $SMART temp.md -s --css /global.css -T Libreboot \
    --template template.html --metadata return="$RETURN"> $FILE.html
