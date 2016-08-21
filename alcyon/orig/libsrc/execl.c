/***************************************************************************
 *									    
 *			    E x e c   F u n c t i o n			    
 *			    -------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	The execl function is called from anywhere to pass control to  	    
 *	another program from the the executing C program.		    
 *	Note that the stream files are closed via '_cleanup()'.		    
 *									    
 *	Calling Sequence:						    
 *									    
 *		execl(name,arg0,arg1,...,argn,NULLPTR);			    
 *									    
 *	Where:								    
 *		name, arg0, arg1...argn are pointers to character strings   
 *									    
 ****************************************************************************/
#include "lib.h"
#include <osif.h>
#include <osiferr.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>


int execl(P(const char *) name, P(const char *) arg0 _va_alist)
PP(const char *name;)
PP(const char *arg0;)									/* pointers arguments      */
_va_dcl
{
	register const char **args;					/* used to index into args */
	char cmdline[128];					/* CP/M command line area  */
	register int i;

	UNUSED(i);
	_cleanup();							/* Close all (stream) files */
	/* now build cmdline */
	strcpy(cmdline, name);				/* Copy name portion       */
#if PCDOS == 0							/* Don't do for PCDOS      */
	for (i = strlen(cmdline); --i >= 0;)	/* uppercase command name  */
		cmdline[i] = toupper(cmdline[i]);
#endif
	args = &arg0;						/* Copy args           */
	args++;								/* arg0 is a dup of the    */
	/* command name */
	while (*args != NULL)
	{
		strcat(cmdline, " ");			/* Add a blank         */
		strcat(cmdline, *args++);		/* Add next arg        */
	}
#if PCDOS								/* handle differently ifso */
	_pc_chain(cmdline);					/* special home grown func */
#else /* O.S. handles it?    */
	__OSIF(SETDMA, cmdline);			/* DMA -> Command line     */
	__OSIF(P_CHAIN, 0);					/* chain to program        */
#endif
	RETERR(-1, ENOENT);					/* error: file not found   */
}
