---
title: How to replace the default GRUB configuration file on a libreboot system 
...

Libreboot on x86 uses the GRUB
[payload](http://www.coreboot.org/Payloads#GRUB_2) by default, which
means that the GRUB configuration file (where your GRUB menu comes from)
is stored directly alongside libreboot and its GRUB payload executable,
inside the flash chip. In context, this means that installing
distributions and managing them is handled slightly differently compared
to traditional BIOS systems.

A libreboot (or coreboot) ROM image is not simply "flat"; there is an
actual filesystem inside called CBFS (coreboot filesystem). A utility
called 'cbfstool' allows you to change the contents of the ROM image.
In this case, libreboot is configured such that the 'grub.cfg' and
'grubtest.cfg' files exist directly inside CBFS instead of inside the
GRUB payload 'memdisk' (which is itself stored in CBFS).

You can either modify the GRUB configuration stored in the flash chip,
or you can modify a GRUB configuration file on the main storage which
the libreboot GRUB payload will automatically search for.

Here is an excellent writeup about CBFS (coreboot filesystem):
<http://lennartb.home.xs4all.nl/coreboot/col5.html>.

**This guide is \*only\* for the GRUB payload. If you use the
depthcharge payload, ignore this section entirely.**

Table of Contents
=================

-   [Introduction](#introduction)
-   [1st option: don't re-flash](#option1_dont_reflash)
-   [2nd option: re-flash](#option2_reflash)
    -   [Acquire the necessary utilities](#tools)
    -   [Acquiring the correct ROM image](#rom)
    -   [Extract grubtest from the ROM image](#extract_testconfig)
    -   [Re-insert the modified grubtest.cfg into the ROM
        image](#reinsert_modified_testconfig)
    -   [Testing](#testing)
    -   [Final steps](#final_steps)

Introduction
------------

Download the latest release from <http://libreboot.org/>\
**If you downloaded from git, refer to
[../git/\#build\_meta](../git/#build_meta) before continuing.**

There are several advantages to modifying the GRUB configuration stored
in CBFS, but this also means that you have to flash a new libreboot ROM
image on your system (some users feel intimidated by this, to say the
least). Doing so can be risky if not handled correctly, because it can
result in a bricked system (recovery is easy if you have the
[equipment](../install/bbb_setup.html) for it, but most people don't).
If you aren't up to that then don't worry; it is possible to use a
custom GRUB menu without flashing a new image, by loading a GRUB
configuration from a partition on the main storage instead.

1st option: don't re-flash {#option1_dont_reflash}
---------------------------

By default, GRUB in libreboot is configured to scan all partitions on
the main storage for /boot/grub/libreboot\_grub.cfg or
/grub/libreboot\_grub.cfg(for systems where /boot is on a dedicated
partition), and then use it automatically.

Simply create your custom GRUB configuration and save it to
**/boot/grub/libreboot\_grub.cfg** on the running system. The next time
you boot, GRUB (in libreboot) will automatically switch to this
configuration file. **This means that you do not have to re-flash,
recompile or otherwise modify libreboot at all!**

Ideally, your distribution should automatically generate a
libreboot\_grub.cfg file that is written specifically under the
assumption that it will be read and used on a libreboot system that uses
GRUB as a payload. If your distribution does not do this, then you can
try to add that feature yourself or politely ask someone involved with
or otherwise knowledgeable about the distribution to do it for you. The
libreboot\_grub.cfg could either contain the full configuration, or it
could chainload another GRUB ELF executable (built to be used as a
coreboot payload) that is located in a partition on the main storage.

If you want to adapt a copy of the existing *libreboot* GRUB
configuration and use that for the libreboot\_grub.cfg file, then follow
[\#tools](#tools), [\#rom](#rom) and
[\#extract\_testconfig](#extract_testconfig) to get the
***grubtest.cfg***. Rename ***grubtest.cfg*** to
***libreboot\_grub.cfg*** and save it to ***/boot/grub/*** on the
running system where it is intended to be used. Modify the file at that
location however you see fit, and then stop reading this guide (the rest
of this page is irrelevant to you); **in libreboot\_grub.cfg on disk, if
you are adapting it based on grub.cfg from CBFS then remove the check
for libreboot\_grub.cfg otherwise it will loop.**.

2nd option: re-flash {#option2_reflash}
--------------------

You can modify what is stored inside the flash chip quite easily. Read
on to find out how.

Acquire the necessary utilities {#tools}
-------------------------------

Use ***cbfstool*** and ***flashrom***. There are available in the
*libreboot\_util* release archive, or they can be compiled (see
[../git/\#build\_flashrom](../git/#build_flashrom)). Flashrom is also
available from the repositories:
    # pacman -S flashrom

Acquiring the correct ROM image {#rom}
-------------------------------

You can either work directly with one of the ROM images already included
in the libreboot ROM archives, or re-use the ROM that you have currently
flashed. For the purpose of this tutorial it is assumed that your ROM
image file is named *libreboot.rom*, so please make sure to adapt.

ROM images are included pre-compiled in libreboot. You can also dump
your current firmware, using flashrom:
    $ sudo flashrom -p internal -r libreboot.rom
    # flashrom -p internal -r libreboot.rom
If you are told to specify the chip, add the option **-c {your chip}**
to the command, for example:

    # flashrom -c MX25L6405 -p internal -r libreboot.rom

Extract grubtest.cfg from the ROM image {#extract_testconfig}
---------------------------------------

You can check the contents of the ROM image, inside CBFS:

    $ cd \.../libreboot\_util/cbfstool** $ ./cbfstool libreboot.rom
print**

The files *grub.cfg* and *grubtest.cfg* should be present. grub.cfg is
loaded by default, with a menuentry for switching to grubtest.cfg. In
this tutorial, you will first modify and test *grubtest.cfg*. This is to
reduce the possibility of bricking your device, so DO NOT SKIP THIS!

Extract grubtest.cfg from the ROM image:

    $ ./cbfstool libreboot.rom extract -n grubtest.cfg -f grubtest.cfg

Modify the grubtest.cfg accordingly.

Re-insert the modified grubtest.cfg into the ROM image {#reinsert_modified_testconfig}
------------------------------------------------------

Once your grubtest.cfg is modified and saved, delete the unmodified
config from the ROM image:

    $ ./cbfstool libreboot.rom remove -n grubtest.cfg

Next, insert the modified version:\
**\$ ./cbfstool libreboot.rom add -n grubtest.cfg -f grubtest.cfg -t
raw**

Testing
-------

**Now you have a modified ROM. Refer back to
[../install/\#flashrom](../install/#flashrom) for information on how to
flash it.
    $ cd /libreboot\_util** \# **./flash update libreboot.rom\
Ocassionally, coreboot changes the name of a given board. If flashrom
complains about a board mismatch, but you are sure that you chose the
correct ROM image, then run this alternative command:
    # ./flash forceupdate libreboot.rom
You should see **"Verifying flash\... VERIFIED."** written at the end
of the flashrom output. Once you have done that, shut down and then boot
up with your new test configuration.**

Choose (in GRUB) the menu entry that switches to grubtest.cfg. If it
works, then your config is safe and you can continue below.

**If it does not work like you want it to, if you are unsure or
sceptical in any way, then re-do the steps above until you get it right!
Do \*not\* proceed past this point unless you are 100% sure that your
new configuration is safe (or desirable) to use.**

Final steps {#final_steps}
-----------

When you are satisfied booting from grubtest.cfg, you can create a copy
of grubtest.cfg, called grub.cfg. This is the same except for one
difference: the menuentry 'Switch to grub.cfg' will be changed to
'Switch to grubtest.cfg' and inside it, all instances of grub.cfg to
grubtest.cfg. This is so that the main config still links (in the menu)
to grubtest.cfg, so that you don't have to manually switch to it, in
case you ever want to follow this guide again in the future (modifying
the already modified config). From /libreboot\_util/cbfstool, do:\
\$ **sed -e 's:(cbfsdisk)/grub.cfg:(cbfsdisk)/grubtest.cfg:g' -e
's:Switch to grub.cfg:Switch to grubtest.cfg:g' < grubtest.cfg >
grub.cfg**\

Delete the grub.cfg that remained inside the ROM:

    $ ./cbfstool libreboot.rom remove -n grub.cfg

Add the modified version that you just made:

    $ ./cbfstool libreboot.rom add -n grub.cfg -f grub.cfg -t raw

**Now you have a modified ROM. Again, refer back to
[../install/\#flashrom](../install/#flashrom) for information on how to
flash it. It's the same method as you used before. Shut down and then
boot up with your new configuration.**

Copyright © 2014, 2015 Leah Rowe <info@minifree.org>\
Copyright © 2015 Jeroen Quint <jezza@diplomail.ch>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
