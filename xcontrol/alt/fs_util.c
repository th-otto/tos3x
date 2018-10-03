#include "aesalt.h"
#include <tos.h>
#include <string.h>

UWORD TOS_version;
char boot_drive;
static char boot_path[129];

static long get_vars(void)
{
	long addr = 2;
	char c;
	addr += ((*(long *)0x4f2));
	TOS_version = *((UWORD *)addr);
	c = 'A' + *((char *)0x447);
	boot_drive = c;
	return addr;
}


void fsel_init(void)
{
	char *p;
	
	Supexec(get_vars);
	p = boot_path;
	*p++ = Dgetdrv() + 'A';
	*p++ = ':';
	Dgetpath(p, 0);
}


WORD fsel_name(BOOLEAN remember, char *name, const char *searchmask, const char *label)
{
	char *p;
	char namebuf[128];
	char pattern[14];
	WORD ret;
	int button;
	
	if (*name != '\0')
	{
		p = strrchr(name, '\\');
		if (p != NULL)
		{
			*p++ = '\0';
			strcpy(namebuf, name);
		} else
		{
			strcpy(namebuf, boot_path);
			p = namebuf;
		}
		strcpy(pattern, p);
		p = strchr(pattern, '.');
		if (p == NULL)
			p = ".*\0";
	} else
	{
		strcpy(namebuf, boot_path);
		pattern[0] = '\0';
		p = ".*\0";
	}
	strcat(namebuf, "\\");
	if (searchmask != NULL)
	{
		strcat(namebuf, searchmask);
	} else
	{
		strcat(namebuf, "*\0");
		strcat(namebuf, p);
	}
	if (label != NULL && TOS_version >= 0x104)
	{
		ret = fsel_exinput(namebuf, pattern, &button, label);
	} else
	{
		ret = fsel_input(namebuf, pattern, &button);
	}
	if (ret <= 0 || (ret = button) == 0)
	{
		*name = '\0';
	} else
	{
		p = strrchr(namebuf, '\\');
		*p = '\0';
		strcpy(name, namebuf);
		strcat(name, "\\");
		strcat(name, pattern);
		p = strrchr(name, '\\');
		label = strrchr(name, '.');
		if (label == NULL || label <= p)
		{
			label = strrchr(searchmask, '.');
			if (label != NULL)
				strcat(name, label);
		}
		if (remember)
			strcpy(boot_path, namebuf);
		if (pattern[0] == '\0')
			ret = 0;
	}
	return ret;
}
