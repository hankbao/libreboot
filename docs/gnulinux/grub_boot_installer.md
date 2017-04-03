% How to install GNU+Linux on a libreboot system

This section relates to preparing, booting and installing a GNU+Linux
distribution on your libreboot system, using nothing more than a USB
flash drive (and *dd*).

-   [List of recommended GNU+Linux distributions](../distros)
-   [Prepare the USB drive (in GNU+Linux)](#prepare)
-   [Installing GNU+Linux with full disk encryption](#encryption)
-   [Debian or Devuan net install?](#debian_netinstall)
-   [Booting ISOLINUX images (automatic method)](#parse_isolinux)
-   [Booting ISOLINUX images (manual method)](#manual_isolinux)
-   [Troubleshooting](#troubleshooting)

[Back to previous index](./)



**This section is only for the GRUB payload. For depthcharge (used on
CrOS devices in libreboot), instructions have yet to be written in the
libreboot documentation.**



Prepare the USB drive (in GNU+Linux)
------------------------------------

If you downloaded your ISO on an existing GNU+Linux system, here is how
to create the bootable GNU+Linux USB drive:

Connect the USB drive. Check dmesg:

    $ dmesg

Check lsblk to confirm which drive it is:

    $ lsblk

Check that it wasn't automatically mounted. If it was, unmount it. For
example:

    $ sudo umount /dev/sdX\*
    # umount /dev/sdX\*

dmesg told you what device it is. Overwrite the drive, writing your
distro ISO to it with dd. For example:

    $ sudo dd if=gnulinux.iso of=/dev/sdX bs=8M; sync
    # dd if=gnulinux.iso of=/dev/sdX bs=8M; sync

You should now be able to boot the installer from your USB drive.
Continue reading, for information about how to do that.

[Back to top of page](#pagetop).

Prepare the USB drive (in NetBSD)
---------------------------------

[This
page](https://wiki.netbsd.org/tutorials/how_to_install_netbsd_from_an_usb_memory_stick/)
on the NetBSD website shows how to create a NetBSD bootable USB drive
from within NetBSD itself. You should use the *dd* method documented
there. This will also work with any GNU+Linux ISO image.

Prepare the USB drive (in FreeBSD)
----------------------------------

[This page](https://www.freebsd.org/doc/handbook/bsdinstall-pre.html) on
the FreeBSD website shows how to create a bootable USB drive for
installing FreeBSD. Use the *dd* on that page. You can also use the same
instructions with any GNU+Linux ISO image..

Prepare the USB drive (in LibertyBSD or OpenBSD)
------------------------------------------------

If you downloaded your ISO on a LibertyBSD or OpenBSD system, here is
how to create the bootable GNU+Linux USB drive:

Connect the USB drive. Check dmesg:

    $ dmesg | tail
Check to confirm which drive it is, for example, if you think its sd3:

    $ disklabel sd3

Check that it wasn't automatically mounted. If it was, unmount it. For
example:

    $ doas umount /dev/sd3i

dmesg told you what device it is. Overwrite the drive, writing the
OpenBSD installer to it with dd. For example:

    $ doas dd if=gnulinux.iso of=/dev/rsdXc bs=1M; sync

You should now be able to boot the installer from your USB drive.
Continue reading, for information about how to do that.

[Back to top of page](#pagetop).



Installing GNU+Linux with full disk encryption
----------------------------------------------

-   [Installing Debian or Devuan GNU+Linux with full disk encryption
    (including /boot)](encrypted_debian.html)
-   [Installing Parabola GNU+Linux with full disk encryption (including
    /boot)](encrypted_parabola.html)

[Back to top of page](#pagetop).



Debian or Devuan net install?
-----------------------------

Download the Debian or Devuan net installer. You can download the ISO
from the homepage on [debian.org](https://www.debian.org/), or [the
Devuan homepage](https://www.devuan.org/) for Devuan. Use this on the
GRUB terminal to boot it from USB (for 64-bit Intel or AMD):\
**set root='usb0'\
linux /install.amd/vmlinuz\
initrd /install.amd/initrd.gz\
boot\
** If you are on a 32-bit system (e.g. X60):\
**set root='usb0'\
linux /install.386/vmlinuz\
initrd /install.386/initrd.gz\
boot**\
We recommend using the *MATE* desktop.

[Back to top of page](#pagetop).



Booting ISOLINUX images (automatic method)
------------------------------------------

Boot it in GRUB using the *Parse ISOLINUX config (USB)* option. A new
menu should appear in GRUB, showing the boot options for that distro;
this is a GRUB menu, converted from the usual ISOLINUX menu provided by
that distro.

[Back to top of page](#pagetop).



Booting ISOLINUX images (manual method)
---------------------------------------

*These are generic instructions. They may or may not be correct for your
distribution. You must adapt them appropriately, for whatever GNU+Linux
distribution it is that you are trying to install.*

If the ISOLINUX parser or *Search for GRUB configuration* options won't
work, then press C in GRUB to access the command line.\
grub> **ls**\
Get the device from above output, eg (usb0). Example:\
grub> **cat (usb0)/isolinux/isolinux.cfg**\
Either this will show the ISOLINUX menuentries for that ISO, or link to
other .cfg files, for example /isolinux/foo.cfg.\
If it did that, then you do:\
grub> **cat (usb0)/isolinux/foo.cfg**\
And so on, until you find the correct menuentries for ISOLINUX. **The
file */isolinux/foo.cfg* is a fictional example. Do not actually use
this example, unless you actually have that file, if it is
appropriate.**

For Debian or Devuan (and other debian-based distros), there are
typically menuentries listed in */isolinux/txt.cfg* or
*/isolinux/gtk.cfg*. For dual-architecture ISO images (i686 and
x86\_64), there may be separate files/directories for each architecture.
Just keep searching through the image, until you find the correct
ISOLINUX configuration file. NOTE: Debian 8.6 ISO only lists 32-bit boot
options in txt.cfg. This is important if you want 64-bit booting on your
system. Devuan versions based on Debian 8.x may also have the same
issue.

Now look at the ISOLINUX menuentry. It'll look like:\
**kernel /path/to/kernel\
append PARAMETERS initrd=/path/to/initrd MAYBE\_MORE\_PARAMETERS\
** GRUB works the same way, but in it's own way. Example GRUB
commands:\
grub> **set root='usb0'**\
grub> **linux /path/to/kernel PARAMETERS MAYBE\_MORE\_PARAMETERS**\
grub> **initrd /path/to/initrd**\
grub> **boot**\
Note: *usb0* may be incorrect. Check the output of the *ls* command in
GRUB, to see a list of USB devices/partitions. Of course this will vary
from distro to distro. If you did all of that correctly, then it should
now be booting your USB drive in the way that you specified.

[Back to top of page](#pagetop).



Troubleshooting
===============

Most of these issues occur when using libreboot with coreboot's 'text
mode' instead of the coreboot framebuffer. This mode is useful for
booting payloads like memtest86+ which expect text-mode, but for
GNU+Linux distributions it can be problematic when they are trying to
switch to a framebuffer because it doesn't exist.

In most cases, you should use the vesafb ROM images. Example filename:
libreboot\_ukdvorak\_vesafb.rom.

parabola won't boot in text-mode
---------------------------------

Use one of the ROM images with vesafb in the filename (uses coreboot
framebuffer instead of text-mode).

debian-installer graphical corruption in text-mode (Debian and Devuan)
----------------------------------------------------------------------

When using the ROM images that use coreboot's "text mode" instead of
the coreboot framebuffer, booting the Debian or Devuan net installer
results in graphical corruption because it is trying to switch to a
framebuffer which doesn't exist. Use that kernel parameter on the
'linux' line when booting it:\
**vga=normal fb=false**

This forces debian-installer to start in text-mode, instead of trying to
switch to a framebuffer.

If selecting text-mode from a GRUB menu created using the ISOLINUX
parser, you can press E on the menu entry to add this. Or, if you are
booting manually (from GRUB terminal) then just add the parameters.

This workaround was found on the page:
<https://www.debian.org/releases/stable/i386/ch05s04.html>. It should
also work for Debian, Devuan and any other apt-get distro that provides
debian-installer (text mode) net install method.

[Back to top of page](#pagetop).



Copyright © 2014, 2015, 2016 Leah Rowe <info@minifree.org>\
Copyright © 2016 Scott Bonds <scott@ggr.com>\
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

