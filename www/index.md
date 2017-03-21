---
title: Libreboot
...

[![Libreboot
logo](logo/logo.svg "Canteloupe, the libreboot mascot"){#logo}](logo.md)

[Downloads](download.md)                --
[Documentation](docs/)                  --
[Donate](donate.md)                     --
[Buy libreboot machines](suppliers.md)  --
[Report a bug](tasks.md)                --
[Submit a patch](git.md)                --
[Hardware Compatibility](docs/hcl/)     --
[Installation Guide](docs/install/)     --
[Mirror](rsync.md)

Libreboot is a [free](https://en.wikipedia.org/wiki/Free_software) BIOS or UEFI
replacement; libre *boot firmware* that initializes the hardware and starts a
bootloader for your operating system. [Watch our FOSDEM 2017 presentation about Libreboot (speaker is Leah Rowe,
libreboot's founder)](https://video.fosdem.org/2017/K.1.105/libreboot.mp4)

Libreboot is a long-time member of the [Peers
Community](https://peers.community/) project, an organisation that supports
Free Software and provides infrastructure, e.g. [Git
hosting](https://notabug.org/). **[Libreboot exited from GNU on 2016-09-15, in protest of transgender discrimination at the FSF](gnu.md)**

**[We call on AMD to release source code and specs for the new AMD Ryzen
platforms!](amd-libre.md)**

Questions? [Read the FAQ](faq.md) or
[join](https://webchat.freenode.net/?channels=libreboot) IRC for general
support questions. You can also [contact the developers](contrib.md). [Sign up
to our newsletter for release announcements!](announce.md). We also have a
subreddit forum, for development discussion and user support:
[r/libreboot/](https://www.reddit.com/r/libreboot/).

Why use libreboot?
------------------

Many people use non-free proprietary boot firmware, even if they use [a
free operating system](docs/distros/). Non-free BIOS/UEFI firmware often
[contains](faq.md#intel) [backdoors](faq.md#amd), can be slow and have
severe bugs, where you are left helpless at the mercy of the developers;
*you have no freedom over your computing*. By contrast, libreboot is
building a world where everyone can use, study, adapt and share
software, with true control and ownership over their technology. **In
other words, you should use Libreboot for your freedom's sake!**

Libreboot is faster, more secure and more reliable than most non-free
firmware, and can provide many advanced features (such as encrypted
/boot/, GPG signature checking before booting your kernel, ability to
load an OS *from the flash chip*, and more).

Libreboot's main upstream providers are
[coreboot](https://www.coreboot.org/) (which we deblob, for hardware
initialization), [depthcharge](docs/depthcharge/) (bootloader, and
default libreboot payload on ARM), and GRUB (bootloader, and default
libreboot payload on x86). We also integrate
[flashrom](https://www.flashrom.org/Flashrom) (for installing
libreboot), and several of our own utilities, scripts and configuration
files. All of this is integrated into a single, coherent package that is
easy to use. We add our own patches to the various upstreams used, and
where feasible try to merge upstream as much as possible.

Libreboot provides a fully automated build system and installation
process, with documentation written for non-technical users, in an
attempt to make the software as easy to use as possible. ROM images are
provided, along with utilities, all built from the publicly distributed
source code.
