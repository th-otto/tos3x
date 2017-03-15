/* time.c - set system time (HH:MM:SS)
*/

#include <stdarg.h>
#include <osbind.h>
#include "dt.h"

VOID entry PROTO((BASEPAGE *bp));

VOID entry(P(BASEPAGE *) bp)
PP(BASEPAGE *bp;)
{
	int h, m, s;
	int time;
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
		h = get_num(q);
		if (h < 0 || *p != ':' || h > 24)
			goto error;
		p++;
		m = get_num(q);
		if (m < 0 || *p != ':' || m > 59)
			goto error;
		p++;
		s = get_num(q);
		if (s < 0 || s > 59)
			goto error;
		s = (s + 1) >> 1;

		time = ((h << 11) & 0xF800) | ((m << 5) & 0x07E0) | (s & 0x001F);

		if (Tsettime(time) < 0L)
		{
		  error:
			Cconws("Invalid time, Enter HH:MM:SS\r\n");
			Pterm(1);
		}
	}

	time = Tgettime();
	put_num((time >> 11) & 0x1F);
	Cconout(':');
	put_num((time >> 5) & 0x3F);
	Cconout(':');
	put_num((time & 0x1F) << 1);
	Cconws("\r\n");
	Pterm(0);
}
