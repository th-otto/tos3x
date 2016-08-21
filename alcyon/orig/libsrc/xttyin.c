/****************************************************************************/
/*									    */
/*			  _ t t y i n   F u n c t i o n			    */
/*			  -----------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
/*									    */
/*	Function "_ttyin" is used to read a line from the terminal.  	    */
/*	It looks for the ISSPTTY flag set in the ccb: if set, it assumes    */
/*	the fcb really contains sgttyb information (see <sgtty.h>), and	    */
/*	behaves accordingly.						    */
/*	Otherwise, it grabs a line at a time from the BDOS.		    */
/*									    */
/*	Calling Sequence:						    */
/*		ret = _ttyin(fp,buffer,bytes);				    */
/*	Where:								    */
/*		fp	-> ccb pointer & channel info			    */
/*		buffer	-> the user's input buffer			    */
/*		bytes	=  the (maximum) number of bytes to read	    */
/*		ret 	=  the number of bytes actually read		    */
/*									    */
/****************************************************************************/
#include <osif.h>
#include "lib.h"
#include <sgtty.h>
#include <stdlib.h>

#define Conin 0xFFL
#define Constat 0xFEL

size_t _ttyin(fp, xbuff, bytes)
register FD *fp;								/* -> filedes           */
VOIDPTR xbuff;									/* -> user's buffer     */
register size_t bytes;							/* =  requested byte count  */
{
	register char *buff = xbuff;
	static char ttybuf[257];				/* Biggest line from tty    */

	register long xbytes,					/* Returned byte count      */
	 nbs;								/* Number to read       */

	register char *p;						/* Temp pointer         */

	static struct sgttyb *tyb;			/* TTY info ptr (at fp->fcb) */

	if (fp->flags & ISSPTTY)			/* is this a special tty?   */
	{
		tyb = (struct sgttyb *)&(fp->fcb);				/* assume info stored here  */
		if (tyb->sg_flags & RAW)		/* are we in raw mode?      */
		{
			*buff = __BDOS(CONIO, Conin);	/* grab a char          */
			return 1;					/* return number bytes read */
		}
		if (tyb->sg_flags & CBREAK)		/* are we half baked?       */
		{
			p = buff;					/* use ptr          */
			*p = __BDOS(CONIO, Conin);	/* get char         */
			if (*p == tyb->sg_kill)		/* kill char typed?     */
				exit(1);				/*  yes: DIE, PROGRAM!      */
			if (tyb->sg_flags & LCASE)	/* they want lower case?    */
				*p = (*p >= 'A' && *p <= 'Z'
					  ? *p - 'A' + 'a' : *p);	/*  give them lower case    */
			if ((tyb->sg_flags & CRMOD)	/* xlate returns?       */
				&& *p == '\r')
			{
				if (tyb->sg_flags & ECHO)
					__BDOS(CONIO, (long)(*p));	/* out before xlate     */
				*p = '\n';				/*  yes, do xlation     */
			}
			if (tyb->sg_flags & ECHO)	/* echo chars?          */
				__BDOS(CONIO, (long)(*p));
			return 1;					/* return number read       */
		}
		/* nothing really special... */
	}
	nbs = bytes < 255 ? bytes : 255;	/* don't read more than they */
	ttybuf[0] = nbs;					/* asked for/we have room fr */
	__BDOS(CONBUF, (long)&ttybuf[0]);			/* Read line from BDOS      */
	
	xbytes = ttybuf[1] & 0xffL;			/* # characters read        */
	if (xbytes < nbs)					/* # read < #asked for,assum */
		__BDOS(CONOUT, (long) '\n');	/*   CR/LF line terminated  */
	p = &ttybuf[2];						/* p -> First character     */
	while (bytes > 0 && xbytes > 0)
	{									/* Copy 1 byte / time       */
		if (*p == EOFCHAR)				/* ^Z typed?            */
		{								/* Yes,             */
			fp->flags |= ATEOF;			/*  Mark with EOF       */
			break;						/*  Exit now        */
		}
		*buff++ = *p++;
		bytes--;						/* Decrement request        */
		xbytes--;						/* Decrement bytes in buff  */
	}
	if (bytes > 0)						/* Terminate on count?      */
	{
		*buff++ = '\n';					/* No, plug in newline      */
		p++;							/* Bump pointer         */
	}
	return ((int) (p - &ttybuf[2]));	/* Return # bytes moved     */
}
