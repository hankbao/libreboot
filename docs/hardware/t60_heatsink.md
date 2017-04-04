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
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
