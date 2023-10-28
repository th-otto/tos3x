/*****************************************************************************
*
*		R U N T I M E   S T A R T O F F   R O U T I N E
*		-----------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Routine "_main" is entered from the C header routine to start a C 
*	program.  The command string from CP/M is parsed into
*	a UNIX-like "argc/argv" setup, including simple I/O redirection.
*
*	Calling Sequence:
*
*		return = _main(command,length);
*
*	Where:
*
*		command		Is the address of the command line from CP/M
*		length		Is the number of characters in the line, 
*				excluding the termination character (CR/LF).
*
*****************************************************************************/

#include "../libsrc/lib.h"
#include <ctype.h>						/* char type definitions    */
#include <portab.h>
#include <fcntl.h>

int _main(P(char *) com, P(int) len)
PP(char *com;)							/* Command address      */
PP(int len;)								/* Command length       */
{
	register char *s;						/* Temp char pointer        */

	/* Initialize channels */
	_chinit();
	open(__tname, O_RDONLY);				/* Open STDIN           */
	open(__tname, O_WRONLY);				/* Open STDOUT          */
	open(__tname, O_WRONLY);				/* Open STDERR          */

	com[len] = '\0';						/* Insure null at line end  */
	
	/* Convert string to lower case */
	return __main(com, len);
}
