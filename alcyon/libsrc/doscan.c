/*****************************************************************************
*
*			_ d o s c a n   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"_doscan" is the common subroutine to "scanf", "fscanf", and
*	"sscanf".
*
*	Calling sequence:
*		ret = _doscan(sp,fmt,aps)
*	Where:
*		int ret	number items matched AND assigned
*				EOF when encountered on stream sp
*		FILE *sp	pointer to input stream
*		char *fmt	input specification string
*		char **aps	pointer to (pointers to) arguments
*
*	Edits:
*	1/11/84 whf	fix "%1s" bug
*	12/02/83 HY	???
*****************************************************************************/

#include "lib.h"
#include <ctype.h>
#include <string.h>
#include <portab.h>
#include <math.h>

#define INFINITY 32767
#define NXTNI(d) if( ((d)=getc(sp)) == EOF ) return(EOF)

/* is_member of a set */
static int _ismem(P(char) c, P(const char *)set, P(int) invert)		/* is_member of a set       */
PP(char c;)									/* candidate for set element */
PP(const char *set;)								/* null terminated string   */
PP(int invert;)							/* invert sense of function */
{
	register int rv;						/* temp return val      */

	/* BUG: strchr not declared; returns ptr */
	rv = (int)strchr(set, c);				/* look for c in set        */
	return invert ? !rv : rv;			/* return (maybe inverted) # */
}


int _doscan(P(FILE *)sp, P(const char *)fmt, P(va_list) args)
PP(register FILE *sp;)							/* Stream ptr           */
PP(const char *fmt;)								/* Input format spec ptr    */
PP(va_list args;)								/* Argument ptrs ptr        */
{
	register char c;							/* Spec character       */
	register int ni;						/* Input char           */

	int noassign,						/* Suppress assignment flag */
	 invert,							/* Invert flag (_ismember)  */
	 numfound,							/* Number found flag        */
	 longf,								/* Long ptr flag        */
	 shortf;							/* Short ptr flag       */

	int width;							/* Max field width      */
	int tval,							/* Temp val holder      */
	 base,								/* For numeric conversion   */
	 nitems;							/* Number items mtchd & assd */

	long val;							/* For numeric conversion   */
	union {								/* Local value area     */
		long l;
		int i;
		float f;
		double d;
		char *s;
	} locval;

	char locbuf[BUFSIZ];				/* Local buffer area        */
	char *db;							/* Destination ptr      */
	char setbuf[128];					/* Area for '[...]' sets    */
	char *sb;							/* Ptr into setbuf      */

	nitems = 0;
	while ((c = *fmt++) != 0)
	{
		if (isspace((UBYTE)c))					/* if space in spec str     */
		{
			do
			{							/* deblank leading blanks   */
				c = *fmt++;
			} while (isspace((UBYTE) c));
			do
			{							/* deblank input stream     */
				NXTNI(ni);
			} while (isspace(ni));		/* read past space in input */
			ungetc(ni, sp);				/* back up once         */
			if (c == '\0')				/* check for this here      */
				break;
		}
		/* c points to non space    */

		if (c != '%')					/* match spec char (non'%')? */
		{
			NXTNI(ni);					/* grab from input      */
			if (ni != c)				/*  & test          */
			{
				ungetc(ni, sp);			/* nope, put it back        */
				return (nitems);		/* return early         */
			}
			continue;					/* yup, do the next char    */
		} else
			c = *fmt++;					/* look for an assignment   */
		noassign = FALSE;				/* assume we will assign    */
		if (c == '*')					/* unless they say otherwise */
		{
			noassign++;
			c = *fmt++;
		}
		width = INFINITY;				/* assume no special width  */
		while (c >= '0' && c <= '9')	/* unless they say otherwise */
		{
			if (width == INFINITY)
				width = 0;
			width = 10 * width + (c - '0');
			c = *fmt++;
		}

		longf = FALSE;					/* assume ptrs not to LONGs */
		shortf = FALSE;					/*   and not to SHORTs      */
		if (c == 'l')					/* unless they say otherwise */
		{
			longf++;
			c = *fmt++;
		} else if (c == 'h')
		{
			shortf++;
			c = *fmt++;
		}
		
		switch (c)						/* which conversion char?   */
		{
		case 'D':						/* decimal input        */
		case 'd':
			base = 10;
			goto procnum;
		case 'O':						/* octal input          */
		case 'o':
			base = 8;
			goto procnum;
		case 'X':						/* hex input            */
		case 'x':
			base = 16;
		  procnum:
			if (isupper((UBYTE) c))		/* upper case means long    */
			{
				/* c = tolower(c); */
				longf++;
			}
			do
			{
				NXTNI(ni);				/* grab a char from input   */
			} while (isspace(ni));		/* until non-blank      */
			if (noassign)
				db = (char *)&locval;			/* got to put it somewhere  */
			else
				db = (char *)va_arg(args, int *);			/* like to put it here      */
			invert = FALSE;				/* assume pos number        */
			if (ni == '+' || ni == '-')
			{							/* leading plus or minus */
				if (ni == '-')
					invert++;
				width--;
				NXTNI(ni);
			}
			val = 0L;					/* initial value        */
			numfound = FALSE;			/* assume guilty til proven */
			while (width-- > 0)
			{							/* for each char *********** */
				tval = ni = toupper(ni);	/* should work for numbers  */
				if (tval < '0' || tval > 'F')
					break;
				if (tval > '9' && tval < 'A')
					break;
				tval -= '0';
				if (tval > 9)
					tval = tval + ('0' - 'A' + 10);
				if (tval >= base)
					break;
				numfound++;				/* is a number          */
				val = base * val + tval;
				NXTNI(ni);				/* next input, please       */
			}							/*  ******************** */
			ungetc(ni, sp);				/* back off input       */
			if (numfound == FALSE)		/* was it there?        */
				return (nitems);		/*   no, too bad        */
			if (invert)					/* leading minus?       */
				val = -val;				/*              */
			if (longf)					/* store this value     */
				*(long *) db = val;		/* somewhere            */
			else if (shortf)			/*              */
				*(short *) db = val;	/* somehow          */
			else						/*              */
				*(int *) db = val;		/* sometime         */
			if (!noassign)				/* increment only if assign */
				nitems++;				/*              */
			break;

		case 's':						/* string input         */
		case 'c':						/* char input           */
		case '[':						/* input 'one of'       */
			NXTNI(ni);					/* grab a character     */
			if (c == 's')				/* scan string ************* */
			{
				sb = " \t\n";			/* point set to white chars */
				while (_ismem(ni, sb, FALSE))	/* keep reading whitespace  */
					NXTNI(ni);			/*  if there is any     */
				invert = TRUE;			/* after, read until whites */
			} else if (c == 'c')		/* scan char *************** */
			{
				sb = "";				/* Null set         */
				if (width == INFINITY)	/* handle default width     */
					width = 1;
				invert = TRUE;			/* invert "_ismem" func     */
			} else						/* scan set **************** */
			{							/* must be a '['        */
				invert = FALSE;
				if ((c = *fmt++) == '^')	/* invert "_ismem" ???      */
				{
					invert = TRUE;
					c = *fmt++;
				}
				for (sb = setbuf; c != '\0' && c != ']'; sb++, c = *fmt++)
					*sb = c;
				*sb = '\0';
				sb = setbuf;
			}
			if (noassign)
				db = locbuf;			/* give rtn somewhere to pt */
			else
				db = va_arg(args, char *);			/* o.w. grab the desired ptr */
			while (width-- > 0 && _ismem(ni, sb, invert))
			{
				*db++ = ni;
				NXTNI(ni);
			}
			ungetc(ni, sp);				/* we've always read 1 past */
			if (c != 'c')				/* not char type?       */
				*db = '\0';				/* then null terminate      */
			if (!noassign)
				nitems++;				/* successful assignment    */
			break;

		case 'E':		/*********** not fully implemented **********/
		case 'e':
		case 'F':
		case 'f':
			if (isupper((UBYTE) c))				/* upper case means long    */
				longf++;
			do
			{
				NXTNI(ni);				/* grab a char from input   */
			} while (isspace(ni));		/* until non-blank      */
			if (noassign)
				db = (char *)&locval;			/* got to put it somewhere  */
			else
				db = (char *)va_arg(args, double *);			/* like to put it here      */

			sb = setbuf;				/* setup up addr to hold string */
			while (width-- > 0)
			{
				*sb++ = ni;
				if ((ni < '0' || ni > '9') && (ni != '.') && (ni != 'e') && (ni != 'E') && (ni != '-') && (ni != '+'))
					break;
				NXTNI(ni);
			}

			ungetc(ni, sp);
			*--sb = '\0';
			if (longf)
				*(double *) db = atof(setbuf);
			else
				*(float *) db = (float) atof(setbuf);

			if (!noassign)
				nitems++;
			break;

		case '%':
		default:
			NXTNI(ni);
			if (ni != c)				/* must match c         */
			{
				ungetc(ni, sp);
				return (nitems);
			}
			break;
		}
	}
	return nitems;
}
