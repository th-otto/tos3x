/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave
	San Diego, Ca.  92121
*/

	/*built-in literals*/
#define	MOV		128
#define	MOVL	129
#define	JSR		130
#define	CLR		131
#define	CLRL	132
#define	EXTW	133
#define	EXTL	134
#define	LEA		135
#define	STK		136

	/*built-in macros*/
#define	TREE	141
#define	LEFT	142
#define	RIGHT	143
#define	LOFFSET	144
#define	ROFFSET	145
#define	LADDR	146
#define	RADDR	147
#define	CR		148
#define	NR		149
#define	CAR		150
#define	NAR		151
#define	TLEFT	152
#define	TRIGHT	153
#define	TEITHER	154
#define	TLEFTL	155
#define	OP		156
#define	AOP		157
#define	MODSWAP	158
#define	EXL		159
#define	EXLR	160
#define	EXLRN	161
#define	EXRL	162
#define	EXRLN	163
#define	PSH		164
#define	POP		165
#define	POP8	166
#define	OPCALL	167
#define	POP4	169
#define	LADDRP	168

	/*modifiers for compiling sub-trees*/
#define	S_INDR		1		/*indirection*/
#define	S_STACK		2		/*onto stack*/
#define	S_FORCC		4		/*set condition codes*/
#define	S_NEXT		8		/*into next register*/

	/*Sethy-Ullman values*/
#define	SU_ZERO		0x000		/*zero*/
#define	SU_ONE		0x100		/*one*/
#define	SU_SMALL	0x200		/*constant between 1 and 8*/
#define	SU_QUICK	0x300		/*quick constant between -128 and 127*/
#define	SU_CONST	0x400		/*any constant*/
#define	SU_AREG		0x500		/*A register*/
#define	SU_REG		0x600		/*register*/
#define	SU_ADDR		0x700		/*addressable*/
#define	SU_XREG		0x800		/*A register used as data...*/
#define	SU_EASY		0x900		/*easy*/
#define	SU_HARD		0xa00		/*hard*/
#define	SU_VHARD	0xb00		/*very hard ... function calls, etc.*/
#define	SU_ANY		0xf00		/*anything*/
#define	ADDRESSABLE(x)		(x->t_su<=SU_ADDR)
#define	NOTADDRESSABLE(x)	(x->t_su>SU_ADDR)
#define	LOADABLE(x)			(x->t_su<=SU_XREG)

	/*flag byte (operand type):*/
#define	T_CHAR		1		/*char only*/
#define	T_SHORT		2		/*short*/
#define	T_INT		3		/*int only*/
#define	T_LONG		4		/*long*/
#define	T_UCHAR		5		/*unsigned char*/
#define	T_USHORT	6		/*unsigned short*/
#define	T_UNSN		7		/*unsigned int*/
#define	T_ULONG		8		/*unsigned long*/
#define	T_FLOAT		9		/*float*/
#define	T_DOUB		10		/*double*/
#define	T_ANY		11		/*int or word (implied)*/
#define T_UANY		12		/*4.2 unsigned short or char*/
#define	T_INDR		0x10	/*pointer type (bit)*/

struct skeleton {
	short sk_left;
	short sk_right;
	char *sk_def;
};
