/*
 *  descriptor_gbe.c
 *  This file is part of the ich9deblob utility from the libreboot project
 * 
 * Purpose: common descriptor/gbe functions used by ich9deblob
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
 
#include <stdio.h>
#include <string.h>
#include "descriptor_gbe.h"

/* 
 * create 12KiB file with descriptor, and then gbe immediately after.
 */
int notCreatedDescriptorGbeFile(struct DESCRIPTORREGIONRECORD descriptorStruct, struct GBEREGIONRECORD_8K gbeStruct8k, char* fileName)
{
	FILE* fileStream = NULL;
	
	/* These will have the structs copied to them */
	char descriptorBuffer[DESCRIPTORREGIONSIZE];
	char gbeBuffer8k[GBEREGIONSIZE_8K];
	
	/* Copy the structs to buffers, to make writing them as files easier */
	memcpy(&descriptorBuffer, &descriptorStruct, DESCRIPTORREGIONSIZE); /* descriptor */
	memcpy(&gbeBuffer8k, &gbeStruct8k, GBEREGIONSIZE_8K);               /* gbe */
	
	/* delete old file before continuing */
	remove(fileName);
	
	/* open new file for writing the descriptor+gbe */
	fileStream = fopen(fileName, "ab");
	
	/* write the descriptor region into the first part */
	if (DESCRIPTORREGIONSIZE != fwrite(descriptorBuffer, sizeof(char), DESCRIPTORREGIONSIZE, fileStream))
	{
		printf("\nerror: writing descriptor region failed\n");
		return 1;
	}
	
	/* add gbe to the end of the file */
	if (GBEREGIONSIZE_8K != fwrite(gbeBuffer8k, sizeof(char), GBEREGIONSIZE_8K, fileStream))
	{
		printf("\nerror: writing GBe region failed\n");
		return 1;
	}
	
	fclose(fileStream);
	
	printf("descriptor and gbe successfully written to the file: %s\n", fileName);
	printf("Now do: dd if=deblobbed_descriptor.bin of=libreboot.rom bs=1 count=12k conv=notrunc\n");
	printf("(in other words, add the modified descriptor+gbe to your ROM image)\n\n");
	
	return 0;
}
