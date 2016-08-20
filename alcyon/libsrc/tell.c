#include "lib.h"

off_t tell(P(int) fd)
PP(int fd;)
{
	return lseek(fd, 0L, SEEK_CUR);
}
