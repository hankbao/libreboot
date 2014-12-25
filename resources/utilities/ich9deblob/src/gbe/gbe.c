/*
 *  gbe/gbe.c
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
 * Provide gbe related functions.
 */

/* structs describing the data from gbe region */
#include "gbe.h"

/*
 * ---------------------------------------------------------------------
 * Gbe functions:
 * ---------------------------------------------------------------------
 */

/* Read a 16-bit unsigned int from a supplied region buffer */
unsigned short gbeGetRegionWordFrom8kBuffer(int index, char* regionData)
{
	return *((unsigned short*)(regionData + (index * 2)));
}

/* 
 * gbe checksum calculation (algorithm based on datasheet)
 */
unsigned short gbeGetChecksumFrom8kBuffer(char* regionData, unsigned short desiredValue, int byteOffset)
{
	int i;
	
	/* 
	 * byteOffset defines the byte address where the gbe begins in the buffer "regionData".
	 * However, this function works with 16-bit words. Shift the byte offset to the right for the word (16-bit) offset.
	 */
	int wordOffset = byteOffset >> 1;
	
	unsigned short regionWord; /* store words here for adding to checksum */
	unsigned short checksum = 0; /* this gbe's checksum */

	for (i = 0; i < 0x3F; i++) {
		regionWord = gbeGetRegionWordFrom8kBuffer(i+wordOffset, regionData);
		checksum += regionWord;
	}
	checksum = desiredValue - checksum;
	return checksum;
}

/* checksum calculation for 4k gbe struct (algorithm based on datasheet) */
unsigned short gbeGetChecksumFrom4kStruct(struct GBEREGIONRECORD_4K gbeStruct4k, unsigned short desiredValue)
{
	char gbeBuffer4k[GBEREGIONSIZE_4K];
	memcpy(&gbeBuffer4k, &gbeStruct4k, GBEREGIONSIZE_4K);
	return gbeGetChecksumFrom8kBuffer(gbeBuffer4k, desiredValue, 0);
}

/* modify the gbe region extracted from a factory.rom dump */
struct GBEREGIONRECORD_8K deblobbedGbeStructFromFactory(struct GBEREGIONRECORD_8K factoryGbeStruct8k) 
{	
	/*
	 * Correct the main gbe region. By default, the X200 (as shipped from Lenovo) comes
	 * with a broken main gbe region, where the backup gbe region is used instead. Modify
	 * it so that the main region is usable.
	 */
	
	struct GBEREGIONRECORD_8K deblobbedGbeStruct8k;
	memcpy(&deblobbedGbeStruct8k, &factoryGbeStruct8k, GBEREGIONSIZE_8K);
	
	deblobbedGbeStruct8k.backup.checkSum = gbeGetChecksumFrom4kStruct(deblobbedGbeStruct8k.backup, GBECHECKSUMTOTAL);
	memcpy(&deblobbedGbeStruct8k.main, &deblobbedGbeStruct8k.backup, GBEREGIONSIZE_4K);
	
	return deblobbedGbeStruct8k;
}

/*
 * ---------------------------------------------------------------------
 * Debugging functions:
 * ---------------------------------------------------------------------
 */

/*
 * show debugging info: show calculated (correct) gbe checksum and what
 * is actually stored, in a 4K gbe struct. Only for a single region.
 */
void printGbeChecksumDataFromStruct4k(struct GBEREGIONRECORD_4K gbeStruct4k, char* romName, char* regionName)
{
	printf(
		"%s Gbe (%s): calculated Gbe checksum: 0x%hx and actual GBe checksum: 0x%hx\n", 
		romName,
		regionName,
		gbeGetChecksumFrom4kStruct(gbeStruct4k, GBECHECKSUMTOTAL), 
		gbeStruct4k.checkSum
	);
	
	return;
}

/*
 * show debugging info: show calculated (correct) gbe checksum and what 
 * is actually stored, in a 8K gbe struct. Do so for main and backup regions.
 */
void printGbeChecksumDataFromStruct8k(struct GBEREGIONRECORD_8K gbeStruct8k, char* romName)
{	
	printGbeChecksumDataFromStruct4k(gbeStruct8k.main, romName, "main");
	printGbeChecksumDataFromStruct4k(gbeStruct8k.backup, romName, "backup");
	
	return;
}
