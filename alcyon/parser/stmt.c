/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"

#define LABGEN(l,sl)    sl=l;l=nextlabel++
#define DOBODY(l)   stmt();OUTLAB((l));clno=lineno;lineno=rinit

short bol;

/* stmt - process a single statement*/
stmt()                                  /* returns - none*/
{
    register short token, lab;

    while( 1 ) {
        switch(token=gettok(0)) {

        case LCURBR:                    /*handle { ... }*/
            scope_decls[scope_level] = 1;   /* [vlh] 4.2, block decls */
            dlist(TYPELESS);
            while( !next(EOF) ) {
                if( next(RCURBR) )
                    return;
                stmt();
            }
        case EOF:
            error("{ not matched by }");
        case SEMI:                      /*null statement*/
            return;

        case RCURBR:
            pbtok(token);
            return;

        case SYMBOL:                    /*symbol: statement*/
            if( peekc(':') ) {
                dolabel();
                continue;
            }
        default:                        /*anything else...*/
            pbtok(token);
            outexpr(expr(0));
            break;
    
        case RESWORD:
            switch(cvalue) {
    
            case R_BREAK:
                lab = brklabel();
                OUTGOTO(lab);       /*branch to break label*/
                break;
    
            case R_CASE:
                docase();
                continue;
    
            case R_CONTINUE:
                lab = contlabel();  /*branch to continue label*/
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
                synerr("invalid declaration");
                return;

            default:
                synerr("invalid keyword");
                return;
            }
        }
        if( !next(SEMI) )
            synerr("missing semicolon");
        return;
    }
}

/* balpar - handle expression within parenthesis for while and if*/
/*      Merely checks for left and right parens and builds expression.*/
char *
balpar()                            /* returns pointer to expression*/
{
    register struct tnode *tp;

    if( next(LPAREN) ) {
        reducep = 1;
        tp = (struct tnode *)expr(0);
        reducep = 0;
        if( next(RPAREN) )
            return((char *)tp);
    }
    synerr("parenthesized expression syntax");
    return(0);
}

/* gotolabel - gets label id for goto*/
/*      This is used for both: goto symbol and if(...)goto symbol*/
gotolabel()                         /* returns 0 if not, else label id*/
{
    register struct symbol *sp;

    if( !next(SYMBOL) )
        synerr("expected label");
    else {
        sp = csp;
        if( !(sp->s_sc) ) {
            sp->s_type = LLABEL;
            sp->s_scope = FUNC_SCOPE;
            if( !sp->s_offset )
                sp->s_offset = nextlabel++;
        }
		else if (sp->s_scope != FUNC_SCOPE) { /* [vlh] 4.2 */
			csp = lookup(sp->s_symbol,1);	/* force individual entry */
			sp = csp;
            sp->s_type = LLABEL;
            sp->s_scope = FUNC_SCOPE;
            if( !sp->s_offset )
                sp->s_offset = nextlabel++;
		}
        if( (!sp->s_sc || sp->s_sc == STATIC ) && sp->s_type == LLABEL )
            return(sp->s_offset);
        synerr("invalid label");
    }
    return(0);
}

/**
 * dolabel - do statement label
 *      Checks current symbol for already being defined, then sets
 *      symbol attributes for label.
**/
dolabel()                               /* returns - none*/
{
    register struct symbol *sp;

    sp = csp;
    if( sp->s_sc ) {	/* [vlh] 4.2 */
		if( sp->s_scope == FUNC_SCOPE ) { /* truly redefined !!!! */ 
			error("label redeclaration: %.8s",sp->s_symbol);
			return;
		}
		csp = lookup(sp->s_symbol,1);	/* force individual entry */
		sp = csp;
    }
	sp->s_attrib |= SDEFINED;
	sp->s_sc = STATIC;
	sp->s_type = LLABEL;
	sp->s_scope = FUNC_SCOPE;
	if( !sp->s_offset )
		sp->s_offset = nextlabel++;
	OUTLAB(sp->s_offset);
}

/* brklabel - generate break label*/
/*      Checks if break label is undefined, and if so, generates message*/
brklabel()                              /* returns label number*/
{
    if( !blabel )
        error("invalid break statement");
    return(blabel);
}

/* contlabel - generate continue label*/
/*  Checks if continue label is undefined, and if so, generates message*/
contlabel()                             /* returns label number*/
{
    if( !clabel )
        error("invalid continue statement");
    return(clabel);
}

/* docase - handles: case constant : statement*/
/*      Checks for being in a switch statement, adds entry to switch table*/
docase()                                /* returns - none*/
{
    register short lab;
    long value;

    colonstop++;
    value = cexpr();                    /*get case value*/
    colonstop--;
    if( !next(COLON) )              /*check for colon*/
        synerr("missing colon");
    if( swp < 0 )
        error("case not inside a switch block");
    else if( swp >= (SWSIZE-1) )
        error("too many cases in switch");
    else {
        addswitch(&swtab[cswp],swp-cswp,(int)value,lab=nextlabel++);
        OUTLAB(lab);
        swp++;
    }
}

/* dodefault - handles: default : statement*/
/*      Checks for colon and being in a switch statement*/
dodefault()                             /* returns - none*/
{
    if( !next(COLON) )
        error("missing colon");
    if( swp < 0 )
        error("default not inside a switch block");
    else {
        dlabel = nextlabel++;           /*allocate default label*/
        OUTLAB(dlabel);                 /*output default label*/
    }
}

/* dodo - handles: do statement while ( expression )*/
dodo()                                  /* returns - none*/
{
    register short lab, saveblab, saveclab;

    LABGEN(blabel,saveblab);
    LABGEN(clabel,saveclab);
    lab = nextlabel++;
    OUTLINE();                          /*[vlh]4.2 output lineno for debugger*/
	OUTNULL();							/*[vlh]4.2 null tree for line number */
    OUTLAB(lab);                        /*branch back to here*/
    stmt();                             /*do statement*/
    OUTLAB(clabel);                     /*continue label*/
    if( !nextrw(R_WHILE)  ) {
        warning("missing while");       /*only advisory...*/
        OUTGOTO(lab);
    }
    else
        outifgoto(balpar(),TRUE,lab);   /*while expression*/
    OUTLAB(blabel);                     /*break label*/
    blabel = saveblab;                  /*restore labels*/
    clabel = saveclab;
}

/*
 * dofor - handle: for ( expression ; expression ; expression ) statement
 *      Hard part is handling re-initialization expression, which is
 *      parsed and saved, then the statement is parsed, then the reinit
 *      clause expression tree is output.
 */
dofor()                                 /* returns - none*/
{                                       /* [vlh] 4.0 reordered */
    register short testlab, stmtlab, saveblab, saveclab;
    register struct tnode *rip, *cp;
    register char *savep;
    short rinit, clno, iscond;

    LABGEN(blabel,saveblab);
    LABGEN(clabel,saveclab);
    if( !next(LPAREN) ) {
forerr:
#ifdef DEBUG
	if (symdebug) printf("invalid for... commastop is %d",commastop);
#endif
        synerr("invalid for statement");
        return;
    }
    if( !next(SEMI) ) {                 /*do init expression*/
        outexpr(expr(0));
        if( !next(SEMI) )
            goto forerr;
    }
    savep = exprp;                      /* save ptr to exprarea */
    if( !next(SEMI) ) {                 /* do for condition */
        testlab = nextlabel++;          /* if condition, get a label */
        OUTGOTO(testlab);               /* only goto cond expr if exists*/
        iscond = 1;
        cp = (struct tnode *)expr(0);
        exprp = opap;
        if( !next(SEMI) )
            goto forerr;
    }
    else
        iscond = 0;
    stmtlab = nextlabel++;
    OUTLAB(stmtlab);                    /* branch back to here */
    rinit = lineno;
    if( next(RPAREN) ) {                /*no re-init - easy case*/
        DOBODY(clabel);                 /*output statement*/
    }
	else {                              /*there is a re-init clause*/
        rip = (struct tnode *)expr(0);  /*save re-init tree until done*/
        exprp = opap;
        if( !next(RPAREN) )
            goto forerr;
        DOBODY(clabel);                 /*do statment*/
        outexpr(rip);                   /*output re-init clause*/
    }
    if (iscond) {
        OUTLAB(testlab);                /* branch for test */
        outifgoto(cp,TRUE,stmtlab);
    }
    else
        OUTGOTO(stmtlab);               /* unconditional branch */
    exprp = savep;
    lineno = clno;
    OUTLAB(blabel);                     /* break label */
    blabel = saveblab;
    clabel = saveclab;                  /*restore labels*/
}

/* doif - handles: if ( expression ) statement [ else statement ]*/
/*      Handles special cases for goto, break, continue and return.*/
doif()                                  /* returns - none*/
{
    register struct tnode *tp;
    register short elselab, exitlab;

    tp = (struct tnode *)balpar();                      /*if( expr )...*/
    exitlab = 0;
    if( nextrw(R_GOTO) )
        exitlab = gotolabel();
    else if( nextrw(R_BREAK) )
        exitlab = brklabel();
    else if( nextrw(R_CONTINUE) )
        exitlab = contlabel();
    else if( nextrw(R_RETURN) ) {
        if( peekc(';') ) {
            exitlab = rlabel;
            putback(';');
        }
        else
            pbtok(RESWORD);
    }
    if( exitlab ) {                     /*easy goto, do branch if true*/
        outifgoto(tp,TRUE,exitlab);
        if( !next(SEMI) )
            synerr("missing semicolon");
        if( nextrw(R_ELSE) )            /*else clause, just output it*/
            stmt();
    }
    else {                          /*hard goto, branch over statement*/
        elselab = nextlabel++;
        outifgoto(tp,FALSE,elselab);
        stmt();
        if( nextrw(R_ELSE) ) {
            exitlab = nextlabel++;      /*branches over else clause*/
            OUTGOTO(exitlab);           /*branch out of then clause*/
            OUTLAB(elselab);            /*label to start else clause*/
            stmt();                     /*else statement*/
            OUTLAB(exitlab);
        }
        else
            OUTLAB(elselab);            /*no else, just branch out*/
    }
}

/*
 * doreturn - handles: return( expression ) ;
 *      Expression is the hard part, must create an assignment expression
 *      to assign expression to the type of the function, then get it
 *      loaded into a specific register.
 */
doreturn()                          /* returns - none*/
{
    register struct tnode *tp;
    short parens;

    /*if( !peekc(';') ) {             /*need to compute return?*/
        /*parens = next(LPAREN);      /*[vlh] 4.2 check for lparen*/
        /*if (!next(RPAREN)) {        /*a return expression exists*/
            /*if ((tp = (struct tnode *)expr(0)) != 0)    { 
                /*outforreg(FRETURN,frp,tp);
                /*if (parens && !next(RPAREN)) /*[vlh]4.2 match paren*/
                    /*synerr("parenthesized expression syntax");
            /*}
        /*}
        /*else 
			/*warning("null expression in return statement");
    /*}*/
	if (!peekc(';')) {
		if ((tp=(struct tnode *)expr(0)) != 0)
			outforreg(FRETURN,frp,tp);
		else
			putback(';');
	}
    else
        putback(';');
    OUTGOTO(rlabel);                /*branch to the return label*/
}

/*
 * doasm - handles: asm( "string" ) ;       [vlh] 4.2
 *      Outputs the string as literal assembly language code
 */
doasm()                         /* returns - none*/
{
    OUTLINE();                          /* [vlh] 4.2 output line number */
	OUTNULL();							/* [vlh]4.2 null tree for line number */
    if( next(LPAREN) ) {
        if (next(STRING))
            if (next(RPAREN)) {
                outasm();
                return;
            }
    }
    synerr("illegal asm syntax");
}

/*
 * doswitch - handles: switch ( expression ) statement
 *      Evaluates the expression, forces the result into a known register
 *      collects the case statements in swtab, then outputs the switch
 *      operator and switch cases.
 */
doswitch()                              /* returns - none*/
{
    register short saveblab, swlab, savedlab, saveswp, i;
    register struct tnode *tp;

    LABGEN(blabel,saveblab);
    tp = (struct tnode *)balpar();
    integral(tp,-1);                    /*must be integral type result*/
    outforreg(ASSIGN,snalloc(INT,AUTO,0,0,0),tp);
    saveswp = swp;                      /*remember old switch pointer*/
    if( saveswp < 0 )
        swp++;
    i = cswp;
    cswp = swp;                         /*remember real first entry*/
    swlab = nextlabel++;
    OUTGOTO(swlab);                     /*branch to switch code*/
    savedlab = dlabel;
    dlabel = 0;
    stmt();                             /*do switch statement*/
    OUTGOTO(blabel);                    /*output branch just in case*/
    OUTLAB(swlab);                      /*here we now do the switch code*/
    if( !dlabel )
        dlabel = blabel;
    outswitch(swp-cswp,dlabel,&swtab[cswp]);
    OUTLAB(blabel);                     /*break to here*/
    cswp = i;
    swp = saveswp;
    blabel = saveblab;
    dlabel = savedlab;
}

/* dowhile - handles: while ( expression ) statement*/
/*      This is fairly straight-forward.*/
dowhile()                               /* returns - none*/
{                                       /* [vlh] 4.0 reworked */
    register short saveclab, saveblab, lab;
    register char *savep;
    register struct tnode *tp;

    LABGEN(blabel,saveblab);
    LABGEN(clabel,saveclab);
    LABGEN(clabel,lab);
    savep = exprp;
    if((tp = (struct tnode *)balpar()) != 0)    /*get condition clause*/
        OUTGOTO(clabel);                /*condition label*/
    exprp = opap;
    OUTLINE();                          /* [vlh] 4.2 output start label.... */
	OUTNULL();							/*[vlh]4.2 null tree for line number */
    OUTLAB(lab);
    stmt();                             /*statement*/
    OUTLAB(clabel);                     /*condition test*/
    outifgoto(tp,TRUE,lab);             /* branch back to top of loop */
    OUTLAB(blabel);                     /*break to here*/
    exprp = savep;
    blabel = saveblab;
    clabel = saveclab;                  /*restore labels*/
}

/* nextrw - is next token the specified reserved word?*/
nextrw(rw)                          /* returns 1 if match, 0 otherwise*/
int rw;                             /* reserved word to match*/
{
    register short token;

    if( (token=gettok(0)) != RESWORD || cvalue != rw ) {
        pbtok(token);
        return(0);
    }
    return(1);
}

/*
 * addswitch - add an entry into current switch table, bubble sorting
 *      This makes it easier on the code generator and also checks for
 *      duplicate labels at the "right" time.
 */
addswitch(sp,ncases,nval,nlab)      /* returns - none*/
struct swtch *sp;                   /* switch table pointer*/
int ncases;                         /* number of cases in switch*/
int nval;                           /* new value */
int nlab;                           /* new label*/
{
    register struct swtch *nswp, *s;
    register short temp;

    nswp = &sp[ncases];
    nswp->sw_value = nval;
    nswp->sw_label = nlab;
    s = nswp--;
    for( ; --ncases >= 0; s--, nswp-- ) {
        if( s->sw_value == nswp->sw_value )
            error("duplicate case value");
        if( s->sw_value < nswp->sw_value ) {
            temp = s->sw_value;
            s->sw_value = nswp->sw_value;
            nswp->sw_value = temp;
            temp = s->sw_label;
            s->sw_label = nswp->sw_label;
            nswp->sw_label = temp;
        }
    }
}

/* outforreg - generate assignment for switch and return*/
outforreg(op,ltp,rtp)           /*returns - none*/
int op;                         /*operator for build tree*/
struct tnode *ltp;              /*left expression tree*/
struct tnode *rtp;              /*right expression tree*/
{
    register struct tnode *tp;

    opp = opstack;
    opdp = opdstack;
    pushopd(ltp);
    pushopd(rtp);
    maketree(op);
    if( tp = (struct tnode *)popopd() )
        outcforreg(tp->t_right);
    opp = 0; opdp = (char **)0;
}

/* outassign - generate assignment for function args*/
outassign(ltp,rtp)              /*returns - none*/
struct tnode *ltp;              /*left expression tree*/
struct tnode *rtp;              /*right expression tree*/
{
    opp = opstack;
    opdp = opdstack;
    pushopd(ltp);
    pushopd(rtp);
    maketree(ASSIGN);
    outexpr(popopd());
    opp = 0; opdp = (char **)0;
}
