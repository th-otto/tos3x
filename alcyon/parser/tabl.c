/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"
#define ASGOP   OPRAS|OPASSIGN|OPLVAL|OPBIN

	/*info on operators: */
	/*000077-- OPPRI - priority */
	/*000100-- OPBIN - binary operator */
	/*000200-- OPLVAL - left operand must be lvalue */
	/*000400-- OPREL - relational operator */
	/*001000-- OPASSIGN - assignment operator */
	/*002000-- OPLWORD - short required on left */
	/*004000-- OPRWORD - short required on right */
	/*010000-- OPCOM commutative */
	/*020000-- OPRAS - right associative */
	/*040000-- OPTERM - termination node */
	/*100000 - OPCONVS - conversion operator */
short opinfo[] = {
	TRMPRI, /*EOF*/ ADDPRI | OPCOM | OPBIN,	/*ADD - expr + expr */
	ADDPRI | OPBIN,						/*SUB - expr - expr */
	MULPRI | OPCOM | OPBIN,				/*MULT - expr * expr */
	MULPRI | OPBIN,						/*DIV - expr / expr */
	MULPRI | OPBIN,						/*MOD - expr % expr */
	SHFPRI | OPLWORD | OPRWORD | OPBIN,	/*RSH - expr >> expr */
	SHFPRI | OPLWORD | OPRWORD | OPBIN,	/*LSH - expr << expr */
	ANDPRI | OPCOM | OPLWORD | OPRWORD | OPBIN,	/*AND - expr & expr */
	ORPRI | OPCOM | OPLWORD | OPRWORD | OPBIN,	/*OR - expr | expr */
	ORPRI | OPCOM | OPLWORD | OPRWORD | OPBIN,	/*XOR - expr ^ expr */
	UNOPRI | OPRAS | OPLWORD,			/*NOT - ! expr */
	UNOPRI | OPRAS,						/*UMINUS - - expr */
	UNOPRI | OPRAS | OPLWORD,			/*COMPL - ~ expr */
	UNOPRI | OPRAS | OPLVAL | OPBIN,	/*PREDEC - --lvalue */
	UNOPRI | OPRAS | OPLVAL | OPBIN,	/*PREINC - ++lvalue */
	UNOPRI | OPRAS | OPLVAL | OPBIN,	/*POSTDEC - lvalue-- */
	UNOPRI | OPRAS | OPLVAL | OPBIN,	/*POSTINC - lvalue++ */
	ASGPRI | ASGOP,						/*ASSIGN - lvalue = expr */
	ASGPRI | ASGOP,						/*EQADD - lvalue += expr */
	ASGPRI | ASGOP,						/*EQSUB - lvalue -= expr */
	ASGPRI | ASGOP,						/*EQMULT - lvalue *= expr */
	ASGPRI | ASGOP,						/*EQDIV - lvalue /= expr */
	ASGPRI | ASGOP,						/*EQMOD - lvalue %= expr */
	ASGPRI | ASGOP | OPLWORD | OPRWORD,	/*EQRSH - lvalue >>= expr */
	ASGPRI | ASGOP | OPLWORD | OPRWORD,	/*EQLSH - lvalue <<= expr */
	ASGPRI | ASGOP | OPLWORD | OPRWORD,	/*EQAND - lvalue &= expr */
	ASGPRI | ASGOP | OPLWORD | OPRWORD,	/*EQOR - lvalue |= expr */
	ASGPRI | ASGOP | OPLWORD | OPRWORD,	/*EQXOR - lvalue ^= expr */
	TRMPRI,								/*FJSR - generate function jsr */
	EQLPRI | OPREL | OPBIN,				/*EQUALS - expr == expr */
	EQLPRI | OPREL | OPBIN,				/*NEQUALS - expr != expr */
	RELPRI | OPREL | OPBIN,				/*GREAT - expr > expr */
	RELPRI | OPREL | OPBIN,				/*GREATEQ - expr >= expr */
	RELPRI | OPREL | OPBIN,				/*LESS - expr < expr */
	RELPRI | OPREL | OPBIN,				/*LESSEQ - expr <= expr */
	TRMPRI | OPCONVS,					/*INT2L */
	TRMPRI | OPCONVS,					/*LONG2I */
	TRMPRI | OPBIN, /*BTST*/ TRMPRI, /*LOAD*/ TRMPRI | OPBIN, /*LMULT*/ TRMPRI | OPBIN, /*LDIV*/ TRMPRI | OPBIN, /*LMOD*/ TRMPRI | OPBIN, /*LEQMULT*/ TRMPRI | OPBIN, /*LEQDIV*/ TRMPRI | OPBIN, /*LEQMOD*/ TRMPRI | ASGOP, /*EQADDR*/ TRMPRI, /*EQNOT*/ TRMPRI, /*EQNEG*/ TRMPRI | OPBIN, /*DOCAST*/ ASGPRI | ASGOP,	/*STASSIGN [vlh] */
	TRMPRI | OPCONVS,					/*LONG2F [vlh] 3.4 */
	TRMPRI | OPCONVS,					/*FLOAT2L [vlh] 3.4 */
	TRMPRI | OPCONVS,					/*INT2F [vlh] 3.4 */
	TRMPRI | OPCONVS,					/*FLOAT2I [vlh] 3.4 */
	UNOPRI | OPRAS,						/*TOCHAR [vlh] 4.0 */
	TRMPRI,								/*unused - 56 */
	TRMPRI,								/*unused - 57 */
	TRMPRI,								/*unused - 58 */
	TRMPRI,								/*unused - 59 */
	UNOPRI | OPRAS | OPLVAL,			/*ADDR - & expr */
	UNOPRI | OPRAS | OPLWORD,			/*INDR - * expr */
	LNDPRI | OPBIN,						/*LAND - expr && expr */
	LORPRI | OPBIN,						/*LOR - expr || expr */
	QMKPRI | OPRAS | OPBIN,				/*QMARK - expr ? expr : expr */
	QMKPRI | OPRAS | OPBIN, /*COLON*/ COMPRI | OPBIN, /*COMMA*/ TRMPRI | OPTERM, /*CINT*/ TRMPRI | OPTERM, /*CLONG*/ TRMPRI | OPTERM, /*SYMBOL*/ TRMPRI | OPTERM, /*AUTOINC*/ TRMPRI | OPTERM, /*AUTODEC*/ LPNPRI | OPBIN,	/*CALL - call with arguments */
	LPNPRI,								/*NACALL - no argument call */
	TRMPRI,								/*BFIELD - field selection */
	TRMPRI, /*IFGOTO*/ TRMPRI, /*INIT*/ TRMPRI, /*CFORREG*/ TRMPRI,	/*unused - 78 */
	TRMPRI | OPTERM,					/*CFLOAT [vlh] 3.4 */
	UNOPRI | OPRAS | OPASSIGN | OPBIN, /*CAST*/ TRMPRI, /*SEMI*/ TRMPRI,	/*LCURBR - { */
	TRMPRI,								/*RCURBR - } */
	LPNPRI,								/*LBRACK - [ */
	RPNPRI,								/*RBRACK - ] */
	LPNPRI,								/*LPAREN - ) */
	RPNPRI,								/*RPAREN - ) */
	TRMPRI | OPTERM, /*STRING*/ TRMPRI, /*RESWORD*/ LPNPRI | OPBIN,	/*APTR - expr -> symbol */
	LPNPRI | OPBIN,						/*PERIOD - expr . symbol */
	UNOPRI | OPRAS,						/*SIZEOF - sizeof expr */
	LPNPRI | OPBIN,						/*MPARENS - matching parens () */
	UNOPRI | OPRAS | OPASSIGN | OPBIN, /*FRETURN*/
};
