/*
 *  ich9show.c
 *  Just a little hack by Leah to show the ICH9 region read/write status
 *
 * Purpose: disable and remove the ME from ich9m/gm45 systems in coreboot.
 *
 *  Copyright (C) 2014 Steve Shenton <sgsit@libreboot.org>
 *  Copyright (C) 2014,2015,2019 Leah Rowe <info@minifree.org>
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
/* Original utility can be found at https://gitorious.org/ich9descriptortool */

/*
 * Read a factory.rom dump (ich9m/gm45 systems) and
 * modify the flash descriptor to remove all regions except descriptor,
 * Gbe and BIOS. Set BIOS region to full size of the ROM image (after
 * the flash descriptor and gbe). Basically, deblob the descriptor.
 *
 * This will will generate a concatenated descriptor+gbe dump suitable
 * for use in libreboot. Currently tested: ThinkPad X200 (coreboot/libreboot)
 */

/*
 * See docs/hardware/x200_remove_me.html for info plus links to datasheet (also linked below)
 *
 * Info about flash descriptor (read page 845 onwards):
 * http://www.intel.co.uk/content/dam/doc/datasheet/io-controller-hub-9-datasheet.pdf
 *
 * Info about Gbe region (read whole datasheet):
 * http://www.intel.co.uk/content/dam/doc/application-note/i-o-controller-hub-9m-82567lf-lm-v-nvm-map-appl-note.pdf
 * https://web.archive.org/web/20150912070329/https://communities.intel.com/community/wired/blog/2010/10/14/how-to-basic-eeprom-checksums
 */

#include "ich9show.h"

int main(int argc, char *argv[])
{
    if(argc!=2) {
        printf("Incorrect number of arguments. Please specify *one* ROM path.\n");
        return 1;
    } /* TODO: Make ich9show support dumping stats of multiple ROM images */

	struct DESCRIPTORREGIONRECORD descriptorStruct;
	uint8_t* descriptorBuffer = (uint8_t*)&descriptorStruct;

	struct GBEREGIONRECORD_8K gbeStruct8k;
	uint8_t* gbeBuffer8k = (uint8_t*)&gbeStruct8k;
	uint32_t gbeRegionStart;

	char* romFilename = argv[1];

	unsigned int bufferLength;
	unsigned int romSize;

	/*
	 * ------------------------------------------------------------------
	 * Compatibility checks. This version of ich9deblob is not yet portable.
	 * ------------------------------------------------------------------
	 */

	if (systemOrCompilerIncompatible(descriptorStruct, gbeStruct8k)) return 1;
	/* If true, fail with error message */

	/*
	 * ------------------------------------------------------------------
	 * Extract the descriptor and gbe regions from the factory.rom dump
	 * ------------------------------------------------------------------
	 */
	FILE* fp = NULL;
	fp = fopen(romFilename, "rb");
	if (NULL == fp)
	{
		printf("\nerror: could not open %s\n", romFilename);
		fclose(fp);
		return 1;
	}
	printf("\n%s opened successfully\n", romFilename);

	/*
	 * Get the descriptor region dump from the the given ROM
	 * (goes in factoryDescriptorBuffer variable)
	 */
	bufferLength = fread(descriptorBuffer, 1, DESCRIPTORREGIONSIZE, fp);
	if (DESCRIPTORREGIONSIZE != bufferLength) //
	{
		printf("\nerror: could not read descriptor from %s (%i) bytes read\n", romFilename, bufferLength);
		fclose(fp);
		return 1;
	}
	printf("\ndescriptor region read successfully\n");

    if(!validDescriptor(descriptorStruct)) {
        printf("Invalid input: incorrect signature in the given descriptor.");
        fclose(fp);
        return 1;
    }

	if (descriptorDefinesGbeRegion(descriptorStruct))
	{
		gbeRegionStart = descriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT;

		/*
		 * Set offset so that we can read the data from
		 * the gbe region
		 */
		fseek(fp, gbeRegionStart, SEEK_SET);
		/* Read the gbe data from the given ROM and put it in factoryGbeBuffer8k */
		bufferLength = fread(gbeBuffer8k, 1, GBEREGIONSIZE_8K, fp);
		if (GBEREGIONSIZE_8K != bufferLength)
		{
			printf("\nerror: could not read GBe region from %s (%i) bytes read\n", romFilename, bufferLength);
			fclose(fp);
			return 1;
		}
		printf("\ngbe (8KiB) region read successfully\n");
	}

	fseek(fp, 0L, SEEK_END);
	romSize = ftell(fp);
	printf("\n%s size: [%i] bytes\n", romFilename, romSize);

	fclose(fp);

    showDescriptorData(descriptorStruct);

	if (descriptorDefinesGbeRegion(descriptorStruct))
	{
        printf("\nMain GbE region:\n\n");
        showGbeData(gbeStruct8k.main);
        printf("\n");

        printf("\nBackup GbE region:\n\n");
	    showGbeData(gbeStruct8k.backup);
        printf("\n");
	}

	return 0;
}
