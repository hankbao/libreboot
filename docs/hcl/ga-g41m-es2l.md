% Gigabyte GA-G41M-ES2L desktop board 

This is a desktop board using intel hardware (circa \~2009, ICH7
southbridge, similar performance-wise to the Libreboot X200. It can make
for quite a nifty desktop. Powered by libreboot.

IDE on the board is untested, but it might be possible to use a SATA HDD
using an IDE SATA adapter. The SATA ports do work.

You need to set a custom MAC address in GNU+Linux for the NIC to work.
In /etc/network/interfaces on debian-based systems like Debian or
Devuan, this would be in the entry for your NIC:\
hwaddress ether macaddressgoeshere

Flashing instructions can be found at
[../install/\#flashrom](../install/#flashrom)

Copyright © 2016 Leah Rowe <info@minifree.org>\
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
