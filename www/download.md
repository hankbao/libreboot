---
title: Downloads
...

**The latest stable release is 20160907, released on 2016-09-07 and can
be found at [\#https](#https).**

Information about specific changes in each Libreboot release, can be found
at [docs/release.md](docs/release.md)

If you're more interested in libreboot development, go to the
[libreboot development page](../git.md), which also includes links to the
Git repositories.

GPG signing key
---------------

Releases are signed with GPG.

    $ gpg --recv-keys 0x05E8C5B2

The GPG key can also be downloaded with this exported dump of the
pubkey: [lbkey.asc](lbkey.asc).

    $ sha512sum -c sha512sum.txt
    $ gpg --verify sha512sum.txt.sig

Do you have a mirror?
---------------------

Let us know! We will add it here. Instructions for how to mirror
libreboot releases can be found [here](#rsync).

HTTPS mirrors {#https}
-------------

These mirrors are recommended, since they use TLS (https://) encryption.

<https://www.mirrorservice.org/sites/libreboot.org/release/> (University
of Kent, UK)

<https://mirror.math.princeton.edu/pub/libreboot/> (Princeton
university, USA)

<https://mirrors.peers.community/mirrors/libreboot/> (Peers Community
Project, USA)

<https://vimuser.org/libreboot/> (vimuser.org, Netherlands)

<https://elgrande74.net/libreboot/> (elgrande74.net, France)

RSYNC mirrors {#rsync}
-------------

Useful for mirroring Libreboot's entire set of release archives.

<rsync://rsync.libreboot.org/mirrormirror/> (Libreboot project official mirror)

<rsync://rsync.mirrorservice.org/libreboot.org/release/> (University of Kent,
UK)

<rsync://mirror.math.princeton.edu/pub/libreboot/> (Princeton university, USA)

<rsync://ftp.linux.ro/libreboot/> (linux.ro, Romania)

<rsync://libreboot.mirror.si/libreboot/>

Are you running a mirror? Contact the libreboot project, and the link will be
added to this page!

HTTP mirrors {#http}
------------

WARNING: these mirrors are non-HTTPS which means that they are
unencrypted. Your traffic could be subject to interference by
adversaries. Make especially sure to check the GPG signatures, assuming
that you have the right key. Of course, you should do this anyway, even
if using HTTPS.

<http://mirrors.mit.edu/libreboot/> (MIT university, USA)

<http://mirror.linux.ro/libreboot/> (linux.ro, Romania)

<http://mirror.helium.in-berlin.de/libreboot/> (in-berlin.de, Germany)

<http://libreboot.mirror.si/> (mirror.si, Slovenia)

<http://ginette.swordarmor.fr/libreboot/> (swordarmor.fr, France)

<http://nephelai.zanity.net/mirror/libreboot/> (zanity.net, New Zealand)

FTP mirrors {#ftp}
-----------

WARNING: FTP is also unencrypted, like HTTP. The same risks are present.

<ftp://ftp.mirrorservice.org/sites/libreboot.org/release/> (University
of Kent, UK)

<ftp://ftp.linux.ro/libreboot/> (linux.ro, Romania)

<ftp://libreboot.mirror.si/libreboot> (mirror.si, Slovenia)

Statically linked
------------------

Libreboot includes statically linked executables. If you need the
sources for those statically linked dependencies inside the executables,
then you can contact the libreboot project using the details on the home
page; source code will be provided. You can download this source code
from [here](ccsource/).
