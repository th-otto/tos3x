#include "lib.h"

VOID alrem(P(long *) al1, P(long) al2)
PP(long *al1;)
PP(long al2;)
{
	*al1 = lrem(*al1, al2);
}
