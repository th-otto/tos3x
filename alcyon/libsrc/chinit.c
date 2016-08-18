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


FD _fds[MAXCCBS];						/* Allocate CCB storage     */

/*****************************************************************************
*		     C C B   I N I T I A L I Z A T I O N
*		     -----------------------------------
*	Routine "_chinit" is called from the run-time initialization to clear
*	out all the CCB storage.
*	Calling sequence:
*		_chinit();
*
*	Routine "__chinit(fd)" is called from other low-level routines
*	for single channel initialization.
*	Calling sequence:
*		__chinit(fd)
*	where:	fd = file descriptor #
*****************************************************************************/

VOID _chinit(NOTHING)
{
	register int i;

	for (i = 0; i < MAXCCBS; i++)
		__chinit(i);
}

/*****************************************************************************
*	The __chinit routine initializes only 1 channel.
*****************************************************************************/

VOID __chinit(P(int) i)
PP(int i;)
{
	register FD *ch;					/* -> CCB           */

	ch = _getccb(i);					/* convert fd to CCB        */
	ch->chan = i;						/* Load channel byte        */
	ch->flags = 0;						/* clear flag word      */
	ch->user = 0;						/* assume user 0        */
	ch->sector = -1;					/* Set no sector in buff    */
	ch->offset = 0;						/* Clear file offset word   */
	ch->hiwater = 0;					/* Init hiwater mark        */
	ch->fcb.drive = 0;					/* Init drive field of fcb  */
	/* Init rest of fcb to zeros */
	blkfill(&ch->fcb, 0, sizeof(ch->fcb));
	/* Init file name fields to spaces */
	blkfill(ch->fcb.fname, ' ', 8 + 3);
}


