% ASUS KFSN4-DRE server/workstation board 

This is a server board using AMD hardware (Fam10h). It can also be used
for building a high-powered workstation. Powered by libreboot.

Flashing instructions can be found at
[../install/\#flashrom](../install/#flashrom)

[Back to previous index](./).



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

**DO NOT hot-swap the chip with your bare hands. Use a PLCC chip
extractor. These can be found online. See
<http://www.coreboot.org/Developer_Manual/Tools#Chip_removal_tools>**



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



Other information
=================

[specifications](ftp://ftp.sgi.com/public/Technical%20Support/Pdf%20files/Asus/kfsn4-dre.pdf)



Copyright © 2015 Leah Rowe <info@minifree.org>\
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

