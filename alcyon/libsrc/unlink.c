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
#include <string.h>
#include <errno.h>


int unlink(P(const char *) filename)
PP(const char *filename;)
{
	int err;
	char tmpbuf[PATH_MAX];
	
	filename = _dosify(strncpy(tmpbuf, filename, sizeof(tmpbuf)));
		
	err = Fdelete(filename);
	if (err >= 0)
		return 0;
	__set_errno(_XltErr(err, ENOENT));
	return -1;
}
