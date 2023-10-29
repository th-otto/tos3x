/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "../common/linux/libcwrap.h"
#include "../include/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "cout.h"

#define BOOLEAN int
#define FALSE 0
#define TRUE 1

#define LIB1MAGIC	0xff6d
#define LIB2MAGIC	0xff65
#define LIB1HDSIZE	16
#define LIB2HDSIZE	28
struct libhdr
{
	char l1fname[8];
	int32_t l1modti;
	char l1userid;
	char l1fimode;
	short l1fsize;
};

struct lib2hdr
{
	char l2fname[14];
	int32_t l2modti;
	char l2userid;
	char l2gid;
	short l2fimode;
	int32_t l2fsize;
	short l2junk;
};

#define	ARMAG	"!<arch>\n"
#define	SARMAG	8

#define	ARFMAG	"`\n"

struct ar_hdr
{
	char ar_name[16];
	char ar_date[12];
	char ar_uid[6];
	char ar_gid[6];
	char ar_mode[8];
	char ar_size[10];
	char ar_fmag[2];
};
/* format of a symbol entry in the main table */

struct symtab {
	char name[SYNAMLEN];	/* symbol name */
	short flags;			/* bit flags */
	int32_t vl1;			/* symbol value */
	struct symtab *next;	/* global table link */
	struct symtab *tlnk;	/* table link */
};

static struct symtab *bmte;				/* beginning of main table */

/* flags for symbols */
#define SYDF	0100000		/* defined */
#define SYEQ	0040000		/* equated */
#define SYGL	0020000		/* global - entry or external */
#define SYER	0010000		/* equated register */
#define SYXR	0004000		/* external reference */
#define SYDA	0002000		/* DATA based relocatable */
#define SYTX	0001000		/* TEXT based relocatable */
#define SYBS	0000400		/* BSS based relocatable */
#define SYFN	0000200		/* ?.o file name */


/* parameters that define the main table*/
#define SZMT 300		/* initial size of the main table */
						/* must be large enough to initialize */
#define ICRSZMT 100		/* add to main table when run out */

struct symhash {
	struct symtab *last;		/* ptr to last entry in chain */
	struct symtab *first;		/* ptr to first entry in chain */
};
/* initial reference table for externals */
#define SZIRT	32
static struct symhash eirt[SZIRT];
static struct symhash saveirt[SZIRT];

/*initial reference table for globals*/
static struct symhash girt[SZIRT];
static struct symhash savgirt[SZIRT];

static struct symhash *pirt;
static struct symtab *lmte;				/* last entry in main table */
static struct symtab *savlmte;

#define AREGLO		8
#define AREGHI		15

/* relocation bit definitions:*/
#define RBMASK		07		/*tells type of relocation*/
#define INSABS		7		/*first word of instr -- absolute*/
#define DABS		0		/*data word absolute*/
#define TRELOC		2		/* TEXT relocatable*/
#define DRELOC		1		/* DATA relocatable*/
#define BRELOC		3		/* BSS relocatable*/
#define EXTVAR		4		/* ref to external variable*/
#define LUPPER		5		/* upper word of long */
#define EXTREL		6		/* relative mode on external variable*/

static FILE *ibuf;
static FILE *tbuf;
static FILE *obuf;
static FILE *rbuf;
static FILE *rtbuf;
static FILE *rdbuf;

static int sflag;		/* remove symbol table */

static int32_t textbase;
static int32_t database;
static int32_t bssbase;
static int32_t textsize;
static int32_t datasize;
static int32_t bsssize;
static int32_t stacksize;

static int32_t textstart;
static int32_t datastart;
static int32_t bssstart;

static char *ifilname;			/*points to name of current input file*/

#define NFILE	256		/* max # files we can process*/
static struct symtab *fsymp[NFILE];		/* points to first symbol for each .o file */
static struct symtab **firstsym;		/* points to entry in fsymp */
static struct symtab *symptr;

static int extmatch;			/*matched an external in a library entry*/
static int noload;				/*dont load this lib file flag*/

#define NLIB	16		/*max # libraries to process*/
static int lbfictr[NLIB];		/*counts files loaded from one library*/
static int *libfctr;			/* points to lbfictr */
static int32_t lbfioff[NFILE];	/*each file offset in library*/
static int32_t *libptr;			/*points to lbfioff*/
static int libhdsize;
union alibhd {
	struct libhdr hdr1;
	struct lib2hdr hdr2;
	struct ar_hdr ar;
	int32_t l[20];					/* length is max lib head size */
};
static union alibhd libhd;
struct hdr couthd;		/* [vlh] 4.1 declared */

static int undflg;
static char dafnc;
static int pass2;
static int32_t stlen;
static int Xflag;
static int Dflag;
static int Bflag;
static int Zflag;

static struct symtab *etextptr;
static struct symtab *edataptr;
static struct symtab *endptr;
static struct symtab *lastdup;

static char rtfnc;
static char rdfnc;
static int saverbits;

#define TWOKSHT		-1		/* 2k shared text */
#define FOURKSHT	1	/* 4k shared text */

#define DEFSHTEXT	FOURKSHT

static struct ar_hdr lib3_hdr;

#define LIB3MAGIC 0x3c21
#define LIB3HDSIZE	(sizeof lib3_hdr)


/* 68000 linking loader*/
/*   Bill Allen*/

/* This loader is basically a two pass operation.  The first pass*/
/*  reads all the relocatable object files including any libraries.*/
/*  It builds the full symbol table, resolves all external symbol*/
/*  references if possible, and calculates the total size of the*/
/*  text, data, and bss segements.  Pass 2 then does the binding,*/
/*  the relocation, and produces the final output file.*/

#ifdef VMS
static char libname[] = "lib:lib6.a";	/*default library name */
#define LIBCHAR libname[7]				/* Character to be modified */
#endif

#ifdef CPM
static char libname[] = "lib6.a";
#define LIBCHAR libname[3]				/* Character to be modified */
#endif

#ifdef GEMDOS
static char libname[] = "libc.a";
#define LIBCHAR libname[3]				/* Character to be modified */
#endif

#ifdef UNIX
static char libname[] = "/lib/lib6.a";			/* Default library name         */
#define LIBCHAR libname[8]				/* Character to be modified     */
#endif

static const char *ofilname = "c.out";	/* output file name */

#ifdef UNIX
#define HAVE_MKTEMP
#endif
#ifdef __unix__
#define HAVE_MKTEMP
#endif
#ifdef __linux__
#define HAVE_MKTEMP
#endif

#ifdef HAVE_MKTEMP
static const char *tfbase = "loXXXXXX";	/* Temp base name */
static const char *tdisk = "/tmp/";		/* Temp disk name */
#else
static const char *tfbase = "loXXXXXA";	/* Temp base name */
static const char *tdisk = "";			/* Temp disk name */
#endif

static char *tfchar;					/* -> changeable character */
static char tfilname[80];				/* first temp file name */

#define TFCHAR *tfchar					/* Name to simplify assignments */
static char const etexstr[] = "_etext\0\0";
static char const edatstr[] = "_edata\0\0";
static char const eendstr[] = "_end\0\0\0\0";

int ignflg;
int debug;
int exstat;
int dmprelocs;

static int32_t lbctr;
static int32_t libfilsize;


static unsigned short get16be(P(FILE *) sp)
PP(register FILE *sp;)					/* the stream to get from */
{
	register unsigned int w1;
	w1 = getc(sp);
	w1 <<= 8;
	return w1 | getc(sp);
}


static int32_t get32be(P(FILE *) sp)
PP(register FILE *sp;)					/* the stream to get from */
{
	register unsigned int w1;
	w1 = get16be(sp);
	return ((int32_t)w1 << 16) | get16be(sp);
}


static VOID put16be(P(unsigned short) w, P(FILE *) sp)
PP(unsigned short w;)
PP(register FILE *sp;)
{
	fputc(w >> 8, sp);
	fputc(w, sp);
}


static VOID put32be(P(int32_t) l, P(FILE *) sp)
PP(int32_t l;)
PP(register FILE *sp;)
{
	put16be((unsigned short)(l >> 16), sp);
	put16be((unsigned short)l, sp);
}


/*
 * read the header of the input file
 */
static int readshort(P(FILE *) fd, P(unsigned short *) s)
PP(FILE *fd;)
PP(unsigned short *s;)
{
	register int w1, w2;

	w1 = getc(fd);
	if (w1 < 0)
		return 1;
	w1 <<= 8;
	w2 = getc(fd);
	if (w2 < 0)
		return 1;
	w1 |= w2;
	*s = w1;
	return 0;
}


static VOID closetemps(NOTHING)
{
	if (rtbuf)
	{
		fclose(rtbuf);
		rtbuf = NULL;
	}
	if (rdbuf)
	{
		fclose(rdbuf);
		rdbuf = NULL;
	}
}


static VOID endit(P(int) stat)
PP(int stat;)
{
	if (obuf)
	{
		fclose(obuf);
		obuf = NULL;
	}
	if (dafnc)
	{
		TFCHAR = dafnc;
		unlink(tfilname);
	}
	closetemps();
	if (saverbits || dmprelocs)
	{
		TFCHAR = rtfnc;
		unlink(tfilname);
		TFCHAR = rdfnc;
		unlink(tfilname);
	}
	exit(stat);
}


static VOID oom(NOTHING)
{
	fprintf(stderr, ": out of memory\n");
	endit(1);
}


/*
 * do a long seek on buffer bp given a long file offset
 * last argument indicates relative or absolute seek
 */
static VOID lbseek(P(int32_t) al, P(FILE *)bp)
PP(int32_t al;)
PP(FILE *bp;)
{
	if (fseek(bp, al, SEEK_SET) < 0)
	{
		fprintf(stderr, ": seek error on file %s\n", ifilname);
		endit(1);
	}
}


static VOID readhdr(NOTHING)
{
	register struct hdr *p = &couthd;
	register FILE *fp = ibuf;
	
	p->ch_magic = get16be(fp);
	p->ch_tsize = get32be(fp);
	p->ch_dsize = get32be(fp);
	p->ch_bsize = get32be(fp);
	p->ch_ssize = get32be(fp);
	p->ch_stksize = get32be(fp);
	p->ch_entry = get32be(fp);
	p->ch_rlbflg = get16be(fp);
	if (p->ch_magic != MAGIC)
	{
		fprintf(stderr, ": file format error: %s\n", ifilname);
		endit(1);
	}
	if (p->ch_rlbflg)
	{
		fprintf(stderr, ": File format error");
		fprintf(stderr, ": no relocation bits in %s\n", ifilname);
		endit(1);
	}
}


/* read a library file header */
static BOOLEAN rdlibhdr(NOTHING)
{
	register union alibhd *p;
	register int i;
	register FILE *fp = ibuf;

	p = &libhd;
	switch (libhdsize)
	{
	case LIB1HDSIZE:
		for (i = 0; i < 8; i++)
			p->hdr1.l1fname[i] = getc(fp);
		p->hdr1.l1modti = get32be(fp);
		p->hdr1.l1userid = getc(fp);
		p->hdr1.l1fimode = getc(fp);
		p->hdr1.l1fsize = get16be(fp);
		libfilsize = p->hdr1.l1fsize;
		break;
	case LIB2HDSIZE:
		for (i = 0; i < 14; i++)
			p->hdr2.l2fname[i] = getc(fp);
		p->hdr2.l2modti = get32be(fp);
		p->hdr2.l2userid = getc(fp);
		p->hdr2.l2gid = getc(fp);
		p->hdr2.l2fimode = get16be(fp);
		p->hdr2.l2fsize = get32be(fp);
		p->hdr2.l2junk = get16be(fp);
		libfilsize = p->hdr2.l2fsize;
		break;
	}
	if (libhd.hdr2.l2fname[0] == '\0')
		return FALSE;
	return TRUE;
}


/*
 * update the relocation counters with the sizes in the header
 */
static VOID addsizes(NOTHING)
{
	textbase += couthd.ch_tsize;
	database += couthd.ch_dsize;
	bssbase += couthd.ch_bsize;
}


/*
 * pack a string into an entry in the main table
 *	call with:
 *		pointer to the string
 *		pointer to desired entry in the main table
 */
static VOID pack(P(const char *) apkstr, P(char *) apkptr)
PP(const char *apkstr;)
PP(char *apkptr;)
{
	register int i;
	register const short *pkstr;
	register short *pkptr;

	pkstr = (const short *)apkstr;
	pkptr = (short *)apkptr;
	for (i = 0; i < SYNAMLEN / (sizeof (*pkstr)); i++)
		*pkptr++ = *pkstr++;
}


/* method for looking up entries in the main table*/
 /**/
/* Note:        The entry to be looked up must be placed at the end*/
/*                      of the main table.  The global cell 'lmte'(last main*/
/*                      entry) points to the next available entry in the main*/
/*                      table.  The address of an initial reference table must*/
/*                      also be provided.*/
/*      1)      Compute the hash code for the symbol and add it to the base address*/
/*              of the initial reference table given as input.  Thus, two words are*/
/*              accessed which define the chain on which the symbol must be if it is*/
/*              in the table at all.*/
/*      2)      Alter the table link of the last symbol in the chain so that it */
/*              points to the symbol being looked up.  Note that the symbol to be */
/*              looked up is always placed at the end of the main table before */
/*              calling the lookup routine.  This essentially adds one more element*/
/*              to the end of the chain, namely the symbol to be looked up.*/
/*      3)      Now start at the first symbol in the chain and follow the chain*/
/*              looking for a symbol equal to the smbol being looked up.  It is*/
/*              quaranteed that such a symbol will be found because it is always */
/*              the last symbol on the chain.*/
/*      4)      When the symbol is found, check to see if it is the last symbol*/
/*              on the chain.  If not, the symbol being looked for is in the table*/
/*              and has been found.  If it is the last symbol, the symbol being */
/*              looked up is not in the table.*/
/*      5)      In the case the looked up symbol is not found, it is usually added*/
/*              to the end of the table.  This is done simply b changing the */
/*              initial reference table entry which points to the previous*/
/*              last symbol on the chain so that is now points to the symbol at the*/
/*              end of the main table.  In case the symbol just looked up is not to*/
/*              be added to the main table then no action is needed .  This means*/
/*              that the table link of the last symbol on a chain may point any-*/
/*              where.*/
/* look up entry in the main table*/
/*              call with:*/
/*                      address of initial reference table*/
/*                      entry to be looked up at the end of the main table*/
/*              returns:*/
/*                      a pointer to the entry.  if this pointer is equal to*/
/*                      lmte then the symbol was not previously in the table.*/


/* compute a hash code for the last entry in the main table*/
/* returns the hash code*/

static int hash(NOTHING)
{
	register int ht1, i;
	register char *p;

	ht1 = 0;
	p = lmte->name;
	for (i = 0; i < SYNAMLEN; i++)
		ht1 += *p++;
	return ht1 & (SZIRT - 1);			/* make hash code even and between 0 and 62 */
}


static struct symtab *nextsy(P(struct symtab *) amtpt)
PP(struct symtab *amtpt;)
{
	register struct symtab *mtpt;
	register short *p1, *p2;
	register int i;

	mtpt = amtpt;

	/* loop to locate entry in main table */
  lemtl:
	p1 = (short *)mtpt->name;
	p2 = (short *)lmte->name;
	for (i = 0; i < SYNAMLEN / (sizeof (*p1)); i++)
	{
		if (*p1++ != *p2++)
		{
			mtpt = mtpt->tlnk;			/* go to next entry in chain */
			goto lemtl;
		}
	}
	return mtpt;
}


static struct symtab *lemt(P(struct symhash *) airt)
PP(struct symhash *airt;)
{
	register struct symtab *mtpt;

	/* pointer to entry in irt */
	pirt = airt + hash();
	/* pointer to first entry in chain */
	mtpt = pirt->first;
	if (mtpt == NULL)
	{
		/* start at end of main table */
		mtpt = lmte;
	} else
	{
		/* last entry in chain is new symbol */
		pirt->last->tlnk = lmte;
	}
	/* return next match on chain */
	return nextsy(mtpt);
}


/* print a symbol name for an error message*/
static VOID prtsym(P(FILE *) fp, P(const struct symtab *) ap)
PP(FILE *fp;)
PP(const struct symtab *ap;)
{
	register int i;
	register const char *p;

	p = ap->name;
	for (i = 0; i < SYNAMLEN && *p != '\0'; i++)
	{
		fputc(*p++, fp);
	}
	fputc('\n', fp);
}


static struct symtab *nthsym(P(unsigned int) extno)
PP(register unsigned int extno;)
{
	register struct symtab *p;
	register unsigned int i;
	
	p = symptr;
	i = extno;
	while (i && p)
	{
		--i;
		p = p->next;
	}
	if (p == NULL)
	{
		fprintf(stderr, "%s: (%u) corrupted\n", ifilname, extno);
		endit(1);
	}
	return p;
}


/************************************************************************
 *
 * extval(snum) -- look up the value of an external symbol given
 *	the external symbol number.
 *
 *	Since externals are duplicated on the hash chains,
 *	all externals don't have values assigned in the symbol table,
 *	so the external name must be looked on the global chains and
 *	the value of the matching symbol used.  symptr points to the
 *	first symbol for the current file's symbol table
 *
 ************************************************************************/
static int32_t extval(P(unsigned int) extno)
PP(unsigned int extno;)
{
	register struct symtab *p;
	register struct symtab *pg;

	p = nthsym(extno);
	if ((p->flags & SYXR) == 0)
	{
		return p->vl1;
	}
	pack(p->name, lmte->name);
	pg = lemt(girt);
	if (pg == lmte)
		pg = lemt(eirt);				/* may be common */
	return pg->vl1;						/* return globals address */
}


/*
 * look up the relocation base for an external symbol.  must use same
 *  method as in extval for the same reasons (see comment above)
 */
static int extbase(P(unsigned int) extno)
PP(unsigned int extno;)
{
	register struct symtab *p;
	register struct symtab *pg;
	register int i;

	p = nthsym(extno);
	if ((p->flags & SYXR) == 0)
	{
		pg = p;
	} else
	{
		pack(p->name, lmte->name);
		pg = lemt(girt);
		if (pg == lmte)
			pg = lemt(eirt);			/* may be common */
	}
	i = pg->flags;
	if (i & SYDA)
		return DRELOC;
	else if (i & SYTX)
		return TRELOC;
	else if (i & SYBS)
		return BRELOC;
	return DABS;
}


/*
 * print an error message giving an external name
 */
static VOID prextname(P(unsigned int) extno)
PP(unsigned int extno;)
{
	fprintf(stderr, ": external name: ");
	prtsym(stderr, nthsym(extno));
}


/*
 * load a file doing relocation and external resolution
 *  lflg is set if we are loading from a library
 */
static VOID do2load(P(int) libflg)
PP(int libflg;)
{
	register unsigned int i, j;
	int longf;
	register FILE *p;
	register FILE *pr;
	int saof;
	register int32_t tpc;
	register int32_t l;
	int32_t l1;
	int wasext;

	tpc = 0;
	p = obuf;
	pr = rtbuf;
	saof = -1;
	/* beginning of this symbol table */
	symptr = *firstsym++;
	l = couthd.ch_tsize + couthd.ch_dsize + couthd.ch_ssize + HDSIZE;
	if (libflg)
		l += lbctr + libhdsize;
	/* long seek */
	lbseek(l, rbuf);
	l = couthd.ch_tsize;
  do2l1:
	while ((l -= 2) >= 0)
	{
		/* relocate the text */
		longf = 0;
		i = get16be(ibuf);
		j = get16be(rbuf);
		/* keep pc in this file */
		tpc += 2;
		wasext = 0;
		switch (j & RBMASK)
		{								/* relocation bits */
		case INSABS:					/* first word of instr */
		case DABS:						/* data absolute */
			put16be(i, p);
			if (saverbits || dmprelocs)
				put16be(j, pr);			/* relocation bits */
			break;

		case LUPPER:					/* high word of long */
			l1 = ((int32_t)i) << 16;
			if (saverbits || dmprelocs)
				put16be(j, pr);			/* upper word relocation bits */
			l1 |= get16be(ibuf);
			j = get16be(rbuf);
			tpc += 2;
			longf++;					/* doing two words */
			l -= 2;						/* count lower half */
		  dorelc:
			switch (j & RBMASK)
			{
			case DABS:
				if (saverbits || dmprelocs)
					put16be(j, pr);
				break;

			case INSABS:
			case LUPPER:
			default:
				goto do2199;

			case TRELOC:
				l1 += textbase;
				if (saverbits || dmprelocs)
					put16be(j, pr);
				break;

			case DRELOC:
				l1 += database;
				if (saverbits || dmprelocs)
					put16be(j, pr);
				break;

			case BRELOC:
				l1 += bssbase;
				if (saverbits || dmprelocs)
					put16be(j, pr);
				break;

			case EXTVAR:
				wasext++;
				l1 += extval(j >> 3);
				if (saverbits || dmprelocs)
					put16be(extbase(j >> 3), pr);
				break;

			case EXTREL:
				l1 = l1 + extval(j >> 3) - textbase - tpc + 2;
				if (l1 < (-32768L) || l1 > 0x7fff)
				{
					fprintf(stderr, ": relative address overflow at %lx in %s\n", (long)tpc - 2, ifilname);
					prextname(j >> 3);	/*give name referenced */
					exstat = 1;
				}
				if (saverbits || dmprelocs)
					put16be(DABS, pr);
				goto outlowd;

			}
			if (ignflg == 0 && longf == 0 && (l1 & 0xffff8000L) && saof)
			{
				fprintf(stderr, ": short address overflow in %s\n", ifilname);
				if (wasext)
					prextname(j >> 3);
				exstat = 1;
				if (libflg)
				{
					l1 = *(libptr - 1);
					printf("library offset = %lx\n", (long) l1);
					l1 = 0;
				}
				saof = 0;
			}
			if (longf)
			{
				put32be(l1, p);
			} else
			{
			outlowd:
				put16be((unsigned short)l1, p);
			}
			break;

		case TRELOC:
		case DRELOC:
		case BRELOC:
		case EXTVAR:
		case EXTREL:
			l1 = (int32_t)(short)i;						/* sign extend to long like 68000 */
			goto dorelc;

		default:
		  do2199:
			fprintf(stderr, ": File format error: invalid relocation flag in %s\n", ifilname);
			endit(1);
		}
	}
	if (p == obuf)
	{
		p = tbuf;						/* place to put data */
		pr = rdbuf;						/* file for data relocation bits */
		l = couthd.ch_dsize;
		goto do2l1;						/* now do the data */
	}
}


/*
 * load files from a library.  the library is open in ibuf, the
 * count of files to load is pointed to by libfctr, and the offset
 * of each file is pointed to by libptr.
 */
static VOID loadlib(NOTHING)
{
	register int i;
	register int32_t l;

	/* # files to load from this library */
	i = *libfctr++;
	if (i == 0)
		return;
	while (i--)
	{
		/* load the files */
		l = *libptr++;					/* library offset for this file */
		lbseek(l, ibuf);				/* seek to beginning of file */
		lbseek(l, rbuf);
		rdlibhdr();						/* read the library header */
		readhdr();						/* read the file header */
		lbctr = l;
		do2load(1);						/* load it */
		addsizes();
	}
}


/************************************************************************
 *
 * getsym()
 *
 * Get one symbol entry from the input file and put it into the symbol
 * table entry pointed to by lmte.
 *
 * If the program is being linked for chaining (CBASIC) and the symbol
 * name is __cbmain, the name is changed to reflect the specific
 * overlay, since each overlay and the root has its own definition for
 * __cbmain.
 *
 ************************************************************************/
static VOID getsym(NOTHING)
{
	register int i;
	register struct symtab *stpt;

	stpt = lmte;
	for (i = 0; i < SYNAMLEN; i++)
		stpt->name[i] = getc(ibuf);
	stpt->flags = get16be(ibuf);				/* flags */
	stpt->vl1 = get32be(ibuf);
}


/*
 * relocate the symbol value pointed to by lmte according to
 * symbol type and corresponding relocation base
 */
static VOID relocsym(NOTHING)
{
	register int32_t l;

	l = 0;
	if (lmte->flags & SYXR)				/* external */
		return;
	if (lmte->flags & SYTX)
		l = textbase;
	else if (lmte->flags & SYDA)
		l = database;
	else if (lmte->flags & SYBS)
		l = bssbase;
	else
	{
		if (lmte->flags & SYEQ)			/* equated */
			return;						/* abs */
		fprintf(stderr, ": File Format error: Invalid symbol flags = %o\n", (int) lmte->flags);
		endit(1);
	}
	lmte->vl1 += l;
}


static VOID prdup(P(const char *) p)
PP(const char *p;)
{
	fprintf(stderr, ": %.*s duplicate definition in %s\n", SYNAMLEN, p, ifilname);
	exstat = 1;
}


/************************************************************************
 *
 * addmte() -- add the symbol pointed to by lmte to symbol table
 *
 ************************************************************************/
static struct symtab *allocsy(NOTHING)
{
	register struct symtab *ptr;
	
	ptr = malloc(sizeof(*ptr));
	if (ptr == NULL)
	{
		oom();
	}
	lmte = ptr;
	ptr->name[0] = '\0';
	ptr->flags = 0;
	ptr->vl1 = 0;
	ptr->tlnk = NULL;
	ptr->next = NULL;
	return ptr;
}


static VOID addmte(NOTHING)
{
	register struct symtab *ptr;
	
	ptr = lmte;
	ptr->next = allocsy();
}


/*
 * make an entry in the main table
 * assumes:
 *	entry to be made is pointed at by lmte
 *	pirt points to the correct initial reference table entry
 */
static VOID mmte(NOTHING)
{
	pirt->last = lmte;					/* pointer to last entry in chain */
	if (pirt->first == NULL)			/* first entry in chain */
		pirt->first = lmte;
	addmte();
}


/*
 * add a symbol to the symbol table
 *  if symbol is an external, put it on eirt even if it does
 *   already exist
 *  if symbol is a global, put it on girt and error if it exists
 *  in any case, add it to the end of the symbol table
 * if libflg is set, we are searching a library.  In this case, when
 *  a global is encountered, search the external symbols and set
 *  extmatch if a match is found.  When loading a library without searching
 *  it, return as if the symbol matches, to make sure the library file loads.
 */
static VOID addsym(P(int) libflg)
PP(int libflg;)
{
	register struct symtab *p;

	if (lmte->flags & SYXR)
	{
		/* external */
		p = lemt(eirt);
		mmte();
	} else if (lmte->flags & SYGL)
	{
		/* global */
		if (libflg)
		{
			/* global in a library */
			p = lemt(eirt);				/* look up in externals */
			if (p != lmte)
			{
				/* found a match */
				extmatch++;
			}
		}
		p = lemt(girt);
		if (p == lmte)
		{
			mmte();
		} else
		{
			if (libflg)
			{
				noload++;
				lastdup = p;
			} else if (p->flags != lmte->flags || p->vl1 != lmte->vl1)
			{
				/* dup defn msg */
				prdup(p->name);
			}
		}
	} else
	{									/* normal symbol */
		if (lmte->name[0] == 'L')		/* compiler label */
			return;
		if (Xflag == 0)					/* dont save local symbols */
			return;
		addmte();
	}
}


/*
 * save the current state of the symbol table -- it may be restored later
 */
static VOID savsymtab(NOTHING)
{
	register struct symhash *p1, *p2;
	register int i;

	savlmte = lmte;
	p2 = eirt;
	p1 = saveirt;
	for (i = 0; i < SZIRT; i++)
		*p1++ = *p2++;
	p2 = girt;
	p1 = savgirt;
	for (i = 0; i < SZIRT; i++)
		*p1++ = *p2++;
}


/*
 * restore the symbol table as it was when we last saved it
 */
static VOID restsymtab(NOTHING)
{
	register struct symhash *p1, *p2;
	register int i;
	register struct symtab *p, *next;
	
	lmte = savlmte;
	p = lmte->next;
	while (p)
	{
		next = p->next;
		free(p);
		p = next;
	}
	lmte->next = NULL;
	lmte->name[0] = '\0';
	lmte->flags = 0;
	lmte->vl1 = 0;
	p1 = eirt;
	p2 = saveirt;
	for (i = 0; i < SZIRT; i++)
		*p1++ = *p2++;
	p1 = girt;
	p2 = savgirt;
	for (i = 0; i < SZIRT; i++)
		*p1++ = *p2++;
}


/*
 * open the file pointed to by ap
 *  check for the -lx type library spec
 */
static VOID openfile(P(char *) ap)
PP(char *ap;)
{
	register char *p;

	p = ap;
	if (*p == '-' && *++p == 'l')
	{
		if (*++p)
			LIBCHAR = *p;
		else
			LIBCHAR = '6';				/* default library name */
		p = libname;
	} else
	{
		p = ap;
	}
	if ((ibuf = fopen(p, "rb")) == NULL)
	{
		fprintf(stderr, ": unable to open %s\n", p);
		endit(1);
	}
	/* point to current file name for error msgs */
	ifilname = p;
	if (readshort(ibuf, &couthd.ch_magic))
	{
		fprintf(stderr, ": read error on file: %s\n", ifilname);
		endit(1);
	}
	if (couthd.ch_magic != LIB2MAGIC)
	{
		fseek(ibuf, 0L, SEEK_SET);
		/* read file header */
		readhdr();
	}
	if (pass2)
	{
		/* need file descriptor for reloc bits */
		rbuf = fopen(p, "rb");
	}
}


/* load one *.o format file using ibuf*/
/* put the symbols into the symbol table relocating each one*/
/* and finally add this files sizes into running totals*/

/* libflg is set if we are in a library*/

static VOID do1load(P(int) libflg)
PP(int libflg;)
{
	register int32_t i;
	register int32_t l;

	*firstsym = lmte;					/*remember where this symbol table starts */
	l = couthd.ch_tsize + couthd.ch_dsize + HDSIZE;
	if (libflg)
		l += lbctr + libhdsize;
	lbseek(l, ibuf);
	/* size of symbol table */
	i = couthd.ch_ssize;
	while (i > 0)
	{
		/* read one symbol entry */
		getsym();
		/* fix its address */
		relocsym();
		/* add to symbol table */
		addsym(libflg);
		i -= OSTSIZE;
	}
}


/*
 * search a library for files to include.  include an entry only 
 *  if it has a global symbol which matches an external.
 */
static VOID searchlib(NOTHING)
{
	if (libfctr >= &lbfictr[NLIB])
	{
		fprintf(stderr, "too many libraries\n");
		endit(1);
	}
	/* no files from this library yet */
	*libfctr = 0;
	/* current library position - skip magic */
	lbctr = 2;
	while (rdlibhdr())					/* read library file header */
	{
		/* save current state of symbol table */
		savsymtab();
		extmatch = 0;
		noload = 0;
		ifilname = libhd.hdr2.l2fname;
		/* read the file header */
		readhdr();
		/* load this files symbol table & try match */
		do1load(1);
		if (extmatch > noload)
		{
			/* found a match */
			if (noload)
				prdup(lastdup->name);
			/* add this file's sizes */
			addsizes();
			firstsym++;
			if (libptr >= &lbfioff[NFILE])
			{
				fprintf(stderr, "%s: too many input files", ifilname);
				endit(1);
			}
			/* count files loaded from this library */
			*libfctr += 1;
			/* remember offset in library */
			*libptr++ = lbctr;
		} else
		{
			/* dont need this file, restore symbol table */
			restsymtab();
		}
		if (libfilsize & 1)
		{
			/* one byte of padding */
			getc(ibuf);
			lbctr++;
		}
		lbctr += libfilsize + libhdsize;
		/* goto begin of next lib entry */
		lbseek(lbctr, ibuf);
	}
	/* point to next library file counter */
	libfctr++;
}


/*
 * pass 1 load routine:
 *  read the file or library and build the symbol table
 *   symbols are organized on the initial reference tables
 *   with externals on eirt and globals on girt
 */
static VOID p1load(P(char *) ap)
PP(char *ap;)
{
	/* get the file opened using ibuf */
	openfile(ap);
	if (couthd.ch_magic == LIB1MAGIC)
	{
		/* library */
		libhdsize = LIB1HDSIZE;			/* old ar header size */
		searchlib();
	} else if (couthd.ch_magic == LIB2MAGIC)
	{
		/* library */
		libhdsize = LIB2HDSIZE;			/* new ar header size */
		searchlib();
	} else if (couthd.ch_magic == LIB3MAGIC)
	{
		get16be(ibuf);
		get16be(ibuf);
		get16be(ibuf);
		libhdsize = LIB3HDSIZE;
		searchlib();
	} else
	{
		/* load a *.o file */
		do1load(0);
		firstsym++;
		addsizes();
	}
	fclose(ibuf);
	ibuf = NULL;
}


/************************************************************************
 *
 * intsytab() -- initialize the symbol table and the heads of the
 *	hash lists
 *
 ************************************************************************/
static VOID intsytab(NOTHING)
{
	register struct symhash *p1;
	register struct symhash *p2;
	register int i;

	allocsy();
	bmte = lmte;						/* beginning main table */
	p1 = eirt;
	p2 = girt;
	for (i = 0; i < SZIRT; i++, p1++, p2++)
	{
		p1->last = (struct symtab *)p1;
		p1->first = NULL;
		p2->last = (struct symtab *)p2;
		p2->first = NULL;
	}
}


/*
 * test two symbol names for equality
 */
static int eqstr(P(const char *) ap1, P(const char *) ap2)
PP(const char *ap1;)
PP(const char *ap2;)
{
	register const char *p1;
	register const char *p2;
	register int i;

	p1 = ap1;
	p2 = ap2;
	for (i = 0; i < SYNAMLEN; i++)
	{
		if (*p1++ != *p2++)
		{
			return 0;
		}
	}
	return 1;
}


/*
 * look for and define if found etext, edata, eroot, and end
 */
static int spendsym(P(struct symtab *) ap)
PP(struct symtab *ap;)
{
	register struct symtab *p;

	p = ap;
	if (eqstr(etexstr, p->name))
	{
		etextptr = p;
		return 1;
	}
	if (eqstr(edatstr, p->name))
	{
		edataptr = p;
		return 1;
	}
	if (eqstr(eendstr, p->name))
	{
		endptr = p;
		return 1;
	}
	return 0;
}


/*
 * get a temp file for the intermediate text
 */
static VOID gettempf(P(FILE **) fp)
PP(FILE **fp;)
{
	static int tfcindex = 0;
	static char const validchars[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	tfcindex++;
	if (tfcindex >= ((int)sizeof(validchars) - 1))
		tfcindex = 0;
	TFCHAR = validchars[tfcindex];
	if ((*fp = fopen(tfilname, "wb")) == NULL)
	{
		fprintf(stderr, ": Unable to open temporary file: %s\n", tfilname);
		endit(1);
	}
}


/************************************************************************
 *
 * asgncomn(*symbol) -- assign an address for a block of bss common
 *
 *	The address of the block is zero-based in the root's bss.  It
 *	will be relocated later when all the globals are adjusted.
 *
 ************************************************************************/

static VOID asgncomn(P(struct symtab *) ap)
PP(struct symtab *ap;)
{
	register struct symtab *p, *p1;
	register int32_t l;

	p = ap;
	pack(p->name, lmte->name);
	p1 = lemt(girt);
	if (p1 != lmte)
	{									/* matches a global entry */
		ap->vl1 = p1->vl1;
		return;
	}
	l = 0;
	lemt(eirt);							/* set up pointer to proper chains  */
	do
	{
		if (p->vl1 > l)
			l = p->vl1;
		p->vl1 = 0;
	} while ((p = nextsy(p->tlnk)) != lmte);

	/* now make a global entry for this common block */

	p->flags = SYDF | SYGL | SYBS;
	p->vl1 = bssbase + bsssize;
	ap->vl1 = p->vl1;					/* copy address to first external */
	bsssize += (l + 1) & ~1;			/* always start at even address */
	lemt(girt);							/* set ptrs for global chain */
	mmte();								/* add to global chain */
}


/*
 * assign an address to an external by matching it with a global
 *  print an error message if no match
 */
static VOID asgnext(P(struct symtab *) ap)
PP(struct symtab *ap;)
{
	register struct symtab *p, *pg;

	p = ap;
	pack(p->name, lmte->name);			/* copy current symbol name */
	pg = lemt(girt);
	if (pg == lmte)
	{									/* no match in global symbols */
		pg = lemt(eirt);				/* set ptrs for external chains */
		if (pg == lmte)
		{
			fprintf(stderr, ": asgnext botch\n");
			endit(1);
		}
		while ((p = nextsy(p->tlnk)) != lmte)
		{
			if (p->vl1)
			{							/* common */
				asgncomn(ap);
				return;
			}
		}
		if (spendsym(ap))				/* end, etext or edata */
			return;
		if (undflg == 0)
		{
			fprintf(stderr, ": Undefined symbol(s)\n");
			undflg++;
			exstat = 1;
		}
		prtsym(stderr, p);
	} else
	{
		p->vl1 = pg->vl1;				/* assign symbol value */
	}
}


/************************************************************************
 *
 * fixexts() -- get addresses for all external symbols
 *
 ************************************************************************/
static VOID fixexts(NOTHING)
{
	register struct symtab *p;
	register struct symhash *sx1;
	register struct symtab *sx2;

	for (sx1 = eirt; sx1 < &eirt[SZIRT]; sx1++)
	{									/* go thru externals */
		if ((sx2 = sx1->first) == NULL)	/* this chain empty */
			continue;

		/* go thru symbols on chain */
		for (;;)
		{
			if (sx2->vl1 == 0)
				asgnext(sx2);			/* match to a global */
			else
				asgncomn(sx2);			/* assign a common address */
			p = sx2;
			if (p == sx1->last)			/* end of chain */
				break;
			sx2 = sx2->tlnk;			/* next entry in chain */
		}
	}
}


/************************************************************************
 *
 * fixsyms() --fix symbol addresses that have been assigned by adding
 *  	in textbase, database, or bssbase for symbol's overlay.
 *
 *	Text-based in root are already correct from pass 1.
 *
 ************************************************************************/
static VOID fixsyms(NOTHING)
{
	register struct symtab *p;

	for (p = bmte; p != NULL && p != lmte; p = p->next)
	{									/* look at each symbol */
		if (p->flags & SYXR)
			continue;
		if (p->name[0] == '\0')
			continue;
		if (p->flags & SYDA)			/* data symbol */
			p->vl1 += database;
		else if (p->flags & SYBS)		/* bss symbol */
			p->vl1 += bssbase;
	}
}


/************************************************************************
 *
 * resolve() -- resolve the external variable addresses and set the
 *	base address of the data and bss segments.
 *
 *	On entry, all the final base addresses are in the overlay tree.
 *	If an external symbol has a value, it points to a jump block.
 *	External references to common areas have been set to zero
 *	when this routine is called.
 *
 ************************************************************************/
static VOID resolve(NOTHING)
{
	register struct symtab *p;

	textsize = textbase - textstart;
	datasize = database;
	bsssize = bssbase;
	if (Dflag)
	{
		database = datastart;
	} else
	{
		database = (textbase + 1) & ~1;
		datastart = database;
	}
	if (Bflag)
	{
		bssbase = bssstart;
	} else
	{
		bssbase = (database + datasize + 1) & ~1;
		bssstart = bssbase;
	}
	textbase = textstart;
	fixsyms();							/* relocate symbols with addresses */
	fixexts();							/* fix external addresses & commons */
	if (etextptr)
	{
		pack(etexstr, lmte->name);
		p = lemt(eirt);
		do
		{
			p->vl1 = textsize;
			p->flags &= ~SYXR;			/* no longer external */
			p->flags |= SYDF | SYGL | SYTX;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
	if (edataptr)
	{
		pack(edatstr, lmte->name);
		p = lemt(eirt);
		do
		{
			p->vl1 = textsize + datasize;
			p->flags &= ~SYXR;			/* no longer external */
			p->flags |= SYDF | SYGL | SYDA;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
	if (endptr)
	{
		pack(eendstr, lmte->name);
		p = lemt(eirt);
		do
		{
			p->vl1 = textsize + datasize + bsssize;
			p->flags &= ~SYXR;			/* no longer external */
			p->flags |= SYDF | SYGL | SYBS;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
}


/************************************************************************
 *
 * p2load(fname) -- pass 2 load routine
 *
 *	Read the file or library and do relocation
 *
 ************************************************************************/
static VOID p2load(P(char *) ap)
PP(char *ap;)
{
	openfile(ap);						/* get the file opened using ibuf */
	if (couthd.ch_magic == LIB1MAGIC)
	{
		libhdsize = LIB1HDSIZE;
		loadlib();
	} else if (couthd.ch_magic == LIB2MAGIC)
	{
		libhdsize = LIB2HDSIZE;			/* new ar header size */
		loadlib();
	} else if (couthd.ch_magic == LIB3MAGIC)
	{
		libhdsize = LIB3HDSIZE;
		loadlib();
	} else
	{
		do2load(0);						/* load a *.o file */
		addsizes();
	}
	fclose(ibuf);
	ibuf = NULL;
	fclose(rbuf);
	rbuf = NULL;
}


/*
 * make the output file and write the header
 */
static VOID makeofile(NOTHING)
{
	if ((obuf = fopen(ofilname, "wb")) == NULL)
	{
		fprintf(stderr, ": Unable to create %s\n", ofilname);
		endit(1);
	}
	if (Dflag | Bflag)
		put16be(MAGIC1, obuf);			/* data & bss bases in header */
	else
		put16be(MAGIC, obuf);			/* normal header */

	put32be(textsize, obuf);
	put32be(datasize, obuf);
	put32be(bsssize, obuf);
	put32be(0L, obuf);	/* symbol table size; will be patched later in finalwr() */
	put32be(stacksize, obuf);
	put32be(textstart, obuf);
	if (saverbits)
	{
		put16be(0, obuf);				/* relocation bits present */
	} else
	{
		put16be(-1, obuf);				/* relocation bits removed */
	}

	if (Dflag | Bflag)
	{									/* output expanded header */
		put32be(database, obuf);
		put32be(bssbase, obuf);
	}

	gettempf(&tbuf);					/*temp for data words */
	dafnc = TFCHAR;
	if (saverbits)
	{
		gettempf(&rtbuf);				/*temp for text relocation bits */
		rtfnc = TFCHAR;
		gettempf(&rdbuf);				/*temp for data relocation bits */
		rdfnc = TFCHAR;
	}
}


/*
 * output symbols in a form to be read by a debugger
 * call with pointer to symbol table entry
 */
static VOID osyme(P(struct symtab *) aosypt)
PP(struct symtab *aosypt;)
{
	register struct symtab *osypt;
	register int i;

	osypt = aosypt;						/* pointer to symbol table entry */

	stlen += OSTSIZE;					/* one more symbol out */

	/* output symbol to loader file */
	for (i = 0; i < SYNAMLEN; i++)
	{									/* output symbol name */
		putc(osypt->name[i], obuf);
	}

	put16be(osypt->flags, obuf);		/* output symbol flags */
	put32be(osypt->vl1, obuf);			/* output symbol value */
}


/*
 * output symbol table to file
 */
static VOID osymt(NOTHING)
{
	register struct symtab *p;

	stlen = 0;
	if (sflag)							/* no symbol table desired */
		return;

	/* now output the symbols deleting externals */

	for (p = bmte; p != NULL && p != lmte; p = p->next)
	{
		if (p->flags & SYXR)			/* external symbol */
			continue;
		if (p->name[0] == '\0')
			continue;
		if ((p->flags & SYGL) == 0 && (p->name[0] == 'L' || Xflag == 0))
			continue;
		osyme(p);
	}
}


/*
 * copy the initialized data words from temp file to output file
 */
static VOID cpdata(P(FILE **) ppb, P(int) fnc, P(int32_t) size)
PP(register FILE **ppb;)
PP(int fnc;)
PP(int32_t size;)
{
	register int j;
	register FILE *pb;

	pb = *ppb;
	fflush(pb);
	fclose(pb);
	*ppb = NULL;
	TFCHAR = fnc;
	if ((pb = fopen(tfilname, "rb")) == NULL)
	{
		fprintf(stderr, ": unable to reopen %s\n", tfilname);
		endit(1);
	}
	while (size > 0)
	{
		j = get16be(pb);				/* Fetch word from source buffer */
		put16be(j, obuf);
		size -= 2;						/* Down by 2 bytes      */
	}
	fclose(pb);
}


/*
 * do the final writting to the output file
 * copy the initialized data from the temp file to the output file
 * write the symbol table to the output file
 */
static VOID finalwr(NOTHING)
{
	if ((textsize + textstart) != textbase)
	{
		fprintf(stderr, ": finalwr: text size error\n");
		exstat = 1;
	}
	cpdata(&tbuf, dafnc, datasize);
	osymt();							/* write the symbol table */
	if (saverbits)
	{
		cpdata(&rtbuf, rtfnc, textsize);
		cpdata(&rdbuf, rdfnc, datasize);
	} else
	{
		closetemps();
	}
	fflush(obuf);
	/* update size of symbols table in header */
	fseek(obuf, 14L, SEEK_SET);
	put32be(stlen, obuf);
	if (ferror(obuf))
	{
		fprintf(stderr, ": output file write error\n");
		exstat = 1;
	}
	endit(exstat);
}


static VOID buildf(NOTHING)
{
	tfilname[0] = 0;					/* Null out present filename    */
	strcat(tfilname, tdisk);			/* Put disk name in first  */
	strcat(tfilname, tfbase);			/* Put in filename now    */

#ifdef HAVE_MKTEMP
	mktemp(tfilname);					/* Make a temp filename   */
#endif

	tfchar = &tfilname[(int)strlen(tfilname) - 1];	/* Set pointer  */
}


/*get constant given radix*/
/* this routine only works for radixes of 2, 8, 16*/
/* so as to avoid lmul because of floating point*/
static int32_t lgetnum(P(char *) apc, P(int) ardx)
PP(char *apc;)
PP(int ardx;)
{
	register int trdx, j;
	register char *pc;
	register int32_t i;

	pc = apc;
	if (ardx == 16)
		trdx = 4;						/*radix as power of 2 */
	else if (ardx == 8)
		trdx = 3;
	else if (ardx == 2)
		trdx = 1;
	else
		return (0);
	i = 0;
	while (1)
	{
		j = *pc++;
		if (j >= '0' && j <= '9')
			j -= '0';
		else if (j >= 'a' && j <= 'f')
			j = j - 'a' + 10;
		else if (j >= 'A' && j <= 'F')
			j = j - 'A' + 10;
		else
			break;						/*not valid numeric char */
		if (j >= 0 && j < ardx)
			i = (i << trdx) + j;
		else
			break;
	}
	return (i);
}


/************************************************************************
 *
 * usage() -- prints a simple help message
 *
 *	This routine is called if only the program name is typed
 *
 ************************************************************************/

static VOID usage(NOTHING)
{
	printf("USAGE: %s [options] [file...]\n\n", "lo68");
	printf("options:\n");
	printf("  -l[c]      add library\n");
	printf("  -o <file>  set output filename\n");
	printf("  -s         don't output symbol table\n");
	printf("  -X         keep local symbols except compiler locals\n");
	printf("  -tnnnnnnn  offset the text base for standalone pgm\n");
	printf("  -dnnnnnnn  data segment base\n");
	printf("  -bnnnnnnn  bss segment base\n");
	printf("  -i         ignore 16-bit addr overflow\n");
	printf("  -r         put relocation bits on output\n");
	printf("  -u<sym>    initial undefined symbol\n");
	printf("  -f <dir>   Temp file prefix\n");
}


#include "../common/linux/libcmain.h"

int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char **p;
	register char *pc;
	register int i;
	register char *p1;
	register int j;

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	buildf();							/* Construct temp file names */
	intsytab();							/*initialize the symbol table */
	firstsym = fsymp;
	libfctr = lbfictr;
	libptr = lbfioff;
	if (argc <= 1)
	{
		fprintf(stderr, ": Invalid lo68 argument list\n");
		usage();
		endit(1);
	}
	p = argv + 1;
	for (i = 1; i < argc; i++)
	{
		/* process each argument */
		pc = *p++;
		/*
		 * Compare for a file named ".o" and skip it for CP/M and VAX
		 * command file operation.
		 */
		if (strcasecmp(pc, ".o") == 0)
			continue;					/* Just skip this one */

		if (*pc == '-')
		{
			switch (*++pc)
			{
			case 'l':					/* library */
				pc--;
				break;

			case 'o':					/* output file name */
				if (*(pc + 1) != '\0')	/* No space?    */
				{
					fprintf(stderr, ": Illegal option %s\n", *(--p));
					return 1;
				}
				ofilname = *p++;		/* next arg is file name */
				i++;
				continue;

			case 's':					/* don't output symbol table */
				sflag++;
				continue;

			case 'X':					/* keep local symbols except L* */
			case 'x':
				Xflag++;
				continue;

			case 'Z':					/* offset the text base for standalone pgm */
			case 'z':

			case 'T':					/* another name for same flag */
			case 't':
				Zflag++;
				textstart = lgetnum(++pc, 16);	/* get a long hex number */
				textbase = textstart;
				continue;

			case 'D':					/* data segment base */
			case 'd':
				Dflag++;
				datastart = lgetnum(++pc, 16);
				continue;

			case 'B':					/* bss segment base */
			case 'b':
				Bflag++;
				bssstart = lgetnum(++pc, 16);
				continue;

			case 'I':					/* ignore 16-bit addr overflow */
			case 'i':
				ignflg++;
				continue;

			case 'r':
				saverbits++;			/* put relocation bits on c.out */
				continue;

			case 'U':					/* initial undefined symbol */
			case 'u':
				p1 = lmte->name;
				pc++;
				for (j = 0; j < SYNAMLEN; j++)
				{
					*p1++ = *pc;
					if (*pc)
						pc++;
				}

				lmte->flags = SYXR;		/*external reference */
				addsym(0);				/*put into symbol table */
				continue;

			case 'f':					/* Temp file prefix */
				tdisk = *p++;			/* Prefix           */
				i++;
				buildf();				/* build file names */
				continue;
			}
		}
		p1load(pc);						/* read file or library */
	}
	resolve();							/*resolve externals and assign addresses */
	pass2++;							/*now in pass 2 */
	p = argv + 1;
	firstsym = fsymp;
	libfctr = lbfictr;
	libptr = lbfioff;
	makeofile();						/*make the output file */
	for (i = 1; i < argc; i++)
	{									/*process each argument */
		pc = *p++;

		/*
		 * Skip files named ".o" again ...
		 */
		if (strcasecmp(pc, ".o") == 0)
			continue;

		if (*pc == '-')
		{
			switch (*++pc)
			{
			case 'l':					/*library */
				pc--;
				break;

			case 'f':					/* Temp file name */
			case 'o':					/*output file name */
				p++;					/*skip file name */
				i++;
				continue;


			case 's':					/*don't output symbol table */
			case 'X':
			case 'x':
			case 'Z':
			case 'z':
			case 'I':
			case 'i':
			case 'U':
			case 'u':
			case 'T':
			case 't':
			case 'D':
			case 'd':
			case 'B':
			case 'b':
			case 'r':
				continue;
			}
		}
		p2load(pc);						/*read file or library */
	}
	finalwr();							/*finish output file */
	
	return 0;
}
