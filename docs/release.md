% Libreboot release information 

Updated versions of libreboot can be found at
[libreboot.org](http://libreboot.org/).

Old releases
------------

See [archive\_old.html](archive_old.html) for information about older
libreboot releases.

Libreboot 20160907 {#release20160907}
==================

Release date: 7th September 2016

For existing boards, there are no new board specific changes.

This release adds one new mainboard to libreboot:

-   Intel D945GCLF desktop motherboard (thanks to Arthur Heymans)

Other bugfixes:

-   Various improvements to the documentation
-   re-added "unset superusers" to the grub.cfg, which was needed for
    some users depending on the distros that they used

Libreboot 20160902 {#release20160922}
==================

Release date: 2nd September 2016.

This fixes build issues in the previous 20160818 release. See change log
for details.

Libreboot 20160818 {#release20160818}
==================

Release date: 18th August 2016.

Installation instructions can be found at ***docs/install/***. Building
instructions (for source code) can be found at ***docs/git/#build***.

Machines supported in this release:
-----------------------------------

-   **ASUS Chromebook C201**
    -   Check notes in ***docs/hcl/c201.html***
-   **Gigabyte GA-G41M-ES2L desktop motherboard**
    -   Check notes in ***docs/hcl/ga-g41m-es2l.html***
-   **Intel D510MO desktop motherboard**
    -   Check notes in ***docs/hcl/d510mo.html***
-   **Intel D945GCLF desktop motherboard**
    -   Check notes in ***docs/hcl/d945gclf.html***
-   **Apple iMac 5,2**
    -   Check notes in ***docs/hcl/imac52.html***
-   **ASUS KFSN4-DRE server board**
    -   PCB revision 1.05G is the best version (can use 6-core CPUs)
    -   Check notes in ***docs/hcl/kfsn4-dre.html***
-   **ASUS KGPE-D16 server board**
    -   Check notes in ***docs/hcl/kgpe-d16.html***
-   **ASUS KCMA-D8 desktop/workstation board**
    -   Check notes in ***docs/hcl/kcma-d8.html***
-   **ThinkPad X60/X60s**
    -   You can also remove the motherboard from an X61/X61s and replace
        it with an X60/X60s motherboard. An X60 Tablet motherboard will
        also fit inside an X60/X60s.
-   **ThinkPad X60 Tablet** (1024x768 and 1400x1050) with digitizer
    support
    -   See ***docs/hcl/#supported\_x60t\_list*** for list of supported
        LCD panels
    -   It is unknown whether an X61 Tablet can have it's mainboard
        replaced with an X60 Tablet motherboard.
-   **ThinkPad T60** (Intel GPU) (there are issues; see below):
    -   See notes below for exceptions, and
        ***docs/hcl/#supported\_t60\_list*** for known working LCD
        panels.
    -   It is unknown whether a T61 can have it's mainboard replaced
        with a T60 motherboard.
    -   See ***docs/future/#t60\_cpu\_microcode***.
    -   T60P (and T60 laptops with ATI GPU) will likely never be
        supported: ***docs/hcl/#t60\_ati\_intel***
-   **ThinkPad X200**
    -   X200S and X200 Tablet are also supported, conditionally; see
        ***docs/hcl/x200.html#x200s***
    -   **ME/AMT**: libreboot removes this, permanently.
        ***docs/hcl/gm45\_remove\_me.html***
-   **ThinkPad R400**
    -   See ***docs/hcl/r400.html***
    -   **ME/AMT**: libreboot removes this, permanently.
        ***docs/hcl/gm45\_remove\_me.html***
-   **ThinkPad T400**
    -   See ***docs/hcl/t400.html***
    -   **ME/AMT**: libreboot removes this, permanently.
        ***docs/hcl/gm45\_remove\_me.html***
-   **ThinkPad T500**
    -   See ***docs/hcl/t500.html***
    -   **ME/AMT**: libreboot removes this, permanently.
        ***docs/hcl/gm45\_remove\_me.html***
-   **Apple MacBook1,1** (MA255LL/A, MA254LL/A, MA472LL/A)
    -   See ***docs/hcl/#macbook11***.
-   **Apple MacBook2,1** (MA699LL/A, MA701LL/A, MB061LL/A, MA700LL/A,
    MB063LL/A, MB062LL/A)
    -   See ***docs/hcl/#macbook21***.

Changes for this release, relative to r20150518 (earliest changes last, recent changes first)
---------------------------------------------------------------------------------------------

-   NEW BOARDS ADDED:
    -   ASUS Chromebook C201 (ARM laptop) (thanks to Paul Kocialkowski)
    -   Gigabyte GA-G41M-ES2L motherboard (desktop) (thanks to Damien
        Zammit)
    -   Intel D510MO motherboard (desktop) (thanks to Damien Zammit)
    -   ASUS KCMA-D8 motherboard (desktop) (thanks to Timothy Pearson)
    -   ASUS KFSN4-DRE motherboard (server) (thanks to Timothy Pearson)
    -   ASUS KGPE-D16 motherboard (server) (thanks to Timothy Pearson)

For details development history on these boards, refer to the git log
and documentation.

For boards previously supported, many fixes from upstream have been
merged.

Other changes (compared to libreboot 20150518), for libreboot in general
or for previously supported systems: (this is a summary. For more
detailed change list, refer to the git log)

256MiB VRAM allocated on GM45 (X200, T400, T500, R400) instead of 32MiB.
This is an improvement over both Lenovo BIOS and Libreboot 20150518,
allowing video decoding at 1080p to be smoother. (thanks Arthur Heymans)
To clarify, GM45 video performance in libreboot 20160818 is better than
on the original BIOS and the previous libreboot release.

64MiB VRAM on i945 (X60, T60, MacBook2,1) now supported in
coreboot-libre, and used by default (in the previous release, it was
8MiB allocated). Thanks to Arthur Heymans.

Higher battery life on GM45 (X200, T400, T500, R400) due to higher
cstates now being supported (thanks Arthur Heymans). C4 power states
also supported.

Higher battery life on i945 (X60, T60, MacBook2,1) due to better CPU
C-state settings. (Deep C4, Dynamicl L2 shrinking, C2E).

Text mode on GM45 (X200, T400, T500, R400) now works, making it possible
to use MemTest86+ comfortably. (thanks to Nick High from coreboot)

Dual channel LVDS displays on GM45 (T400, T500) are now automatically
detected in coreboot-libre. (thanks Vladimir Serbinenko from coreboot)

Partial fix in coreboot-libre for GRUB display on GM45, for dual channel
LVDS higher resolution LCD panels (T400, T500). (thanks Arthur Heymans)

Massively improved GRUB configuration, making it easier to boot more
encrypted systems automatically, and generally a more useful menu for
booting the system (thanks go to Klemens Nanni of the autoboot project).
Libreboot now uses the grub.cfg provided by the installed GNU+Linux
distribution automatically, if present, switching to that configuration.
This is done across many partitions, where libreboot actively searches
for a configuration file (also on LVM volumes and encrypted volumes).
This should make libreboot more easy to use for non-technical users,
without having to modify the GRUB configuration used in libreboot.

Utilities archives is now source only. You will need to compile the
packages in there (build scripts included, and a script for installing
build dependencies). (binary utility archives are planned again in the
next release, when the new build system is merged)

SeaGRUB is now the default payload on all x86 boards. (SeaBIOS
configured to load a compressed GRUB payload from CBFS immediately,
without providing an interface in SeaBIOS. This way, GRUB is still used
but now BIOS services are available, so you get the best of both
worlds). Thanks go to Timothy Pearson of coreboot for this idea.

crossgcc is now downloaded and built as a separate module to
coreboot-libre, with a universal revision used to build all boards.

Individual boards now have their own coreboot revision and patches,
independently of each other board. This makes maintenance easier.

Updated all utilities, and modules (coreboot, GRUB, etc) to newer
versions, with various bugfixes and improvements upstream.

RTC century byte issue now fixed on GM45 in coreboot-libre, so the date
should now be correctly displayed when running the latest linux kernel,
instead of seeing 1970-01-01 when you boot (thanks to Alexander Couzens
from coreboot)

Build system now uses multiple CPU cores when building, speeding up
building for some people. Manually specifying how many cores are needed
is also possible, for those using the build system in a chroot
environment. (thanks go to Timothy Pearson from coreboot)

In the build system (git repository), https:// is now used when cloning
coreboot. http:// is used as a fallback for GRUB, if git:// fails.

New payload, the depthcharge bootloader (free bootloader maintained by
Google) for use on the ASUS Chromebook C201. (thanks go to Paul
Kocialkowski)

Various fixes to the ich9gen utility (e.g. flash component density is
now set correctly in the descriptor, gbe-less descriptors now supported)

Copyright © 2014, 2015, 2016 Leah Rowe <info@minifree.org>

Permission is granted to copy, distribute and/or modify this document
under the terms of the Creative Commons Attribution-ShareAlike 4.0
International license or any later version published by Creative
Commons; A copy of the license can be found at
[cc-by-sa-4.0.txt](cc-by-sa-4.0.txt)

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

