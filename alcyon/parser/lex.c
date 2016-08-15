/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#ifdef PDP11
#	define short int
#endif
#include "parser.h"
#include "lex.h"

/**
 * getdec - get a decimal number
 *		Uses Horner's method to get decimal number.  Note that
 *		multiplication by 10 is cleverly programmed as two shifts and
 *		two adds.  This is because long multiplies are painful on 
 *		both the PDP-11 and 68000.
**/
long 
getdec()						/* returns number*/
{
	register long value;
	register char c;

	for( value = 0; (c=ngetch()) >= '0' && c <= '9'; ) {
		value =<< 1;				/*value = value*2*/
		value =+ value << 2;		/*value*2 + value*8 = value*10*/
		value =+ (c-'0');
	}
	putback(c);
	return(value);
}

/**
 *	getfp - get a floating point constant
 *		we've already gotten the significant digits, now build a
 *		floating point number with possible decimal digits and an
 *		exponent, yields an ieee formated floating point number,
 *		unless the fflag is on, then a ffp constant is generated.
**/
long
getfp(significant,pseen)
long significant;
int pseen;					/* period seen and couldn't be pushed back */
{
	register char c;
	register long places;	/* decimal places */
	short esign;
	double exp, fraction, fp;

	places = 0L; esign = 0; fraction = significant; exp = 0.0;
	if (pseen || (c = ngetch()) == '.')	/* get decimal places */
		for( ; (c=ngetch()) >= '0' && c <= '9';) {
			fraction = fraction * 10.0;
			fraction = fraction + (c - '0');
			places++;
		}

	if (c=='e' || c=='E') {	/* exponent exists */
		esign = (peekis('-')) ? 1 : (peekis('+')) ? 0 : 0;
		for( ; (c=ngetch()) >= '0' && c <= '9'; ) {
			exp = exp * 10.0;
			exp = exp + (c - '0');
		}
	}

	putback(c);
	if (esign)
		exp = -exp;
	places = exp - places;
	fp = fraction * power10(places);
	if (fflag)
		return( toffp(fp) );
	else
		return ( toieee(fp) );
}

double
power10(pwr)			/* used by getfp, 10^pwr */
long pwr;
{
	double f;

	if (pwr < 0L)	/* negative power */
		for (f = 1.0; pwr < 0L; pwr++)
			f = f / 10.0;
	else			/* positive power */
		for (f = 1.0; pwr > 0L; pwr--)
			f = f * 10.0;
	return(f);
}

long
toffp(f)		/* converts current machine float to ffp rep */
double f;
{
	register long exp, l;
	register short sign;

	if (f == 0.0)
		return(0L);
	if (f < 0.0) {
		sign = 1;
		f = -f;
	}
	else
		sign = 0;
	exp = 0L;
	for( ; f >= 1.0; f = f / 2.0)
		exp++;
	for( ; f < 0.5; f = f * 2.0)
		exp--;
	f = f * 16777216.0;	/* 2 ^ 24 */
	l = f;
	l =<< 8;
	if (sign)
		l =| 0x80;
	exp =+ 0x40;
	l =| (exp & 0x7f);
	return(l);
}

long
toieee(f)		/* converts current machine float to ieee rep */
double f;
{
	register long exp, l;
	register short sign;

	if (f == 0.0)
		return(0L);
	if (f < 0.0) {
		sign = 1;
		f = -f;
	}
	else
		sign = 0;
	exp = 0L;
	for( ; f >= 2.0; f = f / 2.0)
		exp++;
	for( ; f < 1.0; f = f * 2.0)
		exp--;
	f = f - 1.0;
	f = f * 8388608.0;	/* 2 ^ 23 */
	l = f;
	if (sign)
		l =| 0x80000000;
	exp = (exp + BIAS)<<23;
	l =| (exp & 0x7f800000);
	return(l);
}

/* gethex - get an hexidecimal number*/
/*		Uses Horner's method to get hexidecimal number*/
long 
gethex()						/* returns number*/
{
	register long value;
	register char c, ch;

	value = 0;
	while( 1 ) {
		if( (c=ngetch()) >= '0' && c <= '9' )
			c =- '0';
		else if((ch=TOUPPER(c)) >= 'A' && ch <= 'F' )	/* [vlh] */
			c = ch - ('A'-10);
		else
			break;
		value = (value<<4) + c;
	}
	putback(c);
	return(value);
}

/* getoct - get an octal number*/
/*		Uses Horner's method to get octal number*/
long 
getoct(flag)				/* returns number*/
int flag;					/* string flag 1=>in string, else 0*/
{
	register long value;
	register char c;
	register short count;

	count = 0;
	for( value = 0; (c=ngetch()) >= '0' && c <= '7'; ) {
		if( flag && ++count > 3 )
			break;
		value = (value<<3) + (c-'0');
	}
	putback(c);
	return(value);
}

/**
 * gettok - get next token from input
 *		Checks pushed-packed token buffer, supresses / * * / comments,
 *		folds multiple character special symbols into single word token.
**/
gettok(force)							/* returns token type*/
int force;								/* force nested decls */
{
	register short c, nextc, i, islong;
	register char *p;
	register long value;
	char sym[SSIZE];

	if( peektok ) {
		i = peektok;
		peektok = 0;
		return(i);
	}
	while( (c=ngetch()) != EOF ) {
		switch(ctype[c]) {

		case BADC:					/*bad character*/
			error("invalid character");
			break;

		case SEMI:
			indecl = 0;
			cvalue = 0;	/* [vlh] not reserved word... */
		default:
			return( ctype[c] );

		case PERIOD:	/* [vlh] 4.2, floating point constant ?? */
			c = ngetch();
			putback(c);
			if (ctype[c] == DIGIT) {
				clvalue = getfp(0L,TRUE);
				return(CFLOAT);
			}
			return(PERIOD);
		
		case LCURBR:				/* [vlh] 4.2, next level increase */
			indecl = 0;	/* [vlh] 4.2, functions which return values */
			if (infunc) /* first curly brace will be missed */
				scope_level++;
			return(LCURBR);

		case RCURBR:				/* [vlh] 4.2, next level decrease */
			if (scope_decls[scope_level]) {
				if (scope_level != FUNC_SCOPE)
					freesyms(scope_level);
				scope_decls[scope_level] = 0;
			}
			if (scope_level != GLOB_SCOPE)
				scope_level--;
			return(RCURBR);

		case WHITSP:				/*skip all white space*/
			break;

		case EXCLAM:				/*!= or !*/
			return( peekis('=') ? NEQUALS : NOT );

		case DQUOTE:				/*quoted string*/
			getstr(cstr,STRSIZE,'"');
			cvalue = nextlabel++;
			return(STRING);

		case PERCNT:				/*%= or %*/
			return( peekis('=') ? EQMOD : MOD );

		case AMPER:					/*&=, && or &*/
			return( peekis('=') ? EQAND : peekis('&') ? LAND : AND );

		case SQUOTE:				/*character constant*/
			getstr(cstr,STRSIZE,'\'');
			if( cstrsize > CHRSPWORD+1 ) {
				error("character constant too long");
				cstrsize = CHRSPWORD + 1;
			}
			ccbytes = cstrsize - 1;
			cvalue = 0;
			for( p = cstr; --cstrsize > 0; ) {
				cvalue =<< BITSPCHAR;
				cvalue =| (*p++ & 0377);
			}
			return(CINT);

		case STAR:					/**= or **/
			return( peekis('=') ? EQMULT : MULT );

		case PLUS:					/*=+, ++ or +*/
			return( peekis('=') ? EQADD : peekis('+') ? PREINC : ADD );

		case MINUS:					/*-=, --, -> or -*/
			return( peekis('=') ? EQSUB : peekis('-') ? PREDEC :
					peekis('>') ? APTR : SUB );

		case SLASH:					/*/ *..* /, //..., /= or /*/
			if( peekis('*') ) {
				while( (c=ngetch()) != EOF )
					if( c == '*' && peekis('/') )
						break;
				if( c == EOF ) {
					error("no */ before EOF");
					return(EOF);
				}
				continue;
			}
			if( peekis('/') ) {
				while( (c=ngetch()) != EOF && c != EOLC )
					;
				continue;
			}
			return( peekis('=') ? EQDIV : DIV );

		case DIGIT:					/*number constant (long or reg)*/
			i = 0;					/*flags if long constant*/
			ccbytes = 0;
			if( c != '0' ) {
				putback(c);
dofp:
				value = getdec();
				islong = ((value > 32767) || (value < 0));
				if ((c=ngetch())=='.' || c=='e' || c=='E') { /*[vlh] 3.4 */
					putback(c);
					clvalue = getfp(value,FALSE);
					return(CFLOAT);
				}
				putback(c);
			}
			else if( peekis('x') || peekis('X') ) {
				value = gethex();
				islong = ((value > 65535) || (value < 0));
			}
			else {
				if (peekis('.')) {
					putback('.');
					goto dofp;
				}
				value = getoct(0);
				islong = ((value > 65535) || (value < 0));
			}
			if( peekis('l') || peekis('L') || islong ) {
				clvalue = value;
				return(CLONG);
			}
			cvalue = value;
			return(CINT);

		case LCAROT:				/*<=, <<, <<= or <*/
			return( peekis('=') ? LESSEQ : peekis('<') ?
					(peekis('=') ? EQLSH : LSH) : LESS );

		case EQUAL:					/*==, =<<, =>>, =+, ..., =*/
			if( peekis('<') ) {
				if( peekis('<') ) {
					warning("old fashion assignment \"=<<\"");
					return(EQLSH);
				}
			}
			else if( peekis('>') ) {
				if( peekis('>') ) {
					warning("old fashion assignment \"=>>\"");
					return(EQRSH);
				}
			}
			else if( (i=index("-*&=+/|^%",(c=ngetch()))) >= 0 ) {
				if( i < 3 ) {
					if( (nextc=ngetch()) != ' ' )
							warning("=%c assumed",c);
					putback(nextc);
				}
				i = asmap[i];
				if (i != EQUALS)
					warning("old fashion assignment statement");
				return( i );
			}
			else
				putback(c);
			return(ASSIGN);

		case RCAROT:				/*>=, >>, >>= or >*/
			return( peekis('=') ? GREATEQ : peekis('>') ?
					(peekis('=') ? EQRSH : RSH) : GREAT );

		case ALPHA:					/*[A-Za-z][A-Za-z0-9]**/
			p = &sym[0];
			i = SSIZE;
			for(; ctype[c] == ALPHA || ctype[c] == DIGIT; c=ngetch(),i-- )
				if( i > 0 )
					*p++ = c;
			if( i > 0 )
				*p = '\0';
			putback(c);
			csp = lookup(sym,indecl|force);
			if( csp->s_attrib & SRESWORD ) {
				cvalue = csp->s_offset;
				if (cvalue == R_SIZEOF) {	/* [vlh] 4.2 */
#ifdef DEBUG
	if (symdebug) printf("presizeof indecl %d\n",indecl);
#endif
					predecl = indecl;
					indecl = 0;
				}
				return(RESWORD);
			}
			smember = 0;
			return(SYMBOL);

		case CAROT:					/*^= or ^*/
			return( peekis('=') ? EQXOR : XOR );

		case BAR:					/*|=, || or |*/
			return( peekis('=') ? EQOR : peekis('|') ? LOR : OR );

		}
	}
	return(EOF);
}

/**
 * peekis - peeks at next character for specific character
 *		Gets next (possibly pushed back) character, if it matches
 *		the given character 1 is returned, otherwise the character
 *		is put back.
**/
peekis(tc)							/* returns 1 if match, 0 otherwise*/
int tc;								/* test character*/
{
	register short c;

	if( (c=ngetch()) == tc )
		return(1);
	putback(c);
	return(0);
}

/**
 * ngetch - get a possibly pushed back character
 *		Checks pbchar variable, returns it if non-zero, handles counting
 *		of new lines and whether you are in an include or not.
**/
ngetch()							/* returns character read or EOF*/
{
	register short c;
	register char *ptr;

	if( pbchar ) {
		c = pbchar;
		pbchar = 0;
		return(c);
	}

	if( (c=getc(&ibuf)) == EOLC ) {
		cr_last = 1;
		lineno++;
	}
	else if( cr_last && c == '#') {	/* [vlh] 4.2 handle: # 33 "file.h" */
		getc(&ibuf);	/* get space */
		lineno = getdec() & 077777;
		ptr = &source[0];
		getc(&ibuf);	/* get double quote */
		while ((c=getc(&ibuf)) != '\"')
			*ptr++ = c&0377;
		c = getc(&ibuf);	/* get carriage return*/
		*ptr = 0;
		cr_last = 1;
	}	/* exits with cr_last still set !!!! */
	else if( c < 0 )
		c = EOF;
	else
		cr_last = 0;
	return(c);
}

/**
 * peekc - peek at the next non-whitespace character after token
 *		This allows for the problem of having to look at two tokens
 *		at once.  The second token is always a semi-colon or colon,
 *		so we only look at the single character, rather than going
 *		thru gettok.
**/
peekc(tc)							/* returns 1 if match, 0 otherwise*/
int tc;								/* character to look for*/
{
	register short c;

	while( ctype[(c=ngetch())] == WHITSP) ;
	if( c == tc )
		return(1);
	putback(c);
	return(0);
}

/* putback - puts back a single character*/
/*		Checks pbchar for error condition.*/
putback(c)							/* returns - none*/
int c;
{
	if( pbchar )
		error("too many chars pushed back");
	else
		pbchar = c;
}

/* getstr - get a quoted (single or double) character string*/
/*		Gets specified number of characters, handling escapes.*/
getstr(str,nchars,endc)				/* returns - none*/
char *str;							/* pointer to string buffer*/
int nchars;							/* max number of characters*/
char endc;							/* ending string character*/
{
	register char *p;
	register short i;
	register short c;
	register short j;

	cstrsize = 1;
	p = str;
	for( i = nchars; (c=ngetch()) != endc; i-- ) {
		if( c == EOF || c == EOLC ) {
			error("string cannot cross line");
			break;
		}
		if( c == '\\' ) {
			if( (c=ngetch()) >= '0' && c <= '7' ) {
				putback(c);
				if( (c=getoct(1)) < 0 || c > 255 ) {
					error("bad character constant");
					continue;
				}
			}
			else if( (j=index("bnrtf",c)) >= 0 )	/* 4.1 added f... */
				c = escmap[j];
			else if( c == EOLC )	/*escape followed by nl->ignore*/
				continue;
		}
		if( i > 0 ) {				/*room left in string?*/
			cstrsize++;
			*p++ = c;
		}
		else if( !i )			/*only say error once...*/
			error("string too long");
	}
	if( i <= 0 )				/*string overflow?*/
		p--;
	*p = '\0';
}

/* next - if next token matches given token, skip and return success*/
/*		This allows for clean parsing of declarations.*/
next(tok)						/* returns 1 if matched, 0 otherwise*/
int tok;
{
	register short token;

	if( (token=gettok(0)) == tok )
		return(1);
	peektok = token;
	return(0);
}

/* pnext - if next token is a symbol, skip and return success*/
/*		This allows for clean parsing of declarations.*/
pnext()						/* returns 1 if matched, 0 otherwise*/
{
	register short token;

	if( (token=gettok(1)) == SYMBOL )	/* next symbol a param ?? */
		return(1);
	peektok = token;
	return(0);
}

/* pbtok - put back the given token*/
/*		This merely sets the peektok variable*/
pbtok(tok)							/* returns - none*/
int tok;
{
	if( peektok )
		error("too many tokens pushed back");
	peektok = tok;
}
