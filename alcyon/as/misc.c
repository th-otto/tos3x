/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* pass 2 miscellaneous routines */

#include "as68.h"
#include <errno.h>
#include <string.h>

int32_t stlen;

short debug = 0;

static short ftudp;
static short udfct;
static short hibytflg[4], hibytw[4];




VOID clrea(P(struct op *) ap)
PP(struct op *ap;)
{
	register struct op *p;

	p = ap;
	p->ea = p->len = p->xmod = p->drlc = 0;
	p->con = 0;
	p->ext = p->idx = -1;
}


/* get any register specification */
static int getrgs(NOTHING)
{
	register struct symtab *i;

	if (pitw->itty == ITSY)
	{
		i = pitw->itop.ptrw2;		/* symbol ptr */
		if (i->flags & SYER)
		{
			pitw++;
			return i->vl1;			/* register # */
		}
	}
	return -1;
}


/*
 * check for a right paren as the next char
 * output error msg if not found
 */
static VOID ckrparen(NOTHING)
{
	if (ckitc(pitw, ')'))				/* found it */
		pitw++;
	else
		uerr(32); /* missing ) */
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
	register int i;

	i = getrgs();
	if (i >= 0 && i <= AREGHI)
	{
		return i;
	} else
	{
		if (i != -1)
			pitw--;
		return -1;
	}
}


static VOID do_ireg(P(struct op *) p, P(int) i, P(int) opn)
PP(struct op *p;)
PP(int i;)
PP(int opn;)
{
	pitw++;
	p->idx = getreg();
	if (p->idx < 0 || p->idx > AREGHI)
		uerr(14); /* illegal index register */
	p->len = 2;
	if (!ckitc(pitw, ')'))
	{
		p->xmod = getrgs() - WORD_ID;
		if (p->xmod < 0 || p->xmod > 1)
		{
			uerr(34); /* illegal size */
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
static int getareg(NOTHING)
{
	register short i;

	i = getreg();
	if (i >= AREGLO && i <= AREGHI)
	{
		return i & 7;
	} else
	{
		if (i != -1)
			pitw--;
		return -1;
	}
}


/* get a special register token (CCR, SR, USP, SFC, DFC or VBR) */
static int gspreg(NOTHING)
{
	register short i;

	i = getrgs();
	if (i > AREGHI)
		return i;
	if (i != -1)
		pitw--;
	return 0;
}


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
	register short i, disp;
	register unsigned short inst;
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
					uerr(14);			/* illegal index register */
					return;
				}
				pitw--;
				goto dosimp;			/* must be expression in () */
			}
			p->ea = i & 7;				/* put in a reg # */
		}
		if (ckitc(pitw, ','))
		{								/* must be index reg # */
			do_ireg(p, i, opn);
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
			{
				p->ea |= INDIRECT;
			}
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
	i = gspreg();
	if (i != 0)
	{
		inst = ins[0];
		if (i == PC || (i == USP && inst != MOVE && inst != MOVEC))
			uerr(20);
		if (i == SR || i == CCR)
			if (inst != AND && inst != OR && inst != EOR && inst != ANDI
				&& inst != ORI && inst != EORI && inst != MOVE)
				uerr(20); /* illegal addressing mode */
		if ((i == SFC || i == DFC || i == VBR) && inst != MOVEC)
			uerr(20); /* illegal addressing mode */
		p->idx = i;
		ckeop(9 + opn);
#if 0
		if (itype == ITCW)
		{								/* 16-bit addrs */
			p->ea = SADDR;
			p->len = 2;
		} else if (itype == ITCN)
		{
			p->ea = LADDR;
			p->len = 4;
		}
#endif
		return;
	}
	i = getreg();
	if (i >= 0)
	{									/* register direct */
		p->ea = i;
		if (modelen == BYTESIZ && i >= AREGLO && i <= AREGHI)
			uerr(20); /* illegal addressing mode */
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
	p->con = ival.l;
	p->drlc = reloc;					/* relocation factor */
	if (ckitc(pitw, '('))
	{
		disp++;
		goto geteal1;
	}
	if (p->ea == 0)
	{									/* memory  address */
		if (pitw >= pnite)
			i = 0;
		else
			i = gspreg();
		if (i == WORD_ID)
			itype = ITCW;
		else if (i == LONG_ID)
			itype = ITCN;
		if (itype == ITCW)
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


/*
 * check intermediate text item for special character
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
	if (ckpt >= pnite || ckpt->itty != ITSP || ckpt->itop.oper != cksc)
		return 0;
	return 1;
}


static VOID doitrd(NOTHING)
{
	register int i;
	
	pitix = 0;
	i = fread(itbuf, 1, ITBSZ * sizeof(itbuf[0]), itfn);
	if (i <= 0)
	{
		fprintf(stderr, _("it read error: %s\n"), ferror(itfn) ? strerror(errno) : _("unexpected EOF"));
		asabort();
	}
}


/*
 * read intermediate text for one statement
 * returns:
 *	intermediate text in stbuf
 */
VOID ristb(NOTHING)
{
	register short riix;
	register unsigned int i;
	register short *pi;

	do
	{
		riix = stbuf[0].itrl;
		itoffset += (riix & 0xff) * sizeof(struct it);
		pi = (short *)stbuf;
		for (i = 0; i < (sizeof(struct it) / (sizeof *pi)); i++)
		{
			if (pitix >= ITBSZ)
			{							/* need new buffer full */
				doitrd();				/* read it buffer */
			}
			*pi++ = itbuf[pitix++];			/* first word of it */
		}
		if (stbuf[0].itty != ITBS)		/* best be beginning of statement */
		{
			rpterr(_("internal: not at beginning of stmt"));
			asabort();
		}
		
		/* get the rest of the statement it */
		riix = stbuf[0].itrl & 0xff;	/* unsigned byte */
		riix--;							/* already got first entry */
		while (riix--)
		{
			for (i = 0; i < (unsigned)(sizeof(struct it)) / (sizeof *pi); i++)
			{
				if (pitix >= ITBSZ)
				{
					doitrd();			/* get new buffer of it */
				}
				*pi++ = itbuf[pitix++];
			}
		}
		/* prstbuf("ristb"); */
	} while (stbuf[1].itrl == -1);		/* eliminated instr, read next one */
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
		return TRUE;
	if (!ckitc(pitw, ','))
	{									/* not end of stmt must be op,op */
		uerr(uen);
		return FALSE;
	}
	return TRUE;
}


/*
 * print undefined symbols
 * call with
 *	pointer to undefined symbol
 */
static VOID pudfs(P(struct symtab *) udspt)
PP(struct symtab *udspt;)
{
	nerror++;
	if (!ftudp)
	{									/* first time thru */
		fprintf(stderr, _("\n&& UNDEFINED SYMBOLS &&\n"));
		ftudp++;
		udfct = 0;						/* no symbols on this line */
	}

	fprintf(stderr, "%-*.*s  ", SYNAMLEN, SYNAMLEN, udspt->name);
	if (udfct++ > 6)
	{
		fputc('\n', stderr);
		udfct = 0;
	}
}


/*
 * output symbols in a form to be read by a debugger
 * call with pointer to symbol table entry
 * prints all undefined symbols
 */
static VOID osyme(P(struct symtab *) aosypt)
PP(struct symtab *aosypt;)
{
	register struct symtab *osypt;
	register char *p1;
	register short i;

	osypt = aosypt;						/* pointer to symbol table entry */
	if (!prtflg && !(osypt->flags & SYDF))
	{									/* undefined symbol */
		pudfs(osypt);					/* print undefined */
		return;
	}
	stlen += OSTSIZE;					/* one more symbol out */

	/* output symbol to loader file */
	p1 = osypt->name;
	for (i = 0; i < SYNAMLEN; i++)		/* output symbol name */
		putc(*p1++, lfil);

	lputw(&osypt->flags, lfil);			/* output symbol flags */
#ifdef DEBUG
	if (debug)							/* prints symbol table entries */
		printf("> %-*.*s* %o\n", SYNAMLEN, SYNAMLEN, osypt->name, osypt->flags);
#endif
	lputl(&osypt->vl1, lfil);			/* symbol value */
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
	for (p = bmte; p != emte; p = p->next)
	{									/* want them in order defined */
		if (p->flags & (SYXR | SYIN))
			continue;
		osyme(p);
	}
	if (udfct != 0)
	{
		fputc('\n', stderr);
		udfct = 0;
	}
	if (prtflg)							/* Printing? */
		psyms();						/* Yes, call ST print routine */
}


/* make all undefined symbols external */
VOID fixunds(NOTHING)
{
	register struct irts *sx1;
	register struct symtab *sx2;

	/* loop thru symbol initial reference table */
	for (sx1 = sirt; sx1 < &sirt[SZIRT]; sx1++)
	{
		if ((sx2 = sx1->irfe) == 0)		/* this chain is empty */
			continue;

		/* symbols on one chain	*/
		for (;;)
		{
			if (!(sx2->flags & SYDF))
			{							/* not defined */
				if (undflg || (sx2->flags & SYGL))
				{						/* all or globals */
					sx2->flags = sx2->flags | SYDF | SYXR;
					mkextidx(sx2);
				}
			}
			if (sx2 == sx1->irle)			/* end of chain */
				break;
			sx2 = sx2->tlnk;			/* next entry in chain */
		}
	}
}


VOID outbyte(P(int) bv, P(int) br)
PP(int bv;)
PP(int br;)
{
	if (rlflg == BSS)
	{
		uerr(39); /* code or data not allowed in bss */
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
		lputw(&val, lfil);
		lputw(&rb, trfil);
		break;

	case DATA:
		lputw(&val, dafil);
		lputw(&rb, drfil);
		break;

	case BSS:
		uerr(39); /* code or data not allowed in bss */
		break;

	default:
		rpterr(_("outword: bad rlflg"));
		asabort();
	}
}


VOID outinstr(NOTHING)
{
	register short i;
	register short *p1;
	register short *p2;

	i = instrlen >> 1;
	p1 = ins;
	p2 = rlbits;
	while (i--)
		outword(*p1++, *p2++);
}


/*
 * copy one of the temporary files to the loader file
 * call with:
 *	file descriptor of the temporary file
 *	last char of the temporary file name
 *	number of bytes to copy
 */
static VOID docp(P(FILE *) fp, P(long) length)
PP(FILE *fp;)
PP(long length;)
{
	register size_t j;

	fflush(lfil);
	fflush(fp);
	fseek(fp, 0L, SEEK_SET);
	while (length > 0)
	{
		if (length > (long)sizeof(itbuf))				/* 512 bytes at a time      */
			j = sizeof(itbuf);
		else
			j = length;					/* Last one                 */
		
		/* Read a buffer full */
		if (fread(itbuf, 1, j, fp) != j)
		{
			rpterr(_("Read error on Intermediate File"));
			asabort();
		}
		
		/* Now write buffer */
		if (fwrite(itbuf, 1, j, lfil) != j)
		{
			rpterr(_("Write error on Intermediate File"));
			asabort();
		}
		length -= j;					/* Decrement byte count     */
	}
}


/* copy data bits from temporary file to loader file */
VOID cpdata(NOTHING)
{
	docp(dafil, savelc[DATA]);
}


/* copy text then data relocation bits from temporary file to loader file */
VOID cprlbits(NOTHING)
{
	docp(trfil, savelc[TEXT]);
	docp(drfil, savelc[DATA]);
}


int ckcomma(NOTHING)
{
	if (ckitc(pitw, ','))
	{									/* next token a comma */
		pitw++;
		return TRUE;
	}
	return FALSE;
}


VOID chkimm(P(struct op *) ap)
PP(struct op *ap;)
{
	register struct op *p;
	register short h;
	
	p = ap;
	h = (short)(p->con >> 16);
	if (modelen == WORDSIZ)
	{									/* word */
		if (h != 0 && h != -1)
			uerr(42); /* illegal 16-bit immediate */
	} else if (modelen == BYTESIZ)
	{									/* byte */
		if (h != 0 && h != -1)
			uerr(43); /* illegal 8-bit immediate */
		h = (short)p->con;
		if (h > 255 || h <= -256)
			uerr(43); /* illegal 8-bit immediate */
	}
}


static VOID doupper(P(struct op *) p)
PP(struct op *p;)
{
	*pins++ = (short)(p->con >> 16);	/* upper half of long addr or constant */
	*prlb++ = LUPPER;
	instrlen += 2;
}


/* generate an immediate instr */
VOID genimm(NOTHING)
{
	ins[0] |= f2mode[modelen] | opnd[1].ea;
	if (modelen == LONGSIZ)
	{
		doupper(&opnd[0]);
		opnd[0].con &= 0xffffL;			/* clear for dodisp check */
	}
	chkimm(&opnd[0]);					/* check for valid immed length */
	dodisp(&opnd[0]);
	doea(&opnd[1]);
}


VOID dodisp(P(struct op *) ap)
PP(struct op *ap;)
{
	register struct op *p;
	short h;
	
	p = ap;
	*pins++ = (short)p->con;			/* displacement */
	h = (short)(p->con >> 16);
	if (h != 0 && h != -1)
		uerr(41);						/* invalid 16-bit disp */
	*prlb++ = (p->ext != -1) ? (p->ext << 3) | EXTVAR : p->drlc;
	instrlen += 2;
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
		if (p->con > 127L || p->con < -128L)
			uerr(35); /* illegal 8-bit displacement */
		i = ((short)p->con & 0xff) | (p->idx << 12) | (p->xmod << 11);
		if (p->drlc != ABS)
			uerr(27); /* relocation error */
		*pins++ = i;
		*prlb++ = DABS;
		instrlen += 2;
		return;

	case 7:							/* xxx.W, xxx.L, or #xxx */
		switch (p->ea & 7)
		{
		case 1:						/* xxx.L */
			doupper(p);
			p->con &= 0xffffL;		/* clear for dodisp check */
			/* fall through */

		case 0:						/* xxx.W */
			dodisp(p);
			return;

		case 2:						/* d(PC) */
		case 3:						/* d(PC,Ri.X) */
			if (p->drlc != ABS)
			{
				if (p->drlc != rlflg)	/* not same reloc base */
				{
					if (rlflg == TEXT && p->drlc == DATA && p->ext == -1)
					{
						p->con -= (loctr + instrlen);
						p->con += couthd.ch_tsize;
					} else if (rlflg == TEXT && p->drlc == BSS && p->ext == -1)
					{
						p->con -= (loctr + instrlen);
						p->con += couthd.ch_tsize;
						p->con += couthd.ch_dsize;
					} else
					{
						uerr(27); /* relocation error */
					}
				} else
				{
					p->con -= (loctr + instrlen);
				}
				p->drlc = ABS;
				if (p->con < -32768L || p->con > 32767L ||
					((p->ea & 7) == 3 &&
					 (p->con < -128L || p->con > 127L)))
					uerr(27); /* relocation error */
			}
			if ((p->ea & 7) == 3)		/* d(PC,Ri.X) */
				goto dindx;
			dodisp(p);
			return;

		case 4:							/* #xxx */
			chkimm(p);					/* check for valid length */
			if (modelen == LONGSIZ)
			{							/* instr mode is long */
				doupper(p);
				p->con &= 0xffffL;		/* clear for dodisp check */
			}
			dodisp(p);
			return;
		}								/* switch in case 7 */
	}
}


/* try to check displacement range */
VOID cksize(P(struct op *) ap)
PP(struct op *ap;)
{
	int32_t value;

	if ((ap->ea & 070) != 070)
		return;
	value = (ap->con > 0 && (ap->con & 0x8000)) ? -(ap->con & ~0x8000) : ap->con;
	if (modelen == BYTESIZ)
	{
		if (value < -128L || value > 255L)	/* 8 bits */
			uerr(35); /* illegal 8-bit displacement */
	} else if (modelen == WORDSIZ)
	{
		if (value > 32767L || value < -32768L)	/* 16 bits */
			uerr(41);
	}
}
