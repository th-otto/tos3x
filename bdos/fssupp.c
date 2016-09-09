#include "tos.h"
#include <ostruct.h>
#include <toserrno.h>
#include <fcntl.h>
#include "fs.h"

/* 306: 00e17662 */
int contains_dots(P(const char *) name, P(char) ill)
PP(register const char *name;)
PP(char ill;)
{
	register int i;
	
	if (*name == '\0')
		return 1;
	for (i = 0; --i > -3; )
	{
		if (*name++ != '.')
			break;
		if (*name == ill)
			return i;
	}
	return 0;
}
