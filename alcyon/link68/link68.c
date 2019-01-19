#include "link68.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#undef max
#define max(x,y)   (((x) > (y)) ? (x) : (y))

int32_t textstart;
int32_t datastart;
int32_t bssstart;

int Dflag;
int Bflag;
int Zflag;
static int Xflag;
static int sflag;						/* if set, keep symbol table */

int ignflg;
int exstat;
int ovflag;

char cmdline[LINELEN + 2];

int mapflg;
int absflg;
int symflg;						/* set if symbol table to be saved */
int udfflg;						/* set if undefined symbols allowed */
int chnflg;						/* set if linking chained program */
int dmpflg;						/* set for dumping symbol table */

static int libflg;				/* set if an input file to be searched */

static struct hdr couthd;

/* parameters that define the main table */
#define SZMT 300						/* initial size of the main table */
					/* must be large enough to initialize */
#define ICRSZMT 100						/* add to main table when run out */

static struct symtab *bmte;				/* beginning of main table */

struct symhash {
	struct symtab *last;		/* ptr to last entry in chain */
	struct symtab *first;		/* ptr to first entry in chain */
};
/* initial reference table for externals */
# define SZIRT	32
static struct symhash eirt[SZIRT];
static struct symhash saveirt[SZIRT];



/* initial reference table for globals */
static struct symhash girt[SZIRT];
static struct symhash savgirt[SZIRT];

static struct symhash *pirt;

static struct symtab *lmte;				/* last entry in main table */

static struct symtab *savlmte;

static FILE *ibuf;
static FILE *tbuf;
static FILE *obuf;
static FILE *rbuf;
static FILE *rtbuf;
static FILE *rdbuf;


static int32_t textbase;
static int32_t database;
static int32_t bssbase;

static int32_t textsize;
static int32_t datasize;
static int32_t bsssize;
static int32_t stacksize;
#define MINSTACK 0x1000

static const char *ifilname;			/* points to name of current input file */
static char ilibmem[FNAMELEN * 2 + 2];
static const char *ilibname;

static const char *outfname;			/* points to name of current output file */

#define NFILE	256						/* max # files we can process */
static struct symtab *fsymp[NFILE];		/* points to first symbol for each .o file */
static struct symtab **firstsym;		/* points to entry in fsymp */
static struct symtab *symptr;

static int extmatch;					/* matched an external in a library entry */

static int noload;						/* dont load this lib file flag */

#define NLIB	16						/* max # libraries to process */
static int lbfictr[NLIB];				/* counts files loaded from one library */

static int *libfctr;					/* points to lbfictr */
static int32_t lbfioff[NFILE];			/* each file offset in library */

static int32_t *libptr;					/* points to lbfioff */

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
static struct ar_hdr lib3_hdr;

#define LIB3MAGIC 0x3c21
#define LIB3HDSIZE	(sizeof lib3_hdr)

static int libhdsize;
union alibhd {
	struct libhdr hdr1;
	struct lib2hdr hdr2;
	struct ar_hdr ar;
	int32_t l[20];					/* length is max lib head size */
};
static union alibhd libhd;

static int umesflg;
static char dafnc;
static int pass2;
static int32_t stlen;


static struct symtab *etextptr;
static struct symtab *edataptr;
static struct symtab *erootptr;
static struct symtab *endptr;
static struct symtab *lastdup;
static struct symtab *stksptr;
static int32_t stkspos;

static char rtfnc;
static char rdfnc;

static int saverbits;

#define TBNAMESIZE 8
#define TBEXTSIZE  3
struct ovtab {
	char tbname[TBNAMESIZE];	/* file name -- pad with blanks */
	char tbext[TBEXTSIZE + 1];	/* file extension -- no dot (.) */
	int32_t tbldpt;				/* load point for module */
};
#define SIZEOF_OVTAB 16

/* structure of an indirect call to an overlay routine */

struct ovcalblk {
	short jsrovh;			/* call overlay handler */
	int32_t ovhandlr;		/* address of overlay handlr */
	int32_t ovtabad;		/* pointer into overlay table */
	short jmprout;			/* jump to overlayed routine */
	int32_t routaddr;		/* address to jump to */
};
#define SIZEOF_OVCALBLK 16

static struct ovtab ovtab1;

/* 68000 linking loader -- adapted from */
/*   Bill Allen's to handle overlays */

/*
 * This loader is a two pass operation.  The first pass
 *  reads all the relocatable object files including any libraries.
 *  It builds the full symbol table, resolves all external symbol
 *  references if possible, and calculates the total size of the
 *  text, data, and bss segements.  Pass 2 then does the binding,
 *  the relocation, and produces the final output file.
 */

#ifdef VMS
static char libname[] = "lib:lib6.a";	/* default library name */
#define	LIBCHAR libname[7]				/* Character to be modified */
#endif

#ifdef CPM
static char libname[] = "lib6.a";
#define	LIBCHAR	libname[3]				/* Character to be modified */
#endif

#ifdef GEMDOS
static char libname[] = "libc.a";
#define	LIBCHAR	libname[3]				/* Character to be modified */
#endif

#ifdef UNIX
static char libname[] = "/lib/lib6.a";	/* Default library name */
#define LIBCHAR libname[8]				/* Character to be modified */
#endif

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
char tdisk[80] = "/tmp/";			/* Temp disk name */
#else /* CP/M and VMS */
static const char *tfbase = "loXXXXXA";	/* Temp base name */
char tdisk[80];					/* Temp disk name */
#endif

static char *tfchar;					/* -> changeable character */
static char tfilname[80];				/* first temp file name */

#define TFCHAR *tfchar					/* Name to simplify assignments */
static char const etexstr[] = "_etext\0\0";
static char const edatstr[] = "_edata\0\0";
static char const erootstr[] = "_eroot\0\0";
static char const eendstr[] = "_end\0\0\0\0";
static char const ovhstr[] = "_ovhdlr\0";
static char const stksstr[] = "__stksiz";

static int32_t rtxsize;					/* size of root's text */
static int32_t rdtsize;					/* size of root's data */
static int32_t hihiaddr;				/* first even word above whole program */

static int32_t ovtable;					/* address of overlay table start */

static struct ovcalblk ovcall;


struct ovtrnode *ovtree[MAXOVLS + 1];	/* command tree */


static int ovpath[MAXOVDEP + 1];		/* current path in ovtree walk */

static int ovpathtp = -1;				/* current end of path */


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
 * do a long seek on buffer bp  given a long file offset
 *  last argument indicates relative or absolute seek
 */

static VOID lbseek(P(int32_t) al, P(FILE *) bp)
PP(int32_t al;)
PP(FILE *bp;)
{
	if (fseek(bp, al, SEEK_SET) < 0)
		fatalx(FALSE, _("seek error on file: %s\n"), ifilname);
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
		fatalx(FALSE, _("file format error in %s\n"), ifilname);
	if (p->ch_rlbflg != 0)
		fatalx(FALSE, _("no relocation bits in %s\n"), ifilname);
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
	sprintf(ilibmem, "%s(%s)", ilibname, p->hdr2.l2fname);
	ifilname = ilibmem;
	return TRUE;
}


/*
 * update the relocation counters with the sizes in the header
 */
static VOID addsizes(NOTHING)
{
	textbase += ((couthd.ch_tsize + 1) & ~1);
	database += ((couthd.ch_dsize + 1) & ~1);
	bssbase += ((couthd.ch_bsize + 1) & ~1);
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


/* method for looking up entries in the main table
 *
 * Note:	The entry to be looked up must be placed at the end
 *			of the main table.  The global cell 'lmte'(last main
 *			entry) points to the next available entry in the main
 *			table.  The address of an initial reference table must
 *			also be provided.
 *	1)	Compute the hash code for the symbol and add it to the base address
 *		of the initial reference table given as input.  Thus, two words are
 *		accessed which define the chain on which the symbol must be if it is
 *		in the table at all.
 *	2)	Alter the table link of the last symbol in the chain so that it
 *		points to the symbol being looked up.  Note that the symbol to be
 *		looked up is always placed at the end of the main table before
 *		calling the lookup routine.  This essentially adds one more element
 *		to the end of the chain, namely the symbol to be looked up.
 *	3)	Now start at the first symbol in the chain and follow the chain
 *		looking for a symbol equal to the smbol being looked up.  It is
 *		quaranteed that such a symbol will be found because it is always
 *		the last symbol on the chain.
 *	4)	When the symbol is found, check to see if it is the last symbol
 *		on the chain.  If not, the symbol being looked for is in the table
 *		and has been found.  If it is the last symbol, the symbol being
 *		looked up is not in the table.
 *	5)	In the case the looked up symbol is not found, it is usually added
 *		to the end of the table.  This is done simply b changing the
 *		initial reference table entry which points to the previous
 *		last symbol on the chain so that is now points to the symbol at the
 *		end of the main table.  In case the symbol just looked up is not to
 *		be added to the main table then no action is needed .  This means
 *		that the table link of the last symbol on a chain may point any-
 *		where.
 * look up entry in the main table
 *		call with:
 *			address of initial reference table
 *			entry to be looked up at the end of the main table
 *		returns:
 *			a pointer to the entry.  if this pointer is equal to
 *			lmte then the symbol was not previously in the table.
 */

/*
 * compute a hash code for the last entry in the main table
 * returns the hash code
 */
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


/************************************************************************
 *
 * prtsym(sym) -- print a symbol name for an error message
 *
 ************************************************************************/
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
	
	p = symptr;
	while (extno && p)
	{
		--extno;
		p = p->next;
	}
	if (p == NULL)
		fatalx(FALSE, _("%s: corrupted"), ifilname);
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
static VOID do2load(P(int) lflg)
PP(int lflg;)
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
	if (lflg)
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
			if (saverbits)
				put16be(j, pr);			/* relocation bits */
			break;

		case LUPPER:					/* high word of long */
			l1 = ((int32_t)i) << 16;
			if (saverbits)
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
				if (saverbits)
					put16be(j, pr);
				break;
			case INSABS:
			case LUPPER:
			default:
				goto do2199;

			case TRELOC:
				l1 += textbase;
				if (saverbits)
					put16be(j, pr);
				break;

			case DRELOC:
				l1 += database;
				if (saverbits)
					put16be(j, pr);
				break;

			case BRELOC:
				l1 += bssbase;
				if (saverbits)
					put16be(j, pr);
				break;

			case EXTVAR:
				wasext++;
				l1 += extval(j >> 3);
				if (saverbits)
					put16be(extbase(j >> 3), pr);
				break;

			case EXTREL:
				l1 = l1 + extval(j >> 3) - textbase - tpc + 2;
				if (l1 < (-32768L) || l1 > 0x7fff)
				{
					errorx(_("relative address overflow at %lx in %s\n"), (long)tpc - 2, ifilname);
					prextname(j >> 3);	/* give name referenced */
				}
				if (saverbits)
					put16be(DABS, pr);
				goto outlowd;

			}
			if (ignflg == 0 && longf == 0 && (l1 & 0xffff8000L) && saof)
			{
				errorx(_("short address overflow at %lx in %s\n"), (long)tpc - 2, ifilname);
				if (wasext)
					prextname(j >> 3);
				if (lflg)
				{
					l1 = *(libptr - 1);
					fprintf(stderr, "library offset = %lx\n", (long)l1);
					l1 = 0;
				}
				saof = 0;
			}
			if (longf)
			{
				if (p == tbuf && ftell(p) == stkspos)
				{
					if (l1 == 0)
						l1 = MINSTACK;
					if (l1 > 4)
						stacksize = l1;
				}
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
			l1 = (int32_t)(int)i;			/* sign extend to long like 68000 */
			goto dorelc;

		default:
		  do2199:
			fatalx(FALSE, _("invalid relocation flag in %s\n"), ifilname);
			break;
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
	ilibname = ifilname;
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
	char lfname[FNAMELEN + 10];
	
	stpt = lmte;
	for (i = 0; i < SYNAMLEN; i++)
		stpt->name[i] = getc(ibuf);
	stpt->ovlnum = ovpath[ovpathtp];	/* mark which module it's in */
	if (chnflg && (strncmp(stpt->name, CBMAIN, SYNAMLEN) == 0))	/* main CBASIC entry point? */
	{
		i = stpt->ovlnum;
		if (i < 0 || i >= 1000)
		{
			fatalx(FALSE, _("overlay number %d out of range\n"), i);
		} else
		{
			sprintf(lfname, "main.%03d", i);	/* make name unique */
			strcpy(stpt->name, lfname);
		}
	}
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
		fatalx(FALSE, _("invalid symbol flag in %s, symbol: \"%.*s\"\n"), ifilname, SYNAMLEN, lmte->name);
	}
	lmte->vl1 += l;
}


static VOID prdup(P(const char *) p)
PP(const char *p;)
{
	errorx(_("\"%.*s\" doubly defined in %s\n"), SYNAMLEN, p, ifilname);
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
	ptr->ovlnum = 0;
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
static VOID addsym(NOTHING)
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
		} else
		{
			/* trick searchlib to load lib file */
			extmatch = 1;
		}
		p = lemt(girt);
	 addtry2:
	 	if (p == lmte)
	 	{
			mmte();
		} else
		{
		 	if (chnflg && (p->ovlnum != ROOT) &&
				(lmte->ovlnum != ROOT) && (p->ovlnum != lmte->ovlnum))
			{
				p = nextsy(p->tlnk);	/* try again */
				goto addtry2;
			}
			if (libflg)
			{
				noload++;
				lastdup = p;
			} else if ((p->ovlnum != lmte->ovlnum) || (p->flags != lmte->flags) || (p->vl1 != lmte->vl1))
			{
				/* dup defn msg */
				prdup(p->name);
			}
		}
	} else
	{
		/* normal symbol */
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
	char tempname[FNAMELEN];

	p = ap;
	if (*p == '-' && *++p == 'l')
	{
		if (*++p)
			LIBCHAR = *p;
		else
			fatalx(FALSE, _("missing filename for -l\n"));
		p = libname;
	} else
	{
		p = ap;
	}
	if ((ibuf = fopen(p, "rb")) == NULL)
	{
		strncpy(tempname, p, FNAMELEN - DEFLEN);
		strcat(tempname, DEFTYPE);
		if ((ibuf = fopen(tempname, "rb")) == NULL)
			fatalx(FALSE, _("cannot open %s for input\n"), p);
		strcpy(p, tempname);
	}
	/* point to current file name for error msgs */
	ifilname = p;
	couthd.ch_magic = get16be(ibuf);
	if (feof(ibuf) || ferror(ibuf))
		fatalx(FALSE, _("read error on file: %s\n"), ifilname);
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


/*
 * load one *.o format file using ibuf
 *	put the symbols into the symbol table relocating each one
 *	and finally add this files sizes into running totals
 *  libflg is set if we are in a library
 */
static VOID do1load(P(int) lflg)
PP(int lflg;)
{
	register int32_t i;
	register int32_t l;

	if (firstsym >= &fsymp[NFILE])
		fatalx(FALSE, _("%s: too many input files"), ifilname);
	/* remember where this symbol table starts */
	*firstsym = lmte;
	l = couthd.ch_tsize + couthd.ch_dsize + HDSIZE;
	if (lflg)
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
		addsym();
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
		fatalx(FALSE, _("too many libraries"));
	}
	/* no files from this library yet */
	*libfctr = 0;
	/* current library position - skip magic */
	lbctr = 2;
	ilibname = ifilname;
	while (rdlibhdr())					/* read library file header */
	{
		/* save current state of symbol table */
		savsymtab();
		extmatch = 0;
		noload = 0;
		/* read the file header */
		readhdr();
		/* load this lib file */
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
				fatalx(FALSE, _("%s: too many input files"), ifilname);
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
		libflg = FALSE;					/* set to TRUE by dopass1 */
		/* load a regular file */
		do1load(0);
		firstsym++;
		addsizes();
	}
	fclose(ibuf);
	ibuf = NULL;
}


/************************************************************************
 *
 * dopass1(*ovnode) -- do pass one processing on a given node
 *
 *
 ************************************************************************/
static VOID dopass1(P(struct ovtrnode *) opt)
PP(register struct ovtrnode *opt;)		/* pointer to node in command tree */
{
	register struct filenode *fpt, *tfpt;

	/* start of this modules symbols */
	opt->ovfsym = lmte;
	if (ovpath[ovpathtp] == ROOT)
		textbase = textstart;
	else
		textbase = 0;
	opt->ovtxbase = textbase;
	database = 0;
	bssbase = 0;

	/* get first input file */
	fpt = opt->ovflist;

	/* process each file */
	while (fpt != NULL)
	{
		if (fpt->fnflags & FNINCL)
		{
			/* next name is a symbol */
			tfpt = fpt->fnnext;
			/* get the include symbols */
			while ((tfpt != NULL) && (tfpt->fnflags & FNSYM))
			{
				/* put in symbol table */
				pack(tfpt->fnfname, lmte->name);
				/* external */
				lmte->flags = SYXR;
				/* mark which module */
				lmte->ovlnum = ovpath[ovpathtp];
				/* regular symbol */
				addsym();
				/* get the next node */
				tfpt = tfpt->fnnext;
			}
			/* skip to real file node */
			fpt->fnnext = tfpt;
		}

		/* search if lib? */
		libflg = !(fpt->fnflags & FNALL);
		/* ommit locals? */
		Xflag = (fpt->fnflags & FNLOCS);

		/* process the file */
		p1load(fpt->fnfname);

		/* get the next file */
		fpt = fpt->fnnext;
	}
	opt->ovdtbase = Dflag ? datastart : (textbase + 1) & ~1;
	opt->ovbsbase = Bflag ? bssstart : (opt->ovdtbase + database + 1) & ~1;
	opt->ovcap = (opt->ovbsbase + bssbase + 1) & ~1;
	if (ovpath[ovpathtp] == ROOT)
	{
		rtxsize = textbase - textstart;
		rdtsize = database;
	}
}


/************************************************************************
 *
 * inkid(num,node) -- is num in command tree node's kidlist?
 *
 ************************************************************************/
static BOOLEAN inkid(P(int) num, P(int) node)
PP(register int num;)
PP(register int node;)
{
	/* get index of first kid */
	node = ovtree[node]->ovfstkid;
	while (node != 0)
	{
		if (node == num)				/* same node? */
			return TRUE;
		/* get the next kid */
		node = ovtree[node]->ovnxtsib;
	}
	/* didn't find it */
	return FALSE;
}


/************************************************************************
 *
 * newjblk() -- allocate a new jump block and put it in the command tree
 *
 ************************************************************************/
static struct jmpblock *newjblk(NOTHING)
{
	register struct jmpblock *npt, *tpt;
	register struct ovtrnode *opt;

	/* get a piece of memory */
	if ((npt = malloc(sizeof(*npt))) == NULL)
		oom();
	/* get current command tree node */
	opt = ovtree[ovpath[ovpathtp]];
	if ((tpt = opt->ovjblck) == NULL)
	{
		/* empty list */
		opt->ovjblck = npt;
	} else
	{
		/* find end of list */
		while (tpt->nxtjbl != NULL)
			tpt = tpt->nxtjbl;
		/* put at end of list */
		tpt->nxtjbl = npt;
	}
	npt->nxtjbl = NULL;
	return npt;
}


/************************************************************************
 *
 * chkovext -- check an external symbol for possible overlay reference
 *
 *	Returns 1 if a ovcall block is built.
 *
 *
 ************************************************************************/
static int chkovext(P(struct symtab *) spt)
PP(register struct symtab *spt;)
{
	register struct symtab *gpt;
	register int i;
	register struct jmpblock *jpt;
	register struct ovtrnode *ovpt;
	register int32_t cbadd;

	if (spt->vl1)						/* already been processed   */
		return 0;
	pack(spt->name, lmte->name);		/* set up for search        */
	gpt = lemt(girt);					/* look for global match    */
	if (gpt == lmte)					/* no match? -- return and let  */
		return 0;						/*   fixexts handle unresolved  */

	/* validate global -- must be either non-overlayed (relative to */
	/* module) or in child module and text-based            */

	for (i = 0; i <= ovpathtp; i++)		/* non-overlayed?   */
		if (gpt->ovlnum == ovpath[i])
			return 0;					/* no jump block    */
	if (!(gpt->flags & SYTX))
	{
		errorx(_("illegal reference to overlay symbol %.*s from module %s\n"), SYNAMLEN, spt->name, ovtree[spt->ovlnum]->ovfname);
		return 0;
	}
	i = ovpath[ovpathtp];				/* current overlay number */
	ovpt = ovtree[i];
	if (!(inkid(gpt->ovlnum, i)))
	{
		errorx(_("illegal reference to overlay symbol %.*s from module %s\n"), SYNAMLEN, spt->name, ovtree[spt->ovlnum]->ovfname);
		return 0;
	}
	jpt = newjblk();					/* put a new jump block in  */
	jpt->globref = gpt;					/*   ovtree, remember symbol    */
	cbadd = ovpt->ovdtbase;				/* get address of new code  */
	if (i != ROOT)						/* non-root text-based globals  */
		cbadd -= ovpt->ovtxbase;		/* are relocated later     */
	ovpt->ovdtbase += SIZEOF_OVCALBLK;	/* bump size up for new code */
	lemt(eirt);							/* get pointers right       */
	for (;;)
	{
		spt->vl1 = cbadd;				/* actual address of call block */
		spt->flags |= SYTX;				/* text-based relocatable   */
		spt->flags &= ~SYXR;			/* no longer external       */
		spt = nextsy(spt->tlnk);		/* get all references   */
		if ((spt == lmte) || (spt->ovlnum != i))
			return 1;					/* done with this symbol    */
	}
}


/************************************************************************
 *
 * ovexts(ovtreenode) -- resolve references to overlayed routines
 *
 *	This routine looks through all of the symbols for a module,
 *	and checks each external reference.  If the matching global
 *	is in a child overlay, an ovcall block is built containing the
 *	appropriate information.  The external is resolved to that
 *	ovcall block, which is placed at the end of the current
 *	module's text segment.
 *
 *	All overlay references are assumed to be jumps or subroutine
 *	calls.  References to non-text-based addresses in overlays
 *	result in an error message.
 *
 *	For chained programs, the routine only adjusts the bases.
 *
 ************************************************************************/
static VOID ovexts(P(struct ovtrnode *) ovpt)
PP(register struct ovtrnode *ovpt;)		/* points to node in command tree */
{
	register struct symtab *cursym;		/* current symbol being examined */
	int ovrefs;							/* count overlay references */
	register int32_t newbase;				/* use for adjusting seg. bases */
	register short onum;				/* this module's number     */

	onum = ovpath[ovpathtp];
	if (onum != ROOT)					/* adjust textbase to above parent */
	{
		newbase = ovtree[ovpt->ovparent]->ovcap;
		ovpt->ovtxbase = newbase;
		ovpt->ovdtbase += newbase;
		ovpt->ovbsbase += newbase;
		ovpt->ovcap += newbase;
	}
	ovrefs = 0;
	cursym = ovpt->ovfsym;				/* first sy tab entry for module */
	if (!chnflg)						/* no ovr tabs in chined program */
		while ((cursym->ovlnum == onum) && (cursym != lmte))
		{
			if (cursym->flags & SYXR)
				ovrefs += chkovext(cursym);	/* check ext ref    */
			cursym++;					/* get the next symbol  */
		}
	newbase = ovrefs * SIZEOF_OVCALBLK;	/* size of extra code    */
	ovpt->ovbsbase += newbase;			/* add in space for ovcalls */
	ovpt->ovcap += newbase;				/* ovdtbase adjusted in chkovext */
	if (onum == ROOT)
	{
		rtxsize += newbase;
		ovtable += newbase;				/* final relocatable address    */
	}
	hihiaddr = max(hihiaddr, ovpt->ovcap);	/* might be new top */
}


/************************************************************************
 *
 * walktree(node, visit, order) -- ordered walk of overlay tree.
 *
 *	Processes each node in the overlay tree in a pre-order or
 *	post-order walk,  starting at the specified node.
 *	At each node, the routine pointed to by visit is called
 *	to work at that node.
 *
 *	The routine pointed to by visit must take a single parameter,
 *	which is a pointer to a filenode.  The routine should not
 *	return a value.
 *
 ************************************************************************/

typedef VOID (*visitf) PROTO((struct ovtrnode *opt));
#define PREORDER 0	/* preorder search of command tree */
#define POSTORDER 1	/* postorder search of command tree */

static VOID walktree(P(int) idx, P(visitf) visit, P(int) order)
PP(register int idx;)					/* index into overlay tree  */
PP(visitf visit;)						/* routine to process node  */
PP(int order;)							/* specifies which order walk   */
{
	register int kidpt;

	ovpath[++ovpathtp] = idx;			/* put node in path list */
	if (order == PREORDER)
		(*visit) (ovtree[idx]);			/* process the node */
	kidpt = ovtree[idx]->ovfstkid;		/* get first descendant */
	while (kidpt != 0)					/* visit each descendant */
	{
		walktree(kidpt, visit, order);	/* depth-first call */
		kidpt = ovtree[kidpt]->ovnxtsib;	/* next descendant */
	}
	if (order != PREORDER)
		(*visit) (ovtree[idx]);			/* process the node */
	ovpathtp--;							/* take node out of list */
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
	register const char *p1, *p2;
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
	if (eqstr(erootstr, p->name))
	{
		erootptr = p;
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
	(TFCHAR)++;
	if ((*fp = fopen(tfilname, "wb")) == NULL)
	{
		fatalx(FALSE, _("unable to open temporary file: %s\n"), tfilname);
	}
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
astry2:
	if (pg == lmte)
	{									/* no match in global symbols */
		pg = lemt(eirt);				/* set ptrs for external chains */
		if (pg == lmte)
			fatalx(FALSE, _("internal error in %s\n"), "asgnext");
		if (spendsym(ap))				/* end, etext, errot, or edata */
			return;
		if (umesflg == 0)
		{
			fprintf(stderr, "UNDEFINED SYMBOL(S):\n");
			umesflg++;
			if (!udfflg)				/* undefined allowed?   */
				exstat++;				/* no, set for exit */
		}
		prtsym(stderr, p);
	} else
	{
		if (chnflg && (pg->ovlnum != ROOT) && (pg->ovlnum != p->ovlnum))
		{
			pg = nextsy(pg->tlnk);		/* must be in right module */
			goto astry2;
		}
		p->vl1 = pg->vl1;				/* assign symbol value */
	}
}


/************************************************************************
 *
 * fixexts() -- get addresses for all external symbols
 *
 *	On entry, any externals with values have been resolved to a
 *	jump block.
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
			if (sx2->vl1 == 0)			/* skip overlay calls   */
				asgnext(sx2);			/* match to a global    */
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
		if (p->flags & SYTX)			/* text symbol */
		{
			if (p->ovlnum != ROOT)
				p->vl1 += ovtree[p->ovlnum]->ovtxbase;
		} else if (p->flags & SYDA)		/* data symbol */
		{
			if (eqstr(stksstr, p->name) && (p->flags & SYDF) && (p->flags & SYGL))
			{
				stksptr = p;
				stkspos = p->vl1;
			}
			p->vl1 += ovtree[p->ovlnum]->ovdtbase;
		} else if (p->flags & SYBS)		/* bss symbol */
		{
			p->vl1 += ovtree[p->ovlnum]->ovbsbase;
		}
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

	stksptr = NULL;
	stkspos = -1;
	fixsyms();							/* relocate symbols with addresses */
	fixexts();							/* fix external addresses & commons */
	if (etextptr)
	{
		pack(etexstr, lmte->name);
		p = lemt(eirt);
		do
		{
			p->vl1 = ovtree[ROOT]->ovtxbase + rtxsize;
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
			p->vl1 = ovtree[ROOT]->ovdtbase + rdtsize;
			p->flags &= ~SYXR;			/* no longer external */
			p->flags |= SYDF | SYGL | SYDA;
		} while ((p = nextsy(p->tlnk)) != lmte);
	}
	if (erootptr)
	{
		pack(erootstr, lmte->name);
		p = lemt(eirt);
		do
		{
			p->vl1 = ovtree[ROOT]->ovcap;
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
			p->vl1 = hihiaddr;
			p->flags &= ~SYXR;			/* no longer external */
			p->flags |= SYDF | SYGL | SYBS;
		} while ((p = nextsy(p->tlnk)) != lmte);
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

static int32_t bsscomm;					/* use only in fixcoms and asgncomn */

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
		ap->vl1 = 0;					/* set once globals resolved */
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
	p->vl1 = bsscomm;
	p->ovlnum = ROOT;					/* the global goes in the root  */
	bsscomm += (l + 1) & ~1;			/* always start at even address */
	lemt(girt);							/* set ptrs for global chain    */
	mmte();								/* add to global chain      */
}


/************************************************************************
 *
 * fixcoms() -- allocate the bss space for common areas and global
 *	static data.
 *
 ************************************************************************/
static VOID fixcoms(NOTHING)
{
	register struct symtab *p;
	register struct symhash *sx1;
	register struct symtab *sx2;
	int32_t oldtop;

	oldtop = ovtree[ROOT]->ovcap - ovtree[ROOT]->ovbsbase;
	bsscomm = oldtop;					/* current free bss */
	for (sx1 = eirt; sx1 < &eirt[SZIRT]; sx1++)
	{									/* go thru externals */
		if ((sx2 = sx1->first) == NULL)	/* this chain empty */
			continue;

		/* go thru symbols on chain */
		for (;;)
		{
			if (sx2->vl1 != 0)
				asgncomn(sx2);			/* assign a common address */
			p = sx2;
			if (p == sx1->last)			/* end of chain */
				break;
			sx2 = sx2->tlnk;			/* next entry in chain */
		}
	}
	ovtree[ROOT]->ovcap += (bsscomm - oldtop);	/* adjust for common   */
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
		libhdsize = LIB2HDSIZE;
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
static VOID makeofile(P(char *) ofilname)
PP(char *ofilname;)
{
	outfname = ofilname;				/* save name in case of error   */
	if ((obuf = fopen(ofilname, "wb")) == NULL)
		fatalx(FALSE, _("unable to create file: %s\n"), ofilname);
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
		put32be(datastart, obuf);
		put32be(bssstart, obuf);
	}

	gettempf(&tbuf);					/* temp for data words */
	dafnc = TFCHAR;
	if (saverbits)
	{
		gettempf(&rtbuf);				/* temp for text relocatin bits */
		rtfnc = TFCHAR;
		gettempf(&rdbuf);				/* temp for data relocation bits */
		rdfnc = TFCHAR;
	}
}


/************************************************************************
 *
 * wrjumps() -- write the overlay call blocks in the text segment
 *
 ************************************************************************/
static VOID wrjumps(NOTHING)
{
	register int onum;
	register struct jmpblock *jpt;

	onum = ovpath[ovpathtp];
	jpt = ovtree[onum]->ovjblck;
	while (jpt != NULL)
	{
		onum = (jpt->globref)->ovlnum;	/* where is global? */
		ovcall.ovtabad = ovtable + ((onum - 1) * SIZEOF_OVCALBLK);
		ovcall.routaddr = (jpt->globref)->vl1;

		put16be(ovcall.jsrovh, obuf);	/* jsr  _ovhdlr */
		put32be(ovcall.ovhandlr, obuf);
		put32be(ovcall.ovtabad, obuf);	/* .dc.l ovtab  */
		put16be(ovcall.jmprout, obuf);	/* jmp  routine */
		put32be(ovcall.routaddr, obuf);

		textbase += SIZEOF_OVCALBLK;	/* bump for block size  */

		if (saverbits)
		{
			put16be(INSABS, rtbuf);		/* jsr          */
			put16be(LUPPER, rtbuf);		/* address of ovhandler */
			put16be(TRELOC, rtbuf);
			put16be(LUPPER, rtbuf);		/* address of ovtable   */
			put16be(DRELOC, rtbuf);
			put16be(INSABS, rtbuf);		/* jmp          */
			put16be(LUPPER, rtbuf);		/* address of routine   */
			put16be(TRELOC, rtbuf);
		}

		jpt = jpt->nxtjbl;				/* next block in list   */
	}
}


/************************************************************************
 *
 * fixname(trnode) -- put an output file name into ovtab1
 *
 *	The name must be in the proper form for direct BDOS calls.
 *	The filename is padded with blanks, and the type is left-
 *	justified in its field.  There is no punctuation between
 *	the two parts.
 *
 ************************************************************************/
static VOID fixname(P(struct ovtrnode *) opt)
PP(register struct ovtrnode *opt;)
{
	register int i, j;

	for (i = 0; i < TBNAMESIZE; i++)
		if (!isalnum((ovtab1.tbname[i] = opt->ovfname[i])))
			break;						/* found end of name        */
	for (; i < TBNAMESIZE; i++)
		ovtab1.tbname[i] = ' ';			/* blank out rest of name   */
	for (i = 0; i < FNAMELEN; i++)
		if (opt->ovfname[i] == '.')		/* find file type   */
			break;
	i++;								/* skip dot, if there   */
	for (j = 0; j < TBEXTSIZE && i < FNAMELEN; i++, j++)
		if (!isalnum((ovtab1.tbext[j] = opt->ovfname[i])))
			break;						/* copy only letters and digits */
	for (; j < TBEXTSIZE; j++)
		ovtab1.tbext[j] = ' ';			/* blank fill rest of type field */
	ovtab1.tbext[TBEXTSIZE] = '\0';		/* zero for overlay handler */
}


/************************************************************************
 *
 * wrovtab() -- write overlay table to root's data segment
 *
 ************************************************************************/
static VOID wrovtab(NOTHING)
{
	register int i, j;
	register struct ovtrnode *opt;
	register short *pt;

	for (i = 1; i <= numovls; i++)
	{
		opt = ovtree[i];
		fixname(opt);					/* put name in ovtab1       */
		ovtab1.tbldpt = opt->ovtxbase;	/* load point for module */
		pt = (short *)&ovtab1;			/* first word of ovtab1     */
		for (j = 1; j <= 6; j++, pt++)	/* write filename   */
		{
			put16be(*pt, tbuf);
			if (saverbits)
				put16be(DABS, rdbuf);
		}
		put32be(ovtab1.tbldpt, tbuf);	/* write loadpt  */
		if (saverbits)
		{
			put16be(LUPPER, rdbuf);
			put16be(TRELOC, rdbuf);
		}
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
	if (!sflag)							/* no symbol table desired */
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
		fatalx(FALSE, _("unable to reopen file: %s\n"), tfilname);
	while (size > 0)
	{
		j = get16be(pb);				/* Fetch word from source buffer */
		put16be(j, obuf);
		size -= 2;						/* Down by 2 bytes      */
	}
	fclose(pb);
}


/************************************************************************
 *
 * finalwr() -- do the final writting to the output file
 *
 *	copy the initialized data from the temp file to the output file
 *	write the symbol table to the output file
 *
 ************************************************************************/
static VOID finalwr(NOTHING)
{
	if (ovflag && !chnflg)
		wrjumps();
	if ((textsize + textstart) != textbase)
	{
		errorx(_("text size error in %s\n"), outfname);
	}
	if (ovflag && !chnflg && (ovpath[ovpathtp] == ROOT))
		wrovtab();
	/* copy initialized data segment to output file */
	cpdata(&tbuf, dafnc, datasize);
	/* write the symbol table */
	osymt();
	if (saverbits)
	{
		/* copy text relocations to output file */
		cpdata(&rtbuf, rtfnc, textsize);
		/* copy data relocations to output file */
		cpdata(&rdbuf, rdfnc, datasize);
	}
	fflush(obuf);
#if 0
	if (stacksize)
	{
		bsssize += stacksize;
		fseek(obuf, 10L, SEEK_SET);
		put32be(bsssize, obuf);
	}
#endif
	/* update size of symbols table in header */
	fseek(obuf, 14L, SEEK_SET);
	put32be(stlen, obuf);
	if (ferror(obuf))
	{
		errorx(_("write error on file: %s\n"), outfname);
	}
	endit(exstat);
}


/************************************************************************
 *
 * dopass2(*ovnode) -- do pass two processing on a given node
 *
 *	Drives the pass 2 routines.  If it is processing the root,
 *	the symbol table is also written to the output file if
 *	specified.  This routine depends on the root being the first
 *	module processed in order to do the symbol table right.
 *
 ************************************************************************/
static VOID dopass2(P(struct ovtrnode *) opt)
PP(register struct ovtrnode *opt;)		/* pointer to node in command tree */
{
	register struct filenode *fpt;

	/* set up sizes and bases for building load file */
	if (opt == ovtree[ROOT])
	{
		textsize = rtxsize;
		datasize = rdtsize;
		bsssize = hihiaddr - opt->ovbsbase;	/* for overlay area */
	} else
	{
		textsize = opt->ovdtbase - opt->ovtxbase;
		datasize = opt->ovbsbase - opt->ovdtbase;
		bsssize = opt->ovcap - opt->ovbsbase;
	}
	textstart = opt->ovtxbase;
	textbase = opt->ovtxbase;
	database = opt->ovdtbase;
	bssbase = opt->ovbsbase;

	sflag = ((opt == ovtree[ROOT]) && symflg);	/* only root module    */

	makeofile(opt->ovfname);			/* set up output file for node  */

	fpt = opt->ovflist;					/* get first input file     */

	while (fpt != NULL)					/* go through whole list    */
	{
		if (!(fpt->fnflags & FNSYM))	/* file, not symbol */
			p2load(fpt->fnfname);		/* load the file    */
		fpt = fpt->fnnext;				/* get the next file    */
	}
	/*  has symbol table  */
	finalwr();							/* finish the output file   */
}


/************************************************************************
 *
 * dumpsyms() -- dump symbol table.  Undocumented debugging aid
 *
 ************************************************************************/
static VOID dumpsyms(NOTHING)
{
	struct symtab *p;

	printf("\nDUMP OF INTERNAL SYMBOL TABLE\n");
	printf("BMTE = %lx, LMTE = %lx\n\n", (intptr_t)bmte, (intptr_t)lmte);

	for (p = bmte; p != NULL && p != lmte; p = p->next)
	{
		if (p->name[0] == '\0')
			continue;
		printf("NAME:    %.*s\n", SYNAMLEN, p->name);
		printf("FLAGS:   ");
		if (p->flags & SYDF)
			printf("DEF ");
		if (p->flags & SYEQ)
			printf("EQU ");
		if (p->flags & SYGL)
			printf("GLB ");
		if (p->flags & SYER)
			printf("REG ");
		if (p->flags & SYXR)
			printf("EXT ");
		if (p->flags & SYDA)
			printf("DAT ");
		if (p->flags & SYTX)
			printf("TEX ");
		if (p->flags & SYBS)
			printf("BSS");
		printf("\n");
		printf("VALUE:   %lx\n", p->vl1);
		printf("OVERLAY: %d\n", p->ovlnum);
		printf("INTERNAL ADDRESS = %lx, LINK = %lx\n\n", (intptr_t)p, (intptr_t)p->tlnk);
	}
	dmpflg = FALSE;
}


static int sortmap(P(const VOIDPTR) p1, P(const VOIDPTR) p2)
PP(const VOIDPTR p1;)
PP(const VOIDPTR p2;)
{
	const struct symtab *s1;
	const struct symtab *s2;
	int idx1, idx2;
	
	s1 = *((const struct symtab * const *)p1);
	s2 = *((const struct symtab * const *)p2);
	if (s1->flags & SYTX)
		idx1 = 0;
	else if (s1->flags & SYDA)
		idx1 = 1;
	else if (s1->flags & SYBS)
		idx1 = 2;
	else
		idx1 = 3;
	if (s2->flags & SYTX)
		idx2 = 0;
	else if (s2->flags & SYDA)
		idx2 = 1;
	else if (s2->flags & SYBS)
		idx2 = 2;
	else
		idx2 = 3;
	if (idx1 < idx2)
		return -1;
	if (idx1 > idx2)
		return 1;
	if (s1->vl1 < s2->vl1)
		return -1;
	if (s1->vl1 > s2->vl1)
		return 1;
	return 0;
}


static VOID dumpmap(NOTHING)
{
	register struct symtab *p;
	size_t count;
	register size_t i, j;
	register struct symtab **sorted;
	char c;
	register unsigned short a_type;
	
	for (p = bmte, count = 0; p != NULL && p != lmte; p = p->next)
	{
		if (p->flags & SYXR)			/* external symbol */
			continue;
		if (p->name[0] == '\0')
			continue;
		if ((p->flags & SYGL) == 0 && (p->name[0] == 'L' || Xflag == 0))
			continue;
		count++;
	}
	sorted = (struct symtab **)malloc(count * sizeof(struct symtab *));
	if (sorted == NULL)
		return;
	for (p = bmte, j = 0; p != NULL && p != lmte; p = p->next)
	{
		if (p->flags & SYXR)			/* external symbol */
			continue;
		if (p->name[0] == '\0')
			continue;
		if ((p->flags & SYGL) == 0 && (p->name[0] == 'L' || Xflag == 0))
			continue;
		sorted[j++] = p;
	}
	qsort(sorted, j, sizeof(struct symtab *), sortmap);
	
	for (i = 0; i < j; i++)
	{
		p = sorted[i];
		a_type = p->flags;
		if (a_type == A_TFILE)
		{
			c = 'f';
		} else
		{
			int ext = (a_type & A_GLOBL) != 0;
			c = '?';
			if ((a_type & A_GLOBL) != 0 || ((a_type & A_DEF) && (a_type & A_EXT)))
			{
				c = 'c';
				a_type &= ~A_GLOBL;
			}
			if ((a_type & A_GLOBL) != 0 && !(a_type & A_DEF) && (a_type & A_EXT) && p->vl1 != 0)
				c = 'C';
			else if (a_type == A_UNDF)
				c = 'U';
			else if ((a_type & A_EQU) != 0)
				c = 'A';
			else if ((a_type & A_EQREG) != 0)
				c = 'R';
			else if ((a_type & A_TEXT) != 0)
				c = 'T';
			else if ((a_type & A_DATA) != 0)
				c = 'D';
			else if ((a_type & A_BSS) != 0)
				c = 'B';
			if (!ext)
				c |= 0x20;
		}
		printf("%-*.*s %c %08lx", SYNAMLEN, SYNAMLEN, p->name, c, p->vl1);
#if 0
		if (p->flags & SYDF)
			printf(" DEF");
		if (p->flags & SYEQ)
			printf(" EQU");
		if (p->flags & SYGL)
			printf(" GLB");
		if (p->flags & SYER)
			printf(" REG");
		if (p->flags & SYXR)
			printf(" EXT");
		if (p->flags & SYDA)
			printf(" DAT");
		if (p->flags & SYTX)
			printf(" TEX");
		if (p->flags & SYBS)
			printf(" BSS");
#endif
		printf("\n");
	}
	free(sorted);
}


static VOID buildf(NOTHING)
{
	tfilname[0] = 0;					/* Null out present filename    */
	strcat(tfilname, tdisk);			/* Put disk name in first  */
	strcat(tfilname, tfbase);			/* Put in filename now    */

#ifdef HAVE_MKTEMP						/* On UNIX,           */
	mktemp(tfilname);					/* Make a temp filename   */
#endif

	tfchar = &tfilname[(int)strlen(tfilname) - 1];	/* Set pointer  */
}


VOID endit(P(int) stat)
PP(int stat;)
{
	if (stat == 0)
		fclose(obuf);
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
	if (dmpflg && stat >= 0)
		dumpsyms();
	if (mapflg && stat >= 0)
		dumpmap();
	if (stat != 0)
		exit(EXIT_FAILURE);
}


/************************************************************************
 *
 * main() -- driver routine for entire linker.
 *
 ************************************************************************/
int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register int i;
	struct symtab *pt;

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (argc == 1)						/* no command line  */
	{
		banner();						/* print the sign-on message    */
		usage();
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++)			/* assemble command line */
	{
		if ((strlen(cmdline) + strlen(argv[i])) > (LINELEN - 2))
		{
			fatalx(FALSE, _("command line too long\n"));
		} else
		{
			strcat(cmdline, argv[i]);	/* put in the command */
			strcat(cmdline, " ");		/* leave blank between */
		}
	}

#if 0
	printf("%s\n", cmdline);			/* echo command line    */
#endif

	preproc();							/* build command tree   */
	ovflag = (numovls > 0);				/* are there overlays?  */
	buildf();							/* construct temp. names */

	/* Open and then close a file in ibuf to allocate its buffer    */
	/* area under the symbol table.  Otherwise the buffer ends up   */
	/* on top of any additions to the symbol table.         */

	openfile((ovtree[ROOT]->ovflist)->fnfname);	/* first input file */
	if (ibuf)
		fclose(ibuf);					/* will open again later */
	intsytab();							/* init. symbol table   */
	firstsym = fsymp;
	libfctr = lbfictr;
	libptr = lbfioff;
	saverbits = !absflg;				/* keep relocation bits */
	if (ovflag && !chnflg)				/* put _ovhdlr in symbol */
	{									/*  table       */
		pack(ovhstr, lmte->name);
		lmte->flags = SYXR;				/* make it external */
		lmte->ovlnum = ROOT;			/* belongs to root  */
		addsym();
	}
	walktree(ROOT, dopass1, POSTORDER);	/* pass1 on each node   */
	if (ovflag && !chnflg)
	{									/* add room for overlay table in root's data segment */
		ovtable = ovtree[ROOT]->ovbsbase;	/* changes in ovexts  */
		ovtree[ROOT]->ovbsbase += numovls * SIZEOF_OVTAB;
		ovtree[ROOT]->ovcap += numovls * SIZEOF_OVTAB;
		rdtsize += numovls * SIZEOF_OVTAB;
	}
	fixcoms();							/* allocate common and global static space */
	hihiaddr = ovtree[ROOT]->ovcap;		/* current top of bss   */
	if (ovflag)							/* resolve calls to overlayed routines  */
		walktree(ROOT, ovexts, PREORDER);
	resolve();							/* resolve externals, assign addresses  */
	if (exstat != 0)					/* quit if there are errors     */
		endit(EXIT_FAILURE);

	pass2++;							/* now in pass two          */
	firstsym = fsymp;
	libfctr = lbfictr;
	libptr = lbfioff;
	Xflag = TRUE;						/* print all locals entered by pass 1   */
	if (ovflag && !chnflg)				/* set up template overlay call block   */
	{
		pack(ovhstr, lmte->name);		/* get overlay handler address  */
		pt = lemt(girt);
		ovcall.ovhandlr = pt->vl1;
		ovcall.jsrovh = JSRL;
		ovcall.jmprout = JMPL;
	}
	walktree(ROOT, dopass2, POSTORDER);	/* pass2 on each node   */

	return exstat == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
