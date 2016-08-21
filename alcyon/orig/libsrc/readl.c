#include "lib.h"

long readl(P(int) fd, P(char *) buf, P(long) lnum)
PP(int fd;)
PP(register char *buf;)
PP(long lnum;)
{
	long R;
	register size_t tmp;

	R = lnum;
	while (lnum != 0)
	{
		tmp = (lnum < 32768L) ? (int) lnum : 32768;
		if (read(fd, buf, tmp) != tmp)
			break;
		buf += tmp;
		lnum -= tmp;
	}
	return R - lnum;					/* number of bytes read */
}
