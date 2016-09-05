/*
 *	Put all the listing stuff in here
 */
#include "as68.h"
#include <stdlib.h>
#include <string.h>


#define	LPP	58							/* # Lines per listing page     */
int xline = LPP;						/* Current line on page         */

int xpage = 0;							/* Current page #               */

short pline;							/* Current listing line #       */

int symcmp PROTO((const VOIDPTR a, const VOIDPTR b));
int psyme PROTO((struct symtab *osypt));


/****************************************************************************/
/*																		    */
/*	.page directive handlers (from dir.c).								    */
/*																		    */
/****************************************************************************/
/* Pass 1 .page routine */
VOID hpage(NOTHING)
{
	opitb();							/* Output statement beginning       */
	wostb();							/* Remaining part of statement      */
	igrst();							/* Ignore optional comment          */
}


/* Pass 2 .page routine */
VOID spage(NOTHING)
{
	print(0);							/* Print directive line             */
	xline = LPP;						/* Force top of                     */
	page();								/*              Listing page        */
}


/****************************************************************************/
/*																		    */
/*	Symbol table print routine.  "psyms" is called AFTER the symbol table   */
/*	has been output to the loader file.  We sort the symbol table, and print*/
/*	the sorted table in PDP-11ish (RT-11) fashion, with a reasonable format */
/*	for the symbols.													    */
/*																		    */
/****************************************************************************/
VOID psyms(NOTHING)
{
	register long j;					/* Temporary                */
	register struct symtab *p;					/* -> Symbol table entries  */

	xline = LPP;						/* Force page               */
	page();								/*              Eject       */
	printf("S y m b o l   T a b l e\n\n");	/* Print Header             */
	xline++;							/* Bump line counter        */
	j = (((__intptr_t)lmte - (__intptr_t)bmte) / sizeof(*p));		/* Compute # elements       */
	qsort(bmte, (int) j, sizeof(*p), symcmp);	/* Sort'em                  */
	j = 0;								/* Count symbols / line     */
	for (p = bmte; p < lmte; p++)	/* Loop through symbol table */
	{
		if (j > 3)						/* 4 Symbols / line         */
		{
			printf("\n");
			page();						/* Check for top of page    */
			j = 0;						/* Reset counter            */
		}
		j += psyme(p);					/* Print 1 table entry (maybe) */
	}
	if (j <= 3)							/* Partial line?            */
		printf("\n");					/*  Yes, finish it          */
}


/* Qsort comparison function */
int symcmp(P(const VOIDPTR) a, P(const VOIDPTR) b)
PP(register const VOIDPTR a;)
PP(register const VOIDPTR b;)										/* -> Elts to compare       */
{
	return strncmp(a, b, SYNAMLEN);
}


/****************************************************************************/
/*																			*/
/*	Psyme function.  This function prints a single symbol table entry on	*/
/*	the listing file, complete with TEXT, DATA, BSS, EXT, or UNDEF tag.		*/
/*																			*/
/****************************************************************************/
int psyme(P(struct symtab *) osypt)
PP(register struct symtab *osypt;)
{
	register char *p;					/* -> Name field            */
	register int i;						/* Count register           */

	/* Do we need to print it?  */
	if ((osypt->flags & SYER) != 0 || (osypt->flags & SYIN))
		return 0;
	
	p = &(osypt->name[0]);				/* p -> Symbol name field   */
	for (i = 0; i < SYNAMLEN; i++)
	{
		/* Print name */
		if (*p)
			putchar(*p);
		else
			putchar(' ');
		p++;
	}
	
	/* Align descriptor */
	printf("  ");
	/* External Reference? */
	if (osypt->flags & SYXR)
	{
		printf("******** EXT   ");		/* Macro-11 style.          */
		return 1;
	}
	/* Defined? */
	if (osypt->flags & SYDF)
	{
		/* Print high word */
		puthex(osypt->vl1.u.hiword, 4);
		/* And   low  word */
		puthex(osypt->vl1.u.loword, 4);
		if (osypt->flags & SYRA)
			printf(" DATA  ");
		else if (osypt->flags & SYRO)
			printf(" TEXT  ");
		else if (osypt->flags & SYBS)
			printf(" BSS   ");
		else
			printf(" ABS   ");
	} else
	{
		/* Bump Error count */
		nerror++;
		/* Identify FUBAR */
		printf("*UNDEFINED*    ");
	}
	return 1;
}


/*
 * output source and object listing
 *	call with
 *		2 => print address and binary code only
 *		1 => object in ins[] and instr type in format
 *		0 => print address only
 */
VOID print(P(int) pflag)
PP(int pflag;)
{
	register short i;
	register short *pi;

	if (!prtflg || fchr == CEOF)		/* no printing desired, or end of source file */
		return;

	i = instrlen;
	instrlen = 1;						/* to print preceeding lines */
	while (pline < p2absln)
	{									/* need to print some lines */
		page();
		printf("%4d ", pline);			/* put source line num on listing */
		printf("                              ");	/* align the source */
		prtline(1);
		putchar('\n');
		if ((fchr = gchr()) == CEOF)
			return;
		pline++;
	}
	instrlen = i;

	/* output current address, binary, and source */
	page();
	printf("%4d ", p2absln);			/* put source line num on listing */
#ifdef __ALCYON__
	puthex(loctr.hiword, 4);
	puthex(loctr.loword, 4);
#else
	puthex((int)(loctr >> 16), 4);
	puthex((int)loctr, 4);
#endif
	putchar(' ');
	if (!pflag)							/* no binary */
	{
		printf("                     ");	/* blanks instead */
	} else
	{
		pi = ins;
		for (i = 0; i < (instrlen / 2); i++)	/* binary */
			puthex(*pi++, 4);
		if (instrlen & 1)
		{
			i++;
			puthex(*pi, 2);
			printf("  ");				/* Word align */
		}
		putchar(' ');
		for (; i < 5; i++)				/* four bytes max per line */
			printf("    ");				/* align the source */
	}
	if (pline > p2absln || pflag == 2)
	{
		putchar('\n');					/* end of line */
	} else
	{
		prtline(0);
		if (fchr == CEOF)
			return;
		putchar('\n');
		fchr = gchr();
		pline++;
	}
}


VOID prtline(P(int) flg)
PP(int flg;)
{
	while (fchr != EOLC && fchr != CEOF)
	{
		putchar(fchr);
		fchr = gchr();
	}
}


/*
 *	Heading Print routine
 */
VOID page(NOTHING)
{
	if ((prtflg == 0) || (++xline < LPP))
		return;
	printf("\fC P / M   6 8 0 0 0   A s s e m b l e r\t\t%s\t\tPage%4d\n", "Revision 04.03", ++xpage);
	printf("Source File: %s\n\n", sfname);
	xline = 3;
}
