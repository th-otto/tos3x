/*  fsbuf.c - buffer mgmt for file system				*/


#include "tos.h"
#include "fs.h"
#include "bios.h"
#include <toserrno.h>


/*
 *  packit - pack into user buffer
 */

char *packit(P(const char *) s, P(char *) d)
PP(register const char *s;)
PP(register char *d;)
{
	const char *s0;
	register int i;

	if (!(*s))
		goto pakok;

	s0 = s;
	for (i = 0; (i < 8) && (*s) && (*s != ' '); i++)
		*d++ = *s++;

	if (*s0 == '.')
		goto pakok;

	s = s0 + 8;							/* ext */

	if (*s != ' ')
		*d++ = '.';
	else
		goto pakok;

	for (i = 0; (i < 3) && (*s) && (*s != ' '); i++)
		*d++ = *s++;
  pakok:*d = 0;
	return d;
}
