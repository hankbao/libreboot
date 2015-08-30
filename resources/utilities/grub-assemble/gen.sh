#!/bin/bash
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

# The original grub-assemble that this tool is based on:
# https://gitorious.org/gnutoo-for-coreboot/grub-assemble

[ "x${DEBUG+set}" = 'xset' ] && set -v
set -u -e

if (( $# != 1 )); then
	printf "Usage: ./gen.sh mode\n"
	printf "Example: ./gen.sh vesafb\n"
	printf "Example: ./gen.sh txtmode\n"
	printf "You need to specify exactly 1 argument\n"
	exit 1
fi

# This is where GRUB is expected to be (outside of the grub-assemble, instead in main checkout)
grubdir="../../../grub"

source "modules.conf"

list_keymaps () {
	for keylayout in keymap/original/*
	do
		keymap="${keylayout##*/}"
		printf "/boot/grub/layouts/%s.gkb=keymap/%s.gkb " "${keymap}" "${keymap}"
	done
}

printf "Creating GRUB ELF executable for configuration '%s'\n" "${1}"
if [ "${1}" = "vesafb" ]
then
	# Generate the grub.elf (vesafb)
	$grubdir/grub-mkstandalone \
	  --grub-mkimage="${grubdir}/grub-mkimage" \
	  -O i386-coreboot \
	  -o "grub_vesafb.elf" \
	  -d "${grubdir}/grub-core/" \
	  --fonts= --themes= --locales=  \
	  --modules="${grub_modules}" \
	  --install-modules="${grub_install_modules}" \
	  /boot/grub/grub.cfg="../../../resources/grub/config/grub_memdisk.cfg" \
	  /dejavusansmono.pf2="../../../resources/grub/font/dejavusansmono.pf2" \
	  $(list_keymaps)
elif [ "${1}" = "txtmode" ]
then
	# Generate the grub.elf (txtmode)
	"${grubdir}/grub-mkstandalone" \
	  --grub-mkimage="${grubdir}/grub-mkimage" \
	  -O i386-coreboot \
	  -o "grub_txtmode.elf" \
	  -d "${grubdir}/grub-core/" \
	  --fonts= --themes= --locales=  \
	  --modules="${grub_modules}" \
	  --install-modules="${grub_install_modules}" \
	  /boot/grub/grub.cfg="../../../resources/grub/config/grub_memdisk.cfg" \
	  /memtest="../../../memtest86plus/memtest" \
	  $(list_keymaps)
else
	printf "grub-assemble gen.sh: invalid mode '%s'\n" "${1}"
	exit 1
fi
printf "\n\n"

