% KGPE-D16 external flashing instructions 

Initial flashing instructions for KGPE-D16.

This guide is for those who want libreboot on their ASUS KGPE-D16
motherboard, while they still have the proprietary ASUS BIOS present.
This guide can also be followed (adapted) if you brick you board, to
know how to recover.

**Memory initialization is still problematic, for some modules. We
recommend avoiding Kingston modules.**

For more general information about this board, refer to
[../hcl/kgpe-d16.html](../hcl/kgpe-d16.html).

TODO: show photos here, and other info.

-   [KGPE-D16 boards (and full systems) with libreboot
    preinstalled](#preinstall)

-   [External programmer](#programmer)

KGPE-D16 boards (and full systems) with libreboot preinstalled {#preinstall}
==============================================================

If you don't want to install libreboot yourself, companies exist that
sell these boards with libreboot pre-installed, along with a free
GNU+Linux distribution.

Check the [suppliers](../../suppliers) page for more information.

External programmer {#programmer}
===================

Refer to [bbb\_setup.html](bbb_setup.html) for a guide on how to set up
an external SPI programmer.

The flash chip is in a PDIP 8 socket (SPI flash chip) on the
motherboard, which you take out and then re-flash with libreboot, using
the programmer. **DO NOT** remove the chip with your hands. Use a chip
extractor tool.

Copyright © 2015 Leah Rowe <info@minifree.org>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
