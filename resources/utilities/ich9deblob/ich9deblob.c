/*
 *  main.c
 *  This file is part of the ich9deblob utility from the libreboot project
 * 
 * Purpose: disable and remove the ME from ich9m/gm45 machines in coreboot.
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
 * Read a factory.rom dump (ich9m/gm45 machines) and 
 * modify the flash descriptor to remove all regions except descriptor,
 * Gbe and BIOS. Set BIOS region to full size of the ROM image (after
 * the flash descriptor and gbe). Basically, deblob the descriptor.
 * 
 * This will will generate a concatenated descriptor+gbe dump suitable
 * for use in libreboot. Currently tested: ThinkPad X200 (coreboot/libreboot)
 */
 
// See docs/hcl/x200_remove_me.html for info plus links to datasheet (also linked below)

// Info about flash descriptor (read page 850 onwards):
// * http://www.intel.co.uk/content/dam/doc/datasheet/io-controller-hub-9-datasheet.pdf

// Info about Gbe region (read whole datasheet):
// * http://www.intel.co.uk/content/dam/doc/application-note/i-o-controller-hub-9m-82567lf-lm-v-nvm-map-appl-note.pdf
// * https://communities.intel.com/community/wired/blog/2010/10/14/how-to-basic-eeprom-checksums

#include <stdio.h>
#include <string.h>
#include "ich9desc.c" // structs describing what's in the descriptor region
#include "ich9gbe.c" // structs describing what's in the gbe region

#define DESCRIPTORREGIONSIZE 0x1000 // 4096 bytes / 4KiB
#define GBEREGIONSIZE 0x2000 // 8192 bytes / 8KiB
// These will have a modified descriptor+gbe based on what's in the factory.rom
// These will be joined into a single 12KiB buffer (descriptor, then gbe) and saved to a file
// NOTE: The GBE region of 8K is actually 2x 4K regions in a single region; both 4K blocks can be identical (and by default, are)
// The 2nd one is a "backup", but we don't know when it's used. perhaps it's used when the checksum on the first one does not match?

// gbe checksum related functions
unsigned short GetChecksum(char* buffer, unsigned short desiredValue, char isBackup); // for GBe region (checksum calculation)
unsigned short GetRegionWord(int i, char* buffer); // used for getting each word needed to calculate said checksum

int main(int argc, char *argv[])
{
	
	// descriptor region. Will have actual descriptor mapped to it (from the factory.rom dump)
	struct DESCRIPTORREGIONRECORD descriptorRegion;
	unsigned int descriptorRegionStructSize = sizeof(descriptorRegion);
	// check compiler bit-packs in a compatible way basically, it is expected that this code will be used on x86
	if (DESCRIPTORREGIONSIZE != descriptorRegionStructSize){
		printf("\nerror: compiler incompatibility: descriptor struct length is %i bytes (should be %i)\n", descriptorRegionStructSize, DESCRIPTORREGIONSIZE);
		return 1;
	}

	// supplied by user, dumped from their machine before flashing libreboot
	char* factoryRomFilename = "factory.rom";
	// name of the file that this utility will create (deblobbed descriptor+gbe)
	char* deblobbedDescriptorFilename = "deblobbed_descriptor.bin";

	// Open factory.rom, needed for extracting descriptor and gbe
	// -----------------------------------------------
	FILE* fp = NULL;
	fp = fopen(factoryRomFilename, "rb"); // open factory.rom
	if (NULL == fp)
	{
		printf("\nerror: could not open factory.rom\n");
		return 1;
	}
	printf("\nfactory.rom opened successfully\n");
	// -----------------------------------------------
	
	// Get the descriptor region dump from the factory.rom

	// Create empty descriptor buffer (populated below)
	char descriptorBuffer[DESCRIPTORREGIONSIZE];
	// Extract the descriptor region from the factory.rom dump
	// (goes in descriptorBuffer variable)
	unsigned int readLen;
	readLen = fread(descriptorBuffer, sizeof(char), DESCRIPTORREGIONSIZE, fp);
	if (DESCRIPTORREGIONSIZE != readLen) // 
	{
		printf("\nerror: could not read descriptor from factory.rom (%i) bytes read\n", readLen);
		return 1;
	}
	printf("\ndescriptor region read successfully\n");
	// copy descriptor buffer into descriptor struct memory
	// descriptorRegion is an instance of a struct that actually
	// defines the locations of all these variables in the descriptor,
	// as defined in the datasheets. This allows us to map the extracted
	// descriptor over the struct so that it can then be modified
	// for libreboot's purpose
	memcpy(&descriptorRegion, &descriptorBuffer, DESCRIPTORREGIONSIZE);
	
	// -----------------------------------------------------------------------------------------------
	
	// Get the gbe region dump from the factory.rom

	// get original GBe region location
	// (it will be moved to the beginning of the flash, after the descriptor region)
	unsigned int flRegionBitShift = 12;// bits 12-24 are represented.
	// note for example, gbeRegionLocation is set to <<12 of actual address (in C). this is how the addresses
	// are stored in the descriptor.
	unsigned int gbeRegionLocation = descriptorRegion.regionSection.flReg3.BASE << flRegionBitShift;

	// Set offset so that we can read the data from
	// the gbe region
	fseek(fp, gbeRegionLocation, SEEK_SET);
	// data will go in here
	char gbeBuffer[GBEREGIONSIZE];
	// Read the gbe data from the factory.rom and put it in gbeBuffer
	readLen = fread(gbeBuffer, sizeof(char), GBEREGIONSIZE, fp);
	if (GBEREGIONSIZE != readLen)
	{
		printf("\nerror: could not read GBe region from factory.rom (%i) bytes read\n", readLen);
		return 1;
	}

	// -----------------------------------------------------------------------------------------------

	// Get size of ROM image
	// This is needed for relocating the BIOS region (per descriptor)
	fseek(fp, 0L, SEEK_END);
	int romSize = ftell(fp);

	printf("\nfactory.rom size: [%i] bytes\n", romSize);

	fclose(fp);
	
	// -----------------------------------------------------------------------------------------------

	// debugging
	printf("\nOriginal Descriptor start block: %08x ; Descriptor end block: %08x\n", descriptorRegion.regionSection.flReg0.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg0.LIMIT << flRegionBitShift);
	printf("Original BIOS start block: %08x ; BIOS end block: %08x\n", descriptorRegion.regionSection.flReg1.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg1.LIMIT << flRegionBitShift);
	printf("Original ME start block: %08x ; ME end block: %08x\n", descriptorRegion.regionSection.flReg2.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg2.LIMIT << flRegionBitShift);
	printf("Original GBe start block: %08x ; GBe end block: %08x\n", gbeRegionLocation, descriptorRegion.regionSection.flReg3.LIMIT << flRegionBitShift);

	// Now we need to modify the descriptor so that the ME can be excluded
	// from the final ROM image (libreboot one) after adding the modified
	// descriptor+gbe. Refer to libreboot docs for details: docs/hcl/x200_remove_me.html

	// set number of regions from 4 -> 2 (0 based, so 4 means 5 and 2
	// means 3. We want 3 regions: descriptor, gbe and bios, in that order)
	descriptorRegion.flMaps.flMap0.NR = 2;

	// make descriptor writable from OS. This is that the user can run:
	// sudo ./flashrom -p internal:laptop=force_I_want_a_brick 
	// from the OS, without relying an an external SPI flasher, while
	// being able to write to the descriptor region (locked by default,
	// until making the change below):
	descriptorRegion.masterAccessSection.flMstr1.fdRegionWriteAccess = 1;

	// relocate BIOS region and increase size to fill image
	descriptorRegion.regionSection.flReg1.BASE = 3; // 3<<12 is 12KiB, which is where BIOS region is to begin (after descriptor and gbe)
	descriptorRegion.regionSection.flReg1.LIMIT = ((romSize >> flRegionBitShift) - 1);
	// ^ for example, 8MB ROM, that's 8388608 bytes.
	// ^ 8388608>>12 (or 8388608/4096) = 2048 bytes
	// 2048 - 1 = 2047 bytes. 
	// This defines where the final 0x1000 (4KiB) page starts in the flash chip, because the hardware does:
	// 2047<<12 (or 2047*4096) = 8384512 bytes, or 7FF000 bytes
	// (it can't be 0x7FFFFF because of limited number of bits)

	// set ME region size to 0 - the ME is a blob, we don't want it in libreboot
	descriptorRegion.regionSection.flReg2.BASE = 0x1FFF; // setting 1FFF means setting size to 0. 1FFF<<12 is outside of the ROM image (8MB) size?
	// ^ datasheet says to set this to 1FFF, but FFF was previously used and also worked.
	descriptorRegion.regionSection.flReg2.LIMIT = 0;
	// ^ 0<<12=0, so basically, the size is 0, and the base (1FFF>>12) is well outside the higher 8MB range. 
	
	// relocate Gbe region to begin at 4KiB (immediately after the flash descriptor)
	descriptorRegion.regionSection.flReg3.BASE = 1; // 1<<12 is 4096, which is where the Gbe region is to begin (after the descriptor)
	descriptorRegion.regionSection.flReg3.LIMIT = 2;
	// ^ 2<<12=8192 bytes. So we are set it to size 8KiB after the first 4KiB in the flash chip.

	// set Platform region size to 0 - another blob that we don't want
	descriptorRegion.regionSection.flReg4.BASE = 0x1FFF; // setting 1FFF means setting size to 0. 1FFF<<12 is outside of the ROM image (8MB) size?
	// ^ datasheet says to set this to 1FFF, but FFF was previously used and also worked.
	descriptorRegion.regionSection.flReg4.LIMIT = 0;
	// ^ 0<<12=0, so basically, the size is 0, and the base (1FFF>>12) is well outside the higher 8MB range.

	// disable ME in ICHSTRAP0 - the ME is a blob, we don't want it in libreboot
	descriptorRegion.ichStraps.ichStrap0.meDisable = 1;

	// disable ME and TPM in MCHSTRAP0
	descriptorRegion.mchStraps.mchStrap0.meDisable = 1; // ME is a blob. not wanted in libreboot.
	descriptorRegion.mchStraps.mchStrap0.tpmDisable = 1; // not wanted in libreboot

	// disable ME, apart from chipset bugfixes (ME region should first be re-enabled above)
	// This is sort of like the CPU microcode updates, but for the chipset
	// (commented out below here, since blobs go against libreboot's purpose,
	// but may be interesting for others)
	// descriptorRegion.mchStraps.mchStrap0.meAlternateDisable = 1;

	// debugging
	printf("\nRelocated Descriptor start block: %08x ; Descriptor end block: %08x\n", descriptorRegion.regionSection.flReg0.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg0.LIMIT << flRegionBitShift);
	printf("Relocated BIOS start block: %08x ; BIOS end block: %08x\n", descriptorRegion.regionSection.flReg1.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg1.LIMIT << flRegionBitShift);
	printf("Relocated ME start block: %08x ; ME end block: %08x\n", descriptorRegion.regionSection.flReg2.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg2.LIMIT << flRegionBitShift);
	printf("Relocated GBe start block: %08x ; GBe end block: %08x\n", descriptorRegion.regionSection.flReg3.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg3.LIMIT << flRegionBitShift);

	// ----------------------------------------------------------------------------------------------------------------

	// Convert the descriptor and gbe back to byte arrays, so that they
	// can more easily be written to files:
	// deblobbed descriptor region
	char deblobbedDescriptorBuffer[DESCRIPTORREGIONSIZE];
	memcpy(&deblobbedDescriptorBuffer, &descriptorRegion, DESCRIPTORREGIONSIZE);

	// delete old file before continuing
	remove(deblobbedDescriptorFilename);
	// open new file for writing the deblobbed descriptor+gbe
	fp = fopen(deblobbedDescriptorFilename, "ab");

	// write the descriptor region into the first part
	if (DESCRIPTORREGIONSIZE != fwrite(deblobbedDescriptorBuffer, sizeof(char), DESCRIPTORREGIONSIZE, fp))
	{
		printf("\nerror: writing descriptor region failed\n");
		return 1;
	}

	// add gbe to the end of the file
	if (GBEREGIONSIZE != fwrite(gbeBuffer, sizeof(char), GBEREGIONSIZE, fp))
	{
		printf("\nerror: writing GBe region failed\n");
		return 1;
	}

	fclose(fp);

	printf("\ndeblobbed descriptor successfully created: deblobbed_descriptor.bin \n");

	// -------------------------------------------------------------------------------------

	// calculate the 0x3F'th 16-bit uint to make the desired final checksum for GBe
	// observed values (from actual factory.rom dumps) 0xBABA 0x3ABA 0x34BA. spec defined as 0xBABA.
	// theoretically, this could be any l33t speak variation of BABA, eg 3434 or BA34, and so on, but this is untested. so far.
	// 40BA was also observed in another factory dump for another X200 - this is l33t speak for aoba, not baba.... 
	// maybe only the 8 least significant bits are checked? or something deeper than that
	// it will need to be tested if those gbe regions that use something other than baba
	// will also work with the checksum changed to match baba (per datasheets)
	unsigned short gbeCalculatedChecksum = GetChecksum(gbeBuffer, 0xBABA, 0);
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	unsigned short gbeChecksum = GetRegionWord(0x3F, gbeBuffer); // from the original factory.rom
	printf("\noriginal Gbe (main): calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);
	
	// same as above, but for 2nd region ("backup") in gbe
	gbeCalculatedChecksum = GetChecksum(gbeBuffer, 0xBABA, 1);
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	gbeChecksum = GetRegionWord(0x3F+(0x1000>>1), gbeBuffer);
	printf("original Gbe (backup) calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);

	return 0;
}

// checksum calculation for gbe region (algorithm based on datasheet)
unsigned short GetChecksum(char* regionData, unsigned short desiredValue, char isBackup)
{
	unsigned short regionWord;
	unsigned short checksum = 0;

	// if isBackup is true, use 2nd gbe region ("backup" region)
	unsigned short offset = 0;
	if (isBackup) offset = 0x1000>>1; // this function uses *word* not *byte* indexes.

	int i;
	for (i = 0; i < 0x3F; i++) {
		regionWord = GetRegionWord(i+offset, regionData);
		checksum += regionWord;
	}
	checksum = desiredValue - checksum;
	return checksum;
}
// Read a 16-bit unsigned int from a supplied region buffer
unsigned short GetRegionWord(int index, char* regionData)
{
	return *((unsigned short*)(regionData + (index * 2)));
}
