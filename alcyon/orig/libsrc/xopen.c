/***************************************************************************
 *									    
 *			_ _ o p e n   F u n c t i o n			    
 *			-----------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	Function "__open" is used to parse the CP/M fcb and open or create  
 *	the requested file.  Created files are deleted first, to avoid 	    
 *	directory problems.						    
 *									    
 *	Calling Sequence:						    
 *									    
 *		ret = __open(ch,filnam,bdosfunc);			    
 *									    
 *	Where:								    
 *		ch	Is a vacant channel number			    
 *		filnam	Is a null-terminated CP/M filename		    
 *		bdosfunc Is the desired BDOS function to perform	    
 *		ret	Is SUCCESS if everything ok, FAILURE o.w. unless:   
 *			  bdosfunc=SEARCHF/N, ret = dcnt (0-3)		    
 *									    
 *	This routine may also be used to delete files as well.		    
 *									    
 *	Modified  4-Nov-83 (whf) open files read-only if appropriate	    
 *	Modified  8-Oct-83 (whf) handle user #s differently		    
 *	Modified  5-Oct-83 (whf) added '_parsefn()'			    
 *	Modified 11-Aug-83 (whf) for DRC				    
 *	Modified 26-Jun-83 (sw) for wild cards and user # support	    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <portab.h>

#define HIBIT 0x80

int __open(P(int) ch, P(const char *) filename, P(int) bdosfunc)
PP(int ch;)								/* Channel number       */
PP(register const char *filename;)						/* -> filename          */
PP(int bdosfunc;)							/* BDOS Function        */
{
	FD *fp;								/* -> ccb area          */
	register struct fcbtab *fcbp;			/* -> FCB area in ccb       */
	register int rv;						/* Return value         */
#if CPM
	register int xuser;						/* User number          */
#endif
	register const char *p;
	
	fp = _getccb(ch);					/* Fetch ccb pointer        */

#if GEMDOS
	{
		char drive;
		
		rv = -1;
		ucase(NO_CONST(filename));			/* WTF, that modifies users argument, and is completely unneccessary */
		
		for (p = filename; *p != '\0' && *p != ':'; p++)
			;
		if (*p != '\0')
		{
			drive = *--p;
			if (drive >= 'A' && drive <= 'A' + 16 - 2)
				filename = p;
			else
				filename = p + 2;
		}
		switch (bdosfunc)
		{
		case CREATE:
			if ((rv = jcreat(filename, 0)) > 0)
			{
				fp->dosfd = rv;
				rv = 0;
			}
			break;
		
		/* strange interface for SEARCHF/SEARCHN; but isn't used anymore */
		case SEARCHF:
			rv = jsfirst(filename, 0) != 0 ? 0 : 255;
			break;
		case SEARCHN:
			rv = jsnext() != 0 ? 0 : 255;
			break;
		
		case OPEN:
			/* BUG: mode not passed to Fopen */
			/* BUG: Fopen can return valid negative file handles */
			if ((rv = trap(0x3d, filename)) > 0)
			{
				fp->dosfd = rv;
				rv = 0;
			}
			break;
		case DELETE:
			rv = junlink(filename);
			break;
			/* 2nd break leftover? */
			/* break; */
		}
	}
	
	UNUSED(fcbp);
	return rv;
	
#else

	fcbp = &(fp->fcb);					/* Fetch fcb pointer        */
	rv = 0;						/* Default to success       */
	
	if (_parsefn(filename, fcbp) != 0)	/* Parse filename into fcb    */
		RETERR(-1, EINVAL);		/* Quit if name not ok      */

#if CPM									/* Handling user numbers?   */
	if (fcbp->fuser)					/* User # specified?        */
		fp->user = fcbp->fuser;			/* put it where we use it   */
#endif
	if (bdosfunc == CREATE &&			/* Creating file?       */
		strchr(fcbp->fname, '?'))		/* Wild cards @!@#$!!!      */
		RETERR(-1, EINVAL);		/* Just quit            */
	
#if CPM
	xuser = _chkuser(fp->user);			/* Change user # if needed  */
#endif
	
	if (bdosfunc == CREATE)				/* Creating file?       */
		__OSIF(DELETE, fcbp);			/*  delete it first     */
	
#if CPM									/* running some brand of CPM */
	if (bdosfunc == OPEN)				/* Opening a file?      */
	{									/* Yes...           */
		if (fp->flags & ISREAD)			/* Open file Read-Only?     */
			(fcbp->fname)[5] |= HIBIT;	/* Turn on F6' attribute bit */
	}									/*              */
#endif
	
	rv = __OSIF(bdosfunc, fcbp);		/* Do requested operation   */
	
#if CPM
	_uchkuser(fp->user, xuser);			/* Change back if needed    */
#endif
	
	if (bdosfunc == SEARCHF || bdosfunc == SEARCHN)	/*           */
		return rv;					/* return directory count   */
	return (rv <= 3) ? 0 : -1;	/* Binary return code       */
#endif
}


VOID ucase(P(char *) str)
PP(char *str;)
{
	do {
		if (*str >= 'a' && *str <= 'z')
			*str -= 'a' - 'A';
	} while (*str++ != 0);
}
