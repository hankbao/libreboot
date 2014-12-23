/*
 *  descriptor/struct.h
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
 
// Purpose: provide struct representing descriptor region.
// Map actual buffers of this regions, directly to instances of these
// structs. This makes working with descriptor really easy.
 
// bit fields used, corresponding to datasheet. See links to datasheets
// and documentation in ich9deblob.c

#ifndef DESCRIPTORSTRUCT_H
#define DESCRIPTORSTRUCT_H

#include <stdio.h>
#include <string.h>

#define DESCRIPTORREGIONSIZE 0x1000 // 4 KiB

// Related to the flash descriptor
#define FLREGIONBITSHIFT 0xC // bits 12(0xC)-24(0x18) are represented for words found in the flash descriptor
												 // To manipulate these easily in C, we shift them by FLREGIONBITSHIFT and then shift them back when done

struct FLVALSIG{
	unsigned int signature;
};

struct FLMAP0 {
	// least signicant bits
	unsigned char FCBA                      : 8;
	unsigned char NC                        : 2;
	unsigned char reserved1                 : 6;
	// ^^^^ unnamed members like these represent unused bits (per datasheet). 
	// the same applies for all structs in this file.
	unsigned char FRBA                      : 8;
	unsigned char NR                        : 3;
	unsigned char reserved2                 : 5;
	// most significant bits. 
	
	// the datasheet lists msb's first and lsb's last, in each table.
	// meanwhile, x86 gcc treats the members at the top of the struct as lsb's
	// and at the bottom of the struct, the members there are msb's. The same
	// fact applies to all the other structs below.
	
	// non-x86 (and/or non-gcc) is untested
	// little endian assumed
};

// ---------------------------------------------------------------------
// Descriptor struct representing the data
// ---------------------------------------------------------------------

struct FLMAP1 {
	unsigned char FMBA                      : 8;
	unsigned char NM                        : 3;
	unsigned char reserved                  : 5;
	unsigned char FISBA                     : 8;
	unsigned char ISL                       : 8;
};

struct FLMAP2 {
	unsigned char FMSBA                     : 8;
	unsigned char MSL                       : 8;
	unsigned short reserved                 : 16;
};

struct FLMAPS {
	struct FLMAP0 flMap0;
	struct FLMAP1 flMap1;
	struct FLMAP2 flMap2;
};

struct FLCOMP {
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
};

struct COMPONENTSECTIONRECORD {
	struct FLCOMP flcomp;
	unsigned int flill;
	unsigned int flpb;
	unsigned char padding[36];
};

struct FLREG {
	unsigned short BASE                     : 13;
	unsigned short reserved1                : 3;
	unsigned short LIMIT                    : 13;
	unsigned short reserved2                : 3;
};

struct REGIONSECTIONRECORD {
	struct FLREG flReg0;                         //  Descriptor 
	struct FLREG flReg1;                         //  BIOS
	struct FLREG flReg2;                         //  ME
	struct FLREG flReg3;                         //  Gbe
	struct FLREG flReg4;                         //  Platform
	unsigned char padding[12];
};

struct FLMSTR {
    unsigned short requesterId             : 16;
	unsigned char fdRegionReadAccess        : 1;
	unsigned char biosRegionReadAccess      : 1;
	unsigned char meRegionReadAccess        : 1;
	unsigned char gbeRegionReadAccess       : 1;
	unsigned char pdRegionReadAccess        : 1;
	unsigned char reserved1                 : 3;
	unsigned char fdRegionWriteAccess       : 1;
	unsigned char biosRegionWriteAccess     : 1;
	unsigned char meRegionWriteAccess       : 1;
	unsigned char gbeRegionWriteAccess      : 1;
	unsigned char pdRegionWriteAccess       : 1;
	unsigned char reserved2                 : 3;
};


struct MASTERACCESSSECTIONRECORD {
	struct FLMSTR flMstr1;
	struct FLMSTR flMstr2;
	struct FLMSTR flMstr3;
	unsigned char padding[148];
};

struct ICHSTRAP0 {
	                                             // todo: add MeSmBus2Sel (boring setting)
	unsigned char meDisable                 : 1; // If true, ME is disabled.
	unsigned char reserved1                 : 6;
	unsigned char tcoMode                   : 1; // TCO Mode: (Legacy,TCO Mode) The TCO Mode, along with the BMCMODE strap, determines the behavior of the IAMT SmBus controller.
	unsigned char smBusAddress              : 7; // The ME SmBus 7-bit address.
	unsigned char bmcMode                   : 1; // BMC mode: If true, device is in BMC mode.  If Intel(R) AMT or ASF using Intel integrated LAN then this should be false.
	unsigned char tripPointSelect           : 1; // Trip Point Select: false the NJCLK input buffer is matched to 3.3v signal from the external PHY device, true is matched to 1.8v.
	unsigned char reserved2                 : 2;
	unsigned char integratedGbe             : 1; // Integrated GbE or PCI Express select: (PCI Express,,Integrated GbE) Defines what PCIe Port 6 is used for.
	unsigned char lanPhy                    : 1; // LANPHYPC_GP12_SEL: Set to 0 for GP12 to be used as GPIO (General Purpose Input/Output), or 1 for GP12 to be used for native mode as LAN_PHYPC for 82566 LCD device
	unsigned char reserved3                 : 3;
	unsigned char dmiRequesterId            : 1; // DMI requestor ID security check disable: The primary purpose of this strap is to support server environments with multiple CPUs that each have a different RequesterID that can access the Flash.
	unsigned char smBus2Address             : 7; // The ME SmBus 2 7-bit address.
};

struct ICHSTRAP1 {
	unsigned char northMlink                : 1;  // North MLink Dynamic Clock Gate Disable : Sets the default value for the South MLink Dynamic Clock Gate Enable registers.
	unsigned char southMlink                : 1;  // South MLink Dynamic Clock Gate Enable : Sets the default value for the South MLink Dynamic Clock Gate Enable registers.
	unsigned char meSmbus                   : 1;  // ME SmBus Dynamic Clock Gate Enable : Sets the default value for the ME SMBus Dynamic Clock Gate Enable for both the ME SmBus controllers.
	unsigned char sstDynamic                : 1;  // SST Dynamic Clock Gate Enable : Sets the default value for the SST Clock Gate Enable registers.
	unsigned char reserved1                 : 4;
	unsigned char northMlink2               : 1;  // North MLink 2 Non-Posted Enable : 'true':North MLink supports two downstream non-posted requests. 'false':North MLink supports one downstream non-posted requests.
	unsigned char reserved2                 : 7;
	unsigned short reserved3                : 16;
};


struct ICHSTRAPSRECORD {
	struct ICHSTRAP0 ichStrap0;
	struct ICHSTRAP1 ichStrap1;
	unsigned char padding[248];
};

struct MCHSTRAP0 {
	unsigned char meDisable                 : 1;  // If true, ME is disabled.
	unsigned char meBootFromFlash           : 1;  // ME boot from Flash - guessed location
	unsigned char tpmDisable                : 1;  // iTPM Disable : When set true, iTPM Host Interface is disabled. When set false (default), iTPM is enabled.
	unsigned char reserved1                 : 3;
	unsigned char spiFingerprint            : 1;  // SPI Fingerprint Sensor Present: Indicates if an SPI Fingerprint sensor is present at CS#1.
	unsigned char meAlternateDisable        : 1;  // ME Alternate Disable: Setting this bit allows ME to perform critical chipset functions but prevents loading of any ME FW applications.
	unsigned char reserved2                 : 8;
	unsigned short reserved3                : 16;
};

struct MCHSTRAPSRECORD {
	struct MCHSTRAP0 mchStrap0;
	unsigned char padding[3292];
};

struct MEVSCCTABLERECORD {
	unsigned int jid0;
	unsigned int vscc0;
	unsigned int jid1;
	unsigned int vscc1;
	unsigned int jid2;
	unsigned int vscc2;
	unsigned char padding[4];
};

struct DESCRIPTORMAP2RECORD {
	unsigned char meVsccTableBaseAddress    : 8;
	unsigned char meVsccTableLength         : 8;
	unsigned short reserved                 : 16;
};

struct OEMSECTIONRECORD {
	unsigned char magicString[8];
	unsigned char padding[248];
};

struct DESCRIPTORREGIONRECORD {
	struct FLVALSIG flValSig;
	struct FLMAPS flMaps;
	struct COMPONENTSECTIONRECORD componentSection;
	struct REGIONSECTIONRECORD regionSection;
	struct MASTERACCESSSECTIONRECORD masterAccessSection;
	struct ICHSTRAPSRECORD ichStraps;
	struct MCHSTRAPSRECORD mchStraps;
	struct MEVSCCTABLERECORD meVsccTable;
	struct DESCRIPTORMAP2RECORD descriptor2Map;
	struct OEMSECTIONRECORD oemSection;
};

// ---------------------------------------------------------------------
// Descriptor functions
// ---------------------------------------------------------------------

// Modify the flash descriptor, to remove the ME/AMT, and disable all other regions
// Only Flash Descriptor, Gbe and BIOS regions (BIOS region fills factoryRomSize-12k) are left.
// Tested on ThinkPad X200 and X200S. X200T and other GM45 targets may also work.
struct DESCRIPTORREGIONRECORD deblobbedDescriptorStructFromFactory(struct DESCRIPTORREGIONRECORD factoryDescriptorStruct, unsigned int factoryRomSize)
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
	deblobbedDescriptorStruct.regionSection.flReg1.LIMIT = ((factoryRomSize >> FLREGIONBITSHIFT) - 1);
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
	
	// debugging
	printf("\nOriginal (factory.rom) Descriptor start block: %08x ; Descriptor end block: %08x\n", factoryDescriptorStruct.regionSection.flReg0.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg0.LIMIT << FLREGIONBITSHIFT);
	printf("Original (factory.rom) BIOS start block: %08x ; BIOS end block: %08x\n", factoryDescriptorStruct.regionSection.flReg1.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg1.LIMIT << FLREGIONBITSHIFT);
	printf("Original (factory.rom) ME start block: %08x ; ME end block: %08x\n", factoryDescriptorStruct.regionSection.flReg2.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg2.LIMIT << FLREGIONBITSHIFT);
	printf("Original (factory.rom) GBe start block: %08x ; GBe end block: %08x\n", factoryDescriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT, factoryDescriptorStruct.regionSection.flReg3.LIMIT << FLREGIONBITSHIFT);
	
	printf("\nRelocated (libreboot.rom) Descriptor start block: %08x ; Descriptor end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg0.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg0.LIMIT << FLREGIONBITSHIFT);
	printf("Relocated (libreboot.rom) BIOS start block: %08x ; BIOS end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg1.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg1.LIMIT << FLREGIONBITSHIFT);
	printf("Relocated (libreboot.rom) ME start block: %08x ; ME end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg2.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg2.LIMIT << FLREGIONBITSHIFT);
	printf("Relocated (libreboot.rom) GBe start block: %08x ; GBe end block: %08x\n", deblobbedDescriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT, deblobbedDescriptorStruct.regionSection.flReg3.LIMIT << FLREGIONBITSHIFT);
	
	return deblobbedDescriptorStruct;
}

#endif
