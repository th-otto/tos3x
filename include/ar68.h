/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) ar68.h - Jul 26, 1983  REGULUS 4.1
*/

#define LIBMAGIC	0xff65
#define LIBHDSIZE	26
#define LIBNSIZE	14

struct libhdr
{
	char lfname[LIBNSIZE];
	long lmodti;
	char luserid;
	char lgid;
	unsigned short lfimode;
	long lfsize;
};
