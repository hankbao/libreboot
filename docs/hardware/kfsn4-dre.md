---
title: ASUS KFSN4-DRE server/workstation board 
...

This is a server board using AMD hardware (Fam10h). It can also be used
for building a high-powered workstation. Powered by libreboot.

Flashing instructions can be found at
[../install/\#flashrom](../install/#flashrom)

Form factor {#formfactor}
===========

These boards use the SSI EEB 3.61 form factor; make sure that your case
supports this. This form factor is similar to E-ATX in that the size is
identical, but the position of the screws are different.

Flash chips {#flashchips}
===========

These boards use LPC flash (not SPI), in a PLCC socket. The default
flash size 1MiB (8Mbits), and can be upgraded to 2MiB (16Mbits).
SST49LF080A is the default that the board uses. SST49LF016C is an
example of a 2MiB (16Mbits) chip, which might work. It is believed that
2MiB (16Mbits) is the maximum size available for the flash chip.

*DO NOT hot-swap the chip with your bare hands. Use a PLCC chip
extractor. These can be found online. See
<http://www.coreboot.org/Developer_Manual/Tools#Chip_removal_tools>*

Native graphics initialization {#graphics}
==============================

Native graphics initialization exists (XGI Z9s) for this board.
Framebuffer- and text-mode both work. A serial port is also available.

Memory
======

DDR2 533/667 Registered ECC. 16 slots. Total capacity up to 64GiB.

Hex-core CPUs {#hexcore}
=============

PCB revision 1.05G is the best version of this board (the revision
number will be printed on the board), because it can use dual hex-core
CPUs (Opteron 2400/8400 series). Other revisions are believed to only
support dual quad-core CPUs.

Current issues {#issues}
==============

-   There seems to be a 30 second bootblock delay (observed by
    tpearson); the system otherwise boots and works as expected. See
    [text/kfsn4-dre/bootlog.txt](text/kfsn4-dre/bootlog.txt) - this uses
    the 'simple' bootblock, while tpearson uses the 'normal'
    bootblock, which tpearson suspects may be a possible cause. This
    person says that they will look into it. [This
    config](http://review.coreboot.org/gitweb?p=board-status.git;a=blob;f=asus/kfsn4-dre/4.0-10101-g039edeb/2015-06-27T03:59:16Z/config.txt;h=4742905c185a93fbda8eb14322dd82c70641aef0;hb=055f5df4e000a97453dfad6c91c2d06ea22b8545)
    doesn't have the issue.

-   Text-mode is a bit jittery (but still usable). (the jitter
    disappears if using KMS, once the kernel starts. The jitter will
    remain, if booting the kernel in text-mode).

-   Booting from USB mass storage devices is not possible; neither GRUB
    nor SeaBIOS detect USB drives when present. USB keyboards function
    under both GRUB and SeaBIOS, albeit slowly under GRUB
    (several seconds per character typed).

Other information
=================

[specifications](ftp://ftp.sgi.com/public/Technical%20Support/Pdf%20files/Asus/kfsn4-dre.pdf)

Copyright © 2015 Leah Rowe <info@minifree.org>\

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License Version 1.3 or any later
version published by the Free Software Foundation
with no Invariant Sections, no Front Cover Texts, and no Back Cover Texts.
A copy of this license is found in [../fdl-1.3.md](../fdl-1.3.md)
