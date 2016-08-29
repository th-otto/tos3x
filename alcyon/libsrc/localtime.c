#include <time.h>
#include "lib.h"

struct tm *localtime(P(const time_t *) ptime)
PP(const time_t *ptime;)
{
	time_t ttime;

	if (ptime == NULL)
		return NULL;
	/* Check for invalid time value */
	ttime = *ptime;
	if (ttime < 0)
		return 0;

	return _conSD(ttime, 1);
}
