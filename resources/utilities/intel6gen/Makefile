# 
# Makefile for intel6deblob utility from libreboot project
#
# Copyright (C) 2014, 2015 Leah Rowe <info@minifree.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CC=gcc
FORCEC99=-std=c99
CFLAGS=-I. -Wall -Wextra -g $(FORCEC99)
NOLINKER=-c

all: intel6deblob intel6gen demefactory

intel6deblob: obj/intel6deblob.o obj/common/descriptor_gbe.o \
		obj/descriptor/descriptor.o obj/gbe/gbe.o obj/common/x86compatibility.o
		
	$(CC) $(CFLAGS) obj/intel6deblob.o obj/common/descriptor_gbe.o \
		obj/common/x86compatibility.o obj/descriptor/descriptor.o obj/gbe/gbe.o \
		 -o intel6deblob
		 
intel6gen: obj/intel6gen.o obj/intel6gen/mkdescriptor.o obj/intel6gen/mkgbe.o \
		obj/common/descriptor_gbe.o \
		obj/descriptor/descriptor.o obj/gbe/gbe.o obj/common/x86compatibility.o
		
	$(CC) $(CFLAGS) obj/intel6gen.o obj/intel6gen/mkdescriptor.o obj/intel6gen/mkgbe.o \
	 obj/common/descriptor_gbe.o \
		obj/common/x86compatibility.o obj/descriptor/descriptor.o obj/gbe/gbe.o \
		 -o intel6gen

demefactory: obj/demefactory.o obj/common/descriptor_gbe.o \
		obj/descriptor/descriptor.o obj/gbe/gbe.o obj/common/x86compatibility.o
		
	$(CC) $(CFLAGS) obj/demefactory.o obj/common/descriptor_gbe.o \
		obj/common/x86compatibility.o obj/descriptor/descriptor.o obj/gbe/gbe.o \
		 -o demefactory

# for demefactory
# ----------------------------------------------------------------------

obj/demefactory.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/demefactory.c -o obj/demefactory.o

# for intel6deblob
# ----------------------------------------------------------------------

obj/intel6deblob.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/intel6deblob.c -o obj/intel6deblob.o

# for intel6gen
# ----------------------------------------------------------------------

obj/intel6gen.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/intel6gen.c -o obj/intel6gen.o
	
obj/intel6gen/mkdescriptor.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/intel6gen/mkdescriptor.c -o obj/intel6gen/mkdescriptor.o

obj/intel6gen/mkgbe.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/intel6gen/mkgbe.c -o obj/intel6gen/mkgbe.o

# for demefactory, intel6deblob and intel6gen:
# ----------------------------------------------------------------------

obj/common/descriptor_gbe.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/common/descriptor_gbe.c -o obj/common/descriptor_gbe.o
	
obj/common/x86compatibility.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/common/x86compatibility.c -o obj/common/x86compatibility.o

obj/descriptor/descriptor.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/descriptor/descriptor.c -o obj/descriptor/descriptor.o

obj/gbe/gbe.o:
	$(CC) $(CFLAGS) $(NOLINKER) src/gbe/gbe.c -o obj/gbe/gbe.o
	
# make clean
# ----------------------------------------------------------------------
clean:
	rm -Rf intel6deblob intel6gen demefactory obj/*.o obj/*/*.o
