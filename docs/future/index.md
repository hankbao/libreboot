% Development notes 

These are development notes, for future use. For old (obselete) notes,
see [old.html](old.html).

Or go [back to main task list](../tasks.html).

Table of contents
=================

-   [Standard test](#standard_test)
-   [T60 cpu microcode](#t60_cpu_microcode)
-   [i945 VRAM size](#i945_vram_size)
-   [LCD panels on i945 - fix incompatible
    panels](#lcd_i945_incompatibility)

-   [i945 X60/T60 VBT implementation (experimental: testing)](#i945_vbt)
-   [IntelVbtTool results](#intelvbttool_results)
-   [Fallback patches for i945](#fallback_patches)

standard test {#standard_test}
=============

These logs are usually obtained when testing changes related to graphics
on i945 (X60 and T60).

-   Make a copy of these files:
    -   /var/log/dmesg
    -   /var/log/kern.log
    -   /var/log/Xorg.0.log
    -   /proc/ioports
    -   /proc/iomem
    -   /sys/class/drm/card0/error
-   Record these outputs:
    -   sudo intel\_reg\_dumper
    -   uname -r
    -   lspci -vvvvnnnnxxxx
    -   sudo modprobe msr
    -   sudo inteltool -a
    -   sudo cbmem -c
-   Try some 3D games with latest kernel.

T60 cpu microcode {#t60_cpu_microcode}
=================

TODO: T60: find (for rare buggy CPUs that are unstable without microcode
updates) if there is a workaround (patched kernel, special parameter,
etc) So far, only 1 processor has been found to have issues. See
microcode errata sheets
http://download.intel.com/design/mobile/SPECUPDT/31407918.pdf and
http://download.intel.com/design/mobile/SPECUPDT/30922214.pdf and then
look at the debugging results collected in [t7200q](../t7200q) directory
(q means quirk).

Every other T7200 tested so far has worked without microcode updates.

i945 VRAM size {#i945_vram_size}
==============

Apparently, only 8MB VRAM is available on i945 GPUs (though it could do
64MB):\
phcoder: No. Hardware default is 8 MiB. When I wanted to make it
configurable, I saw that docs mention only one other alternative: 1MiB.
Later isn't event enough for 1024x768 at 24bpp without any acceleration
or double buffering. It's possible that there are undocumented values.
Which options do you have in vendor BIOS? How to find out how much vram
you have:\
phcoder: TOM - BSM\
phcoder: check what vendor BIOS offers as options\
vimuser: I thought it could do 64MB usually\
phcoder: not accorging to doc.\
phcoder: see mobile-945-express-chipset-datasheet page 93\
phcoder: see also
src/northbridge/intel/i945/{early\_init,northbridge,gma}.c\
vimuser: "011 = DVMT (UMA) mode, 8 MB of memory pre-allocated for\
vimuser: frame buffer."\
vimuser: "Others - reserved"\
phcoder: the easiest way is a loop at this position which tries
different values and reads (and prints) BSM with them\
stefanct: vimuser: they suggest that you change the value and look how
BSM reacts to that\
stefanct: as they pointed out earlier vram size = TOM - BSM\
stefanct: different values of GMS\
stefanct: phcoder: hm\... this could be a hint. look at the text
description of TOLUD at page 103\
stefanct: it mentions 64 MB in the text about BSM as well\
stefanct: table 18\...\
phcoder: stefanct: I have a guess which value make is 64 but I will not
tell to avoid skewing test results\
stefanct: phcoder: sure\... i assumed you were not sure if it supports
it at all. testing it properly is of course a good idea :)\
stefanct: test the various possible (but reserved) values of GMS and see
what the resulting VRAM size is\
vimuser: so, TOM - BSM

LCD panels on i945 - fix incompatible panels {#lcd_i945_incompatibility}
============================================

Fix T60 issues (see incompatible panels listed at
[../hcl/\#supported\_t60\_list](../hcl/#supported_t60_list)).

Run that tool (resources/utilities/i945gpu/intel-regs.py) as root on
systems with the offending panels in:

-   Coreboot (or libreboot, whatever) with VBIOS (disable native
    graphics also)
-   (Factory BIOS also?)

This shows values in devicetree.cb and src/northbridge/intel/i945/gma.c,
the idea is that you run it on factory bios or vbios and that it will
(might) show different values: then you try those in the native graphics
(in libreboot).

Other values/registers might also need to be added to the script for
these tests.

check if intel\_bios\_reader from intel-gpu-tools reports the same value
(BIOS has a hardcoded value) for PWM modulation frequency. This file can
read the VBIOS (64K dump).

Check other tools in intel-gpu-tools aswell, compare outputs. Possibly
add more information to intel-regs.py output (submit changes to mtjm).
Do oprom trace / replay
(<http://www.coreboot.org/User:GNUtoo#How_to_get_rid_of_the_vbios_of_the_x60_.5BNew_Version.5D>)

Study how EDID works and how gma.c handles it.

Original getregs.py script can be found at
<http://hg.mtjm.eu/scripts/file/tip/intel-regs.py> written by Michał
Masłowski.

About fixing remaining LCD panels on 5345:\
'polarity' is mentioned in coreboot log (cbmem -c). compare output
(with working and non-working panel). (and see the other notes in
docs/future/)

There was a theory that working panels are 6bpc (bits per colour) and
nonworking are 8bpc, but all EDIDs that we have here are for 6bpc
panels.

We should compare coreboot logs (from `cbmem -c` from systems with
working and nonworking panels.

How to dump EDID:

    # apt-get install i2c-tools
    # modprobe i2c-dev
Find out the correct ID to use:
    # i2cdetect -l\
Example:

    # i2cdump -y 2 0x50

Working panel: EDID dump from LG-Philips LP150E05-A2K1:

    No size specified (using byte-data access)
         0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
    00: XX ff ff ff ff ff ff 00 30 ae 43 40 00 00 00 00    X.......0?C@....
    10: 00 0f 01 03 80 1e 17 78 ea af 00 99 58 53 8c 2a    .??????x??.?XS?*
    20: 25 50 54 21 08 00 81 80 01 01 01 01 01 01 01 01    %PT!?.??????????
    30: 01 01 01 01 01 01 30 2a 78 20 51 1a 10 40 30 70    ??????0*x Q??@0p
    40: 13 00 31 e4 10 00 00 18 28 23 78 20 51 1a 10 40    ?.1??..?(#x Q??@
    50: 30 70 13 00 31 e4 10 00 00 18 00 00 00 0f 00 90    0p?.1??..?...?.?
    60: 43 32 90 43 28 14 06 00 32 0c 00 00 00 00 00 fe    C2?C(??.2?.....?
    70: 00 4c 50 31 35 30 45 30 35 2d 41 32 0a 20 00 9b    .LP150E05-A2? .?
    80: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    90: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    a0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    b0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    c0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    d0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    e0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    f0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................

Working panel: EDID dump from Boe-Hydis HT14P12-100 FRU: 13N7061
(Inverter FRU: 41W1010)

         0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
    00: 00 ff ff ff ff ff ff 00 30 ae 22 40 00 00 00 00    ........0?"@....
    10: 00 0f 01 03 80 1c 15 78 ea af 40 95 56 4a 8f 25    .??????x??@?VJ?%
    20: 20 50 54 21 08 00 81 80 01 01 01 01 01 01 01 01     PT!?.??????????
    30: 01 01 01 01 01 01 30 2a 78 20 51 1a 10 40 30 70    ??????0*x Q??@0p
    40: 13 00 1d d6 10 00 00 19 25 23 78 20 51 1a 10 40    ?.???..?%#x Q??@
    50: 30 70 13 00 1d d6 10 00 00 19 00 00 00 0f 00 90    0p?.???..?...?.?
    60: 43 32 90 43 28 0f 01 00 09 e5 00 00 00 00 00 fe    C2?C(??.??.....?
    70: 00 48 54 31 34 50 31 32 2d 31 30 30 0a 20 00 3f    .HT14P12-100? .?
    80: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    90: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    a0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    b0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    c0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    d0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    e0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    f0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................

LTN141XA-L01: revert to libreboot git commit c943281 to see this

**Non-**working panel: EDID dump from Samsung LTN150XG-L08:

         0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
    00: 00 ff ff ff ff ff ff 00 30 ae 40 40 00 00 00 00    ........0?@@....
    10: 00 0f 01 03 80 1f 17 78 ea 2d 05 97 57 51 8a 27    .??????x?-??WQ?'
    20: 23 50 54 21 08 00 01 01 01 01 01 01 01 01 01 01    #PT!?.??????????
    30: 01 01 01 01 01 01 64 19 00 40 41 00 26 30 18 88    ??????d?.@A.&0??
    40: 36 00 30 e4 10 00 00 19 28 15 00 40 41 00 26 30    6.0??..?(?.@A.&0
    50: 18 88 36 00 30 e4 10 00 00 19 00 00 00 0f 00 61    ??6.0??..?...?.a
    60: 43 32 61 43 28 14 01 00 4c a3 58 47 00 00 00 fe    C2aC(??.L?XG...?
    70: 00 4c 54 4e 31 35 30 58 47 2d 4c 30 38 0a 00 6d    .LTN150XG-L08?.m
    80: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    90: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    a0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    b0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    c0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    d0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    e0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................
    f0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff    ................

i945 gfx: X60/T60 VBT implementation (experimental: testing) {#i945_vbt}
============================================================

intel\_bios\_dumper in intel-gpu-tools seems interesting.

**Use 'drm.debug=0x06' kernel parameter when booting in grub!**

Before each test run, boot a live USB and delete the old logs in
/var/log (kernel log, xorg log, dmesg and so on).

Load (from the ROM) the runningvga.bin for each LCD panel on each
system; do not execute it, only load it! (coreboot will have to be
modified). Rename the ROM appropriately, based on the system name and
the panel name.
coreboot\_nativegfx\_5868\_plusrunningvga\_t60\_14\_LTD141ECMB.rom, for
instance. Keep a copy for later use.

You are supposed to:

-   enable native graphics in menuconfig
-   include the self-modified VGA ROM (load, but not execute) - for
    reverse engineering the correct VBT tables.

With each boot, make notes about what you see and get logs using the
[standard test](#standard_test). You will need the files from
[\#intelvbttool\_results](#intelvbttool_results) for each system.

Results (\# means untested):
-   **X60/X60s:**
    -   TMD-Toshiba LTD121ECHB: \#
    -   CMO N121X5-L06: \#
    -   Samsung LTN121XJ-L07: \#
    -   BOE-Hydis HT121X01-101: \#
-   **X60T XGA:**
    -   BOE-Hydis HV121X03-100: \#
-   **X60T SXGA+:**
    -   BOE-Hydis HV121P01-100: \#
-   **T60 14" XGA:**
    -   Samsung LTN141XA-L01: \#
    -   CMO N141XC: \#
    -   BOE-Hydis HT14X14: \#
    -   TMD-Toshiba LTD141ECMB: \#
-   **T60 14" SXGA+**
    -   TMD-Toshiba LTD141EN9B: \#
    -   Samsung LTN141P4-L02: \#
    -   Boe-Hydis HT14P12: \#
-   **T60 15" XGA**
    -   Samsung LTN150XG-L08: \#
    -   LG-Philips LP150X09: \#
    -   13N7068 (IDtech): \#
    -   13N7069 (CMO): \#
-   **T60 15" SXGA+**
    -   LG-Philips LP150E05-A2K1: \#
    -   BOE-Hydis HV150P01-100: \#
-   **T60 15" UXGA**
    -   BOE-Hydis HV150UX1-100: \#
    -   IDTech N150U3-L01: \#
    -   BOE-Hydis HV150UX1-102: \#
-   **T50 15" QXGA**
    -   IDtech IAQX10N: \#
    -   IDtech IAQX10S: \#

[Back to top of page](#pagetop)

intelvbttool test results (VGA ROM dumps) {#intelvbttool_results}
=========================================

The VBIOS on i945 (intel gpu) platforms is self-modifying; that is, its
contents change when you run it. intelvbttool takes a dump of the
currently running vbios, and parses it.

The idea is that we can extract the VBT tables using this knowledge, on
the X60, X60 Tablet and T60 (Intel GPU).

Here is an example of how VBT was implemented on the ThinkPad X230:
<http://review.coreboot.org/#/c/5396>.

You'll need to build a T60 ROM with SeaBIOS and the VGA ROM (for Intel
GPU). An X60 ROM is also needed (same configuration, using the VGA ROM
for X60).

T60 has DVI on its dock, make sure that the dock is attached when
getting this output.

Get intelvbttool here: <http://review.coreboot.org/#/c/5842>
(util/intelvbttool).

Now dump a copy of the running VGA BIOS: **\$ sudo dd if=/dev/mem bs=64k
of=runningvga.bin skip=12 count=1**\
Then do (and record the output):

    $ ./intelvbttool runningvga.bin > intelvbttool\_out

Backup both files (runningvga.bin and intelvbttool\_out), renaming them
to match the system and LCD panel used.
[../misc/\#get\_edid\_panelname](../misc/#get_edid_panelname) will show
you how to get the name (model) of the LCD panel used.

Test results (\# means untested and all had docks, unless noted).
-----------------------------------------------------------------

-   **X60/X60s:**
    -   TMD-Toshiba LTD121ECHB: \#
    -   CMO N121X5-L06: \#
    -   Samsung LTN121XJ-L07: \#
    -   BOE-Hydis HT121X01-101: \#
-   **X60T XGA (1024x768):**
    -   BOE-Hydis HV121X03-100: \#
-   **X60T SXGA+ (1400x1050):**
    -   BOE-Hydis HV121P01-100: \#
-   **T60 14" XGA (1024x768):**
    -   Samsung LTN141XA-L01: \#
    -   CMO N141XC: \#
    -   BOE-Hydis HT14X14: \#
    -   TMD-Toshiba LTD141ECMB: \#
-   **T60 14" SXGA+ (1400x1050):**
    -   TMD-Toshiba LTD141EN9B: \#
    -   Samsung LTN141P4-L02: \#
    -   Boe-Hydis HT14P12: \#
-   **T60 15" XGA (1024x768):**
    -   Samsung LTN150XG-L08: \#
    -   LG-Philips LP150X09: \#
    -   13N7068 (IDtech): \#
    -   13N7069 (CMO): \#
-   **T60 15" SXGA+ (1400x1050):**
    -   LG-Philips LP150E05-A2K1: \#
    -   BOE-Hydis HV150P01-100: \#
-   **T60 15" UXGA (1600x1200):**
    -   BOE-Hydis HV150UX1-100: \#
    -   IDTech N150U3-L01: \#
    -   BOE-Hydis HV150UX1-102: \#
-   **T60 15" QXGA (2048x1536):**
    -   IDtech IAQX10N: \#
    -   IDtech IAQX10S: \#

Fallback patches {#fallback_patches}
================

-   Todo: test gnutoo's fallback patches:
    <http://www.coreboot.org/Fallback_mechanism> see [this IRC
    log](gnutoo_fallback_patch) (warning, not cleaned up)

Other - unlisted (low priority) {#other}
===============================

-   PTE errors still exist on i945 with 6718 (which is merged).
    <http://review.coreboot.org/#/c/6718/>
    -   [dmesg](future/dumps/pte_x60_6718/dmesg)
    -   [kern.log](future/dumps/pte_x60_6718/kern.log)
    -   This doesn't seem to cause any issues for general use.
-   Implement INT 10H support in i945 native graphics. (this will be a
    lot of work. SeaBIOS has code for some geode boards, to get a
    general idea).
-   T60 15" QXGA (2048x1536): fixed issue where GRUB wouldn't show any
    graphics. (Not many people have this panel)
    -   It might be the same issue I ran into with Paul on the X60 (not
        enough memory being allocated) since the same code works on T60
        SXGA+ and UXGA, where GRUB actually display graphics.

Copyright © 2014, 2015 Leah Rowe <info@minifree.org>\
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

