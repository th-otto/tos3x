/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
short bol;

outinit(tp,type)						/* returns - none*/
struct tnode *tp;
{
	outexpr(tnalloc(INIT,type,0,0,tp));
}

outcforreg(tp)
struct tnode *tp;
{
	outexpr(tnalloc(CFORREG,tp->t_type,0,0,tp));
}

outifgoto(tp,dir,lab)
struct tnode *tp;
int dir;
int lab;
{
	outexpr(tnalloc(IFGOTO,dir,lab,0,tp));
}

/* outasm - outputs asm literal.		 [vlh] 4.2 asm instruction */
outasm()
{
	register char *p;

	OUTLINE();
	OUTNULL();
	putchar('\t');
	for (p = cstr; *p ; p++) {
		putchar(*p);
		if (*p == '\n')
			putchar('\t');
	}
	putchar('\n');
}

outexpr(tp)
struct tnode *tp;
{
	if (!tp)
		return;
	OUTLINE();
	outtree(tp);
}

/* interprets and prints the parse tree */
outtree(tp)
struct tnode *tp;
{
	short w1, w2;

	if( !tp )
		return;
	
	printf("%x.%x",tp->t_op,tp->t_type);

	switch( tp->t_op ) {

		case CINT:
			printf(".%x\n",((struct conode *)tp)->t_value);
			break;

		case CLONG:
			w1 = ((struct lconode *)tp)->t_lvalue.hiword; 
			w2 = ((struct lconode *)tp)->t_lvalue.loword;
			printf(".%x.%x\n",w1,w2);
			break;

		case CFLOAT:	/*[vlh] 3.4*/
			w1 = ((struct lconode *)tp)->t_lvalue.hiword; 
			w2 = ((struct lconode *)tp)->t_lvalue.loword;
			printf(".%x.%x\n",w1,w2);
			break;

		case SYMBOL:
			printf(".%x",((struct symnode *)tp)->t_sc);
			if( ((struct symnode *)tp)->t_sc == EXTERNAL )
				printf(".%.8s\n",((struct extnode *)tp)->t_symbol);
			else
				printf(".%x\n",((struct symnode *)tp)->t_offset);
			break;

		case 0:
			putchar('\n');
			break;

		case IFGOTO:
		case BFIELD:
			printf(".%x\n",tp->t_dp);
			outtree(tp->t_left);
			break;

		default:
			putchar('\n');
			outtree(tp->t_left);
			if( BINOP(tp->t_op) ) {
				outtree(tp->t_right);
			}
			break;
	}	/* end of case... */
}

