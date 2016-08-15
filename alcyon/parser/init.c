/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"

#define NULL	(char *)0

short inittype;

short bfield_ty;	/* [vlh] 4.2 type of bit field currently initializing */
short bits_init;	/* [vlh] 4.2 current bit field init value */
short in_bitinit;	/* [vlh] 4.2 are we currently initializing a bit field ?? */
short lst_boffset;	/* [vlh] 4.2 new bit field entity ?? */

char *opname[], *types[];

#define BCHK(plus) {plus = out_bfield(CHAR); nbout += plus; nbleft += plus;}

/*
 * doinit - do external definition initialization
 *      Handles following C syntax:
 *          initializer:
 *                  = constant_expression
 *                  = { initializer_list }
 *                  = { initializer_list , }
 *          initializer_list:
 *                  constant_expression
 *                  initializer_list , initializer_list
 *                  { initializer_list }
 */
doinit(sp)                          /* returns number of elements init'ed*/
struct symbol *sp;                  /* pointer to symbol to init*/
{
    register short type;
    long isize, datasize;

    type = sp->s_type;
    datasize = dsize(type,sp->s_dp,sp->s_ssp);
    if( PEEK(COMMA) || PEEK(SEMI) ) {   /*no initialization*/
        isize = WALIGN(datasize);
#ifdef DEBUG
	if(symdebug) 
		printf("%s sc %x attrib %x\n",sp->s_symbol,sp->s_sc,sp->s_attrib);
#endif
        if( sp->s_sc == EXTERNAL )
            OUTCOMMON(sp->s_symbol,isize);
        else {
            OUTBSS();               /*bss segment*/
            OUTLAB(sp->s_offset);
            OUTRESMEM(isize);
            OUTTEXT();              /*resume text segment*/
        }
    }
    else {
        if (!(next(ASSIGN)))               /*ignore '=' if there*/
			 warning("old fashion initialization");
        if( type == LONG || ISPOINTER(type) )
            outldata();
        else if(type == DOUBLE || type == FLOAT)    /*[vlh] 3.4*/
            outfpdata();
        else
            outdata();
        if( sp->s_sc == EXTERNAL )
            OUTDLAB(sp->s_symbol);
        else
            OUTLAB(sp->s_offset);
        isize = initlist(sp, sp->s_type, sp->s_dp, sp->s_ssp);
        if( isize < datasize ) {
            OUTRESMEM(datasize-isize);
            isize = datasize;
        }
        if( WALIGN(isize) != isize )
            OUTPAD();
        if( isize > datasize ) {
            if( ISARRAY(sp->s_type) ) {
                for( type = sp->s_type; ISARRAY(type); type = delsp(type) )
                    ;
                dtab[sp->s_dp] = (isize / dsize(type,sp->s_dp,sp->s_ssp));
            }
            else
                error("too many initializers");
        }
        if( sp->s_sc == STATIC )
            OUTTEXT();
    }
}

/*
 * initlist - handles initializer lists
 *      This handles multi-level braces, and a character pointer pointing
 *      to a string.  Most of the work is in keeping track of how many
 *      bytes are left on the current "row", and outputting padding if
 *      necessary.
 */
long
initlist(sp,type,dp,ssp)            /* returns size of initializers in bytes*/
struct symbol *sp;					/* [vlh] 4.2 for use in struct init */
int type, dp, ssp;
{
    register short onetype, plus, atype;
    long datasize, i, j, elsize, nbleft, nbout;    /* [vlh] 3.4 int=>long */

	i = (next(LCURBR));
	for( onetype = type; ISARRAY(onetype); onetype = delsp(onetype) )
        ;
#ifdef DEBUG
	if (symdebug)  {
		printf("type: %d BTYPE %d ISARRAY %d ",type,BTYPE(type),ISARRAY(type));
		printf("POINTER %d\n",ISPOINTER(type));
		printf("onetype: %d BTYPE %d ",onetype,BTYPE(onetype));
		printf("ISARRAY %d POINTER %d\n",ISARRAY(onetype),ISPOINTER(onetype));
	}
#endif
    nbout = 0;
    datasize = dsize(type,dp,ssp);
    nbleft = 0;                     /*keep track of no. of bytes left*/
	atype = (ISARRAY(type)) ? delsp(type) : type; /*[vlh]4.2 multi dimens */
    if(type==(ARRAY|CHAR) || type==(ARRAY|INT) || type==(ARRAY|LONG))
        nbout = str_init(datasize,type);	
    else if(atype==(ARRAY|CHAR) || atype==(ARRAY|INT) || atype==(ARRAY|LONG)) {
		datasize = dsize(atype,dp,ssp);
		elsize = dsize(atype,dp+1,ssp);
		if( !i )
			error("missing { in initialization");
		do {
			j = (next(LCURBR));
			nbout += str_init(elsize,atype);
			if (j && !(next(RCURBR)))
				error("missing } in initialization");
			next(COMMA);		/* skip over comma... */
		} while(!PEEK(RCURBR) && !PEEK(SEMI));
		nbleft = datasize - nbout;
		if ((elsize==INTSIZE && (onetype==INT) || (onetype==SHORT)) ||
					(elsize==CHARSIZE && (onetype==CHAR)) || 
					(elsize==LONGSIZE && (onetype==LONG)) || 
					(datasize==elsize)) /*undimensioned array*/
			nbleft = 0;	
		if (nbleft < 0) {
			error("initializer list too long");
			nbleft = 0;
		}
	}	/* multi-dimensional array */
	else if (BTYPE(type)==STRUCT && NOTPOINTER(type) && ISPOINTER(onetype)) {
		elsize = PTRSIZE;	/* [vlh] 4.2.f */
		if( !i )
			error("missing { in initialization");
		do {
			nbleft = s_or_a(POINTER|CHAR,elsize,&nbout,nbleft,NULL,NULL);
			next(COMMA);		/* skip over comma... */
		} while(!PEEK(RCURBR) && !PEEK(SEMI));
	}
    else if (ISARRAY(type) || (BTYPE(type)==STRUCT && NOTPOINTER(type))) {
		elsize = dsize(delsp(type),dp+1,ssp);
        /* [vlh] 4.2, added proper handling of structure init... */
		if( !i && BTYPE(type)==STRUCT )
			error("missing { in initialization");
		do {
			nbleft = s_or_a(onetype,elsize,&nbout,nbleft,sp->s_par,NULL);
			next(COMMA);		/* skip over comma... */
		} while(!PEEK(RCURBR) && !PEEK(SEMI));
	}
    else {
		indecl = 0;					/* [vlh] 4.2 */	
        nbout = oneinit(onetype,dp,(int)sp->s_sc);	/* [vlh] 4.2.c add 2 args */
    }
	if (in_bitinit) 
		BCHK(plus)  /* [vlh] 4.2 bit field initialization */
    if( nbleft && (nbleft != elsize)) {                  /*pad current row*/
        OUTRESMEM(nbleft);
        nbout += nbleft;
    }
	if (i) {
		next(COMMA);		/* [vlh] 4.2 skip over extra comma */
		if (!(next(RCURBR)))
			error("missing } in initialization");
	}
    return(nbout);
}

/**
 * s_or_a - initialize structure or array
 *		routine to deal with initializing structures and arrays and
 *		assure that structures are aligned properly.  Checks initializers
 *		with the expected type.
 *		Attempts to match structure initializations against the 
 *		elements of the structure.
 *		sp -- pointer to structure symbol table entry
**/
long
s_or_a(original,elsize,pnbout,bleft,sp,child) /* returns nbleft */
int original;		/* [vlh] original type... */
long elsize, bleft, *pnbout;
struct symbol *sp;					/* [vlh] 4.2 for use in struct init */
struct symbol *child;				/* [vlh] 4.2 for use in struct init */
{
	register long nbleft, nbout;
	register short onetype, plus;
	long i, temp, datasize;
	short nest;	/* [vlh] 4.2 nest <= number of LCURBR */
	struct symbol *hold;

#ifdef DEBUG
	if (symdebug) printf("s_or_a: elsize %ld\n",elsize);
#endif
	nbleft = bleft; nbout = *pnbout; onetype = original; nest = i = 0;
	do {                        /*in current row.*/
restart:
		if (BTYPE(original)==STRUCT && !child)	/* [vlh] 4.2 */
			if ((child = sp->s_child) == 0)
				break;
#ifdef DEBUG
	if (symdebug && BTYPE(original)==STRUCT)
		printf("s_or_a: child %s type %d\n",child->s_symbol,child->s_type);
#endif
		if( PEEK(SEMI) || PEEK(EOF) || PEEK(RCURBR) )
			break;
		if( next(LCURBR) ) {    /*recurse down one level?*/
			nest++;
			if (in_bitinit) 
				BCHK(plus) /* [vlh] 4.2 bit field initialization */
			if( nbout && nbleft && (nbleft<elsize)) {
				OUTRESMEM(nbleft);  /*pad rest of current row*/
				nbout += nbleft;
				nbleft = 0;
			}
			if (!nbout && !nbleft)
				goto restart;
			if( i > elsize )
				error("initializer list too long");
			child = 0;	/* force restart of structure match up */
			goto restart;
		}
		else if( PEEK(RCURBR) ) {
			if (nest) {
				next(RCURBR); 
				nest--;
				continue; 	/* goes to next comma... */
			}
			else
				break;
		}
		else {
			if (BTYPE(original) == STRUCT) {			/* [vlh] 4.2 */
				if ((onetype = child->s_type) == STRUCT) {
					temp = nbout;
					hold = child->s_child;
					i = dsize(onetype,(hold->s_dp)+1,hold->s_ssp);
					s_or_a(onetype,i,&temp,nbleft,hold,NULL);
					if (!peektok)
						peektok = COMMA;	/* [vlh] 4.2 put it back.... */
					goto past;
				}
			}
			if(onetype==(ARRAY|CHAR) || onetype==(ARRAY|INT) || 
						onetype==(ARRAY|LONG)) {	/* [vlh]4.2 simple arrays */
				datasize = dsize(onetype,child->s_dp,child->s_ssp);
				i = str_init(datasize, onetype);
			}
			else if(original==(ARRAY|CHAR) || original==(ARRAY|INT) || 
						original==(ARRAY|LONG)) {	/* [vlh]4.2 simple arrays */
				datasize = dsize(original,child->s_dp,child->s_ssp);
				i = str_init(datasize, original);
			}
			else if (ISARRAY(onetype)) {	/* [vlh] 4.2 */
				temp = nbout;
#ifdef DEBUG
	if (symdebug) printf("ISARRAY, recursing on s_or_a... type %d\n",onetype);
#endif
				i = dsize(delsp(onetype),child->s_dp+1,child->s_ssp);
				s_or_a(delsp(onetype),i,&temp,nbleft,hold,NULL);
				goto past;
			}
			else {
				if ((nbleft & 1) && onetype != CHAR) {
					nbleft--;
					nbout++;
					OUTEVEN();
				}
				i = oneinit(onetype,child->s_dp,(int)child->s_sc);
			}
past:
			nbout += i;
			if(!nbleft)
				nbleft = elsize;
			if( i > nbleft )
				error("initializer alignment");
			nbleft = (i >= nbleft) ? 0 : nbleft - i;
			if (BTYPE(original)==STRUCT) {	/* [vlh] 4.2, get next child */
				if (!(child = child->s_sib)) {	/* [vlh] 4.2 at end... */
					plus = (in_bitinit) ? out_bfield(CHAR) : 0;
					nbleft -= plus;
					nbout += nbleft;
					if (nbleft && (nbleft < elsize)) {
						OUTRESMEM(nbleft);
						nbleft = 0;
					}
					if (SUPTYPE(original) != ARRAY) {
						break;
					}
				}
			}
		}
	} while( next(COMMA) );
	if (BTYPE(original)==STRUCT) {	/* [vlh] 4.2, ensure padding... */
		plus = (in_bitinit) ? out_bfield(CHAR) : 0;
		nbout += nbleft;
		if (nbleft && (nbleft < elsize)) {
			OUTRESMEM(nbleft);
			nbleft = 0;
		}
	}
	*pnbout = nbout;
	while(nest--) {
		next(COMMA);		/* comma's may separate.... */
		if (!next(RCURBR)) 	/* force release of matched curbr */
			error("mismatched curly braces");
	}
	next(COMMA);		/* comma's may separate.... */
	return(nbleft);
}

/**
 * str_init - string (array) initialization			[vlh] 4.2
 * 		initialize a character array, truncating or padding as required
**/
long
str_init(datasize,type)
long datasize;
int type;		/* should not get a bit field here..... */
{
	register long maxsize, output;

#ifdef DEBUG
	if (symdebug) printf("str_init: type %d, datasize %ld\n",type,datasize);
	if (symdebug) printf("str_init: SUPTYPE %d, ",SUPTYPE(type));
	if (symdebug) printf("BTYPE %d\n",BTYPE(type));
#endif
	output = 0L;
	if ((datasize==CHARSIZE && (BTYPE(type)==CHAR)) ||  /*undimensioned array*/
			(datasize==INTSIZE && (BTYPE(type)==INT) || (BTYPE(type)==SHORT)) ||
			(datasize==LONGSIZE && (BTYPE(type)==LONG))) {
		datasize = 0;
		maxsize = 0x7fff;	/* max 32 bit positive value */
#ifdef DEBUG
	if (symdebug) printf("undimensioned type %d datasize %ld\n",type,datasize);
#endif
	}
	else
		maxsize = datasize;
	do {
		if (next(STRING)) {
			output += (!datasize) ? outstr((long)cstrsize,(long)cstrsize) :
								outstr(maxsize-output,(long)cstrsize);
			return(output);
		}
		output += oneinit(BTYPE(type)|ARRAY,0,0);
#ifdef DEBUG
	if (symdebug) printf("output %ld, ",output);
	if (symdebug) printf("maxsize %ld, ",maxsize);
#endif
		if (maxsize <= output) {
			if (maxsize < output)
				error("character array initializer alignment");
			return(output);
		}
	} while (next(COMMA) && !PEEK(SEMI) && !PEEK(RCURBR));
	if (datasize) {
		OUTRESMEM(maxsize-output);
		return(maxsize);
	}
	return(output);
}

/**
 * oneinit - get one initializer expression or constant
 *      This checks the type of the data item expected against the
 *      type obtained from expr.  
**/
oneinit(type,dp,sc)                 /* returns size of initializer*/
int type;                           /* type of initializer*/
int dp, sc;							/* [vlh] 4.2.c add for bit field init */
{
    register short op, plus, ivalue;
    register struct tnode *tp;
    register long value;

    commastop++;
#ifdef DEBUG
	if (symdebug) printf("oneinit: type = %d\n",type);
#endif
    if ((tp = (struct tnode *)expr(0)) == 0) {
        error("invalid initializer");
		commastop--;
        return(0);
    }
    commastop--;

    if ((op = tp->t_op) == CINT)
        clvalue = value = ((struct conode *)tp)->t_value; 
	else if (op == CLONG)
		clvalue = value = ((struct lconode *)tp)->t_lvalue; 
	else
		value = clvalue; 

   	if (sc == BFIELDCL)
		return(one_binit(type,dp,sc,value));

	ivalue = value;
	plus = (in_bitinit) ? out_bfield(type) : 0;

    switch( ISALLTYPE(type) ) {

        case UCHAR:
            if( op == CINT || op == CLONG ) {	/* [vlh] 4.2, added CLONG */
				outc(CHAR,((int)(ivalue&0xff)));
				if ((ivalue&0xff00) && ((ivalue&0xff00) != 0xff00))
					warning("initializer truncated");
				return(1+plus);
			}
			if( op == ADDR ) {	/* [vlh] 4.2, STRING.... */
				outc(CHAR,((int)(*cstr&0xff)));
				warning("string used to initialize character value");
				return(1+plus);
			}
			break;

        case CHAR:
            if( op == CINT || op == CLONG ) {	/* [vlh] 4.2, added CLONG */
				outc(CHAR,((int)ivalue)&0xff);
#ifdef DEBUG
	if (symdebug) printf("ivalue %d\n",ivalue);
#endif
				if ((ivalue>255) || (ivalue<-128))
					warning("initializer truncated");
				return(1+plus);
			}
			if( op == ADDR ) {	/* [vlh] 4.2, STRING.... */
				outc(CHAR,((int)*cstr)&0xff);
				warning("string used to initialize character value");
				return(1+plus);
			}
			break;

        case ARRAY|CHAR:
            if( op == CINT || op == CLONG ) {	/* [vlh] 4.2, added CLONG */
                if (ccbytes < 2) {
					outc(CHAR,((int)ivalue)&0xff);
					if ((ivalue>255) || (ivalue<-128))
						warning("initializer truncated");
					return(1+plus);
				}
				else {	/* 2 character  charconst eg. 'ab' */
					outc(INT,ivalue);
					return(2+plus);
				}
            }
            break;
    
        case INT:
        case ARRAY|INT:
			if (op == ADDR)
				break;
            if( op == CINT || op == CLONG )	{ /* [vlh] 4.2, added CLONG */
                outc(INT,(int)value);
				if ((value&0xffff0000) && ((value&0xffff0000) != 0xffff0000))
					warning("initializer truncated");
            }
			else
                outinit(tp,inittype);
            return(2+plus);
    
        case UNSIGNED:  /* [vlh] 4.0 split from INT... */
        case USHORT:  	/* [vlh] 4.2 added... */
        case ARRAY|UNSIGNED:
            if (op == ADDR)
                break;
            if( op == CLONG || op == CINT ) {	/* [vlh] 4.2, rewrote */
				if ((value&0xffff0000) && ((value&0xffff0000) != 0xffff0000))
					warning("initializer truncated");
                ivalue = value & 0xffff;
                outc(INT,ivalue);
            }
			else
                outinit(tp,inittype);
            return(2+plus);
    
        case LONG:
        case ULONG:
        case ARRAY|LONG:
        case POINTER|CHAR:
        case POINTER|UCHAR:
        case POINTER|INT:
        case POINTER|LONG:
        case POINTER|ULONG:
        case POINTER|STRUCT:
        case POINTER|UNSIGNED:
            if( op==CINT || op==CLONG )
                outlong(value);
            else
                outinit(tp,inittype);
            return(4+plus);
    
        case DOUBLE:    /* [vlh] 3.4 */
        case FLOAT:
            outfp(value);
            return(4+plus);
    
    }
    error("invalid initializer type=%d",ISALLTYPE(type));
    return(plus);
}

/**
 *	one_binit - individula element bit field initialization. [vlh] 4.2
 *		returns the number of bytes actually initialize 1, 2 or 0
**/
one_binit(type,dp,sc,value)
int type,dp,sc;
long value;
{
	register short plus, mask, op;

	op = (BTYPE(type) == CHAR) ? BITSPCHAR : BITSPWORD;
	op = (dp>>8)%op;
	if (in_bitinit && ((bfield_ty != BTYPE(type)) || (op < lst_boffset)))
		plus = out_bfield(type);
	else
		plus = 0;
	lst_boffset = op;
	bfield_ty = BTYPE(type);
	mask = (1 << (dp & 0xff)) - 1;
	bits_init |= ((value & mask) << op);
	in_bitinit++;
	return(plus);
}

/* out_bfield - last item was a bit field output it [vlh] 4.2 */
out_bfield(type)	/* return number of bytes output */
int type;
{
	in_bitinit = 0;
	if (bfield_ty == CHAR) {	/* char bit field type */
		outc(CHAR,(int)bits_init);
		bits_init = 0;
		if (ISALLTYPE(type) != CHAR)	/* returns 2 */
			OUTEVEN();
		else
			return(1);
	}
	else {				/* int/unsigned bit field type */
		outc(INT,(int)bits_init);
		bits_init = 0;
	}
	return(2);
}
