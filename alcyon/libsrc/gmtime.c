#include <time.h>
#include "lib.h"

struct tm *gmtime(P(const time_t *)ptime)
PP(const time_t *ptime;)
{
	time_t ttime;

	if (ptime == NULL)
		return NULL;
	ttime = *ptime;
	if (ttime < 0)
		return NULL;
	return _conSD(ttime, 0);
}
