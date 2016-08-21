/***************************************************************************
 *									    
 *			U n l i n k   F u n c t i o n			    
 *			-----------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	The unlink function is used to delete a CP/M file by name.	    
 *									    
 *	Calling Sequence:						    
 *									    
 *		ret = unlink(filename);					    
 *									    
 *	Where:								    
 *		filename	Is the null-terminated name of the file	    
 *									    
 *		ret		Is 0 for success, -1 for failure	    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <unistd.h>

int unlink(P(const char *) filename)
PP(const char *filename;)
{
#if GEMDOS
	return Fdelete(filename) >= 0 ? 0 : -1;
#else
	register int ch;						/* Channel number       */
	register int ret;						/* Temp return value        */

	if ((ch = _allocc()) < 0)			/* Allocate a channel       */
		return -1;				/* Can't            */

	__chinit(ch);						/* Init fcb and ccb     */
	ret = __open(ch, filename, DELETE);	/* Delete the file      */
	_freec(ch);							/* Free the channel     */
	return ret;						/* Return result of DELETE  */
#endif
}
