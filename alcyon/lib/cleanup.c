/**********************************************************************
*
*			_ c l e a n u p   F u n c t i o n
*			---------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"_cleanup" closes all buffered files
*
*	Calling sequence:
*		_cleanup()
*
***************************************************************************/

#include "lib.h"

VOID _cleanup(NOTHING)
{
	register int ii;

	for (ii = 0; ii < MAXFILES; ii++)
		fclose(&_iob[ii]);
}
