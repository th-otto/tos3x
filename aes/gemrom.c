/*
 *************************************************************************
 *			Revision Control System
 * =======================================================================
 *  $Author: mui $	$Date: 89/04/26 18:26:27 $
 * =======================================================================
 *
 * Revision 2.2  89/04/26  18:26:27  mui
 * TT
 * 
 * Revision 2.1  89/02/22  05:29:13  kbad
 * *** TOS 1.4  FINAL RELEASE VERSION ***
 * 
 * Revision 1.5  88/11/01  18:02:04  kbad
 * Revamped PRG define
 * 
 * Revision 1.4  88/10/03  12:08:52  kbad
 * New build obsoletes ROM/IMG/PRG changes.
 * 
 * Revision 1.3  88/09/22  04:49:08  kbad
 * Added "tosrsc" label of inline linked resource
 * 
 * Revision 1.2  88/09/08  18:59:25  kbad
 * added <flavor.h> for prg/img/rom flag
 * 
 * Revision 1.1  88/06/02  12:34:59  lozben
 * Initial revision
 * 
 *************************************************************************
 */
/*	NEW	GEMROM.C	01/11/88 - 1/19/88	Derek Mui	*/
/*	Look for TOS.RSC and read in GEM.RSC,DESKTOP.RSC,DESKTOP.INF	*/
/*	Make sure you set the switches right at machine.h		*/
/*	Change at rsc_read do_cdir	1/28/88		D.Mui		*/
/*	Change at rsc_read for TOS and ROM system 3/11/88		*/
/*	Incorporated all the resources together	7/14/92	D.Mui		*/
/*	Chnage the rsc table and time code format 7/16/92	D.Mui	*/

#include "aes.h"
#include "gemlib.h"
#include "taddr.h"

RSHDR *gemptr;		/* GEM's rsc pointer        */
RSHDR *deskptr;		/* DESKTOP'S rsc pointer    */
char *infptr;
uint16_t infsize;
uint16_t gemsize;
uint16_t desksize;
VOIDPTR gl_pglue;
BOOLEAN nodesk;
BOOLEAN nogem;
int16_t st_lang;		/* Language code    */
uint16_t st_time;		/* time code        */
uint16_t st_date;
uint16_t st_dchar;
int16_t st_keybd;

/*
 * points to the built-in resource data.
 * tosrsc[0] = offset to start of desktop resource header
 * tosrsc[1] = offset to start of desktop.inf
 * tosrsc[2] = total size of data
 * tosrsc[3] reserved
 * tosrsc[4] reserved
 * The gem resource header follows those fields
 */
static const uint16_t *tosrsc;

extern uint16_t const USARSC[];			/* USA, UK  */
extern uint16_t const GRMRSC[];			/* GERMAN   */
extern uint16_t const FRERSC[];			/* FRENCH   */
extern uint16_t const ITARSC[];			/* ITALIAN  */
#if 0
extern uint16_t const SWERSC[];			/* SWEDISH  */
#endif
extern uint16_t const SPARSC[];				/* SPANISH  */

static char const TIMETABLE[] = {		/* Eurotime, Eurodate and separator */
	0, 0, '/',							/* USA  */
	1, 1, '.',							/* GERMANY  */
	1, 1, '/',							/* FRENCH   */
	0, 0, '/',							/* USA  */
	1, 1, '/',							/* SPAIN    */
	1, 1, '/',							/* ITALY    */
	1, 2, '-'							/* SWEDEN,NORWAY,FINLAND */
};

static const uint16_t *const RSCTABLE[] = {
	USARSC,								/* USA, UK  */
	GRMRSC,								/* GERMAN   */
	FRERSC,								/* FRENCH   */
	USARSC,								/* reserved */
	SPARSC,								/* SPANISH  */
	ITARSC,								/* ITALIAN  */
	USARSC								/* suppose to be SWEDISH    */
};


/* do this whenever the Gem or desktop is ready */
int16_t rom_ram(P(int) which, P(intptr_t) pointer)
PP(int which;)
PP(register intptr_t pointer;)
{
	int16_t size;
	register BOOLEAN doit;

	if (which == 3)						/* read in desktop.inf      */
	{
		LBCOPY((VOIDPTR)pointer, infptr, infsize);
		return infsize;
	}

	rs_global = pointer;
	doit = TRUE;

	if (!which)
	{
		/* read in gem rsc */
		if (nogem)
			doit = FALSE;
		else
			nogem = TRUE;				/* already read in      */

		rs_hdr = gemptr;
		size = gemsize;
	} else
	{
		/* read in desktop rsc */
		if (nodesk)
			doit = FALSE;
		else
			nodesk = TRUE;

		rs_hdr = deskptr;
		size = desksize;
	}


	LLSET(pointer + 14, rs_hdr);
	LWSET(pointer + 18, size);

	if (doit)
	{
		do_rsfix((intptr_t)rs_hdr, size);
		rs_fixit(pointer);
	}
#if !BINEXACT
	/* BUG: no return here */
	return size;
#endif
}


VOID rsc_free(NOTHING)
{
	dos_free(gl_pglue);
	gemptr = NULL;
	deskptr = NULL;
	/* infptr should also be nullified, just in case... */
}


/*	Read in the resource file	*/

BOOLEAN rsc_read(NOTHING)
{
	uint16_t size;
	register const uint16_t *intptr;
	const char *a;
	char *b;
	int32_t value;
	int16_t code;

	/* The value is defined as  */
	/* X, X, LANGUAGE, KEYVBOARD    */

	if (getcookie(0x5F414B50L, &value))	/* get _AKP cookie */
	{
		st_keybd = value & 0x00FFL;
		st_lang = (value >> 8) & 0x00FFL;	/* get the keyboard preferences */
	} else
	{
		st_keybd = st_lang = 0;
	}
	
	if ((st_keybd > 8) || (st_keybd < 0))
		st_keybd = 0;

	if ((st_lang > 6) || (st_lang < 0) || (st_lang == 3))
		st_lang = 0;

	code = st_lang * 3;

	st_time = TIMETABLE[code++];
	st_date = TIMETABLE[code++];
	st_dchar = TIMETABLE[code];

    /* The IDT format is as follow                          */
    /* HIGH WORD  |          LOW WORD                       */
    /* 31 - 16    | 15-12      11-8           7-0 bit       */
    /* Reserved   | st_time    st_date        st_char       */
    /*              0 12 hour  0  MM-DD-YY                  */
    /*              1 24 hour  1  DD-MM-YY                  */      
    /*                         2  YY-MM-DD                  */
    /*                         3  YY-DD-MM                  */

	if (getcookie(0x5F494454L, &value))	/* get _IDT cookie */
	{
		st_date = (value >> 8) & 0xFL;	/* get the date preferences */
		st_time = (value >> 12) & 0xFL;
		if (!(st_dchar = value & 0xffL))
			st_dchar = '/';
	}

	tosrsc = RSCTABLE[st_lang];

	if (!(gl_pglue = dos_alloc((int32_t) tosrsc[2])))
	{
		trap(9, "Unable to install AES resource!\r\n");
		return FALSE;
	}
	/* copy rsc to ram */
	intptr = tosrsc;

	size = intptr[2];
	a = (const char *)tosrsc;
	b = (char *)gl_pglue;

	while (size)
	{
		*b++ = *a++;
		size--;
	}

	intptr = (const uint16_t *)gl_pglue;
	gemptr = (RSHDR *) ((char *)gl_pglue + 10);	/* now fix the resource   */
	deskptr = (RSHDR *) ((char *)gl_pglue + intptr[0]);
	infptr = (char *) ((char *)gl_pglue + intptr[1]);
	gemsize = intptr[0];    /* BUG: that includes the size of the GLUE header */
	desksize = intptr[1] - intptr[0];
	infsize = intptr[2] - intptr[1];
	nodesk = FALSE;
	nogem = FALSE;
#if !BINEXACT
	/* BUG: no return here */
	return TRUE;
#endif
}
