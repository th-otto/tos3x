/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/**
 **	formated print
 **/

#include "lib.h"
#include <string.h>


static VOID __putch(P(FILE *, stream), P(int, mode), P(char, c))
PP(FILE *, stream;)
PP(int, mode;)
PP(char, c;)
{
	char **str;
	
	if (mode)
	{
		str = (char **)stream;
		*(*str)++ = c;
	} else
	{
		putc(c, stream);
	}
}


int __doprint(P(FILE *, stream), P(const char *, fmt), P(int, mode), P(va_list, args))
PP(FILE *, stream;)
PP(register const char *, fmt;)
PP(int, mode;)
PP(va_list, args;)
{
	register char c;
	char buf[BUFSIZ];
	int width, prec;
	int left, longf, prepend;
	char padchar;
	char *s;
	long n;
	double d;
	int total;
	char *(*fn) PROTO((long n, char **pbuf, int base, int signed, char *digs));
	int len;

	total = 0;
	while ((c = *fmt++) != 0)
	{
		if (c != '%')
		{
			__putch(stream, mode, c);
			total++;
			continue;
		}
		prepend = left = 0;
		if ((c = *fmt++) == '-')
		{
			c = *fmt++;
			left++;
		}
		if (c == '#')
		{
			c = *fmt++;
			prepend++;
		}
		padchar = ' ';
		if (c == '0')
		{
			padchar = c;
			c = *fmt++;
		}
		width = -1;
		while (c >= '0' && c <= '9')
		{
			if (width < 0)
				width = 0;
			width = width * 10 + (c - '0');
			c = *fmt++;
		}
		if (c == '*')
		{
			c = *fmt++;
			width = va_arg(args, int);
		}
		prec = -1;
		if (c == '.')
		{
			prec = 0;
			c = *fmt++;
		}
		while (c >= '0' && c <= '9')
		{
			prec = prec * 10 + (c - '0');
			c = *fmt++;
		}
		if (c == '*')
		{
			c = *fmt++;
			prec = va_arg(args, int);
		}
		longf = 0;
		if (c == 'l')
		{
			longf++;
			c = *fmt++;
		}
		/*
		 * we now have all the prelims out of the way;
		 *  let's see what we want to print
		 */
		s = buf;
		switch (c)
		{
		case 'd':						/* decimal signed */
		case 'D':
			if (longf)
			{
				n = va_arg(args, long);
				fn = __prtld;
			} else
			{
				n = va_arg(args, int);
				fn = __prtshort;
			}
			__prtint(n, buf, 10, 1, fn, 0);
			break;

		case 'u':						/* decimal unsigned */
		case 'U':
			if (longf)
			{
				n = va_arg(args, long);
				fn = __prtld;
			} else
			{
				n = va_arg(args, int);
				fn = __prtshort;
			}
			__prtint(n, buf, 10, 0, fn, 0);
			break;

		case 'o':						/* octal unsigned */
		case 'O':
			if (prepend)
			{
				__putch(stream, mode, '0');
				total++;
			}
			if (longf)
			{
				n = va_arg(args, long);
				fn = __prtld;
			} else
			{
				n = va_arg(args, int);
				fn = __prtshort;
			}
			__prtint(n, buf, 8, 0, fn, 0);
			break;

		case 'x':						/* hexadecimal unsigned */
		case 'X':
			if (prepend)
			{							/* [vlh] 26 jul 83 */
				__putch(stream, mode, '0');
				__putch(stream, mode, 'x');
				total += 2;
			}
			if (longf)
			{
				n = va_arg(args, long);
				fn = __prtld;
			} else
			{
				n = va_arg(args, int);
				fn = __prtshort;
			}
			__prtint(n, buf, 16, 0, fn, c == 'X');
			break;

		case 's':						/* string */
		case 'S':
			s = va_arg(args, char *);
			break;

		case 'c':						/* character */
		case 'C':
			n = va_arg(args, int);
			buf[0] = n;
			buf[1] = '\0';
			break;

		case 'e':						/* exponential */
		case 'E':
			d = va_arg(args, double);
			petoa(d, buf, prec);
			prec = -1;
			break;

		case 'f':						/* floating */
		case 'F':
			d = va_arg(args, double);
			pftoa(d, buf, prec);
			prec = -1;
			break;

		case 'g':						/* e or f */
		case 'G':
			d = va_arg(args, double);
			pftoa(d, buf, prec);
			if (strlen(buf) > (7 + prec))	/* smallest fp string */
				petoa(d, buf, prec);
			prec = -1;
			break;

		default:						/* just print the character */
			__putch(stream, mode, c);
			total++;
			continue;
		}
		len = strlen(s);
		if (prec < len && prec >= 0)
			len = prec;
		n = width - len;
		if (!left)
		{
			if (padchar != ' ' && *s == '-')
			{
				len--;
				__putch(stream, mode, *s);
				total++;
				s++;
			}
			while (n-- > 0)
			{
				__putch(stream, mode, padchar);
				total++;
			}
		}
		while (len--)
		{
			__putch(stream, mode, *s);
			total++;
			s++;
		}
		while (n-- > 0)
		{
			__putch(stream, mode, padchar);
			total++;
		}
	}
	
	return total;
}


#ifdef __USE_STDARG
int fprintf(FILE *fp, const char *fmt, ...)
#else
int fprintf(P(FILE *, fp), P(const char *, fmt), va_alist)
PP(FILE *, fp;)
PP(const char *, fmt;)
va_dcl
#endif
{
	register int ret;
	va_list args;
	
	if (!(fp->_flag & _IOWRT))
		return -1;

	__c_va_start(args, fmt);
	ret = __doprint(fp, fmt, 0, args);
	__c_va_end(args);
	fflush(fp);
	return ret;
}


#ifdef __USE_STDARG
int sprintf(char *s, const char *fmt, ...)
#else
int sprintf(P(char *, s), P(const char *, fmt), va_alist)
PP(char *, s;)
PP(const char *, fmt;)
va_dcl
#endif
{
	register int ret;
	FILE *stream;
	va_list args;
	
	stream = (FILE *)&s;
	__c_va_start(args, fmt);
	ret = __doprint(stream, fmt, 1, args);
	__c_va_end(args);
	*s = '\0';
	return ret;
}
