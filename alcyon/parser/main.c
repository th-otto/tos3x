/*
 *	Copyright 1983
 *	Alcyon Corporation
 *	8716 Production Ave.
 *	San Diego, Ca.  92121
 */

#include "parser.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


/*
 *	ALCYON C Compiler for the Motorola 68000 - Parser
 *
 *	Called from c68:
 *
 *		c068 source icode link
 *
 *	source:		input source code, preprocessed with comments stripped
 *
 *	icode:		contains the intermediate code for the code generator,
 *				for a detailed explanaion see ../doc/icode.
 *
 *	link:		contains the procedure link and movem instructions.
 *
 *	The basic structure of the parser is as follows:
 *
 *	main							main driver for parser
 *		syminit						initializes symbol table
 *		doextdef					external definition syntax
 *			getatt					get type attributes
 *				dlist				declaration list for strucs/unions
 *					getatt			recursive gettype call
 *					dodecl			do one declaration
 *						declarator	handle declarator syntax
 *			dodecl					do one external declaraion
 *			initlist				external initialization list
 *				cexpr				constant expressions
 *					expr			arithmetic expressions
 *						maketree	build operator tree
 *			funcbody				function body
 *				dlist				declaration list
 *				stmt				function statements
 *					stmt			recursive stmt call
 *					expr			arithmetic expressions
 *
 */

static char *strfile;

static char const program_name[] = "c068";


static VOID cleanup(NOTHING)
{
	signal(SIGINT, SIG_IGN);
	if (lfil)
		fclose(lfil);
	if (ifil && ifil != stdin)
		fclose(ifil);
	if (ofil && ofil != stdout)
		fclose(ofil);
	if (sfil)
		fclose(sfil);
	if (strfile)
		unlink(strfile);
	exit(errcnt != 0);
}


/* usage - output usage error message and die */
static VOID usage(NOTHING)
{
	fatal(_("usage: %s source link icode strings [-e|-f] [-w] [-t]\n"), program_name);
}


/*
 * main - main routine for parser
 *		Checks arguments, opens input and output files, does main loop
 *		for external declarations and blocks.
 */
int main(P(int) argc, P(char **)argv)
PP(int argc;)								/* argument count */
PP(char **argv;)							/* argument pointers */
{
	register char *q, *p;

	if (argc < 5)
		usage();

	signal(SIGINT, (sighandler_t)cleanup);
	signal(SIGQUIT, (sighandler_t)cleanup);
	signal(SIGHUP, (sighandler_t)cleanup);
	signal(SIGTERM, (sighandler_t)cleanup);

	argv++;
	for (q = source, p = *argv++; (*q++ = *p++) != 0;)
		;
	if ((ifil = fopen(source, "r")) == NULL)
		fatal(_("can't open %s"), source);
	source[(int)strlen(source) - 1] = 'c';
	if ((ofil = fopen(*argv++, "w")) == NULL || (lfil = fopen(*argv++, "w")) == NULL)
		fatal(_("temp creation error"));

	strfile = *argv++;
	if ((sfil = fopen(strfile, "w")) == NULL)
		fatal(_("string file temp creation error"));
	obp = ofil;
	lineno++;
	frstp = -1;							/* initialize only once */
	cr_last = 1;

	for (argc -= 5; argc; argv++, argc--)
	{
		q = *argv;
		if (*q++ != '-')
			usage();
		for (;;)
		{
			switch (*q++)
			{
			case 'e':					/* ieee floats */
				fflag = 0;				
				continue;

			case 'f':					/* FFP floats */
				fflag = 1;
				continue;

			case 'g':					/* symbolic debugger flag */
				gflag++;
				continue;

			case 't':					/* put strings into text segment */
				tflag++;
				continue;
#ifndef NOPROFILE
			case 'p':					/* profiler output file */
				profile++;
				continue;
#endif
			case 'w':					/* warning messages, not fatal */
				wflag++;
				continue;

#ifdef DEBUG
			case 'd':					/* turn debugging on */
				while (*q)
				{
					switch (*q)
					{
					case 'i':					/* if debug on, debug initialization */
						initdebug++;
						break;
					case 's':					/* if debug on, debug symbols */
						symdebug++;
						break;
					case 'x':					/* if debug on, debug expr tree */
						treedebug++;
						break;
					}
					q++;
				}
				continue;
#endif

			case '\0':
				break;

			default:
				usage();
				break;
			}
			break;
		}
	}

	syminit();
	while (!PEEK(CEOF))
		doextdef();
	outeof();
	if (!tflag)
		outdata();
	else
		OUTTEXT();
	copysfile(strfile);
	cleanup();
	return 0;
}


static VOID verror(P(const char *) s, P(va_list) args)
{
	fprintf(stderr, "\"%s\", * %d: ", source, lineno);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	errcnt++;
}


/*
 * error - report an error message
 *		outputs current line number and error message
 *		generate filename and approp line number
 */
VOID error(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	va_end(args);
}


/*
 * fatal - fatal error
 * Outputs error message and exits
 */
VOID fatal(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	va_end(args);
	errcnt = -1;
	cleanup();
}


/*
 * warning - Bad practices error message (non-portable code)
 *		Outputs error message
 *		generate filename and approp line number
 */
VOID warning(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	if (wflag)
		return;
	va_start(args, s);
	fprintf(stderr, "\"%s\", * %d: (warning) ", source, lineno);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}


/*
 * synerr - syntax error
 * Outputs error message and tries to resyncronize input.
 */
VOID synerr(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	register short token;
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	while ((token = gettok(0)) != SEMI && token != CEOF && token != LCURBR && token != RCURBR)
		;
	pbtok(token);
}


/*
 * strindex - find the index of a character in a string
 * This is identical to Software Tools index.
 * returns index of c in str or -1
 */
int strindex(P(const char *) str, P(char) chr)
PP(const char *str;)								/* pointer to string to search */
PP(char chr;)								/* character to search for */
{
	register const char *s;
	register short i;

	for (s = str, i = 0; *s != '\0'; i++)
		if (*s++ == chr)
			return i;
	return -1;
}


VOID oprintf(P(const char *) string _va_alist)
PP(const char *string;)
_va_dcl
{
	char area[256];
	register char *p;
	va_list args;
	
	va_start(args, string);
	vsprintf(area, string, args);
	for (p = &area[0]; *p; p++)
		oputchar(*p);
	va_end(args);
}
