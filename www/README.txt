How to fork libreboot.org. (but why would you do that?)

Make sure you have gettext enabled in PHP.

You also need some symlinks. I won't explain too much about it, it's self-explanatory.
Just have a look at this output for a rough idea (it's from the site/ directory):

$ ls -lh
total 52K
drwxr-xr-x 2 vimuser vimuser 4.0K May 15 13:46 contrib
drwxr-xr-x 2 vimuser vimuser 4.0K May 22 08:55 css
lrwxrwxrwx 1 vimuser vimuser   27 Apr  5 03:58 docs -> ../../lbdev/libreboot/docs/
drwxr-xr-x 2 vimuser vimuser 4.0K May 23 20:33 download
-rw-r--r-- 1 vimuser vimuser 1.2K May 23 20:35 footer.php
-rw-r--r-- 1 vimuser vimuser 1.3K May 15 13:02 functions.php
-rw-r--r-- 1 vimuser vimuser 3.9K May 23 20:33 index.php
drwxr-xr-x 2 vimuser vimuser 4.0K May 19 21:53 logo
-rw-r--r-- 1 vimuser vimuser   24 Apr  5 04:16 robots.txt
-rw-r--r-- 1 vimuser vimuser 4.5K May 23 20:35 variables.php

(notice how anything not part of the git repository, i.e. in .gitignore, is a symlink to some place outside this git clone)

This document is....
Copyright 2015 Leah Rowe <info@minifree.org>
This document is released under the Creative Commons Attribution-ShareAlike 4.0 International Public License and all future versions. 
A copy of the license can be found at "cc-by-sa-4.txt".

This document is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See ../cc-by-sa-4.txt for more information.

