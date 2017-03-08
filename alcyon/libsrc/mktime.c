#include <time.h>
#include "lib.h"

time_t mktime(P(struct tm *) tm)
PP(struct tm *tm;)
{
	time_t t;
	
	t = _cnvDS(tm);
	if (t == (time_t)-1)
		return t;
	tzset();
	t += timezone;
	if (daylight)
		t -= 3600;
	_conSD_r(t, 1, tm);
	return t;
}
