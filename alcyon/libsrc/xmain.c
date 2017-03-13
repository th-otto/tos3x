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

#include <osif.h>
#include "lib.h"
#include <portab.h>
#include <fcntl.h>

static VOID initstd(NOTHING)
{
	register FD *ch;;
	
	/* Initialize channels */
	_chinit();
	ch = _fd_get(O_RDONLY | O_TEXT);				/* Open STDIN           */
	if (ch)
	{
		ch->dosfd = STDIN;
		ch->flags |= OPENED;
	}
	ch = _fd_get(O_WRONLY | O_TEXT);				/* Open STDOUT          */
	if (ch)
	{
		ch->dosfd = STDOUT;
		ch->flags |= OPENED;
	}
	ch = _fd_get(O_WRONLY | O_TEXT);				/* Open STDERR          */
	if (ch)
	{
		ch->dosfd = STDERR;
		ch->flags |= OPENED;
	}
	
}

int _main(P(char *) com, P(int) len)
PP(char * com;)							/* Command address      */
PP(int len;)								/* Command length       */
{
	initstd();
	/* Insure null at line end  */
	if (len < 127)
		com[len] = '\0';
	else
		com[126] = '\0';
	return __main(com, len);
}
