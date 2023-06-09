/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) ar68.h - Jul 26, 1983  REGULUS 4.1
*/

#ifndef __AR68_H__
#define __AR68_H__ 1

#include <stdint.h>

#define LIBMAGIC	0xff65
#define LIBRMAGIC	0xff66
#define LIBHDSIZE	28
#define LIBNSIZE	14

struct libhdr
{
	char lfname[LIBNSIZE];
	int32_t lmodti;
	char luserid;
	char lgid;
	unsigned short lfimode;
	int32_t lfsize;
	unsigned short junk;
};

#endif
