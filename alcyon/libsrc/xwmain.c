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

#include <osbind.h>
#include "lib.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <portab.h>
#include <fcntl.h>
#include <stdint.h>
#include <ostruct.h>
#include <mint/basepage.h>


int main PROTO((int argc, char **argv, char **envp));

#define ISWHITE(ch) ((ch) == '\0' || isspace((UBYTE)ch))

char **environ;

static int argc;						/* Arg count */
static char **argv;						/* -> array of pointers */
static char **argv2;					/* Companion ptr to argv */
static _DTA dta;

#if 0
#define STRICTLY_COMPATIBLE_WITH_STANDARD
#endif

/* Error routine */
/* output directly to CON: */
static VOID _err(P(const char *) s1, P(const char *) s2)
PP(const char *s1;)
PP(const char *s2;)
{
	/* Output error message */
	Cconws(s1);
	/* And filename */
	Cconws(s2);
	/* + Newline */
	Cconws("\r\n");
	/* And fail hard */
	exit(-1);
}


/*
 *	Addargv function -- adds a pointer to the argv array, getting the
 *	space from the heap.
 */
static VOID addargv(P(register char *) ptr)
PP(register char *ptr;)							/* -> Argument string to add */
{
	/* Load pointer */
	*argv2 = ptr;
	/* More room from heap */
	argv2++;
	/* Increment arg count */
	argc++;
}


/*****************************************************************************/


static VOID initargs(P(char *) com, P(int) len)
PP(char *com;)								/* Command address */
PP(int len;)								/* Command length */
{
	register int i;							/* Define a count var. */
	register char *s;						/* Temp byte pointer */
	register char *p;						/* Another "" */
	register char c;						/* Character temp */
	register BASEPAGE *bp;
	
	/* set DTA to a save place; it might point to the commandline buffer */
	Fsetdta(&dta);
	/* -> first free location */
	environ = (char **)sbrk(0);
	argv2 = environ;
	/* No args yet */
	argc = 0;
	bp = _base;
	if ((p = bp->p_env) != NULL)
	{
		while (*p != '\0')
		{
			if (p[0] == 'A' && p[1] == 'R' && p[2] == 'G' && p[3] == 'V' && p[4] == '=')
			{
				*p = '\0';
				*argv2++ = NULL;
#ifdef STRICTLY_COMPATIBLE_WITH_STANDARD
				if (len != 127)
					break;
#endif
				/* skip ARGV= string */
				p += 5;
				/* skip ARGV= value */
				while (*p++ != '\0')
					;
				argv = argv2;
				*argv2++ = p;
				/* skip argv[0] */
				while (*p++ != '\0')
					;
				argc++;
				while (*p != '\0')
				{
					*argv2++ = p;
					argc++;
					while (*p++ != '\0')
						;
				}
				break;
			}
			*argv2++ = p;
			while (*p++ != '\0')
				;
		}
	}
	/* terminate environment */
	*argv2++ = NULL;
	
	if (argc == 0)
	{
		/* -> first free location */
		argv = argv2;
		if (len > 126)
			len = 126;
		com[len] = '\0';
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
				i = (int)((intptr_t)p - (intptr_t)s);
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
					if (open(s + 1, O_RDONLY | O_TEXT) != STDIN)
						_err("Cannot open ", s + 1);
					break;
	
				case '>':
					/* Redirecting output */
					close(STDOUT);
					if (s[1] == '>')
					{
						/* Appending, try to open old */
						if (open(s + 2, O_WRONLY | O_TEXT | O_CREAT | O_APPEND) != STDOUT)
							_err("Cannot append ", s + 1);
					} else
					{
						/* Try to open new */
						if (open(s + 1, O_WRONLY | O_CREAT | O_TRUNC | O_TEXT) != STDOUT)
							_err("Cannot create ", s + 1);
					}
					break;
	
				default:
	
					/* save in argv */
					addargv(s);
					break;
				}
			}
		}
		/* Insure terminator */
		addargv(NULL);
		/* Back off by 1 */
		argc--;
	}
	
}


int __main(P(char *) com, P(int) len)
PP(char *com;)								/* Command address */
PP(int len;)								/* Command length */
{
	initargs(com, len);
	/* Allocate the pointers */
	if (brk((VOIDPTR)argv2) < 0)
		_err("Stack Overflow", "");
	/* Invoke C program (No Env) */
	exit(main(argc, argv, NULL));
}
