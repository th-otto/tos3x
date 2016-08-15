/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "cgen.h"
#include "cskel.h"

#define	I_NULL	0
#define	I_ADD	1
#define	I_INC	2
#define	I_SUB	3
#define	I_DEC	4
#define	I_MULS	5
#define	I_MULU	6
#define	I_DIVS	7
#define	I_DIVU	8
#define	I_ASR	9
#define	I_LSR	10
#define	I_ASL	11
#define	I_LSL	12
#define	I_AND	13
#define	I_OR	14
#define	I_EOR	15
#define	I_NEG	16
#define	I_NOT	17
#define	I_MOVE	18
#define	I_CLR	19
#define	I_CMP	20
#define	I_TST	21
#define	I_LMUL	22
#define	I_LDIV	23
#define	I_LREM	24
#define	I_LEML	25
#define I_ALDIV 26
#define	I_LERM	27
#define	I_BEQ	28
#define	I_BNE	29
#define	I_BGT	30
#define	I_BGE	31
#define	I_BLT	32
#define	I_BLE	33
#define	I_BLS	34
#define	I_BLO	35
#define	I_BCC	36
#define	I_BHI	37
#define	I_BRA	38
#define	I_NOP	39
#define	I_BTST	40

char *mnemonics[] = {
	"",
	"add",
	"inc",
	"sub",
	"dec",
	"muls",
	"mulu",
	"divs",
	"divu",
	"asr",
	"lsr",
	"asl",
	"lsl",
	"and",
	"or",
	"eor",
	"neg",
	"not",
	"move",
	"clr",
	"cmp",
	"tst",
	"lmul",
	"_ldiv",
	"lrem",
	"almul",
	"aldiv",
	"alrem",
	"beq",
	"bne",
	"bgt",
	"bge",
	"blt",
	"ble",
	"bls",
	"blo",
	"bcc",
	"bhi",
	"jmp",
	"*nop",
	"btst",
};

#define	FE_EQOP		1
#define	FE_ASSIGN	2
#define	FE_EQSHFT	3
#define	FE_EQXOR	4
#define	FE_EQADDR	5
#define	FC_FIX		6
#define	FC_REL		7
#define	FC_BTST		8
#define	FS_OP		9
#define	FS_ITL		10
#define	FS_LD		11
#define	FR_ADD		12
#define	FR_MULT		13
#define	FR_DIV		14
#define	FR_SHFT		15
#define	FR_XOR		16
#define	FR_NEG		17
#define	FR_EQOP		18
#define	FR_POSTOP	19
#define	FR_ASSIGN	20
#define	FR_EQMULT	21
#define	FR_EQDIV	22
#define	FR_EQSHFT	23
#define	FR_EQXOR	24
#define	FR_CALL		25
#define	FR_ITL		26
#define	FR_LTI		27
#define	FR_LD		28
#define	FR_EQADDR	29
#define	FR_EQNOT	30
#define	FE_EQNOT	31
#define	FR_DOCAST	32
#define	FS_DOCAST	33
#define	FR_FTOL		34
#define	FR_LTOF		35
#define	FR_FTOI		36
#define	FR_ITOF		37
#define FE_EQMULT	38
#define FE_EQDIV	39
#define FE_EQMOD	40
#define FR_TOCHAR	41

extern struct skeleton /*char /* really struct skeleton.... */
	fe_eqop[],						/* 1=FE_EQOP */
	fe_assign[],					/* 2=FE_ASSIGN */
	fe_eqshft[],					/* 3=FE_EQSHFT */
	fe_eqxor[],						/* 4=FE_EQXOR */
	fe_eqaddr[],					/* 5=FE_EQADDR */
	fc_fix[],						/* 6=FC_FIX */
	fc_rel[],						/* 7=FC_REL */
	fc_btst[],						/* 8=FC_BTST */
	fs_op[],						/* 9=FS_OP */
	fs_itl[],						/* 10=FS_ITL */
	fs_ld[],						/* 11=FS_LD */
	fr_op[],						/* 12=FR_OP */
	fr_mult[],						/* 13=FR_MULT */
	fr_div[],						/* 14=FR_DIV */
	fr_shft[],						/* 15=FR_SHFT */
	fr_xor[],						/* 16=FR_XOR */
	fr_neg[],						/* 17=FR_NEG */
	fr_eqop[],						/* 18=FR_EQOP */
	fr_postop[],					/* 19=FR_POSTOP */
	fr_assign[],					/* 20=FR_ASSIGN */
	fr_eqmult[],					/* 21=FR_EQMULT */
	fr_eqdiv[],						/* 22=FR_EQDIV */
	fr_eqshft[],					/* 23=FR_EQSHFT */
	fr_eqxor[],						/* 23=FR_EQXOR */
	fr_call[],						/* 24=FR_CALL */
	fr_itl[],						/* 25=FR_ITL */
	fr_lti[],						/* 26=FR_LTI */
	fr_ld[],						/* 27=FR_LD */
	fr_eqaddr[],					/* 28=FR_EQADDR */
	fr_eqnot[],						/* 29=FR_EQNOT */
	fe_eqnot[],						/* 30=FE_EQNOT */
	fr_docast[],					/* 31=FR_DOCAST */
	fs_docast[],					/* 32=FS_DOCAST */
	fr_ftol[],						/* 34=FE_FTOL */
	fr_ltof[],						/* 35=FE_LTOF */
	fr_ftoi[],						/* 36=FE_FTOI */
	fr_itof[],						/* 37=FE_ITOF */
	fe_eqmult[],					/* 38=FE_EQMULT */
	fe_eqdiv[],						/* 39=FE_EQDIV */
	fe_eqmod[],						/* 40=FE_EQMOD */
	fr_tochar[];					/* 41=FR_TOCHAR */

char *codeskels[] = {
	0,								/*NULL*/
	fe_eqop,						/*1=FE_EQOP*/
	fe_assign,						/*2=FE_ASSIGN*/
	fe_eqshft,						/*3=FE_EQSHFT*/
	fe_eqxor,						/*4=FE_EQXOR*/
	fe_eqaddr,						/*5=FE_EQADDR*/
	fc_fix,							/*6=FC_FIX*/
	fc_rel,							/*7=FC_REL*/
	fc_btst,						/*8=FC_BTST*/
	fs_op,							/*9=FS_OP*/
	fs_itl,							/*10=FS_ITL*/
	fs_ld,							/*11=FS_LD*/
	fr_op,							/*12=FR_OP*/
	fr_mult,						/*13=FR_MULT*/
	fr_div,							/*14=FR_DIV*/
	fr_shft,						/*15=FR_SHFT*/
	fr_xor,							/*16=FR_XOR*/
	fr_neg,							/*17=FR_NEG*/
	fr_eqop,						/*18=FR_EQOP*/
	fr_postop,						/*19=FR_POSTOP*/
	fr_assign,						/*20=FR_ASSIGN*/
	fr_eqmult,						/*21=FR_EQMULT*/
	fr_eqdiv,						/*22=FR_EQDIV*/
	fr_eqshft,						/*23=FR_EQSHFT*/
	fr_eqxor,						/*24=FR_EQXOR*/
	fr_call,						/*25=FR_CALL*/
	fr_itl,							/*26=FR_ITL*/
	fr_lti,							/*27=FR_LTI*/
	fr_ld,							/*28=FE_LD*/
	fr_eqaddr,						/*29=FE_EQADDR*/
	fr_eqnot,						/*30=FE_EQNOT*/
	fe_eqnot,						/*31=FE_EQNOT*/
	fr_docast,						/*32=FE_DOCAST*/
	fs_docast,						/*33=FS_DOCAST*/
	fr_ftol,						/*34=FE_FTOL*/
	fr_ltof,						/*35=FE_LTOF*/
	fr_ftoi,						/*36=FE_FTOI*/
	fr_itof,						/*37=FE_ITOF*/
	fe_eqmult,						/*38=FE_EQMULT*/
	fe_eqdiv,						/*39=FE_EQDIV*/
	fe_eqmod,						/*40=FE_EQMOD*/
	fr_tochar,						/*41=FR_TOCHAR*/
};
	/*
	 *This is the major table directing the code generation process.
	 *It is indexed by an O_op operator, which is obtained from the
	 *opinfo table for an intermediate code operator.  The actual
	 *code skeleton macros are in cskels.c, which are in a linked
	 *list in order of decreasing order of difficulty.
	 */
char optab[][6] = {

/*	I		I2		effect		cc's	stack	register*/

	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	I_NULL,		/*0=NULL*/
	I_ADD,	I_INC,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*1=ADD*/
	I_SUB,	I_DEC,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*2=SUB*/
	I_MULS,	I_MULU,	I_NULL,		FC_FIX,	I_NULL,	FR_MULT,	/*3=MULT*/
	I_DIVS,	I_DIVU,	I_NULL,		FC_FIX,	I_NULL,	FR_DIV,		/*4=DIV*/
	I_DIVS,	I_DIVU,	I_NULL,		I_NULL,	I_NULL,	FR_DIV,		/*5=MOD*/
	I_ASR,	I_LSR,	I_NULL,		FC_FIX,	I_NULL,	FR_SHFT,	/*6=RSH*/
	I_ASL,	I_LSL,	I_NULL,		FC_FIX,	I_NULL,	FR_SHFT,	/*7=LSH*/
	I_AND,	I_AND,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*8=AND*/
	I_OR,	I_OR,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*9=OR*/
	I_EOR,	I_EOR,	I_NULL,		FC_FIX,	I_NULL,	FR_XOR,		/*10=XOR*/
	I_NULL,	I_NULL,	I_NULL,		FC_FIX,	I_NULL,	I_NULL,		/*11=NOT*/
	I_NEG,	I_NEG,	I_NULL,		FC_FIX,	I_NULL,	FR_NEG,		/*12=NEG*/
	I_NOT,	I_NOT,	I_NULL,		I_NULL,	I_NULL,	FR_NEG,		/*13=COMPL*/
	I_SUB,	I_DEC,	FE_EQOP,	FC_FIX,	I_NULL,	FR_EQOP,	/*14=PREDEC*/
	I_ADD,	I_INC,	FE_EQOP,	FC_FIX,	I_NULL,	FR_EQOP,	/*15=PREINC*/
	I_SUB,	I_DEC,	FE_EQOP,	I_NULL,	I_NULL,	FR_POSTOP,	/*16=POSTDEC*/
	I_ADD,	I_INC,	FE_EQOP,	I_NULL,	I_NULL,	FR_POSTOP,	/*17=POSTINC*/
	I_MOVE,	I_CLR,	FE_ASSIGN,	I_NULL,	I_NULL,	FR_ASSIGN,	/*18=ASSIGN*/
	I_ADD,	I_INC,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*19=EQADD*/
	I_SUB,	I_DEC,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*20=EQSUB*/
	I_MULS,	I_MULU,	FE_EQMULT,	FC_FIX,	I_NULL,	FR_EQMULT,	/*21=EQMULT*/
	I_DIVS,	I_DIVU,	FE_EQDIV,	FC_FIX,	I_NULL,	FR_EQDIV,	/*22=EQDIV*/
	I_DIVS,	I_DIVU,	FE_EQMOD,	I_NULL,	I_NULL,	FR_EQDIV,	/*23=EQMOD*/
	I_ASR,	I_LSR,	FE_EQSHFT,	I_NULL,	I_NULL,	FR_EQSHFT,	/*24=EQRSH*/
	I_ASL,	I_LSL,	FE_EQSHFT,	I_NULL,	I_NULL,	FR_EQSHFT,	/*25=EQLSH*/
	I_AND,	I_AND,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*26=EQAND*/
	I_OR,	I_OR,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*27=EQOR*/
	I_EOR,	I_EOR,	FE_EQXOR,	FC_FIX,	I_NULL,	FR_EQXOR,	/*28=EQXOR*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_CALL,	/*29=FJSR*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*30=EQUALS*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*31=NEQUALS*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*32=GREAT*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*33=GREATEQ*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*34=LESS*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*35=LESSEQ*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	FS_ITL,	FR_ITL,		/*36=INT2L*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_LTI,		/*37=LONG2I*/
	I_BTST,	I_BTST,	I_NULL,		FC_BTST,	I_NULL,	I_NULL,	/*38=BTST*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	FS_LD,	FR_LD,		/*39=LOAD*/
	I_MULS,	I_MULU,	I_NULL,		I_NULL,	I_NULL,	FR_MULT,	/*40=LMULT*/
	I_DIVS,	I_DIVU,	I_NULL,		I_NULL,	I_NULL,	FR_DIV,		/*41=LDIV*/
	I_DIVS,	I_DIVU,	I_NULL,		I_NULL,	I_NULL,	FR_DIV,		/*42=LMOD*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	I_NULL,		/*43=NULL*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	I_NULL,		/*44=NULL*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	I_NULL,		/*45=NULL*/
	I_NULL,	I_NULL,	FE_EQADDR,	I_NULL,	I_NULL,	FR_EQADDR,	/*46=EQADDR*/
	I_NOT,	I_NOT,	FE_EQNOT,	I_NULL,	I_NULL,	FR_EQNOT,	/*47=EQNOT*/
	I_NEG,	I_NEG,	FE_EQNOT,	I_NULL,	I_NULL,	FR_EQNOT,	/*48=EQNEG*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	FS_DOCAST,	FR_DOCAST,	/*49=DOCAST*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	I_NULL,		/*50=STASSIGN*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_LTOF,	/*51=LONG2F*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_FTOL,	/*52=FLOAT2L*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_ITOF,	/*53=INT2F*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_FTOI,	/*54=FLOAT2I*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_TOCHAR,	/*55=TOCHAR*/
};


	/*this maps comparison operators and comparison types into the*/
	/*actual branch opcode used.*/
char brtab[][2] = {
	I_BEQ,	I_BEQ,					/*EQUALS*/
	I_BNE,	I_BNE,					/*NEQUALS*/
	I_BGT,	I_BHI,					/*GREAT*/
	I_BGE,	I_BCC,					/*GREATEQ*/
	I_BLT,	I_BLO,					/*LESS*/
	I_BLE,	I_BLS,					/*LESSEQ*/
};

	/*turns !x>y into x<=y*/
short invrel[] = { NEQUALS, EQUALS, LESSEQ, LESS, GREATEQ, GREAT };

	/*turns x>y into y<=x*/
short swaprel[] = { EQUALS, NEQUALS, LESS, LESSEQ, GREAT, GREATEQ };

	/*code skeleton built-in strings*/
char *strtab[] = {
	"move",							/*MOV*/
	"move.l",						/*MOVL*/
	"jsr",							/*JSR*/
	"clr",							/*CLR*/
	"clr.l",						/*CLRL*/
	"ext.w",						/*EXTW*/
	"ext.l",						/*EXTL*/
	"lea",							/*LEA*/
	"(sp)",							/*STK*/
};
