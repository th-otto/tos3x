/*
 * This file is part of ROMDUMP.
 *
 * ROMDUMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ROMDUMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ROMDUMP.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ROMDUMP: a simple utility to write out the ROM of Atari ST/STe/TT/Falcon.
 *
 * (c) 2018 Thorsten Otto
 *
 */

#include <stdio.h>
#include <osbind.h>

#define FALSE 0
#define TRUE 1

#ifdef __ALCYON__
typedef long ulong;
typedef char uchar;
#define UC_MASK(c) ((c) & 0xff)
#else
typedef unsigned long ulong;
typedef unsigned char uchar;
#define UC_MASK(c) c
#endif


static ulong romstart;
static ulong romsize;
static unsigned short tos_version;
static int is_emutos;
static char filename[256];

#define KB_192 (192 * 1024L)
#define KB_256 (256 * 1024L)
#define KB_512 (512 * 1024L)
#define MB_1   (1024 * 1024L)

static short const crc_tab[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};


static long getromaddr(NOTHING)
{
	return *((volatile long *)4);
}


static ulong get_osdate(P(ulong) date, P(int *) fixed)
PP(ulong date;)
PP(int *fixed;)
{
	if (fixed != NULL)
		*fixed = FALSE;
	/*
	 * EmuTOS before 2011-04-27 had the date in format YYYY-MM-DD,
	 * but TOS ROMs expect it to be in MM-DD-YYYY
	 */
	if ((date & 0xff000000L) >= 0x19000000L)
	{
		date = ((date & 0xffff) << 16) | ((date >> 16) & 0xffff);
		if (fixed != NULL)
			*fixed = TRUE;
	}
	date = ((date & 0xffff) << 16) | ((date >> 16) & 0xffff);
	return date;
}


static unsigned short calc_crc(P(ulong) pos, P(ulong) size, P(ulong) inc)
PP(ulong pos;)
PP(register ulong size;)
PP(register ulong inc;)
{
	register unsigned short sum;
	register unsigned short run;
	register const uchar *p;
	
	printf("calc_crc: %08lx %08lx %ld\n", pos, size, inc);
	p = (const uchar *)pos;
	sum = run = 0;
	do {
		run = sum;
		sum <<= 8;
		run >>= 8;
		run ^= UC_MASK(*p);
		sum ^= crc_tab[run];
		p += inc;
	} while (--size != 0);
	return sum;
}


int main(NOTHING)
{
	OSHEADER *hdr;
	const char *country;
	const char *ccode;
	unsigned short ctry;
	char ccode_buf[3];
	ulong date;
	int osdate_fixed;
	unsigned short dosdate;
	int i, banks;
	unsigned short crc, sum;
	uchar *crcpos;
	ulong banksize;
	int fd;
	int err = 0;
	
#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	romstart = Supexec(getromaddr) & 0x00ffff0fL;
	
	hdr = (OSHEADER *)romstart;

	tos_version = hdr->os_version;
	is_emutos = (ulong)hdr->p_rsv2 == 0x45544f53L;
	if (tos_version == 0x206 && hdr->os_dosdate == 0x186A)
		tos_version = 0x208;
	romsize = romstart == 0xfc0000L ? KB_192 : MB_1;
	
	printf("romstart: %08lx\n", romstart);
	printf("version: %x.%02x%s\n", tos_version >> 8, tos_version & 0xff, is_emutos ? " (EmuTOS)" : "");

	if ((tos_version >= 0x400 && tos_version < 0x500) && romsize > KB_256)
	{
		/*
		 * Falcon TOS 4.x: a single 512KB ROM
		 */
		banks = 1;
		romsize = KB_512;
		banksize = ((romsize + 1) & ~1) - 2;
	} else if ((tos_version == 0x208) &&
		romsize > KB_256 &&
		!is_emutos)
	{
		/*
		 * ST-Book TOS 2.x: a single 256KB ROM
		 * The second half of the image contains a cartridge ROM
		 */
		romsize = KB_256;
		banks = 1;
		banksize = (((romsize >> 1) + 1) & ~1) - 2;
	} else if (romsize > KB_256)
	{
		/*
		 * TT TOS 3.x: 4 128KB ROMs
		 */
		romsize = KB_512;
		banks = 4;
		banksize = (((romsize + 3) >> 2) & ~1) - 2;
	} else if (romsize > KB_192)
	{
		/*
		 * STE TOS 2.x: 2 128KB ROMs
		 */
		romsize = KB_256;
		banks = 2;
		/*
		 * size should be only -4 (for the 4 checksum bytes at the end),
		 * but i've seen patched TOSes that are 1 byte too short,
		 * hence the +1
		 */
		banksize = (((romsize + 1) >> 1) & ~1) - 2;
	} else if (romstart == 0xfc0000L)
	{
		/*
		 * TOS 1.x: 6 32KB ROMs, without CRC
		 */
		romsize = KB_192;
		banks = -1;
		banksize = 0;
	} else
	{
		printf("cannot determine rom size\n");
		fflush(stdout);
		Cnecin();
		return 1;
	}

	if (banks > 0)
	{
		for (i = 0; i < banks; i++)
		{
			crc = calc_crc(romstart + i, banksize, (ulong)banks);
			crcpos = (uchar *)(romstart + banksize * banks + i);
			sum = (UC_MASK(crcpos[0]) << 8) | UC_MASK(crcpos[banks]);
			if (crc != sum)
			{
				printf("WARNING: BAD ROM CRC IN CHIP %d: %04x != %04x\n", i, sum, crc);
			}
		}
	}

	ctry = (hdr->os_conf >> 1) & 0x7f;
	switch (ctry)
	{
		case 0: country = "USA"; ccode = "us"; break;
		case 1: country = "Germany"; ccode = "de"; break;
		case 2: country = "France"; ccode = "fr"; break;
		case 3: country = "UK"; ccode = "uk"; break;
		case 4: country = "Spain"; ccode = "es"; break;
		case 5: country = "Italy"; ccode = "it"; break;
		case 6: country = "Sweden"; ccode = "se"; break;
		case 7: country = "Switzerland (french)"; ccode = "sf"; break;
		case 8: country = "Switzerland (german)"; ccode = "sg"; break;
		case 9: country = "Turkey"; ccode = "tr"; break;
		case 10: country = "Finland"; ccode = "fi"; break;
		case 11: country = "Norway"; ccode = "no"; break;
		case 12: country = "Denmark"; ccode = "dk"; break;
		case 13: country = "Saudi Arabia"; ccode = "sa"; break;
		case 14: country = "Netherlands"; ccode = "nl"; break;
		case 15: country = "Czechoslovakia"; ccode = "cs"; break;
		case 16: country = "Hungary"; ccode = "hu"; break;
		case 17: country = "Poland"; ccode = "pl"; break;
		case 18: country = "Lithuania"; ccode = "lt"; break;
		case 19: country = "Russia"; ccode = "ru"; break;
		case 20: country = "Estonia"; ccode = "et"; break;
		case 21: country = "Belarus"; ccode = "by"; break;
		case 22: country = "Ukraine"; ccode = "ua"; break;
		case 23: country = "Slovak Republic"; ccode = "sk"; break;
		case 24: country = "Romania"; ccode = "ro"; break;
		case 25: country = "Bulgaria"; ccode = "bg"; break;
		case 26: country = "Slovenia"; ccode = "sl"; break;
		case 27: country = "Croatia"; ccode = "hr"; break;
		case 28: country = "Serbia"; ccode = "sr"; break;
		case 29: country = "Montenegro"; ccode = "me"; break;
		case 30: country = "Macedonia"; ccode = "mk"; break;
		case 31: country = "Greece"; ccode = "el"; break;
		case 32: country = "Latvia"; ccode = "lv"; break;
		case 33: country = "Israel"; ccode = "he"; break;
		case 34: country = "South Africa"; ccode = "af"; break;
		case 35: country = "Portugal"; ccode = "pt"; break;
		case 36: country = "Belgium"; ccode = "be"; break;
		case 37: country = "Japan"; ccode = "jp"; break;
		case 38: country = "China"; ccode = "cn"; break;
		case 39: country = "Korea"; ccode = "ko"; break;
		case 40: country = "Vietnam"; ccode = "vn"; break;
		case 41: country = "India"; ccode = "in"; break;
		case 42: country = "Iran"; ccode = "ir"; break;
		case 43: country = "Mongolia"; ccode = "mn"; break;
		case 44: country = "Nepal"; ccode = "np"; break;
		case 45: country = "Lao People's Democratic Republic"; ccode = "la"; break;
		case 46: country = "Cambodia"; ccode = "kh"; break;
		case 47: country = "Indonesia"; ccode = "id"; break;
		case 48: country = "Bangladesh"; ccode = "bd"; break;
		case 127: country = "Multi-Language"; ccode = "xx"; break;
		default:
			country = "unknown";
			sprintf(ccode_buf, "%02x", ctry);
			ccode = ccode_buf;
			break;
	}
	
	printf("Language: %s (%s)\n", country, ccode);
	
	date = get_osdate(hdr->os_date, &osdate_fixed);
	printf("TOS date: %04x-%02x-%02x%s\n",
		(unsigned int) ((date >> 16) & 0xffff),
		(unsigned int) ((date >> 8) & 0xff),
		(unsigned int) ((date >> 0) & 0xff),
		osdate_fixed ? " (fixed)" : "");
	dosdate = hdr->os_dosdate;
	printf("DOS date: %04d-%02d-%02d\n", ((dosdate >> 9) & 0x7f) + 1980, (dosdate >> 5) & 0x0f, dosdate & 0x1f);
	
	sprintf(filename, "tos%03x%s.img", tos_version, ccode);
	printf("writing %dK rom to %s\n", (unsigned int)(romsize >> 10), filename);
	
	fd = (int)Fcreate(filename, 0);
	if (fd < 0)
	{
		printf("can't create %s\n", filename);
	} else
	{
		err = Fwrite(fd, romsize, (VOIDPTR)romstart) != romsize;
		Fclose(fd);
		if (err)
			printf("error writing %s\n", filename);
	}
	
	printf("press any key to exit");
	fflush(stdout);
	Cnecin();

	return err;
}
