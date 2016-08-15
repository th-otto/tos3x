/* NOFILESZ: eliminates routine to calculate filesize (used in lseek()) */

#include "lib.h"

VOID nofilesz(NOTHING) { ; }				/* stubroutine 'tag'	    */


long _filesz(P(int) fd)
{
	return 32767;
}

