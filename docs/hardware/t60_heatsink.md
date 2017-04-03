% Changing heatsink (or CPU) on the ThinkPad T60

Using this guide you can also change/upgrade the CPU.

[Back to previous index](./)



Hardware requirements {#hardware_requirements}
=====================

-   rubbing alcohol or isopropyl alcohol, and thermal compound for
    changing CPU heatsink (procedure involves removing heatsink)

-   thermal compound/paste (Arctic MX-4 is good. Others are also good.)



Software requirements {#software_requirements}
=====================

-   xsensors
-   stress



Disassembly {#recovery}
===========

Remove those screws and remove the HDD:\
![](../images/t60_dev/0001.JPG) ![](../images/t60_dev/0002.JPG)

Lift off the palm rest:\
![](../images/t60_dev/0003.JPG)

Lift up the keyboard, pull it back a bit, flip it over like that and
then disconnect it from the board:\
![](../images/t60_dev/0004.JPG) ![](../images/t60_dev/0005.JPG)
![](../images/t60_dev/0006.JPG)

Gently wedge both sides loose:\
![](../images/t60_dev/0007.JPG) ![](../images/t60_dev/0008.JPG)

Remove that cable from the position:\
![](../images/t60_dev/0009.JPG) ![](../images/t60_dev/0010.JPG)

Remove the bezel (sorry forgot to take pics).

On the CPU (and there is another chip south-east to it, sorry forgot to
take pic) clean off the old thermal paste (with the alcohol) and apply
new (Artic Silver 5 is good, others are good too) you should also clean
the heatsink the same way\
![](../images/t60_dev/0051.JPG)

This is also an opportunity to change the CPU to another one. For
example if you had a Core Duo T2400, you can upgrade it to a better
processor (higher speed, 64-bit support). A Core 2 Duo T7600 was
installed here.

Attach the heatsink and install the screws (also, make sure to install
the AC jack as highlighted):\
![](../images/t60_dev/0052.JPG)

Reinstall that upper bezel:\
![](../images/t60_dev/0053.JPG)

Do that:\
![](../images/t60_dev/0054.JPG) ![](../images/t60_dev/0055.JPG)

Attach keyboard:\
![](../images/t60_dev/0056.JPG)

Place keyboard and (sorry, forgot to take pics) reinstall the palmrest
and insert screws on the underside:\
![](../images/t60_dev/0058.JPG)

It lives!\
![](../images/t60_dev/0071.JPG) ![](../images/t60_dev/0072.JPG)
![](../images/t60_dev/0073.JPG)

Always stress test ('stress -c 2' and xsensors. below 90C is ok) when
replacing cpu paste/heatsink:\
![](../images/t60_dev/0074.JPG)



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

