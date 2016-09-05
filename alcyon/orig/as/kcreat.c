/****************************************************************************
*
*		   C   F I L E   C R E A T E   R O U T I N E
*		   -----------------------------------------
*
*	The "creat" routine opens a new "C" file and returns a file id. 
*
*	Calling Sequence:
*
*		fid = creat(fname,prot,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		prot	is the UNIX file protection
*		type	is 0 for ASCII, 1 for BINARY
*
*****************************************************************************/

#include "../libsrc/lib.h"
#include <sys/types.h>
#include "../libsrc/klib.h"

int xcreat(P(const char *) fname, P(mode_t) prot, P(int) binary)
PP(const char *fname;)			/* -> File name		    */
PP(mode_t prot;)				/* Open mode		    */
PP(int binary;)					/* ASCII/BINARY flag	    */
{
#ifdef __ALCYON__
	_creat(fname, prot, binary);	/* Call clib routine	    */
	asm("nop");
#else
	return _creat(fname, prot, binary);	/* Call clib routine	    */
#endif
}
