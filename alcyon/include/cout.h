/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sys/cout.h - Jul 26, 1983  REGULUS 4.1
*/

#ifndef __COUT_H__
#define __COUT_H__ 1

#include <stdint.h>

struct hdr
{
	unsigned short ch_magic;			/* c.out magic number 060016 = $600E */
	int32_t ch_tsize;					/* text size */
	int32_t ch_dsize;					/* data size */
	int32_t ch_bsize;					/* bss size */
	int32_t ch_ssize;					/* symbol table size */
	int32_t ch_stksize;					/* stack size */
	int32_t ch_entry;					/* location of entry point */
	unsigned short ch_rlbflg;			/* relocation bits present flag, must be signed */
};

struct hdr_cout
{
	unsigned short ftype;				/* c.out magic number 060016 = $600E */
	int32_t ftext;						/* text size */
	int32_t fdata;						/* data size */
	int32_t fbss;						/* bss size */
	int32_t fsym;						/* symbol table size */
	int32_t fssize;						/* stack size */
	int32_t fentry;						/* location of entry point */
	short fflag;						/* relocation bits present flag, must be signed */
};

struct hdr2
{
	unsigned short ch_magic;			/* c.out magic number = 601B hex */
	int32_t ch_tsize;					/* # bytes in program text segment */
	int32_t ch_dsize;					/* # bytes in program data segment */
	int32_t ch_bsize;					/* # bytes in program bss  segment */
	int32_t ch_ssize;					/* # bytes in symbol table */
	int32_t ch_stksize;					/* initial stack size */
	int32_t ch_entry;					/* entry point--address of text segment */
	unsigned short ch_rlbflg;			/* relocation bits suppressed flag, must be signed */
	int32_t ch_dstart;					/* address of data segment */
	int32_t ch_bstart;					/* address of bss segment */
};

/* structure of the symbol table */

#define SYNAMLEN	8

struct nlist {
	char n_name[SYNAMLEN];
	unsigned short n_type;
	int32_t n_value;
};
#define OSTSIZE    14      /* symbol table entry length on object file */

/* HDSIZE should be 28 bytes, HDSIZ2 should be 36 bytes */
#define HDSIZE		28		/* sizeof(hdr) on disk */
#define HDSIZ2		36		/* sizeof(hdr2) on disk */

#define MAGIC		(unsigned short) 0x601a	/*  bra .+26 instruction */
#define MAGIC1		(unsigned short) 0x601b	/* data & bss base defined */
#define MAGICST2 	(unsigned short) 0x601c	/* shared text 2K boundary */
#define MAGICID 	(unsigned short) 0x601d	/* I & D split */
#define MAGICST		(unsigned short) 0x601e	/* shared test 4k boundary (-n default) */

#define EX_MAGIC	(unsigned short) 0x601a
#define EX_ABMAGIC	(unsigned short) 0x601b
#define EX_2KSTXT	(unsigned short) 0x601c
#define EX_IDMAGIC	(unsigned short) 0x601d
#define EX_4KSTXT	(unsigned short) 0x601e

#define SHT2KBOUND		2048
#define SHT2KFIX(x)		(((x+SHT2KBOUND-1)/SHT2KBOUND)*SHT2KBOUND)
#define SHT4KBOUND		4096
#define SHT4KFIX(x)		(((x+SHT4KBOUND-1)/SHT4KBOUND)*SHT4KBOUND)

#endif /* __COUT_H__ */
