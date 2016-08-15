/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/**
 * symbol table entry allocation and lookup routines
**/

#include "parser.h"

#define	STEL	HSIZE/2

struct symbol *symtab[HSIZE];		/*hash table*/
struct symbol *symbols;				/*pointer to next avail symbol buf*/

/* syminit - initialize the symbol table, install reswords*/
/*		Goes thru the resword table and installs them into the symbol*/
/*		table.*/
syminit()							/* returns - none*/
{
	register struct resword *rp;

	for( rp = &reswords[0]; rp->r_name != 0; rp++ )
		install(rp->r_name,SRESWORD|SDEFINED,rp->r_value);
}

/**
 * install - install a symbol in the symbol table
 *		Allocates a symbol entry, copies info into it and links it
 *		into the hash table chain.
**/
char *
install(sym,attrib,offset)			/* returns pointer to symbol struct*/
char *sym;							/* symbol to install*/
int attrib;							/* attribues of symbol*/
int offset;							/* symbol offset (resword value)*/
{
	register struct symbol *sp;
	register short i;

	while( (sp=symbols) <= 0 ) {
		if( (sp = (struct symbol *)sbrk(SYMSIZE)) == -1 )
			ferror("symbol table overflow");
		for( i = SYMSIZE/(sizeof *symbols); --i >= 0; ) {
			if (sp <= 0)
				ferror("bad symbol table");
			sp->s_next = symbols;
			symbols = sp++;
		}
	}
	symbols = sp->s_next;
	sp->s_attrib = attrib;
	sp->s_sc = sp->s_type = sp->s_dp = sp->s_ssp = 0;
	sp->s_offset = offset;
	if (in_struct) {
		sp->s_par = struc_parent[in_struct];
		hold_sib = struc_sib[in_struct];
		sp->s_scope = (infunc) ? FUNC_SCOPE : GLOB_SCOPE;		/* [vlh] 4.2 */
		
#ifdef DEBUG
		if (symdebug)
			printf("    struct element parent is %.8s\n", sp->s_par->s_symbol);
#endif
		if (struc_sib[in_struct]) {
			struc_sib[in_struct]->s_sib = sp;
#ifdef DEBUG
		if (symdebug)
			printf("    sparent sib is %.8s\n",struc_sib[in_struct]->s_symbol);
#endif
		}
		else {
			struc_parent[in_struct]->s_child = sp;
#ifdef DEBUG
			if (symdebug)
				printf("    child of %.8s\n",struc_parent[in_struct]->s_symbol);
#endif
		}
		struc_sib[in_struct] = sp;
		sp->s_child = sp->s_sib = 0;
	}
	else {
		sp->s_sib = sp->s_child = sp->s_par = 0;
		sp->s_scope = scope_level;			/* [vlh] 4.2 */
	}
	symcopy(sym,sp->s_symbol);			/*copy symbol to symbol struct*/
	i = symhash(sym,in_struct|smember);	/*link into chain list*/
	sp->s_next = symtab[i];
	symtab[i] = sp;
	return((char *)sp);
}

/* lookup - looks up a symbol in symbol table*/
/*		Hashes symbol, then goes thru chain, if not found, then*/
/*		installs the symbol.*/
char *
lookup(sym,force)			/* returns pointer to symbol buffer*/
char *sym;					/* pointer to symbol*/
int force;					/* [vlh] 4.2 force entry in symbol table */
{
	register struct symbol *sp, *hold;
	register char *p;
	short exact, prev_level;		/* same name, diff type or offset */

	p = sym; prev_level = 0;
	for( sp = symtab[symhash(p,0)]; sp != 0; sp = sp->s_next )
		if((sp->s_attrib&(SRESWORD|STYPEDEF)) && symequal(p,sp->s_symbol))
			return(sp);
	if (!(smember|in_struct)) {	/*[vlh]*/
		hold = 0;
		for( sp=symtab[symhash(p,0)]; sp!=0; sp=sp->s_next )
			if(symequal(p,sp->s_symbol))
				if (scope_level == sp->s_scope)	/* [vlh] 4.2 added scope... */
					return(sp);					/* perfect scope match */
				else
					if (prev_level <= sp->s_scope) {	/* only closer scope */
						hold = sp;
						prev_level = sp->s_scope;
					}
		if (!force && hold) /* [vlh] 4.2 added scope... */
			return(hold);
	}
	else {	/* doing a declaration or an expression */
		hold = 0; exact = 0;
		for( sp=symtab[symhash(p,in_struct|smember)]; sp!=0; sp=sp->s_next ) {
			if( symequal(p,sp->s_symbol) ) {
				if (symsame(sp,hold,&exact)) 
					return(sp);
				else if (!hold && !exact) 
					hold = sp;
			}
		}
		if (hold && (instmt || in_struct==0 || smember!=0)) /*4.1*/
			return(hold);
	}
#ifdef DEBUG
	if (symdebug) printf("installing [%.8s] %d\n",p,indecl);
#endif
	return(install(p,0,0));
}

/* freesyms - frees all local symbols at end of function declaration*/
/*		Searches thru symbol table, deleting all symbols marked as locals*/
freesyms(level)							/* returns - none*/
int level;								/* [vlh] 4.2 scope levels... */
{
	register short i;
	register struct symbol *sp, *tp, *nextp, **htp;

	for( htp = &symtab[0], i = HSIZE; --i >= 0; htp++ )
		for( tp = 0, sp = *htp; sp != 0; sp = nextp ) {
			nextp = sp->s_next;
			if (level == FUNC_SCOPE)
				if( !(sp->s_attrib&SDEFINED) ) {
					error("undefined label: %.8s",sp->s_symbol);
					sp->s_attrib |= SDEFINED;
				}
			if (sp->s_attrib & (SGLOBAL|SRESWORD) )
				tp = sp;
			else if ( !(sp->s_attrib & SGLOBAL) && sp->s_scope < level)
				tp = sp;
			else {
#ifdef DEBUG
	if (symdebug)
		printf("freeing %s, level %d\n",sp->s_symbol,level);
#endif
				if( tp )
					tp->s_next = sp->s_next;
				else
					*htp = sp->s_next;
				sp->s_next = symbols;
				symbols = sp;
			}
		}
}

/* chksyms - checks symbol table for undefined symbols, etc.*/
/*		Goes thru the symbol table checking for undeclared forward*/
/*		referenced structures, and outputs local symbols for debugger.*/
chksyms(ok)							/* returns - none*/
int ok;
{
	register struct symbol **htp, *sp;
	register short i, sc;

	for( htp = &symtab[0], i = HSIZE; --i >= 0; htp++ )
		for( sp = *htp; sp != 0; sp = sp->s_next ) {
			sc = sp->s_sc;
			if(sc!=0 && sp->s_ssp>=0 && (BTYPE(sp->s_type))==FRSTRUCT) {
				sp->s_ssp = frstab[sp->s_ssp]->s_ssp;	/* 3.4 ssp>0 */
				sp->s_type = (sp->s_type&~TYPE) | STRUCT;
				sp->s_par = frstab[sp->s_ssp]->s_par;
			}
			if( sc == PDECLIST ) {
				error("not in parameter list: %.8s",sp->s_symbol);
				sp->s_sc = AUTO;
				if (ok) 
					outlocal(sp->s_type,sc,sp->s_symbol,sp->s_offset);
			}
		}
}

/* symhash - compute hash value for symbol*/
/*		Sums the symbols characters and takes that modulus the hash table*/
/*		size.*/
symhash(sym,stel)					/* returns hash value for symbol*/
char *sym;							/* pointer to symbol*/
int stel;							/* structure element flag*/
{
	register char *p;
	register short hashval, i;

	hashval = (stel ? STEL : 0 );
	for( p = sym, i = SSIZE; *p != '\0' && i > 0; i-- )
		hashval += *p++;
	return( hashval % HSIZE );
}

/* symequal - check for symbol equality*/
/*		Does comparison between two symbols.*/
symequal(sym1,sym2)					/* returns 1 if equal, 0 otherwise*/
char *sym1;							/* pointer to first symbol*/
char *sym2;							/* pointer to second symbol*/
{
	register char *p, *q;
	register short i;

	for( p = sym1, q = sym2, i = SSIZE; *p == *q++; )
		if( *p++ == '\0' || --i == 0 )
			return(1);
	return(0);
}

/* symsame - symbol member same as declared */
symsame(sp,hold,exact)	/* [vlh] */
struct symbol *sp, *hold;
short *exact;
{
	if (struc_parent[in_struct]==sp->s_par) /* all structures have parents */
		return(1);
	if (hold)
		if (sp->s_type != hold->s_type || sp->s_offset != hold->s_offset)
			*exact = 1;
	return(0);
}

/* symcopy - symbol copy*/
/*		Copies one symbol to another.*/
symcopy(sym1,sym2)					/* returns - none*/
char *sym1;							/* pointer to symbol to copy*/
char *sym2;							/* pointer to area to copy to*/
{
	register char *p, *q;
	register short i;

	for( p = sym1, q = sym2, i = SSIZE; --i >= 0; )
		*q++ = ( *p ? *p++ : '\0');
}
