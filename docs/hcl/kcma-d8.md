
ASUS KCMA-D8 desktop/workstation board 
======================================

This is a desktop board using AMD hardware (Fam10h **and Fam15h** CPUs
available). It can also be used for building a high-powered workstation.
Powered by libreboot. The coreboot port was done by Timothy Pearson of
Raptor Engineering Inc. and, working with them, merged into libreboot.

**Memory initialization is still problematic, for some modules. We
recommend avoiding Kingston modules.**

Flashing instructions can be found at
[../install/\#flashrom](../install/#flashrom) - note that external
flashing is required (e.g. BBB), if the proprietary (ASUS) firmware is
currently installed. If you already have libreboot, by default it is
possible to re-flash using software running in GNU+Linux on the kcma-d8,
without using external hardware.

[Back to previous index](./).



CPU compatibility
=================

**Use Opteron 4200 series (works without microcode updates, including hw
virt).** 4300 series needs microcode updates, so avoid those CPUs. 4100
series is too old, and mostly untested.



Board status (compatibility) {#boardstatus}
============================

See <https://raptorengineeringinc.com/coreboot/kcma-d8-status.php>.



Form factor {#formfactor}
===========

These boards use the SSI EEB 3.61 form factor; make sure that your case
supports this. This form factor is similar to E-ATX in that the size is
identical, but the position of the screws are different.



IPMI iKVM module add-on {#ipmi}
=======================

Don't use it. It uses proprietary firmware and adds a backdoor (remote
out-of-band management chip, similar to the [Intel Management
Engine](http://libreboot.org/faq/#intelme). Fortunately, the firmware is
unsigned (possibly to replace) and physically separate from the
mainboard since it's on the add-on module, which you don't have to
install.



Flash chips {#flashchips}
===========

2MiB flash chips are included by default, on these boards. It's on a
P-DIP 8 slot (SPI chip). The flash chip can be upgraded to higher sizes:
4MiB, 8MiB or 16MiB. With at least 8MiB, you could feasibly fit a
compressed linux+initramfs image (BusyBox+Linux system) into CBFS and
boot that, loading it into memory.

Libreboot has configs for 2, 4, 8 and 16 MiB flash chip sizes (default
flash chip is 2MiB).

**DO NOT hot-swap the chip with your bare hands. Use a P-DIP 8 chip
extractor. These can be found online. See
<http://www.coreboot.org/Developer_Manual/Tools#Chip_removal_tools>**



Native graphics initialization {#graphics}
==============================

Only text-mode is known to work, but linux(kernel) can initialize the
framebuffer display (if it has KMS - kernel mode setting).



Current issues {#issues}
==============

-   LRDIMM memory modules are currently incompatible
-   SAS (via PIKE 2008 module) requires non-free option ROM (and
    SeaBIOS) to boot from it (theoretically possible to replace, but you
    can put a kernel in CBFS or on SATA and use that to boot GNU, which
    can be on a SAS drive. The linux kernel can use those SAS drives
    (via PIKE module) without an option ROM).
-   IPMI iKVM module (optional add-on card) uses proprietary firmware.
    Since it's for remote out-of-band management, it's theoretically a
    backdoor similar to the Intel Management Engine. Fortunately, unlike
    the ME, this firmware is unsigned which means that a free
    replacement is theoretically possible. For now, the libreboot
    project recommends not installing the module. [This
    project](https://github.com/facebook/openbmc) might be interesting
    to derive from, for those who want to work on a free replacement. In
    practise, out-of-band management isn't very useful anyway (or at
    the very least, it's not a major inconvenience to not have it).
-   Graphics: only text-mode works. See [\#graphics](#graphics)



Hardware specifications {#specifications}
-----------------------

Check the ASUS website.



Copyright © 2016 Leah Rowe &lt;info@minifree.org&gt;\
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

