

/* Error routine */
static VOID _err(s1, s2)
char *s1;								/* Message text         */
char *s2;								/* Filename         */
{
	write(STDERR, s1, strlen(s1));		/* Output error message     */
	write(STDERR, s2, strlen(s2));		/* And filename         */
	write(STDERR, "\n", 1);				/* + Newline            */
	exit(-1);							/* And fail hard        */
}

/*
 *	Addargv function -- adds a pointer to the argv array, getting the 
 *	space from the heap.
 */
static VOID addargv(ptr)
register char *ptr;							/* -> Argument string to add */
{
	register char **p;						/* Temp             */

	p = _break;							/* Load next spot       */
	if (brk(_break + 4) != 0)			/* Allocate a pointer       */
		_sovf();						/* Call "Stack Overflow" rtn */
	*p = ptr;							/* Load pointer         */
	argc++;								/* Increment arg count      */
}

/*
 *	Toasc routine -- combines the FCB name in the DMA and the user number
 *	/ drive field to produce an ascii file name for SEARCHes.
 *
 */
_toasc(p, c, buf)
register FD *p;								/* -> Data area          */
register char c;								/* 0 .. 3 search code        */
register char *buf;							/* Output buffer area        */
{
	register char *f;						/* -> Fcb in DMA buffer      */
	register int i;							/* Temp.             */

	f = &p->buffer[c * 32];				/* -> FCB drive code         */
	*buf = '\0';						/* Nullify at first      */
	if (p->user != __BDOS(USER, 0xffL))	/* User numbers different?   */
	{									/* Yes               */
		sprintf(buf, "%d:", p->user);	/*  convert user number      */
		i = TRUE;						/* remember          */
	} else								/* No user number        */
		i = FALSE;
	
	buf += strlen(buf);					/* buf -> null           */
	if (p->fcb.drive)					/* Drive specified?      */
	{									/* Yes               */
		if (i)							/* User #?           */
			buf--;						/* Yes, back up over ':'     */
		*buf++ = p->fcb.drive - 1 + 'a';	/* Put in drive code         */
		*buf++ = ':';					/* And delimiter         */
	}									/*               */
	
	for (i = 1; i < 9; i++)				/* Move the filename         */
	{
		if (f[i] != ' ')				/* Non-blank?            */
			*buf++ = tolower((f[i] & 0x7f));	/* Yes, move it in       */
	}
	
	*buf++ = '.';						/* Put in delimiter      */
	for (i = 9; i < 12; i++)			/* Move in extension         */
	{									/*               */
		if (f[i] != ' ')				/* Non-blank?            */
			*buf++ = tolower((f[i] & 0x7f));	/* Yes, move it in       */
	}
	*buf++ = '\0';						/* Null at end           */
}
