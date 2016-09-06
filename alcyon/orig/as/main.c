/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

char *version = "@(#)main.c	1.5    12/28/83";

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
#include <fcntl.h>
#include "def.h"
#ifdef __ALCYON__
/* BUG: signal() not declared */
#define signal std_signal
#include <signal.h>
#undef signal
int (*(signal()))();
#else
#include <signal.h>
#endif
#include <stdlib.h>
#include <string.h>

#ifndef __ALCYON__
int mkstemp(char *template);
#define mktemp(f) close(mkstemp(f))
#endif


VOID usage PROTO((NOTHING));
VOID mloop PROTO((NOTHING));
VOID cisit PROTO((NOTHING));
VOID getmode PROTO((NOTHING));
int modeok PROTO((NOTHING));
int calcilen PROTO((NOTHING));
int lenea PROTO((int lidx));
int shiftea PROTO((int lidx));
VOID chgclr PROTO((NOTHING));
VOID tryquick PROTO((NOTHING));
VOID mystrncpy PROTO((char *astr1, const char *astr2, int alen));




/*
 * Define Temporary and Init file names per O/S.  We use code here to
 * allow re-directing temp files and the init file via command line
 * switches.
 */

const char *tdname = "";					/*  Temp files in same directory    */
const char *idname = "0:";					/*  Init file  in user 0        */

/*
 * BUG: template name too long (9 chars),
 */
char const tfilebase[] = "a6AXXXXXX";		/*  Temp file  basename         */
char const initbase[] = "as68symb.dat";		/*  Init file  basename         */


#define INIT(op,ptr) pack(op,lmte); ptr=lemt(TRUE,oirt)


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register short i, ttmp;
	register __intptr_t longtmp;

	nerror = initflg = 0;
	prtchidx = prtchars; /* unused */

	/* BUG: signal() not declared; useless anyway, since they are not implemented in the library */
	/* BUG2: comparing the result of signal to a function pointer generates totally bogus code */
#ifdef __ALCYON__
	asm("move.l    #$00000001,(a7)");
	asm("move.w    #$0002,-(a7)");
	asm("jsr       _signal");
	asm("addq.l    #2,a7");
	asm("ext.l     d0");
	asm("cmp.l     #$00000001,d0");
	asm("beq.s     L8001");
	signal(SIGINT, (sighandler_t)rubout);
	asm("L8001:");
	asm("move.l    #$00000001,(a7)");
	asm("move.w    #$0003,-(a7)");
	asm("jsr       _signal");
	asm("addq.l    #2,a7");
	asm("ext.l     d0");
	asm("cmp.l     #$00000001,d0");
	asm("beq.s     L8002");
	signal(SIGQUIT, (sighandler_t)rubout);
	asm("L8002:");
	asm("move.l    #$00000001,(a7)");
	asm("move.w    #$0001,-(a7)");
	asm("jsr       _signal");
	asm("addq.l    #2,a7");
	asm("ext.l     d0");
	asm("cmp.l     #$00000001,d0");
	asm("beq.s     L8003");
	signal(SIGHUP, (sighandler_t)rubout);
	asm("L8003:");
#else
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, (sighandler_t)rubout);
	if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
		signal(SIGQUIT, (sighandler_t)rubout);
	if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
		signal(SIGHUP, (sighandler_t)rubout);
#endif
	signal(SIGTERM, (sighandler_t)rubout);

	pitix = itbuf;
	pexti = extbl;

	ttmp = (sizeof(struct symtab) * SZMT) + 2;
	bmte = (struct symtab *)sbrk(ttmp);
	longtmp = (__intptr_t)bmte;
	if (longtmp & 1L)
#ifdef __ALCYON__
		asm("addq.l #1,_bmte");							/* make it even */
#else
		bmte = (struct symtab *)((char *)bmte + 1);							/* make it even */
#endif
	emte = (struct symtab *)((char *)bmte + ttmp - 2);				/* end of main table */

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
			initflg++;
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
			idname = argv[i++];
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
	ifn = openfi(argv[i], O_RDONLY, 0);
	/* create relocatable object file name */
	setldfn(argv[i]);
	/* open loader file */
	lfn = openfi(ldfn, O_WRONLY, 1);

	/* Init for strcat */
	tfilname[0] = '\0';
	initfnam[0] = '\0';
	/* Build temp file names */
	strcat(tfilname, tdname);
	strcat(tfilname, tfilebase);
	/* Make it unique */
	mktemp(tfilname);
	tfilptr = &tfilname[(int)strlen(tfilname) - 1] - 6;
	/* Build Symbol file name */
	strcat(initfnam, idname);
	strcat(initfnam, initbase);

	/* get a temp file for it */
	itfn = gettempf();
	/* remember last char for unlink */
	itfnc = LASTCHTFN;
	/* temp for text relocation bits */
	trbfn = gettempf();
	trbfnc = LASTCHTFN;
	/* temp for data binary */
	dafn = gettempf();
	dafnc = LASTCHTFN;
	/* temp for data relocation bits */
	drbfn = gettempf();
	drbfnc = LASTCHTFN;

	ttmp = (sizeof(struct symtab) * SZMT) + 2;
	bmte = (struct symtab *)sbrk(ttmp - 1);
	longtmp = (__intptr_t)bmte;
	if (longtmp & 1L)
#ifdef __ALCYON__
		asm("addq.l #1,_bmte");							/* make it even */
#else
		bmte = (struct symtab *)((char *)bmte + 1);							/* make it even */
#endif
	/* end of main table */
	emte = (struct symtab *)((char *)bmte + ttmp - 2);				/* end of main table */

	if (initflg)
	{
		/* initializing the main table */
		lmte = bmte;
		cszmt = SZMT;
		for (i = 0; i <= SZIRT - 2; i += 2)
		{
			sirt[i] = (struct symtab *)&sirt[i];			/* initialize the initial ref tables */
#ifdef __ALCYON__
			asm("movea.w   d7,a0");
			asm("addq.w    #1,a0");
			asm("adda.l    a0,a0");
			asm("adda.l    a0,a0");
			asm("adda.l    #_sirt,a0");
			asm("clr.l     (a0)"); /* generated: move.l #$0,(a0) */
#else
			sirt[i + 1] = 0;
#endif
			oirt[i] = (struct symtab *)&oirt[i];
#ifdef __ALCYON__
			asm("movea.w   d7,a0");
			asm("addq.w    #1,a0");
			asm("adda.l    a0,a0");
			asm("adda.l    a0,a0");
			asm("adda.l    #_oirt,a0");
			asm("clr.l     (a0)"); /* generated: move.l #$0,(a0) */
#else
			oirt[i + 1] = 0;
#endif
		}

		/* make entries in main table for directives */
		mdemt("opd", 0);				/* opcode definition */
		mdemt(endstr, 1);				/* end statement */
		mdemt("data", 2);				/* dsect directive(code DATA based) */
		mdemt("text", 3);				/* psect directive(code TEXT based) */
		mdemt(equstr, 4);				/* equate */
		mdemt("set", 5);				/* .set - same as .equ */
		mdemt("dc", 8);					/* define byte */
		mdemt("globl", 9);				/* define global (public) symbols */
		mdemt("xdef", 9);				/* define global (public) symbols */
		mdemt("xref", 9);				/* define global (public) symbols */
		mdemt("comm", 10);				/* define external symbols */
		mdemt("bss", 11);				/* block storage based */
		mdemt("ds", 12);				/* block storage based */
		mdemt(evnstr, 13);				/* round pc */
		mdemt(orgstr1, 14);				/* internal, *= */
		mdemt(orgstr2, 14);				/* org location, also *= */
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
	} else
	{
		/* read initialized main table */
		getsymtab();
	}
	
	rlflg = TEXT;						/* code initially TEXT based */
	inoffset = 0;						/* not in offset mode */
	loctr = 0;							/* no generated code */
	ca = 0;								/* depth of conditional assembly */
	extindx = 0;						/* no external symbols yet */
	p2flg = 0;							/* pass 1 */
	ca_true = 1;						/* true unless in side false case */
	absln = 1;
	sbuflen = -1;						/* no source yet */
	fchr = gchr();						/* get first char */
	if (!initflg)
	{									/* not initializing */
		INIT(orgstr2, orgptr);
		INIT(endstr, endptr);
		INIT(equstr, equptr);
		INIT("add", addptr);
		INIT("addi", addiptr);
		INIT("addq", addqptr);
		INIT("sub", subptr);
		INIT("subi", subiptr);
		INIT("subq", subqptr);
		INIT("cmp", cmpptr);
		INIT("adda", addaptr);
		INIT("cmpa", cmpaptr);
		INIT("suba", subaptr);
		INIT("cmpm", cmpmptr);
		INIT("and", andptr);
		INIT("andi", andiptr);
		INIT("or", orptr);
		INIT("ori", oriptr);
		INIT("cmpi", cmpiptr);
		INIT("eor", eorptr);
		INIT("eori", eoriptr);
		INIT("move", moveptr);
		INIT("moveq", moveqptr);
		INIT("exg", exgptr);
		INIT("jsr", jsrptr);
		INIT("bsr", bsrptr);
		INIT("nop", nopptr);
		INIT(evnstr, evenptr);
	}
	mloop();
#ifndef __ALCYON__
	return EXIT_SUCCESS;
#endif
}


VOID usage(NOTHING)
{
	rpterr("Usage: as68 [-p] [-u] [-l] [-n] [-s d:] [-f d:] sourcefile\n");
	endit();
}


/* main loop */
VOID mloop(NOTHING)
{
	register short i;

	while (fchr != CEOF)
	{
		if (absln >= brkln1)			/* break for debugging the assembler */
			i = 0;
		fcflg = 0;						/* first time thru expr pass one */
		cisit();						/* create it for one statement */
	}
	opcpt = endptr;
	hend();
}


#define NOCODE ((i>=0&&i<6)||i==9||i==11||i==12||i==16||(i>=20&&i<=30))
/* cond-directives, section, ds, set, equ, reg, globl, end, offset */

/*
 * create intermediate text (it) for one statement
 * call with first character of statement in fchr
 */
VOID cisit(NOTHING)
{
	register short *p1, *p2;
	register adirect dirop;
	register short i, col1;		/* col1 labels in col 1... */
	char str[SYNAMLEN], *l;

  ciss1:
	immed[0] = immed[1] = indir[0] = indir[1] = numcon[0] = 0;
	numcon[1] = numsym[0] = numsym[1] = numreg[0] = numreg[1] = 0;
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
		peekc = (char) fchr;
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
			uerr(2);
			lbt[0] = 0;					/* no label */
		} else
		{
			p2 = (short *)&lmte->name[0];
			for (p1 = (short *)&lbt[0]; p1 < (short *)(&lbt[SYNAMLEN]);)
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
				pack(tlab1, lmte);		/* restor the old lable */
			}
			goto label;
		}
	} else
		lbt[0] = 0;						/* no label */

	igblk();
	if (fchr == '=')
		goto label;
	if (itype == ITSP)
	{
		if (ival.u.loword == '=')
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
			xerr(3);
		igrst();
		return;
	}
	getmode();							/* look for .b .w or .l mode flag */
	if (opcpt->flags & OPDR)
	{									/* its a directive */
		i = opcpt->vl1.l;
		if (!ca_true && (i < LOW_CA || i > HI_CA))
		{
			igrst();
			return;
		}
		if (inoffset)
		{
			if (!(NOCODE))
			{							/* can't generate code in offset */
				xerr(12);
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
		xerr(12);
		return;
	}

	opcval = opcpt->vl1.l;				/* opcode */
	format = opcpt->flags & OPFF;		/* format of this instr */
	if (explmode)
		if (!modeok())
		{
			xerr(16);
			return;
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
	stbuf[0].itrl = (char)itwc;				/* number of it entries */
	wostb();							/* write out statement buffer */
	loctr += i;
}


VOID getmode(NOTHING)
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
			peekc = (char)fchr;
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
int modeok(NOTHING)
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


/* calculate the instruction length in bytes */
int calcilen(NOTHING)
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
			if (stbuf[ITOP1].itty == ITCN)
				l = stbuf[ITOP1].itop.l;
			else if (stbuf[ITOP1].itty == ITSY)
			{
				p = stbuf[ITOP1].itop.ptrw2;
				if (p->flags & SYDF)
					l = p->vl1.l;			/* symbol value */
				else
					goto loffst;
			} else
				goto loffst;
			l -= (loctr + 2);
			if (l <= 127 && l >= -128)	/* 8 bit offset */
				break;
		}
	  loffst:
		if (!explmode || modelen > BYTESIZ)	/* recognize br extensions */
			i += 2;						/* long offset for branches */
		break;

	case 2:
		i += (mode == LONG ? 4 : 2) + lenea(1);
		break;

	case 23:
		if (immed[0])
			i += (mode == LONG ? 4 : 2);
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


/* calc the length of an effective address */
int lenea(P(int) lidx)
PP(int lidx;)
{
	if (immed[lidx])
		return mode == LONG ? LONGSIZ : WORDSIZ;
	return shiftea(lidx);
}


int shiftea(P(int) lidx)
PP(int lidx;)
{
	if (indir[lidx] && numreg[lidx])
		return (numcon[lidx] || numsym[lidx]) ? 2 : 0;
	if (numsym[lidx] || numcon[lidx])
		return (!shortadr || numcon[lidx] == 2) ? 4 : 2;
	return 0;
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
					uerr(29);
					lblpt = 0;
					return;
				}
				if ((lblpt->flags) & SYDF)
				{
					uerr(1);
					lblpt = 0;
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
		lblpt->vl1.l = loctr;				/* label value */
	} else
		lblpt = 0;
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
	while (1)
	{
		starmul = symcon;				/* star is multiply op if flag is set */
		if (fchr == '\'' || fchr == '"')
			lopcomma = 0;
		gterm(FALSE);					/* get a term */
		if (itwc == ITOP1 && format == CLRFOR && opcval == CLRVAL)
			chgclr();
		opitoo();						/* output it for one operand */
		if (itype == ITSP && ival.u.loword == ',')
		{
			if (plevel == 1 && !numcon[opdix])
				numcon[opdix] = 1;
			if (lopcomma)
				uerr(30);
			lopcomma++;
			igblk();					/* ignore blanks for 68000 C compiler */
		} else
			lopcomma = 0;
		if (ival.l == EOLC && itype == ITSP)	/* end of operands */
			break;
		if (fchr == EOLC)
		{
			fchr = gchr();
			break;
		}
	}
	if (chmvq)							/* changed move to moveq */
		if (numops != 2 || immed[1] || indir[1] || numcon[1] || numsym[1] || numreg[1] >= AREGLO)
		{
			stbuf[2].itop.ptrw2 = moveptr;	/* change it back */
			opcpt = moveptr;
		}

	if (stbuf[2].itop.ptrw2 == cmpptr)	/* cmp -> cmpm ?? */
		if (numreg[0] && numreg[1] && indir[0] && indir[1])
		{
			stbuf[2].itop.ptrw2 = cmpmptr;
			opcpt = cmpmptr;
		}

	if (lopcomma)
		uerr(30);
}


/* change clr.l An to suba.l An,An */
VOID chgclr(NOTHING)
{
	register struct symtab *p;

	if (itype == ITSY)
	{									/* first op is symbol */
		p = lemt(FALSE, sirt);
		if (p == lmte)
			return;
		if (!(p->flags & SYER) || p->vl1.l < AREGLO)	/* not A reg */
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


/* output it for one operand */
VOID opitoo(NOTHING)
{
	register struct symtab *sp;

	symcon = 0;
	if (itype == ITSP)
	{									/* special symbol */
		if (ival.u.loword == ',' && !plevel)
		{								/* another operand */
			numops++;
			if (!opdix)
				opdix++;
		}
		if (ival.u.loword == ')')
			symcon = 1;					/* star is multiply */
		if (ival.u.loword == ' ')
		{								/* end of operands */
			while (fchr != EOLC)		/* ignore rest of statement */
				fchr = gchr();
			return;
		}
		if (ival.u.loword == EOLC)
			return;
	} else								/* symbol or constant */
		symcon = 1;

	if (itwc >= STMAX)
	{									/* it overflow */
		rpterr("i.t. overflow\n");
		asabort();
	}
	pitw->itty = itype;					/* type of it entry */

	/* put symbol in it buffer */
	if (itype == ITSY)
	{
		sp = lemt(FALSE, sirt);			/* look up it main table */
		pitw->itop.ptrw2 = sp;			/* ptr to symbol entry */
		if (sp == lmte)					/* first occurrance */
			mmte();
		itwc++;							/* count entries in it buffer */
		pitw++;
		if (!(sp->flags & SYER))		/* is it a register? */
			numsym[opdix]++;
		else if (sp->vl1.l)				/* yes, a register & not D0 */
			numreg[opdix] = sp->vl1.l;
		return;
	} else if (itype == ITCN)
	{
		if (ival.u.hiword && ival.u.hiword != -1)
			numcon[opdix] = 2;
		else if (!numcon[opdix])
			numcon[opdix] = 1;
		if (numops == 1)
			tryquick();
	}

/* special characters and constants */
	pitw->itop.p = ival.p;
	pitw->itrl = reloc;
	itwc++;
	pitw++;
}


/* change add into addq and sub into subq if possible */
VOID tryquick(NOTHING)
{
	register struct symtab *p;
	register long l;

	if (fchr != ',' || !immed[0])
		return;
	l = ival.l;
	if (itwc != ITOP1 + 1)
	{
		if (itwc != ITOP1 + 2 || stbuf[ITOP1 + 1].itty != ITSP || stbuf[ITOP1 + 1].itop.u.loword != '-')
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


VOID mystrncpy(P(char *) astr1, P(const char *) astr2, P(int) alen)
PP(char *astr1;)
PP(const char *astr2;)
PP(register int alen;)
{
	register char *str1;
	register const char *str2;

	str1 = astr1;
	str2 = astr2;
	while (--alen != -1)
		*str1++ = *str2++;
}


/*
 * index - find the index of a character in a string
 *      This is identical to Software Tools index.
 */
int strindex(P(const char *) str, P(char) chr)
PP(const char *str;)								/* pointer to string to search */
PP(char chr;)								/* character to search for */
{
	register const char *s;
	register short i;

	for (s = str, i = 0; *s != '\0'; i++)
		if (*s++ == chr)
			return i;
	return -1;
}




/*
 * these are here only to be able to compile & link with non-Alcyon compilers;
 * running the program will not work
 */

#ifndef __ALCYON__
#undef FILE
#undef getc
#undef putchar
#undef fopen
#undef open
#undef creat
int xgetc(struct iob *i)
{
	FILE *fp = *((FILE **)i);
	return fgetc(fp);
}
int xputc(char c, struct iob *o)
{
	FILE *fp = *((FILE **)o);
	return fputc(c, fp);
}
int xputw(int w, struct iob *i)
{
	xputc((w >> 8) & 0xff, i);
	xputc(w & 0xff, i);
	return w;
}
int xputwp(unsigned short *w, struct iob *i)
{
	return xputw(*w, i);
}
int xfopen(const char *fname, struct iob *i, int binary)
{
	FILE *fp;
	fp = fopen(fname, "r");
	if (fp)
	{
		*((FILE **)i) = fp;
		return 0;
	}
	return -1;
}
int xopen(const char *fname, int m, int binary)
{
	int fd;
	fd = open(fname, m);
	return fd;
}
int xcreat(const char *fname, int m, int binary)
{
	int fd;
	fd = creat(fname, m);
	return fd;
}
int xfcreat(const char *fname, struct iob *o, int binary)
{
	FILE *fp;
	fp = fopen(fname, "w");
	if (fp)
	{
		*((FILE **)o) = fp;
		return 0;
	}
	return -1;
}
#endif
