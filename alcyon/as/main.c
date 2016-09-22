/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

/*
 * a two pass relocatable assembler for the Motorola 68000 microprocessor
 *
 *  Bill Allen
 *  Modified by Vicki Hutchison
 *
 *  after any of this assembler is recompiled, it must be initialized
 *  before it will execute properly.  To initialize, become super user
 *  and execute the command:
 *
 *      as68 -i as68init
 *
 *  where as68 is the newly compiled version of the assembler.  With-
 *  out this initialization, the assembler will not run (probably bus
 *  error).
 */

#include "as68.h"
#include "def.h"
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef __ALCYON__
int mkstemp(char *template);
#define mktemp(f) close(mkstemp(f))
#endif




/*
 * Define Temporary and Init file names per O/S.  We use code here to
 * allow re-directing temp files and the init file via command line
 * switches.
 */

static const char *tdname = "";				/*  Temp files in same directory    */
static char const tfilebase[] = "a6XXXXXX";		/*  Temp file  basename         */

static int symcon;
static int explmode;   /* explicit mode length given */
static int opcval;     /* opcode */
static int chmvq;

short indir[2];
short immed[2];
static short numcon[2];
static short numsym[2];
static short numreg[2];


/* get a temp file for the intermediate text */
static FILE *gettempf(P(char *) filnam)
PP(char *filnam;)
{
	FILE *fp;
	
	/* Build temp file names */
	strcpy(filnam, tdname);
	strcat(filnam, tfilebase);
	/* Make it unique */
	mktemp(filnam);
	
	fp = fopen(filnam, "w+b");
	if (fp != NULL)
		return fp;
	rpterr(_("temp file create error: %s %s"), filnam, strerror(errno));
	endit();
}


static VOID rubout(P(int) sig)
PP(int sig;)
{
	UNUSED(sig);
	nerror = -1;
	endit();
}


static VOID mystrncpy(P(char *) astr1, P(const char *) astr2, P(int) alen)
PP(char *astr1;)
PP(const char *astr2;)
PP(register int alen;)
{
	register char *str1;
	register const char *str2;

	str1 = astr1;
	str2 = astr2;
	while (--alen >= 0)
		*str1++ = *str2++;
}


static VOID getmode(NOTHING)
{
	if (fchr == '.')
	{
		fchr = gchr();
		switch (fchr)
		{
		case 'b':
		case 'B':
		case 's':
		case 'S':
			modelen = BYTESIZ;
			mode = BYTE;
			break;
		case 'w':
		case 'W':
			modelen = WORDSIZ;
			mode = WORD;
			break;
		case 'l':
		case 'L':
			modelen = LONGSIZ;
			mode = LONG;
			break;
		default:
			peekc = fchr;
			fchr = '.';
			goto getm1;
		}
		explmode++;
		fchr = gchr();
		igblk();
		return;
	}
  getm1:
	if (opcpt == exgptr)
	{									/* length is long */
		modelen = LONGSIZ;
		mode = LONG;
	} else
	{
		mode = WORD;					/* default is word */
		modelen = WORDSIZ;
	}
}


/* check to be sure specified mode is legal */
static int modeok(NOTHING)
{
	switch (format)
	{
	case 0:
	case 14:
	case 18:
		return FALSE;
	case 13:
	case 15:
	case 20:
	case 21:
		return modelen == BYTESIZ ? FALSE : TRUE;
	case 4:
	case 25:
		return modelen == BYTESIZ ? TRUE : FALSE;
	case 7:
	case 9:
		return modelen == WORDSIZ ? FALSE : TRUE;
	case 5:
	case 11:
	case 28:
		return modelen == WORDSIZ ? TRUE : FALSE;
	case 6:
		return modelen == LONGSIZ ? FALSE : TRUE;
	case 12:
	case 30:
	case 22:
	case 29:
		return modelen == LONGSIZ ? TRUE : FALSE;
	default:
		return TRUE;
	}
}


static int shiftea(P(int) lidx)
PP(int lidx;)
{
	if (indir[lidx] && numreg[lidx])
	{
		if (numreg[lidx] == WORD_ID)
		{
			numcon[lidx] = 1;
			return WORDSIZ;
		}
		if (numreg[lidx] == LONG_ID)
		{
			numcon[lidx] = 2;
			return LONGSIZ;
		}
		return (numcon[lidx] || numsym[lidx]) ? WORDSIZ : 0;
	}
	if (numsym[lidx])
	{
		if (numreg[lidx] == WORD_ID)
		{
			numcon[lidx] = 1;
			return WORDSIZ;
		}
		if (numreg[lidx] == LONG_ID)
		{
			numcon[lidx] = 2;
			return LONGSIZ;
		}
		return LONGSIZ;
	}
	if (numcon[lidx])
		return numcon[lidx] == 2 ? LONGSIZ : WORDSIZ;
	return 0;
}


/* calc the length of an effective address */
static int lenea(P(int) lidx)
PP(int lidx;)
{
	int l;
	
	if (immed[lidx])
		l = mode == LONG ? LONGSIZ : WORDSIZ;
	else
		l = shiftea(lidx);
	return l;
}


/* calculate the instruction length in bytes */
static int calcilen(NOTHING)
{
	register short i;
	register long l;
	register struct symtab *p;

	i = 2;								/* all instrs at least 2 bytes */

	switch (format)
	{
	case 20:
		i += 2;							/* for reg mask */
	case 1:								/* two ea's -- one of which may be a reg */
	case 15:
	case 30:
	case 26:
	case 5:
	case 3:
	case 21:
		i += lenea(1);
	case 16:
	case 24:
	case 25:
	case 29:
		i += lenea(0);
		break;

	case 9:							/* explicit jmp length... */
		if (!explmode)
			i += lenea(0);
		else
			return mode == LONG ? 6 : 4;	/* explicit jmp.? */
		break;

	case 7:
		i += immed[0] ? 2 + lenea(1) : lenea(1);
		break;

	case 14:
	case 11:
	case 19:
	case 31:
		i += 2;							/* always 4 bytes */
		break;

	case 6:								/* relative branches */
		if (itwc == ITOP1 + 1)
		{
			if (stbuf[ITOP1].itty == ITCN || stbuf[ITOP1].itty == ITCW)
			{
				l = stbuf[ITOP1].itop.l;
			} else if (stbuf[ITOP1].itty == ITSY)
			{
				p = stbuf[ITOP1].itop.ptrw2;
				if (p->flags & SYDF)
					l = p->vl1;			/* symbol value */
				else
					goto loffst;
			} else
			{
				goto loffst;
			}
			l -= (loctr + 2);
			if (l <= 127 && l >= -128)	/* 8 bit offset */
				break;
		}
	  loffst:
		if (!explmode || modelen > BYTESIZ)	/* recognize br extensions */
			i += 2;						/* long offset for branches */
		break;

	case 2:
		i += (mode == LONG ? LONGSIZ : WORDSIZ) + lenea(1);
		break;

	case 23:
		if (immed[0])
			i += (mode == LONG ? LONGSIZ : WORDSIZ);
	case 17:
	case 22:
		i += lenea(1);
		break;

	case 8:
		if (numops == 1)				/* memory shift instruction */
			i += shiftea(0);
		break;
	}

	return i;
}


#define NOCODE ((i>=0&&i<6)||i==9||i==11||i==12||i==16||(i>=20&&i<=30))
/* cond-directives, section, ds, set, equ, reg, globl, end, offset */

/*
 * create intermediate text (it) for one statement
 * call with first character of statement in fchr
 */
static VOID cisit(NOTHING)
{
	register char *p1, *p2;
	register adirect dirop;
	register short i, col1;		/* col1 labels in col 1... */
	char str[SYNAMLEN], *l;
	char tlab1[SYNAMLEN];

  ciss1:
	immed[0] = immed[1] =
	indir[0] = indir[1] =
	numcon[0] = numcon[1] =
	numsym[0] = numsym[1] =
	numreg[0] = numreg[1] = 0;
	plevel = numops = opdix = explmode = 0;
  cistop:
	col1 = 1;
	if (fchr == EOLC)
	{
		fchr = gchr();
		goto cistop;
	}
	if (fchr == ' ')
	{
		col1 = 0;
		igblk();
		if (fchr == EOLC)				/* blank line */
			goto cistop;
		peekc = fchr;
		if (fchr != CEOF)
			fchr = ' ';					/* catch eof... */
	}
	if (fchr == CEOF)
		return;
	if (fchr == '*')
	{									/* ignore comments */
		fchr = gchr();
		if (fchr == '=')
		{								/* relocation counter assignment */
			fchr = gchr();				/* pass the = */
			horg();						/* output constants if not bss */
		}
		igrst();
		fcflg = 0;						/* clear expr first time flag for next stmt */
		goto ciss1;
	}

	/* get the opcode and label */

	mode = 'w';							/* word mode */
	igblk();							/* ignore blanks */
	poslab = 1;
	gterm(TRUE);
	poslab = 0;
	if (fchr == ':' || fchr == '=')
	{									/* there is a label */
	  label:
		col1 = 0;
		if (itype != ITSY)
		{								/* not a symbol */
			uerr(2);					/* invalid label */
			lbt[0] = 0;
		} else
		{
			p2 = lmte->name;
			for (p1 = lbt; p1 < &lbt[SYNAMLEN];)
			{
				*p1++ = *p2++;
			}
			if (fchr == ':')
				fchr = gchr();			/* ignore the colons */
		}
	  labl1:
		ligblk();
		if (fchr == CEOF)
			return;
		if (fchr == '*')
		{
			igrst();					/* comment */
			goto labl1;
		}
		gterm(TRUE);
		if (fchr == ':' || fchr == '=')
		{								/* another label */
			if (lbt[0])
			{
				mystrncpy(tlab1, lmte->name, SYNAMLEN);	/* save current label */
				dlabl();				/* define the last one */
				pack(tlab1, lmte);		/* restore the old label */
			}
			goto label;
		}
	} else
	{
		lbt[0] = 0;						/* no label */
	}
	
	igblk();
	if (fchr == '=')
		goto label;
	if (itype == ITSP)
	{
		if (ival.oper == '=')
		{
			hequ();
			return;
		}
	}
	if (itype != ITSY)					/* not valid opcode */
		goto cisi3;
	if (col1)
	{									/* could be a label save as is... */
		l = str;
		mystrncpy(l, lmte->name, SYNAMLEN);
	}
	if ((opcpt = lemt(TRUE, oirt)) == lmte)
	{									/* not in opcode table */
		if (col1)
		{								/* it's a label... */
			mystrncpy(lmte->name, l, SYNAMLEN);
			goto label;
		}
	  cisi3:
		if (ca_true)					/* report error if not in CA false */
			xerr(3); /* invalid opcode */
		igrst();
		return;
	}
	getmode();							/* look for .b .w or .l mode flag */
	if (opcpt->flags & OPDR)
	{									/* its a directive */
		i = opcpt->vl1;
		if (!ca_true && (i < LOW_CA || i > HI_CA))
		{
			igrst();
			return;
		}
		if (inoffset)
		{
			if (!(NOCODE))
			{
				xerr(12); /* no code or data allowed in offset */
				return;
			}
		}
		dirop = p1direct[i];			/* call routine to handle directive */
		(*dirop) ();
		return;
	} else if (!ca_true)
	{
		igrst();
		return;
	} else if (inoffset)
	{
		xerr(12); /* no code or data allowed in offset */
		return;
	}

	opcval = opcpt->vl1;				/* opcode */
	format = opcpt->flags & OPFF;		/* format of this instr */
	if (explmode)
	{
		if (!modeok())
		{
			xerr(16); /* illegal extension */
			return;
		}
	}
	dlabl();							/* define label */
	opitb();							/* beginning of statement */
	if (format)
		opito();						/* may have operands */
	else
		igrst();						/* only comments */
	format = (opcpt->flags & OPFF);		/* may have changed */


	/* end of statement */

	i = calcilen();
	stbuf[1].itrl = i;					/* assumed instruction length */
	stbuf[0].itrl = itwc;				/* number of it entries */
	wostb();							/* write out statement buffer */
	loctr += i;
}


/* main loop */
static VOID mloop(NOTHING)
{
	while (fchr != CEOF)
	{
		fcflg = 0;						/* first time thru expr pass one */
		cisit();						/* create it for one statement */
	}
	opcpt = endptr;
	hend();
}


/*
 * define a label if there is one to define
 *  call with:
 *      label name in lbt if it exists
 *      else lbt[0] == 0
 */
VOID dlabl(NOTHING)
{
	if (lbt[0])
	{									/* got a label */
		pack(lbt, lmte);				/* put label in main table */
		lblpt = lemt(FALSE, sirt);		/* look up label */
		if (lblpt != lmte)
		{								/* symbol entered previously */
			if (lbt[0] == '~')
			{							/* local symbol -- may be duplicate */
				lblpt = lmte;
				mmte();
			} else
			{
				if (lblpt->flags & SYXR)
				{
					uerr(29); /* bad use of symbol */
					lblpt = NULL;
					return;
				}
				if (lblpt->flags & SYDF)
				{
					uerr(1, lblpt->name);	/* label redefined */
					lblpt = NULL;
					return;
				}
			}
		} else
		{
			mmte();						/* make label entry in main table */
		}
		lblpt->flags |= SYDF;			/* label is now defined */
		if (rlflg == TEXT)
			lblpt->flags |= SYRO;
		else if (rlflg == DATA)
			lblpt->flags |= SYRA;
		else if (rlflg == BSS)
			lblpt->flags |= SYBS;
		/* No flags to set if absolute */
		lblpt->vl1 = loctr;				/* label value */
	} else
	{
		lblpt = NULL;
	}
}


/* change clr.l An to suba.l An,An */
static VOID chgclr(NOTHING)
{
	register struct symtab *p;

	if (itype == ITSY)
	{									/* first op is symbol */
		p = lemt(FALSE, sirt);
		if (p == lmte)
			return;
		if (!(p->flags & SYER) || p->vl1 < AREGLO)	/* not A reg */
			return;
		opcpt = subaptr;				/* make it a suba instr */
		opitb();
		opitoo();						/* output first operand -- An */
		itype = ITSP;
		ival.l = ',';
		opitoo();						/* output a comma */
		itype = ITSY;					/* now the A reg again */
	}
}


/*
 * output it for operands
 *      gets intput from gterm
 *      puts output in stbuf using itwc as an index
 *      itwc should point at the next entry to be made in stbuf
 */
VOID opito(NOTHING)
{
	register int lopcomma;

	lopcomma = symcon = chmvq = 0;
	numops++;							/* count first operand */
	for (;;)
	{
		starmul = symcon;				/* star is multiply op if flag is set */
		if (fchr == '\'' || fchr == '"')
			lopcomma = 0;
		gterm(FALSE);					/* get a term */
		if (itwc == ITOP1 && format == CLRFOR && opcval == CLRVAL)
			chgclr();
		opitoo();						/* output it for one operand */
		if (itype == ITSP && ival.oper == ',')
		{
			if (plevel == 1 && !numcon[opdix])
				numcon[opdix] = 1;
			if (lopcomma)
				uerr(30);	/* invalid data list */
			lopcomma++;
			igblk();					/* ignore blanks for 68000 C compiler */
		} else
		{
			lopcomma = 0;
		}
		if (ival.l == EOLC && itype == ITSP)	/* end of operands */
			break;
		if (fchr == EOLC)
		{
			fchr = gchr();
			break;
		}
	}
	if (chmvq)							/* changed move to moveq */
	{
		if (numops != 2 || immed[1] || indir[1] || numcon[1] || numsym[1] || numreg[1] >= AREGLO)
		{
			stbuf[2].itop.ptrw2 = moveptr;	/* change it back */
			opcpt = moveptr;
		}
	}
	
	if (stbuf[2].itop.ptrw2 == cmpptr)	/* cmp -> cmpm ?? */
	{
		if (numreg[0] && numreg[1] && indir[0] && indir[1])
		{
			stbuf[2].itop.ptrw2 = cmpmptr;
			opcpt = cmpmptr;
		}
	}
	
	if (lopcomma)
		uerr(30); /* invalid data list */
}


/* change add into addq and sub into subq if possible */
static VOID tryquick(NOTHING)
{
	register struct symtab *p;
	register long l;

	if (fchr != ',' || !immed[0])
		return;
	l = ival.l;
	if (itwc != ITOP1 + 1)
	{
		if (itwc != ITOP1 + 2 || stbuf[ITOP1 + 1].itty != ITSP || stbuf[ITOP1 + 1].itop.oper != '-')
			return;
		l = -l;
	}
	p = stbuf[2].itop.ptrw2;
	if (p == moveptr)
	{
		if (explmode && modelen != LONGSIZ)	/* dont change .w or .b */
			return;
		if (l >= -128 && l <= 127)
		{
			stbuf[2].itop.ptrw2 = moveqptr;
			opcpt = moveqptr;
			chmvq++;
		}
		return;
	}
	if (l <= 0 || l > 8)
	{
		return;
	}
	if (p == addptr || p == addiptr)
	{
		stbuf[2].itop.ptrw2 = opcpt = addqptr;
	} else if (p == subptr || p == subiptr)
	{
		stbuf[2].itop.ptrw2 = opcpt = subqptr;
	}
}


/* output it for one operand */
VOID opitoo(NOTHING)
{
	register struct symtab *sp;
	register int16_t h;
	
	symcon = 0;
	if (itype == ITSP)
	{									/* special symbol */
		if (ival.oper == ',' && !plevel)
		{								/* another operand */
			numops++;
			if (!opdix)
				opdix++;
		}
		if (ival.oper == ')')
			symcon = 1;					/* star is multiply */
		if (ival.oper == ' ')
		{								/* end of operands */
			while (fchr != EOLC)		/* ignore rest of statement */
				fchr = gchr();
			return;
		}
		if (ival.oper == EOLC)
			return;
	} else								/* symbol or constant */
	{
		symcon = 1;
	}
	
	if (itwc >= STMAX)
	{									/* it overflow */
		rpterr(_("i.t. overflow"));
		asabort();
	}
	pitw->itty = itype;					/* type of it entry */

	/* put symbol in it buffer */
	if (itype == ITSY)
	{
		sp = lemt(FALSE, sirt);			/* look up it main table */
		if (sp == lmte)					/* first occurrance */
		{
			mmte();
		}
		pitw->itop.ptrw2 = sp;			/* ptr to symbol entry */
		if (!(sp->flags & SYER))		/* is it a register? */
		{
			numsym[opdix]++;
		} else							/* yes, a register */
		{
			if (numreg[opdix] == 0)
				numreg[opdix] = sp->vl1;
			if (itwc > ITOP1)
			{
				if (pitw[-1].itty == ITCN || pitw[-1].itty == ITCW)
				{
					if (sp->vl1 == WORD_ID)
					{
						h = (int16_t)(pitw[-1].itop.l >> 16);
						if (h != 0 && h != -1)
						{
							uerr(44); /* warning: constant out of range */
							nerror--;
						}
						pitw[-1].itty = ITCW;
						numcon[opdix] = 1;
					} else if (sp->vl1 == LONG_ID)
					{
						pitw[-1].itty = ITCN;
						numcon[opdix] = 2;
					}
				}	
			}
		}
		itwc++;							/* count entries in it buffer */
		pitw++;
		return;
	} else if (itype == ITCN)
	{
		h = (int16_t)(ival.l >> 16);
		if (!shortadr || (h != 0 && h != -1))
		{
			numcon[opdix] = 2;
		} else if (!numcon[opdix])
		{
			numcon[opdix] = 1;
			itype = pitw->itty = ITCW;
		}
		if (numops == 1)
			tryquick();
		reloc = ABS;
	}

	/* special characters and constants */
	pitw->itop.p = ival.p;
	pitw->itrl = reloc;
	itwc++;
	pitw++;
}


static struct {
	char name[4];
	short val;
} const regnames[] = {
	{ "R0", 0 },
	{ "R1", 1 },
	{ "R2", 2 },
	{ "R3", 3 },
	{ "R4", 4 },
	{ "R5", 5 },
	{ "R6", 6 },
	{ "R7", 7 },
	{ "R8", 8 },
	{ "R9", 9 },
	{ "R10", 10 },
	{ "R11", 11 },
	{ "R12", 12 },
	{ "R13", 13 },
	{ "R14", 14 },
	{ "R15", 15 },
	{ "D0", 0 },
	{ "D1", 1 },
	{ "D2", 2 },
	{ "D3", 3 },
	{ "D4", 4 },
	{ "D5", 5 },
	{ "D6", 6 },
	{ "D7", 7 },
	{ "A0", 8 },
	{ "A1", 9 },
	{ "A2", 10 },
	{ "A3", 11 },
	{ "A4", 12 },
	{ "A5", 13 },
	{ "A6", 14 },
	{ "A7", 15 },
	{ "SP", 15 },
	{ "CCR", CCR },
	{ "SR", SR },
	{ "USP", USP },
	{ ".B", BYTE_ID },
	{ ".W", WORD_ID },
	{ ".L", LONG_ID },
	{ "PC", PC },
	{ "SFC", SFC },
	{ "DFC", DFC },
	{ "VSR", VBR },
	{ "VBR", VBR },

	{ "r0", 0 },
	{ "r1", 1 },
	{ "r2", 2 },
	{ "r3", 3 },
	{ "r4", 4 },
	{ "r5", 5 },
	{ "r6", 6 },
	{ "r7", 7 },
	{ "r8", 8 },
	{ "r9", 9 },
	{ "r10", 10 },
	{ "r11", 11 },
	{ "r12", 12 },
	{ "r13", 13 },
	{ "r14", 14 },
	{ "r15", 15 },
	{ "d0", 0 },
	{ "d1", 1 },
	{ "d2", 2 },
	{ "d3", 3 },
	{ "d4", 4 },
	{ "d5", 5 },
	{ "d6", 6 },
	{ "d7", 7 },
	{ "a0", 8 },
	{ "a1", 9 },
	{ "a2", 10 },
	{ "a3", 11 },
	{ "a4", 12 },
	{ "a5", 13 },
	{ "a6", 14 },
	{ "a7", 15 },
	{ "sp", 15 },
	{ "ccr", CCR },
	{ "sr", SR },
	{ "usp", USP },
	{ ".b", BYTE_ID },
	{ ".w", WORD_ID },
	{ ".l", LONG_ID },
	{ "pc", PC },
	{ "sfc", SFC },
	{ "dfc", DFC },
	{ "vsr", VBR },
	{ "vbr", VBR },
};


static VOID equreg(P(const char *) name, P(int) val)
PP(const char *name;)
PP(int val;)
{
	register struct symtab *ptr;
	
	pack(name, lmte);
	ptr = lemt(FALSE, sirt);
	if (ptr != lmte)
	{
		uerr(1, name);	/* label redefined */
		asabort();
	}
	mmte();
	ptr->flags = SYDF | SYEQ | SYER | SYIN;
	ptr->vl1 = val;
}


static struct symtab *mkopd(P(const char *) name, P(int) formt, P(unsigned short) val)
PP(const char *name;)
PP(int formt;)
PP(unsigned short val;)
{
	register struct symtab *ptr;
	
	pack(name, lmte);
	ptr = lemt(FALSE, oirt);
	if (ptr != lmte)
	{
		uerr(5, name); /* opcode redefined */
		asabort();
	}
	mmte();
	ptr->flags = formt | SYIN;
	ptr->vl1 = val;
	return ptr;
}


static VOID usage(NOTHING)
{
	rpterr(_("Usage: as68 [-p] [-u] [-l] [-n] [-s d:] [-f d:] sourcefile"));
	exit(EXIT_FAILURE);
}


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register short i;

	nerror = 0;

	signal(SIGINT, rubout);
	signal(SIGQUIT, rubout);
	signal(SIGHUP, rubout);
	signal(SIGTERM, rubout);

	pitix = 0;

	if (argc <= 1)
		usage();
	i = 1;
	shortadr = 0;						/* long addresses... */
	while (argv[i][0] == '-')
	{									/* may be print or initialize */
		switch (argv[i++][1])
		{
		case 'a':						/* short addresses only */
			shortadr = -1;
			break;

		case 'i':						/* initialize the assembler */
			/* ignored; no longer needed */
			break;

		case 'p':						/* produce a listing */
			prtflg++;
			break;

		case 'u':						/* make undefined symbols external */
			undflg++;
			break;

		case 'N':						/* no branch optimization */
		case 'n':
			didorg++;
			break;

		case 'L':						/* OBSOLETE, long addresses only */
		case 'l':
			shortadr = 0;
			break;

		case 'T':						/* generating code suitable for the 68010 */
		case 't':
			m68010++;
			break;

		case 'f':						/* Redirect temp files */
			tdname = argv[i++];
			break;

		case 's':						/* Change symbol table prefix */
			i++;
			/* ignored; no longer needed */
			break;

		default:
			usage();
		}
	}
	if (i >= argc)
		usage();
	/* Remember source filename */
	sfname = argv[i];
	/* open source file */
	ifn = openfi(argv[i], "r");
	/* create relocatable object file name */
	setldfn(argv[i]);
	/* open loader file */
	lfil = openfi(ldfn, "wb");
	
	/* get a temp file for it */
	itfn = gettempf(itfilnam);
	/* temp for text relocation bits */
	trfil = gettempf(trfilnam);
	/* temp for data binary */
	dafil = gettempf(dafilnam);
	/* temp for data relocation bits */
	drfil = gettempf(drfilnam);
	
	initsy();
	
	/* make entries in main table for directives */
	mdemt("opd", 0);				/* opcode definition */
	endptr = mdemt("end", 1);		/* end statement */
	mdemt("data", 2);				/* dsect directive(code DATA based) */
	mdemt("text", 3);				/* psect directive(code TEXT based) */
	equptr = mdemt("equ", 4);		/* equate */
	mdemt("set", 5);				/* .set - same as .equ */
	mdemt("dc", 8);					/* define byte */
	mdemt("globl", 9);				/* define global (public) symbols */
	mdemt("xdef", 9);				/* define global (public) symbols */
	mdemt("xref", 9);				/* define global (public) symbols */
	mdemt("comm", 10);				/* define external symbols */
	mdemt("bss", 11);				/* block storage based */
	mdemt("ds", 12);				/* block storage based */
	evenptr = mdemt("even", 13);	/* round pc */
	mdemt("~.yxzorg", 14);			/* internal, *= */
	orgptr = mdemt("org", 14);		/* org location, also *= */
	mdemt("mask2", 15);				/* assemble for mask2, ignore */
	mdemt("reg", 16);				/* register equate */
	mdemt("dcb", 17);				/* define block */
	mdemt("comline", 18);			/* command line */
	mdemt("idnt", 19);				/* relocateable id record, ignore */
	mdemt("offset", 20);			/* define offsets */
	mdemt("section", 21);			/* define sections */
	mdemt("ifeq", 22);				/* ca if expr = 0 */
	mdemt("ifne", 23);				/* ca if expr != 0 */
	mdemt("iflt", 24);				/* ca if expr < 0 */
	mdemt("ifle", 25);				/* ca if expr <= 0 */
	mdemt("ifgt", 26);				/* ca if expr > 0 */
	mdemt("ifge", 27);				/* ca if expr >= 0 */
	mdemt("endc", 28);				/* end ca */
	mdemt("ifc", 29);				/* ca if string compare */
	mdemt("ifnc", 30);				/* ca if not string compare */
	mdemt("opt", 31);				/* ignored, assemb options */
	mdemt("ttl", 32);				/* ttl define, ignore */
	mdemt("page", 33);				/* page define, ignore */
	
	/* make entries in main table for registers */
	for (i = 0; i < sizeof(regnames) / sizeof(regnames[0]); i++)
		equreg(regnames[i].name, regnames[i].val);
	
	/* make entries in opcode table */

	mkopd("abcd",    4, 0140400);
	addptr = mkopd("add",    1, 0150000);
	addaptr = mkopd("adda",    15, 0150000);
	addiptr = mkopd("addi",    2, 0003000);
	addqptr = mkopd("addq",    17, 0050000);
	mkopd("inc",    16, 0050000);
	mkopd("addx",    27, 0150400);
	andptr = mkopd("and",    1, 0140000);
	andiptr = mkopd("andi",    2, 0001000);
	mkopd("asl",    8, 0160400);
	mkopd("asr",    8, 0160000);
	mkopd("bcc",    6, 0062000);
	mkopd("bcs",    6, 0062400);
	mkopd("beq",    6, 0063400);
	mkopd("bze",    6, 0063400);
	mkopd("bge",    6, 0066000);
	mkopd("bgt",    6, 0067000);
	mkopd("bhi",    6, 0061000);
	mkopd("bhis",    6, 0062000);
	mkopd("bhs",    6, 0062000);
	mkopd("ble",    6, 0067400);
	mkopd("blo",    6, 0062400);
	mkopd("bls",    6, 0061400);
	mkopd("blos",    6, 0061400);
	mkopd("blt",    6, 0066400);
	mkopd("bmi",    6, 0065400);
	mkopd("bne",    6, 0063000);
	mkopd("bnz",    6, 0063000);
	mkopd("bpl",    6, 0065000);
	mkopd("bvc",    6, 0064000);
	mkopd("bvs",    6, 0064400);
	mkopd("bchg",    7, 0000100);
	mkopd("bclr",    7, 0000200);
	mkopd("bra",    6, 0060000);
	mkopd("bt",    6, 0060000);
	mkopd("bset",    7, 0000300);
	bsrptr = mkopd("bsr",    6, 0060400);
	mkopd("btst",    7, 0000000);
	mkopd("chk",    26, 0040600);
	mkopd("clr",    24, 0041000);
	cmpptr = mkopd("cmp",    26, 0130000);
	cmpaptr = mkopd("cmpa",    15, 0130000);
	cmpiptr = mkopd("cmpi",    2, 0006000);
	cmpmptr = mkopd("cmpm",    10, 0130410);
	mkopd("dbcc",    11, 0052310);
	mkopd("dbcs",    11, 0052710);
	mkopd("dblo",    11, 0052710);
	mkopd("dbeq",    11, 0053710);
	mkopd("dbze",    11, 0053710);
	mkopd("dbra",    11, 0050710);
	mkopd("dbf",    11, 0050710);
	mkopd("dbge",    11, 0056310);
	mkopd("dbgt",    11, 0057310);
	mkopd("dbhi",    11, 0051310);
	mkopd("dbhs",    11, 0051310);
	mkopd("dble",    11, 0057710);
	mkopd("dbls",    11, 0051710);
	mkopd("dblt",    11, 0056710);
	mkopd("dbmi",    11, 0055710);
	mkopd("dbne",    11, 0053310);
	mkopd("dbnz",    11, 0053310);
	mkopd("dbpl",    11, 0055310);
	mkopd("dbt",    11, 0050310);
	mkopd("dbvc",    11, 0054310);
	mkopd("dbvs",    11, 0054710);
	mkopd("divs",    5, 0100700);
	mkopd("divu",    5, 0100300);
	eorptr = mkopd("eor",    23, 0130000);
	eoriptr = mkopd("eori",    2, 0005000);
	exgptr = mkopd("exg",    12, 0140400);
	mkopd("ext",    13, 0044000);
	mkopd("jmp",    9, 0047300);
	jsrptr = mkopd("jsr",    9, 0047200);
	mkopd("illegal",   0, 0045374);
	mkopd("lea",    30, 0040700);
	mkopd("link",    19, 0047120);
	mkopd("lsr",    8, 0160010);
	mkopd("lsl",    8, 0160410);
	moveptr = mkopd("move",    3, 0000000);
	mkopd("movea",    3, 0000100);
	mkopd("movec",    31, 0047172);
	mkopd("movem",    20, 0044200);
	mkopd("movep",    21, 0000010);
	moveqptr = mkopd("moveq",    22, 0070000);
	mkopd("moves",    31, 0007000);
	mkopd("muls",    5, 0140700);
	mkopd("mulu",    5, 0140300);
	mkopd("nbcd",    25, 0044000);
	mkopd("neg",    24, 0042000);
	mkopd("negx",    24, 0040000);
	nopptr = mkopd("nop",    0, 0047161);
	mkopd("not",    24, 0043000);
	orptr = mkopd("or",    1, 0100000);
	oriptr = mkopd("ori",    2, 0000000);
	mkopd("pea",    29, 0044100);
	mkopd("reset",    0, 0047160);
	mkopd("rol",    8, 0160430);
	mkopd("ror",    8, 0160030);
	mkopd("roxl",    8, 0160420);
	mkopd("roxr",    8, 0160020);
	mkopd("rtd",    14, 0047164);
	mkopd("rte",    0, 0047163);
	mkopd("rtr",    0, 0047167);
	mkopd("rts",    0, 0047165);
	mkopd("sbcd",    4, 0100400);
	mkopd("scc",    25, 0052300);
	mkopd("shs",    25, 0052300);
	mkopd("scs",    25, 0052700);
	mkopd("slo",    25, 0052700);
	mkopd("seq",    25, 0053700);
	mkopd("sze",    25, 0053700);
	mkopd("sf",    25, 0050700);
	mkopd("sge",    25, 0056300);
	mkopd("sgt",    25, 0057300);
	mkopd("shi",    25, 0051300);
	mkopd("sle",    25, 0057700);
	mkopd("sls",    25, 0051700);
	mkopd("slt",    25, 0056700);
	mkopd("smi",    25, 0055700);
	mkopd("sne",    25, 0053300);
	mkopd("snz",    25, 0053300);
	mkopd("spl",    25, 0055300);
	mkopd("st",    25, 0050300);
	mkopd("svc",    25, 0054300);
	mkopd("svs",    25, 0054700);
	mkopd("stop",    14, 0047162);
	subptr = mkopd("sub",    1, 0110000);
	subaptr = mkopd("suba",    15, 0110000);
	subiptr = mkopd("subi",    2, 0002000);
	subqptr = mkopd("subq",    17, 0050400);
	mkopd("dec",    16, 0050400);
	mkopd("subx",    27, 0110400);
	mkopd("swap",    28, 0044100);
	mkopd("tas",    25, 0045300);
	mkopd("trap",    18, 0047100);
	mkopd("trapv",    0, 0047166);
	mkopd("tst",    24, 0045000);
	mkopd("unlk",    13, 0047130);
	
	rlflg = TEXT;						/* code initially TEXT based */
	inoffset = 0;						/* not in offset mode */
	loctr = 0;							/* no generated code */
	ca = 0;								/* depth of conditional assembly */
	extindx = 0;						/* no external symbols yet */
	p2flg = 0;							/* pass 1 */
	ca_true = 1;						/* true unless in side false case */
	absln = 1;
	fchr = gchr();						/* get first char */

	mloop();
	return EXIT_SUCCESS;
}
