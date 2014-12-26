/*
 *  gbe/gbe.h
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
 * Purpose: provide struct representing gbe region.
 * Map actual buffers of this regions, directly to instances of these
 * structs. This makes working with gbe really easy.
 */
 
/*
 * bit fields used, corresponding to datasheet. See links to datasheets
 * and documentation in ich9deblob.c
 */

#ifndef GBESTRUCT_H
#define GBESTRUCT_H

#include <stdio.h>
#include <string.h>

/* Size of the full gbe region in bytes */
#define GBEREGIONSIZE_8K 0x2000
/*
 * Size of each sub-region in gbe. 
 * gbe contains two regions which
 * can be identical: main and backup.
 * These are each half the size of the full region
 */
#define GBEREGIONSIZE_4K 0x1000

/* 
 * When adding up the first 0x3F 16-bit words
 * in a 4KiB GBE region, it should be equal
 * to 0xBABA
 */
#define GBECHECKSUMTOTAL 0xBABA

/*
 * These will have a modified descriptor+gbe based on what's in the factory.rom
 * These will be joined into a single 12KiB buffer (descriptor, then gbe) and saved to a file
 * NOTE: The GBE region of 8K is actually 2x 4K regions in a single region; both 4K blocks can be identical (and by default, are)
 * The 2nd one is a "backup", but we don't know when it's used. perhaps it's used when the checksum on the first one does not match?
 */

/*
 * ---------------------------------------------------------------------
 * Gbe struct representing the data:
 * ---------------------------------------------------------------------
 */ 

struct GBEREGIONRECORD_4K {
	unsigned char macAddress[6]; /* 0x03 words, or 0x06 bytes */
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned short reserved3;
	unsigned short reserved4;
	unsigned short reserved5;
	unsigned short pbaLow;
	unsigned short pbaHigh;
	unsigned short pciInitializationControlWord;
	unsigned short subsystemId;
	unsigned short subsystemVendorId;
	unsigned short deviceId;
	unsigned short vendorId;
	unsigned short deviceRevId;
	unsigned short lanPowerConsumption;
	unsigned short reserved6;
	unsigned short reserved7;
	unsigned short sharedInitializationControlWord;
	unsigned short extendedConfigurationControlWord1;
	unsigned short extendedConfigurationControlWord2;
	unsigned short extendedConfigurationControlWord3;
	unsigned short ledCtl1;
	unsigned short ledCtl02;
	unsigned short reserved8;
	unsigned short reserved9;
	unsigned short reserved10;
	unsigned short reserved11;
	unsigned short reserved12;
	unsigned short _82567lmDeviceId;
	unsigned short _82567lfDeviceId;
	unsigned short reserved13;
	unsigned short _82567vDeviceId;
	unsigned short reserved14;
	unsigned short reserved15;
	unsigned short reserved16[12];
	unsigned short pxeSoftwareRegion[15];
	unsigned short checkSum; /* when added to the sum of all words above, this should match GBECHECKSUMTOTAL */
	unsigned char padding1[3968];
};

/*  main and backup region in gbe */ 
struct GBEREGIONRECORD_8K {
	struct GBEREGIONRECORD_4K main;
	struct GBEREGIONRECORD_4K backup;
	/* 
	 * Backup region:
	 * This is actually "main" on X200, since the real main has a bad checksum
	 * and other errors. You should do what you need on this one (if modifying
	 * lenovobios's gbe region) and then copy to main
	 */
};

/*
 * ---------------------------------------------------------------------
 * Function declarations (keep gcc/make happy. check them in gbe.c)
 * ---------------------------------------------------------------------
 */
 
unsigned short gbeGetRegionWordFrom8kBuffer(int index, char* regionData);
unsigned short gbeGetChecksumFrom8kBuffer(char* regionData, unsigned short desiredValue, int byteOffset);
unsigned short gbeGetChecksumFrom4kStruct(struct GBEREGIONRECORD_4K gbeStruct4k, unsigned short desiredValue);
struct GBEREGIONRECORD_8K deblobbedGbeStructFromFactory(struct GBEREGIONRECORD_8K factoryGbeStruct8k);
void printGbeChecksumDataFromStruct4k(struct GBEREGIONRECORD_4K gbeStruct4k, char* romName, char* regionName);
void printGbeChecksumDataFromStruct8k(struct GBEREGIONRECORD_8K gbeStruct8k, char* romName);

#endif
