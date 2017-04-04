% MacBook 1,1

There is an Apple laptop called the macbook1,1 from 2006 which uses the
same i945 chipset as the ThinkPad X60/T60. A developer ported the
[MacBook2,1](#macbook21) to coreboot, the ROM images also work on the
macbook1,1.

You can refer to [\#macbook21](#macbook21) for most of this. Macbook2,1
laptops come with Core 2 Duo processors which support 64-bit operating
systems (and 32-bit). The MacBook1,1 uses Core Duo processors (supports
32-bit OS but not 64-bit), and it is believed that this is the only
difference.

It is believed that all models are compatible, listed here:

-   <http://www.everymac.com/ultimate-mac-lookup/?search_keywords=MacBook1,1>

### Compatible models

Specifically (Order No. / Model No. / CPU):

-   MA255LL/A / A1181 (EMC 2092) / Core Duo T2500 **(tested - working)**
-   MA254LL/A / A1181 (EMC 2092) / Core Duo T2400 **(tested - working)**
-   MA472LL/A / A1181 (EMC 2092) / Core Duo T2500 (untested)

Also of interest:
[../git/\#config\_macbook21](../git/#config_macbook21).

Unbricking: [this page shows disassembly
guides](https://www.ifixit.com/Device/MacBook_Core_2_Duo) and mono's
page (see [\#macbook21](#macbook21)) shows the location of the SPI flash
chip on the motherboard. [How to remove the
motherboard](https://www.ifixit.com/Guide/MacBook+Core+2+Duo+PRAM+Battery+Replacement/529).

No method is yet known for flashing in GNU+Linux while the Apple
firmware is running. You will need to disassemble the system and flash
externally. Reading from flash seems to work. For external flashing,
refer to [../install/bbb\_setup.html](../install/bbb_setup.html).
