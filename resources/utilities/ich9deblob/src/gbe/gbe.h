/*
 *  gbe/gbe.h
 *  This file is part of the ich9deblob utility from the libreboot project
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
 
/*
 * Purpose: provide struct representing gbe region.
 * Map actual buffers of this regions, directly to instances of these
 * structs. This makes working with gbe really easy.
 */
 
/*
 * bit fields used, corresponding to datasheet. See links to datasheets
 * and documentation in ich9deblob.c
 */
 
 /*
 * See docs/hcl/x200_remove_me.html for info plus links to datasheet (also linked below)
 * 
 * Info about Gbe region (read whole datasheet):
 * http://www.intel.co.uk/content/dam/doc/application-note/i-o-controller-hub-9m-82567lf-lm-v-nvm-map-appl-note.pdf
 * https://communities.intel.com/community/wired/blog/2010/10/14/how-to-basic-eeprom-checksums
 */

#ifndef GBESTRUCT_H
#define GBESTRUCT_H

#include <stdio.h>
#include <string.h>

/* Size of the full gbe region in bytes */
#define GBEREGIONSIZE_8K 0x2000
/*
 * Size of each sub-region in gbe. 
 * gbe contains two regions which
 * can be identical: main and backup.
 * These are each half the size of the full region
 */
#define GBEREGIONSIZE_4K 0x1000

/* 
 * When adding up the first 0x3F 16-bit words
 * in a 4KiB GBE region, it should be equal
 * to 0xBABA
 */
#define GBECHECKSUMTOTAL 0xBABA

/*
 * These will have a modified descriptor+gbe based on what's in the factory.rom
 * These will be joined into a single 12KiB buffer (descriptor, then gbe) and saved to a file
 * NOTE: The GBE region of 8K is actually 2x 4K regions in a single region; both 4K blocks can be identical (and by default, are)
 * The 2nd one is a "backup", but we don't know when it's used. perhaps it's used when the checksum on the first one does not match?
 */

/*
 * ---------------------------------------------------------------------
 * Gbe struct representing the data:
 * ---------------------------------------------------------------------
 */ 

struct GBE_RESERVED_WORD_03H {
	/* least significant bits */
	unsigned char reserved1_0                  : 8; /* bits should all be set to zero */
	unsigned char reserved1_1                  : 3;  /* ^ part of above. Separated so that the bitfields align */
	unsigned char ibaLom                       : 1;  /* set to 1 for intel boot agent to work (i.e. set it to 0) */
	unsigned char reserved2 		             : 4;  /* bits should all be set to zero */
	/* most significant bits */
};

/* Word 0A */
struct GBE_PCI_INITIALIZATION_CONTROL_WORD {
	/* least significant bits */
	unsigned char loadVendorDeviceId           : 1;  /* 1 = load from NVM. 0 = load from MAC fuses. It's 1 in my deblobbed_descriptor.bin */
	unsigned char loadSubsystemId              : 1;  /* 1 = load from NVM. 0 = load from MAC fuses. It's 1 in my deblobbed_descriptor.bin */
	unsigned char reserved1                    : 1;  /* Reserved. Set to 0 (according to datasheet). 0 in my deblobbed_descriptor.bin */
	unsigned char reserved2                    : 3;  /* Reserved. Set them to 0 (according to datasheet). 0 in my deblobbed_descriptor.bin */
	unsigned char pmEnable                     : 1;  /* Power Management Enable. 1=Enable. It's 1 in my deblobbed_descriptor.bin */
	unsigned char auxPwr                       : 1;  /* Auxiliary Power Indication. See datasheet. it's 1 in my deblobbed_descriptor.bin */
	unsigned char reserved3                    : 4;  /* Reserved. Set to 0000 (according to datasheet). */
	unsigned char reserved4                    : 4;  /* Reserved. Set to 0001 (according to datasheet). */
	/* most significant bits */
};

/* Word 10h. */
struct GBE_LAN_POWER_CONSUMPTION {
	/* least significant bits */
	unsigned char lanD3Power                   : 5;  /* It's 00001b (0x1) in deblobbed_descriptor.bin */
	unsigned char reserved                     : 3;  /* Reserved. These bits should all be 0. confirmed from deblobbed_descriptor.bin */
	unsigned char lanD0Power                   : 8;  /* default value: 0x0D (according to datasheet). confirmed from deblobbed_descriptor.bin */
	/* most significant bits */
};

/* Word 13h */
struct GBE_SHARED_INITIALIZATION_CONTROL_WORD {
	/* least significant bits */
	unsigned char reserved1                    : 3;  /* Reserved. These bits should be set to 101 (0x5) in binary (according to datasheet and deblobbed_descriptor.bin) */
	unsigned char forceDuplex                  : 1;  /* Hardware default is 0 according to datasheet and deblobbed_descriptor.bin. Presumably to set whether the chipset is to operate at full- or half-duplex */
	unsigned char forceSpeedEnable             : 1;  /* Hardware default is 0. Presumably to limited speed eg 10, 10/100, 10/100/1000 */
	unsigned char reserved2_0                  : 3;  /* Reserved. All bits should be set to 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char reserved2_1                  : 1;  /* ^ part of above. separated so that bitfields align */
	unsigned char phyPowerDownEnable           : 1;  /* PHY Power Down in D3/Dr (if WoL is disabled), 1 means Enable power down. deblobbed_descriptor.bin says 1 */
	unsigned char reserved3                    : 1;  /* Reserved. Should be set to 1 according to datasheet and deblobbed_descriptor.bin */
	unsigned char reserved4                    : 3;  /* Reserved. These bits should all be 0 according to datasheet and deblobbed_descriptor.bin */
	/* ^ reserved4: indicates whether a valid NVM is present. If invalid, MAC does not read NVM and uses default values. */
	/* 00 = invalid NVM, 01 = invalid NVM, 10 = valid NVM present, 11 = invalid NVM */
	/* Default should be 10 (binary) according to datasheet and deblobbed_descriptor.bin */
	unsigned char sign                         : 2;  /* Make sure to set this to 0x2 (10 in binary) */
	/* most significant bits */
};

/* Word 14h */
struct GBE_EXTENDED_CONFIGURATION_CONTROL_WORD_1 {
	/* least significant bits */
	unsigned short extendedConfigurationPointer: 12; /* dword: base address of extended configuration area in NVM. should not be zero. Default is 020h according to datasheet and deblobbed_descriptor.bin */
	unsigned char oemWriteEnable               : 1;  /* 1=enable. if set, loads oem bits from phy_ctrl register to the 82567. loaded to EXTCNF_CTRL register. default is 1 according to datasheet and deblobbed_descriptor.bin */
	unsigned char reserved1                    : 1;  /* Reserved. default value 1 according to datasheet and deblobed_descriptor.bin */
	unsigned char reserved2                    : 1;  /* Reserved. default value 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char reserved3                    : 1;  /* Reserved. default value 0 according to datasheet and deblobbed_descriptor.bin */
	/* most significant bits */
};

/* Word 15h */
struct GBE_EXTENDED_CONFIGURATION_CONTROL_WORD_2 {
	/* least significant bits */
	unsigned char reserved                     : 8;  /* Reserved. Should be 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char extendedPhyLength            : 8;  /* dword: size of extended phy configuration area. most be 0 if phy config area is disabled. default is 0000101 (binary) or 05 (hex) according to datasheet, but 00001010 (0A) according to deblobbed_descriptor.bin. Is 0000101 (in the datasheet) a typo that actually means 00001010? */
	/* most significant bits */
};

/*
 * Word 17h: LED 1 Configuration and Power Management
 * 
 * Default values for LEDCTL register fields controlling LED1 (LINK_1000)
 * output behaviours and OEM fields that define PHY power management
 * parameters loaded to the PHY_CTRL register.
 */
struct LED_CTL_1 {
	/* least significant bits */
	
	/* See page 16 in the datasheet to show the different modes. deblobbed_descriptor.bin has "ACTIVITY" mode set */
	unsigned char led1Mode                     : 4;  /* Default value 0111 (bin) 7 (hex) says datasheet. 1011 (bin) B (hex) according to deblobbed_descriptor.bin */
	
	unsigned char reserved1                    : 1;  /* Reserved. Should be 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led1BlinkMode                : 1;  /* 0 = slow blink, 1 = fast blink. should be identical to led0 blink mode. Default is 0 according to datasheet and deblobbed_descriptor.bin */
	/* By setting this and led0 blink mode (see word 18h) to 1, you could enable a faster blinking on the LED's where the ethernet cable goes
	 * on the gigabit ethernet port. Not really useful. Slow blink is fine, and probably better (the LED will probably last longer) */

	unsigned char led1Invert                   : 1;  /* initial value of LED1_IVRT field. 0 = led1 has active low output, 1 is high active output. Default is 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led1Blink                    : 1;  /* 1 = led1 blinks, 0 = it does not. default 0 according to datasheet, but it's 1 in deblobbed_descriptor.bin */
	unsigned char reserved2                    : 1;  /* Reserved. should be 1 according to datasheet and deblobbed_descriptor.bin */
	unsigned char lpluEnable                   : 1;  /* Low Power Link Up. Enable links at lowest supported speed by both link partners in all power states. 1=enabled(all power states), 0=disabled. Default is 0 according to datasheet and 1 according to deblobbed_descriptor.bin */
	unsigned char lpluEnableNonD0a             : 1;  /* Low Power Link up (non-D0a states). Same as above but only for non-D0a states. default is 1 according to datasheet but 0 in deblobbed_descriptor.bin */
	unsigned char gbeDisableNonD0a             : 1;  /* If set to 1, disable gigabit speeds in non-D0a power states. Must be 1 (according to datasheet) because GbE is not supported in Sx mode. It's also set to 1 in deblobbed_descriptor.bin */
	unsigned char reserved3                    : 2;  /* Reserved. Datasheet says both bits should be 0 (confirmed in deblobbed_descriptor.bin) */
	unsigned char gbeDisable                   : 1;  /* When 1, gigabit speeds are disabled in all power states including D0a. Default is 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char reserved4                    : 1;  /* Reserved. Should be 1, according to datasheet and deblobbed_descriptor.bin */
	/* most significant bits */
};

/* 
 * Word 18: LED 0 and 2 Configuration Defaults
 * 
 * Hardware defaults for LEDCTL register fields controlling LED0 (LINK/ACTIVITY)
 * and LED2 (LINK_100) output behaviours.
 */
struct LED_CTL_02 {
	/* least significant bits */
	
	/* see page 16 in datasheet to show the different modes. deblobbed_descriptor has "LINK-UP" mode set */
	unsigned char led0Mode                     : 4;  /* default value 0100 (bin) or 4 (hex) according to datasheet. It's 0010 (bin) or 2 (hex) according to deblobbed_descriptor.bin */
	
	unsigned char reserved1                    : 1;  /* Reserved. Should be set to 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led0BlinkMode                : 1;  /* This should be the same as led1BlinkMode (see word 17h). Default is 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led0Invert                   : 1;  /* initial value of LED0_IVRT field. 0 = led0 has active low output, 1 is high active output. Default is 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led0Blink                    : 1;  /* LED0_BLINK field. Should be 0 according to datasheet and deblobbed_descriptor.bin */
	
	/* see page 16 in datasheet to shew the different modes. deblobbed_descriptor has "LINK_100" mode set */
	unsigned char led2Mode                     : 4;  /* default value 0110 (bin) or 6 (hex) according to datasheet and deblobbed_descriptor.bin */
	
	unsigned char reserved2                    : 1;  /* Reserved. Should be 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led2BlinkMode                : 1;  /* 0 = slow blink. 1 = fast. default 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led2Invert                   : 1;  /* LED2_IVRT field. Should be 0 according to datasheet and deblobbed_descriptor.bin */
	unsigned char led2Blink                    : 1;  /* LED2_BLINK field. should be 0 according to datasheet and deblobbed_descriptor.bin */
	/* most significant bits */
};

/* Word 30h */
struct GBE_PXE_BOOT_AGENT_MAIN_SETUP_OPTIONS {
	/* least significant bits */
	unsigned char protocolSelect               : 2;  /* Default 00 binary (PXE) according to datasheet. 01 is reserved. 10/11 are undefined. deblobbed_descriptor.bin says 00 */
	unsigned char reserved1                    : 1;  /* Reserved. deblobbed_descriptor.bin says 0 */
	unsigned char defaultBootSelection         : 2;  /* deblobbed_descriptor.bin says 00 (binary). 00 is network boot, then local. 01 is local boot, then network. 10 is network boot only. 11 is local boot only */
	unsigned char reserved2                    : 1;  /* Reserved. deblobbed_descriptor.bin says 0. */
	unsigned char promptTime                   : 2;  /* deblobbed_descriptor.bin says 00. delay for how long "press ctrl-s" setup prompt message appears. 00 = 2 secs, 01 is 3 secs, 10 is 5 secs, 11 is 0 secs. */
	unsigned char displaySetupMessage          : 1;  /* default 1 says datasheet. deblobbed_descriptor.bin says 1. if 1, "ctrl-s" setup prompt message appears after the title message. */
	unsigned char reserved3                    : 1;  /* Datasheet says to set 0. deblobbed_descriptor.bin says 0. */
	unsigned char forceSpeed                   : 2;  /* deblobbed_descriptor.bin says 00. 00 = auto-negotiate, 01 = 10Mbps, 10 = 100Mbps, 11 = "not allowed" */
	unsigned char forceFullDuplex              : 1;  /* deblobbed_descriptor.bin says 0. Only relevant when bits 10/11 are set; if so, then: 0 = half duplex, 1 = full duplex */
	unsigned char reserved4                    : 1;  /* Reserved. deblobbed_descriptor.bin says 0. datasheet recommends 0. */
	unsigned char efiPresence                  : 1;  /* 1 means that an EFI image is present (0 means not present). deblobbed_descriptor.bin says 0. if 1, eeprom word 33h (efi version) becomes valid. if pxePresent is 1, that means EFI and PXE are both present.*/
	unsigned char pxePresence                  : 1;  /* 0 means that a PXE image is present. 1 means to pxe present. deblobbed_descriptor.bin says 0. if 0, then word 32h (PXE version) in eeprom becomes valid */
	/* most significant bits */
	
	/* This whole data structure is pointless, since libreboot doesn't (read: won't)
	 * include the proprietary intel boot agent. Struct exists here simply for documentations sake. */
};
/* Word 31h */
struct GBE_PXE_BOOT_AGENT_CONFIGURATION_CUSTOMIZATION_OPTIONS_31H {
	/* least significant bits */
	unsigned char disableSetupMenu             : 1;  /* 1 means invoking setup menu with ctrl-s won't work. deblobbed_descriptor.bin says 0 (as is default, per datasheet) */
	unsigned char disableTitleMessage          : 1;  /* 1 means that title in boot agent screen is suppressed, as is ctrl-s message. default is 0, and deblobbed_descriptor.bin says 0 */
	unsigned char disableProtocolSelect        : 1;  /* 1 means no changes to boot protocol are allowed. default is 0, and deblobbed_descriptor.bin says 0 */
	unsigned char disableBootSelection         : 1;  /* 1 means no changes in boot order option menu are allowed. default is 0, and deblobbed_descriptor.bin says 0 */
	unsigned char disableLegacyWakeupSupport   : 1;  /* 1 means no changes in legacy wakeup support menu is allowed. default is 0, and deblobbed_descriptor.bin says 0 */
	unsigned char disableFlashUpdate           : 1;  /* 1 means no changes to flash image using PROset is allowed. default is 0, and deblobbed_descriptor.bin says 0 */
	unsigned char reserved1                    : 2;  /* Reserved. Datasheet says these must be 0, and deblobbed_descriptor.bin sets them to 0. */
	
	/*
	 * deblobbed_descriptor says 000
	 * 000 = normal behaviour
	 * see datasheet (page 21) for other modes.
	 */
	unsigned char ibaBootOrderSetupMode        : 3;
	
	unsigned char reserved2                    : 3;  /* Reserved. Datasheet says these must be set to 0, and deblobbed_descriptor.bin sets them to 0. */
	unsigned char signature                    : 2;  /* Must be set to 01 to indicate that this whole word has been configured by the agent or other software. deblobbed_descriptor.bin says 01. */
	/* most significant bits */
	
	/* This whole data structure is pointless, since libreboot doesn't (read: won't)
	 * include the proprietary intel boot agent. Struct exists here simply for documentations sake. */
};
/* Word 32h */
struct GBE_PXE_BOOT_AGENT_CONFIGURATION_CUSTOMIZATION_OPTIONS_32H {
	/* least significant bits */
	unsigned char buildNumber                  : 8;  /* PXE boot agent build number. default is 28 (hex). deblobbed_descriptor.bin says 18 (hex) */
	unsigned char minorVersionNumber           : 4;  /* PXE boot agent minor number. default is 2 (hex). deblobbed_descriptor.bin says 3 (hex) */
	unsigned char majorVersionNumber           : 4;  /* PXE boot agent makor number. default is F (hex). deblobbed_descriptor.bin says 1 (hex) */
	/* most significant bits */
	
	/* This whole data structure is pointless, since libreboot doesn't (read: won't)
	 * include the proprietary intel boot agent. Struct exists here simply for documentations sake. */
};
/* Word 33h */
struct GBE_PXE_IBA_CAPABILITIES {
	/* least significant bits */
	unsigned char baseCodePresent              : 1;  /* 0 means PXE base code is indicated as being present. 1 (default) means not. deblobbed_descriptor.bin says 1 */
	unsigned char undiCapabilityPresent        : 1;  /* 1 (default) means pxe/undi capability is indicated present. 0 means not present. deblobbed_descriptor.bin says 1 */
	unsigned char reserved1                    : 1;  /* Reserved. Must be 1. deblobbed_descriptor.bin says 1 */
	unsigned char efiUndiCapabilityPresent     : 1;  /* EFI UNDI capability present: 0 (default) means not present. 1 means present. deblobbed_descriptor.bin says 0 */
	unsigned char reserved2_0                  : 4;  /* reserved. all bits must be 0. deblobbed_descriptor.bin sets them to 0. */
	unsigned char reserved2_1                  : 6;  /* ^ part of reserved2_0. split this way so that the bitfields align */
	unsigned char signature                    : 2;  /* must be 01 to indicate that the word is configured by the agent or other software. deblobbed_descriptor.bin says 01 */
	/* most significant bits */
	
	/* This whole data structure is pointless, since libreboot doesn't (read: won't)
	 * include the proprietary intel boot agent. Struct exists here simply for documentations sake. */
};
/* Words 30h to 3Eh */
struct GBE_PXE_SOFTWARE_REGION {
	struct GBE_PXE_BOOT_AGENT_MAIN_SETUP_OPTIONS bootAgentMainSetupOptions;                                          /* Word 30h */
	struct GBE_PXE_BOOT_AGENT_CONFIGURATION_CUSTOMIZATION_OPTIONS_31H bootAgentConfigurationCustomizationOptions31h; /* Word 31h */
	struct GBE_PXE_BOOT_AGENT_CONFIGURATION_CUSTOMIZATION_OPTIONS_32H bootAgentConfigurationCustomizationOptions32h; /* Word 32h */
	struct GBE_PXE_IBA_CAPABILITIES ibaCapabilities;                                                                 /* Word 33h */
	
	/* Words 34h to 3Eh (padding). Set these to 0xFFFF (according to deblobbed_descriptor.bin) */
	unsigned short paddingWords34hTo3Eh[11];
	
	/*
	 * the pxe software region  is practically useless in libreboot, since
	 * libreboot does not include the intel boot agent (it's proprietary software). 
	 * 
	 * Having this struct in place is simply for documentations sake. It is completely
	 * irrelevant what you put here. filling it with 0xFFFF would probably be fine.
	 */
};

struct GBEREGIONRECORD_4K {
	unsigned char macAddress[6];                             /* Word 00 to 02 */
	struct GBE_RESERVED_WORD_03H reservedWord03h;            /* Reserved word 03. */
	unsigned short reservedWord04h;                          /* Reserved word 04: set it to 0xFFFF (according to datasheet) */
	unsigned short imageVersionInformation;                  /* Reserved word 05: 83 10 (little endian) in my deblobbed_descriptor.bin. Set this to 0x1083 (in C, assuming little endian byte order). "cannot be changed" according to datasheet */
	unsigned short reservedWords06h07h[2];                   /* Reserved words 06-07: set both to 0xFFFF (according to datasheet) */

	/*
	 * Word 08 and 09 (pba low and pba high):
	 * 
	 * Both of these should be set to 0xFFFF by default, according to the datasheet.
	 * "nine digit printed board assembly (PBA) number" for intel cards to be stored
	 * in a 4 byte (read: 2 word) field. 
	 * 
	 * Example: if pba number is 123456-003, then word 08 should be 1234h and word 09 becomes 5603.
	 * Note: 1234 and 5603 above are big endian. In the image it would actually be 34 12 and 0356
	 * 
	 * Example: in mine it was (in the image): 08 10 FF FF. That becomes 1008h and FFFFh, or
	 * basically: 1008FF-0FF. The same was observed in another.
	 * 
	 * Setting it to FF FF FF FF should be fine, according to the datasheet.
	 */
	unsigned short pbaLow;												/* Word 08. Set it to 0x1008. */
	unsigned short pbaHigh;												/* Word 09. Set it to 0xFFFF. */
	
	/* Word 0A */
	struct GBE_PCI_INITIALIZATION_CONTROL_WORD pciInitializationControlWord;
	
	/* 
	 * Word 0B; subsystem ID
	 * 
	 * If load subsystem ID bit of word 0A (pci init control word) is
	 * set to 1 (read: it is. in my deblobbed_descriptor.bin), store
	 * the subsystem id here. Datasheet says that the default value is
	 * 0000h, but you should set this to 20EEh (little endian: EE 20)
	 */
	unsigned short subsystemId; /* Set this to 0x20EE */
	
	/*
	 * Word 0C; subsystem vendor ID
	 * 
	 * If load subsystem vendor ID bit of word 0A (pci init control word)
	 * is set to 1 (read: it is. in my deblobbed_descriptor.bin), store
	 * the subsystem vendor id here. Datasheet says that the default 
	 * value is 8086h, but you should set this to 17AAh (lendian: AA 17).
	 */
	unsigned short subsystemVendorId; /* Set this to 0x17AA */
	
	/* 
	 * Word 0D: device ID
	 * 
	 * If load vendor/device ID in word 0A (pci init control word) is 1
	 * (it is) then this word is used to init device id using word 21h,
	 * 1Eh or 1Fh. In my case, deviceId is 0x10F5. Word 21h is set to
	 * 0x10CB, word 1Eh is 0x10F5 and 1Fh is 0x10BF
	 * 
	 * The datasheet says that 10F5 is for Intel 82567LM gigabit ethernet
	 * controller; 10BF is for Intel 82567LF and 10CB is for Intel 82567V.
	 * 
	 * Based on this, the X200 is shown to have the Intel 82567LM ethernet
	 * controller.
	 */
	unsigned short deviceId; /* Set this to 0x10F5. */
	/* It is important that this is correct, for the linux kernel driver */
	
	/* 
	 * Word 0E: vendor ID
	 * 
	 * If load vendor/device ID in word 0A (pci init control) is 1 (it is),
	 * then this word used read to initialize the PCI vendor ID. Default
	 * value is 8086 according to datasheets, and deblobbed_descriptor.bin. 
	 * 
	 * Intel is often 8086 as a PCI vendor ID. Because 8086. As in the CPU architecture.
	 */
	unsigned short vendorId;
	
	unsigned short deviceRevId;                              /* Word 0F: reserved bits. Set all bits to 0. */
	struct GBE_LAN_POWER_CONSUMPTION lanPowerConsumption;    /* Word 10: LAN Power Consumption (see struct definition) */
	unsigned short reservedWords11h12h[2];							/* Words 11-12: Reserved. Set both of them to 0x0000 (according to datasheet). */
	
	/* Word 13: Shared Initialization Control Word */
	struct GBE_SHARED_INITIALIZATION_CONTROL_WORD sharedInitializationControlWord;
	
	/* Word 14: Extended Configuration Control Word 1 */
	struct GBE_EXTENDED_CONFIGURATION_CONTROL_WORD_1 extendedConfigurationControlWord1;
	
	/* Word 15: Extended Configuration Control Word 2 */
	struct GBE_EXTENDED_CONFIGURATION_CONTROL_WORD_2 extendedConfigurationControlWord2;
	
	/* Word 16: Extended Configuration Control Word 3 */
	/* All bits reserved. Datasheet and deblobbed_descriptor.bin say to set it to zero */
	unsigned short extendedConfigurationControlWord3;
	
	struct LED_CTL_1 ledCtl1;                                /* Word 17: LED 1 Configuration and Power Management */
	struct LED_CTL_02 ledCtl02;                              /* Word 18: LED 0 and 2 Configuration Defaults */
	unsigned short reservedWord19h;                          /* Word 19: Reserved. Default is 0x2B00 according to datasheet, but in deblobbed_descriptor.bin it is 0x2B40 */
	unsigned short reservedWord1Ah;                          /* Word 1A: Reserved. Default is 0x0043 according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWord1Bh;                          /* Word 1B: Reserved. Should be 0x0000 according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWord1Ch;                          /* Word 1C: Reserved. Should be 0x10F5 according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWord1Dh;                          /* Word 1D: Reserved. Should be 0xBAAD according to datasheet and deblobbed_descriptor.bin */
	unsigned short _82567lmDeviceId;                         /* Word 1E: Device ID for Intel 82567LM gigabit ethernet controller (note: X200 uses this). Should be 0x10F5 according to datasheet and deblobbed_descriptor.bin*/
	unsigned short _82567lfDeviceId;                         /* Word 1F: Device ID for Intel 82567LF gigabit ethernet controller. Should be 0x10BF according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWord20h;									/* Word 20: Reserved. Should be 0xBAAD according to datasheet and deblobbed_descriptor.bin */
	unsigned short _82567vDeviceId;                          /* Word 21: Device ID for Intel 82567V gigabit ethernet controller. Should be 0x10CB according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWord22h;                          /* Word 22: Reserved. Should be 0xBAAD according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWord23h;                          /* Word 23: Reserved. Should be 0xBAAD according to datasheet and deblobbed_descriptor.bin */
	unsigned short reservedWords24to2Fh[12];                 /* Words 24-2F: Reserved. These should all be 0x0000 according to datasheet and deblobbed_descriptor.bin */
	struct GBE_PXE_SOFTWARE_REGION pxeSoftwareRegion;        /* Words 30-3E: PXE Software Region */
	unsigned short checkSum; /* when added to the sum of all words above, this should match GBECHECKSUMTOTAL */
	
	/* set all bytes in here to 0xFF */
	unsigned char padding[3968];
};

/*  main and backup region in gbe */ 
struct GBEREGIONRECORD_8K {
	struct GBEREGIONRECORD_4K main;
	struct GBEREGIONRECORD_4K backup;
	/* 
	 * Backup region:
	 * This is actually "main" on X200, since the real main has a bad checksum
	 * and other errors. You should do what you need on this one (if modifying
	 * lenovobios's gbe region) and then copy to main
	 */
};

/*
 * ---------------------------------------------------------------------
 * Function declarations (keep gcc/make happy. check them in gbe.c)
 * ---------------------------------------------------------------------
 */
 
unsigned short gbeGetRegionWordFrom8kBuffer(int index, char* regionData);
unsigned short gbeGetChecksumFrom8kBuffer(char* regionData, unsigned short desiredValue, int byteOffset);
unsigned short gbeGetChecksumFrom4kStruct(struct GBEREGIONRECORD_4K gbeStruct4k, unsigned short desiredValue);
struct GBEREGIONRECORD_8K deblobbedGbeStructFromFactory(struct GBEREGIONRECORD_8K factoryGbeStruct8k);
void printGbeChecksumDataFromStruct4k(struct GBEREGIONRECORD_4K gbeStruct4k, char* romName, char* regionName);
void printGbeChecksumDataFromStruct8k(struct GBEREGIONRECORD_8K gbeStruct8k, char* romName);

#endif
