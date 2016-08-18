/*****************************************************************************
*
*	  C H A N N E L   A L L O C A T I N G   R O U T I N E S
*	  -----------------------------------------------------
*	Copyright 1984 by Digital Research Inc.  All rights reserved.
*
*	Herein are all the routines which deal with Channel Control Block
*	(CCB) allocation and initialization.  These routines (documented
*	individually below) are:
*
*		i = _allocc();
*		_freec(i);
*		_chinit();
*		__chinit(fd)
*		ccbptr = _chkc(ch);
*
*****************************************************************************/
#include <osif.h>					/* Also CP/M ones       */
#include "lib.h"					/* Include std definitions  */
#include <osiferr.h>				/* To set error vars        */
#include <errno.h>					/* Error return vals        */

/*****************************************************************************
*	      C H A N N E L   N U M B E R   V A L I D A T I O N
*	      -------------------------------------------------
*	This routine is used to validate a channel number and return the
*	pointer to the ccb area.  The channel must be in range and open.
*
*	Calling Sequence:
*
*	Where:
*		ch	Is the channel number
*		ccbptr	Is the returned ccb address, NULLFD if error
*****************************************************************************/

/* Facilitate error check */
FD *_chkc(P(unsigned int) ch)
PP(register unsigned int ch;)
{
	register FD *xcb;					/* -> CCB           */

	/* Is channel in range?  */
	if (ch >= MAXCCBS)
		RETERR(NULLFD, EBADF);
	
	xcb = _getccb(ch);
	/* Is channel OPEN? */
	if ((xcb->flags & OPENED) == 0)
		RETERR(NULLFD, EBADF);
	return xcb;
}
