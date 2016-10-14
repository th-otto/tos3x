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

#define lseek bug_lseek
#include <osif.h>
#include "../libsrc/lib.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <portab.h>
#include <fcntl.h>
#undef lseek
int lseek PROTO((int fd, long offs, int whence));


int main PROTO((int argc, char **argv, char **envp));

#define ISWHITE(ch) ((long)(ch) == 0L || isspace((long)(ch)))


static int argc;						/* Arg count */
static char **argv;						/* -> array of pointers */
static char **argv2;						/* Companion ptr to argv */


static VOID _err PROTO((const char *s1, const char *s2)) __attribute__((noreturn));
static VOID addargv PROTO((char *ptr));
static VOID _toasc PROTO((FD *p, char c, char * buf));


int __main(P(char *) com, P(int) len)
PP(char *com;)								/* Command address */
PP(int len;)								/* Command length */
{
	register int i;							/* Define a count var. */
	register char *s;						/* Temp byte pointer */
	register char *p;						/* Another "" */
	register char c;						/* Character temp */
	FD *pfd;							/* File Desc temp */
	
	char tmpbuf[30];					/* Filename temp */
	int j;

	/* -> first free location */
	argv2 = argv = (char **)sbrk(0);
	/* No args yet */
	argc = 0;
	addargv(__pname);
	for (s = com; *s; s += i)
	{
		/* Skip leading spaces */
		while (*s && isspace((UBYTE)*s))
		{
			/* kludge alert: should be s++, but couldnt convince assembler to not optimize it to addq */
#ifdef __ALCYON__
			asm("adda.l #1,a5");
#else
			s++;
#endif
		}
		/* End of line? */
		c = *s;
		if (c == '\0')
			break;
		if (strchr("\"'", c) != NULL)
		{
			/* Quoted string */
			/* Find next */
			/* kludge alert: see above */
#ifdef __ALCYON__
			asm("move.b    d6,d0");
			asm("ext.w     d0");
			asm("move.w    d0,(a7)");
			asm("move.l    a5,-(a7)");
			asm("dc.w $0697,$0000,$0001");
			asm("jsr _strchr");
			asm("addq.l    #4,a7");
			asm("move.l d0,a4");
#else
			p = strchr(s + 1, c);
#endif
			if (p == NULL)
				_err(s, ": unmatched quote");
			/* Compute length */
			i = (long) p - (long)s;
			/* kludge alert: see above */
#ifdef __ALCYON__
			asm("movea.w   d7,a0");
			asm("adda.l    a5,a0");
			asm("clr.b     (a0)");
			asm("dc.w $de7c,$0001");
			asm("move.l    a5,(a7)");
			asm("dc.w $0697,$0000,$0001");
			asm("jsr _addargv");
#else
			s[i++] = '\0';
			/* Add to arg list */
			addargv(s + 1);
#endif
		} else
		{
#ifdef __ALCYON__
			asm("clr.w     d7");
			asm("bra.w     L9001");
			asm("L9000:");
			asm("dc.w $de7c,$0001");
			asm("L9001:");
			asm("move.w    d7,d0");
			asm("ext.l     d0");
			asm("move.b    0(a5,d0.l),d0");
			asm("ext.w     d0");
			asm("ext.l     d0");
			asm("tst.l     d0");
			asm("beq.w     L9002");
			asm("move.w    d7,d0");
			asm("ext.l     d0");
			asm("move.b    0(a5,d0.l),d0");
			asm("ext.w     d0");
			asm("ext.l     d0");
			asm("add.l     #___atab,d0");
			asm("movea.l   d0,a0");
			asm("btst      #5,(a0)");
			asm("beq.s     L9000");
			asm("L9002:");
			/* If last is space, make it a null for C */
			asm("move.w    d7,d0");
			asm("ext.l     d0");
			asm("tst.b     0(a5,d0.l)");
			asm("beq.w     L9003");
			asm("movea.w   d7,a0");
			asm("adda.l    a5,a0");
			asm("clr.b     (a0)");
			asm("dc.w $de7c,$0001");
			asm("L9003:");
#else
			/* How many characters? */
			for (i = 0; !ISWHITE(s[i]); )
			{
				i++;
				asm("dc.w $de7c,$0001");
			}
			/* If last is space, make it a null for C */
			if (s[i])
				s[i++] = '\0';
#endif
			/* Now do i/o scan */
			if (strchr(s, '?') ||	/* Wild */
				strchr(s, '*'))		/* Cards? */
			{
				/* Use unused channel */
				pfd = _getccb(STDERR + 1);
				/* Use buffer for DMA */
				__OSIF(SETDMA, pfd->buffer);
				/* Do the search */
				c = (char)__open(STDERR + 1, s, SEARCHF);
				if (c == 0xff)
					_err(s, ": No match");
				for (j = strlen(s); j != 0; j--)
				{
					if (strchr("\\:", *((j - 1) + s)) != NULL)
						break;
				}
				/* Do search next's */
				while (c != 0xff)
				{
					/* Convert file to ascii */
					_toasc(pfd, c, tmpbuf);
					/* Allocate area */
					p = _salloc(strlen(tmpbuf) + j + 1);
					/* Move in filename */
					strncpy(p, s, j);
#ifdef __ALCYON__
				asm("movea.w   -36(a6),a0");
				asm("adda.l    a4,a0");
				asm("clr.b     (a0)");
#else
					p[j] = '\0';
#endif
					strcat(p, tmpbuf);
					/* Add this file to argv */
					addargv(p);
					c = (char)__open(STDERR + 1, s, SEARCHN);
				}
			} else
			{
				/* save in argv */
				addargv(s);
			}
		}
	}
	/* Insure terminator */
	addargv(NULL);
	/* Back off by 1 */
#ifdef __ALCYON__
	asm("dc.w $0479,$0001");
	asm("dc.l L1");
#else
	argc--;
#endif
	/* Allocate the pointers */
	if (brk((VOIDPTR)argv2) == -1)
		_err("Stack Overflow", "");
	/* Invoke C program (No Env) */
	exit(main(argc, argv, NULL));
}


/* Error routine */
static VOID _err(P(const char *) s1, P(const char *) s2)
PP(const char *s1;)
PP(const char *s2;)
{
	char buf[128];						/* place to build message */

	/* Output error message */
	strcpy(buf, s1);
	/* And filename */
	strcat(buf, s2);
	/* + Newline */
	strcat(buf, "\r\n");
	/* output directly to CON: */
	__OSIF(C_WRITESTR, buf);
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
	register char *s;
	
	if (ptr)
	{
		s = ptr;
#ifdef __ALCYON__
				asm("bra.w     L8000");
				asm("L7999:");
				asm("dc.w $d9fc,0,1");
				asm("L8000:");
				asm("move.b    (a4),d0");
				asm("ext.w     d0");
				asm("ext.l     d0");
				asm("add.l     #___atab,d0");
				asm("movea.l   d0,a0");
				asm("btst      #3,(a0)");
				asm("beq.w     L8001");
				asm("move.b    (a4),d0");
				asm("ext.w     d0");
				asm("add.w     #$0020,d0");
				asm("bra.w     L8002");
				asm("L8001:");
				asm("move.b    (a4),d0");
				asm("ext.w     d0");
				asm("L8002:");
				asm("ext.w     d0");
				asm("move.b    d0,(a4)");
				asm("bne.s     L7999");
#else
		for (; (*s = toupper(*s)); )
		{
			s++;
		}
#endif
	}
	/* Load pointer */
	*argv2 = ptr;
#ifdef __ALCYON__
	asm("dc.w $06b9");
	asm("dc.l 4,L3");
	asm("dc.w $0679,1");
	asm("dc.l L1");
#else
	/* More room from heap */
	argv2++;
	/* Increment arg count */
	argc++;
#endif
}

/*****************************************************************************/


/*
 *	Toasc routine -- combines the FCB name in the DMA and the user number
 *	/ drive field to produce an ascii file name for SEARCHes.
 *
 */
static VOID _toasc(P(register FD *) p, P(register char) c, P(register char *) buf)
PP(register FD *p;)						/* -> Data area */
PP(register char c;)						/* 0 .. 3 search code */
PP(register char *buf;)					/* Output buffer area */
{
	register char *f;						/* -> Fcb in DMA buffer */
#if GEMDOS
	f = p->buffer + 30; /* DTA.d_name */
	while ((*buf++ = *f++) != '\0')
		;
#else
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
#endif
}
