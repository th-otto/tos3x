#include <time.h>
#include <sys/stat.h>
#include <osbind.h>
#include <ctype.h>
#include <errno.h>
#include <osif.h>
#include "lib.h"

int fstat(P(int) fd, P(struct stat *) st)
PP(int fd;)
PP(struct stat *st;)
{
	long pos;
	unsigned short buf[2];
	unsigned short mode;
	DOSTIME dtime;
	register FD *fp;	
	
	/* is fd valid? */
	if ((fp = _chkc(fd)) == NULLFD)
		return -1;
	st->st_nlink = 1;
	st->st_ino = 0;
	st->st_uid = st->st_gid = 0;
	st->st_blksize = 512;
	if (fp->flags & (ISTTY | ISLPT))
	{
		st->st_dev = st->st_rdev = fp->dosfd;
		st->st_size = 0;
		st->st_atime = 0;
		st->st_mtime = 0;
		st->st_ctime = 0;
		st->st_mode = S_IFCHR;
		return 0;
	}
	st->st_dev = st->st_rdev = Dgetdrv(); /* FIXME: can't determine drive of open file */
	mode = S_IFREG | S_IREAD | S_IWRITE;
	pos = Fseek(0L, fp->dosfd, SEEK_CUR);
	if (pos < 0)
	{
		errno = EBADF;
		return -1;
	}
	st->st_size = Fseek(0L, fp->dosfd, SEEK_END);
	if (fp->dosfd > 2)
	{
		Fseek(0L, fp->dosfd, SEEK_SET);
		if (Fread(fp->dosfd, 2L, buf) < 0)
		{
			errno = EBADF;
			return -1;
		}
		if (buf[0] == 0x601a)
			mode |= S_IEXEC;
		Fseek(0L, fp->dosfd, SEEK_SET);
		if (Fwrite(fp->dosfd, 1L, buf) == -36)
			mode &= ~S_IWRITE;
	}
	Fseek(pos, fp->dosfd, SEEK_SET);
	Fdatime(&dtime, fp->dosfd, 0);
	st->st_atime = st->st_ctime = st->st_mtime = ftimtosec(&dtime);
	st->st_mode = mode;
	st->st_blocks = (st->st_size + 511) >> 9;

	return 0;
}
