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

long _chvec = 0;						/* Allocate storage     */

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
	_chvec &= ~(1 << ch);				/* Clear appropriate bit    */
	return 0;
}

