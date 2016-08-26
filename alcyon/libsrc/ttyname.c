/*************************************************************************
*
*			Channel Info Functions
*			----------------------
*	Copyright 1982,83 by Digital Reserach Inc.  All rights reserved.
*
*	"isatty(fd)" == TRUE iff the file on channel fd is a 'CON:' device.
*	"isdev(fd)" == TRUE iff fd is 'CON:', 'LST:', 'AUX:'
*
*	Where:
*		fd  = file descriptor returned by 'open'
*
**************************************************************************/

/* ttyname - find name of a terminal */
/*	returns "CON:" if isatty, NULL o.w., under CP/M */

#include "lib.h"
#include <unistd.h>

static char tname[] = "CON:";

char *ttyname(P(int) fd)
PP(int fd;)
{
	if (isatty(fd))
		return tname;
	return NULL;
}
