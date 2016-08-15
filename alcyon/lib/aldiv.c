#include "lib.h"

VOID aldiv(P(long *) al1, P(long) al2)
PP(long *al1;)
PP(long al2;)
{
	*al1 = ldiv(*al1, al2);
}
