/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"



#define LMIN(x,y) (x <= y) ? x : y
#define LMAX(x,y) (x >= y) ? x : y

/*
 *  This interfaces the Parser and the Code Generator, note that these
 *  allow you to link together the Parser and the Code Generator.
 */

#define SAVESTATE(sp,xfil,sb) sp = obp; obp = xfil; sb = bol; bol = 1
#define RESTORSTATE(savep,sbol)   obp = savep; bol = sbol

short inittype;

short bol = 1;

short begseq;

/* output current line number into icode */
VOID outline(NOTHING)
{
	if (!bol)
		oputchar('\n');
	oprintf(".%X.%s\n", lineno, source);
	lst_ln_id = lineno;
}


/* defbdata - set up for byte data, was outbdata */
VOID defbdata(NOTHING)
{
	inittype = CHAR;
	oprintf("\t.dc.b ");
}


/* defwdata - set up for word data, was outwdata */
VOID defwdata(NOTHING)
{
	inittype = INT;
	oprintf("\t.dc.w ");
}


/* defldata - set up for long data, */
VOID defldata(NOTHING)
{
	inittype = LONG;
	oprintf("\t.dc.l ");
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
	oprintf("$%X\n", (unsigned short)value);
}


/* outdata - set up for data output */
VOID outdata(NOTHING)
{
	inittype = INT;
	oprintf("\t.data\n");
}


/* outldata - set up for long data output */
VOID outldata(NOTHING)
{
	inittype = LONG;
	oprintf("\t.data\n");
}


/* outfpdata - set up for floating poshort data output */
VOID outfpdata(NOTHING)
{
	inittype = FLOAT;
	oprintf("\t.data\n");
}


/* outbexit - output function exit code */
VOID outbexit(P(int) nlocs, P(int) nds, P(int) nas)
PP(int nlocs;)
PP(int nds;)								/* number of D registers */
PP(int nas;)								/* number of A registers */
{
	FILE *savep;
	int sbol;

	if (gflag)							/* for symbolic debugger */
		oprintf("\n\t~_lE%d:", lineno);
	if (nds || nas)
	{
		oprintf("\ttst.l (sp)+\n\tmovem.l (sp)+,");	/* 1 arg stuff */
		if (nds)
		{
			oprintf("R%d-R7", 8 - nds);
			if (nas)
				oputchar('/');
		}
		if (nas)
			oprintf("R%d-R13", 14 - nas);
		oputchar('\n');
	}
	oprintf("\tunlk R14\n\trts\n");
	SAVESTATE(savep, lfil, sbol);
	if (!nds && !nas)					/* adjust for 1 arg */
		nlocs += 4;
	oprintf("link R14,#%d\n", -nlocs);
	if (nds || nas)
	{
		oprintf("movem.l R%d-R7", 7 - nds);
		if (nas)
			oprintf("/R%d-R13,-(sp)\n", 14 - nas);
		else
			oprintf(",-(sp)\n");
	}
	oputchar('%');
	RESTORSTATE(savep, sbol);
}


/* outlocal - output local symbol for debugger */
VOID outlocal(P(int) type, P(int) sc, P(const char *) sym, P(int) val)
PP(int type;)								/* local name type */
PP(int sc;)									/* storage type */
PP(const char *sym;)								/* symbol name */
PP(int val;)
{
	if (!gflag)
		return;
	switch (sc)
	{
	case STATIC:
		if (NOTFUNCTION(type))
			oprintf("\t~%.*s=L%d\n", SSIZE, sym, val);
		break;

	case REGISTER:
	case PDECREG:
		oprintf("\t~%.*s=R%d\n", SSIZE, sym, val);
		break;

	case AUTO:
	case PDECLIST:
		oprintf("\t~%.*s=%d\n", SSIZE, sym, val);
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
	int32_t lswvalue;

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
				oprintf("\ttst R0\n");
			else
				oprintf("\tcmp #%d,R0\n", s->sw_value);
			oprintf("\tbeq L%d\n", s->sw_label);
		}
		OUTGOTO(deflab);
	} else if (vdif > 0 && vdif <= ncases * 3)
	{
		/* jump switch, uses value in R0 to index into table of labels */

		if (val)
			oprintf("\tsub #%d,R0\n", val);
		tlab = nextlabel++;
		oprintf("\tcmp #%d,R0\n\tbhi L%d\n", vdif, deflab);	/* check for max */
		oprintf("\tasl #2,R0\n\tmove R0,R8\n\tadd.l #L%d,R8\n", tlab);
		oprintf("\tmove.l (R8),R8\n\tjmp (R8)\n");
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
		oprintf("\text.l R0\n\tmove.l #L%d,R8\n\tmove #%d,R1\n", tlab, ncases);
		i = nextlabel++;
		OUTLAB(i);						/* loop label */
		oprintf(DSW, i, ncases * 4);
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


/* outword - output a word of data */
static VOID outword(P(int) w)								/* word expression */
PP(int w;)
{
	if (begseq)
		oputchar(',');
	begseq++;
	oprintf("$%X", (unsigned short)w);
}


static VOID outendseq(NOTHING)
{
	begseq = 0;
	oputchar('\n');
}


/*
 * outlong - output a long data
 * outfp - output floating poshort data
 */
VOID outfp_or_l(P(int32_t) l)
PP(int32_t l;)									/* long data (float or fixed point) to output */
{
	defldata();
	oprintf("$%lx", (long)l);
	outendseq();
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
	FILE *savep;
	int sbol;

	SAVESTATE(savep, sfil, sbol);		/* save to restore later... */
	oprintf("\tL%d:", lab);
	outstr((int32_t) cstrsize, (int32_t) cstrsize);
	RESTORSTATE(savep, sbol);
}


/*
 * outstr - output a string as a sequence of bytes
 * Outputs ".dc.b <byte1>,<byte2>,...,<0>
 */
int32_t outstr(P(int32_t) maxsize, P(int32_t) strsize)
PP(int32_t maxsize;)
PP(int32_t strsize;)
{
	register char *s;
	register int32_t i;

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
		OUTRESMEM((int32_t) (maxsize - strsize));
	else if (maxsize && (strsize > maxsize))
		warning(_("String initalizer truncated"));
	return LMAX(strsize, maxsize);

}


/*
 * oputchar - This catches tabs to allow for the integration of the
 *      parser and code generator into one pass.  By merely throwing
 *      away the tabs here, the output will be OK for the assembler.
 */
VOID oputchar(P(char) c)
PP(char c;)
{
	if (c == '\t')
	{
		if (bol)						/* not used  && !onepass ) */
			kputc('(', obp);				/* for code generator */
	} else
	{
		bol = c == '\n';
		kputc(c, obp);
	}
}
