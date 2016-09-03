/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "as68.h"
#include <string.h>


/* Pass 1 and pass 2 directive handling routines */
/* code to handle conditional assembly directives */

VOID hdata PROTO((int mul));
VOID chkeven PROTO((NOTHING));
int mkmask PROTO((NOTHING));
int chkreg PROTO((NOTHING));
VOID sdata PROTO((int dtyp));
int acok PROTO((NOTHING));
int cmp_ops PROTO((NOTHING));



/* directive to define an opcode */
VOID hopd(NOTHING)
{
	if (!lbt[0])
	{
		xerr(4);						/* no label */
		return;
	}
	setname();							/* move label into main table */
	if ((lblpt = lemt(TRUE, oirt)) != lmte)
	{
		xerr(5);						/* opcode redefined */
		return;
	}
	mmte();								/* make main table entry */
	expr(p1gi);							/* get instruction format */
	if (itype != ITCN || ival.l < 0 || ival.l > OPFF)
	{
		xerr(18);						/* illegal format specifier */
		return;
	}
	lblpt->flags |= ival.l | SYIN;		/* remember format */
	if (fchr != ',')
	{									/* field separator */
		xerr(10);
		return;
	}
	expr(p1gi);							/* get opcode value */
	if (itype != ITCN)
	{
		xerr(17);						/* not a constant */
		return;
	}
	lblpt->vl1.l = ival.l;				/* put value in main table */
	igrst();							/* ignore rest of statement-comment */
}


/* equate directive */
VOID hequ(NOTHING)
{
	if (lbt[0] == 0)
	{
		xerr(4);						/* no label */
		return;
	}
	setname();
	if ((lblpt = lemt(FALSE, sirt)) != lmte)
	{									/* aready there */
		if (lbt[0] == '~')
		{								/* local symbol */
			lblpt = lmte;
			mmte();
		}
	} else
	{
		mmte();
	}
	if (lblpt->flags & SYXR)
	{
		xerr(29);
		return;
	}
	lblpt->flags |= SYDF | SYEQ;		/* defined & equate */
	equflg = 1;
	modelen = LONGSIZ;
	expr(p1gi);
	equflg = 0;
	if (itype == ITSY && ival.ptrw2->flags & SYER)
	{
		lblpt->flags |= SYER;			/* equated register */
		ival.l = ival.ptrw2->vl1.l;
	} else if (itype != ITCN)
	{
		xerr(7);						/* not a constant */
		return;
	}
	if (inoffset && reloc != ABS)
	{
		xerr(11);
		return;
	}
	if (initflg)						/* doing symbol table initialization */
		lblpt->flags |= SYIN;			/* internal symbol */
	lblpt->vl1.l = ival.l;
	if (reloc == DATA)					/* check relocation */
	{
		lblpt->flags |= SYRA;			/* DATA relocatable */
	} else if (reloc == TEXT)
		lblpt->flags |= SYRO;			/* TEXT relocatable */
	else if (reloc == BSS)
		lblpt->flags |= SYBS;			/* BSS relocatable */
	else if (fchr == ',' && (fchr = gchr()) == 'r')
		lblpt->flags |= SYER;			/* equated register */
	if (refpc)							/* flag directive is pc relative */
		lblpt->flags |= SYPC;
	igrst();
}


/* process dsect directive */
VOID hdsect(NOTHING)
{
	dorlst(DATA);
}


VOID dorlst(P(int) xrtyp)
PP(int xrtyp;)
{
	inoffset = 0;						/* offset mode terminated my sect directive */
	chkeven();							/* adjust boundary if need be */
	dlabl();							/* define label on old base if there is one */
	savelc[rlflg] = loctr;				/* save old base relocation */
	rlflg = xrtyp;
	loctr = savelc[xrtyp];				/* set new base relocation ctr */
	opitb();
	stbuf[0].itrl = itwc;
	wostb();
	igrst();
}


/* process psect directive */
VOID hpsect(NOTHING)
{
	dorlst(TEXT);
}


VOID hbss(NOTHING)
{
	dorlst(BSS);
}


/* make pc even */
VOID heven(NOTHING)
{
	modelen = 2;						/* Set up so chkeven() won't barf ... */
	if (loctr & 1)
	{									/* have to make it even */
		dorlst(rlflg);
	} else
	{
		igrst();
	}
}


/* process globl directive */
VOID hent(NOTHING)
{
	while (1)
	{
		gterm(TRUE);					/* get entry symbol */
		if (itype != ITSY)
		{								/* error if not symbol */
			xerr(28);
			return;
		}
		if ((lblpt = lemt(FALSE, sirt)) == lmte)	/* look up in main table */
			mmte();						/* not there, make new entry */
		else if (lblpt->flags & SYER)	/* already there */
			uerr(29);
		lblpt->flags |= SYGL;			/* symbol is an entry */
		if (lblpt->flags & SYXR)		/* been thru hext code */
			lblpt->flags &= ~(SYXR | SYDF);	/* reset for init of .comm */
		if (inoffset && reloc != ABS)
		{
			xerr(11);
			return;
		}
		if (fchr == ',')				/* skip ',' between entries */
			fchr = gchr();
		else
		{
			igrst();					/* statement finished */
			return;
		}
	}
}


/* process comm directive */
VOID hext(NOTHING)
{
	gterm(TRUE);						/* get external symbol */
	if (itype != ITSY)
	{									/* error if not symbol */
		xerr(28);
		return;
	}
	if ((lblpt = lemt(FALSE, sirt)) == lmte)	/* look up in main table */
		mmte();							/* not there, make new entry */
	else if (lblpt->flags & SYDF && (lblpt->flags & SYXR) == 0)	/* already there */
		uerr(29);
	lblpt->flags |= SYXR | SYDF;		/* symbol is an external */
	mkextidx(lblpt);					/* put into external table */
	if (fchr == ',')
	{									/* skip ',' between operands */
		fchr = gchr();
		gterm(TRUE);
		if (itype != ITCN)
		{
			xerr(17);
			return;
		}
		lblpt->vl1.l = ival.l;			/* # bytes of storage required */
	} else
	{
		lblpt->vl1.l = 1;					/* default # bytes */
	}
	igrst();
}


VOID mkextidx(P(struct symtab *) p)
PP(struct symtab *p;)
{
	if (extindx >= EXTSZ)
	{									/* check for overflow of external symbol tbl */
		rpterr("overflow of external table\n");
		endit();
	}
	p->vextno = ((int) ((__intptr_t)pexti - (__intptr_t)extbl)) / sizeof(VOIDPTR);	/* external symbol index # */ /* XXX */
	*pexti++ = p;						/* save external in external table */
	extindx++;
}


/* end statement */
VOID hend(NOTHING)
{
	register short i;

	inoffset = 0;						/* turn off inoffset mode */
	lblpt = 0;							/* no label */
	opitb();							/* output beginning of statement */
	igrst();							/* ignore operands */
	stbuf[0].itrl = itwc;				/* number of it entries */
	wostb();							/* write out statement buffer */
	if (pitix > itbuf)					/* some it in buffer */
		if (write(itfn, itbuf, ITBSZ * (sizeof i)) != ITBSZ * (sizeof i))
		{
			rpterr("I/O write error on it file\n");
			endit();
		}
	if (initflg)
	{
		putsymtab();
		printf("68000 assembler initialized\n");
		endit();
	}
	if ((fchr = gchr()) != CEOF)
		rpterr("end statement not at end of source\n");
	savelc[rlflg] = loctr;				/* last location on current reloc base */
	fixunds();							/* make golbals and maybe undefineds external */
	if (!didorg)						/* did not assign to location counter */
		pass1a();						/* resolve short branches */
	pass2();							/* assembler pass 2 */
}


/* define storage given number of bytes */
VOID hds(NOTHING)
{
	chkeven();							/* may need to make pc even */
	dlabl();							/* define label maybe */
	if (!inoffset)
		opitb();						/* output it for beginning of statement */
	refpc = 0;
	expr(p1gi);
	if (itype != ITCN)
	{
		xerr(17);						/* must be constant */
		return;
	}
	if (reloc != ABS)
	{
		xerr(9);						/* must be absolute */
		return;
	}
	if (!inoffset)
	{									/* don't generate it if in offset */
		opitoo();						/* output one operand */
		stbuf[0].itrl = itwc;
		wostb();						/* write out statement buffer */
	}
	loctr += (ival.l * modelen);
	igrst();
}


/* make pc even if necessary for .dc and .ds */
VOID chkeven(NOTHING)
{
	register struct symtab *pi;

	if (modelen > BYTESIZ && (loctr & 1))
	{
		pi = opcpt;
		opcpt = evenptr;
		opitb();
		stbuf[0].itrl = itwc;
		wostb();
		opcpt = pi;
		loctr++;
	}
}


/* define byte directive */
VOID hdc(NOTHING)
{
	chkeven();
	hdata(modelen);
}


/*
 * define bytes or words of data
 *	call with:
 *		1 => defining bytes
 *		2 => defining words
 *		4 => defining long words
 */
VOID hdata(P(int) mul)
PP(int mul;)
{
	dlabl();							/* define label */
	opitb();							/* beginning of statement */
	numops = 0;							/* initialize count for number of operands */
	opito();							/* output it for operands */
	stbuf[0].itrl = itwc;				/* # of it entries */
	wostb();							/* write out statement buffer */
	loctr += numops * mul;				/* count by bytes or words */
}


/* handle org statement */
VOID horg(NOTHING)
{
	if (rlflg == TEXT && loctr != 0)
		didorg++;						/* can't do branch optimization as separate pass now */
	expr(p1gi);							/* value of new relocation counter */
	if (reloc != rlflg && reloc != ABS)
	{
		xerr(27);
		return;
	}
	if (ival.l < loctr)
	{
		xerr(40);						/* trying to go backwards */
		return;
	}
	opcpt = orgptr;						/* org directive for pass 2 */
	opitb();
	opitoo();
	stbuf[0].itrl = itwc;
	wostb();
	loctr = ival.l;
	dlabl();							/* define label */
}


/* Assemble for mask2 (R9M), ignore... */
VOID hmask2(NOTHING)
{
	igrst();
}


/* Define register list */
VOID hreg(NOTHING)
{
	short mask;

	if (lbt[0] == 0)
	{
		xerr(4);						/* no label */
		return;
	}
	setname();							/* move label into main table */
	if ((lblpt = lemt(FALSE, sirt)) != lmte)
	{
		xerr(5);						/* opcode redefined */
		return;
	}
	if (inoffset)
		if (reloc != ABS)
		{
			xerr(11);
			return;
		}
	mmte();								/* make main table entry */
	if ((mask = mkmask()) == -1)
	{
		xerr(6);
		return;
	}
	lblpt->flags |= SYDF | SYEQ | SYRM;	/* register mask, defined & equated */
	lblpt->vl1.l = mask;
	igrst();
}

short regmsk[] = { 0100000, 040000, 020000, 010000, 04000, 02000, 01000, 0400, 0200,
	0100, 040, 020, 010, 4, 2, 1
};


/* make a register mask for the reg routine */
int mkmask(NOTHING)
{
	register short *p, i, j, mask;

	p = regmsk;
	mask = 0;
	while ((i = chkreg()) != -1)
	{
		if (fchr == '-')
		{
			fchr = gchr();
			if ((j = chkreg()) == -1)
			{
				xerr(40);
				return (-1);
			}
			while (i <= j)
				mask |= p[i++];
		} else
			mask |= p[i];
		if (fchr != '/' && fchr != ',')
			break;						/* Signetics fix */
		fchr = gchr();
	}
	return (mask);
}


/* get a register # from file, return -1 if none or illegal */
int chkreg(NOTHING)
{
	register short i, j;

	i = j = 0;
	if (fchr == 'a' || fchr == 'A')
		i = 8;
	else if (fchr != 'd' && fchr != 'r' && fchr != 'D' && fchr != 'R')
		return (-1);
	fchr = gchr();
	do
	{
		j = (j * 10) + (fchr - '0');
		fchr = gchr();
	} while (isdigit(fchr));
	if (j < 0 || j > AREGHI)
		return (-1);
	i += j;
	if (i >= 0 && i <= AREGHI)
		return (i);
	else
		return (-1);
}


/* Define constant block */
VOID hdcb(NOTHING)
{
	chkeven();							/* on even boundary if not byte block. */
	dlabl();							/* define label... */
	opitb();
	opito();
	stbuf[0].itrl = itwc;
	numops = stbuf[ITOP1].itop.l;
	loctr += numops * modelen;
	wostb();							/* write out statement buffer */
}


/* Command line, similar to ds.b */
VOID hcomline(NOTHING)
{
	dlabl();							/* define label maybe */
	modelen = BYTESIZ;					/* byte store... */
	opitb();							/* output it for beginning of statement */
	refpc = 0;
	expr(p1gi);
	if (itype != ITCN)
	{
		xerr(17);						/* must be constant */
		return;
	}
	if (reloc != ABS)
	{
		xerr(9);						/* must be absolute */
		return;
	}
	opitoo();							/* output one operand */
	stbuf[0].itrl = itwc;
	wostb();							/* write out statement buffer */
	loctr += ival.l;
	igrst();
}


/* Relocateable id record, ignore */
VOID hidnt(NOTHING)
{
	igrst();
}


/* Define offsets */
VOID hoffset(NOTHING)
{
	inoffset = 1;
	expr(p1gi);							/* get new location counter */
	if (itype != ITCN)
	{
		xerr(17);						/* constant required */
		return;
	}
	if (reloc != ABS)
	{
		xerr(9);						/* must be absolute */
		return;
	}
	savelc[rlflg] = loctr;				/* Save old section loctr */
	loctr = ival.l;
	rlflg = ABS;						/* This section is absolute */
	igrst();
}


/* define sections: map to bss, text and data */
VOID hsection(NOTHING)
{
	inoffset = 0;						/* reseting section turns off offset mode */
	dlabl();							/* define label on old base if there is one */
	savelc[rlflg] = loctr;				/* save old base relocation */
	opitb();							/* intermediate table... */
	expr(p1gi);							/* get section # */
	if (itype != ITCN)
	{
		xerr(17);						/* must be a constant */
		return;
	}
	if (ival.l > 15 || ival.l < 0)
	{
		xerr(9);						/* proper range 0..15 */
		return;
	}
	rlflg = (ival.l == 14) ? DATA : (ival.l == 15) ? BSS : TEXT;
	loctr = savelc[rlflg];
	stbuf[3].itop.l = loctr;				/* pass 1 location counter */
	stbuf[3].itrl = rlflg;				/* relocation base */
	stbuf[0].itrl = itwc;
	wostb();
	igrst();
}


/* hopt -- ignore, set up assembler options */
VOID hopt(NOTHING)
{
	igrst();
}


/* httl - title directive, ignore */
VOID httl(NOTHING)
{
	igrst();
}

/****	Second pass directive handling routines ****/

/* second pass end statement */
VOID send(NOTHING)
{
	register short i;

	savelc[rlflg] = loctr;
	if (savelc[TEXT] & 1)
	{
		rlflg = TEXT;
		outbyte(0, DABS);
		savelc[TEXT]++;					/* need correct count here */
	}
	if (savelc[DATA] & 1)
	{
		rlflg = DATA;
		outbyte(0, DABS);
		savelc[DATA]++;					/* need correct count here */
	}
	if (savelc[BSS] & 1)
	{
		savelc[BSS]++;
	}
	ival.l = 0;
	reloc = ABS;
	ckeop(9);
	print(0);
	cpdata();							/* copy data to loader file */
	osymt();							/* output symbol table */
	xfflush(&tbuf);					/* flush text relocation bits */
	cprlbits();							/* copy relocation bits */
	xfflush(&lbuf);
	i = (sizeof couthd.ch_magic) + 3 * (sizeof couthd.ch_tsize);
	if ((lseek(lfn, (long) i, 0) == -1L) || write(lfn, &stlen, sizeof(stlen)) != sizeof(stlen))
		rpterr("I/O error on loader output file\n");
	endit();
}


/* second pass define storage - ds */
VOID sds(NOTHING)
{
	print(0);
	if (rlflg == TEXT || rlflg == DATA)
	{
		expr(p2gi);
		if (itype != ITCN)
		{
			uerr(13);
			return;
		}
		ival.l *= modelen;
		while (ival.l)
		{
			outbyte(0, DABS);
			loctr++;
			ival.l--;
		}
	} else
	{
		loctr += stbuf[ITOP1].itop.l * modelen;	/* reserve storage */
	}
}


/* second pass - define block storage, initialized */
VOID sdcb(NOTHING)
{
	register short pfg, i, hflg, len;

	i = pfg = hflg = 0;
	expr(p2gi);
	if (itype != ITCN || reloc != ABS)
	{
		uerr(13);						/* must be absolute constant */
		return;
	}
	len = ival.l;
	expr(p2gi);
	if (modelen == BYTESIZ && (ival.l < -128 || ival.l >= 256 || reloc != ABS))
	{
		uerr(20);
		ival.l = 0;
		reloc = ABS;
	}
	while (len--)
	{
		if (modelen == BYTESIZ)
		{
			if (!hflg)
			{
				ins[i].b.hibyte = ival.l;
				outbyte(ival.u.loword, DABS);
				hflg++;
			} else
			{
				ins[i++].b.lobyte = ival.l;
				outbyte(ival.u.loword, DABS);
				hflg = 0;
			}
			goto sdbl2;
		} else if (modelen == WORDSIZ)
		{
		  sdbl1:
			ins[i++].w = ival.u.loword;
			outword((int) ival.u.loword, reloc);
		  sdbl2:
			if (i > 3)
			{
				instrlen = i * 2;
				print((pfg++) ? 2 : 1);
				loctr += instrlen;
				i = 0;
			}
		} else
		{								/* long word... */
			ins[i++].w = ival.u.hiword;
			outword(ival.u.hiword, LUPPER);
			goto sdbl1;
		}
	}
	if (i)
	{									/* more printing */
		instrlen = i * 2 - hflg;
		print((pfg) ? 2 : 1);
		loctr += instrlen;
	}
}


/* second pass data */
VOID sdsect(NOTHING)
{
	savelc[rlflg] = loctr;
	rlflg = DATA;
	loctr = savelc[DATA];
	print(0);							/* print the new location counter */
}


/* second pass text */
VOID spsect(NOTHING)
{
	savelc[rlflg] = loctr;
	rlflg = TEXT;
	loctr = savelc[TEXT];
	print(0);							/* print the new location counter */
}


VOID sbss(NOTHING)
{
	savelc[rlflg] = loctr;
	rlflg = BSS;
	loctr = savelc[BSS];
	print(0);							/* print the new location counter */
}


/* make loctr even */
VOID seven(NOTHING)
{
	if (loctr & 1)
	{
		if (rlflg == TEXT || rlflg == DATA)
			outbyte(0, DABS);
		loctr++;
	}
	print(0);
}


/* second pass org */
VOID sorg(NOTHING)
{
	register long l;

	if (rlflg == TEXT || rlflg == DATA)
	{									/* must put out zeros */
		l = stbuf[ITOP1].itop.l - loctr;	/* # zeroes to output */
		ins[0].w = 0;
		print(1);
		while (l > 0)
		{
			outbyte(0, DABS);
			loctr++;
			l--;
		}
	} else
	{
		/* BSS */
		loctr = stbuf[ITOP1].itop.l;	/* new location counter */
		print(0);
	}
}


/*
 * second pass define data (words or bytes)
 * call with
 *	2 => defining words
 *	1 => defining bytes
 *	4 => defining long words
 */
VOID sdata(P(int) dtyp)
PP(int dtyp;)
{
	register short pfg, i, hflg;

	hflg = i = pfg = 0;
	while (1)
	{
		expr(p2gi);						/* evaluate expression */
		if (pitw < pnite)
			pitw--;						/* expr passed a token */
		if (itype != ITCN && reloc != EXTRN)
		{								/* must be constant */
			uerr(13);
			ival.l = 0;
			reloc = ABS;
		}
		if (reloc == EXTRN)
			reloc = (extref << 3) | EXTVAR;	/* gen extern reference */
		if (dtyp == 1)
		{								/* defining a byte */
			if (ival.l < -128 || ival.l >= 256 || reloc != ABS)
			{							/* not a byte */
				uerr(20);
				ival.l = 0;
				reloc = ABS;
			}
			if (!hflg)
			{
				ins[i].b.hibyte = ival.l;
				outbyte(ival.u.loword, DABS);
				hflg++;
			} else
			{
				ins[i++].b.lobyte = ival.l;
				hflg = 0;
				outbyte(ival.u.loword, DABS);
			}
			goto sdal2;
		} else if (dtyp == 2)
		{								/* defining a word */
		  sdal1:
			ins[i++].w = ival.u.loword;
			outword(ival.u.loword, reloc);
		  sdal2:
			if (i > 3)
			{
				instrlen = i * 2;
				print((pfg++) ? 2 : 1);
				loctr += instrlen;
				i = 0;
			}
		} else
		{								/* long words */
			ins[i++].w = ival.u.hiword;
			outword(ival.u.hiword, LUPPER);
			goto sdal1;
		}
		if (!ckeop(15))					/* should be end of operand */
			return;
		pitw++;
		if (ckein())
		{
			if (hflg)
			{
				ins[i++].b.lobyte = 0;
			}
			if (i)
			{							/* more printing */
				instrlen = i * 2 - hflg;
				print((pfg) ? 2 : 1);
				loctr += instrlen;
			}
			return;
		}
	}
}


VOID sdc(NOTHING)
{
	sdata(modelen);
}


VOID ssection(NOTHING)
{
	short sect;

	sect = stbuf[3].itrl;
	if (sect == DATA)
		sdsect();
	else if (sect == BSS)
		sbss();
	else
		spsect();
}

/****	Conditional assembly directives ****/

VOID hifeq(NOTHING)
{
	if (!acok())
		return;
	if (ival.l)
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hifne(NOTHING)
{
	if (!acok())
		return;
	if (!ival.l)
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hiflt(NOTHING)
{
	if (!acok())
		return;
	if (ival.l >= 0)
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hifle(NOTHING)
{
	if (!acok())
		return;
	if (ival.l > 0)
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hifgt(NOTHING)
{
	if (!acok())
		return;
	if (ival.l <= 0)
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hifge(NOTHING)
{
	if (!acok())
		return;
	if (ival.l < 0)
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hifc(NOTHING)
{
	if (!cmp_ops())
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hifnc(NOTHING)
{
	if (cmp_ops())
	{
		if (ca_true)
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}


VOID hendc(NOTHING)
{
	if (!ca)
	{
		xerr(25);						/* unexpected endc */
		return;
	}
	ca--;
	if (!ca_true)
		if (ca_level == ca)
			ca_true = 1;
	igrst();
}


int acok(NOTHING)
{
	expr(p1gi);
	if (itype != ITCN)
	{
		xerr(7);						/* must be a constant */
		return (0);
	}
	if (reloc != ABS)
	{
		xerr(11);						/* must be absolute, no forward references */
		return (0);
	}
	igrst();
	return (1);
}


/* return 1 true, 0 false */
int cmp_ops(NOTHING)
{
	char str1[25], str2[25];
	register short len1, len2;

	if (fchr != '\'')
	{
		xerr(9);
		return FALSE;
	}
	len1 = len2 = 0;
	while ((fchr = gchr()) && fchr != '\'')
	{
		if (fchr == EOLC)
			return FALSE;
		str1[len1++] = fchr;
	}
	if ((fchr = gchr()) != ',')
	{
		xerr(9);
		return FALSE;
	}
	if ((fchr = gchr()) != '\'')
	{
		xerr(10);
		return FALSE;
	}
	while ((fchr = gchr()) && fchr != '\'')
	{
		if (fchr == EOLC)
			return FALSE;
		str2[len2++] = fchr;
	}
	igrst();
	if (len1 != len2)
		return FALSE;
	str1[len1] = str2[len2] = '\0';
	return strcmp(str1, str2) == 0;
}


int strcmp(P(const char *) s, P(const char *) t)
PP(register const char *s;)
PP(register const char *t;)
{
	for (; *s == *t; s++, t++)
		if (*s == '\0')
			return 0;
	return *s - *t;
}
