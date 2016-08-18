#include "lib.h"
#include <fcntl.h>
#include <errno.h>


/* Dissect the given mode string into an mode_t.  */
int __getmode(P(const char *) modeptr)
PP(const char *modeptr;)
{
	int mode;
	
	if (modeptr == NULL)
	{
		__set_errno(EINVAL);
		return -1;
	}

	switch (*modeptr)
	{
	case 'a':
		mode = O_CREAT | O_APPEND | O_WRONLY;
		break;
	case 'w':
		mode = O_CREAT | O_TRUNC | O_WRONLY;
		break;
	case 'r':
		mode = O_RDONLY;
		break;
	default:
		__set_errno(EINVAL);
		return -1;
	}

	while (*++modeptr)
	{
		switch (*modeptr)
		{
		case '+':
			mode = (mode & ~O_ACCMODE) | O_RDWR;
			break;
		case 'b':
			mode |= O_BINARY;
			break;
		case 't':
			mode |= O_TEXT;
			break;
		case 'x':
			mode |= O_EXCL;
			break;
		}
	}

	return mode;
}
