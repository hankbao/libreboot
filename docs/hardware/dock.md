<div class="section">

Notes about DMA and the docking station (X60/T60)
=================================================

</div>

<div class="section">


    Use case:
    ---------
    Usually when people do full disk encryption, it's not really full disk,
    instead they still have a /boot in clear.

    So an evil maid attack can still be done, in two passes:
    1) Clone the hdd, Infect the initramfs or the kernel.
    2) Wait for the user to enter its password, recover the password,
    luksOpen the hdd image.

    I wanted a real full-disk encryption so I've put grub in flash and I
    have the following: The HDD has a LUKS rootfs(containing /boot) on an
    lvm partition, so no partition is in clear.

    So when the computer boots it executes coreboot, then grub as a payload.
    Grub then opens the LUKS partition and loads the kernel and initramfs
    from there.

    To prevent hardware level tempering(like reflashing), I used nail
    polish with a lot of gilder, that acts like a seal. Then a high
    resolution picture of it is taken, to be able to tell the difference.

    The problem:
    ------------
    But then comes the docking port issue: Some LPC pins are exported
    there, such as the CLKRUN and LDRQ#.

    LDRQ# is "Encoded DMA/Bus Master Request": "Only needed by
    peripherals that need DMA or bus mastering. Requires an
    individual signal per peripheral. Peripherals may not share
    an LDRQ# signal."

    So now DMA access is possible trough the dock connector.
    So I want to be able to turn that off.

    If I got it right, the X60 has 2 superio, one is in the dock, and the
    other one is in the laptop, so we have:
                                ________________
     _________________         |                |
    |                 |        | Dock connector:|
    |Dock: NSC pc87982|<--LPC--->D_LPC_DREQ0    |
    |_________________|        |_______^________|
                                       |
                                       |
                                       |
                                       |
                    ___________________|____
                   |                   v    |
                   | SuperIO:        DLDRQ# |
                   | NSC pc87382     LDRQ#  |
                   |___________________^____|
                                       |
                                       |
                                       |
                                       |
                    ___________________|___
                   |                   v   |
                   | Southbridge:    LDRQ0 |
                   | ICH7                  |
                   |_______________________|


    The code:
    ---------
    Now if I look at the existing code, there is some superio drivers, like
    pc87382 in src/superio/nsc, the code is very small. 
    The only interesting part is the pnp_info pnp_dev_info struct.

    Now if I look inside src/mainboard/lenovo/x60 there is some more
    complete dock driver:

    Inside dock.c I see some dock_connect and dock_disconnect functions.

    Such functions are called during the initialisation (romstage.c) and
    from the X60 SMI handler (smihandler.c).

    Questions:
    ----------
    1) Would the following be sufficent to prevent DMA access from the
    outside:
    > int dock_connect(void)
    > {
    >          int timeout = 1000;
    > +        int val;
    > +        
    > +        if (get_option(&val, "dock") != CB_SUCCESS)
    > +                val = 1;
    > +        if (val == 0)
    > +                return 0;
    >          [...]
    > }
    >
    > void dock_disconnect(void) {
    > +        if (dock_present())
    > +                return;
    >          [...]
    > }
    2) Would an nvram option be ok for that? Should a Kconfig option be
    added too?

    > config DOCK_AUTODETECT
    >         bool "Autodetect"
    >         help
    >           The dock is autodetected. If unsure select this option.
    >
    > config DOCK_DISABLED
    >         bool "Disabled"
    >         help
    >           The dock is always disabled.
    >
    > config DOCK_NVRAM_ENABLE
    >         bool "Nvram"
    >         help
    >           The dock autodetection is tried only if it is also enabled
    > trough nvram.

</div>

<div class="section">

Copyright © 2014, 2015 Leah Rowe &lt;info@minifree.org&gt;\
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
