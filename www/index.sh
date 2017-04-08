#!/bin/bash
#
# Copyright (C) 2017 Alyssa Rosenzweig <alyssa@rosenzweig.io>
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

BLOGTITLE="Libreboot News"
BLOGBASE="https://libreboot.org/"
BLOGDESCRIPTION="News on Libreboot development"

title() {
    sed -n 1p $f | sed -e s-^..--
}

meta() {
    URL=$(echo ${f%.md}.html | sed -e s-news/--)

    echo "[$(title)]($URL){.title}"
    echo "[$(sed -n 3p $f | sed -e s-^..--)]{.date}"
    echo ""
    tail -n +5 $f | perl -p0e 's/(\.|\?|\!)( |\n)(.|\n)*/.../g'

    echo ""
    echo ""
}

# generate the index file

FILES=$(ls -1 -t news/*.md | sed -e s-.*index.md-- -e s-.*presentation.md--)

cat news-list.md > news/index.md

for f in $FILES
do
    touch -d "$(sed -n 3p $f | sed -e 's/^..//g')" $f
    meta >> news/index.md
done

# generate an RSS index

rss() {
    echo '<rss version="2.0">'
    echo '<channel>'

    echo "<title>$BLOGTITLE</title>"
    echo "<link>"$BLOGBASE"news/</link>"
    echo "<description>$BLOGDESCRIPTION</description>"

    for f in $FILES
    do
        # render content and escape
        desc=$(sed ${f%.md}.bare.html -e 's/</\&lt;/g' | sed -e 's/>/\&gt;/g')
        url="${f%.md}.html"

        echo '<item>'
        echo "<title>$(title)</title>"
        echo "<link>$BLOGBASE$url</link>"
        echo "<description>$desc</description>"
        echo '</item>'
    done

    echo '</channel>'
    echo '</rss>'
}

rss > news/feed.xml
cp news/feed.xml feed.xml
