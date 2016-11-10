/*
    Copyright 1981
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "as68.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int poslab;

static int xcol = 0;							/* Column number            */
static int spcnt = 0;							/* Fill counter             */


static const char *const ermsg[] = {
    N_("label %.8s redefined"),             /*  1 */
    N_("invalid label"),                    /*  2 */
    N_("invalid opcode"),                   /*  3 */
    N_("no label for operand"),             /*  4 */
    N_("opcode %s redefined"),              /*  5 */
    N_("illegal expr"),                     /*  6 */
    N_("undefined symbol in equate"),       /*  7 */
    N_("opcode for 68010 only"),			/*  8 */
    N_("invalid first operand"),            /*  9 */
    N_("invalid second operand"),           /* 10 */
    N_("absolute value required"),          /* 11 */
    N_("no code or data allowed in offset"),/* 12 */
    N_("undefined symbol"),                 /* 13 */
    N_("illegal index register"),           /* 14 */
    N_("illegal constant"),                 /* 15 */
    N_("illegal extension"),                /* 16 */
    N_("constant required"),                /* 17 */
    N_("illegal format"),                   /* 18 */
    N_("illegal string"),                   /* 19 */
    N_("illegal addressing mode"),          /* 20 */
    N_("assembler confusion..."),			/* 21 */
    N_("illegal relative address"),         /* 22 */
    N_("invalid bit range"),                /* 23 */
    N_("illegal text delimiter"),           /* 24 */
    N_("unexpected endc"),                  /* 25 */
    N_("endc expected"),                    /* 26 */
    N_("relocation error"),                 /* 27 */
    N_("symbol required"),                  /* 28 */
    N_("bad use of symbol"),                /* 29 */
    N_("invalid data list"),                /* 30 */
    N_("warning: cmpm generated for 68010"),/* 31 */
    N_("missing )"),                        /* 32 */
    N_("register required"),                /* 33 */
    N_("illegal size"),                     /* 34 */
    N_("illegal 8-bit displacement"),       /* 35 */
    N_("illegal external"),                 /* 36 */
    N_("illegal shift count"),              /* 37 */
    N_("invalid instruction length"),       /* 38 */
    N_("code or data not allowed in bss"),  /* 39 */
    N_("backward assignment to *"),         /* 40 */
    N_("illegal 16-bit displacement"),      /* 41 */
    N_("illegal 16-bit immediate"),         /* 42 */
    N_("illegal 8-bit immediate"),          /* 43 */
    N_("warning: constant out of range"),   /* 44 */
    N_("illegal register list"),            /* 45 */
};

static const char *const itbtname[] = { "ITBS", "ITSY", "ITCN", "ITSP", "ITRM", "ITPC", "ITCW" };


/* output it for beginning of statement */
VOID opitb(NOTHING)
{
	stbuf[0].itty = ITBS;				/* beginning of statement */
	stbuf[0].itop.l = (fchr != EOLC) ? absln : absln - 1;
	stbuf[1].itty = ITSY;				/* label entry */
	stbuf[1].itop.ptrw2 = lblpt;		/* pointer to symbol or 0 */

	/* put opcode in it buffer */
	stbuf[2].itty = ITSY;
	stbuf[2].itrl = modelen;			/* mode of instr(byte, word, long) */
	stbuf[2].itop.ptrw2 = opcpt;		/* pointer to opcode in main table */
	stbuf[3].itty = ITCN;
	stbuf[3].itrl = rlflg;				/* relocation base */
	stbuf[3].itop.l = loctr;			/* pass1 location counter */
	itwc = ITOP1;						/* next available slot-currently 4 */
	pitw = &stbuf[ITOP1];				/* init the pointer */
}


static int astr1(P(int) adelim)
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
			xerr(19); /* illegal format */
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
				peekc = fchr;			/* next char in string */
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


/* astring - check for an ascii string enclosed in single quotes */
static int astring(NOTHING)
{
	register char delim;

	if (fchr != '\'' && fchr != '"')	/* valid delimiter */
		return FALSE;
	delim = fchr;
	if (equflg || (itype == ITSP && ival.oper == '#'))
	{									/* immediate operand */
		if (astr1(delim))
		{
			fchr = gchr();
			if (fchr != delim)
				xerr(19); /* illegal format */
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


/* get constant given radix */
static VOID oconst(P(int) ardx)
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
		rpterr(_("invalid radix in oconst"));
		asabort();
	}
	i = 0;
	for (;;)
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
static int constant(P(long *) pnum, P(char *) pstr, P(int) idx)
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
static int const sttbl[] = { 0, 1, 1, 0, 3, 0, 3, 3, 3 };	/* state table for parser */

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
		*p++ = fchr;					/* save character */
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
			ival.l = j->vl1;
			reloc = (j->flags & SYRO) ? TEXT : (j->flags & SYRA) ? DATA : (j->flags & SYBS) ? BSS : ABS;
		}
		break;

	case 1:								/* constant */
		if (!constant(&num, istr, i))
		{
			uerr(17);					/* illegal constant */
			num = 0;
		}
		ival.l = num;
		itype = ITCN;
		reloc = ABS;
		break;

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
			itype = constpc ? ITCN : ITPC;
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
		break;

	default:
		rpterr(_("internal: invalid smode %d"), smode);
		asabort();						/* not possible */
		break;
	}
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

/*
 * compute a hash code for the last entry in the main table
 * returns the hash code
 */
static short hash(NOTHING)
{
	register short i, ht1;
	register char *p;

	ht1 = 0;
	p = &lmte->name[0];
	for (i = 0; i < SYNAMLEN; i++)
		ht1 += *p++;
	return ht1 & (SZIRT - 1);			/* make hash code even and between 0 & SZIRT-2 */
}


struct symtab *lemt(P(int) oplook, P(struct irts *) airt)
PP(int oplook;)								/* if true then looking in opcode table */
PP(struct irts *airt;)
{
	register struct symtab *mtpt;
	register char *p1, *p2;
	register short i, j;

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
			lmte->name[i] = tolower(lmte->name[j]);
	}
	pirt = airt + hash();				/* hashed ptr to irt */
	mtpt = pirt->irfe;					/* pointer to first entry in chain */
	if (!mtpt)							/* empty chain */
		mtpt = lmte;					/* start at end of main table */
	else
		pirt->irle->tlnk = lmte;		/* last entry in chain is new symbol */
	if (lmte->name[0] == '~' && lmte->name[1] != '~' && lmte->name[1] != '.')
		return lmte;					/* force local symbols */

	/* loop to locate entry in main table */
  lemtl:
	p1 = mtpt->name;
	p2 = lmte->name;
	i = SYNAMLEN;
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


static struct symtab *allocsy(NOTHING)
{
	register struct symtab *ptr;
	
	ptr = malloc(sizeof(*ptr));
	if (ptr == NULL)
	{
		perror(NULL);
		asabort();
	}
	emte = ptr;
	ptr->name[0] = '\0';
	ptr->flags = 0;
	ptr->vl1 = 0;
	ptr->vextno = 0;
	ptr->tlnk = NULL;
	ptr->next = NULL;
	return ptr;
}


/*
 * Make an entry in the main table
 * assumes :
 *   entry to be made is pointed at by lmte
 *   pirt points to the correct initial reference table entry.
 */
VOID mmte(NOTHING)
{
	register struct symtab *ptr;
	
	ptr = lmte;
	pirt->irle = ptr;					/* pointer to last entry in chain */
	if (pirt->irfe == NULL)				/* first entry in chain */
		pirt->irfe = ptr;
	lmte = allocsy();
	ptr->next = lmte;
}


/*
 * make an entry in the main table for a directive
 *  call with:
 *      pointer to string containing directive name
 *      address of routine to handle directive in pass one
 *      address of routine to handle directive in pass two
 */
struct symtab *mdemt(P(const char *) mdstr, P(int) dirnum)
PP(const char *mdstr;)
PP(int dirnum;)
{
	register struct symtab *mdept;

	pack(mdstr, lmte);					/* pack name at end of main table */
	mdept = lemt(TRUE, oirt);			/* look up in opcode table */
	if (mdept != lmte)
	{									/* best not be there already */
		uerr(5, mdstr);	/* opcode redefined */
		asabort();
		return NULL;
	}
	mmte();								/* make main table entry */
	mdept->flags |= OPDR | SYIN;		/* directive */
	mdept->vl1 = dirnum;				/* directive # */
	return mdept;
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


/* function to get characters from source file */
int gchr(NOTHING)
{
	register short chr1;

	if (peekc)
	{
		chr1 = peekc;
		peekc = 0;
		xcol--;
	} else if (spcnt)
	{
		spcnt--;
		return ' ';
	} else
	{
		chr1 = fgetc(ifn);
		if (chr1 == EOF)
			return CEOF;
		if (chr1 == 0x0d)
		{
			chr1 = fgetc(ifn);
			if (chr1 == EOF)
			{
				chr1 = EOLC;
			} else if (chr1 != 0x0a)
			{
				peekc = chr1;
				chr1 = EOLC;
			} else
			{
				chr1 = EOLC;
			}
		}
	}
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


static VOID doitwr(NOTHING)
{
	if (fwrite(itbuf, 1, ITBSZ * sizeof(itbuf[0]), itfn) != ITBSZ * sizeof(itbuf[0]))
	{
		rpterr(_("it write error: %s"), strerror(errno));
		asabort();
	}
	pitix = 0;
}


VOID prits(P(const struct it *) its, P(int) cnt)
PP(register const struct it *its;)
PP(int cnt;)
{
	register int i;

	for (i = 0; i < cnt; i++)
	{
		switch (its[i].itty)
		{
		case ITBS:
			printf("(stmt)");
			break;
		case ITSY:
			printf("%s", its[i].itop.ptrw2 ? its[i].itop.ptrw2->name : "(nil)");
			break;
		case ITCN:
			printf("$%lx", its[i].itop.l);
			break;
		case ITSP:
			printf("%c", (int)its[i].itop.l);
			break;
		case ITRM:
			printf("rm(%lx)", its[i].itop.l);
			break;
		case ITPC:
			printf("*");
			break;
		case ITCW:
			printf("$%lx.w", its[i].itop.l);
			break;
		}
	}
	printf("\n");
}


VOID prstbuf(P(const char *) tag)
PP(const char *tag;)
{
	register int i, cnt;
	
	cnt = stbuf[0].itrl & 0xff;
	printf("%s %3d: ", tag, cnt);
	if (cnt >= ITOP1 && stbuf[0].itty == ITBS && stbuf[1].itty == ITSY && stbuf[2].itty == ITSY && stbuf[3].itty == ITCN)
	{
		printf("    [%08lx:%02x] ", stbuf[3].itop.l, stbuf[1].itrl);
		if (stbuf[1].itop.ptrw2)
			printf("%-*.*s:   ", SYNAMLEN, SYNAMLEN, stbuf[1].itop.ptrw2->name);
		else
			printf("%-*.*s    ", SYNAMLEN, SYNAMLEN, "");
		printf("%s%s    ", stbuf[2].itop.ptrw2->name,
			stbuf[2].itrl == 0 ? "" :
			stbuf[2].itrl == BYTESIZ ? ".b" :
			stbuf[2].itrl == WORDSIZ ? ".w" :
			stbuf[2].itrl == LONGSIZ ? ".l" :
			".?");
		prits(&stbuf[ITOP1], cnt - ITOP1);
	} else
	{
		for (i = 1; i < cnt; i++)
			printf("    %s %02x %08lx", itbtname[stbuf[i].itty & 0xff], stbuf[i].itrl, stbuf[i].itop.l);
		printf("\n");
	}
}


/*
 * write out intermediate text for one statement
 *  call with
 *      the it for the statement in stbuf
 */
VOID wostb(NOTHING)
{
	register short woix, *itwo;
	register unsigned short i;

	if (stbuf[0].itty != ITBS)
	{
		rpterr(_("internal: not at beginning of stmt"));
		asabort();						/* not beginning of stmt */
	}
	itwo = (short *)&stbuf;
	woix = stbuf[0].itrl & 0xff;		/* unsigned byte */
	/* prstbuf("wostb"); */
	while (woix--)
	{
		for (i = 0; i < (sizeof(struct it) / (sizeof *itwo)); i++)
		{
			if (pitix >= ITBSZ)	/* no room in buffer */
				doitwr();
			itbuf[pitix++] = *itwo++;			/* first word */
		}
	}
}


/*
 * user source error
 *  call with:
 *      number to indicate reason for error
 *  types the error number and the line number on which
 *  the error occured.
 */
static VOID verr(P(int) errn, P(va_list) args)
PP(int errn;)
PP(va_list args;)
{
	if (p2flg)
	{									/* pass 2 gets two ampersands */
		in_err++;
		fprintf(stderr, "&& %d: ", p2absln);
	} else
	{
		fprintf(stderr, "& %d: ", fchr == EOLC ? absln - 1 : absln);
	}
	vfprintf(stderr, _(ermsg[errn - 1]), args);
	fputc('\n', stderr);
	nerror++;
}


VOID uerr(P(int) errn _va_alist)
PP(int errn;)
_va_dcl
{
	va_list args;
	
	va_start(args, errn);
	verr(errn, args);
	va_end(args);
}


/*
 * user error that causes the statement to be abandoned
 *  call with:
 *      error number
 */
VOID xerr(P(int) errn _va_alist)
PP(int errn;)
_va_dcl
{
	va_list args;
	
	va_start(args, errn);
	verr(errn, args);					/* type error message */
	va_end(args);
	if (!p2flg)							/* pass one */
		igrst();						/* pass rest of source */
}


/* abort the assembly */
VOID asabort(NOTHING)
{
	rpterr(_("as68 abort"));
	endit();
}


/* ignore rest of statement */
VOID igrst(NOTHING)
{
	while (fchr != EOLC && fchr != CEOF)	/* until end of line */
		fchr = gchr();
	while ((fchr = gchr()) == EOLC)			/* ignore empty lines */
		;
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


/* exit from the assembler */
VOID endit(NOTHING)
{
	if (itfn)
		fclose(itfn);
	if (lfil)
		fclose(lfil);
	if (dafil)
		fclose(dafil);
	if (trfil)
		fclose(trfil);
	if (drfil)
		fclose(drfil);
	if (*itfilnam)
		unlink(itfilnam);					/* delete temporary files */
	if (*trfilnam)
		unlink(trfilnam);
	if (*dafilnam)
		unlink(dafilnam);
	if (*drfilnam)
		unlink(drfilnam);
	if (nerror > 0)
	{
		fprintf(stderr, _("& %d errors\n"), nerror);
	}
	/* get rid of empty .o file */
	if (nerror != 0 && *ldfn)
		unlink(ldfn);
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
FILE *openfi(P(const char *) pname, P(const char*) m)
PP(const char *pname;)
PP(const char *m;)
{
	register FILE *fp;

	fp = fopen(pname, m);
	if (fp == NULL)
	{									/* open failed */
		rpterr(_("can't open %s: %s"), pname, strerror(errno));
		endit();
	}
	return fp;
}


/* move label name from lbt to main table entry pointed to by lmte */
VOID setname(NOTHING)
{
	register char *p1, *p2;

	p1 = &lmte->name[0];
	for (p2 = &lbt[0]; p2 < &lbt[SYNAMLEN];)
	{
		*p1++ = *p2;
		*p2++ = 0;
	}
}


VOID initsy(NOTHING)
{
	register int i;
	
	lmte = bmte = allocsy();

	/* initializing the main table */
	for (i = 0; i < SZIRT; i++)
	{
		/* initialize the initial ref tables */
		sirt[i].irle = (struct symtab *)&sirt[i];
		sirt[i].irfe = NULL;
		oirt[i].irle = (struct symtab *)&oirt[i];
		oirt[i].irfe = NULL;
	}
}


/*
 * print an error on stderr
 * used for errors with disasterous consequences
 */
VOID rpterr(P(const char *) ptch _va_alist)
PP(const char *ptch;)
_va_dcl
{
	va_list args;
	
	va_start(args, ptch);
	fprintf(stderr, "& %d: ", fchr == EOLC ? absln - 1 : absln);
	vfprintf(stderr, ptch, args);
	va_end(args);
	fputc('\n', stderr);
	nerror++;
}


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
