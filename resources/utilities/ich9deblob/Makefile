# 
# Makefile for ich9deblob utility from libreboot project
#
# Copyright (C) 2014 Francis Rowe <info@gluglug.org.uk>
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
CFLAGS=-I. -Wall -g

all: ich9deblob

ich9deblob: ich9deblob.o common/common.o descriptor/descriptor.o gbe/gbe.o x86compatibility.o
	$(CC) $(CFLAGS) ich9deblob.o common/common.o descriptor/descriptor.o gbe/gbe.o x86compatibility.o -o ich9deblob

common/common.o: common/common.c
	$(CC) $(CFLAGS) -c common/common.c -o common/common.o

ich9deblob.o: ich9deblob.c
	$(CC) $(CFLAGS) -c ich9deblob.c -o ich9deblob.o

descriptor/descriptor.o: descriptor/descriptor.c
	$(CC) $(CFLAGS) -c descriptor/descriptor.c -o descriptor/descriptor.o

gbe/gbe.o: gbe/gbe.c
	$(CC) $(CFLAGS) -c gbe/gbe.c -o gbe/gbe.o

x86compatibility.o: x86compatibility.c
	$(CC) $(CFLAGS) -c x86compatibility.c -o x86compatibility.o
	
clean:
	rm -f ich9deblob *.o */*.o
