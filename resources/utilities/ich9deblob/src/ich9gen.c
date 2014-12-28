/*
 *  Copyright (C) 2014 Francis Rowe <info@gluglug.org.uk>
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
 
/* Generate deblobbed descriptor and gbe 12KiB file from scratch
 * without relying on a factory.rom dump */
 
#include "ich9gen.h"

int main(int argc, char *argv[]) 
{
	struct GBEREGIONRECORD_8K gbeStruct8k = generatedGbeStruct8k();
	struct DESCRIPTORREGIONRECORD descriptorStruct4M = generatedDescriptorStructRom4M();
	struct DESCRIPTORREGIONRECORD descriptorStruct8M = generatedDescriptorStructRom8M();
	
	/* Only for the compatibility checks */
	struct DESCRIPTORREGIONRECORD dummyDescriptorStruct;
	struct GBEREGIONRECORD_8K dummyGbeStruct8k;
	
	/*
	 * ------------------------------------------------------------------
	 * Compatibility checks. This version of ich9deblob is not yet portable.
	 * ------------------------------------------------------------------
	 */

	if (systemOrCompilerIncompatible(dummyDescriptorStruct, dummyGbeStruct8k)) return 1;
	/* If true, fail with error message */
	
	/*
	 * ------------------------------------------------------------------
	 * Down to business
	 * ------------------------------------------------------------------
	 */
	
	if (notCreatedDescriptorGbeFile(descriptorStruct4M, gbeStruct8k, "ich9fdgbe_4m.bin")) {
		return 1;
	}
	
	if (notCreatedDescriptorGbeFile(descriptorStruct8M, gbeStruct8k, "ich9fdgbe_8m.bin")) {
		return 1;
	}
	
	return 0;
}
