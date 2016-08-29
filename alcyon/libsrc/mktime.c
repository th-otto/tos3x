#include <time.h>
#include "lib.h"

time_t mktime(P(struct tm *) tm)
PP(struct tm *tm;)
{
	time_t t;
	
	t = _cnvDS(tm);
	if (t < 0)
		return -1;
	_conSD_r(t, 0, tm);
	if (daylight)
	{
		tm->tm_isdst = 1;
		t -= 3600;
	}
	t -= timezone;
	if (t < 0)
		return -1;
	return t;
}
