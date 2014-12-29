/*
 *  ich9deblob.c
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
 
/* Initially based on proof of concept by Steve Shenton. */
 
/*
 * Read a factory.rom dump (ich9m/gm45 machines) and 
 * modify the flash descriptor to remove all regions except descriptor,
 * Gbe and BIOS. Set BIOS region to full size of the ROM image (after
 * the flash descriptor and gbe). Basically, deblob the descriptor.
 * 
 * This will will generate a concatenated descriptor+gbe dump suitable
 * for use in libreboot. Currently tested: ThinkPad X200 (coreboot/libreboot)
 */
 
/*
 * See docs/hcl/x200_remove_me.html for info plus links to datasheet (also linked below)
 * 
 * Info about flash descriptor (read page 845 onwards):
 * http://www.intel.co.uk/content/dam/doc/datasheet/io-controller-hub-9-datasheet.pdf
 * 
 * Info about Gbe region (read whole datasheet):
 * http://www.intel.co.uk/content/dam/doc/application-note/i-o-controller-hub-9m-82567lf-lm-v-nvm-map-appl-note.pdf
 * https://communities.intel.com/community/wired/blog/2010/10/14/how-to-basic-eeprom-checksums
 */

#include "ich9deblob.h"

int main(int argc, char *argv[])
{
	/*
	 * descriptor region. Will have an actual descriptor struct mapped to it (from the factory.rom dump)
	 * and then it will be modified (deblobbed) to remove the ME/AMT
	 */
	char factoryDescriptorBuffer[DESCRIPTORREGIONSIZE];
	struct DESCRIPTORREGIONRECORD factoryDescriptorStruct;
	struct DESCRIPTORREGIONRECORD deblobbedDescriptorStruct;
	
	/* 
	 * gbe region. Well have actual gbe buffer mapped to it (from the factory.rom dump)
	 * and then it will be modified to correct the main region
	 */
	char factoryGbeBuffer8k[GBEREGIONSIZE_8K];
	struct GBEREGIONRECORD_8K factoryGbeStruct8k;
	struct GBEREGIONRECORD_8K deblobbedGbeStruct8k;
	
	/*
	 * Used to store the location of the Gbe
	 * region inside the factory.rom image.
	 */
	unsigned int factoryGbeRegionStart;
	
	/* names of the files that this utility will handle */
	char* factoryRomFilename = "factory.rom";                       /* user-supplied factory.bin dump (original firmware) */
	char* deblobbedDescriptorFilename = "deblobbed_descriptor.bin"; /* descriptor+gbe: to be dd'd to beginning of a libreboot image */
	
	/* Used when reading the factory.rom to extract descriptor/gbe regions */
	unsigned int bufferLength;
	
	/* For storing the size of the factory.rom dump in bytes */
	unsigned int factoryRomSize;
	
	/*
	 * ------------------------------------------------------------------
	 * Compatibility checks. This version of ich9deblob is not yet portable.
	 * ------------------------------------------------------------------
	 */

	if (systemOrCompilerIncompatible(factoryDescriptorStruct, factoryGbeStruct8k)) return 1;
	/* If true, fail with error message */
	
	/*
	 * ------------------------------------------------------------------
	 * Extract the descriptor and gbe regions from the factory.rom dump
	 * ------------------------------------------------------------------
	 */
	FILE* fileStream = NULL;
	fileStream = fopen(factoryRomFilename, "rb"); /* open factory.rom */
	if (NULL == fileStream)
	{
		printf("\nerror: could not open factory.rom\n");
		fclose(fileStream);
		return 1;
	}
	printf("\nfactory.rom opened successfully\n");
	
	/* 
	 * Get the descriptor region dump from the factory.rom
	 * (goes in factoryDescriptorBuffer variable)
	 */
	bufferLength = fread(factoryDescriptorBuffer, sizeof(char), DESCRIPTORREGIONSIZE, fileStream);
	if (DESCRIPTORREGIONSIZE != bufferLength) // 
	{
		printf("\nerror: could not read descriptor from factory.rom (%i) bytes read\n", bufferLength);
		fclose(fileStream);
		return 1;
	}
	printf("\ndescriptor region read successfully\n");
	/*
	 * copy descriptor buffer into descriptor struct memory
	 * factoryDescriptorStruct is an instance of a struct that actually
	 * defines the locations of all these variables in the descriptor,
	 * as defined in the datasheets. This allows us to map the extracted
	 * descriptor over the struct so that it can then be modified
	 * for libreboot's purpose
	 */
	memcpy(&factoryDescriptorStruct, &factoryDescriptorBuffer, DESCRIPTORREGIONSIZE);
	/*
	 * ^ the above is just for reference if needed. The modifications will be made here:
	 */
	memcpy(&deblobbedDescriptorStruct, &factoryDescriptorBuffer, DESCRIPTORREGIONSIZE);
	
	/*
	 * Get the gbe region dump from the factory.rom
	 */

	/*
	 * get original GBe region location
	 * (it will be moved to the beginning of the flash, after the descriptor region)
	 * note for example, factoryGbeRegionStart is set to <<FLREGIONBITSHIFT of actual address (in C). this is how the addresses
	 * are stored in the descriptor.
	 */
	factoryGbeRegionStart = factoryDescriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT;

	/*
	 * Set offset so that we can read the data from
	 * the gbe region
	 */
	fseek(fileStream, factoryGbeRegionStart, SEEK_SET);
	/* Read the gbe data from the factory.rom and put it in factoryGbeBuffer8k */
	bufferLength = fread(factoryGbeBuffer8k, sizeof(char), GBEREGIONSIZE_8K, fileStream);
	if (GBEREGIONSIZE_8K != bufferLength)
	{
		printf("\nerror: could not read GBe region from factory.rom (%i) bytes read\n", bufferLength);
		fclose(fileStream);
		return 1;
	}
	printf("\ngbe (8KiB) region read successfully\n");
	/*
	 * copy gbe buffer into gbe struct memory
	 * factoryGbeStruct8k is an instance of a struct that actually
	 * defines the locations of all these variables in the gbe,
	 * as defined in the datasheets. This allows us to map the extracted
	 * gbe over the struct so that it can then be modified
	 * for libreboot's purpose
	 */
	memcpy(&factoryGbeStruct8k, &factoryGbeBuffer8k, GBEREGIONSIZE_8K);
	/*
	 * the original factoryGbeStruct8k is only reference. Changes go here:
	 */
	memcpy(&deblobbedGbeStruct8k, &factoryGbeBuffer8k, GBEREGIONSIZE_8K);

	/* 
	 * Get size of ROM image
	 * This is needed for relocating the BIOS region (per descriptor)
	 */
	fseek(fileStream, 0L, SEEK_END);
	factoryRomSize = ftell(fileStream);

	printf("\nfactory.rom size: [%i] bytes\n", factoryRomSize);

	fclose(fileStream);
	
	/*
	 * ------------------------------------------------------------------
	 * Modify the descriptor and gbe regions, ready to go in libreboot.rom
	 * ------------------------------------------------------------------
	 */

	/*
	 * Disable the ME and Platform regions. Put Gbe at the beginning (after descriptor). 
	 * Also, extend the BIOS region to fill the ROM image (after descriptor+gbe).
	 */
	deblobbedDescriptorStruct = deblobbedDescriptorStructFromFactory(factoryDescriptorStruct, factoryRomSize);
	/*
	 * Debugging: show region locations in the 
	 * original descriptor and the modified one
	 */
	printDescriptorRegionLocations(factoryDescriptorStruct, "Original");
	printDescriptorRegionLocations(deblobbedDescriptorStruct, "Modified");

	/*
	 * Modify the Gbe region (see function for details)
	 */
	deblobbedGbeStruct8k = deblobbedGbeStructFromFactory(factoryGbeStruct8k);
	/*
	 * Debugging: show difference between Gbe checksums in the
	 * original gbe and the modified one
	 */
	printf("\n");
	printGbeChecksumDataFromStruct8k(factoryGbeStruct8k, "Original");
	printGbeChecksumDataFromStruct8k(deblobbedGbeStruct8k, "Modified");

	/*
	 * ------------------------------------------------------------------
	 * Create the file with the modified descriptor and gbe inside
	 * ------------------------------------------------------------------
	 */

	printf("\n");
	if (notCreatedDescriptorGbeFile(deblobbedDescriptorStruct, deblobbedGbeStruct8k, deblobbedDescriptorFilename)) {
		return 1;
	}
	
	/*
	 * ------------------------------------------------------------------
	 * Generate ich9gen data (C code that will recreate the deblobbed descriptor+gbe from scratch)
	 * ------------------------------------------------------------------
	 */
	/* Code for generating the Gbe struct */
	/* mkgbe.h */
	if (notCreatedHFileForGbeCFile("mkgbe.h", "mkgbe.c")) {
		return 1;
	} /* and now mkgbe.c */
	if (notCreatedCFileFromGbeStruct4k(deblobbedGbeStruct8k.backup, "mkgbe.c", "mkgbe.h")) {
		return 1;
	}
	/* Code for generating the Descriptor struct */
	/* mkdescriptor.h */
	if (notCreatedHFileForDescriptorCFile("mkdescriptor.h", "mkdescriptor.c")) {
		return 1;
	} /* and now mkdescriptor.c */
	if (notCreatedCFileFromDescriptorStruct(deblobbedDescriptorStruct, "mkdescriptor.c", "mkdescriptor.h")) {
		return 1;
	}
	printf("The modified descriptor and gbe regions have also been dumped as src files: mkgbe.c, mkgbe.h, mkdescriptor.c, mkdescriptor.h\n");
	printf("To use these in ich9gen, place them in src/ich9gen/ and re-build ich9gen.\n\n");

	return 0;
}
