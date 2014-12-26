/*
 *  descriptor/descriptor.c
 *  This file is part of the ich9deblob utility from the libreboot project
 *
 *  Copyright (C) 2014 Steve Shenton <sgsit@libreboot.org>
 *                     Francis Rowe <info@gluglug.org.uk>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Provide descriptor related functions.
 */

/* structs describing the data in descriptor region */
#include "descriptor.h"

/*
 * ---------------------------------------------------------------------
 * Descriptor functions
 * ---------------------------------------------------------------------
 */

/* 
 * Modify the flash descriptor, to remove the ME/AMT, and disable all other regions
 * Only Flash Descriptor, Gbe and BIOS regions (BIOS region fills factoryRomSize-12k) are left.
 * Tested on ThinkPad X200 and X200S. X200T and other GM45 targets may also work.
 * Also described in docs/hcl/x200_remove_me.html
 */
struct DESCRIPTORREGIONRECORD deblobbedDescriptorStructFromFactory(struct DESCRIPTORREGIONRECORD factoryDescriptorStruct, unsigned int factoryRomSize)
{
	struct DESCRIPTORREGIONRECORD deblobbedDescriptorStruct;
	memcpy(&deblobbedDescriptorStruct, &factoryDescriptorStruct, DESCRIPTORREGIONSIZE);

	/* 
	 * set number of regions from 4 -> 2 (0 based, so 4 means 5 and 2
	 * means 3. We want 3 regions: descriptor, gbe and bios, in that order)
	 */
	deblobbedDescriptorStruct.flMaps.flMap0.NR = 2;

	/* 
	 * make descriptor writable from OS. This is that the user can run:
	 * sudo ./flashrom -p internal:laptop=force_I_want_a_brick 
	 * from the OS, without relying an an external SPI flasher, while
	 * being able to write to the descriptor region (locked by default,
	 * until making the change below):
	 */
	deblobbedDescriptorStruct.masterAccessSection.flMstr1.fdRegionWriteAccess = 1;

	/* relocate BIOS region and increase size to fill image */
	deblobbedDescriptorStruct.regionSection.flReg1.BASE = 3; // 3<<FLREGIONBITSHIFT is 12KiB, which is where BIOS region is to begin (after descriptor and gbe)
	deblobbedDescriptorStruct.regionSection.flReg1.LIMIT = ((factoryRomSize >> FLREGIONBITSHIFT) - 1);
	/*
	 * ^ for example, 8MB ROM, that's 8388608 bytes.
	 * ^ 8388608>>FLREGIONBITSHIFT (or 8388608/4096) = 2048 bytes
	 * 2048 - 1 = 2047 bytes. 
	 * This defines where the final 0x1000 (4KiB) page starts in the flash chip, because the hardware does:
	 * 2047<<FLREGIONBITSHIFT (or 2047*4096) = 8384512 bytes, or 7FF000 bytes
	 * (it can't be 0x7FFFFF because of limited number of bits)
	 */

	/* set ME region size to 0 - the ME is a blob, we don't want it in libreboot */
	deblobbedDescriptorStruct.regionSection.flReg2.BASE = 0x1FFF; // setting 1FFF means setting size to 0. 1FFF<<FLREGIONBITSHIFT is outside of the ROM image (8MB) size?
	/* ^ datasheet says to set this to 1FFF, but FFF was previously used and also worked. */
	deblobbedDescriptorStruct.regionSection.flReg2.LIMIT = 0;
	/*
	 * ^ 0<<FLREGIONBITSHIFT=0, so basically, the size is 0,
	 * ^ and the base (1FFF>>FLREGIONBITSHIFT) is well outside the higher 8MB range. 
	 */
	
	/* relocate Gbe region to begin at 4KiB (immediately after the flash descriptor) */
	deblobbedDescriptorStruct.regionSection.flReg3.BASE = 1; // 1<<FLREGIONBITSHIFT is 4096, which is where the Gbe region is to begin (after the descriptor)
	deblobbedDescriptorStruct.regionSection.flReg3.LIMIT = 2;
	/*
	 * ^ 2<<FLREGIONBITSHIFT=8192 bytes. So we are set it to size 8KiB after the first 4KiB in the flash chip.
	 */

	/* set Platform region size to 0 - another blob that we don't want */
	deblobbedDescriptorStruct.regionSection.flReg4.BASE = 0x1FFF; // setting 1FFF means setting size to 0. 1FFF<<FLREGIONBITSHIFT is outside of the ROM image (8MB) size?
	/* ^ datasheet says to set this to 1FFF, but FFF was previously used and also worked. */
	deblobbedDescriptorStruct.regionSection.flReg4.LIMIT = 0;
	/*
	 * ^ 0<<FLREGIONBITSHIFT=0, so basically, the size is 0, 
	 * and the base (1FFF>>FLREGIONBITSHIFT) is well outside the higher 8MB range.
	 */

	/* disable ME in ICHSTRAP0 - the ME is a blob, we don't want it in libreboot */
	deblobbedDescriptorStruct.ichStraps.ichStrap0.meDisable = 1;

	/* disable ME and TPM in MCHSTRAP0 */
	deblobbedDescriptorStruct.mchStraps.mchStrap0.meDisable = 1; // ME is a blob. not wanted in libreboot.
	deblobbedDescriptorStruct.mchStraps.mchStrap0.tpmDisable = 1; // not wanted in libreboot

	/* 
	 * disable ME, apart from chipset bugfixes (ME region should first be re-enabled above)
	 * This is sort of like the CPU microcode updates, but for the chipset
	 * (commented out below here, since blobs go against libreboot's purpose,
	 * but may be interesting for others)
	 * deblobbedDescriptorStruct.mchStraps.mchStrap0.meAlternateDisable = 1;
	 */
	
	return deblobbedDescriptorStruct;
}

/*
 * ---------------------------------------------------------------------
 * Debugging functions:
 * ---------------------------------------------------------------------
 */

/*
 * show debugging info: descriptor region boundaries, in a 4KB struct.
 */
void printDescriptorRegionLocations(struct DESCRIPTORREGIONRECORD descriptorStruct, char* romName)
{
	printf("\n");
	
	/* Descriptor region */
	printf(
		"%s: Descriptor start block: %08x ; Descriptor end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg0.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg0.LIMIT << FLREGIONBITSHIFT
	);
	
	/* BIOS region */
	printf(
		"%s: BIOS start block: %08x ; BIOS end block: %08x\n", 
		romName,
		descriptorStruct.regionSection.flReg1.BASE << FLREGIONBITSHIFT, 
		descriptorStruct.regionSection.flReg1.LIMIT << FLREGIONBITSHIFT
	);
	
	/* ME region */
	printf(
		"%s: ME start block: %08x ; ME end block: %08x\n", 
		romName,
		descriptorStruct.regionSection.flReg2.BASE << FLREGIONBITSHIFT, 
		descriptorStruct.regionSection.flReg2.LIMIT << FLREGIONBITSHIFT
	);
	
	/* GBe region */
	printf(
		"%s: GBe start block: %08x ; GBe end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg3.LIMIT << FLREGIONBITSHIFT
	);
	
	/* Platform region */
	printf(
		"%s: Platform start block: %08x ; Platform end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg4.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg4.LIMIT << FLREGIONBITSHIFT
	);
	
	return;
}
