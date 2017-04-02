% List of recommended GNU+Linux distributions for libreboot 

Any distribution should work, so long as it uses kernel mode setting for
graphics. We recommend using distributions that are entirely free
software; most distros out there have proprietary software in them, but
the ones that we recommend do not have proprietary software by default.

Note: we are not the Free Software Foundation. The recommendations here
consist of both FSF-endorsed and non-FSF-endorsed distros. Some of these
distributions wrongly call the whole system *Linux*. Despite
libreboot's [stance against the GNU project](../../gnu/), we still
agree with the free software philosophy and we still want you to call
the whole system *GNU+Linux*, since this is the technically correct name
for the system.

[How to install GNU+Linux on a libreboot
system](../gnulinux/grub_boot_installer.html)

Debian
------

Most of the developers use it (Leah Rowe always uses testing releases of
Debian). We recommend Debian stable releases for most users. If you want
to do development, e.g. libreboot development, then you should use the
testing release of Debian.

Debian by default comes without non-free software in the default
installation or repos. There is a non-free repo that's not added by
default. Just avoid adding this, and you should have a fully free
system.

[Visit the Debian website](https://www.debian.org/)

We recommend using the *MATE* desktop.

Devuan
------

Devuan is a fork of Debian that does not use the controversial *systemd*
init system.

See: <https://www.devuan.org/os/init-freedom/>.

[Visit the Devuan website](https://www.devuan.org/)

We recommend using the *MATE* desktop.

Qubes
-----

Security-focussed distribution. It makes heavy use of IOMMU and hardware
virtualization to separate running applications, along with other
security features. Provides a full hypervisor for running applications.

Also heavily in favour of adoption of both Libreboot and Coreboot.

**Of all available Libreboot systems, Qubes is currently only known to
be compatible with the ASUS KGPE-D16 and KCMA-D8, due to the hardware
virtualization requirement (and even on those boards, only certain CPUs
have fully functional virtualization in Libreboot).**

[Visit the Qubes website](https://www.qubes-os.org/)

Dragora
-------

Fully free distribution, with a focus on simple package management and
configuration. Experienced users might like this distro. It also has
native support for multiple languages.

[Visit the Dragora website](https://dragora.org/)

Parabola
--------

Fully free bleeding edge distribution, based on the *Arch* distribution.
Good for saavy users and developers in general (some libreboot
developers use this).

Arch users, refer to the [Parabola migration
guide](https://wiki.parabola.nu/Migration_from_the_GNU+Linux_distribution_of_Arch).

[Visit the Parabola website](https://www.parabola.nu/)

We recommend using the *MATE* desktop.



Copyright © 2016 Leah Rowe <info@minifree.org>\
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

