#include <time.h>
#include <ostruct.h>
#include "lib.h"

time_t ftimtosec(P(_DOSTIME *) f)
PP(_DOSTIME *f;)
{
	struct tm tm;
	
	tm.tm_sec = (f->time & 0x1f) << 1;
	tm.tm_min = (f->time >> 5) & 0x3f;
	tm.tm_hour = (f->time >> 11) & 0x1f;
	tm.tm_mday = (f->date & 0x1f);
	tm.tm_mon = ((f->date >> 5) & 0x0f) - 1;
	tm.tm_year = ((f->date >> 9) & 0x7f) + 80;
	return _cnvDS(&tm);
}
