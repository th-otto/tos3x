/***************************************************************************
 *									    
 *			    E x i t   F u n c t i o n			    
 *			    -------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	The exit function is called from anywhere to pass control back to   
 *	the CCP from the executing C program.				    
 *	Note that the stream files are closed via '_cleanup()'.		    
 *									    
 *	Calling Sequence:						    
 *									    
 *		exit(code);						    
 *									    
 *	Where:								    
 *		code	Is the exit status (ignored)			    
 *									    
 ****************************************************************************/

#include "lib.h"
#include <stdlib.h>

VOID exit(P(int) code)
PP(int code;)
{
	/* Close all (stream) files */
	_cleanup();
	/* return to O.S. */
	_exit(code);
}
