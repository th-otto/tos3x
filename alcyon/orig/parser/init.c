/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121

	@(#)init.c	1.13	1/3/84
*/

#include "parser.h"

short inittype;

short bfield_ty;						/* type of bit field currently initializing */

short bits_init;						/* current bit field init value */

short in_bitinit;						/* are we currently initializing a bit field ?? */

short lst_boffset;						/* new bit field entity ?? */

#define BCHK(plus) {plus = out_bfield(CHAR); nbout += plus; nbleft += plus;}


long s_or_a PROTO((int original, long elsize, long *pnbout, long bleft, struct symbol *sp, struct symbol *child));
long str_init PROTO((long datasize, int type));
int oneinit PROTO((int type, int dp, int sc));
int one_binit PROTO((int type, int dp, long value));
int out_bfield PROTO((int type));




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
VOID doinit(P(struct symbol *) sp)
PP(struct symbol *sp;)						/* pointer to symbol to init */
{
	register short type;
	long isize, datasize;

	type = sp->s_type;
	datasize = dsize(type, sp->s_dp, sp->s_ssp);
	if (PEEK(COMMA) || PEEK(SEMI))
	{									/* no initialization */
		isize = WALIGN(datasize);
#ifdef DEBUG
		if (initdebug)
			printf("%s sc %x attrib %x\n", sp->s_symbol, sp->s_sc, sp->s_attrib);
#endif
		if (sp->s_sc == EXTERNAL)
			OUTCOMMON(sp->s_symbol, isize);
		else if (sp->s_sc == DEXTERN)	/* explicitly EXTERN */
			sp->s_sc = EXTERNAL;
		else
		{
			OUTBSS();					/* bss segment */
			OUTLAB(sp->s_offset);
			OUTRESMEM(isize);
			OUTTEXT();					/* resume text segment */
		}
	} else
	{
		if (!(next(ASSIGN)))			/* ignore '=' if there */
			warning(_("old fashion initialization"));
		if (type == LONG || ISPOINTER(type))
			outldata();
		else if (type == DOUBLE || type == FLOAT)
			outfpdata();
		else
			outdata();
		if (sp->s_sc == EXTERNAL)
			OUTDLAB(sp->s_symbol);
		else
			OUTLAB(sp->s_offset);
		isize = initlist(sp, sp->s_type, sp->s_dp, sp->s_ssp);
		if (isize < datasize)
		{
			OUTRESMEM(datasize - isize);
			isize = datasize;
		}
		if (WALIGN(isize) != isize)
			OUTPAD();
		if (isize > datasize)
		{
			if (ISARRAY(sp->s_type))
			{
				for (type = sp->s_type; ISARRAY(type); type = delsp(type))
					;
				dtab[sp->s_dp] = isize / dsize(type, sp->s_dp, sp->s_ssp);
			} else
			{
				error(_("too many initializers"));
			}
		}
		if (sp->s_sc == STATIC)
			OUTTEXT();
	}
}


/*
 * initlist - handles initializer lists
 *      This handles multi-level braces, and a character pointer pointing
 *      to a string.  Most of the work is in keeping track of how many
 *      bytes are left on the current "row", and outputting padding if
 *      necessary.
 * returns size of initializers in bytes
 */
long initlist(P(struct symbol *) sp, P(int) type, P(int) dp, P(int) ssp)
PP(struct symbol *sp;)
PP(int type;)
PP(int dp;)
PP(int ssp;)
{
	register short onetype, plus, atype;
	long datasize, i, j, elsize, nbleft, nbout;

	i = next(LCURBR);
	for (onetype = type; ISARRAY(onetype); onetype = delsp(onetype))
		;
#ifdef DEBUG
	if (initdebug)
	{
		printf("type: %d BTYPE %d ISARRAY %d ", type, BTYPE(type), ISARRAY(type));
		printf("POINTER %d\n", ISPOINTER(type));
		printf("onetype: %d BTYPE %d ", onetype, BTYPE(onetype));
		printf("ISARRAY %d POINTER %d\n", ISARRAY(onetype), ISPOINTER(onetype));
	}
#endif
	nbout = 0;
	datasize = dsize(type, dp, ssp);
	nbleft = 0;							/* keep track of no. of bytes left */
#if !BINEXACT
	elsize = 0; /* quiet compiler */
#endif
	atype = (ISARRAY(type)) ? delsp(type) : type;
	if (type == (ARRAY | CHAR) || type == (ARRAY | INT) || type == (ARRAY | LONG))
	{
		nbout = str_init(datasize, type);
	} else if (atype == (ARRAY | CHAR) || atype == (ARRAY | INT) || atype == (ARRAY | LONG))
	{
		datasize = dsize(atype, dp, ssp);
		elsize = dsize(atype, dp + 1, ssp);
		if (!i)
			error(_("missing { in initialization"));
		do
		{
			j = (next(LCURBR));
			nbout += str_init(elsize, atype);
			if (j && !(next(RCURBR)))
				error(_("missing } in initialization"));
			next(COMMA);				/* skip over comma... */
		} while (!PEEK(RCURBR) && !PEEK(SEMI));
		nbleft = datasize - nbout;
		if (((elsize == INTSIZE && onetype == INT) || onetype == SHORT) || /* BUG: missing parenthesis; && has higher precedence */
		    (elsize == CHARSIZE && onetype == CHAR) ||
		    (elsize == LONGSIZE && onetype == LONG) ||
		    datasize == elsize)	/* undimensioned array */
			nbleft = 0;
		if (nbleft < 0)
		{
			error(_("initializer list too long"));
			nbleft = 0;
		}
	} else if (BTYPE(type) == STRUCT && NOTPOINTER(type) && ISPOINTER(onetype))
	{
		/* multi-dimensional array */
		nbleft = elsize = PTRSIZE;
		if (!i)
			error(_("missing { in initialization"));
		do
		{
			nbleft = s_or_a(POINTER | CHAR, elsize, &nbout, nbleft, NULL, NULL);
			next(COMMA);				/* skip over comma... */
		} while (!PEEK(RCURBR) && !PEEK(SEMI));
	} else if (ISARRAY(type) || (BTYPE(type) == STRUCT && NOTPOINTER(type)))
	{
		nbleft = elsize = dsize(delsp(type), dp, ssp);	/* was dp+1 */
		if (!i && BTYPE(type) == STRUCT)
			error(_("missing { in initialization"));
		do
		{
			nbleft = s_or_a(onetype, elsize, &nbout, nbleft, sp->s_par, NULL);
			next(COMMA);				/* skip over comma... */
		} while (!PEEK(RCURBR) && !PEEK(SEMI));
	} else
	{
		indecl = 0;
		nbout = oneinit(onetype, dp, (int) sp->s_sc);
	}
	if (in_bitinit)
		BCHK(plus)
	if (nbleft && nbleft != elsize)
	{								/* pad current row */
		OUTRESMEM(nbleft);
		nbout += nbleft;
	}
	if (i)
	{
		next(COMMA);					/* skip over extra comma */
		if (!(next(RCURBR)))
			error(_("missing } in initialization"));
	}
	return nbout;
}

#define COPY(c,s) c = s
#define HCOPY(c,s) c = s
#define BASIC_AR(x) (x==(ARRAY|CHAR)||x==(ARRAY|INT)||x==(ARRAY|LONG))

/*
 * s_or_a - initialize structure or array
 *		routine to deal with initializing structures and arrays and
 *		assure that structures are aligned properly.  Checks initializers
 *		with the expected type.
 *		Attempts to match structure initializations against the 
 *		elements of the structure.
 *		sp -- pointer to structure symbol table entry
 * returns nbleft
 */
long s_or_a(P(int) original, P(long) elsize, P(long *) pnbout, P(long) bleft, P(struct symbol *) sp, P(struct symbol *) child)
PP(int original;)							/* original type... */
PP(long elsize;)
PP(long *pnbout;)
PP(long bleft;)
PP(struct symbol *sp;)
PP(struct symbol *child;)
{
	register long nbleft, nbout;
	register short onetype, plus;
	long i, temp, datasize, addup, x;
	short nest, snest;					/* nest <= number of LCURBR */
	struct symbol *hold;

	nbleft = bleft;
	nbout = *pnbout;
	onetype = original;
	nest = i = 0;
#ifdef DEBUG
	if (initdebug)
		printf("s_or_a: [%s] elsize %ld\n", (sp) ? sp->s_symbol : "", elsize);
#endif
	do
	{									/* in current row. */
	  restart:
		if (BTYPE(original) == STRUCT)
		{
			if (!child)
			{
				if (!sp->s_child)
					break;
				else
				{
					COPY(child, sp->s_child);
				}
			}
			onetype = child->s_type;
#ifdef DEBUG
			if (initdebug)
				printf("s_or_a: child %s type %d\n", child->s_symbol, onetype);
#endif
		}
		if (PEEK(SEMI) || PEEK(CEOF) || PEEK(RCURBR))
			break;
		if (child && onetype == STRUCT)
		{
#ifdef DEBUG
			if (initdebug)
				printf("s_or_a recurse onetype STRUCT\n");
#endif
			snest = (next(LCURBR));
			temp = nbout;
			HCOPY(hold, child->s_par);	/* was s_child */
			i = dsize(onetype, (hold->s_dp) + 1, hold->s_ssp);
			s_or_a(onetype, i, &temp, i, hold, NULL);
			if (snest)
			{
				if (!next(RCURBR))		/* force release of matched curbr */
					error(_("mismatched curly braces"));
			} else if (!peektok)
			{
				peektok = COMMA;
			}
			goto past;
		} else if (next(LCURBR))
		{								/* recurse down one level? */
			nest++;
			if (in_bitinit)
				BCHK(plus)
			if (nbout && nbleft && (nbleft < elsize))
			{
				OUTRESMEM(nbleft);	/* pad rest of current row */
				nbout += nbleft;
				nbleft = 0;
			}
			if (!nbout && !nbleft)
				goto restart;
			if (i > elsize)
				error(_("initializer list too long"));
			child = 0;					/* force restart of structure match up */
			goto restart;
		} else if (PEEK(RCURBR))
		{
			if (nest)
			{
				next(RCURBR);
				nest--;
				continue;				/* goes to next comma... */
			} else
				break;
		} else
		{
			if (BASIC_AR(onetype))
			{
#ifdef DEBUG
				if (initdebug)
					printf("BASIC_AR onetype 0%lo\n", onetype);
#endif
				datasize = dsize(onetype, child->s_dp, child->s_ssp);
				i = str_init(datasize, onetype);
			} else if (BASIC_AR(original))
			{
#ifdef DEBUG
				if (initdebug)
					printf("BASIC_AR original 0%o\n", original);
#endif
				datasize = dsize(original, child->s_dp, child->s_ssp);
				i = str_init(datasize, original);
			} else if (ISARRAY(onetype))
			{
#ifdef DEBUG
				if (initdebug)
					printf("ISARRAY onetype 0%o\n", onetype);
#endif
				addup = 0;				/* array of structures or arrays */
				temp = nbout;
				i = dsize(onetype, child->s_dp, child->s_ssp);
				onetype = delsp(onetype);
				datasize = dsize(onetype, child->s_dp + 1, child->s_ssp);
				HCOPY(hold, child->s_par);	/* was s_child */
				x = onetype & ~POINTER;
				if (SIMPLE_TYP(x))
				{
#ifdef DEBUG
					if (initdebug)
						printf("simple type 0%o 0%o\n", onetype, x);
#endif
					for (addup = 0; addup != i; addup += datasize)
					{
						if (PEEK(RCURBR) || PEEK(SEMI))
						{
							OUTRESMEM(i);
							break;
						}
						oneinit(onetype, hold->s_dp, (int) hold->s_sc);
						next(COMMA);
					}
					if (!peektok)
						peektok = COMMA;
#ifdef DEBUG
					if (initdebug)
						printf("peektok %d addup %ld\n", peektok, addup);
#endif
				} else
					for (addup = 0; addup != i; addup += datasize)
					{
						x = s_or_a(onetype, datasize, &temp, datasize, hold, NULL);
						if (x)
						{
							/* no initializer for current item */
							OUTRESMEM(i - (addup + datasize - x));
							break;
						}
					}
			} else
			{							/* simple type....... */
				if ((nbleft & 1) && onetype != CHAR)
				{
					nbleft--;
					nbout++;
					OUTEVEN();
				}
				if (!child)				/* don't access off of zero !!! */
					i = oneinit(onetype, 0, 0);
				else
					i = oneinit(onetype, child->s_dp, (int) child->s_sc);
			}
		  past:
			nbout += i;
			if (!nbleft && !SIMPLE_TYP(original))
				nbleft = elsize;
			if (i > nbleft)
				error(_("initializer alignment"));
			nbleft = (i >= nbleft) ? 0 : nbleft - i;
			if (BTYPE(original) == STRUCT)
			{
				if (!child->s_sib)
				{
					plus = (in_bitinit) ? out_bfield(CHAR) : 0;
					nbleft -= plus;
					nbout += nbleft;
					if (nbleft)
					{					/* was checking against elsize */
						OUTRESMEM(nbleft);
						nbleft = 0;
					}
					if (SUPTYPE(original) != ARRAY)
						break;
#ifdef DEBUG
					if (initdebug)
						printf("last child out...\n");
#endif
				} else
				{
					COPY(child, child->s_sib);
				}
			}
		}
#ifdef DEBUG
		if (initdebug && child)
			printf("child [%s] PEEKTOK comma %d\n", child->s_symbol, PEEK(COMMA));
#endif
	} while (next(COMMA));
#ifdef DEBUG
	if (initdebug)
		printf("exiting s_or_a: nbleft %ld\n", nbleft);
#endif
	if (BTYPE(original) == STRUCT)
	{
		/* ensure padding... */
		plus = (in_bitinit) ? out_bfield(CHAR) : 0;
		nbout += nbleft;
		if (nbleft)
		{								/* was checking against elsize */
			OUTRESMEM(nbleft);
			nbleft = 0;
		}
	}
	*pnbout = nbout;
	while (nest--)
	{
		next(COMMA);					/* comma's may separate.... */
		if (!next(RCURBR))				/* force release of matched curbr */
			error(_("mismatched curly braces"));
	}
	next(COMMA);						/* comma's may separate.... */
	return nbleft;
}


/*
 * str_init - string (array) initialization
 * 		initialize a character array, truncating or padding as required
 */
long str_init(P(long) datasize, P(int) type)
PP(long datasize;)
PP(int type;)								/* should not get a bit field here..... */
{
	register long maxsize, output;

#ifdef DEBUG
	if (initdebug)
		printf("str_init: type %d, datasize %ld\n", type, datasize);
	if (initdebug)
		printf("str_init: SUPTYPE %d, ", SUPTYPE(type));
	if (initdebug)
		printf("BTYPE %d\n", BTYPE(type));
#endif
	output = 0L;
	if ((datasize == CHARSIZE && BTYPE(type) == CHAR) ||	/* undimensioned array */
		((datasize == INTSIZE && BTYPE(type) == INT) || BTYPE(type) == SHORT) || /* BUG: && has higher predence */
		(datasize == LONGSIZE && BTYPE(type) == LONG))
	{
		datasize = 0;
		maxsize = 0x7fff;				/* max 32 bit positive value */
#ifdef DEBUG
		if (initdebug)
			printf("undimensioned type %d datasize %ld\n", type, datasize);
#endif
	} else
		maxsize = datasize;
	do
	{
		if (next(STRING))
		{
			output += (!datasize) ? outstr((long) cstrsize, (long) cstrsize) :
				outstr(maxsize - output, (long) cstrsize);
			return output;
		}
		output += oneinit(BTYPE(type) | ARRAY, 0, 0);
#ifdef DEBUG
		if (initdebug)
			printf("output %ld, ", output);
		if (initdebug)
			printf("maxsize %ld, ", maxsize);
#endif
		if (maxsize <= output)
		{
			if (maxsize < output)
				error(_("character array initializer alignment"));
			return output;
		}
	} while (next(COMMA) && !PEEK(SEMI) && !PEEK(RCURBR));
	if (datasize)
	{
		OUTRESMEM(maxsize - output);
		return maxsize;
	}
	return output;
}


/* 
 * oneinit - get one initializer expression or constant
 *      This checks the type of the data item expected against the
 *      type obtained from expr.  
 * returns size of initializer
 */
int oneinit(P(int) type, P(int) dp, P(int) sc)
PP(int type;)								/* type of initializer */
PP(int dp;)
PP(int sc;)									/* for bit field init */
{
	register short op, plus, ivalue;
	register struct tnode *tp;
	register long value;

	commastop++;
#ifdef DEBUG
	if (initdebug)
		printf("oneinit: type = %d\n", type);
#endif
	if ((tp = (struct tnode *) expr(0)) == 0)
	{
		error(_("invalid initializer"));
		commastop--;
		return 0;
	}
	commastop--;

	if ((op = tp->t_op) == CINT)
		clvalue = ((struct conode *) tp)->t_value;
	else if (op == CLONG)
		clvalue = ((struct lconode *) tp)->t_lvalue;
	ivalue = value = clvalue;

	if (sc == BFIELDCL)
		return one_binit(type, dp, value);

	plus = (in_bitinit) ? out_bfield(type) : 0;

	switch (ISALLTYPE(type))
	{
	case UCHAR:
		if (op == CINT || op == CLONG)
		{
			outc(CHAR, ((int) (ivalue & 0xff)));
			if ((ivalue & 0xff00) && ((ivalue & 0xff00) != 0xff00))
				warning(_("initializer truncated"));
			return 1 + plus;
		}
		if (op == ADDR)
		{
			outc(CHAR, ((int) (*cstr & 0xff)));
			warning(_("string used to initialize character value"));
			return 1 + plus;
		}
		break;

	case CHAR:
		if (op == CINT || op == CLONG)
		{
			outc(CHAR, ((int) ivalue) & 0xff);
#ifdef DEBUG
			if (initdebug)
				printf("ivalue %d\n", ivalue);
#endif
			if ((ivalue > 255) || (ivalue < -128))
				warning(_("initializer truncated"));
			return 1 + plus;
		}
		if (op == ADDR)
		{
			outc(CHAR, ((int) *cstr) & 0xff);
			warning(_("string used to initialize character value"));
			return 1 + plus;
		}
		break;

	case ARRAY | CHAR:
		if (op == CINT || op == CLONG)
		{
			if (ccbytes < 2)
			{
				outc(CHAR, ((int) ivalue) & 0xff);
				if ((ivalue > 255) || (ivalue < -128))
					warning(_("initializer truncated"));
				return 1 + plus;
			} else
			{							/* 2 character  charconst eg. 'ab' */
				outc(INT, ivalue);
				return 2 + plus;
			}
		}
		break;

	case INT:
	case ARRAY | INT:
		if (op == ADDR)
			break;
		if (op == CINT || op == CLONG)
		{
			outc(INT, (int) value);
			if ((value & 0xffff0000) && ((value & 0xffff0000) != 0xffff0000))
				warning(_("initializer truncated"));
		} else
		{
			outinit(tp, inittype);
		}
		return 2 + plus;

	case UNSIGNED:
	case USHORT:
	case ARRAY | UNSIGNED:
		if (op == ADDR)
			break;
		if (op == CLONG || op == CINT)
		{
			if ((value & 0xffff0000) && ((value & 0xffff0000) != 0xffff0000))
				warning(_("initializer truncated"));
			ivalue = value & 0xffff;
			outc(INT, ivalue);
		} else
		{
			outinit(tp, inittype);
		}
		return 2 + plus;

	case LONG:
	case ULONG:
	case ARRAY | LONG:
	case POINTER | CHAR:
	case POINTER | UCHAR:
	case POINTER | INT:
	case POINTER | LONG:
	case POINTER | ULONG:
	case POINTER | STRUCT:
	case POINTER | UNSIGNED:
	case POINTER | DOUBLE:
	case POINTER | FLOAT:
		if (op != CINT && op != CLONG)
		{
			outinit(tp, inittype);
			return 4 + plus;
		}
	case DOUBLE:
	case FLOAT:
		outfp_or_l(value);
		return 4 + plus;

	}
	error(_("invalid initializer type=%d"), ISALLTYPE(type));
	return plus;
}


/*
 * one_binit - individula element bit field initialization.
 * returns the number of bytes actually initialize 1, 2 or 0
 */
int one_binit(P(int) type, P(int) dp, P(long) value)
PP(int type;)
PP(int dp;)
PP(long value;)
{
	register short plus, mask, op;

	op = (BTYPE(type) == CHAR) ? BITSPCHAR : BITSPWORD;
	op = (dp >> 8) % op;
	if (in_bitinit && ((bfield_ty != BTYPE(type)) || (op < lst_boffset)))
		plus = out_bfield(type);
	else
		plus = 0;
	lst_boffset = op;
	bfield_ty = BTYPE(type);
	mask = (1 << (dp & 0xff)) - 1;
	bits_init |= ((value & mask) << op);
	in_bitinit++;
	return plus;
}


/*
 * out_bfield - last item was a bit field output it
 * return number of bytes output
 */
int out_bfield(P(int) type)
PP(int type;)
{
	in_bitinit = 0;
	if (bfield_ty == CHAR)
	{									/* char bit field type */
		outc(CHAR, (int) bits_init);
		bits_init = 0;
		if (ISALLTYPE(type) != CHAR)	/* returns 2 */
			OUTEVEN();
		else
			return 1;
	} else
	{									/* int/unsigned bit field type */
		outc(INT, (int) bits_init);
		bits_init = 0;
	}
	return 2;
}
