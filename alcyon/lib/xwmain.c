/*****************************************************************************
*
*		"__main()" Subroutine to "_main()"
*		----------------------------------
*	Copyright 1983 by Digital Research Inc. All rights reserved.
*
*	Routine "__main()" is entered from the "_main()" routine to start a
*	C program. The command string from CP/M is parsed into
*	a UNIX-like "argc/argv" setup, including simple I/O redirection.
*
*	This module can be compiled without wildcard parsing if the
*	symbol "NOWILD" (note upper case) is defined in the compile line.
*
*	Edits:
*		02-Feb-84 whf  nowild -> NOWILD, removed write()
*		01-Jan-84 whf  add in handle for OPTION*.h
*		08-Dec-83 whf  handle PC-DOS diffs, add ">>" redirection
*		11-Oct-83 whf  converted to DRC from CPM68K
*
*	Calling Sequence:
*		return = _main(command,length);
*
*	Where:
*		command		Is the address of the command line from CP/M
*		length		Is the number of characters in the line,
*				excluding the termination character (CR/LF).
*
*****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <portab.h>
#include <fcntl.h>
#include <ostruct.h>


#define ISWHITE(ch) ((ch) == '\0' || isspace((UBYTE)ch))


static int argc;						/* Arg count */
static char **argv;						/* -> array of pointers */
static char **argv2;					/* Companion ptr to argv */


/* Error routine */
static VOID _err(P(const char *, s1), P(const char *, s2))
PP(const char *, s1;)
PP(const char *, s2;)
{
	char buf[128];						/* place to build message */

	/* Output error message */
	strcpy(buf, s1);
	/* And filename */
	strcat(buf, s2);
	/* + Newline */
	strcat(buf, "\r\n$");
	/* output directly to CON: */
	__OSIF(C_WRITESTR, buf);
	/* And fail hard */
	exit(-1);
}


/*
 *	Addargv function -- adds a pointer to the argv array, getting the
 *	space from the heap.
 */
static VOID addargv(P(register char *, ptr))
PP(register char *, ptr;)							/* -> Argument string to add */
{
	/* Load pointer */
	*argv2 = ptr;
	/* More room from heap */
	argv2++;
	/* Increment arg count */
	argc++;
}


/*****************************************************************************/


#ifndef NOWILD

#if !GEMDOS
/*
 *	Toasc routine -- combines the FCB name in the DMA and the user number
 *	/ drive field to produce an ascii file name for SEARCHes.
 *
 */
static VOID _toasc(P(register FD *, p), P(register char, c), P(register char *, buf))
PP(register FD *, p;)						/* -> Data area */
PP(register char, c;)						/* 0 .. 3 search code */
PP(register char *, buf;)					/* Output buffer area */
{
	register char *f;						/* -> Fcb in DMA buffer */
	int i;

	/* Nullify at first */
	*buf = '\0';
	i = FALSE;
	/* Pnt to results of search */
	f = p->buffer;
#if CPM
	/* c == directory search code */
	f += c * 32;
	if (p->user)
	{
		/* User # not default, cvt to real user # */
		i = (p->user) - 1;
		if (i >= 10)
			*buf++ = '1';				/* Assume user # <15 */
		*buf++ = (i % 10) + '0';
		*buf++ = ':';
		i = TRUE;
	}
#endif
	/* Drive specified? */
	if (p->fcb.drive)
	{
		if (i)							/* User #? */
			buf--;						/* Yes, back up over ':' */
		/* Put in drive code */
		*buf++ = p->fcb.drive - 1 + 'a';
		*buf++ = ':';					/* And delimiter */
	}
	/* Move the filename */
	for (i = 1; i < 9; i++)
	{
		if (f[i] != ' ')
			*buf++ = tolower((f[i] & 0x7f));
	}
	/* Put in delimiter */
	*buf++ = '.';
	/* Move in extension */
	for (i = 9; i < 12; i++)
	{
		if (f[i] != ' ')
			*buf++ = tolower((f[i] & 0x7f));
	}
	/* Null at end */
	*buf++ = '\0';
}

#endif

#else

/* stubroutine for OPTION*.h package */
VOID nowildcards(NOTHING)
{
}

#endif


int __main(P(char *, com), P(int, len))
PP(char *, com;)								/* Command address */
PP(int, len;)								/* Command length */
{
	register int i;							/* Define a count var. */
	register char *s;						/* Temp byte pointer */
	register char *p;						/* Another "" */
	register char c;						/* Character temp */

	/* -> first free location */
	argv2 = argv = (char **)sbrk(0);
	/* No args yet */
	argc = 0;
	addargv(__pname);
	for (s = com; *s; s += i)
	{
		/* Skip leading spaces */
		while (*s && isspace((UBYTE)*s))
			++s;
		/* End of line? */
		if (!*s)
			break;
		if (*s == '"' || *s == '\'')
		{
			/* Quoted string */
			c = *s;
			/* Find next */
			p = strchr(s + 1, c);
			if (p == NULL)
				_err(s, ": unmatched quote");
			/* Compute length */
			i = (int) p - (long)s; /* BUG; truncating pointer to int */
			s[i++] = '\0';
			/* Add to arg list */
			addargv(s + 1);
		} else
		{
			/* How many characters? */
			for (i = 0; !ISWHITE(s[i]); ++i)
				;
			/* If last is space, make it a null for C */
			if (s[i])
				s[i++] = '\0';
			/* Now do i/o scan */
			switch (*s)
			{
			case '<':
				/* Redirecting input */
				close(STDIN);
				if (opena(s + 1, O_RDONLY) != STDIN)
					_err("Cannot open ", s + 1);
				break;

			case '>':
				/* Redirecting output */
				close(STDOUT);
				if (s[1] == '>')
				{
					/* Appending, try to open old */
					if (opena(s + 2, O_WRONLY) != STDOUT || lseek(STDOUT, 0L, SEEK_END) < 0)
						_err("Cannot append ", s + 1);
				} else
				{
					/* Try to open new */
					if (creata(s + 1, 0) != STDOUT)
						_err("Cannot create ", s + 1);
				}
				break;

			default:

#ifndef NOWILD
				if (strchr(s, '?') ||	/* Wild */
					strchr(s, '*'))		/* Cards? */
				{
#if GEMDOS
					DTA dta;
					long res;
					
					__OSIF(SETDMA, &dta);
					res = jsfirst(s, 0);
					if (res < 0)
						_err(s, ": No match");
					/* Do search next's */
					while (res >= 0)
					{
						/* Allocate area */
						p = _salloc(strlen(dta.d_fname) + 1);
						/* Move in filename */
						strcpy(p, dta.d_fname);
						/* Add this file to argv */
						addargv(p);
						res = jsnext();
					}
#else
					FD *pfd;							/* File Desc temp */
					char tmpbuf[30];					/* Filename temp */

					/* Use unused channel */
					pfd = _getccb(STDERR + 1);
					/* Use buffer for DMA */
					__OSIF(SETDMA, pfd->buffer);
					/* Do the search */
					c = jsfirst(s, 0);
					if (c < 0)
						_err(s, ": No match");
					/* Do search next's */
					while (c >= 0)
					{
						/* Convert file to ascii */
						_toasc(pfd, c, tmpbuf);
						/* Allocate area */
						p = _salloc(strlen(tmpbuf) + 1);
						/* Move in filename */
						strcpy(p, tmpbuf);
						/* Add this file to argv */
						addargv(p);
						c = jsnext();
					}
#endif
				} else
#endif
				{
					/* save in argv */
					addargv(s);
				}
			}
		}
	}
	/* Insure terminator */
	addargv(NULL);
	/* Back off by 1 */
	argc--;
	/* Allocate the pointers */
	if (brk((VOIDPTR)argv2) < 0)
		_err("Stack Overflow", "");
	/* Invoke C program (No Env) */
	exit(main(argc, argv, NULL));
}


