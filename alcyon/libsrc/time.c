#include <time.h>
#include "lib.h"
#include <osif.h>

time_t time(P(time_t *) ptime)
PP(time_t *ptime;)
{
	time_t ttime;
	unsigned short tos_date;
	unsigned short tos_time;
	struct tm tm;
	
	tos_date = gemdos(0x2a);
	tos_time = gemdos(0x2c);

	tm.tm_sec = (tos_time & 0x1f) * 2;
	tm.tm_min = (tos_time >> 5) & 0x3f;
	tm.tm_hour = tos_time >> 11;
	tm.tm_mday = tos_date & 0x1f;
	tm.tm_mon = ((tos_date >> 5) & 0xf) - 1;
	tm.tm_year = (tos_date >> 9) + 80;
	tm.tm_isdst = -1;
	
	ttime = mktime(&tm);
	if (ttime < 0)
		return ttime;
	
	ttime += timezone;
	
    if (ptime)
    {
        *ptime = ttime;
    }
    return ttime;
}
