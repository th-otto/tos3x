#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "fext.h"


FILE *efopen(P(const char *)filename, P(const char *) mode)
PP(const char *filename;)
PP(const char *mode;)
{
	FILE *fp;
	
	if ((fp = fopen(filename, mode)) == NULL)
	{
		fprintf(stderr, "Cannot open '%s'\n", filename);
		exit(EXIT_FAILURE);
	}
	return fp;
}


char *fext(P(char *) filename, P(const char *) ext, P(int) flag)
PP(char *filename;)
PP(const char *ext;)
PP(int flag;)
{
	char *pext;
	char *p;
	
	for (p = filename; *p != '\0'; p++)
		;
	for (; p > filename && p[-1] != '\\'; )
		--p;
	if (flag)
	{
		for (pext = p; *pext != '\0' && *pext != '.'; pext++)
			;
		*pext = '\0';
	}
	for (pext = p; *pext != '.'; pext++)
	{
		if (*pext == '\0')
		{
			strcat(p, ext);
			break;
		}
	}
	return filename;
}
