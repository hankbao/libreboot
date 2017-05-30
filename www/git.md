---
title: Information about sending patches for review
x-toc-enable: true
...

Useful information, related to Libreboot development, including links to the
Git repositories where development is done. Libreboot development is discussed
on the [IRC channel](https://webchat.freenode.net/?channels=libreboot).

Download Libreboot from the Git repository here:

**git clone https://notabug.org/libreboot/libreboot.git**

Editing the website and documentation, wiki-style
-------------------------------------------------

The entire website on libreboot.org, including all of the Libreboot
documentation, is hosted in the Git repository. The pages are written in
Markdown, which is an easy-to-use markup language similar to the one used
when, for instance, editing pages on WikiPedia or a MediaWiki-based website.

Just download Libreboot's Git repository, make changes to the website under
the ***www*** directory. Changes to the documentation section are made under
the ***docs*** directory. Once you've made your changes, you can submit them
for inclusion live on libreboot.org, using the instructions on the rest of this
page.

***You can use any standard text editor to edit and improve the entire
Libreboot website. E.g. Vim, GNU Emacs, GNU Nano, Gedit or whatever you
prefer to use.***

We currently have a shortage of people who possess the skill to write and
maintain good documentation for Libreboot, so we welcome any and all
contributions!

General guidelines for submitting patches
-----------------------------------------

We require all patches to be submitted under a free license:
<https://www.gnu.org/licenses/license-list.html>.

You *must* declare copyright on your work, and declare a free license. Not
declaring copyright, especially if you also don't declare a license, means that
the default restrictive copyright laws apply under the Berne convention.

If you wish to submit work without copyright, or with permissive terms, then
the *Creative Commons Zero* license is highly recommended. Otherwise, the **GNU
General Public License, version 3 or (at your option) any later version**, is
preferred; we will also accept any other free license.

Libreboot development is facilitated by *[git](https://git-scm.com/)*, a
*distributed* version control system. You will need to install Git (most
GNU+Linux distributions package it in their repositories).

GNU+Linux is generally recommended as the OS of choice, for Libreboot
development.

General code review guidelines
------------------------------

Non-core members will push to their own repository, and issue a Pull Request.
Core maintainers of Libreboot can push to the repository, but not to the master
branch; they must push it to a non-master branch, and again issue a Pull
Request.

An individual or group of people will then review the patch sent by the
contributor. With approval from a core maintainer, or otherwise someone with
merge rights on the repository, the patch will be merged into the official
*master* branch.

Those with push/merge access on the official Libreboot repository must never
directly push their own patches to the master branch, and they must never
merge their own Pull Request. Regardless of the individual's standing and
frequency of contributions within the project, those who decide whether to
merge a patch will *always* differ from the original contributor.

These guidelines are *enforced*, to avoid the appearance of a BDFL (Benevolent
Dictator for Life) in the Libreboot project, as one previously existed in the
project. Libreboot is a collectively and democratically governed project.
Maintainers who violate these guidelines may, at the discretion of other
maintainers, have their push/merge rights revoked.

How to download Libreboot from the Git repository
-------------------------------------------------

In your terminal:

**git clone https://notabug.org/libreboot/libreboot.git**

A new directory named ***libreboot*** will have been created, containing
libreboot.

How to submit your patches 
--------------------------

Make an account on <https://notabug.org/> and navigate (while logged in) to
<https://notabug.org/libreboot/libreboot>. Click *Fork* and in your account,
you will have your own repository of Libreboot. Clone your repository, make
whatever changes you like to it and then push to your repository, in your
account on NotABug.

Now, navigate to <https://notabug.org/libreboot/libreboot/pulls> and click
*New Pull Request*. 

You can submit your patches there. Alternative, you can log onto the Libreboot
IRC channel and notify the channel of which patches you want reviewed, if you
have your own Git repository with the patches.

Once you have issued a Pull Request, the Libreboot maintainers will be notified
via email. If you do not receive a fast enough response from the project, then
you could also notify the project via the #libreboot channel on Freenode.
