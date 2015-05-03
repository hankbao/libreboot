/*
 *  descriptor/descriptor.c
 *  This file is part of the ich9deblob utility from the libreboot project
 * 
 *	 Copyright (C) 2014, 2015 Francis Rowe <info@gluglug.org.uk>
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
 * Descriptor functions
 * ---------------------------------------------------------------------
 */

/* Set the Host CPU / BIOS region to have read-write access on all regions */ 
struct DESCRIPTORREGIONRECORD descriptorHostRegionsUnlocked(struct DESCRIPTORREGIONRECORD descriptorStruct)
{	
   /* FLMSTR1 (Host CPU / BIOS) */
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
   
   return descriptorStruct;
}

/* Set the ME to have *no* read-write access on any region */
struct DESCRIPTORREGIONRECORD descriptorMeRegionsForbidden(struct DESCRIPTORREGIONRECORD descriptorStruct)
{	
   /* FLMSTR2 (ME) */
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
 
struct DESCRIPTORREGIONRECORD descriptorMePlatformRegionsRemoved(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
	/* 
	 * set number of regions from 4 -> 2 (0 based, so 4 means 5 and 2
	 * means 3. We want 3 regions: descriptor, gbe and bios, in that order)
	 */
	descriptorStruct.flMaps.flMap0.NR = 2;
	
	/*
	 * There are 5 regions. Since we set the number now to 3, that means
	 * we need to disable 2 regions. ME and Platform will be disabled!
	 * 
	 * To disable a region, set the BASE to 1FFF. Shifted by FLREGIONBITSHIFT,
	 * this puts the beginning of that region well outside the ROM image. 
	 * Also set the LIMIT to 0.
	 */
	/* Disable (delete) the ME region */
	descriptorStruct.regionSection.flReg2.BASE = 0x1FFF;
	descriptorStruct.regionSection.flReg2.LIMIT = 0;
	/* Disable (delete) the Platform region */
	descriptorStruct.regionSection.flReg4.BASE = 0x1FFF;
	descriptorStruct.regionSection.flReg4.LIMIT = 0;
	
	return descriptorStruct;
}

struct DESCRIPTORREGIONRECORD descriptorDisableMeTpm(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
	descriptorStruct.ichStraps.ichStrap0.meDisable = 1; /* Disable the ME in ICHSTRAP0 */
	descriptorStruct.mchStraps.mchStrap0.meDisable = 1; /* Disable the ME in MCHSTRAP0 */
	descriptorStruct.mchStraps.mchStrap0.tpmDisable = 1; /* Disable the TPM in MCHSTRAP0 */
	
	return descriptorStruct;
}

struct DESCRIPTORREGIONRECORD descriptorMoveGbeToStart(struct DESCRIPTORREGIONRECORD descriptorStruct)
{
	/* Relocate the Gbe region to begin at 4KiB (immediately after the flash descriptor) */
	descriptorStruct.regionSection.flReg3.BASE = DESCRIPTORREGIONSIZE >> FLREGIONBITSHIFT;
	descriptorStruct.regionSection.flReg3.LIMIT = GBEREGIONSIZE_8K >> FLREGIONBITSHIFT;
	
	return descriptorStruct;
}

struct DESCRIPTORREGIONRECORD descriptorBiosRegionFillImageAfterGbe(struct DESCRIPTORREGIONRECORD descriptorStruct, unsigned int romSize)
{
	/* BIOS Region begin after descriptor+gbe at first 12KiB, fills the rest of the image */
	descriptorStruct.regionSection.flReg1.BASE = (DESCRIPTORREGIONSIZE + GBEREGIONSIZE_8K) >> FLREGIONBITSHIFT;
	descriptorStruct.regionSection.flReg1.LIMIT = (romSize >> FLREGIONBITSHIFT) - 1;
	
	return descriptorStruct;
}
 
struct DESCRIPTORREGIONRECORD deblobbedDescriptorStructFromFactory(struct DESCRIPTORREGIONRECORD descriptorStruct, unsigned int romSize)
{
	// Disable the ME/TPM and remove the ME/Platform regions:
	descriptorStruct = descriptorMePlatformRegionsRemoved(descriptorStruct);
	descriptorStruct = descriptorDisableMeTpm(descriptorStruct);

	// Move GbE region to the start of the image (after the descriptor)
	descriptorStruct = descriptorMoveGbeToStart(descriptorStruct);

	/* BIOS region fills the remaining space */
	descriptorStruct = descriptorBiosRegionFillImageAfterGbe(descriptorStruct, romSize);
	 
	/*
	 * Set region read/write access
	 * -------------
	 */
	 
	/* Host/CPU is allowed to read/write all regions. */
	descriptorStruct = descriptorHostRegionsUnlocked(descriptorStruct);
	/* The ME is disallowed read-write access to all regions
	 * (this is probably redundant, since the ME is already removed from libreboot) */
	descriptorStruct = descriptorMeRegionsForbidden(descriptorStruct);
	
	/*
	 * Miscellaneous
	 * -------------
	 */
	
	/* Set OEM string to "LIBERATE" */
	descriptorStruct.oemSection.magicString[0] = 0x4C;
	descriptorStruct.oemSection.magicString[1] = 0x49;
	descriptorStruct.oemSection.magicString[2] = 0x42;
	descriptorStruct.oemSection.magicString[3] = 0x45;
	descriptorStruct.oemSection.magicString[4] = 0x52;
	descriptorStruct.oemSection.magicString[5] = 0x41;
	descriptorStruct.oemSection.magicString[6] = 0x54;
	descriptorStruct.oemSection.magicString[7] = 0x45;
	
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
	
	fprintf(fp, "/* %s: generated C code from ich9deblob */\n", outFileName);
	fprintf(fp, "/* .h header file for the descriptor-generating C code (%s) */\n\n", cFileName);
	
	fprintf(fp, "#ifndef ICH9GEN_MKDESCRIPTOR_H\n");
	fprintf(fp, "#define ICH9GEN_MKDESCRIPTOR_H\n\n");
	
	fprintf(fp, "#include <stdio.h>\n");
	fprintf(fp, "#include <string.h>\n");
	fprintf(fp, "#include \"../descriptor/descriptor.h\"\n\n");
	
	fprintf(fp, "/* ROM image sizes in bytes */\n");
	fprintf(fp, "#define ROMSIZE_4MB 0x400000\n");
	fprintf(fp, "#define ROMSIZE_8MB 0x800000\n\n");
	
	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStruct(unsigned int romSize);\n");
	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStructRom4M();\n");
	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStructRom8M();\n");
	
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
	
	fprintf(fp, "/* %s: generated C code from ich9deblob */\n", outFileName);
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
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.FISBA = 0x%02x;\n", descriptorStruct.flMaps.flMap1.FISBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap1.ISL = 0x%02x;\n", descriptorStruct.flMaps.flMap1.ISL);
	fprintf(fp, "    /* FLMAP2 */\n");
	fprintf(fp, "    descriptorStruct.flMaps.flMap2.FMSBA = 0x%02x;\n", descriptorStruct.flMaps.flMap2.FMSBA);
	fprintf(fp, "    descriptorStruct.flMaps.flMap2.MSL = 0x%02x;\n", descriptorStruct.flMaps.flMap2.MSL);
	fprintf(fp, "    descriptorStruct.flMaps.flMap2.reserved = 0x%04x;\n", descriptorStruct.flMaps.flMap2.reserved);
	fprintf(fp, "\n");
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
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.readStatusClockFrequency = 0x%01x;\n", descriptorStruct.componentSection.flcomp.readStatusClockFrequency);
	fprintf(fp, "    descriptorStruct.componentSection.flcomp.reserved4 = 0x%01x;\n", descriptorStruct.componentSection.flcomp.reserved4);
	fprintf(fp, "    /* FLILL */\n");
	fprintf(fp, "    descriptorStruct.componentSection.flill = 0x%08x;\n", descriptorStruct.componentSection.flill);
	fprintf(fp, "    /* FLPB */\n");
	fprintf(fp, "    descriptorStruct.componentSection.flpb = 0x%08x;\n", descriptorStruct.componentSection.flpb);
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 36; i++) {
		if (descriptorStruct.componentSection.padding[i] != 0xFF) {
			for (j = 0; j < 36; j++) {
				fprintf(fp, "    descriptorStruct.componentSection.padding[%d] = 0x%02x;\n", j, descriptorStruct.componentSection.padding[j]);
			}
			break;
		} else if (i == 35) {
			fprintf(fp, "    for (i = 0; i < 36; i++) {\n");
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
	fprintf(fp, "    /* descriptorStruct.regionSection.flReg1.BASE = 0x%04x; */\n", descriptorStruct.regionSection.flReg1.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.BASE = (DESCRIPTORREGIONSIZE + GBEREGIONSIZE_8K) >> FLREGIONBITSHIFT; /* see ../descriptor/descriptor.c */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg1.reserved1);
	fprintf(fp, "    /* descriptorStruct.regionSection.flReg1.LIMIT = 0x%04x; */\n", descriptorStruct.regionSection.flReg1.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.LIMIT = ((romSize >> FLREGIONBITSHIFT) - 1); /* see ../descriptor/descriptor.c */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg1.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg1.reserved2);
	fprintf(fp, "    /* FLREG2 (ME) */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.BASE = 0x%04x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.regionSection.flReg2.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg2.reserved1);
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.LIMIT = 0x%04x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.regionSection.flReg2.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg2.reserved2 = 0x%01x;\n", descriptorStruct.regionSection.flReg2.reserved2);
	fprintf(fp, "    /* FLREG3 (Gbe) */\n");
	fprintf(fp, "    /* descriptorStruct.regionSection.flReg3.BASE = 0x%04x; */\n", descriptorStruct.regionSection.flReg3.BASE);
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.BASE = DESCRIPTORREGIONSIZE >> FLREGIONBITSHIFT; /* see ../descriptor/descriptor.c */\n");
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.reserved1 = 0x%01x;\n", descriptorStruct.regionSection.flReg3.reserved1);
	fprintf(fp, "    /* descriptorStruct.regionSection.flReg3.LIMIT = 0x%04x; */\n", descriptorStruct.regionSection.flReg3.LIMIT);
	fprintf(fp, "    descriptorStruct.regionSection.flReg3.LIMIT = GBEREGIONSIZE_8K >> FLREGIONBITSHIFT; /* see ../descriptor/descriptor.c */\n");
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
	/* ICH straps */
	fprintf(fp, "    /* ICH straps */\n");
	fprintf(fp, "    /* ICHSTRAP0 */\n");
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.meDisable = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.ichStraps.ichStrap0.meDisable);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.reserved1 = 0x%02x;\n", descriptorStruct.ichStraps.ichStrap0.reserved1);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.tcoMode = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.tcoMode);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.smBusAddress = 0x%02x;\n", descriptorStruct.ichStraps.ichStrap0.smBusAddress);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.bmcMode = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.bmcMode);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.tripPointSelect = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.tripPointSelect);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.reserved2 = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.reserved2);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.integratedGbe = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.integratedGbe);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.lanPhy = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.lanPhy);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.reserved3 = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.reserved3);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.dmiRequesterId = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap0.dmiRequesterId);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap0.smBus2Address = 0x%02x;\n", descriptorStruct.ichStraps.ichStrap0.smBus2Address);
	fprintf(fp, "    /* ICHSTRAP1 */\n");
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.northMlink = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap1.northMlink);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.southMlink = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap1.southMlink);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.meSmbus = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap1.meSmbus);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.sstDynamic = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap1.sstDynamic);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.reserved1 = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap1.reserved1);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.northMlink2 = 0x%01x;\n", descriptorStruct.ichStraps.ichStrap1.northMlink2);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.reserved2 = 0x%02x;\n", descriptorStruct.ichStraps.ichStrap1.reserved2);
	fprintf(fp, "    descriptorStruct.ichStraps.ichStrap1.reserved3 = 0x%04x;\n", descriptorStruct.ichStraps.ichStrap1.reserved3);
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 248; i++) {
		if (descriptorStruct.ichStraps.padding[i] != 0xFF) {
			for (j = 0; j < 248; j++) {
				fprintf(fp, "    descriptorStruct.ichStraps.padding[%d] = 0x%02x;\n", j, descriptorStruct.ichStraps.padding[j]);
			}
			break;
		} else if (i == 247) {
			fprintf(fp, "    for (i = 0; i < 248; i++) {\n");
			fprintf(fp, "        descriptorStruct.ichStraps.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");
	/* MCH straps */
	fprintf(fp, "    /* MCH straps */\n");
	fprintf(fp, "    /* MCHSTRAP0 */\n");
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.meDisable = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.mchStraps.mchStrap0.meDisable);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.meBootFromFlash = 0x%01x;\n", descriptorStruct.mchStraps.mchStrap0.meBootFromFlash);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.tpmDisable = 0x%01x; /* see ../descriptor/descriptor.c */\n", descriptorStruct.mchStraps.mchStrap0.tpmDisable);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.reserved1 = 0x%01x;\n", descriptorStruct.mchStraps.mchStrap0.reserved1);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.spiFingerprint = 0x%01x;\n", descriptorStruct.mchStraps.mchStrap0.spiFingerprint);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.meAlternateDisable = 0x%01x;\n", descriptorStruct.mchStraps.mchStrap0.meAlternateDisable);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.reserved2 = 0x%02x;\n", descriptorStruct.mchStraps.mchStrap0.reserved2);
	fprintf(fp, "    descriptorStruct.mchStraps.mchStrap0.reserved3 = 0x%04x;\n", descriptorStruct.mchStraps.mchStrap0.reserved3);
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 3292; i++) {
		if (descriptorStruct.mchStraps.padding[i] != 0xFF) {
			for (j = 0; j < 3292; j++) {
				fprintf(fp, "    descriptorStruct.mchStraps.padding[%d] = 0x%02x;\n", j, descriptorStruct.mchStraps.padding[j]);
			}
			break;
		} else if (i == 3291) {
			fprintf(fp, "    for (i = 0; i < 3292; i++) {\n");
			fprintf(fp, "        descriptorStruct.mchStraps.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");
	/* ME VSCC Table */
	fprintf(fp, "    /* ME VSCC Table */\n");
	fprintf(fp, "    descriptorStruct.meVsccTable.jid0 = 0x%08x;\n", descriptorStruct.meVsccTable.jid0);
	fprintf(fp, "    descriptorStruct.meVsccTable.vscc0 = 0x%08x;\n", descriptorStruct.meVsccTable.vscc0);
	fprintf(fp, "    descriptorStruct.meVsccTable.jid1 = 0x%08x;\n", descriptorStruct.meVsccTable.jid1);
	fprintf(fp, "    descriptorStruct.meVsccTable.vscc1 = 0x%08x;\n", descriptorStruct.meVsccTable.vscc1);
	fprintf(fp, "    descriptorStruct.meVsccTable.jid2 = 0x%08x;\n", descriptorStruct.meVsccTable.jid2);
	fprintf(fp, "    descriptorStruct.meVsccTable.vscc2 = 0x%08x;\n", descriptorStruct.meVsccTable.vscc2);
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 4; i++) {
		if (descriptorStruct.meVsccTable.padding[i] != 0xFF) {
			for (j = 0; j < 4; j++) {
				fprintf(fp, "    descriptorStruct.meVsccTable.padding[%d] = 0x%02x;\n", j, descriptorStruct.meVsccTable.padding[j]);
			}
			break;
		} else if (i == 3) {
			fprintf(fp, "    for (i = 0; i < 4; i++) {\n");
			fprintf(fp, "        descriptorStruct.meVsccTable.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");
	/* Descriptor Map 2 Record */
	fprintf(fp, "    /* Descriptor Map 2 Record */\n");
	fprintf(fp, "    descriptorStruct.descriptor2Map.meVsccTableBaseAddress = 0x%02x;\n", descriptorStruct.descriptor2Map.meVsccTableBaseAddress);
	fprintf(fp, "    descriptorStruct.descriptor2Map.meVsccTableLength = 0x%02x;\n", descriptorStruct.descriptor2Map.meVsccTableLength);
	fprintf(fp, "    descriptorStruct.descriptor2Map.reserved = 0x%04x;\n", descriptorStruct.descriptor2Map.reserved);
	fprintf(fp, "\n");
	/* OEM section */
	fprintf(fp, "    /* OEM section */\n");
	fprintf(fp, "    /* see ../descriptor/descriptor.c */\n");
	fprintf(fp, "    /* Magic String (ascii characters) */\n");
	for(i = 0; i < 8; i++) {
		fprintf(fp, "    descriptorStruct.oemSection.magicString[%d] = 0x%02x;\n", i, descriptorStruct.oemSection.magicString[i]);
	}
	fprintf(fp, "    /* Padding */\n");
	for (i = 0; i < 248; i++) {
		if (descriptorStruct.oemSection.padding[i] != 0xFF) {
			for (j = 0; j < 248; j++) {
				fprintf(fp, "    descriptorStruct.oemSection.padding[%d] = 0x%02x;\n", j, descriptorStruct.oemSection.padding[j]);
			}
			break;
		} else if (i == 247) {
			fprintf(fp, "    for (i = 0; i < 248; i++) {\n");
			fprintf(fp, "        descriptorStruct.oemSection.padding[i] = 0xFF;\n");
			fprintf(fp, "    }\n");
			break;
		}
	}
	fprintf(fp, "\n");
	fprintf(fp, "    return descriptorStruct;\n");
	fprintf(fp, "}\n\n");
	
	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStructRom4M()\n");
	fprintf(fp, "{\n");
	fprintf(fp, "    return generatedDescriptorStruct(ROMSIZE_4MB);\n");
	fprintf(fp, "}\n\n");
	
	fprintf(fp, "struct DESCRIPTORREGIONRECORD generatedDescriptorStructRom8M()\n");
	fprintf(fp, "{\n");
	fprintf(fp, "    return generatedDescriptorStruct(ROMSIZE_8MB);\n");
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
