/***************************************************************************
 *									    
 *			W r t c h r    F u n c t i o n			    
 *			------------------------------			    
 *									    
 *	Function "wrtchr" is called from "write" to handle character 	    
 *	oriented devices: TTY, LPT, and QUE.				    
 *									    
 *	Calling sequence:						    
 *		ret = _wrtchr(fp,buffer,bytes);				    
 *									    
 *	Where:								    
 *		fp	Points to the affected ccb.			    
 *		buffer	Is the buffer address				    
 *		bytes	Is the number of bytes to write			    
 *									    
 *		ret	Is the number of bytes actually written		    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <sgtty.h>

#define FIXED_BDOS 1

size_t _wrtchr(P(FD *) fp, P(const VOIDPTR) bufp, P(size_t) bytes)
PP(FD *fp;)									/* -> CCB           */
PP(const VOIDPTR bufp;)						/* -> User's buffer     */
PP(size_t bytes;)							/* # bytes to write     */
{
#ifdef FIXED_BDOS
#define buf bufp
#else
	register const char *buf = bufp;
#endif
	size_t nbs;							/* num bytes remaining      */
	int ii;								/* local counter        */
	char cp[SECSIZ];					/* ptr to local buffer      */
	int col;							/* column counter       */
	int nsp;							/* temp counter         */

	int (*fnout) PROTO (( const char * ));					/* func placeholder     */

	int DoAscii;						/* Flag: xlate newlines?    */

	int DoXTabs;						/* Flag: xpand tabs?        */

	struct sgttyb *tyb;					/* Special TTY ptr      */

	DoAscii = 1;						/* Default: xlate newlines  */
	DoXTabs = 0;					/* Default: no expand tabs  */
	if (fp->flags & ISTTY)				/* If TTY Output        */
		fnout = _ttyout;				/*   use that function      */
	else if (fp->flags & ISLPT)			/* If LST Output        */
		fnout = _lstout;				/*   use this function      */
#ifndef FIXED_BDOS
	else
		return 0;
#endif
#if CPM3
	else if (fp->flags & ISQUE)
	{
		/* Maybe change DoAscii here */
	}
#endif
	if (fp->flags & ISSPTTY)			/* Special Output handling? */
	{									/*   yes...         */
		tyb = (struct sgttyb *)&fp->fcb;				/* assume info stored here  */
		DoXTabs = (tyb->sg_flags) & XTABS;
	}


	col = 0;							/* Assume we're at start of */
	/*   line: not really kosher */
	for (nbs = bytes; nbs != 0;)		/* For all the bytes        */
	{
		for (ii = 1; ii < SECSIZ && nbs != 0;)	/* Not too full & chars left */
		{
			if (*buf == '\n')			/* Newline char?        */
			{							/* yes...           */
				col = -1;				/* set to zero when incr'd  */
				if (DoAscii)			/* Newline needs xlation?   */
				{						/*   yes...         */
					if (ii + 1 >= SECSIZ)
						break;			/* Buf too full, next time  */
					cp[ii] = '\r';		/* Preceed with Return char */
					++ii;				/* Incr num in buffer       */
				}
			} else /*****			    */ if (*buf == '\t' && DoXTabs)	/* Expand this tab char?    */
			{							/*   yes...         */
				nsp = 8 - (col & 7);	/* calc number spaces       */
				if (ii + nsp >= SECSIZ)
					break;				/* Buf too full, next time */
				col += nsp;
				while (nsp--)			/* for all the spaces       */
				{
					cp[ii] = ' ';
					++ii;				/*  put them in buf     */
				}
				++buf;					/* Skip over tab        */
				continue;
			}
			cp[ii] = *buf++;			/* Xfer char to buffer      */
			++ii;
			--nbs;
			++col;						/* Handle counters      */
		}
		cp[0] = ii - 1;					/* Num chars output     */
		(*fnout) (cp);					/* go do the output     */
	}
	return bytes;						/* return what they gave us */
}
