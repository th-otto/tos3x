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

#ifdef MAXF5
#  define	MAXCCBS 5					/* Maximum Num CCBs     */
/* stubroutine for option.h */
VOID maxfiles5(NOTHING)
{;
}
#else

#ifdef MAXF10
#  define	MAXCCBS 10					/* Maximum Num CCBs     */
#else
#  define	MAXCCBS 16					/* Maximum Num CCBs     */
#endif
#endif

long _chvec = 0;						/* Allocate storage     */

FD _fds[MAXCCBS];						/* Allocate CCB storage     */



/*****************************************************************************
*		_ A L L O C C   /   _ F R E E C   R O U T I N E S
*		-------------------------------------------------
*	Routines "_allocc" and "_freec" are used to allocate / deallocate a 
*	channel number so that the user may do I/O directly without the OTS
*	getting in the way.
*	Note that this scheme preserves compatibility with OS's that really
*	truly use channels, and allows users to inform the C Run Time Library
*	routines that they are using the channel.
*	It's not important to use these routines under CP/M, since CP/M
*	allows you to have as many FCBs as you wish.
*
*	Calling Sequence:
*		i = _allocc();
*		_freec(i);
*****************************************************************************/

int _allocc(NOTHING)
{
	register int i;							/* Define 2 temporaries     */
	register long j;

	j = 1;								/* Start with channel 0     */
	for (i = 0; i < MAXCCBS; i++)		/* Look at the bits     */
	{
		if ((j & _chvec) == 0)			/* If 0, then channel free  */
		{
			_chvec |= j;				/* set allocated bit        */
			return i;					/* and return the channel # */
		}
		j <<= 1;						/* Up to next bit       */
	}									/* End FOR loop         */
	RETERR(-1, EMFILE);			/* All channels in use!     */
}

int _freec(P(int) ch)
PP(int ch;)								/* Channel number to free   */
{
	_chvec &= ~(1L << ch);				/* Clear appropriate bit    */
	return 0;
}

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
