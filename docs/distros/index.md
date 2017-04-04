% List of recommended GNU+Linux distributions for libreboot 

Any distribution should work, so long as it uses kernel mode setting for
graphics. We recommend using distributions that are entirely free
software; most distros out there have proprietary software in them, but
the ones that we recommend do not have proprietary software by default.

[How to install GNU+Linux on a libreboot
system](../gnulinux/grub_boot_installer.html)

Debian
------

Most of the developers use it. We recommend Debian stable releases for most
users. If you want to do development, e.g. libreboot development, then you
should use the testing release of Debian.

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
This page is available under the [CC BY SA 4.0](../cc-by-sa-4.0.txt)
