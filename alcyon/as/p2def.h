/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#define MOVEA 0100


extern char tfilname[];	/*name of it file*/
extern char initfnam[];	/*name of the initilization file*/

int (*opfary[])() = {
	0,		/*0*/
	opf1,	/*1*/
	opf2,	/*2*/
	opf3,	/*3*/
	opf4,	/*4*/
	opf5,	/*5*/
	relbr,	/*6*/
	opf7,	/*7*/
	opf8,	/*8*/
	opf9,	/*9*/
	opf4,	/*10*/
	opf11,	/*11*/
	opf12,	/*12*/
	opf13,	/*13*/
	opf9,	/*14*/
	opf15,	/*15*/
	opf17,	/*16*/
	opf17,	/*17*/
	opf13,	/*18*/
	opf11,	/*19*/
	opf20,	/*20*/
	opf21,	/*21*/
	opf22,	/*22*/
	opf23,	/*23*/
	opf9,	/*24*/
	opf9,	/*25*/
	opf5,	/*26*/		/* [vlh] cmp, chk, extention verification */
	opf4,	/*27*/		/* [vlh] addx, subx, extension verification */
	opf13,	/*28*/		/* [vlh] swap, extension verification */
	opf9,	/*29*/		/* [vlh] pea, extention verification */
	opf15,  /*30*/		/* [vlh] lea, extension verification */
	opf31	/*31*/		/* [vlh] 4.2, movec & moves 68010 */
};

#define LSTFRMT 31

short f1mode[]  = {0,0,0100,0,0200};
short f2mode[]  = {0,0,0100,0,0200};
short f3mode[]  = {0,010000,030000,0,020000};
short f15mode[] = {0,0,0300,0,0700};
short f5mode[]  = {0,0,0100,0,0200};
short f5amode[] = {0,0,0300,0,0700};
short f13mode[] = {0,0,0200,0,0300};
short f23mode[] = {0,0400,0500,0,0600};
short rlbits[5];		/*holds relocation bits for instr*/
short pline;			/*number of last printed line*/
short brkln2 = 077777;	/*pass 2 break line number for debugging*/
short prsp;				/*special print alignment flag*/
