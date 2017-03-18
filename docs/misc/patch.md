<div class="section">

Diff and patch {#pagetop}
==============

This is just a quick guide for reference, use \'man\' to know more.

[Back to index](./)

</div>

<div class="section">

Apply a patch
=============

To apply a patch to a single file, do that in it\'s directory:\
**\$ patch &lt; foo.patch**

Assuming that the patch is distributed in unified format identifying the
file the patch should be applied to, the above will work. Otherwise:\
**\$ patch foo.txt &lt; bar.patch**

You can apply a patch to an entire directory, but note the \"p level\".
What this means is that inside patch files will be the files that you
intend to patch, identified by path names that might be different when
the files ane located on your own computer instead of on the computer
where the patch was created. \'p\' level instructs the \'patch\' utility
to ignore parts of the path name to identify the files correctly.
Usually a p level of 1 will work, so you would use:\
**\$ patch -p1 &lt; baz.patch**

Change to the top level directory before running this. If a patch level
of 1 cannot identify the files to patch, then inspect the patch file for
file names. For example:\
**/home/user/do/not/panic/yet.c**

and you are working in a directory that contains panic/yet.c, use:\
**\$ patch -p5 &lt; baz.patch**

You usually count one up for each path separator (forward slash) removed
from the beginning of the path, until you are left with a path that
exists in the current working directory. The count is the p level.

Removing a patch using the -R flag\
**\$ patch -p5 -R &lt; baz.patch**

[Back to top of page.](#pagetop)

</div>

<div class="section">

Create a patch with diff
========================

Diff can create a patch for a single file:\
**\$ diff -u original.c new.c &gt; original.patch**

For diff\'ing a source tree:\
**\$ cp -R original new**

Do whatever you want in new/ and then diff it:\
**\$ diff -rupN original/ new/ &gt; original.patch**

[Back to top of page.](#pagetop)

</div>

<div class="section">

git diff
========

git is something special.

Note: this won\'t show new files created.

Just make whatever changes you want to a git clone and then:\
**\$ git diff &gt; patch.git**

Note the git revision that you did this with:\
**\$ git log**

Alternatively (better yet), commit your changes and then use:\
\$ **git format-patch -N**\
Replace N with the number of commits that you want to show.

[Back to top of page.](#pagetop)

</div>

<div class="section">

git apply
=========

it really is.

Now to apply that patch in the future, just git clone it again and do
with the git revision you found from above:\
**\$ git reset \--hard REVISIONNUMBER**

Now put patch.git in the git clone directory and do:\
**\$ git apply patch.git**

If you use a patch from git format-patch, then use **git am patch.git**
instead of **git apply patch.git**. git-am will re-create the commits
aswell, instead of just applying the patch.

[Back to top of page.](#pagetop)

</div>

<div class="section">

Copyright © 2014, 2015 Leah Rowe &lt;info@minifree.org&gt;\
Permission is granted to copy, distribute and/or modify this document
under the terms of the Creative Commons Attribution-ShareAlike 4.0
International license or any later version published by Creative
Commons; A copy of the license can be found at
[../cc-by-sa-4.0.txt](../cc-by-sa-4.0.txt)

Updated versions of the license (when available) can be found at
<https://creativecommons.org/licenses/by-sa/4.0/legalcode>

UNLESS OTHERWISE SEPARATELY UNDERTAKEN BY THE LICENSOR, TO THE EXTENT
POSSIBLE, THE LICENSOR OFFERS THE LICENSED MATERIAL AS-IS AND
AS-AVAILABLE, AND MAKES NO REPRESENTATIONS OR WARRANTIES OF ANY KIND
CONCERNING THE LICENSED MATERIAL, WHETHER EXPRESS, IMPLIED, STATUTORY,
OR OTHER. THIS INCLUDES, WITHOUT LIMITATION, WARRANTIES OF TITLE,
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT,
ABSENCE OF LATENT OR OTHER DEFECTS, ACCURACY, OR THE PRESENCE OR ABSENCE
OF ERRORS, WHETHER OR NOT KNOWN OR DISCOVERABLE. WHERE DISCLAIMERS OF
WARRANTIES ARE NOT ALLOWED IN FULL OR IN PART, THIS DISCLAIMER MAY NOT
APPLY TO YOU.

TO THE EXTENT POSSIBLE, IN NO EVENT WILL THE LICENSOR BE LIABLE TO YOU
ON ANY LEGAL THEORY (INCLUDING, WITHOUT LIMITATION, NEGLIGENCE) OR
OTHERWISE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL, CONSEQUENTIAL,
PUNITIVE, EXEMPLARY, OR OTHER LOSSES, COSTS, EXPENSES, OR DAMAGES
ARISING OUT OF THIS PUBLIC LICENSE OR USE OF THE LICENSED MATERIAL, EVEN
IF THE LICENSOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSSES,
COSTS, EXPENSES, OR DAMAGES. WHERE A LIMITATION OF LIABILITY IS NOT
ALLOWED IN FULL OR IN PART, THIS LIMITATION MAY NOT APPLY TO YOU.

The disclaimer of warranties and limitation of liability provided above
shall be interpreted in a manner that, to the extent possible, most
closely approximates an absolute disclaimer and waiver of all liability.

</div>
