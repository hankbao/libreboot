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
[Chat](https://webchat.freenode.net/?channels=libreboot)                    --
[Bugs](https://notabug.org/libreboot/libreboot/issues)                      --
[Send patches](git.md)                                                      --
[Buy preinstalled](suppliers.md)

Libreboot is [freedom-respecting](https://www.gnu.org/philosophy/free-sw.html)
*boot firmware*, initialising the hardware and booting an operating system.
This replaces the proprietary BIOS/UEFI boot firmware found in computers.
Libreboot is compatible with [certain computers that have
been ported to](docs/hardware/) on ARM and x86.

*Boot firmware* is the low-level software in a computer, which executes the
moment that computer is turned on. It brings all of the components (CPU,
[memory controller](https://en.wikipedia.org/wiki/Memory_controller),
some peripherals and so on) into a useable state so that it can easily run
software. The boot firmware will typically load an *operating system*
(GNU+Linux, BSD, etc) which provides a common interface for application
software to make use of hardware in the computer. In addition to Libreboot, we
also recommend the use of a freedom respecting operating system as defined
by the [GNU Free System Distribution
Guidelines](https://gnu.org/distros/free-system-distribution-guidelines.html)
(so, no Windows/Mac. Use GNU+Linux!)

Combined, the boot firmware and operating system provide a unified interface
which makes computers functional whether for day to day usage or software
development.

Libreboot's main upstream provider is [coreboot](https://www.coreboot.org/),
from which we [remove binary blobs](docs/#about-the-libreboot-project).
We upstream our custom patches to projects like coreboot,
depthcharge, GRUB, and flashrom where possible. Together, our build system
and documentation is provided with the aim of making free boot firmware
accessible to all. *In other words, Libreboot is a coreboot distribution!* Put
simply, Libreboot integrates all of the required software components into a
single unified package that is applicable to most use-case scenarios.

We provide user support via [\#libreboot
IRC](https://webchat.freenode.net/?channels=libreboot) on Freenode. Development
discussion also occurs on IRC. Instructions for sending patches are
on the [git](git.md) page.

Why use Libreboot?
------------------

Did you know you have rights? The right to privacy, freedom of thought, freedom
of speech and the right to read. In the context of computing, that means anyone
can use [free software](https://www.gnu.org/philosophy/free-sw.html). Simply
speaking, free software is software that is under the direct sovereignty of the
user and, more importantly, the collective that is the *community*. Non-free
software (e.g. Windows, MacOS or proprietary BIOS/UEFI) is under the exclusive
control of its owners, not users! With freedom-respecting software, the users
can at any time study the source code and even become developers. Other ways
(for example) to contribute could be by hiring for development (free software
enables the right to hire *absolutely anyone*), or contribute in a number
of other ways (e.g. writing documentation or providing user support) which
gives the developers more time to focus on code. We believe deeply in software
freedom!

Many people use [non-free](https://www.gnu.org/philosophy/proprietary.html)
proprietary boot firmware, even if they use GNU+Linux. Non-free BIOS/UEFI
firmware often [contains](faq.md#intel) [backdoors](faq.md#amd), can be slow
and have severe bugs. Development and support can be abandoned at any time.
By contrast, libreboot is fully free software, where anyone can contribute or
inspect its code. Libreboot is not simply free software; it is also 
[copyleft](http://www.gnu.org/philosophy/copyleft.html) software, released
under (for the most part) a mixture of
[GNU General Public License v2 and v3](http://www.gnu.org/licenses/gpl.html).
This means that the software will always be *free* for everyone. If someone
were to take Libreboot and try to make it proprietary, they'd be *breaking the
law*. In other words, copyleft ensures a public *commons* where all knowledge
and power is shared without discrimination.

Libreboot is faster, more secure and more reliable than most non-free
firmware. Libreboot provides many advanced features, like encrypted
/boot/, GPG signature checking before booting a Linux kernel and more!
