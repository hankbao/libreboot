% Installing Parabola or Arch GNU+Linux with full disk encryption (including /boot)

Libreboot on x86 uses the GRUB
[payload](http://www.coreboot.org/Payloads#GRUB_2) by default, which
means that the GRUB configuration file (where your GRUB menu comes from)
is stored directly alongside libreboot and it's GRUB payload
executable, inside the flash chip. In context, this means that
installing distributions and managing them is handled slightly
differently compared to traditional BIOS systems.

On most systems, the /boot partition has to be left unencrypted while
the others are encrypted. This is so that GRUB, and therefore the
kernel, can be loaded and executed since the firmware can't open a LUKS
volume. Not so with libreboot! Since GRUB is already included directly
as a payload, even /boot can be encrypted. This protects /boot from
tampering by someone with physical access to the system.

**This guide is \*only\* for the GRUB payload. If you use the
depthcharge payload, ignore this section entirely.**

This guide is intended for the Parabola distribution, but it should also
work (with some adaptation) for *Arch*. We recomend using Parabola,
which is a version of Arch that removes all proprietary software, both
in the default installation and in the package repositories. It usually
lags behind Arch by only a day or two, so it is still usable for most
people. See [Arch to Parabola migration
guide](https://wiki.parabola.nu/index.php?title=Migration_from_the_GNU+Linux_distribution_of_Arch&redirect=no).

Note: on some thinkpads, a faulty DVD drive can cause the cryptomount -a
step during boot to fail. If this happens to you, try removing the
drive.

[Back to previous index](./)



Boot Parabola's install environment. [How to boot a GNU+Linux
installer](grub_boot_installer.html).

For this guide I used the 2015 08 01 image to boot the live installer
and install the system. This is available at [this
page](https://wiki.parabola.nu/Get_Parabola#Main_live_ISO).

This guide will go through the installation steps taken at the time of
writing, which may or may not change due to the volatile nature of
Parabola (it changes all the time). In general most of it should remain
the same. If you spot mistakes, please say so! This guide will be ported
to the Parabola wiki at a later date. For up to date Parabola install
guide, go to the Parabola wiki. This guide essentially cherry picks the
useful information (valid at the time of writing: 2015-08-25).



This section deals with wiping the storage device on which you plan to
install Parabola GNU+Linux. Follow these steps, but if you use an SSD,
also:

- beware there are issues with TRIM (not enabled through luks) and
security issues if you do enable it. See [this
page](https://wiki.archlinux.org/index.php/Dm-crypt/Specialties#Discard.2FTRIM_support_for_solid_state_drives_.28SSD.29)
for more info.

- make sure it's brand-new (or barely used). Or, otherwise, be sure
that it never previously contained plaintext copies of your data.

- make sure to read [this
article](https://wiki.archlinux.org/index.php/Solid_State_Drives). Edit
/etc/fstab later on when chrooted into your install. Also, read the
whole article and keep all points in mind, adapting them for this guide.

Securely wipe the drive:
    # dd if=/dev/urandom of=/dev/sda; sync

NOTE: If you have an SSD, only do this the first time. If it was already
LUKS-encrypted before, use the info below to wipe the LUKS header. Also,
check online for your SSD what the recommended erase block size is. For
example if it was 2MiB:
    # dd if=/dev/urandom of=/dev/sda bs=2M; sync

If your drive was already LUKS encrypted (maybe you are re-installing
your distro) then it is already 'wiped'. You should just wipe the LUKS
header.
<https://www.lisenet.com/2013/luks-add-keys-backup-and-restore-volume-header/>
showed me how to do this. It recommends doing the first 3MiB. Now, that
guide is recommending putting zero there. I'm going to use urandom. Do
this:

    # head -c 3145728 /dev/urandom > /dev/sda; sync
(Wiping the LUKS header is important, since it has hashed passphrases
and so on. It's 'secure', but 'potentially' a risk).



Change keyboard layout
----------------------

Parabola live shell assumes US Qwerty. If you have something different,
list the available keymaps and use yours:

    # localectl list-keymaps
    # loadkeys LAYOUT
For me, LAYOUT would have been dvorak-uk.



Establish an internet connection
--------------------------------

Refer to [this
guide](https://wiki.parabola.nu/Beginners%27_guide#Establish_an_internet_connection).
Wired is recommended, but wireless is also explained there.



Getting started
---------------

The beginning is based on
<https://wiki.parabolagnulinux.org/Installation_Guide>. Then I referred
to <https://wiki.archlinux.org/index.php/Partitioning> at first.



dm-mod
------

device-mapper will be used - a lot. Make sure that the kernel module is
loaded:

    # modprobe dm-mod

Create LUKS partition
---------------------

Note that the default iteration time is 2000ms (2 seconds) if not
specified in cryptsetup. You should set a lower time than this,
otherwise there will be an approximate 20 second delay when booting your
system. We recommend 500ms (0.5 seconds), and this is included in the
prepared cryptsetup command below. Note that the iteration time is for
security purposes (mitigates brute force attacks), so anything lower
than 5 seconds is probably not ok.

I am using MBR partitioning, so I use cfdisk:

    # cfdisk /dev/sda

I create a single large sda1 filling the whole drive, leaving it as the
default type 'Linux' (83).

Now I refer to
<https://wiki.archlinux.org/index.php/Dm-crypt/Drive_preparation#Partitioning>:\
I am then directed to
<https://wiki.archlinux.org/index.php/Dm-crypt/Device_encryption>.

Parabola forces you to RTFM. Do that.

It tells me to run:

    # cryptsetup benchmark (for making sure the list below is
populated)\
Then:

    # cat /proc/crypto
This gives me crypto options that I can use. It also provides a
representation of the best way to set up LUKS (in this case, security is
a priority; speed, a distant second). To gain a better understanding, I
am also reading:

    # man cryptsetup

Following that page, based on my requirements, I do the following based
on
<https://wiki.archlinux.org/index.php/Dm-crypt/Device_encryption#Encryption_options_for_LUKS_mode>.
Reading through, it seems like Serpent (encryption) and Whirlpool (hash)
is the best option.

I am initializing LUKS with the following:\
\# **cryptsetup -v --cipher serpent-xts-plain64 --key-size 512 --hash
whirlpool --iter-time 500 --use-random --verify-passphrase luksFormat
/dev/sda1** Choose a **secure** passphrase here. Ideally lots of
lowercase/uppercase numbers, letters, symbols etc all in a random
pattern. The password length should be as long as you are able to handle
without writing it down or storing it anywhere.

Use of the *diceware method* is recommended, for generating secure
passphrases (instead of passwords).



Create LVM
----------

Now I refer to <https://wiki.archlinux.org/index.php/LVM>.

Open the LUKS partition:

    # cryptsetup luksOpen /dev/sda1 lvm
(it will be available at /dev/mapper/lvm)

Create LVM partition:

    # pvcreate /dev/mapper/lvm
Show that you just created it:

    # pvdisplay

Now I create the volume group, inside of which the logical volumes will
be created:

    # vgcreate matrix /dev/mapper/lvm
(volume group name is 'matrix' - choose your own name, if you like)
Show that you created it:

    # vgdisplay

Now create the logical volumes:

    # lvcreate -L 2G matrix -n swapvol (2G swap partition, named
swapvol)\
Again, choose your own name if you like. Also, make sure to choose a
swap size of your own needs. It basically depends on how much RAM you
have installed. I refer to
<http://www.linux.com/news/software/applications/8208-all-about-linux-swap-space>.

    # lvcreate -l +100%FREE matrix -n root (single large partition in
the rest of the space, named root)\
You can also be flexible here, for example you can specify a /boot, a /,
a /home, a /var, a /usr, etc. For example, if you will be running a
web/mail server then you want /var in its own partition (so that if it
fills up with logs, it won't crash your system). For a home/laptop
system (typical use case), a root and a swap will do (really).

Verify that the logical volumes were created, using the following
command:

    # lvdisplay



Create / and swap partitions, and mount
---------------------------------------

For the swapvol LV I use:

    # mkswap /dev/mapper/matrix-swapvol
Activate swap:

    # swapon /dev/matrix/swapvol

For the root LV I use:

    # mkfs.btrfs /dev/mapper/matrix-root

Mount the root (/) partition:

    # mount /dev/matrix/root /mnt



Continue with Parabola installation
-----------------------------------

This guide is really about GRUB, Parabola and cryptomount. I have to
show how to install Parabola so that the guide can continue.

Now I am following the rest of
<https://wiki.parabolagnulinux.org/Installation_Guide>. I also cross
referenced <https://wiki.archlinux.org/index.php/Installation_guide>.

Create /home and /boot on root mountpoint:

    # mkdir -p /mnt/home
    # mkdir -p /mnt/boot

Once all the remaining partitions, if any, have been mounted, the
devices are ready to install Parabola.

In **/etc/pacman.d/mirrorlist**, comment out all lines except the Server
line closest to where you are (I chose the UK Parabola server (main
server)) and then did:

    # pacman -Syy
    # pacman -Syu
    # pacman -Sy pacman (and then I did the other 2 steps above,
again)\
In my case I did the steps in the next paragraph, and followed the steps
in this paragraph again.

<troubleshooting>\
   The following is based on 'Verification of package signatures' in
the Parabola install guide.\
   Check there first to see if steps differ by now.\
   Now you have to update the default Parabola keyring. This is used for
signing and verifying packages:\
   \# **pacman -Sy parabola-keyring**\
   It says that if you get GPG errors, then it's probably an expired
key and, therefore, you should do:\
   \# **pacman-key --populate parabola**\
   \# **pacman-key --refresh-keys**\
   \# **pacman -Sy parabola-keyring**\
   To be honest, you should do the above anyway. Parabola has a lot of
maintainers, and a lot of keys. Really!\
   If you get an error mentioning dirmngr, do:\
   \# **dirmngr </dev/null**\
   Also, it says that if the clock is set incorrectly then you have to
manually set the correct time\
   (if keys are listed as expired because of it):\
   \# **date MMDDhhmm\[\[CC\]YY\]\[.ss\]**\
   I also had to install:\
   \# **pacman -S archlinux-keyring**\
   \# **pacman-key --populate archlinux**\
   In my case I saw some conflicting files reported in pacman, stopping
me from using it.\
   I deleted the files that it mentioned and then it worked.
Specifically, I had this error:\
   *licenses: /usr/share/licenses/common/MPS exists in filesystem*\
   I rm -Rf'd the file and then pacman worked. I'm told that the
following would have also made it work:\
   \# **pacman -Sf licenses**\
</troubleshooting>\

I also like to install other packages (base-devel, compilers and so on)
and wpa\_supplicant/dialog/iw/wpa\_actiond are needed for wireless after
the install:\
\# **pacstrap /mnt base base-devel wpa\_supplicant dialog iw
wpa\_actiond**



Configure the system
--------------------

Generate an fstab - UUIDs are used because they have certain advantages
(see <https://wiki.parabola.nu/Fstab#Identifying_filesystems>. If you
prefer labels instead, replace the -U option with -L):

    # genfstab -U -p /mnt >> /mnt/etc/fstab
Check the created file:

    # cat /mnt/etc/fstab
(If there are any errors, edit the file. Do **NOT** run the genfstab
command again!)

Chroot into new system:

    # arch-chroot /mnt /bin/bash

It's a good idea to have this installed:

    # pacman -S linux-libre-lts

It was also suggested that you should install this kernel (read up on
what GRSEC is):

    # pacman -S linux-libre-grsec

This is another kernel that sits inside /boot, which you can use. LTS
means 'long-term support'. These are so-called 'stable' kernels that
can be used as a fallback during updates, if a bad kernel causes issues
for you.

Parabola does not have wget. This is sinister. Install it:

    # pacman -S wget

Locale:

    # vi /etc/locale.gen
Uncomment your needed localisations. For example en\_GB.UTF-8 (UTF-8 is
highly recommended over other options).

    # locale-gen
    # echo LANG=en\_GB.UTF-8 > /etc/locale.conf
    # export LANG=en\_GB.UTF-8

Console font and keymap:

    # vi /etc/vconsole.conf
In my case:

    KEYMAP=dvorak-uk
    FONT=lat9w-16

Time zone:

    # ln -s /usr/share/zoneinfo/Europe/London /etc/localtime
(Replace Zone and Subzone to your liking. See /usr/share/zoneinfo)

Hardware clock:

    # hwclock --systohc --utc

Hostname: Write your hostname to /etc/hostname. For example, if your
hostname is parabola:

    # echo parabola > /etc/hostname
Add the same hostname to /etc/hosts:

    # vi /etc/hosts

    #<ip-address> <hostname.domain.org> <hostname>
    127.0.0.1   localhost.localdomain   localhost   parabola
    ::1     localhost.localdomain   localhost   parabola

Configure the network: Refer to
<https://wiki.parabola.nu/Beginners%27_guide#Configure_the_network>.

Mkinitcpio: Configure /etc/mkinitcpio.conf as needed (see
<https://wiki.parabola.nu/Mkinitcpio>). Runtime modules can be found in
/usr/lib/initcpio/hooks, and build hooks can be found in
/usr/lib/initcpio/install. (\# **mkinitcpio -H hookname** gives
information about each hook.) Specifically, for this use case:

    # vi /etc/mkinitcpio.conf
Then modify the file like so:

-   MODULES="i915"
-   This forces the driver to load earlier, so that the console font
    isn't wiped out after getting to login). Macbook21 users will also
    need **hid-generic, hid and hid-apple to have a working keyboard
    when asked to enter the LUKS password.**
-   HOOKS="base udev autodetect modconf block keyboard keymap
    consolefont encrypt lvm2 filesystems fsck shutdown"
-   Explanation:
-   keymap adds to initramfs the keymap that you specified in
    /etc/vconsole.conf
-   consolefont adds to initramfs the font that you specified in
    /etc/vconsole.conf
-   encrypt adds LUKS support to the initramfs - needed to unlock your
    disks at boot time
-   lvm2 adds LVM support to the initramfs - needed to mount the LVM
    partitions at boot time
-   shutdown is needed according to Parabola wiki for unmounting devices
    (such as LUKS/LVM) during shutdown)

Now using mkinitcpio, you can create the kernel and ramdisk for booting
with (this is different from Arch, specifying linux-libre instead of
linux):

    # mkinitcpio -p linux-libre
Also do it for linux-libre-lts:

    # mkinitcpio -p linux-libre-lts
Also do it for linux-libre-grsec:

    # mkinitcpio -p linux-libre-grsec

Set the root password: At the time of writing, Parabola used SHA512 by
default for its password hashing. I referred to
<https://wiki.archlinux.org/index.php/SHA_password_hashes>.

    # vi /etc/pam.d/passwd
Add rounds=65536 at the end of the uncommented 'password' line.

    # passwd root
Make sure to set a secure password! Also, it must never be the same as
your LUKS password.

Use of the *diceware method* is recommended, for generating secure
passphrases (instead of passwords).



Extra security tweaks
---------------------

Based on <https://wiki.archlinux.org/index.php/Security>.

Restrict access to important directories:

    # chmod 700 /boot /etc/{iptables,arptables}

Lockout user after three failed login attempts:\
Edit the file /etc/pam.d/system-login and comment out that line:\
*\# auth required pam\_tally.so onerr=succeed file=/var/log/faillog*\
Or just delete it. Above it, put:\
*auth required pam\_tally.so deny=2 unlock\_time=600 onerr=succeed
file=/var/log/faillog*\
To unlock a user manually (if a password attempt is failed 3 times),
do:

    # pam\_tally --user *theusername* --reset What the above
configuration does is lock the user out for 10 minutes, if they make 3
failed login attempts.

Configure sudo - not covered here. Will be covered post-installation in
another tutorial, at a later date. If this is a single-user system, you
don't really need sudo.



Unmount, reboot!
----------------

Exit from chroot:

    # exit

unmount:

    # umount -R /mnt
    # swapoff -a

deactivate the lvm lv's:

    # lvchange -an /dev/matrix/root
    # lvchange -an /dev/matrix/swapvol

Lock the encrypted partition (close it):

    # cryptsetup luksClose lvm

    # shutdown -h now
Remove the installation media, then boot up again.



Booting from GRUB
-----------------

Initially you will have to boot manually. Press C to get to the GRUB
command line. The underlined parts are optional (using those 2
underlines will boot lts kernel instead of normal).

grub> **cryptomount -a**\
grub> **set root='lvm/matrix-root'**\
grub> **linux /boot/vmlinuz-linux-libre-lts root=/dev/matrix/root
cryptdevice=/dev/sda1:root**\
grub> **initrd /boot/initramfs-linux-libre-lts.img**\
grub> **boot**\

You could also make it load /boot/vmlinuz-linux-libre-grsec and
/boot/initramfs-linux-libre-grsec.img



Follow-up tutorial: configuring Parabola
----------------------------------------

We will modify grub.config inside the ROM and do all kinds of fun stuff,
but I recommend that you first transform the current bare-bones Parabola
install into a more useable system. Doing so will make the upcoming ROM
modifications MUCH easier to perform and less risky!
[configuring\_parabola.html](configuring_parabola.html) shows my own
notes post-installation. Using these, you can get a basic system similar
to the one that I chose for myself. You can also cherry pick useful
notes and come up with your own system. Parabola is user-centric, which
means that you are in control. For more information, read [The Arch
Way](https://wiki.archlinux.org/index.php/The_Arch_Way) (Parabola also
follows it).



Modify grub.cfg inside the ROM
------------------------------

(Re-)log in to your system, pressing C, so booting manually from GRUB
(see above). You need to modify the ROM, so that Parabola can boot
automatically with this configuration. [grub\_cbfs.html](grub_cbfs.html)
shows you how. Follow that guide, using the configuration details below.
If you go for option 2 (re-flash), promise to do this on grubtest.cfg
first! We can't emphasise this enough. This is to reduce the
possibility of bricking your device!

I will go for the re-flash option here. Firstly, cd to the
libreboot\_util/cbfstool/{armv7l i686 x86\_64} directory. Dump the
current firmware - where *libreboot.rom* is an example: make sure to
adapt:

    # flashrom -p internal -r libreboot.rom
If flashrom complains about multiple flash chips detected, add a *-c*
option at the end, with the name of your chosen chip is quotes.\
You can check if everything is in there (*grub.cfg* and *grubtest.cfg*
would be really nice):
    $ ./cbfstool libreboot.rom print
Extract grubtest.cfg:
    $ ./cbfstool libreboot.rom extract -n grubtest.cfg -f grubtest.cfg\
And modify:

    $ vi grubtest.cfg

In grubtest.cfg, inside the 'Load Operating System' menu entry, change
the contents to:

    cryptomount -a

    set root='lvm/matrix-root'

    linux /boot/vmlinuz-linux-libre-lts root=/dev/matrix/root cryptdevice=/dev/sda1:root

    initrd /boot/initramfs-linux-libre-lts.img

Note: the underlined parts above (-lts) can also be removed, to boot the
latest kernel instead of LTS (long-term support) kernels. You could also
copy the menu entry and in one have -lts, and without in the other
menuentry. You could also create a menu entry to load
/boot/vmlinuz-linux-libre-grsec and
/boot/initramfs-linux-libre-grsec.img The first entry will load by
default.

Without specifying a device, the *-a* parameter tries to unlock all
detected LUKS volumes. You can also specify -u UUID or -a (device).

[Refer to this guide](grub_hardening.html) for further guidance on
hardening your GRUB configuration, for security purposes.

Save your changes in grubtest.cfg, then delete the unmodified config
from the ROM image:

    $ ./cbfstool libreboot.rom remove -n grubtest.cfg
and insert the modified grubtest.cfg:\
\$ **./cbfstool libreboot.rom add -n grubtest.cfg -f grubtest.cfg -t
raw**\

Now refer to <http://libreboot.org/docs/install/#flashrom>. Cd (up) to
the libreboot\_util directory and update the flash chip contents:

    # ./flash update libreboot.rom
Ocassionally, coreboot changes the name of a given board. If flashrom
complains about a board mismatch, but you are sure that you chose the
correct ROM image, then run this alternative command:

    # ./flash forceupdate libreboot.rom
You should see "Verifying flash\... VERIFIED." written at the end of
the flashrom output.

With this new configuration, Parabola can boot automatically and you
will have to enter a password at boot time, in GRUB, before being able
to use any of the menu entries or switch to the terminal. Let's test it
out: reboot and choose grubtest.cfg from the GRUB menu, using the arrow
keys on your keyboard. Enter the name you chose, the GRUB password, your
LUKS passphrase and login as root/your user. All went well? Great!

If it does not work like you want it to, if you are unsure or sceptical
in any way, don't despair: you have been wise and did not brick your
device! Reboot and login the default way, and then modify your
grubtest.cfg until you get it right! **Do \*not\* proceed past this
point unless you are 100% sure that your new configuration is safe (or
desirable) to use.**

Now, we can easily and safely create a copy of grubtest.cfg, called
grub.cfg. This will be the same except for one difference: the menuentry
'Switch to grub.cfg' is changed to 'Switch to grubtest.cfg' and,
inside it, all instances of grub.cfg to grubtest.cfg. This is so that
the main config still links (in the menu) to grubtest.cfg, so that you
don't have to manually switch to it, in case you ever want to follow
this guide again in the future (modifying the already modified config).
Inside libreboot\_util/cbfstool/{armv7l i686 x86\_64}, we can do this
with the following command:\
\$ **sed -e 's:(cbfsdisk)/grub.cfg:(cbfsdisk)/grubtest.cfg:g' -e
's:Switch to grub.cfg:Switch to grubtest.cfg:g' < grubtest.cfg >
grub.cfg**\
Delete the grub.cfg that remained inside the ROM:

    $ ./cbfstool libreboot.rom remove -n grub.cfg
Add the modified version that you just made:

    $ ./cbfstool libreboot.rom add -n grub.cfg -f grub.cfg -t raw

Now you have a modified ROM. Once more, refer to
<http://libreboot.org/docs/install/#flashrom>. Cd to the libreboot\_util
directory and update the flash chip contents:

    # ./flash update libreboot.rom
And wait for the "Verifying flash\... VERIFIED." Once you have done
that, shut down and then boot up with your new configuration.

When done, delete GRUB (remember, we only needed it for the
*grub-mkpasswd-pbkdf2* utility; GRUB is already part of libreboot,
flashed alongside it as a *payload*):

    # pacman -R grub



If you followed all that correctly, you should now have a fully
encrypted Parabola installation. Refer to the wiki for how to do the
rest.



Bonus: Using a key file to unlock /boot/
----------------------------------------

By default, you will have to enter your LUKS passphrase twice; once in
GRUB, and once when booting the kernel. GRUB unlocks the encrypted
partition and then loads the kernel, but the kernel is not aware of the
fact that it is being loaded from an encrypted volume. Therefore, you
will be asked to enter your passphrase a second time. A workaround is to
put a keyfile inside initramfs, with instructions for the kernel to use
it when booting. This is safe, because /boot/ is encrypted (otherwise,
putting a keyfile inside initramfs would be a bad idea).\
Boot up and login as root or your user. Then generate the key file:\
\# **dd bs=512 count=4 if=/dev/urandom of=/etc/mykeyfile
iflag=fullblock**\
Insert it into the luks volume:

    # cryptsetup luksAddKey /dev/sdX /etc/mykeyfile
and enter your LUKS passphrase when prompted. Add the keyfile to the
initramfs by adding it to FILES in /etc/mkinitcpio.conf. For example:

    # FILES="/etc/mykeyfile"
Create the initramfs image from scratch:

    # mkinitcpio -p linux-libre
    # mkinitcpio -p linux-libre-lts
    # mkinitcpio -p linux-libre-grsec
Add the following to your grub.cfg - you are now able to do that, see
above! -, or add it in the kernel command line for GRUB:

    # cryptkey=rootfs:/etc/mykeyfile
\
You can also place this inside the grub.cfg that exists in CBFS:
[grub\_cbfs.html](grub_cbfs.html).



Further security tips
---------------------

<https://wiki.archlinux.org/index.php/Security>.\
<https://wiki.parabolagnulinux.org/User:GNUtoo/laptop>



Troubleshooting
===============

A user reported issues when booting with a docking station attached on
an X200, when decrypting the disk in GRUB. The error *AHCI transfer
timed out* was observed. The workaround was to remove the docking
station.

Further investigation revealed that it was the DVD drive causing
problems. Removing that worked around the issue.


    "sudo wodim -prcap" shows information about the drive:
    Device was not specified. Trying to find an appropriate drive...
    Detected CD-R drive: /dev/sr0
    Using /dev/cdrom of unknown capabilities
    Device type    : Removable CD-ROM
    Version        : 5
    Response Format: 2
    Capabilities   : 
    Vendor_info    : 'HL-DT-ST'
    Identification : 'DVDRAM GU10N    '
    Revision       : 'MX05'
    Device seems to be: Generic mmc2 DVD-R/DVD-RW.

    Drive capabilities, per MMC-3 page 2A:

      Does read CD-R media
      Does write CD-R media
      Does read CD-RW media
      Does write CD-RW media
      Does read DVD-ROM media
      Does read DVD-R media
      Does write DVD-R media
      Does read DVD-RAM media
      Does write DVD-RAM media
      Does support test writing

      Does read Mode 2 Form 1 blocks
      Does read Mode 2 Form 2 blocks
      Does read digital audio blocks
      Does restart non-streamed digital audio reads accurately
      Does support Buffer-Underrun-Free recording
      Does read multi-session CDs
      Does read fixed-packet CD media using Method 2
      Does not read CD bar code
      Does not read R-W subcode information
      Does read raw P-W subcode data from lead in
      Does return CD media catalog number
      Does return CD ISRC information
      Does support C2 error pointers
      Does not deliver composite A/V data

      Does play audio CDs
      Number of volume control levels: 256
      Does support individual volume control setting for each channel
      Does support independent mute setting for each channel
      Does not support digital output on port 1
      Does not support digital output on port 2

      Loading mechanism type: tray
      Does support ejection of CD via START/STOP command
      Does not lock media on power up via prevent jumper
      Does allow media to be locked in the drive via PREVENT/ALLOW command
      Is not currently in a media-locked state
      Does not support changing side of disk
      Does not have load-empty-slot-in-changer feature
      Does not support Individual Disk Present feature

      Maximum read  speed:  4234 kB/s (CD  24x, DVD  3x)
      Current read  speed:  4234 kB/s (CD  24x, DVD  3x)
      Maximum write speed:  4234 kB/s (CD  24x, DVD  3x)
      Current write speed:  4234 kB/s (CD  24x, DVD  3x)
      Rotational control selected: CLV/PCAV
      Buffer size in KB: 1024
      Copy management revision supported: 1
      Number of supported write speeds: 4
      Write speed # 0:  4234 kB/s CLV/PCAV (CD  24x, DVD  3x)
      Write speed # 1:  2822 kB/s CLV/PCAV (CD  16x, DVD  2x)
      Write speed # 2:  1764 kB/s CLV/PCAV (CD  10x, DVD  1x)
      Write speed # 3:   706 kB/s CLV/PCAV (CD   4x, DVD  0x)

    Supported CD-RW media types according to MMC-4 feature 0x37:
      Does write multi speed       CD-RW media
      Does write high  speed       CD-RW media
      Does write ultra high speed  CD-RW media
      Does not write ultra high speed+ CD-RW media



Copyright © 2014, 2015, 2016 Leah Rowe <info@minifree.org>\
Copyright © 2015 Jeroen Quint <jezza@diplomail.ch>\
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

