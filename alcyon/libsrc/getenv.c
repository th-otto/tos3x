#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


char *getenv(P(const char *) tag)
PP(register const char *tag;)
{
	char **var;
	char *name;
	size_t len;

	if (environ && tag && *tag)
	{
		len = strlen(tag);

		for (var = environ; (name = *var) != NULL; var++)
		{
			if (strncmp(name, tag, len) == 0 && name[len] == '=')
				return name + len + 1;
		}
	}
	return NULL;
}
