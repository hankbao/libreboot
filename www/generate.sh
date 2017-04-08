#!/bin/bash

[ "x${DEBUG+set}" = 'xset' ] && set -v
set -e

find -L . -name '*.html' -and -not -name 'template.html' -delete
find -L . -name '*.md' -exec ./publish.sh {} \;
./index.sh
