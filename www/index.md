---
title: Libreboot
...

[![Libreboot logo](logo/logo.svg "Canteloupe, the libreboot
mascot"){#logo}](faq.md#who-did-the-logo)

[FAQ](faq.md)                                                               --
[Download](download.md)                                                     --
[Install](docs/install/)                                                    --
[Documentation](docs/)                                                      --
[News](news/)                                                               --
[Discuss](https://webchat.freenode.net/?channels=libreboot)                 --
[Bugs](https://notabug.org/libreboot/libreboot/issues)                      --
[Contribute](git.md)                                                        --
[Tasks](tasks.md)                                                           --
[Management](management.md)

Libreboot is a [free](https://www.gnu.org/philosophy/free-sw.html) and Open
Source BIOS or UEFI replacement, initialising the hardware and booting your
operating system. We are a member of the
[Peers Community](https://peers.community/) project, an organisation that
supports Free Software.

**[Libreboot has applied to re-join the GNU project,
under new democratic leadership](news/formalised-structure.md)**

Why use Libreboot?
------------------

Many people use non-free proprietary boot firmware, even if they use GNU+Linux.
Non-free BIOS/UEFI firmware often
[contains](faq.md#intel) [backdoors](faq.md#amd), can be slow and have severe
bugs, and you are left helpless at the mercy of the developers. By contrast,
libreboot is fully free software, where anyone can contribute or inspect its
code.

Libreboot is faster, more secure and more reliable than most non-free
firmware, and can provide many advanced feature, like encrypted
/boot/, GPG signature checking before booting your kernel, ability to
load an OS *from the flash chip*, and more.

Libreboot's main upstream provider is [coreboot](https://www.coreboot.org/),
which we deblob. We upstream our custom patches to projects like coreboot,
depthcharge, GRUB, and flashrom where possible. Together, we provide an
automated build and installation system with nontechnical documentation,
allowing Libreboot to be widely used. 

Watch our [FOSDEM 2017 presentation](https://video.fosdem.org/2017/K.1.105/libreboot.mp4) to learn more.
