
Configuring Parabola (post-install) 
===================================

Post-installation configuration steps for Parabola GNU+Linux-libre.
Parabola is extremely flexible; this is just an example. This example
uses LXDE because it\'s lightweight, but we recommend the *MATE* desktop
(which is actually about as lightweight as LXDE).

[Back to previous index](./)



Table of Contents
=================

-   [Configuring pacman](#pacman_configure)
    -   [Updating Parabola](#pacman_update)
    -   [Maintaining Parabola during system updates](#pacman_maintain)
        -   [Clearing package cache after updating](#pacman_cacheclean)
        -   [Pacman command equivalents (compared to other package
            managers)](#pacman_commandequiv)
    -   [your-freedom](#yourfreedom)
-   [Add a user account](#useradd)
-   [System D](#systemd)
-   [Interesting repositories](#interesting_repos)
-   [Setup a network connection in Parabola](#network)
    -   [Setting hostname](#network_hostname)
    -   [Network status](#network_status)
    -   [Network interface names](#network_devicenames)
    -   [Network setup](#network_setup)
-   [System maintenance](#system_maintain) - important!
-   [Configuring the desktop](#desktop)
    -   [Install Xorg](#desktop_xorg)
    -   [Xorg keyboard layout](#desktop_kblayout)
    -   [Install LXDE](#desktop_lxde)
    -   [LXDE - clock](#lxde_clock)
    -   [LXDE - font](#lxde_font)
    -   [LXDE - screenlock](#lxde_screenlock)
    -   [LXDE - automounting](#lxde_automount)
    -   [LXDE - disable suspend](#lxde_suspend)
    -   [LXDE - battery monitor](#lxde_battery)
    -   [LXDE - network manager](#lxde_network)



While not strictly related to the libreboot project, this guide is
intended to be useful for those interested in installing Parabola on
their libreboot system.

It details configuration steps that I took after installing the base
system, as a follow up to
[encrypted\_parabola.html](encrypted_parabola.html). This guide is
likely to become obsolete at a later date (due to the volatile
\'rolling-release\' model that Arch/Parabola both use), but attempts
will be made to maintain it.

**This guide was valid on 2014-09-21. If you see any changes that should
to be made at the present date, please get in touch with the libreboot
project!**



You do not necessarily have to follow this guide word-for-word;
*parabola* is extremely flexible. The aim here is to provide a common
setup that most users will be happy with. While Parabola can seem
daunting at first glance (especially for new GNU+Linux users), with a
simple guide it can provide all the same usability as Debian or Devuan,
without hiding any details from the user.

Paradoxically, as you get more advanced Parabola can actually become
*easier to use* when you want to set up your system in a special way
compared to what most distributions provide. You will find over time
that other distributions tend to *get in your way*.



**This guide assumes that you already have Parabola installed. If you
have not yet installed Parabola, then [this
guide](encrypted_parabola.html) is highly recommended!**

A lot of the steps in this guide will refer to the Arch wiki. Arch is
the upstream distribution that Parabola uses. Most of this guide will
also tell you to read wiki articles, other pages, manuals, and so on. In
general it tries to cherry pick the most useful information but
nonetheless you are encouraged to learn as much as possible. **It might
take you a few days to fully install your system how you like, depending
on how much you need to read. Patience is key, especially for new
users**.

The Arch wiki will sometimes use bad language, such as calling the whole
system Linux, using the term open-source (or closed-source), and it will
sometimes recommend the use of proprietary software. You need to be
careful about this when reading anything on the Arch wiki.



Some of these steps require internet access. I\'ll go into networking
later but for now, I just connected my system to a switch and did:\
\# **systemctl start dhcpcd.service**\
You can stop it later by running:\
\# **systemctl stop dhcpcd.service**\
For most people this should be enough, but if you don\'t have DHCP on
your network then you should setup your network connection first:\
[Setup network connection in Parabola](#network)



Configure pacman {#pacman_configure}
----------------

pacman (**pac**kage **man**ager) is the name of the package management
system in Arch, which Parabola (as a deblobbed parallel effort) also
uses. Like with \'apt-get\' on Debian or Devuan, this can be used to
add/remove and update the software on your computer.

Based on
<https://wiki.parabolagnulinux.org/Installation_Guide#Configure_pacman>
and from reading <https://wiki.archlinux.org/index.php/Pacman> (make
sure to read and understand this, it\'s very important) and
<https://wiki.parabolagnulinux.org/Official_Repositories>

[Back to top of page.](#pagetop)



Updating Parabola {#pacman_update}
-----------------

In the end, I didn\'t change my configuration for pacman. When you are
updating, resync with the latest package names/versions:\
\# **pacman -Syy**\
(according to the wiki, -Syy is better than Sy because it refreshes the
package list even if it appears to be up to date, which can be useful
when switching to another mirror).\
Then, update the system:\
\# **pacman -Syu**

**Before installing packages with \'pacman -S\', always update first,
using the notes above.**

Keep an eye out on the output, or read it in /var/log/pacman.log.
Sometimes, pacman will show messages about maintenance steps that you
will need to perform with certain files (typically configurations) after
the update. Also, you should check both the Parabola and Arch home pages
to see if they mention any issues. If a new kernel is installed, you
should also update to be able to use it (the currently running kernel
will also be fine). It\'s generally good enough to update Parabola once
every week, or maybe twice. As a rolling release distribution, it\'s a
good idea never to leave your install too outdated; update regularly.
This is simply because of the way the project works; old packages are
deleted from the repositories quickly, once they are updated. A system
that hasn\'t been updated for quite a while will mean potentially more
reading of previous posts through the website, and more maintenance
work.

The Arch forum can also be useful, if others have the same issue as you
(if you encounter issues, that is). The *Parabola* IRC channel
(\#parabola on freenode) can also help you.

Due to this and the volatile nature of Parabola/Arch, you should only
update when you have at least a couple hours of spare time in case of
issues that need to be resolved. You should never update, for example,
if you need your system for an important event, like a presentation or
sending an email to an important person before an allocated deadline,
and so on.

Relax - packages are well-tested regularly when new updates are made to
the repositories. Separate \'testing\' repositories exist for this exact
reason. Despite what many people will tell you, Parabola is fairly
stable and trouble-free, so long as you are aware of how to check for
issues, and are willing to spend some time fixing issues in the rare
event that they do occur.

[Back to top of page.](#pagetop)



Maintaining Parabola {#pacman_maintain}
--------------------

Parabola is a very simple distro, in the sense that you are in full
control and everything is made transparent to you. One consequence is
that you also need to know what you are doing, and what you have done
before. In general, keeping notes (such as what I have done with this
page) can be very useful as a reference in the future (if you wanted to
re-install it or install the distro on another computer, for example).

[Back to top of page.](#pagetop)

### Cleaning the package cache {#pacman_cacheclean}

**The following is very important as you continue to use, update and
maintain your Parabola system:\
<https://wiki.archlinux.org/index.php/Pacman#Cleaning_the_package_cache>.
Essentially, this guide talks about a directory that has to be cleaned
once in a while, to prevent it from growing too big (it\'s a cache of
old package information, updated automatically when you do anything in
pacman).**

To clean out all old packages that are cached:\
\# **pacman -Sc**

The wiki cautions that this should be used with care. For example, since
older packages are deleted from the repo, if you encounter issues and
want to revert back to an older package then it\'s useful to have the
caches available. Only do this if you are sure that you won\'t need it.

The wiki also mentions this method for removing everything from the
cache, including currently installed packages that are cached:\
\# **pacman -Scc**\
This is inadvisable, since it means re-downloading the package again if
you wanted to quickly re-install it. This should only be used when disk
space is at a premium.

[Back to top of page.](#pagetop)

### pacman command equivalents {#pacman_commandequiv}

The following table lists other distro package manager commands, and
their equivalent in pacman:\
<https://wiki.archlinux.org/index.php/Pacman_Rosetta>

[Back to top of page.](#pagetop)



your-freedom {#yourfreedom}
------------

your-freedom is a package specific to Parabola, and it is installed by
default. What it does is conflict with packages from Arch that are known
to be non-free (proprietary) software. When migrating from Arch (there
is a guide on the Parabola wiki for migrating - converting - an existing
Arch system to a Parabola system), installing your-freedom will also
fail if these packages are installed, citing them as conflicts; the
recommended solution is then to delete the offending packages, and
continue installing *your-freedom*.

[Back to top of page.](#pagetop)



Add a user {#useradd}
----------

Based on <https://wiki.archlinux.org/index.php/Users_and_Groups>.

It is important (for security reasons) to create and use a non-root
(non-admin) user account for everyday use. The default \'root\' account
is intended only for critical administrative work, since it has complete
access to the entire operating system.

Read the entire document linked to above, and then continue.

Add your user:\
\# **useradd -m -G wheel -s /bin/bash *yourusername***\
Set a password:\
\# **passwd *yourusername***

Use of the *diceware method* is recommended, for generating secure
passphrases (instead of passwords).

[Back to top of page](#pagetop)



systemd
-------

This is the name of the system used for managing services in Parabola.
It is a good idea to become familiar with it. Read
<https://wiki.archlinux.org/index.php/systemd> and
<https://wiki.archlinux.org/index.php/systemd#Basic_systemctl_usage> to
gain a full understanding. **This is very important! Make sure to read
them.**

An example of a \'service\' could be a webserver (such as lighttpd), or
sshd (openssh), dhcp, etc. There are countless others.

<https://bbs.archlinux.org/viewtopic.php?pid=1149530#p1149530> explains
the background behind the decision by Arch (Parabola\'s upstream
supplier) to use systemd.

The manpage should also help:\
\# **man systemd**\
The section on \'unit types\' is especially useful.

According to the wiki, systemd \'journal\' keeps logs of a size up to
10% of the total size your / partition takes up. on a 60GB root this
would mean 6GB. That\'s not exactly practical, and can have performance
implications later when the log gets too big. Based on instructions from
the wiki, I will reduce the total size of the journal to 50MiB (the wiki
recommends 50MiB).

Open /etc/systemd/journald.conf and find the line that says:\
*\#SystemMaxUse=*\
Change it to say:\
*SystemMaxUse=50M*

The wiki also recommended a method for forwarding journal output to TTY
12 (accessible by pressing ctrl+alt+f12, and you use ctrl+alt+\[F1-F12\]
to switch between terminals). I decided not to enable it.

Restart journald:\
\# **systemctl restart systemd-journald**

The wiki recommends that if the journal gets too large, you can also
simply delete (rm -Rf) everything inside /var/log/journald/\* but
recommends backing it up. This shouldn\'t be necessary, since you
already set the size limit above and systemd will automatically start to
delete older records when the journal size reaches it\'s limit
(according to systemd developers).

Finally, the wiki mentions \'temporary\' files and the utility for
managing them.\
\# **man systemd-tmpfiles**\
The command for \'clean\' is:\
\# **systemd-tmpfiles \--clean**\
According to the manpage, this *\"cleans all files and directories with
an age parameter\"*. According to the Arch wiki, this reads information
in /etc/tmpfiles.d/ and /usr/lib/tmpfiles.d/ to know what actions to
perform. Therefore, it is a good idea to read what\'s stored in these
locations to get a better understanding.

I looked in /etc/tmpfiles.d/ and found that it was empty on my system.
However, /usr/lib/tmpfiles.d/ contained some files. The first one was
etc.conf, containing information and a reference to this manpage:\
\# **man tmpfiles.d**\
Read that manpage, and then continue studying all the files.

The systemd developers tell me that it isn\'t usually necessary to touch
the systemd-tmpfiles utility manually at all.

[Back to top of page](#pagetop)



Interesting repositories {#interesting_repos}
------------------------

Parabola wiki at
<https://wiki.parabolagnulinux.org/Repositories#kernels> mentions about
a repository called \[kernels\] for custom kernels that aren\'t in the
default base. It might be worth looking into what is available there,
depending on your use case.

I enabled it on my system, to see what was in it. Edit /etc/pacman.conf
and below the \'extra\' section add:\
*\[kernels\]\
Include = /etc/pacman.d/mirrorlist*

Now sync with the repository:\
\# **pacman -Syy**

List all available packages in this repository:\
\# **pacman -Sl kernels**

In the end, I decided not to install anything from it but I kept the
repository enabled regardless.

[Back to top of page.](#pagetop)



Setup a network connection in Parabola {#network}
--------------------------------------

Read <https://wiki.archlinux.org/index.php/Configuring_Network>.

[Back to top of page.](#pagetop)

### Set the hostname {#network_hostname}

This should be the same as the hostname that you set in /etc/hostname
when installing Parabola. You can also do it with systemd (do so now, if
you like):\
\# **hostnamectl set-hostname *yourhostname***\
This writes the specified hostname to /etc/hostname. More information
can be found in these manpages:\
\# **man hostname**\
\# **info hostname**\
\# **man hostnamectl**

Add the same hostname to /etc/hosts, on each line. Example:\
*127.0.0.1 localhost.localdomain localhost myhostname\
::1 localhost.localdomain localhost myhostname*

You\'ll note that I set both lines; the 2nd line is for IPv6. More and
more ISPs are providing this now (mine does) so it\'s good to be
forward-thinking here.

The *hostname* utility is part of the *inetutils* package and is in
core/, installed by default (as part of *base*).

[Back to top of page.](#pagetop)

### Network Status {#network_status}

According to the Arch wiki,
[udev](https://wiki.archlinux.org/index.php/Udev) should already detect
the ethernet chipset and load the driver for it automatically at boot
time. You can check this in the *\"Ethernet controller\"* section when
running this command:\
\# **lspci -v**

Look at the remaining sections *\'Kernel driver in use\'* and *\'Kernel
modules\'*. In my case it was as follows:\
*Kernel driver in use: e1000e\
Kernel modules: e1000e*

Check that the driver was loaded by issuing *dmesg | grep module\_name*.
In my case, I did:\
\# **dmesg | grep e1000e**

### Network device names {#network_devicenames}

According to
<https://wiki.archlinux.org/index.php/Configuring_Network#Device_names>,
it is important to note that the old interface names like eth0, wlan0,
wwan0 and so on no longer apply. Instead, *systemd* creates device names
starting with en (for enternet), wl (for wifi) and ww (for wwan) with a
fixed identifier that systemd automatically generates. An example device
name for your ethernet chipset would be *enp0s25*, where it is never
supposed to change.

If you want to enable the old names (eth0, wlan0, wwan0, etc), the Arch
wiki recommends adding *net.ifnames=0* to your kernel parameters (in
libreboot context, this would be accomplished by following the
instructions in [grub\_cbfs.html](grub_cbfs.html)).

For background information, read [Predictable Network Interface
Names](http://www.freedesktop.org/wiki/Software/systemd/PredictableNetworkInterfaceNames/)

Show device names:\
\# **ls /sys/class/net**

Changing the device names is possible (I chose not to do it):\
<https://wiki.archlinux.org/index.php/Configuring_Network#Change_device_name>

[Back to top of page.](#pagetop)

### Network setup {#network_setup}

I actually chose to ignore most of Networking section on the wiki.
Instead, I plan to set up LXDE desktop with the graphical
network-manager client. Here is a list of network managers:\
<https://wiki.archlinux.org/index.php/List_of_applications/Internet#Network_managers>.
If you need to, set a static IP address (temporarily) using the
networking guide and the Arch wiki, or start the dhcpcd service in
systemd. NetworkManager will be setup later, after installing LXDE.

[Back to top of page.](#pagetop)



System Maintenance {#system_maintain}
------------------

Read <https://wiki.archlinux.org/index.php/System_maintenance> before
continuing. Also read
<https://wiki.archlinux.org/index.php/Enhance_system_stability>. **This
is important, so make sure to read them!**

Install smartmontools (it can be used to check smart data. HDDs use
non-free firmware inside, but it\'s transparent to you but the smart
data comes from it. Therefore, don\'t rely on it too much):\
\# **pacman -S smartmontools**\
Read <https://wiki.archlinux.org/index.php/S.M.A.R.T.> to learn how to
use it.

[Back to top of page.](#pagetop)



Configuring the desktop {#desktop}
-----------------------

Based on steps from [General
Recommendations](https://wiki.archlinux.org/index.php/General_recommendations#Graphical_user_interface)
on the Arch wiki. The plan is to use LXDE and LXDM/LightDM, along with
everything else that you would expect on other distributions that
provide LXDE by default.

[Back to top of page.](#pagetop)


### Installing Xorg {#desktop_xorg}

Based on <https://wiki.archlinux.org/index.php/Xorg>.

Firstly, install it!\
\# **pacman -S xorg-server**\
I also recommend installing this (contains lots of useful tools,
including *xrandr*):\
\# **pacman -S xorg-server-utils**

Install the driver. For me this was *xf86-video-intel* on the ThinkPad
X60. T60 and macbook11/21 should be the same.\
\# **pacman -S xf86-video-intel**\
For other systems you can try:\
\# **pacman -Ss xf86-video- | less**\
Combined with looking at your *lspci* output, you can determine which
driver is needed. By default, Xorg will revert to xf86-video-vesa which
is a generic driver and doesn\'t provide true hardware acceleration.

Other drivers (not just video) can be found by looking at the
*xorg-drivers* group:\
\# **pacman -Sg xorg-drivers**\

Mostly you will rely on a display manager, but in case you ever want to
start X without one:\
\# **pacman -S xorg-xinit**

&lt;optional&gt;\
   Arch wiki recommends installing these, for testing that X works:\
   \# **pacman -S xorg-twm xorg-xclock xterm**\
   Refer to <https://wiki.archlinux.org/index.php/Xinitrc>. and test X:\
   \# **startx**\
   When you are satisfied, type ***exit*** in xterm, inside the X
session.\
   Uninstall them (clutter. eww): \# **pacman -S xorg-xinit xorg-twm
xorg-xclock xterm**\
&lt;/optional&gt;

[Back to top of page.](#pagetop)



### Xorg keyboard layout {#desktop_kblayout}

Refer to
<https://wiki.archlinux.org/index.php/Keyboard_configuration_in_Xorg>.

Xorg uses a different configuration method for keyboard layouts, so you
will notice that the layout you set in /etc/vconsole.conf earlier might
not actually be the same in X.

To see what layout you currently use, try this on a terminal emulator in
X:\
\# **setxkbmap -print -verbose 10**

In my case, I wanted to use the Dvorak (UK) keyboard which is quite
different from Xorg\'s default Qwerty (US) layout.

I\'ll just say it now: *XkbModel* can be *pc105* in this case (ThinkPad
X60, with a 105-key UK keyboard). If you use an American keyboard
(typically 104 keys) you will want to use *pc104*.

*XkbLayout* in my case would be *gb*, and *XkbVariant* would be
*dvorak*.

The Arch wiki recommends two different methods for setting the keyboard
layout:\
<https://wiki.archlinux.org/index.php/Keyboard_configuration_in_Xorg#Using_X_configuration_files>
and\
<https://wiki.archlinux.org/index.php/Keyboard_configuration_in_Xorg#Using_localectl>.

In my case, I chose to use the *configuration file* method:\
Create the file /etc/X11/xorg.conf.d/10-keyboard.conf and put this
inside:\
*Section \"InputClass\"\
        Identifier \"system-keyboard\"\
        MatchIsKeyboard \"on\"\
        Option \"XkbLayout\" \"gb\"\
        Option \"XkbModel\" \"pc105\"\
        Option \"XkbVariant\" \"dvorak\"\
EndSection*

For you, the steps above may differ if you have a different layout. If
you use a US Qwerty keyboard, then you don\'t even need to do anything
(though it might help, for the sake of being explicit).

[Back to top of page.](#pagetop)



### Install LXDE {#desktop_lxde}

Desktop choice isn\'t that important to me, so for simplicity I decided
to use LXDE. It\'s lightweight and does everything that I need. If you
would like to try something different, refer to
<https://wiki.archlinux.org/index.php/Desktop_environment>

Refer to <https://wiki.archlinux.org/index.php/LXDE>.

Install it, choosing \'all\' when asked for the default package list:\
\# **pacman -S lxde obconf**

I didn\'t want the following, so I removed them:\
\# **pacman -R lxmusic lxtask**

I also lazily installed all fonts:\
\# **pacman -S \$(pacman -Ssq ttf-)**

And a mail client:\
\# **pacman -S icedove**

In IceCat, go to *Preferences :: Advanced* and disable *GNU IceCat
Health Report*.

I also like to install these:\
\# **pacman -S xsensors stress htop**

Enable LXDM (the default display manager, providing a graphical login):\
\# **systemctl enable lxdm.service**\
It will start when you boot up the system. To start it now, do:\
\# **systemctl start lxdm.service**

Log in with your standard (non-root) user that you created earlier. It
is advisable to also create an xinitrc rule in case you ever want to
start lxde without lxdm. Read
<https://wiki.archlinux.org/index.php/Xinitrc>.

Open LXterminal:\
\$ **cp /etc/skel/.xinitrc \~**\
Open .xinitrc and add the following plus a line break at the bottom of
the file.\
*\# Probably not needed. The same locale info that we set before\
\# Based on advice from the LXDE wiki export LC\_ALL=en\_GB.UTF-8\
export LANGUAGE=en\_GB.UTF-8\
export LANG=en\_GB.UTF-8\
\
\# Start lxde desktop\
exec startlxde\
* Now make sure that it is executable:\
\$ **chmod +x .xinitrc**

[Back to top of page.](#pagetop)



### LXDE - clock {#lxde_clock}

In **Digital Clock Settings** (right click the clock) I set the Clock
Format to *%Y/%m/%d %H:%M:%S*

[Back to top of page.](#pagetop)



### LXDE - font {#lxde_font}

NOTE TO SELF: come back to this later.

[Back to top of page.](#pagetop)



### LXDE - screenlock {#lxde_screenlock}

Arch wiki recommends to use *xscreensaver*:\
\# **pacman -S xscreensaver**

Under *Preferences :: Screensaver* in the LXDE menu, I chose *Mode:
Blank Screen Only*, setting *Blank After*, *Cycle After* and *Lock
Screen After* (checked) to 10 minutes.

You can now lock the screen with *Logout :: Lock Screen* in the LXDE
menu.

[Back to top of page.](#pagetop)



### LXDE - automounting {#lxde_automount}

Refer to
<https://wiki.archlinux.org/index.php/File_manager_functionality>.

I chose to ignore this for now. NOTE TO SELF: come back to this later.

[Back to top of page.](#pagetop)



### LXDE - disable suspend {#lxde_suspend}

When closing the laptop lid, the system suspends. This is annoying at
least to me. NOTE TO SELF: disable it, then document the steps here.

[Back to top of page.](#pagetop)



### LXDE - battery monitor {#lxde_battery}

Right click lxde panel and *Add/Remove Panel Items*. Click *Add* and
select *Battery Monitor*, then click *Add*. Close and then right-click
the applet and go to *Battery Monitor Settings*, check the box that says
*Show Extended Information*. Now click *Close*. When you hover the
cursor over it, it\'ll show information about the battery.

[Back to top of page.](#pagetop)



### LXDE - Network Manager {#lxde_network}

Refer to <https://wiki.archlinux.org/index.php/LXDE#Network_Management>.
Then I read: <https://wiki.archlinux.org/index.php/NetworkManager>.

Install Network Manager:\
\# **pacman -S networkmanager**

You will also want the graphical applet:\
\# **pacman -S network-manager-applet**\
Arch wiki says that an autostart rule will be written at
*/etc/xdg/autostart/nm-applet.desktop*

I want to be able to use a VPN at some point, so the wiki tells me to
do:\
\# **pacman -S networkmanager-openvpn**

LXDE uses openbox, so I refer to:\
<https://wiki.archlinux.org/index.php/NetworkManager#Openbox>.

It tells me for the applet I need:\
\# **pacman -S xfce4-notifyd gnome-icon-theme**\
Also, for storing authentication details (wifi) I need:\
\# **pacman -S gnome-keyring**

I wanted to quickly enable networkmanager:\
\# **systemctl stop dhcpcd**\
\# **systemctl start NetworkManager**\
Enable NetworkManager at boot time:\
\# **systemctl enable NetworkManager**

Restart LXDE (log out, and then log back in).

I added the volume control applet to the panel (right click panel, and
add a new applet). I also later changed the icons to use the gnome icon
theme, in *lxappearance*.

[Back to top of page.](#pagetop)




Copyright © 2014, 2015 Leah Rowe &lt;info@minifree.org&gt;\
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

