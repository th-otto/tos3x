/*
    Copyright 1981
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "as68.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


char ldfn[40];          /* name of the relocatable object file */
extern char tlab1[SYNAMLEN];
short ftudp;
int poslab;


int astring PROTO((NOTHING));
int astr1 PROTO((int adelim));
VOID oconst PROTO((int ardx));
int constant PROTO((long *pnum, char *pstr, int idx));
short hash PROTO((NOTHING));
VOID doitwr PROTO((NOTHING));



/* output it for beginning of statement */
VOID opitb(NOTHING)
{
	stbuf[0].itty = ITBS;				/* beginning of statement */
	stbuf[0].itop.l = (fchr != EOLC) ? absln : absln - 1;
	stbuf[1].itty = ITSY;				/* label entry */
	stbuf[1].itop.ptrw2 = lblpt;		/* pointer to symbol or 0 */

	/* put opcode in it buffer */
	stbuf[2].itty = ITSY;
	stbuf[2].itrl = (char)modelen;			/* mode of instr(byte, word, long) */
	stbuf[2].itop.ptrw2 = opcpt;		/* pointer to opcode in main table */
	stbuf[3].itty = ITCN;
	stbuf[3].itrl = (char)rlflg;				/* relocation base */
	stbuf[3].itop.l = loctr;			/* pass1 location counter */
	itwc = ITOP1;						/* next available slot-currently 4 */
	pitw = &stbuf[ITOP1];				/* init the pointer */
}


/*
 *  get an input term  (symbol, constant, or special character)
 *  call with:
 *      the first character in fchr
 *  returns:
 *      item type in itype
 *      item value in ival if item is a constant or special character
 *      if it is a symbol it is placed at the end of the main table
 *
 * meaning of state table:
 * 
 * currently getting:       symbol(0)   constant(1)     beginning(2)
 *
 * next char:
 * digit(0)                 0           1               1
 *
 * letter(3)                0           3               0
 * 
 * special char(6)          3           3               3
 *
 * contents of the state table is the next state.  processing stops when
 * state 3 is encountered.  state 2 is the beginning state.
 */
int const sttbl[] = { 0, 1, 1, 0, 3, 0, 3, 3, 3 };	/* state table for parser */

VOID gterm(P(int) constpc)
PP(int constpc;)
{
	register int smode, i;
	register char *p;
	register int tmode;
	register struct symtab *j;
	long num;
	char istr[80];

	if (fchr == '\'' || fchr == '"')
		if (astring())					/* maybe ascii string */
			return;
	smode = 2;							/* beginning state */
	i = 0;
	p = istr;

	/* loop to put item on istr */
	while (fchr >= ' ')
	{									/* until a control char */
		if (smode == 2 && fchr == '.')
			tmode = 3;
		else if (isalpha(fchr) || fchr == '~' || fchr == '_' || (fchr == '$' && i))
			tmode = 3;
		else if (isdigit(fchr))
			tmode = 0;
		else
			tmode = 6;
		tmode = sttbl[tmode + smode];	/* new state */
		if (tmode == 3)
			break;						/* end of item */
		smode = tmode;
		*p++ = (char)fchr;					/* save character */
		i++;
		fchr = gchr();
	}
	/* end of item */

	switch (smode)
	{
	case 0:								/* symbol */
		*p = '\0';						/* end of symbol */
		itype = ITSY;					/* symbol */
		pack(istr, lmte);				/* put symbol at end of main table */
		j = lemt(FALSE, sirt);
		if (istr[0] != '~' && !poslab && (j->flags & (SYEQ | SYER)) == SYEQ)
		{
			itype = (j->flags & SYRM) ? ITRM : ITCN;
			ival.l = j->vl1.l;
			reloc = ((j->flags) & SYRO) ? TEXT : ((j->flags) & SYRA) ? DATA : ((j->flags) & SYBS) ? BSS : ABS;
		}
		return;

	case 1:								/* constant */
		if (!constant(&num, istr, i))
		{
			uerr(17);					/* illegal constant */
			num = 0;
		}
		ival.l = num;
		itype = ITCN;
		reloc = ABS;
		return;

	case 2:								/* just a special char */
		switch (fchr)
		{
		case '*':						/* location counter */
			if (starmul)
			{							/* multiply */
				starmul = 0;
				goto specsy;
			}
			refpc++;					/* referenced pgm ctr */
			reloc = rlflg;				/* relocation of location counter */
			ival.l = loctr;
			itype = (constpc) ? ITCN : ITPC;
			break;

		case '$':						/* hex constant */
			oconst(16);
			return;

		case '@':						/* octal const */
			oconst(8);
			return;

		case '%':						/* binary const */
			oconst(2);
			return;

		case '#':
			immed[opdix]++;
			goto specsy;

		case '(':
			indir[opdix]++;
			plevel++;
			goto specsy;

		case ')':
			plevel--;
			goto specsy;

		default:
		  specsy:
			itype = ITSP;				/* return special char */
			ival.l = fchr;
		}
		if (fchr != EOLC)
			fchr = gchr();				/* get next char */
		if ((ival.l == '>' && fchr == '>') || (ival.l == '<' && fchr == '<'))
			fchr = gchr();				/* shift op, ignore second char */
		return;

	default:
		asabort();						/* not possible */
	}
}


/* astring - check for an ascii string enclosed in single quotes */
int astring(NOTHING)
{
	register char delim;

	if (fchr != '\'' && fchr != '"')	/* valid delimiter */
		return FALSE;
	delim = (char)fchr;
	if (equflg || (itype == ITSP && ival.u.loword == '#'))
	{									/* immediate operand */
		if (astr1(delim))
		{
			fchr = gchr();
			if (fchr != delim)
				xerr(19);
			fchr = gchr();
		}
		return equflg ? TRUE : FALSE;
	}
	while (astr1(delim))
	{
		itype = ITSP;
		ival.l = ',';						/* separate by commas */
		reloc = ABS;
		opitoo();
	}
	return FALSE;
}


int astr1(P(int) adelim)
PP(int adelim;)
{
	register int delim, i, retv;
	register long l;

	delim = adelim;
	i = 0;
	l = 0;
	retv = TRUE;
	while ((fchr = gchr()) != CEOF)
	{
		if (fchr == delim)
		{
			fchr = gchr();
			if (fchr != delim)
			{
				retv = FALSE;				/* end of string */
				break;
			}
		}
		if (fchr == EOLC)
		{
			xerr(19);
			retv = FALSE;					/* end of string */
			break;
		}
		l = (l << 8) | fchr;
		if (++i >= modelen)
		{
			if ((fchr = gchr()) == delim)
			{
				fchr = gchr();
				retv = FALSE;				/* end of string */
			} else
			{
				peekc = (char)fchr;			/* next char in string */
			}
			break;						/* filled one bucket */
		}
	}
	while (i < modelen)
	{
		l <<= 8;
		i++;
	}
	itype = ITCN;
	ival.l = l;
	reloc = ABS;
	if (!equflg)
		opitoo();						/* output one operand */
	return retv;
}


/* get constant given radix */
VOID oconst(P(int) ardx)
PP(int ardx;)
{
	register short trdx, j;
	register long i;

	switch (ardx)
	{									/* radix as power of 2 */
	case 16:
		trdx = 4;
		break;
	case 8:
		trdx = 3;
		break;
	case 2:
		trdx = 1;
		break;
	default:
		rpterr("invalid radix in oconst");
		asabort();
	}
	i = 0;
	while (1)
	{
		fchr = gchr();
		j = fchr;
		if (isdigit(j))
			j -= '0';
		else if ((j = tolower(j)) >= 'a' && j <= 'f')
			j = j - 'a' + 10;
		else
			break;						/* not valid numeric char */
		if (j >= 0 && j < ardx)
			i = (i << trdx) + j;
		else
			break;
	}
	ival.l = i;
	itype = ITCN;
	reloc = ABS;
}


/* convert ascii constant to binary */
int constant(P(long *) pnum, P(char *) pstr, P(int) idx)
PP(long *pnum;)
PP(register char *pstr;)
PP(int idx;)
{
	register short i, j;
	register long l;

	l = 0;
	for (i = 0; i < idx; i++)
	{
		j = *pstr++;
		if (isdigit(j))
			j -= '0';
		if (j < 0 || j >= 10)
			return FALSE;
		l = (l << 3) + (l << 1) + j;	/* l = l*10 + j */
	}
	*pnum = l;
	return TRUE;
}


/*
 * method for looking up entries in the main table
 *
 * Note:    The entry to be looked up must be placed at the end
 *          of the main table.  The global cell 'lmte'(last main
 *          entry) points to the next available entry in the main
 *          table.  The address of an initial reference table must
 *          also be provided.
 *
 *  1)  Compute the hash code for the symbol and add it to the base address
 *      of the initial reference table given as input.  Thus, two words are
 *      accessed which define the chain on which the symbol must be if it
 *      is in the table at all.
 *
 *  2)  Alter the table link of the last symbol in the chain so that it
 *      points to the symbol being looked up.  Note that the symbol to be 
 *      looked up is always placed at the end of the main table before
 *      calling the lookup routine.  This essentially adds one more element
 *      to the end of the chain, namely the symbol to be looked up.
 *
 *  3)  Now start at the first symbol in the chain and follow the chain
 *      looking for a symbol equal to the symbol being looked up.  It is
 *      guaranteed that such a symbol will be found because it is always
 *      the last symbol on the chain.
 *
 *  4)  When the symbol is found, check to see if it is the last symbol
 *      on the chain.  If not, the symbol being looked for is in the table
 *      and has been found.  If it is the last symbol, the symbol being
 *      looked up is not in the table.
 *
 *  5)  In the case the looked up symbol is not found, it is usually added
 *      to the end of the table.  This is done simply b changing the
 *      initial reference table entry which points to the previous
 *      last symbol on the chain so that is now points to the symbol at the
 *      end of the main table.  In case the symbol just looked up is not to
 *      be added to the main table then no action is needed .  This means
 *      that the table link of the last symbol on a chain may point any-
 *      where.
 *
 * look up entry in the main table
 *      call with:
 *          address of initial reference table
 *          entry to be looked up at the end of the main table
 *      returns:
 *          a pointer to the entry.  if this pointer is equal to
 *          lmte then the symbol was not previously in the table.
 */
struct symtab *lemt(P(int) oplook, P(struct symtab **) airt)
PP(int oplook;)								/* if true then looking in opcode table */
PP(struct symtab **airt;)
{
	register struct symtab *mtpt;
	register short *p1, *p2, i, j;

	if (oplook)
	{									/* get rid of preceding '.', to lowercase */
		if (lmte->name[0] == '.')
		{
			lmte->name[SYNAMLEN - 1] = '\0';	/* in case of '.' */
			j = 1;
		} else
		{
			j = 0;
		}
		for (i = 0; j < SYNAMLEN; i++, j++)
			lmte->name[i] = tolower(lmte->name[j]); /* WTF? */
	}
	pirt = airt + hash();				/* hashed ptr to irt */
	mtpt = ((struct irts *)pirt)->irfe;					/* pointer to first entry in chain */
	if (!mtpt)							/* empty chain */
		mtpt = lmte;					/* start at end of main table */
	else
		(((struct irts *)pirt)->irle)->tlnk = lmte;		/* last entry in chain is new symbol */
	if ((lmte->name[0] == '~') && (lmte->name[1] != '~') && (lmte->name[1] != '.'))
		return lmte;					/* force local symbols */

	/* loop to locate entry in main table */
  lemtl:
	p1 = (short *)&mtpt->name[0];
	p2 = (short *)&lmte->name[0];
	i = SYNAMLEN / (sizeof *p1);
	while (i)
	{
		if (*p1++ != *p2++)
		{
			mtpt = mtpt->tlnk;			/* go to next entry in chain */
			goto lemtl;
		}
		i--;
	}
	return mtpt;
}


/*
 * compute a hash code for the last entry in the main table
 * returns the hash code
 */
short hash(NOTHING)
{
	register short i, ht1;
	register char *p;

	ht1 = 0;
	p = &lmte->name[0];
	for (i = 0; i < SYNAMLEN; i++)
		ht1 += *p++;
	return ht1 & (SZIRT - 2);			/* make hash code even and between 0 & SZIRT-2 */
}


/*
 * Make an entry in the main table
 * assumes :
 *   entry to be made is pointed at by lmte
 *   pirt points to the correct initial reference table entry.
 */
VOID mmte(NOTHING)
{
	((struct irts *)pirt)->irle = lmte;					/* pointer to last entry in chain */
	if (((struct irts *)pirt)->irfe == 0)				/* first entry in chain */
		((struct irts *)pirt)->irfe = lmte;
	lmte++;								/* bump last main table entry pointer */
	if (lmte >= emte)
	{									/* main table overflow */
		if (sbrk(sizeof(struct symtab) * ICRSZMT) == (VOIDPTR)-1)
		{								/* get more memory */
			rpterr("symbol table overflow\n");
			endit();
		} else
		{
			emte += ICRSZMT;	/* move end of main table */
			cszmt += ICRSZMT;
		}
	}
}


/*
 * make an entry in the main table for a directive
 *  call with:
 *      pointer to string containing directive name
 *      address of routine to handle directive in pass one
 *      address of routine to handle directive in pass two
 */
VOID mdemt(P(const char *) mdstr, P(int) dirnum)
PP(const char *mdstr;)
PP(int dirnum;)
{
	register struct symtab *mdept;

	pack(mdstr, lmte);					/* pack name at end of main table */
	mdept = lemt(TRUE, oirt);			/* look up in opcode table */
	if (mdept != lmte)
	{									/* best not be there already */
		uerr(5);
		asabort();
		return;
	}
	mmte();								/* make main table entry */
	mdept->flags |= OPDR | SYIN;		/* directive */
	mdept->vl1.l = dirnum;				/* directive # */
}


/*
 * pack a string into an entry in the main table
 *  call with:
 *      pointer to the string
 *      pointer to desired entry in the main table
 */
VOID pack(P(const char *) apkstr, P(struct symtab *) apkptr)
PP(const char *apkstr;)
PP(struct symtab *apkptr;)
{
	register short i;
	register const char *pkstr;
	register char *pkptr;

	pkstr = apkstr;
	pkptr = apkptr->name;
	i = SYNAMLEN;
	while (*pkstr && i)
	{
		*pkptr++ = *pkstr++;
		i--;
	}
	while (i--)
		*pkptr++ = '\0';				/* pad with zeroes */
}


int xcol = 0;							/* Column number            */
int spcnt = 0;							/* Fill counter             */


/* function to get characters from source file */
int gchr(NOTHING)
{
	register short chr1;

	if (peekc)
	{
		chr1 = peekc;
		peekc = 0;
		if (chr1 != SOH)
			xcol--;
	} else if (spcnt)
	{
		spcnt--;
		return ' ';
	} else
	{
	  gchr1:
		if (sbuflen <= 0)
		{								/* nothing on input buffer */
			sbuflen = read(ifn, sbuf, BLEN);	/* read in source */
			if (sbuflen <= 0)
				return CEOF;			/* end of file */
			psbuf = sbuf;
		}
		chr1 = *psbuf++;
		sbuflen--;
	}
	if (chr1 == SOH)					/* preprocessor flag */
		goto gchr1;						/* ignore it */
	if (chr1 == EOLC)
	{									/* end of line */
		xcol = -1;						/* Init column counter  */
		if (!p2flg)						/* pass 1 only */
			absln++;
	} else if (chr1 == '\t')
	{									/* convert tabs to spaces */
		spcnt += 7 - (xcol & 7);		/* Set fill count           */
		xcol += spcnt;					/* Adjust column number     */
		chr1 = ' ';
	}
	xcol++;								/* Bump column number       */
	return chr1;
}


/*
 * write out intermediate text for one statement
 *  call with
 *      the it for the statement in stbuf
 */
VOID wostb(NOTHING)
{
	register short woix, *itwo, i;

	if (stbuf[0].itty != ITBS)
		asabort();						/* not beginning of stmt */
	itwo = (short *)&stbuf;
	woix = stbuf[0].itrl & 0xff;		/* unsigned byte */
	while (woix--)
	{
		for (i = 0; i < (unsigned)(sizeof(struct it) / (sizeof *itwo)); i++)
		{
			if (pitix > &itbuf[ITBSZ - 1])	/* no room in buffer */
				doitwr();
			*pitix++ = *itwo++;			/* first word */
		}
	}
}


VOID doitwr(NOTHING)
{
	register short i;

	if (write(itfn, itbuf, ITBSZ * sizeof(itbuf[0])) != ITBSZ * sizeof(itbuf[0]))
	{
		rpterr("it write error errno=%o\n", errno);
		endit();
	}
	UNUSED(i);
	pitix = itbuf;
}


/*
 * user source error
 *  call with:
 *      number to indicate reason for error
 *  types the error number and the line number on which
 *  the error occured.
 */
VOID uerr(P(int) errn)
PP(int errn;)
{
	if (p2flg)
	{									/* pass 2 gets two ampersands */
		in_err++;
		fprintf(stderr, "&& %d: %s\n", p2absln, ermsg[errn - 1]);
	} else
	{
		fprintf(stderr, "& %d: %s\n", (fchr == EOLC) ? absln - 1 : absln, ermsg[errn - 1]);
	}
	nerror++;
}


/*
 * user error that causes the statement to be abandoned
 *  call with:
 *      error number
 */
VOID xerr(P(int) xern)
PP(int xern;)
{
	uerr(xern);							/* type error message */
	if (!p2flg)							/* pass one */
		igrst();						/* pass rest of source */
}


/* abort the assembly */
VOID asabort(NOTHING)
{
	rpterr("as68 abort\n");
	endit();
}


/* ignore rest of statement */
VOID igrst(NOTHING)
{
	while (fchr != EOLC && fchr != CEOF)	/* until end of line */
		fchr = gchr();
	while ((fchr = gchr()) == EOLC) ;	/* ignore empty lines */
}


/* ignore blanks after a label */
VOID ligblk(NOTHING)
{
	if (fchr == CEOF)
		return;
	igblk();
	if (fchr == EOLC)
	{
		fchr = gchr();
		ligblk();
	}
}


VOID rubout(NOTHING)
{
	nerror = -1;
	endit();
}


/* exit from the assembler */
VOID endit(NOTHING)
{
	LASTCHTFN = itfnc;
	unlink(tfilname);					/* delete temporary files */
	LASTCHTFN = trbfnc;
	unlink(tfilname);
	LASTCHTFN = dafnc;
	unlink(tfilname);
	LASTCHTFN = drbfnc;
	unlink(tfilname);
	if (nerror != -1)
	{									/* not rubout */
		if (ftudp)
			putchar('\n');
	}
	if (nerror > 0)
	{
		fprintf(stderr, "& %d errors\n", nerror);
	}
	if (initflg)
		unlink(ldfn);					/* get rid of empty .o file */
	exit(nerror != 0);
}


/*
 * open files
 *  call with:
 *      pointer to name of file to open
 *      flag for how to open
 *          0 => read
 *          1 => write
 *		Flag for file type
 *			0 => ASCII
 *			1 => Binary
 */
int openfi(P(const char *) pname, P(int) hflag, P(int) file)
PP(const char *pname;)
PP(int hflag;)
PP(int file;)
{
	register short fd;

	fd = (hflag) ? creat(pname, 0666, file) : open(pname, hflag, file);
	if (fd < 0)
	{									/* open failed */
		rpterr("can't open %s errno=%o\n", pname, errno);
		endit();
	}
	return fd;
}


/* get a temp file for the intermediate text */
int gettempf(NOTHING)
{
	register short j;

	while (LASTCHTFN < 'z')
	{
		LASTCHTFN++;
#ifdef __ALCYON__
		/* BUG: missing 3rd arg for creat */
		if ((j = creat(tfilname, 0600)) >= 0)
#else
		if ((j = creat(tfilname, 0600, 1)) >= 0)
#endif
			return j;
	}
	rpterr("temp file create error: %s errno=%o\n", tfilname, errno);
	endit();
}


/* move label name from lbt to main table entry pointed to by lmte */
VOID setname(NOTHING)
{
	register short *p1, *p2;

	p1 = (short *)&lmte->name[0];
	for (p2 = (short *)&lbt[0]; p2 < (short *)(&lbt[SYNAMLEN]);)
	{
		*p1++ = *p2;
		*p2++ = 0;
	}
}


/*
 * get the initialized main table and initial reference tables from
 * the initialize file
 */
VOID getsymtab(NOTHING)
{
	register char **p;
	register struct symtab *p1;
	register char *p2;
	register short fd, i;
	int j;

#ifdef __ALCYON__
	/* BUG: missing 3rd argument for open */
	if ((fd = open(initfnam, O_RDONLY)) < 0)
#else
	if ((fd = open(initfnam, O_RDONLY, 1)) < 0)
#endif
	{
	  rerr:
		rpterr("& Unable to read init file: %s\n", initfnam);
		endit();
	}
	if (read(fd, sirt, SZIRT * sizeof(struct symtab *)) != SZIRT * sizeof(struct symtab *))
	{
		goto rerr;
	}
	
	if (read(fd, oirt, SZIRT * sizeof(struct symtab *)) != SZIRT * sizeof(struct symtab *))
		goto rerr;

	if (read(fd, &j, sizeof(j)) != sizeof(j))
		goto rerr;
	if ((i = read(fd, bmte, j)) != j)
		goto rerr;

	if ((i % (unsigned)sizeof(struct symtab)) != 0)
		goto rerr;

	lmte = (struct symtab *)((char *)bmte + i);
	p2 = ((char *)bmte - 1);
	for (p = (char **)sirt; p < (char **)&sirt[SZIRT]; p++)
	{
		if (*p)
			*p += (__intptr_t)p2;
	}
	for (p = (char **)oirt; p < (char **)&oirt[SZIRT]; p++)
	{
		if (*p)
			*p += (__intptr_t)p2;
	}
	for (p1 = bmte; p1 < lmte; p1++)
	{
		if (p1->tlnk)
#ifdef __ALCYON__
			(char *)p1->tlnk += (__intptr_t)p2;
#else
			p1->tlnk = (struct symtab *)((char *)p1->tlnk + (__intptr_t)p2);
#endif
	}
	close(fd);
}


/* write the initialization file */
VOID putsymtab(NOTHING)
{
	register char **p;
	register struct symtab *p1;
	register char *p2;
	register short fd, i;
	short j;

#ifdef __ALCYON__
	/* BUG: missing 3rd argument for open */
	if ((fd = creat(initfnam, 0644)) < 0)
#else
	if ((fd = creat(initfnam, 0644, 1)) < 0)
#endif
	{
	  werr:
		printf("& Write error on init file: %s\n", initfnam);
		return;
	}
	/*
	 * change all pointers so that they are relative to the beginning
	 * of the symbol table
	 */
	p2 = ((char *)bmte - 1);
	for (p = (char **)sirt; p < (char **)&sirt[SZIRT]; p++)
	{
		if (*p)
			*p = *p - (__intptr_t)p2;
	}
	for (p = (char **)oirt; p < (char **)&oirt[SZIRT]; p++)
	{
		if (*p)
			*p = *p - (__intptr_t)p2;
	}
	for (p1 = bmte; p1 < lmte; p1++)
	{
		if (p1->tlnk)
			p1->tlnk = (struct symtab *)((char *)p1->tlnk - (__intptr_t)p2);
	}

	if (write(fd, sirt, SZIRT * sizeof(struct symtab *)) != SZIRT * sizeof(struct symtab *))
	{
		goto werr;
	}

	if (write(fd, oirt, SZIRT * sizeof(struct symtab *)) != SZIRT * sizeof(struct symtab *))
		goto werr;

	i = (__intptr_t) lmte - (__intptr_t)bmte;			/* length of current main table */
	if ((i % (unsigned)sizeof(struct symtab)) != 0)
	{
		goto werr;
	}
	j = i;								/* Put in memory                */
	if (write(fd, &j, sizeof(j)) != sizeof(j))
		goto werr;
	if (write(fd, bmte, i) != i)
		goto werr;
	close(fd);
}


/*
 * print an error on stderr
 * used for errors with disasterous consequences
 */
#ifdef __ALCYON__
VOID rpterr(ptch, x1, x2, x3, x4, x5, x6)
const char *ptch;
int x1, x2, x3, x4, x5, x6;
{
	fprintf(stderr, "& %d: ", absln);
	fprintf(stderr, ptch, x1, x2, x3, x4, x5, x6);
	nerror++;
}
#else
VOID rpterr(P(const char *) ptch _va_alist)
PP(const char *ptch;)
_va_dcl
{
	va_list args;
	
	va_start(args, ptch);
	fprintf(stderr, "& %d: ", absln);
	vfprintf(stderr, ptch, args);
	va_end(args);
	nerror++;
}
#endif


/* set the file name for the relocatable object file (sourcefile.o) */
VOID setldfn(P(const char *) ap)
PP(const char *ap;)
{
	register const char *p1;
	register char *p2;

	p1 = ap;
	p2 = ldfn;
	while (*p1)
	{
		*p2++ = *p1++;
	}
	if (*(p2 - 2) != '.')
	{									/* not name.? */
		*p2++ = '.';
		*p2++ = 'o';
	} else
	{									/* is name.? */
		*(p2 - 1) = 'o';
	}
	*p2 = '\0';
}
