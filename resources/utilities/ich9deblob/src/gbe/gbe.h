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

struct GBE_RESERVED_WORD_03H {
	/* least significant bits */
	unsigned short reserved1                   : 11; /* bits should all be set to zero */
	unsigned char ibaLom                       : 1;  /* set to 1 for intel boot agent to work (i.e. set it to 0) */
	unsigned char reserved2 		             : 4;  /* bits should all be set to zero */
	/* most significant bits */
};

struct PCI_INITIALIZATION_CONTROL_WORD {
	/* least significant bits */
	unsigned char loadVendorDeviceId           : 1;  /* 1 = load from NVM. 0 = load from MAC fuses. It's 1 in my deblobbed_descriptor.bin */
	unsigned char loadSubsystemId              : 1;  /* 1 = load from NVM. 0 = load from MAC fuses. It's 1 in my deblobbed_descriptor.bin */
	unsigned char reserved1                    : 1;  /* Reserved. Set to 0 (according to datasheet). 0 in my deblobbed_descriptor.bin */
	unsigned char reserved2                    : 3;  /* Reserved. Set them to 0 (according to datasheet). 0 in my deblobbed_descriptor.bin */
	unsigned char pmEnable                     : 1;  /* Power Management Enable. 1=Enable. It's 1 in my deblobbed_descriptor.bin */
	unsigned char auxPwr                       : 1;  /* Auxiliary Power Indication. See datasheet. it's 1 in my deblobbed_descriptor.bin */
	unsigned char reserved3                    : 4;  /* Reserved. Set to 0000 (according to datasheet). */
	unsigned char reserved4                    : 4;  /* Reserved. Set to 0001 (according to datasheet). */
	/* most significant bits */
};

struct GBEREGIONRECORD_4K {
	unsigned char macAddress[6];                             /* Word 00 to 02 */
	struct GBE_RESERVED_WORD_03H reservedWord03h;            /* Reserved word 03. */
	unsigned short reservedWord04h;                          /* Reserved word 04: set it to 0xFFFF (according to datasheet) */
	unsigned short reservedWord05h;                          /* Reserved word 05: 83 10 (little endian) in my deblobbed_descriptor.bin. Set this to 0x1083 (in C, assuming little endian byte order). "cannot be changed" according to datasheet */
	unsigned short reservedWords06h07h[2];                   /* Reserved words 06-07: set both to 0xFFFF (according to datasheet) */

	/*
	 * Word 08 and 09 (pba low and pba high):
	 * 
	 * Both of these should be set to 0xFFFF by default, according to the datasheet.
	 * "nine digit printed board assembly (PBA) number" for intel cards to be stored
	 * in a 4 byte (read: 2 word) field. 
	 * 
	 * Example: if pba number is 123456-003, then word 08 should be 1234h and word 09 becomes 5603.
	 * Note: 1234 and 5603 above are big endian. In the image it would actually be 34 12 and 0356
	 * 
	 * Example: in mine it was (in the image): 08 10 FF FF. That becomes 1008h and FFFFh, or
	 * basically: 1008FF-0FF. The same was observed in another.
	 * 
	 * Setting it to FF FF FF FF should be fine, according to the datasheet.
	 */
	unsigned short pbaLow;												/* Word 08. Set it to 0x1008. */
	unsigned short pbaHigh;												/* Word 09. Set it to 0xFFFF. */
	
	/* Word 0A */
	struct PCI_INITIALIZATION_CONTROL_WORD pciInitializationControlWord;
	
	/* 
	 * Word 0B; subsystem ID
	 * 
	 * If load subsystem ID bit of word 0A (pci init control word) is
	 * set to 1 (read: it is. in my deblobbed_descriptor.bin), store
	 * the subsystem id here. Datasheet says that the default value is
	 * 0000h, but you should set this to 20EEh (little endian: EE 20)
	 */
	unsigned short subsystemId; /* Set this to 0x20EE */
	
	/*
	 * Word 0C; subsystem vendor ID
	 * 
	 * If load subsystem vendor ID bit of word 0A (pci init control word)
	 * is set to 1 (read: it is. in my deblobbed_descriptor.bin), store
	 * the subsystem vendor id here. Datasheet says that the default 
	 * value is 8086h, but you should set this to 17AAh (lendian: AA 17).
	 */
	unsigned short subsystemVendorId; /* Set this to 0x17AA */
	
	/* 
	 * Word 0D: device ID
	 * 
	 * If load vendor/device ID in word 0A (pci init control word) is 1
	 * (it is) then this word is used to init device id using word 21h,
	 * 1Eh or 1Fh. In my case, deviceId is 0x10F5. Word 21h is set to
	 * 0x10CB, word 1Eh is 0x10F5 and 1Fh is 0x10BF
	 * 
	 * The datasheet says that 10F5 is for Intel 82567LM gigabit ethernet
	 * controller; 10BF is for Intel 82567LF and 10CB is for Intel 82567V.
	 * 
	 * Based on this, the X200 is shown to have the Intel 82567LM ethernet
	 * controller.
	 */
	unsigned short deviceId; /* Set this to 0x10F5. */
	/* It is important that this is correct, for the linux kernel driver */
	
	/* 
	 * Word 0E: vendor ID
	 * 
	 * If load vendor/device ID in word 0A (pci init control) is 1 (it is),
	 * then this word used read to initialize the PCI vendor ID. Default
	 * value is 8086 according to datasheets, and deblobbed_descriptor.bin. 
	 * 
	 * Intel is often 8086 as a PCI vendor ID. Because 8086. As in the CPU architecture.
	 */
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
