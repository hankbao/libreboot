
Installing libreboot 
====================

This section relates to installing libreboot on supported targets.

NOTE: if running flashrom -p internal for software based flashing, and
you get an error related to /dev/mem access, you should reboot with
iomem=relaxed kernel parameter before running flashrom, or use a kernel
that has CONFIG\_STRICT\_DEVMEM not enabled.

[Back to previous index](../)



General information
-------------------

-   [Information about libreboot ROM images](#rom)

Flashing via software methods, on system:
-----------------------------------------

-   [How to update or install libreboot on all systems](#flashrom)
-   [ASUS KFSN4-DRE](#flashrom)
-   [ThinkPad X60/T60 (if running Lenovo BIOS)](#flashrom_lenovobios)
-   [Apple MacBook2,1](#flashrom_macbook21)
-   [ASUS Chromebook C201](c201.html)

Setting up programmers, for external flashing via hardware method
-----------------------------------------------------------------

-   [How to program an SPI flash chip with the BeagleBone
    Black](bbb_setup.html)
-   [How to program an SPI flash chip with the Raspberry
    Pi](rpi_setup.html)

Flashing via hardware methods, on system:
-----------------------------------------

-   [Gigabyte GA-G41-ES2L](ga-g41m-es2l.html)
-   [Intel D510MO](d510mo.html)
-   [Intel D945GCLF](d945gclf.html)
-   [ASUS KGPE-D16](kgpe-d16.html)
-   [ASUS KCMA-D8](kcma-d8.html)
-   [ASUS Chromebook C201](c201.html)
-   [ThinkPad X60](x60_unbrick.html)
-   [ThinkPad X60 Tablet](x60tablet_unbrick.html)
-   [ThinkPad T60](t60_unbrick.html)
-   [ThinkPad X200/X200S/X200T](x200_external.html)
-   [ThinkPad R400](r400_external.html)
-   [ThinkPad T400](t400_external.html)
-   [ThinkPad T500](t500_external.html)



Information about libreboot ROM images {#rom}
======================================

Libreboot distributes pre-compiled ROM images, built from the libreboot
source code. These images are provided for user convenience, so that
they don't have to build anything from source on their own.

The ROM images in each archive use the following at the end of the file
name, if they are built with the GRUB payload:
**\_*keymap*\_*mode*.rom**

Available *modes*: **vesafb** or **txtmode**. The *vesafb* ROM images
are recommended, in most cases; *txtmode* ROM images come with
MemTest86+, which requires text-mode instead of the usual framebuffer
used by coreboot native graphics initialization.

*keymap* can be one of several keymaps that keyboard supports (there are
quite a few), which affects the keyboard layout configuration that is
used in GRUB. It doesn't matter which ROM image you choose here, as far
as the keymap in GNU+Linux is concerned.

Keymaps are named appropriately according to each keyboard layout
support in GRUB. To learn how these keymaps are created, see
[../grub/\#grub\_keyboard](../grub/#grub_keyboard)

QEMU
----

Libreboot comes with ROM images built for QEMU, by default:

Examples of how to use libreboot ROM images in QEMU:

-   $ **qemu-system-i386 -M q35 -m 512 -bios
    qemu\_q35\_ich9\_keymap\_mode.rom**
-   $ **qemu-system-i386 -M pc -m 512 -bios
    qemu\_i440fx\_piix4\_keymap\_mode.rom**

You can optionally specify the **-serial stdio** argument, so that QEMU
will emulate a serial terminal on the standard input/output (most likely
your terminal emulator or TTY).

Other arguments are available for QEMU. The manual will contain more
information.

[Back to top of page.](#pagetop)



How to update or install libreboot (if you are already running libreboot or coreboot) {#flashrom}
=====================================================================================

On all current targets, updating libreboot can be accomplished without
disassembly and, therefore, without having to externally re-flash using
any dedicated hardware. In other words, you can do everything entirely
in software, directly from the OS that is running on your libreboot
system.


**If you are using libreboot\_src or git, then make sure that you built
the sources first (see [../git/\#build](../git/#build)).**


Look at the [list of ROM images](#rom) to see which image is compatible
with your device.

Are you currently running the original, proprietary firmware?
-------------------------------------------------------------

If you are currently running the proprietary firmware (not libreboot or
coreboot), then the flashing instructions for your system are going to
be different.

X60/T60 users running the proprietary firmware should refer to
[\#flashrom\_lenovobios](#flashrom_lenovobios). MacBook2,1 users running
Apple EFI should refer to [\#flashrom\_macbook21](#flashrom_macbook21)

X200 users, refer to [x200\_external.html](x200_external.html), R400
users refer to [r400\_external.html](r400_external.html), T400 users
refer to [t400\_external.html](t400_external.html), T500 users refer to
[t500\_external.html](t500_external.html)

ASUS KFSN4-DRE?
---------------

Internal flashing should work just fine, even if you are currently
booting the proprietary firmware.

Libreboot currently lacks documentation for externally re-flashing an
LPC flash chip. However, these boards have the flash chip inside of a
PLCC socket, and it is possible to hot-swap the chips. If you want to
back up your known-working image, simply hot-swap the chip for one that
is the same capacity, after having dumped a copy of the current firmware
(flashrom -p internal -r yourchosenname.rom), and then flash that chip
with the known-working image. Check whether the system still boots, and
if it does, then it should be safe to flash the new image (because you
now have a backup of the old image).

Keeping at least one spare LPC PLCC chip with working firmware on it is
highly recommended, in case of bricks.

**DO NOT hot-swap the chip with your bare hands. Use a PLCC chip
extractor. These can be found online. See
<http://www.coreboot.org/Developer_Manual/Tools#Chip_removal_tools>**

Do check the HCL entry: [../hcl/kfsn4-dre.html](../hcl/kfsn4-dre.html)

ASUS KGPE-D16?
--------------

If you have the proprietary BIOS, you need to flash libreboot
externally. See [kgpe-d16.html](kgpe-d16.html).

If you already have coreboot or libreboot installed, without write
protection on the flash chip, then you can do it in software (otherwise,
see link above).

**DO NOT hot-swap the chip with your bare hands. Use a PDIP-8 chip
extractor. These can be found online. See
<http://www.coreboot.org/Developer_Manual/Tools#Chip_removal_tools>**

Do check the HCL entry: [../hcl/kgpe-d16.html](../hcl/kgpe-d16.html)

ASUS KCMA-D8?
-------------

If you have the proprietary BIOS, you need to flash libreboot
externally. See [kcma-d8.html](kgpe-d8.html).

If you already have coreboot or libreboot installed, without write
protection on the flash chip, then you can do it in software (otherwise,
see link above).

**DO NOT hot-swap the chip with your bare hands. Use a PDIP-8 chip
extractor. These can be found online. See
<http://www.coreboot.org/Developer_Manual/Tools#Chip_removal_tools>**

Do check the HCL entry: [../hcl/kcma-d8.html](../hcl/kcma-d8.html)

Intel D945GCLF?
---------------

If you're running the original Intel factory BIOS, then you will need
to flash externally. For instructions on how to do that, refer to
[d945gclf.html](d945gclf.html).

Otherwise, read the generic instructions below for using the *flash*
script.

Are you currently running libreboot (or coreboot)?
--------------------------------------------------

X60/T60 users should be fine with this guide. If you write-protected the
flash chip, please refer to [x60\_unbrick.html](x60_unbrick.html),
[x60tablet\_unbrick.html](x60tablet_unbrick.html) or
[t60\_unbrick.html](t60_unbrick.html). *This probably does not apply to
you. Most people do not write-protect the flash chip, so you probably
didn't either.*

Similarly, it is possible to write-protect the flash chip in coreboot or
libreboot on GM45 laptops (X200/R400/T400/T500). If you did this, then
you will need to use the links above for flashing, treating your laptop
as though it currently has the proprietary firmware (because
write-protected SPI flash requires external re-flashing, as is also the
case when running the proprietary firmware).

If you did not write-protect the flash chip, or it came to you without
any write-protection (***libreboot does not write-protect the flash chip
by default, so this probably applies to you***), read on!

MAC address on GM45 (X200/R400/T400/T500)
-----------------------------------------

**Users of the X200/R400/T400/T500 take note:** The MAC address for the
onboard ethernet chipset is located inside the flash chip. Libreboot ROM
images for these laptops contain a generic MAC address by default, but
this is not what you want. *Make sure to change the MAC address inside
the ROM image, before flashing it. The instructions on
[../hcl/gm45\_remove\_me.html\#ich9gen](../hcl/gm45_remove_me.html#ich9gen)
show how to do this.*

It is important that you change the default MAC address, before
flashing. It will be printed on a sticker at the bottom of the laptop,
or it will be printed on a sticker next to or underneath the RAM.
Alternatively, and assuming that your current firmware has the correct
MAC address in it, you can get it from your OS.

Apple iMac 5,2?
---------------

Internal flashing works, even when flashing from Apple EFI to libreboot.
Continue reading the instructions below.

**NOTE: If you're flashing an older version of Libreboot, the iMac5,2
motherboard is compatible with the MacBook2,1. Simply flash a MacBook2,1
ROM image, and it should work.**

Flash chip size
---------------

Use this to find out:\
\# **flashrom -p internal -V**

All good?
---------

Excellent! Moving on\...

Download the *libreboot\_util.tar.xz* archive, and extract it. Inside,
you will find a directory called *flashrom*. This contains statically
compiled executable files of the *flashrom* utility, which you will use
to re-flash your libreboot system.

Simply use *cd* on your terminal, to switch to the *libreboot\_util*
directory. Inside, there is a script called *flash*, which will detect
what CPU architecture you have (e.g. i686, x86\_64) and use the
appropriate executable. It is also possible for you to build these
executables from the libreboot source code archives.


How to update the flash chip contents:\
$ **sudo ./flash update [yourrom.rom](#rom)**



Ocassionally, coreboot changes the name of a given board. If flashrom
complains about a board mismatch, but you are sure that you chose the
correct ROM image, then run this alternative command:\
$ **sudo ./flash forceupdate [yourrom.rom](#rom)**



You should see **"Verifying flash\... VERIFIED."** written at the end
of the flashrom output. **Shut down** after you see this, and then boot
up again after a few seconds.


[Back to top of page](#pagetop)



ThinkPad X60/T60: Initial installation guide (if running the proprietary firmware) {#flashrom_lenovobios}
==================================================================================

**This is for the ThinkPad X60 and T60 while running Lenovo BIOS. If you
already have coreboot or libreboot running, then go to
[\#flashrom](#flashrom) instead!**

**If you are flashing a Lenovo ThinkPad T60, be sure to read
[../hcl/\#supported\_t60\_list](../hcl/#supported_t60_list)**


**If you are using libreboot\_src or git, then make sure that you built
the sources first (see [../git/\#build](../git/#build)).**


**Warning: this guide will not instruct the user how to backup the
original Lenovo BIOS firmware. These backups are tied to each system,
and will not work on any other. For that, please refer to
<http://www.coreboot.org/Board:lenovo/x60/Installation>.**


**If you're using libreboot 20150518, note that there is a mistake in
the flashing script. do this: *rm -f patch && wget -O flash
https://notabug.org/vimuser/libreboot/raw/9d850543ad90b72e0e333c98075530b31e5d23f1/flash
&& chmod +x flash***

The first half of the procedure is as follows:\
$ **sudo ./flash i945lenovo\_firstflash [yourrom.rom](#rom).**



You should see within the output the following:\
**"Updated BUC.TS=1 - 64kb address ranges at 0xFFFE0000 and 0xFFFF0000
are swapped"**.

You should also see within the output the following:\
**"Your flash chip is in an unknown state"**, **"FAILED"** and
**"DO NOT REBOOT OR POWEROFF"**\
Seeing this means that the operation was a **resounding** success!
**DON'T PANIC**.

See this link for more details:
<http://thread.gmane.org/gmane.linux.bios.flashrom/575>.

If the above is what you see, then **SHUT DOWN**. Wait a few seconds,
and then boot; libreboot is running, but there is a 2nd procedure
***needed*** (see below).



When you have booted up again, you must also do this:\
$ **sudo ./flash i945lenovo\_secondflash [yourrom.rom](#rom)**

If flashing fails at this stage, try the following:\
$ **sudo ./flashrom/i686/flashrom -p
internal:laptop=force\_I\_want\_a\_brick -w [yourrom.rom](#rom)**



You should see within the output the following:\
**"Updated BUC.TS=0 - 128kb address range 0xFFFE0000-0xFFFFFFFF is
untranslated"**

You should also see within the output the following:\
**"Verifying flash\... VERIFIED."**


[Back to top of page.](#pagetop)



MacBook2,1: Initial installation guide (if running the proprietary firmware) {#flashrom_macbook21}
============================================================================


**If you have a MacBook1,1, refer to
[../hcl/\#macbook11](../hcl/#macbook11) for flashing instructions.**


**This is for the MacBook2,1 while running Apple EFI firmware. If you
already have coreboot or libreboot running, then go to
[\#flashrom](#flashrom) instead!**

Be sure to read the information in
[../hcl/\#macbook21](../hcl/#macbook21).

**Warning: this guide will not instruct the user how to backup the
original Apple EFI firmware. For that, please refer to
<http://www.coreboot.org/Board:apple/macbook21>.**


**If you are using libreboot\_src or git, then make sure that you built
the sources first (see [../git/\#build](../git/#build)).**


Look at the [list of ROM images](#rom) to see which image is compatible
with your device.


Use this flashing script, to install libreboot:\
$ **sudo ./flash i945apple\_firstflash [yourrom.rom](#rom)**



You should also see within the output the following:\
**"Verifying flash\... VERIFIED."**

Shut down.


[Back to top of page.](#pagetop)



Copyright © 2014, 2015, 2016 Leah Rowe <info@minifree.org>\
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

