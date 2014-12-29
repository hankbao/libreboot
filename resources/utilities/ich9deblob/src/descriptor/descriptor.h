/*
 *  descriptor/descriptor.h
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
 * Purpose: provide struct representing descriptor region.
 * Map actual buffers of this regions, directly to instances of these
 * structs. This makes working with descriptor really easy.
 *
 * bit fields used, corresponding to datasheet. See links to datasheets
 * and documentation in ich9deblob.c
 */
 
/*
 * See docs/hcl/x200_remove_me.html for info plus links to datasheet (also linked below)
 * 
 * Info about flash descriptor (read page 845 onwards):
 * http://www.intel.co.uk/content/dam/doc/datasheet/io-controller-hub-9-datasheet.pdf
 */

#ifndef DESCRIPTORSTRUCT_H
#define DESCRIPTORSTRUCT_H

#include <stdio.h>
#include <string.h>
#include "../gbe/gbe.h" /* Needed for GBEREGIONSIZE_4K/8K define */

/* size of the descriptor in bytes */
#define DESCRIPTORREGIONSIZE 0x1000

/* 
 * Related to the flash descriptor
 * bits 12(0xC)-24(0x18) are represented for words found in the flash descriptor
 * To manipulate these easily in C, we shift them by FLREGIONBITSHIFT and then shift them back when done
 * (because this is how data is stored in the flash descriptor)
 */
#define FLREGIONBITSHIFT 0xC

/*
 * ---------------------------------------------------------------------
 * Descriptor struct representing the data
 * ---------------------------------------------------------------------
 */

/* Flash Valid Signature Register */
struct FLVALSIG
{
	/*
	 * 4 bytes. 
	 * descriptor mode = 0FF0A55A (hex, big endian). Note: stored in ROM in little endian order.
	 * Anything else is considered invalid and will put the machine in non-descriptor mode.
	 */
	unsigned int signature; /* Put 0x0FF0A55A here. confirmed in deblobbed_descriptor.bin */
};

/*  */
struct FLMAP0
{
	/* least signicant bits */
	unsigned char FCBA                      : 8;
	unsigned char NC                        : 2;
	unsigned char reserved1                 : 6;
	unsigned char FRBA                      : 8;
	unsigned char NR                        : 3;
	unsigned char reserved2                 : 5;
	/* most significant bits. */
};

struct FLMAP1 
{
	/* least significant bits */
	unsigned char FMBA                      : 8;
	unsigned char NM                        : 3;
	unsigned char reserved                  : 5;
	unsigned char FISBA                     : 8;
	unsigned char ISL                       : 8;
	/* most significant bits */
};

struct FLMAP2 
{
	/* least significant bits */
	unsigned char FMSBA                     : 8;
	unsigned char MSL                       : 8;
	unsigned short reserved                 : 16;
	/* most significant bits */
};

/* Flash Map Registers */
struct FLMAPS 
{
	struct FLMAP0 flMap0;
	struct FLMAP1 flMap1;
	struct FLMAP2 flMap2;
};

/* Flash Components Register */
struct FLCOMP 
{
	/* least significant bits */
	unsigned char component1Density         : 3;
	unsigned char component2Density         : 3;
	unsigned char reserved1                 : 2;
	unsigned char reserved2                 : 8;
	unsigned char reserved3                 : 1;
	unsigned char readClockFrequency        : 3;
	unsigned char fastReadSupport           : 1;
	unsigned char fastreadClockFrequency    : 3;
	unsigned char writeEraseClockFrequency  : 3;
	unsigned char readStatusClockFrequency  : 3;
	unsigned char reserved4                 : 2;
	/* most significant bits */
};

struct COMPONENTSECTIONRECORD 
{
	struct FLCOMP flcomp;
	unsigned int flill;
	unsigned int flpb;
	unsigned char padding[36];
};

struct FLREG 
{
	/* least significant bits */
	unsigned short BASE                     : 13;
	unsigned short reserved1                : 3;
	unsigned short LIMIT                    : 13;
	unsigned short reserved2                : 3;
	/* most significant bits */
};

/* Flash Descriptor Region Section */
/*
 * Defines where all the regions begin/end.
 * This is very important for disabling ME/AMT
 */
struct REGIONSECTIONRECORD 
{
	struct FLREG flReg0;                         /*  Descriptor */
	struct FLREG flReg1;                         /*  BIOS       */
	struct FLREG flReg2;                         /*  ME         */
	struct FLREG flReg3;                         /*  Gbe        */
	struct FLREG flReg4;                         /*  Platform   */
	unsigned char padding[12];
};

struct FLMSTR 
{
	/* least significant bits */
   unsigned short requesterId              : 16;
	unsigned char fdRegionReadAccess        : 1;
	unsigned char biosRegionReadAccess      : 1;
	unsigned char meRegionReadAccess        : 1;
	unsigned char gbeRegionReadAccess       : 1;
	unsigned char pdRegionReadAccess        : 1;
	unsigned char reserved1                 : 3; /* Must be zero, according to datasheet */
	unsigned char fdRegionWriteAccess       : 1;
	unsigned char biosRegionWriteAccess     : 1;
	unsigned char meRegionWriteAccess       : 1;
	unsigned char gbeRegionWriteAccess      : 1;
	unsigned char pdRegionWriteAccess       : 1;
	unsigned char reserved2                 : 3; /* Must be zero, according to datasheet */
	/* most significant bits */
};

/* Master Access Section */
struct MASTERACCESSSECTIONRECORD
{
	struct FLMSTR flMstr1;                       /* Flash Master 1 (Host CPU / BIOS) */
	struct FLMSTR flMstr2;                       /* Flash Master 2 (ME) */
	struct FLMSTR flMstr3;                       /* Flash Master 3 (Gbe) */
	unsigned char padding[148];
};

struct ICHSTRAP0 
{
	/* least significant bits */
	                                             /* todo: add MeSmBus2Sel (boring setting) */
	unsigned char meDisable                 : 1; /* If true, ME is disabled. */
	unsigned char reserved1                 : 6;
	unsigned char tcoMode                   : 1; /* TCO Mode: (Legacy,TCO Mode) The TCO Mode, along with the BMCMODE strap, determines the behavior of the IAMT SmBus controller. */
	unsigned char smBusAddress              : 7; /* The ME SmBus 7-bit address. */
	unsigned char bmcMode                   : 1; /* BMC mode: If true, device is in BMC mode.  If Intel(R) AMT or ASF using Intel integrated LAN then this should be false. */
	unsigned char tripPointSelect           : 1; /* Trip Point Select: false the NJCLK input buffer is matched to 3.3v signal from the external PHY device, true is matched to 1.8v. */
	unsigned char reserved2                 : 2;
	unsigned char integratedGbe             : 1; /* Integrated GbE or PCI Express select: (PCI Express,,Integrated GbE) Defines what PCIe Port 6 is used for. */
	unsigned char lanPhy                    : 1; /* LANPHYPC_GP12_SEL: Set to 0 for GP12 to be used as GPIO (General Purpose Input/Output), or 1 for GP12 to be used for native mode as LAN_PHYPC for 82566 LCD device */
	unsigned char reserved3                 : 3;
	unsigned char dmiRequesterId            : 1; /* DMI requestor ID security check disable: The primary purpose of this strap is to support server environments with multiple CPUs that each have a different RequesterID that can access the Flash. */
	unsigned char smBus2Address             : 7; /* The ME SmBus 2 7-bit address. */
	/* most significant bits */
};

struct ICHSTRAP1 
{
	/* least significant bits */
	unsigned char northMlink                : 1;  /* North MLink Dynamic Clock Gate Disable : Sets the default value for the South MLink Dynamic Clock Gate Enable registers. */
	unsigned char southMlink                : 1;  /* South MLink Dynamic Clock Gate Enable : Sets the default value for the South MLink Dynamic Clock Gate Enable registers. */
	unsigned char meSmbus                   : 1;  /* ME SmBus Dynamic Clock Gate Enable : Sets the default value for the ME SMBus Dynamic Clock Gate Enable for both the ME SmBus controllers. */
	unsigned char sstDynamic                : 1;  /* SST Dynamic Clock Gate Enable : Sets the default value for the SST Clock Gate Enable registers. */
	unsigned char reserved1                 : 4;
	unsigned char northMlink2               : 1;  /* North MLink 2 Non-Posted Enable : 'true':North MLink supports two downstream non-posted requests. 'false':North MLink supports one downstream non-posted requests. */
	unsigned char reserved2                 : 7;
	unsigned short reserved3                : 16;
	/* most significant bits */
};

/* ICH straps */
struct ICHSTRAPSRECORD 
{
	struct ICHSTRAP0 ichStrap0;
	struct ICHSTRAP1 ichStrap1;
	unsigned char padding[248];
};

struct MCHSTRAP0 
{
	/* least significant bits */
	unsigned char meDisable                 : 1;  /* If true, ME is disabled. */
	unsigned char meBootFromFlash           : 1;  /* ME boot from Flash - guessed location */
	unsigned char tpmDisable                : 1;  /* iTPM Disable : When set true, iTPM Host Interface is disabled. When set false (default), iTPM is enabled. */
	unsigned char reserved1                 : 3;
	unsigned char spiFingerprint            : 1;  /* SPI Fingerprint Sensor Present: Indicates if an SPI Fingerprint sensor is present at CS#1. */
	unsigned char meAlternateDisable        : 1;  /* ME Alternate Disable: Setting this bit allows ME to perform critical chipset functions but prevents loading of any ME FW applications. */
	unsigned char reserved2                 : 8;
	unsigned short reserved3                : 16;
	/* most significant bits */
};

/* MCH straps */
struct MCHSTRAPSRECORD 
{
	struct MCHSTRAP0 mchStrap0;
	unsigned char padding[3292];
};

/* ME VSCC Table */
struct MEVSCCTABLERECORD 
{
	unsigned int jid0;
	unsigned int vscc0;
	unsigned int jid1;
	unsigned int vscc1;
	unsigned int jid2;
	unsigned int vscc2;
	unsigned char padding[4];
};

/* Descriptor Map 2 Record */
struct DESCRIPTORMAP2RECORD
{
	/* least significant bits */
	unsigned char meVsccTableBaseAddress    : 8;
	unsigned char meVsccTableLength         : 8;
	unsigned short reserved                 : 16;
	/* most significant bits */
};

/* OEM section */
struct OEMSECTIONRECORD 
{
	unsigned char magicString[8];
	unsigned char padding[248];
};

/* 4KiB descriptor region, goes at the beginning of the ROM image */
struct DESCRIPTORREGIONRECORD 
{
	struct FLVALSIG flValSig;                                   /* Flash Valid Signature Register */
	struct FLMAPS flMaps;                                       /* Flash Map Registers */
	struct COMPONENTSECTIONRECORD componentSection;             /* Component Section Record */
	struct REGIONSECTIONRECORD regionSection;                   /* Flash Descriptor Region Section */
	struct MASTERACCESSSECTIONRECORD masterAccessSection;       /* Master Access Section */
	struct ICHSTRAPSRECORD ichStraps;                           /* ICH straps */
	struct MCHSTRAPSRECORD mchStraps;                           /* MCH straps */
	struct MEVSCCTABLERECORD meVsccTable;                       /* ME VSCC Table */
	struct DESCRIPTORMAP2RECORD descriptor2Map;                 /* Descriptor Map 2 Record */
	struct OEMSECTIONRECORD oemSection;                         /* OEM section */
};

/*
 * ---------------------------------------------------------------------
 * Function declarations (keep gcc/make happy. check them in descriptor.c)
 * ---------------------------------------------------------------------
 */
 
struct DESCRIPTORREGIONRECORD descriptorHostRegionsUnlocked(struct DESCRIPTORREGIONRECORD descriptorStruct);
struct DESCRIPTORREGIONRECORD descriptorMeRegionsForbidden(struct DESCRIPTORREGIONRECORD descriptorStruct);
struct DESCRIPTORREGIONRECORD deblobbedDescriptorStructFromFactory(struct DESCRIPTORREGIONRECORD factoryDescriptorStruct, unsigned int factoryRomSize);
int notCreatedHFileForDescriptorCFile(char* outFileName, char* cFileName);
int notCreatedCFileFromDescriptorStruct(struct DESCRIPTORREGIONRECORD descriptorStruct, char* outFileName, char* headerFileName);
void printDescriptorRegionLocations(struct DESCRIPTORREGIONRECORD descriptorStruct, char* romName);

#endif
