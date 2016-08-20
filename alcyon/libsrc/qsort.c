/**************************************************************************
*
*			q s o r t   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"qsort" sorts the "number" of items, each of length "size", starting
*	at "base", using function "compare" to compare 2 items in the vector.
*
*	Calling sequence:
*		ret = qsort(base,number,size,compar)
*	Where:
*		ret = always 0
*		base -> start of items vector
*		number = number of elements in vector
*		size = number of bytes per item in vector
*		compar -> comparator function, taking ptrs to items,
*			returning int:
*			compar(a,b) <  0  if  *a < *b
*			compar(a,b) == 0  if  *a == *b
*			compar(a,b) >  0  if  *a > *b
*
*	"qsort" uses the quicksort algorithm by C.A.R. Hoare.
*	Ref: "Software Tools in Pascal" by Kernighan & Plauger.
*****************************************************************************/

#include "lib.h"
#include <stdlib.h>

#define LINEPOS(nn) ((nn) * siz + (char *)bas)
#define EXCHANGE(aa,bb) _swap(aa, bb, siz)


static VOID _swap(P(char *) a, P(char *) b, P(size_t) wid)
PP(register char *a;)
PP(register char *b;)
PP(register size_t wid;)
{
	register char tmp;

	if (a != b)
		for (; wid-- > 0; a++, b++)
		{
			tmp = *a;
			*a = *b;
			*b = tmp;
		}
}


VOID qsort(P(VOIDPTR) bas, P(size_t) num, P(size_t) siz, P(__compar_fn_t) cmp)
PP(VOIDPTR bas;)
PP(size_t num;)
PP(size_t siz;)
PP(__compar_fn_t cmp;)
{
	register int i, j;
	register char *pivline;

	if (num > 1)
	{
		i = 0;
		j = num - 1;
		pivline = LINEPOS(j);			/* choose last line for pvt */
		do
		{
			while (i < j && (*cmp) (LINEPOS(i), pivline) <= 0)
				i++;
			while (j > i && (*cmp) (LINEPOS(j), pivline) >= 0)
				j--;
			if (i < j)					/* out of order pair        */
				EXCHANGE(LINEPOS(i), LINEPOS(j));
		} while (i < j);
		EXCHANGE(LINEPOS(i), pivline);
		if (i < num - 1 - i)			/* sort shorter subset 1st  */
		{
			qsort(bas, i, siz, cmp);
			qsort(LINEPOS(i + 1), num - 1 - i, siz, cmp);
		} else
		{
			qsort(LINEPOS(i + 1), num - 1 - i, siz, cmp);
			qsort(bas, i, siz, cmp);
		}
	}
}
