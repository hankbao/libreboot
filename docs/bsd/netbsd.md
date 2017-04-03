% How to install NetBSD on a libreboot system

This section relates to preparing, booting and installing NetBSD on your
libreboot system, using nothing more than a USB flash drive (and *dd*).
They've only been tested on a librebooted ThinkPad X60.

It is expected that you use text mode in libreboot (txtmode images), for
the early boot process in NetBSD. **Suspend/hibernate is broken,
according to at least 1 user.**

Thanks go to ioxcide in [this Reddit
post](https://www.reddit.com/r/BSD/comments/53jt70/libreboot_and_bsds/)
for the initial instructions.

-   [Prepare the USB drive (in NetBSD)](#prepare)
-   [Installing NetBSD without full disk encryption](#noencryption)
-   [Installing NetBSD with full disk encryption](#encryption)
-   [Booting](#booting)
-   [Configuring Grub](#configuring_grub)
-   [Troubleshooting](#troubleshooting)

[Back to previous index](./)

**This section is only for the GRUB payload. For depthcharge (used on
CrOS devices in libreboot), instructions have yet to be written in the
libreboot documentation.**

netbsd.iso is the installation image for NetBSD. Adapt the filename
accordingly, for your version of NetBSD.

Prepare the USB drive (in NetBSD)
---------------------------------

[This
page](https://wiki.netbsd.org/tutorials/how_to_install_netbsd_from_an_usb_memory_stick/)
on the NetBSD website shows how to create a NetBSD bootable USB drive
from within NetBSD itself. You should use the *dd* method documented
there.

Prepare the USB drive (in FreeBSD)
----------------------------------

[This page](https://www.freebsd.org/doc/handbook/bsdinstall-pre.html) on
the FreeBSD website shows how to create a bootable USB drive for
installing FreeBSD. Use the *dd* on that page. You can also use the same
instructions with a NetBSD ISO image.

Prepare the USB drive (in LibertyBSD or NetBSD)
-----------------------------------------------

If you downloaded your ISO on a LibertyBSD or NetBSD system, here is how
to create the bootable NetBSD USB drive:

Connect the USB drive. Check dmesg:

    $ dmesg | tail

Check to confirm which drive it is, for example, if you think its sd3:

    $ disklabel sd3

Check that it wasn't automatically mounted. If it was, unmount it. For
example:

    $ doas umount /dev/sd3i

dmesg told you what device it is. Overwrite the drive, writing the
NetBSD installer to it with dd. For example:

    $ doas netbsd.iso of=/dev/rsdXc bs=1M; sync

You should now be able to boot the installer from your USB drive.
Continue reading, for information about how to do that.

[Back to top of page](#pagetop).

Prepare the USB drive (in GNU+Linux)
------------------------------------

If you downloaded your ISO on a GNU+Linux system, here is how to create
the bootable NetBSD USB drive:

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

    $ sudo dd if=install60.fs of=/dev/sdX bs=8M; sync
    # dd if=netbsd.iso of=/dev/sdX bs=8M; sync

You should now be able to boot the installer from your USB drive.
Continue reading, for information about how to do that.

[Back to top of page](#pagetop).

Installing NetBSD without full disk encryption
----------------------------------------------

You might have to use an external USB keyboard during the installation.
Press C to access the GRUB terminal.

    grub> knetbsd -r sd0a (usb0,netbsd1)/netbsd
    grub> boot

It will start booting into the NetBSD installer. Follow the normal
process for installing NetBSD.

[Back to top of page](#pagetop).

Installing NetBSD with full disk encryption
-------------------------------------------

TODO

[Back to top of page](#pagetop).

Booting
-------

Press C in GRUB to access the command line:

    grub> knetbsd -r wd0a (ahci0,netbsd1)/netbsd
    grub> boot

NetBSD will start booting. Yay!

[Back to top of page](#pagetop).

Configuring Grub
----------------

If you don't want to drop to the GRUB command line and type in a
command to boot NetBSD every time, you can create a GRUB configuration
that's aware of your NetBSD installation and that will automatically be
used by libreboot.

On your NetBSD root partition, create the **/grub** directory and add
the file **libreboot\_grub.cfg** to it. Inside the
**libreboot\_grub.cfg** add these lines:

**default=0 timeout=3 menuentry "NetBSD" {\
    knetbsd -r wd0a (ahci0,netbsd1)/netbsd\
}\
**

The next time you boot, you'll see the old Grub menu for a few seconds,
then you'll see the a new menu with only NetBSD on the list. After 3
seconds NetBSD will boot, or you can hit enter to boot.

[Back to top of page](#pagetop).

Troubleshooting
===============

Most of these issues occur when using libreboot with coreboot's 'text
mode' instead of the coreboot framebuffer. This mode is useful for
booting payloads like memtest86+ which expect text-mode, but for NetBSD
it can be problematic when they are trying to switch to a framebuffer
because it doesn't exist.

In most cases, you should use the vesafb ROM images. Example filename:
libreboot\_ukdvorak\_vesafb.rom.

won't boot\...something about file not found
---------------------------------------------

Your device names (i.e. usb0, usb1, sd0, sd1, wd0, ahci0, hd0, etc) and
numbers may differ. Use TAB completion.

[Back to top of page](#pagetop).

Copyright © 2016 Leah Rowe <info@minifree.org>\
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

