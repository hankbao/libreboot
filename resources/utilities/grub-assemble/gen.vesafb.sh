#!/bin/bash
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

set -u -e -v

# TODO: Fail (and clean up) if GRUB isn't actually built. Error checking.

# This is where GRUB is expected to be (outside of the grub-assemble, instead in main checkout)
grubdir="../../../grub"

# Install modules (installed, but not automatically loaded)
grub_install_modules="adler32 all_video archelp ata backtrace bitmap bitmap_scale cmp cpio_be cpio cpuid crc64 cs5536 div_test efiemu elf eval exfat extcmd file fshelp gettext gfxmenu gptsync gzio hashsum hexdump http linux16 loadenv lzopio mda_text mmap mpi msdospart multiboot2 multiboot nativedisk net newc ntfscomp ntfs odc offsetio parttool priority_queue procfs progress read relocator scsi search_fs_file search_fs_uuid search_label search setjmp setpci sleep squash4 tar terminfo testload testspeed tftp time trig tr true udf ufs1_be ufs1 ufs2 usbserial_common usbserial_ftdi usbserial_pl2303 usbserial_usbdebug usbtest video_colors videotest_checksum xzio"

# Modules (and always loaded)
grub_modules="acpi ahci at_keyboard boot cat cbfs cbls cbtime chain cmosdump cmostest cbmemc crypto cryptodisk configfile datehook date datetime diskfilter disk echo ext2 ehci fat halt help iorw iso9660 keystatus linux loopback ls lsacpi lsmmap lspci luks lvm memdisk minicmd memrw morse normal ohci part_gpt part_msdos password password_pbkdf2 pbkdf2 pcidump pci play probe reboot serial terminal test usb_keyboard usbms uhci gcry_arcfour gcry_blowfish gcry_camellia gcry_cast5 gcry_crc gcry_des gcry_dsa gcry_idea gcry_md4 gcry_md5 gcry_rfc2268 gcry_rijndael gcry_rmd160 gcry_rsa gcry_seed gcry_serpent gcry_sha1 gcry_sha256 gcry_sha512 gcry_tiger gcry_twofish gcry_whirlpool hdparm regexp spkmodem syslinuxcfg usb verify videoinfo videotest xfs btrfs zfs sfs romfs reiserfs nilfs2 minix_be minix3_be minix3 minix2_be minix2 minix jfs hfsplus hfs bfs afs affs gfxmenu gfxterm_background gfxterm_menu jpeg png tga pata"

# Generate the grub.elf
$grubdir/grub-mkstandalone \
  --grub-mkimage=$grubdir/grub-mkimage \
  -O i386-coreboot \
  -o grub_vesafb.elf \
  -d $grubdir/grub-core/ \
  --fonts= --themes= --locales=  \
  --modules="$grub_modules" \
  --install-modules="$grub_install_modules" \
  /boot/grub/grub.cfg="../../../resources/grub/config/grub_memdisk.cfg" \
  /background.jpg="../../../resources/grub/background/background.jpg" \
  /dejavusansmono.pf2="../../../resources/grub/font/dejavusansmono.pf2" \
  $(./grub_memdisk_keymap) \
