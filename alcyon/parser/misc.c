/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/**
 * alignment, node type altering routines, dimension table allocating
 * routine, and routines to determine elements actual size
**/

#include "parser.h"

#define DTSIZE	077					/*data size in bytes*/

char dinfo[];

/* dalloc - dimension table allocation*/
/*		Allocates an entry in the dimension table.*/
dalloc(dimsize)						/* returns ptr to dimension allocated*/
long dimsize;						/* dimension size [vlh] 3.4 i=>l*/
{
	register short i;

	if( (i=cdp++) >= DSIZE-1 )
		ferror("dimension table overflow");
	dtab[i] = dimsize;
	return(i);
}

/* addsp - add special type to special type info*/
/*		Takes given special type and adds it into the special type field.*/
addsp(type,nsptype)					/* returns resulting type*/
int type;							/* old type field*/
int nsptype;						/* special type to be added*/
{
	register short dtype;

	dtype = BTYPE(type);
	type &= (~TYPE);
	return( (type<<SUTYPLEN) | SUPTYPE(nsptype) | dtype );
}

/* delsp - delete one special type info field from special type info*/
/*		Takes given special type field and deletes least sign.*/
delsp(type)							/* returns resulting type*/
int type;							/* old special type*/
{
	register short dtype;

	dtype = BTYPE(type);
	type &= (~(ALLTYPE));
	return( (type>>SUTYPLEN) | dtype );
}

/*
 * revsp - reverse special type info
 *		This allows for the processing of the super-type info in
 *		the reverse order, which is necessary for initializations.
 */
revsp(type)							/* returns reversed type info*/
int type;							/* type to reverse*/
{
	register short t;

	for( t = BTYPE(type); SUPTYPE(type) != 0; type = delsp(type) )
		t = addsp(t,type);
	return(t);
}

/* falign - handle bit field alignments*/
falign(type,flen,offset)			/* returns number of bytes padded*/
int type;							/* data type*/
int flen;							/* field length*/
int offset;							/* current structure offset */
{
	register short off;

	if( flen <= 0 ) {
		error("invalid field size");
		flen = 0;
	}
	switch( type ) {

	case INT:
	case UNSIGNED:
		if( flen > BITSPWORD )
			error("field overflows word");
		if((flen+boffset) > BITSPWORD) 
			off = CHRSPWORD;
		else
			off = (offset & 1);		/* 1 if odd, 0 if even */
		break;

	case CHAR:
	case UCHAR:
		if( flen > BITSPCHAR )
			error("field overflows byte");
		off = ((flen+boffset) > BITSPCHAR );
		break;

	default:
		error("invalid field type description");
		return(0);

	}
	if( off )
		boffset = 0;
	boffset += flen;
	return(off);
}

/* salign - structure alignment*/
salign(type,offset)					/* returns bytes of padding*/
int type;							/* data type to align*/
int offset;							/* current structure offset*/
{
	register short off;

	off = offset;
	if( boffset ) {					/*remaining bit fields, flush 'em*/
		off += (boffset+(BITSPCHAR-1))/BITSPCHAR;
		boffset = 0;
	}
	while( ISARRAY(type) )			/*get base type*/
		type = delsp(type);
	if( type != CHAR )				/*need word boundary*/
		off = WALIGN(off);
	return( off - offset );
}

/* delspchk - delete one special reference and check if non-zero*/
delspchk(type)						/*returns new special type*/
int type;							/* type to modify*/
{
	if(!(SUPTYPE(type))) {
#ifdef DEBUG
	if (treedebug) printf("bad indirection..................\n");
#endif
		error("bad indirection");
	}
	return( delsp(type) );
}

/* dosizeof - return size of object ptd at by POINTER [vlh] 4.2 */
dosizeof(tp,bool)					/* returns size of object in bytes*/
struct tnode *tp;					/* POINTER to tree node*/
int bool;							/* 1==>sizeof expr, 0==>other sizeof*/
{
	short size;
	struct tnode *lp, *rp;

	PUTEXPR(treedebug,"dosizeof",tp);
	if (NOTPOINTER(tp->t_type) || tp->t_op != ADD)
		size = dsize(tp->t_type, tp->t_dp, tp->t_ssp);
	else {
		lp = tp->t_left; rp = tp->t_right;
		while (lp->t_op == ADD) {
			rp = lp->t_right;
			lp = lp->t_left;
		}
		size = dtab[lp->t_dp - 1] * dsize(rp->t_type,rp->t_dp,rp->t_ssp);
	}
	if (bool)	/* [vlh] 4.2 */
		indecl = predecl;	/* value previous to sizeof.... */
	return(size);
}

/* psize - return size of object ptd at by POINTER*/
long								/* [vlh] 3.4 short => long */
psize(tp)							/* returns size of object in bytes*/
struct tnode *tp;					/* POINTER to tree node*/
{
	if ( !(SUPTYPE(tp->t_type)) )	/* what case ??? */
		return(1);
	return(dsize(delsp(tp->t_type),tp->t_dp,tp->t_ssp));
}

/* dsize - returns size of data object in bytes*/
long								/* [vlh] 3.4 */
dsize(type,dp,sp)					/* returns number of bytes*/
int type;							/* type of node*/
int dp;								/* dimension POINTER*/
int sp;								/* size POINTER if structure*/
{
	register long nel, size;

	nel = 1;
	if (ISARRAY(type)) {
		do { 
			type = delsp(type);
		} while (ISARRAY(type));
		nel = dtab[dp];
	}
	if( ISFUNCTION(type) )
		return(0);
	size = (ISPOINTER(type)) ? PTRSIZE : (type == STRUCT) ? 
			dtab[sp] : dinfo[type]&DTSIZE;
	if(!size)
		error("invalid data type");
	return( size * nel );
}

/* integral - checks operand for integral type*/
/*		This checks for needing an integral operand.*/
integral(tp,atype)					/* returns - none*/
struct tnode *tp;					/* pointer to tree node*/
int atype;							/* alternate type allowable*/
{
	register short type;

	type = tp->t_type;
	if( type != INT && type != UNSIGNED && type != CHAR && type != UCHAR &&
			!SUPTYPE(type) && type != atype )	/* [vlh]4.2 was error: */
		warning("integral type expected");		/* "invalid operand type" */
}

