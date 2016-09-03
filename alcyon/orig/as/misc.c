/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* pass 2 miscellaneous routines */

#include "as68.h"
#include "../util/util.h"

long stlen;

short stdofd = STDOUT;

short debug = 0;

short udfct, ftudp, pline, prsp;

VOID do_ireg PROTO((struct op *p, int i, int opn));
int getareg PROTO((NOTHING));
int getrgs PROTO((NOTHING));
VOID ckrparen PROTO((NOTHING));
VOID doitrd PROTO((NOTHING));
VOID osyme PROTO((struct symtab *aosypt));
VOID pudfs PROTO((struct symtab *udspt));
VOID docp PROTO((int cfn, int cfnc, long length));
VOID chkimm PROTO((struct op *ap));
int gspreg PROTO((NOTHING));
VOID doupper PROTO((struct op *p));





VOID clrea(P(struct op *) ap)
PP(struct op *ap;)
{
	register struct op *p;

	p = ap;
	p->ea = p->len = p->xmod = p->drlc = 0;
	p->con.l = 0;
	p->ext = p->idx = -1;
}


#define US	(unsigned short)
/*
 * get one operand effective adddress (operand until , or EOS)
 * returns:
 *	opnd[opn].ea set to effective address mode bits
 *	opnd[opn].len set to # bytes for operand
 *	opnd[opn].con set to constant part of ea
 *	opnd[opn].ext set to external symbol # if any
 *	opnd[opn].idx set to index register if any
 *	opnd[opn].drlc set to effective address relocation mode
 *	opnd[opn].xmod set to index register addressing mode (word or long)
 */
VOID getea(P(int) opn)
PP(int opn;)
{
	register short i, disp, inst;
	register struct op *p;

	p = &opnd[opn];
	disp = 0;
	clrea(p);
	if (ckitc(pitw, '#'))
	{
		p->len = (modelen == BYTESIZ) ? WORDSIZ : modelen;
		p->ea = IMM;
		pitw++;
		goto dosimp;
	}
	if (ckitc(pitw, '('))
	{
	  geteal1:
		pitw++;
		if ((i = getrgs()) == PC)
		{								/* pc relative */
			p->ea = 072;				/* set mode & register bits */
			p->len = 2;
		} else
		{
			if (i != -1)				/* last was some type of register */
				pitw--;					/* havent used it yet */
			if ((i = getareg()) < 0)
			{							/* not a reg # next */
				if (disp || getreg() != -1)
				{
					uerr(14);			/* illegal index reg */
					return;
				}
				pitw--;
				goto dosimp;			/* must be expression in () */
			}
			p->ea = i & 7;				/* put in a reg # */
		}
		if (ckitc(pitw, ','))
		{								/* must be index reg # */
#ifdef __ALCYON__
			do_ireg(p, i);		/* BUG: parameter opn not passed */
#else
			do_ireg(p, i, opn);
#endif
			return;
		}
		ckrparen();
		if (i != PC)
		{
			if (!disp && ckitc(pitw, '+'))
			{
				pitw++;
				p->ea |= INDINC;
			} else if (disp)
			{							/* indirect with displacement */
				p->ea |= INDDISP;
				p->len = 2;
			} else
				p->ea |= INDIRECT;
		}
		ckeop(9 + opn);
		return;
	}
	if (ckitc(pitw, '-'))
	{									/* predecrement maybe */
		pitw++;
		if (ckitc(pitw, '('))
		{								/* must be */
			pitw++;
			if ((i = getareg()) < 0)
			{							/* not valid a reg */
				pitw -= 2;				/* must be negative expr */
				goto dosimp;
			}
			p->ea = i | DECIND;
			ckrparen();
			ckeop(9 + opn);
			return;
		}
		pitw--;
	}
  dosimp:								/* simple addr or imm expr */
	if ((i = gspreg())) /* XXX */
	{
		inst = ins[0].w;
		if (i == PC || (i == USP && inst != MOVE && inst != MOVEC))
			uerr(20);
		if (i == SR || i == CCR)
			if (inst != (US AND) && inst != (US OR) && inst != (US EOR) && inst != ANDI
				&& inst != ORI && inst != EORI && inst != MOVE)
				uerr(20);
		if ((i == SFC || i == DFC || i == VSR) && inst != MOVEC)
			uerr(20);
		p->idx = i;
		ckeop(9 + opn);
		return;
	}
	if ((i = getreg()) >= 0)
	{									/* register direct */
		p->ea = i;
		if (modelen == BYTESIZ && i >= AREGLO && i <= AREGHI)
			uerr(20);
		ckeop(9 + opn);
		return;
	}
	expr(p2gi);
	if (pitw < pnite)					/* expr passes one token */
		pitw--;
	if (extflg)
	{
		p->ext = extref;
		extflg = 0;
	}
	p->con.l = ival.l;
	p->drlc = reloc;					/* relocation factor */
	if (ckitc(pitw, '('))
	{
		disp++;
		goto geteal1;
	}
	if (!p->ea)
	{									/* memory  address */
		if (shortadr && (!ival.u.hiword || ival.u.hiword == -1))
		{								/* 16-bit addrs */
			p->ea = SADDR;
			p->len = 2;
		} else
		{
			p->ea = LADDR;
			p->len = 4;
		}
	}
	ckeop(9 + opn);
}


VOID do_ireg(P(struct op *) p, P(int) i, P(int) opn)
PP(struct op *p;)
PP(int i;)
PP(int opn;) /* BUG: never passed */
{
	pitw++;
	p->idx = getreg();
	if (p->idx < 0 || p->idx > AREGHI)
		uerr(14);
	p->len = 2;
	if (!ckitc(pitw, ')'))
	{
		p->xmod = getrgs() - WORD_ID;
		if (p->xmod < 0 || p->xmod > 1)
		{
			uerr(34);
			p->xmod = 0;
		}
	}
	ckrparen();
	ckeop(9 + opn);
	if (i == PC)
		p->ea += 1;
	else
		p->ea |= INDINX;
}


/*
 * get an A register specification
 *  call with:
 *		pitw pointing to reg operand
 *  returns:
 *		-1 if not vaid A reg
 *		A reg # if valid
 *		also updates pitw if valid
 */
int getareg(NOTHING)
{
	register short i;

	i = getreg();
	if (i >= AREGLO && i <= AREGHI)
	{
		return (i & 7);
	} else
	{
		if (i != -1)
			pitw--;
		return (-1);
	}
}


/*
 * get any register specification
 *  call with :
 *		pitw pointing at operand
 *  returns:
 *		register # with pitw updated
 *		-1 if not valid register
 */
int getreg(NOTHING)
{
	register short i;

	i = getrgs();
	if (i >= 0 && i <= AREGHI)
		return (i);
	else
	{
		if (i != -1)
			pitw--;
		return (-1);
	}
}


/* get any register specification */
int getrgs(NOTHING)
{
	register struct symtab *i;

	if (pitw->itty == ITSY)
	{
		i = pitw->itop.ptrw2;			/* symbol ptr */
		if (i->flags & SYER)
		{
			pitw++;
			return i->vl1.u.loword;		/* register # */
		}
	}
	return -1;
}


/*
 * check for a right paren as the next char
 * output error msg if not found
 */
VOID ckrparen(NOTHING)
{
	if (ckitc(pitw, ')'))				/* found it */
		pitw++;
	else
		uerr(32);
}


/*
 * check intermedate text item for special character
 *	call with:
 *		pointer to desired item in stbuf
 *		character to check for
 *	returns:
 *		0 => no match
 *		1 => match
 */
int ckitc(P(const struct it *) ckpt, P(int) cksc)
PP(const struct it *ckpt;)
PP(int cksc;)
{
	if (ckpt >= pnite || ckpt->itty != ITSP || ckpt->itop.u.loword != cksc)
		return (0);
	return (1);
}


/*
 * read intermediate text for one statement
 * returns:
 *	intermediate text in stbuf
 */
VOID ristb(NOTHING)
{
	register short riix, i;
	register short *pi;

	do
	{
		riix = stbuf[0].itrl;
		itoffset += (riix & 0377) * STBFSIZE;
		pi = (short *)&stbuf[0];
		for (i = 0; i < (STBFSIZE) / (sizeof *pi); i++)
		{
			if (pitix > &itbuf[ITBSZ - 1])
			{							/* need new buffer full */
				doitrd();				/* read it buffer */
			}
			*pi++ = *pitix++;			/* first word of it */
		}
		if (stbuf[0].itty != ITBS)		/* best be beginning of statement */
			asabort();

		/* get the rest of the statement it */
		riix = stbuf[0].itrl & 0377;	/* unsigned byte */
		riix--;							/* already got first entry */
		while (riix--)
		{
			for (i = 0; i < (STBFSIZE) / (sizeof *pi); i++)
			{
				if (pitix > &itbuf[ITBSZ - 1])
				{
					doitrd();			/* get new buffer of it */
				}
				*pi++ = *pitix++;
			}
		}
	} while (stbuf[1].itrl == -1);		/* eliminated instr, read next one */
}

int errno; /* WTF ? */

VOID doitrd(NOTHING)
{
	register short i;

	pitix = itbuf;
	if ((i = read(itfn, itbuf, ITBSZ * (sizeof i))) <= 0)
	{
		fprintf(stderr, "it read error i=%d errno=%o\n", i, errno);
		asabort();
	}
}

/*
 * check for end of operand
 * call with
 *		error number if this is not end of operand
 */
int ckeop(P(int) uen)
PP(int uen;)
{
	if (pitw >= pnite)					/* end of all operands */
		return (1);
	if (!ckitc(pitw, ','))
	{									/* not end of stmt must be op,op */
		uerr(uen);
		return (0);
	}
	return (1);
}


/* output symbol table to file */
VOID osymt(NOTHING)
{
	register struct symtab **sx1;
	register struct symtab *p;
	register short i;

	stlen = 0;
	if (extindx)
	{									/* output external symbols first */
		sx1 = extbl;
		for (i = 0; i < extindx; i++)	/* go through external table */
			osyme(*sx1++);				/* output symbol */
	}
	for (p = bmte; p < lmte; p++)
	{									/* want them in order defined */
		if (p->flags & (SYXR | SYIN))
			continue;
		osyme(p);
	}
	if (prtflg)							/* Printing? */
		psyms();						/* Yes, call ST print routine */
}


/* make all undefined symbols external */
VOID fixunds(NOTHING)
{
	register struct symtab **sx1, **sx2;

	/* loop thru symbol initial reference table */
	for (sx1 = sirt; sx1 < &sirt[SZIRT - 1]; sx1 += 2)
	{
		if (*(sx2 = sx1 + 1) == 0)		/* this chain is empty */
			continue;

		/* symbols on one chain	*/
		sx2 = (struct symtab **)*sx2;						/* first entry on this chain */
		while (1)
		{
			if (!(((struct symtab *)sx2)->flags & SYDF))
			{							/* not defined */
				if (undflg || ((struct symtab *)sx2)->flags & SYGL)
				{						/* all or globals */
					((struct symtab *)sx2)->flags = ((struct symtab *)sx2)->flags | SYDF | SYXR;
					mkextidx(((struct symtab *)sx2));
				}
			}
			if (((struct symtab *)sx2) == *sx1)			/* end of chain */
				break;
			sx2 = (struct symtab **)(((struct symtab *)sx2)->tlnk);			/* next entry in chain */
		}
	}
}


/*
 * output symbols in a form to be read by a debugger
 * call with pointer to symbol table entry
 * prints all undefined symbols
 */
VOID osyme(P(struct symtab *) aosypt)
PP(struct symtab *aosypt;)
{
	register struct symtab *osypt;
	register char *p1;
	register short i;
	long lsyval;

	osypt = aosypt;						/* pointer to symbol table entry */
	if (!prtflg && !(osypt->flags & SYDF))
	{									/* undefined symbol */
		pudfs(osypt);					/* print undefined */
		return;
	}
	stlen += 14;						/* one more symbol out */

	/* output symbol to loader file */
	p1 = &(osypt->name[0]);
	for (i = 0; i < SYNAMLEN; i++)		/* output symbol name */
		putc(*p1++, &lbuf);

	lputw(&osypt->flags, &lbuf);		/* output symbol flags */
#ifdef DEBUG
	if (debug)							/* prints symbol table entries */
		printf("> %-8.8s* %o\n", osypt->name, (int) osypt->flags);
#endif
	lputl(&osypt->vl1.l, &lbuf);			/* symbol value */
	UNUSED(lsyval);
}


/*
 * print undefined symbols
 * call with
 *	pointer to undefined symbol
 */
VOID pudfs(P(struct symtab *) udspt)
PP(struct symtab *udspt;)
{
	nerror++;
	if (!ftudp)
	{									/* first time thru */
		printf("\n&& UNDEFINED SYMBOLS &&\n");
		ftudp++;
		udfct = 0;						/* no symbols on this line */
	}

	printf("%-8.8s  ", &(udspt->name[0]));
	if (udfct++ > 6)
	{
		printf("\n");
		udfct = 0;
	}
}


#if 0
/* buffered putchar routine */
VOID putchar(P(char) c)
PP(char c;)
{
	register short i;

	*prtchidx++ = c;
	if (!c || prtchidx >= &prtchars[PRTCHLEN])
	{
		i = (short) (prtchidx - prtchars);
		write(stdofd, prtchars, i);
		prtchidx = prtchars;
	}
}
#endif


short hibytflg[4], hibytw[4];


VOID outbyte(P(int) bv, P(int) br)
PP(int bv;)
PP(int br;)
{
	if (rlflg == BSS)
	{
		uerr(39);
		return;
	}
	if (hibytflg[rlflg])
	{
		outword(hibytw[rlflg] | (bv & 0xff), br);
		hibytflg[rlflg] = 0;
	} else
	{
		hibytw[rlflg] = bv << 8;
		hibytflg[rlflg]++;
	}
}


VOID outword(P(unsigned short) val, P(unsigned short) rb)
PP(unsigned short val;)
PP(unsigned short rb;)
{
	switch (rlflg)
	{
	case TEXT:
		lputw(&val, &lbuf);
		lputw(&rb, &tbuf);
		break;

	case DATA:
		lputw(&val, &dabuf);
		lputw(&rb, &drbuf);
		break;

	case BSS:
		uerr(39);
		break;

	default:
		rpterr("& outword: bad rlflg\n");
		asabort();
	}
}


VOID outinstr(NOTHING)
{
	register short i;
	register union insw *p1;
	register short *p2;

	i = instrlen >> 1;
	p1 = ins;
	p2 = rlbits;
	while (i--)
		outword((p1++)->w, *p2++);

}


/* copy data bits from temporary file to loader file */
VOID cpdata(NOTHING)
{
	xfflush(&lbuf);
	xfflush(&dabuf);
	docp(dafn, dafnc, savelc[DATA]);
}


/* copy text then data relocation bits from temporary file to loader file */
VOID cprlbits(NOTHING)
{
	xfflush(&lbuf);
	xfflush(&drbuf);
	docp(trbfn, trbfnc, savelc[TEXT]);
	docp(drbfn, drbfnc, savelc[DATA]);
}


/*
 * copy one of the temporary files to the loader file
 * call with:
 *	file descriptor of the temporary file
 *	last char of the temporary file name
 *	number of bytes to copy
 */
VOID docp(P(int) cfn, P(int) cfnc, P(long) length)
PP(int cfn;)
PP(int cfnc;)
PP(long length;)
{
	register short i;
	register short j;

	close(cfn);
	LASTCHTFN = cfnc;
	cfn = openfi(tfilname, 0, 1);
	while (length > 0)
	{
		if (length > BLEN)				/* 512 bytes at a time      */
			j = BLEN;
		else
			j = length;					/* Last one                 */
		
		/* Read a buffer full */
		if ((i = read(cfn, itbuf, j)) != j)
		{
			rpterr("& Read error on Intermediate File\n");
			asabort();
		}
		
		/* Now write buffer */
		if (write(lfn, itbuf, i) != i)
		{
			rpterr("& Write error on Intermediate File\n");	/*              */
			asabort();
		}
		length -= j;					/* Decrement byte count     */
	}
}


/* print one word in hex */
VOID puthex(P(int) v, P(int) l)
PP(int v;)
PP(int l;)
{
	register short i, j, k;

	j = 12;
	for (i = 0; i < l; i++)
	{
		k = (v >> j) & 017;
		k += (k >= 10) ? ('A' - 10) : '0';
		putchar(k);
		j -= 4;
	}
}


/* check for a control operand */
int controlea(P(struct op *) ap)
PP(struct op *ap;)
{
	register short i;

	i = ap->ea & 070;
	if (i == INDIRECT || i == INDDISP || i == INDINX)
		return (1);
	if (i == 070)
	{
		if ((ap->ea & 7) <= 3)
			return (1);
	}
	return (0);
}


int ckcomma(NOTHING)
{
	if (ckitc(pitw, ','))
	{									/* next token a comma */
		pitw++;
		return (1);
	}
	return (0);
}


/*
 * generate any necessary additional words for the effective address
 *  call with:
 *		pins pointing to next available word in ins[]
 *		prlb pointing to next available word in rlbits[]
 *		argument is ptr to op structure
 *
 * returns:
 *		appropriate words in ins[] and rlbits[] for operand
 *		pins and prlb updated.
 */
VOID doea(P(struct op *) apea)
PP(struct op *apea;)
{
	register short i;
	register struct op *p;

	p = apea;
	switch ((p->ea >> 3) & 7)
	{									/* ea mode bits */
	default:							/* no more words */
		return;

	case 5:							/* d(An) */
		dodisp(p);
		return;

	case 6:							/* d(An,Ri) */
	  dindx:
		if (p->con.l > 255L || p->con.l < -128L)
			uerr(35);
		i = (p->con.u.loword & 0377) | (p->idx << 12) | (p->xmod << 11);
		if (p->drlc != ABS)
			uerr(27);
		(pins++)->w = i;
		*prlb++ = DABS;
		instrlen += 2;
		return;

	case 7:							/* xxx.W, xxx.L, or #xxx */
		switch (p->ea & 7)
		{
		case 1:						/* xxx.L */
			doupper(p);
			p->con.u.hiword = 0;			/* clear for dodisp check */

		case 0:						/* xxx.W */
			dodisp(p);
			return;

		case 2:						/* d(PC) */
		case 3:						/* d(PC,Ri.X) */
			if (p->drlc != ABS)
			{
				if (p->drlc != rlflg)	/* not same reloc base */
					uerr(27);
				p->con.l -= (loctr + instrlen);
				p->drlc = ABS;
			}
			if ((p->ea & 7) == 3)		/* d(PC,Ri.X) */
				goto dindx;
			dodisp(p);
			return;

		case 4:						/* #xxx */
			chkimm(p);					/* check for valid length */
			if (modelen == LONGSIZ)
			{							/* instr mode is long */
				doupper(p);
				p->con.u.hiword = 0;		/* clear for dodisp check */
			}
			dodisp(p);
			return;
		}								/* switch in case 7 */
	}
}


VOID dodisp(P(struct op *) ap)
PP(struct op *ap;)
{
	register struct op *p;

	p = ap;
	(pins++)->w = p->con.u.loword;			/* displacement */
	if (p->con.u.hiword && p->con.u.hiword != -1)
		uerr(41);						/* invalid 16-bit disp */
	*prlb++ = (p->ext != -1) ? (p->ext << 3) | EXTVAR : p->drlc;
	instrlen += 2;
}


VOID doupper(P(struct op *) p)
PP(struct op *p;)
{
	(pins++)->w = p->con.u.hiword;			/* upper half of long addr or constant */
	*prlb++ = LUPPER;
	instrlen += 2;
}


/*
 * build a format 1 (add, sub, and, etc) instr
 * call with:
 *	register #
 *	mode bits
 *	ptr to operand structure for effective address
 */
VOID makef1(P(int) arreg, P(int) armode, P(struct op *) apea)
PP(int arreg;)
PP(int armode;)
PP(struct op *apea;)
{
	register struct op *p;

	p = apea;
	ins[0].w |= (arreg << 9);				/* put in reg # */
	ins[0].w |= armode;					/* instr mode bits */
	ins[0].w |= p->ea;					/* put in effective addr bits */
	doea(p);							/* may be more words in ea */
}


/* generate an immediate instr */
VOID genimm(NOTHING)
{
	ins[0].w |= (f2mode[modelen] | opnd[1].ea);
	if (modelen == LONGSIZ)
	{
		doupper(&opnd[0]);
		opnd[0].con.u.hiword = 0;			/* clear for dodisp check */
	}
	chkimm(&opnd[0]);					/* check for valid immed length */
	dodisp(&opnd[0]);
	doea(&opnd[1]);
}


VOID chkimm(P(struct op *) ap)
PP(struct op *ap;)
{
	register struct op *p;

	p = ap;
	if (modelen == WORDSIZ)
	{									/* word */
		if (p->con.u.hiword && p->con.u.hiword != -1)
			uerr(42);
	} else if (modelen == BYTESIZ)
	{									/* byte */
		if (p->con.u.hiword && p->con.u.hiword != -1)
			uerr(43);
		if (p->con.u.loword > 255 || p->con.u.loword <= -256)
			uerr(43);
	}
}


/* try to make a normal instr into an immediate instr */
int makeimm(NOTHING)
{
	if (opnd[0].ea != IMM)
		return (0);
	if (!dataalt(&opnd[1]))
		return (0);
	if (opcpt == addptr)
		opcpt = addiptr;
	else if (opcpt == andptr)
		opcpt = andiptr;
	else if (opcpt == orptr)
		opcpt = oriptr;
	else if (opcpt == subptr)
		opcpt = subiptr;
	else if (opcpt == cmpptr)
		opcpt = cmpiptr;
	else if (opcpt == eorptr)
		opcpt = eoriptr;
	else
		return (0);
	ins[0].w = opcpt->vl1.u.loword;
	format = opcpt->flags & OPFF;
	genimm();
	return (1);
}


VOID ckbytea(NOTHING)
{
	if (modelen == BYTESIZ && !dataea(&opnd[0]))
		uerr(20);						/* byte mod not allowed */
}


/* get a special register token (CCR, SR, USP, SFC, DFC or VSR) */
int gspreg(NOTHING)
{
	register short i;

	i = getrgs();
	if (i > AREGHI)
		return (i);
	if (i != -1)
		pitw--;
	return (0);
}


/*
 * check an operand for a special register
 * call with:
 *  ptr to operand struct
 *  special register value
 */
int cksprg(P(struct op *) ap, P(int) v1)
PP(struct op *ap;)
PP(int v1;)
{
	if (ap->ea)
		return (0);
	return ((ap->idx == v1));
}


/* check for operand as any special register */
int anysprg(P(struct op *) ap)
PP(struct op *ap;)
{
	if (ap->ea)
		return (0);
	if (ap->idx >= CCR && ap->idx <= USP)
		return (1);
	return (0);
}


/* copy opnd 0 to opnd 1 */
VOID cpop01(NOTHING)
{
	opnd[1].ea = opnd[0].ea;
	opnd[1].len = opnd[0].len;
	opnd[1].con.l = opnd[0].con.l;
	opnd[1].drlc = opnd[0].drlc;
	opnd[1].ext = opnd[0].ext;
	opnd[1].idx = opnd[0].idx;
	opnd[1].xmod = opnd[0].xmod;
}


/* try to check displacement range */
VOID cksize(P(struct op *) ap)
PP(struct op *ap;)
{
	long value;

	if ((ap->ea & 070) != 070)
		return;
	value = (ap->con.l > 0 && ap->con.l & 0100000) ? -(ap->con.l & ~0100000) : ap->con.l;
	if (modelen == BYTESIZ)
	{
		if (value < -128L || value > 255L)	/* 8 bits */
			uerr(35);
	} else if (modelen == WORDSIZ)
	{
		if (value > 32767L || value < -32768L)	/* 16 bits */
			uerr(41);
	}
}


VOID ccr_or_sr(NOTHING)
{
	if (opnd[1].idx == CCR)
	{
		modelen = BYTESIZ;				/* byte mode only */
	} else if (modelen != WORDSIZ)
	{
		modelen = WORDSIZ;
		uerr(34);
	}
	cksize(&opnd[0]);
	ins[0].w |= IMM | f2mode[modelen];
	dodisp(&opnd[0]);
}


int get2ops(NOTHING)
{
	getea(0);							/* get first effective address */
	if (!ckcomma())
	{
		uerr(10);
		return (1);						/* no second op */
	}
	getea(1);							/* get second effective address */
	return (0);
}


int xfflush(P(FILE *) ibuf)
PP(register FILE *ibuf;)
{
	register short i;

	i = BLEN - ibuf->cc;
	ibuf->cc = BLEN;
	ibuf->cp = &(ibuf->cbuf[0]);
	if (write(fileno(ibuf), ibuf->cp, i) != i)
		return EOF;
	return 0;
}
