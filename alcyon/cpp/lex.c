/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"

char const ctype[] = {
	CEOF, ANYC, ANYC, ANYC, ANYC, ANYC, ANYC, ANYC,
	ANYC, WHITE, NEWL, ANYC, ANYC, ANYC, ANYC, ANYC,
	ANYC, ANYC, ANYC, ANYC, NEWL, ANYC, ANYC, ANYC,
	ANYC, ANYC, ANYC, ANYC, ANYC, ANYC, ANYC, ANYC,
	WHITE, NOT, DQUOTE, POUND, ANYC, MOD, AND, SQUOTE,
	LPAREN, RPAREN, MULT, ADD, COMMA, SUB, ANYC, DIV,
	DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT,
	DIGIT, DIGIT, COLON, ANYC, LESS, EQUAL, GREAT, QMARK,
	ANYC, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
	ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
	ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
	ALPHA, ALPHA, ALPHA, ANYC, BSLASH, ANYC, XOR, ALPHA,
	ANYC, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
	ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
	ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
	ALPHA, ALPHA, ALPHA, ANYC, OR, ANYC, COMPL, ANYC
};

struct stackstruc *filep;
struct stackstruc filestack[FSTACK];
int lineno;
char pbbuf[PBSIZE];					/* push back buffer */
char *pbp;							/* push back pointer */
int pbflag;							/* checks for recursive definition */
struct symbol symtab[HSIZE];


/*
 * symhash - compute hash value for symbol
 *      Sums the symbols characters and takes that modulus the hash table
 *      size.
 * returns hash value for symbol
 */
static int symhash(P(const char *) sym)
PP(const char *sym;)
{
	register const char *p;
	register int hashval, i;

	for (p = sym, i = SSIZE, hashval = 0; *p != '\0' && i > 0; i--)
		hashval += *p++;
	return hashval % (HSIZE - 2);
}


/*
 * symequal - check for symbol equality
 *      Does comparison between two symbols.
 * returns 1 if equal, 0 otherwise
 */
static int symequal(P(const char *) sym1, P(const char *) sym2)
PP(const char *sym1;)
PP(const char *sym2;)
{
	register const char *p;
	register const char *q;
	register int i;

	q = sym2;
	i = SSIZE;
	for (p = sym1; *p == *q++;)
	{
		if (*p++ == '\0' || --i == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}


/*
 * symcopy - symbol copy
 *      Copies one symbol to another.
 */
VOID symcopy(P(const char *) sym1, P(char *) sym2)
PP(const char *sym1;)
PP(char *sym2;)
{
	register const char *p;
	register char *q;
	register int i;

	for (p = sym1, q = sym2, i = SSIZE; --i >= 0;)
	{
		if (*p)
			*q++ = *p++;
		else
			*q++ = '\0';
	}
	*q = '\0';
}


/*
 * error - output error message
 *      Outputs line number and error message and keeps track of errors.
 */
#ifdef __USE_VARARGS
VOID error(P(const char *) s, va_alist)
PP(const char *s;)
va_dcl
#else
VOID error(const char *s, ...)
#endif
{
	va_list args;
	
	if (literal)
		fprintf(stderr, "%s, # line %d: ", lit_file, lit_num);
	else if (filep == &filestack[0])	/* not in include */
		fprintf(stderr, "%s, # line %d: ", source, lineno);
	else
		fprintf(stderr, "%s, # line %d: ", (filep - 1)->ifile, (filep - 1)->lineno);
	va_start(args, s);
	vfprintf(stderr, s, args);
	fputc('\n', stderr);
	va_end(args);
	status++;
}


/*
 * putback - puts back a single character
 *      Checks for push back buffer overflow.
 */
VOID putback(P(int) c)
PP(int c;)
{
	if (pbp >= &pbbuf[PBSIZE])
	{
		error("too many characters pushed back");
		cexit();
	}
	*pbp++ = c;
}


/*
 * pbtok - push back a token
 *      Reverses token as its pushing it back.
 */
VOID pbtok(P(const char *) s)
PP(const char *s;)
{
	register const char *p;

	for (p = s + strlen(s); p > s;)
		putback(*--p);
}


/*
 * ngetch - get a (possibly) pushed back character
 *      This handles the include file stack and incrementing the line
 *      number for the lowest level file.
 * returns character or CEOF
 */
int ngetch(NOTHING)
{
	register int c;

	if (pbp > &pbbuf[0])
		return *--pbp;
	pbflag = 0;
	while ((c = getc(inbuf)) < 0)
	{
		if (filep == &filestack[0])
			return CEOF;
		fclose(inbuf);
		filep--;
		inbuf = filep->ifd;
		if (filep == &filestack[0])
		{								/* need line for #include... */
			lineno++;
			putid(source, lineno);		/* id line .... */
		} else
		{
			(filep - 1)->lineno++;
			putid((filep - 1)->ifile, (filep - 1)->lineno);
		}
	}
	return c;
}


/*
 * getsp - get symbol pointer
 *      Calculates the symbol table pointer for a given symbol, if symbol
 *      is not defined, will point to appropriate place to insert symbol.
 */
struct symbol *getsp(P(const char *) name)
PP(const char *name;)
{
	register int wrap;
	register struct symbol *sp;
	register struct symbol *asp;

	wrap = 0;
	asp = 0;
	for (sp = &symtab[symhash(name)]; sp->s_def != null;)
	{
		if (symequal(sp->s_name, name))
			return sp;
		if (!asp && sp->s_def == null)
			asp = sp;
		if (++sp >= &symtab[HSIZE])
		{
			if (wrap++)
			{
				error("symbol table overflow");
				cexit();
			}
			sp = &symtab[0];
		}
	}
	return asp ? asp : sp;
}


/*
 * lookup - looks up a symbol to see if it is defined
 *		Returns pointer to definition if found.
 */
struct symbol *lookup(P(const char *) name)
PP(const char *name;)
{
	register struct symbol *sp;

	sp = getsp(name);
	if (sp->s_def == 0 || sp->s_def == null)
		return NULL;
	return sp;
}


/*
 * peekis - peeks at next character for specific character
 *      Gets next (possibly pushed back) character, if it matches
 *      the given character 1 is returned, otherwise the character
 *      is put back.
 */
static int peekis(P(int) tc)
PP(int tc;)
{
	register int c;

	if ((c = ngetch()) == tc)
		return TRUE;
	putback(c);
	return FALSE;
}


/*
 * gettok - gets next token from input
 *      Collects character string in token and handles special tokens for
 *      the expression evaluator.
 */
int gettok(P(char *) token)
PP(char *token;)
{
	register char *p;
	register int c;
	register char *s;
	register int type;
	register int t;
	register int l;

	p = token;
	c = ngetch();
	*p++ = c;
	switch (type = ctype[c])
	{
	case SQUOTE:
	case DQUOTE:
		if (getstr(token, TOKSIZE, c))
			return type;
		else
		{								/* ignore incomplete strings... could be asm comment */
			token[0] = '\n';
			return NEWL;
		}

	case DIGIT:
	case ALPHA:
		for (; p < &token[TOKSIZE]; p++)
		{
			c = ngetch();
			*p = c;
			if ((t = ctype[c]) != ALPHA && t != DIGIT)
				break;
		}
		putback(*p);
		break;

	case NOT:
		if (peekis('='))
		{
			type = NEQUAL;
			*p++ = '=';
		}
		break;

	case GREAT:
		if (peekis('>'))
		{
			type = RSHIFT;
			*p++ = '>';
		} else if (peekis('='))
		{
			type = GREQUAL;
			*p++ = '=';
		}
		break;

	case LESS:
		if (peekis('<'))
		{
			type = LSHIFT;
			*p++ = '<';
		} else if (peekis('='))
		{
			type = LSEQUAL;
			*p++ = '=';
		}
		break;

	case EQUAL:
		if (peekis('='))
			*p++ = '=';
		else
			type = ANYC;
		break;

	case DIV:
		if (peekis('*'))
		{
			if (Cflag)
			{
				ppputl('/');
				ppputl('*');
			}
			l = lineno;
			while ((c = ngetch()) != CEOF)
			{
				if (c == '\n')
				{
					if (filep == &filestack[0] && pbp == &pbbuf[0])
						lineno++;
					if (Cflag)
					{
						ppputl('\0');
						s = line;
						while (*s)
							fputc(*s++, outbuf);
						initl();
					}
					fputc(' ', outbuf);
					fputc('\n', outbuf);
				} else if (c == '*' && peekis('/'))
				{
					if (Cflag)
					{
						ppputl('*');
						ppputl('/');
					}
					break;
				} else if (Cflag)
				{
					ppputl(c);
				}
			}
			if (c == CEOF)
			{
				lineno = l;
				error("no */ before EOF");
			}
			type = WHITE;
			token[0] = ' ';
		} else if (peekis('/'))
		{
			if (Cflag)
			{
				ppputl('/');
				ppputl('/');
			}
			while ((c = ngetch()) != CEOF && c != '\n')
			{
				if (Cflag)
					ppputl(c);
			}
			type = NEWL;
			token[0] = '\n';
		}
		break;

	case BADC:
		error("bad character 0%o", c);
		break;

	}
	*p = '\0';
	return type;
}


/*
 * getstr - get a quoted (single or double) character string
 *      Gets specified number of characters, handling escapes.
 */
int getstr(P(char *) str, P(int) nchars, P(char) endc)
PP(char *str;)
PP(int nchars;)
PP(char endc;)
{
	register char *p;
	register int i, c;

	p = str;
	*p++ = endc;
	for (i = nchars - 2; (c = ngetch()) != endc;)
	{
		if (c == CEOF || c == '\n')
		{								/* ignore non ended string */
			*p = '\0';
			p = str;
			while (*p)
				ppputl((int) *p++);
			return FALSE;
		}								/* may be assembly language comment !!!! */
		if (--i > 0)
			*p++ = c;
		else if (!i)
			error("string too long");
		if (c == '\\')
		{
			c = ngetch();
			if (--i > 0)
				*p++ = c;
			else if (!i)
				error("string too long");
		}
	}
	*p++ = endc;
	*p = '\0';
	return TRUE;
}
