/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121

	@(#)decl.c	1.13	1/3/84
*/

#include "parser.h"

long dodecl PROTO((int sc, int type, int offset, long size));
short get_s_or_u PROTO((struct symbol *sp, struct symbol **parent, long *ptsize, short *pdtype));
VOID funcbody PROTO((struct symbol *fsp));
VOID copyargs PROTO((NOTHING));



/* PNEXT - if next token is a symbol, skip and return success, allows */
/*         for clean parsing of declarations */
#define PNEXT()	next(SYMBOL)

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

VOID doextdef(NOTHING)
{
	register struct symbol *sp;
	register short dflag;
	short sc, type;
	long size;

	tdflag = 0;							/* reset on sighting a semicolon */
	if (!next(SEMI))
	{
		opap = exprp = exprarea;
		sc = EXTERNAL;
		type = (xflag ? LONG : INT);
		dflag = gettype(&sc, &type, &size, 1);
#ifdef DEBUG
		if (symdebug)
			printf("dflag %d sc %d type 0%o size %d\n", dflag, sc, type, size);
#endif
		if (type == STRUCT)				/* deal with forward ref structures */
			chksyms(0);
		while (dodecl(sc, type, 0, size), (sp = dsp) != 0)
		{
			if (type == STRUCT)
			{
				sp->s_child = sp->s_par = struc_parent[0];
				TO_DSK(sp, sp_addr);
				READ_ST(dsp, dsp_addr);
			}
			if (!dflag && NOTFUNCTION(sp->s_type))
			{
				synerr("external definition syntax");
				return;
			}
			if (!ISTYPEDEF(sp) && sc != STATIC)
				if (NOTFUNCTION(sp->s_type))	/* .globl ext. vars */
					OUTEXTDEF(sp->s_symbol);	/* BUG: causes assembler to spit out unresolved reference */
			if (NOTFUNCTION(sp->s_type))
			{							/* not function, check init */
				if (!ISTYPEDEF(sp))
				{
					doinit(sp);
					if (sc == STATIC)
						chksyms(0);
				}
			} else if (PEEK(RESWORD) || PEEK(LCURBR) || (PEEK(SYMBOL) && ISTYPEDEF(csp)))
			{
				if (!ISTYPEDEF(sp) && sc != STATIC)	/* .globl local proc */
					OUTEXTDEF(sp->s_symbol);
				funcbody(sp);
				return;
			}
			ZERO_DSP();
			if (!next(COMMA))
				break;
		}
		if (gettok(0) != SEMI)
		{
#ifdef DEBUG
			if (symdebug)
				printf("external definition syntax due to lost comma...\n");
#endif
			synerr("external definition syntax");
		}
	}
}


/*
 * gettype - get attribute types in attribute list
 *      Handles single word keywords, such as int, char, etc. and also
 *      handles the declarations of structures and unions.
 * returns 0 for no type, 1 otherwise
 */
short gettype(P(short *) defsc, P(short *) deftype, P(long *) size, P(int) declok)
PP(short *defsc;)							/* default storage class */
PP(short *deftype;)							/* default data type */
PP(long *size;)								/* size of data element 3.4 int=>long */
PP(int declok;)								/* as opposed to casting op */
{
	register short token, sc;
	short dtype, sflag, uflag, lflag, decflag;
	long tsize;
	struct symbol *sp;
	struct symbol *parent;

	if (declok || instmt)
		tdp = 0;
	uflag = decflag = lflag = sflag = 0;
	tsize = 0L;							/* tdp=0 4.3 removed */
	dtype = TYPELESS;
	sc = *defsc;
	indecl = 0;							/* start off at 0 !!!! */
#ifndef __ALYCON__
	sp = NULL;
#endif
	for (;; decflag++)
	{
		if ((token = gettok(0)) == SYMBOL && ISTYPEDEF(csp))
		{
			dtype = 0;
			indecl = declok;
			if (declok || instmt)		/* tdp not used in initialization cast */
				tdp = csp;
			continue;
		}
		if (token != RESWORD)
			break;
		indecl = declok;				/* if not trying for casting operator */
		switch (cvalue)
		{
		default:						/* not a declaration type reserved word */
			indecl = 0;
			break;

		case R_TYPEDEF:
			if (tdflag)
				error("invalid typedef statement");
			tdflag++;
			continue;

		case R_STATIC:
			if (sc && sc != STATIC && sc != EXTERNAL)
				error("invalid storage class");
			sc = STATIC;
			continue;

		case R_AUTO:
			if (sc && sc != AUTO)
				error("invalid storage class");
			sc = AUTO;
			continue;

		case R_EXTERNAL:
			if (sc && sc != EXTERNAL)
				error("invalid storage class");
			sc = (scope_level == GLOB_SCOPE) ? DEXTERN : EXTERNAL;
			continue;

		case R_REGISTER:
			if (sc && sc != REGISTER && sc != PDECLIST && sc != PDECREG)
				error("invalid register specification");
			sc = (sc != PDECLIST) ? REGISTER : PDECREG;
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
			/* BUG: sp never assigned */
			token = get_s_or_u(sp, &parent, &tsize, &dtype);
			if (token != FRSTRUCT)
				struc_parent[0] = parent;
			continue;

		case R_INT:
			if (dtype != TYPELESS)
				error("invalid type declaration");
			dtype = INT;
			continue;

		case R_CHAR:
			if (dtype != TYPELESS)
				error("invalid type declaration");
			dtype = CHAR;
			continue;

		case R_FLOAT:
		case R_DOUBLE:
			if (dtype != TYPELESS)
				error("invalid type declaration");
			dtype = FLOAT;
			continue;
		}
		break;
	}
	pbtok(token);
	if (dtype == TYPELESS)
		dtype = INT;
	if (!sc)
		sc = AUTO;
	if (lflag)
	{									/* allow: long float, long int */
		if (dtype == INT)
			dtype = LONG;
		else if (dtype == FLOAT)
			dtype = DOUBLE;
		else
			error("invalid long declaration");
	}
	if (sflag && dtype != INT)
		error("invalid short declaration");
	if (uflag)
	{
		if (dtype == LONG)
		{
			dtype = LONG;
			warning("unsigned long unimplemented, signed long assumed");
		} else if (dtype == CHAR)
		{
			dtype = CHAR;
			warning("unsigned char unimplemented, signed char assumed");
		} else if (dtype == INT)
		{
			dtype = UNSIGNED;
		} else
		{
			error("invalid unsigned declaration");
		}
	}
	if (!sflag && xflag && dtype == INT)
		dtype = LONG;
	*defsc = sc;
	*deftype = dtype;
	*size = tsize;
	return decflag;
}


/*
 * get_s_or_u - get attribute types from a union or structure declaration
 *		This routine parses a structure or union.  It is called by
 *		"gettype" and returns the token parsed.
 * returns token
 */
short get_s_or_u(P(struct symbol *) sp, P(struct symbol **) parent, P(long *) ptsize, P(short *) pdtype)
PP(struct symbol *sp;)
PP(struct symbol **parent;)
PP(long *ptsize;)
PP(short *pdtype;)
{
	char sym[8];
	register short token, stdflag, sbits, fake;

	stdflag = tdflag;
	tdflag = 0;
	sp = 0;
	token = cvalue;
	smember++;
	if (!next(SYMBOL))
	{									/* force fake struct name into symbol table */
		fake = 1;
		genunique(sym);
		csp = (struct symbol *) lookup(sym, 1);
	} else
	{
		fake = 0;
	}
	/* struct [symbol] { ... } */
	*parent = sp = csp;
	if (!sp->s_sc)
	{
		sp->s_attrib |= SDEFINED;
		if (!infunc)
			sp->s_attrib |= SGLOBAL;
		sp->s_sc = STRPROTO;
		sp->s_type = STRUCT;
		sp->s_ssp = dalloc(0L);
		TO_DSK(sp, sp_addr);
	} /* sc <-- STRPROTO */
	else if (sp->s_sc != STRPROTO)
	{
#ifdef DEBUG
		if (treedebug)
		{
			printf("redec2 typ %d sc %d ", sp->s_type, sp->s_sc);
			printf("scope %d %d\n", sp->s_scope, scope_level);
		}
#endif
		error("redeclaration: %.8s", sp->s_symbol);
	}
	smember = 0;
	if (next(LCURBR))
	{
		struc_sib[in_struct] = hold_sib;
		if (hold_sib)
		{								/* not struct element yet... */
			hold_sib->s_sib = 0;		/* not null parent !! */
		}
		in_struct++;
		struc_parent[in_struct] = sp;
		struc_sib[in_struct] = 0;
		sbits = boffset;
		boffset = 0;
		*ptsize = dlist(token == STRUCT ? STELCL : UNELCL);
		boffset = sbits;
		if (!next(RCURBR))
			synerr("structure declaration syntax");
		else if (sp)
		{
			if (dtab[sp->s_ssp])
			{
#ifdef DEBUG
				if (treedebug)
				{
					printf("redec3 typ %d sc %d ", sp->s_type, sp->s_sc);
					printf("scope %d %d\n", sp->s_scope, scope_level);
				}
#endif
				error("redeclaration: %.8s", sp->s_symbol);
			}
			dtab[sp->s_ssp] = *ptsize;
		}
		struc_parent[in_struct] = 0;
		struc_sib[in_struct] = 0;
		if (!(--in_struct))
			hold_sib = 0;
	} else if (fake)
		error("no structure name");
	else if (sp->s_sc != STRPROTO)
		error("invalid structure prototype: %.8s", sp->s_symbol);
	else if (!dtab[sp->s_ssp])
	{									/* FRSTRUCT */
		if (struc_sib[in_struct])
		{								/* don't access off of zero */
			if (struc_sib[in_struct]->s_type == STRUCT)
			{
				struc_sib[in_struct] = hold_sib;
				if (hold_sib)
				{
					hold_sib->s_sib = hold_sib->s_par = 0;
				}
			}
		}
		token = FRSTRUCT;
		if (++frstp >= NFRSTR)
			ferror("structure table overflow");
		frstab[frstp] = sp;
	} else
	{
		*ptsize = dtab[sp->s_ssp];
	}
	tdflag = stdflag;
	if (*pdtype != TYPELESS)
		error("invalid type declaration");
	*pdtype = (token == R_UNION) ? STRUCT : token;
	return token;
}


/*
 * dodecl - process a single declarator
 *      This does all the grubby handling of a single declarator given
 *      the attributes for the declarator.  Handles typedef attributes
 *      adjusts offsets for structure elements, allocates register
 *      variables, etc.
 * returns size of declarator
 */
long dodecl(P(int) sc, P(int) type, P(int) offset, P(long) size)
PP(int sc;)									/* storage class */
PP(int type;)								/* data type */
PP(int offset;)								/* offset if in structure or union */
PP(long size;)								/* size of single data item 3.4 i=> l */
{
	register struct symbol *sp;
	register short dtype, j;
	long constval;

	if (PEEK(SEMI) || PEEK(RPAREN))
		return 0;
	if (in_struct && next(COLON))
	{									/* handle bit filler field */
		if (!(constval = cexpr()))
			size = salign(INT, offset);
		else
			size = falign(type, (int) constval, offset);
	} else if ((type |= declarator(0)) >= 0 && dsp != 0)
	{
		sp = dsp;
		if (ISFUNCTION(type))
			sp->s_scope = GLOB_SCOPE;
		if (tdp)
		{								/* typedef name in declaration */
			type = addtdtype(tdp, type, sp->s_dp, &(sp->s_ssp));
			if (BTYPE(type) == STRUCT)
				sp->s_child = sp->s_par = tdp->s_par;
		} else if (BTYPE(type) == STRUCT)
		{
			sp->s_par = struc_parent[0];
			if (size)
				sp->s_ssp = dalloc(size);
			else
				error("invalid structure declaration: %.8s", sp->s_symbol);
		} else if (BTYPE(type) == FRSTRUCT)
			sp->s_ssp = frstp;
		TO_DSK(sp, sp_addr);
		READ_ST(dsp, dsp_addr);
		switch (sp->s_sc)
		{								/* check for redeclarations. */

		case STELCL:
		case UNELCL:
		case 0:
			break;

		case PARMLIST:
			if (sc != PDECLIST && sc != PDECREG)
				goto redec;
			break;

		case BFIELDCL:
			if (sc != STELCL && sc != UNELCL)
				goto redec;
			break;

		case STATIC:					/* incomplete handling */
			if (ISFUNCTION(sp->s_type))
				break;
			goto redec;

		case DEXTERN:
		case EXTERNAL:
			if (sp->s_type == type)
			{
				if (sc == sp->s_sc || sc == EXTERNAL || sc == DEXTERN)
					break;
				if (ISFUNCTION(sp->s_type) && sc == STATIC)
					break;
				if (sc == AUTO && SUPTYPE(type) == FUNCTION)
				{
					sc = EXTERNAL;
					break;
				}
			}							/* fall through...... */
		default:
		  redec:
#ifdef DEBUG
			if (treedebug)
			{
				printf("redec4 typ %d %d sc %d %d ", sp->s_type, type, sp->s_sc, sc);
				printf("scope %d %d\n", sp->s_scope, scope_level);
			}
#endif
			if (scope_level == GLOB_SCOPE)	/* extern signif to 7 */
				error("redeclaration: %.7s", sp->s_symbol);
			else
				error("redeclaration: %.8s", sp->s_symbol);
			return size;
		}
		sp->s_type = type;
		dtype = SUPTYPE(type);
		type = BTYPE(type);
		if (tdflag)						/* we are declaring typedef? */
			sp->s_attrib |= STYPEDEF;
		if (in_struct)
		{
			if (next(COLON))
			{							/* handle bit field */
				sc = BFIELDCL;
				constval = cexpr();
				sp->s_dp = (boffset << 8) | ((int) constval);
				size = j = falign(type, (int) constval, offset);
				if (j)					/* implies move to new word.... */
					sp->s_dp = (int) constval;
			} else
			{
				j = salign(sp->s_type, offset);	/* type is not enough */
				size = dsize(sp->s_type, sp->s_dp, sp->s_ssp) + j;
			}
			offset += j;
			sp->s_offset = offset;
#ifdef DEBUG
			if (symdebug)
				printf("%s <= offset %d\n", sp->s_symbol, offset);
#endif
		}
		if (dtype == FUNCTION)
		{
			if (sc != AUTO && sc != EXTERNAL && sc != DEXTERN && sc != STATIC)
				error("illegal function declaration");
			if (sc != STATIC)
				sc = EXTERNAL;
		} else if (sc == REGISTER || sc == PDECREG)
		{
			if (!dtype)
			{
				if (!(dinfo[type] & DREG) || !dregtab[ndregs])
					sc = AUTO;			/* ignore reg specification */
				else
					sp->s_offset = dregtab[ndregs++];
			} else if (!aregtab[naregs] || dtype != POINTER)
				sc = AUTO;				/* no more regs, make it auto */
			else
				sp->s_offset = aregtab[naregs++];
		}
		if (sc == AUTO)
		{
			localsize += WALIGN(dsize(sp->s_type, sp->s_dp, sp->s_ssp));
			sp->s_offset = -localsize;
		} else if (sc == STATIC)
			sp->s_offset = nextlabel++;
		sp->s_sc = sc;
		sp->s_attrib |= SDEFINED;
		if (!infunc)
			sp->s_attrib |= SGLOBAL;
		TO_DSK(sp, sp_addr);
		READ_ST(dsp, dsp_addr);
		/* trying to output locals in the appropriate order.... */
		if (infunc && scope_level == FUNC_SCOPE)
			if (sc == STATIC || sc == PDECREG || sc == REGISTER || sc == AUTO)
				outlocal(type, sc, sp->s_symbol, sp->s_offset);
	}
	return size;
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
VOID funcbody(P(struct symbol *) fsp)
PP(struct symbol *fsp;)
{
	register short olddp, offset, toff;
	register struct symbol *sp;
	register struct farg *fp;

	infunc++;
	opap = exprp;
	sp = fsp;
	/*
	 * BUG? snalloc allocates a symnode,
	 */
	frp = (struct tnode *) snalloc(delsp(sp->s_type), sp->s_sc, sp->s_type, sp->s_dp, sp->s_ssp);
	exprp = opap;
	OUTTEXT();
	OUTFLAB(fsp->s_symbol);
	olddp = cdp;
	dlist(PDECLIST);
	rlabel = nextlabel++;
	if (!next(LCURBR))
	{
		synerr("function body syntax");
	} else
	{
		localsize = 0;					/* end of first auto offset from l.e.p. */
		offset = 8;						/* first arg offset from l.e.p. */
		scope_level = FUNC_SCOPE;
		scope_decls[FUNC_SCOPE] = 1;	/* force at this level */
		for (fp = &fargtab[0]; fp->f_sp; fp++)
		{
			sp = fp->f_sp;
#ifdef DEBUG
			if (symdebug)
				printf("farg: %s type 0%o sc %d\n", sp->s_symbol, sp->s_type, sp->s_sc);
#endif
			toff = offset;
			if (sp->s_type == CHAR)		/* char argument */
				toff++;					/* offset of lower byte in word */
			if (sp->s_sc == PDECREG)
			{
				fp->f_offset = toff;
				sp->s_sc = REGISTER;
			} else
			{
				fp->f_offset = 0;		/* really is auto arg */
				sp->s_offset = toff;
				sp->s_sc = AUTO;
			}
			if (ISARRAY(sp->s_type))
			{							/* change array ref to pointer */
				sp->s_type = addsp(delsp(sp->s_type), POINTER);
				sp->s_dp++;
			}
			TO_DSK(sp, sp_addr);
			offset += WALIGN(dsize(sp->s_type, sp->s_dp, sp->s_ssp));
			/* output argument list for debugger */
			if (sp->s_sc == STATIC || sp->s_sc == AUTO)
				outlocal(sp->s_type, sp->s_sc, sp->s_symbol, sp->s_offset);
		}
		if (gflag)
			outlocal(CHAR, AUTO, "_EnD__", offset);
		offset += 2;					/* for cdb, argument end argument */
		OUTBENTRY();					/* must be before declarations */
		dlist(TYPELESS);
		chksyms(0);
#ifndef NOPROFILE
		if (profile)
			OUTPCALL(fsp->s_symbol);
#endif
		copyargs();						/* copy args to registers where required */
		while (!next(RCURBR))
		{
			if (next(CEOF))
			{
				error("{ not matched by }");
				break;
			}
			instmt = 1;
			stmt();
			instmt = 0;
		}
	}
	OUTLAB(rlabel);
	outbexit(localsize, ndregs, naregs);
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
VOID copyargs(NOTHING)
{
	register struct symbol *sp;
	register struct farg *fp;

	for (fp = &fargtab[0]; fp->f_sp; fp++)
	{
		sp = fp->f_sp;
		if (fp->f_offset)				/* was declared register */
			outassign((struct tnode *)snalloc(sp->s_type, sp->s_sc, sp->s_offset, 0, 0), (struct tnode *)snalloc(sp->s_type, AUTO, fp->f_offset, 0, 0));
	}
}


/*
 * dlist - declaration list
 *      Handles declaration lists in the following places:
 *      function parameter list declarations, structure or union member
 *      declarations and local declarations in functions.
 * returns length of declarators
 */
long dlist(P(int) defsc)
PP(int defsc;)								/* default storage class */
{
	register short offset;
	register long lret, ddsize;
	struct tnode *tp;
	struct symnode *p;
	long size;
	short type, sc;

	offset = 0;
	ddsize = 0L;
	do
	{
		sc = defsc;
		type = INT;
		if (!gettype(&sc, &type, &size, 1))
			break;
		indecl = 1;
		do
		{
			lret = dodecl(sc, type, offset, size);
			if (defsc != UNELCL)
			{
				offset += (int) lret;
				ddsize += lret;
			} else if (lret > ddsize)
				ddsize = lret;
			if (sc == STATIC && dsp && !ISTYPEDEF(dsp))
				doinit(dsp);			/* process any initializer */
			ZERO_DSP();

			if (next(ASSIGN))
			{							/* auto initialization */
				indecl = 0;				/* don't redeclare expr vars */
				READ_ST(csp, csp_addr);
				if (!SIMPLE_TYP(csp->s_type) && NOTPOINTER(csp->s_type))
				{
					synerr("illegal autoinitialization data type");
					break;
				}
				peektok = ASSIGN;
				commastop++;			/* stop initializing at a comma */
				expr_setup();			/* setup expr op stack */
				p = get_symbol();
				if (doopd((struct tnode *)p))
					synerr("auto initilization syntax");
				else if ((tp = expr(1)) != 0)
					outexpr(tp);
				else
					synerr("auto initialization syntax");
				commastop--;			/* back to previous comma handling */
				indecl = 1;
			}
		} while (next(COMMA));
		if (!next(SEMI))
		{
			synerr("declaration syntax");
			tdflag = 0;					/* reset on sighting a semicolon */
			break;
		}
		tdflag = 0;						/* reset on sighting a semicolon */
	} while (1);
	ddsize += salign(INT, (int) ddsize);
	return ddsize;
}


/*
 * declarator - get one declarator
 *      Basically uses getdecl, which returns the declaration types
 *      reversed in the type word.
 * returns type or -1
 */
int declarator(P(int) castflg)
PP(int castflg;)
{
	short type;

	ZERO_DSP();
	if ((type = getdecl(castflg)) >= 0)
		return revsp(type);
	return type;
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
 * returns special type of declarator
 */
int getdecl(P(int) castflg)
PP(int castflg;)							/* casting flag, 1=>allow no declarator */
{
	register short type, i, sdp;
	register struct symbol *sp, *tsp, *p;
	register struct farg *fp;
	long lvalue, value;

	type = 0;
	if (next(LPAREN))
	{									/* ( declarator ) ... */
		type = getdecl(castflg);
		if (!next(RPAREN))
			goto baddec;
	}
	if (next(MULT))
		return addsp(getdecl(castflg), POINTER);
	sdp = cdp;
	if (next(SYMBOL))
	{
		sp = dsp = csp;
		type = 0;
		sp->s_dp = sdp;
		TO_DSK(sp, sp_addr);
	}
	while (1)
	{
		if (next(LPAREN))
		{								/* declarator ( ... ) */
#ifndef __ALCYON__
			fp = NULL; /* quiet compiler */
#endif
			if (!infunc)
			{
				ndregs = naregs = 0;
				scope_level = FUNC_SCOPE;
				indecl++;
				for (fp = &fargtab[0]; PNEXT();)
				{
					p = csp;
					if (p->s_attrib & SDEFINED)
					{
#ifdef DEBUG
						if (treedebug)
						{
							printf("redec1 typ %d %d sc %d", p->s_type, type, p->s_sc);
							printf("scope %d %d\n", p->s_scope, scope_level);
						}
#endif
						error("redeclaration: %.8s", p->s_symbol);
					} else if (fp >= &fargtab[NFARGS - 1])
					{
						synerr("too many parameters");
						break;
					} else
					{
						p->s_attrib |= SDEFINED;
						p->s_scope = FUNC_SCOPE;
						p->s_sc = PARMLIST;
						p->s_type = INT;	/* default to int */
						fp->f_sp = p;
						fp++;
						TO_DSK(p, csp_addr);
					}
					if (!next(COMMA))
						break;
				}
				indecl--;				/* must not confuse, we are in decls */
				fp->f_sp = 0;
			}
			if (!next(RPAREN))
				break;
			if (!infunc && fp == &fargtab[0])
				scope_level = GLOB_SCOPE;
			type = addsp(type, FUNCTION);
			continue;
		}
		if (next(LBRACK))
		{								/* declarator [ cexpr ] */
			if (next(RBRACK))
				dalloc(1L);
			else
			{
				tsp = dsp;				/* 4 save in case of reset */
				value = cexpr();		/* recurses on sizeof.... resets dsp */
				if (dsp != tsp)
				{
					tsp->s_dp = cdp;
					dsp = tsp;
				}
				lvalue = value;
				for (i = sdp; i < cdp; i++)
					dtab[i] *= lvalue;
				dalloc(lvalue);
				if (!next(RBRACK))
					break;
			}
			type = addsp(type, ARRAY);
			continue;
		}
		if (castflg || dsp)
		{
			if (!infunc && castflg)
				scope_level = GLOB_SCOPE;
			return type;
		}
		break;
	}
  baddec:
	synerr("invalid declarator");
	return -1;
}


/*
 * addtdtype - add typedef info into declarator
 *      here we very carefully add in the dimensions for an array typedef
 *      declaration.  Note that declarator has already allocated the
 *      declarator-specific dimensions, now we allocate the typedef
 *      dimensions and adjust the size of the declarator's dimensions.
 *      Note that this must be done before the dalloc for the structure,
 *      otherwise we would mix up array and structure sizes.
 * returns type
 */
short addtdtype(P(struct symbol *) tddp, P(int) type, P(int) dp, P(short *) ssp)
PP(struct symbol *tddp;)
PP(int type;)
PP(int dp;)
PP(short *ssp;)
{
	register short ntype, t, i, tdf;

	for (tdf = 0, t = tddp->s_type; SUPTYPE(t); t = delsp(t))
		if (ISARRAY(t))
		{
			tdf++;
			break;
		}
	ntype = 0;
	for (t = type, i = dp; SUPTYPE(t); t = delsp(t))
	{
		ntype = addsp(ntype, t);
		if (tdf && ISARRAY(t))
		{
			dtab[i] *= dtab[tddp->s_dp];
			i++;
		}
	}
	for (t = tddp->s_type, i = tddp->s_dp; SUPTYPE(t); t = delsp(t))
		if (ISARRAY(t))
			dalloc(dtab[i++]);
	for (t = tddp->s_type; SUPTYPE(ntype); ntype = delsp(ntype))
		t = addsp(t, ntype);
	if ((ntype = BTYPE(t)) == STRUCT)
		*ssp = tddp->s_ssp;
	else if (ntype == FRSTRUCT)
		*ssp = frstp;
	return t;
}
