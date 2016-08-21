/*
 *	Dummy routine for use with floating point stuff ...
 */
#include "lib.h"
#include <math.h>

double _atof(P(const char *) buf)
PP(const char *buf;)
{
	return atof(buf);
}
