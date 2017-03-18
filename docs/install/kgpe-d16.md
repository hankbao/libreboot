<div class="section">

KGPE-D16 external flashing instructions {#pagetop}
=======================================

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

[Back to main index](./)

</div>

<div class="section">

KGPE-D16 boards (and full systems) with libreboot preinstalled {#preinstall}
==============================================================

If you don\'t want to install libreboot yourself, companies exist that
sell these boards with libreboot pre-installed, along with a free
GNU+Linux distribution.

Check the [suppliers](../../suppliers) page for more information.

</div>

<div class="section">

External programmer {#programmer}
===================

Refer to [bbb\_setup.html](bbb_setup.html) for a guide on how to set up
an external SPI programmer.

The flash chip is in a PDIP 8 socket (SPI flash chip) on the
motherboard, which you take out and then re-flash with libreboot, using
the programmer. **DO NOT** remove the chip with your hands. Use a chip
extractor tool.

</div>

<div class="section">

Copyright © 2015 Leah Rowe &lt;info@minifree.org&gt;\
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

</div>
