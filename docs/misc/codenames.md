---
title: Product Codenames
...

Introduction
============

This document lists product codenames for some hardware.
Please note that just because a certain device is listed here does NOT mean
that it is supported in Libreboot. For supported devices refer to the
installation documentation.

### A note on GPUs

Some laptops come with and without a discrete GPU (dGPU). Whether the
motherboard includes one or not can be determined by (in descending order
of reliability):

-   often thorough disassembly and searching for the actual chip
-   looking at white PCB markings near RAM slots
    and comparing with some known IDs listed below.
-   sometimes by looking at heatsink grills: on
    discrete GPU laptops these will look orange and on intergrated ones
    they will appear silver.

List of models and codenames
============================

### Codenames

-   Asus Chromebook C201PA: speedy\_rk3288, veyron-speedy

-   Thinkpad X200: Mocha-1
-   Thinkpad X200s (slim): Pecan-1
-   Thinkpad X200 Tablet: Caramel-1

-   Thinkpad T400: Malibu-3
    -   with discrete GPU (dGPU), at board(?) revision 7: "MLB3D-7
    -   with only integrated GPU (iGPU), at board(?) revision 7: "MLB3I-7"

-   Thinkpad T400s (slim): Shinai-MV
-   Thinkpad R500: Waikiki-3

-   Thinkpad T500/W500: Coronado-5
    - with dGPU (radeon): "COR5D-9"
    - with only iGPU: "COR5I-9"

-   T6x (whole family): Davinci
-   T60
    -   with dGPU (radeon): Magi
    -   with iGPU: Lisa

-   With ThinkPads on Intel platforms newer than Montevina (Except T410),
    the codenames become more consistent. All boards have the following
    appended based on the type of graphics they have:
    -   with dGPU: SWG (SWitchable Graphics)
    -   with only iGPU: UMA (Unified Memory Access)

    These are the known model codenames:
    -   Thinkpad T410: NOZOMI-1 # EXT/INT
    -   Thinkpad T410s: SHINAI-2 # SWG/UMA
    -   Thinkpad T420: NOZOMI-3 # SWG/UMA
    -   Thinkpad T420s: SHINAI-3 # SWG/UMA
    -   Thinkpad T430: NOZOMI-4 # SWG/UMA
    -   Thinkpad T430s: SHINAI-4 # SWG/UMA
    -   Thinkpad T520: KENDO-1
    -   Thinkpad W520: KENDO-1 WS
    -   Thinkpad T520: KENDO-3
    -   Thinkpad W520: KENDO-3 WS
    -   Thinkpad T530: KENDO-4
    -   Thinkpad W530: KENDO-4 WS


### Miscellaneous
-   Calistoga: 945GM chipset family name
-   Napa: calistoga based platform
-   Cantiga: GM45/GS45/PM45 chipset family name.
    This is the chipset used in T400,X200 and similar.
-   Montevina: cantiga based platform.
-   PMH: the Power Management Hub is a gate array for managing the power
    up/down sequence. It is additionally tasked with extending EC's I/O.
    Its? last version was called "Thinker-1".
-   Kozak, Tsurumai, Rinkan: These are power management ICs for Notebook PCs.
    Tsurumai is marked as "TB62501F" and its datasheet has a description of
    its operation.

See also
========
-   Many more Intel codenames can be found at
    [Wikipedia](https://en.wikipedia.org/wiki/List_of_Intel_codenames).
-   For Thinkpads see [Documentation/thinkpad/codenames.csv @ Coreboot](https://review.coreboot.org/cgit/coreboot.git/tree/Documentation/thinkpad/codenames.csv)

Copyright Â© 2018 Fedja Beader <fedja@protonmail.ch>

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License Version 1.3 or any later
version published by the Free Software Foundation
with no Invariant Sections, no Front Cover Texts, and no Back Cover Texts.
A copy of this license is found in [../fdl-1.3.md](../fdl-1.3.md)
