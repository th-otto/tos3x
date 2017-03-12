/* date.c - set system date (MM/DD/YY)
*/

#include <stdarg.h>
#include <osbind.h>
#include "dt.h"

VOID entry PROTO((BASEPAGE *bp));

VOID entry(P(BASEPAGE *) bp)
PP(BASEPAGE *bp;)
{
	int m, d, y;
	int date;
	char **q;
	char *p;
	register char *sp;
	
	sp = bp->p_cmdlin;
	if (*sp == 0x7f)
	{
		*sp = 0;
		if ((sp = bp->p_env) != NULL)
		{
			while (*sp != '\0')
			{
				if (sp[0] == 'A' && sp[1] == 'R' && sp[2] == 'G' && sp[3] == 'V' && sp[4] == '=')
				{
					sp += 5;
					/* skip ARGV= value */
					while (*sp++ != '\0')
						;
					/* skip argv[0] */
					while (*sp++ != '\0')
						;
					p = sp;
					break;
				}
				while (*sp++ != '\0')
					;
			}
		}
	} else if (*sp)
	{
		sp[*sp + 1] = '\0';
		sp++;
		p = sp;
	}
	if (*p)
	{
		q = &p;
		m = get_num(q);
		if (m < 0 || *p != '/' || m > 12)
			goto error;
		p++;
		d = get_num(q);
		if (d < 0 || **q != '/' || d > 31)
			goto error;
		(*q)++;
		y = get_num(q);
		if (y < 0)
			goto error;
		if (y < 100)
			y += 1900;

		date = (((y - 1980) << 9) & 0xFE00) | ((m << 5) & 0x01E0) | (d & 0x001F);

		if (Tsetdate(date) < 0L)
		{
		  error:
			Cconws("Invalid date, Enter mm/dd/yy\r\n");
			Pterm(1);					/* terminate and exit */
		}
	}

	date = Tgetdate();
	put_num((date >> 5) & 0xF);
	Cconout('/');
	put_num(date & 0x1F);
	Cconout('/');
	put_num(((date >> 9) & 0x7F) + 80);
	Cconws("\r\n");
	Pterm0();
}
