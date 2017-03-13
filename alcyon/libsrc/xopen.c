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
#include <stdlib.h>
#include <ctype.h>
#include <portab.h>
#include <fcntl.h>

#define HIBIT 0x80

char *_dosify(P(char *) s)
PP(register char *s;)
{
	register char *ret;
	
	ret = s;
	while (*s)
	{
		if (*s == '/')
			*s = '\\';
		s++;
	}
	return ret;
}


int __open(P(int) ch, P(const char *) filename, P(int) bdosfunc)
PP(int ch;)								/* Channel number       */
PP(register const char *filename;)						/* -> filename          */
PP(int bdosfunc;)							/* BDOS Function        */
{
	register FD *fp;						/* -> ccb area          */
	register int rv;						/* Return value         */
	
	fp = _getccb(ch);					/* Fetch ccb pointer        */

	{
		register short mode;
		register long dosfd;
		char tmpbuf[PATH_MAX];
		
		filename = _dosify(strncpy(tmpbuf, filename, sizeof(tmpbuf)));
		
		rv = -1;
		switch (bdosfunc)
		{
		case CREATE:
			if ((rv = Fcreate(filename, 0)) > 0)
			{
				fp->dosfd = rv;
				rv = 0;
			}
			break;
		
		/* strange interface for SEARCHF/SEARCHN; but isn't used anymore */
		case SEARCHF:
			rv = Fsfirst(filename, 0) != 0 ? 0 : 255;
			break;
		case SEARCHN:
			rv = Fsnext() != 0 ? 0 : 255;
			break;
		
		case OPEN:
			/* GEMDOS uses the same values for the mode as the open() call */
			switch (fp->flags & (ISREAD|ISWRITE))
			{
			case 0:
			case ISREAD:
				mode = O_RDONLY;
				break;
			case ISWRITE:
				mode = O_WRONLY;
				break;
			default:
				mode = O_RDWR;
				break;
			}
			/*
			 * Remember that we can get valid negative handle from e.g. Fopen("PRN:"),
			 * hence the >= -6
			 */
			if ((dosfd = Fopen(filename, mode)) >= -6)
			{
				fp->dosfd = dosfd;
				rv = 0;
			}
			break;
		case DELETE:
			rv = Fdelete(filename);
			break;
		}
	}
	
	return rv;
}
