#!/bin/bash

[ "x${DEBUG+set}" = 'xset' ] && set -v
set -euo pipefail
IFS=$'\n\t'

find -L . -name '*.html' -and -not -name 'template.html' -delete
find -L . -name '*.md' -exec ./publish.sh {} \;
