/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)pass2.c	1.5	12/16/83
*/

/*
 * pass two for the 68000 assembler
 *		Bill Allen
 *		March 1980
 */

#include "as68.h"
#include <ar68.h>
#include <cout.h>



static short const f1mode[]  = { 0,      0,   0100, 0,   0200 };
       short const f2mode[]  = { 0,      0,   0100, 0,   0200 };
static short const f3mode[]  = { 0, 010000, 030000, 0, 020000 };
static short const f15mode[] = { 0,      0,   0300, 0,   0700 };
static short const f5mode[]  = { 0,      0,   0100, 0,   0200 };
static short const f5amode[] = { 0,      0,   0300, 0,   0700 };
static short const f13mode[] = { 0,      0,   0200, 0,   0300 };
static short const f23mode[] = { 0,   0400,   0500, 0,   0600 };
short rlbits[5];		/* holds relocation bits for instr */


/* format 20 -- movem */
static short const regmsk0[] = {
	0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100,
	0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001
};

static short const regmsk1[] = {
	0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
	0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
};



/* relative branches */
static VOID relbr(NOTHING)
{
	expr(p2gi);
	if (extflg)
	{									/* external reference */
		instrlen += 2;					/* long relative */
		*pins++ = ival.l;			/* pass constant part */
		*prlb++ = (extref << 3) | EXTREL;	/* ext ref */
		return;
	}
	ival.l -= (loctr + 2);				/* calc relative offset */
	if (itype != ITCN || reloc != rlflg)
	{
		uerr(22);						/* invalid relative branch */
		ival.p = 0;
	}
	reloc = ABS;
	if (p1inlen == 4)
	{									/* long displacement */
		if (ival.l > 32767 || ival.l < -32768)
			uerr(22);	/* illegal relative address */
		instrlen += 2;
		*pins++ = ival.l;
		*prlb++ = DABS;					/* data absolute */
	} else
	{									/* short displacement */
		if (ival.l > 127 || ival.l < -128)
			uerr(22); /* illegal relative address */
		ins[0] |= ((short)ival.l & 0xff);
	}
	/* make it a nop if -N specified */
	if ((ival.l == 0) || (ival.l == 2 && didorg))
	{
		opcpt = nopptr;
		ins[0] = opcpt->vl1;
		if (instrlen == 4)
		{								/* long branch */
			pins = &ins[1];
			*pins++ = opcpt->vl1;
			rlbits[1] = INSABS;
		}
	}
	in_err++;							/* ignore extra eg. bra *+$d04(pc) vs. bra *+d04 */
}


/*
 * build a format 1 (add, sub, and, etc) instr
 * call with:
 *	register #
 *	mode bits
 *	ptr to operand structure for effective address
 */
static VOID makef1(P(int) arreg, P(int) armode, P(struct op *) apea)
PP(int arreg;)
PP(int armode;)
PP(register struct op *apea;)
{
	ins[0] |= (arreg << 9);				/* put in reg # */
	ins[0] |= armode;					/* instr mode bits */
	ins[0] |= apea->ea;					/* put in effective addr bits */
	doea(apea);							/* may be more words in ea */
}


/* try to make a normal instr into an immediate instr */
static int makeimm(NOTHING)
{
	if (opnd[0].ea != IMM)
		return FALSE;
	if (!dataalt(&opnd[1]))
		return FALSE;
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
		return FALSE;
	ins[0] = opcpt->vl1;
	format = opcpt->flags & OPFF;
	genimm();
	return TRUE;
}


/*
 * check an operand for a special register
 * call with:
 *  ptr to operand struct
 *  special register value
 */
static int cksprg(P(struct op *) ap, P(int) v1)
PP(struct op *ap;)
PP(int v1;)
{
	if (ap->ea)
		return 0;
	return ap->idx == v1;
}


static VOID ccr_or_sr(NOTHING)
{
	if (opnd[1].idx == CCR)
	{
		modelen = BYTESIZ;				/* byte mode only */
	} else if (modelen != WORDSIZ)
	{
		modelen = WORDSIZ;
		uerr(34); /* illegal size */
	}
	cksize(&opnd[0]);
	ins[0] |= IMM | f2mode[modelen];
	dodisp(&opnd[0]);
}


static int get2ops(NOTHING)
{
	getea(0);							/* get first effective address */
	if (!ckcomma())
	{
		uerr(10); /* invalid second operand */
		return TRUE;						/* no second op */
	}
	getea(1);							/* get second effective address */
	return FALSE;
}


/*
 * format one -- add, sub, and, or, cmp, etc.
 * one operand must be a D reg (or A reg dest for add, sub, or cmp)
 */
static VOID opf1(NOTHING)
{
	register const short *p;

	if (get2ops())
		return;
	if (ins[0] == AND || ins[0] == OR)
	{
		if (cksprg(&opnd[1], CCR) || cksprg(&opnd[1], SR))
		{
			if (ins[0] == AND)
				opcpt = andiptr;
			else
				opcpt = oriptr;
			ins[0] = opcpt->vl1;
			format = opcpt->flags & OPFF;
			ccr_or_sr();
			return;
		}
	}
	p = f1mode;
	if (ckdreg(&opnd[1]))
	{									/* destn is D reg */
		if ((opcpt == andptr || opcpt == orptr) && ckareg(&opnd[0]))	/* A source */
			uerr(20); /* illegal addressing mode */
		makef1(opnd[1].ea, p[modelen], &opnd[0]);	/* make instr */
		return;
	} else if (ckdreg(&opnd[0]) && memalt(&opnd[1]))
	{									/* source is D reg */
		if (pcea(&opnd[1]))
			uerr(10); /* invalid second operand */
		makef1(opnd[0].ea, p[modelen] + 0400, &opnd[1]);
		return;
	} else if (ckareg(&opnd[1]))
	{									/* A reg is dstn */
		if (opcpt == addptr)
			opcpt = addaptr;
		else if (opcpt == cmpptr)
			opcpt = cmpaptr;
		else if (opcpt == subptr)
			opcpt = subaptr;
		else
		{
			uerr(20); /* illegal addressing mode */
			return;
		}
		format = opcpt->flags & OPFF;
		opnd[1].ea &= 07;
		p = f15mode;
		makef1(opnd[1].ea, p[modelen], &opnd[0]);	/* make instr */
		return;
	} else if (!makeimm())				/* make an immediate instr */
	{
		uerr(20); /* illegal addressing mode */
	}
}


/* format 2 -- addi, andi, subi, etc */
static VOID opf2(NOTHING)
{
	if (get2ops())
		return;
	if (ins[0] == ANDI || ins[0] == ORI || ins[0] == EORI)
	{
		if (cksprg(&opnd[1], CCR) || cksprg(&opnd[1], SR))
		{
			ccr_or_sr();
			return;
		}
	}
	if (opnd[0].ea != IMM)
	{
		uerr(9); /* invalid first operand */
		return;
	}
	if (!dataalt(&opnd[1]) || pcea(&opnd[1]))
	{
		uerr(20); /* illegal addressing mode */
		return;
	}
	genimm();
}


static VOID ckbytea(NOTHING)
{
	if (modelen == BYTESIZ && !dataea(&opnd[0]))
		uerr(34);						/* byte mod not allowed */
}


/* check for operand as any special register */
static int anysprg(P(struct op *) ap)
PP(struct op *ap;)
{
	if (ap->ea)
		return FALSE;
	if (ap->idx >= CCR && ap->idx <= USP)
		return TRUE;
	return FALSE;
}


#define MOVEA   0100
/* format #3 -- move and movea */
static VOID opf3(NOTHING)
{
	register short k;

	if (get2ops())
		return;
	if (cksprg(&opnd[0], CCR))
	{
		ins[0] = MOVEFCC;
		if (anysprg(&opnd[1]))
			uerr(20); /* illegal addressing mode */
		if (modelen == BYTESIZ || modelen == LONGSIZ)
			uerr(34); /* illegal size */
		if (!m68010)
			uerr(8); /* opcode for 68010 only */
		ins[0] |= opnd[1].ea;
		if (!dataea(&opnd[1]))
			uerr(9); /* invalid first operand */
		doea(&opnd[1]);
		return;
	}
	if (cksprg(&opnd[1], CCR))
	{
		ins[0] = MOVETCC;
	  opf3l1:
		if (anysprg(&opnd[0]))
			uerr(20); /* illegal addressing mode */
		if (modelen == BYTESIZ || modelen == LONGSIZ)
			uerr(34); /* illegal size */
		ins[0] |= opnd[0].ea;
		if (!dataea(&opnd[0]))
			uerr(9); /* invalid first operand */
		doea(&opnd[0]);
		return;
	}
	if (cksprg(&opnd[1], SR))
	{
		ins[0] = MOVESR;
		goto opf3l1;
	}
	if (cksprg(&opnd[0], SR))
	{
		if (modelen == BYTESIZ || modelen == LONGSIZ)
			uerr(34); /* illegal size */
		if (anysprg(&opnd[1]))
			uerr(20); /* illegal addressing mode */
		ins[0] = SRMOVE | opnd[1].ea;
		if (!dataalt(&opnd[1]) || pcea(&opnd[1]))
			uerr(10); /* invalid second operand */
		doea(&opnd[1]);
		return;
	}
	if (cksprg(&opnd[0], USP))
	{
		if (modelen == BYTESIZ)
			uerr(34);					/* default is word, can't test */
		if (!ckareg(&opnd[1]))
			uerr(33); /* register required */
		ins[0] = MOVEUSP | 8 | (opnd[1].ea & 7);
		return;
	}
	if (cksprg(&opnd[1], USP))
	{
		if (modelen == BYTESIZ)
			uerr(34);					/* default is word, can't test */
		if (!ckareg(&opnd[0]))
			uerr(33); /* register required */
		ins[0] = MOVEUSP | (opnd[0].ea & 7);
		return;
	}
	k = ins[0];
	ins[0] |= f3mode[modelen];
	ckbytea();
	ins[0] |= opnd[0].ea;				/* source ea */
	doea(&opnd[0]);
	ins[0] |= (opnd[1].ea & 7) << 9;	/* dest register */
	ins[0] |= (opnd[1].ea & 070) << 3;	/* dest mode */
	doea(&opnd[1]);
	if (k == MOVEA)
	{
		if (dataea(&opnd[1]))
			uerr(20); /* illegal addressing mode */
	} else if ((pcea(&opnd[1]) && dataea(&opnd[1])) || opnd[1].ea == IMM)
	{
		uerr(20); /* illegal addressing mode */
	}
}


/* format 4 -- abcd, sbcd */
/* format 10 -- cmpm */
/* format 27 -- addx, subx */
static VOID opf4(NOTHING)
{
	if (get2ops())
		return;
	if (format == 27)
	{									/* addx,subx add in size bits */
		ins[0] |= f1mode[modelen];
	} else if (format == 10)
	{									/* cmpm */
		if ((opnd[0].ea & 070) != INDINC || (opnd[1].ea & 070) != INDINC)
			uerr(20); /* illegal addressing mode */
		ins[0] |= f1mode[modelen] | ((opnd[0].ea & 7) | ((opnd[1].ea & 7) << 9));
		if (m68010)
		{
			uerr(31); /* warning: cmpm generated for 68010 */
			nerror--;					/* just a warning */
		}
		return;
	}
	if (ckdreg(&opnd[0]) && ckdreg(&opnd[1]))
	{
		ins[0] |= ((opnd[0].ea & 7) | ((opnd[1].ea & 7) << 9));
		return;
	}
	if ((opnd[0].ea & 070) == DECIND && (opnd[1].ea & 070) == DECIND)
	{
		ins[0] |= 010 | ((opnd[0].ea & 7) | ((opnd[1].ea & 7) << 9));
		return;
	}
	uerr(20); /* illegal addressing mode */
}


/* format 5 -- div, mul */
/* format 26 -- cmp, chk */
static VOID opf5(NOTHING)
{
	if (get2ops())
		return;
	if (!ckdreg(&opnd[1]))
	{
		if (opcpt == cmpptr)
		{
			if (!dataea(&opnd[1]))
				ins[0] |= f5amode[modelen];	/* was pumode */
			else if (makeimm())
				return;
			else
				uerr(20); /* illegal addressing mode */
		} else
		{
			uerr(20); /* illegal addressing mode */
		}
	}
	if (opcpt == cmpptr)
	{
		ins[0] |= f5mode[modelen];		/* was pumode */
		ckbytea();
	} else if (!dataea(&opnd[0]))
	{
		uerr(20); /* illegal addressing mode */
	}
	ins[0] |= (opnd[1].ea & 7) << 9 | opnd[0].ea;
	doea(&opnd[0]);
}


#define BTST	0000
/* format 7 -- bit instrs -- btst, bclr, bset, etc */
static VOID opf7(NOTHING)
{
	if (get2ops())
		return;
	if (opnd[1].ea == IMM || (ins[0] != BTST && pcea(&opnd[1])) || ckareg(&opnd[1]))
		uerr(20); /* illegal addressing mode */
	if (ckdreg(&opnd[0]))
	{
		ins[0] |= (opnd[0].ea << 9) | 0400;
	} else
	{									/* static bit # */
		if (opnd[0].con < 0L || opnd[0].con > 31 || (opnd[1].ea & INDIRECT && opnd[0].con > 7))
			uerr(23); /* invalid bit range */
		if (opnd[0].ea != IMM)
			uerr(17); /* illegal format */
		ins[0] |= 04000;
		dodisp(&opnd[0]);
	}
	if (modelen == 1 && !(memea(&opnd[1])))
		uerr(20); /* illegal addressing mode */
	else if (!(ckdreg(&opnd[1])) && modelen == 4)
		uerr(20); /* illegal addressing mode */
	ins[0] |= opnd[1].ea;
	doea(&opnd[1]);
}


/* copy opnd 0 to opnd 1 */
static VOID cpop01(NOTHING)
{
	opnd[1].ea = opnd[0].ea;
	opnd[1].len = opnd[0].len;
	opnd[1].con = opnd[0].con;
	opnd[1].drlc = opnd[0].drlc;
	opnd[1].ext = opnd[0].ext;
	opnd[1].idx = opnd[0].idx;
	opnd[1].xmod = opnd[0].xmod;
}


/* format 8 -- shifts and rotates */
static VOID opf8(NOTHING)
{
	register short i;

	getea(0);							/* get first operand */
	if (pitw >= pnite)
	{									/* end of all ops */
		if (ckdreg(&opnd[0]))
		{								/* shift dreg one bit */
			cpop01();					/* copy opnd 0 to 1 */
			opnd[0].ea = IMM;
			opnd[0].con = 1L;
			if (!ckdreg(&opnd[1]))
				uerr(20); /* illegal addressing mode */
		  opf8l1:
			if (opnd[0].con < 1 || opnd[0].con > 8)	/* legal range 1..8 */
				uerr(37); /* illegal shift count */
			ins[0] |= (((short)opnd[0].con & 7) << 9) | f1mode[modelen] | opnd[1].ea;
			return;
		}
		i = (ins[0] & 077) << 6;
		ins[0] &= 0177700;
		ins[0] |= 0300 | i | opnd[0].ea;
		if (!memalt(&opnd[0]) || pcea(&opnd[0]) || modelen != 2)
			uerr(20); /* illegal addressing mode */
		doea(&opnd[0]);
		return;
	}
	if (!ckcomma())
	{
		uerr(10); /* invalid second operand */
		return;
	}
	getea(1);							/* get second operand */
	if (!ckdreg(&opnd[1]))				/* second operand must be dreg */
		uerr(20); /* illegal addressing mode */
	if (ckdreg(&opnd[0]))
	{									/* first op is D reg */
		ins[0] |= (opnd[0].ea << 9) | 040;	/* reg # and reg bit */
	} else
	{
		if (opnd[0].ea != IMM)
			uerr(20); /* illegal addressing mode */
		goto opf8l1;
	}
	ins[0] |= f1mode[modelen] | opnd[1].ea;	/* put in size and reg # */
}


/* check for a control operand */
static int controlea(P(struct op *) ap)
PP(struct op *ap;)
{
	register short i;

	i = ap->ea & 070;
	if (i == INDIRECT || i == INDDISP || i == INDINX)
		return TRUE;
	if (i == 070)
	{
		if ((ap->ea & 7) <= 3)
			return TRUE;
	}
	return FALSE;
}


/* format 9 -- jmp, jsr */
/* format 14 -- stop */
/* format 14 -- rtd (68010) */
/* format 24 -- clr, neg, negx, not */
/* format 25 -- s?? */
/* format 29 -- pea */
/* one operand instructions -- jmp, clr, neg, not, sge, etc. */
static VOID opf9(NOTHING)
{
	getea(0);
	if (format == 24)
	{									/* clr, not, etc */
		ins[0] |= f1mode[modelen];		/* add size bits */
		if (!dataalt(&opnd[0]) || pcea(&opnd[0]))
			uerr(20); /* illegal addressing mode */
	} else if (format == 25)
	{									/* tas,scc, etc */
		if (ckareg(&opnd[0]) || pcea(&opnd[0]) || opnd[0].ea == IMM)
			uerr(20); /* illegal addressing mode */
	} else if (format == 14)
	{									/* stop */
		if (ins[0] == RTD && !m68010)
			uerr(8); /* opcode for 68010 only */
		if (modelen != 2 || opnd[0].ea != IMM)
			uerr(20); /* illegal addressing mode */
		doea(&opnd[0]);
		return;
	} else if (!controlea(&opnd[0]))	/* jmp, jsr, etc */
	{
		uerr(20); /* illegal addressing mode */
	}
	ins[0] |= opnd[0].ea;
	doea(&opnd[0]);
}


/* format 11 -- dbcc */
/* format 19 -- link */
static VOID opf11(NOTHING)
{
	if (get2ops())
		return;
	if (format == 19)
	{									/* link */
		if (!ckareg(&opnd[0]))
			uerr(33); /* register required */
		if (opnd[1].ea != IMM)
			uerr(17); /* constant required */
	} else
	{
		if (!ckdreg(&opnd[0]))
			uerr(33); /* register required */
		if (opnd[1].drlc != rlflg)		/* don't chk opnd[1].ea!=LADDR||SADDR */
			uerr(22); /* illegal relative address */
		opnd[1].con -= (loctr + 2L);
		cksize(&opnd[1]);
		opnd[1].drlc = ABS;				/* not relocatable */
	}
	ins[0] |= opnd[0].ea & 7;			/* put in reg # */
	dodisp(&opnd[1]);
}


/* format 12 -- exg */
static VOID opf12(NOTHING)
{
	register short i;

	if (get2ops())
		return;
	if (ckdreg(&opnd[0]))
	{
		if (ckdreg(&opnd[1]))
		{								/* exchange D regs */
			ins[0] |= 0100 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
		if (ckareg(&opnd[1]))
		{								/* ins[0] <- A and D flag */
			ins[0] |= 0210 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
	}
	if (ckareg(&opnd[0]))
	{
		if (ckareg(&opnd[1]))
		{								/* both a regs */
			ins[0] |= 0110 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
		if (ckdreg(&opnd[1]))
		{								/* A and D regs */
			i = opnd[0].ea;				/* exchg ea's */
			opnd[0].ea = opnd[1].ea;
			opnd[1].ea = i;
			ins[0] |= 0210 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
	}
	uerr(20); /* illegal addressing mode */
}


/* format 13 -- ext, unlk */
/* format 18 -- trap */
/* format 28 -- swap */
#define UNLK	047130

static VOID opf13(NOTHING)
{
	getea(0);
	if (format == 18)
	{									/* trap */
		if (opnd[0].con < 0 || opnd[0].con > 15)
			uerr(15); /* illegal constant */
		ins[0] |= (short)opnd[0].con;
		return;
	}
	if (ins[0] == UNLK)
	{									/* unlk instr */
		if (!ckareg(&opnd[0]))
			uerr(20); /* illegal addressing mode */
	} else
	{
		if (!ckdreg(&opnd[0]))
			uerr(20); /* illegal addressing mode */
		if (format == 13)				/* ext */
			ins[0] |= f13mode[modelen];
	}
	ins[0] |= opnd[0].ea & 7;
}


/* format 15 -- adda, cmpa, suba */
/* format 30 -- lea */
static VOID opf15(NOTHING)
{
	register short i;

	if (get2ops())
		return;
	if (!ckareg(&opnd[1]))
		uerr(33); /* register required */
	if (format == 30)
	{
		i = 0700;
		if (!controlea(&opnd[0]))
			uerr(20); /* illegal addressing mode */
	} else
	{
		i = f15mode[modelen];
	}
	makef1(opnd[1].ea & 7, i, &opnd[0]);
	if (format == 15 && opnd[0].ea != 071)
		cksize(&opnd[0]);
}


/* formats 16 and 17 -- addq, inc, subq, dec */
static VOID opf17(NOTHING)
{
	if (format == 16)
	{									/* inc or dec */
		clrea(&opnd[0]);
		opnd[0].ea = IMM;
		opnd[0].con = 1L;
		opnd[0].drlc = ABS;
		getea(1);
	} else
	{
		if (get2ops())
			return;
	}
	if (opnd[0].ea != IMM || !altea(&opnd[1]) || pcea(&opnd[1]))
		uerr(20); /* illegal addressing mode */
	if (opnd[0].con <= 0 || opnd[0].con > 8)
		uerr(15); /* illegal constant */
	if (modelen == 1 && !dataea(&opnd[1]))
		uerr(34); /* illegal size */
	ins[0] |= f1mode[modelen] | (((short)opnd[0].con & 7) << 9) | opnd[1].ea;
	doea(&opnd[1]);
}


/* reverse a movem register mask for control ea to memory */
static int fixmask(P(int) msk)
PP(int msk;)
{
	register short i, j, k;

	k = 0;
	i = 1;
	j = 0x8000;
	while (i)
	{
		if (msk & i)
			k |= j;
		i <<= 1;
		j >>= 1;
	}
	return k;
}


/*
 * get a list of registers for the movem instr
 * call with:
 *	ptr to reg-to-mem or mem-to-reg array of bits
 */
static int getrlist(P(const short *) ap)
PP(const short *ap;)
{
	register const short *p;
	register short i, j, mask;

	p = ap;
	mask = 0;
	while ((i = getreg()) != -1)
	{
		if (ckitc(pitw, '-'))
		{
			pitw++;
			if ((j = getreg()) < 0)
			{
				uerr(40); /* backward assignment to * */
				break;
			}
			while (i <= j)
				mask |= p[i++];
		} else
		{
			mask |= p[i];
		}
		if (ckitc(pitw, '/'))
			pitw++;
		else
			break;
	}
	if (!mask)
		uerr(40); /* backward assignment to * */
	return mask;
}


static VOID opf20(NOTHING)
{
	register short dr, i, j;

	dr = 0;
	if (getreg() != -1 || pitw->itty == ITRM)
	{									/* regs to memory */
		if (pitw->itty != ITRM)
		{
			pitw--;
			j = getrlist(regmsk0);
		} else
		{
			j = pitw->itop.l;
			pitw++;
		}
		if (!ckcomma())
			uerr(10); /* invalid second operand */
	} else
	{
		dr = 02000;
	}
	getea(0);
	if (dr)
	{
		if (!ckcomma())
			uerr(10); /* invalid second operand */
		if (pitw->itty != ITRM)
		{
			j = getrlist(regmsk1);		/* mem to regs */
		} else
		{
			j = pitw->itop.l;
			j = fixmask(j);
			pitw++;
		}
	} else
	{
		if (controlea(&opnd[0]))
			j = fixmask(j);
	}
	i = opnd[0].ea & 070;
	if (!controlea(&opnd[0]) && i != INDINC && i != DECIND)
		uerr(20); /* illegal addressing mode */
	if (modelen == 4)					/* long */
		ins[0] |= 0100;
	ins[0] |= opnd[0].ea | dr;
	*pins++ = j;						/* reg mask */
	*prlb++ = DABS;
	instrlen += 2;
	doea(&opnd[0]);
	if (!dr)
	{									/* 1st argument (2nd is reg list) */
		if (pcea(&opnd[0]) || (opnd[0].ea & 070) == INDINC)
			uerr(20);					/* xx(pc), xx(pc,dx), -(ax) */
	} else /* 2nd argument (1st is reg list) */ if ((opnd[0].ea & 070) == DECIND)
		uerr(20);						/* (ax)+ */
}


/* format 21 -- movep */
static VOID opf21(NOTHING)
{
	register short m, d;
	register struct op *p;

	if (get2ops())
		return;
	if (ckdreg(&opnd[0]))
	{									/* d reg source */
		m = 0600;
		d = opnd[0].ea;
		p = &opnd[1];
	} else if (ckdreg(&opnd[1]))
	{									/* d reg dest */
		m = 0400;
		d = opnd[1].ea;
		p = &opnd[0];
	} else
	{
		uerr(20); /* illegal addressing mode */
		return;
	}
	if ((p->ea & 070) != INDDISP)
		uerr(20); /* illegal addressing mode */
	if (modelen == 4)
		m |= 0100;
	ins[0] |= (d << 9) | m | (p->ea & 7);
	*pins++ = (short)p->con;
	*prlb++ = p->drlc;
	instrlen += 2;
}


/* format 22 -- moveq */
static VOID opf22(NOTHING)
{
	if (get2ops())
		return;
	if (opnd[0].ea != IMM)
		uerr(17); /* constant required */
	if (opnd[0].con > 255L || opnd[0].con < -256L)
		uerr(15); /* illegal constant */
	if (!ckdreg(&opnd[1]))
		uerr(33); /* register required */
	ins[0] |= (opnd[1].ea << 9) | ((short)opnd[0].con & 0xff);
}


/* format 23 -- eor */
static VOID opf23(NOTHING)
{
	if (get2ops())
		return;
	if (cksprg(&opnd[1], CCR) || cksprg(&opnd[1], SR))
	{
		opcpt = eoriptr;
		ins[0] = opcpt->vl1;
		format = opcpt->flags & OPFF;
		ccr_or_sr();
		return;
	}
	if (!ckdreg(&opnd[0]))
	{
		if (makeimm())					/* must be immediate */
			return;
		uerr(20);						/* or error */
	}
	if (!dataalt(&opnd[1]) || pcea(&opnd[1]))
		uerr(20); /* illegal addressing mode */
	ins[0] |= (opnd[0].ea << 9) | f23mode[modelen] | opnd[1].ea;
	doea(&opnd[1]);
}


/* format 31 -- movec and moves (68010 only) */
static VOID opf31(NOTHING)
{
	register struct op *cntrl, *genrl, *eaop;

	instrlen += 2;
	if (!m68010)
		uerr(8); /* opcode for 68010 only */

	if (get2ops())
		return;
	if (ins[0] == MOVEC)
	{
		if (modelen == BYTESIZ)
			uerr(34); /* illegal size */
		if (cksprg(&opnd[0], USP) || cksprg(&opnd[0], SFC) || cksprg(&opnd[0], DFC) || cksprg(&opnd[0], VBR))
		{
			cntrl = &opnd[0];
			genrl = &opnd[1];
		} else
		{
			if (!cksprg(&opnd[1], USP) && !cksprg(&opnd[1], SFC) && !cksprg(&opnd[1], DFC) && !cksprg(&opnd[1], VBR))
				uerr(18); /* illegal format */
			ins[0] |= 1;				/* direction Rn --> Rc */
			cntrl = &opnd[1];
			genrl = &opnd[0];
		}
		if (!ckreg(genrl))
			uerr(18); /* illegal format */
		*pins = ((genrl->ea) << 12) & 0xF000;
		if (cksprg(cntrl, DFC))
			*pins |= DFC_CR;
		else if (cksprg(cntrl, USP))
			*pins |= USP_CR;
		else if (cksprg(cntrl, VBR))
			*pins |= VBR_CR;
		/* else... *pins |= SFC_CR; (SFC_CR == 0) */
	} else
	{									/* MOVES */
		ins[0] |= f1mode[modelen];
		if (ckreg(&opnd[0]))
		{
			genrl = &opnd[0];
			eaop = &opnd[1];
			*pins = 0x800;				/* from general register to <ea> */
		} else
		{
			genrl = &opnd[1];
			eaop = &opnd[0];
			*pins = 0;
		}
		*pins |= ((genrl->ea) << 12) & 0xF000;
		if (!memalt(eaop) || pcea(eaop) || ckreg(eaop))
			uerr(20); /* illegal addressing mode */
		ins[0] |= eaop->ea;
		doea(eaop);
	}
}


static adirect const opfary[] = {
	0,		/*  0 */
	opf1,	/*  1 */
	opf2,	/*  2 */
	opf3,	/*  3 */
	opf4,	/*  4 */
	opf5,	/*  5 */
	relbr,	/*  6 */
	opf7,	/*  7 */
	opf8,	/*  8 */
	opf9,	/*  9 */
	opf4,	/* 10 */
	opf11,	/* 11 */
	opf12,	/* 12 */
	opf13,	/* 13 */
	opf9,	/* 14 */
	opf15,	/* 15 */
	opf17,	/* 16 */
	opf17,	/* 17 */
	opf13,	/* 18 */
	opf11,	/* 19 */
	opf20,	/* 20 */
	opf21,	/* 21 */
	opf22,	/* 22 */
	opf23,	/* 23 */
	opf9,	/* 24 */
	opf9,	/* 25 */
	opf5,	/* 26 */		/* cmp, chk, extention verification */
	opf4,	/* 27 */		/* addx, subx, extension verification */
	opf13,	/* 28 */		/* swap, extension verification */
	opf9,	/* 29 */		/* pea, extention verification */
	opf15,  /* 30 */		/* lea, extension verification */
	opf31	/* 31 */		/* movec & moves 68010 */
};
#define LSTFRMT 31


/*
 * generate code for an instruction
 * call with
 * intermediate text for instruction in stbuf
 */
static VOID gcist(NOTHING)
{
	if (stbuf[0].itty != ITBS)			/* beginning of statement */
	{
		rpterr(_("internal: not at beginning of stmt"));
		asabort();
	}
	format = opcpt->flags & OPFF;
	in_err = 0;							/* no error this instruction, yet */
	ival.p = 0;							/* initial value for possible operand */
	reloc = ABS;
	instrlen = 2;						/* at least 2 bytes */
	ins[0] = opcpt->vl1;				/* opcode value */
	rlbits[0] = INSABS;					/* instruction absolute */
	pins = &ins[1];
	prlb = &rlbits[1];
	if (nite > ITOP1)
	{									/* operands */
		if (!format)
		{
			uerr(9); /* invalid first operand */
		} else if (format > LSTFRMT)		/* was a magic number... */
		{
			rpterr(_("internal: illegal format %d"), format);
			asabort();
		} else
		{
			(*opfary[format]) ();
		}
	}
	if (!ckein() && !in_err)			/* at end of statement ?? */
		uerr(6); /* illegal expr */
	print(1);							/* print source */

	loctr += p1inlen;
	if (!in_err && p1inlen != instrlen)	/* 2nd pass error recovery */
		uerr(38); /* invalid instruction length */
	outinstr();							/* write out instr binary */
}


/* pass two driver */
VOID pass2(NOTHING)
{
	register short i;
	register adirect dirop;

	pitix = ITBSZ;						/* it buffer is empty */
	couthd.ch_magic = MAGIC;			/* c.out magic number */
	if (savelc[TEXT] & 1)
		savelc[TEXT]++;					/* make it even */
	couthd.ch_tsize = savelc[TEXT];		/* text size */
	if (savelc[DATA] & 1)
		savelc[DATA]++;					/* make it even */
	couthd.ch_dsize = savelc[DATA];		/* data size */
	couthd.ch_bsize = savelc[BSS];		/* bss size */
	/*
	 * symbol table size is not known now -- it is set at end of pass 2
	 * entry point and stack size are zero for now
	 */
	putchd(lfil, &couthd);
	savelc[ABS] = 0;
	savelc[DATA] = 0;
	savelc[TEXT] = 0;
	savelc[BSS] = 0;
	loctr = 0;							/* location counter */
	rlflg = TEXT;						/* TEXT relocatable */
	p2flg = 1;							/* pass two */
	if (fseek(ifn, 0L, SEEK_SET) < 0)
	{									/* beginning of source */
		rpterr(_("seek error on source file"));
		asabort();
	}
	fflush(itfn);
	if (fseek(itfn, 0L, SEEK_SET) < 0)
	{
		rpterr(_("seek error on intermediate file"));
		asabort();
	}
	fchr = gchr();						/* get first char */
	for (;;)
	{									/* pass 2 main loop */
		ristb();						/* read it for one statement */
		p2absln = stbuf[0].itop.l;		/* line number */
		opcpt = stbuf[2].itop.ptrw2;	/* ptr to opcode entry in main tab */
		nite = stbuf[0].itrl & 0xff;	/* number of it entries */
		pnite = &stbuf[nite];		    /* ptr to end of stmt */
		modelen = stbuf[2].itrl;		/* instr mode length */
		p1inlen = stbuf[1].itrl;		/* pass 1 instr length guess */
		opdix = ITOP1;					/* first operand */
		pitw = &stbuf[ITOP1];		    /* ptr to first operand */
		instrlen = 2;					/* default for print */
		if (opcpt->flags & OPDR)
		{								/* opcode is a directive */
			i = opcpt->vl1;				/* directive number */
			if (i < DIRECT)
			{
				dirop = p2direct[i];
				(*dirop) ();			/* handle directive */
			} else
			{
				rpterr(_("internal: invalid directive %d"), i);
				asabort();
			}
		} else
		{
			gcist();					/* generate code for one statement */
		}
	}
}
