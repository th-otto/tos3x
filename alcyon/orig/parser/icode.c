/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121

	@(#)icode.c	1.5	12/30/83
*/

#include "parser.h"

VOID outword PROTO((int w));
VOID outendseq PROTO((NOTHING));


#define LMIN(x,y) (x <= y) ? x : y
#define LMAX(x,y) (x >= y) ? x : y

/*
 *  This interfaces the Parser and the Code Generator, note that these
 *  allow you to link together the Parser and the Code Generator.
 */

#define SAVESTATE(sp,xbuf,sb) sp = obp; obp = &xbuf; sb = bol; bol = 1
#define RESTORSTATE(savep,sbol)   obp = savep; bol = sbol

short inittype;

short bol = 1;

short begseq;

/* output current line number into icode */
VOID outline(NOTHING)
{
	if (!bol)
		putchar('\n');
	printf(".%x.%s\n", lineno, source);
	lst_ln_id = lineno;
}


/* defbdata - set up for byte data, was outbdata */
VOID defbdata(NOTHING)
{
	inittype = CHAR;
	printf("\t.dc.b ");
}


/* defwdata - set up for word data, was outwdata */
VOID defwdata(NOTHING)
{
	inittype = INT;
	printf("\t.dc.w ");
}


/* defldata - set up for long data, */
VOID defldata(NOTHING)
{
	inittype = LONG;
	printf("\t.dc.l ");
}


/* outc - output a constant */
VOID outc(P(int) type, P(int) value)
PP(int type;)
PP(int value;)
{
	if (type == CHAR)
		defbdata();
	else
		defwdata();
	printf("$%x\n", value);
}


/* outdata - set up for data output */
VOID outdata(NOTHING)
{
	inittype = INT;
	printf("\t.data\n");
}


/* outldata - set up for long data output */
VOID outldata(NOTHING)
{
	inittype = LONG;
	printf("\t.data\n");
}


/* outfpdata - set up for floating poshort data output */
VOID outfpdata(NOTHING)
{
	inittype = FLOAT;
	printf("\t.data\n");
}


/* outbexit - output function exit code */
VOID outbexit(P(int) nlocs, P(int) nds, P(int) nas)
PP(int nlocs;)
PP(int nds;)								/* number of D registers */
PP(int nas;)								/* number of A registers */
{
	struct iob *savep;
	short sbol;

	if (gflag)							/* for symbolic debugger */
		printf("\n\t~_lE%d:", lineno);
	if (nds || nas)
	{
		printf("\ttst.l (sp)+\n\tmovem.l (sp)+,");	/* 1 arg stuff */
		if (nds)
		{
			printf("R%d-R7", 8 - nds);
			if (nas)
				putchar('/');
		}
		if (nas)
			printf("R%d-R13", 14 - nas);
		putchar('\n');
	}
	printf("\tunlk R14\n\trts\n");
	SAVESTATE(savep, lbuf, sbol);
	if (!nds && !nas)					/* adjust for 1 arg */
		nlocs += 4;
	printf("link R14,#%d\n", -nlocs);
	if (nds || nas)
	{
		printf("movem.l R%d-R7", 7 - nds);
		if (nas)
			printf("/R%d-R13,-(sp)\n", 14 - nas);
		else
			printf(",-(sp)\n");
	}
	putchar('%');
	RESTORSTATE(savep, sbol);
}


/* outlocal - output local symbol for debugger */
VOID outlocal(P(int) type, P(int) sc, P(const char *) sym, P(int) val)
PP(int type;)								/* local name type */
PP(int sc;)									/* storage type */
PP(const char *sym;)								/* symbol name */
PP(int val;)
{
	switch (sc)
	{
	case STATIC:
		if (NOTFUNCTION(type))
			printf("\t~%.8s=L%d\n", sym, val);
		break;

	case REGISTER:
	case PDECREG:
		printf("\t~%.8s=R%d\n", sym, val);
		break;

	case AUTO:
	case PDECLIST:
		printf("\t~%.8s=%d\n", sym, val);
		break;
	}
}

#define DSW "\tcmp.l (R8)+,R0\n\tdbeq R1,L%d\n\tmove.l %d(R8),R8\n\tjmp (R8)\n"

/* outswitch - output switch table info */
VOID outswitch(P(int) ncases, P(int) deflab, P(struct swtch *) sp)
PP(int ncases;)								/* number of cases in switch */
PP(int deflab;)								/* default label */
PP(struct swtch *sp;)						/* switch table pointer */
{
	register short vdif, val, hval, i, tlab;
	register struct swtch *s;
	long lswvalue;

	val = sp->sw_value;
	hval = sp[ncases - 1].sw_value;
	vdif = hval - val;
	if (ncases <= 4)
	{
		/*
	     * simple switch, do compares and branches, followed by branch to default
	     */
		for (s = sp; --ncases >= 0; s++)
		{
			if (!s->sw_value)
				printf("\ttst R0\n");
			else
				printf("\tcmp #%d,R0\n", s->sw_value);
			printf("\tbeq L%d\n", s->sw_label);
		}
		OUTGOTO(deflab);
	} else if (vdif > 0 && vdif <= ncases * 3)
	{
		/* jump switch, uses value in R0 to index into table of labels */

		if (val)
			printf("\tsub #%d,R0\n", val);
		tlab = nextlabel++;
		printf("\tcmp #%d,R0\n\tbhi L%d\n", vdif, deflab);	/* check for max */
		printf("\tasl #2,R0\n\tmove R0,R8\n\tadd.l #L%d,R8\n", tlab);
		printf("\tmove.l (R8),R8\n\tjmp (R8)\n");
		outdata();
		OUTLAB(tlab);
		for (s = sp; val <= hval; val++)
		{
			if (val == s->sw_value)
			{
				OUTCLAB(s->sw_label);
				s++;
			} else
				OUTCLAB(deflab);
		}
		OUTTEXT();
	} else
	{
		/*
	     * direct switch, searches down table of values for match, if match
	     * found, branches to corresponding label in label table.
	     */
		tlab = nextlabel++;
		printf("\text.l R0\n\tmove.l #L%d,R8\n\tmove #%d,R1\n", tlab, ncases);
		i = nextlabel++;
		OUTLAB(i);						/* loop label */
		printf(DSW, i, ncases * 4);
		outdata();
		OUTLAB(tlab);
		for (s = sp, i = ncases; --i >= 0; s++)
		{
			lswvalue = s->sw_value;
			OUTLCON(lswvalue);
		}
		OUTLCON(0L);					/* mark for default label */
		for (s = sp, i = ncases; --i >= 0; s++)
			OUTCLAB(s->sw_label);
		OUTCLAB(deflab);
		OUTTEXT();
	}
}

VOID outeof(NOTHING)
{
	v6flush(&lbuf);
	v6flush(&sbuf);
	v6flush(&obuf);
}


/* copysfile - copy string file to end of output file */
VOID copysfile(P(const char *) fname)
PP(const char *fname;)
{
	register short c;

	close(fileno(&sbuf));
	if (fopen(fname, &sbuf, 0) < 0)		/* 3rd arg for versados */
		ferror("can't copy %s", fname);
	while ((c = getc(&sbuf)) > 0)
		putc(c, &obuf);
	v6flush(&obuf);
}


/* outword - output a word of data */
VOID outword(P(int) w)								/* word expression */
PP(int w;)
{
	if (begseq)
		putchar(',');
	begseq++;
	printf("$%x", w);
}


/*
 * outlong - output a long data
 * outfp - output floating poshort data
 */
VOID outfp_or_l(P(long) l)
PP(long l;)									/* long data (float or fixed point) to output */
{
	defldata();
	printf("$%lx", l);
	outendseq();
}


VOID outendseq(NOTHING)
{
	begseq = 0;
	putchar('\n');
}


/*
 * outtstr - output text string
 *      This outputs a string to the string file, this is used wherever
 *      you cannot output the string directly to data space, such as in
 *      the middle of expressions.
 */
VOID outtstr(P(int) lab)
PP(int lab;)
{
	struct iob *savep;

	short sbol;

	SAVESTATE(savep, sbuf, sbol);		/* save to restore later... */
	printf("\tL%d:", lab);
	outstr((long) cstrsize, (long) cstrsize);
	RESTORSTATE(savep, sbol);
}


/*
 * outstr - output a string as a sequence of bytes
 * Outputs ".dc.b <byte1>,<byte2>,...,<0>
 */
long outstr(P(long) maxsize, P(long) strsize)
PP(long maxsize;)
PP(long strsize;)
{
	register char *s;
	register long i;

	defbdata();
	i = LMIN(strsize, maxsize);
	for (s = cstr; i > 0; i--)
	{
		outword((int) (*s++ & 0xff));
		if (begseq == 30 && i > 2)
		{
			outendseq();				/* limit line length to something */
			defbdata();					/* reasonable, next .dc.b */
		}
	}
	outendseq();
	if (maxsize > strsize)
		OUTRESMEM((long) (maxsize - strsize));
	else if (maxsize && (strsize > maxsize))
		warning("String initalizer truncated");
	return LMAX(strsize, maxsize);

}


/* cputc - put a character to a file descriptor (used by error) */
VOID cputc(P(char) c, P(int) fn)
PP(char c;)
PP(int fn;)
{
	if (fn == STDERR)
		write(STDERR, &c, 1);
	else
		putchar(c);
}


/*
 * putchar - This catches tabs to allow for the integration of the
 *      parser and code generator into one pass.  By merely throwing
 *      away the tabs here, the output will be OK for the assembler.
 */
VOID putchar(P(char) c)
PP(char c;)
{
	if (c == '\t')
	{
		if (bol)						/* not used  && !onepass ) */
			putc('(', obp);				/* for code generator */
	} else
	{
		bol = (c == '\n');
		putc(c, obp);
	}
}
