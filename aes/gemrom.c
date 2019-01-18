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
#include "ctrycode.h"

RSHDR *gemptr;		/* GEM's rsc pointer        */
RSHDR *deskptr;		/* DESKTOP'S rsc pointer    */
char *infptr;
uint16_t infsize;
uint16_t gemsize;
uint16_t desksize;
VOIDPTR gl_pglue;
BOOLEAN nodesk;		/* desk.rsc already read in ? */
BOOLEAN nogem;		/* gem.rsc already read in ? */

#if (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_FR) | (OS_COUNTRY == CTRY_IT) | (OS_COUNTRY == CTRY_ES) | (OS_COUNTRY == CTRY_SG) | (OS_COUNTRY == CTRY_SF) | (OS_COUNTRY == CTRY_MX) | (OS_COUNTRY == CTRY_TR) | (OS_COUNTRY == CTRY_DK)
#define EUROTIME 1		/*			European Style       */
#define EURODATE 1		/*			Date/TimeFlag	     */
#endif

#if (OS_COUNTRY == CTRY_UK)
#define EUROTIME 0
#define EURODATE 1
#endif

#if (OS_COUNTRY == CTRY_US)
#define EUROTIME 0
#define EURODATE 0
#endif

#if (OS_COUNTRY == CTRY_SE) | (OS_COUNTRY == CTRY_NO) | (OS_COUNTRY == CTRY_FI)
#define EUROTIME 1
#define SWEDDATE 1
#endif

#if MULTILANG_SUPPORT

/*
 * these are referenced by DESKTOP.APP
 */
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

extern uint16_t const RSCUS[];			/* USA, UK  */
extern uint16_t const RSCDE[];			/* GERMAN   */
extern uint16_t const RSCFR[];			/* FRENCH   */
extern uint16_t const RSCES[];			/* SPANISH  */
extern uint16_t const RSCIT[];			/* ITALIAN  */
#define RSCSV RSCUS
extern uint16_t const RSCSV[];			/* SWEDISH  */
#define RSCSF RSCFR
extern uint16_t const RSCSF[];			/* Switzerland (French)  */
#define RSCSG RSCDE
extern uint16_t const RSCSG[];			/* Switzerland (German)  */
#define RSCTR RSCUS
extern uint16_t const RSCTR[];			/* Turkish  */
#define RSCFI RSCSV
extern uint16_t const RSCFI[];			/* Finnish */
#define RSCNO RSCSV
extern uint16_t const RSCNO[];			/* Norwegian  */

static char const TIMETABLE[] = {		/* Eurotime, Eurodate and separator */
	0, 0, '/',							/* USA */
	1, 1, '.',							/* Germany */
	1, 1, '/',							/* France */
	0, 0, '/',							/* UK */
	1, 1, '/',							/* Spain */
	1, 1, '/',							/* Italy */
	1, 2, '-',							/* Sweden */
#if !BINEXACT
	1, 1, '/',							/* Switzerland (French)  */
	1, 1, '.',							/* Switzerland (German)  */
	1, 1, '.',							/* Turkey  */
	1, 2, '-',							/* Finland */
	1, 2, '-',							/* Norway */
#endif
};

static const uint16_t *const RSCTABLE[] = {
	RSCUS,								/* USA */
	RSCDE,								/* German */
	RSCFR,								/* French */
	RSCUS,								/* UK */
	RSCES,								/* Spanish */
	RSCIT,								/* Italian */
	RSCUS,								/* supposed to be SWEDISH    */
#if !BINEXACT
	RSCFR,								/* Switzerland (French)  */
	RSCDE,								/* Switzerland (German)  */
	RSCTR,								/* Turkish */
	RSCFI,								/* Finnish */
	RSCNO,								/* Norwegian */
#endif
};

#else

extern uint16_t const tosrsc[];

#endif

#define Cconws(a) trap(9, a)
#define Bconstat(a) (int)trp13(1, a)


/*
 * do this whenever the Gem or desktop is ready
 */
/* 306de: 00e20244 */
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


/* 306de: 00e20316 */
VOID rsc_free(NOTHING)
{
	dos_free(gl_pglue);
	gemptr = NULL;
	deskptr = NULL;
	/* infptr should also be nullified, just in case... */
}


/*
 * Read in the resource file
 */
/* 206de: 00e1c8d0 */
/* 306de: 00e20336 */
BOOLEAN rsc_read(NOTHING)
{
	register const uint16_t *intptr;
	uint16_t size;
	const char *a;
	char *b;
	int32_t value;
	int16_t code;
#if MULTILANG_SUPPORT

	/* The value is defined as X, X, LANGUAGE, KEYBOARD */

	if (getcookie(0x5F414B50L, &value))	/* get _AKP cookie */
	{
		st_keybd = value & 0x00FFL;
		st_lang = (value >> 8) & 0x00FFL;	/* get the keyboard preferences */
	} else
	{
		st_keybd = st_lang = CTRY_US;
	}
	
	if (st_keybd > 8 || st_keybd < 0)
		st_keybd = CTRY_US;

#if BINEXACT
	/*
	 * BUG: that will use US resources for CTRY_SF(7) & CTRY_SG(8)
	 * It will also set language to US when it should be UK
	 */
	if (st_lang > (sizeof(RSCTABLE) / sizeof(RSCTABLE[0]) - 1) || st_lang < 0 || st_lang == CTRY_UK)
		st_lang = CTRY_US;
#else
	if (st_lang >= (sizeof(RSCTABLE) / sizeof(RSCTABLE[0])) || st_lang < 0)
		st_lang = CTRY_US;
#endif

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
		Cconws("Unable to install AES resource!\r\n");
		return FALSE;
	}

	/* copy rsc to ram */
	intptr = tosrsc;

	/* why inline LBCOPY here??? */
	size = intptr[2];
	a = (const char *)tosrsc;
	b = (char *)gl_pglue;

	while (size)
	{
		*b++ = *a++;
		size--;
	}

#else
	short unused;
	
	/* copy rsc to ram */
	intptr = tosrsc;

	gl_pglue = dos_alloc((int32_t) intptr[2]);
	if (!gl_pglue)
	{
		Cconws("Unable to install resource !");
		while (!Bconstat(2))
			;
#if BINEXACT
		/* BUG: no return value here */
		return;
#else
		return FALSE;
#endif
	}

	LBCOPY(gl_pglue, tosrsc, intptr[2]);

	UNUSED(size);
	UNUSED(a);
	UNUSED(b);
	UNUSED(value);
	UNUSED(code);
	UNUSED(unused);
	
#endif

	intptr = (const uint16_t *)gl_pglue;
	gemptr = (RSHDR *) ((char *)gl_pglue + 10);	/* now fix the resource   */
	deskptr = (RSHDR *) ((char *)gl_pglue + (intptr_t)intptr[0]);
	infptr = (char *) ((char *)gl_pglue + (intptr_t)intptr[1]);
	gemsize = intptr[0];    /* BUG: that includes the size of the GLUE header */
	desksize = intptr[1] - intptr[0];
	infsize = intptr[2] - intptr[1];

	nodesk = FALSE;
	nogem = FALSE;

#if !BINEXACT
	/* BUG: no return value here */
	return TRUE;
#endif
}
