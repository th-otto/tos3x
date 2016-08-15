/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"

/*
 * doextdef - external definition syntax
 *      This is fairly complicated since you do not know if you are
 *      parsing a external function declaration or a real function
 *      until after you've already scanned the argument list for the
 *      function.  Basically you start off scanning an external declaration
 *      or function in the same way by collecting attributes, scanning
 *      the declarator, then scanning the function arguments if a function.
 *      At that point you look at the next token and if its a '{', keyword
 *      proceed accordingly.
 *      The C Syntax handled by this routine is (roughly):
 *          external_definition:
 *                  function_definition
 *                  data_definition
 *          function_definition:
 *                  type_specifier function_declarator function_body
 *          function_declarator:
 *                  declarator ( parameter_list )
 *          data_definition:
 *                  EXTERNAL type_specifier init_declarator_list ;
 *                  STATIC type_specifier init_declarator_list ;
 */
doextdef()                          /* returns 0 for EOF or 1*/
{
    register struct symbol *sp;
    register short dflag;
    short sc, type;
    long size;

	tdflag = 0;	/* [vlh] 4.2.b reset on sighting a semicolon */
    if(!next(SEMI)) {
        opap = exprp = exprarea;
        sc = EXTERNAL;
        type = (xflag?LONG:INT);
        dflag = gettype(&sc,&type,&size,1);
        if(type==STRUCT)    /* deal with forward ref structures */
            chksyms(0);
        while( dodecl(sc,type,0,size), (sp=dsp) != 0 ) {
			if (type==STRUCT) {		/* [vlh] 4.2 */
				sp->s_par = struc_parent[0];
				sp->s_child = struc_parent[0];
			}
            if( !dflag && NOTFUNCTION(sp->s_type) ) {
                synerr("external definition syntax");
                return;
            }
            if( !ISTYPEDEF(sp) && sc != STATIC )
                if (NOTFUNCTION(sp->s_type))   /*[vlh] .globl ext. vars*/
                    OUTEXTDEF(sp->s_symbol);
            if( NOTFUNCTION(sp->s_type) ) { /*not function, check init*/
                if( !ISTYPEDEF(sp) ) {
                    doinit(sp);
                    if (sc == STATIC)
                        chksyms(0);
                }
            }
            else if( PEEK(RESWORD) || PEEK(LCURBR) ||
                    (PEEK(SYMBOL) && ISTYPEDEF(csp)) ) {
                if(!ISTYPEDEF(sp) && sc!=STATIC)  /*[vlh] .globl local proc*/
                    OUTEXTDEF(sp->s_symbol);
                funcbody(sp);
                return;
            }
            dsp = 0;
            if( !next(COMMA) )
                break;
        }
        if( gettok(0) != SEMI ) {
#ifdef DEBUG
	if (symdebug) printf("external definition syntax due to lost comma...\n");
#endif
            synerr("external definition syntax");
		}
    }
}

/**
 * gettype - get attribute types in attribute list
 *      Handles single word keywords, such as int, char, etc. and also
 *      handles the declarations of structures and unions.
**/
gettype(defsc,deftype,size,declok)  /* returns 0 for no type, 1 otherwise*/
short *defsc;                         /* default storage class*/
short *deftype;                       /* default data type*/
long *size;                           /* size of data element 3.4 int=>long*/
int declok;                         /* as opposed to casting op */
{
    register short token, sc;
    short dtype, sflag, uflag, lflag, decflag;
    long tsize;
    struct symbol *sp, *parent;

    tdp = 0; uflag = decflag = lflag = sflag = 0; tsize = 0L;
    dtype = TYPELESS;
    sc = *defsc;
	indecl = 0;		/* start off at 0 !!!! */
    for( ; ; decflag++ ) {
        if( (token=gettok(0)) == SYMBOL && ISTYPEDEF(csp) ) {
            dtype = 0;
			indecl = declok;    /* if not trying for casting operator */
            tdp = csp;
            continue;
        }
        if( token != RESWORD )
            break;
        indecl = declok;    /* if not trying for casting operator */
        switch( cvalue ) {

        default:        /* not a declaration type reserved word */
            indecl = 0;
            break;
            
        case R_TYPEDEF:
            if( tdflag )
                error("invalid typedef statement");
            tdflag++;
            continue;

        case R_STATIC:
            if( sc && sc != STATIC && sc != EXTERNAL )
                error("invalid storage class");
            sc = STATIC;
            continue;

        case R_AUTO:
            if( sc && sc != AUTO )
                error("invalid storage class");
            sc = AUTO;
            continue;

        case R_EXTERNAL:
            if( sc && sc != EXTERNAL )
                error("invalid storage class");
            sc = EXTERNAL;
            continue;
    
        case R_REGISTER:
            if( sc && sc != REGISTER && sc != PDECLIST )
                error("invalid register specification");
            sc = REGISTER;
            continue;
    
        case R_LONG:
            lflag++;
            continue;

        case R_SHORT:
            sflag++;
            continue;

        case R_UNSIGNED:
            uflag++;
            continue;

        case R_STRUCT:
            cvalue = STRUCT;
        case R_UNION:
			token = get_s_or_u(sp,&parent,&tsize,&dtype);
			if (token != FRSTRUCT)	/* [vlh] 4.2.e */
				struc_parent[0] = parent;
            continue;

        case R_INT:
            if( dtype != TYPELESS )
                error("invalid type declaration");
            dtype = INT;
            continue;

        case R_CHAR:
            if( dtype != TYPELESS )
                error("invalid type declaration");
            dtype = CHAR;
            continue;

        case R_FLOAT:   /*[vlh] ver. 3.4*/
        case R_DOUBLE:
            if( dtype != TYPELESS )
                error("invalid type declaration");
            dtype = FLOAT;
            continue;
        }
        break;
    }
    pbtok(token);
    if( dtype == TYPELESS )
        dtype = INT;
    if(!sc)
        sc = AUTO;
    if( lflag ) {                   /*allow: long float, long int*/
        if( dtype == INT )
            dtype = LONG;
        else if( dtype == FLOAT )
            dtype = DOUBLE;
        else
            error("invalid long declaration");
    }
    if( sflag ) {
        if( dtype != INT )
            error("invalid short declaration");
    }
    if( uflag )	/* [vlh] 4.2 legal from now on... */
        if (dtype == LONG) {
			dtype = LONG;	
			warning("unsigned long unimplemented, signed long assumed");
		}
		else if (dtype == CHAR) {
			dtype = CHAR;
			warning("unsigned char unimplemented, signed char assumed");
		}
		else
			dtype = UNSIGNED;
    if( !sflag && xflag && dtype == INT )
        dtype = LONG;
    *defsc = sc;
    *deftype = dtype;
    *size = tsize;
    return(decflag);
}

/**
 * get_s_or_u - get attribute types from a union or structure declaration
 *		This routine parses a structure or union.  It is called by
 *		"gettype" and returns the token parsed.
**/
get_s_or_u(sp,parent,ptsize,pdtype)			/* returns token */
struct symbol *sp, **parent;
long *ptsize;
short *pdtype;
{
	char sym[8];
	register short token, stdflag, sbits, fake;

	stdflag = tdflag;
	tdflag = 0; sp = 0;
	token = cvalue;
	smember++;
	if (!next(SYMBOL)) { /* force fake struct name into symbol table */
		fake=1;
		sym[0] = ' ';	/* no symbol could start with space */
		genunique(sym);
		csp = (struct symbol *)lookup(sym,1);	/* [vlh] 4.2 */
	}
	else
		fake=0;
	/*struct [symbol] { ... }*/
	sp = csp;
	*parent = csp;
	if( !sp->s_sc ) {
		sp->s_attrib |= SDEFINED;
		if(!infunc)
			sp->s_attrib |= SGLOBAL;
		sp->s_sc = STRPROTO;
		sp->s_type = STRUCT;
		sp->s_ssp = dalloc(0L);
	}
	if( sp->s_sc != STRPROTO )
		error("redeclaration: %.8s",sp->s_symbol);
	smember = 0;
	if( next(LCURBR) ) {
		struc_sib[in_struct] = hold_sib;	/* [vlh] 4.2.e */
		if (hold_sib) {	/* [vlh] 4.2, not struct element yet... */
			hold_sib->s_sib = hold_sib->s_par = 0;
		}
		in_struct++;
		struc_parent[in_struct] = sp;
		struc_sib[in_struct] = 0;
		sbits = boffset;
		boffset = 0;
		*ptsize = dlist(token==STRUCT?STELCL:UNELCL);
		boffset = sbits;
		if(!next(RCURBR))
			synerr("structure declaration syntax");
		else if( sp ) {
			if( dtab[sp->s_ssp] )
				error("redeclaration: %.8s",sp->s_symbol);
			dtab[sp->s_ssp] = *ptsize;
		}
		struc_parent[in_struct] = 0;
		struc_sib[in_struct] = 0;
		--in_struct;
		if (!in_struct)
			hold_sib = 0;
	}
	else if (fake)
		error("no structure name");
	else if( sp->s_sc != STRPROTO )
		error("invalid structure prototype: %.8s",sp->s_symbol);
	else if( !dtab[sp->s_ssp] ) {   /* FRSTRUCT */
		if (struc_sib[in_struct]->s_type == STRUCT) {	/* [vlh] 4.2.e */
			struc_sib[in_struct] = hold_sib;
			if (hold_sib)
				hold_sib->s_sib = hold_sib->s_par = 0;
		}
		token = FRSTRUCT;
		if( ++frstp >= NFRSTR )
			ferror("structure table overflow");
		frstab[frstp] = sp;
	}
	else
		*ptsize = dtab[sp->s_ssp];
	tdflag = stdflag;
	if( *pdtype != TYPELESS )
		error("invalid type declaration");
	*pdtype = (token == R_UNION) ? STRUCT : token;
	return(token);
}

/*
 * dodecl - process a single declarator
 *      This does all the grubby handling of a single declarator given
 *      the attributes for the declarator.  Handles typedef attributes
 *      adjusts offsets for structure elements, allocates register
 *      variables, etc.
 */
long                                /* [vlh] 3.4 short => long */
dodecl(sc,type,offset,size)         /* returns size of declarator*/
int sc;                             /* storage class*/
int type;                           /* data type*/
int offset;                         /* offset if in structure or union*/
long size;                          /* size of single data item 3.4 i=> l*/
{
    register struct symbol *sp;
    register short dtype, j;
    long constval;

    if( PEEK(SEMI) || PEEK(RPAREN) )
        return(0);
    if( in_struct && next(COLON) ) { /*handle bit filler field*/
        if(!(constval=cexpr()))
            size = salign(INT,offset);
		else
            size = falign(type,(int)constval,offset);
    }
    else if( (type |= declarator(0)) >= 0 && (sp=dsp) != 0 ) {
		if (ISFUNCTION(type))			/* [vlh] 4.2 */
			sp->s_scope = GLOB_SCOPE;
        if (tdp) {                /*typedef name in declaration*/
            type = addtdtype(tdp,type,sp->s_dp,&(sp->s_ssp));
			if (BTYPE(type)==STRUCT) /* [vlh] 4.2 */
				sp->s_child = sp->s_par = tdp->s_par;
		}
        else if( BTYPE(type) == STRUCT ) {
			sp->s_par = struc_parent[0];	/* [vlh] 4.2 */
            if( size )
                sp->s_ssp = dalloc(size);
			else
                error("invalid structure declaration: %.8s",sp->s_symbol);
        }
        else if( BTYPE(type) == FRSTRUCT )
            sp->s_ssp = frstp;
        switch( sp->s_sc ) {        /*check for redeclarations.*/

			case STELCL:
			case UNELCL:
			case 0:
				break;

			case PARMLIST:
				if( sc != PDECLIST && sc != REGISTER )
					goto redec;
				break;

			case BFIELDCL:
				if( sc != STELCL && sc != UNELCL )
					goto redec;
				break;

			case STATIC:		/* [vlh] 4.2, incomplete handling */
				if (ISFUNCTION(sp->s_type))
					break;
				goto redec;

			case EXTERNAL:
				if (sp->s_type == type) {
					if (sc == sp->s_sc) 
						break;
					if (ISFUNCTION(sp->s_type) && sc == STATIC) /* [vlh] 4.2 */
						break;
					if (sc == AUTO && SUPTYPE(type) == FUNCTION) {
						sc = EXTERNAL;
						break;
					}
				}
			default:
redec:
#ifdef DEBUG
	if (treedebug) 
		printf("redec typ %d %d sc %d %d\n",sp->s_type,type,sp->s_sc,sc);
#endif
				error("redeclaration: %.8s",sp->s_symbol);
				return(size);
        }
        sp->s_type = type;
        dtype = SUPTYPE(type);
        type = BTYPE(type);
        if( tdflag )                /*we are declaring typedef?*/
            sp->s_attrib |= STYPEDEF;
        if( in_struct ) {
            if( next(COLON) ) {     /*handle bit field*/
                sc = BFIELDCL;
                constval = cexpr();
                sp->s_dp = (boffset<<8) | ((int)constval);
                size = j = falign(type,(int)constval,offset);
				if (j)	/* [vlh] 4.2 implies move to new word.... */
					sp->s_dp = (int)constval;
            }
            else {
                j=salign(sp->s_type,offset); /* [vlh] 4.1 type is not enough */
                size = dsize(sp->s_type,sp->s_dp,sp->s_ssp) + j;
            }
            offset += j;
            sp->s_offset = offset;
        }
        if( dtype == FUNCTION ) {
            if( sc != AUTO && sc != EXTERNAL && sc != STATIC )
                error("illegal function declaration");
            if( sc != STATIC )
                sc = EXTERNAL;
        }
        else if( sc == REGISTER ) {
            if( !dtype ) {
                if( !(dinfo[type]&DREG) || !dregtab[ndregs] )
                    sc = AUTO;/* ignore reg specification */
                else
                    sp->s_offset = dregtab[ndregs++];
            }
            else if( !aregtab[naregs] || dtype!=POINTER )
                sc = AUTO;          /*no more regs, make it auto*/
            else
                sp->s_offset = aregtab[naregs++];
        }
        if( sc == AUTO ) {
            localsize += WALIGN(dsize(sp->s_type,sp->s_dp,sp->s_ssp));
            sp->s_offset = -localsize;
        }
        else if( sc == STATIC )
            sp->s_offset = nextlabel++;
        sp->s_sc = sc;
        sp->s_attrib |= SDEFINED;
        if(!infunc)
            sp->s_attrib |= SGLOBAL;
		/* trying to output locals in the appropriate order.... */
		if (infunc && scope_level==FUNC_SCOPE)
			if (sc==STATIC || sc==REGISTER || sc==AUTO)
				outlocal(type,sc,sp->s_symbol,sp->s_offset);
    }
    return(size);
}

/*
 * funcbody - do function body declaration
 *      Basically handles function after we have scanned the parameter
 *      list, which is now set up in fargs array.  We now proceed to
 *      look for any declarations for the function arguments, then the
 *      function declaration list, followed by the function statement list.
 *      The C syntax is:
 *          function_body:
 *                  type_decl_list function_statement
 *          function_statement:
 *                  { declaration_list statement_list }
 */
funcbody(fsp)
struct symbol *fsp;
{
    register short olddp, offset, toff;
    register struct symbol *sp;
    register struct farg *fp;

    infunc++;
    sp = fsp;
    opap = exprp;
    frp = (struct farg *)snalloc(delsp(sp->s_type),sp->s_sc,sp->s_type,
                                      sp->s_dp, sp->s_ssp);
    exprp = opap;
    OUTTEXT();
    OUTFLAB(sp->s_symbol);
    olddp = cdp;
    dlist(PDECLIST);
    rlabel = nextlabel++;
    if(!next(LCURBR))
        synerr("function body syntax");
    else {
        scope_level = FUNC_SCOPE;       /* [vlh] 4.2 */
        scope_decls[FUNC_SCOPE] = 1;    /* [vlh] 4.2, force at this level */
        localsize = 0;          		/*end of first auto offset from l.e.p.*/
        offset = 8;             		/*first arg offset from l.e.p.*/
        for( fp = &fargtab[0]; sp = fp->f_sp; fp++ ) {
            toff = offset;
            if( sp->s_type == CHAR )    /*char argument*/
                toff++;         /*offset of lower byte in word*/
            if( sp->s_sc == REGISTER )
                fp->f_offset = toff;
            else {
                fp->f_offset = 0;   /*really is auto arg*/
                sp->s_offset = toff;
                sp->s_sc = AUTO;
            }
            if( ISARRAY(sp->s_type) ) { /*change array ref to pointer*/
                sp->s_type = addsp(delsp(sp->s_type),POINTER);
                sp->s_dp++;
            }
            offset += WALIGN(dsize(sp->s_type,sp->s_dp,sp->s_ssp));
				/* [vlh] 4.2, output argument list for debugger */
			if (sp->s_sc==STATIC || sp->s_sc==AUTO)	/* [vlh] not register... */
				outlocal(sp->s_type,sp->s_sc,sp->s_symbol,sp->s_offset);
        }
        OUTBENTRY();		/* [vlh] 4.2, must be before declarations */
        dlist(TYPELESS);    /* [vlh] 4.1 was just a zero... */
        chksyms(0);
        copyargs();         /*copy args to registers where required*/
        while(!next(RCURBR)) {
            if( next(EOF) ) {
                error("{ not matched by }");
                break;
            }
            instmt=1;
            stmt();
            instmt=0;
        }
    }
    OUTLAB(rlabel);
    outbexit(localsize,ndregs,naregs);
    freesyms(FUNC_SCOPE);
    cdp = olddp;
    infunc--;
}

/*
 * copyargs - copy args to register where required
 *      fargtab has been set so that args declared to be registers have a
 *      non-zero offset value and the register number is in the symbol
 *      table pointed to by symbol.
 */
copyargs()                          /* returns - none*/
{
    register struct symbol *sp;
    register struct farg *fp;

    for( fp = &fargtab[0]; sp = fp->f_sp; fp++ ) {
        if( fp->f_offset )      /*was declared register*/
            outassign(snalloc(sp->s_type,sp->s_sc,sp->s_offset,0,0),
                    snalloc(sp->s_type,AUTO,fp->f_offset,0,0));
    }
}

/*
 * dlist - declaration list
 *      Handles declaration lists in the following places:
 *      function parameter list declarations, structure or union member
 *      declarations and local declarations in functions.
 */
long
dlist(defsc)                        /* returns length of declarators*/
int defsc;                          /* default storage class*/
{
    register short offset;
    register long lret, ddsize;
    struct tnode *tp;
    char *p;
    long size;  /* [vlh] 3.4 short => long */
    short type, sc;

    offset = 0; ddsize = 0L;
    do {
        sc = defsc;
        type = INT;
        if( !gettype(&sc,&type,&size,1) )
            break;
        indecl = 1;
        do {
            lret = dodecl(sc,type,offset,size);
            if( defsc != UNELCL ) {
                offset += (int)lret;
                ddsize += lret;
            }
            else if( lret > ddsize )
                ddsize = lret;
            if( sc == STATIC && dsp && !ISTYPEDEF(dsp) )
                doinit(dsp);        /*process any initializer*/
            dsp = 0;
        
            if (next(ASSIGN)) { /* [vlh] 4.2 auto initialization */
                indecl = 0;     /* don't redeclare expr vars */
                if (!SIMPLE_TYP(csp->s_type) && NOTPOINTER(csp->s_type)) {
                    synerr("illegal autoinitialization data type");
                    break;
                }
                peektok = ASSIGN;
                commastop++;    /* stop initializing at a comma */
                expr_setup();   /* setup expr op stack */
                p = get_symbol();
                if (doopd(p))
                    synerr("auto initilization syntax");
                else
                    if ((tp = (struct tnode *)expr(1)) != 0)
                        outexpr(tp);
                    else
                        synerr("auto initialization syntax");
                commastop--;    /* back to previous comma handling */
                indecl = 1;
            }
        } while ( next(COMMA) );
        if(!next(SEMI)) {
            synerr("declaration syntax");
			tdflag = 0;	/* [vlh] 4.2.b reset on sighting a semicolon */
            break;
        }
		tdflag = 0;	/* [vlh] 4.2.b reset on sighting a semicolon */
    } while( 1 );
    ddsize += salign(INT,(int)ddsize);
    return(ddsize);
}

/*
 * declarator - get one declarator
 *      Basically uses getdecl, which returns the declaration types
 *      reversed in the type word.
 */
declarator(castflg)                         /* returns type or -1*/
int castflg;
{
    short type;

    dsp = 0;
    if( (type=getdecl(castflg)) >= 0 )
        return( revsp(type) );
    return(type);
}

/*
 * getdecl - get one declarator, handling *, (), etc.
 *      The significance of the value returned by declarator is:  the
 *      least significant two bits represent the values (POINTER,FUNCTION,
 *      ARRAY), these values are repeated through the word.  For example,
 *      the declarations result in the following values for declarator:
 *              *x() => (POINTER,FUNCTION)
 *              (*x)() => (FUNCTION,POINTER)
 *              *(*x)() => (POINTER,FUNCTION,POINTER)
 *      The following C syntax is handled here:
 *          function_declarator:
 *                  declarator ( parameter_list )
 *          declarator:
 *                  identifier
 *                  ( declarator )
 *                  * declarator
 *                  declarator [ constant-expression ]
 */
getdecl(castflg)                /* returns special type of declarator*/
int castflg;                    /* casting flag, 1=>allow no declarator*/
{
    register short type, i, sdp;
    register struct symbol *sp, *tsp, *p;
    register struct farg *fp;
    long lvalue, value;

    type = 0;
    if( next(LPAREN) ) {            /*( declarator ) ...*/
        type = getdecl(castflg);
        if(!next(RPAREN))
            goto baddec;
    }
    if( next(MULT) )
        return(addsp(getdecl(castflg),POINTER));
    sdp = cdp;
    if( next(SYMBOL) ) {
        sp = dsp = csp;
        type = 0;
        sp->s_dp = sdp;
    }
    while( 1 ) {
        if( next(LPAREN) ) {        /*declarator ( ... )*/
            if(!infunc) {
                ndregs = naregs = 0;
                scope_level = FUNC_SCOPE;       /* [vlh] 4.2 */
                for( fp = &fargtab[0]; pnext(); ) {
                    p = csp;
                    if( p->s_attrib & SDEFINED )
                        error("redeclaration: %.8s",p->s_symbol);
                    else if( fp >= &fargtab[NFARGS-1] ) {
                        synerr("too many parameters");
                        break;
                    }
                    else {
                        p->s_attrib |= SDEFINED;
                        p->s_scope = FUNC_SCOPE;
                        p->s_sc = PARMLIST;
                        p->s_type = INT;    /*default to int*/
                        fp->f_sp = p;
                        fp++;
                    }
                    if(!next(COMMA))
                        break;
                }
                fp->f_sp = 0;
            }
            if(!next(RPAREN))
                break;
			if (!infunc && fp == &fargtab[0])       /* [vlh] 4.2 */
				scope_level = GLOB_SCOPE;
            type = addsp(type,FUNCTION);
            continue;
        }
        if( next(LBRACK) ) {    /*declarator [ cexpr ]*/
            if( next(RBRACK) )
                dalloc(1L);
			else {
                tsp = dsp;          /* [vlh] 3.4 save in case of reset */
                value = cexpr();    /* recurses on sizeof.... resets dsp */
				if (dsp != tsp)		/* [vlh] 4.2.... */
					tsp->s_dp = cdp;
                dsp = tsp;          /* [vlh] 3.4 */
                lvalue = value;     /* [vlh] 3.4 */
                for( i = sdp; i < cdp; i++)
                    dtab[i] *= lvalue;
                dalloc(lvalue);
                if( !next(RBRACK) )
                    break;
            }
            type = addsp(type,ARRAY);
            continue;
        }
        if( castflg || dsp ) {
			if (!infunc && castflg)
				scope_level = GLOB_SCOPE;       /* [vlh] 4.2 */
            return(type);
		}
        break;
    }
baddec:
    synerr("invalid declarator");
    return(-1);
}

/*
 * addtdtype - add typedef info into declarator
 *      here we very carefully add in the dimensions for an array typedef
 *      declaration.  Note that declarator has already allocated the
 *      declarator-specific dimensions, now we allocate the typedef
 *      dimensions and adjust the size of the declarator's dimensions.
 *      Note that this must be done before the dalloc for the structure,
 *      otherwise we would mix up array and structure sizes.
 */
addtdtype(tddp,type,dp,ssp)         /* returns type*/
struct symbol *tddp;
int type;
int dp;
int *ssp;
{
    register short ntype, t, i, tdf;

    for( tdf = 0, t = tddp->s_type; SUPTYPE(t); t = delsp(t) )
        if( ISARRAY(t) ) {
            tdf++;
            break;
        }
    ntype = 0;
    for( t = type, i = dp; SUPTYPE(t); t = delsp(t) ) {
        ntype = addsp(ntype,t);
        if( tdf && ISARRAY(t) ) {
            dtab[i] *= dtab[tddp->s_dp];
			i++;
		}
    }
    for( t = tddp->s_type, i = tddp->s_dp; SUPTYPE(t); t = delsp(t) )
        if( ISARRAY(t) )
            dalloc(dtab[i++]);
    for( t = tddp->s_type; SUPTYPE(ntype); ntype = delsp(ntype) )
        t = addsp(t,ntype);
    if( (ntype=BTYPE(t)) == STRUCT )
        *ssp = tddp->s_ssp;
    else if( ntype == FRSTRUCT )
        *ssp = frstp;
    return(t);
}
