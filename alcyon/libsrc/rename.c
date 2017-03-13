/***************************************************************************
 *									    
 *			r e n a m e   F u n c t i o n			    
 *			-----------------------------			    
 *	Copyright 1983 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	'rename()' changes a file's name.				    
 *									    
 *	Calling sequence:						    
 *		ret = rename(from,to)					    
 *									    
 *	Where:								    
 *		from -> a EOS terminated (existing) file name		    
 *		to -> the EOS terminated new file name			    
 *		ret = SUCCESS if everything ok, FAILURE o.w.		    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <osbind.h>
#include <errno.h>
#include <string.h>


int rename(P(const char *) from, P(const char *) to)
PP(const char *from;)								/* Existing file name       */
PP(const char *to;)								/* New file name        */
{
	int err;
	char tmpbuf[PATH_MAX];
	char tmpbuf2[PATH_MAX];
	
	from = _dosify(strncpy(tmpbuf, from, sizeof(tmpbuf)));
	to = _dosify(strncpy(tmpbuf2, to, sizeof(tmpbuf2)));
	
	if ((err = Frename(from, to)) < 0)
	{
		__set_errno(_XltErr(err, ENOENT));
		return -1;
	}
	return 0;						/* It worked!           */
}
