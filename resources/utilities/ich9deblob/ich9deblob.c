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

#define DESCRIPTORREGIONSIZE 0x1000 // 4 KiB
#define GBEREGIONSIZE 0x2000 // 8 KiB
// These will have a modified descriptor+gbe based on what's in the factory.rom
// These will be joined into a single 12KiB buffer (descriptor, then gbe) and saved to a file
// NOTE: The GBE region of 8K is actually 2x 4K regions in a single region; both 4K blocks can be identical (and by default, are)
// The 2nd one is a "backup", but we don't know when it's used. perhaps it's used when the checksum on the first one does not match?

// Related to the flash descriptor
#define FLREGIONBITSHIFT 0xC // bits 12(0xC)-24(0x18) are represented for words found in the flash descriptor
												 // To manipulate these easily in C, we shift them by FLREGIONBITSHIFT and then shift them back when done

// gbe checksum related functions
unsigned short gbeGetChecksumFrom4kStruct(struct GBEREGIONRECORD_4K gbeStruct4k, unsigned short desiredValue);
unsigned short gbeGetChecksumFrom8kBuffer(char* buffer, unsigned short desiredValue, char isBackup); // for GBe region (checksum calculation)
unsigned short gbeGetRegionWordFrom8kBuffer(int i, char* buffer); // used for getting each word needed to calculate said checksum
struct DESCRIPTORREGIONRECORD deblobbedFromFactory(struct DESCRIPTORREGIONRECORD factoryDescriptorStruct, int romSize);
int structSizesIncorrect(struct DESCRIPTORREGIONRECORD descriptorDummy, struct GBEREGIONRECORD_8K gbe8kDummy);
int systemIsBigEndian();

int main(int argc, char *argv[])
{
	// descriptor region. Will have actual descriptor mapped to it (from the factory.rom dump)
	// and then it will be modified (deblobbed) to remove the ME/AMT
	struct DESCRIPTORREGIONRECORD factoryDescriptorStruct;
	struct DESCRIPTORREGIONRECORD deblobbedDescriptorStruct;
	// gbe region. Well have actual gbe buffer mapped to it (from the factory.rom dump)
	// and then it will be modified to correct the main region
	struct GBEREGIONRECORD_8K factoryGbeStruct8k;
	struct GBEREGIONRECORD_8K deblobbedGbeStruct8k;
	
	// Compatibility checks. This version of ich9deblob is not yet porable.
	if (structSizesIncorrect(factoryDescriptorStruct, factoryGbeStruct8k)) return 1;
	if (systemIsBigEndian()) return 1;
	
	// -----------------------------------------------------------------------------------------------
	
	// files that this utility will handle

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
	char factoryDescriptorBuffer[DESCRIPTORREGIONSIZE];
	// Extract the descriptor region from the factory.rom dump
	// (goes in factoryDescriptorBuffer variable)
	unsigned int readLen;
	readLen = fread(factoryDescriptorBuffer, sizeof(char), DESCRIPTORREGIONSIZE, fp);
	if (DESCRIPTORREGIONSIZE != readLen) // 
	{
		printf("\nerror: could not read descriptor from factory.rom (%i) bytes read\n", readLen);
		return 1;
	}
	printf("\ndescriptor region read successfully\n");
	// copy descriptor buffer into descriptor struct memory
	// factoryDescriptorStruct is an instance of a struct that actually
	// defines the locations of all these variables in the descriptor,
	// as defined in the datasheets. This allows us to map the extracted
	// descriptor over the struct so that it can then be modified
	// for libreboot's purpose
	memcpy(&factoryDescriptorStruct, &factoryDescriptorBuffer, DESCRIPTORREGIONSIZE);
	// ^ the above is just for reference if needed. The modifications will be made here:
	memcpy(&deblobbedDescriptorStruct, &factoryDescriptorBuffer, DESCRIPTORREGIONSIZE);
	
	// -----------------------------------------------------------------------------------------------
	
	// Get the gbe region dump from the factory.rom

	// get original GBe region location
	// (it will be moved to the beginning of the flash, after the descriptor region)
	// note for example, factoryGbeRegionLocation is set to <<FLREGIONBITSHIFT of actual address (in C). this is how the addresses
	// are stored in the descriptor.
	unsigned int factoryGbeRegionLocation = factoryDescriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT;

	// Set offset so that we can read the data from
	// the gbe region
	fseek(fp, factoryGbeRegionLocation, SEEK_SET);
	// data will go in here
	char factoryGbeBuffer8k[GBEREGIONSIZE];
	// Read the gbe data from the factory.rom and put it in factoryGbeBuffer8k
	readLen = fread(factoryGbeBuffer8k, sizeof(char), GBEREGIONSIZE, fp);
	if (GBEREGIONSIZE != readLen)
	{
		printf("\nerror: could not read GBe region from factory.rom (%i) bytes read\n", readLen);
		return 1;
	}
	printf("\ngbe (8KiB) region read successfully\n");
	// copy gbe buffer into gbe struct memory
	// factoryGbeStruct8k is an instance of a struct that actually
	// defines the locations of all these variables in the gbe,
	// as defined in the datasheets. This allows us to map the extracted
	// gbe over the struct so that it can then be modified
	// for libreboot's purpose
	memcpy(&factoryGbeStruct8k, &factoryGbeBuffer8k, GBEREGIONSIZE);
	// the original factoryGbeStruct8k is only reference. Changes go here:
	memcpy(&deblobbedGbeStruct8k, &factoryGbeBuffer8k, GBEREGIONSIZE);

	// -----------------------------------------------------------------------------------------------

	// Get size of ROM image
	// This is needed for relocating the BIOS region (per descriptor)
	fseek(fp, 0L, SEEK_END);
	int romSize = ftell(fp);

	printf("\nfactory.rom size: [%i] bytes\n", romSize);

	fclose(fp);
	
	// -----------------------------------------------------------------------------------------------

	// debugging
	printf("\nOriginal (factory.rom) Descriptor start block: %08x ; Descriptor end block: %08x\n", factoryDescriptorStruct.regionSection.flReg0.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg0.LIMIT << FLREGIONBITSHIFT);
	printf("Original (factory.rom) BIOS start block: %08x ; BIOS end block: %08x\n", factoryDescriptorStruct.regionSection.flReg1.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg1.LIMIT << FLREGIONBITSHIFT);
	printf("Original (factory.rom) ME start block: %08x ; ME end block: %08x\n", factoryDescriptorStruct.regionSection.flReg2.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg2.LIMIT << FLREGIONBITSHIFT);
	printf("Original (factory.rom) GBe start block: %08x ; GBe end block: %08x\n", factoryGbeRegionLocation, factoryDescriptorStruct.regionSection.flReg3.LIMIT << FLREGIONBITSHIFT);

	// Disable the ME and Platform regions. Put Gbe at the beginning (after descriptor). 
	// Also, extend the BIOS region to fill the ROM image (after descriptor+gbe).
	deblobbedDescriptorStruct = deblobbedFromFactory(deblobbedDescriptorStruct, romSize);

	// debugging
	printf("\nRelocated (libreboot.rom) Descriptor start block: %08x ; Descriptor end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg0.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg0.LIMIT << FLREGIONBITSHIFT);
	printf("Relocated (libreboot.rom) BIOS start block: %08x ; BIOS end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg1.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg1.LIMIT << FLREGIONBITSHIFT);
	printf("Relocated (libreboot.rom) ME start block: %08x ; ME end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg2.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg2.LIMIT << FLREGIONBITSHIFT);
	printf("Relocated (libreboot.rom) GBe start block: %08x ; GBe end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg3.LIMIT << FLREGIONBITSHIFT);

	// ----------------------------------------------------------------------------------------------------------------

	// Correct the main gbe region. By default, the X200 (as shipped from Lenovo) comes
	// with a broken main gbe region, where the backup gbe region is used instead. Modify
	// the descriptor so that the main region is usable.
	
	deblobbedGbeStruct8k.backup.checkSum = gbeGetChecksumFrom4kStruct(deblobbedGbeStruct8k.backup, 0xBABA);
	memcpy(&deblobbedGbeStruct8k.main, &deblobbedGbeStruct8k.backup, GBEREGIONSIZE>>1);

	// ----------------------------------------------------------------------------------------------------------------

	// Convert the descriptor and gbe back to byte arrays, so that they
	// can more easily be written to files:
	// deblobbed descriptor region
	char deblobbedDescriptorBuffer[DESCRIPTORREGIONSIZE];
	memcpy(&deblobbedDescriptorBuffer, &deblobbedDescriptorStruct, DESCRIPTORREGIONSIZE);
	
	char deblobbedGbeBuffer8k[GBEREGIONSIZE];
	memcpy(&deblobbedGbeBuffer8k, &deblobbedGbeStruct8k, GBEREGIONSIZE);

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
	if (GBEREGIONSIZE != fwrite(deblobbedGbeBuffer8k, sizeof(char), GBEREGIONSIZE, fp))
	{
		printf("\nerror: writing GBe region failed\n");
		return 1;
	}

	fclose(fp);

	printf("\ndeblobbed descriptor successfully created: deblobbed_descriptor.bin \n");

	// -------------------------------------------------------------------------------------

	// calculate the 0x3F'th 16-bit uint to make the desired final checksum for GBe
	// observed checksum matches (from X200 factory.rom dumps) on main: 0x3ABA 0x34BA 0x40BA. spec defined as 0xBABA.
	// X200 ships with a broken main gbe region by default (invalid checksum, and more)
	// The "backup" gbe regions on these machines are correct, though, and is what the machines default to
	// For libreboot's purpose, we can do much better than that by fixing the main one... below is only debugging
	
	unsigned short gbeCalculatedChecksum;
	unsigned short gbeChecksum;
	
	gbeCalculatedChecksum = gbeGetChecksumFrom4kStruct(factoryGbeStruct8k.main, 0xBABA);
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	gbeChecksum = factoryGbeStruct8k.main.checkSum; // for the libreboot.rom image
	printf("\nfactory Gbe (main): calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);
	// same as above, but for 2nd region ("backup") in gbe
	gbeCalculatedChecksum = gbeGetChecksumFrom4kStruct(factoryGbeStruct8k.backup, 0xBABA); // factory.rom
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	gbeChecksum = factoryGbeStruct8k.backup.checkSum; // from the factory.rom
	printf("factory Gbe (backup) calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);
	
	// Do the same as above, for the deblobbed gbe region:
	gbeCalculatedChecksum = gbeGetChecksumFrom4kStruct(deblobbedGbeStruct8k.main, 0xBABA);
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	gbeChecksum = deblobbedGbeStruct8k.main.checkSum; // for the libreboot.rom image
	printf("\ndeblobbed Gbe (main): calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);
	// same as above, but for 2nd region ("backup") in gbe
	gbeCalculatedChecksum = gbeGetChecksumFrom4kStruct(deblobbedGbeStruct8k.backup, 0xBABA);
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	gbeChecksum = deblobbedGbeStruct8k.backup.checkSum; // for the libreboot.rom image
	printf("deblobbed Gbe (backup) calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);

	printf("\nNow do: dd if=deblobbed_descriptor.bin of=libreboot.rom bs=1 count=12k conv=notrunc");
	printf("\n(in other words, add the modified descriptor+gbe to your ROM image)\n");

	return 0;
}

// Basically, this should only return true on non-x86 machines
int structSizesIncorrect(struct DESCRIPTORREGIONRECORD descriptorDummy, struct GBEREGIONRECORD_8K gbe8kDummy) {
	unsigned int descriptorRegionStructSize = sizeof(descriptorDummy);
	unsigned int gbeRegion8kStructSize = sizeof(gbe8kDummy);
	// check compiler bit-packs in a compatible way. basically, it is expected that this code will be used on x86
	if (DESCRIPTORREGIONSIZE != descriptorRegionStructSize){
		printf("\nerror: compiler incompatibility: descriptor struct length is %i bytes (should be %i)\n", descriptorRegionStructSize, DESCRIPTORREGIONSIZE);
		return 1;
	}
	if (GBEREGIONSIZE != gbeRegion8kStructSize){
		printf("\nerror: compiler incompatibility: gbe struct length is %i bytes (should be %i)\n", gbeRegion8kStructSize, GBEREGIONSIZE);
		return 1;
	}
	return 0;
}

int systemIsBigEndian() {
	// endianness check. big endian forced to fail
	unsigned short steak = 0xBEEF;
	unsigned char *grill = (char*)&steak;
	if (*grill==0xBE) {
		printf("\nunsigned short 0xBEEF: first byte should be EF, but it's BE. Your system is big endian, and unsupported (only little endian is tested)\n");
		return 1;
	}
	return 0;
}

// Modify the flash descriptor, to remove the ME/AMT, and disable all other regions
// Only Flash Descriptor, Gbe and BIOS regions (BIOS region fills romSize-12k) are left.
// Tested on ThinkPad X200 and X200S. X200T and other GM45 targets may also work.
struct DESCRIPTORREGIONRECORD deblobbedFromFactory(struct DESCRIPTORREGIONRECORD factoryDescriptorStruct, int romSize)
{
	struct DESCRIPTORREGIONRECORD deblobbedDescriptorStruct;
	memcpy(&deblobbedDescriptorStruct, &factoryDescriptorStruct, DESCRIPTORREGIONSIZE);
	
	// Now we need to modify the descriptor so that the ME can be excluded
	// from the final ROM image (libreboot one) after adding the modified
	// descriptor+gbe. Refer to libreboot docs for details: docs/hcl/x200_remove_me.html

	// set number of regions from 4 -> 2 (0 based, so 4 means 5 and 2
	// means 3. We want 3 regions: descriptor, gbe and bios, in that order)
	deblobbedDescriptorStruct.flMaps.flMap0.NR = 2;

	// make descriptor writable from OS. This is that the user can run:
	// sudo ./flashrom -p internal:laptop=force_I_want_a_brick 
	// from the OS, without relying an an external SPI flasher, while
	// being able to write to the descriptor region (locked by default,
	// until making the change below):
	deblobbedDescriptorStruct.masterAccessSection.flMstr1.fdRegionWriteAccess = 1;

	// relocate BIOS region and increase size to fill image
	deblobbedDescriptorStruct.regionSection.flReg1.BASE = 3; // 3<<FLREGIONBITSHIFT is 12KiB, which is where BIOS region is to begin (after descriptor and gbe)
	deblobbedDescriptorStruct.regionSection.flReg1.LIMIT = ((romSize >> FLREGIONBITSHIFT) - 1);
	// ^ for example, 8MB ROM, that's 8388608 bytes.
	// ^ 8388608>>FLREGIONBITSHIFT (or 8388608/4096) = 2048 bytes
	// 2048 - 1 = 2047 bytes. 
	// This defines where the final 0x1000 (4KiB) page starts in the flash chip, because the hardware does:
	// 2047<<FLREGIONBITSHIFT (or 2047*4096) = 8384512 bytes, or 7FF000 bytes
	// (it can't be 0x7FFFFF because of limited number of bits)

	// set ME region size to 0 - the ME is a blob, we don't want it in libreboot
	deblobbedDescriptorStruct.regionSection.flReg2.BASE = 0x1FFF; // setting 1FFF means setting size to 0. 1FFF<<FLREGIONBITSHIFT is outside of the ROM image (8MB) size?
	// ^ datasheet says to set this to 1FFF, but FFF was previously used and also worked.
	deblobbedDescriptorStruct.regionSection.flReg2.LIMIT = 0;
	// ^ 0<<FLREGIONBITSHIFT=0, so basically, the size is 0, and the base (1FFF>>FLREGIONBITSHIFT) is well outside the higher 8MB range. 
	
	// relocate Gbe region to begin at 4KiB (immediately after the flash descriptor)
	deblobbedDescriptorStruct.regionSection.flReg3.BASE = 1; // 1<<FLREGIONBITSHIFT is 4096, which is where the Gbe region is to begin (after the descriptor)
	deblobbedDescriptorStruct.regionSection.flReg3.LIMIT = 2;
	// ^ 2<<FLREGIONBITSHIFT=8192 bytes. So we are set it to size 8KiB after the first 4KiB in the flash chip.

	// set Platform region size to 0 - another blob that we don't want
	deblobbedDescriptorStruct.regionSection.flReg4.BASE = 0x1FFF; // setting 1FFF means setting size to 0. 1FFF<<FLREGIONBITSHIFT is outside of the ROM image (8MB) size?
	// ^ datasheet says to set this to 1FFF, but FFF was previously used and also worked.
	deblobbedDescriptorStruct.regionSection.flReg4.LIMIT = 0;
	// ^ 0<<FLREGIONBITSHIFT=0, so basically, the size is 0, and the base (1FFF>>FLREGIONBITSHIFT) is well outside the higher 8MB range.

	// disable ME in ICHSTRAP0 - the ME is a blob, we don't want it in libreboot
	deblobbedDescriptorStruct.ichStraps.ichStrap0.meDisable = 1;

	// disable ME and TPM in MCHSTRAP0
	deblobbedDescriptorStruct.mchStraps.mchStrap0.meDisable = 1; // ME is a blob. not wanted in libreboot.
	deblobbedDescriptorStruct.mchStraps.mchStrap0.tpmDisable = 1; // not wanted in libreboot

	// disable ME, apart from chipset bugfixes (ME region should first be re-enabled above)
	// This is sort of like the CPU microcode updates, but for the chipset
	// (commented out below here, since blobs go against libreboot's purpose,
	// but may be interesting for others)
	// deblobbedDescriptorStruct.mchStraps.mchStrap0.meAlternateDisable = 1;
	
	return deblobbedDescriptorStruct;
}

// checksum calculation for 4k gbe struct (algorithm based on datasheet)
unsigned short gbeGetChecksumFrom4kStruct(struct GBEREGIONRECORD_4K gbeStruct4k, unsigned short desiredValue)
{
	char gbeBuffer4k[GBEREGIONSIZE>>1];
	memcpy(&gbeBuffer4k, &gbeStruct4k, GBEREGIONSIZE>>1);
	return gbeGetChecksumFrom8kBuffer(gbeBuffer4k, desiredValue, 0);
}
// checksum calculation for 8k gbe region (algorithm based on datasheet)
// also works for 4k buffers, so long as isBackup remains false
unsigned short gbeGetChecksumFrom8kBuffer(char* regionData, unsigned short desiredValue, char isBackup)
{
	unsigned short regionWord;
	unsigned short checksum = 0;

	// if isBackup is true, use 2nd gbe region ("backup" region)
	unsigned short offset = 0;
	if (isBackup) offset = 0x1000>>1; // this function uses *word* not *byte* indexes.

	int i;
	for (i = 0; i < 0x3F; i++) {
		regionWord = gbeGetRegionWordFrom8kBuffer(i+offset, regionData);
		checksum += regionWord;
	}
	checksum = desiredValue - checksum;
	return checksum;
}
// Read a 16-bit unsigned int from a supplied region buffer
unsigned short gbeGetRegionWordFrom8kBuffer(int index, char* regionData)
{
	return *((unsigned short*)(regionData + (index * 2)));
}
