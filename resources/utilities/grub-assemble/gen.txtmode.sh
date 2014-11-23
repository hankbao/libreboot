#!/bin/bash
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

set -u -e -v

# TODO: Fail (and clean up) if GRUB isn't actually built. Error checking.

# This is where GRUB is expected to be (outside of the grub-assemble, instead in main checkout)
grubdir="../../../grub"

source "modules.conf"

# Generate the grub.elf
$grubdir/grub-mkstandalone \
  --grub-mkimage=$grubdir/grub-mkimage \
  -O i386-coreboot \
  -o grub_txtmode.elf \
  -d $grubdir/grub-core/ \
  --fonts= --themes= --locales=  \
  --modules="$grub_modules" \
  --install-modules="$grub_install_modules" \
  /boot/grub/grub.cfg="../../../resources/grub/config/grub_memdisk.cfg" \
  /memtest="../../../memtest86+-5.01/memtest" \
  /invaders.exec="../../../grubinvaders/invaders.exec" \
  $(./grub_memdisk_keymap) \

