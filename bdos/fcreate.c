#include "tos.h"
#include <ostruct.h>
#include <toserrno.h>
#include "fs.h"

/*
**  xcreat -
**  create file with specified name, attributes
**
**	Function 0x3C	f_create
**
**	Error returns
**		EPTHNF
**		EACCDN
**		ENHNDL
**
**	Last modified	SCC	13 May 85
*/

/* 306: 00e1728c */
ERROR xcreat(P(const char *) fname, P(int8_t) attr)
PP(const char *fname;)
PP(int8_t attr;)
{
	if ((attr & FA_DIREC) || ((attr & FA_LABEL) && (attr != FA_LABEL)))
		return E_BADRQ;
	return ixcreat(fname, attr & (FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_LABEL|FA_ARCH));
}


/*	
**  xopen - open a file (path name)
**
**  returns
**	<0 = error
**	>0 = file handle
**
**	Function 0x3D	f_open
**
**	Error returns
**		EFILNF
**		opnfil()
**
**	Last modified	SCC	5 Apr 85
*/

/* 306: 00e172c8 */
ERROR xopen(P(const char *) fname, P(int16_t) mode)
{
	return ixopen(fname, mode);
}
