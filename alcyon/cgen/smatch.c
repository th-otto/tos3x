/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* Code Skeleton expansion and matching */

#include "cgen.h"
#include "cskel.h"
#define	SK_TYPE(x)	(x&017)

/* expand - code skeleton expansion*/
/*		Handles the expansion of code skeleton macros.*/
expand(tp,cookie,freg,skp)			/* returns register result is in*/
struct tnode *tp;					/* pointer to expression tree*/
int cookie;							/* goal of expression tree*/
int freg;							/* register to leave results in*/
struct skeleton *skp;				/* pointer to code skeleton*/
{
	register short op, nreg, reg;
	register short c;
	register short extf, i2f;
	register struct tnode *ltp, *rtp;
	register char *p;
	register short i, sreg, flag, subtrees, scookie;
	register char *macro;

/**
 *	This is a kludge because not all of the arithmetic operators
 *	will work on address registers - the 68000 clone strikes again
**/
	op = tp->t_op;	
#ifdef DEBUG
	if(eflag) printf("expand op=%d left=%x right=%x skp=%lo\n",op,
				skp->sk_left,skp->sk_right,skp);
#endif
	if(((op>=MULT && op<=COMPL) || (op>=LMULT && op<=LMOD)) || tp->t_type==CHAR)
		freg = DREG(freg);	/* [vlh] 4.1 added UMINUS, COMPL, LMUL, LDIV */
	macro = skp->sk_def;
	i2f = extf = 0;
	rtp = ltp = tp->t_left;
	subtrees = 1;
	if( BINOP(op) ) {
		subtrees++;
		rtp = tp->t_right;
		if( (LONGORPTR(tp->t_type)) && (op == DIV || op == MOD ||
				(op != MULT && (ISDREG(freg)) &&
				!(LONGORPTR(ltp->t_type)) && !(LONGORPTR(rtp->t_type)))) )
			extf++;
		switch( op ) {

		case RSH:
		case LSH:
		case EQLSH:
		case EQRSH:
			if( UNSIGN(ltp->t_type) )
				i2f++;
			break;

		case MULT:
		case EQMULT:
		case DIV:
		case MOD:
		case EQDIV:
		case EQMOD:
			if( UNSIGN(ltp->t_type) || UNSIGN(rtp->t_type) )
				i2f++;
			break;
		}
	}
	nreg = freg + 1;
	while( c = *macro++ ) {
		c &= 0xff;
		switch( c ) {

		default:
			putchar(c);
			break;

		case POP:
			stacksize--;
			printf("(sp)+");
			break;

		case POP4:
			stacksize--;
			popstack(4);
			break;

		case POP8:
			stacksize -= 2;
			popstack(8);
			break;

		case PSH:
			if( cookie == FORSP )				/*don't affect sp*/
				printf("(sp)");
			else
				printf("-(sp)");
			stacksize++;
			break;

		case MOV:
		case MOVL:
		case JSR:
		case CLR:
		case CLRL:
		case EXTW:
		case EXTL:
		case LEA:
		case STK:
			printf("%s",strtab[c-128]);
			break;

		case OPCALL:
			op_expand(op,tp->t_type,ltp->t_type);
			break;

		case TLEFT:
			outtype( LEAFOP(op) ? tp->t_type : ltp->t_type );
			break;

		case TLEFTL:
			outatype( LEAFOP(op) ? tp->t_type : ltp->t_type );
			break;

		case TEITHER:
			if( LONGORPTR(rtp->t_type) || LONGORPTR(ltp->t_type) )
				outtype(LONG);
			break;

		case TRIGHT:
			outtype(rtp->t_type);
			break;

		case OP:
		case AOP:
			if( c == AOP || i2f )
				i = optab[op][1];
			else
				i = optab[op][0];
			printf(mnemonics[i]);
			break;

		case LADDR:
		case RADDR:
			p = ((c==RADDR) ? rtp : ltp);
			outaexpr(p,IMMED);
			break;

		case CR:
			outcreg(freg);
			break;

		case NR:
			outcreg(nreg);
			break;

		case CAR:
			outcreg(AREG(freg));
			break;

		case NAR:
			outcreg(AREG(nreg));
			break;

		case EXL:
			outextend(ltp,LONG,freg);
			break;

		case EXRL:
		case EXRLN:
			outextend(rtp,ltp->t_type,c==EXRL?freg:nreg);
			break;

		case EXLR:
		case EXLRN:
			outextend(ltp,rtp->t_type,c==EXLR?freg:nreg);
			break;

		case LEFT:
		case RIGHT:
			subtrees--;
		case TREE:
			p = (c==LEFT?ltp:c==RIGHT?rtp:tp);
			flag = *macro++;
			scookie = FORREG;
			if( flag & S_STACK ) {
				if( cookie == FORSP )
					scookie = FORSP;
				else
					scookie = FORSTACK;
			}
			else if( flag & S_FORCC )
				scookie = FORCC;
			if( flag & S_NEXT )
				reg = nreg;
			else
				reg = freg;
			if( flag & S_INDR ) {
				if( p->t_op != INDR )
					error("code skeleton error: %d\n",op);
				p = p->t_left;		/*skip INDR*/
				if( coffset(p) ) {
					p = p->t_left;
					if( LONGORPTR(p->t_type) == 0 && (flag&S_STACK) != 0 )
						p = tnalloc(INT2L,LONG,0,0,p);
				}
				reg = AREG(reg);	/*no qualifications before...*/
#ifdef DEBUG
	if (eflag)
		printf("reg = %d, nreg = %d, freg = %d\n",reg,nreg,freg);
#endif
			}
			sreg = codegen(p,scookie,reg);	/*code for subtree*/
			if( scookie == FORREG ) {
				if( flag & S_INDR ) {
					if( ISDREG(sreg) )
						outmovr(sreg,AREG(reg),p);
				}
				else if( flag & S_NEXT )
					nreg = sreg;
				else if( sreg != reg ) {
	/*
	 * result was not in expected register, if remaining sub-tree can be
	 * compiled using the remaining registers, update current and next
	 * registers, saving us the trouble of moving the register.
	 */
					if( c == TREE || ((ISDREG(sreg)) && subtrees > 0 &&
						((c == LEFT &&
							sucomp(rtp,sreg,0) <= skp->sk_right &&
							sucomp(rtp,sreg,1) <= SU_ANY) ||
						( c == RIGHT &&
							sucomp(ltp,sreg,0) <= skp->sk_left &&
							sucomp(ltp,sreg,1) <= SU_ANY))) ) {
						freg = DREG(sreg);
						nreg = freg + 1;
					}
					else
						outmovr(sreg,DREG(freg),p);
				}
			}
			break;

		case LOFFSET:
		case ROFFSET:
			p = (c==LOFFSET) ? ltp->t_left : rtp->t_left;
			if((p=coffset(p)) != 0 && (p->t_op != CINT || p->t_value != 0))
				outaexpr(p,NOTIMMED);
			break;

		case MODSWAP:
			switch( op ) {

			case MOD:
			case EQMOD:
			case LMOD:
			case LEQMOD:
				OUTSWAP(freg);
			}
			break;

		}
	}
	if( extf && cookie == FORREG && (ISDREG(freg)) ) {
		if( UNSIGN(ltp->t_type) || UNSIGN(rtp->t_type) )
			OUTUEXT(freg);
		else
			OUTEXT(freg);
	}
#ifdef DEBUG
	if(eflag) printf("ending expand skp=%lo\n",skp);
#endif
	return(freg);
}

/* op_expand - printf out the subroutine being called */
op_expand(op,type,ltype)
int op, type,ltype;
{
	if( ISFLOAT(type) || ISFLOAT(ltype) ) {
		switch( op ) {

			case ADD:
			case EQADD:
			case PREINC:	/* [vlh] 4.2 */
			case POSTINC:	/* [vlh] 4.2 */
				printf("_fpadd");
				break;

			case SUB:
			case EQSUB:
			case PREDEC:	/* [vlh] 4.2 */
			case POSTDEC:	/* [vlh] 4.2 */
				printf("_fpsub");
				break;

			case MULT:
			case EQMULT:
				printf("_fpmult");
				break;

			case DIV:
			case EQDIV:
				printf("_fpdiv");
				break;

			case UMINUS:
			case EQNEG:
				printf("_fpneg");
				break;

			case FLOAT2L:
			case FLOAT2I:
				printf("_fpftol");
				break;

			case LONG2F:
			case INT2F:
				printf("_fpltof");
				break;

			case EQUALS:
			case NEQUALS:
			case GREAT:
			case GREATEQ:
			case LESS:
			case LESSEQ:
				printf("_fpcmp");
				break;

			default:
				error("invalid floating point op %d\n",op);
				break;
		}
	}
	else {
		switch( op ) {

			case MULT:
			case LMULT:
				printf("lmul");
				break;
	
			case DIV:
			case LDIV:
				printf("ldiv");
				break;

			case MOD:
			case LMOD:
				printf("lrem");
				break;

			default:
				error("opcall bad op %d",op);
				break;
		}
	}
}	

/*
 * match - try to match expression tree with code skeleton
 *		Given the expression tree, tries to match the given tree with
 *		the appropriate code skeleton.  The code skeleton list is
 *		gotten from the root operator and the cookie value.  The code
 *		skeleton list is then searched, checking the Sethy-Ullman numbers
 *		of the sub-trees against the Sethy-Ullman numbers in the code
 *		skeleton list.  If the Sethy-Ullman numbers are OK, then the
 *		left and right sub-trees are checked for compatability, e.g.
 *		integer pointers, etc.  If a match is found, the code skeleton
 *		list pointer is returned.
 */
char *									/* returns ptr to code skeleton*/
match(tp,cookie,reg) 					/* or 0 if no skeleton*/
struct tnode *tp;						/* pointer to tree*/
int cookie;								/* goal for code expansion*/
int reg;								/* register to use*/
{
	register struct skeleton *skp;
	register short op, bop;
	short i;
	register struct tnode *ltp, *rtp;

#ifdef DEBUG
	if(mflag) printf("match op=%d cookie=%d reg=%d\n",tp->t_op,cookie,reg);
#endif
	PUTEXPR(mflag,"match",tp);
	if( (op=tp->t_op) >= LCGENOP )
		return(0);
	if( LEAFOP(op) )
		ltp = tp;
	else
		ltp = tp->t_left;
	if( (bop=BINOP(op)) && cookie!=FORSP) {	/* [vlh] 4.2 !FORSP */
		rtp = tp->t_right;
		if( CONVOP(ltp->t_op) ) {
			if( op != LSH && NOTCONVOP(rtp->t_op) ) {
				if( !(UNSIGN(ltp->t_left->t_type)) || op == ASSIGN ) {
					tp->t_left = ltp->t_left;
					if( (skp=match(tp,cookie,reg)) != 0 )
						return(skp);
					tp->t_left = ltp;
				}
			}
		}
		else if( CONVOP(rtp->t_op) ) {
			if( !(UNSIGN(rtp->t_left->t_type)) || op == ASSIGN ) {
				tp->t_right = rtp->t_left;
				if( (skp=match(tp,cookie,reg)) != 0 )
					return(skp);
				tp->t_right = rtp;
			}
		}
	}
	
	switch( cookie ) {
		case FORCC: 
			i = 3; 
			break;

		case FOREFF: 
			i = 2; 
			break;

		case FORSTACK:
		case FORSP:
			i = 4;
			break;

		case FORREG:
			i = 5;
			break;
	
		default:
			error("match cookie=%d\n",cookie);
			return(0);
		}
#ifdef DEBUG
	if(mflag) printf("match op=%d i=%d ",op,i);
#endif
	if( !(i=optab[op][i]) )
		return(0);
	skp = codeskels[i];
#ifdef DEBUG
	if(mflag) printf("codeskels[%d]=%o\n",i,skp);
#endif
#ifdef DEBUG
	if(mflag) {
		printf("match LEFT ");
		puttsu(ltp);
		if(bop) {
			printf(" RIGHT ");
			puttsu(rtp);
		}
		putchar('\n');
	}
#endif
	for( ; skp->sk_left != 0; skp++ ) {
#ifdef DEBUG
		if( mflag > 1 )
			printf("sk_left=%x sk_right=%x\n",skp->sk_left,skp->sk_right);
#endif
		if( !(skelmatch(ltp,skp->sk_left)) )
			continue;
		if( bop && !(skelmatch(rtp,skp->sk_right)) )
			continue;
#ifdef DEBUG
		if( mflag )
			printf("match found skp=%o left=%x right=%x\n",skp,
					skp->sk_left,skp->sk_right);
#endif
		return(skp);
	}
	return(0);
}

/* skelmatch - sub-tree type matching for match*/
/*		This checks a subtree for type compatability in match.*/
skelmatch(tp,skinfo)					/* returns 1 if matched, else 0*/
struct tnode *tp;						/* pointer to expression tree*/
int skinfo;
{
	register short type, unsignf, const, stype;

	if( tp->t_su > skinfo || ((skinfo&T_INDR) && tp->t_op != INDR) )
		return(0);
	stype = SK_TYPE(skinfo);
	type = tp->t_type;
	if( ISFUNCTION(type) )
		type = BTYPE(type);
	if( unsignf = UNSIGN(type) )
		type = BASETYPE(type);
	const = 0;
	switch( tp->t_op ) {

	case CFLOAT:		/* [vlh] 3.4 */
	case CLONG:
		if( tp->t_su > SU_CONST )
			break;
	case CINT:
		const++;
		break;
	}
	switch( stype ) {

		case T_CHAR:
			return( type == CHAR );

		case T_ANY:							/*either short or char*/
			if( type == CHAR )
				return(1);
		case T_INT:
			return( type == INT || const );

		case T_UNSN:
		case T_UANY:	/* [vlh] 4.2 */
		case T_UCHAR:	/* [vlh] 4.2 */
		case T_ULONG:	/* [vlh] 4.2 */
			return( unsignf );

		case T_LONG:
			return( LONGORPTR(type) );

		case T_FLOAT:
			return( ISFLOAT(type) );

		default:
			error("skelmatch type: %x",stype);
			return(0);
	}
}

