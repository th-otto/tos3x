/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

*/

#include "preproc.h"

char *strcpy(P(char *) s, P(const char *)t)
PP(register char *s;)
PP(register const char *t;)
{
	register char *s1;

	s1 = s;
	for (;;)
	{
		if ((*s++ = *t++) == '\0')
			break;
	}
#ifdef __ALCYON__
	asm("move.l a3,d0");
	asm("nop");
#else
	return s1;
#endif
}


int strcmp(P(const char *) s, P(const char *) t)
PP(register const char *s;)
PP(register const char *t;)
{
	for (;;)
	{
		if (*s == '\0')
			break;
		if (*s++ != *t++)
		{
			s--;
			t--;
			break;
		}
	}
#ifdef __ALCYON__
	asm("move.b    (a5),d0");
	asm("ext.w     d0");
	asm("move.b    (a4),d1");
	asm("ext.w     d1");
	asm("sub.w     d1,d0");
	asm("nop");
#else
	return *s - *t;
#endif
}
