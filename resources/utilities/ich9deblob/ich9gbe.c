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
	unsigned char macAddress[6];
	unsigned char otherStuff[120];
	unsigned short checkSum;
	unsigned char padding1[3968];
	unsigned char macAddress2[6];
	unsigned char otherStuff2[120];
	unsigned short checkSum2;
	unsigned char padding2[3968];
};
