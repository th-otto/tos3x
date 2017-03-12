/* dt.c - common C routines for DATE and TIME utilities
*/

#include <stdarg.h>
#include <osbind.h>
#include "dt.h"

int get_num(P(char **) q)
PP(char **q;)
{
	register int i, j;
	register char *p;
	
	j = -1;
	p = *q;
	while (*p == ' ')
		p++;
	for (i = 0; (*p >= '0') && (*p <= '9'); j++, p++)
	{
		i = *p - '0' + i * 10;
	}
	*q = p;
	if (j == -1)
		return j;
	return i;
}


VOID put_num(P(unsigned int) i)
PP(unsigned int i;)
{
	register unsigned int q, r;
	register int j;

	j = i;
	if (j < 10)
		Cconout('0');
	q = i / 10;
	r = i % 10;

	if (q)
		Cconout(q % 10 + '0');

	Cconout(r + '0');
}
