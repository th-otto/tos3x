#include "lib.h"

/*
 *	this routine multiplies together two 32 bit signed long integers.
 *	the first arg is a pointer to the long, which is multiplied by the
 *	second arg, with the result being stored at the firstarg.
 *	lmul.s does the meat of the work
 */

VOID almul(P(register long *, l1), P(long, l2))
PP(register long *, l1;)
PP(long, l2;)
{
	*l1 = lmul(*l1, l2);
}
