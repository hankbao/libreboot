/*
 *  descriptor/descriptor.c
 *  This file is part of the intel6deblob utility from the libreboot project
 *
 *	Copyright (C) 2014, 2015, 2017 Leah Rowe <info@minifree.org>
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
 * Provide descriptor related functions.
 */

/* structs describing the data in descriptor region */
#include "descriptor.h"

/*
 * ---------------------------------------------------------------------
 * Descriptor related functions
 * ---------------------------------------------------------------------
 */

/* Set the Host CPU / BIOS region to have read-write access on all regions */
struct DESCRIPTORREGIONRECORD descriptorHostRegionsUnlocked(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
   descriptorStruct.masterAccessSection.flMstr1.fdRegionReadAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.biosRegionReadAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.meRegionReadAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.gbeRegionReadAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.pdRegionReadAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.fdRegionWriteAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.biosRegionWriteAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.meRegionWriteAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.gbeRegionWriteAccess = 0x1;
   descriptorStruct.masterAccessSection.flMstr1.pdRegionWriteAccess = 0x1;

    /* Recommended by Intel, if all regions are opened (they are) */
    /* that is, set both of them (3-bit values) to 111b */

    descriptorStruct.masterAccessSection.flMstr1.reserved1 = 7;
    descriptorStruct.masterAccessSection.flMstr1.reserved2 = 7;

   return descriptorStruct;
}

/* Set the ME to have *no* read-write access on any region */
struct DESCRIPTORREGIONRECORD descriptorMeRegionsForbidden(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
   descriptorStruct.masterAccessSection.flMstr2.fdRegionReadAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.biosRegionReadAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.meRegionReadAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.gbeRegionReadAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.pdRegionReadAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.fdRegionWriteAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.biosRegionWriteAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.meRegionWriteAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.gbeRegionWriteAccess = 0x0;
   descriptorStruct.masterAccessSection.flMstr2.pdRegionWriteAccess = 0x0;

   return descriptorStruct;
}

/* Assault the ME in PCHSTRAP0, PCHSTRAP10, PCHSTRAP2 and PROCSTRAP0 */
struct DESCRIPTORREGIONRECORD descriptorDisableMe(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
    /* These are adapted from ICH9 and are totally guessed */
	descriptorStruct.pchStraps.pchStrap0.reservedGuessedMeDisable = 1;
	descriptorStruct.procStraps.procStrap0.reservedGuessedMeDisable = 1;

    /* These are guesses, based on new entries in the sandybridge descriptor */
    /* This is another guessed ME Disable bit (reset line) */
    descriptorStruct.pchStraps.pchStrap10.reserved = 1;

    /* This is so that the ME does not load its own boot ROM (probably harmless to set): */
    descriptorStruct.pchStraps.pchStrap10.meBootFlash = 1; /* only load SPI */

    /* clocking parameters that the platform will boot with. it may be necessary to change this. */
    /* my x220 factory ROM has this set to 000 */
    // descriptorStruct.pchStraps.pchStrap10.integratedClockingConfigurationSelect = 7;
    /* NOTE: iccProfileSelection is set to 1, meaning that integratedClockingConfigurationSelect
    in PCH strap 10 sets this setting. If iccProfileSelection were 0, then the BIOS
    (coreboot) would have to set this. */

    /*
    NOTE: meDebugLanEmergencyMode is interesting (in PCH strap 10). If you set this to 1,
    you can use a ME Debug tool to capture events on the Intel NIC, over the network.
    In other words, this turns the ME into a super backdoor. But it might let us
    explore vulnerabilities in the ME. It's set to 0 by default, especially on
    production systems (and set to 0 by default in Libreboot)
    */

    /*
    X220 factory ROM red flag:
    NOTE: PCH strap 2
    intelMeSmbusMctpAddressEnable is set to 1
    0101011 (0x2B)
    See: https://en.wikipedia.org/wiki/Management_Component_Transport_Protocol
    TEST CASE:
    */
    descriptorStruct.pchStraps.pchStrap2.intelMeSmbusMctpAddressEnable = 0;
    descriptorStruct.pchStraps.pchStrap2.intelMeSmbusMctpAddress = 0;

    /* Disable ME Smbus segment. Intel says not to, so let's do it: */
    descriptorStruct.pchStraps.pchStrap0.meSmbusSelect = 0;

    /* Intel says this should be set to 01 (100kHz). Maybe setting it to something
    else will trip up the ME and make it crash? */
    descriptorStruct.pchStraps.pchStrap0.intelMeSmbusFrequency = 0; /* no idea what speed this is. it's reserved by Intel. TODO: investigate this setting more */

    /*
        NOTE: bits 23-31 in PCH strap 9 are "reserved". Here's are the bits that
        my factory X220 ROM has there: 001100000
        TODO: investigate
    */

    /*
        NOTE: pch strap 10 has this in bit 0-7: 44 (01000100)
        bit 2-7 is 010001 and these bits are "reserved"
        TODO: investigate what these bits do. They're right next
        to ME-related settings.
        TODO: maybe try setting them to all 1s and see what happens
        e.g. check kernel logs and see difference?
    */

    /*
        TODO: investigate integrated clock mode select in pch strap 17
        it's set to 0, which is default, but what happens if we set this to 1?
        also investigate the pchhot/sml1alert setting
    */

    /* Remove the ME region in the SPI flash: */
    descriptorStruct.regionSection.flReg2.BASE = 0x1FFF;
    descriptorStruct.regionSection.flReg2.LIMIT = 0;

	return descriptorStruct;
}

/* Disable the TPM in MCHSTRAP0 */
struct DESCRIPTORREGIONRECORD descriptorDisableTpm(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
	descriptorStruct.procStraps.procStrap0.reservedGuessedTpmDisable = 1;

	return descriptorStruct;
}

/* Relocate the Gbe region to begin at 4KiB (immediately after the flash descriptor) */
struct DESCRIPTORREGIONRECORD descriptorMoveGbeToStart(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
	descriptorStruct.regionSection.flReg3.BASE = DESCRIPTORREGIONSIZE >> FLREGIONBITSHIFT;
	descriptorStruct.regionSection.flReg3.LIMIT = GBEREGIONSIZE_8K >> FLREGIONBITSHIFT;

	return descriptorStruct;
}

/* BIOS Region begin after descriptor+gbe at first 12KiB, fills the rest of the image */
struct DESCRIPTORREGIONRECORD descriptorBiosRegionFillImageAfterGbe(struct DESCRIPTORREGIONRECORD descriptorStruct, unsigned int romSize)
{
	descriptorStruct.regionSection.flReg1.BASE = (DESCRIPTORREGIONSIZE + GBEREGIONSIZE_8K) >> FLREGIONBITSHIFT;
	descriptorStruct.regionSection.flReg1.LIMIT = (romSize >> FLREGIONBITSHIFT) - 1;

	return descriptorStruct;
}

/* Set OEM string to "LIBERATE" */
struct DESCRIPTORREGIONRECORD descriptorOemString(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
	int i;
	descriptorStruct.oemSection[0] = 0x4C;
	descriptorStruct.oemSection[1] = 0x49;
	descriptorStruct.oemSection[2] = 0x42;
	descriptorStruct.oemSection[3] = 0x45;
	descriptorStruct.oemSection[4] = 0x52;
	descriptorStruct.oemSection[5] = 0x41;
	descriptorStruct.oemSection[6] = 0x54;
	descriptorStruct.oemSection[7] = 0x45;
	for (i=8;i<256;i++) descriptorStruct.oemSection[i]=0xFF;

	return descriptorStruct;
}

uint8_t componentDensity(unsigned int romSizeInBytes)
{
	/* component density, see Component Section Record. page 848 in the datasheet */
	switch (romSizeInBytes)
	{
		case ROMSIZE_512KB: return 0;
		case ROMSIZE_1MB: return 1;
		case ROMSIZE_2MB: return 2;
		case ROMSIZE_4MB: return 3;
		case ROMSIZE_8MB: return 4;
		case ROMSIZE_16MB: return 5;
		default: return 0x7; /* reserved value */
	}
}

/* From a factory.rom image, create a modified descriptor region, suitable
 * for use by the libreboot project */
struct DESCRIPTORREGIONRECORD librebootDescriptorStructFromFactory(struct DESCRIPTORREGIONRECORD descriptorStruct, unsigned int romSize)
{
	/* Disable the ME itself, so that it doesn't try to start when this descriptor is in use */
	descriptorStruct = descriptorDisableMe(descriptorStruct);
	/* Also disable the TPM, by default */
	descriptorStruct = descriptorDisableTpm(descriptorStruct);

	descriptorStruct = descriptorOemString(descriptorStruct);

    /* disable platform data region */
    descriptorStruct.regionSection.flReg4.BASE = 0x1fff;
    descriptorStruct.regionSection.flReg4.LIMIT = 0;

	/* 
	 * set number of regions from 4 -> 2 (0 based, so 4 means 5 and 2
	 * means 3. We want 3 regions: descriptor, gbe and bios, in that order)
	 */
	descriptorStruct.flMaps.flMap0.NR = 2;
	/* Move GbE region to the start of the image (after the descriptor) */
	descriptorStruct = descriptorMoveGbeToStart(descriptorStruct);
	/* BIOS region fills the remaining space */
	descriptorStruct = descriptorBiosRegionFillImageAfterGbe(descriptorStruct, romSize);

	/* unlock regions (set read-write for BIOS / Host CPU */
	descriptorStruct = descriptorHostRegionsUnlocked(descriptorStruct);
	/* tell the ME that it cannot access any regions */
	descriptorStruct = descriptorMeRegionsForbidden(descriptorStruct);

	return descriptorStruct;
}

/*
 * ---------------------------------------------------------------------
 * C code generator (self-writing code)
 * ---------------------------------------------------------------------
 */

/*
 * Generate a C (.h) header file for the C source file made by notCreatedCFileFromDescriptorStruct()
 *
 * Output it to a file.
 */
int notCreatedHFileForDescriptorCFile(char* outFileName, char* cFileName)
{
	remove(outFileName); /* Remove the old file before continuing */

	/* Open the file that will be written to */
	FILE* fp = fopen(outFileName, "w+");

	/* ------------------------------ */

	fprintf(fp, "/* %s: generated C code from intel6deblob */\n", outFileName);
	fprintf(fp, "/* .h header file for the descriptor-generating C code (%s) */\n\n", cFileName);

	fprintf(fp, "#ifndef INTEL6GEN_MKDESCRIPTOR_H\n");
	fprintf(fp, "#define INTEL6GEN_MKDESCRIPTOR_H\n\n");

	fprintf(fp, "#include <stdio.h>\n");
	fprintf(fp, "#include <string.h>\n");
	fprintf(fp, "#include \"../descriptor/descriptor.h\"\n\n");

	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStruct(unsigned int romSize);\n");

	fprintf(fp, "#endif\n");

	/* ------------------------------ */

	fclose(fp); /* Always close the file when done. */

	return 0;
}

/*
 * Generate a C source file that initializes the same data from a given
 * 4KiB Descriptor data structure.
 *
 * Output it to a file.
 */
int notCreatedCFileFromDescriptorStruct(struct DESCRIPTORREGIONRECORD descriptorStruct, char* outFileName, char* headerFileName)
{
	int i, j;

	remove(outFileName); /* Remove the old file before continuing */

	/* Open the file that will be written to */
	FILE* fp = fopen(outFileName, "w+");

	/* ------------------------------ */

	fprintf(fp, "/* %s: generated C code from intel6deblob */\n", outFileName);
	fprintf(fp, "/* .c source file for the descriptor-generating C code */\n\n");

	fprintf(fp, "#include \"%s\"\n\n", headerFileName);

	fprintf(fp, "/* Generate a 4KiB Descriptor struct, with default values. */\n");
	fprintf(fp, "/* Read ../descriptor/descriptor.h for an explanation of the default values used here */\n\n");

	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStruct(unsigned int romSize)\n");
	fprintf(fp, "{\n");
	fprintf(fp, "    int i;\n");
	fprintf(fp, "    struct DESCRIPTORREGIONRECORD descriptorStruct;\n");
	fprintf(fp, "\n");

	/* Flash Valid Signature Register */
	fprintf(fp, "    /* Flash Valid Signature Register */\n");
	for (i = 0; i < 16; i++) {
		if (descriptorStruct.padding[i] != 0xFF) {
			for (j = 0; j < 16; j++) {
				fprintf(fp, "    descriptorStruct.padding[%d] = 0x%02x;\n", j, descriptorStruct.padding[j]);
			}
			break;
		} else if (i == 15) {
			fprintf(fp, "    for (i = 0; i < 16; i++) {\n");
			fprintf(fp, "        descriptorStruct.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "    descriptorStruct.flValSig.signature = 0x%08x;\n", descriptorStruct.flValSig.signature);
	fprintf(fp, "\n");

	/* Flash Map Registers */
	fprintf(fp, "    /* Flash Map Registers */\n");
	fprintf(fp, "    /* FLMAP0 */\n");
	fprintf(fp, "    descriptorStruct.flMaps.flMap0.FCBA = 0x%02x;\n", descriptorStruct.flMaps.flMap0.FCBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap0.NC = 0x%01x;\n", descriptorStruct.flMaps.flMap0.NC);
	fprintf(fp, "    descriptorStruct.flMaps.flMap0.reserved1 = 0x%02x;\n", descriptorStruct.flMaps.flMap0.reserved1);
	fprintf(fp, "    descriptorStruct.flMaps.flMap0.FRBA = 0x%02x;\n", descriptorStruct.flMaps.flMap0.FRBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap0.NR = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.flMaps.flMap0.NR);
	fprintf(fp, "    descriptorStruct.flMaps.flMap0.reserved2 = 0x%02x;\n", descriptorStruct.flMaps.flMap0.reserved2);
	fprintf(fp, "    /* FLMAP1 */\n");
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.FMBA = 0x%02x;\n", descriptorStruct.flMaps.flMap1.FMBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.NM = 0x%01x;\n", descriptorStruct.flMaps.flMap1.NM);
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.reserved = 0x%02x;\n", descriptorStruct.flMaps.flMap1.reserved);
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.FPSBA = 0x%02x;\n", descriptorStruct.flMaps.flMap1.FPSBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.ISL = 0x%02x;\n", descriptorStruct.flMaps.flMap1.ISL);
	fprintf(fp, "    /* FLMAP2 */\n");
	fprintf(fp, "    descriptorStruct.flMaps.flMap2.FPSBA = 0x%02x;\n", descriptorStruct.flMaps.flMap2.FPSBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap2.PSL = 0x%02x;\n", descriptorStruct.flMaps.flMap2.PSL);
	fprintf(fp, "    descriptorStruct.flMaps.flMap2.reserved = 0x%04x;\n", descriptorStruct.flMaps.flMap2.reserved);
	fprintf(fp, "    /* FLMAP3 */\n");
	fprintf(fp, "    descriptorStruct.flMaps.flMap3.reserved = 0x%08x;\n", descriptorStruct.flMaps.flMap3.reserved);
	fprintf(fp, "\n");
   
	for (i = 0; i < 6; i++) {
		if (descriptorStruct.padding2[i] != 0xFF) {
			for (j = 0; j < 6; j++) {
				fprintf(fp, "    descriptorStruct.padding2[%d] = 0x%02x;\n", j, descriptorStruct.padding2[j]);
			}
			break;
		} else if (i == 5) {
			fprintf(fp, "    for (i = 0; i < 16; i++) {\n");
			fprintf(fp, "        descriptorStruct.padding2[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}

	/* Component Section Record */
	fprintf(fp, "    /* Component Section Record */\n");
	fprintf(fp, "    /* FLCOMP */\n");
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.component1Density = 0x%01x;\n", descriptorStruct.componentSection.flcomp.component1Density);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.component2Density = 0x%01x;\n", descriptorStruct.componentSection.flcomp.component2Density);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.reserved1 = 0x%01x;\n", descriptorStruct.componentSection.flcomp.reserved1);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.reserved2 = 0x%02x;\n", descriptorStruct.componentSection.flcomp.reserved2);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.reserved3 = 0x%01x;\n", descriptorStruct.componentSection.flcomp.reserved3);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.readClockFrequency = 0x%01x;\n", descriptorStruct.componentSection.flcomp.readClockFrequency);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.fastReadSupport = 0x%01x;\n", descriptorStruct.componentSection.flcomp.fastReadSupport);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.fastreadClockFrequency = 0x%01x;\n", descriptorStruct.componentSection.flcomp.fastreadClockFrequency);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.writeEraseClockFrequency = 0x%01x;\n", descriptorStruct.componentSection.flcomp.writeEraseClockFrequency);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.readIdReadStatusClkFreq = 0x%01x;\n", descriptorStruct.componentSection.flcomp.readIdReadStatusClkFreq);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.singleIoFastReadSupport = 0x%01x;\n", descriptorStruct.componentSection.flcomp.singleIoFastReadSupport);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.reserved4 = 0x%01x;\n", descriptorStruct.componentSection.flcomp.reserved4);

    fprintf(fp, "    /* FLILL */\n");
	fprintf(fp, "    descriptorStruct.componentSection.flill.invalidInstruction0 = 0x%02x;\n", descriptorStruct.componentSection.flill.invalidInstruction0);
	fprintf(fp, "    descriptorStruct.componentSection.flill.invalidInstruction1 = 0x%02x;\n", descriptorStruct.componentSection.flill.invalidInstruction1);
	fprintf(fp, "    descriptorStruct.componentSection.flill.invalidInstruction2 = 0x%02x;\n", descriptorStruct.componentSection.flill.invalidInstruction2);
	fprintf(fp, "    descriptorStruct.componentSection.flill.invalidInstruction3 = 0x%02x;\n", descriptorStruct.componentSection.flill.invalidInstruction3);

	fprintf(fp, "    /* FLPB */\n");
	fprintf(fp, "    descriptorStruct.componentSection.flpb.FPBA = 0x%04x;\n", descriptorStruct.componentSection.flpb.FPBA);
	fprintf(fp, "    descriptorStruct.componentSection.flpb.reserved = 0x%05x;\n", descriptorStruct.componentSection.flpb.reserved);

	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 4; i++) {
		if (descriptorStruct.componentSection.padding[i] != 0xFF) {
			for (j = 0; j < 4; j++) {
				fprintf(fp, "    descriptorStruct.componentSection.padding[%d] = 0x%02x;\n", j, descriptorStruct.componentSection.padding[j]);
			}
			break;
		} else if (i == 3) {
			fprintf(fp, "    for (i = 0; i < 4; i++) {\n");
			fprintf(fp, "        descriptorStruct.componentSection.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");

	/* Flash Descriptor Region Section */
	fprintf(fp, "    /* Flash Descriptor Region Section */\n");
	fprintf(fp, "    /* FLREG0 (Descriptor) */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg0.BASE = 0x%04x;\n", descriptorStruct.regionSection.flReg0.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg0.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg0.reserved1);
	fprintf(fp, "    descriptorStruct.regionSection.flReg0.LIMIT = 0x%04x;\n", descriptorStruct.regionSection.flReg0.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg0.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg0.reserved2);
	fprintf(fp, "    /* FLREG1 (BIOS) */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.BASE = 0x%04x;\n", descriptorStruct.regionSection.flReg1.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg1.reserved1);
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.LIMIT = 0x%04x;\n", descriptorStruct.regionSection.flReg1.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg1.reserved2);
	fprintf(fp, "    /* FLREG2 (ME) */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.BASE = 0x%04x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.regionSection.flReg2.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg2.reserved1);
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.LIMIT = 0x%04x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.regionSection.flReg2.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg2.reserved2);
	fprintf(fp, "    /* FLREG3 (Gbe) */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.BASE = 0x%04x;\n", descriptorStruct.regionSection.flReg3.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg3.reserved1);
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.LIMIT = 0x%04x;\n", descriptorStruct.regionSection.flReg3.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg3.reserved2);
	fprintf(fp, "    /* FLREG4 (Platform) */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg4.BASE = 0x%04x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.regionSection.flReg4.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg4.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg4.reserved1);
	fprintf(fp, "    descriptorStruct.regionSection.flReg4.LIMIT = 0x%04x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.regionSection.flReg4.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg4.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg4.reserved2);
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 12; i++) {
		if (descriptorStruct.regionSection.padding[i] != 0xFF) {
			for (j = 0; j < 12; j++) {
				fprintf(fp, "    descriptorStruct.regionSection.padding[%d] = 0x%02x;\n", j, descriptorStruct.regionSection.padding[j]);
			}
			break;
		} else if (i == 11) {
			fprintf(fp, "    for (i = 0; i < 12; i++) {\n");
			fprintf(fp, "        descriptorStruct.regionSection.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");

	/* Master Access Section */
	fprintf(fp, "    /* Master Access Section */\n");
	fprintf(fp, "    /* FLMSTR1 (Host CPU / BIOS) */\n");
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.requesterId = 0x%04x;\n", descriptorStruct.masterAccessSection.flMstr1.requesterId);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.fdRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.fdRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.biosRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.biosRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.meRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.meRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.gbeRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.gbeRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.pdRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.pdRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.reserved1 = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr1.reserved1);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.fdRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.fdRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.biosRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.biosRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.meRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.meRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.gbeRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.gbeRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.pdRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr1.pdRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr1.reserved2 = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr1.reserved2);
	fprintf(fp, "    /* FLMSTR2 (ME) */\n");
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.requesterId = 0x%04x;\n", descriptorStruct.masterAccessSection.flMstr2.requesterId);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.fdRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.fdRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.biosRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.biosRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.meRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.meRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.gbeRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.gbeRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.pdRegionReadAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.pdRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.reserved1 = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr2.reserved1);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.fdRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.fdRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.biosRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.biosRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.meRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.meRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.gbeRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.gbeRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.pdRegionWriteAccess = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.masterAccessSection.flMstr2.pdRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr2.reserved2 = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr2.reserved2);
	fprintf(fp, "    /* FLMSTR3 (Gbe) */\n");
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.requesterId = 0x%04x;\n", descriptorStruct.masterAccessSection.flMstr3.requesterId);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.fdRegionReadAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.fdRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.biosRegionReadAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.biosRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.meRegionReadAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.meRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.gbeRegionReadAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.gbeRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.pdRegionReadAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.pdRegionReadAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.reserved1 = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.reserved1);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.fdRegionWriteAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.fdRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.biosRegionWriteAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.biosRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.meRegionWriteAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.meRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.gbeRegionWriteAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.gbeRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.pdRegionWriteAccess = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.pdRegionWriteAccess);
	fprintf(fp, "    descriptorStruct.masterAccessSection.flMstr3.reserved2 = 0x%01x;\n", descriptorStruct.masterAccessSection.flMstr3.reserved2);
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 148; i++) {
		if (descriptorStruct.masterAccessSection.padding[i] != 0xFF) {
			for (j = 0; j < 148; j++) {
				fprintf(fp, "    descriptorStruct.masterAccessSection.padding[%d] = 0x%02x;\n", j, descriptorStruct.masterAccessSection.padding[j]);
			}
			break;
		} else if (i == 147) {
			fprintf(fp, "    for (i = 0; i < 148; i++) {\n");
			fprintf(fp, "        descriptorStruct.masterAccessSection.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");

    /* PCH straps */

    /* PCHSTRP0 */
    fprintf(fp, "   /* PCHSTRP1 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.reservedGuessedMeDisable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.reservedGuessedMeDisable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.chipConfigSoftstrap1 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.chipConfigSoftstrap1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.reserved1 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap0.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.meSmbusSelect = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.meSmbusSelect);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.meSmlink0enable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.meSmlink0enable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.meSmlink1enable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.meSmlink1enable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.smlink1frequency = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.smlink1frequency);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.intelMeSmbusFrequency = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.intelMeSmbusFrequency);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.smlink0frequency = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.smlink0frequency);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.reserved2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.reserved2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.lanPhyPwrCtrlGpio12Sel = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.lanPhyPwrCtrlGpio12Sel);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.linkSecDisable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.linkSecDisable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.reserved3 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.reserved3);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.dmiRequesterIdChkDisable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.dmiRequesterIdChkDisable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.reserved4 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.reserved4);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.biosBootBlockSize = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.biosBootBlockSize);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap0.reserved5 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap0.reserved5);
    fprintf(fp, "\n");

    /* PCHSTRP1 */
    fprintf(fp, "   /* PCHSTRP1 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap1.chipConfigSoftstrap2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap1.chipConfigSoftstrap2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap1.unknown = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap1.unknown);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap1.chipConfigSoftstrap3 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap1.chipConfigSoftstrap3);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap1.reserved1 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap1.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap1.reserved2 = 0x%04x;\n", descriptorStruct.pchStraps.pchStrap1.reserved2);
    fprintf(fp, "\n");

    /* PCHSTRP2 */
    fprintf(fp, "   /* PCHSTRP2 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.reserved = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap2.reserved);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.intelMeSmbusAsdEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap2.intelMeSmbusAsdEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.intelMeSmbusAsdAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap2.intelMeSmbusAsdAddress);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.intelMeSmbusMctpAddressEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap2.intelMeSmbusMctpAddressEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.intelMeSmbusMctpAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap2.intelMeSmbusMctpAddress);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.intelMeSmbusI2cAddressEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap2.intelMeSmbusI2cAddressEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap2.intelMeSmbusI2cAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap2.intelMeSmbusI2cAddress);
    fprintf(fp, "\n");

    /* PCHSTRP3 */
    fprintf(fp, "   /* PCHSTRP3 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap3.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap3.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP4 */
    fprintf(fp, "   /* PCHSTRP4 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.intelPhyConnectivity = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap4.intelPhyConnectivity);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.reserved1 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap4.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.gbeMacSmbusAddressEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap4.gbeMacSmbusAddressEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.gbeMacSmbusAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap4.gbeMacSmbusAddress);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.reserved2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap4.reserved2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.gbePhySmbusAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap4.gbePhySmbusAddress);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap4.reserved3 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap4.reserved3);
    fprintf(fp, "\n");

    /* PCHSTRP5 */
    fprintf(fp, "   /* PCHSTRP5 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap5.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap5.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP6 */
    fprintf(fp, "   /* PCHSTRP6 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap6.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap6.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP7 */
    fprintf(fp, "   /* PCHSTRP7 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap7.intelMeSubsystemVendorId = 0x%04x;\n", descriptorStruct.pchStraps.pchStrap7.intelMeSubsystemVendorId);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap7.intelMeSubsystemDeviceId = 0x%04x;\n", descriptorStruct.pchStraps.pchStrap7.intelMeSubsystemDeviceId);
    fprintf(fp, "\n");

    /* PCHSTRP8 */
    fprintf(fp, "   /* PCHSTRP8 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap8.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap8.reserved);
    fprintf(fp, "\n");

    fprintf(fp, "   /* PCHSTRP9 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.pciExpressPortConfigurationStrap1 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.pciExpressPortConfigurationStrap1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.pciExpressPortConfigurationStrap2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.pciExpressPortConfigurationStrap2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.pcieLaneReversal1 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.pcieLaneReversal1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.pcieLaneReversal2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.pcieLaneReversal2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.dmiAndIntelFdiReversal = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.dmiAndIntelFdiReversal);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.chipConfigSoftstrap4 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.chipConfigSoftstrap4);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.intelPhyPciePortSelect = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.intelPhyPciePortSelect);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.intelPhyOverPciExpressEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.intelPhyOverPciExpressEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.reserved1 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.subtractiveDecodeAgentEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.subtractiveDecodeAgentEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.reserved2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.reserved2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.reserved3 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap9.reserved3);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.pchHotOrSml1AlertSelect = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap9.pchHotOrSml1AlertSelect);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap9.reserved4 = 0x%03x;\n", descriptorStruct.pchStraps.pchStrap9.reserved4);
    fprintf(fp, "\n");

    /* PCHSTRP10 */
    fprintf(fp, "   /* PCHSTRP10 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.reserved = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.reserved);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.meBootFlash = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.meBootFlash);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.reserved1 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap10.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.meDebugSmbusEmergencyModeEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.meDebugSmbusEmergencyModeEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.meDebugSmbusEmergencyModeAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap10.meDebugSmbusEmergencyModeAddress);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.reserved2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.reserved2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.integratedClockingConfigurationSelect = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.integratedClockingConfigurationSelect);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.intelMeResetCaptureOnClRst1 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.intelMeResetCaptureOnClRst1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.iccProfileSelection = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.iccProfileSelection);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.deepSxEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.deepSxEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.meDebugLanEmergencyMode = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap10.meDebugLanEmergencyMode);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap10.reserved3 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap10.reserved3);
    fprintf(fp, "\n");

    /* PCHSTRP11 */
    fprintf(fp, "   /* PCHSTRP11 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap11.smlink1GpAddressEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap11.smlink1GpAddressEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap11.swLink1GpAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap11.swLink1GpAddress);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap11.reserved1 = 0x%04x;\n", descriptorStruct.pchStraps.pchStrap11.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap11.smlink1I2cTargetAddressEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap11.smlink1I2cTargetAddressEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap11.smlink1I2cTargetAddress = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap11.smlink1I2cTargetAddress);
    fprintf(fp, "\n");

    /* PCHSTRP12 */
    fprintf(fp, "   /* PCHSTRP12 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap12.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap12.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP13 */
    fprintf(fp, "   /* PCHSTRP13 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap13.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap13.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP14 */
    fprintf(fp, "   /* PCHSTRP14 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap14.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap14.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP15 */
    fprintf(fp, "   /* PCHSTRP15 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.chipConfigSoftstrap6 = 0x%02x;\n", descriptorStruct.pchStraps.pchStrap15.chipConfigSoftstrap6);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.intelIntegratedWiredLanEnable = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap15.intelIntegratedWiredLanEnable);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.reserved1 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap15.reserved1);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.chipConfigSoftstrap5 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap15.chipConfigSoftstrap5);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.reserved2 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap15.reserved2);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.smlink1ThermalReportingSelect = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap15.smlink1ThermalReportingSelect);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.slpLanGpio29Sel = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap15.slpLanGpio29Sel);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap15.reserved3 = 0x%04x;\n", descriptorStruct.pchStraps.pchStrap15.reserved3);
    fprintf(fp, "\n");

    /* PCHSTRP16 */
    fprintf(fp, "   /* PCHSTRP16 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap16.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap16.reserved);
    fprintf(fp, "\n");

    /* PCHSTRP17 */
    fprintf(fp, "   /* PCHSTRP17 */\n");
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap17.integratedClockModeSelect = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap17.integratedClockModeSelect);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap17.chipConfigSoftstrap7 = 0x%01x;\n", descriptorStruct.pchStraps.pchStrap17.chipConfigSoftstrap7);
    fprintf(fp, "   descriptorStruct.pchStraps.pchStrap17.reserved = 0x%08x;\n", descriptorStruct.pchStraps.pchStrap17.reserved);
    fprintf(fp, "\n");

	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 184; i++) {
		if (descriptorStruct.pchStraps.padding[i] != 0xFF) {
			for (j = 0; j < 184; j++) {
				fprintf(fp, "    descriptorStruct.pchStraps.padding[%d] = 0x%02x;\n", j, descriptorStruct.pchStraps.padding[j]);
			}
			break;
		} else if (i == 183) {
			fprintf(fp, "    for (i = 0; i < 184; i++) {\n");
			fprintf(fp, "        descriptorStruct.pchStraps.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");

    /* PROCSTRAP0 */
    fprintf(fp, "   /* PROCSTRAP0 */\n");
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.reservedGuessedMeDisable = 0x%01x;\n", descriptorStruct.procStraps.procStrap0.reservedGuessedMeDisable);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.meBootFromFlash = 0x%01x;\n", descriptorStruct.procStraps.procStrap0.meBootFromFlash);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.reservedGuessedTpmDisable = 0x%01x;\n", descriptorStruct.procStraps.procStrap0.reservedGuessedTpmDisable);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.reserved1 = 0x%01x;\n", descriptorStruct.procStraps.procStrap0.reserved1);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.spiFingerprint = 0x%01x;\n", descriptorStruct.procStraps.procStrap0.spiFingerprint);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.meAlternateDisable = 0x%01x;\n", descriptorStruct.procStraps.procStrap0.meAlternateDisable);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.reserved2 = 0x%02x;\n", descriptorStruct.procStraps.procStrap0.reserved2);
    fprintf(fp, "   descriptorStruct.procStraps.procStrap0.reserved3 = 0x%04x;\n", descriptorStruct.procStraps.procStrap0.reserved3);
    fprintf(fp, "\n");

    /* padding */
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 3320; i++) {
		if (descriptorStruct.procStraps.padding[i] != 0xFF) {
			for (j = 0; j < 3320; j++) {
				fprintf(fp, "    descriptorStruct.procStraps.padding[%d] = 0x%02x;\n", j, descriptorStruct.procStraps.padding[j]);
			}
			break;
		} else if (i == 3319) {
			fprintf(fp, "    for (i = 0; i < 3320; i++) {\n");
			fprintf(fp, "        descriptorStruct.procStraps.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}

    /* Descriptor Upper Map Section */
    fprintf(fp, "   /* Descriptor Upper Map Section */\n");
    fprintf(fp, "   descriptorStruct.descriptorUpperMapSection.VTBA = 0x%02x;\n", descriptorStruct.descriptorUpperMapSection.VTBA);
    fprintf(fp, "   descriptorStruct.descriptorUpperMapSection.VTL = 0x%02x;\n", descriptorStruct.descriptorUpperMapSection.VTL);
    fprintf(fp, "   descriptorStruct.descriptorUpperMapSection.reserved = 0x%04x;\n", descriptorStruct.descriptorUpperMapSection.reserved);
    fprintf(fp, "\n");

	/* OEM section */
	fprintf(fp, "    /* OEM section */\n");
	fprintf(fp, "    /* see ../descriptor/descriptor.c */\n");
	fprintf(fp, "    /* Magic String (ascii characters) */\n");
	for(i = 0; i < 256; i++) {
		fprintf(fp, "    descriptorStruct.oemSection[%d] = 0x%02x;\n\n", i, descriptorStruct.oemSection[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "\n");
	fprintf(fp, "    return descriptorStruct;\n");
	fprintf(fp, "}\n\n");

	/* ------------------------------ */

	fclose(fp); /* Always close the file when done. */

	return 0;
}

/*
 * ---------------------------------------------------------------------
 * Debugging functions:
 * ---------------------------------------------------------------------
 */

/*
 * show debugging info: descriptor region boundaries, in a 4KB struct.
 */
void printDescriptorRegionLocations(struct DESCRIPTORREGIONRECORD descriptorStruct, char* romName)
{
	printf("\n");

	/* Descriptor region */
	printf(
		"%s: Descriptor start block: %08x ; Descriptor end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg0.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg0.LIMIT << FLREGIONBITSHIFT
	);

	/* BIOS region */
	printf(
		"%s: BIOS start block: %08x ; BIOS end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg1.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg1.LIMIT << FLREGIONBITSHIFT
	);

	/* ME region */
	printf(
		"%s: ME start block: %08x ; ME end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg2.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg2.LIMIT << FLREGIONBITSHIFT
	);

	/* GBe region */
	printf(
		"%s: GBe start block: %08x ; GBe end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg3.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg3.LIMIT << FLREGIONBITSHIFT
	);

	/* Platform region */
	printf(
		"%s: Platform start block: %08x ; Platform end block: %08x\n",
		romName,
		descriptorStruct.regionSection.flReg4.BASE << FLREGIONBITSHIFT,
		descriptorStruct.regionSection.flReg4.LIMIT << FLREGIONBITSHIFT
	);

	return;
}
