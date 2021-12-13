#include "as68.h"

#define AES 0
#define DESKTOP 1

struct aestab {
	unsigned short opcode;
	char name[SYNAMLEN + 1];
	short from;
	short unused;
};

static int numfuncs;

/* actually there are 604 functions defined */
#define MAXFUNCS 800

static struct aestab aesfuncs[MAXFUNCS];


VOID readlineftab(NOTHING)
{
	register FILE *fp;
	register int ch;
	register int i;
	register unsigned short opcode;
	
	fp = openfi(lineftbl, "r");
	ch = fgetc(fp);
	while (ch != EOF)
	{
		if (ch != '#')
		{
			opcode = 0;
			while (ch != ' ' && ch != EOF && ch != 0x0a)
			{
				opcode <<= 4;
				if (ch >= 'A' && ch <= 'F')
					opcode += ch - 'A' + 10;
				else if (ch >= 'a' && ch <= 'f')
					opcode += ch - 'a' + 10;
				else
					opcode += ch - '0';
				ch = fgetc(fp);
			}
			if (numfuncs >= MAXFUNCS)
			{
				rpterr(_("too many LINEF functions"));
				endit();
			}
			while (ch == ' ')
				ch = fgetc(fp);	
			i = 0;
			for (;;)
			{
				if (ch == 0x0d)
					ch = fgetc(fp);
				if (ch == EOF || ch == 0x0a || ch == ' ')
					break;
				if (i < SYNAMLEN)
				{
					aesfuncs[numfuncs].name[i] = ch;
				}
				i++;
				ch = fgetc(fp);
			}
			aesfuncs[numfuncs].name[i] = '\0';
			aesfuncs[numfuncs].opcode = opcode;
			aesfuncs[numfuncs].unused = FALSE;
			if (ch == ' ')
			{
				while (ch == ' ')
					ch = fgetc(fp);	
				if (ch == '1')
					aesfuncs[numfuncs].unused = TRUE;
			}
			numfuncs++;
		}
		while (ch != EOF && ch != 0x0a)
			ch = fgetc(fp);	
		if (ch != EOF)
			ch = fgetc(fp);
	}
	fclose(fp);
}


unsigned short isaes(P(const char *) name)
PP(register const char *name;)
{
	register struct aestab *tab;
	register struct aestab *end;
	register int len;
	
	if (*name != '_')
		return FALSE;
	name++;
	
	len = SYNAMLEN - 1;
	end = &aesfuncs[numfuncs];
	for (tab = aesfuncs; tab < end; tab++)
		if (!tab->unused && nameeq(tab->name, name, len))
			return tab->opcode;
	return 0;
}
