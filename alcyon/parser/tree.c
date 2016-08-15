/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"

short strassign, strsize;	/* [vlh] 4.2 save structure size for assignment */

/**
 * This indicates if a conversion operator is needed given the types
 * of the two operands.  The upper diagonal is applied ONLY if this
 * is an assignment operator and the indexes are swapped so the lower
 * diagonal is used otherwise.
 *   INT		UNSN		LONG		DOUB		PTR
**/
char cvmap[5][5] = {
	0,			UNSN_INT,	LONG_INT,	DOUB_INT,	PTR_INT,
	INT_UNSN,	0,			LONG_UNSN,	DOUB_UNSN,	PTR_UNSN,
	INT_LONG,	UNSN_LONG,	0,			DOUB_LONG,	PTR_LONG,
	INT_DOUB,	UNSN_DOUB,	LONG_DOUB,	0,			BADCONV,
	INT_PTR,	UNSN_PTR,	LONG_PTR,	BADCONV,	PTR_PTR,
};

/**
 * maketree - given operator, takes operands and builds up tree
 *		This takes the given operator, allocates a node for it
 *		and links up the subtrees on the operand stack.  A lot of
 *		work is in inserting the proper conversions.
**/
maketree(op)						/* returns success or failure */
int op;								/* new root operator*/
{
	register struct tnode *ltp, *rtp, *p;
	register short type, ltype, rtype, lconv, conv, pconv, docast, left, right;

#ifdef DEBUG
	if (treedebug)
		printf("maketree op = %d\n",op);
#endif
	if( BINOP(op) ) {
		if(!(rtp=(struct tnode *)popopd()))
			return(0);
		rtype = (rtp=(struct tnode *)funcref(arrayref(rtp)))->t_type;
		PUTEXPR(treedebug,"maketree r",rtp);
	}
	if(!(ltp=(struct tnode *)popopd()))
		return(0);
	PUTEXPR(treedebug,"maketree l",ltp);
	if( op == ASM )		/* [vlh] 4.2 asm internal function */
		return(1);
	if( op == SIZEOF ) {	/* [vlh] 4.2 dosizeof */
		pushopd(cnalloc(INT,dosizeof(ltp,1)));
		return(1);
	}
	if( op != ADDR ) {
		ltp = (struct tnode *)arrayref(ltp);
		if (op!=CALL && op!=NACALL)
			ltp = (struct tnode *)funcref(ltp);
	}
	else {	/* [vlh] 4.0 &p->array */
		if (ltp->t_op == ADD)	/* there must be a better way... */
			if (ltp->t_left->t_op==SYMBOL && ltp->t_right->t_op==CINT) {
				pushopd(ltp);
				return(1);
			}
	}
	if( specops(op,ltp,rtp) )
		return( 1 );
	ltype = ltp->t_type;
	if( LINTEGRAL(op) )
		integral(ltp,LONG);
	if( RINTEGRAL(op) )
		integral(rtp,LONG);
	if( LVALOP(op) && ltp->t_op != SYMBOL && ltp->t_op != INDR &&
			ltp->t_op != BFIELD )
		error("assignable operand required");
	if( UNARYOP(op) ) {
		if(!unopeval(op,ltp))
			pushopd(tnalloc(op,ltype,ltp->t_dp,ltp->t_ssp,ltp,0L));
		return(1);
	}
	if( ltype == STRUCT || rtype == STRUCT ) {
		if (ltype==STRUCT && rtype==STRUCT && op==ASSIGN) { /*[vlh]*/
			left = dosizeof(ltp,0);				/* [vlh] 4.2 */
			strsize = right = dosizeof(rtp,0);	/* [vlh] 4.2 */
			if (left < right) {	/* left side smaller than right */
				warning("left side of structure assignment smaller than right");
				strsize = left;
			}	/* [vlh] 4.2 */
			pushopd(ltp);
			maketree(ADDR);
			pushopd(rtp);
			maketree(ADDR);
			maketree(STASSIGN);
			strassign = 1;
			return(1);
		}
		ltype = rtype = INT;
		error("illegal structure operation");
	}
	type = ltype;
	if( rtype == TYPELESS ) {
		rtp->t_type = rtype = INT;
		lconv = conv = 0;
	}
	else {
		lconv = ttoconv(ltype);
		conv = ttoconv(rtype);
		if (ASGNOP(op) || lconv >= conv || ((RELOP(op) || COMOP(op)) && 
			 ltype==INT && rtype==LONG && rtp->t_op==CLONG)) {	/*[vlh] 4.2*/
			conv = cvmap[lconv][conv];
			lconv = 0;
		}
		else {
			conv = cvmap[conv][lconv];
			lconv = 1;
			type = rtype;
		}
	}
	if( ASGNOP(op) ) {
		if( (op == ASSIGN || op == FRETURN) && rtp->t_op != CINT && 
				(conv == INT_PTR || conv == UNSN_PTR) )
			warning("short assigned to pointer");
		else if( op == ASSIGN || op == CAST )
			switch( conv ) {

				case INT_PTR:
				case UNSN_PTR:
				case PTR_PTR:
				case PTR_LONG:
				case LONG_PTR:
					conv = 0;
					break;
			}
	}
	else if( op == COLON && SUPTYPE(ltype) != 0 && ltype == rtype )
		conv = 0;
	else if( RELOP(op) && conv == PTR_PTR )
		conv = 0;
	else if ( BINOP(op) && (ltp->t_op == CINT || ltp->t_op == CLONG) &&
				(rtp->t_op == CINT || rtp->t_op == CLONG))		/* [vlh] 4.2 */
		conv = 0;
	pconv = 0;
	if (RELOP(op) && ISALLTYPE(ltype) == (STRUCT | POINTER)
				&& (conv == INT_PTR || conv == LONG_PTR))	/* [vlh] 3.4 */
		conv = 0;	/* short compare to struct pointer, no conversion */
	if (op==FRETURN && conv == LONG_PTR)
		conv = 0;
	if( conv == PTR_PTR ) {
		conv = 0;
		if( op == SUB ) {
			type = LONG;
			pconv++;
		}
		else if(op != FRETURN && ( (ISALLTYPE(ltype) != ISALLTYPE(rtype) ||
					ISALLTYPE(ltype) != (POINTER|CHAR)) ) )
			warning("suspect conversion operation");
	}
	docast=(ltp->t_type==CHAR && rtp->t_sc!=REGISTER && rtp->t_sc<=STATIC);
	if( conv ) {
		if( conv == BADCONV )
			error("illegal type conversion");
		else if( lconv )
			ltp = (struct tnode *)cvopgen(ltp,type,conv,psize(rtp),op);
		else
			rtp = (struct tnode *)cvopgen(rtp,type,conv,psize(ltp),op);
	}
	if( op == CAST ) {	/* [vlh] 4.0 */
		if ( docast )	/* predefined to handle conv/cast ops */
			rtp = (struct tnode *)tnalloc(TOCHAR,CHAR,0,0,rtp,0L);
		else if ( rtp->t_type != CHAR && !conv ) {
			rtp->t_type = ltp->t_type;
			rtp->t_ssp = ltp->t_ssp;		/* [vlh] 3.4 */
			rtp->t_dp = ltp->t_dp;			/* [vlh] 3.4 */
		}
		pushopd(rtp);
	}
	if( RELOP(op) )
		type = INT;
	if(op != CAST && !binopeval(op,ltp,rtp)) {
		if( BTYPE(ltype) == STRUCT || BTYPE(rtype) != STRUCT )
			p = ltp;
		else
			p = rtp;
		p = (struct tnode *)tnalloc(op,type,p->t_dp,p->t_ssp,ltp,rtp);
		pushopd(p);
	}
	if(op==SUB && (ltype&POINTER) && (rtype&POINTER) && !lconv && !conv) {
		warning("pointer subtraction yields a long result");
#ifdef DEBUG
	if(symdebug) printf("pconv = %d\n",pconv);
#endif
	}
	if( pconv && ltype != (POINTER|CHAR) ) {
		if(!(ltp=(struct tnode *)popopd()))
			return(0);
		pushopd(cvopgen(ltp,LONG,PTR_LONG,psize(ltp->t_left),op));
	}
	return(1);
}

/* specops - handle special operators in building tree*/
specops(op,ltp,rtp)				/* returns 1 if op special, 0 otherwise*/
int op;							/* operator*/
struct tnode *ltp;				/* left subtree pointer*/
struct tnode *rtp;				/* right subtree pointer*/
{
	register short type;

	type = ltp->t_type;
#ifdef DEBUG
	if (treedebug)
		printf("specops: op = %d type = %d\n",op,type);
#endif
	switch (op) {

	case 0:
		break;

	default:
		return(0);

	case APTR:							/*expr -> name*/
		integral(ltp,LONG);				/*we need to turn expr into a*/
		ltp->t_type = POINTER|STRUCT;	/*pointer to a struct, then use*/
		pushopd(ltp);					/*expr . name stuff*/
		maketree(INDR);
		ltp = (struct tnode *)popopd();	/*ltp cannot be 0*/
	case PERIOD:						/*expr . name*/
		if( !(ISSTEL(rtp)) )
			error("invalid structure member name");
		type = rtp->t_type;
		if( ISARRAY(type) ) {
			type = delspchk(type);
			rtp->t_dp++;
		}
		tadjust(ltp,type,rtp->t_dp,rtp->t_ssp);
		pushopd(ltp);
		maketree(ADDR);
		pushopd(cnalloc(TYPELESS,rtp->t_offset));
		maketree(ADD);
		if( NOTARRAY(rtp->t_type) )
			maketree(INDR);
		ltp = (struct tnode *)popopd();
		if( rtp->t_sc == BFIELDCL )		/*ltp cannot be 0*/
			ltp = tnalloc(BFIELD,type,rtp->t_dp,rtp->t_ssp,ltp,0L);
		break;

	case QMARK:
		if( rtp->t_op != COLON )
			error("invalid ?: operator syntax");
		if( ltp->t_op == CINT && rtp->t_left->t_op == CINT &&
				rtp->t_right->t_op == CINT )
			ltp->t_value = (ltp->t_value ? rtp->t_left->t_value :
					rtp->t_right->t_value);
		else
			ltp = (struct tnode *)tnalloc(op,rtp->t_type,0,0,ltp,rtp);
		break;

	case LAND:
	case LOR:
	case COMMA:							/*don't need conversions here*/
		ltp = (struct tnode *)tnalloc(op,INT,0,0,ltp,rtp);
		break;

	case INDR:
		if( ltp->t_op == ADDR  )		/**& is null op*/
			ltp = ltp->t_left;
		else {
			if( ISFUNCTION(type) )
				error("indirection on function invalid");
			ltp = (struct tnode *)tnalloc(INDR,delspchk(type),ltp->t_dp,
													   ltp->t_ssp,ltp,0L);
		}
		break;

	case STASSIGN:	/*[vlh]*/
		ltp = (struct tnode *)tnalloc(STASSIGN,strsize,0,0,ltp,rtp);
		break;

	case NACALL:
	case CALL:
		if( NOTFUNCTION(type) ) {
			error("illegal call");
#ifdef DEBUG
	if (symdebug) printf("illegal call...................\n");
#endif
		}
		ltp = (struct tnode *)tnalloc(op,delspchk(type),ltp->t_dp,ltp->t_ssp,
												 ltp,rtp);
		break;

	case ADDR:
		if( ltp->t_op == INDR )		/*&* is null op*/
			ltp = ltp->t_left;
		else if( ltp->t_op == SYMBOL ) {
			if( ((struct symnode *)ltp)->t_sc == REGISTER )
				error("address of register");
			ltp = (struct tnode *)tnalloc(ADDR,addsp(type,POINTER),ltp->t_dp,
													ltp->t_ssp, ltp,0L);
		}
		else
			error("& operator illegal");
		break;
	}
	pushopd(ltp);
	return(1);
}

/* cvopgen - generate a conversion operator*/
/*		Generates conversions necessary for integers, pointers and longs.*/
char *
cvopgen(tp,type,conv,len,op)		/* returns pointer to conv node*/
struct tnode *tp;					/* pointer to node to do conversion*/
int type;							/* type to convert to*/
int conv;							/* specified conversion*/
long len;							/* object length [vlh] 3.4 i=>l */
int op;								/* for cast operator*/
{
	register struct tnode *rtp;
	register short cop;

#ifdef DEBUG
	if (treedebug) {
		printf("cvopgen: type=%d conv=%d op=%d ",type,conv,op);
		printf("- type=%d op=%d\n",tp->t_type,tp->t_op);
	}
#endif

	switch(conv) {

	case INT_PTR:
	case UNSN_PTR:
		if ((type == (POINTER|CHAR) || ISALLTYPE(type) == (POINTER|STRUCT)) &&
			(op == CAST || op == FRETURN) ) {
			cop = INT2L;	/* [vlh] 4.0 */
			break;
		}
		else
			if( op == CAST || op == FRETURN ) {
				cop = INT2L;		/*of the ptd to objects length plus*/
				if( len != 1L ) {	/*an integer to long covnversion*/
					rtp = (struct tnode *)cnalloc(INT,(int)len);
					tp = (struct tnode *)tnalloc(MULT,type,0,0,tp,rtp);
				}
				break;
			}
	case PTR_LONG:						/*need to generate mult or div*/
	case LONG_PTR:						/*of the ptd to objects length*/
		if( len == 1 )
			return((char *)tp);
		cop = (conv == PTR_LONG ? DIV : MULT);
		rtp = (struct tnode *)cnalloc(INT,(int)len);
		break;

	case INT_LONG:
		if (tp->t_op == CINT) /* [vlh] 4.1 constant conversion */
			return(lcnalloc(type,(long)((struct conode *)tp)->t_value));
	case UNSN_LONG:
		cop = INT2L;
		break;

	case INT_DOUB:	/*[vlh] 3.4*/
	case UNSN_DOUB:
		cop = INT2F;
		break;

	case LONG_DOUB:	/*[vlh] 3.4*/
		cop = LONG2F;
		break;

	case DOUB_LONG:	/*[vlh] 3.4*/
		cop = FLOAT2L;
		break;

	case DOUB_INT:	/*[vlh] 3.4*/
	case DOUB_UNSN:
		cop = FLOAT2I;
		break;

	case LONG_INT:
	case LONG_UNSN:
	case PTR_INT:
	case PTR_UNSN:
		cop = LONG2I;
		break;

	default:
		error("invalid conversion");
		return((char *)tp);
	}
	return( tnalloc(cop,type,0,0,tp,rtp) );
}

/* tadjust - expression tree type adjustment*/
/*		Adjusts the types of subtrees to agree with the top of the tree.*/
tadjust(tp,type,dp,ssp)				/* returns - none*/
struct tnode *tp;					/* pointer to tree*/
int type;							/* type to adjust to*/
int dp;								/* dimension pointer or info*/
int ssp;							/* structure pointer*/
{
	register short op;

	tp->t_type = type;
	if( dp >= 0 ) {
		tp->t_dp = dp;
		tp->t_ssp = ssp;
	}
	if( (op=tp->t_op) == ADDR )
		type = delspchk(type);
	else if( op == INDR )
		type = addsp(type,POINTER);
	else if( op != ADD && op != SUB )
		return;
	tadjust(tp->t_left,type,dp,ssp);
}

/**
 * funcref - handle tree function reference
 *		Turns a reference to a function into the address of the function.
**/
char *
funcref(tp)							/* returns pointer to node*/
struct tnode *tp;					/* pointer to old node*/
{
	if (ISFUNCTION(tp->t_type) )
		tp = (struct tnode *)tnalloc(ADDR,addsp(tp->t_type,POINTER),tp->t_dp,
			tp->t_ssp,tp,0L);
	return((char *)tp);
}

/* arrayref - handle tree array reference*/
/*		Turns a reference to an array into the address of the array.*/
char *
arrayref(tp)					/* returns pointer to tree node*/
struct tnode *tp;					/* tree node pointer*/
{
	register short dp;

	if( ISARRAY(tp->t_type) && !(ISSTEL(tp)) ) {
		tp->t_dp++;
		dp = tp->t_dp;
		pushopd(tp);
		tadjust(tp,delspchk(tp->t_type),-1,0);
		maketree(ADDR);
		tp = (struct tnode *)popopd();				/*tp cannot be 0*/
		tp->t_dp = dp;	/* 4.1 [vlh] get proper dp !!!! */
	}
	return((char *)tp);
}

/* ttoconv - maps normal type into conversion table type*/
ttoconv(type)						/* returns conversion type*/
int type;							/* type to convert*/
{
	switch(type) {

		case CHAR:
		case INT:
			return(0);

		case UCHAR:
		case USHORT:
		case UNSIGNED:
			return(1);

		case ULONG:
		case LONG:
			return(2);

		case FLOAT:
		case DOUBLE:
			return(3);

		default:
			return(4);
	}
}
