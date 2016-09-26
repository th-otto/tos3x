/*
 *	Put all the listing stuff in here
 */
#include "as68.h"
#include <stdlib.h>
#include <string.h>


#define	LPP	58							/* # Lines per listing page     */
static int xline = LPP;					/* Current line on page         */
static int xpage = 0;					/* Current page #               */
static int pline = 1;					/* number of last printed line */



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
/*																			*/
/*	Psyme function.  This function prints a single symbol table entry on	*/
/*	the listing file, complete with TEXT, DATA, BSS, EXT, or UNDEF tag.		*/
/*																			*/
/****************************************************************************/
static int psyme(P(struct symtab *) osypt)
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
		/* Print value */
		printf("%08lx", osypt->vl1);
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
		printf(_("*UNDEFINED*    "));
	}
	return 1;
}


/****************************************************************************/
/*																		    */
/*	Symbol table print routine.  "psyms" is called AFTER the symbol table   */
/*	has been output to the loader file.  We sort the symbol table, and print*/
/*	the sorted table in PDP-11ish (RT-11) fashion, with a reasonable format */
/*	for the symbols.													    */
/*																		    */
/****************************************************************************/

/* Qsort comparison function */
static int symcmp(P(const VOIDPTR) a, P(const VOIDPTR) b)
PP(register const VOIDPTR a;)
PP(register const VOIDPTR b;)										/* -> Elts to compare       */
{
	register const struct symtab *s1 = *((const struct symtab *const *)a);
	register const struct symtab *s2 = *((const struct symtab *const *)b);
	
	return strncmp(s1->name, s2->name, SYNAMLEN);
}


VOID psyms(NOTHING)
{
	register size_t i, j, num;					/* Temporary                */
	register struct symtab **sorted;
	register struct symtab *p;					/* -> Symbol table entries  */
	
	xline = LPP;						/* Force page               */
	page();								/*              Eject       */
	printf(_("S y m b o l   T a b l e\n\n"));	/* Print Header             */
	xline++;							/* Bump line counter        */
	/* Compute # elements */
	for (num = 0, p = bmte; p != emte; num++, p = p->next)
		;
	sorted = (struct symtab **)malloc(num * sizeof(struct symtab *));
	if (sorted == NULL)
		return;
	for (i = 0, p = bmte; p != emte; i++, p = p->next)
		sorted[i] = p;
	qsort(sorted, num, sizeof(p), symcmp);	/* Sort'em                  */
	
	j = 0;								/* Count symbols / line     */
	for (i = 0; i < num; i++)
	{
		if (j > 3)						/* 4 Symbols / line         */
		{
			printf("\n");
			page();						/* Check for top of page    */
			j = 0;						/* Reset counter            */
		}
		j += psyme(sorted[i]);			/* Print 1 table entry (maybe) */
	}
	if (j <= 3)							/* Partial line?            */
		printf("\n");					/*  Yes, finish it          */
	free(sorted);
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
	printf("%4d %08lx", p2absln, loctr);			/* put source line num on listing */
	putchar(' ');
	if (!pflag)							/* no binary */
	{
		printf("                     ");	/* blanks instead */
	} else
	{
		pi = ins;
		for (i = 0; i < (instrlen / 2); i++)	/* binary */
			printf("%04x", (uint16_t)*pi++);
		if (instrlen & 1)
		{
			i++;
			printf("%02x  ", *pi);
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
	printf(_("\fC P / M   6 8 0 0 0   A s s e m b l e r\t\t%s\t\tPage%4d\n"), "Revision 04.03", ++xpage);
	printf(_("Source File: %s\n\n"), sfname);
	xline = 3;
}
