/****************************************************************************
*
*               C   R U N T I M E   G R E P   R O U T I N E
*               -------------------------------------------
*       Copyright 1983 by Digital Research Inc.  All rights reserved.
*
*       The routine "grep" searches for an ascii character pattern in
*       a series of files.  "Grep" allows the following options:
*
*       OPTION  FUNCTION
*       ------  --------
*       -v      Print only lines not matching the pattern
*       -c      Print only a count of the lines
*       -n      Print with line numbers
*       -s      Write out the sector number in which the pattern was found
*       -l      Write out a list of the files in which the pattern was found
*       -y      Do not distinguish between upper & lower case
*
*       Calling Sequence:
*
*               grep(options,expression,files);
*
*       Where:
*
*               options         Is a pointer to a string of options
*               expression      Is a pointer to the search pattern
*               files           Is an array of pointers to filenames
*
*****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1


static char __match = TRUE;					/* Print Matching lines only */
static char __count = FALSE;					/* Just count the lines     */
static char __num = FALSE;						/* Print line #'s           */
static char __sector = FALSE;					/* Display the sector       */
static char __list = FALSE;					/* Display only a file list */
static char __nocase = FALSE;					/* Distinguish between cases */
static FILE *fstream;							/* file stream ptr          */
static char line[129];							/* line buffer              */
static char line1[129];						/* Temp line buffer         */
static char first = TRUE;						/* used on the list option  */
static unsigned int numfound = 0;						/* number of lines found    */


/*
 * PRINT ROUTINE
 */
static VOID prtline(P(char) found, P(unsigned int) char_cnt, P(unsigned int) line_cnt, P(char **)files)
PP(char found;)
PP(unsigned int char_cnt;)
PP(unsigned int line_cnt;)
PP(char **files;)
{
	if ((__match && found) || (!__match && !found))
	{
		/* count the # lines found  */
		numfound++;
		if (__count)
			return;
		if (__list)
		{
			if (first)
				printf("\n%s", *files);
			first = FALSE;
			return;
		}
		/* do we print the sector# ? */
		if (__sector)
		{
			printf("\n%d", char_cnt / 128);
			return;
		}
		/* do we print a line # ?   */
		if (__num)
		{
			printf("%5d:  ", line_cnt);
		}
		fputs(line1, stdout);
	}
}



static VOID grep(P(char *) options, P(char *) expression, P(char **) files)
PP(register char *options;)
PP(register char *expression;)
PP(char **files;)
{
	register char *p;						/* temp pointer             */
	register unsigned int patt_cnt;					/* pattern counter          */
	register unsigned int char_cnt;					/* character count          */
	register unsigned int line_cnt;					/* line count               */
	register unsigned int i;						/* counter                  */
	unsigned int length;						/* length of pattern        */
	char found;							/* set if pattern is found  */


#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (*options != '-')
	{
		fprintf(stderr, "FIND--Illegal option list: %s\n", options);
		exit(1);
	}
	while (*options)
	{
		switch (*++options)
		{
		case 'v':
			__match = FALSE;
			break;
		case 'c':
			__count = TRUE;
			break;
		case 'n':
			__num = TRUE;
			break;
		case 's':
			__sector = TRUE;
			break;
		case 'l':
			__list = TRUE;
			break;
		case 'y':
			__nocase = TRUE;

		}
	}

	if (__list)
	{
		__count = FALSE;
		__num = FALSE;
		__sector = FALSE;
	} else if (__count)
	{
		__num = FALSE;
		__sector = FALSE;
	} else if (__num)
		__sector = FALSE;
	if (__nocase)
	{
		p = expression;
		while (*p)
		{
			*p = toupper(*p);
			p++;
		}
	}
	length = strlen(expression);

	while (*files)
	{
		if ((fstream = fopen(*files, "rb")) != NULL)
		{
			char_cnt = 0;
			line_cnt = 0;
			numfound = 0;
			first = TRUE;
			if (!__list && !__count)
			{
				printf("\nFILE:  %s\n", *files);
				printf("----\n");
			}
			while (!feof(fstream))
			{
				found = FALSE;
				patt_cnt = 0;
				p = expression;
				for (i = 0; i < 129; i++)
					line[i] = line1[i] = 0;
				fgets(line1, 129, fstream);

				for (i = 0; line1[i]; i++)
					if (__nocase)
						line[i] = toupper(line1[i]);
					else
						line[i] = line1[i];

				i = 0;
				while (line[i])
				{
					char_cnt++;
					if (line[i] == *p)
					{
						p++;
						patt_cnt++;
						if (patt_cnt == length)
						{
							found = TRUE;
							p = expression;
							patt_cnt = 0;
						}
					} else
					{
						p = expression;
						patt_cnt = 0;
					}
					i++;
				}
				line[i - 1] = 0;
				line_cnt++;
				prtline(found, char_cnt, line_cnt, files);
			}
			if (__count)
				printf("\n%s: %d", *files, numfound);

			fclose(fstream);
			files++;
		} else
		{
			printf("\n%s not found\n", *files);
			/* terminate process        */
			exit(1);
		}
	}
}


int main(P(int) argc, P(char **)argv)
PP(int argc;)
PP(char **argv;)
{
	char *options;						/* Option pointer           */
	char *expression;					/* -> expression            */
	char **files;						/* -> files                 */

	if (argc < 3)
	{
	  usage:printf("Usage: find [-options] pattern files\n");
		exit(1);
	}

	options = "-ny";
	if (*argv[1] == '-')
	{
		options = argv[1];
		if (argc < 4)
			goto usage;
		expression = argv[2];
		files = &argv[3];
	} else
	{
		expression = argv[1];
		files = &argv[2];
	}
	/* Call the real function   */
	grep(options, expression, files);
	return 0;
}
