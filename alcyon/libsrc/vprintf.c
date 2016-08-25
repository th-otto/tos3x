#include "lib.h"

int vprintf(P(const char *fmt) fmt, P(va_list) args)
PP(const char *fmt;)
PP(va_list args;)
{
	return __doprint(stdout, fmt, 0, args);
}
