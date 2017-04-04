---
title: Contributors
...

If we forgot to mention you here, let us know and we'll add you. (or if
you don't want to be mentioned, let us know and we'll remove your
entry)

Leah Rowe
---------

Founder of the Libreboot project, and lead developer managing the project.

Leah is also responsible for [vimuser.org](https://vimuser.org/)
(personal website) and [transit.org.uk](https://transit.org.uk/)
(support group for transgender people).

Alyssa Rosenzweig
-----------------

Switched the website to use markdown in lieu of handwritten HTML and custom
PHP. Additionally assisted with [booting
Linux](https://blog.rosenzweig.io/blobless-linux-on-the-pi.md) on the Raspberry
Pi without blobs. Current sysadmin and assists with project management.

Timothy Pearson
---------------

Ported the ASUS KGPE-D16 board to coreboot for the company Raptor
Engineering of which Timothy is the CEO.
Timothy maintains this code in coreboot,
helping the project with the libreboot integration for it. This person's
contact details are on the raptor site, or you can ping **tpearson** on
the freenode IRC network.

Paul Kocialkowski
-----------------

Ported the ARM (Rockchip RK3288 SoC) based *Chromebook* laptops to
libreboot. Also one of the main [Replicant](http://www.replicant.us/)
developers. Contact Paul on the libreboot IRC channel by the alias
**paulk** or **paulk-&lt;hostname&gt;** (hostname is variable).

Damien Zammit
-------------

Maintains the Gigabyte GA-G41M-ES2L coreboot port, which is integrated
in libreboot. Also works on other hardware for the benefit of the
libreboot project. Contact **damo22** on the freenode IRC network. This
persons website is [zammit.org](http://www.zammit.org/).

Patrick "P. J." McDermott
---------------------------

Patrick also did a lot of research and wrote the libreboot FAQ section
relating to the [Intel Management Engine](../faq.md#intelme), in addition
to making several improvements to the build system in libreboot.

Steve Shenton
-------------

Ported the ThinkPad X200 to libreboot, by figuring out how to deactive
the Intel Management Engine on that laptop, and remove the firmware
while still making the system boot correctly. The ThinkPad T400, T500,
R400 and R500 were also ported to libreboot, based on this work.

Denis Carikli
-------------

Based on the work done by Peter Stuge, Vladimir Serbineko and others in
the coreboot project, got native graphics initialization to work on the
ThinkPad X60, allowing it to be supported in libreboot. Denis gave
a lot of advice and helped found the libreboot project. Contact
**GNUtoo-irssi** in the \#libreboot IRC channel. Denis is also one of
the founders of the [Replicant](http://replicant.us/) project.

Vladimir Serbinenko
-------------------

Ported many of the thinkpads supported in libreboot, to coreboot, and
made many fixes in coreboot which benefited the libreboot project.
Contact **phcoder** in the \#coreboot IRC channel on freenode.

Paul Menzel
-----------

Investigated and fixed a bug in coreboot on the ThinkPad X60/T60 exposed
by Linux kernel 3.12 and up, which caused 3D acceleration to stop
working and video generally to become unstable. Paul worked with Libreboot on
this, sending patches to test periodically until the bug was fixed
in coreboot, and then helped her integrate the fix in libreboot. Contact
**PaulePanter** in the \#libreboot IRC channel.

Jeroen Quint
------------

Contributed several fixes to the libreboot documentation, relating to
installing Parabola with full disk encryption on libreboot systems.
Contact **Jezza** in the \#libreboot IRC channel.

Arthur Heymans
--------------

Merged a patch from coreboot into libreboot, enabling C3 and C4 power
states to work correctly on GM45 laptops. This was a long-standing issue
before Athur's contribution. Arthur also fixed VRAM size on i945 on
GM45 systems, allowing maximum VRAM allocation for the onboard GPUs on
these systems, another longstanding issue in libreboot. Contact **apvh**
in the \#libreboot IRC channel.

Peter Stuge
-----------

Helped write the section about DMA on
[../faq/\#firmware-hddssd](../faq/#firmware-hddssd), and provided
general advice in early days of the project. Contact **CareBear\\** in
the \#coreboot IRC channel on freenode.

Klemens Nanni
-------------

Made many fixes and improvements to the GRUB configuration used in
libreboot, and several tweaks to the build system. Contact **kl3** in
the \#libreboot IRC channel.

Marcus Moeller
--------------

Made the libreboot logo.

Swift Geek
----------

Contributed a patch for ich9gen to generate 16MiB descriptors. Contact
**swiftgeek** in the IRC channel.
