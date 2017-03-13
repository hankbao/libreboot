/*
 *  Copyright (C) 2017 Leah Rowe <info@minifree.org>
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

#ifndef INTEL6GEN_MKDESCRIPTOR_H
#define INTEL6GEN_MKDESCRIPTOR_H

#include <stdio.h>
#include <string.h>
#include "../descriptor/descriptor.h"

struct DESCRIPTORREGIONRECORD generatedDescriptorStruct(unsigned int romSize, int hasGbe);
#endif
