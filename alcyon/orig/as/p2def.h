/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#define MOVEA 0100


adirect const opfary[] = {
	0,		/*  0 */
	opf1,	/*  1 */
	opf2,	/*  2 */
	opf3,	/*  3 */
	opf4,	/*  4 */
	opf5,	/*  5 */
	relbr,	/*  6 */
	opf7,	/*  7 */
	opf8,	/*  8 */
	opf9,	/*  9 */
	opf4,	/* 10 */
	opf11,	/* 11 */
	opf12,	/* 12 */
	opf13,	/* 13 */
	opf9,	/* 14 */
	opf15,	/* 15 */
	opf17,	/* 16 */
	opf17,	/* 17 */
	opf13,	/* 18 */
	opf11,	/* 19 */
	opf20,	/* 20 */
	opf21,	/* 21 */
	opf22,	/* 22 */
	opf23,	/* 23 */
	opf9,	/* 24 */
	opf9,	/* 25 */
	opf5,	/* 26 */		/* cmp, chk, extention verification */
	opf4,	/* 27 */		/* addx, subx, extension verification */
	opf13,	/* 28 */		/* swap, extension verification */
	opf9,	/* 29 */		/* pea, extention verification */
	opf15,  /* 30 */		/* lea, extension verification */
	opf31	/* 31 */		/* movec & moves 68010 */
};

#define LSTFRMT 31

short const f1mode[]  = { 0,      0,   0100, 0,   0200 };
short const f2mode[]  = { 0,      0,   0100, 0,   0200 };
short const f3mode[]  = { 0, 010000, 030000, 0, 020000 };
short const f15mode[] = { 0,      0,   0300, 0,   0700 };
short const f5mode[]  = { 0,      0,   0100, 0,   0200 };
short const f5amode[] = { 0,      0,   0300, 0,   0700 };
short const f13mode[] = { 0,      0,   0200, 0,   0300 };
short const f23mode[] = { 0,   0400,   0500, 0,   0600 };
short rlbits[5];		/* holds relocation bits for instr */
int pline;				/* number of last printed line */
int brkln2 = 077777;	/* pass 2 break line number for debugging */
short prsp;				/* special print alignment flag */ /* unused */
