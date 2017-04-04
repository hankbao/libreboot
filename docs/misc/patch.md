% Diff and patch 

This is just a quick guide for reference, use 'man' to know more.

[Back to index](./)

Apply a patch
=============

To apply a patch to a single file, do that in it's directory:

    $ patch < foo.patch

Assuming that the patch is distributed in unified format identifying the
file the patch should be applied to, the above will work. Otherwise:

    $ patch foo.txt < bar.patch

You can apply a patch to an entire directory, but note the "p level".
What this means is that inside patch files will be the files that you
intend to patch, identified by path names that might be different when
the files ane located on your own computer instead of on the computer
where the patch was created. 'p' level instructs the 'patch' utility
to ignore parts of the path name to identify the files correctly.
Usually a p level of 1 will work, so you would use:

    $ patch -p1 < baz.patch

Change to the top level directory before running this. If a patch level
of 1 cannot identify the files to patch, then inspect the patch file for
file names. For example:\
**/home/user/do/not/panic/yet.c**

and you are working in a directory that contains panic/yet.c, use:

    $ patch -p5 < baz.patch

You usually count one up for each path separator (forward slash) removed
from the beginning of the path, until you are left with a path that
exists in the current working directory. The count is the p level.

Removing a patch using the -R flag

    $ patch -p5 -R < baz.patch

Create a patch with diff
========================

Diff can create a patch for a single file:

    $ diff -u original.c new.c > original.patch

For diff'ing a source tree:

    $ cp -R original new

Do whatever you want in new/ and then diff it:

    $ diff -rupN original/ new/ > original.patch

git diff
========

git is something special.

Note: this won't show new files created.

Just make whatever changes you want to a git clone and then:

    $ git diff > patch.git

Note the git revision that you did this with:

    $ git log

Alternatively (better yet), commit your changes and then use:
    $ git format-patch -N
Replace N with the number of commits that you want to show.

git apply
=========

it really is.

Now to apply that patch in the future, just git clone it again and do
with the git revision you found from above:

    $ git reset --hard REVISIONNUMBER

Now put patch.git in the git clone directory and do:

    $ git apply patch.git

If you use a patch from git format-patch, then use **git am patch.git**
instead of **git apply patch.git**. git-am will re-create the commits
aswell, instead of just applying the patch.

Copyright © 2014, 2015 Leah Rowe <info@minifree.org>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
