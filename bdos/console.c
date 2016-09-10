/*	con.c
 * 
 * GEMDOS console system
 * 
 * Originally written by JSL as noted below.
 * 
 * MODIFICATION HISTORY
 * 
 * 	10 Mar 85	SCC	Added xauxout(), xprtout(), xauxin().
 * 				(getch() can be used to perform function 7.)
 * 				Updated rawconio() to spec (no 0xFE).
 * 				Register optimization.
 * 
 * 	11 Mar 85	SCC	Further register optimization.
 * 				Added xconostat(), xprtostat(), xauxistat(), &
 * 				xauxostat().
 * 	12 Mar 85	SCC	Fixed xauxin().
 * 	14 Mar 85	SCC	Extended path from BIOS character input
 * 				through to returning the character to the
 * 				user to be long.
 * 				OOPS!  Repaired a '=' to a '==' in rawconio().
 * 	19 Mar 85	SCC	Modified tests in conbrk() to just check low
 * 				byte of character (ignoring scan code info).
 * 	21 Mar 85	SCC	Modified conin()'s echo of long input
 * 				character to int.
 * 				Added definition of 'h' to conout parameters.
 * 	25 Mar 85	SCC	Changed constat return to -1 (to spec).
 * 				Modified xauxistat() to use constat() rather
 * 				than bconstat() directly.
 * 	 1 Apr 85	SCC	Added x7in().  getch() cannot be used directly
 * 				for function 0x07.
 * 	10 Apr 85	EWF	Installed circular buffer for typeahead.
 * 	11 Apr 85	EWF	Modified ^R handling in cgets().
 * 	12 Apr 85	EWF	Installed 'ring bell' on typeahead full.
 * 			SCC	Installed EWF's changes of 10, 11 & 12 Apr 85.
 * 	26 Apr 85	SCC	Modified ^X & ^C handling to flush BIOS buffer
 * 				as well as BDOS buffer.
 * 	29 Apr 85	SCC	Modified buffer flushing to flush just BDOS
 * 				buffer, but to re-insert ^X into buffer.
 * 
 * 	11 Aug 85	SCC	Modified references that convert p_uft[] entry
 * 				to BIOS handle to use HXFORM() macro.
 * 				Added 'extern int bios_dev[]' for HXFORM().
 * 
 * 	16 Aug 85	SCC	Modified xtabout().  The character parameter
 * 				was omitted from the the call to tabout()
 * 				inadvertently in the course of the previous
 * 				changes.
 * 
 * 				Modified xconin().  It was not checking for
 * 				possible break.
 * 
 * 				Modified x8in().  It was checking for break
 * 				after getting a character, rather than before.
 * 
 * 	18 Aug 85	SCC	Modified constat() to return input status
 * 				for all new character devices.
 * 
 * 				Modified all references of p_uft[3] (for PRN:)
 * 				to p_uft[4], because of insertion of stderr.
 * 
 * 				Modified all references of p_uft[2] (for AUX:)
 * 				to p_uft[3], because of insertion of stderr.
 * 
 *	15 Oct 85	KTB	M01.01.01: code to accomodate split of fs.h
 *				into fs.h and bdos.h
 *
 *	21 Oct 85	KTB	M01.01.02: included portab.h
 * 	
 * NAMES
 * 
 * 	JSL	Jason S. Loveman
 * 	SCC	Steve C. Cavender
 * 	EWF	Eric W. Fleischman
 *	KTB	Karl T. Braun (kral)
 */

/* console system for GEMDOS 3/6/85 JSL */

#include	"tos.h"
#include 	"fs.h"
#include	"bios.h"

/* *************************** typeahead buffer ************************* */
/* The following data structures are used for the typeahead buffer:	  */

long glbkbchar[3][KBBUFSZ];				/* The actual typeahead buffer    */

					/* The 3 elements are prn,aux,con */
int add[3];								/*  index of add position     */

int remove[3];							/*  index of remove position      */


VOID conbrk PROTO((FH h));
VOID buflush PROTO((FH h));
VOID conout PROTO((FH h, int ch));
VOID cookdout PROTO((FH h, int ch));
int32_t getch PROTO((FH h));
VOID prt_line PROTO((FH h, const char *p));
VOID newline PROTO((FH h, int startcol));
int backsp PROTO((FH h, const char *cbuf, int retlen, int col));


int glbcolumn[3];

#define UBWORD(x) (((int) x) & 0x00ff)

#define   ctrlc  0x03
#define   ctrle  0x05
#define   ctrlq  0x11
#define   ctrlr  0x12
#define   ctrls  0x13
#define   ctrlu  0x15
#define   ctrlx  0x18

#define   cr      0x0d
#define   lf      0x0a
#define   tab     0x09
#define   rub     0x7f
#define   bs      0x08
#define   space   0x20

#define warmboot() xterm(-32)

/*****************************************************************************
**
** constat -
**
******************************************************************************
 */

int32_t constat(P(FH) h)
PP(FH h;)
{
	if (h > BFHCON)
		return 0;

	return add[h] > remove[h] ? -1L : bconstat(h);
}

/*****************************************************************************
**
** xconstat - 
**	Function 0x0B - Cconis - Console input status
**
**	Last modified	SCC	11 Aug 85
**
******************************************************************************
 */

/* 306: 00e138e8 */
int32_t xconstat(NOTHING)
{
	return constat(HXFORM(run->p_uft[0]));
}

/*****************************************************************************
**
** xconostat -
**	Function 0x10 - Cconos - console output status
**
**	Last Modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13902 */
int32_t xconostat(NOTHING)
{
	return bconostat(HXFORM(run->p_uft[1]));
}

/*****************************************************************************
**
** xprtostat -
**	Function 0x11 - Cprnos - Printer output status
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13926 */
int32_t xprtostat(NOTHING)
{
	return bconostat(HXFORM(run->p_uft[4]));
}

/*****************************************************************************
**
** xauxistat -
**	Function 0x12 - Cauxis - Auxiliary input status
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e1394a */
int32_t xauxistat(NOTHING)
{
	return constat(HXFORM(run->p_uft[3]));
}

/*****************************************************************************
**
** xauxostat -
**	Function 0x13 - Cauxos - Auxiliary output status
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13966 */
int32_t xauxostat(NOTHING)
{
	return bconostat(HXFORM(run->p_uft[3]));
}


/********************/
/* check for ctrl/s */
/* used internally  */
/********************/
VOID conbrk(P(FH) h)
PP(FH h;)
{
	register long ch;
	register int stop, c;

	stop = 0;
	if (bconstat(h))
	{
		do
		{
			c = (ch = bconin(h)) & 0xFF;
			if (c == ctrlc)
			{
				buflush(h);				/* flush BDOS & BIOS buffers */
				warmboot();
				return;
			}

			if (c == ctrls)
				stop = 1;
			else if (c == ctrlq)
				stop = 0;
			else if (c == ctrlx)
			{
				buflush(h);
				glbkbchar[h][add[h]++ & KBBUFMASK] = ch;
			} else
			{
				if (add[h] < remove[h] + KBBUFSZ)
				{
					glbkbchar[h][add[h]++ & KBBUFMASK] = ch;
				} else
				{
					bconout(h, 7);
				}
			}
		} while (stop);
	}
}


VOID buflush(P(FH) h)
PP(FH h;)
{
	/* flush BDOS type-ahead buffer */

	add[h] = remove[h] = 0;
}

/******************/
/* console output */
/* used internally*/
/******************/

VOID conout(P(FH) h, P(int) ch)
PP(FH h;)
PP(int ch;)
{
	conbrk(h);							/* check for control-s break */
	bconout(h, ch);						/* output character to console */
	if (ch >= ' ')
		glbcolumn[h]++;					/* keep track of screen column */
	else if (ch == cr)
		glbcolumn[h] = 0;
	else if (ch == bs)
		glbcolumn[h]--;
}

/*****************************************************************************
**
** xtabout -
**	Function 0x02 - Cconout - console output with tab expansion
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13bbc */
VOID xtabout(P(int16_t) ch)
PP(int16_t ch;)
{
	tabout(HXFORM(run->p_uft[1]), ch);
}

/*****************************************************************************
**
** tabout -
**
******************************************************************************
 */

VOID tabout(P(FH) h, P(int) ch)
PP(FH h;)
PP(int ch;)									/* character to output to console   */
{
	if (ch == tab)
	{
		do
			conout(h, ' ');
		while (glbcolumn[h] & 7);
	} else
	{
		conout(h, ch);
	}
}

/*******************************/
/* console output with tab and */
/* control character expansion */
/*******************************/

VOID cookdout(P(FH) h, P(int) ch)
PP(FH h;)
PP(int ch;)									/* character to output to console   */
{
	if (ch == tab)
	{
		tabout(h, ch);					/* if tab, expand it   */
	} else
	{
		if (ch < ' ')
		{
			conout(h, '^');
			ch |= 0x40;
		}
		conout(h, ch);					/* output the character */
	}
}

/*****************************************************************************
**
** xauxout -
**	Function 0x04 - Cauxout - auxiliary output
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13c7e */
int16_t xauxout(P(int16_t) ch)
PP(int16_t ch;)
{
	return bconout(HXFORM(run->p_uft[3]), ch);
}

/*****************************************************************************
**
** xprtout -
**	Function 0x05 - Cprnout - printer output
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13ca6 */
int32_t xprtout(P(int16_t) ch)
PP(int16_t ch;)
{
	return bconout(HXFORM(run->p_uft[4]), ch);
}


int32_t getch(P(FH) h)
PP(FH h;)
{
	int32_t temp;

	if (add[h] > remove[h])
	{
		temp = glbkbchar[h][remove[h]++ & KBBUFMASK];
		if (add[h] == remove[h])
		{
			buflush(h);
		}
		return temp;
	}

	return bconin(h);
}

/*****************************************************************************
**
** x7in -
**	Function 0x07 - Crawcin - Direct console input without echo
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13d7c */
int32_t x7in(NOTHING)
{
	return getch(HXFORM(run->p_uft[0]));
}


int32_t conin(P(FH) h)							/* BDOS console input function */
PP(FH h;)
{
	long ch;

	conout(h, (int) (ch = getch(h)));
	return ch;
}

/*****************************************************************************
**
** xconin -
**	Function 0x01 - Cconin - console input
**
**	Last modified	SCC	16 Aug 85
******************************************************************************
 */

/* 306: 00e13dd0 */
int32_t xconin(NOTHING)
{
	int h;

	h = HXFORM(run->p_uft[0]);
	conbrk(h);
	return conin(h);
}

/*****************************************************************************
 *
 * x8in -
 *	Function 0x08 - Cnecin - Console input without echo
 *
 *	Last modified	SCC	24 Sep 85
 *****************************************************************************
 */

/* 306: 00e13dea */
int32_t x8in(NOTHING)
{
	register int h;
	register long ch;

	h = HXFORM(run->p_uft[0]);
	conbrk(h);
	ch = getch(h);
	if ((ch & 0xFF) == ctrlc)
	{
		warmboot();
		__builtin_unreachable(); /* quiet compiler about no return value */
	} else
	{
		return ch;
	}
}

/*****************************************************************************
 *
 * xauxin -
 *	Function 0x03 - Cauxin - Auxiliary input
 *
 *	Last modified	SCC	11 Aug 85
 *****************************************************************************
 */

/* 306: 00e13e22 */
int32_t xauxin(NOTHING)
{
	return bconin(HXFORM(run->p_uft[3]));
}

/*****************************************************************************
**
** rawconio -
**	Function 0x06 - Crawio - Raw console I/O
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13e46 */
int32_t rawconio(P(int16_t) parm)
PP(int16_t parm;)
{
	int i;

	if (parm == 0xFF)
	{
		i = HXFORM(run->p_uft[0]);
		return constat(i) ? getch(i) : 0L;
	}
	return bconout(HXFORM(run->p_uft[1]), parm);
}

/*****************************************************************************
**
** xprt_line -
**	Function 0x09 - Cconws - Print line up to nul with tab expansion
**
**	Last modified	SCC	11 Aug 85
******************************************************************************
 */

/* 306: 00e13ea6 */
VOID xprt_line(P(const char *) p)
PP(const char *p;)
{
	prt_line(HXFORM(run->p_uft[1]), p);
}


VOID prt_line(P(FH) h, P(const char *) p)
PP(FH h;)
PP(const char *p;)
{
	while (*p)
		tabout(h, *p++);
}


/**********************************************/
/* read line with editing and bounds checking */
/**********************************************/

/* Two subroutines first */

VOID newline(P(FH) h, P(int) startcol)
PP(FH h;)
PP(int startcol;)
{
	conout(h, cr);						/* go to new line */
	conout(h, lf);
	while (startcol)
	{
		conout(h, ' ');
		startcol -= 1;					/* start output at starting column */
	}
}


/* backspace one character position */
int backsp(P(FH) h, P(const char *) cbuf, P(int) retlen, P(int) col)
PP(FH h;)
PP(const char *cbuf;)
PP(int retlen;)
PP(int col;)								/* starting console column  */
{
	register char ch;					/* current character        */
	register int i;
	register const char *p;					/* character pointer        */

	if (retlen)
		--retlen;
	/* if buffer non-empty, decrease it by 1 */
	i = retlen;
	p = cbuf;
	while (i--)							/* calculate column position    */
	{									/*  across entire char buffer   */
		ch = *p++;						/* get next char        */
		if (ch == tab)
		{
			col += 8;
			col &= ~7;					/* for tab, go to multiple of 8 */
		} else if (ch < ' ')
			col += 2;
		/* control chars put out 2 printable chars */
		else
			col += 1;
	}
	while (glbcolumn[h] > col)
	{
		conout(h, bs);					/* backspace until we get to proper column */
		conout(h, ' ');
		conout(h, bs);
	}
	return retlen;
}

/*****************************************************************************
**
** readline -
**	Function 0x0A - Cconrs - Read console string into buffer
******************************************************************************
 */

/* 306: 00e13fd6 */
VOID readline(P(char *) p)
PP(char *p;)								/* max length, return length, buffer space */
{
	p[1] = cgets(HXFORM(run->p_uft[0]), (((int) p[0]) & 0xFF), &p[2]);
}


int cgets(P(FH) h, P(int) maxlen, P(char *) buf)
PP(FH h;)									/* h is special handle denoting device number */
PP(int maxlen;)
PP(char *buf;)
{
	char ch;
	int i, stcol, retlen;

	stcol = glbcolumn[h];				/* set up starting column */
	for (retlen = 0; retlen < maxlen;)
	{
		switch (ch = getch(h))
		{
		case cr:
		case lf:
			conout(h, cr);
			goto getout;
		case bs:
		case rub:
			retlen = backsp(h, buf, retlen, stcol);
			break;
		case ctrlc:
			warmboot();
		case ctrlx:
			do
				retlen = backsp(h, buf, retlen, stcol);
			while (retlen);
			break;
		case ctrlu:
			conout(h, '#');
			newline(h, stcol);
			retlen = 0;
			break;
		case ctrlr:
			conout(h, '#');
			newline(h, stcol);
			for (i = 0; i < retlen; i++)
				cookdout(h, buf[i]);
			break;
		default:
			cookdout(h, buf[retlen++] = ch);
		}
	}
getout:
	return retlen;
}
