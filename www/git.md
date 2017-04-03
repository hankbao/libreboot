---
title: Contributing
x-toc-enable: true
...

Useful information, related to libreboot development, including links to
the Git repositories where development is done. Development discussion
is done on IRC and on the libreboot
[subreddit](https://www.reddit.com/r/libreboot/)

Download Libreboot from the Git repository here:

**git clone https://notabug.org/libreboot/libreboot**.

How can I help the libreboot project? 
--------------------------------------

**At any given time, here is a list of tasks if there are any, for the
next stable release of libreboot: [click here](tasks.md)** - this is
also where to send bug reports.

There are many ways that you can assist the libreboot project. We have a
[list of tasks that need to be completed](tasks.md), which you can work
on (you can also add to that list). Generally speaking, adding support
for more hardware is a priority for the libreboot project. Information
about building libreboot can be found at [here](docs/git/).

We especially need more skilled coreboot hackers to do work that will
benefit the libreboot project. This includes porting new boards to
coreboot, which are likely to be valid candidates for libreboot.

Learn libreboot from the inside out; download the git repository, and
study libreboot. We need more full-time developers who can help to
maintain the project.

Not a developer? You can still help!

-   Work towards convincing hardware manufacturers and designers to
    start caring about free software. Perhaps even build your own
    hardware.

-   Spread the word about libreboot! Tell your friends about it, and
    talk about it on public internet-based discussion networks (e.g.
    *social networks*).
-   Install and use libreboot yourself, showing it to others and
    promoting it that way. [Computers with libreboot preinstalled are
    also available](suppliers.md), which helps to fund the libreboot
    project.
-   Help others to install libreboot. For example, organize an
    installation workshop at your local hackerspace.
-   Assist with improving the libreboot documentation, so that others
    may find it easier to use.

General guidelines for submitting patches 
-----------------------------------------

Some people put their name on their work, for recognition, and it's OK
if you want to do that; however, the libreboot project does not require
this. Some projects (such as coreboot) require a legal name, and this
can be problematic for certain groups of people.

Using your legal name is **not** required when submitting patches to
libreboot. For reasons why we have this policy, read [this
article](http://geekfeminism.org/2012/09/29/quick-hit-how-git-shows-the-patriarchal-nature-of-the-software-industry/).
You can use any name of your choosing, or your company name (if you have
one), if you want or need to do that. You can also submit patches
*without a name*, if you want or need to do that (instructions are
provided on this page). Also, read [this
article](http://www.kalzumeus.com/2010/06/17/falsehoods-programmers-believe-about-names/).
Unfortunately, git appears to be stuck with these problems, when an
author changes their name, and we don't have a concrete answer to it.
As far as we know, publishing your legal name isn't even required for
copyright purposes; in fact, "pen" names are commonly used by literary
authors (computer programs are literary works).

When submitting any kind of documentation, try not to use the terms
him/her, she/he, his/her, or anything that is gender biased. **Use
their, they, them, those people, that person, and so on.** [You are making a
huge
difference](https://gist.github.com/0xabad1dea/8870b192fd1758743f66#file-singularthey-md)

<https://www.gnu.org/licenses/license-list.html> (we will reject any
documentation released under the GNU Free Documentation License, for
reasons mentioned in [../why-not-gnu/](why-not-gnu.md)). NOTE: not
putting a copyright notice on a work does not mean it lacks copyright.
Copyright is *automatic* in most countries. Not putting a license on a
work also does not make that work *free*; you have to declare a free
license, otherwise the default, restrictive copyright laws apply for
those who wish to do anything with your work. Always put a license on
your work!

Libreboot development is facilitated by *[git](https://git-scm.com/)*, a
*distributed* version control system. You will need to install git (most
distributions package it in their repositories).

Use this command, to download the repository: 

**git clone https://notabug.org/libreboot/libreboot**.

A new directory named ***libreboot*** will have been created, containing
libreboot.

How to submit your patches 
--------------------------

### Method 0: Use NotABug pull requests

NotABug, libreboot's main git hosting provider, now provides a *pull requests*
feature. <https://notabug.org/libreboot/> has the libreboot repository. Create an
account on the website, then view the libreboot repo and click "fork" and clone
your fork. Then commit your patch on top, and push into your repository (we
recommend into a non-master branch). NotABug guides you through the process of
pushing patches to it.

Once you have pushed your patches, go to the relevant repository on
<https://notabug.org/libreboot/> and click "Pull Requests". Then click
"New Pull Request". Select the relevant branches that you want to
submit a pull request for, between the two repositories, and then click
"Submit".

When this is done, Leah Rowe (maintainer of the main repository) or
someone else (with push access) will review your patch, and decide
whether to merge it. Relevant links to bug trackers for discussing
development (especially patch review) are on [../tasks/](tasks.md)

###  Method 1: Host a fork

Give the checkout details to [Leah Rowe](contrib.md), along with
information on which commits in what branch contain your changes.

**Check [\#githosting](#githosting), for a list of Git hosting providers
that we recommend.**

List of recommended Git hosting providers 
-----------------------------------------

There are several that we could recommend:

-   [You!](https://git-scm.com/book/en/v2/Git-on-the-Server-The-Protocols)
    (host your own Git repository, on your own server, on your own
    internet connection, in your own physical space)
-   [NotABug.org](https://notabug.org/) - the
    [source](https://notabug.org/hp/gogs/) is also available if you want to run
    it on your own server.

The libreboot project itself uses all of the above options.

Most Git hosting providers distribute non-free JavaScript on their
website, but as far as we are aware all of these providers are
freedom-friendly and reliable to use (and their JavaScript is free).
This list will be expanded upon at a later date. The hosting providers
above are all powered by Free Software, which means that you can also
host your own version of the software that they use on their website.

We generally recommend self-hosting (first option in the list above).
Git is a *decentralized* (distributed) version control system. However,
not everyone can afford to do this, so using one of the other providers
on the list above is also acceptable.
