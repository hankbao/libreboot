#!/usr/bin/env bash
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

# usage: title file
title() {
    sed -n 1p "$1" | sed -e s-^..--
}

# usage: meta file
meta() {
    file=$1
    URL=$(printf '%s\n' "${file%.md}.html" | sed -e s-news/--)

    printf '%s\n' "[$(title "$file")]($URL){.title}"
    printf '%s\n' "[$(sed -n 3p "$file" | sed -e s-^..--)]{.date}"
    printf '\n'
    tail -n +5 "$file" | perl -p0e 's/(\.|\?|\!)( |\n)(.|\n)*/.../g'

    printf '\n'
    printf '\n'
}

# generate the index file

# MANIFEST determines the order of news articles in news/index.md
FILES=$(< news/MANIFEST)

cat news-list.md > news/index.md

for f in $FILES
do
    meta "$f" >> news/index.md
done

# generate an RSS index

rss() {
    printf '%s\n' '<rss version="2.0">'
    printf '%s\n' '<channel>'

    printf '%s\n' "<title>$BLOGTITLE</title>"
    printf '%s\n' "<link>"$BLOGBASE"news/</link>"
    printf '%s\n' "<description>$BLOGDESCRIPTION</description>"

    for f in $FILES
    do
        # render content and escape
        desc=$(sed -e 's/</\&lt;/g' ${f%.md}.bare.html | sed -e 's/>/\&gt;/g')
        url="${f%.md}.html"

        printf '%s\n' '<item>'
        printf '%s\n' "<title>$(title "$f")</title>"
        printf '%s\n' "<link>$BLOGBASE$url</link>"
        printf '%s\n' "<description>$desc</description>"
        printf '%s\n' '</item>'
    done

    printf '%s\n' '</channel>'
    printf '%s\n' '</rss>'
}

rss > news/feed.xml
cp news/feed.xml feed.xml
