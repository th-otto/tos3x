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

#include <osif.h>
#include "lib.h"
#include <unistd.h>


/* ttyname - find name of a terminal */
/*	returns "CON:" if isatty, NULLPTR o.w., under CP/M */

char *ttyname(P(int) fd)
PP(int fd;)
{
	if (isatty(fd))
		return __tname;
	return NULL;
}