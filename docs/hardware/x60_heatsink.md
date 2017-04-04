% Changing the fan/heatsink on the ThinkPad X60

This guide will teach you how to replace the fan and heatsink on your
ThinkPad X60.

Table of Contents
=================

-   [Hardware Requirements](#hardware_requirements)
-   [Software Requirements](#software_requirements)
-   [Disassembly](#procedure)

Hardware requirements {#hardware_requirements}
=====================

-   isopropyl alcohol (sometimes called rubbing alcohol)
-   your new fan and/or heatsink
-   CPU thermal compound (some say Arctic MX-4 is good, others are also
    'ok')

-   Something to spread the paste with

Software requirements (for CPU stress testing) {#software_requirements}
==============================================

-   xsensors utility
-   stress utility

Disassembly {#procedure}
===========

Remove those screws:\
![](../images/x60_heatsink/0000.jpg)

Push the keyboard forward (carefully):\
![](../images/x60_heatsink/0001.jpg)

Lift the keyboard up and disconnect it from the board:\
![](../images/x60_heatsink/0002.jpg)

Grab the right-hand side of the chassis and force it off (gently) and
pry up the rest of the chassis:\
![](../images/x60_heatsink/0003.jpg)

You should now have this:\
![](../images/x60_heatsink/0004.jpg)

Disconnect the wifi antenna cables, the modem cable and the speaker:\
![](../images/x60_heatsink/0005.jpg)

Unroute the cables along their path, carefully lifting the tape that
holds them in place. Then, disconnect the modem cable (other end) and
power connection and unroute all the cables so that they dangle by the
monitor hinge on the right-hand side:\
![](../images/x60_heatsink/0006.jpg)

Disconnect the monitor from the motherboard, and unroute the grey
antenna cable, carefully lifting the tape that holds it into place:\
![](../images/x60_heatsink/0008.jpg)

Carefully lift the remaining tape and unroute the left antenna cable so
that it is loose:\
![](../images/x60_heatsink/0009.jpg)

Remove those screws:\
![](../images/x60_heatsink/0011.jpg)

Remove those screws:\
![](../images/x60_heatsink/0012.jpg)

Carefully remove the plate, like so:\
![](../images/x60_heatsink/0013.jpg)

Remove the SATA connector:\
![](../images/x60_heatsink/0014.jpg)

Now remove the motherboard (gently) and cast the lcd/chassis aside:\
![](../images/x60_heatsink/0015.jpg)

Look at that black tape above the heatsink, remove it:\
![](../images/x60_heatsink/0016.jpg)

Now you have removed it:\
![](../images/x60_heatsink/0017.jpg)

Disconnect the fan and remove all the screws, heatsink will easily come
off:\
![](../images/x60_heatsink/0018.jpg)

Remove the old paste with a cloth (from the CPU and heatsink) and then
clean both of them with the alcohol (to remove remaining residue of the
paste). Apply a pea-sized amount of paste to the both chipsets that the
heatsink covered and spread it evenly (uniformally). Finally reinstall
the heatsink, reversing previous steps.

**stress -c 2** command can be used to push the CPU to 100%, and
**xsensors** (or **watch sensors** command) can be used to monitor heat.
Below 90C is ok.

Copyright © 2014, 2015 Leah Rowe <info@minifree.org>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
