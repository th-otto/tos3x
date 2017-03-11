#include "lib.h"
#include <string.h>

/*
 * memcmp - compare bytes
 *
 * CHARBITS should be defined only if the compiler lacks "unsigned char".
 * It should be a mask, e.g. 0377 for an 8-bit machine.
 */
#define	UNSCHAR(c)	((c) & 0xff)

/* <0, == 0, >0 */
int memcmp(P(const VOIDPTR) s1, P(const VOIDPTR) s2, P(size_t) size)
PP(const VOIDPTR s1;)
PP(const VOIDPTR s2;)
PP(size_t size;)
{
	register const char *scan1;
	register const char *scan2;
	register size_t n;

	scan1 = (const char *) s1;
	scan2 = (const char *) s2;
	for (n = size; n > 0; n--)
	{
		if (*scan1 == *scan2)
		{
			scan1++;
			scan2++;
		} else
		{
			return UNSCHAR (*scan1) - UNSCHAR (*scan2);
		}
	}
	
	return 0;
}
