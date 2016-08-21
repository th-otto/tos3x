/***********************************************************************
*
*			g e t p a s s   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"getpass" writes a prompt to the console (NOT stdout) and accepts
*	a string of 1-8 chars from the console (NOT stdin) without 
*	echoing the input to the terminal.  Only limited CP/M line edit
*	commands are available (CTL H, DEL, CTL X) on input.
*
*	Calling sequence:
*		pret = getpass(prompt)
*	Where:
*		pret -> ptr to 1-8 chars of null terminated password
*		prompt -> null teminated console out msg.
*
*	Modified:
*	Oct 83 whf - handle OS Independent Raw Input
*
**************************************************************************/

#include "lib.h"
#include <osif.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define CMASK	0x7F
#define DEL	0x7F
#define CTRLX	0x18
#define CTRLC	0x03


static VOID _noecho(P(char *) bf, P(int) ln)
PP(register char *bf;)
PP(int ln;)
{
	register int cur;
	int ch;

	cur = 0;
	for (;;)
	{
		switch ((ch = (CMASK & _ttyinraw())) != 0)
		{
		case '\b':
		case DEL:
			if (cur > 0)
			{
				cur--;
				bf--;
			}
			break;
		case CTRLX:
			bf -= cur;
			cur = 0;
			break;
		case '\r':
		case '\n':
			*bf = '\0';
			return;
		case CTRLC:
			exit(1);
		default:
			*bf++ = ch;
			cur++;
			if (cur + 1 >= ln)
			{
				*bf = '\0';
				return;
			}
		}
	}
}


char *getpass(P(const char *) prompt)
PP(const char *prompt;)
{
	static char ibuf[9];

	__OSIF(C_WRITESTR, prompt);
	_noecho(ibuf, 9);
	return ibuf;
}
