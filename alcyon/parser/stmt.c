/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"

char exprarea[EXPSIZE];
char *exprp = &exprarea[0];
short nextlabel	= 1;

#define LABGEN(l,sl)    sl=l;l=nextlabel++
#define DOBODY(l)   stmt();OUTLAB((l));clno=lineno;lineno=rinit

static short swp = -1;				/* current entry in switch table */



/*
 * gotolabel - gets label id for goto
 * This is used for both: goto symbol and if(...)goto symbol
 returns 0 if not, else label id
 */
static int gotolabel(NOTHING)
{
	register struct symbol *sp;

	if (!next(SYMBOL))
		synerr(_("expected label"));
	else
	{
		sp = csp;
		if (!(sp->s_sc))
		{
			sp->s_type = LLABEL;
			sp->s_scope = FUNC_SCOPE;
			if (!sp->s_offset)
				sp->s_offset = nextlabel++;
			TO_DSK(sp, csp_addr);
		} else if (sp->s_scope != FUNC_SCOPE)
		{
			csp = lookup(sp->s_symbol, 1);	/* force individual entry */
			sp = csp;
			sp->s_type = LLABEL;
			sp->s_scope = FUNC_SCOPE;
			if (!sp->s_offset)
				sp->s_offset = nextlabel++;
			TO_DSK(sp, csp_addr);
		}
		if ((!sp->s_sc || sp->s_sc == STATIC) && sp->s_type == LLABEL)
			return sp->s_offset;
		synerr(_("invalid label"));
	}
	return 0;
}


/*
 * dolabel - do statement label
 *      Checks current symbol for already being defined, then sets
 *      symbol attributes for label.
 */
static VOID dolabel(NOTHING)
{
	register struct symbol *sp;

	sp = csp;
	if (sp->s_sc)
	{
		if (sp->s_scope == FUNC_SCOPE)
		{								/* truly redefined !!!! */
			error(_("label redeclaration: %.*s"), SSIZE, sp->s_symbol);
			return;
		}
		csp = lookup(sp->s_symbol, 1);	/* force individual entry */
		sp = csp;
	}
	sp->s_attrib |= SDEFINED;
	sp->s_sc = STATIC;
	sp->s_type = LLABEL;
	sp->s_scope = FUNC_SCOPE;
	if (!sp->s_offset)
		sp->s_offset = nextlabel++;
	TO_DSK(sp, csp_addr);
	OUTLAB(sp->s_offset);
}


/*
 * brklabel - generate break label
 * Checks if break label is undefined, and if so, generates message
 * returns label number
 */
static int brklabel(NOTHING)
{
	if (!blabel)
		error(_("invalid break statement"));
	return blabel;
}


/*
 * contlabel - generate continue label
 * Checks if continue label is undefined, and if so, generates message
 * returns label number
 */
static int contlabel(NOTHING)
{
	if (!clabel)
		error(_("invalid continue statement"));
	return clabel;
}


/*
 * addswitch - add an entry into current switch table, bubble sorting
 *      This makes it easier on the code generator and also checks for
 *      duplicate labels at the "right" time.
 */
static int addswitch(P(struct swtch *) sp, P(int) ncases, P(int) nval, P(int) nlab)
PP(struct swtch *sp;)						/* switch table pointer */
PP(int ncases;)								/* number of cases in switch */
PP(int nval;)								/* new value */
PP(int nlab;)								/* new label */
{
	register struct swtch *nswp, *s;
	register short temp;

	nswp = &sp[ncases];
	nswp->sw_value = nval;
	nswp->sw_label = nlab;
	s = nswp--;
	for (; --ncases >= 0; s--, nswp--)
	{
		if (s->sw_value == nswp->sw_value)
		{
			error(_("duplicate case value"));
			return 0;					/* don't add it in !!!! */
		}
		if (s->sw_value < nswp->sw_value)
		{
			temp = s->sw_value;
			s->sw_value = nswp->sw_value;
			nswp->sw_value = temp;
			temp = s->sw_label;
			s->sw_label = nswp->sw_label;
			nswp->sw_label = temp;
		}
	}
	return 1;
}


/*
 * docase - handles: case constant : statement
 * Checks for being in a switch statement, adds entry to switch table
 */
static VOID docase(NOTHING)
{
	register short lab;
	long value;

	colonstop++;
	value = cexpr();					/* get case value */
	colonstop--;
	if (!next(COLON))					/* check for colon */
		synerr(_("missing colon"));
	if (swp < 0)
	{
		error(_("case not inside a switch block"));
	} else if (swp >= (SWSIZE - 1))
	{
		error(_("too many cases in switch"));
	} else
	{
		if (addswitch(&swtab[cswp], swp - cswp, (int) value, lab = nextlabel))
		{
			nextlabel++;
			OUTLAB(lab);
			swp++;
		}
	}
}


/*
 * dodefault - handles: default : statement
 * Checks for colon and being in a switch statement
 */
static VOID dodefault(NOTHING)
{
	if (!next(COLON))
		error(_("missing colon"));
	if (swp < 0)
		error(_("default not inside a switch block"));
	else
	{
		dlabel = nextlabel++;			/* allocate default label */
		OUTLAB(dlabel);					/* output default label */
	}
}


/*
 * nextrw - is next token the specified reserved word?
 * returns 1 if match, 0 otherwise
 */
static int nextrw(P(int) rw)
PP(int rw;)									/* reserved word to match */
{
	register short token;

	if ((token = gettok(0)) != RESWORD || cvalue != rw)
	{
		pbtok(token);
		return 0;
	}
	return 1;
}


/*
 * balpar - handle expression within parenthesis for while and if
 * Merely checks for left and right parens and builds expression.
 * returns pointer to expression
 */
static struct tnode *balpar(NOTHING)
{
	register struct tnode *tp;

	if (next(LPAREN))
	{
		reducep = 1;
		tp = expr(0);
		reducep = 0;
		if (next(RPAREN))
			return tp;
	}
	synerr(_("parenthesized expression syntax"));
	return NULL;
}


/* dodo - handles: do statement while ( expression ) */
static VOID dodo(NOTHING)
{
	register short lab, saveblab, saveclab;

	LABGEN(blabel, saveblab);
	LABGEN(clabel, saveclab);
	lab = nextlabel++;
	outline();							/* output lineno for debugger */
	OUTNULL();							/* null tree for line number */
	OUTLAB(lab);						/* branch back to here */
	stmt();								/* do statement */
	OUTLAB(clabel);						/* continue label */
	if (!nextrw(R_WHILE))
	{
		warning(_("missing while"));	/* only advisory... */
		OUTGOTO(lab);
	} else
	{
		outifgoto(balpar(), TRUE, lab);	/* while expression */
	}
	OUTLAB(blabel);						/* break label */
	blabel = saveblab;					/* restore labels */
	clabel = saveclab;
}


/*
 * dofor - handle: for ( expression ; expression ; expression ) statement
 *      Hard part is handling re-initialization expression, which is
 *      parsed and saved, then the statement is parsed, then the reinit
 *      clause expression tree is output.
 */
static VOID dofor(NOTHING)
{
	register short testlab, stmtlab, saveblab, saveclab;
	register struct tnode *rip, *cp;
	register char *savep;
	short rinit, clno, iscond;

	testlab = 0; /* quiet compiler */
	cp = NULL;
	LABGEN(blabel, saveblab);
	LABGEN(clabel, saveclab);
	if (!next(LPAREN))
	{
	  forerr:
#ifdef DEBUG
		if (symdebug)
			fprintf(stderr, "invalid for... commastop is %d", commastop);
#endif
		synerr(_("invalid for statement"));
		return;
	}
	if (!next(SEMI))
	{									/* do init expression */
		outexpr(expr(0));
		if (!next(SEMI))
			goto forerr;
	}
	savep = exprp;						/* save ptr to exprarea */
	if (!next(SEMI))
	{									/* do for condition */
		testlab = nextlabel++;			/* if condition, get a label */
		OUTGOTO(testlab);				/* only goto cond expr if exists */
		iscond = 1;
		cp = expr(0);
		exprp = opap;
		if (!next(SEMI))
			goto forerr;
	} else
	{
		iscond = 0;
	}
	stmtlab = nextlabel++;
	OUTLAB(stmtlab);					/* branch back to here */
	rinit = lineno;
	if (next(RPAREN))
	{									/* no re-init - easy case */
		DOBODY(clabel);					/* output statement */
	} else
	{									/* there is a re-init clause */
		rip = expr(0);					/* save re-init tree until done */
		exprp = opap;
		if (!next(RPAREN))
			goto forerr;
		DOBODY(clabel);					/* do statment */
		outexpr(rip);					/* output re-init clause */
	}
	if (iscond)
	{
		OUTLAB(testlab);				/* branch for test */
		outifgoto(cp, TRUE, stmtlab);
	} else
	{
		OUTGOTO(stmtlab);				/* unconditional branch */
	}
	exprp = savep;
	lineno = clno;
	OUTLAB(blabel);						/* break label */
	blabel = saveblab;
	clabel = saveclab;					/* restore labels */
}


/*
 * doif - handles: if ( expression ) statement [ else statement ]
 * Handles special cases for goto, break, continue and return.
 */
static VOID doif(NOTHING)
{
	register struct tnode *tp;
	register short elselab, exitlab;

	outline();
	OUTNULL();
	tp = balpar();		/* if( expr )... */
	exitlab = 0;
	if (nextrw(R_GOTO))
		exitlab = gotolabel();
	else if (nextrw(R_BREAK))
		exitlab = brklabel();
	else if (nextrw(R_CONTINUE))
		exitlab = contlabel();
	else if (nextrw(R_RETURN))
	{
		if (peekc(';'))
		{
			exitlab = rlabel;
			putback(';');
		} else
			pbtok(RESWORD);
	}
	if (exitlab)
	{									/* easy goto, do branch if true */
		outifgoto(tp, TRUE, exitlab);
		if (!next(SEMI))
			synerr(_("missing semicolon"));
		if (nextrw(R_ELSE))				/* else clause, just output it */
			stmt();
	} else
	{									/* hard goto, branch over statement */
		elselab = nextlabel++;
		outifgoto(tp, FALSE, elselab);
		stmt();
		if (nextrw(R_ELSE))
		{
			exitlab = nextlabel++;		/* branches over else clause */
			OUTGOTO(exitlab);			/* branch out of then clause */
			OUTLAB(elselab);			/* label to start else clause */
			stmt();						/* else statement */
			OUTLAB(exitlab);
		} else
			OUTLAB(elselab);			/* no else, just branch out */
	}
}


/* outforreg - generate assignment for switch and return */
static VOID outforreg(P(int) op, P(struct tnode *) ltp, P(struct tnode *) rtp)
PP(int op;)									/* operator for build tree */
PP(struct tnode *ltp;)						/* left expression tree */
PP(struct tnode *rtp;)						/* right expression tree */
{
	register struct tnode *tp;

	opp = opstack;
	opdp = opdstack;
	pushopd(ltp);
	pushopd(rtp);
	maketree(op);
	if ((tp = (struct tnode *) popopd()) != NULL)
		outcforreg(tp->t_right);
	opp = NULL;
	opdp = NULL;
}


/*
 * doreturn - handles: return( expression ) ;
 *      Expression is the hard part, must create an assignment expression
 *      to assign expression to the type of the function, then get it
 *      loaded into a specific register.
 */
static VOID doreturn(NOTHING)
{
	register struct tnode *tp;

	if (!peekc(';'))
	{
		if ((tp = expr(0)) != NULL)
			outforreg(FRETURN, frp, tp);
		else
			putback(';');
	} else
		putback(';');
	OUTGOTO(rlabel);					/* branch to the return label */
}


/*
 * doasm - handles: asm( "string" ) ;
 *      Outputs the string as literal assembly language code
 */
static VOID doasm(NOTHING)
{
	outline();							/* output line number */
	OUTNULL();							/* null tree for line number */
	if (next(LPAREN))
	{
		if (next(STRING))
			if (next(RPAREN))
			{
				outasm();
				return;
			}
	}
	synerr(_("illegal asm syntax"));
}


/*
 * doswitch - handles: switch ( expression ) statement
 *      Evaluates the expression, forces the result into a known register
 *      collects the case statements in swtab, then outputs the switch
 *      operator and switch cases.
 */
static VOID doswitch(NOTHING)
{
	register short saveblab, swlab, savedlab, saveswp, i;
	register struct tnode *tp;

	LABGEN(blabel, saveblab);
	tp = balpar();
	integral(tp, -1);					/* must be integral type result */
	outforreg(ASSIGN, (struct tnode *)snalloc(INT, AUTO, 0, 0, 0), tp);
	saveswp = swp;						/* remember old switch pointer */
	if (saveswp < 0)
		swp++;
	i = cswp;
	cswp = swp;							/* remember real first entry */
	swlab = nextlabel++;
	OUTGOTO(swlab);						/* branch to switch code */
	savedlab = dlabel;
	dlabel = 0;
	stmt();								/* do switch statement */
	OUTGOTO(blabel);					/* output branch just in case */
	OUTLAB(swlab);						/* here we now do the switch code */
	if (!dlabel)
		dlabel = blabel;
	outswitch(swp - cswp, dlabel, &swtab[cswp]);
	OUTLAB(blabel);						/* break to here */
	cswp = i;
	swp = saveswp;
	blabel = saveblab;
	dlabel = savedlab;
}


/*
 * dowhile - handles: while ( expression ) statement
 * This is fairly straight-forward.
 */
static VOID dowhile(NOTHING)
{
	register short saveclab, saveblab, lab;
	register char *savep;
	register struct tnode *tp;

	LABGEN(blabel, saveblab);
	LABGEN(clabel, saveclab);
	LABGEN(clabel, lab);
	savep = exprp;
	outline();							/* output line number on cond */
	OUTNULL();							/* null tree for line number */
	if ((tp = balpar()) != 0)	/* get condition clause */
		OUTGOTO(clabel);				/* condition label */
	exprp = opap;
	OUTLAB(lab);
	stmt();								/* statement */
	OUTLAB(clabel);						/* condition test */
	outifgoto(tp, TRUE, lab);			/* branch back to top of loop */
	OUTLAB(blabel);						/* break to here */
	exprp = savep;
	blabel = saveblab;
	clabel = saveclab;					/* restore labels */
}


/* stmt - process a single statement */
VOID stmt(NOTHING)
{
	register short token, lab;

	while (1)
	{
		switch (token = gettok(0))
		{
		case LCURBR:					/* handle { ... } */
			scope_decls[scope_level] = 1;
			dlist(TYPELESS);
			while (!next(CEOF))
			{
				if (next(RCURBR))
					return;
				stmt();
			}
		case CEOF:
			error(_("{ not matched by }"));
		case SEMI:						/* null statement */
			return;

		case RCURBR:
			pbtok(token);
			return;

		case SYMBOL:					/* symbol: statement */
			if (peekc(':'))
			{
				dolabel();
				continue;
			}
		default:						/* anything else... */
			pbtok(token);
			outexpr(expr(0));
			break;

		case RESWORD:
			switch (cvalue)
			{
			case R_BREAK:
				lab = brklabel();
				OUTGOTO(lab);			/* branch to break label */
				break;

			case R_CASE:
				docase();
				continue;

			case R_CONTINUE:
				lab = contlabel();		/* branch to continue label */
				OUTGOTO(lab);
				break;

			case R_DEFAULT:
				dodefault();
				continue;

			case R_DO:
				dodo();
				break;

			case R_FOR:
				dofor();
				return;

			case R_GOTO:
				lab = gotolabel();
				OUTGOTO(lab);
				break;

			case R_IF:
				doif();
				return;

			case R_RETURN:
				doreturn();
				break;

			case R_SWITCH:
				doswitch();
				return;

			case R_WHILE:
				dowhile();
				return;

			case R_ASM:
				doasm();
				return;

			case R_CHAR:
			case R_INT:
			case R_FLOAT:
			case R_LONG:
			case R_DOUBLE:
			case R_STRUCT:
			case R_UNION:
			case R_REGISTER:
				synerr(_("invalid declaration"));
				return;

			default:
				synerr(_("invalid keyword"));
				return;
			}
		}
		if (!next(SEMI))
			synerr(_("missing semicolon"));
		return;
	}
}


/* outassign - generate assignment for function args */
VOID outassign(P(struct tnode *) ltp, P(struct tnode *) rtp)
PP(struct tnode *ltp;)						/* left expression tree */
PP(struct tnode *rtp;)						/* right expression tree */
{
	opp = opstack;
	opdp = opdstack;
	pushopd(ltp);
	pushopd(rtp);
	maketree(ASSIGN);
	outexpr(popopd());
	opp = NULL;
	opdp = NULL;
}
