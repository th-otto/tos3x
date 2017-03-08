#include <time.h>
#include "lib.h"

static short const _monDaySum[12] = {
	0,
	31,
	59,
	90,
	120,
	151,
	181,
	212,
	243,
	273,
	304,
	334
};

time_t _cnvDS(P(const struct tm *) tm)
PP(const struct tm *tm;)
{
	int months;
	int years;
	int leapdays;
	int days;
	
	years = tm->tm_year - 70;
	months = tm->tm_mon;
	while (months < 0)
	{
		years--;
		months += 12;
	}
	while (months >= 12)
	{
		years++;
		months -= 12;
	}
	if (years < 0)
		return (time_t)-1;
	leapdays = (years + 1) >> 2;
	days = years * 365 + leapdays + _monDaySum[months] + tm->tm_mday - 1;
	if (((leapdays + 2) & 3) == 0 && months >= 2)
		days++;
	return (((long)days * 24 + tm->tm_hour) * 60 + tm->tm_min) * 60 + tm->tm_sec;
}
