/* strtol - convert string representation of a number into a long integer value. */

/* Copyright (C) 1991,92,94,95,96,97,98,99,2000,2001 Free Software Foundation, Inc.

   This file is part of GNU Bash, the Bourne Again SHell.

   Bash is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Bash is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Bash.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "lib.h"
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define ulong long

/* Nonzero if we are defining `strtoul' or `strtoull', operating on
   unsigned integers.  */
#ifndef UNSIGNED
#  define UNSIGNED 0
#  define INT long
#else
#  define INT ulong
#endif

#if UNSIGNED
#  define strtol strtoul
#endif

#define STR_LMIN LONG_MIN
#define STR_LMAX LONG_MAX
#define STR_ULMAX ULONG_MAX

/* Convert NPTR to an `unsigned long int' or `long int' in base BASE.
   If BASE is 0 the base is determined by the presence of a leading
   zero, indicating octal or a leading "0x" or "0X", indicating hexadecimal.
   If BASE is < 2 or > 36, it is no longer reset to 10; EINVAL is returned.
   If ENDPTR is not NULL, a pointer to the character after the last
   one converted is stored in *ENDPTR.  */

INT strtol(P(const char *) nptr, P(char **) endptr, P(int) base)
PP(const char *nptr;)
PP(char **endptr;)
PP(int base;)
{
	int negative;
	register ulong cutoff;
	register unsigned int cutlim;
	register ulong i;
	register const char *s;
	register unsigned int c;
	const char *save;
	int overflow;

	if (base < 0 || base == 1 || base > 36)
	{
		__set_errno(EINVAL);
		return 0;
	}

	save = s = nptr;

	/* Skip white space.  */
	while (isspace((__uint8_t)*s))
		++s;
	if (*s == '\0')
		goto noconv;

	/* Check for a sign.  */
	if (*s == '-' || *s == '+')
	{
		negative = (*s == '-');
		++s;
	} else
	{
		negative = 0;
	}
	
	/* Recognize number prefix and if BASE is zero, figure it out ourselves.  */
	if (*s == '0')
	{
		if ((base == 0 || base == 16) && toupper((__uint8_t)s[1]) == 'X')
		{
			s += 2;
			base = 16;
		} else if (base == 0)
		{
			base = 8;
		}
	} else if (base == 0)
	{
		base = 10;
	}
	
	/* Save the pointer so we can check later if anything happened.  */
	save = s;

	cutoff = STR_LMAX / base;
	cutlim = STR_LMAX % base;

	overflow = 0;
	i = 0;
	c = *s & 0xff;
	for (; c != '\0'; )
	{
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c = toupper(c) - 'A' + 10;
		else
			break;
		if ((int) c >= base)
			break;
		/* Check for overflow.  */
		if (i > cutoff || (i == cutoff && c > cutlim))
		{
			overflow = 1;
		} else
		{
			i *= (ulong) base;
			i += c;
		}
		++s;
		c = *s & 0xff;
	}

	/* Check if anything actually happened.  */
	if (s == save)
		goto noconv;

	/* Store in ENDPTR the address of one character
	   past the last character we converted.  */
	if (endptr != NULL)
		*endptr = (char *)NO_CONST(s);

#if !UNSIGNED
	/* Check for a value that is within the range of
	   `unsigned long int', but outside the range of `long int'.  */
	if (overflow == 0
		&& i > (negative ? -((ulong) (STR_LMIN + 1)) + 1 : (ulong) STR_LMAX))
		overflow = 1;
#endif

	if (overflow)
	{
		__set_errno(ERANGE);
#if UNSIGNED
		return STR_ULMAX;
#else
		return negative ? STR_LMIN : STR_LMAX;
#endif
	}

	/* Return the result of the appropriate sign.  */
	return negative ? -i : i;

  noconv:
	/* We must handle a special case here: the base is 0 or 16 and the
	   first two characters are '0' and 'x', but the rest are no
	   hexadecimal digits.  This is no error case.  We return 0 and
	   ENDPTR points to the `x`.  */
	if (endptr != NULL)
	{
		if ((long)save - (long)nptr >= 2 && toupper((__uint8_t)save[-1]) == 'X' && save[-2] == '0')
			*endptr = (char *) NO_CONST(&save[-1]);
		else
			/*  There was no number to convert.  */
			*endptr = (char *) NO_CONST(nptr);
	}

	return 0;
}
