/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "cgen.h"

short bol;
short gflag;
short onepass;

/* outexpr - output expression*/
outexpr(tp)						/* returns - none*/
struct tnode *tp;				/* pointer to tree node*/
{
	outline();
	if( exprok(tp) ) {
#ifdef DEBUG
	if (cflag) printf("outexpr 1: type is %d\n",tp->t_type);
#endif
		scodegen(canon(tp),FOREFF,0);
#ifdef DEBUG
	if (cflag) printf("outexpr 2: type is %d\n",tp->t_type);
#endif
	}
}

outifgoto(tp,dir,lab)
struct tnode *tp;
int dir;
int lab;
{
	outline();
	if( exprok(tp) )
		condbr(canon(tp),dir,lab,0);
}

outcforreg(tp)
struct tnode *tp;
{
	outline();
	if( exprok(tp) )
		outmovr(scodegen(canon(tp),FORREG,0),0,tp);
}

outinit(tp)							/* returns - none*/
struct tnode *tp;
{
	register short typeout;

	outline();
	if( exprok(tp) ) {
		typeout = tp->t_type;
		tp = canon(tp);
		if( tp->t_op == ADDR )
			tp = tp->t_left;
		if( tp->t_op == CINT || tp->t_op == CLONG || tp->t_op == SYMBOL ) {
			if( tp->t_op != CINT )	/* [vlh] 4.1 added CLONG... */
				printf(".dc.l ");
			else {
				printf(".dc");
				outtype(typeout);
				putchar(' ');
			}
			outaexpr(tp,NOTIMMED);	/* [vlh] 4.0 not immed... */
		}
		else
			error("invalid initialization");
		putchar('\n');
	}
}

/* snalloc - code generator symbol node allocation*/
/*		This might be coalesced into parser snalloc.*/
char *snalloc(type,sc,offset,dp,ssp)	/* returns ptr to node alloced*/
int type;						/* type of symbol*/
int sc;							/* storage class*/
int offset;						/* offset from Local Environment Ptr*/
int dp;							/*for compatability with parser*/
int ssp;						/*for compatability with parser*/
{
	register struct symnode *sp;

	sp = talloc(sizeof(*sp));
	sp->t_op = SYMBOL;
	sp->t_type = type;
	sp->t_su = dp;
	sp->t_ssp = ssp;
	sp->t_sc = sc;
	switch( sc ) {

	case STATIC:
		sp->t_offset = 0;
		sp->t_reg = 0;
		sp->t_label = offset;
		break;

	case REGISTER:
		sp->t_offset = 0;
		sp->t_reg = offset;
		sp->t_label = 0;
		break;

	case AUTO:
		sp->t_sc = REGOFF;
		sp->t_offset = offset;
		sp->t_reg = LEP;
		sp->t_label = 0;
		break;

	default:
		sp->t_offset = offset;
		sp->t_reg = 0;
		sp->t_label = 0;
		break;
	}
	return(sp);
}

exprok(tp)
struct tnode *tp;
{
	if( tp < exprarea || tp > &exprarea[EXPSIZE] )
		return(0);
	if( LEAFOP(tp->t_op) )
		return(1);
	if( BINOP(tp->t_op) && !exprok(tp->t_right) )
		return(0);
	return( exprok(tp->t_left) );
}

outline()
{
	if( onepass && !bol )
		putchar('\n');
	if (!gflag)
		printf("*line %d\n",lineno);
	else
		printf("~_lN%d:\n",lineno);
}
