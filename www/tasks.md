---
title: Tasks for the next stable version release of Libreboot
...

Libreboot now has an issue tracker, at
<https://notabug.org/libreboot/libreboot/issues>. The purpose of this page is
to list current high priority tasks, which are desirable and/or required for
the next release.

Libreboot releases happen whenever Libreboot is in a state that the maintainers
agree is suitable to be considered for making a new stable release. This is to
be determined by overall consensus, per release.

This page is updated periodically, to reflect the issues currently open on the
bug tracker.

Port x86 to the Paper build system
----------------------------------

<https://notabug.org/libreboot/libreboot/issues/108>

Paper is the new build system in Libreboot, feature-complete for ARM and with
many improvements, but the old build system is still used for x86 Intel and AMD
systems in Libreboot.

Paper has more features, is more modular, is better designed and superior in
terms of maintainability.

Dump coreboot as firmware upstream. Adopt librecore instead
-----------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/44>

It may be necessary to keep coreboot, for the ARM chromebooks (rk3288 chipset).

Libreboot is a coreboot distribution, which integrates payloads, utilities,
documentation, additional patches/fixes and integrates everything together.
You can think of it like a GNU+Linux distribution, but it's a distribution of
the boot firmware, not the operating system. Libreboot incorporates coreboot,
deblobbing it with deblob scripts.

Librecore is a whole new fork of coreboot, and it is its own upstream, with
many coreboot developers working on it. It deblobs coreboot in-tree, without
deblob scripts. The only blobs that Librecore distributes is CPU microcode
updates, and we can simply exclude those in Libreboot, just as the current
build system deletes them after it downloads coreboot.

Librecore generally has a much more focused team, and their priority is libre
hardware. It is a very suitable upstream for Libreboot, much more so than the
coreboot project. Librecore is a very new project (formed in December 2016).

See: <http://librecore.info/>
IRC channel (on freenode): #librecore

Its main developers are damo22, avph, funfunctor and others from the #coreboot
IRC channel. Of interest: damo22 and avph used to work for Libreboot, before
forming the Librecore project.

Current board fixes in Libreboot's git repository, not in the current stable
release of Libreboot:

-   gm45: use correct vbt string (needed for external monitors)
-   i945: add fake vbt (needed for external screens)
-   ga-g41m-es2l: enable use of PCIe x16 port
-   i945: add support for external monitors in grub
-   add vesafb mode to ga-g41m-es2l
-   gm45: add support for external monitors in grub
-   Fix low-res screens on T60
-   add hda verb to t60 (fixes mic)
-   gm45: fix IOMMU

There may be others. The above patches are most likely already merged in
Librecore's main Git repository. This should be investigated. If so, then they
can be deleted from Libreboot since they would already be merged upstream.

Reproducible builds
-------------------

<https://notabug.org/libreboot/libreboot/issues/15>

NOTE: Integrate librecore first.

NOTE: Finish Paper integration first.

ThinkPad X220 support
---------------------

<https://notabug.org/libreboot/libreboot/issues/48>

NOTE: Merge after Librecore is adopted in Libreboot.

ThinkPad T400S support
----------------------

<https://notabug.org/libreboot/libreboot/issues/7>

NOTE: Merge after Librecore is adopted in Libreboot.

Lenovo G505S support
--------------------

<https://notabug.org/libreboot/libreboot/issues/104>

NOTE: Merge after Librecore is adopted in Libreboot.

ThinkPad W500 support
---------------------

<https://notabug.org/libreboot/libreboot/issues/87>

Merge this upstream in coreboot and librecore, and port downstream to
Libreboot

NOTE: Merge after Librecore is adopted in Libreboot.

ThinkPad R500 support
---------------------

<https://notabug.org/libreboot/libreboot/issues/107>

Merge this upstream in coreboot and librecore, and port downstream to
Libreboot.

NOTE: Merge after Librecore is adopted in Libreboot.

Fix broken reboot on Intel D510MO
---------------------------------

<https://notabug.org/libreboot/libreboot/issues/103>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Fedora doesn't boot on Libreboot systems. Fix that
--------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/102>

This is a Fedora issue.

TODO: Consider closing this bug (report it to Fedora instead).
NOTE: This is most likely E\_NOT\_OUR\_BUG.

Add support for various i945 desktops
-------------------------------------

<https://notabug.org/libreboot/libreboot/issues/45>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Adopt Unofficial BASH Strict Mode in the entire build system (Paper)
--------------------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/84>

NOTE: Merge x86 into Paper build system first.

Investigate GPU hanging issue on GM45 laptops
---------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/62>

Fix this in Librecore, if possible. Otherwise, submit bug report (regression)
on Linux kernel bug tracker.

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Fix or work around for issues with dock drive hotplug detection on Linux 4.10.x
-------------------------------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/61>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Fix Parabola build issue for cbfstool in coreboot
-------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/59>

This may already be fixed in Librecore, our future upstream.

NOTE: This may affect any GNU+Linux distribution which uses a modern toolchain.

Disable framebuffer backdoor on SandyBridge (for ThinkPad X220)
---------------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/58>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Add SeaBIOS payload option for all x86 boards in Libreboot
----------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/55>

Fix boot issue for Debian and other systems (and fix FDE in grub.cfg)
---------------------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/54>

i945 and GM45: Support fine tuning BLC\_PWM\_CTL register value in CBFS
-----------------------------------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/50>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Add support for low-power cstates on MacBook2,1
-----------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/49>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Implement support for T400 docking station
------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/46>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.

Bug: KGPE-D16 stuck at: calibrating apic timer
----------------------------------------------

<https://notabug.org/libreboot/libreboot/issues/39>

NOTE: Investigate this in Librecore. Libreboot is abandoning coreboot.
