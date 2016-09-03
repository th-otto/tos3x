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
#include "../util/util.h"

/* Second Pass Subroutines */
VOID relbr PROTO((NOTHING));
VOID opf1 PROTO((NOTHING));
VOID opf2 PROTO((NOTHING));
VOID opf3 PROTO((NOTHING));
VOID opf4 PROTO((NOTHING));
VOID opf5 PROTO((NOTHING));
VOID opf7 PROTO((NOTHING));
VOID opf8 PROTO((NOTHING));
VOID opf9 PROTO((NOTHING));
VOID opf11 PROTO((NOTHING));
VOID opf12 PROTO((NOTHING));
VOID opf13 PROTO((NOTHING));
VOID opf15 PROTO((NOTHING));
VOID opf17 PROTO((NOTHING));
VOID opf20 PROTO((NOTHING));
VOID opf21 PROTO((NOTHING));
VOID opf22 PROTO((NOTHING));
VOID opf23 PROTO((NOTHING));
VOID opf31 PROTO((NOTHING));

#include "p2def.h"


VOID gcist PROTO((NOTHING));
int getrlist PROTO((short *ap));
int fixmask PROTO((int msk));



/* pass two driver */
VOID pass2(NOTHING)
{
	register short i;
	register adirect dirop;

	pitix = &itbuf[ITBSZ];				/* it buffer is empty */
	lbuf.cc = tbuf.cc = dabuf.cc = drbuf.cc = BLEN;
	fileno(&lbuf) = lfn;						/* set buffered io for binary file */
	lbuf.cp = &lbuf.cbuf[0];
	fileno(&tbuf) = trbfn;					/* set buffered io for text reloc bits file */
	tbuf.cp = &tbuf.cbuf[0];
	fileno(&dabuf) = dafn;					/* set buffered io for data bytes */
	dabuf.cp = &dabuf.cbuf[0];
	fileno(&drbuf) = drbfn;					/* set buffered io for data reloc bits */
	drbuf.cp = &drbuf.cbuf[0];
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
	putchd(&lbuf, &couthd);				/* replaces write_header */
	savelc[0] = 0;
	savelc[1] = 0;
	savelc[2] = 0;
	savelc[3] = 0;
	loctr = 0;							/* location counter */
	rlflg = TEXT;						/* TEXT relocatable */
	p2flg = 1;							/* pass two */
	if (lseek(ifn, 0L, 0) == -1L)
	{									/* beginning of source */
		rpterr("seek error on source file\n");
		asabort();
	}
	close(itfn);
	LASTCHTFN = itfnc;
	itfn = openfi(tfilname, 0, 1);		/* open it for reading */
	pline = 1;							/* no lines printed */
	fchr = gchr();						/* get first char */
	while (1)
	{									/* pass 2 main loop */
		ristb();						/* read it for one statement */
		p2absln = stbuf[0].itop.l;		/* line number */
		if (p2absln >= brkln2)			/* for debugging the assembler */
			i = 0;
		opcpt = stbuf[2].itop.ptrw2;	/* ptr to opcode entry in main tab */
		nite = stbuf[0].itrl & 0377;	/* number of it entries */
		pnite = &stbuf[nite];		    /* ptr to end of stmt */
		modelen = stbuf[2].itrl;		/* instr mode length */
		p1inlen = stbuf[1].itrl;		/* pass 1 instr length guess */
		opdix = ITOP1;					/* first operand */
		pitw = &stbuf[ITOP1];		    /* ptr to first operand */
		prsp = 0;						/* special print flag off */
		instrlen = 2;					/* default for print */
		if (opcpt->flags & OPDR)
		{								/* opcode is a directive */
			i = opcpt->vl1.l;				/* directive number */
			if (i <= (DIRECT - 1))
			{
				dirop = p2direct[i];
				(*dirop) ();			/* handle directive */
			} else
			{
				uerr(21);
			}
		} else
		{
			gcist();					/* generate code for one statement */
		}
	}
}


/*
 * generate code for an instruction
 * call with
 * intermediate text for instruction in stbuf
 */
VOID gcist(NOTHING)
{
	if (stbuf[0].itty != ITBS)			/* beginning of statement */
		asabort();
	format = opcpt->flags & OPFF;
	in_err = 0;							/* no error this instruction, yet */
	ival.p = 0;							/* initial value for possible operand */
	reloc = ABS;
	instrlen = 2;						/* at least 2 bytes */
	ins[0].w = opcpt->vl1.u.loword;			/* opcode value, 4.2 ==> loword */
	rlbits[0] = INSABS;					/* instruction absolute */
	pins = &ins[1];
	prlb = &rlbits[1];
	if (nite > ITOP1)
	{									/* operands */
		if (!format)
		{
			uerr(9);
		} else if (format > LSTFRMT)		/* was a magic number... */
		{
			asabort();
		} else
		{
			(*opfary[format]) ();
		}
	}
	if (!ckein() && !in_err)			/* at end of statement ?? */
		uerr(6);
	print(1);							/* print source */

	loctr += p1inlen;
	if (!in_err && p1inlen != instrlen)	/* 2nd pass error recovery */
		uerr(38);
	outinstr();							/* write out instr binary */
}


/* relative branches */
VOID relbr(NOTHING)
{
	expr(p2gi);
	if (extflg)
	{									/* external reference */
		instrlen += 2;					/* long relative */
		(pins++)->w = ival.l;			/* pass constant part */
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
			uerr(22);
		instrlen += 2;
		(pins++)->w = ival.l;
		*prlb++ = DABS;					/* data absolute */
	} else
	{									/* short displacement */
		if (ival.l > 127 || ival.l < -128)
			uerr(22);
		ins[0].w |= (ival.u.loword & 0377);
	}
	/* make it a nop if -N specified */
	if ((ival.l == 0) || (ival.l == 2 && didorg))
	{
		opcpt = nopptr;
		ins[0].w = opcpt->vl1.u.loword;
		if (instrlen == 4)
		{								/* long branch */
			pins = &ins[1];
			(pins++)->w = opcpt->vl1.u.loword;
			rlbits[1] = INSABS;
		}
	}
	in_err++;							/* ignore extra eg. bra *+$d04(pc) vs. bra *+d04 */
}


#define US	(unsigned short)
/*
 * format one -- add, sub, and, or, cmp, etc.
 * one operand must be a D reg (or A reg dest for add, sub, or cmp)
 */
VOID opf1(NOTHING)
{
	register short *p;

	if (get2ops())
		return;
	if (ins[0].w == (US AND) || ins[0].w == (US OR))
		if (cksprg(&opnd[1], CCR) || cksprg(&opnd[1], SR))
		{
			if (ins[0].w == (US AND))
				opcpt = andiptr;
			else
				opcpt = oriptr;
			ins[0].w = opcpt->vl1.u.loword;
			format = opcpt->flags & OPFF;
			ccr_or_sr();
			return;
		}
	p = f1mode;
	if (ckdreg(&opnd[1]))
	{									/* destn is D reg */
		if ((opcpt == andptr || opcpt == orptr) && ckareg(&opnd[0]))	/* A source */
			uerr(20);
		makef1(opnd[1].ea, p[modelen], &opnd[0]);	/* make instr */
		return;
	} else if (ckdreg(&opnd[0]) && memalt(&opnd[1]))
	{									/* source is D reg */
		if (pcea(&opnd[1]))
			uerr(10);
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
			uerr(20);
			return;
		}
		format = opcpt->flags & OPFF;
		opnd[1].ea &= 07;
		p = f15mode;
		makef1(opnd[1].ea, p[modelen], &opnd[0]);	/* make instr */
		return;
	} else if (!makeimm())				/* make an immediate instr */
		uerr(20);
}


/* format 2 -- addi, andi, subi, etc */
VOID opf2(NOTHING)
{
	if (get2ops())
		return;
	if (ins[0].w == ANDI || ins[0].w == ORI || ins[0].w == EORI)
	{
		if (cksprg(&opnd[1], CCR) || cksprg(&opnd[1], SR))
		{
			ccr_or_sr();
			return;
		}
	}
	if (opnd[0].ea != IMM)
	{
		uerr(9);
		return;
	}
	if (!dataalt(&opnd[1]) || pcea(&opnd[1]))
	{
		uerr(20);
		return;
	}
	genimm();
}


/* format #3 -- move and movea */
VOID opf3(NOTHING)
{
	register short k;

	if (get2ops())
		return;
	if (cksprg(&opnd[0], CCR))
	{
		ins[0].w = MOVEFCC;
		if (anysprg(&opnd[1]))
			uerr(20);
		if (modelen == BYTESIZ || modelen == LONGSIZ)
			uerr(34);
		if (!m68010)
			uerr(8);
		ins[0].w |= opnd[1].ea;
		if (!dataea(&opnd[1]))
			uerr(9);
		doea(&opnd[1]);
		return;
	}
	if (cksprg(&opnd[1], CCR))
	{
		ins[0].w = MOVETCC;
	  opf3l1:
		if (anysprg(&opnd[0]))
			uerr(20);
		if (modelen == BYTESIZ || modelen == LONGSIZ)
			uerr(34);
		ins[0].w |= opnd[0].ea;
		if (!dataea(&opnd[0]))
			uerr(9);
		doea(&opnd[0]);
		return;
	}
	if (cksprg(&opnd[1], SR))
	{
		ins[0].w = MOVESR;
		goto opf3l1;
	}
	if (cksprg(&opnd[0], SR))
	{
		if (modelen == BYTESIZ || modelen == LONGSIZ)
			uerr(34);
		if (anysprg(&opnd[1]))
			uerr(20);
		ins[0].w = SRMOVE | opnd[1].ea;
		if (!dataalt(&opnd[1]) || pcea(&opnd[1]))
			uerr(10);
		doea(&opnd[1]);
		return;
	}
	if (cksprg(&opnd[0], USP))
	{
		if (modelen == BYTESIZ)
			uerr(34);					/* default is word, can't test */
		if (!ckareg(&opnd[1]))
			uerr(33);
		ins[0].w = MOVEUSP | 8 | (opnd[1].ea & 7);
		return;
	}
	if (cksprg(&opnd[1], USP))
	{
		if (modelen == BYTESIZ)
			uerr(34);					/* default is word, can't test */
		if (!ckareg(&opnd[0]))
			uerr(33);
		ins[0].w = MOVEUSP | (opnd[0].ea & 7);
		return;
	}
	k = ins[0].w;
	ins[0].w |= f3mode[modelen];
	ckbytea();
	ins[0].w |= opnd[0].ea;				/* source ea */
	doea(&opnd[0]);
	ins[0].w |= (opnd[1].ea & 7) << 9;	/* dest register */
	ins[0].w |= (opnd[1].ea & 070) << 3;	/* dest mode */
	doea(&opnd[1]);
	if (k == MOVEA)
	{
		if (dataea(&opnd[1]))
			uerr(20);
	} else if ((pcea(&opnd[1]) && dataea(&opnd[1])) || opnd[1].ea == IMM)
		uerr(20);
}


/* format 4 -- abcd, sbcd */
/* format 10 -- cmpm */
/* format 27 -- addx, subx */
VOID opf4(NOTHING)
{
	if (get2ops())
		return;
	if (format == 27)
	{									/* addx,subx add in size bits */
		ins[0].w |= f1mode[modelen];
	} else if (format == 10)
	{									/* cmpm */
		if ((opnd[0].ea & 070) != INDINC || (opnd[1].ea & 070) != INDINC)
			uerr(20);
		ins[0].w |= f1mode[modelen] | ((opnd[0].ea & 7) | ((opnd[1].ea & 7) << 9));
		if (m68010)
		{
			uerr(31);
			nerror--;					/* just a warning */
		}
		return;
	}
	if (ckdreg(&opnd[0]) && ckdreg(&opnd[1]))
	{
		ins[0].w |= ((opnd[0].ea & 7) | ((opnd[1].ea & 7) << 9));
		return;
	}
	if ((opnd[0].ea & 070) == DECIND && (opnd[1].ea & 070) == DECIND)
	{
		ins[0].w |= 010 | ((opnd[0].ea & 7) | ((opnd[1].ea & 7) << 9));
		return;
	}
	uerr(20);
}


/* format 5 -- div, mul */
/* format 26 -- cmp, chk */
VOID opf5(NOTHING)
{
	if (get2ops())
		return;
	if (!ckdreg(&opnd[1]))
	{
		if (opcpt == cmpptr)
		{
			if (!dataea(&opnd[1]))
				ins[0].w |= f5amode[modelen];	/* was pumode */
			else if (makeimm())
				return;
			else
				uerr(20);
		} else
			uerr(20);
	}
	if (opcpt == cmpptr)
	{
		ins[0].w |= f5mode[modelen];		/* was pumode */
		ckbytea();
	} else if (!dataea(&opnd[0]))
	{
		uerr(20);
	}
	ins[0].w |= (opnd[1].ea & 7) << 9 | opnd[0].ea;
	doea(&opnd[0]);
}


#define BTST	0000
/* format 7 -- bit instrs -- btst, bclr, bset, etc */
VOID opf7(NOTHING)
{
	if (get2ops())
		return;
	if (opnd[1].ea == IMM || (ins[0].w != BTST && pcea(&opnd[1])) || ckareg(&opnd[1]))
		uerr(20);
	if (ckdreg(&opnd[0]))
	{
		ins[0].w |= (opnd[0].ea << 9) | 0400;
	} else
	{									/* static bit # */
		if (opnd[0].con.l < 0L || opnd[0].con.l > 31 || (opnd[1].ea & INDIRECT && opnd[0].con.l > 7))
			uerr(23);
		if (opnd[0].ea != IMM)
			uerr(17);
		ins[0].w |= 04000;
		dodisp(&opnd[0]);
	}
	if (modelen == 1 && !(memea(&opnd[1])))
		uerr(20);
	else if (!(ckdreg(&opnd[1])) && modelen == 4)
		uerr(20);
	ins[0].w |= opnd[1].ea;
	doea(&opnd[1]);
}


/* format 8 -- shifts and rotates */
VOID opf8(NOTHING)
{
	register short i;

	getea(0);							/* get first operand */
	if (pitw >= pnite)
	{									/* end of all ops */
		if (ckdreg(&opnd[0]))
		{								/* shift dreg one bit */
			cpop01();					/* copy opnd 0 to 1 */
			opnd[0].ea = IMM;
			opnd[0].con.l = 1L;
			if (!ckdreg(&opnd[1]))
				uerr(20);
		  opf8l1:
			if (opnd[0].con.l < 1 || opnd[0].con.l > 8)	/* legal range 1..8 */
				uerr(37);
			ins[0].w |= ((opnd[0].con.u.loword & 7) << 9) | f1mode[modelen] | opnd[1].ea;
			return;
		}
		i = (ins[0].w & 077) << 6;
		ins[0].w &= 0177700;
		ins[0].w |= 0300 | i | opnd[0].ea;
		if (!memalt(&opnd[0]) || pcea(&opnd[0]) || modelen != 2)
			uerr(20);
		doea(&opnd[0]);
		return;
	}
	if (!ckcomma())
	{
		uerr(10);
		return;
	}
	getea(1);							/* get second operand */
	if (!ckdreg(&opnd[1]))				/* second operand must be dreg */
		uerr(20);
	if (ckdreg(&opnd[0]))
	{									/* first op is D reg */
		ins[0].w |= (opnd[0].ea << 9) | 040;	/* reg # and reg bit */
	} else
	{
		if (opnd[0].ea != IMM)
			uerr(20);
		goto opf8l1;
	}
	ins[0].w |= f1mode[modelen] | opnd[1].ea;	/* put in size and reg # */
}


/* format 9 -- jmp, jsr */
/* format 14 -- stop */
/* format 14 -- rtd (68010) */
/* format 24 -- clr, neg, negx, not */
/* format 25 -- s?? */
/* format 29 -- pea */
/* one operand instructions -- jmp, clr, neg, not, sge, etc. */
VOID opf9(NOTHING)
{
	getea(0);
	if (format == 24)
	{									/* clr, not, etc */
		ins[0].w |= f1mode[modelen];		/* add size bits */
		if (!dataalt(&opnd[0]) || pcea(&opnd[0]))
			uerr(20);
	} else if (format == 25)
	{									/* tas,scc, etc */
		if (ckareg(&opnd[0]) || pcea(&opnd[0]) || opnd[0].ea == IMM)
			uerr(20);
	} else if (format == 14)
	{									/* stop */
		if (ins[0].w == RTD && !m68010)
			uerr(8);
		if (modelen != 2 || opnd[0].ea != IMM)
			uerr(20);
		doea(&opnd[0]);
		return;
	} else if (!controlea(&opnd[0]))	/* jmp, jsr, etc */
		uerr(20);
	ins[0].w |= opnd[0].ea;
	doea(&opnd[0]);
}


/* format 11 -- dbcc */
/* format 19 -- link */
VOID opf11(NOTHING)
{
	if (get2ops())
		return;
	if (format == 19)
	{									/* link */
		if (!ckareg(&opnd[0]))
			uerr(33);
		if (opnd[1].ea != IMM)
			uerr(17);
	} else
	{
		if (!ckdreg(&opnd[0]))
			uerr(33);
		if (opnd[1].drlc != rlflg)		/* don't chk opnd[1].ea!=LADDR||SADDR */
			uerr(22);
		opnd[1].con.l -= (loctr + 2L);
		cksize(&opnd[1]);
		opnd[1].drlc = ABS;				/* not relocatable */
	}
	ins[0].w |= opnd[0].ea & 7;			/* put in reg # */
	dodisp(&opnd[1]);
}


/* format 12 -- exg */
VOID opf12(NOTHING)
{
	register short i;

	if (get2ops())
		return;
	if (ckdreg(&opnd[0]))
	{
		if (ckdreg(&opnd[1]))
		{								/* exchange D regs */
			ins[0].w |= 0100 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
		if (ckareg(&opnd[1]))
		{								/* ins[0] <- A and D flag */
			ins[0].w |= 0210 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
	}
	if (ckareg(&opnd[0]))
	{
		if (ckareg(&opnd[1]))
		{								/* both a regs */
			ins[0].w |= 0110 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
		if (ckdreg(&opnd[1]))
		{								/* A and D regs */
			i = opnd[0].ea;				/* exchg ea's */
			opnd[0].ea = opnd[1].ea;
			opnd[1].ea = i;
			ins[0].w |= 0210 | ((opnd[0].ea & 7) << 9) | (opnd[1].ea & 7);
			return;
		}
	}
	uerr(20);
}


/* format 13 -- ext, unlk */
/* format 18 -- trap */
/* format 28 -- swap */
#define UNLK	047130

VOID opf13(NOTHING)
{
	getea(0);
	if (format == 18)
	{									/* trap */
		if (opnd[0].con.l < 0 || opnd[0].con.l > 15)
			uerr(15);
		ins[0].w |= opnd[0].con.u.loword;
		return;
	}
	if (ins[0].w == UNLK)
	{									/* unlk instr */
		if (!ckareg(&opnd[0]))
			uerr(20);
	} else
	{
		if (!ckdreg(&opnd[0]))
			uerr(20);
		if (format == 13)				/* ext */
			ins[0].w |= f13mode[modelen];
	}
	ins[0].w |= opnd[0].ea & 7;
}


/* format 15 -- adda, cmpa, suba */
/* format 30 -- lea */
VOID opf15(NOTHING)
{
	register short i;

	if (get2ops())
		return;
	if (!ckareg(&opnd[1]))
		uerr(33);
	if (format == 30)
	{
		i = 0700;
		if (!controlea(&opnd[0]))
			uerr(20);
	} else
	{
		i = f15mode[modelen];
	}
	makef1(opnd[1].ea & 7, i, &opnd[0]);
	if (format == 15 && opnd[0].ea != 071)
		cksize(&opnd[0]);
}


/* formats 16 and 17 -- addq, inc, subq, dec */
VOID opf17(NOTHING)
{
	if (format == 16)
	{									/* inc or dec */
		clrea(&opnd[0]);
		opnd[0].ea = IMM;
		opnd[0].con.l = 1L;
		opnd[0].drlc = ABS;
		getea(1);
	} else
	{
		if (get2ops())
			return;
	}
	if (opnd[0].ea != IMM || !altea(&opnd[1]) || pcea(&opnd[1]))
		uerr(20);
	if (opnd[0].con.l <= 0 || opnd[0].con.l > 8)
		uerr(15);
	if (modelen == 1 && !dataea(&opnd[1]))
		uerr(34);
	ins[0].w |= f1mode[modelen] | ((opnd[0].con.u.loword & 7) << 9) | opnd[1].ea;
	doea(&opnd[1]);
}


/* format 20 -- movem */
short regmsk0[] = { 0100000, 040000, 020000, 010000, 04000, 02000, 01000, 0400, 0200,
	0100, 040, 020, 010, 4, 2, 1
};

short regmsk1[] = { 1, 2, 4, 010, 020, 040, 0100, 0200, 0400, 01000, 02000, 04000, 010000,
	020000, 040000, 0100000
};

VOID opf20(NOTHING)
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
			uerr(10);
	} else
		dr = 02000;
	getea(0);
	if (dr)
	{
		if (!ckcomma())
			uerr(10);
		if (pitw->itty != ITRM)
			j = getrlist(regmsk1);		/* mem to regs */
		else
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
		uerr(20);
	if (modelen == 4)					/* long */
		ins[0].w |= 0100;
	ins[0].w |= opnd[0].ea | dr;
	(pins++)->w = j;						/* reg mask */
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


/*
 * get a list of registers for the movem instr
 * call with:
 *	ptr to reg-to-mem or mem-to-reg array of bits
 */
int getrlist(P(short *) ap)
PP(short *ap;)
{
	register short *p, i, j, mask;

	p = ap;
	mask = 0;
	while ((i = getreg()) != -1)
	{
		if (ckitc(pitw, '-'))
		{
			pitw++;
			if ((j = getreg()) == -1)
			{
				uerr(40);
				break;
			}
			while (i <= j)
				mask |= p[i++];
		} else
			mask |= p[i];
		if (ckitc(pitw, '/'))
			pitw++;
		else
			break;
	}
	if (!mask)
		uerr(40);
	return (mask);
}


/* reverse a movem register mask for control ea to memory */
int fixmask(P(int) msk)
PP(int msk;)
{
	register short i, j, k;

	k = (msk & 1) ? 0100000 : 0;
	i = 2;
	j = 040000;
	while (i)
	{
		if (msk & i)
			k |= j;
		i <<= 1;
		j >>= 1;
	}
	return (k);
}


/* format 21 -- movep */
VOID opf21(NOTHING)
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
		uerr(20);
		/* BUG: falls through with undefined values for m, d, p */
#ifndef __ALCYON__
		return;
#endif
	}
	if ((p->ea & 070) != INDDISP)
		uerr(20);
	if (modelen == 4)
		m |= 0100;
	ins[0].w |= (d << 9) | m | (p->ea & 7);
	(pins++)->w = p->con.u.loword;
	*prlb++ = p->drlc;
	instrlen += 2;
}


/* format 22 -- moveq */
VOID opf22(NOTHING)
{
	if (get2ops())
		return;
	if (opnd[0].ea != IMM)
		uerr(17);
	if (opnd[0].con.l > 255L || opnd[0].con.l < -256L)
		uerr(15);
	if (!ckdreg(&opnd[1]))
		uerr(33);
	ins[0].w |= (opnd[1].ea << 9) | (opnd[0].con.u.loword & 0377);
}


/* format 23 -- eor */
VOID opf23(NOTHING)
{
	if (get2ops())
		return;
	if (cksprg(&opnd[1], CCR) || cksprg(&opnd[1], SR))
	{
		opcpt = eoriptr;
		ins[0].w = opcpt->vl1.u.loword;
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
		uerr(20);
	ins[0].w |= (opnd[0].ea << 9) | f23mode[modelen] | opnd[1].ea;
	doea(&opnd[1]);
}


/* format 31 -- movec and moves (68010 only) */
VOID opf31(NOTHING)
{
	register struct op *cntrl, *genrl, *eaop;

	instrlen += 2;
	if (!m68010)
		uerr(8);

	if (get2ops())
		return;
	if (ins[0].w == MOVEC)
	{
		if (modelen == BYTESIZ)
			uerr(34);
		if (cksprg(&opnd[0], USP) || cksprg(&opnd[0], SFC) || cksprg(&opnd[0], DFC) || cksprg(&opnd[0], VSR))
		{
			cntrl = &opnd[0];
			genrl = &opnd[1];
		} else
		{
			if (!cksprg(&opnd[1], USP) && !cksprg(&opnd[1], SFC) && !cksprg(&opnd[1], DFC) && !cksprg(&opnd[1], VSR))
				uerr(18);
			ins[0].w |= 1;				/* direction Rn --> Rc */
			cntrl = &opnd[1];
			genrl = &opnd[0];
		}
		if (!ckreg(genrl))
			uerr(18);
		pins->w = ((genrl->ea) << 12) & 0xF000;
		if (cksprg(cntrl, DFC))
			pins->w |= DFC_CR;
		else if (cksprg(cntrl, USP))
			pins->w |= USP_CR;
		else if (cksprg(cntrl, VSR))
			pins->w |= VSR_CR;
		/* else... *pins |= SFC_CR; (SFC_CR == 0) */
	} else
	{									/* MOVES */
		ins[0].w |= f1mode[modelen];
		if (ckreg(&opnd[0]))
		{
			genrl = &opnd[0];
			eaop = &opnd[1];
			pins->w = 0x800;				/* from general register to <ea> */
		} else
		{
			genrl = &opnd[1];
			eaop = &opnd[0];
			pins->w = 0;
		}
		pins->w |= ((genrl->ea) << 12) & 0xF000;
		if (!memalt(eaop) || pcea(eaop) || ckreg(eaop))
			uerr(20);
		ins[0].w |= eaop->ea;
		doea(eaop);
	}
}
