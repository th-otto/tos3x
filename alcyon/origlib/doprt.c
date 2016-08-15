/*************************************************************************
*
*			_ d o p r t   F u n c t i o n 
*			-----------------------------
*	Copyright 1982 by Digital Reseacrh Inc.  All rights reserved.
*
*	"_doprt" handles the printing for functions "printf", "fprintf",
*	and "sprintf".  Each of these sets up the parameters and calls _doprt.
*
*	Calling sequence:
*
*		nchrs = _doprt(stream,fmt,args)
*	Where:
*		nchrs  =  number of chars output by _doprt
*		stream -> destination buffer (FILE *)
*		fmt    -> a string specifying how "args" are to be output.
*		args   -> a list of arguments
*
*****************************************************************************/
#include "lib.h"
#include <string.h>
#include <osif.h>

#define TRUE  1
#define FALSE 0

/* print subroutine */

#define ARGWIDTH (sizeof(int))			/* width of arg to function */
#define NXTARG(ptr,isl) ((ptr)+=((isl) ? MAX(ARGWIDTH,sizeof(long)):ARGWIDTH))
#define IARG(ptr) ((ptr)+=ARGWIDTH)		/* int arg increment */
#if CPM68K
#define FARG(ptr) ((ptr)+=sizeof(float))	/* float/double arg increment */
#else
#define FARG(ptr) ((ptr)+=sizeof(double))	/* float/double arg increment */
#endif
#define CHOOSELS(isl) ((isl) ? __prtld : __prtshort )	/* prt long or short  */
#define XPUTC(a1,a2) {putc(a1,a2);nchrs++;}	/* count chars output       */

int _doprt(P(FILE *, sp), P(const char *, fmt), P(char *, pb))
PP(FILE *, sp;)							/* stream buffer ptr        */
PP(const char *, fmt;)					/* format string        */
PP(register char *, pb;)				/* pointer to args      */
{
	register char c;					/* format char          */
	char **ppi;							/* pointer to ptr       */
	int *pw;							/* pointer to word      */
	char padchar;						/* for padding          */
	const char *s;						/* string ptr for output    */
	char buf[BUFSIZ];					/* s usually points here    */

	int width;							/* for format spec      */

	register int prec,					/* precision            */
	 len;								/* total length of field    */

	int n,								/* counter          */
	 nchrs;								/* number chars output      */

	int left,							/* pad on left?         */
	 longf;								/* a long arg?          */

	static printfunc fn;				/* function holder      */

#ifndef __GNUC__
	/* unused; left in to produce binary equivalent output */
	double *dblptr;
#endif

	nchrs = 0;							/* init # chars output      */
	if (fmt)							/* make sure it's not NULL  */
	{
		/* examine the format string */
		while (1)
		{
			for (len = 0, s = fmt; *s && *s != '%'; ++s, ++len)
				;						/* grab up to EOS or '%'    */
			/* did we grab anything? */
			if (len > 0)
			{
				fputn(fmt, len, sp);	/* put them out         */
				fmt = s;				/* update this pointer      */
				nchrs += len;			/* update the counter       */
			}
			/* Did we stop for '%'? */
			if (*fmt++ != '%')
				break;					/*   no, EOS: we're finished */

			left = 0;					/* assume no left pad       */
			if ((c = *fmt++) == '-')	/*   left pad specified     */
			{
				c = *fmt++;
				left++;
			}
			padchar = ' ';				/* assume blank padding     */
			if (c == '0')				/*   zero fill specified    */
			{
				padchar = c;
				c = *fmt++;
			}
			width = -1;					/* assume no field spc width */
			if (c == '*')
			{
				/* get width from args */
				pw = (int *)pb;
				width = *pw;
				IARG(pb);
				c = *fmt++;
			} else
			{
				/* get width from fmt */
				while (c >= '0' && c <= '9')
				{
					if (width < 0)
						width = 0;
					width = width * 10 + (c - '0');
					c = *fmt++;
				}
			}
			
			prec = -1;					/* assume no precision      */
			if (c == '.')				/*   get precision      */
			{
				prec = 0;
				c = *fmt++;
			}
			if (c == '*')
			{
				/* get prec from args */
				pw = (int *)pb;
				prec = *pw;
				IARG(pb);
				c = *fmt++;
			} else
			{
				/* get prec from fmt */
				while (c >= '0' && c <= '9')
				{
					prec = prec * 10 + (c - '0');
					c = *fmt++;
				}
			}
			longf = 0; /* assume short int     */
			if (c == 'l' || c == 'L')	/* long int specified */
			{
				longf++;
				c = *fmt++;
			}


			/* we now have all prelims  */
			/* out of the way; let's see */
			/* what we want to print    */
			s = buf;
			switch (c)
			{
			case 'D':
				/* decimal signed */
				/* capital letter means long */
				longf++;
			case 'd':
				fn = CHOOSELS(longf);
				__prtint(pb, buf, 10, TRUE, fn);
				/* point to next arg */
				NXTARG(pb, longf);
				break;

			case 'U':
				/* decimal unsigned */
				longf++;
			case 'u':
				fn = CHOOSELS(longf);
				__prtint(pb, buf, 10, FALSE, fn);
				NXTARG(pb, longf);
				break;

			case 'O':
				/* octal signed */
				longf++;
			case 'o':
				fn = CHOOSELS(longf);
				__prtint(pb, buf, 8, FALSE, fn);
				NXTARG(pb, longf);
				break;

			case 'X':
				/* hexadecimal unsigned */
				longf++;
			case 'x':
				fn = CHOOSELS(longf);
				__prtint(pb, buf, 16, FALSE, fn);
				NXTARG(pb, longf);
				break;

			case 's':
			case 'S':
				/* string */
				ppi = (char **)pb;				/* need to deal with ptr    */
				s = *ppi;				/* cast to pointer      */
				NXTARG(pb, (sizeof(char *) == sizeof(long)));
				/* TRUE if pointers are long */
				/* FALSE if not         */
				break;

			case 'c':
			case 'C':
				/* character */
				pw = (int *)pb;				/* word ptr for portability */
				buf[0] = (*pw & 0377);	/* assume chars passed as ints */
				buf[1] = '\0';
				IARG(pb);
				break;

			case 'E':
			case 'e':
				/* E format */
				_petoa(pb, buf, prec, c);	/* Do the conversion       */
				FARG(pb);				/* Bump to next arg     */
				prec = -1;				/* Avoid truncation     */
				break;

			case 'F':
			case 'f':
				/* F format floating */
				_pftoa(pb, buf, prec, c);	/* do the conversion       */
				FARG(pb);				/* Bump to next arg     */
				prec = -1;				/* Avoid truncation     */
				break;

			case 'G':
			case 'g':
				/* G format floating */
				_pgtoa(pb, buf, prec, c);	/* do the conversion       */
				FARG(pb);				/* Bump to next arg     */
				prec = -1;				/* Avoid truncation     */
				break;

			default:
				/* just print the character */
				XPUTC(c, sp);
				continue;
			}
			
			/* how long is the output?  */
			len = strlen(s);
			/* cut off if user says so  */
			if (prec < len && prec >= 0)
				len = prec;
			/* how much extra room?     */
			n = width - len;
			if (!left)
			{
				/* check for neg   */
				if (padchar == '0' && *s == '-')
				{
					len--;
					/* output minus if leading 0 */
					XPUTC(*s++, sp);
				}
				/* now pad on left */
				while (n-- > 0)
					XPUTC(padchar, sp);
			}
			/* put buffered n chars */
			fputn(s, len, sp);
			nchrs += len;
			/* anything remain, then pad */
			while (n-- > 0)
				XPUTC(padchar, sp);
		}
	}
	
	/* report on # chars output */
	return nchrs;
}
