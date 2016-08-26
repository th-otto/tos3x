/****************************************************************************
*
*		   C   F I L E   C R E A T E   R O U T I N E
*		   -----------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	The "creat" routine opens a new "C" file and returns a file id. 
*	Comes in 3 flavors: ascii (CP/M text files), binary, and default
*	(currently ascii).
*
*	Calling Sequence:
*		fid = creat(fname,prot)
*		fid = creata(fname,prot)
*		fid = creatb(fname,prot)
*		fid = _creat(fname,prot,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		prot	is the UNIX file protection
*		type	is 0 for ASCII, 1 for BINARY
*
*****************************************************************************/
#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>


/* default to binary */
int creat(P(const char *) fname, P(mode_t) prot)
PP(const char *fname;)
PP(mode_t prot;)
{
	return open(fname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, prot);
}
