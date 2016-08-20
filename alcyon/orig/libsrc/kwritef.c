#include "lib.h"
#include <stdlib.h>
#include "klib.h"

VOID xwritefail(NOTHING)
{
	fprintf(stderr, "Write error on output file\n");
	exit(-1);
}
