#include "lib.h"
#include <stdlib.h>

__EXITING abort(NOTHING)
{
	for (;;)
	{
		asm("illegal");
	}
}
