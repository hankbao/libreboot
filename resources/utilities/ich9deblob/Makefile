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
CFLAGS=-I.

ich9deblob: ich9deblob.c descriptor/struct.h gbe/struct.h
	$(CC) $(CFLAGS) ich9deblob.c descriptor/struct.h gbe/struct.h -o ich9deblob
	
clean:
	rm -f ich9deblob *.o
