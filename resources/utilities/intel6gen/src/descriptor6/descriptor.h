/*
 *  descriptor/descriptor.h
 *  This file is part of the intel6deblob utility from the libreboot project
 *
 *  Copyright (C) 2014, 2015 Leah Rowe <info@minifree.org>
 *  Copyright (C) 2014 Steve Shenton <sgsit@libreboot.org>
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
 * and documentation in intel6deblob.c
 */
 
#ifndef DESCRIPTORSTRUCT_H
#define DESCRIPTORSTRUCT_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../gbe/gbe.h" /* Needed for GBEREGIONSIZE_4K/8K define */

/* size of the descriptor in bytes */
#define DESCRIPTORREGIONSIZE 0x1000

/* ROM image sizes in bytes */
#define ROMSIZE_512KB 0x80000
#define ROMSIZE_1MB 0x100000
#define ROMSIZE_2MB 0x200000
#define ROMSIZE_4MB 0x400000
#define ROMSIZE_8MB 0x800000
#define ROMSIZE_16MB 0x1000000

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
/* Address in chip: FDBAR(0x0) + 0x10 */
struct FLVALSIG /* DONE (sandybridge conversion) */
{
	/*
	 * 4 bytes. 
	 * descriptor mode = 0FF0A55A (hex, big endian). Note: stored in ROM in little endian order.
	 * Anything else is considered invalid and will put the system in non-descriptor mode.
	 */
    uint16_t padding[16];  /* 16 bytes is padding - set all of them to 0xFF */
    /* This isn't actually part of FLVALSIG, it's just put here for simplicity. First 16 bytes of the chip is padding */
	uint32_t signature                 : 32; /* Put 0x0FF0A55A here. confirmed in deblobbed_descriptor.bin */
    /* any other value means non-descriptor mode, which probably won't work on most sandybridge systems */
};

/* 
    DONE (sandybridge conversion)
    Address in chip: FDBAR(0x0) + 0x14
*/
struct FLMAP0 /* DONE (sandybridge conversion) */
{
	/* least signicant bits */
	uint8_t FCBA                      : 8; /* Flash Component Base Address. set this to 0x03 - this will define FCBA as 0x30 (0x03 shifted 4 bits left) */
	uint8_t NC                        : 2; /* Number of Components (flash chips). 00 = 1 flash chip. 01 = 2 flash chips. X220 has 1 flash chip, so use 00b */
	uint8_t reserved1                 : 6; /* unused? */
	uint8_t FRBA                      : 8; /* Flash Region Base Address. set this to 0x04 - this will define FRBA as 0x40 */
	uint8_t NR                        : 3; /* Number of Regions in the flash chip (NOTE: important for disabling ME) */
	uint8_t reserved2                 : 5; /* unused? */
	/* most significant bits. */
};

/* DONE (sandybridge conversion) */
/*
    Address in chip: FDBAR(0x0) + 0x18
*/
struct FLMAP1 /* Recommended value 0x12100206 */
{
	/* least significant bits */
	uint8_t FMBA                      : 8; /* Flash Master Base Address. set this to 0x06 (defines FMBA as 0x60) */
	uint8_t NM                        : 3; /* Number of Masters. total number of flash masters (flash chips?) set this to 010b */
	uint8_t reserved                  : 5; /* unused? */
	uint8_t FPSBA                     : 8; /* Flash PCH Strap Base Address - set to 0x10 (defines it as 0x100) */
	uint8_t ISL                       : 8; /* PCH Strap Length - 1s based number of dwords of PCH straps to be read - 0xFF means there are 255 DWords (is it actually 256 though?) - meaning, 1KiB of straps to be read. all zeroes means that there are no straps to be read.
    ISL ****MUSH**** be set to 0x12 */
	/* most significant bits */
};

/*
    DONE (sandybridge conversion)
    Address in chip: FDBAR(0x0) + 0x1c
*/
struct FLMAP2  /* recommended value 0x00000120 */
{
	/* least significant bits */
	uint8_t FPSBA                     : 8; /* Flash Processor Strap Base Address. Set this to 0x20 (defines it as 0x200) */
	uint8_t PSL                       : 8; /* PROC Strap Length. 1s based number of dwords of processor straps to be read. up to 255 dwords (1KB) max. all zeroes indicates that no straps are to be read. Set this to 0x01 */
	uint16_t reserved                 : 16;
        /* Of interest: factory.rom on X220 showed this value (little endian) as 0x0021 (stored at 21 00 in the ROM) */
	/* most significant bits */
};

/* Flash Map Registers */
struct FLMAPS 
{
    /* 
        These sections describe the data format of the Flash Descriptor on
        the flash chip. These do not define memory spaces within the PCH.
        FDBAR  is address 0x0 on the chip
    */
	struct FLMAP0 flMap0;
	struct FLMAP1 flMap1;
	struct FLMAP2 flMap2;
};

/* 0x30 is default address in the flash chip */
/* hex dump from X220 factory ROM: 24 00 90 49 (little endian)*/
/* actual value: 0x49900024 */
/* Flash Components Register */
/* TODO FOR LEAH: make sure to set fastReadSupport to 0 always, because it might be that some flash chips */
/* according to swiftgeek in IRC, read can always be 50MHz and write/erase doesn't matter much */
/* do not support it, and we're making a one size fits all setting. As far as I know, all chips will work without fast read enabled */
struct FLCOMP
{
	/* least significant bits */
	uint8_t component1Density         : 3; /* Defines the size of the first flash chip. 0=512k chip (also for smaller flash chips), 1=1MB, 2=2MB, 3=4MB, 4=8MB, 5=16MB, 6=undefined, 7=reserved */
	uint8_t component2Density         : 3; /* Defines the size of the second flash chip. (same values have the same meaning as component1Density) */
	uint8_t reserved1                 : 2; /* reversed */ /* 3 variables due to how C bitfields work. In other languages, this can be all a single 11-bit variable */
	uint8_t reserved2                 : 8; /* reverved */
	uint8_t reserved3                 : 1; /* reserved */
	uint8_t readClockFrequency        : 3; /* set clock frequency for the chip. if 2 chips are used, then use the lowest frequency that both chips support. on X220, typically 1 chip is present, so this is probably not very important. 0=20MHz. all other settings reserved. so it's pretty much that you can only set 20MHz anyway. totally daft. Why is this setting even present? alas, X220 factory ROM sets this to 0 */
	uint8_t fastReadSupport           : 1; /* 0=no fast read, 1=fast read. X220 factory ROM says 1, so that has fast read. intel also recommends setting it to 1 */ /* */
	uint8_t fastreadClockFrequency    : 3; /*  000=20MHz, 001=33MHz, 100=50MHz, other=reserved - TODO FOR LEAH: always set this to 000 - X220 factory ROM that I dumped sets this to 100, or 50MHz. it also enables fast read */
	uint8_t writeEraseClockFrequency  : 3; /* 000=20MHz, 001=33MHz, 100=50MHz. speed at which erase occurs. This is set to 33MHz in the X220 factory ROM that I dumped */
	uint8_t readIdReadStatusClkFreq   : 3; /* Read ID and Read Status Clock Frequency. Same deal. 000=20MHz,001=33MHz,100=50MHz,other=reserved My X220 has this at 33MHz in factory BIOS */
    uint8_t singleIoFastReadSupport   : 1; /* Single Input Dual Output Fast Read Support. My factory bios has this at 1 (supported), 0 means insupported. single input, dual input fast read opcode. TODO: make sure that this setting is supported across all flash chips */
	uint8_t reserved4                 : 1;
	/* most significant bits */
};

/* address in chip: FCBA(0x30)+4 bytes. So, 0x34. 4 bytes in size (32 bits), value is a single 32 bit integer little endian */
struct FLILL /* Flash Invalid Instructions Record */
{
    /* least significant bits */ 
    uint8_t invalidInstruction0 :8;
    uint8_t invalidInstruction1 :8;
    uint8_t invalidInstruction2 :8;
    uint8_t invalidInstruction3 :8; 
    /* these let you define flash chip opcodes which are invalid and should be protected againstc
    - software slequenced opcode menu configuration and prefix opcode configuration
    treat this as a Do Not Care */
    /* most significant bits */

    /* These are all 0x00 in my factory BIOS image from an X220 */
};

/* address in chip: FCBA(0x30)+8. So, 0x38. 4 bytes in size (32 bits), value is a single 32 bit integer little endian */
/* my factory BIOS has this as all 0x00 */
struct FLPB
{
   /* least significant bits */
    uint32_t FPBA                    : 13; 
    uint32_t reserved                : 19;
   /* most significant bits */
};

/* starts at memory address in SPI flash: FCBA (0x30) */
struct COMPONENTSECTIONRECORD
{
	struct FLCOMP flcomp;
	struct FLILL flill;
    struct FLPB flpb; /* starts at 0x38 and is 4 bytes, next section (REGIONSECTIONRECORD) begins at 0x40 */
	uint8_t padding[4]; /* thus, this is 4 bytes */
};

struct FLREG 
{
	/* least significant bits */
	uint16_t BASE                     : 13; /* where this section begins (real value: shift 13 bit value left by 12 bits) */
	uint16_t reserved1                : 3;
	uint16_t LIMIT                    : 13; /* how large this in bytes (real value: shift 13 bit value left by 12 bits) */
	uint16_t reserved2                : 3;
	/* most significant bits */
};
/* Flash Descriptor Region Section */
/*
 * Defines where all the regions begin/end.
 * This is very important for disabling ME/AMT
 */
 /* region section begins at address 0x40 in flash chip, or same as FRBA value in other words */
 /* each FLREG is 4 bytes (little endian 32-bit integer) */
struct REGIONSECTIONRECORD /* DONE (sandybridge conversion) */ 
{
	struct FLREG flReg0;                         /*  Descriptor - all 0s - address 0x40 */
	struct FLREG flReg1;                         /*  BIOS       - address 0x44 */
	struct FLREG flReg2;                         /*  ME         - address 0x48 */
	struct FLREG flReg3;                         /*  Gbe        - address 0x4c */
	struct FLREG flReg4;                         /*  Platform   - address 0x50 - ends at 0x53 */
	uint8_t padding[12]; /* next section, MASTERACCESSSECTIONRECORD, starts at 0x60 */
        /* thus, padding is 12 bytes (no change from ICH9) */
};

struct FLMSTR 
{
	/* least significant bits */
   uint16_t requesterId              : 16;
	uint8_t fdRegionReadAccess        : 1;
	uint8_t biosRegionReadAccess      : 1;
	uint8_t meRegionReadAccess        : 1;
	uint8_t gbeRegionReadAccess       : 1;
	uint8_t pdRegionReadAccess        : 1;
	uint8_t reserved1                 : 3; /* Must be zero, according to datasheet */
	uint8_t fdRegionWriteAccess       : 1;
	uint8_t biosRegionWriteAccess     : 1;
	uint8_t meRegionWriteAccess       : 1;
	uint8_t gbeRegionWriteAccess      : 1;
	uint8_t pdRegionWriteAccess       : 1;
	uint8_t reserved2                 : 3; /* Must be zero, according to datasheet */
	/* most significant bits */
};

/* Master Access Section - starts at address FMBA(0x60) */
struct MASTERACCESSSECTIONRECORD
{
	struct FLMSTR flMstr1;                       /* Flash Master 1 (Host CPU / BIOS) - address 0x60 */
	struct FLMSTR flMstr2;                       /* Flash Master 2 (ME) - address 0x64 */
	struct FLMSTR flMstr3;                       /* Flash Master 3 (Gbe) - address 0x68 */
	uint8_t padding[148]; /* starts at 0x6c and then it's PCHSTRP0 which is 0x100 */
};

/* starts at address FPSBA (0x100) */
struct PCHSTRP0 
{
	/* least significant bits */
	                                             /* todo: add MeSmBus2Sel (boring setting) */
	uint8_t meDisable                 : 1; /* If true, ME is disabled. (it's a reset line, forcing the ME into a reset loop) */
    uint8_t chipConfigSoftstrap       : 1; /* Chipset configuration soft strap. must be set to 1 (my X220 factory.rom sets it to 1) */
	uint8_t reserved1                 : 5;
	uint8_t meSmbusSelect             : 1; /* configures if ME Smbus Segment is enabled. set this to 0? */

	uint8_t smBusAddress              : 7; /* The ME SmBus 7-bit address. */
	uint8_t bmcMode                   : 1; /* BMC mode: If true, device is in BMC mode.  If Intel(R) AMT or ASF using Intel integrated LAN then this should be false. */
	uint8_t tripPointSelect           : 1; /* Trip Point Select: false the NJCLK input buffer is matched to 3.3v signal from the external PHY device, true is matched to 1.8v. */
	uint8_t reserved2                 : 2;
	uint8_t integratedGbe             : 1; /* Integrated GbE or PCI Express select: (PCI Express,,Integrated GbE) Defines what PCIe Port 6 is used for. */
	uint8_t lanPhy                    : 1; /* LANPHYPC_GP12_SEL: Set to 0 for GP12 to be used as GPIO (General Purpose Input/Output), or 1 for GP12 to be used for native mode as LAN_PHYPC for 82566 LCD device */
	uint8_t reserved3                 : 3;
	uint8_t dmiRequesterId            : 1; /* DMI requestor ID security check disable: The primary purpose of this strap is to support server environments with multiple CPUs that each have a different RequesterID that can access the Flash. */
	uint8_t smBus2Address             : 7; /* The ME SmBus 2 7-bit address. */
	/* most significant bits */
};

struct PCHSTRP1 
{
	/* least significant bits */
	uint8_t northMlink                : 1;  /* North MLink Dynamic Clock Gate Disable : Sets the default value for the South MLink Dynamic Clock Gate Enable registers. */
	uint8_t southMlink                : 1;  /* South MLink Dynamic Clock Gate Enable : Sets the default value for the South MLink Dynamic Clock Gate Enable registers. */
	uint8_t meSmbus                   : 1;  /* ME SmBus Dynamic Clock Gate Enable : Sets the default value for the ME SMBus Dynamic Clock Gate Enable for both the ME SmBus controllers. */
	uint8_t sstDynamic                : 1;  /* SST Dynamic Clock Gate Enable : Sets the default value for the SST Clock Gate Enable registers. */
	uint8_t reserved1                 : 4;
	uint8_t northMlink2               : 1;  /* North MLink 2 Non-Posted Enable : 'true':North MLink supports two downstream non-posted requests. 'false':North MLink supports one downstream non-posted requests. */
	uint8_t reserved2                 : 7;
	uint16_t reserved3                : 16;
	/* most significant bits */
};

/* PCH straps */
/* starts at address FPSBA (0x100) */
struct PCHSTRAPSRECORD 
{
	struct PCHSTRP0 pchStrap0; /* address 0x100 */
	struct PCHSTRP1 pchStrap1; /* address 0x104 */
    uint32_t pchStrap2; /* address 0x108 */
    uint32_t pchStrap3; /* address 0x10c */
    uint32_t pchStrap4; /* address 0x110 */
    uint32_t pchStrap5; /* address 0x114 */
    uint32_t pchStrap6; /* address 0x118 */
    uint32_t pchStrap7; /* address 0x11c */
    uint32_t pchStrap8; /* address 0x120 */
    uint32_t pchStrap9; /* address 0x124 */
    uint32_t pchStrap10; /* address 0x128 */
    uint32_t pchStrap11; /* address 0x12c */
    uint32_t pchStrap12; /* address 0x130 */
    uint32_t pchStrap13; /* address 0x134 */
    uint32_t pchStrap14; /* address 0x138 */
    uint32_t pchStrap15; /* address 0x13c */
    uint32_t pchStrap16; /* address 0x140 */
    uint32_t pchStrap17; /* address 0x144 */
	uint8_t padding[184]; /* TODO: probably wrong. recalculate */
};

/* 32-bit little endian integer starting at address 0x200 */
struct PROCSTRAP0 
{
    /* TODO: investigate this. it's reserved and this is copied from ICH9 */
    /* knowing intel, bit 0 is probably still meDisable */

	/* least significant bits */
	uint8_t meDisable                 : 1;  /* If true, ME is disabled. */
	uint8_t meBootFromFlash           : 1;  /* ME boot from Flash - guessed location */
	uint8_t tpmDisable                : 1;  /* iTPM Disable : When set true, iTPM Host Interface is disabled. When set false (default), iTPM is enabled. */
	uint8_t reserved1                 : 3;
	uint8_t spiFingerprint            : 1;  /* SPI Fingerprint Sensor Present: Indicates if an SPI Fingerprint sensor is present at CS#1. */
	uint8_t meAlternateDisable        : 1;  /* ME Alternate Disable: Setting this bit allows ME to perform critical chipset functions but prevents loading of any ME FW applications. */
	uint8_t reserved2                 : 8;
	uint16_t reserved3                : 16;
	/* most significant bits */
};

/* Processor SoftStraps */
/* 32-bit little endian integer */
/* starts at address 0x200 in the flash chip */
struct PROCSTRAPSRECORD 
{
	struct PROCSTRAP0 procStrap0;
	uint8_t padding[3320];
};

/* 32-bit little endian integer */
/* starts at address FDBAR(0x0) + 0xEFC */
/* my factory image has this (dumped): DF 12 00 00 */
/* real value: 0x000012DF */
/* VTBA = 0xDF - address DF0 in the flash chip */
/* VTL  = 0x12 - 0x12 dwords - meaning, 18 32-bit little endian interegers (are they little endian? TODO: check endianness!) */
struct DESCRIPTORUPPERMAPRECORD /* only contains FLUMAP1 - Flash Upper Map 1 / Flash Descriptor Records */
{
    /* least significant bits */
    uint8_t VTBA      : 8; /* Intel ME VSCC Table Base Address. defines bits 4-11 (11:4 in intelspeak) for the VSCC Table. bits 0-3 are 0. So... whatever is in VTBA, OR it into 0x000 (12-bit integer) and shift left 4 times. In my factory image this value was 0xDF, so the address is 0xDF0. OR that into 0x0000 and bits 12-24 are also to remain at 0 */ 

   /* Sure enough, in my factory ROM image, I check 0xDF0 and it starts after a bunch of 0xFF's which begin at address 0x148 which is after pchStrap17 and within that is PROCSTRAP0 */

    /* this should be below FLUMAP1 and it is. it should be above offset of PROCSTRAP0 and it is */
    /* everything matches up (this is just Leah's mental notes while writing this. TODO: remove these lines when releasing the new version of Libreboot) */

    uint8_t VTL       : 8; /* Intel ME VSCC Table Length. number of dwords (32 bit integers) contained in the VSCC table. 0x12 - meaning 18. each SPI component entry in the table is 2 dwords (2 bytes) long */
    uint16_t reserved : 16;
    /* most significant bits */
}

/* TODO: this is defined by VTBA and VTL */
/*
// ME VSCC Table
struct MEVSCCTABLERECORD 
{
	uint32_t jid0;
	uint32_t vscc0;
	uint32_t jid1;
	uint32_t vscc1;
	uint32_t jid2;
	uint32_t vscc2;
	uint8_t padding[4];
};
*/

/* Descriptor Map 2 Record */
struct DESCRIPTORMAP2RECORD
{
	/* least significant bits */
	uint8_t meVsccTableBaseAddress    : 8;
	uint8_t meVsccTableLength         : 8;
	uint16_t reserved                 : 16;
	/* most significant bits */
};

/* 4KiB descriptor region, goes at the beginning of the ROM image */
struct DESCRIPTORREGIONRECORD 
{
	struct FLVALSIG flValSig;                                   /* Flash Valid Signature Register */
	struct FLMAPS flMaps;                                       /* Flash Map Registers */
	struct COMPONENTSECTIONRECORD componentSection;             /* Component Section Record */
	struct REGIONSECTIONRECORD regionSection;                   /* Flash Descriptor Region Section */
	struct MASTERACCESSSECTIONRECORD masterAccessSection;       /* Master Access Section */
	struct PCHSTRAPSRECORD pchStraps;                           /* ICH straps */
	struct PROCSTRAPSRECORD procStraps;                         /* Processor straps (was MCH straps, but functionality was merged into CPU) */
    struct DESCRIPTORUPPERMAPRECORD descriptorUpperMapSection;  /* Descriptor Upper Map Section */
	uint8_t oemSection[256];                         /* OEM section - address 0xF00 - 256 bytes. you can put text here, encoded in ASCII */
    /*
	struct MEVSCCTABLERECORD meVsccTable;                       // ME VSCC Table
	struct DESCRIPTORMAP2RECORD descriptor2Map;                 // Descriptor Map 2 Record
    */

};

/*
 * ---------------------------------------------------------------------
 * Function declarations (keep gcc/make happy. check them in descriptor.c)
 * ---------------------------------------------------------------------
 */
 
struct DESCRIPTORREGIONRECORD descriptorHostRegionsUnlocked(struct DESCRIPTORREGIONRECORD descriptorStruct);
struct DESCRIPTORREGIONRECORD descriptorMeRegionsForbidden(struct DESCRIPTORREGIONRECORD descriptorStruct);
struct DESCRIPTORREGIONRECORD descriptorDisableMe(struct DESCRIPTORREGIONRECORD descriptorStruct);
struct DESCRIPTORREGIONRECORD descriptorDisableTpm(struct DESCRIPTORREGIONRECORD descriptorStruct);
uint8_t componentDensity(unsigned int romSizeInBytes);
struct DESCRIPTORREGIONRECORD librebootDescriptorStructFromFactory(struct DESCRIPTORREGIONRECORD descriptorStruct);
int notCreatedHFileForDescriptorCFile(char* outFileName, char* cFileName);
int notCreatedCFileFromDescriptorStruct(struct DESCRIPTORREGIONRECORD descriptorStruct, char* outFileName, char* headerFileName);
void printDescriptorRegionLocations(struct DESCRIPTORREGIONRECORD descriptorStruct, char* romName);

#endif
