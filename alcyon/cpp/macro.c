/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"
#include <ctype.h>

int literal;
int lit_num;
char lit_file[MAXPSIZE];
char null[] = "";
int nincl;
char *incl[NINCL];

char line[LINESIZE+2];				/* line buffer */
char *linep;						/* current line pointer */
int loverflow;						/* line overflow flag */

struct defstruc defs[NDEFS];

static int skip;					/* skipping current line */
static char *defp;					/* pointer to next avail define byte */
static int defcount;				/* bytes left in define area */
static int defused;					/* number of bytes used in define area */
static int defmax;					/* maximum define area used */

static int clabel = LABSTART;
static int nlabel = LABSTART + 1;

static char tmp[NUMLEN];						/* temporary spot for line number itoa conversion */


static struct builtin
{
	char *b_name;
	int b_type;
} btab[] =
{
	{ "define", DEFINE },
	{ "include", INCLUDE },
	{ "undef", UNDEF },
	{ "ifdef", IFDEF },
	{ "ifndef", IFNDEF },
	{ "else", ELSE },
	{ "endif", ENDIF },
	{ "if", IF },
	{ "line", LINE },
	{ 0, 0 }
};

static char cstack[CSTKSIZE];
static char *cstkptr;
static char inclname[TOKSIZE];


static int getaline(const char *infile);


/*
 * domacro - do macro processing
 *      Does the macro pre-processing on the input file and leaves the
 *      result on the output file.
 */
int domacro(int nd)
{
	register char *l;
	register struct symbol *sp;

	filep = &filestack[0];				/* [vlh] 4.0 moved for error msgs */
	lineno = 1;							/* [vlh] 4.0 moved for error msgs */
	if ((inbuf = fopen(source, "r")) == NULL)
	{									/* 3rd arg for versados */
		error("can't open source file %s\n", source);
		return 0;
	}
	if (!Eflag)
	{
		if ((outbuf = fopen(dest, "w")) == NULL)
		{								/* 3rd arg for versados */
			error("can't creat %s\n", dest);
			return 0;
		}
	}
	putid(source, 1);					/* identify as first line in source file */

	for (sp = &symtab[0]; sp <= &symtab[HSIZE - 1]; sp++)	/*3.4 */
		sp->s_def = null;				/* clear out symbol table */
	defp = malloc(DEFSIZE);
	if (defp == NULL)
	{
		error("define table overflow");
		cexit();
	}
	defmax = defcount = DEFSIZE;

	defused = 0;
	pbp = &pbbuf[0];
	cstkptr = &cstack[0];
	install("Newlabel", NEWLABEL);
	install("Label", LABEL);
	while (--nd >= 0)
		dinstall(defs[nd].ptr, defs[nd].value);
	while (getaline(source))
	{
		l = line;
		if (filep == &filestack[0] && pbp == &pbbuf[0])
			lineno++;
		else if (filep != &filestack[0])	/*[vlh] include file */
			(filep - 1)->lineno++;
		while (*l)
			fputc(*l++, outbuf);
		fputc(' ', outbuf);
		fputc('\n', outbuf);
		if (literal)
		{
			if (filep != &filestack[0])
			{
				(filep - 1)->lineno++;
				putid((filep - 1)->ifile, (filep - 1)->lineno);
			} else
			{
				lineno++;
				putid(source, lineno);
			}
			literal = 0;
		}
	}
	if (cstkptr != &cstack[0])
		error("unmatched conditional");
	if (defused > defmax)
		defmax = defused;
	fflush(outbuf);
	if (!Eflag)
		fclose(outbuf);
	fclose(inbuf);
	return 1;
}


/* [vlh] 4.0 SOH line header */
void putid(const char *fname, int lnum)
{
	register const char *p;

	if (asflag || pflag)
		return;

	if (literal)
	{
		strcpy(lit_file, fname);
		lit_num = lnum;
	}

	fputc('#', outbuf);
	fputc(' ', outbuf);
	itoa(lnum, tmp, NUMLEN - 1);
	for (p = tmp; *p == ' ';)
		p++;
	for (; *p; p++)
		fputc(*p, outbuf);
	fputc(' ', outbuf);
	fputc('"', outbuf);
	for (p = fname; *p; p++)
		fputc(*p, outbuf);
	fputc('"', outbuf);
	fputc('\n', outbuf);
}


void install(const char *name, int def)
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name, sp->s_name);
	sp->s_def = defp;
	putd(def);
	putd('\0');
}


void dinstall(const char *name, const char *def)
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name, sp->s_name);
	sp->s_def = defp;
	putd(NOARGS);
	if (def)							/* [vlh] character strings... */
		while (*def)
			putd(*def++);
	else
		putd('1');						/* [vlh] default define value */
	putd('\0');
}


/* kwlook - look up the macro built-in names*/
/*      Searches thru the built-in table for the name.*/
int kwlook(const char *name)
{
	register struct builtin *bp;

	for (bp = &btab[0]; bp->b_name; bp++)
		if (strcmp(bp->b_name, name) == 0)
			return bp->b_type;
	return 0;
}


/* special - check for predefined macros, if they exist expand them */
/*  __FILE - current file name, __LINE - current line number */
static int special(const char *token, const char *infile)
{
	register const char *p;
	int xline;
	char buf[8];

	if (strcmp(token, "__FILE") == 0)
	{
		putl('"');
		for (p = infile; *p;)
			putl(*p++);
		putl('"');
	} else if (strcmp(token, "__LINE") == 0)
	{
		xline = (literal) ? lit_num : (filep == &filestack[0]) ? lineno : (filep - 1)->lineno;
		itoa(xline, buf, 7);
		buf[7] = 0;
		for (p = &buf[0]; *p == ' ';)
			p++;
		while (*p)
			putl(*p++);
	} else
		return 0;
	return 1;
}


/* eatup - eat up the rest of the input line until a newline or CEOF*/
/*      Does gettok calls.*/
static void eatup(void)									/* returns - none */
{
	register int type;
	char etoken[TOKSIZE];

	while ((type = gettok(etoken)) != NEWL && type != CEOF)
		;
}


/* undefine - does undef command */
/*      Sets the symbols definition to the null pointer*/
static void undefine(const char *name)
{
	register struct symbol *sp;

	sp = getsp(name);
	if (sp->s_def)
		sp->s_def = null;
}


/* pattern - if the pattern occurs in the token starting at the first */
/*  position in the string, pattern returns the length of the pattern */
/*  else pattern returns a zero. */
static int pattern(const char *pat, const char *token)
{
	register int len;

	len = strlen(pat);
	if (len > strlen(token))			/* couldn't possibly work */
		return 0;
	if (isalnum(token[len]) || token[len] == '_')
		return 0;						/* not deliminated by non-alphanumeric */

	for (len = 0; *pat;)
	{
		if (*pat++ != *token++)
			return 0;
		len++;
	}
	return len;
}


/* trymatch - check for arguments */
static void trymatch(const char *token, int type, int nargs, char *args[])
{
	register const char *p;
	register int i, len;

	p = token;
	if (type != ALPHA)
		putd(*p++);
	while (*p != 0)
	{
		for (i = 0; i < nargs; i++)
			if ((len = pattern(args[i], p)) != 0)
				break;
		if (i < nargs)
		{								/* sub ARG marker for formal arg */
			putd(i + 1);
			putd(ARG);
			p += len;
		} else
			do
			{
				putd(*p++);
			} while (isalnum(*p) || *p == '_');
		while (!(isalnum(*p)) && *p != '_' && *p)	/* get to next possible */
			putd(*p++);
	}
}


/* getfarg - get macro formal parameters*/
/*      Skips blanks and handles "," and ")".*/
static int getfarg(char *token)
{
	register int type;

	if ((type = getntok(token)) == RPAREN || type == ALPHA)
		return type;
	if (type != COMMA || (type = getntok(token)) != ALPHA)
		error("bad argument:%s", token);
	return type;
}


/* dodefine - do #define processing*/
/*      Checks the define name, collects formal arguements and saves*/
/*      macro definition, substituting for formal arguments as it goes.*/
static void dodefine(void)
{
	char token[TOKSIZE],
	*args[MAXARGS],
	 argbuf[ARGBSIZE];

	register char *abp,
	*p;

	register int type,
	 nargs,
	 i;

	register struct symbol *sp;

	if ((type = getntok(token)) != ALPHA)
	{
		error("bad define name: %s", token);
		return;
	}
	sp = getsp(token);
	symcopy(token, sp->s_name);
	sp->s_def = defp;
	nargs = 0;
	abp = argbuf;
	if ((type = gettok(token)) == LPAREN)
	{
		for (; (type = getfarg(token)) != RPAREN; nargs++)
		{
			if (nargs >= MAXARGS)
			{
				error("too many arguments");
				break;
			}
			args[nargs] = abp;
			for (p = token; (*abp++ = *p++) != 0;)
			{
				if (abp >= &argbuf[ARGBSIZE])
				{
					error("argument buffer overflow");
					break;
				}
			}
		}
		putd(nargs);
	} else
	{
		pbtok(token);
		putd(NOARGS);
	}
	type = getntok(token);				/*get next non-white token */
	for (; type != NEWL && type != CEOF; type = gettok(token))
	{
		if (type == ALPHA || type == SQUOTE || type == DQUOTE)
		{								/* [vlh] 4.1 */
			trymatch(token, type, nargs, args);	/* [vlh] 4.1 */
			continue;
		} else if (type == BSLASH)
		{
			if ((i = ngetch()) == '\n')
			{							/*multi-line macro? */
				if (filep == &filestack[0] && pbp == &pbbuf[0])
				{
					lineno++;
					fputc('\n', outbuf);
				}
			}
			putd(i);
			continue;
		}
		for (p = token; *p;)
			putd(*p++);
	}
	pbtok(token);
	putd('\0');
}


/* push - push a #ifdef condition value on condition stack*/
/*      Checks for stack overflow.*/
static void push(int val)
{
	if (cstkptr >= &cstack[CSTKSIZE])
	{
		error("condition stack overflow");
		cexit();
	}
	*cstkptr++ = val;
}


/* pop - pop the #ifdef, etc. condition stack*/
/*      Checks for stack undeflow.*/
static int pop(void)
{
	if (cstkptr <= &cstack[0])
		return -1;
	return *--cstkptr;
}


static void doifile(const char *p)
{
	register char *iptr;

	for (iptr = filep->ifile; *p;)
		*iptr++ = *p++;
	*iptr = 0;
}


/* getinclude - get include file full pathname */
static char *getinclude(const char *fname, const char *parent)
{
	register const char *q;
	register char *t;
	register int i, ndx;
	register FILE *fd;
	
	if (parent)
	{									/* include filename surrounded by quotes */
		q = (filep == &filestack[0]) ? parent : (filep - 1)->ifile;
		t = &inclname[0];
		while ((ndx = strindex(q, FILESEP)) >= 0)
		{
			ndx++;
			while (ndx--)
				*t++ = *q++;
		}
		for (q = fname; (*t++ = *q++) != 0; )
			;
		*t = 0;
		if ((fd = fopen(inclname, "r")) != NULL)
		{								/* found it */
			fclose(fd);
			return inclname;
		}
	}
	for (i = 0; i < nincl; i++)
	{
		for (t = inclname, q = incl[i]; (*t++ = *q++) != 0; )
			;
		*(t - 1) = FILESEP;
		for (q = fname; (*t++ = *q++) != 0; )
			;
		*t = 0;
		if ((fd = fopen(inclname, "r")) != NULL)
		{
			fclose(fd);
			return inclname;
		}
	}
	return inclname;
}


/* doinclude - handle #include command*/
/*      Checks for file name or library file name and pushes file on*/
/*      include file stack.*/
static void doinclude(const char *infile)
{
	register int type;
	char token[TOKSIZE];
	char fname[TOKSIZE];
	register char *p, *q, c;

	p = fname;
	if ((type = getntok(token)) == SQUOTE || type == DQUOTE)
	{
		for (c = token[0], q = &token[1]; *q != c;)
			*p++ = *q++;
		*p = '\0';
		p = getinclude(fname, infile);
	} else if (type != LESS)
	{
		error("bad include file");
		return;
	} else
	{
		while ((type = gettok(token)) != GREAT && type != NEWL && type != CEOF)
			for (q = token; (*p = *q++) != 0; p++)
				;
		if (type != GREAT)
		{
			error("bad include file name");
			pbtok(token);
			return;
		}
		p = getinclude(fname, (char *) 0L);
	}
	eatup();							/*need here... */
	if (filep >= &filestack[FSTACK])
		error("includes nested too deeply");
	else
	{
		if ((inbuf = fopen(p, "r")) == NULL)
		{								/* 3rd arg for versados */
			if (type != SQUOTE && type != DQUOTE)
				error("can't open include file %s", p);
			else
				error("can't open include file %s", fname);
		} else
		{
			filep->ifd = inbuf;
			filep->lineno = 1;			/* [vlh] */
			putid(p, 1);				/* id for include file */
			doifile(p);
			filep++;
		}
	}
}


#define SKIPWHITE(ch)     do { ch = ngetch(); } while (ctype[ch] == WHITE)
static void doline(void)
{
	register char *ptr;
	char token[TOKSIZE];
	register int ch, lnum, type;

	/* get line number associated with #LINE */
	while ((type = gettok(token)) == WHITE) ;	/* skip white space */
	if (type != DIGIT)
	{
		error("invalid #line args");
		return;
	}
	lnum = atoi(token);
	literal = 1;
	SKIPWHITE(ch);
	if (ctype[ch] != NEWL && ctype[ch] != CEOF)
	{									/* associated filename */
		ptr = &token[0];
		do
		{
			*ptr++ = ch;
			ch = ngetch();
		} while (ctype[ch] != NEWL && ctype[ch] != CEOF && ctype[ch] != WHITE);
		*ptr = 0;
		putid(token, lnum);
	} else /* source or header file */ if (filep == &filestack[0])
		putid(source, lnum);			/* [vlh] 4.2.c */
	else
		putid((filep - 1)->ifile, lnum);	/* [vlh] 4.2.c */
	if (ch != NEWL)
		for (; ctype[ch] != NEWL && ctype[ch] != CEOF;)
			ch = ngetch();
}


/* putl - put a character to the current output line */
/*      Checks for line overflow.*/
void putl(int c)
{
	if (linep < &line[LINESIZE])
		*linep++ = c;
	else if (!loverflow)
	{
		loverflow++;
		error("line overflow");
	}
}


/* initl - initialize current line*/
/*      Sets the line pointer and the line overflow flag.*/
void initl(void)
{
	*(linep = &line[0]) = '\0';
	loverflow = 0;
}


/* putd - put a character to the define buffer*/
/*      Does dynamic allocation for define buffer*/
void putd(int c)
{
	if (!defcount)
	{
		if ((defp = realloc(defp, defused + DEFSIZE)) == NULL)
		{
			error("define table overflow");
			cexit();
		}
		defcount = DEFSIZE;
	}
	defused++;
	defcount--;
	*defp++ = c;
}


/* getaarg - get macro actual argument*/
/*      This handles the collecting of the macro's call arguments.*/
/*      Note that you may have parenthesis as part of the macro argument,*/
/*      hence you need to keep track of them.*/
static int getaarg(char *argp)
{
	int type, plevel, i;
	register char *p;
	register char *ap;
	char token[TOKSIZE];

	ap = argp;
	*ap = '\0';
	plevel = 0;
	i = TOKSIZE;
	while (((type = gettok(token)) != COMMA && type != RPAREN) || plevel)
	{
		for (p = token; (*ap = *p++) != 0; ap++)
			if (--i <= 0)
			{
				error("macro argument too long");
				return CEOF;
			}
		if (type == LPAREN)
			plevel++;
		else if (type == RPAREN)
			plevel--;
		else if (type == CEOF)
		{
			error("unexpected EOF");
			cexit();
		}
	}
	if (ap == argp)
		type = CEOF;
	return type;
}


static void pbnum(int num)
{
	register int digit;

	do
	{
		digit = num % 10;
		num /= 10;
		putback(digit + '0');
	} while (num > 0);
}


/* expand - expands the macro definition*/
/*  Checks for define recursion and #define x x problems, collects*/
/*  the actual arguments using getaarg, and then expands the macro*/
/*  by pushing it onto the push back buffer, substituting arguments*/
/*  as it goes.*/
void expand(struct symbol *sp)
{
	char argbuf[ARGBSIZE];
	char *args[MAXARGS];
	char token[TOKSIZE];
	register char *p, *abp, *mdef;
	register int i, j, nargs, type;

	if (pbflag++ > 100)
	{
		error("define recursion");
		return;
	}
	if (strcmp(sp->s_name, mdef = sp->s_def) == 0)
	{									/*handle #define x x */
		while (*mdef)
			putl(*mdef++);
		return;
	}
	nargs = 0;
	if (*mdef == NOARGS)				/*suppress grabbing of args */
		;
	else if (getntok(token) != LPAREN)	/* [vlh] 4.1 ignore white space */
		pbtok(token);
	else
	{
		abp = &argbuf[0];
		while ((type = getaarg(token)) != CEOF)
		{
			if (nargs >= MAXARGS)
			{
				error("too many arguments");
				return;
			}
			args[nargs++] = abp;
			for (p = token; (*abp++ = *p++) != 0; )
			{
				if (abp >= &argbuf[ARGBSIZE])
				{
					error("argument buffer overflow");
					return;
				}
			}
			if (type == RPAREN)
				break;
		}
	}
	if (*mdef == NEWLABEL)
	{
		clabel = nlabel;
		if (!nargs)
			nlabel++;
		else
			nlabel += atoi(args[0]);
	} else if (*mdef == LABEL)
	{
		if (!nargs)
			i = clabel;
		else
			i = clabel + atoi(args[0]);
		pbnum(i);
		pbtok("_L");
	} else
	{
		mdef++;							/*skip no. of args */
		for (p = mdef + strlen(mdef) - 1; p >= mdef; p--)
		{
			if (*p == ARG)
			{
				if ((j = *--p) <= nargs)
					pbtok(args[j - 1]);
			} else
				putback(*p);
		}
	}
}


/* getntok - get next token, suppressing white space*/
/*      Merely gettok's until non-white space is there*/
int getntok(char *token)
{
	register int type;

	while ((type = gettok(token)) == WHITE)
		;
	return type;
}


/*
 * getaline - get input line handling macro statements
 *  Checks for a preprocessor statement on the line and if there
 *  is one there, it processes it.  Note that most of the work is
 *  in determining whether we need to skip the current line or not.
 *  This is all handled with the condition stack and the skip variable.
 *  The skip variable is non-zero if any condition on the condition
 *  stack is SKIP.
 */
static int getaline(const char *infile)
{
	char token[TOKSIZE];
	register int type, i;
	register struct symbol *p;
	register const char *cp;
	
	initl();
	if ((type = gettok(token)) == CEOF)
		return 0;
	if (type == POUND)
	{
		if ((type = getntok(token)) == NEWL)
			return 1;
		switch (kwlook(token))
		{

		case IFDEF:
			if (getntok(token) == ALPHA && lookup(token))
				push(NOSKIP);
			else
			{
				push(SKIP);
				skip++;
			}
			break;

		case IFNDEF:
			if (getntok(token) == ALPHA && lookup(token))
			{
				push(SKIP);
				skip++;
			} else
				push(NOSKIP);
			break;

		case ENDIF:
			if ((i = pop()) == SKIP)
				skip--;
			else if (i != NOSKIP)
				error("invalid #endif");
			break;

		case ELSE:
			if ((i = pop()) == SKIP)
			{
				skip--;
				push(NOSKIP);
			} else if (i == NOSKIP)
			{
				skip++;
				push(SKIP);
			} else
				error("invalid #else");
			break;

		case DEFINE:
			if (!skip)					/*if in skip, don't do define */
				dodefine();
			break;

		case UNDEF:
			if (!skip)
			{							/*if in skip, don't undef */
				if ((type = getntok(token)) == ALPHA)
					undefine(token);
			}
			break;

		case INCLUDE:
			if (!skip)
			{							/*if in skip, don't do include */
				doinclude(infile);
				if (filep != &filestack[0])
					i = getaline((filep - 1)->ifile);
				else
					i = getaline(infile);
				return i;
			}
			break;

		case IF:
			if (cexpr())				/*evaluate constant expression */
				push(NOSKIP);			/*non-zero, so don't skip */
			else
			{
				push(SKIP);
				skip++;
			}
			break;

		case LINE:						/* [vlh] 4.0 */
			doline();
			return getaline(infile);
			break;

		default:
			error("invalid preprocessor command");
			break;
		}
		eatup();
	} else if (type != NEWL)
	{
		if (skip)
			eatup();
		else
		{
			for (; type != NEWL && type != CEOF; type = gettok(token))
			{
				if (type == ALPHA && (p = lookup(token)) != NULL)
				{
					expand(p);
				} else if (!special(token, infile))	/* [vlh] 4.1 */
				{
					for (cp = token; *cp;)
						putl(*cp++);
				}
			}
		}
	}
	putl('\0');
	return 1;
}
