/****************************************************************************/
/*                                                                          */
/*                          R e l o c   P r o g r a m                       */
/*                          -------------------------                       */
/*                                                                          */
/*      This program is used to relocate an Alcyon format load module into  */
/*      an absolute module using the minimum disk space possible.  The      */
/*      relocation information, symbol table, and trailing zeros in the     */
/*      data segment are deleted.                                           */
/*                                                                          */
/*      Invokation:                                                         */
/*                                                                          */
/*              A>reloc -bhhhhhh in.68k out.68k                             */
/*                                                                          */
/*      Where:                                                              */
/*              hhhhhh  Is the desired new base address (TPA + 100) in hex  */
/*              in.68k  Is the input file                                   */
/*              out.68k Is the output file                                  */
/*                                                                          */
/****************************************************************************/
#include "../common/linux/libcwrap.h"
#include "../include/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "cout.h"						/* Output header area       */

/****************************************************************************/
/*                                                                          */
/*                      Relocation bit definitions                          */
/*                                                                          */
/*      Note that this program is intended only for load module files. Any  */
/*      references to external symbols or relative references are illegal.  */
/*                                                                          */
/****************************************************************************/

#define	DABS	0	/* Data absolute        */
#define	DRELOC	1	/* Data segment relative    */
#define	TRELOC	2	/* Text segment relative    */
#define	BRELOC	3	/* BSS  segment relative    */
#define	EXTVAL	4	/* External variable        */
#define	LUPPER	5	/* Upper word of long       */
#define	EXTREL	6	/* External relative symbol */
#define	INSABS	7	/* 1st word of instruction  */


/****************************************************************************/
/*                                                                          */
/*                      G l o b a l   D a t a                               */
/*                                                                          */
/****************************************************************************/

#define DEFAULT_TPA 0xa900

static char *ifname = 0;						/* -> Input filename        */

static char *ofname = 0;						/* -> Output filename       */

static int32_t base = 0;							/* New relocation base      */


static struct hdr couthd;


static unsigned short get16be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	register unsigned int w1;
	w1 = getc(sp);
	w1 <<= 8;
	return w1 | getc(sp);
}


static int put16be(P(int) wrd, P(FILE *) sp)
PP(int wrd;)								/* the word to be put       */
PP(FILE *sp;)								/* the stream to put to     */
{
	if (putc(wrd >> 8, sp) < 0)
		return EOF;
	if (putc(wrd, sp) < 0)
		return EOF;
	return 0;
}

static long get32be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	register unsigned int w1;
	w1 = get16be(sp);
	return ((long)w1 << 16) | get16be(sp);
}


static int put32be(P(long) l, P(FILE *) sp)
PP(long l;)
PP(register FILE *sp;)
{
	if (put16be((unsigned short)(l >> 16), sp) < 0)
		return EOF;
	if (put16be((unsigned short)l, sp) < 0)
		return EOF;
	return 0;
}


/****************************************************************************/
/*                                                                          */
/*                          U s a g e   F u n c t i o n                     */
/*                          ---------------------------                     */
/*                                                                          */
/*      Routine "usage" is used to print out an error message when the      */
/*      program is invoked in an improper manner.                           */
/*                                                                          */
/*      Calling Sequence:                                                   */
/*                                                                          */
/*              usage();                                                    */
/*                                                                          */
/*      No return is made, "exit()" is called to go back to the O/S.        */
/*                                                                          */
/****************************************************************************/
static VOID usage(NOTHING)
{
	FILE *out = stderr;
	
	fprintf(out, "Usage: reloc [-bhhhhhh] input output\n");
	fprintf(out, "       where:  hhhhhh is new base address\n");
	fprintf(out, "               (TPA+100 is default base address)\n");
	fprintf(out, "               input  is relocatable file\n");
	fprintf(out, "               output is absolute file\n");
}

/****************************************************************************/
/*                                                                          */
/*                      D e c o d e    F u n c t i o n                      */
/*                      ------------------------------                      */
/*                                                                          */
/*      Routine "decode" is called to process the relocation base argument  */
/*      from the command line.                                              */
/*                                                                          */
/*      Calling sequence:                                                   */
/*                                                                          */
/*              ret = decode(string,&address);                              */
/*                                                                          */
/*      Where:                                                              */
/*              string   -> argument string (usually argv[1])               */
/*              &address -> long word to receive converted value            */
/*                                                                          */
/*              ret      =  0 if successful conversion                      */
/*                       = -1 if anything amiss                             */
/*                                                                          */
/****************************************************************************/
static int decode(P(char *) string, P(int32_t *) addr)
PP(register char *string;)						/* -> Command argument      */
PP(int32_t *addr;)								/* =  return value      */
{
	register int32_t a;						/* Temp return value        */
	register char c;						/* Temp character       */

	if (*string++ != '-' || *string++ != 'b')	/* Check for valid switch   */
		return -1;						/* quit if NFG          */

	a = 0;								/* Zero out accumulator     */
	while (*string)						/* Until no more chars      */
	{
		c = *string & 0177;				/* Pick up next char        */
		if (c >= '0' && c <= '9')		/* Digit            */
			a = (a << 4) + c - '0';		/* Un-ASCIIfy           */
		else if (c >= 'A' && c <= 'F')	/* Hex              */
			a = (a << 4) + c - 'A' + 10;	/* Un-ASCIIfy           */
		else if (c >= 'a' && c <= 'f')	/* Lower case hex       */
			a = (a << 4) + c - 'a' + 10;	/* Un-ASCII it          */
		else							/* Bad character        */
			return -1;					/* So long, sucker!!        */
		string++;						/* Increment pointer        */
	}
	*addr = a;							/* Store result         */
	return 0;							/* Return all ok        */
}


/* Print bad reloc message  */
static VOID badrel(P(short) r, P(int32_t) offset)
PP(short r;)									/* Relocation bits      */
PP(int32_t offset;)							/* File offset          */
{
	fprintf(stderr, "Illegal reloc = %x at %lx\n",	/* Print error message      */
		   r, (long)offset);
	exit(1);							/* quit             */
}

/*****************a***********************************************************/
/*                                                                          */
/*                          R e l o c   F u n c t i o n                     */
/*                          ---------------------------                     */
/*                                                                          */
/*      Function "reloc" is called to perform the relocation operation and  */
/*      write the output file simultaneously.                               */
/*                                                                          */
/*      Calling Sequence:                                                   */
/*                                                                          */
/*              reloc(ifp,rfp,ofp,length);                                  */
/*                                                                          */
/*      Where:                                                              */
/*                                                                          */
/*              ifp     Is the input stream pointer (positioned at text)    */
/*              rfp     Is a stream pointer positioned at relocation bits   */
/*              ofp     Is a stream pointer for the output file             */
/*              length  Is the total length of the output file              */
/*                                                                          */
/****************************************************************************/
static VOID reloc(P(FILE *) ifp, P(FILE *) rfp, P(FILE *) ofp, P(int32_t) length)
PP(register FILE *ifp;)							/* -> Text information      */
PP(register FILE *rfp;)							/* -> Relocation info       */
PP(register FILE *ofp;)							/* -> Output file       */
PP(int32_t length;)							/* =  # bytes in the file   */
{
	struct hdr x;						/* Duplicate header     */
	register int32_t bytes;				/* File offset          */
	unsigned int loword, hiword;		/* Temp for long relocation */
	int32_t j;
	register short r;					/* Relocation word      */
	register short k;					/* Temp data word       */
	short longf;						/* Long relocation flag     */
	int32_t bias;						/* Relocation bias      */

	bias = base - couthd.ch_entry;		/* Compute relocation bias  */

	/*************************************************			    */
	/*									    */
	/*	Prepare and write the new file header in structure "x".		    */
	/*									    */
	/*************************************************			    */
	x.ch_magic = MAGIC;					/* Magic word           */
	x.ch_tsize = couthd.ch_tsize;		/* Text size            */
	j = HDSIZE + couthd.ch_tsize +	/* Compute number of bytes  */
		couthd.ch_dsize - length;		/* moved from data to bss   */
	x.ch_dsize = couthd.ch_dsize - j;	/* New data size        */
	x.ch_bsize = couthd.ch_bsize + j;	/* New bss size         */
	x.ch_ssize = 0;						/* No symbols           */
	x.ch_stksize = 0;					/* No stack         */
	x.ch_entry = base;					/* New base address     */
	x.ch_rlbflg = -1;					/* No relocation bits       */

	/* Do the write         */
	if (put16be(x.ch_magic, ofp) < 0 ||
		put32be(x.ch_tsize, ofp) < 0 ||
		put32be(x.ch_dsize, ofp) < 0 ||
		put32be(x.ch_bsize, ofp) < 0 ||
		put32be(x.ch_ssize, ofp) < 0 ||
		put32be(x.ch_stksize, ofp) < 0 ||
		put32be(x.ch_entry, ofp) < 0 ||
		put16be(x.ch_rlbflg, ofp) < 0)
	{									/* Here if failed       */
		fprintf(stderr, "Write error on %s\n", ofname);	/* print message        */
		exit(1);						/* Quit             */
	}

	/****************************************************************************/
	/*                                                                          */
	/* Here begins the actual file relocation procedure.  Read a word           */
	/* from the relocation bits and from the file together.  If long,           */
	/* read another word from each.  Perform indicated relocation.              */
	/*                                                                          */
	/****************************************************************************/
	bytes = 0;							/* No bytes written yet ... */
	length -= HDSIZE;			/* Just program, please...  */
	while (bytes < length)				/* until we are done        */
	{
		k = get16be(ifp);					/* get next text/data word  */
		r = get16be(rfp);					/* get next relocation word */

		if ((r & ~7) != 0)				/* Check for validity       */
			badrel(r, bytes);			/* Not valid, quit      */

		if (r == LUPPER)				/* Doing a long?        */
		{
			hiword = k;				/* Yes, get another word    */
			loword = get16be(ifp);		/* From both            */
			r = get16be(rfp);				/*      streams     */
			longf = 2;					/* Set flag         */
			j = ((int32_t)k << 16) | loword;
		}
		else							/* Not a long           */
		{
			loword = k;					/* Put in low order word    */
			hiword = 0;					/* Make top part 0      */
			longf = 0;					/* Clear flag           */
			j = k;
		}
		switch (r)						/* Now do indicated reloc   */
		{
		case DRELOC:
		case TRELOC:					/* If relocatable, relocate */
		case BRELOC:
			j += bias;					/* Add in bias          */
			loword = (unsigned short)j;
			hiword = (unsigned short)((j >> 16) & 0xffff);
			break;
		case DABS:						/* If not relocatable, don't */
		case INSABS:
			break;
		default:						/* Any others illegal       */
			badrel(r, bytes);			/* Print message        */
			break;
		}
		if (longf != 0)					/* Long?            */
			put16be(hiword, ofp);		/*  yes, output high    */
		if (put16be(loword, ofp) != 0)	/* Check for write error    */
		{
			fprintf(stderr, "Write error on %s ", ofname);	/* Had one,             */
			fprintf(stderr, "Offset = %lx data= %x\n",
				   (long)bytes, loword);	/* Print out message        */
			exit(1);					/*        & quit    */
		}
		bytes += 2 + longf;				/* Increment byte count     */
	}
}


#include "../common/linux/libcmain.h"

/****************************************************************************/
/*                                                                          */
/*                          M a i n   P r o c e d u r e                     */
/*                          ---------------------------                     */
/*                                                                          */
/*      Here we get things started:                                         */
/*                                                                          */
/*              1). Get relocation base                                     */
/*              2). Open input and output files                             */
/*              3). Determine data area (new) size                          */
/*                                                                          */
/*      Any errors here are fatal, and cause program termination.           */
/*                                                                          */
/****************************************************************************/

int main(P(int) argc, P(char **) argv)
PP(int argc;)					/* Int var from RTL startup */
PP(char **argv;)					/* -> Arg strings       */
{
	register int32_t i, l;				/* Temps            */
	register FILE *ifp, *ofp,*rfp;		/* File pointers        */

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (argc != 4 && argc != 3)		/* Gotta have right format  */
	{
		usage();						/* Print out nasty message  */
		return 1;
	}

	if (argc == 4)						/* "-bxxxx" form        */
	{
		if (decode(argv[1], &base) < 0)	/* Convert to binary        */
		{
			fprintf(stderr, "Illegal option: %s\n",	/* Print offending option   */
				   argv[1]);
			usage();					/* Print reminder messages  */
			return 1;
		}
		ifname = argv[2];				/* -> Input file name       */
		ofname = argv[3];				/* -> Output file name      */
	} else								/* Default form         */
	{
		base = DEFAULT_TPA;					/* Use start of reloc itself */
		ifname = argv[1];				/* -> Input filename        */
		ofname = argv[2];				/* -> Output filename       */
	}

	if ((base & 1) != 0)				/* In range?            */
	{									/*  no          */
		fprintf(stderr, "Illegal base address=%lx\n", (long) base);	/*      print message  */
		return 1;						/* And quit         */
	}

	if ((ifp = fopen(ifname, "rb")) == NULL)	/* Try to open input        */
	{
		fprintf(stderr, "Cannot open %s\n", ifname);	/* Complain         */
		return 1;						/*      & quit      */
	}

	if ((ofp = fopen(ofname, "wb")) == NULL)	/* Try to create output     */
	{
		fprintf(stderr, "Cannot create %s\n", ifname);	/* Complain         */
		return 1;						/*      & quit      */
	}


	/****************************************************************************/
	/*                                                                          */
	/* Now read the file header and compute the new data segment size ...       */
	/*                                                                          */
	/****************************************************************************/
	/* Do the read */
	couthd.ch_magic = get16be(ifp);
	couthd.ch_tsize = get32be(ifp);
	couthd.ch_dsize = get32be(ifp);
	couthd.ch_bsize = get32be(ifp);
	couthd.ch_ssize = get32be(ifp);
	couthd.ch_stksize = get32be(ifp);
	couthd.ch_rlbflg = get16be(ifp);

	if (couthd.ch_magic != MAGIC)		/* Valid file?          */
	{
		fprintf(stderr, "File format error: %s\n",	/* Print nasty message      */
			   ifname);
		return 1;						/* Quit             */
	}

	l = couthd.ch_tsize + HDSIZE;	/* Seek address for data    */
	fseek(ifp, l, 0);					/* Seek to data start       */

	i = couthd.ch_dsize;				/* Fetch data size      */
	l = ftell(ifp);						/* l <= new data end addr   */

	while (i > 0)						/* Until all data examined  */
	{
		if (get16be(ifp) != 0)			/* This data word = 0?      */
			l = ftell(ifp);				/* No, record seek address  */
		i -= 2;							/* Down count           */
	}

	/****************************************************************************/
	/*                                                                          */
	/*      Open the file a second time, and perform the proper seeks to align  */
	/*      one copy of the open file to the beginning of the text segment, and */
	/*      the other copy to the beginning of the relocation information       */
	/*                                                                          */
	/****************************************************************************/

	if ((rfp = fopen(ifname, "rb")) == NULL)	/* Try to open again        */
	{
		fprintf(stderr, "Cannot re-open %s\n", ifname);	/* Print nasty message      */
		return 1;						/*   and quit       */
	}
	fseek(ifp, (int32_t) (HDSIZE), 0);	/* Seek to text beginning   */
	fseek(rfp, (int32_t) (HDSIZE) +	/* Seek to          */
		  couthd.ch_tsize + couthd.ch_dsize +	/*       relocation     */
		  couthd.ch_ssize, 0);			/*          info */

	reloc(ifp, rfp, ofp, l);			/* Do relocation        */
	
	return 0;
}
