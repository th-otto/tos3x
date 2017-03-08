#include <time.h>
#include "lib.h"
#include <string.h>
#include <stdlib.h>

long timezone = -3600;
int daylight = 0;

/* buffers must hold 64 characters! */
static char tz_name[64] = "PST";
static char tz_dst_name[64] = "PDT";

char *tzname[2] = {
  tz_name,
  tz_dst_name,
};

static long dst_begin = 0;

static char _tz_is_set = 0;

VOID tzset(NOTHING)
{
    const char *str;
    long hour;
    long minute;
    long sec;
    size_t len;
    int sign;

    if (_tz_is_set)
    {
        return;
    }

    /* Try to read the timezone from environment */
    str = getenv("TZ");
    if (str && str[0] != 0)
    {
        len = strlen(str);
        dst_begin = 0;
		hour = 0;
		minute = 0;
		sec = 0;
		sign = 1;
        for (;;)
        {
            /* Copy timezone name */
            strncpy(tz_name, str, 3);
            str += 3;
            len -= 3;

            if (len < 1)
            	break;

            if (*str == '+' || *str == '-')
            {
                sign = *str == '-' ? -1 : 1;
                str++;
                len--;
            }

            if (len < 1)
            	break;

            hour = atol(str);

            while (*str != 0 && *str != ':')
            	str++;
            if (*str == 0)
            	break;

            minute = atol(++str);

            while (*str != 0 && *str != ':')
            	str++;
            if (*str == 0)
            	break;

            sec = atol(++str);

            while (*str != 0 && *str <= '9')
            	str++;
            if (*str == 0)
            	break;

            /* Copy DST name */
            strncpy(tz_dst_name, str, 3);

            /* We are finished */
            break;
        }

        timezone = sign * (((hour * 60) + minute) * 60 + sec);
    }
    _tz_is_set = 1;
}
