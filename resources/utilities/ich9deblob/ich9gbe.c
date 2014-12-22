/*
 *  ich9gbe.c
 *
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

struct GBEREGIONRECORD {
	unsigned char macAddress[6]; // 0x03 words, or 0x06 bytes
	unsigned char otherStuff[120];  // 0x3c words, or 0x7E bytes
	unsigned short checkSum; // when added to the sum of all words above, this should be 0xBABA
	unsigned char padding1[3968];
	// Backup region:
	// This is actually "main" on X200, since the real main has a bad checksum
	// and other errors. You should do what you need on this one (if modifying
	// lenovobios's gbe region) and then copy to main
	unsigned char macAddress2[6]; // ditto
	unsigned char otherStuff2[120]; // ditto 
	unsigned short checkSum2; // ditto
	unsigned char padding2[3968];
};
