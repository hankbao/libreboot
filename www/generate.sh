#!/bin/bash
find -L . -name '*.md' -exec ./publish.sh {} \;
