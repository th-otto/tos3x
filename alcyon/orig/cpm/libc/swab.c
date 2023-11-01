/* swab - swap (hi/lo) bytes in an area of memory */
/*	constructed to allow in-place swabs (fr==to) */
#include <compiler.h>
#include <types.h>

VOID swab(P(VOIDPTR) _from, P(VOIDPTR) _to, P(ssize_t) num)
PP(VOIDPTR _from;)
PP(VOIDPTR _to;)
PP(register ssize_t num;)
{
	register char *from = _from;
	register char *to = _to;
	register char t;

	for (; num > 0; num -= 2, from += 2, to += 2)
	{
		t = from[0];
		to[0] = from[1];
		to[1] = t;
	}
#ifdef __ALCYON__
	return 0;
#endif
}
