#include <time.h>
#include "lib.h"

#define DAYS_PER_4YEARS ((time_t)(365 * 4 + 1))
#define HOURS_PER_4YEARS (DAYS_PER_4YEARS * 24)

static char const _monDayLen[12] = {
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31
};

/*
 * reentrant function used internally
 */
struct tm *_conSD_r(P(time_t) clock, P(int) local, P(struct tm *) tp)
PP(time_t clock;)
PP(int local;)
PP(struct tm *tp;)
{
	int days;
	int leaps;
	int leaphours;

	tzset();
	if (clock < 0)
		clock = 0;
	tp->tm_gmtoff = -timezone;
	if (local && daylight)
	{
		tp->tm_isdst = 1;
		tp->tm_gmtoff += 3600;
	} else
	{
		tp->tm_isdst = 0;
	}
	if (local)
		clock += tp->tm_gmtoff;
	tp->tm_sec = (int)(clock % 60);
	clock /= 60;
	tp->tm_min = (int)(clock % 60);
	clock /= 60;
	leaps = (int)(clock / HOURS_PER_4YEARS);
	tp->tm_year = leaps * 4 + 70;
	days = leaps * (int)DAYS_PER_4YEARS;
	
	clock = clock % HOURS_PER_4YEARS;
	for (;;)
	{
		leaphours = 365 * 24;
		if ((tp->tm_year & 3) == 0)
			leaphours += 24;
		if (clock < leaphours)
			break;
		days += leaphours / 24;
		tp->tm_year++;
		clock -= leaphours;
	}
	
	tp->tm_hour = (int)(clock % 24);
	clock /= 24;
	tp->tm_yday = (int)clock;
	days += (int)(clock + 4);
	tp->tm_wday = days % 7;
	clock++;
	if ((tp->tm_year & 3) == 0)
	{
		if (clock > 60)
		{
			clock--;
		} else if (clock == 60)
		{
			tp->tm_mon = 1;
			tp->tm_mday = 29;
			return tp;
		}
	}
	tp->tm_mon = 0;
	while (clock > _monDayLen[tp->tm_mon])
	{
		clock -= _monDayLen[tp->tm_mon];
		tp->tm_mon++;
	}
	tp->tm_mday = (int)clock;
	return tp;
}


/*
 * non-reentrant compatibility function.
 */
struct tm *_conSD(P(time_t) clock, P(int) local)
PP(time_t clock;)
PP(int local;)
{
	static struct tm tm;
	
	return _conSD_r(clock, local, &tm);
}
