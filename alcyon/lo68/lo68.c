/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "../include/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lo68.h"

#define LIB1MAGIC	0xff6d
#define LIB2MAGIC	0xff65
#define LIB1HDSIZE	16
#define LIB2HDSIZE	28

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

struct lib2hdr *lib2hd = 0;

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
static struct ar_hdr lib3_hdr;

#define LIB3MAGIC 0x3c21
#define LIB3HDSIZE	(sizeof lib3_hdr)


#define BLKSIZE 512

/* 68000 linking loader*/
/*   Bill Allen*/

/* This loader is basically a two pass operation.  The first pass*/
/*  reads all the relocatable object files including any libraries.*/
/*  It builds the full symbol table, resolves all external symbol*/
/*  references if possible, and calculates the total size of the*/
/*  text, data, and bss segements.  Pass 2 then does the binding,*/
/*  the relocation, and produces the final output file.*/

#define MC68000

#ifdef VMS
char libname[] = "lib:lib6.a";			/*default library name */
#define LIBCHAR libname[7]				/* Character to be modified */
#endif

#ifdef CPM
char libname[] = "lib6.a";
#define LIBCHAR libname[3]				/* Character to be modified */
#endif

#ifdef GEMDOS
char libname[] = "libc.a";
#define LIBCHAR libname[3]				/* Character to be modified */
#endif

#ifdef UNIX
char libname[] = "/lib/lib6.a";			/* Default library name         */
#define LIBCHAR libname[8]				/* Character to be modified     */
#endif

char *ofilname = "c.out";				/*output file name */

#ifdef  UNIX					/************************/
char *tfbase = "loXXXXXX";				/* Temp base name       */
char *tdisk = "/tmp/";					/* Temp disk name       */
#else /* CP/M and VMS         */
char *tfbase = "loXXXXXA";				/* Temp base name       */
char *tdisk = "";						/* Temp disk name       */
#endif							/************************/

char *tfchar = 0;						/* -> changeable character */
char tfilname[80] = { 0 };				/*first temp file name */

#define TFCHAR *tfchar					/* Name to simplify assignments */
char etexstr[] = "_etext\0\0";
char edatstr[] = "_edata\0\0";
char eendstr[] = "_end\0\0\0\0";

int ignflg = 0;
int debug = 0;
int exstat = 0;

#ifdef VAX
struct
{
	char lobyte;
	char hibyte;
};
struct {
	short loword;
	short hiword;
};
#endif

#ifdef PDP11
struct
{
	char lobyte;
	char hibyte;
};
struct {
	short loword;
	short hiword;
};
#endif

#ifdef MC68000
struct
{
	char hibyte;
	char lobyte;
};
struct	{
	short	hiword;
	short	loword;
};
#endif

/*** Predeclared Functions ***/
VOID buildf PROTO((NOTHING));
VOID p1load PROTO((char *));
VOID searchlib PROTO((NOTHING));
VOID rdlibhdr PROTO((NOTHING));
VOID openfile PROTO((char *ap));
int readshort PROTO((int fd, short *s));
VOID readhdr PROTO((NOTHING));
VOID do1load PROTO((int libflg));
VOID getsym PROTO((NOTHING));
VOID relocsym PROTO((NOTHING));
VOID addsym PROTO((int libflg));
VOID prdup PROTO((char *p));
VOID intsytab PROTO((NOTHING));

char *lemt PROTO((char **airt));
char *nextsy PROTO((char *amtpt));
VOID mmte PROTO((NOTHING));
VOID addmte PROTO((NOTHING));
int hash PROTO((NOTHING));
VOID pack PROTO((int *apkstr, int *apkptr));
VOID gettempf PROTO((NOTHING));
VOID addsizes PROTO((NOTHING));
VOID savsymtab PROTO((NOTHING));
VOID restsymtab PROTO((NOTHING));
VOID resolve PROTO((NOTHING));
VOID fixsyms PROTO((NOTHING));
VOID fixexts PROTO((NOTHING));
VOID asgncomn PROTO((char *ap));
VOID asgnext PROTO((char *ap));
VOID prtsym PROTO((char *ap));
VOID p2load PROTO((char *ap));
VOID makeofile PROTO((NOTHING));

VOID tmpbinit PROTO((struct buf *abufp));
VOID endit PROTO((int stat));
VOID loadlib(P(NOTHING));
VOID lbseek PROTO((long al, struct buf *bp));
int32_t extval PROTO((int extno));
VOID extbase PROTO((int extno));
VOID do2load PROTO((int libflg));
VOID finalwr PROTO((NOTHING));
VOID osymt PROTO((NOTHING));
VOID osyme PROTO((struct symtab *aosypt));
int32_t lgetnum PROTO((char *apc, int ardx));
VOID spendsym PROTO((struct symtab *ap));
int eqstr PROTO((char *ap1, char *ap2));
VOID prextname PROTO((int extno));


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char **p;
	register char *pc;
	register i;
	register char *p1;
	register int j;

	buildf();							/* Construct temp file names */
	intsytab();							/*initialize the symbol table */
	firstsym = fsymp;
	libfctr = lbfictr;
	libptr = lbfioff;
	if (argc <= 1)
	{
	  usage:
		printf(": Invalid lo68 argument list\n");
		endit(-1);
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
		if (_strcmp(pc, ".o") == 0)
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
					printf(": Illegal option %s\n", *(--p));
					exit(-1);
				}
				ofilname = *p++;		/*next arg is file name */
				i++;
				continue;

			case 's':					/*don't output symbol table */
				sflag++;
				continue;

			case 'X':					/*keep local symbols except L* */
			case 'x':
				Xflag++;
				continue;

			case 'Z':					/*offset the text base for standaone pgm */
			case 'z':

			case 'T':					/*another name for same flag */
			case 't':
				Zflag++;
				textstart = lgetnum(++pc, 16);	/*get a long hex number */
				textbase = textstart;
				continue;

			case 'D':					/*data segment base */
			case 'd':
				Dflag++;
				datastart = lgetnum(++pc, 16);
				continue;

			case 'B':					/*bss segment base */
			case 'b':
				Bflag++;
				bssstart = lgetnum(++pc, 16);
				continue;

			case 'I':					/*ignore 16-bit addr overflow */
			case 'i':
				ignflg++;
				continue;

			case 'r':
				saverbits++;			/*put relocation bits on c.out */
				continue;

			case 'U':					/*initial undefined symbol */
			case 'u':
				p1 = lmte;
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
		p1load(pc);						/*read file or library */
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
		if (_strcmp(pc, ".o") == 0)
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
}


VOID buildf(P(NOTHING))
{
	tfilname[0] = 0;					/* Null out present filename    */
	strcat(tfilname, tdisk);			/* Put disk name in first      */
	strcat(tfilname, tfbase);			/* Put in filename now        */

								  /******************************/
#ifdef UNIX							/* On UNIX,                   */
	mktemp(tfilname);					/* Make a temp filename       */
#endif							  /******************************/

	tfchar = &tfilname[strlen(tfilname) - 1];	/* Set pointer  */
}


/* pass 1 load routine:*/
/* read the file or library and build the symbol table*/
/* symbols are organized on the initial reference tables*/
/* with externals on eirt and globals on girt*/
int ii = 0;

VOID p1load(P(char *) ap)
PP(char *ap;)
{
	ii = 0;
	openfile(ap);						/*get the file opened using ibuf */
	if (couthd.ch_magic == LIB1MAGIC)
	{									/*library */
		libhdsize = LIB1HDSIZE;			/*old ar header size */
		searchlib();
	} else if (couthd.ch_magic == LIB2MAGIC)
	{									/*library */
		libhdsize = sizeof *lib2hd;		/*new ar header size */
		searchlib();
	} else if (couthd.ch_magic == LIB3MAGIC)
	{
		getw(&ibuf);
		getw(&ibuf);
		getw(&ibuf);
		libhdsize = LIB3HDSIZE;
		searchlib();
	} else
	{
		do1load(0);						/*load a *.o file */
		firstsym++;
		addsizes();
	}
	close(ibuf.fildes);
}

/* search a library for files to include.  include an entry only*/
/* if it has a global symbol which matches an external.*/

int32_t lbctr = 0;
int32_t libfilsize = 0;

VOID searchlib(P(NOTHING))
{
	*libfctr = 0;						/*no files from this library yet */
	lbctr = sizeof(short);				/*current library position - skip magic */
	while (rdlibhdr())
	{									/*read library file header */
		savsymtab();					/*save current state of symbol table */
		extmatch = 0;
		noload = 0;
		ifilname = libhd;
		readhdr();						/*read the file header */
		do1load(1);						/*load this files symbol table & try match */
		if (extmatch > noload)
		{								/*found a match */
			if (noload)
				prdup(lastdup);			/*print dup defn */
			addsizes();					/*add this files sizes */
			firstsym++;
			*libfctr += 1;				/*count files loaded from this library */
			*libptr++ = lbctr;			/*remember offset in library */
		} else
		{								/*dont need this file */
			restsymtab();				/*restore symbol table */
		}
		if (libfilsize & 1)
		{								/*one byte of padding */
			getc(&ibuf);
			lbctr++;
		}
		lbctr += libfilsize + libhdsize;
		lbseek(lbctr, &ibuf);			/*goto begin of next lib entry */
	}
	libfctr++;							/*point to next library file counter */
}


/*read a library file header*/
VOID rdlibhdr(P(NOTHING))
{
	register char *p;
	register int i;
	register char *pc;

	p = libhd;
	for (i = 0; i < sizeof *lib2hd; i++)
		*p++ = getc(&ibuf);
	libhd->l2modti = 0;
	libfilsize = libhd->l2fsize;
	if (libhd->l2fname[0] == '\0')
		return (0);
	return (1);
}


/* open the file pointed to by ap*/
/* check for the -lx type library spec*/
VOID openfile(P(char *) ap)
PP(char *ap;)
{
	register char *p;
	register short i;

	p = ap;
	if (*p == '-' && *++p == 'l')
	{
		if (*++p)
			LIBCHAR = *p;
		else
			LIBCHAR = '6';				/*default library name */
		p = libname;
	} else
		p = ap;
	if ((ibuf.fildes = fopen(p, &ibuf, 1)) < 0)
	{
		printf(": unable to open %s\n", p);
		endit(-1);
	}
	ifilname = p;						/*point to current file name for error msgs */
	if (readshort(ibuf.fildes, &couthd.ch_magic))
	{
		printf(": read error on file: %s\n", ifilname);
		endit(-1);
	}
	ibuf.nunused = 0;
	if (couthd.ch_magic != LIB2MAGIC)
	{
		lseek(ibuf.fildes, 0L, 0);
		readhdr();						/*read file header */
	}
	if (pass2)
	{									/*need file descrptr for reloc bits */
		fopen(p, &rbuf, 1);
	}
}


/* read the header of the input file*/

int readshort(P(int) fd, P(short *) s)
PP(int fd;)
PP(short *s;)
{
	short ts;

	if (read(fd, &ts, sizeof(short)) != sizeof(short))
		return (1);
	*s = ts;
	return (0);
}


VOID readhdr(P(NOTHING))
{
	register int i;
	register short *p;

	p = &couthd;
	for (i = 0; i < (sizeof couthd) / 2; i++)
		*p++ = getw(&ibuf);
	if (couthd.ch_magic != MAGIC)
	{
		printf(": file format error: %s\n", ifilname);
		endit(-1);
	}
	if (couthd.ch_rlbflg)
	{
		printf(": File format error");
		printf(": no relocation bits in %s\n", ifilname);
		endit(-1);
	}
}


/* load one *.o format file using ibuf*/
/* put the symbols into the symbol table relocating each one*/
/* and finally add this files sizes into running totals*/

/* libflg is set if we are in a library*/

VOID do1load(P(int) libflg)
PP(int libflg;)
{
	register int32_t i;
	register int32_t l;

	*firstsym = lmte;					/*remember where this symbol table starts */
	l = couthd.ch_tsize + couthd.ch_dsize + sizeof couthd;
	if (libflg)
		l += lbctr + sizeof *lib2hd;
	lbseek(l, &ibuf);
	i = couthd.ch_ssize;				/*size of symbol table */
	while (i > 0)
	{
		getsym();						/*read one symbol entry */
		relocsym();						/*fix its address */
		addsym(libflg);					/*add to symbol table */
		i -= OSTSIZE;
	}
}


/* get one symbol entry from the input file and put it into*/
/* the symbol table entry pointed to by lmte*/
VOID getsym(P(NOTHING))
{
	register int i;
	register short *p;

	p = lmte;
	for (i = 0; i < SYNAMLEN / (sizeof *p); i++)
		*p++ = getw(&ibuf);
	*p++ = getw(&ibuf);					/* flags */
	/*      *p++=0; *//* skip upp half of internal flags on VAX */
	p->hiword = getw(&ibuf);			/* sym value high word */
	p->loword = getw(&ibuf);			/* sym vaue low word */
}


/* relocate the symbol value pointed to by lmte according to*/
/* symbol type and corresponding relocation base*/

VOID relocsym(P(NOTHING))
{
	register int32_t l;

	l = 0;
	if (lmte->flags & SYXR)				/*external */
		return;
	if (lmte->flags & SYTX)
		l = textbase;
	else if (lmte->flags & SYDA)
		l = database;
	else if (lmte->flags & SYBS)
		l = bssbase;
	else
	{
		if (lmte->flags & SYEQ)			/*equated */
			return;						/* abs */
		printf(": File Format error: Invalid symbol flags = %o\n", (int) lmte->flags);
		endit(-1);
	}
	lmte->vl1 += l;
}


/* add a symbol to the symbol table*/
/* if symbol is an external, put it on eirt even if it does*/
/* already exist*/
/* if symbol is a global, put it on girt and error if it exists*/
/* in any case, add it to the end of the symbol table*/
/* if libflg is set, we are searching a library.  In this case, when*/
/* a global is encountered, search the external symbols and set*/
/* extmatch if a match is found.*/

VOID addsym(P(int) libflg)
PP(int libflg;)
{
	register char *p;

	if (lmte->flags & SYXR)
	{									/*external */
		p = lemt(eirt);
		mmte();
	} else if (lmte->flags & SYGL)
	{									/*global */
		if (libflg)
		{								/*global in a library */
			p = lemt(eirt);				/*look up in externals */
			if (p != lmte)
			{							/*found a match */
				extmatch++;
			}
		}
		p = lemt(girt);
		if (p == lmte)
			mmte();
		else
		{
		  dupdef:
			if (libflg)
			{
				noload++;
				lastdup = p;
			} else if (p->flags != lmte->flags || p->vl1 != lmte->vl1)
				prdup(p);				/*dup defn msg */
		}
	} else
	{									/*normal symbol */
		if (lmte->name[0] == 'L')		/*compiler label */
			return;
		if (Xflag == 0)					/*dont save local symbols */
			return;
		addmte();
	}
}


VOID prdup(P(char *) p)
PP(char *p;)
{
	printf(": %s duplicate definition in %s\n", p, ifilname);
	exstat++;
}


/* initialize the symbol table and the heads of the hash lists*/

VOID intsytab(P(NOTHING))
{
	register char **p1,	**p2;
	register i;

	bmte = (sbrk((sizeof *symptr) * SZMT + 2));
	if ((int32_t) bmte & 1)
		bmte++;
	emte = bmte + (sizeof *symptr) * SZMT;	/*end of main table */
	lmte = bmte;						/*beginning main table */
	cszmt = SZMT;						/*current size of main table */
	p1 = eirt;
	p2 = girt;
	for (i = 0; i < 32; i++)
	{
		*p1++ = p1;
		*p1++ = 0;
		*p2++ = p2;
		*p2++ = 0;
	}
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
char *lemt(P(char **) airt)
PP(char **airt;)
{
	register char *mtpt;

	pirt = airt + hash();				/*pointer to entry in irt */
	mtpt = pirt->irfe;					/*pointer to first entry in chain */
	if (mtpt == 0)						/*empty chain */
		mtpt = lmte;					/*start at end of main table */
	else
		(pirt->irle)->tlnk = lmte;		/*last entry in chain is new symbol */
	return (nextsy(mtpt));				/*return next match on chain */
}


char *nextsy(P(char *) amtpt)
PP(char *amtpt;)
{
	register char *mtpt;
	register int *p1, *p2;
	register int i;

	mtpt = amtpt;

	/*loop to locate entry in main table*/
  lemtl:
	p1 = &mtpt->name[0];
	p2 = &lmte->name[0];
	for (i = 0; i < SYNAMLEN / (sizeof i); i++)
	{
		if (*p1++ != *p2++)
		{
			mtpt = mtpt->tlnk;			/*go to next entry in chain */
			goto lemtl;
		}
	}
	return (mtpt);
}


/*make an entry in the main table*/
/* assumes:*/
/* entry to be made is pointed at by lmte*/
/* pirt points to the correct initial reference table entry*/

VOID mmte(P(NOTHING))
{

	pirt->irle = lmte;					/*pointer to last entry in chain */
	if (pirt->irfe == 0)				/*first entry in chain */
		pirt->irfe = lmte;
	addmte();
}


/* add the symbol pointed to by lmte to symbol table*/
VOID addmte(P(NOTHING))
{
	lmte += (sizeof *symptr);			/*bump last main table entry pointer */
	if (lmte >= emte)
	{									/*main table overflow */
		if (sbrk((sizeof *symptr) * ICRSZMT) == -1)
		{								/*get more memory */
			printf(": symbol table overflow\n");
			exit(-1);
		} else
		{
			emte += (sizeof *symptr) * ICRSZMT;	/*move end of main table */
			cszmt += ICRSZMT;
		}
	}
}

/* compute a hash code for the last entry in the main table*/
/* returns the hash code*/

int hash(P(NOTHING))
{
	register ht1, i;					/*temps */
	register char *p;

	ht1 = 0;
	p = &lmte->name[0];
	for (i = 0; i < SYNAMLEN; i++)
		ht1 += *p++;
	return (ht1 & 076);					/*make hash code even and between 0 and 62 */
}


/* pack a string into an entry in the main table*/
/* call with:*/
/* pointer to the string*/
/* pointer to desired entry in the main table*/

VOID pack(P(int *) apkstr, P(int *) apkptr)
PP(int *apkstr;)
PP(int *apkptr;)
{
	register int i;
	register int *pkstr, *pkptr;

	pkstr = apkstr;
	pkptr = apkptr;
	for (i = 0; i < SYNAMLEN / (sizeof i); i++)
		*pkptr++ = *pkstr++;
}


/* get a temp file for the intermediate text*/

VOID gettempf(P(NOTHING))
{
	register i, j;

	(TFCHAR)++;
	if ((j = creat(tfilname, 0600, 1)) >= 0)
	{
		return (j);
	}
	printf(": Unable to open temporary file: %s\n", tfilname);
	endit(-1);
}


/* update the relocation counters with the sizes in the header*/

VOID addsizes(P(NOTHING))
{
	textbase += couthd.ch_tsize;
	database += couthd.ch_dsize;
	bssbase += couthd.ch_bsize;
}


/*save the current state of the symbol table -- it may be restored later*/

VOID savsymtab(P(NOTHING))
{
	register char **p1, **p2;
	register i;

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


/*restore the symbol table as it was when we last saved it*/
VOID restsymtab(P(NOTHING))
{
	register char **p1, **p2;
	register i;

	lmte = savlmte;
	p1 = eirt;
	p2 = saveirt;
	for (i = 0; i < SZIRT; i++)
		*p1++ = *p2++;
	p1 = girt;
	p2 = savgirt;
	for (i = 0; i < SZIRT; i++)
		*p1++ = *p2++;
}


/* resolve the external variable addresses and set the*/
/* base address of the data and bss segments.*/
/* also allocate storage for the common variables.*/
VOID resolve(P(NOTHING))
{
	register char *p;

	textsize = textbase - textstart;
	datasize = database;
	bsssize = bssbase;
	if (Dflag)
		database = datastart;
	else
	{
		database = (textbase + 1) & ~1;
		datastart = database;
	}
	if (Bflag)
		bssbase = bssstart;
	else
	{
		bssbase = (database + datasize + 1) & ~1;
		bssstart = bssbase;
	}
	textbase = textstart;
	fixsyms();							/*relocate symbols with addresses */
	fixexts();							/*fix external addresses & commons */
	if (etextptr)
	{
		pack(etexstr, lmte);
		p = lemt(eirt);
		do
		{
			p->vl1 = textsize;
			p->flags &= ~SYXR;			/*no longer external */
			p->flags |= SYDF | SYGL;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
	if (edataptr)
	{
		pack(edatstr, lmte);
		p = lemt(eirt);
		do
		{
			p->vl1 = textsize + datasize;
			p->flags &= ~SYXR;			/*no longer external */
			p->flags |= SYDF | SYGL;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
	if (endptr)
	{
		pack(eendstr, lmte);
		p = lemt(eirt);
		do
		{
			p->vl1 = textsize + datasize + bsssize;
			p->flags &= ~SYXR;			/*no longer external */
			p->flags |= SYDF | SYGL;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
}


/* fix symbol addresses that have been assigned by adding in*/
/* database and bssbase*/
VOID fixsyms(P(NOTHING))
{
	register struct symtab *p;

	for (p = bmte; p < lmte; p++)
	{									/*look at each symbol */
		if (p->flags & SYXR)
			continue;
		if (p->flags & SYDA)			/*data symbol */
			p->vl1 += database;
		else if (p->flags & SYBS)		/* bss symbol */
			p->vl1 += bssbase;
	}
}


/* get addresses for all external symbols and common symbols*/
VOID fixexts(P(NOTHING))
{
	register char *p;
	register char **sx1, **sx2;

	for (sx1 = eirt; sx1 < &eirt[63]; sx1 += 2)
	{									/*go thru externals */
		if (*(sx2 = sx1 + 1) == 0)		/*this chain empty */
			continue;

		/* go thru symbols on chain*/
		sx2 = *sx2;						/*first entry on this chain */
		while (1)
		{
			if (sx2->vl1)
				asgncomn(sx2);			/*assign a common address */
			else
				asgnext(sx2);			/*match to a global */
			p = sx2;
			if (p == *sx1)				/*end of chain */
				break;
			sx2 = sx2->tlnk;			/*next entry in chain */
		}
	}
}


/* assign an address for a block of bss common*/
VOID asgncomn(P(char *) ap)
PP(char *ap;)
{
	register char *p, *p1;
	register int32_t l;

	p = ap;
	pack(p, lmte);
	p1 = lemt(girt);
	if (p1 != lmte)
	{									/*matches a global entry */
		ap->vl1 = p1->vl1;
		return;
	}
	l = 0;
	lemt(eirt);
	do
	{
		if (p->vl1 > l)
			l = p->vl1;
		p->vl1 = 0;
	} while ((p = nextsy(p->tlnk)) != lmte);
/*now make a global entry for this common block*/
	p->flags = SYDF | SYGL | SYBS;
	p->vl1 = bssbase + bsssize;
	ap->vl1 = p->vl1;					/*copy address to first external */
	bsssize += l;
	lemt(girt);							/*set ptrs for global chain */
	mmte();								/*add to global chain */
}


/* assign an address to an external by matching it with a global*/
/* print an error message if no match*/
VOID asgnext(P(char *) ap)
PP(char *ap;)
{
	register char *p, *pg;

	p = ap;
	pack(p, lmte);						/*copy current symbol name */
	pg = lemt(girt);
	if (pg == lmte)
	{									/*no match in global symbols */
		pg = lemt(eirt);				/*set ptrs for external chains */
		if (pg == lmte)
		{
			printf(": asgnext botch\n");
			endit(-1);
		}
		while ((p = nextsy(p->tlnk)) != lmte)
		{
			if (p->vl1)
			{							/*common */
				asgncomn(ap);
				return;
			}
		}
		if (spendsym(ap))				/*end, etext or edata */
			return;
		if (undflg == 0)
		{
			printf(": Undefined symbol(s)\n");
			undflg++;
			exstat++;
		}
		prtsym(p);
	} else
	{
		p->vl1 = pg->vl1;				/*assign symbol value */
	}
}


/* print a symbol name for an error message*/
VOID prtsym(P(char *) ap)
PP(char *ap;)
{
	register i;
	register char *p;

	p = ap;
	for (i = 0; i < SYNAMLEN; i++)
	{
		if (*p)
			putchar(*p++);
		else
			break;
	}
	putchar('\n');
}


/* pass 2 load routine:*/
/* read the file or library and do relocation */

VOID p2load(P(char *) ap)
PP(char *ap;)
{
	openfile(ap);						/*get the file opened using ibuf */
	if (couthd.ch_magic == LIB1MAGIC)
	{
		libhdsize = LIB1HDSIZE;
		loadlib();
	} else if (couthd.ch_magic == LIB2MAGIC)
	{
		libhdsize = sizeof *lib2hd;
		loadlib();
	} else if (couthd.ch_magic == LIB3MAGIC)
	{
		libhdsize = LIB3HDSIZE;
		loadlib();
	} else
	{
		do2load(0);						/*load a *.o file */
		addsizes();
	}
	close(ibuf.fildes);
	close(rbuf.fildes);
}


/*
 * make the outut file and write the header
 */
VOID makeofile(P(NOTHING))
{
	int32_t l;

	if ((obuf.fildes = creat(ofilname, 0666, 1)) < 0)
	{
		printf(": Unable to create %s\n", ofilname);
		endit(-1);
	}
	obuf.nunused = 512;
	obuf.xfree = &obuf.buff[0];
	if (Dflag | Bflag)
		putw(MAGIC1, &obuf);			/*data & bss bases in header */
	else
		putw(MAGIC, &obuf);				/*normal header */

#ifdef VAX
	putw(textsize.loword, &obuf);
	putw(textsize.hiword, &obuf);
	putw(datasize.loword, &obuf);
	putw(datasize.hiword, &obuf);
	putw(bsssize.loword, &obuf);
	putw(bsssize.hiword, &obuf);
	l = lmte - bmte;
	putw(l.loword, &obuf);
	putw(l.hiword, &obuf);
	putw(stacksize.loword, &obuf);
	putw(stacksize.hiword, &obuf);
	putw(textstart.loword, &obuf);
	putw(textstart.hiword, &obuf);
	if (saverbits)
	{
		putw(0, &obuf);					/*relocation bits present */
	} else
	{
		putw(-1, &obuf);				/*relocation bits removed */
		/*              putw(0,&obuf); *//*pad for VAX header */
	}

	if (Dflag | Bflag)
	{									/*output expanded header */
		putw(datastart.loword, &obuf);
		putw(datastart.hiword, &obuf);
		putw(bssstart.loword, &obuf);
		putw(bssstart.hiword, &obuf);
	}
#endif

#ifdef PDP11
	putw(textsize.loword, &obuf);
	putw(textsize.hiword, &obuf);
	putw(datasize.loword, &obuf);
	putw(datasize.hiword, &obuf);
	putw(bsssize.loword, &obuf);
	putw(bsssize.hiword, &obuf);
	l = lmte - bmte;
	putw(l.loword, &obuf);
	putw(l.hiword, &obuf);
	putw(stacksize.loword, &obuf);
	putw(stacksize.hiword, &obuf);
	putw(textstart.loword, &obuf);
	putw(textstart.hiword, &obuf);
	if (saverbits)
	{
		putw(0, &obuf);					/*relocation bits present */
	} else
	{
		putw(-1, &obuf);				/*relocation bits removed */
		/*              putw(0,&obuf); *//*pad for VAX header */
	}

	if (Dflag | Bflag)
	{									/*output expanded header */
		putw(datastart.loword, &obuf);
		putw(datastart.hiword, &obuf);
		putw(bssstart.loword, &obuf);
		putw(bssstart.hiword, &obuf);
	}
#endif

#ifdef MC68000
	putw(textsize.hiword, &obuf);
	putw(textsize.loword, &obuf);
	putw(datasize.hiword, &obuf);
	putw(datasize.loword, &obuf);
	putw(bsssize.hiword, &obuf);
	putw(bsssize.loword, &obuf);
	l = (long)lmte - (long)bmte;
	putw(l.hiword, &obuf);
	putw(l.loword, &obuf);
	putw(stacksize.hiword, &obuf);
	putw(stacksize.loword, &obuf);
	putw(textstart.hiword, &obuf);
	putw(textstart.loword, &obuf);
	if (saverbits)
	{
		putw(0, &obuf);					/*relocation bits present */
	} else
	{
		putw(-1, &obuf);				/*relocation bits removed */
	}

	if (Dflag | Bflag)
	{									/*output expanded header */
		putw(datastart.hiword, &obuf);
		putw(datastart.loword, &obuf);
		putw(bssstart.hiword, &obuf);
		putw(bssstart.loword, &obuf);
	}
#endif

	tmpbinit(&tbuf);					/*temp for data words */
	dafnc = TFCHAR;
	if (saverbits)
	{
		tmpbinit(&rtbuf);				/*temp for text relocation bits */
		rtfnc = TFCHAR;
		tmpbinit(&rdbuf);				/*temp for data relocation bits */
		rdfnc = TFCHAR;
	}
}


VOID tmpbinit(P(struct buf *) abufp)
PP(struct buf *abufp;)
{
	register struct buf *bufp;

	bufp = abufp;
	bufp->fildes = gettempf();
	bufp->nunused = BSIZE;
	bufp->xfree = &bufp->buff[0];
}


VOID endit(P(int) stat)
PP(int stat;)
{
	if (dafnc)
	{
		TFCHAR = dafnc;
		unlink(tfilname);
	}
	if (saverbits)
	{
		TFCHAR = rtfnc;
		unlink(tfilname);
		TFCHAR = rdfnc;
		unlink(tfilname);
	}
	exit(stat);
}


/* load files from a library.  the library is open in ibuf, the*/
/* count of files to load is pointed to by libfctr, and the offset*/
/* of each file is pointed to by libptr.*/
VOID loadlib(P(NOTHING))
{
	register i, j;
	register long l;

	i = *libfctr++;						/*# files to load from this library */
	if (i == 0)
		return;							/*none to load */
	while (i--)
	{									/*load the files */
		l = *libptr++;					/*library offset for this file */
		lbseek(l, &ibuf);				/*seek to beginning of file */
		lbseek(l, &rbuf);
		rdlibhdr();						/*read the library header */
		readhdr();						/*read the file header */
		lbctr = l;
		do2load(1);						/*load it */
		addsizes();
	}
}


/* do a long seek on buffer bp given a long file offset*/
/* last argument indicates relative or absolute seek*/
VOID lbseek(P(long) al, P(struct buf *)bp)
PP(long al;)
PP(struct buf *bp;)
{
	register long bn;

	bn = (al / BLKSIZE) * BLKSIZE;
	if (lseek(bp->fildes, bn, 0) < 0)
	{
		printf(": seek error on file %s\n", ifilname);
		endit();
	}
	bp->nunused = 0;					/*input buff empty */
	bp->xfree = &bp->buff[0];
	for (; bn < al; bn++)
		getc(bp);
}


/* look up the value of an external symbol given the external symbol*/
/* number.  Since externals are not duplicated on the hash chains,*/
/* all externals don't have values assigned in the symbol table,*/
/* so the external name must be looked on the global chains and*/
/* the value of the matching symbol used. symptr points to the*/
/* first symbol for the current file's symbol table*/
int32_t extval(P(int) extno)
PP(int extno;)
{
	register struct symtab *p;
	register char *pg;

	p = symptr + extno;
	if ((p->flags & SYXR) == 0)
	{
		return (p->vl1);
	}
	pack(p, lmte);
	pg = lemt(girt);
	if (pg == lmte)
		pg = lemt(eirt);				/*may be common */
	return (pg->vl1);					/*return globals address */
}


/* look up the relocation base for an external symbol. must use same*/
/* method as in extval for the same reasons (see comment above)*/
VOID extbase(P(int) extno)
{
	register struct symtab *p;
	register char *pg;
	register int i;

	p = symptr + extno;
	if ((p->flags & SYXR) == 0)
	{
		pg = p;
	} else
	{
		pack(p, lmte);
		pg = lemt(girt);
		if (pg == lmte)
			pg = lemt(eirt);			/*may be common */
	}
	i = pg->flags;
	if (i & SYDA)
		return (DRELOC);
	else if (i & SYTX)
		return (TRELOC);
	else if (i & SYBS)
		return (BRELOC);
	return (DABS);
}


/* load a file doing relocation and external resolution*/
/* libflg is set if we are loading from a library*/
VOID do2load(P(int) libflg)
PP(int libflg;)
{
	register i, j;
	int longf;
	register struct buf *p;
	register struct buf *pr;
	int saof;
	register long tpc;
	register long l;
	long l1;
	int wasext;

	tpc = 0;
	p = &obuf;
	pr = &rtbuf;
	saof = -1;
	symptr = *firstsym++;				/*beginning of this symbol table */
	l = couthd.ch_tsize + couthd.ch_dsize + couthd.ch_ssize + HDSIZE;
	if (libflg)
		l += lbctr + sizeof *lib2hd;
	lbseek(l, &rbuf);					/*long seek */
	l = couthd.ch_tsize;
  do2l1:
	while ((l -= 2) >= 0)
	{									/*relocate the text */
		longf = 0;
		i = getw(&ibuf);
		j = getw(&rbuf);
		tpc += 2;						/*keep pc in this file */
		wasext = 0;
		switch (j & 7)
		{								/*relocation bits */

		case INSABS:					/*first word of instr */
		case DABS:						/*data absolute */
			putw(i, p);
			if (saverbits)
				putw(j, pr);			/*relocation bits */
			break;

		case LUPPER:					/*high word of long */
			l1.hiword = i;
			if (saverbits)
				putw(j, pr);			/*upper word relocation bits */
			l1.loword = getw(&ibuf);
			j = getw(&rbuf);
			tpc += 2;
			longf++;					/*doing two words */
			l -= 2;						/*count lower half */
		  dorelc:
			switch (j & 7)
			{

			case DABS:
				if (saverbits)
					putw(j, pr);
				break;
			case INSABS:
			case LUPPER:
			default:
				goto do2199;

			case TRELOC:
				l1 += textbase;
				if (saverbits)
					putw(j, pr);
				break;

			case DRELOC:
				l1 += database;
				if (saverbits)
					putw(j, pr);
				break;

			case BRELOC:
				l1 += bssbase;
				if (saverbits)
					putw(j, pr);
				break;

			case EXTVAR:
				wasext++;
				l1 += extval(j >> 3);
				if (saverbits)
					putw(extbase(j >> 3), pr);
				break;

			case EXTREL:
				l1 = l1 + extval(j >> 3) - textbase - tpc + 2;
				if (l1 < (-32768L) || l1 > 0x7fff)
				{
					printf(": relative address overflow at %lx in %s\n", tpc, ifilname);
					prextname(j >> 3);	/*give name referenced */
					exstat++;
				}
				l1.hiword = 0;
				if (saverbits)
					putw(DABS, pr);
				goto outlowd;

			}
			if (ignflg == 0 && longf == 0 && l1 & 0xffff8000 && saof)
			{
				printf(": short address overflow in %s\n", ifilname);
				if (wasext)
					prextname(j >> 3);
				exstat++;
				if (libflg)
				{
					l1 = *(libptr - 1);
					printf("library offset = %x\n", (int) l1.loword);
					l1 = 0;
				}
				saof = 0;
			}
			if (longf)
				putw(l1.hiword, p);
		  outlowd:
			putw(l1.loword, p);
			break;

		case TRELOC:
		case DRELOC:
		case BRELOC:
		case EXTVAR:
		case EXTREL:
#ifdef NOMMU
			l1 = *(libptr - 1);
			printf("library offset = %u\n", l1.loword);
			printf("16-bit reloc in %s\n", ifilname);
#endif
			l1 = i;						/*sign extend to long like 68000 */
			goto dorelc;

		default:
		  do2199:
			printf(": File format error: invalid relocation flag in %s\n", ifilname);
			endit(-1);

		}
	}
	if (p == &obuf)
	{
		p = &tbuf;						/*place to put data */
		pr = &rdbuf;					/*file for data relocatin bits */
		l = couthd.ch_dsize;
		goto do2l1;						/*now do the data */
	}
}


/* do the final writting to the output file*/
/* copy the initialized data from the temp file to the output file*/
/* write the symbol table to the output file*/
VOID finalwr(P(NOTHING))
{
	if ((textsize + textstart) != textbase)
	{
		printf(": finalwr: text size error\n");
		exstat++;
	}
	cpdata(&tbuf, dafnc, datasize);
	osymt();							/*write the symbol table */
	if (saverbits)
	{
		cpdata(&rtbuf, rtfnc, textsize);
		cpdata(&rdbuf, rdfnc, datasize);
	}
	fflush(&obuf);
	if (lseek(obuf.fildes, 14L, 0) < 0 || write(obuf.fildes, &stlen, 4) != 4)
	{
		printf(": output file write error\n");
		exstat++;
	}
	endit(exstat);
}


/* copy the initialized data words from temp file to output file*/
VOID cpdata(P(struct buf *) pb, P(int) fnc, P(long) size)
PP(register struct buf *pb;)
PP(int fnc;)
PP(long size;)
{
	register j;

	fflush(pb);
	close(pb->fildes);
	TFCHAR = fnc;
	if ((fopen(tfilname, pb, 1)) < 0)
	{
		printf(": unable to reopen %s\n", tfilname);
		endit(-1);
	}
	while (size > 0)
	{
		j = getw(pb);					/* Fetch word from source buffer */
		putw(j, &obuf);
		size -= 2;						/* Down by 2 bytes              */
	}
}


/* output symbol table to file*/
VOID osymt(P(NOTHING))
{
	register struct symtab *p;

	stlen = 0;
	if (sflag)							/*no symbol table desired */
		return;

	/* now output the symbols deleting externals*/

	for (p = bmte; p < lmte; p++)
	{
		if (p->flags & SYXR)			/*external symbol */
			continue;
		if ((p->flags & SYGL) == 0 && (p->name[0] == 'L' || Xflag == 0))
			continue;
		osyme(p);
	}
}

/* output symbols in a form to be read by a debugger */
/* call with pointer to symbol table entry*/

long ll = 0;

VOID osyme(P(struct symtab *) aosypt)
PP(struct symtab *aosypt;)
{
	register struct symtab *osypt;
	register short *p1;
	register int i;

	osypt = aosypt;						/*pointer to symbol table entry */

	p1 = &(osypt->name[0]);

	stlen += OSTSIZE;					/*one more symbol out */

	/*output symbol to loader file*/
	p1 = &(osypt->name[0]);
	for (i = 0; i < SYNAMLEN / 2; i++)
	{									/*output symbol name */
		putw(*p1++, &obuf);
	}

	putw(osypt->flags, &obuf);			/*output symbol flags */
	ll = osypt->vl1;
	putw(ll.hiword, &obuf);				/*output symbol value */
	putw(ll.loword, &obuf);
}


/*get constant given radix*/
/* this routine only works for radixes of 2, 8, 16*/
/* so as to avoid lmul because of floating point*/
int32_t lgetnum(P(char *) apc, P(int) ardx)
PP(char *apc;)
PP(int ardx;)
{
	register trdx, j;
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


/* look for and define if found etext, edata, and end*/

VOID spendsym(P(struct symtab *) ap)
PP(struct symtab *ap;)
{
	register struct symtab *p;

	p = ap;
	if (eqstr(etexstr, &p->name[0]))
	{
		etextptr = p;
		return (1);
	}
	if (eqstr(edatstr, &p->name[0]))
	{
		edataptr = p;
		return (1);
	}
	if (eqstr(eendstr, &p->name[0]))
	{
		endptr = p;
		return (1);
	}
	return (0);
}


/* test two symbol names for equality */
int eqstr(P(char *) ap1, P(char *) ap2)
PP(char *ap1;)
PP(char *ap2;)
{
	register char *p1;
	register char *p2;
	register int i;

	p1 = ap1;
	p2 = ap2;
	for (i = 0; i < SYNAMLEN; i++)
	{
		if (*p1++ != *p2++)
		{
			return (0);
		}
	}
	return (1);
}


/* print an error message giving an external name */
VOID prextname(P(int) extno)
PP(int extno;)
{
	register struct symtab *p;
	register char *pc;

	p = symptr + extno;
	printf(": external name: ");
	for (pc = &p->name[0]; pc < &p->name[SYNAMLEN]; pc++)
	{
		if (*pc == 0)
			break;
		putchar(*pc);
	}
	putchar('\n');
}
