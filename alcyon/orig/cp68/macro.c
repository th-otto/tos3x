/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"
#include <fcntl.h>

#ifndef DECC
#	include <ctype.h>
#else
#	include "ICTYPE"
#endif

#ifdef REGULUS
const char *stdincl = "/usr/include";			/*standard include directory */
#endif

#ifdef VMS
#	ifdef DECC
		/* if on vax/vms set up to make a logical translation */
#		include descrip
#		define DEFBUFSIZ 	200			/* size of buffer to hold complete entry */
#		define ENDDEF  		-13			/* flag that end of define entry was reached */
char *last_slash;

int rc, rsllen;

char inc_nam[81];

char eqv_nam[81];

$DESCRIPTOR(inc_desc, inc_nam);
$DESCRIPTOR(eqv_desc, eqv_nam);
char defbuf[DEFBUFSIZ];					/* define buffer */

char *defbufp = &defbuf[0];				/* pointer int buffer */

char *defpnew;							/* address of beginning of define entry */

const char *stdincl = "/usr/include/";		/*standard include directory */
#	else
const char *stdincl = "lib:";					/*standard include directory */
#	endif
#endif

#ifdef UNIX
const char *stdincl = "/usr/include/c68";		/*standard include directory */
#endif

#ifdef CPM
const char *stdincl = "";
#endif

#ifdef GEMDOS
const char *stdincl = "";
#endif

#define NUMINCL 10

short clabel = LABSTART;
short nlabel = LABSTART + 1;
char null[] = "";
short nincl;
char *incl[NUMINCL];
char tmp[NUMLEN];						/* temporary spot for line number itoa conversion */
extern char const ctype[];
char lit_file[MAXPSIZE];
short lit_skip;

struct builtin
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

char *getinclude PROTO((const char *fname, const char *parent));
int getaline PROTO((const char *infile));
VOID push PROTO((int val));
int pop PROTO((NOTHING));
VOID dodefine PROTO((NOTHING));
VOID doinclude PROTO((const char *infile));
VOID doifile PROTO((const char *p));
VOID undefine PROTO((const char *name));
VOID doline PROTO((NOTHING));
VOID eatup PROTO((NOTHING));
int special PROTO((const char *token, const char * infile));
int getfarg PROTO((char *token));
VOID trymatch PROTO((const char *token, int type, int nargs, char **args));
int pattern PROTO((const char *pat, const char *token));
int getaarg PROTO((char *argp));
VOID pbnum PROTO((int num));





char cstack[CSTKSIZE];

char *cstkptr;
char inclname[TOKSIZE];

/*
 * domacro - do macro processing
 *      Does the macro pre-processing on the input file and leaves the
 *      result on the output file.
 */
int domacro(P(int) nd)
PP(int nd;)									/* number of defines */
{
	register char *l;
	register struct symbol *sp;
	register int x;

	filep = &filestack[0];				/* [vlh] 4.0 moved for error msgs */
	lineno = 1;							/* [vlh] 4.0 moved for error msgs */
	if (fopen(source, &(filep->inbuf), 0) < 0)
	{									/*sw 3rd arg for versados */
		error("can't open source file %s\n", source);
		return FALSE;
	}
	if (!Eflag)
	{
		if (fcreat(dest, &outbuf, 0) < 0)
		{
			error("can't creat %s\n", dest);
			return FALSE;
		}
	}
	putid(source, 1);					/* identify as first line in source file */

	/* clear out symbol table */
	for (sp = &symtab[0]; ; sp++)
	{
		if (sp > &symtab[HSIZE - 1])
			break;
		sp->s_def = null;
	}
	defp = defap = sbrk(DEFSIZE);
	if (defp == (char *) -1)
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
	for (;;)
	{
		if (--nd < 0)
			break;
		dinstall(defs[nd].ptr, defs[nd].value);
	}
	for (;;)
	{
		if (getaline(source) == FALSE)
			break;
		if (filep == &filestack[0] && pbp == &pbbuf[0])
			lineno++;
		else if (filep != &filestack[0])	/*[vlh] include file */
			(filep)->lineno++;
		if (!skip)
		{								/* [vlh] 4.3, don't put out cr-lf if skipping */
			if (!lit_skip)
			{							/* [vlh] 4.3, endif literalized */
				l = line;
				for (;;)
				{
					if (*l == '\0')
						break;
					doputc(*l++, &outbuf);
				}
#ifdef VERSADOS
				doputc(' ', &outbuf);
#endif
				doputc('\n', &outbuf);
			} else
				lit_skip = 0;
		}
		if (literal)
		{
			if (filep != &filestack[0])
			{
				(filep)->lineno++;
				putid((filep)->ifile, (filep)->lineno);
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
	v6flush(&outbuf);
	if (!Eflag)
		close(fileno(&outbuf));
	close(fileno(&filep->inbuf));
	UNUSED(x);
#ifdef __ALCYON__
	asm("moveq #1,d0");
	asm("nop");
#else
	return TRUE;
#endif
}


/* [vlh] 4.0 SOH line header */
VOID putid(P(const char *) fname, P(int) lnum)
PP(const char *fname;)
PP(int lnum;)
{
	register const char *p;

	if (asflag || pflag)
		return;

	if (literal)
	{
		strcpy(lit_file, fname);
		lit_num = lnum;
	}

	doputc('#', &outbuf);
	doputc(' ', &outbuf);
	itoa(lnum, tmp, NUMLEN - 1);
	for (p = tmp; ;)
	{
		if (*p != ' ')
			break;
		p++;
	}
	for (; ; p++)
	{
		if (*p == '\0')
			break;
		doputc(*p, &outbuf);
	}
	doputc(' ', &outbuf);
	for (p = fname; ; p++)
	{
		if (*p == '\0')
			break;
		doputc(*p, &outbuf);
	}
	doputc('\n', &outbuf);
}


VOID install(P(const char *) name, P(int) def)
PP(const char *name;)
PP(int def;)
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name, sp->s_name);
	sp->s_def = defp;
	putd(def);
	putd('\0');
}


VOID dinstall(P(const char *) name, P(const char *) def)
PP(const char *name;)								/* macro name */
PP(const char *def;)								/* pointer to definition */
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name, sp->s_name);
	sp->s_def = defp;
	putd(NOARGS);
	if (def)							/* [vlh] character strings... */
	{
		for (;;)
		{
			if (*def == '\0')
				break;
			putd(*def++);
		}
	} else
	{
		putd('1');						/* [vlh] default define value */
	}
	putd('\0');
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

	for (bp = &btab[0]; ; bp++)
	{
		if (bp->b_name == NULL)
			break;
		if (strcmp(bp->b_name, name) == 0)
			return bp->b_type;
	}
#ifdef __ALCYON__
	asm("clr.w d0");
	asm("nop");
#else
	return 0;
#endif
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
int getaline(P(const char *) infile)
PP(const char *infile;)
{
	char token[TOKSIZE];
	register int type, i;
	register char *p;

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
			if (getntok(token) == ALPHA && lookup(token))
			{
				push(NOSKIP);
			} else
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
			{
				push(NOSKIP);
			}
			break;

		case ENDIF:
			if ((i = pop()) == SKIP)
			{
				skip--;
				if (!skip)
				{						/* [vlh] 4.3, matches original skip.... */
					if (filep != &filestack[0])
						putid((filep)->ifile, (filep)->lineno);
					else
						putid(source, lineno);
#ifdef __ALCYON__
					lit_skip; /* hmpf? */
#endif
				}						/* [vlh]4.3, identify this line */
			} else if (i != NOSKIP)
			{
				error("invalid #endif");
			}
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
			{
				error("invalid #else");
			}
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
					i = getaline((filep)->ifile);
				else
					i = getaline(infile);
				return i;
			}
			break;

		case IF:
			if (!skip && cexpr())		/*[vlh]4.3, don't do if skipping */
			{
				push(NOSKIP);			/*non-zero, so don't skip */
			} else
			{							/* don't do if skipping or cexpr evaluates zero */
				push(SKIP);
				skip++;
			}
			break;

		case LINE:						/* [vlh] 4.0 */
			if (!skip)
			{							/* [vlh] 4.3, do skip... */
				doline();
				return getaline(infile);
			}
			break;

		default:
			error("invalid preprocessor command");
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
			for (; ; type = gettok(token))
			{
				if (type == NEWL || type == CEOF)
					break;
				if (type == ALPHA && (p = lookup(token)) != NULL)
				{
					expand((struct symbol *) p);
				} else if (!special(token, infile))
				{
					for (p = token; ;)
					{
						if (*p == '\0')
							break;
						ppputl(*p++);
					}
				}
			}
		}
	}
	ppputl('\0');
#ifdef __ALCYON__
	asm("moveq #1,d0");
	asm("nop");
#else
	return TRUE;
#endif
}


/*
 * special - check for predefined macros, if they exist expand them 
 *  __FILE - current file name, __LINE - current line number
 */
int special(P(const char *) token, P(const char *) infile)
PP(const char *token;)
PP(const char *infile;)
{
	register const char *p;
	int xline;
	char buf[8];

	if (strcmp(token, "__FILE") == 0)
	{
		ppputl('"');
		for (p = infile; ;)
		{
			if (*p == '\0')
				break;
			ppputl(*p++);
		}
		ppputl('"');
	} else if (strcmp(token, "__LINE") == 0)
	{
		xline = (literal) ? lit_num : (filep == &filestack[0]) ? lineno : (filep)->lineno;
		itoa(xline, buf, 7);
		buf[7] = 0;
		for (p = &buf[0]; ;)
		{
			if (*p != ' ')
				break;
			p++;
		}
		for (;;)
		{
			if (*p == '\0')
				break;
			ppputl(*p++);
		}
	} else
	{
		return FALSE;
	}
#ifdef __ALCYON__
	asm("moveq #1,d0");
	asm("nop");
#else
	return TRUE;
#endif
}


/*
 * eatup - eat up the rest of the input line until a newline or CEOF
 *      Does gettok calls.
 */
VOID eatup(NOTHING)
{
	register int type;
	char etoken[TOKSIZE];

	for (;;)
	{
		if ((type = gettok(etoken)) == NEWL || type == CEOF)
			break;
	}
}


/*
 * ppputl - put a character to the current output line
 *      Checks for line overflow.
 */
VOID ppputl(P(int) c)
PP(int c;)
{
	if (linep < &line[LINESIZE])
	{
		*linep++ = c;
	} else if (!loverflow)
	{
		loverflow++;
		error("line overflow");
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
 * putd - put a character to the define buffer
 *      Does dynamic allocation for define buffer
 */
VOID putd(P(int) c)
PP(int c;)
{
	if (!defcount)
	{
		if (sbrk(DEFSIZE) == (char *) -1)
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


/*
 * undefine - does undef command
 *      Sets the symbols definition to the null pointer
 */
VOID undefine(P(const char *) name)
PP(const char *name;)
{
	register struct symbol *sp;

	sp = getsp(name);
	if (sp->s_def)
		sp->s_def = null;
}


/*
 * dodefine - do #define processing
 *      Checks the define name, collects formal arguements and saves
 *      macro definition, substituting for formal arguments as it goes.
 */
VOID dodefine(NOTHING)
{
	char token[TOKSIZE];
	char *args[MAXARGS];
	char argbuf[ARGBSIZE];
	register char *abp, *p;
	register int type, nargs, i;
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
		for (; ; nargs++)
		{
			if ((type = getfarg(token)) == RPAREN)
				break;
			if (nargs >= MAXARGS)
			{
				error("too many arguments");
				break;
			}
			args[nargs] = abp;
			for (p = token; ;)
			{
				if ((*abp++ = *p++) == 0)
					break;
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
	for (; ; type = gettok(token))
	{
		if (type == NEWL || type == CEOF)
			break;
		if (type == ALPHA || type == SQUOTE || type == DQUOTE)
		{								/* [vlh] 4.1 */
			trymatch(token, type, nargs, args);	/* [vlh] 4.1 */
			continue;
		} else if (type == BSLASH)
		{
			if ((i = ngetch()) == '\n')
			{							/*multi-line macro? */
				if (filep == &filestack[0])
					lineno++;
				else
					(filep)->lineno++;
				putd('\n');
			}
			putd(i);
			continue;
		}
		for (p = token; ;)
		{
			if (*p == '\0')
				break;
			putd(*p++);
		}
	}
	pbtok(token);
	putd('\0');
}


/* trymatch - check for arguments */
VOID trymatch(P(const char *) token, P(int) type, P(int) nargs, P(char **) args)
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
	for (;;)
	{
		if (*p == 0)
			break;
		for (i = 0; ; i++)
		{
			if (i >= nargs)
				break;
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
			} while (isalnum((__uint8_t)*p) || *p == '_');
		}
		/* get to next possible */
		for (;;)
		{
			if (isalnum((__uint8_t)*p) || *p == '_' || *p == '\0')
				break;
			putd(*p++);
		}
	}
}


/*
 * pattern - if the pattern occurs in the token starting at the first
 *	 position in the string, pattern returns the length of the pattern
 *	 else pattern returns a zero.
 */
int pattern(P(const char *) pat, P(const char *) token)
PP(const char *pat;)
PP(const char *token;)
{
	register int len;

	len = strlen(pat);
	if (len > (int)strlen(token))			/* couldn't possibly work */
		return FALSE;
	if (isalnum((__uint8_t)token[len]) || token[len] == '_')
		return FALSE;						/* not deliminated by non-alphanumeric */

	for (len = 0; ;)
	{
		if (*pat == '\0')
			break;
		if (*pat++ != *token++)
			return 0;
		len++;
	}
#ifdef __ALCYON__
	asm("move.w d7,d0");
	asm("nop");
#else
	return len;
#endif
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
	register char *p, *abp, *mdef;
	register int i, j, nargs, type;

	if (pbflag++ > 100)
	{
		error("define recursion");
		return;
	}
	if (strcmp(sp->s_name, mdef = sp->s_def) == 0)
	{									/*handle #define x x */
		for (;;)
		{
			if (*mdef == '\0')
				break;
			ppputl(*mdef++);
		}
		return;
	}
	nargs = 0;
	if (*mdef == NOARGS)				/*suppress grabbing of args */
	{
		;
	} else if (getntok(token) != LPAREN)	/* [vlh] 4.1 ignore white space */
	{
		pbtok(token);
	} else
	{
		abp = &argbuf[0];
		for (;;)
		{
			if ((type = getaarg(token)) == CEOF)
				break;
			if (nargs >= MAXARGS)
			{
				error("too many arguments");
				return;
			}
			args[nargs++] = abp;
			for (p = token; ; )
			{
				if ((*abp++ = *p++) == 0)
					break;
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
		for (p = mdef + (int)strlen(mdef) - 1; ; p--)
		{
			if (p < mdef)
				break;
			if (*p == ARG)
			{
				j = *--p;
				if (nargs >= j)
					pbtok(args[j - 1]);
			} else
			{
				putback(*p);
			}
		}
	}
}


/*
 * getfarg - get macro formal parameters
 *      Skips blanks and handles "," and ")".
 * returns token type
 */
int getfarg(P(char *) token)
PP(char *token;)
{
	register int type;

	if ((type = getntok(token)) == RPAREN || type == ALPHA)
		return type;
	if (type != COMMA || (type = getntok(token)) != ALPHA)
		error("bad argument:%s", token);
#ifdef __ALCYON__
	asm("move.w d7,d0");
	asm("nop");
#else
	return type;
#endif
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

	for (;;)
	{
		if ((type = gettok(token)) != WHITE)
			break;
	}
#ifdef __ALCYON__
	asm("move.w d7,d0");
	asm("nop");
#else
	return type;
#endif
}


/*
 * getaarg - get macro actual argument
 *      This handles the collecting of the macro's call arguments.
 *      Note that you may have parenthesis as part of the macro argument,
 *      hence you need to keep track of them.
 */
int getaarg(P(char *) argp)
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
	for (;;)
	{
		if (((type = gettok(token)) == COMMA || type == RPAREN) && plevel == 0)
			break;
		if (type == NEWL)				/* [vlh] 4.3 multi line macro expansion */
		{
			ppputl('\n');
		} else
		{
			for (p = token; ; ap++)
			{
				if ((*ap = *p++) == 0)
					break;
				if (--i <= 0)
				{
					error("macro argument too long");
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
			error("unexpected EOF");
			cexit();
		}
	}
	if (ap == argp)
		type = CEOF;
#ifdef __ALCYON__
	asm("move.w -2(a6),d0");
	asm("nop");
#else
	return type;
#endif
}


/*
 * push - push a #ifdef condition value on condition stack
 *      Checks for stack overflow.
 */
VOID push(P(int) val)
PP(int val;)
{
	if (cstkptr >= &cstack[CSTKSIZE])
	{
		error("condition stack overflow");
		cexit();
	}
	*cstkptr++ = val;
}


/*
 * pop - pop the #ifdef, etc. condition stack
 *      Checks for stack undeflow.
 * returns - top of condition stack
 */
int pop(NOTHING)
{
	if (cstkptr <= &cstack[0])
		return -1;
	--cstkptr;
#ifdef __ALCYON__
	asm("movea.l _cstkptr,a0");
	asm("move.b (a0),d0");
	asm("ext.w     d0");
	asm("nop");
#else
	return *cstkptr;
#endif
}


/*
 * doinclude - handle #include command
 *      Checks for file name or library file name and pushes file on
 *      include file stack.
 */
VOID doinclude(P(const char *) infile)
PP(const char *infile;)
{
	register int type;
	register int fd, i;
	char token[TOKSIZE];
	char fname[TOKSIZE];
	register char *p, *q, c;
	register char *ptr1, *ptr2;

	p = fname;
	if ((type = getntok(token)) == SQUOTE || type == DQUOTE)
	{
		for (c = token[0], q = &token[1]; ;)
		{
			if (*q == c)
				break;
			*p++ = *q++;
		}
		*p = '\0';
		p = getinclude(fname, infile);
	} else if (type != LESS)
	{
		error("bad include file");
		return;
	} else
	{
		for (;;)
		{
			if ((type = gettok(token)) == GREAT || type == NEWL || type == CEOF)
				break;
			for (q = token; ; p++)
			{
				if ((*p = *q++) == 0)
					break;
			}
		}
		if (type != GREAT)
		{
			error("bad include file name");
			pbtok(token);
			return;
		}
		p = getinclude(fname, (char *) 0L);
	}
	eatup();							/*need here... */
	filep++;							/*sw Increment BEFORE */
	if (filep >= &filestack[FSTACK])
	{
		error("includes nested too deeply");
	} else
	{
/*sw    fd = inbuf.fd; */
#ifdef NONEST
		filep->tcc = inbuf.cc;
		filep->tcp = inbuf.cp;
		ptr1 = &filep->tbuf[0];
		ptr2 = &inbuf.cbuf[0];
		for (i = 0; i < BSIZE; i++)
			*ptr1++ = *ptr2++;
#else
		/*sw    seek(fd,-inbuf.cc,1);  *//*back up file ptr */
#endif
/*      inbuf.cc = 0; */
		if (fopen(p, &(filep->inbuf), 0) < 0)
		{								/* 3rd arg for versados */
			if (type != SQUOTE && type != DQUOTE)
				error("can't open include file %s", p);
			else
				error("can't open include file %s", fname);
#ifdef NONEST
			inbuf.cc = filep->tcc;
#endif
			filep--;					/*sw Undo the damage        */
		} else
		{
/*sw        filep->ifd = fd;	*/
			filep->lineno = 1;			/* [vlh] */
			putid(p, 1);				/* id for include file */
			doifile(p);
/*sw        filep++;*/
		}
	}
	UNUSED(ptr1);
	UNUSED(ptr2);
	UNUSED(i);
	UNUSED(fd);
}


VOID doifile(P(const char *) p)
PP(const char *p;)
{
	register char *iptr;
	register int ndx;

	for (iptr = filep->ifile; ;)
	{
		if (*p == '\0')
			break;
		*iptr++ = *p++;
	}
	*iptr = 0;
	UNUSED(ndx);
}


/* getinclude - get include file full pathname */
char *getinclude(P(const char *) fname, P(const char *) parent)
PP(const char *fname;)
PP(const char *parent;)
{
	register const char *q;
	register char *t;
	register int i, fd, ndx;

	for (i = 0; ; i++)
	{
		if (i >= nincl)
			break;
		t = inclname;
		for (q = incl[i]; ;)
		{
			if ((*t++ = *q++) == 0)
				break;
		}
		*(t - 1) = FILESEP;
		for (q = fname; ;)
		{
			if ((*t++ = *q++) == 0)
				break;
		}
		*t = 0;
#ifdef DECC
		v11c_i1();						/* translation routine */
#endif
		/* BUG: open redirected here to xfopen, which needs a third argument */
#ifdef __ALCYON__
		fd = open(inclname, O_RDONLY);
#else
		fd = open(inclname, O_RDONLY, FALSE);
#endif
		if (fd >= 0)
		{
			close(fd);
			return &inclname[0];
		}
	}
	if (parent)
	{									/* include filename surrounded by quotes */
		q = (filep == &filestack[0]) ? parent : (filep)->ifile;
		t = &inclname[0];
		for (;;)
		{
			ndx = strindex(q, FILESEP);
			if (ndx < 0)
				break;
			ndx++;
			for (;;)
			{
				if (ndx-- == 0)
					break;
				*t++ = *q++;
			}
		}
		for (q = fname; ; )
		{
			if ((*t++ = *q++) == '\0')
				break;
		}
		*t = 0;
		/* BUG: open redirected here to xfopen, which needs a third argument */
#ifdef __ALCYON__
		fd = open(inclname, O_RDONLY);
#else
		fd = open(inclname, O_RDONLY, FALSE);
#endif
		if (fd >= 0)
		{								/* found it */
			close(fd);
			return &inclname[0];
		}
	}
#ifndef DECC
	for (t = inclname, q = stdincl; ; )
	{
		if ((*t++ = *q++) == '\0')
			break;
	}
#ifdef	UNIX							/*sw You can't have tested this.... */
	*(t - 1) = FILESEP;
#endif
#ifdef	REGULUS
	*(t - 1) = FILESEP;
#endif
#ifdef	WHITESM
	t--;
#endif
#ifdef CPM
	t--;
#endif
#ifdef GEMDOS
	t--;
#endif
	for (q = fname; ;)
	{
		if ((*t++ = *q++) == '\0')
			break;
	}
	*t = 0;
#else
	v11c_i3(fname);
#endif
#ifdef __ALCYON__
	asm("move.l #_inclname,d0");
	asm("nop");
#else
	return inclname;
#endif
}


#define SKIPWHITE(ch)     do { ch = ngetch(); } while (ctype[ch] == WHITE)
VOID doline(NOTHING)
{
	register char *ptr;
	char token[TOKSIZE];
	register int ch, lnum, type;

	/* get line number associated with #LINE */
	/* skip white space */
	for (;;)
	{
		if ((type = gettok(token)) != WHITE)
			break;
	}
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
	} else if (filep == &filestack[0])	/* source or header file */
	{
		putid(source, lnum);
	} else
	{
		putid((filep)->ifile, lnum);
	}
	if (ch != NEWL)
	{
		for (; ;)
		{
			if (ctype[ch] == NEWL || ctype[ch] == CEOF)
				break;
			ch = ngetch();
		}
	}
}


VOID pbnum(P(int) num)
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
