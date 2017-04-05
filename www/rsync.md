---
title: Mirroring libreboot
...

Create a directory in your web directory (e.g. libreboot/) and put one
of these in your crontab:

Libreboot project, UK (main rsync mirror)
-----------------------------------------

**rsync -avxP --delete --stats rsync.libreboot.org::mirrormirror
/path/to/docroot/libreboot/**

University of Kent, UK (backup mirror)
--------------------------------------

**rsync -avxP --delete --stats
rsync://rsync.mirrorservice.org/libreboot.org/release/
/path/to/docroot/libreboot/**

Princeton university, USA (backup mirror)
-----------------------------------------

**rsync -avxP --delete --stats
rsync://mirror.math.princeton.edu/pub/libreboot/
/path/to/docroot/libreboot/**

linux.ro, Romania (backup mirror)
---------------------------------

**rsync -avxP --delete --stats rsync://ftp.linux.ro/libreboot/
/path/to/docroot/libreboot/**

partyvan.eu, Sweden (backup mirror)
-----------------------------------

**rsync -avxP --delete --stats
rsync://mirror.se.partyvan.eu/pub/libreboot/
/path/to/docroot/libreboot/**

mirror.si, Slovenia (backup mirror)
-----------------------------------

**rsync -avxP --delete --stats rsync://libreboot.mirror.si/libreboot
/path/to/docroot/libreboot/**

Are you running a mirror? Contact the libreboot project, and the link will be
added to the [download](download.md) page.

Libreboot includes statically linked executables. If you need the
sources for those statically linked dependencies inside the executables,
then you can contact the libreboot project using the details on the home
page; source code will be provided. You can download this source code
from [here](ccsource.md).
