#include "tos.h"
#include <ostruct.h>
#include <toserrno.h>
#include "fs.h"

/* 306: 00e17306 */
VOID xmovs(P(int) n, P(const VOIDPTR) psrc, P(VOIDPTR) pdst)
PP(register int n;)
PP(register const VOIDPTR psrc;)
PP(register VOIDPTR pdst;)
{
	register const char *src = psrc;
	register char *dst = pdst;
	while (n--)
		*dst++ = *src++;
}


/*
 *  xcmps - utility routine to compare two 11-character strings
 *
 *	Last modified	19 Jul 85	SCC
 */

/* 306: 00e17330 */
int xcmps(P(const char *) s, P(const char *) d)
PP(register const char *s;)
PP(register const char *d;)
{
	register int i;

	for (i = 0; i < 11; i++)
		if (uc(*s++) != uc(*d++))
			return 0;
	return 1;
}


/*
 *  match - utility routine to compare file names
 */

/* 306: 00e1737c */
BOOLEAN match(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)							/*  name we are checking        */
PP(register const char *s2;)									/*  name in fcb             */
{
	register int i;

	/*
	 *  check for deleted entry.  wild cards don't match deleted entries,
	 *  only specific requests for deleted entries do.
	 */

	if (*s2 == 0xe5)
	{
		if (*s1 == '?')
			return FALSE;
		else if (*s1 == 0xe5)
			return TRUE;
	}

	/*
	 *  compare names
	 */

	for (i = 0; i < 11; i++, s1++, s2++)
		if (*s1 != '?')
			if (uc(*s1) != uc(*s2))
				return FALSE;

	/*
	 *  check attribute match
	 * volume labels and subdirs must be specifically asked for
	 */

	if ((*s1 != FA_LABEL) && (*s1 != FA_DIREC))
		if (!(*s2))
			return TRUE;

	return *s1 & *s2 ? TRUE : FALSE;
}


#ifdef	NEWCODE
#define	isnotdelim(x)	((x) && (x!='*') && (x!=SLASH) && (x!='.') && (x!=' '))

#define	MAXFNCHARS	8


/*
 *  builds - build a directory style file spec from a portion of a path name
 *	the string at 's1' is expected to be a path spec in the form of
 *	(xxx/yyy/zzz).  *builds* will take the string and crack it
 *	into the form 'ffffffffeee' where 'ffffffff' is a non-terminated
 *	string of characters, padded on the right, specifying the filename
 *	portion of the file spec.  (The file spec terminates with the first
 *	occurrence of a SLASH or EOS, the filename portion of the file spec
 *	terminates with SLASH, EOS, PERIOD or WILDCARD-CHAR).  'eee' is the
 *	file extension portion of the file spec, and is terminated with
 *	any of the above.  The file extension portion is left justified into
 *	the last three characters of the destination (11 char) buffer, but is
 *	padded on the right.  The padding character depends on whether or not
 *	the filename or file extension was terminated with a separator
 *	(EOS, SLASH, PERIOD) or a WILDCARD-CHAR.
 *
 */

/* 306: 00e17424 */
VOID builds(P(const char *) s1, P(char *) s2)
PP(register char *s1;)							/*  source          */
PP(register char *s2;)									/*  s2 dest         */
{
	register int i;
	char c;

	/*
	 * copy filename part of pathname to destination buffer until a
	 * delimiter is found
	 */

	for (i = 0; (i < MAXFNCHARS) && isnotdelim(*s1); i++)
		*s2++ = uc(*s1++);

	/*
	 *  if we have reached the max number of characters for the filename
	 * part, skip the rest until we reach a delimiter
	 */

	if (i == MAXFNCHARS)
		while (*s1 && (*s1 != '.') && (*s1 != SLASH))
			s1++;

	/*
	 *  if the current character is a wildcard character, set the padding
	 * char with a "?" (wildcard), otherwise replace it with a space
	 */

	c = (*s1 == '*') ? '?' : ' ';


	if (*s1 == '*')						/*  skip over wildcard char */
		s1++;

	if (*s1 == '.')						/*  skip over extension delim   */
		s1++;

	/*
	 *  now that we've parsed out the filename part, pad out the
	 * destination with "?" wildcard chars
	 */

	for (; i < MAXFNCHARS; i++)
		*s2++ = c;

	/*
	 *  copy extension part of file spec up to max number of characters
	 * or until we find a delimiter
	 */

	for (i = 0; i < 3 && isnotdelim(*s1); i++)
		*s2++ = uc(*s1++);

	/*
	 *  if the current character is a wildcard character, set the padding
	 * char with a "?" (wildcard), otherwise replace it with a space
	 */

	c = ((*s1 == '*') ? '?' : ' ');

	/*
	 *  pad out the file extension
	 */

	for (; i < 3; i++)
		*s2++ = c;
}

#else

/*
 *  builds -
 *
 *	Last modified	LTG	23 Jul 85
 */

VOID builds(P(const char *) s1, P(char *) s2)
PP(register char *s1;)							/*  source          */
PP(register char *s2;)									/*  s2 dest         */
{
	int i;
	char c;

	for (i = 0; (i < 8) && (*s1) && (*s1 != '*') && (*s1 != SLASH) && (*s1 != '.') && (*s1 != ' '); i++)
		*s2++ = uc(*s1++);

	if (i == 8)
		while (*s1 && (*s1 != '.') && (*s1 != SLASH))
			s1++;

	c = ((*s1 == '*') ? '?' : ' ');

	if (*s1 == '*')
		s1++;

	if (*s1 == '.')
		s1++;

	for (; i < 8; i++)
		*s2++ = c;

	for (i = 0; (i < 3) && (*s1) && (*s1 != '*') && (*s1 != SLASH) && (*s1 != '.') && (*s1 != ' '); i++)
		*s2++ = uc(*s1++);

	c = ((*s1 == '*') ? '?' : ' ');

	for (; i < 3; i++)
		*s2++ = c;
}

#endif


/*
 *  getpath - get a path element
 *	The buffer pointed to by 'd' must be at least the size of the file
 *	spec buffer in a directory entry (including file type), and will
 *	be filled with the directory style format of the path element if
 *	no error has occurred.
 *
 *  returns
 *	-1 if '.'
 *	-2 if '..'
 *	 0 if p => name of a file (no trailing SLASH or !dirspec)
 *	>0 (nbr of chars in path element (up to SLASH)) && buffer 'd' filled.
 *
 */

/* 306: 00e17508 */
int getpath(P(const char *) p, P(char *) d, P(int) dirspec)
PP(const char *p;)								/*  start of path element to crack  */
PP(char *d;)									/*  ptr to destination buffer       */
PP(int dirspec;)							/*  true = no file name, just dir path  */
{
	register int i, i2;
	register const char *p1;

	for (i = 0, p1 = p; *p1 && (*p1 != SLASH); p1++, i++)
		;

	/*
	 *  If the string we have just scanned over is a directory name, it
	 * will either be terminated by a SLASH, or 'dirspec' will be set
	 * indicating that we are dealing with a directory path only
	 * (no file name at the end).
	 */

	if (*p1 != '\0' || dirspec)
	{									/*  directory name  */
		i2 = 0;
		if (p[0] == '.')				/*  dots in name    */
		{
			i2--;						/*  -1 for dot      */
			if (p[1] == '.')
				i2--;					/*  -2 for dotdot   */
			return i2;
		}

		if (i)							/*  if not null path el */
			builds(p, d);				/*  d => dir style fn   */

		return i;						/*  return nbr chars    */
	}

	return 0;							/*  if string is a file name        */
}


/*
 *  dopath -
 *
 */

/* 306: 00e1761a */
char *dopath(P(DND *) p, P(char *) buf)
PP(DND *p;)
PP(char *buf;)
{
	char temp[14];
	char *tp;
	long tlen;
	int len;
	
	if (p->d_parent)
		buf = dopath(p->d_parent, buf);

	tlen = (long) packit(p->d_name, temp) - (long) temp;
	tp = temp;
	while (len)
	{
		len--;						/* len must never go < 0 */
		if (tlen--)
			*buf++ = *tp++;
		else
		{
			*buf++ = SLASH;
			break;
		}
	}
	return buf;
}


/* 306: 00e17662 */
int contains_dots(P(const char *) name, P(char) ill)
PP(register const char *name;)
PP(char ill;)
{
	register int i;
	
	if (*name == '\0')
		return 1;
	for (i = 0; --i > -3; )
	{
		if (*name++ != '.')
			break;
		if (*name == ill)
			return i;
	}
	return 0;
}


/*
 *  match1 - check for bad chars in path name
 *	check thru test string to see if any character in the ref str is found
 *	(utility routine for ixcreat())
 *	by scc
 */

BOOLEAN match1(P(const char *) ref, P(const char *) test)
PP(register const char *ref;)
PP(const char *test;)
{
	register const char *t;

	while (*ref)
	{
		for (t = test; *t; t++)
			if (*t == *ref)
				return TRUE;
		ref++;
	}

	return FALSE;
}
