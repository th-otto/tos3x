/***************************************************************************
 * TTYINRAW.C - handles raw input independent of (CP/M) OS.		    
 *	Problem is that different OSs handle Raw Input in different ways.   
 *	CPM-86 and CPM68K use only 0xFF, but CPM68K waits for a character.  
 *	Hard loops in multi-tasking, however, waste CPU cycles, so MPM and  
 *	Concurrent use 0xFD.						    
 *
 * Edits:								    
 *	26-Jan-84 whf	simplify for 68k compiler.
 *	8-Dec-83 whf	handle PC-DOS like CPM-86			    
 ****************************************************************************/

#include "lib.h"
#include <osif.h>
#include <osiferr.h>
#if CPM
#include "osattr.h"
#endif

#if CPM
static int CharInReq = 0xFF;			/* Default CONIO Char In Rqs */
#else
#define CharInReq 0xff
#endif


int _ttyinraw(P(int) flag)
PP(int flag;)
{
#if GEMDOS
	return (int)Crawcin();
#else
	register int ic;						/* Input Char           */

	while ((ic = __OSIF(CONIO, CharInReq)) == 0)	/* Wait in loop if needed   */
	{									/*              */
#if CPM
		if (!flag)
			break;
		/* This goes inside the loop; for Concurrent, it will be called   */
		/* only once, while for other O.S.'s, it doesn't matter what we   */
		/* do while we're waiting for a char.                 */
		__OSIF(S_BDOSVER, 0);			/* Puts AX in __cpmrv       */
		if (__cpmrv & 0x0500)			/* Concurrent or MPM        */
			CharInReq = 0xFD;			/* Change Char Input Parm   */
#endif
	}
	return ic;
#endif
}


#if CPM

int ttyinraw(P(int) mode)
PP(int mode;)
{
	switch (mode)
	{
	case 2:
		if (os_abilities & _os_0x0004)
			return 0;
		/* fall through */
	case 0:
		return _ttyinraw(0);
	case 3:
		return __OSIF(C_STAT, 0);
	case 1:
	default:
		return _ttyinraw(1);
	}
}
#endif
