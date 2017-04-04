% GA-G41M-ES2L flashing tutorial 

This guide is for those who want libreboot on their Intel GA-G41M-ES2L
motherboard while they still have the original BIOS present.

Flash chip size {#flashchips}
===============

Use this to find out:
    # flashrom -p internal -V

Flashing instructions {#clip}
=====================

Refer to [bbb\_setup.html](bbb_setup.html) for how to set up the BBB for
external flashing. **You can only externally reprogram one of the chips
at a time, and you need to disable the chip that you're not flashing,
by connecting 3v3 to /CS of that chip, so you will actually need 2 test
clips (you also need to connect GND on the chip that you're
disabling).**

Here is an image of the flash chip:\
![](../images/ga-g41m-es2l/ga-g41m-es2l.jpg)

Internal flashing is possible. Boot with the proprietary BIOS and
GNU+Linux. There are 2 flash chips (one is backup).

Flash the first chip:\
**./flashrom -p internal:dualbiosindex=0 -w libreboot.rom**

Flash the second chip:\
**./flashrom -p internal:dualbiosindex=1 -w libreboot.rom**

NOTE: you can still boot the system with just the main flash chip
connected, after desoldering the backup chip. This has been tested while
libreboot was already installed onto the main chip.

NOTE: You need the latest flashrom. Just get it on flashrom.org from
their SVN or Git repos.

NOTE: due to a bug in the hardware, the MAC address is hardcoded in
coreboot-libre. Therefore, you must set your own MAC address in your
operating system.

Copyright © 2016 Leah Rowe <info@minifree.org>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
