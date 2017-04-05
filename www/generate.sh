#!/bin/bash
find -L . -name '*.html' -and -not -name 'template.html' -delete
find -L . -name '*.md' -exec ./publish.sh {} \;
