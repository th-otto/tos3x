#include "cgen.h"

char ctnull[] = { 0 };

char cteqa01[] = {
	LEA, ' ', RADDR, ',', LADDR, '-', '\n',
	MOVL, ' ', LADDR, '+', ',', CR, '\n',
	0
};

char cteqa02[] = {
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LADDR, '\n',
	MOVL, ' ', CAR, ',', CR, '\n',
	0
};

char cteqa03[] = {
	LEFT, S_NEXT | S_INDR,
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LOFFSET, '(', NAR, ')', '\n',
	MOVL, ' ', CAR, ',', CR, '\n',
	0
};

struct skeleton fr_eqaddr[] = {
	{ SU_AREG | T_LONG, SU_ANY | T_ANY, cteqa01 },
	{ SU_AREG | T_LONG, SU_ANY | T_LONG, cteqa01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, cteqa02 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, cteqa02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, cteqa03 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, cteqa03 },
	{ 0, 0, NULL }
};

char cteqa04[] = {
	LEA, ' ', RADDR, ',', LADDR, '\n',
	0
};

char cteqa05[] = {
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LADDR, '\n',
	0
};

char cteqa06[] = {
	LEFT, S_NEXT | S_INDR,
	LEA, ' ', RADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

struct skeleton fe_eqaddr[] = {
	{ SU_AREG | T_LONG, SU_ANY | T_ANY, cteqa04 },
	{ SU_AREG | T_LONG, SU_ANY | T_LONG, cteqa04 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, cteqa05 },
	{ SU_ADDR | T_LONG, SU_ANY | T_LONG, cteqa05 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, cteqa06 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_LONG, cteqa06 },
	{ 0, 0, NULL }
};

char ctcal01[] = {
	JSR, ' ', LADDR, '\n',
	0
};

char ctcal02[] = {
	LEFT, S_INDR,
	JSR, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctcal03[] = {
	LEFT, 0,
	MOV, TLEFT, ' ', CR, ',', CAR, '\n',
	JSR, ' ', '(', CAR, ')', '\n',
	0
};

struct skeleton fr_call[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctcal01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctcal01 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctcal02 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctcal02 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctcal03 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctcal03 },
	{ 0, 0, NULL }
};

char ctlod00[] = {
	CLR, ' ', CR, '\n',
	0
};

char ctlod01[] = {
	CLR, TLEFT, ' ', CR, '\n',
	0
};

char ct11d[] = {
	CLR, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	0
};

char ctlod02[] = {
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	0
};

char ctlod03[] = {
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	0
};

char ct14d[] = {
	LEFT, S_INDR,
	CLR, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

char ctlod04[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	0
};

char ctlod05[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

struct skeleton fr_ld[] = {
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

char ct17d[] = {
	CLR, ' ', PSH, '\n',
	0
};

char ctlod06[] = {
	CLR, TLEFT, ' ', PSH, '\n',
	0
};

char ctlod07[] = {
	LEFT, 0,
	MOV, ' ', CR, ',', PSH, ' ', '\n',
	0
};

char ctlod08[] = {
	MOV, TLEFT, ' ', LADDR, ',', PSH, '\n',
	0
};

char ctlod09[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', PSH, '\n',
	0
};

struct skeleton fs_ld[] = {
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

char ctpst03[] = {
	MOVL, ' ', '#', '1', ',', PSH, '\n',
	JSR, ' ', '_', 'f', 'p', 'l', 't', 'o', 'f', '\n',
	POP4, MOVL, ' ', 'D', '0', ',', PSH, '\n',
	LEFT, S_INDR,
	MOVL, ' ', LOFFSET, '(', CAR, ')', ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP8, MOVL, ' ', 'D', '0', ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctpst02[] = {
	MOVL, ' ', '#', '1', ',', PSH, '\n',
	JSR, ' ', '_', 'f', 'p', 'l', 't', 'o', 'f', '\n',
	POP4, MOVL, ' ', 'D', '0', ',', PSH, '\n',
	MOVL, ' ', LADDR, '-', ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP8, MOVL, ' ', 'D', '0', ',', LADDR, '+', '\n',
	0
};

char ctpst01[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', LADDR, '+', '\n',
	0
};

char ctpst04[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fr_postop[] = {
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctpst03 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctpst02 },
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ctpst01 },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctpst01 },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ctpst04 },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ctpst04 },
	{ 0, 0, NULL }
};

char ctneg01[] = {
	LEFT, 0,
	OP, ' ', CR, '\n',
	0
};

char ctneg02[] = {
	LEFT, 0,
	OP, TLEFT, ' ', CR, '\n',
	0
};

char ctneg03[] = {
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP4,
	0
};

struct skeleton fr_neg[] = {
	{ SU_ANY | T_CHAR, SU_ANY | T_ANY, ctneg01 },
	{ SU_ANY | T_INT, SU_ANY | T_ANY, ctneg02 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctneg02 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

char ctasg01[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctasg02[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctasg03[] = {
	LEFT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', LOFFSET, '(', NAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

char ctasg04[] = {
	LEFT, S_INDR,
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	EXRL,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctasg05[] = {
	RIGHT, 0,
	EXRL,
	LEFT, S_NEXT | S_INDR,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

char ctasg06[] = {
	LEFT, S_STACK | S_INDR,
	RIGHT, 0,
	EXRL,
	MOVL, ' ', POP, ',', CAR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fr_assign[] = {
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

char ctasg07[] = {
	AOP, TLEFT, ' ', LADDR, '\n',
	0
};

char ctasg08[] = {
	LEFT, S_INDR,
	AOP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctasg09[] = {
	OP, TLEFT, ' ', RADDR, ',', LADDR, '\n',
	0
};

char ctasg0a[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOVL, ' ', CR, ',', LADDR, '\n',
	0
};

char ctasg10[] = {
	RIGHT, 0,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctasg0b[] = {
	CLR, TLEFT, ' ', LADDR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	0
};

char ctasg11[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	EXTW, ' ', LADDR, '\n',
	0
};

char ctasg12[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	EXTW, ' ', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	0
};

char ctasg13[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctasg22[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	0
};

char ctasg14[] = {
	OP, TRIGHT, ' ', RADDR, ',', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	0
};

char ctasg15[] = {
	MOV, ' ', RADDR, ',', CAR, '\n',
	OP, TLEFT, ' ', CAR, ',', LADDR, '\n',
	0
};

char ctasg16[] = {
	LEFT, S_INDR,
	CLRL, ' ', CR, '\n',
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctasg17[] = {
	RIGHT, S_INDR,
	OP, TLEFT, ' ', ROFFSET, '(', CAR, ')', ',', LADDR, '\n',
	0
};

char ctasg18[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctasg19[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctasg20[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT | S_INDR,
	OP, TLEFT, ' ', ROFFSET, '(', NAR, ')', ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctasg23[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	OP, TLEFT, ' ', POP, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fe_assign[] = {
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

char ctbop00[] = {
	LEFT, 0,
	CLR, ' ', CR, '\n',
	0
};

char ctbop01[] = {
	LEFT, 0,
	CLR, TLEFT, ' ', CR, '\n',
	0
};

char ctbop02[] = {
	LEFT, 0,
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	0
};

char ctbop03[] = {
	LEFT, 0,
	CLRL, ' ', NR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', NR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	0
};

char ctbop04[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	RIGHT, S_NEXT,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	0
};

char ctbop05[] = {
	LEFT, 0,
	OP, TLEFT, ' ', RADDR, ',', CR, '\n',
	0
};

char ctbop06[] = {
	LEFT, 0,
	EXLR,
	RIGHT, S_NEXT,
	EXRLN,
	OP, TEITHER, ' ', NR, ',', CR, '\n',
	0
};

char ctbop07[] = {
	RIGHT, 0,
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, 0,
	OP, TLEFT, ' ', POP, ',', CR, '\n',
	0
};

char ctbop08[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	0
};

char ctbop09[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	0
};

char ctbop12[] = {
	RIGHT, S_STACK,
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

struct skeleton fr_op[] = {
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

char ctbop10[] = {
	LEFT, S_STACK,
	OP, TLEFTL, ' ', RADDR, ',', STK, '\n',
	0
};

char ctbop11[] = {
	LEFT, S_STACK,
	RIGHT, 0,
	OP, TLEFTL, ' ', CR, ',', STK, '\n',
	0
};

struct skeleton fs_op[] = {
	{ SU_ANY | T_ANY, SU_CONST | T_INT, ctbop10 },
	{ SU_ANY | T_LONG, SU_CONST | T_INT, ctbop10 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctbop11 },
	{ 0, 0, NULL }
};

char ctxor01[] = {
	LEFT, S_STACK,
	RIGHT, 0,
	OP, ' ', CR, ',', STK, '\n',
	MOV, ' ', POP, ',', CR, '\n',
	0
};

char ct71r[] = {
	LEFT, S_STACK,
	RIGHT, 0,
	OP, TLEFT, ' ', CR, ',', STK, '\n',
	MOVL, ' ', POP, ',', CR, '\n',
	0
};

struct skeleton fr_xor[] = {
	{ SU_ANY | T_ANY, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_ANY, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_ANY, ctbop06 },
	{ SU_ANY | T_LONG, SU_EASY | T_LONG, ctbop06 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctxor01 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ct71r },
	{ 0, 0, NULL }
};

char ctshft00[] = {
	LEFT, 0,
	0
};

char ctshf01[] = {
	LEFT, 0,
	OP, TLEFTL, ' ', RADDR, ',', CR, '\n',
	0
};

char ctshf02[] = {
	LEFT, 0,
	RIGHT, S_NEXT,
	OP, TLEFTL, ' ', NR, ',', CR, '\n',
	0
};

struct skeleton fr_shft[] = {
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

char ctmul01[] = {
	LEFT, 0,
	OP, ' ', RADDR, ',', CR, '\n',
	0
};

char ctmul02[] = {
	LEFT, 0,
	RIGHT, S_NEXT | S_INDR,
	OP, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	0
};

char ctmul03[] = {
	LEFT, 0,
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	0
};

char ctmul04[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	0
};

char ctmul05[] = {
	MOVL, ' ', RADDR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

char ctmul06[] = {
	RIGHT, 0,
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

char ctmul07[] = {
	LEFT, 0,
	EXLR,
	MOVL, ' ', CR, ',', PSH, '\n',
	RIGHT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

struct skeleton fr_mult[] = {
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

char ctdiv00[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, 0
};

char ctdif01[] = {
	LEFT, 0,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv02[] = {
	LEFT, 0,
	EXL,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv03[] = {
	RIGHT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv4a[] = {
	LEFT, 0,
	RIGHT, S_NEXT | S_INDR,
	OP, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv04[] = {
	LEFT, 0,
	EXL,
	RIGHT, S_NEXT | S_INDR,
	OP, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv05[] = {
	CLRL, ' ', CR, '\n',
	MOV, ' ', LADDR, ',', CR, '\n',
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv06[] = {
	LEFT, 0,
	EXL,
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv07[] = {
	LEFT, 0,
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv08[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv09[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	EXL,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, 0
};

char ctdiv10[] = {
	RIGHT, S_STACK,
	LEFT, 0,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, 0
};

struct skeleton fr_div[] = {
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

char ctldiv1[] = {
	RIGHT, 0,
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

char ctldiv2[] = {
	RIGHT, S_NEXT | S_INDR,
	MOV, ' ', ROFFSET, '(', NAR, ')', ',', CR, '\n',
	EXRL,
	MOVL, ' ', CR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

char ctldiv3[] = {
	RIGHT, S_NEXT,
	EXRLN,
	MOVL, ' ', NR, ',', PSH, '\n',
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

struct skeleton fr_ldiv[] = {
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

char ctreo01[] = {
	OP, TLEFT, ' ', RADDR, ',', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

char ctreo02[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo24[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo03[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo04[] = {
	RIGHT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctreo26[] = {
	RIGHT, S_INDR,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo05[] = {
	RIGHT, S_INDR,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo06[] = {
	RIGHT, S_STACK,
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctreo07[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	RIGHT, S_NEXT,
	EXRLN,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo27[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXLR,
	RIGHT, S_NEXT,
	OP, TEITHER, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo08[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	RIGHT, S_NEXT,
	EXRLN,
	OP, TEITHER, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo09[] = {
	RIGHT, S_STACK,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctreo28[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo10[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo11[] = {
	RIGHT, 0,
	EXRL,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	LEFT, 0,
	OP, TLEFT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ctreo12[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctreo13[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo14[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

char ctreo15[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo16[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	EXRLN,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ct35o[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	CLR, TRIGHT, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo17[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ct37o[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	CLR, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo18[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo19[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP8, MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo20[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ct41o[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo21[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	EXLR,
	OP, TRIGHT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo22[] = {
	RIGHT, 0,
	EXRL,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctreo23[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fr_eqop[] = {
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

char cteop01[] = {
	RIGHT, S_INDR,
	MOV, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char cteop02[] = {
	RIGHT, S_INDR,
	LEFT, S_NEXT | S_INDR,
	MOV, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', NAR, ')', '\n',
	0
};

char cteop03[] = {
	RIGHT, S_INDR,
	MOV, TLEFT, ' ', LADDR, '-', ',', NR, '\n',
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', NR, '\n',
	MOV, TLEFT, ' ', NR, ',', LADDR, '+', '\n',
	0
};

char cteop04[] = {
	RIGHT, 0,
	MOV, TLEFT, ' ', LADDR, '-', ',', NR, '\n',
	OP, TLEFT, ' ', CR, ',', NR, '\n',
	MOV, TLEFT, ' ', NR, ',', LADDR, '+', '\n',
	0
};

char cteop05[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char cteop06[] = {
	CLRL, ' ', CR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char cteop07[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char cteop08[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TRIGHT, ' ', POP, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char cteop09[] = {
	LEFT, S_INDR,
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	EXRL,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char cteop10[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, ' ', POP, ',', CR, '\n',
	EXRL,
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fe_eqop[] = {
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

struct skeleton fr_eqmult[] = {
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

char ct89g[] = {
	AOP, ' ', RADDR, ',', LADDR, '\n',
	0
};

struct skeleton fe_eqmult[] = {
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctasg09 },
	{ SU_REG | T_UNSN, SU_ADDR | T_INT, ct89g },
	{ SU_REG | T_UNSN, SU_ADDR | T_UNSN, ct89g },
	{ 0, 0, NULL }
};

char ctedv01[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctedv07[] = {
	EXTW, ' ', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	0
};

char ctedv08[] = {
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	0
};

struct skeleton fe_eqdiv[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctedv01 },
	{ SU_REG | T_CHAR, SU_ADDR | T_INT, ctedv07 },
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctedv08 },
	{ 0, 0, NULL }
};

char ctedv10[] = {
	EXTW, ' ', LADDR, '\n',
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	's', 'w', 'a', 'p', ' ', LADDR, '\n',
	0
};

char ctedv11[] = {
	EXTL, ' ', LADDR, '\n',
	OP, ' ', RADDR, ',', LADDR, '\n',
	's', 'w', 'a', 'p', ' ', LADDR, '\n',
	0
};

struct skeleton fe_eqmod[] = {
	{ SU_ADDR | T_UNSN, SU_ADDR | T_INT, ctedv01 },
	{ SU_REG | T_CHAR, SU_ADDR | T_INT, ctedv10 },
	{ SU_REG | T_INT, SU_ADDR | T_INT, ctedv11 },
	{ 0, 0, NULL }
};

char ctedv12[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	EXL,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctedv02[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXL,
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctedv03[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	RIGHT, S_NEXT,
	OP, ' ', NR, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctedv04[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MODSWAP, EXRL,
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ct01v[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctedv05[] = {
	RIGHT, S_STACK,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXL,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, EXRL,
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctedv06[] = {
	RIGHT, S_STACK,
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXL,
	OP, ' ', POP, ',', CR, '\n',
	MODSWAP, EXRL,
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fr_eqdiv[] = {
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

char ctexo01[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

char ctexo02[] = {
	LEFT, S_STACK | S_INDR,
	RIGHT, 0,
	EXRL,
	MOVL, ' ', POP, ',', CAR, '\n',
	OP, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

struct skeleton fr_eqxor[] = {
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

char ctexo05[] = {
	MOV, TRIGHT, ' ', RADDR, ',', CR, '\n',
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

struct skeleton fe_eqxor[] = {
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

char ctesh01[] = {
	OP, TLEFT, ' ', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

char ctesh02[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

char ctesh03[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctesh04[] = {
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctesh11[] = {
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctesh05[] = {
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LADDR, '-', ',', CR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LADDR, '+', '\n',
	0
};

char ctesh12[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctesh06[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', RADDR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctesh13[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	EXTW, ' ', CR, '\n',
	OP, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctesh07[] = {
	LEFT, S_INDR,
	RIGHT, S_NEXT,
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fr_eqshft[] = {
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

char ctesh08[] = {
	OP, TLEFT, ' ', LADDR, '\n',
	0
};

char ctesh09[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

char ctesh10[] = {
	RIGHT, 0,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

struct skeleton fe_eqshft[] = {
	{ SU_ADDR | T_INT, SU_ONE | T_ANY, ctesh08 },
	{ SU_ANY | T_INT | T_INDR, SU_ONE | T_ANY, ctesh09 },
	{ SU_REG | T_ANY, SU_SMALL | T_INT, ctasg09 },
	{ SU_REG | T_LONG, SU_SMALL | T_INT, ctasg09 },
	{ SU_REG | T_ANY, SU_ANY | T_ANY, ctesh10 },
	{ SU_REG | T_LONG, SU_ANY | T_ANY, ctesh10 },
	{ 0, 0, NULL }
};

char ctitl01[] = {
	CLRL, ' ', CR, '\n',
	0
};

char ctitl02[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	0
};

char ctitl03[] = {
	MOVL, ' ', LADDR, ',', CR, '\n',
	0
};

char ctitl04[] = {
	LEFT, 0,
	EXL,
	0
};

struct skeleton fr_itl[] = {
	{ SU_ZERO | T_ANY, SU_ANY | T_ANY, ctitl01 },
	{ SU_ADDR | T_UNSN, SU_ANY | T_ANY, ctitl02 },
	{ SU_CONST | T_INT, SU_ANY | T_ANY, ctitl03 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctitl04 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctshft00 },
	{ 0, 0, NULL }
};

char ctitl05[] = {
	CLRL, ' ', CR, '\n',
	MOV, TLEFT, ' ', LADDR, ',', CR, '\n',
	MOVL, ' ', CR, ',', PSH, '\n',
	0
};

char ctitl06[] = {
	LEFT, 0,
	EXL,
	MOVL, ' ', CR, ',', PSH, '\n',
	0
};

char ctitl07[] = {
	MOV, ' ', LADDR, ',', CAR, '\n',
	MOVL, ' ', CAR, ',', PSH, '\n',
	0
};

char ct42l[] = {
	LEFT, S_STACK,
	0
};

struct skeleton fs_itl[] = {
	{ SU_ADDR | T_UNSN, SU_ANY | T_ANY, ctitl05 },
	{ SU_ANY | T_UNSN, SU_ANY | T_ANY, ctitl06 },
	{ SU_ADDR | T_INT, SU_ANY | T_ANY, ctitl07 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctitl06 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ct42l },
	{ 0, 0, NULL }
};

struct skeleton fr_lti[] = {
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ctitl03 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctshft00 },
	{ 0, 0, NULL }
};

struct skeleton fr_ftol[] = {
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

struct skeleton fr_ftoi[] = {
	{ SU_EASY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ SU_EASY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctneg03 },
	{ SU_ANY | T_FLOAT, SU_ANY | T_ANY, ctneg03 },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

struct skeleton fr_ltof[] = {
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctneg03 },
	{ 0, 0, NULL }
};

char ct48g[] = {
	LEFT, 0,
	EXL,
	MOVL, ' ', CR, ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP4, 0
};

struct skeleton fr_itof[] = {
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ct48g },
	{ 0, 0, NULL }
};

char cttoc01[] = {
	LEFT, 0,
	'a', 'n', 'd', ' ', '#', '$', 'f', 'f', ',', CR, '\n',
	0
};

char cttoc02[] = {
	LEFT, 0,
	EXTW, ' ', CR, '\n',
	0
};

struct skeleton fr_tochar[] = {
	{ SU_ANY | T_UNSN, SU_ANY | T_ANY, cttoc01 },
	{ SU_ANY | T_ULONG, SU_ANY | T_ANY, cttoc01 },
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, cttoc02 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, cttoc02 },
	{ 0, 0, NULL }
};

char ctrel01[] = {
	LEFT, S_FORCC,
	0
};

char ctrel02[] = {
	RIGHT, S_INDR,
	OP, TRIGHT, ' ', ROFFSET, '(', CAR, ')', ',', LADDR, '\n',
	0
};

char ctrel03[] = {
	RIGHT, 0,
	EXRL,
	OP, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ct66p[] = {
	LEFT, 0,
	CLRL, ' ', NR, '\n',
	MOV, TRIGHT, ' ', RADDR, ',', NR, '\n',
	OP, TLEFT, ' ', NR, ',', CR, '\n',
	0
};

char ctrel05[] = {
	LEFT, 0,
	MOV, ' ', RADDR, ',', CAR, '\n',
	OP, TLEFT, ' ', CAR, ',', CR, '\n',
	0
};

char ct72p[] = {
	RIGHT, S_STACK,
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP8, 0
};

struct skeleton fc_rel[] = {
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

char ctbtt01[] = {
	OP, ' ', RADDR, ',', LADDR, '\n',
	0
};

char ctbtt02[] = {
	LEFT, S_INDR,
	OP, ' ', RADDR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fc_btst[] = {
	{ SU_ADDR | T_ANY, SU_CONST | T_INT, ctbtt01 },
	{ SU_ADDR | T_LONG, SU_CONST | T_INT, ctbtt01 },
	{ SU_ANY | T_ANY | T_INDR, SU_CONST | T_INT, ctbtt02 },
	{ SU_ANY | T_LONG | T_INDR, SU_CONST | T_INT, ctbtt02 },
	{ 0, 0, NULL }
};

char ctfix01[] = {
	TREE, 0,
	0
};

struct skeleton fc_fix[] = {
	{ SU_ANY | T_ANY, SU_ANY | T_ANY, ctfix01 },
	{ SU_ANY | T_LONG, SU_ANY | T_LONG, ctfix01 },
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ctfix01 },
	{ SU_ANY | T_ANY, SU_ANY | T_LONG, ctfix01 },
	{ 0, 0, NULL }
};

char ct76x[] = {
	OP, TLEFT, ' ', LADDR, '-', '\n',
	MOV, TLEFT, ' ', LADDR, '+', ',', CR, '\n',
	0
};

char ct77x[] = {
	LEFT, S_STACK,
	JSR, ' ', OPCALL, '\n',
	POP4, MOV, TLEFT, ' ', CR, ',', LADDR, '\n',
	0
};

char ct78x[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	MOV, TLEFT, ' ', LOFFSET, '(', CAR, ')', ',', CR, '\n',
	0
};

char ct79x[] = {
	LEFT, S_INDR,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	JSR, ' ', OPCALL, '\n',
	POP4, MOV, TLEFT, ' ', CR, ',', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fr_eqnot[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ct76x },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ct76x },
	{ SU_ADDR | T_FLOAT, SU_ANY | T_ANY, ct77x },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ct78x },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ct78x },
	{ SU_ANY | T_FLOAT | T_INDR, SU_ANY | T_ANY, ct79x },
	{ 0, 0, NULL }
};

char ct80x[] = {
	OP, TLEFT, ' ', LADDR, '\n',
	0
};

char ct81x[] = {
	LEFT, S_INDR,
	OP, TLEFT, ' ', LOFFSET, '(', CAR, ')', '\n',
	0
};

struct skeleton fe_eqnot[] = {
	{ SU_ADDR | T_ANY, SU_ANY | T_ANY, ct80x },
	{ SU_ADDR | T_LONG, SU_ANY | T_ANY, ct80x },
	{ SU_ANY | T_ANY | T_INDR, SU_ANY | T_ANY, ct81x },
	{ SU_ANY | T_LONG | T_INDR, SU_ANY | T_ANY, ct81x },
	{ 0, 0, NULL }
};

char ct82x[] = {
	RIGHT, 0,
	'a', 'n', 'd', ' ', '#', '$', 'f', 'f', ',', CR, '\n',
	0
};

char ct83x[] = {
	RIGHT, 0,
	EXTW, ' ', CR, '\n',
	0
};

char ct84x[] = {
	RIGHT, 0,
	0
};

struct skeleton fr_docast[] = {
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

char ct85x[] = {
	RIGHT, 0,
	EXRL,
	MOV, TLEFT, ' ', CR, ',', PSH, '\n',
	0
};

struct skeleton fs_docast[] = {
	{ SU_ANY | T_LONG, SU_ANY | T_ANY, ct85x },
	{ 0, 0, NULL }
};
