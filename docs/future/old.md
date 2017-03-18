
Development notes (old/obsolete notes) 
======================================

For current notes, see [](./).

These are old (obsolete) notes that mare kept because they might become
useful again in the future.



Table of contents
=================

-   [X60 native graphics initialization (backlight
    controls)](#x60_native_notes)
-   [T60 native graphics initialization (backlight
    controls)](#t60_native_notes)
-   [i945: 3D fix (based on 5927) for kernel 3.12+ on
    5320](#5320_kernel312fix)
-   [i945/x60: coreboot 5927 testing (3D fix for kernel 3.12+ on replay
    code)](#x60_cb5927_testing)



X60 native graphics initialization (with backlight controls) {#x60_native_notes}
============================================================

**This is now obsolete. A better way was found (included in libreboot):
<http://review.coreboot.org/#/c/6731/>**

***Also check [\#5320\_kernel312fix](#5320_kernel312fix) (to fix 3D on
kernel 3.12/higher)***

**The fix below was done on 5320/6 (from review.coreboot.org) but should
work just fine on later versions of 5320.**

Native gpu init + backlight controls! (Fn keys). Also confirmed on X60
Tablet (1024x768) and X60 Tablet (1400x1050)

**Add backlight controls:** in *src/mainboard/lenovo/x60/devicetree.cb*,
change **gpu\_backlight** to **0x879F879E**

That's all! **This has also been backported into libreboot 5th release
(line 1233 in src/mainboard/lenovo/x60/i915io.c)**. GNUtoo (Denis
Carikli) told me about the register **BLC\_PWM\_CTL** and that you could
set it to control backlight. I read that address using devmem2 while
running the VBIOS:\
**\# devmem2 0xe4361254 w**

The change is also included in libreboot 6.

When doing this, it gave back that value. The same trick was used to get
backlight controls for T60 (see
[\#t60\_native\_notes](#t60_native_notes)).

Further notes
-------------

Reading **0xe4361254** (address) in Lenovo BIOS always yields FFFFFFFF,
even when writing to it (and writing to it doesn't affect brightness
controls). 'mtjm' on IRC found that the buttons (Fn keys) control
/sys/class/backlight/acpi\_video0 which has no affect on 61254
(BLC\_PWM\_CTL). This person says intel\_backlight has different values
and uses the register. devmem2 works, needs checking **lspci -vv** for
where the memory is mapped, which is different than on coreboot; mtjm
found that it was 0xec061254 on his system (X60 Tablet), and the
register value is different too. **This is relevant, because we still
don't know how backlight controls are actually handled. We got it
working by accident. We need to know more.**.

Intel-gpu-tools may prove useful for further debugging:
<http://cgit.freedesktop.org/xorg/app/intel-gpu-tools/>

mtjm says 0xe4300000 is an MMIO region of the gpu (lspci -vv shows it),
0x61254 (BLC\_PWM\_CTL) is a documented register. Searching the kernel
driver for backlight shows that in intel\_panel.c this register is used
(there is an XXX comment about finding the right value, where recent
kernels get it from.

What we want to do is calculate a good value, instead of setting it in
devicetree.cb. mtjm says about backlight physics: it has a light source
, uses pulse width modulation (PWM) to turn it on/off, dimming is done
by spending less time on. **Note: this may not be correct; this person
says that their understanding is based on how the Lenote yeeloong
works**.

mtjm goes on to say, that the register specifies the frequency used for
PWM in its depending on the GPU core frequency, so it might be possible
to calculate it without hardcoded laptop-specific values. Therefore, I
am supposed to find out the 'display core frequency' (mtjm says there
might be a register for it; also, it might be in 5320 or the replay
code) and the PWM modulation frequency.
https://en.wikipedia.org/wiki/Backlight\#Flicker\_due\_to\_backlight\_dimming

phcoder (Vladimir Serbinenko) who is author of 5320
(review.coreboot.org) talks about 'duty cycle limit' and 'flickering
frequency'.

[Back to top of page](#pagetop)



T60 native graphics initialization (with backlight controls) {#t60_native_notes}
============================================================

**This is now obsolete. A better way was found (included in libreboot):
<http://review.coreboot.org/#/c/6731/>**

***Also check [\#5320\_kernel312fix](#5320_kernel312fix) (to fix 3D on
kernel 3.12/higher)***

**The fix below was done on an earlier version of 5345 changeset
(review.coreboot.org), but should work on the current version. it is
included in libreboot 6**

**Add backlight controls:** in *src/mainboard/lenovo/t60/devicetree.cb*,
change **gpu\_backlight** to **0x58BF58BE**

Hold on! Check
[../misc/\#get\_edid\_panelname](../misc/#get_edid_panelname) to know
what LCD panel you have. This is important for the next step!

Supported panels
----------------

[../hcl/\#supported\_t60\_list](../hcl/#supported_t60_list).

See [\#lcd\_i945\_incompatibility](./#lcd_i945_incompatibility).

[Back to top of page](#pagetop)



i945: 3D fix (based on 5927) for kernel 3.12+ on 5320 {#5320_kernel312fix}
=====================================================

**This is now obsolete. Merged in coreboot:
<http://review.coreboot.org/#/c/5927/>**

**This needs to be rewritten (or better organized, or deleted?)**. This
is also now included in libreboot 6 (using the proper way, not the
7c0000 method which was a hack)

**This was done on 5320/6 so far. The fix below is for 5320/6 which is
now obsolete. This needs to be re-done for the latest version of 5320.
The fix below is (in practise) only for reference, therefore.**

See [\#x60\_cb5927\_testing](#x60_cb5927_testing) for the original (and
current) fix, for the replay code. Now we want to implement that on top
of <http://review.coreboot.org/#/c/5320> which is the current code for
native graphics initialization on i945.

src/northbridge/intel/i945/gma.c (using the 7c0000 hack) on 5320:
[5320\_7c0000\_gma.c](dumps/5320_7c0000_gma.c) (rename it to gma.c,
replacing the current one).

The above is a hack (as is the original). A better (more correct) method
is implemented in later versions of 5927, so that should also be adapted
for 5320. For now, you can use the above fix.

The correct way to do it is to set gtt address to (end of stolen memory
- gtt size), which is what later versions of 5927 do (successfully).

Here is some debugging output using intel\_gpu\_tools v1.2-1 (from
apt-get repositories) using tool "intel\_gtt":

-   kernel 3.14.4:
    -   with libreboot 5th release (using the 7c0000 gtt hack from
        5927/3): <http://paste.debian.net/104306>
    -   with coreboot+vgarom: <http://paste.debian.net/104309>
-   kernel 3.2.0-60 (from apt-get repositories):
    -   with coreboot (no vbios or native init):
        <http://paste.debian.net/104341>

[Back to top of page](#pagetop)



i945/X60: Coreboot 5927 testing (3D fix for kernel 3.12+ on replay code) {#x60_cb5927_testing}
========================================================================

**This is now obsolete. Merged in coreboot:
<http://review.coreboot.org/#/c/5927/>**

**The latest version as-is (5927/11) has not been tested by me yet.
Always boot with 'drm.debug=0x06' kernel parameter when testing
this.**

This is the fix for 3D on kernel 3.12 and higher on i945 (ThinkPad X60
in this case). This is for the replay code. Libreboot 5th release has a
version of this backported already (based on 5927/3 using the '7c0000'
hack).

**The replay code is obsolete (see 5320 changeset on review.coreboot.org
for better version which supports more systems/screens, and then 5345
for T60). Information here for reference since that is where the fix was
first applied.**

Read the information on <http://review.coreboot.org/#/c/5927/>.

For historical purposes, here is a collection of IRC logs that once
existed on this page, related to the issue:
[kernel312\_irc](dumps/kernel312_irc).

PGETBL\_CTL differs between VBIOS (-) and native graphics init (+).\
- PGETBL\_CTL: 0x3ffc0001\
+ PGETBL\_CTL: 0x3f800001

GTT (graphics translation table) size is PGETBL\_save, max 256 KiB. BSM
(Base of Stolen Memory) is given by the bios.

-   5927/7: [5927\_7.tar.gz](dumps/5927_7.tar.gz) (GRUB graphics are
    correct now, and 3D still works)
-   5927/6: [5927\_6.tar.gz](dumps/5927_6.tar.gz) (GRUB graphics still
    corrupt, 3D/everything still works after GRUB)
-   5927/5: [5927\_5.tar.gz](dumps/5927_5.tar.gz) (GRUB graphics
    corrupt, 3D/everything still works after GRUB)
-   5927/3: [5927\_3.tar.gz](dumps/5927_3.tar.gz) (3D still works!
    kernel 3.14.4) - the '7c0000' hack
-   5927/2: [5927\_2.tar.gz](dumps/5927_2.tar.gz) (3D works! kernel
    3.14.4) - the '7c0000' hack
-   5927/1 (didn't fix the 3D issue):
    -   [cbmem -c](dumps/5927_cbmemc)
    -   [/sys/class/drm/card0/error](dumps/5927_crashdump)
    -   [.config](dumps/5927_config)

[Back to top of page](#pagetop)



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

