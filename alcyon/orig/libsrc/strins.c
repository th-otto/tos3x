/********************************************************/
/*							*/
/*	strins(s1,s2,pos)				*/
/*	insert s1 into s2 at pos			*/
/*	(pos starts at 0)				*/
/*							*/
/********************************************************/

#include "lib.h"
#include <string.h>

/*
PROCEDURE INSERT(SOURCE:STRING; VAR DEST:STRING; INDEX:INTEGER);
BEGIN
  MOVERIGHT(DEST[INDEX],DEST[INDEX+LENGTH(SOURCE)],LENGTH(DEST)-INDEX+1);
  MOVE(SOURCE[1],DEST[INDEX],LENGTH(SOURCE));
  DEST[0] := CHR(LENGTH(DEST) + LENGTH(SOURCE))
END;
*/
VOID strins(P(const char *) s1, P(char *) s2, P(int) pos)
PP(const char *s1;)
PP(char *s2;)
PP(int pos;)
{
	int ls1, ls2, lm;							/* length s1, length s2, length to move */
	int mp1, mp2;								/* move "pointers" (indexes) */
	int i, j, k;

	ls1 = strlen(s1);
	ls2 = strlen(s2);
	lm = ls2 - pos;

	mp1 = pos + lm;
	mp2 = pos + ls1 + lm;

	s2[mp2] = '\0';

	for (i = 0; i < lm; i++)
		s2[--mp2] = s2[--mp1];

	for (i = 0; i < ls1; i++)
		s2[pos + i] = s1[i];
	UNUSED(j);
	UNUSED(k);
}


/****************************************************************
 *
 *	strdel(sp,len)
 *	remove len characters from string pointed to by sp
 *
 ***************************************************************/
VOID strdel(P(char *) sp, P(int) len)
PP(register char *sp;)
PP(register int len;)
{
	register char *ssp;
	ssp = sp + len;

	while ((*sp++ = *ssp++) != 0)
		;
}
