/***************************************************************************
 *									    
 *			  _ t t y i n   F u n c t i o n			    
 *			  -----------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	Function "_ttyin" is used to read a line from the terminal.  	    
 *	It looks for the ISSPTTY flag set in the ccb: if set, it assumes    
 *	the fcb really contains sgttyb information (see <sgtty.h>), and	    
 *	behaves accordingly.						    
 *	Otherwise, it grabs up to a line at a time from the BDOS.	    
 *									    
 *	Calling Sequence:						    
 *		ret = _ttyin(fp,buffer,bytes);				    
 *	Where:								    
 *		fp	-> ccb pointer & channel info			    
 *		buffer	-> the user's input buffer			    
 *		bytes	=  the (maximum) number of bytes to read	    
 *		ret 	=  the number of bytes actually read		    
 *									    
 *	Modified:
 *		2/84 whf: conditionally compile RAWIO code  
 *		10/83 whf: eliminate static data (ttybuf), use _ttyinraw()   
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <sgtty.h>

#define Bytesized 0xFF

size_t _ttyin(P(FD *) fp, P(VOIDPTR) pbuff, P(size_t) bytes)
PP(FD *fp;)
PP(VOIDPTR pbuff;)						/* -> user's buffer     */
PP(size_t bytes;)							/* =  requested byte count  */
{
	register char *p;						/* Temp pointer         */
	char ttybuf[Bytesized + 2];			/* Biggest line from tty    */
	register char *buff = pbuff;

	size_t xbytes;						/* Returned byte count      */
	size_t nbs;								/* Number to read       */

	if (fp->flags & ISSPTTY)			/* is this a special tty?   */
	{
#ifdef RAWIO
		struct sgttyb *tyb;			/* TTY info ptr (at fp->fcb) */

		tyb = &(fp->fcb);				/* assume info stored here  */
		if (tyb->sg_flags & RAW)		/* are we in raw mode?      */
		{								/*              */
			*buff = _ttyinraw();		/* grab a char          */
			return 1;					/* return number bytes read */
		}								/*   *********************** */
		if (tyb->sg_flags & CBREAK)		/* are we half baked?       */
		{								/*              */
			p = buff;					/* use ptr          */
			*p = _ttyinraw();			/* get char         */
			if (*p == tyb->sg_kill)		/* kill char typed?     */
				exit(1);				/*  yes: DIE, PROGRAM!      */
			if (tyb->sg_flags & LCASE)	/* they want lower case?    */
				*p = (*p >= 'A' && *p <= 'Z'	/*              */
					  ? *p + ('a' - 'A') : *p);	/*  give them lower case    */
			if ((tyb->sg_flags & CRMOD)	/* xlate returns?       */
				&& *p == '\r')			/*              */
			{							/*              */
				if (tyb->sg_flags & ECHO)	/*              */
					__OSIF(CONIO, *p);	/* out before xlate     */
				*p = '\n';				/*  yes, do xlation     */
			}							/*              */
			if (tyb->sg_flags & ECHO)	/* echo chars?          */
				__OSIF(CONIO, *p);		/*              */
			return (1);					/* return number read       */
		}
#else
		_optoff("Raw I/O");
#endif
	}
	nbs = bytes < Bytesized ? bytes : Bytesized;	/* don't read more than */
	ttybuf[0] = nbs;					/* asked for/we have room fr */
	if (nbs == 1)						/* then use conin not rdln  */
	{
		ttybuf[2] = __OSIF(CONIN, 0);
		ttybuf[1] = 1;
		if (ttybuf[2] == '\r')
			ttybuf[1] = 0;
	} else
	{
		__OSIF(CONBUF, ttybuf);			/* Read line from BDOS      */
	}
	
	xbytes = ttybuf[1] & 0xff;			/* # characters read        */
	if (xbytes < nbs)					/* # read < #asked for,assum */
		__OSIF(CONOUT, '\n');			/*   CR/LF line terminated  */
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
		p++;
	}
	return ((int) ((long)p - (long)&ttybuf[2]));	/* Return # bytes moved     */
}
