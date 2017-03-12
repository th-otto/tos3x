/* date.c - set system date (MM/DD/YY)
*/

#include <stdarg.h>
#include "dt.h"

VOID entry PROTO((char *bp));

VOID entry(P(char *) bp)
PP(char *bp;)
{
	int m, d, y;
	int date;
	char **q;

	bp += 0x80;

	if (*bp)
	{
		bp += 2;
		q = &bp;
		m = get_num(q);
		if ((m == -1) || (**q != '/') || (m > 12))
			goto error;
		(*q)++;
		d = get_num(q);
		if ((d == -1) || (**q != '/') || (d > 31))
			goto error;
		(*q)++;
		y = get_num(q);
		if (y == -1)
			goto error;
		if (y < 100)
			y += 1900;

		date = (((y - 1980) << 9) & 0xFE00) | ((m << 5) & 0x01E0) | (d & 0x001F);

		if (bdos(T_SetDate, date) < 0L)
		{
		  error:
			bdos(C_ConWS, "Invalid date, Enter mm/dd/yy");
			bdos(P_Term);					/* terminate and exit */ /* BUG: no retcode */
		}
	}

	date = bdos(T_GetDate);
	put_num((date >> 5) & 0xF);
	bdos(C_ConOut, '/');
	put_num(date & 0x1F);
	bdos(C_ConOut, '/');
	put_num(((date >> 9) & 0x7F) + 80);
	/* BUG: not CR/LF */
	bdos(P_Term); /* BUG: no retcode */
}
