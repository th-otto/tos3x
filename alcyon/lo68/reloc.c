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
#include <stdio.h>
#include <ctype.h>
/****************************************************************************/
/*                                                                          */
/*      The following code defines the organization of a "long" variable on */
/*      either the VAX (11) or 68000.  This allows us to use "getw" on the  */
/*      separate parts of the long and to recombine them for 32-bit         */
/*      arithmetic operations.                                              */
/*                                                                          */
/****************************************************************************/

#include "machine.h"
#ifdef	UNIX
#include <portab.h>
#endif

#ifdef	VAX
struct
{			/* low word first on VAX    */
	WORD loword;
	WORD hiword;
};
#endif

#ifdef	PDP11
struct
{			/* low word first on PDP11  */
	WORD loword;
	WORD hiword;
};
#endif

#ifdef	MC68000
struct
{
	WORD hiword;	/* High word first on 68K   */
	WORD loword;
};
#endif

#ifdef	UNIX					/****************************/
#define	fopenb	fopen				/* Not in UNIX library      */
#endif						/****************************/

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

BYTE *ifname = 0;						/* -> Input filename        */

BYTE *ofname = 0;						/* -> Output filename       */

LONG base = 0;							/* New relocation base      */

#ifdef	CPM
EXTERN BYTE _start;						/* Start of program     */
#endi

#include "cout.h"						/* Output header area       */


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

VOID main(argc, argv)
DEFAULT argc;					/* Int var from RTL startup */
BYTE **argv;					/* -> Arg strings       */
{
	REG LONG i, l;				/* Temps            */
	REG FILE *ifp, *ofp,*rfp;		/* File pointers        */

	if (argc != 4 && argc != 3)		/* Gotta have right format  */
		usage();						/* Print out nasty message  */

	if (argc == 4)						/* "-bxxxx" form        */
	{
		if (decode(argv[1], &base) == FAILURE)	/* Convert to binary        */
		{
			printf("Illegal option: %s\n",	/* Print offending option   */
				   argv[1]);
			usage();					/* Print reminder messages  */
		}
		ifname = argv[2];				/* -> Input file name       */
		ofname = argv[3];				/* -> Output file name      */
	} else								/* Default form         */
	{
#ifdef	CPM								/* On CP/M,         */
		base = &_start;					/* Use start of reloc itself */
#else
		base = 0x500;					/*      500H is default */
#endif
		ifname = argv[1];				/* -> Input filename        */
		ofname = argv[2];				/* -> Output filename       */
	}

	if ((base & 1) != 0)				/* In range?            */
	{									/*  no          */
		printf("Illegal base address=%lx\n", base);	/*      print message  */
		exit(-1);						/* And quit         */
	}

	if ((ifp = fopenb(ifname, "r")) == NULL)	/* Try to open input        */
	{
		printf("Cannot open %s\n", ifname);	/* Complain         */
		exit(-1);						/*      & quit      */
	}

	if ((ofp = fopenb(ofname, "w")) == NULL)	/* Try to create output     */
	{
		printf("Cannot create %s\n", ifname);	/* Complain         */
		exit(-1);						/*      & quit      */
	}


	/****************************************************************************/
	/*                                                                          */
	/* Now read the file header and compute the new data segment size ...       */
	/*                                                                          */
	/****************************************************************************/
	i = fread(&couthd, 1, (sizeof couthd),	/* Do the read          */
			  ifp);
	if (i != (sizeof couthd))			/* Did it work??        */
	{
		printf("Read error on %s\n", ifname);	/* No, complain         */
		exit(-1);						/* and quit         */
	}

	if (couthd.ch_magic != MAGIC)		/* Valid file?          */
	{
		printf("File format error: %s\n",	/* Print nasty message      */
			   ifname);
		exit(-1);						/* Quit             */
	}

	l = couthd.ch_tsize + sizeof couthd;	/* Seek address for data    */
	fseek(ifp, l, 0);					/* Seek to data start       */

	i = couthd.ch_dsize;				/* Fetch data size      */
	l = ftell(ifp);						/* l <= new data end addr   */

	while (i > 0)						/* Until all data examined  */
	{
		if (getw(ifp) != 0)				/* This data word = 0?      */
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

	if ((rfp = fopenb(ifname, "r")) == NULL)	/* Try to open again        */
	{
		printf("Cannot re-open %s\n", ifname);	/* Print nasty message      */
		exit(-1);						/*   and quit       */
	}
	fseek(ifp, (LONG) (sizeof couthd), 0);	/* Seek to text beginning   */
	fseek(rfp, (LONG) (sizeof couthd) +	/* Seek to          */
		  couthd.ch_tsize + couthd.ch_dsize +	/*       relocation     */
		  couthd.ch_ssize, 0);			/*          info */

	reloc(ifp, rfp, ofp, l);			/* Do relocation        */
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
VOID usage()
{
	printf("Usage: reloc [-bhhhhhh] input output\n");
	printf("       where:  hhhhhh is new base address\n");
	printf("               (TPA+100 is default base address)\n");
	printf("               input  is relocatable file\n");
	printf("               output is absolute file\n");
	exit(-1);
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
WORD decode(string, addr)
REG BYTE *string;						/* -> Command argument      */
LONG *addr;								/* =  return value      */
{
	REG LONG a;							/* Temp return value        */
	REG BYTE c;							/* Temp character       */

	if (*string++ != '-' || *string++ != 'b')	/* Check for valid switch   */
		return (FAILURE);				/* quit if NFG          */

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
			return (FAILURE);			/* So long, sucker!!        */
		string++;						/* Increment pointer        */
	}
	*addr = a;							/* Store result         */
	return (SUCCESS);					/* Return all ok        */
}

/****************************************************************************/
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
VOID reloc(ifp, rfp, ofp, length)
REG FILE *ifp;							/* -> Text information      */
REG FILE *rfp;							/* -> Relocation info       */
REG FILE *ofp;							/* -> Output file       */
LONG length;							/* =  # bytes in the file   */
{
	struct hdr x;						/* Duplicate header     */
	REG LONG bytes;						/* File offset          */
	LONG j;								/* Temp for long relocation */
	REG WORD r;							/* Relocation word      */
	REG WORD k;							/* Temp data word       */
	WORD longf;							/* Long relocation flag     */
	WORD z;								/* Temp for error codes     */
	LONG bias;							/* Relocation bias      */

	bias = base - couthd.ch_entry;		/* Compute relocation bias  */

	/*************************************************			    */
	/*									    */
	/*	Prepare and write the new file header in structure "x".		    */
	/*									    */
	/*************************************************			    */
	x.ch_magic = MAGIC;					/* Magic word           */
	x.ch_tsize = couthd.ch_tsize;		/* Text size            */
	j = sizeof couthd + couthd.ch_tsize +	/* Compute number of bytes  */
		couthd.ch_dsize - length;		/* moved from data to bss   */
	x.ch_dsize = couthd.ch_dsize - j;	/* New data size        */
	x.ch_bsize = couthd.ch_bsize + j;	/* New bss size         */
	x.ch_ssize = 0;						/* No symbols           */
	x.ch_stksize = 0;					/* No stack         */
	x.ch_entry = base;					/* New base address     */
	x.ch_rlbflg = -1;					/* No relocation bits       */
						/****************************/
	if (fwrite(&x, 1, sizeof x, ofp) !=	/* Do the write         */
		sizeof x)
	{									/* Here if failed       */
		printf("Write error on %s\n", ofname);	/* print message        */
		exit(-1);						/* Quit             */
	}

	/****************************************************************************/
	/*                                                                          */
	/* Here begins the actual file relocation procedure.  Read a word           */
	/* from the relocation bits and from the file together.  If long,           */
	/* read another word from each.  Perform indicated relocation.              */
	/*                                                                          */
	/****************************************************************************/
	bytes = 0;							/* No bytes written yet ... */
	length -= sizeof(couthd);			/* Just program, please...  */
	while (bytes < length)				/* until we are done        */
	{
		k = getw(ifp);					/* get next text/data word  */
		r = getw(rfp);					/* get next relocation word */

		if ((r & ~7) != 0)				/* Check for validity       */
			badrel(r, bytes);			/* Not valid, quit      */

		if (r == LUPPER)				/* Doing a long?        */
		{
			j.hiword = k;				/* Yes, get another word    */
			j.loword = getw(ifp);		/* From both            */
			r = getw(rfp);				/*      streams     */
			longf = 2;					/* Set flag         */
		}
		else							/* Not a long           */
		{
			j.loword = k;				/* Put in low order word    */
			j.hiword = 0;				/* Make top part 0      */
			longf = 0;					/* Clear flag           */
		}
		switch (r)						/* Now do indicated reloc   */
		{
		case TRELOC:					/* If relocatable, relocate */
		case BRELOC:
			j += bias;					/* Add in bias          */
		case DABS:						/* If not relocatable, don't */
			break;
		default:						/* Any others illegal       */
			badrel(r, bytes);			/* Print message        */
		}
		if (longf == 0 && j.hiword != 0)	/* 16-bit overflow?     */
		{
			printf("16-bit overflow at %ld\n",	/* Print out message        */
				   bytes);				/*  with offset     */
			exit(-1);
		}
		if (longf != 0)					/* Long?            */
			putw(j.hiword, ofp);		/*  yes, output high    */
#ifndef UNIX							/* A bug in UNIX RTL!!      */
		if ((z = putw(j.loword, ofp)) != j.loword)	/* Check for write error    */
		{
			printf("Write error on %s ", ofname);	/* Had one,             */
			printf("Offset = %lx data= %x err =%x\n",
				   bytes, j.loword, z);	/* Print out message        */
			exit(-1);					/*        & quit    */
		}
#else /* Just output and pray...  */
		putw(j.loword, ofp);			/* Output low word      */
#endif
		bytes += 2 + longf;				/* Increment byte count     */
	}									/* End while loop       */
}										/* End reloc procedure      */


VOID badrel(r, offset)					/* Print bad reloc message  */
WORD r;									/* Relocation bits      */
LONG offset;							/* File offset          */
{
	printf("Illegal reloc = %x at %lx\n",	/* Print error message      */
		   r, offset);
	exit(-1);							/* quit             */
}
