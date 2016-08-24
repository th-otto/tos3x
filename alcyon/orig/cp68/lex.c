/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"

#ifdef WHITESM
#define ALTER_PR 1
#endif
#ifdef GEMDOS
#define ALTER_PR 1
#endif
#ifdef CPM
#define ALTER_PR 1
#endif

int status;

extern char null[];

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

int peekis PROTO((int c));
int symhash PROTO((const char *sym));
int symequal PROTO((const char *sym1, const char *sym2));




/*
 * symhash - compute hash value for symbol
 *      Sums the symbols characters and takes that modulus the hash table
 *      size.
 * returns hash value for symbol
 */
int symhash(P(const char *) sym)
PP(const char *sym;)
{
	register const char *p;
	register int hashval, i;

	p = sym;
	i = SSIZE;
	hashval = 0;
	for (;;)
	{
		if (*p == '\0' || i <= 0)
			break;
		hashval += *p++;
		i--;
	}
#ifdef __ALCYON__
	asm("move.w    d7,d0");
	asm("ext.l     d0");
	asm("divs.w    #$03FE,d0");
	asm("swap      d0");
	asm("nop");
#else
	return hashval % (HSIZE - 2);
#endif
}


/*
 * symequal - check for symbol equality
 *      Does comparison between two symbols.
 * returns 1 if equal, 0 otherwise
 */
int symequal(P(const char *) sym1, P(const char *) sym2)
PP(const char *sym1;)
PP(const char *sym2;)
{
	register const char *p;
	register const char *q;
	register int i;

	q = sym2;
	i = SSIZE;
	p = sym1;
	for (;;)
	{
		if (*p != *q++)
			break;
		if (*p++ == '\0' || --i == 0)
		{
			return TRUE;
		}
	}
#ifdef __ALCYON__
	asm("clr.w     d0");
	asm("nop");
#else
	return FALSE;
#endif
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

	p = sym1;
	q = sym2;
	i = SSIZE;
	for (;;)
	{
		if (--i < 0)
			break;
		if (*p)
			*q++ = *p++;
		else
			*q++ = '\0';
	}
	*q = '\0';							/* [vlh] 4.1, force null terminator */
}


/*
 * error - output error message
 *      Outputs line number and error message and keeps track of errors.
 */
#ifdef __USE_VARARGS
VOID error(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
#ifndef ALTER_PR
	if (literal)
		printf((char *) STDERR, "%s, # line %d: ", lit_file, lit_num);
	else if (filep == &filestack[0])	/* [vlh] 3.4 not in include */
		printf((char *) STDERR, "%s, # line %d: ", source, lineno);
	else
		printf((char *) STDERR, "%s, # line %d: ", (filep)->ifile, (filep)->lineno);
	printf((char *) STDERR, s, x1, x2, x3, x4, x5, x6);
	cputc('\n', STDERR);
#else
	if (literal)
		printf("%s, # line %d: ", lit_file, lit_num);
	else if (filep == &filestack[0])	/* [vlh] 3.4 not in include */
		printf("%s, # line %d: ", source, lineno);
	else
		printf("%s, # line %d: ", (filep)->ifile, (filep)->lineno);
	printf(s, x1, x2, x3, x4, x5, x6);
	printf("\n");
#endif
	status++;
}
#else
VOID error(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	if (literal)
		fprintf(stderr, "%s, # line %d: ", lit_file, lit_num);
	else if (filep == &filestack[0])	/* [vlh] 3.4 not in include */
		fprintf(stderr, "%s, # line %d: ", source, lineno);
	else
		fprintf(stderr, "%s, # line %d: ", (filep - 1)->ifile, (filep - 1)->lineno);
	va_start(args, s);
	vfprintf(stderr, s, args);
	fputc('\n', stderr);
	va_end(args);
	status++;
}
#endif


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

	p = s + (int)strlen(s);
	for (;;)
	{
		if (p <= s)
			break;
		putback(*--p);
	}
}


/*
 * ngetch - get a (possibly) pushed back character
 *      This handles the include file stack and incrementing the line
 *      number for the lowest level file.
 * returns character or CEOF
 */
int ngetch(NOTHING)
{
	register int c, i;
	register char *p, *q;

	if (pbp > &pbbuf[0])
		return *--pbp;
	pbflag = 0;
	for (;;)
	{
		c = getc(&(filep->inbuf));
		if (c >= 0)
			break;
		if (filep == &filestack[0])
			return CEOF;
		close(fileno(&filep->inbuf));
		filep--;
#ifdef NONEST
		inbuf.cc = filep->tcc;
		inbuf.cp = filep->tcp;
		p = &inbuf.cbuf[0];
		q = &filep->tbuf[0];
		for (i = 0; i < BSIZE; i++)
			*p++ = *q++;
#else
/*sw This code no longer necessary ...
		inbuf.cc = 0;
        inbuf.cp = &inbuf.cbuf[0];
*/
#endif
/*sw    filep->inbuf.fd = filep->ifd; */
		if (filep == &filestack[0])
		{								/* need line for #include... */
			lineno++;
			putid(source, lineno);		/* [vlh] 4.2 id line .... */
		} else
		{
			(filep)->lineno++;
			putid((filep)->ifile, (filep)->lineno);
		}
	}
	UNUSED(p);
	UNUSED(q);
	UNUSED(i);
#ifdef __ALCYON__
	asm("move.w d7,d0");
	asm("nop");
#else
	return c;
#endif
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
	sp = &symtab[symhash(name)];
	for (;;)
	{
		if (sp->s_def == null)
			break;
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
#ifdef __ALCYON__
	asm("move.l a4,d0");
	asm("beq.s *+6");
	asm("move.l a4,d0");
	asm("bra.s *+4");
	asm("move.l a5,d0");
	asm("nop");
#else
	return asp ? asp : sp;
#endif
}


/*
 * lookup - looks up a symbol to see if it is defined
 *		Returns pointer to definition if found.
 */
char *lookup(P(const char *) name)
PP(const char *name;)
{
	register struct symbol *sp;

	sp = getsp(name);
	if (sp->s_def == 0 || sp->s_def == null)
		return NULL;
#ifdef __ALCYON__
	asm("move.l a5,d0");
	asm("nop");
#else
	return (char *) sp;
#endif
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
	register char c;
	register char *s;
	register int type;
	register int t;
	register int l;

	p = token;
	c = ngetch();
	*p++ = c;
	switch (type = ctype[(__uint8_t)c])
	{
	case SQUOTE:
	case DQUOTE:
		if (getstr(token, TOKSIZE, c))
			return type;
		else
		{								/* [vlh]4.3, ignore incomplete strings... could be asm comment */
			token[0] = '\n';
			return NEWL;
		}

	case DIGIT:
	case ALPHA:
		for (; ; p++)
		{
			if (p >= &token[TOKSIZE])
				break;
			*p = ngetch();
			if ((t = ctype[(__uint8_t)*p]) != ALPHA && t != DIGIT)
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
			l = 0;						/* [vlh] 4.3, change in line counting technique */
			for (;;)
			{
				if ((c = ngetch()) == CEOF)
					break;
				if (c == '\n')
				{
					l++;				/* [vlh] 4.3, keep line counter */
					if (Cflag)
					{					/* [vlh] 4.2 */
						ppputl('\0');
						s = line;
						for (;;)
						{
							if (*s == 0)
								break;
							doputc(*s++, &outbuf);
						}
						initl();
					}
					doputc(' ', &outbuf);
					doputc('\n', &outbuf);
				} else if (c == '*' && peekis('/'))
				{
					if (Cflag)
					{					/* [vlh] 4.2 */
						ppputl('*');
						ppputl('/');
					}
					break;
				} else if (Cflag)		/* [vlh] 4.2.c */
				{
					ppputl(c);
				}
			}
			if (c == CEOF)
				error("no */ before EOF");
			if (filep == &filestack[0])	/* [vlh] 4.3 */
				lineno += l;
			else						/* [vlh] 4.3, update include lineno */
				(filep)->lineno += l;
			type = WHITE;
			token[0] = ' ';
		} else if (peekis('/'))
		{
			if (Cflag)
			{
				ppputl('/');
				ppputl('/');
			}
			for (;;)
			{
				if ((c = ngetch()) == CEOF || c == '\n')
					break;
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
#ifdef __ALCYON__
	asm("move.w d6,d0");
	asm("nop");
#else
	return type;
#endif
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
	for (i = nchars - 2; ;)
	{
		if ((c = ngetch()) == endc)
			break;
		if (c == CEOF || c == '\n')
		{								/* [vlh] 4.3, ignore non ended string */
			*p = '\0';
			p = str;
			for (;;)
			{
				if (*p == '\0')
					break;
				ppputl((int) *p++);
			}
			return FALSE;
		}								/* [vlh] 4.3, may be assembly language comment !!!! */
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
#ifdef __ALCYON__
	asm("moveq #1,d0");
	asm("nop");
#else
	return TRUE;
#endif
}


/*
 * peekis - peeks at next character for specific character
 *      Gets next (possibly pushed back) character, if it matches
 *      the given character 1 is returned, otherwise the character
 *      is put back.
 */
int peekis(P(int) tc)
PP(int tc;)
{
	register int c;

	if ((c = ngetch()) == tc)
		return TRUE;
	putback(c);
#ifdef __ALCYON__
	asm("clr.w d0");
	asm("nop");
#else
	return FALSE;
#endif
}


VOID doputc(P(char) ch, P(struct iob *) buffer)
PP(char ch;)
PP(struct iob *buffer;)
{
	if (!Eflag)
		putc(ch, buffer);
	else
		putchar(ch);
}


#ifdef DECC								/*sw You should really do this as a library... */
int getc(ibuf)
struct iob *ibuf;
{
	if (ibuf->cc <= 0)
	{
		ibuf->cp = &(ibuf->cbuf[0]);
		ibuf->cc = read(ibuf->fd, ibuf->cp, BSIZE);
	}
	if (ibuf->cc <= 0)
		return EOF;
	ibuf->cc--;
	return ((int) (*(ibuf->cp)++) & 0xff);
}


int fopen(fname, ibuf)
char *fname;
register struct iob *ibuf;
{
	ibuf->cc = 0;						/* no chars */
	ibuf->fd = open(fname, 0);
	return ibuf->fd;
}
#endif
