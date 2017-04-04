% Hardware compatibility list 

This sections relates to known hardware compatibility in libreboot.

-   [List of supported hardware](#supported_list)
    -   [Desktops (x86, AMD and Intel)](#supported_desktops_x86amdintel)
    -   [Servers/workstations](#supported_workstations_x86amd)
    -   [Laptops (ARM)](#supported_laptops_arm)
    -   [Laptops (Intel, x86)](#supported_laptops_x86intel)

-   NOTES:
    -   [Updating the EC firmware on i945 and GM45
        (recommended)](#ecupdate)
    -   [How to find what EC version you have (i945/GM45)](#ecversion)
-   [Recommended wifi chipsets](#recommended_wifi)

List of supported hardware {#supported_list}
--------------------------

Libreboot supports the following systems in this release:

### Desktops (AMD, Intel, x86) {#supported_desktops_x86amdintel}

-   [Gigabyte GA-G41M-ES2L motherboard](ga-g41m-es2l.md)
-   [Intel D510MO motherboard](d510mo.md)
-   [ASUS KCMA-D8 motherboard](kcma-d8.md)
-   [Intel D945GCLF](d945gclf.md)
-   [Apple iMac 5,2](imac52.md)

### Servers/workstations (AMD, x86) {#supported_workstations_x86amd}

-   [ASUS KFSN4-DRE motherboard](kfsn4-dre.md)
-   [ASUS KGPE-D16 motherboard](kgpe-d16.md)

### Laptops (ARM) {#supported_laptops_arm}

-   [ASUS Chromebook C201](c201.md)

### Laptops (Intel, x86) {#supported_laptops_x86intel}

-   [Lenovo ThinkPad X60](x60.md)
-   [Lenovo ThinkPad T60](t60.md)
-   [Lenovo ThinkPad X200](x200.md)
-   [Lenovo ThinkPad R400](r400.md)
-   [Lenovo ThinkPad T400](t400.md)
-   [Lenovo ThinkPad T500](t500.md)
-   [Apple MacBook1,1](macbook11.md)
-   [Apple MacBook2,1](macbook21.md)

'Supported' means that the build scripts know how to build ROM images
for these systems, and that the systems have been tested (confirmed
working). There may be exceptions; in other words, this is a list of
'officially' supported systems.

It is also possible to build ROM images (from source) for other systems
(and virtual systems, e.g. QEMU).

[Back to top of page](#pagetop)

EC update on i945 (X60, T60) and GM45 (X200, T400, T500, R400) {#ecupdate}
==============================================================

It is recommended that you update to the latest EC firmware version. The
[EC firmware](https://libreboot.org/faq/#firmware-ec) is separate from
libreboot, so we don't actually provide that, but if you still have
Lenovo BIOS then you can just run the Lenovo BIOS update utility, which
will update both the BIOS and EC version. See:

-   <https://libreboot.org/docs/install/index.md#flashrom>
-   <http://www.thinkwiki.org/wiki/BIOS_update_without_optical_disk>

NOTE: this can only be done when you are using Lenovo BIOS. How to
update the EC firmware while running libreboot is unknown. Libreboot
only replaces the BIOS firmware, not EC.

Updated EC firmware has several advantages e.g. bettery battery
handling.

[Back to top of page](#pagetop)

How to find what EC version you have (i945/GM45) {#ecversion}
================================================

In GNU+Linux, you can try this:\
**grep 'at EC' /proc/asound/cards**

Sample output:\
**ThinkPad Console Audio Control at EC reg 0x30, fw 7WHT19WW-3.6**

7WHT19WW is the version in different notation, use search engine to find
out regular version - in this case it's a 1.06 for x200 tablet

[Back to top of page](#pagetop)

Recommended wifi chipsets {#recommended_wifi}
-------------------------

The following are known to work well:

-   mini PCI express cards using the Atheros AR9285 chipset (e.g.
    Atheros AR5B95) - 802.11n
-   USB dongles using the AR9271 chipset (e.g. Unex DNuA 93-F) - 802.11n
-   Any of the chipsets listed at
    <https://h-node.org/wifi/catalogue/en/1/1/undef/undef/yes?>

The following was mentioned (on IRC), but it's unknown to the libreboot
project if these work with linux-libre kernel (TODO: test):

-   ar5bhb116 ar9382 ABGN
-   \[0200\]: Qualcomm Atheros AR242x / AR542x Wireless Network Adapter
    (PCI-Express) \[168c:001c\]

[Back to top of page](#pagetop)

Copyright © 2014, 2015, 2016 Leah Rowe <info@minifree.org>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
