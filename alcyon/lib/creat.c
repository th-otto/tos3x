/****************************************************************************
*
*		   C   F I L E   C R E A T E   R O U T I N E
*		   -----------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	The "creat" routine opens a new "C" file and returns a file id. 
*	Comes in 3 flavors: ascii (CP/M text files), binary, and default
*	(currently ascii).
*
*	Calling Sequence:
*		fid = creat(fname,prot)
*		fid = creata(fname,prot)
*		fid = creatb(fname,prot)
*		fid = _creat(fname,prot,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		prot	is the UNIX file protection
*		type	is 0 for ASCII, 1 for BINARY
*
*****************************************************************************/
#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>



int _creat(P(const char *) fname, P(mode_t) prot, P(int) binary)
PP(const char *fname;)				/* -> File name         */
PP(mode_t prot;)						/* Open mode            */
PP(int binary;)						/* ASCII/BINARY flag        */
{
	register int ich;						/* Channel number for open  */
	register FD *ch;						/* -> CCB for channel       */

	/* Allocate a channel */
	if ((ich = _allocc()) == -1)
		return -1;
	
	__chinit(ich);						/* Clear out channel's ccb  */
	ch = _getccb(ich);					/* Get address of ccb       */
	
	if (binary == 0)						/* ASCII file?          */
		ch->flags |= ISASCII;			/*  Yes, mark it.       */
	
	if (strcmp(fname, __tname) == 0)	/* Terminal file?       */
	{
		ch->flags |= ISTTY | OPENED;	/* Set flag         */
		return ich;						/* Return file descriptor   */
	} else if (strcmp(fname, __lname) == 0)	/* List device?         */
	{
		ch->flags |= ISLPT | OPENED;	/* set flag         */
		return ich;
	}
	

	if (__open(ich, fname, CREATE) != 0)	/* Use BDOS interface       */
		RETERR(-1, ENODSPC);		/*   Oops, No dir space.    */

	ch->flags |= OPENED;				/*   Set OPEN bit       */
	return ich;
}

/* default to ascii */
int creat(P(const char *) fname, P(mode_t) prot)
PP(const char *fname;)
PP(mode_t prot;)
{
	return _creat(fname, prot, 0);
}

/* ascii file open */
int creata(P(const char *) fname, P(mode_t) prot)
PP(const char *fname;)
PP(mode_t prot;)
{
	return _creat(fname, prot, 0);
}

/* binary file open */
int creatb(P(const char *) fname, P(mode_t) prot)
PP(const char *fname;)
PP(mode_t prot;)
{
	return _creat(fname, prot, 1);
}
