#include "cgen.h"

static char const cteqa01[] = {
	LEA, ' ', RADDR, ',', LADDR, '-', '\n',
	MOVL, ' ', LADDR, '+', ',', CR, '\n',
	0
};

static char const cteqa02[] = {
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LADDR, '\n',
	MOVL, ' ', CAR, ',', CR, '\n',
	0
};

static char const cteqa03[] = {
	LEFT, S_NEXT | S_INDR,
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LOFFSET, '(', NAR, ')', '\n',
	MOVL, ' ', CAR, ',', CR, '\n',
	0
};

static struct skeleton const fr_eqaddr[] = {
	{ SU_AREG | T_LONG, SU_ANY | T_ANY, cteqa01 },
	{ SU_AREG | T_LONG, SU_ANY | T_LONG, cteqa01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, cteqa02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, cteqa02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, cteqa03 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, cteqa03 },
	{ 0, 0, NULL }
};

static char const cteqa04[] = {
	LEA, ' ', RADDR, ',', LADDR, '\n',
	0
};

static char const cteqa05[] = {
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LADDR, '\n',
	0
};

static char const cteqa06[] = {
	LEFT, S_NEXT | S_INDR,
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

static struct skeleton const fe_eqaddr[] = {
	{ SU_AREG | T_LONG, SU_ANY | T_ANY, cteqa04 },
	{ SU_AREG | T_LONG, SU_ANY | T_LONG, cteqa04 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, cteqa05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, cteqa05 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, cteqa06 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, cteqa06 },
	{ 0, 0, NULL }
};

static char const ctcal01[] = {
	JSR, ' ', LADDR, '\n',
	0
};

static char const ctcal02[] = {
	LEFT, S_INDR,
	JSR, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctcal03[] = {
	LEFT, 0,
	MOV, TLEFT, ' ', CR, ',', CAR, '\n',
	JSR, ' ', '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_call[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctcal01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctcal01 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctcal02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctcal02 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctcal03 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctcal03 },
	{ 0, 0, NULL }
};

static char const ctlod00[] = {
	CLR, ' ', CR, '\n',
	0
};

static char const ctlod01[] = {
	CLR, TLEFT, ' ', CR, '\n',
	0
};

static char const ct11d[] = {
	CLR, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	0
};

static char const ctlod02[] = {
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	0
};

static char const ctlod03[] = {
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	0
};

static char const ct14d[] = {
	LEFT, S_INDR,
	CLR, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

static char const ctlod04[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	0
};

static char const ctlod05[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

static struct skeleton const fr_ld[] = {
	{ SU_ZERO | T_CHAR, SU_ANY | T_ANY, ctlod00 },
	{ SU_ZERO | T_INT, SU_ANY | T_ANY, ctlod01 },
	{ SU_ZERO | T_LONG, SU_ANY | T_ANY, ctlod01 },
	{ SU_ADDR | T_UCHAR, SU_ANY | T_ANY, ct11d },
	{ SU_ADDR | T_CHAR, SU_ANY | T_ANY, ctlod02 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctlod03 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctlod03 },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_ANY, ctlod03 },
	{ SU_ANY | T_UCHAR | T_INDR, SU_ANY | T_ANY, ct14d },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_ANY, ctlod04 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_ANY, ctlod05 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctlod05 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctlod05 },
	{ 0, 0, NULL }
};

static char const ct17d[] = {
	CLR, ' ', PSH, '\n',
	0
};

static char const ctlod06[] = {
	CLR, TLEFT, ' ', PSH, '\n',
	0
};

static char const ctlod07[] = {
	LEFT, 0,
	MOV, ' ', CR, ',', PSH, ' ', '\n',
	0
};

static char const ctlod08[] = {
	MOV, TLEFT, ' ', LADDR, ',', PSH, '\n',
	0
};

static char const ctlod09[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', PSH, '\n',
	0
};

static struct skeleton const fs_ld[] = {
	{ SU_ZERO | T_CHAR, SU_ANY | T_ANY, ct17d },
	{ SU_ZERO | T_INT, SU_ANY | T_ANY, ctlod06 },
	{ SU_ZERO | T_LONG, SU_ANY | T_ANY, ctlod06 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_ANY, ctlod07 },
	{ SU_ADDR | T_INT, SU_ANY | T_ANY, ctlod08 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctlod08 },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_ANY, ctlod08 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_ANY, ctlod09 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctlod09 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctlod09 },
	{ 0, 0, NULL }
};

static char const ctpst03[] = {
	MOVL, ' ', '#', '1', ',', PSH, '\n',
	JSR, ' ', '_', 'f', 'p', 'l', 't', 'o', 'f', '\n',
	POP4, MOVL, ' ', 'D', '0', ',', PSH, '\n',
	LEFT, S_INDR,
	MOVL, ' ', LOFFSET, '(', CAR, ')', ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP8, MOVL, ' ', 'D', '0', ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctpst02[] = {
	MOVL, ' ', '#', '1', ',', PSH, '\n',
	JSR, ' ', '_', 'f', 'p', 'l', 't', 'o', 'f', '\n',
	POP4, MOVL, ' ', 'D', '0', ',', PSH, '\n',
	MOVL, ' ', LADDR, '-', ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP8, MOVL, ' ', 'D', '0', ',', LADDR, '+', '\n',
	0
};

static char const ctpst01[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', LADDR, '+', '\n',
	0
};

static char const ctpst04[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_postop[] = {
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctpst03 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctpst02 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctpst01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctpst01 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctpst04 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctpst04 },
	{ 0, 0, NULL }
};

static char const ctneg01[] = {
	LEFT, 0,
	OP, ' ', CR, '\n',
	0
};

static char const ctneg02[] = {
	LEFT, 0,
	OP, TLEFT, ' ', CR, '\n',
	0
};

static char const ctneg03[] = {
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP4,
	0
};

static struct skeleton const fr_neg[] = {
	{ SU_ANY | T_CHAR, SU_ANY | T_ANY, ctneg01 },
	{ SU_ANY | T_INT, SU_ANY | T_ANY, ctneg02 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctneg02 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

static char const ctasg01[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctasg02[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctasg03[] = {
	LEFT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', LOFFSET, '(', NAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

static char const ctasg04[] = {
	LEFT, S_INDR,
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	EXRL,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctasg05[] = {
	RIGHT, 0,
	EXRL,
	LEFT, S_NEXT | S_INDR,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

static char const ctasg06[] = {
	LEFT, S_STACK | S_INDR,
	RIGHT, 0,
	EXRL,
	MOVL, ' ', POP, ',', CAR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_assign[] = {
	{ SU_ADDR | T_LONG, SU_ADDR | T_UNSN, ctasg01 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctasg02 },
	{ SU_ADDR | T_ANY, SU_ANY | T_LONG, ctasg02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctasg02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctasg02 },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_FLOAT, ctasg02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_UNSN, ctasg03 },
	{ SU_ANY | T_ANY | T_INDR, SU_ADDR | T_INT, ctasg04 },
	{ SU_ANY | T_ANY | T_INDR, SU_ADDR | T_LONG, ctasg04 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_INT, ctasg04 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_LONG, ctasg04 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ADDR | T_FLOAT, ctasg04 },
	{ SU_EASY | T_ANY | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_EASY | T_ANY | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_EASY | T_LONG | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_EASY | T_LONG | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctasg06 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_LONG, ctasg06 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctasg06 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, ctasg06 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_FLOAT, ctasg06 },
	{ 0, 0, NULL }
};

static char const ctasg07[] = {
	AOP, TLEFT, ' ', LADDR, '\n',
	0
};

static char const ctasg08[] = {
	LEFT, S_INDR,
	AOP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctasg09[] = {
	OP, TLEFT, ' ', RADDR, ',', LADDR, '\n',
	0
};

static char const ctasg0a[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOVL, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctasg10[] = {
	RIGHT, 0,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctasg0b[] = {
	CLR, TLEFT, ' ', LADDR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	0
};

static char const ctasg11[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	EXTW, ' ', LADDR, '\n',
	0
};

static char const ctasg12[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	EXTW, ' ', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	0
};

static char const ctasg13[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctasg22[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	0
};

static char const ctasg14[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	0
};

static char const ctasg15[] = {
	MOV, ' ', RADDR, ',', CAR, '\n',
	OP, TLEFT, ' ', CAR, ',', LADDR, '\n',
	0
};

static char const ctasg16[] = {
	LEFT, S_INDR,
	CLRL, ' ', CR, '\n',
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctasg17[] = {
	RIGHT, S_INDR,
	OP, TLEFT, ' ', ROFFSET, '(', CAR, ')', ',', LADDR, '\n',
	0
};

static char const ctasg18[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctasg19[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctasg20[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT | S_INDR,
	OP, TLEFT, ' ', ROFFSET, '(', NAR, ')', ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctasg23[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	OP, TLEFT, ' ', POP, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fe_assign[] = {
	{ SU_ADDR | T_ANY, SU_ZERO | T_ANY, ctasg07 },
	{ SU_ADDR | T_LONG, SU_ZERO | T_ANY, ctasg07 },
	{ SU_ANY | T_ANY | T_INDR, SU_ZERO | T_ANY, ctasg08 },
	{ SU_ADDR | T_CHAR, SU_ADDR | T_CHAR, ctasg09 },
	{ SU_ADDR | T_INT, SU_ADDR | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_LONG, ctasg09 },
	{ SU_ADDR | T_ANY, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_ANY, SU_REG | T_INT, ctasg09 },
	{ SU_ADDR | T_ANY, SU_REG | T_LONG, ctasg09 },
	{ SU_ADDR | T_FLOAT, SU_ADDR | T_FLOAT, ctasg09 },
	{ SU_AREG | T_LONG, SU_ADDR | T_UCHAR, ctasg0a },
	{ SU_AREG | T_ANY, SU_ADDR | T_UCHAR, ctasg0a },
	{ SU_AREG | T_LONG, SU_ADDR | T_CHAR, ctasg10 },
	{ SU_AREG | T_ANY, SU_ADDR | T_CHAR, ctasg10 },
	{ SU_REG | T_LONG, SU_ADDR | T_UNSN, ctasg0b },
	{ SU_REG | T_LONG, SU_ADDR | T_UCHAR, ctasg0b },
	{ SU_REG | T_INT, SU_ADDR | T_UCHAR, ctasg0b },
	{ SU_REG | T_INT, SU_ADDR | T_CHAR, ctasg11 },
	{ SU_REG | T_LONG, SU_ADDR | T_CHAR, ctasg12 },
	{ SU_AREG | T_LONG, SU_ADDR | T_UCHAR, ctasg13 },
	{ SU_AREG | T_LONG, SU_ADDR | T_INT, ctasg22 },
	{ SU_REG | T_LONG, SU_ADDR | T_INT, ctasg14 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_UNSN, ctasg01 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_INT, ctasg15 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_UNSN, ctasg16 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_CHAR | T_INDR, ctasg17 },
	{ SU_ADDR | T_INT, SU_ANY | T_INT | T_INDR, ctasg17 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG | T_INDR, ctasg17 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctasg18 },
	{ SU_ADDR | T_ANY, SU_ANY | T_LONG, ctasg18 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctasg18 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctasg18 },
	{ SU_ANY | T_INT | T_INDR, SU_ADDR | T_INT, ctasg19 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ADDR | T_CHAR, ctasg19 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_LONG, ctasg19 },
	{ SU_ANY | T_ANY | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ADDR | T_FLOAT, ctasg19 },
	{ SU_EASY | T_CHAR | T_INDR, SU_EASY | T_CHAR | T_INDR, ctasg20 },
	{ SU_EASY | T_INT | T_INDR, SU_EASY | T_INT | T_INDR, ctasg20 },
	{ SU_EASY | T_LONG | T_INDR, SU_EASY | T_LONG | T_INDR, ctasg20 },
	{ SU_EASY | T_ANY | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_EASY | T_ANY | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_EASY | T_LONG | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_EASY | T_LONG | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_ANY, ctasg23 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, ctasg23 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctasg06 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_LONG, ctasg06 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctasg06 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ADDR | T_FLOAT, ctasg06 },
	{ 0, 0, NULL }
};

static char const ctbop00[] = {
	LEFT, 0,
	CLR, ' ', CR, '\n',
	0
};

static char const ctbop01[] = {
	LEFT, 0,
	CLR, TLEFT, ' ', CR, '\n',
	0
};

static char const ctbop02[] = {
	LEFT, 0,
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	0
};

static char const ctbop03[] = {
	LEFT, 0,
	CLRL, ' ', NR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', NR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	0
};

static char const ctbop04[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	RIGHT, S_NEXT,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	0
};

static char const ctbop05[] = {
	LEFT, 0,
	OP, TLEFT, ' ', RADDR, ',', CR, '\n',
	0
};

static char const ctbop06[] = {
	LEFT, 0,
	EXLR,
	RIGHT, S_NEXT,
	EXRLN,
	OP, TEITHER, ' ', NR, ',', CR, '\n',
	0
};

static char const ctbop07[] = {
	RIGHT, 0,
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, 0,
	OP, TLEFT, ' ', POP, ',', CR, '\n',
	0
};

static char const ctbop08[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	0
};

static char const ctbop09[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	0
};

static char const ctbop12[] = {
	RIGHT, S_STACK,
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static struct skeleton const fr_add[] = {
	{ SU_ANY | T_CHAR, SU_ZERO | T_ANY, ctbop00 },
	{ SU_ANY | T_INT, SU_ZERO | T_ANY, ctbop01 },
	{ SU_ANY | T_LONG, SU_ZERO | T_ANY, ctbop01 },
	{ SU_ANY | T_ANY, SU_ADDR | T_INT, ctbop02 },
	{ SU_ANY | T_ANY, SU_ADDR | T_LONG, ctbop02 },
	{ SU_ANY | T_LONG, SU_ADDR | T_LONG, ctbop02 },
	{ SU_ANY | T_LONG, SU_ADDR | T_UNSN, ctbop03 },
	{ SU_ADDR | T_UNSN, SU_ANY | T_LONG, ctbop04 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctbop05 },
	{ SU_ANY | T_LONG, SU_ADDR | T_LONG, ctbop05 },
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_ANY, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctbop07 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctbop08 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctbop08 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctbop09 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_FLOAT, ctbop12 },
	{ 0, 0, NULL }
};

static char const ctbop10[] = {
	LEFT, S_STACK,
	OP, TLEFTL, ' ', RADDR, ',', STK, '\n',
	0
};

static char const ctbop11[] = {
	LEFT, S_STACK,
	RIGHT, 0,
	OP, TLEFTL, ' ', CR, ',', STK, '\n',
	0
};

static struct skeleton const fs_op[] = {
	{ SU_ANY | T_ANY, SU_CONST | T_INT, ctbop10 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctbop10 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctbop11 },
	{ 0, 0, NULL }
};

static char const ctxor01[] = {
	LEFT, S_STACK,
	RIGHT, 0,
	OP, ' ', CR, ',', STK, '\n',
	MOV, ' ', POP, ',', CR, '\n',
	0
};

static char const ct71r[] = {
	LEFT, S_STACK,
	RIGHT, 0,
	OP, TLEFT, ' ', CR, ',', STK, '\n',
	MOVL, ' ', POP, ',', CR, '\n',
	0
};

static struct skeleton const fr_xor[] = {
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_ANY, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctxor01 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ct71r },
	{ 0, 0, NULL }
};

static char const ctshft00[] = {
	LEFT, 0,
	0
};

static char const ctshf01[] = {
	LEFT, 0,
	OP, TLEFTL, ' ', RADDR, ',', CR, '\n',
	0
};

static char const ctshf02[] = {
	LEFT, 0,
	RIGHT, S_NEXT,
	OP, TLEFTL, ' ', NR, ',', CR, '\n',
	0
};

static struct skeleton const fr_shft[] = {
	{ SU_ANY | T_ANY, SU_ZERO | T_ANY, ctshft00 },
	{ SU_ANY | T_LONG, SU_ZERO | T_ANY, ctshft00 },
	{ SU_ANY | T_ANY, SU_SMALL | T_INT, ctshf01 },
	{ SU_ANY | T_LONG, SU_SMALL | T_INT, ctshf01 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctshf02 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctshf02 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctshf02 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctshf02 },
	{ 0, 0, NULL }
};

static char const ctmul01[] = {
	LEFT, 0,
	OP, ' ', RADDR, ',', CR, '\n',
	0
};

static char const ctmul02[] = {
	LEFT, 0,
	RIGHT, S_NEXT | S_INDR,
	OP, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	0
};

static char const ctmul03[] = {
	LEFT, 0,
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	0
};

static char const ctmul04[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	0
};

static char const ctmul05[] = {
	MOVL, ' ', RADDR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static char const ctmul06[] = {
	RIGHT, 0,
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static char const ctmul07[] = {
	LEFT, 0,
	EXLR,
	MOVL, ' ', CR, ',', PSH, '\n',
	RIGHT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static struct skeleton const fr_mult[] = {
	{ SU_ANY | T_CHAR, SU_ZERO | T_ANY, ctbop00 },
	{ SU_ANY | T_INT, SU_ZERO | T_ANY, ctbop01 },
	{ SU_ANY | T_LONG, SU_ZERO | T_ANY, ctbop01 },
	{ SU_ANY | T_ANY, SU_ADDR | T_INT, ctmul01 },
	{ SU_ANY | T_ANY, SU_EASY | T_INT | T_INDR, ctmul02 },
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctmul03 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctmul04 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctmul05 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctmul06 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctmul07 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctbop12 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_FLOAT, ctbop12 },
	{ 0, 0, NULL }
};

static char const ctdiv00[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdif01[] = {
	LEFT, 0,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv02[] = {
	LEFT, 0,
	EXL,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv03[] = {
	RIGHT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv4a[] = {
	LEFT, 0,
	RIGHT, S_NEXT | S_INDR,
	OP, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv04[] = {
	LEFT, 0,
	EXL,
	RIGHT, S_NEXT | S_INDR,
	OP, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv05[] = {
	CLRL, ' ', CR, '\n',
	MOV, ' ', LADDR, ',', CR, '\n',
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv06[] = {
	LEFT, 0,
	EXL,
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv07[] = {
	LEFT, 0,
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv08[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv09[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	EXL,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, 0
};

static char const ctdiv10[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, 0
};

static struct skeleton const fr_div[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctdiv00 },
	{ SU_ANY | T_LONG, SU_ADDR | T_INT, ctdif01 },
	{ SU_ANY | T_ANY, SU_ADDR | T_INT, ctdiv02 },
	{ SU_ADDR | T_UNSN, SU_EASY | T_INT | T_INDR, ctdiv03 },
	{ SU_ANY | T_LONG, SU_EASY | T_INT | T_INDR, ctdiv4a },
	{ SU_ANY | T_ANY, SU_EASY | T_INT | T_INDR, ctdiv04 },
	{ SU_ADDR | T_UNSN, SU_EASY | T_ANY, ctdiv05 },
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctdiv06 },
	{ SU_ANY | T_LONG, SU_EASY | T_ANY, ctdiv07 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctdiv08 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctdiv09 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctdiv10 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctmul07 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctbop12 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_FLOAT, ctbop12 },
	{ 0, 0, NULL }
};

static char const ctldiv1[] = {
	RIGHT, 0,
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static char const ctldiv2[] = {
	RIGHT, S_NEXT | S_INDR,
	MOV, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static char const ctldiv3[] = {
	RIGHT, S_NEXT,
	EXRLN,
	MOVL, ' ', NR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static struct skeleton const fr_ldiv[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctdiv00 },
	{ SU_ANY | T_ANY, SU_ADDR | T_INT, ctdiv02 },
	{ SU_ANY | T_LONG, SU_ADDR | T_INT, ctldiv1 },
	{ SU_ADDR | T_UNSN, SU_EASY | T_INT | T_INDR, ctdiv03 },
	{ SU_ANY | T_ANY, SU_EASY | T_INT | T_INDR, ctdiv04 },
	{ SU_ANY | T_LONG, SU_EASY | T_INT | T_INDR, ctldiv2 },
	{ SU_ADDR | T_UNSN, SU_EASY | T_ANY, ctdiv05 },
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctdiv06 },
	{ SU_ANY | T_LONG, SU_EASY | T_ANY, ctldiv3 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctmul05 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctdiv09 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctmul06 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctmul07 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctbop12 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_FLOAT, ctbop12 },
	{ 0, 0, NULL }
};

static char const ctreo01[] = {
	OP, TLEFT, ' ', RADDR, ',', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

static char const ctreo02[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo24[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo03[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo04[] = {
	RIGHT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctreo26[] = {
	RIGHT, S_INDR,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo05[] = {
	RIGHT, S_INDR,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo06[] = {
	RIGHT, S_STACK,
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctreo07[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	RIGHT, S_NEXT,
	EXRLN,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo27[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXLR,
	RIGHT, S_NEXT,
	OP, TEITHER, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo08[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	RIGHT, S_NEXT,
	EXRLN,
	OP, TEITHER, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo09[] = {
	RIGHT, S_STACK,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctreo28[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo10[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo11[] = {
	RIGHT, 0,
	EXRL,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	LEFT, 0,
	OP, TLEFT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ctreo12[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctreo13[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo14[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

static char const ctreo15[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo16[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	EXRLN,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ct35o[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	CLR, TRIGHT, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo17[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ct37o[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	CLR, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo18[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo19[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP8, MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo20[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ct41o[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo21[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo22[] = {
	RIGHT, 0,
	EXRL,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctreo23[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_eqop[] = {
	{ SU_ANY | T_FLOAT | T_INDR, SU_ONE | T_ANY, ctpst03 },
	{ SU_ANY | T_FLOAT, SU_ONE | T_ANY, ctpst02 },
	{ SU_ADDR | T_ANY, SU_ZERO | T_ANY, ctreo01 },
	{ SU_ANY | T_LONG, SU_ZERO | T_ANY, ctreo01 },
	{ SU_ADDR | T_INT, SU_CONST | T_INT, ctreo01 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctreo01 },
	{ SU_ADDR | T_UNSN, SU_ADDR | T_LONG, ctreo02 },
	{ SU_ADDR | T_CHAR, SU_CONST | T_INT, ctreo24 },
	{ SU_ADDR | T_CHAR, SU_ADDR | T_INT, ctreo24 },
	{ SU_ADDR | T_CHAR, SU_ADDR | T_LONG, ctreo24 },
	{ SU_ADDR | T_CHAR, SU_CONST | T_INT, ctreo03 },
	{ SU_ADDR | T_INT, SU_ADDR | T_INT, ctreo03 },
	{ SU_ADDR | T_INT, SU_ADDR | T_LONG, ctreo03 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_LONG, ctreo03 },
	{ SU_ADDR | T_UNSN, SU_ANY | T_LONG | T_INDR, ctreo04 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_INT | T_INDR, ctreo26 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_LONG | T_INDR, ctreo26 },
	{ SU_ADDR | T_INT, SU_ANY | T_INT | T_INDR, ctreo05 },
	{ SU_ADDR | T_INT, SU_ANY | T_LONG | T_INDR, ctreo05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG | T_INDR, ctreo05 },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_FLOAT, ctreo06 },
	{ SU_ADDR | T_UNSN, SU_EASY | T_LONG, ctreo07 },
	{ SU_ADDR | T_CHAR, SU_EASY | T_ANY, ctreo27 },
	{ SU_ADDR | T_CHAR, SU_EASY | T_LONG, ctreo27 },
	{ SU_ADDR | T_INT, SU_EASY | T_ANY, ctreo08 },
	{ SU_ADDR | T_INT, SU_EASY | T_LONG, ctreo08 },
	{ SU_ADDR | T_LONG, SU_EASY | T_ANY, ctreo08 },
	{ SU_ADDR | T_LONG, SU_EASY | T_LONG, ctreo08 },
	{ SU_ADDR | T_UNSN, SU_ANY | T_LONG, ctreo09 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_INT, ctreo28 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_LONG, ctreo28 },
	{ SU_ADDR | T_INT, SU_ANY | T_INT, ctreo10 },
	{ SU_ADDR | T_INT, SU_ANY | T_LONG, ctreo10 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctreo10 },
	{ SU_ADDR | T_INT, SU_ANY | T_CHAR, ctreo11 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctreo11 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_CHAR, ctreo12 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ADDR | T_INT, ctreo13 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ADDR | T_LONG, ctreo13 },
	{ SU_ANY | T_INT | T_INDR, SU_ZERO | T_ANY, ctreo14 },
	{ SU_ANY | T_LONG | T_INDR, SU_ZERO | T_ANY, ctreo14 },
	{ SU_ANY | T_INT | T_INDR, SU_CONST | T_INT, ctreo14 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctreo14 },
	{ SU_ANY | T_INT | T_INDR, SU_ADDR | T_INT, ctreo15 },
	{ SU_ANY | T_INT | T_INDR, SU_ADDR | T_LONG, ctreo15 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_LONG, ctreo15 },
	{ SU_ANY | T_INT | T_INDR, SU_EASY | T_ANY, ctreo16 },
	{ SU_ANY | T_INT | T_INDR, SU_EASY | T_LONG, ctreo16 },
	{ SU_ANY | T_LONG | T_INDR, SU_EASY | T_ANY, ctreo16 },
	{ SU_ANY | T_LONG | T_INDR, SU_EASY | T_LONG, ctreo16 },
	{ SU_ANY | T_UCHAR | T_INDR, SU_EASY | T_INT, ct35o },
	{ SU_ANY | T_UCHAR | T_INDR, SU_EASY | T_LONG, ct35o },
	{ SU_ANY | T_CHAR | T_INDR, SU_EASY | T_INT, ctreo17 },
	{ SU_ANY | T_CHAR | T_INDR, SU_EASY | T_LONG, ctreo17 },
	{ SU_ANY | T_UCHAR | T_INDR, SU_EASY | T_CHAR, ct37o },
	{ SU_ANY | T_CHAR | T_INDR, SU_EASY | T_CHAR, ctreo18 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_FLOAT, ctreo19 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_INT, ctreo20 },
	{ SU_ANY | T_UCHAR | T_INDR, SU_ANY | T_INT, ct41o },
	{ SU_ANY | T_UCHAR | T_INDR, SU_ANY | T_LONG, ct41o },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_INT, ctreo21 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_LONG, ctreo21 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctreo22 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_CHAR, ctreo23 },
	{ 0, 0, NULL }
};

static char const cteop01[] = {
	RIGHT, S_INDR,
	MOV, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const cteop02[] = {
	RIGHT, S_INDR,
	LEFT, S_NEXT | S_INDR,
	MOV, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

static char const cteop03[] = {
	RIGHT, S_INDR,
	MOV, TLEFT, ' ', LADDR, '-', ',', NR, '\n',
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', NR, '\n',
	MOV, TLEFT, ' ', NR, ',', LADDR, '+', '\n',
	0
};

static char const cteop04[] = {
	RIGHT, 0,
	MOV, TLEFT, ' ', LADDR, '-', ',', NR, '\n',
	OP, TLEFT, ' ', CR, ',', NR, '\n',
	MOV, TLEFT, ' ', NR, ',', LADDR, '+', '\n',
	0
};

static char const cteop05[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const cteop06[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const cteop07[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const cteop08[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TRIGHT, ' ', POP, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const cteop09[] = {
	LEFT, S_INDR,
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	EXRL,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const cteop10[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, ' ', POP, ',', CR, '\n',
	EXRL,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fe_eqop[] = {
	{ SU_ADDR | T_ANY, SU_ZERO | T_ANY, ctasg09 },
	{ SU_ANY | T_LONG, SU_ZERO | T_ANY, ctasg09 },
	{ SU_ADDR | T_ANY, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctasg09 },
	{ SU_ANY | T_ANY | T_INDR, SU_ZERO | T_ANY, ctasg19 },
	{ SU_ANY | T_LONG | T_INDR, SU_ZERO | T_ANY, ctasg19 },
	{ SU_ANY | T_ANY | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_ADDR | T_LONG, SU_AREG | T_LONG, ctasg02 },
	{ SU_ADDR | T_INT, SU_REG | T_INT, ctasg09 },
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_REG | T_LONG, ctasg09 },
	{ SU_REG | T_LONG, SU_ADDR | T_LONG, ctasg09 },
	{ SU_AREG | T_LONG, SU_REG | T_INT, ctasg22 },
	{ SU_ADDR | T_INT, SU_ADDR | T_INT, ctasg02 },
	{ SU_ADDR | T_CHAR, SU_ADDR | T_CHAR, ctasg02 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_LONG, ctasg02 },
	{ SU_ADDR | T_INT, SU_ANY | T_INT | T_INDR, cteop01 },
	{ SU_ADDR | T_INT, SU_ANY | T_ANY, ctasg02 },
	{ SU_EASY | T_INT | T_INDR, SU_ANY | T_INT | T_INDR, cteop02 },
	{ SU_EASY | T_LONG | T_INDR, SU_ANY | T_LONG | T_INDR, cteop02 },
	{ SU_ADDR | T_ANY, SU_ANY | T_INT | T_INDR, cteop03 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, cteop04 },
	{ SU_EASY | T_INT | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_ANY, ctasg06 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, cteop05 },
	{ SU_REG | T_LONG, SU_ADDR | T_UNSN, cteop06 },
	{ SU_REG | T_LONG, SU_ADDR | T_ANY, cteop07 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctasg02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG | T_INDR, cteop01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctasg02 },
	{ SU_EASY | T_LONG | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, cteop08 },
	{ SU_ANY | T_LONG | T_INDR, SU_ADDR | T_ANY, cteop09 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, cteop10 },
	{ 0, 0, NULL }
};

static struct skeleton const fr_eqmult[] = {
	{ SU_ADDR | T_CHAR, SU_ADDR | T_INT, ctreo24 },
	{ SU_ADDR | T_INT, SU_ADDR | T_INT, ctreo03 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_INT | T_INDR, ctreo26 },
	{ SU_ADDR | T_INT, SU_ANY | T_INT | T_INDR, ctreo05 },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_FLOAT, ctreo06 },
	{ SU_ADDR | T_CHAR, SU_EASY | T_ANY, ctreo27 },
	{ SU_ADDR | T_INT, SU_EASY | T_ANY, ctreo08 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_INT, ctreo28 },
	{ SU_ADDR | T_INT, SU_ANY | T_INT, ctreo10 },
	{ SU_ADDR | T_INT, SU_ANY | T_CHAR, ctreo11 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_CHAR, ctreo12 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ADDR | T_INT, ctreo13 },
	{ SU_ANY | T_INT | T_INDR, SU_ADDR | T_INT, ctreo15 },
	{ SU_ANY | T_INT | T_INDR, SU_EASY | T_ANY, ctreo16 },
	{ SU_ANY | T_CHAR | T_INDR, SU_EASY | T_INT, ctreo17 },
	{ SU_ANY | T_CHAR | T_INDR, SU_EASY | T_CHAR, ctreo18 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_FLOAT, ctreo19 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_INT, ctreo20 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_INT, ctreo21 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_CHAR, ctreo23 },
	{ 0, 0, NULL }
};

static char const ct89g[] = {
	AOP, ' ', RADDR, ',', LADDR, '\n',
	0
};

static struct skeleton const fe_eqmult[] = {
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctasg09 },
	{ SU_REG | T_UNSN, SU_ADDR | T_INT, ct89g },
	{ SU_REG | T_UNSN, SU_ADDR | T_UNSN, ct89g },
	{ 0, 0, NULL }
};

static char const ctedv01[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctedv07[] = {
	EXTW, ' ', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	0
};

static char const ctedv08[] = {
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	0
};

static struct skeleton const fe_eqdiv[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctedv01 },
	{ SU_REG | T_CHAR, SU_ADDR | T_INT, ctedv07 },
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctedv08 },
	{ 0, 0, NULL }
};

static char const ctedv10[] = {
	EXTW, ' ', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	's', 'w', 'a', 'p', ' ', LADDR, '\n',
	0
};

static char const ctedv11[] = {
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	's', 'w', 'a', 'p', ' ', LADDR, '\n',
	0
};

static struct skeleton const fe_eqmod[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctedv01 },
	{ SU_REG | T_CHAR, SU_ADDR | T_INT, ctedv10 },
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctedv11 },
	{ 0, 0, NULL }
};

static char const ctedv12[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXL,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctedv02[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXL,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctedv03[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctedv04[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, EXRL,
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ct01v[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctedv05[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXL,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, EXRL,
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctedv06[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXL,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, EXRL,
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_eqdiv[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctedv01 },
	{ SU_ADDR | T_CHAR, SU_ADDR | T_INT, ctedv12 },
	{ SU_ADDR | T_INT, SU_ADDR | T_INT, ctedv02 },
	{ SU_ADDR | T_UNSN, SU_EASY | T_ANY, ctedv03 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_INT, ctedv04 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_ANY, ct01v },
	{ SU_ADDR | T_INT, SU_ANY | T_ANY, ctedv05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctedv05 },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_FLOAT, ctreo06 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctedv06 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctedv06 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_FLOAT, ctreo19 },
	{ 0, 0, NULL }
};

static char const ctexo01[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

static char const ctexo02[] = {
	LEFT, S_STACK | S_INDR,
	RIGHT, 0,
	EXRL,
	MOVL, ' ', POP, ',', CAR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

static struct skeleton const fr_eqxor[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctexo01 },
	{ SU_ADDR | T_ANY, SU_ANY | T_LONG, ctexo01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctexo01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctexo01 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctexo02 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_LONG, ctexo02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctexo02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, ctexo02 },
	{ 0, 0, NULL }
};

static char const ctexo05[] = {
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static struct skeleton const fe_eqxor[] = {
	{ SU_ADDR | T_ANY, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctasg02 },
	{ SU_ADDR | T_ANY, SU_ANY | T_LONG, ctasg02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctasg02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctasg02 },
	{ SU_ANY | T_ANY | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_ANY | T_ANY | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_ANY | T_LONG | T_INDR, SU_EASY | T_ANY, ctasg05 },
	{ SU_ANY | T_LONG | T_INDR, SU_EASY | T_LONG, ctasg05 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctasg06 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_ADDR | T_LONG, ctexo05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG | T_INDR, cteop01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctasg02 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_EASY | T_LONG | T_INDR, SU_ANY | T_LONG, ctasg05 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, cteop08 },
	{ 0, 0, NULL }
};

static char const ctesh01[] = {
	OP, TLEFT, ' ', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

static char const ctesh02[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

static char const ctesh03[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctesh04[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctesh11[] = {
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctesh05[] = {
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

static char const ctesh12[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctesh06[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctesh13[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctesh07[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_eqshft[] = {
	{ SU_ADDR | T_INT, SU_ONE | T_ANY, ctesh01 },
	{ SU_ANY | T_INT | T_INDR, SU_ONE | T_ANY, ctesh02 },
	{ SU_ADDR | T_CHAR, SU_SMALL | T_INT, ctesh03 },
	{ SU_ADDR | T_INT, SU_SMALL | T_INT, ctesh04 },
	{ SU_ADDR | T_LONG, SU_SMALL | T_INT, ctesh04 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_ANY, ctesh11 },
	{ SU_ADDR | T_CHAR, SU_ANY | T_LONG, ctesh11 },
	{ SU_ADDR | T_INT, SU_ANY | T_ANY, ctesh05 },
	{ SU_ADDR | T_INT, SU_ANY | T_LONG, ctesh05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctesh05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, ctesh05 },
	{ SU_ANY | T_CHAR | T_INDR, SU_SMALL | T_INT, ctesh12 },
	{ SU_ANY | T_INT | T_INDR, SU_SMALL | T_INT, ctesh06 },
	{ SU_ANY | T_LONG | T_INDR, SU_SMALL | T_INT, ctesh06 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_ANY, ctesh13 },
	{ SU_ANY | T_CHAR | T_INDR, SU_ANY | T_LONG, ctesh13 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_ANY, ctesh07 },
	{ SU_ANY | T_INT | T_INDR, SU_ANY | T_LONG, ctesh07 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctesh07 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, ctesh07 },
	{ 0, 0, NULL }
};

static char const ctesh08[] = {
	OP, TLEFT, ' ', LADDR, '\n',
	0
};

static char const ctesh09[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

static char const ctesh10[] = {
	RIGHT, 0,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static struct skeleton const fe_eqshft[] = {
	{ SU_ADDR | T_INT, SU_ONE | T_ANY, ctesh08 },
	{ SU_ANY | T_INT | T_INDR, SU_ONE | T_ANY, ctesh09 },
	{ SU_REG | T_ANY, SU_SMALL | T_INT, ctasg09 },
	{ SU_REG | T_LONG, SU_SMALL | T_INT, ctasg09 },
	{ SU_REG | T_ANY, SU_ANY | T_ANY, ctesh10 },
	{ SU_REG | T_LONG, SU_ANY | T_ANY, ctesh10 },
	{ 0, 0, NULL }
};

static char const ctitl01[] = {
	CLRL, ' ', CR, '\n',
	0
};

static char const ctitl02[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	0
};

static char const ctitl03[] = {
	MOVL, ' ', LADDR, ',', CR, '\n',
	0
};

static char const ctitl04[] = {
	LEFT, 0,
	EXL,
	0
};

static struct skeleton const fr_itl[] = {
	{ SU_ZERO | T_ANY, SU_ANY | T_ANY, ctitl01 },
	{ SU_ADDR | T_UNSN, SU_ANY | T_ANY, ctitl02 },
	{ SU_CONST | T_INT, SU_ANY | T_ANY, ctitl03 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctitl04 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctshft00 },
	{ 0, 0, NULL }
};

static char const ctitl05[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	MOVL, ' ', CR, ',', PSH, '\n',
	0
};

static char const ctitl06[] = {
	LEFT, 0,
	EXL,
	MOVL, ' ', CR, ',', PSH, '\n',
	0
};

static char const ctitl07[] = {
	MOV, ' ', LADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', PSH, '\n',
	0
};

static char const ct42l[] = {
	LEFT, S_STACK,
	0
};

static struct skeleton const fs_itl[] = {
	{ SU_ADDR | T_UNSN, SU_ANY | T_ANY, ctitl05 },
	{ SU_ANY | T_UNSN, SU_ANY | T_ANY, ctitl06 },
	{ SU_ADDR | T_INT, SU_ANY | T_ANY, ctitl07 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctitl06 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ct42l },
	{ 0, 0, NULL }
};

static struct skeleton const fr_lti[] = {
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctitl03 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctshft00 },
	{ 0, 0, NULL }
};

static struct skeleton const fr_ftol[] = {
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

static struct skeleton const fr_ftoi[] = {
	{ SU_EASY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ SU_EASY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctneg03 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

static struct skeleton const fr_ltof[] = {
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

static char const ct48g[] = {
	LEFT, 0,
	EXL,
	MOVL, ' ', CR, ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP4, 0
};

static struct skeleton const fr_itof[] = {
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ct48g },
	{ 0, 0, NULL }
};

static char const cttoc01[] = {
	LEFT, 0,
	'a', 'n', 'd', ' ', '#', '$', 'f', 'f', ',', CR, '\n',
	0
};

static char const cttoc02[] = {
	LEFT, 0,
	EXTW, ' ', CR, '\n',
	0
};

static struct skeleton const fr_tochar[] = {
	{ SU_ANY | T_UNSN, SU_ANY | T_ANY, cttoc01 },
	{ SU_ANY | T_ULONG, SU_ANY | T_ANY, cttoc01 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, cttoc02 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, cttoc02 },
	{ 0, 0, NULL }
};

static char const ctrel01[] = {
	LEFT, S_FORCC,
	0
};

static char const ctrel02[] = {
	RIGHT, S_INDR,
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', LADDR, '\n',
	0
};

static char const ctrel03[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ct66p[] = {
	LEFT, 0,
	CLRL, ' ', NR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', NR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	0
};

static char const ctrel05[] = {
	LEFT, 0,
	MOV, ' ', RADDR, ',', CAR, '\n',
	OP, TLEFT, ' ', CAR, ',', CR, '\n',
	0
};

static char const ct72p[] = {
	RIGHT, S_STACK,
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

static struct skeleton const fc_rel[] = {
	{ SU_CONST | T_INT, SU_ZERO | T_ANY, ctshft00 },
	{ SU_CONST | T_INT, SU_CONST | T_INT, ctbop05 },
	{ SU_AREG | T_LONG, SU_ZERO | T_ANY, ctlod03 },
	{ SU_ADDR | T_ANY, SU_ZERO | T_ANY, ctasg07 },
	{ SU_ADDR | T_LONG, SU_ZERO | T_ANY, ctasg07 },
	{ SU_ADDR | T_FLOAT, SU_ZERO | T_ANY, ctasg07 },
	{ SU_ANY | T_ANY | T_INDR, SU_ZERO | T_ANY, ctasg08 },
	{ SU_ANY | T_LONG | T_INDR, SU_ZERO | T_ANY, ctasg08 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ZERO | T_ANY, ctasg08 },
	{ SU_ANY | T_ANY, SU_ZERO | T_ANY, ctrel01 },
	{ SU_ANY | T_LONG, SU_ZERO | T_ANY, ctrel01 },
	{ SU_ANY | T_FLOAT, SU_ZERO | T_ANY, ctrel01 },
	{ SU_ADDR | T_ANY, SU_CONST | T_INT, ctasg09 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctasg09 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_ANY | T_ANY | T_INDR, SU_CONST | T_INT, ctasg19 },
	{ SU_AREG | T_LONG, SU_ADDR | T_INT, ctasg22 },
	{ SU_REG | T_CHAR, SU_ADDR | T_CHAR, ctasg09 },
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctasg09 },
	{ SU_REG | T_LONG, SU_ADDR | T_LONG, ctasg09 },
	{ SU_REG | T_INT, SU_ANY | T_CHAR, ctasg10 },
	{ SU_REG | T_INT, SU_ANY | T_INT | T_INDR, ctrel02 },
	{ SU_REG | T_LONG, SU_ANY | T_LONG | T_INDR, ctrel02 },
	{ SU_AREG | T_LONG, SU_ANY | T_INT | T_INDR, ctrel02 },
	{ SU_REG | T_INT, SU_EASY | T_ANY, ctrel03 },
	{ SU_REG | T_LONG, SU_EASY | T_LONG, ctrel03 },
	{ SU_REG | T_LONG, SU_EASY | T_ANY, ctrel03 },
	{ SU_ANY | T_INT, SU_ADDR | T_INT, ctbop05 },
	{ SU_ANY | T_CHAR, SU_ADDR | T_CHAR, ctbop05 },
	{ SU_ANY | T_LONG, SU_ADDR | T_LONG, ctbop05 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctbop05 },
	{ SU_ANY | T_CHAR, SU_ADDR | T_INT, ctbop02 },
	{ SU_ANY | T_LONG, SU_ADDR | T_UNSN, ct66p },
	{ SU_ANY | T_LONG, SU_ADDR | T_INT, ctrel05 },
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_ANY, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctbop07 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctbop08 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctbop08 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctbop09 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_FLOAT, ct72p },
	{ 0, 0, NULL }
};

static char const ctbtt01[] = {
	OP, ' ', RADDR, ',', LADDR, '\n',
	0
};

static char const ctbtt02[] = {
	LEFT, S_INDR,
	OP, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fc_btst[] = {
	{ SU_ADDR | T_ANY, SU_CONST | T_INT, ctbtt01 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctbtt01 },
	{ SU_ANY | T_ANY | T_INDR, SU_CONST | T_INT, ctbtt02 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctbtt02 },
	{ 0, 0, NULL }
};

static char const ctfix01[] = {
	TREE, 0,
	0
};

static struct skeleton const fc_fix[] = {
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctfix01 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctfix01 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctfix01 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctfix01 },
	{ 0, 0, NULL }
};

static char const ct76x[] = {
	OP, TLEFT, ' ', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

static char const ct77x[] = {
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP4, MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

static char const ct78x[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

static char const ct79x[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP4, MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fr_eqnot[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ct76x },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ct76x },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_ANY, ct77x },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ct78x },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ct78x },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ct79x },
	{ 0, 0, NULL }
};

static char const ct80x[] = {
	OP, TLEFT, ' ', LADDR, '\n',
	0
};

static char const ct81x[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

static struct skeleton const fe_eqnot[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ct80x },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ct80x },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ct81x },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ct81x },
	{ 0, 0, NULL }
};

static char const ct82x[] = {
	RIGHT, 0,
	'a', 'n', 'd', ' ', '#', '$', 'f', 'f', ',', CR, '\n',
	0
};

static char const ct83x[] = {
	RIGHT, 0,
	EXTW, ' ', CR, '\n',
	0
};

static char const ct84x[] = {
	RIGHT, 0,
	0
};

static struct skeleton const fr_docast[] = {
	{ SU_ANY | T_UCHAR, SU_ANY | T_INT, ct82x },
	{ SU_ANY | T_UCHAR, SU_ANY | T_LONG, ct82x },
	{ SU_ANY | T_CHAR, SU_ANY | T_INT, ct83x },
	{ SU_ANY | T_CHAR, SU_ANY | T_LONG, ct83x },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ct84x },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ct84x },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ct84x },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ct84x },
	{ 0, 0, NULL }
};

static char const ct85x[] = {
	RIGHT, 0,
	EXRL,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	0
};

static struct skeleton const fs_docast[] = {
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ct85x },
	{ 0, 0, NULL }
};


const struct skeleton *const codeskels[] = {
	0,									/* NULL */
	fe_eqop,							/* 1=FE_EQOP */
	fe_assign,							/* 2=FE_ASSIGN */
	fe_eqshft,							/* 3=FE_EQSHFT */
	fe_eqxor,							/* 4=FE_EQXOR */
	fe_eqaddr,							/* 5=FE_EQADDR */
	fc_fix,								/* 6=FC_FIX */
	fc_rel,								/* 7=FC_REL */
	fc_btst,							/* 8=FC_BTST */
	fs_op,								/* 9=FS_OP */
	fs_itl,								/* 10=FS_ITL */
	fs_ld,								/* 11=FS_LD */
	fr_add,								/* 12=FR_ADD */
	fr_mult,							/* 13=FR_MULT */
	fr_div,								/* 14=FR_DIV */
	fr_shft,							/* 15=FR_SHFT */
	fr_xor,								/* 16=FR_XOR */
	fr_neg,								/* 17=FR_NEG */
	fr_eqop,							/* 18=FR_EQOP */
	fr_postop,							/* 19=FR_POSTOP */
	fr_assign,							/* 20=FR_ASSIGN */
	fr_eqmult,							/* 21=FR_EQMULT */
	fr_eqdiv,							/* 22=FR_EQDIV */
	fr_eqshft,							/* 23=FR_EQSHFT */
	fr_eqxor,							/* 24=FR_EQXOR */
	fr_call,							/* 25=FR_CALL */
	fr_itl,								/* 26=FR_ITL */
	fr_lti,								/* 27=FR_LTI */
	fr_ld,								/* 28=FR_LD */
	fr_eqaddr,							/* 29=FE_EQADDR */
	fr_eqnot,							/* 30=FE_EQNOT */
	fe_eqnot,							/* 31=FE_EQNOT */
	fr_docast,							/* 32=FE_DOCAST */
	fs_docast,							/* 33=FS_DOCAST */
	fr_ftol,							/* 34=FR_FTOL */
	fr_ltof,							/* 35=FR_LTOF */
	fr_ftoi,							/* 36=FR_FTOI */
	fr_itof,							/* 37=FR_ITOF */
	fe_eqmult,							/* 38=FE_EQMULT */
	fe_eqdiv,							/* 39=FE_EQDIV */
	fe_eqmod,							/* 40=FE_EQMOD */
	fr_tochar,							/* 41=FR_TOCHAR */
	fr_ldiv								/* 42=FR_LDIV */
};
