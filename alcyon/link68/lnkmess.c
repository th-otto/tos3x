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
	printf("--------------------------------------------------\n");
	printf("LINK68 Overlay Linker                  Release 0.f\n");
	printf("Serial No. XXXX-0000-654321    All Rights Reserved\n");
	printf("Copyright (c) 1983          Digital Research, Inc.\n");
	printf("--------------------------------------------------\n\n");
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
	endit(EXIT_FAILURE);
}

VOID synerr(P(const char *) str)
PP(const char *str;)
{
	fatalx(TRUE, "SYNTAX ERROR, EXPECTED: %s\n", str);
}

VOID oom(NOTHING)
{
	fatalx(FALSE, "HEAP OVERFLOW -- NOT ENOUGH MEMORY\n");
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
	printf("USAGE: %s [options] output = file, file, ...\n\n", progname);
	printf("THE OPTIONS ARE:\n\n");
	printf("\tABSOLUTE\n");
	printf("\tBSSBASE[hex number]\n");
	printf("\tCOMMAND[filename]\n");
	printf("\tCHAINED\n");
	printf("\tDATABASE[hex number]\n");
	printf("\tIGNORE\n");
	printf("\tLOCALS\n");
	/* printf("\tMAP\n"); */
	printf("\tNOLOCALS\n");
	printf("\tSYMBOLS\n");
	printf("\tTEMPFILES[drive]\n");
	printf("\tTEXTBASE[hex number]\n");
	printf("\tUNDEFINED\n");
}
