#include <stdlib.h>

VOID abort(NOTHING)
{
	for (;;)
	{
		asm("illegal");
	}
}
