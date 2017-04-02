---
title: Libreboot
...

[![Libreboot
logo](logo/logo.svg "Canteloupe, the libreboot mascot"){#logo}](logo-info.md)

[Downloads](download.md)                                                    --
[Documentation](docs/)                                                      --
[FAQ](faq.md)                                                               --
[Donate](donate.md)                                                         --
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

Open Letter to the Free Software Community
---------------------------------------------

To the free software community:

Over the past six months, the Libreboot project has been in a state of discord.
After an issue with a transgender employee at the FSF escalated, Libreboot
publicly left GNU with little consultation from the community. Relations with
so many people were strained. Friendships broken, lines of code never written:
the chaos needs to come to an end.

A series of organisational issues with Libreboot enabled this fallout to occur.
We have since corrected them:

Previously, the libreboot repository and the website could only be modified by
the project leader, Leah Rowe. This setup created a single point of failure,
with little leeway for dissenting contributors. Since then, I have joined the
project as the sysadmin. Along with another contributor, Sebastian "Swift Geek"
Grzywna, direct access to the code and servers is shared. Though the project
cannot yet be completely decentralised, this change is a win for transparency.

Previously, most of libreboot.org, including public statements such as those
regarding GNU, were issued by Leah herself. The rest of the team and the
community were not consulted. As Damien Zammit, a former contributor noted, the
word "we" on old Libreboot notices meant "Leah". But alas, there is no room for
the "royal we" in democracy.

Finally, on a personal note, Leah was at the time struggling with gender
dysphoria and substance abuse. Since then, she has been managing these issues.
She agrees that her behaviour was rash and is determined to find a unifying
solution. 

With all of this in mind, were the allegations against the Free Software
Foundation true? Perhaps. Perhaps not. At this point, it doesn't matter.
Indeed, it is unlikely that Libreboot will ever rejoin GNU, but feuding in an
already fragmented community helps nobody. The world of free software is
shrinking and under attack. Though the FSF may make mistakes from time to time,
so do we. We do not need another divide.

Similarly, we would like to work closely with librecore, another project
working to deblob coreboot, whose team includes Damien Zammit. Once librecore
matures, libreboot plans to switch to librecore in lieu of our current
deblobbed fork. That is, there will be less duplication of effort; instead of
competing with librecore, libreboot will complement it.

As a technical update, we are currently working on a Libreboot port to
the X220. Leah and Swift are investigating ways to disable the ME on
Sandybridge hardware, which potentially means more modern Intel hardware may be
supported. Additionally, Paul Kocialkowski has been working on supporting
several new Chromebooks with ARM chips; these ports will also be available in
an upcoming release.

No more "royal we". No more notorious surprises. No more late night
"typofixes".

Transparency and collaboration are the way forward.

~Alyssa Rosenzweig

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
