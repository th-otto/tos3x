/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/


#define TOUPPER(c)	((c) & ~32)
#define BIAS	127L
#define EXPSIZ	4
#define FRACSIZ	20

	/**
	 * the following are the cases within gettok, all other cases are
	 * single character unambiguous tokens.  Note that we need to take
	 * special care not to interfere with the single character unambiguous
	 * operators, this is why there is a gap between WHITSP and EXCLAM.
	**/
#define	BADC	0					/*bad character*/
#define	WHITSP	101					/*white space*/
#define	EXCLAM	102					/*exlamation point*/
#define	DQUOTE	103					/*double quote*/
#define	PERCNT	104					/*percent sign*/
#define	AMPER	105					/*ampersand*/
#define	SQUOTE	106					/*single quote*/
#define	STAR	107					/*asterisk or mult sign*/
#define	PLUS	108					/*plus sign*/
#define	MINUS	109					/*minus sign*/
#define	SLASH	110					/*divide sign*/
#define	DIGIT	111					/*0..9*/
#define	LCAROT	112					/*less than sign*/
#define	EQUAL	113					/*equals sign*/
#define	RCAROT	114					/*greater than*/
#define	ALPHA	115					/*a..z,A..Z and underbar*/
#define	CAROT	116					/*^*/
#define	BAR		117					/*vertical bar*/

char ctype[] = {
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	WHITSP,	WHITSP,	WHITSP,	WHITSP,	WHITSP,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	WHITSP,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	WHITSP,	EXCLAM,	DQUOTE,	BADC,	BADC,	PERCNT,	AMPER,	SQUOTE,
	LPAREN,	RPAREN,	STAR,	PLUS,	COMMA,	MINUS,	PERIOD,	SLASH,
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,
	DIGIT,	DIGIT,	COLON,	SEMI,	LCAROT,	EQUAL,	RCAROT,	QMARK,
	BADC,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	LBRACK,	BADC,	RBRACK,	CAROT,	ALPHA,
	BADC,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	LCURBR,	BAR,	RCURBR,	COMPL,	BADC
};

#define	SOI			'\01'

	/**
	 * this table is used to check for an operator after an equals sign.
	 * note that =-, =* and =& may all have an ambiguous meaning if not
	 * followed by a space, this is checked for in gettok.
	**/
char asmap[] = {
	EQSUB,							/*=- Self modify... */
	EQMULT,							/*=* Self modify... */
	EQAND,							/*=& Self modify... */
	EQUALS,							/*==*/
	EQADD,							/*=+*/
	EQDIV,							/*=/*/
	EQOR,							/*=|*/
	EQXOR,							/*=^*/
	EQMOD,							/*=%*/
};

short pbchar;								/*pushed back character*/
char escmap[]   = "\b\n\r\t\f";			/* 4.1 added \f */

long toieee();
long toffp();
double power10();

