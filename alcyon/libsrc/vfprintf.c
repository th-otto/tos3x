#include "lib.h"

int vfprintf(P(FILE *) fp, P(const char *fmt) fmt, P(va_list) args)
PP(FILE *fp;)
PP(const char *fmt;)
PP(va_list args;)
{
	return __doprint(fp, fmt, 0, args);
}
