---
title: Information about sending patches for review
x-toc-enable: true
...

Download Libreboot from the Git repository here:

**git clone https://notabug.org/libreboot/libreboot.git**

You can submit your patches via
[Notabug pull requests](#how-to-submit-your-patches-via-pull-requests) or
[via mailing list](#how-to-submit-your-patches-via-mailing-list).

Information about how the Libreboot project is governed is documented in our
[general management guidelines](management.md).

Editing the website and documentation, wiki-style
-------------------------------------------------

The website and documentation is inside the **www** directory in the
[Git repository](#how-to-download-libreboot-from-the-git-repository), in
Pandoc flavoured Markdown. The website is generated into static HTML via Pandoc
with the following scripts in that directory:

- index.sh: generates the news feed (on the News section of the website)
- publish.sh: converts an .md file to an .html file
- generate.sh: with calls to index.sh and publish.sh, compiles the entire
  Libreboot website

Use any standard text editor (e.g. Vim, Emacs, Nano, Gedit) to edit the files,
commit the changes and
[send patches](#how-to-submit-your-patches-via-pull-requests).

Optionally, you can install a web server (e.g. lighttpd, nginx) locally and
set the document root to the *www* directory in your local Git repository.
With this configuration, you can then generate your local version of the
website and view it by typing `localhost` in your browser's URL bar.

General guidelines for submitting patches
-----------------------------------------

We require all patches to be submitted under a free license:
<https://www.gnu.org/licenses/license-list.html>.

- GNU General Public License v3 is highly recommended
- For documentation, we require GNU Free Documentation License v1.3 or higher

*Always* declare a license on your work! Not declaring a license means that
the default, restrictive copyright laws apply, which would make your work
non-free.

GNU+Linux is generally recommended as the OS of choice, for Libreboot
development.

General code review guidelines
------------------------------

Any member of the public can
[submit a patch](#how-to-submit-your-patches-via-pull-requests).
Members with push access must *never* push directly to the master branch;
issue a Pull Request, and wait for someone else to merge. Never merge your own
work!

Your patch will be reviewed for quality assurance, and merged if accepted.

How to download Libreboot from the Git repository
-------------------------------------------------

In your terminal:

**git clone https://notabug.org/libreboot/libreboot.git**

A new directory named ***libreboot*** will have been created, containing
libreboot.

How to submit your patches (via pull requests)
----------------------------------------------

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

How to submit your patches (via mailing list)
---------------------------------------------

We also have a mailing list at
[development@libreboot.org](mailto:development@libreboot.org)

Information about this mailing list, including how to subscribe, can be found
at <https://libreboot.org/lists/>.

Archives of previous discussion on this mailing list can be found at
<https://libreboot.org/pipermail/development/>.

The mailing list software in use is
[GNU Mailman](https://www.gnu.org/software/mailman/), so the way to use it is
the same as on other libre software projects.

If you prefer, you can submit patches the old-fashioned (as some may say,
proper) way by sending them to our mailing list. Code review by our
[maintainers](management.md) is done on both Notabug and on the mailing list.

Once approved, your patch will be merged in the master branch of the main Git
repository.
