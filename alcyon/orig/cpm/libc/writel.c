#include "lib.h"

long writel(P(int) fd, P(const char *) buf, P(long) lnum)
PP(int fd;)
PP(register const char *buf;)
PP(long lnum;)
{
	long R;
	register size_t tmp;

	R = lnum;
	while (lnum != 0)
	{
		tmp = (lnum < 32768L) ? (int) lnum : 32768;
		if (write(fd, buf, tmp) != tmp)
			break;
		buf += tmp;
		lnum -= tmp;
	}
	return R - lnum;					/* number of bytes written */
}
