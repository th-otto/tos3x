#include "lib.h"
#include <time.h>
#include <errno.h>


#define DAYSPERWEEK 7
#define MONSPERYEAR 12
#define HUNDREDYEAROFFSET 19

struct abbrev {
	char name[4];
};

static struct abbrev const wday_name[DAYSPERWEEK] =
{
	{ 'S', 'u', 'n', ' ' },
	{ 'M', 'o', 'n', ' ' },
	{ 'T', 'u', 'e', ' ' },
	{ 'W', 'e', 'd', ' ' }, 
	{ 'T', 'h', 'u', ' ' },
	{ 'F', 'r', 'i', ' ' },
	{ 'S', 'a', 't', ' ' }
};

static struct abbrev const mon_name[MONSPERYEAR] = 
{
	{ 'J', 'a', 'n', ' ' },
	{ 'F', 'e', 'b', ' ' },
	{ 'M', 'a', 'r', ' ' },
	{ 'A', 'p', 'r', ' ' },
	{ 'M', 'a', 'y', ' ' },
	{ 'J', 'u', 'n', ' ' },
	{ 'J', 'u', 'l', ' ' },
	{ 'A', 'u', 'g', ' ' },
	{ 'S', 'e', 'p', ' ' },
	{ 'O', 'c', 't', ' ' },
	{ 'N', 'o', 'v', ' ' },
	{ 'D', 'e', 'c', ' ' }
};

typedef union
{
	char text[26];
	struct
	{
		struct abbrev WeekDay;
		struct abbrev Month;
		char Day[2];
		char Space1;
		char Hour[2];
		char Sep1;
		char Minute[2];
		char Sep2;
		char Second[2];
		char Space2;
		char Year[2][2];
		char lb;
		char zt;
	};
} timebuf_t;


static VOID IntToChar2(P(int) x, P(char *)array)
PP(int x;)
PP(char *array;)
{
	array[0] = '0' + (x / 10);
	array[1] = '0' + (x % 10);
}


static VOID FillBuf(P(timebuf_t *) buf, P(const struct tm *) ptm)
PP(register timebuf_t *buf;)
PP(register const struct tm *ptm;)
{
	/* Format looks like this: 
	 * "Sun Mar 01 12:34:56 1902\n\0" */
	buf->WeekDay = wday_name[ptm->tm_wday];
	buf->Month = mon_name[ptm->tm_mon];
	IntToChar2(ptm->tm_mday, buf->Day);
	buf->Space1 = ' ';
	IntToChar2(ptm->tm_hour, buf->Hour);
	buf->Sep1 = ':';
	IntToChar2(ptm->tm_min, buf->Minute);
	buf->Sep2 = ':';
	IntToChar2(ptm->tm_sec, buf->Second);
	buf->Space2 = ' ';
	IntToChar2(ptm->tm_year / 100 + HUNDREDYEAROFFSET, buf->Year[0]);
	IntToChar2(ptm->tm_year % 100, buf->Year[1]);
	buf->lb = '\n';
	buf->zt = '\0';
}

char *asctime(P(const struct tm *) ptm)
PP(const struct tm *ptm;)
{
	static timebuf_t asctime_buffer;

	/* Fill the buffer */
	FillBuf(&asctime_buffer, ptm);

	return asctime_buffer.text;
}
