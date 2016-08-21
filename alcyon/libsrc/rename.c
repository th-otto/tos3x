/***************************************************************************
 *									    
 *			r e n a m e   F u n c t i o n			    
 *			-----------------------------			    
 *	Copyright 1983 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	'rename()' changes a file's name.				    
 *									    
 *	Calling sequence:						    
 *		ret = rename(from,to)					    
 *									    
 *	Where:								    
 *		from -> a EOS terminated (existing) file name		    
 *		to -> the EOS terminated new file name			    
 *		ret = SUCCESS if everything ok, FAILURE o.w.		    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>

int rename(P(const char *) from, P(const char *) to)
PP(const char *from;)								/* Existing file name       */
PP(const char *to;)								/* New file name        */
{
#if GEMDOS
	if (Frename(from, to) < 0)
		RETERR(-1, ERENAME);
#else
	char fcbbuf[32 + 16];				/* Extra large fcb      */
	register struct fcbtab *fcbp;			/* FCB pointer          */
#if CPM
	register int nuser;						/* new user #           */
	register int xuser;						/* system user #        */
#endif
	int rv;							/* Return value         */

	fcbp = (struct fcbtab *)fcbbuf;						/* Point fcb to start       */
	if (-1 == (int)_parsefn(from, fcbp))	/* Parse 'from' into fcb    */ /* BUG: _parsefn returns char */
		RETERR(-1, EINVAL);		/*    Bad filename      */
#if CPM
	nuser = fcbp->fuser;				/* Save specified User #    */
#endif
	fcbp = (struct fcbtab *)(&(fcbbuf[16]));				/* F_RENAME needs it        */
	if (-1 == (int)_parsefn(to, fcbp))	/* Parse 'to' to fcb[16]    */ /* BUG: _parsefn returns char */
		RETERR(-1, EINVAL);		/*    Bad filename      */
#if CPM
	if (fcbp->fuser)					/* user # specified?        */
	{
		if (nuser && nuser != fcbp->fuser)	/* Differing User #s?       */
			RETERR(-1, EINVAL);	/*   that's a no-no     */
		nuser = fcbp->fuser;			/* Use this user #      */
	}
	xuser = _chkuser(nuser);			/* Change user # if needed  */
#endif

	rv = __OSIF(F_RENAME, fcbbuf);		/* Use special fcb      */

#if CPM
	_uchkuser(nuser, xuser);			/* Change back if needed    */
#endif
	if (rv != 0)						/* Did it work?         */
		RETERR(-1, ERENAME);		/*   no...          */
#endif
	return 0;						/* It worked!           */
}
