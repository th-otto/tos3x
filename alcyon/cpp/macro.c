/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"
#include <ctype.h>
#include <unistd.h>

int literal;
int lit_num;
char lit_file[MAXPSIZE];
int nincl;
char *incl[NINCL];

char line[LINESIZE+2];				/* line buffer */
char *linep;						/* current line pointer */
int loverflow;						/* line overflow flag */

struct defstruc defs[NDEFS];

static int skip;					/* skipping current line */
static char *defap;					/* pointer start of define area */
static int defcount;				/* bytes left in define area */
static long defused;				/* number of bytes used in define area */
static long defmax;					/* maximum define area used */

static int clabel = LABSTART;
static int nlabel = LABSTART + 1;


static struct builtin
{
	const char *b_name;
	int b_type;
} const btab[] =
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


/*
 * putd - put a character to the define buffer
 *      Does dynamic allocation for define buffer
 */
static VOID putd(P(int) c)
PP(int c;)
{
	char *mdef;
	
	if (!defcount)
	{
		if ((defap = lrealloc(defap, defused + DEFSIZE)) == NULL)
		{
			error(_("define table overflow"));
			cexit();
		}
		defcount = DEFSIZE;
	}
	defcount--;
	mdef = defap + defused++;
	*mdef = c;
}


static VOID dinstall(P(const char *) name, P(const char *) def)
PP(const char *name;)								/* macro name */
PP(const char *def;)								/* pointer to definition */
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name, sp->s_name);
	sp->s_def = defused;
	putd(NOARGS);
	if (def)
	{
		while (*def)
			putd(*def++);
	} else
	{
		putd('1');						/* default define value */
	}
	putd('\0');
}


static VOID install(P(const char *) name, P(int) def)
PP(const char *name;)
PP(int def;)
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name, sp->s_name);
	sp->s_def = defused;
	putd(def);
	putd('\0');
}


/*
 * special - check for predefined macros, if they exist expand them 
 *  __FILE - current file name, __LINE - current line number
 */
static int special(P(const char *) token, P(const char *) infile)
PP(const char *token;)
PP(const char *infile;)
{
	register const char *p;
	int xline;
#define NUMLEN  20
	char buf[NUMLEN];

	if (strcmp(token, "__FILE") == 0 || strcmp(token, "__FILE__") == 0)
	{
		ppputl('"');
		for (p = infile; *p;)
			ppputl(*p++);
		ppputl('"');
	} else if (strcmp(token, "__LINE") == 0 || strcmp(token, "__LINE__") == 0)
	{
		xline = literal ? lit_num : (filep == &filestack[0]) ? lineno : (filep - 1)->lineno;
		itoa(xline, buf, 0);
		p = buf;
		while (*p)
			ppputl(*p++);
	} else
	{
		return FALSE;
	}
	return TRUE;
}


/*
 * eatup - eat up the rest of the input line until a newline or CEOF
 *      Does gettok calls.
 */
static VOID eatup(NOTHING)
{
	register int type;
	char etoken[TOKSIZE];

	while ((type = gettok(etoken)) != NEWL && type != CEOF)
		;
}


/*
 * undefine - does undef command
 *      Sets the symbols definition to the null pointer
 */
static VOID undefine(P(const char *) name)
PP(const char *name;)
{
	register struct symbol *sp;

	sp = getsp(name);
	if (sp->s_def > 0)
		sp->s_def = 0;
}


/*
 * pattern - if the pattern occurs in the token starting at the first
 *	 position in the string, pattern returns the length of the pattern
 *	 else pattern returns a zero.
 */
static int pattern(P(const char *) pat, P(const char *) token)
PP(const char *pat;)
PP(const char *token;)
{
	register int len;

	len = strlen(pat);
	if (len > (int)strlen(token))			/* couldn't possibly work */
		return FALSE;
	if (isalnum(token[len]) || token[len] == '_')
		return FALSE;						/* not deliminated by non-alphanumeric */

	for (len = 0; *pat;)
	{
		if (*pat++ != *token++)
			return 0;
		len++;
	}
	return len;
}


/* trymatch - check for arguments */
static VOID trymatch(P(const char *) token, P(int) type, P(int) nargs, P(char **) args)
PP(const char *token;)
PP(int type;)
PP(int nargs;)
PP(char **args;)
{
	register const char *p;
	register int i, len;

	p = token;
	if (type != ALPHA)
		putd(*p++);
	while (*p != 0)
	{
		for (i = 0; i < nargs; i++)
		{
			if ((len = pattern(args[i], p)) != 0)
				break;
		}
		if (i < nargs)
		{								/* sub ARG marker for formal arg */
			putd(i + 1);
			putd(ARG);
			p += len;
		} else
		{
			do
			{
				putd(*p++);
			} while (isalnum(*p) || *p == '_');
		}
		while (!isalnum(*p) && *p != '_' && *p)	/* get to next possible */
			putd(*p++);
	}
}


/*
 * getfarg - get macro formal parameters
 *      Skips blanks and handles "," and ")".
 * returns token type
 */
static int getfarg(P(char *) token)
PP(char *token;)
{
	register int type;

	if ((type = getntok(token)) == RPAREN || type == ALPHA)
		return type;
	if (type != COMMA || (type = getntok(token)) != ALPHA)
		error(_("bad argument:%s"), token);
	return type;
}


/*
 * dodefine - do #define processing
 *      Checks the define name, collects formal arguements and saves
 *      macro definition, substituting for formal arguments as it goes.
 */
static VOID dodefine(NOTHING)
{
	char token[TOKSIZE];
	char *args[MAXARGS];
	char argbuf[ARGBSIZE];
	register char *abp, *p;
	register int type, nargs, i;
	register struct symbol *sp;

	if ((type = getntok(token)) != ALPHA)
	{
		error(_("bad define name: %s"), token);
		return;
	}
	sp = getsp(token);
	symcopy(token, sp->s_name);
	sp->s_def = defused;
	nargs = 0;
	abp = argbuf;
	if ((type = gettok(token)) == LPAREN)
	{
		for (; (type = getfarg(token)) != RPAREN; nargs++)
		{
			if (nargs >= MAXARGS)
			{
				error(_("too many arguments"));
				break;
			}
			args[nargs] = abp;
			for (p = token; (*abp++ = *p++) != 0;)
			{
				if (abp >= &argbuf[ARGBSIZE])
				{
					error(_("argument buffer overflow"));
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
	type = getntok(token);				/* get next non-white token */
	for (; type != NEWL && type != CEOF; type = gettok(token))
	{
		if (type == ALPHA || type == SQUOTE || type == DQUOTE)
		{
			trymatch(token, type, nargs, args);
			continue;
		} else if (type == BSLASH)
		{
			if ((i = ngetch()) == '\n')
			{							/* multi-line macro? */
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


/*
 * push - push a #ifdef condition value on condition stack
 *      Checks for stack overflow.
 */
static VOID push(P(int) val)
PP(int val;)
{
	if (cstkptr >= &cstack[CSTKSIZE])
	{
		error(_("condition stack overflow"));
		cexit();
	}
	*cstkptr++ = val;
}


/* SOH line header */
VOID putid(P(const char *) fname, P(int) lnum)
PP(const char *fname;)
PP(int lnum;)
{
	register const char *p;
	char tmp[NUMLEN];						/* temporary spot for line number itoa conversion */

	if (asflag || pflag)
		return;

	if (literal)
	{
		strcpy(lit_file, fname);
		lit_num = lnum;
	}

	fputc('#', outbuf);
	fputc(' ', outbuf);
	itoa(lnum, tmp, 0);
	for (p = tmp; *p; p++)
		fputc(*p == '\\' ? '/' : *p, outbuf);
	fputc(' ', outbuf);
	fputc('"', outbuf);
	for (p = fname; *p; p++)
		fputc(*p, outbuf);
	fputc('"', outbuf);
	fputc('\n', outbuf);
}


/*
 * pop - pop the #ifdef, etc. condition stack
 *      Checks for stack underflow.
 * returns - top of condition stack
 */
static int pop(NOTHING)
{
	if (cstkptr <= &cstack[0])
		return -1;
	return *--cstkptr;
}


static VOID doifile(P(const char *) p)
PP(const char *p;)
{
	register char *iptr;

	for (iptr = filep->ifile; *p;)
		*iptr++ = *p++;
	*iptr = 0;
}


/* getinclude - get include file full pathname */
static char *getinclude(P(const char *) fname, P(const char *) parent)
PP(const char *fname;)
PP(const char *parent;)
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
		t--;
		if (t > inclname && t[-1] != FILESEP && t[-1] != FILESEP2)
			*t++ = FILESEP;
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


/*
 * doinclude - handle #include command
 *      Checks for file name or library file name and pushes file on
 *      include file stack.
 */
static VOID doinclude(P(const char *) infile)
PP(const char *infile;)
{
	register int type;
	char token[TOKSIZE];
	char fname[TOKSIZE];
	register char *p, *q, c;
	FILE *ifd;
	
	p = fname;
	if ((type = getntok(token)) == SQUOTE || type == DQUOTE)
	{
		for (c = token[0], q = &token[1]; *q != c;)
			*p++ = *q++;
		*p = '\0';
		p = getinclude(fname, infile);
	} else if (type != LESS)
	{
		error(_("bad include file"));
		return;
	} else
	{
		while ((type = gettok(token)) != GREAT && type != NEWL && type != CEOF)
		{
			for (q = token; (*p = *q++) != 0; p++)
				;
		}
		if (type != GREAT)
		{
			error(_("bad include file name"));
			pbtok(token);
			return;
		}
		p = getinclude(fname, (char *) 0L);
	}
	eatup();							/* need here... */
	if (filep >= &filestack[FSTACK])
	{
		error(_("includes nested too deeply"));
	} else
	{
		if ((ifd = fopen(p, "r")) == NULL)
		{
			if (type != SQUOTE && type != DQUOTE)
				error(_("can't open include file %s"), p);
			else
				error(_("can't open include file %s"), fname);
		} else
		{
			filep->ifd = inbuf;
			filep->lineno = 1;
			putid(p, 1);				/* id for include file */
			doifile(p);
			filep++;
			inbuf = ifd;
		}
	}
}


#define SKIPWHITE(ch)     do { ch = ngetch(); } while (ctype[ch] == WHITE)
static VOID doline(NOTHING)
{
	register char *ptr;
	char token[TOKSIZE];
	register int ch, lnum, type;

	/* get line number associated with #LINE */
	/* skip white space */
	while ((type = gettok(token)) == WHITE)
		;
	if (type != DIGIT)
	{
		error(_("invalid #line args"));
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
	} else if (filep == &filestack[0])	/* source or header file */
	{
		putid(source, lnum);
	} else
	{
		putid((filep - 1)->ifile, lnum);
	}
	if (ch != NEWL)
	{
		for (; ctype[ch] != NEWL && ctype[ch] != CEOF;)
			ch = ngetch();
	}
}


/*
 * getaarg - get macro actual argument
 *      This handles the collecting of the macro's call arguments.
 *      Note that you may have parenthesis as part of the macro argument,
 *      hence you need to keep track of them.
 */
static int getaarg(P(char *) argp)
PP(char *argp;)
{
	int type, plevel, i;
	register char *p;
	register char *ap;
	char token[TOKSIZE];

	ap = argp;
	*ap = '\0';
	plevel = 0;
	i = TOKSIZE;
	while (((type = gettok(token)) != COMMA && type != RPAREN) || plevel != 0)
	{
		if (type == NEWL)				/* multi line macro expansion */
		{
			ppputl('\n');
		} else
		{
			for (p = token; (*ap = *p++) != 0; ap++)
			{
				if (--i <= 0)
				{
					error(_("macro argument too long"));
					return CEOF;
				}
			}
		}
		if (type == LPAREN)
		{
			plevel++;
		} else if (type == RPAREN)
		{
			plevel--;
		} else if (type == CEOF)
		{
			error(_("unexpected EOF"));
			cexit();
		}
	}
	if (ap == argp)
		type = CEOF;
	return type;
}


static VOID pbnum(P(int) num)
PP(int num;)
{
	register int digit;

	do
	{
		digit = num % 10;
		num /= 10;
		putback(digit + '0');
	} while (num > 0);
}


/*
 * getaline - get input line handling macro statements
 *  Checks for a preprocessor statement on the line and if there
 *  is one there, it processes it.  Note that most of the work is
 *  in determining whether we need to skip the current line or not.
 *  This is all handled with the condition stack and the skip variable.
 *  The skip variable is non-zero if any condition on the condition
 *  stack is SKIP.
 * returns 0 for CEOF, 1 otherwise
 */
static int getaline(P(const char *) infile)
PP(const char *infile;)
{
	char token[TOKSIZE];
	register int type, i;
	register struct symbol *p;
	register const char *cp;
	
	initl();
	if ((type = gettok(token)) == CEOF)
		return FALSE;
	if (type == POUND)
	{
		if ((type = getntok(token)) == NEWL)
			return TRUE;
		switch (kwlook(token))
		{
		case IFDEF:
			if (!skip && getntok(token) == ALPHA && lookup(token))
			{
				push(NOSKIP);
			} else
			{
				push(SKIP);
				skip++;
			}
			break;

		case IFNDEF:
			if (skip || (getntok(token) == ALPHA && lookup(token)))
			{
				push(SKIP);
				skip++;
			} else
			{
				push(NOSKIP);
			}
			break;

		case ENDIF:
			if ((i = pop()) == SKIP)
			{
				skip--;
			} else if (i != NOSKIP)
			{
				error(_("invalid #endif"));
			}
			break;

		case ELSE:
			/* BUG: does not detect else after else */
			if ((i = pop()) == SKIP)
			{
				skip--;
				push(NOSKIP);
			} else if (i == NOSKIP)
			{
				skip++;
				push(SKIP);
			} else
			{
				error(_("invalid #else"));
			}
			break;

		case DEFINE:
			if (!skip)					/* if in skip, don't do define */
				dodefine();
			break;

		case UNDEF:
			if (!skip)
			{							/* if in skip, don't undef */
				if ((type = getntok(token)) == ALPHA)
					undefine(token);
			}
			break;

		case INCLUDE:
			if (!skip)
			{							/* if in skip, don't do include */
				doinclude(infile);
				if (filep != &filestack[0])
					i = getaline((filep - 1)->ifile);
				else
					i = getaline(infile);
				return i;
			}
			break;

		case IF:
			if (!skip && cexpr())		/* evaluate constant expression */
			{
				push(NOSKIP);			/* non-zero, so don't skip */
			} else
			{							/* don't do if skipping or cexpr evaluates zero */
				push(SKIP);
				skip++;
			}
			break;

		case LINE:
			if (!skip)
			{
				doline();
				return getaline(infile);
			}
			break;

		default:
			error(_("invalid preprocessor command"));
			break;
		}
		eatup();
	} else if (type != NEWL)
	{
		if (skip)
		{
			eatup();
		} else
		{
			for (; type != NEWL && type != CEOF; type = gettok(token))
			{
				if (type == ALPHA && (p = lookup(token)) != NULL)
				{
					expand(p);
				} else if (!special(token, infile))
				{
					for (cp = token; *cp;)
						ppputl(*cp++);
				}
			}
		}
	}
	ppputl('\0');
	return TRUE;
}


/*
 * domacro - do macro processing
 *      Does the macro pre-processing on the input file and leaves the
 *      result on the output file.
 */
int domacro(P(int) nd)
PP(int nd;)
{
	register char *l;
	register struct symbol *sp;

	filep = &filestack[0];
	if ((inbuf = fopen(source, "r")) == NULL)
	{
		error(_("can't open source file %s\n"), source);
		return FALSE;
	}
	if (!Eflag)
	{
		if ((outbuf = fopen(dest, "wb")) == NULL)
		{
			error(_("can't creat %s\n"), dest);
			return FALSE;
		}
	}
	lineno = 1;
	putid(source, 1);					/* identify as first line in source file */

	/* clear out symbol table */
	for (sp = &symtab[0]; sp <= &symtab[HSIZE - 1]; sp++)
		sp->s_def = -1;
	defap = lmalloc((long)DEFSIZE);
	if (defap == NULL)
	{
		error(_("define table overflow"));
		cexit();
	}
	defcount = DEFSIZE;
	defmax = 0;
	defused = 0;
	putd('\0');

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
		else if (filep != &filestack[0])
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
		error(_("unmatched conditional"));
	if (defused > defmax)
		defmax = defused;
	fflush(outbuf);
	if (!Eflag)
		fclose(outbuf);
	fclose(inbuf);
	return 1;
}


/*
 * kwlook - look up the macro built-in names
 *      Searches thru the built-in table for the name.
 * returns keyword index or 0
 */
int kwlook(P(const char *) name)
PP(const char *name;)
{
	register const struct builtin *bp;

	for (bp = &btab[0]; bp->b_name != NULL; bp++)
		if (strcmp(bp->b_name, name) == 0)
			return bp->b_type;
	return 0;
}


/* ppputl - put a character to the current output line */
/*      Checks for line overflow.*/
VOID ppputl(P(int) c)
PP(int c;)
{
	if (linep < &line[LINESIZE])
	{
		*linep++ = c;
	} else if (!loverflow)
	{
		loverflow++;
		error(_("line overflow"));
	}
}


/*
 * initl - initialize current line
 *      Sets the line pointer and the line overflow flag.
 */
VOID initl(NOTHING)
{
	*(linep = &line[0]) = '\0';
	loverflow = 0;
}


/*
 * expand - expands the macro definition
 *  Checks for define recursion and #define x x problems, collects
 *  the actual arguments using getaarg, and then expands the macro
 *  by pushing it onto the push back buffer, substituting arguments
 *  as it goes.
 */
VOID expand(P(struct symbol *) sp)
PP(struct symbol *sp;)
{
	char argbuf[ARGBSIZE];
	char *args[MAXARGS];
	char token[TOKSIZE];
	register const char *p;
	register char *abp;
	register const char *mdef;
	register int i, j, nargs, type;

	if (pbflag++ > 100)
	{
		error(_("define recursion"));
		return;
	}
	mdef = defap + sp->s_def;
	if (strcmp(sp->s_name, mdef) == 0)
	{									/* handle #define x x */
		while (*mdef)
			ppputl(*mdef++);
		return;
	}
	nargs = 0;
	if (*mdef == NOARGS)				/* suppress grabbing of args */
	{
		;
	} else if (getntok(token) != LPAREN)	/* ignore white space */
	{
		pbtok(token);
	} else
	{
		abp = &argbuf[0];
		while ((type = getaarg(token)) != CEOF)
		{
			if (nargs >= MAXARGS)
			{
				error(_("too many arguments"));
				return;
			}
			args[nargs++] = abp;
			for (p = token; (*abp++ = *p++) != 0; )
			{
				if (abp >= &argbuf[ARGBSIZE])
				{
					error(_("argument buffer overflow"));
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
		mdef++;							/* skip no. of args */
		for (p = mdef + strlen(mdef) - 1; p >= mdef; p--)
		{
			if (*p == ARG)
			{
				if ((j = *--p) <= nargs)
					pbtok(args[j - 1]);
			} else
			{
				putback(*p);
			}
		}
	}
}


/*
 * getntok - get next token, suppressing white space
 *      Merely gettok's until non-white space is there
 * returns token type
 */
int getntok(P(char *) token)
PP(char *token;)
{
	register int type;

	while ((type = gettok(token)) == WHITE)
		;
	return type;
}
