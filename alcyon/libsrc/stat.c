#include <time.h>
#include <sys/stat.h>
#include <osbind.h>
#include <ctype.h>
#include <errno.h>
#include "lib.h"


int stat(P(const char *) filename, P(struct stat *) st)
PP(register const char *filename;)
PP(register struct stat;)
{
	unsigned short buf[2];
	unsigned short mode;
	_DTA *olddta;
	_DTA dta;
	int drv;
	int handle;
	int res;
	char tmpbuf[128];
	
	if (strchr(filename, '/') != 0)
	{
		size_t len = strlen(filename) + 1;
		if (len < sizeof(tmpbuf))
		{
			strcpy(tmpbuf, filename);
			filename = _dosify(tmpbuf);
		}
	}
		
	olddta = (_DTA *)Fgetdta();
	Fsetdta(&dta);
	res = Fsfirst(filename, FA_RDONLY | FA_ARCH | FA_DIREC | FA_HIDDEN | FA_SYSTEM);
	Fsetdta(olddta);
	if (res != 0)
	{
		__set_errno(ENOENT);
		return -1;
	}
	if (filename[0] == '\0' || filename[1] != ':')
	{
		drv = Dgetdrv();
	} else
	{
		drv = toupper(filename[0]) - 'A';
	}
	st->st_dev = st->st_rdev = drv;
	st->st_ino = 0;
	st->st_blksize = 512;
	mode = S_IREAD;
	if (dta.d_attrib & FA_DIREC)
	{
		mode = S_IFDIR | S_IREAD | S_IWRITE | S_IEXEC;
	} else
	{
		if (dta.d_attrib & FA_SYMLINK)
			mode |= S_IFLNK;
		else
			mode |= S_IFREG;
		if (!(dta.d_attrib & FA_RDONLY))
			mode |= S_IWRITE;
		handle = (int)Fopen(filename, 0);
		if (handle >= 0)
		{
			if (handle > 2 && Fread(handle, 2L, buf) >= 2 && buf[0] == 0x601a)
				mode |= S_IEXEC;
			Fclose(handle);
		}
	}
	st->st_mode = mode;
	st->st_nlink = 1;
	st->st_uid = st->st_gid = 0;
	st->st_size = dta.d_length;
	st->st_atime = st->st_ctime = st->st_mtime = ftimtosec((struct ftime *)&dta.d_time);
	st->st_blocks = (st->st_size + 511) >> 9;
	
	return 0;
}
