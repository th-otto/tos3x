#include "link68.h"
#include <stdlib.h>


static char const progname[] = PROGNAME;

/************************************************************************
 *
 * banner() -- print the sign-on message
 *
 ************************************************************************/

VOID banner(NOTHING)
{
	printf(_("--------------------------------------------------\n"));
	printf(_("LINK68 Overlay Linker                  Release 0.f\n"));
	printf(_("Serial No. XXXX-0000-654321    All Rights Reserved\n"));
	printf(_("Copyright (c) 1983          Digital Research, Inc.\n"));
	printf(_("--------------------------------------------------\n\n"));
}


/************************************************************************
 *
 * errorx(errnum, ptr) -- error processor for all modules.
 *
 *	Does the action specified for the error number.  The usual
 *	action is to print a message and exit.
 *
 ************************************************************************/

VOID errorx(P(const char *) fmt _va_alist)
PP(const char *fmt;)
_va_dcl
{
	va_list args;
	
	fprintf(stderr, "%s: ", progname);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exstat++;
}


VOID fatalx(P(int) witharrow, P(const char *) fmt _va_alist)
PP(int witharrow;)
PP(const char *fmt;)
_va_dcl
{
	va_list args;
	
	if (witharrow)
		putarrow();
	fprintf(stderr, "%s: ", progname);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	endit(-1);
}

VOID synerr(P(const char *) str)
PP(const char *str;)
{
	fatalx(TRUE, _("syntax error, expected: %s\n"), str);
}

VOID oom(NOTHING)
{
	fatalx(FALSE, _("heap overflow -- not enough memory\n"));
}

/************************************************************************
 *
 * usage() -- prints a simple help message
 *
 *	This routine is called if only the program name is typed
 *
 ************************************************************************/

VOID usage(NOTHING)
{
	printf(_("usage: %s [options] output = file, file, ...\n\n"), progname);
	printf(_("The options are:\n\n"));
	printf(_("\tABSOLUTE\n"));
	printf(_("\tBSSBASE[hex number]\n"));
	printf(_("\tCOMMAND[filename]\n"));
	printf(_("\tCHAINED\n"));
	printf(_("\tDATABASE[hex number]\n"));
	printf(_("\tIGNORE\n"));
	printf(_("\tLOCALS\n"));
	/* printf(_("\tMAP\n")); */
	printf(_("\tNOLOCALS\n"));
	printf(_("\tSYMBOLS\n"));
	printf(_("\tTEMPFILES[drive]\n"));
	printf(_("\tTEXTBASE[hex number]\n"));
	printf(_("\tUNDEFINED\n"));
}
