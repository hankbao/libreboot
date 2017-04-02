---
title: Libreboot
...

[![Libreboot
logo](logo/logo.svg "Canteloupe, the libreboot mascot"){#logo}](logo-info.md)

[Downloads](download.md)                                                    --
[Documentation](docs/)                                                      --
[FAQ](faq.md)                                                               --
[Donate](donate.md)                                                         --
[Buy libreboot machines](suppliers.md)                                      --
[Contributors](contrib.md)                                                  --
[Report a bug](tasks.md)                                                    --
[Submit a patch](git.md)                                                    --
[Hardware Compatibility](docs/hcl/)                                         --
[Installation Guide](docs/install/)                                         --
[Mirror](rsync.md)                                                          --
[IRC](https://webchat.freenode.net/?channels=libreboot)

Libreboot is a [free](https://en.wikipedia.org/wiki/Free_software) and Open
Source BIOS or UEFI replacement, initialising the hardware and booting your
operating system. We are a member of the [Peers Community](https://peers.community/)
project, an organisation that supports Free Software.

%%UNITYLETTER%%

Why use libreboot?
------------------

Many people use non-free proprietary boot firmware, even if they use [a
free operating system](docs/distros/). Non-free BIOS/UEFI firmware often
[contains](faq.md#intel) [backdoors](faq.md#amd), can be slow and have severe
bugs, and you are left helpless at the mercy of the developers.. By contrast,
libreboot is fully free software, where anyone can contribute or inspect its
code.

Libreboot is faster, more secure and more reliable than most non-free
firmware, and can provide many advanced feature, like encrypted
/boot/, GPG signature checking before booting your kernel, ability to
load an OS *from the flash chip*, and more.

Libreboot's main upstream provider is coreboot,
[coreboot](https://www.coreboot.org/), which we deblob. We upstream our custom
patches to projects like coreboot, depthcharge, GRUB, and flashrom where
possible. Together, we provide an automated build and installation system with
nontechnical documentation, allowing Libreboot to be widely used. 

[Watch our FOSDEM 2017 presentation about Libreboot (speaker is Leah
Rowe)](https://video.fosdem.org/2017/K.1.105/libreboot.mp4)
