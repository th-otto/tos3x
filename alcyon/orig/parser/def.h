/*
 *	Copyright 1983
 *	Alcyon Corporation
 *	8716 Production Ave.
 *	San Diego, CA  92121
 *
 *	@(#)def.h	1.3	11/4/83
 */

/* Parser External Definition File */

/* key word table */
struct resword const reswords[] = {
	{ "asm",		R_ASM },
	{ "auto",		R_AUTO },
	{ "break",		R_BREAK },
	{ "case",		R_CASE },
	{ "char",		R_CHAR },
	{ "continue",	R_CONTINUE },
	{ "default",	R_DEFAULT },
	{ "do",			R_DO },
	{ "double",		R_DOUBLE },
	{ "goto",		R_GOTO },
	{ "else",		R_ELSE },
	{ "extern",		R_EXTERNAL },
	{ "float",		R_FLOAT },
	{ "for",		R_FOR },
	{ "if",			R_IF },
	{ "int",		R_INT },
	{ "long",		R_LONG },
	{ "register",	R_REGISTER },
	{ "return",		R_RETURN },
	{ "short",		R_SHORT },
	{ "sizeof",		R_SIZEOF },
	{ "static",		R_STATIC },
	{ "struct",		R_STRUCT },
	{ "switch",		R_SWITCH },
	{ "typedef",	R_TYPEDEF },
	{ "union",		R_UNION },
	{ "unsigned",	R_UNSIGNED },
	{ "while",		R_WHILE },
	{ 0, 0 }
};

char const dinfo[] = {
    0,                              /* TYPELESS=0 */
    1|DREG,                         /* CHAR=1 */
    2|DREG,                         /* SHORT=2 */
    2|DREG,                         /* INT=3 */
    4|DREG,                         /* LONG=4 */
    1|DREG,                         /* UCHAR=5 */
    2|DREG,                         /* USHORT=6 */
    2|DREG,                         /* UNSIGNED=7 */
    4|DREG,                         /* ULONG=8 */
    4|DREG,                         /* FLOAT=9 */
    4|DREG,                         /* DOUBLE=10 */
    0,                              /* STRUCT=11 */
    0,                              /* FRSTRUCT=12 */
    0,                              /* LLABEL=13 */
    0,                              /* INVALID=14 */
    0,                              /* INVALID=15 */
};

char const aregtab[] = { AREG5, AREG4, AREG3, 0 };
char const dregtab[] = { DREG7, DREG6, DREG5, DREG4, DREG3, 0 };

short nextlabel	= 1;
short structlabel = 1;
char *exprp		= &exprarea[0];
short swp = -1;
