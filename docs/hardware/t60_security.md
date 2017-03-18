% Security on the ThinkPad T60

Hardware modifications to enhance security on the ThinkPad T60. This
tutorial is **incomplete** at the time of writing.

[Back to previous index](./)

Table of Contents
=================

-   [Hardware Requirements](#hardware_requirements)
-   [Software Requirements](#software_requirements)
-   [The procedure](#procedure)

Hardware requirements {#hardware_requirements}
=====================

-   A T60
-   screwdriver
-   Rubbing or isopropyl alcohol, and thermal compound.
-   (in a later version of this tutorial: soldering iron and scalpel)

Software requirements {#software_requirements}
=====================

-   none (at least in the scope of the article as-is)
-   You probably want to encrypt your GNU+Linux install using LUKS

Rationale
=========

Most people think of security on the software side: the hardware is
important aswell.

This tutorial deals with reducing the number of devices that have direct
memory access that could communicate with inputs/outputs that could be
used to remotely command the system (or leak data). All of this is
purely theoretical for the time being.

Disassembly {#procedure}
===========

Remove those screws and remove the HDD:

![](../images/t60_dev/0001.JPG) ![](../images/t60_dev/0002.JPG)

Lift off the palm rest:

![](../images/t60_dev/0003.JPG)

Lift up the keyboard, pull it back a bit, flip it over like that and
then disconnect it from the board:

![](../images/t60_dev/0004.JPG) ![](../images/t60_dev/0005.JPG)
![](../images/t60_dev/0006.JPG)

Gently wedge both sides loose:

![](../images/t60_dev/0007.JPG) ![](../images/t60_dev/0008.JPG)

Remove that cable from the position:

![](../images/t60_dev/0009.JPG) ![](../images/t60_dev/0010.JPG)

Now remove that bezel. Remove wifi, nvram battery and speaker connector
(also remove 56k modem, on the left of wifi):

![](../images/t60_dev/0011.JPG)

Reason: has direct (and very fast) memory access, and could
(theoretically) leak data over a side-channel.

**Wifi:** The ath5k/ath9k cards might not have firmware at all. They
might safe but could have access to the computer's RAM trough DMA. If
people have an intel card(most T60 laptops come with Intel wifi by
default, until you change it),then that card runs a non-free firwamre
and has access to the computer's RAM trough DMA! So the risk-level is
very high.

Remove those screws:

![](../images/t60_dev/0012.JPG)

Disconnect the power jack:

![](../images/t60_dev/0013.JPG)

Remove nvram battery (we will put it back later):

![](../images/t60_dev/0014.JPG)

Disconnect cable (for 56k modem) and disconnect the other cable:

![](../images/t60_dev/0015.JPG) ![](../images/t60_dev/0016.JPG)

Disconnect speaker cable:

![](../images/t60_dev/0017.JPG)

Disconnect the other end of the 56k modem cable:

![](../images/t60_dev/0018.JPG)

Make sure you removed it:

![](../images/t60_dev/0019.JPG)

Unscrew those:

![](../images/t60_dev/0020.JPG)

Make sure you removed those:

![](../images/t60_dev/0021.JPG)

Disconnect LCD cable from board:

![](../images/t60_dev/0022.JPG)

Remove those screws then remove the LCD assembly:

![](../images/t60_dev/0023.JPG) ![](../images/t60_dev/0024.JPG)
![](../images/t60_dev/0025.JPG)

Once again, make sure you removed those:

![](../images/t60_dev/0026.JPG)

Remove the shielding containing the motherboard, then flip it over.
Remove these screws, placing them on a steady surface in the same layout
as they were in before you removed them. Also, you should mark each
screw hole after removing the screw (a permanent marker pen will do),
this is so that you have a point of reference when re-assembling the
system:

![](../images/t60_dev/0027.JPG) ![](../images/t60_dev/0028.JPG)
![](../images/t60_dev/0029.JPG) ![](../images/t60_dev/0031.JPG)
![](../images/t60_dev/0032.JPG) ![](../images/t60_dev/0033.JPG)

Remove microphone (soldering iron not needed. Just wedge it out
gently):

![](../images/t60_dev/0039.JPG)

    Rationale:
Another reason to remove the microphone: If your computer
gets[\[1\]](#ref1) compromised, it can record what you say, and use it
to receive data from nearby devices if they're compromised too. Also,
we do not know what the built-in microcode (in the CPU) is doing; it
could theoretically be programmed to accept remote commands from some
speaker somewhere (remote security hole). **In other words, the system
could already be compromised from the factory.**

Remove infrared:

![](../images/t60_dev/0040.JPG) ![](../images/t60_dev/0042.JPG)

Remove cardbus (it's in a socket, no need to disable. Just remove the
port itself):

![](../images/t60_dev/0041.JPG)

    Rationale:
It has direct memory access and can be used to extract sensitive details
(such as LUKS keys). See 'GoodBIOS' video linked at the end (speaker
is Peter Stuge, a coreboot hacker). The video covers X60 but the same
topics apply to T60.

Before re-installing the upper chassis, remove the speaker:

![](../images/t60_dev/0043.JPG) ![](../images/t60_dev/0044.JPG)

Reason: combined with the microphone issue, this could be used to leak
data.

If your computer gets[\[1\]](#ref1) compromised, it can be used to
transmit data to nearby compromised devices. It's unknown if it can be
turned into a microphone[\[2\]](#ref2).

Replacement: headphones/speakers (line-out) or external DAC (USB).

Remove the wwan:

![](../images/t60_dev/0045.JPG)

**Wwan (3g modem):** They run proprietary software! It's like AMT but
over the GSM network which is probably even worse.

Replacement: external USB wifi dongle. (or USB wwan/3g dongle; note,
this has all the same privacy issues as mobile phones. wwan not
recommended).

This is where the simcard connector is soldered. See notes above about
wwan. Remove simcard by removing battery and then it's accessible (so,
remember to do this when you re-assemble. or you could do it now?)

![](../images/t60_dev/0046.JPG)

Put those screws back:

![](../images/t60_dev/0047.JPG)

Put it back into lower chassis:

![](../images/t60_dev/0048.JPG)

Attach LCD and insert screws (also, attach the lcd cable to the board):

![](../images/t60_dev/0049.JPG)

Insert those screws:

![](../images/t60_dev/0050.JPG)

On the CPU (and there is another chip south-east to it, sorry forgot to
take pic) clean off the old thermal paste (with the alcohol) and apply
new (Artic Silver 5 is good, others are good too) you should also clean
the heatsink the same way

![](../images/t60_dev/0051.JPG)

Attach the heatsink and install the screws (also, make sure to install
the AC jack as highlighted):

![](../images/t60_dev/0052.JPG)

Reinstall that upper bezel:

![](../images/t60_dev/0053.JPG)

Do that:

![](../images/t60_dev/0054.JPG) ![](../images/t60_dev/0055.JPG)

Attach keyboard and install nvram battery:

![](../images/t60_dev/0056.JPG) ![](../images/t60_dev/0057.JPG)

Place keyboard and (sorry, forgot to take pics) reinstall the palmrest
and insert screws on the underside:

![](../images/t60_dev/0058.JPG)

Remove those covers and unscrew:

![](../images/t60_dev/0059.JPG) ![](../images/t60_dev/0060.JPG)
![](../images/t60_dev/0061.JPG)

Gently pry off the front bezel (sorry, forgot to take pics).

Remove bluetooth module:

![](../images/t60_dev/0062.JPG) ![](../images/t60_dev/0063.JPG)

Re-attach the front bezel and re-insert the screws (sorry, forgot to
take pics).

It lives!

![](../images/t60_dev/0071.JPG) ![](../images/t60_dev/0072.JPG)
![](../images/t60_dev/0073.JPG)

Always stress test ('stress -c 2' and xsensors. below 90C is ok) when
replacing cpu paste/heatsink:

![](../images/t60_dev/0074.JPG)

Not covered yet:
----------------

-   Disable flashing the ethernet firmware
-   Disable SPI flash writes (can be re-enabled by unsoldering two
    parts)
-   Disable use of xrandr/edid on external monitor (cut 2 pins on VGA)
-   Disable docking station (might be possible to do it in software, in
    coreboot upstream as a Kconfig option)

Go to
<http://media.ccc.de/browse/congress/2013/30C3_-_5529_-_en_-_saal_2_-_201312271830_-_hardening_hardware_and_choosing_a_goodbios_-_peter_stuge.html>
or directly to the video:
<http://mirror.netcologne.de/CCC/congress/2013/webm/30c3-5529-en-Hardening_hardware_and_choosing_a_goodBIOS_webm.webm>.

A lot of this tutorial is based on that video. Look towards the second
half of the video to see how to do the above.

Also not covered yet:
---------------------

-   Intrusion detection: randomized seal on screws

    Just put nail polish with lot of glider on the important screws,
    take some good pictures. Keep the pictueres and make sure of their
    integrity. Compare the nail polish with the pictures before powering
    on the laptop.
-   Tips about preventing/mitigating risk of cold boot attack.
    -   soldered RAM?
    -   wipe all RAM at boot/power-off/power-on? (patch in coreboot
        upstream?)
    -   ask gnutoo about fallback patches (counts number of boots)
-   General tips/advice and web links showing how to detect physical
    intrusions.
-   For example: <http://cs.tau.ac.il/~tromer/acoustic/> or
    <http://cyber.bgu.ac.il/content/how-leak-sensitive-data-isolated-computer-air-gap-near-mobile-phone-airhopper>.
-   <https://en.wikipedia.org/wiki/Tempest_%28codename%29>
-   https://gitorious.org/gnutoo-for-coreboot/grub-assemble/source/a61f636797777a742f65f4c9c58032aa6a9b23c3:

Extra notes
===========

EC: Cannot be removed but can be mitigated: it contains non-free
non-loadable code, but it has no access to the computer's RAM. It has
access to the on-switch of the wifi, bluetooth, modem and some other
power management features. The issue is that it has access to the
keyboard, however if the software security howto **(not yet written)**
is followed correctly, it won't be able to leak data to a local
attacker. It has no network access but it may still be able to leak data
remotely, but that requires someone to be nearby to recover the data
with the help of an SDR and some directional antennas[\[3\]](#ref3).

[Intel 82573 Ethernet
controller](http://www.coreboot.org/Intel_82573_Ethernet_controller) on
the X60 seems safe, according to Denis.

Risk level
----------

-   Modem (3g/wwan): highest
-   Intel wifi: Near highest
-   Atheros PCI wifi: unknown, but lower than intel wifi.
-   Microphone: only problematic if the computer gets compromised.
-   Speakers: only problematic if the computer gets compromised.
-   EC: can be mitigated if following the guide on software security.

Further reading material (software security)
============================================

-   [Installing Debian or Devuan GNU+Linux with full disk encryption
    (including /boot)](../gnulinux/encrypted_debian.html)
-   [Installing Parabola GNU+Linux with full disk encryption (including
    /boot)](../gnulinux/encrypted_parabola.html)
-   [Notes about DMA access and the docking station](dock.html)

References
==========

\[1\] physical access {#ref1}
---------------------

Explain that black hats, TAO, and so on might use a 0day to get in, and
explain that in this case it mitigates what the attacker can do. Also
the TAO do some evaluation before launching an attack: they take the
probability of beeing caught into account, along with the kind of
target. A 0day costs a lot of money, I heard that it was from 100000$
to 400000$, some other websites had prices 10 times lower but that but
it was probably a typo. So if people increase their security it makes it
more risky and more costly to attack people.

\[2\] microphone {#ref2}
----------------

It's possible to turn headphones into a microphone, you could try
yourself, however they don't record loud at all. Also intel cards have
the capability to change a connector's function, for instance the
microphone jack can now become a headphone plug, that's called
retasking. There is some support for it in GNU+Linux but it's not very
well known.

\[3\] Video (CCC) {#ref3}
-----------------

30c3-5356-en-Firmware\_Fat\_Camp\_webm.webm from the 30th CCC. While
their demo is experimental(their hardware also got damaged during the
transport), the spies probably already have that since a long time.
<http://berlin.ftp.media.ccc.de/congress/2013/webm/30c3-5356-en-Firmware_Fat_Camp_webm.webm>

Copyright © 2014, 2015 Leah Rowe <info@minifree.org>

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

