/*
	Copyright 1982, 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*
 * alignment, node type altering routines, dimension table allocating
 * routine, and routines to determine elements actual size
 */

#include "parser.h"

#define DTSIZE	077						/* data size in bytes */

/*
 * dalloc - dimension table allocation
 * Allocates an entry in the dimension table.
 * returns ptr to dimension allocated
 */
short dalloc(P(long) dimsize)
PP(long dimsize;)
{
	register short i;

	if ((i = cdp++) >= DSIZE - 1)
		ferror("dimension table overflow");
	dtab[i] = dimsize;
	return i;
}


/*
 * addsp - add special type to special type info
 * Takes given special type and adds it into the special type field.
 * returns resulting type
 */
int addsp(P(int) type, P(int) nsptype)
PP(int type;)								/* old type field */
PP(int nsptype;)							/* special type to be added */
{
	register short dtype;

	dtype = BTYPE(type);
	type &= (~TYPE);
	if (type & (0xc000 >> SUTYPLEN))	/* 4.3, top type field full */
		error("arrays limited to five dimensions");
	else
		type <<= SUTYPLEN;
	return type | SUPTYPE(nsptype) | dtype;
}


/*
 * delsp - delete one special type info field from special type info
 * Takes given special type field and deletes least sign.
 * returns resulting type
 */
int delsp(P(int) type)
PP(int type;)								/* old special type */
{
	register short dtype;

	dtype = BTYPE(type);
	type &= (~(ALLTYPE));
	return (type >> SUTYPLEN) | dtype;
}


/*
 * revsp - reverse special type info
 * This allows for the processing of the super-type info in
 * the reverse order, which is necessary for initializations.
 * returns reversed type info 
 */
int revsp(P(int) type)
PP(int type;)								/* type to reverse */
{
	register short t;

	for (t = BTYPE(type); SUPTYPE(type) != 0; type = delsp(type))
		t = addsp(t, type);
	return t;
}


/*
 * falign - handle bit field alignments
 * returns number of bytes padded
 */
int falign(P(int) type, P(int) flen, P(int) offset)
PP(int type;)								/* data type */
PP(int flen;)								/* field length */
PP(int offset;)								/* current structure offset */
{
	register short off;

	if (flen <= 0)
	{
		error("invalid field size");
		flen = 0;
	}
	switch (type)
	{
	case INT:
	case UNSIGNED:
		if (flen > BITSPWORD)
			error("field overflows word");
		if ((flen + boffset) > BITSPWORD)
			off = CHRSPWORD;
		else
			off = (offset & 1);			/* 1 if odd, 0 if even */
		break;

	case CHAR:
	case UCHAR:
		if (flen > BITSPCHAR)
			error("field overflows byte");
		off = ((flen + boffset) > BITSPCHAR);
		break;

	default:
		error("invalid field type description");
		return 0;

	}
	if (off)
		boffset = 0;
	boffset += flen;
	return off;
}


/*
 * salign - structure alignment
 * returns bytes of padding
 */
int salign(P(int) type, P(int) offset)
PP(int type;)								/* data type to align */
PP(int offset;)								/* current structure offset */
{
	register short off;

	off = offset;
	if (boffset)
	{									/* remaining bit fields, flush 'em */
		off += (boffset + (BITSPCHAR - 1)) / BITSPCHAR;
		boffset = 0;
	}
	while (ISARRAY(type))				/* get base type */
		type = delsp(type);
	if (type != CHAR)					/* need word boundary */
		off = WALIGN(off);
	return off - offset;
}


/*
 * delspchk - delete one special reference and check if non-zero
 * returns new special type
 */
int delspchk(P(int) type)
PP(int type;)								/* type to modify */
{
	if (!(SUPTYPE(type)))
		error("bad indirection");
	return delsp(type);
}


/*
 * dosizeof - return size of object ptd at by POINTER
 * returns size of object in bytes
 */
int dosizeof(P(struct tnode *) tp, P(int) tbool)
PP(struct tnode *tp;)						/* POINTER to tree node */
PP(int tbool;)								/* 1==>sizeof expr, 0==>other sizeof */
{
	short size, type;
	struct tnode *lp, *rp;

	PUTEXPR(treedebug, "dosizeof", tp);
	if (NOTPOINTER(tp->t_type) || tp->t_op != ADD)
		size = dsize(tp->t_type, tp->t_dp, tp->t_ssp);
	else
	{
		lp = tp->t_left;
		rp = tp->t_right;
		while (lp->t_op == ADD)
		{
			rp = lp->t_right;
			lp = lp->t_left;
		}
		type = lp->t_type;
		if ((type & STRUCT) == STRUCT || (type & FRSTRUCT) == FRSTRUCT)
			size = dtab[lp->t_dp - 1] * dsize(rp->t_type, rp->t_dp, rp->t_ssp);
		else
			size = dtab[lp->t_dp - 1] * (dinfo[type & TYPE] & DTSIZE);
#ifdef DEBUG
		if (treedebug)
		{
			printf("second case: rtype 0%o ltype 0%o ", rp->t_type, lp->t_type);
			printf("dtab %ld\n", dtab[lp->t_dp - 1]);
		}
#endif
	}
	if (tbool)
		indecl = predecl;				/* value previous to sizeof.... */
#ifdef DEBUG
	if (treedebug)
	{
		printf("rtype 0%o ltype 0%o ", rp->t_type, lp->t_type);
		printf("size %d, dtab %ld\n", size, dtab[lp->t_dp - 1]);
	}
#endif
	return size;
}


/*
 * psize - return size of object ptd at by POINTER
 * returns size of object in bytes 
 */
long psize(P(struct tnode *) tp)
PP(struct tnode *tp;)						/* POINTER to tree node */
{
	if (!(SUPTYPE(tp->t_type)))			/* what case ??? */
		return 1;
	return dsize(delsp(tp->t_type), tp->t_dp, tp->t_ssp);
}


/*
 * dsize - returns size of data object in bytes
 * returns number of bytes
 */
long dsize(P(int) type, P(int) dp, P(int) sp)
PP(int type;)								/* type of node */
PP(int dp;)									/* dimension POINTER */
PP(int sp;)									/* size POINTER if structure */
{
	register long nel, size;

	nel = 1;
	if (ISARRAY(type))
	{
		do
		{
			type = delsp(type);
		} while (ISARRAY(type));
		nel = dtab[dp];
	}
	if (ISFUNCTION(type))
		return 0;
	size = (ISPOINTER(type)) ? PTRSIZE : (type == STRUCT) ? dtab[sp] : dinfo[type] & DTSIZE;
#ifdef DEBUG
	if (treedebug)
	{
		printf("size %ld ", size);
		printf("nel %ld ", nel);
		printf("dinfo[type=0%o] ==> %d\n", type, dinfo[type] & DTSIZE);
	}
#endif
	if (!size)
		error("invalid data type");
	return size * nel;
}


/*
 * integral - checks operand for integral type
 * This checks for needing an integral operand.
 */
VOID integral(P(struct tnode *) tp, P(int) atype)
PP(struct tnode *tp;)						/* pointer to tree node */
PP(int atype;)								/* alternate type allowable */
{
	register short type;

	type = tp->t_type;
	if (type != INT && type != UNSIGNED && type != CHAR && type != UCHAR && !SUPTYPE(type) && type != atype)
		warning("integral type expected");	/* "invalid operand type" */
}
