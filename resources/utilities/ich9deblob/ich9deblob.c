/*
 *  ich9deblob.c
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

#include <stdio.h>
#include <string.h>
#include "ich9desc.c" // structs describing what's in the descriptor region
#include "ich9gbe.c" // structs describing what's in the gbe region

#define DESCRIPTORREGIONSIZE 0x1000 // 4096 bytes / 4KiB
#define GBEREGIONSIZE 0x2000 // 8192 bytes / 8KiB

unsigned short GetChecksum(char* buffer, unsigned short desiredValue); // for GBe region (checksum calculation)
unsigned short GetRegionWord(int i, char* buffer); // used for getting each word needed to calculate said checksum

int main(int argc, char *argv[])
{
	// check compiler bit-packs in a compatible way
	struct DESCRIPTORREGIONRECORD descriptorRegion;
	unsigned int descriptorRegionStructSize = sizeof(descriptorRegion);
	// basically, it is expected that this code will be used on x86
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

	// get original GBe region location
	// (it will be moved to the beginning of the flash, after the descriptor region)
	unsigned int flRegionBitShift = 12;
	unsigned int gbeRegionLocation = descriptorRegion.regionSection.flReg3.BASE << flRegionBitShift;

	fseek(fp, gbeRegionLocation, SEEK_SET);

	char gbeBuffer[GBEREGIONSIZE];

	readLen = fread(gbeBuffer, sizeof(char), GBEREGIONSIZE, fp);
	if (GBEREGIONSIZE != readLen)
	{
		printf("\nerror: could not read GBe region from factory.rom (%i) bytes read\n", readLen);
		return 1;
	}

	// Get size of ROM image
	// This is needed for relocating the BIOS region (per descriptor)
	fseek(fp, 0L, SEEK_END);
	int romSize = ftell(fp);

	printf("\nfactory.rom size: [%i] bytes\n", romSize);

	fclose(fp);

	printf("\nOriginal Descriptor start block: %08x ; Descriptor end block: %08x\n", descriptorRegion.regionSection.flReg0.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg0.LIMIT << flRegionBitShift);
	printf("Original BIOS start block: %08x ; BIOS end block: %08x\n", descriptorRegion.regionSection.flReg1.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg1.LIMIT << flRegionBitShift);
	printf("Original ME start block: %08x ; ME end block: %08x\n", descriptorRegion.regionSection.flReg2.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg2.LIMIT << flRegionBitShift);
	printf("Original GBe start block: %08x ; GBe end block: %08x\n", gbeRegionLocation, descriptorRegion.regionSection.flReg3.LIMIT << flRegionBitShift);

	// set number of regions from 4 -> 2 (0 based)
	descriptorRegion.flMaps.flMap0.NR = 2;

	// make descriptor writable from OS.
	descriptorRegion.masterAccessSection.flMstr1.fdRegionWriteAccess = 1;

	// relocate BIOS region and increase size to fill image
	descriptorRegion.regionSection.flReg1.BASE = 3;
	descriptorRegion.regionSection.flReg1.LIMIT = ((romSize / 0x1000) - 1);

	// set ME region size to 0
	descriptorRegion.regionSection.flReg2.BASE = 0xFFF;
	descriptorRegion.regionSection.flReg2.LIMIT = 0;

	// relocate Gbe region
	descriptorRegion.regionSection.flReg3.BASE = 1;
	descriptorRegion.regionSection.flReg3.LIMIT = 2;

	// set Platform region size to 0
	descriptorRegion.regionSection.flReg4.BASE = 0xFFF;
	descriptorRegion.regionSection.flReg4.LIMIT = 0;

	// disable ME in ICHSTRAP0
	descriptorRegion.ichStraps.ichStrap0.meDisable = 1;

	// disable ME and TPM in MCHSTRAP0
	descriptorRegion.mchStraps.mchStrap0.meDisable = 1;
	descriptorRegion.mchStraps.mchStrap0.tpmDisable = 1;

	// disable ME, apart from chipset bugfixes (ME region still required)
	//descriptorRegion.mchStraps.mchStrap0.meAlternateDisable = 1;

	// debugging
	printf("\nRelocated Descriptor start block: %08x ; Descriptor end block: %08x\n", descriptorRegion.regionSection.flReg0.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg0.LIMIT << flRegionBitShift);
	printf("Relocated BIOS start block: %08x ; BIOS end block: %08x\n", descriptorRegion.regionSection.flReg1.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg1.LIMIT << flRegionBitShift);
	printf("Relocated ME start block: %08x ; ME end block: %08x\n", descriptorRegion.regionSection.flReg2.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg2.LIMIT << flRegionBitShift);
	printf("Relocated GBe start block: %08x ; GBe end block: %08x\n", descriptorRegion.regionSection.flReg3.BASE << flRegionBitShift, descriptorRegion.regionSection.flReg3.LIMIT << flRegionBitShift);


	char deblobbedDescriptorBuffer[DESCRIPTORREGIONSIZE];
	memcpy(&deblobbedDescriptorBuffer, &descriptorRegion, DESCRIPTORREGIONSIZE);

	remove(deblobbedDescriptorFilename);
	fp = fopen(deblobbedDescriptorFilename, "ab");

	if (DESCRIPTORREGIONSIZE != fwrite(deblobbedDescriptorBuffer, sizeof(char), DESCRIPTORREGIONSIZE, fp))
	{
		printf("\nerror: writing descriptor region failed\n");
		return 1;
	}

	if (GBEREGIONSIZE != fwrite(gbeBuffer, sizeof(char), GBEREGIONSIZE, fp))
	{
		printf("\nerror: writing GBe region failed\n");
		return 1;
	}

	fclose(fp);

	printf("\ndeblobbed descriptor successfully created: deblobbed_descriptor.bin \n");

	// calculate the 0x3F'th 16-bit uint to make the desired final checksum for GBe
	// observed values (from actual factory.rom dumps) 0xBABA 0x3ABA 0x34BA. spec defined as 0xBABA.
	// theoretically, this could be any l33t speak variation of BABA, eg 3434 or BA34, and so on, but this is untested. so far.
	// 40BA was also observed in another factory dump for another X200 - this is not l33t speak. 
	// maybe only the 8 least significant bits are checked? or something deeper than that
	unsigned short gbeCalculatedChecksum = GetChecksum(gbeBuffer, 0xBABA);
	// get the actual 0x3F'th 16-bit uint that was already in the supplied (pre-compiled) region data
	unsigned short gbeChecksum = GetRegionWord(0x3F, gbeBuffer);

	printf("\ncalculated Gbe checksum: 0x%hx actual GBe checksum: 0x%hx\n", gbeCalculatedChecksum, gbeChecksum);

	return 0;
}

unsigned short GetChecksum(char* regionData, unsigned short desiredValue)
{
	unsigned short regionWord;
	unsigned short checksum = 0;

	int i;
	for (i = 0; i < 0x3F; i++) {
		regionWord = GetRegionWord(i, regionData);
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
