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
#include <portab.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>



BYTE __match = TRUE;					/* Print Matching lines only */
BYTE __count = FALSE;					/* Just count the lines     */
BYTE __num = FALSE;						/* Print line #'s           */
BYTE __sector = FALSE;					/* Display the sector       */
BYTE __list = FALSE;					/* Display only a file list */
BYTE __nocase = FALSE;					/* Distinguish between cases */
FILE *fstream;							/* file stream ptr          */
BYTE line[129];							/* line buffer              */
BYTE line1[129];						/* Temp line buffer         */
BYTE first = TRUE;						/* used on the list option  */
UWORD numfound = 0;						/* number of lines found    */


#ifdef __ALCYON__
#define BINEXACT 1
#define fopenbr(f) fopenb(f, "r")
FILE *fopenb();							/* returns a ptr to a file  */
#else
#define BINEXACT 0
#define fopenbr(f) fopen(f, "rb")
#endif


VOID grep PROTO((char *options, char *expression, char **files));
VOID prtline PROTO((char found, UWORD char_cnt, UWORD line_cnt, char **files));


int main(P(int) argc, P(char **)argv)
PP(int argc;)
PP(char **argv;)
{
	BYTE *options;						/* Option pointer           */
	BYTE *expression;					/* -> expression            */
	BYTE **files;						/* -> files                 */

	if (argc < 2)
	{
	  usage:printf("Usage: find [-options] pattern files\n");
#if BINEXACT
		exit();
#else
		exit(1);
#endif
	}

	options = "-ny";
	if (*argv[1] == '-')
	{
		options = argv[1];
		if (argc < 3)
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
#if !BINEXACT
	return 0;
#endif
}


VOID grep(P(char *) options, P(char *) expression, P(char **) files)
PP(register char *options;)
PP(register char *expression;)
PP(char **files;)
{
	register BYTE *p;						/* temp pointer             */
	register UWORD patt_cnt;					/* pattern counter          */
	register UWORD char_cnt;					/* character count          */
	register UWORD line_cnt;					/* line count               */
	register UWORD i;						/* counter                  */
	UWORD length;						/* length of pattern        */
	BYTE found;							/* set if pattern is found  */


	if (*options != '-')
	{
		fprintf(stderr, "FIND--Illegal option list: %s\n", options);
		exit(-1);
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
		if ((fstream = fopenbr(*files)) != NULL)
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

		}
		fclose(fstream);
		files++;
	}
}

/*
 * PRINT ROUTINE
 */
VOID prtline(P(char) found, P(UWORD) char_cnt, P(UWORD) line_cnt, P(char **)files)
PP(char found;)
PP(UWORD char_cnt;)
PP(UWORD line_cnt;)
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
