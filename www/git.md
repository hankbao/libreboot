---
title: Information about sending patches for review
x-toc-enable: true
...

Useful information, related to Libreboot development, including links to the
Git repositories where development is done. Libreboot development is discussed
on the [IRC channel](https://webchat.freenode.net/?channels=libreboot).

Download Libreboot from the Git repository here:

**git clone https://notabug.org/libreboot/libreboot.git**

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
