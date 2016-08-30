/****************************************************************************
*
*		   C   F I L E   O P E N   R O U T I N E
*		   -------------------------------------
*
*	The "open" routine opens a "C" file and returns a file id. 
*
*	Calling Sequence:
*
*		fid = open(fname,mode,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		mode	is the open mode:
*
*				0 => READ access only
*				1 => WRITE access only
*				2 => Both READ and WRITE
*		type	is 0 for ASCII files, 1 for BINARY
*
*
*****************************************************************************/

#include "../libsrc/lib.h"
#include "../libsrc/klib.h"

int xopen(P(const char *) fname, P(int) mode, P(int) binary)
PP(const char *fname;)				/* -> File name		    */
PP(int mode;)				/* Open mode		    */
PP(int binary;)				/* File type		    */
{
	return _open(fname, mode, binary);	/* Call clib routine	    */
}
