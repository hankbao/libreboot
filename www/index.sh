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

meta() {
    URL=$(echo ${f%.md}.html | sed -e s-news/--)

    echo "[$(sed -n 1p $f | sed -e s-^..--)]($URL){.title}"
    echo "[$(sed -n 3p $f | sed -e s-^..--)]{.date}"
    echo ""
    tail -n +5 $f | perl -p0e 's/(\.|\?|\!)( |\n)(.|\n)*/.../g'

    echo ""
    echo ""
}

cat news-list.md > news/index.md

for f in $(ls -1 -t news/*.md | sed -e s-.*index.md-- -e s-.*presentation.md--)
do
    touch -d "$(sed -n 3p $f | sed -e 's/^..//g')" $f
    meta >> news/index.md
done
