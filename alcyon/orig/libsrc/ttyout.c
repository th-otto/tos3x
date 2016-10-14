/***************************************************************************
 *									    
 *			_ t t y o u t   F u n c t i o n			    
 *			-------------------------------			    
 *									    
 *	Function "_ttyout" is called to perform "write" operations to the   
 *	console device.							    
 *									    
 *	Calling Sequence:						    
 *		ret = _ttyout(buffer);					    
 *									    
 *	Where:								    
 *		buffer	-> the output buffer (byte 0 is count)		    
 *		ret	=  count on return				    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"

int _ttyout(P(const char *) buf)
PP(register const char *buf;)			/* -> 1st char output      */
{
	register int ii;					/* counter for '$' check   */
	int count;							/* =  # bytes to xfer      */
	const char *cp;						/* ptr for '$' check       */

	count = *buf++;						/* Get num bytes to output */
#ifdef __ALCYON__
	buf[count] = '$';					/* Terminate string    */
#endif
	for (ii = 0, cp = buf; ii < count; ++ii)	/* Check for internal '$'  */
		if (buf[ii] == '$')				/* Found one?          */
		{
#ifdef BUGGY_BDOS
			if (*cp != '$')				/* If '$' not at start     */
				__BDOS(C_WRITESTR, cp);	/* Print the string    */
			__BDOS(CONOUT, (long)'$');		/* And output the '$'      */
#else
			if (*cp != '$')				/* If '$' not at start     */
				__OSIF(C_WRITESTR, cp);	/* Print the string    */
			__OSIF(CONOUT, '$');		/* And output the '$'      */
#endif
			cp = &buf[ii + 1];			/* Reset start of string   */
		}
	if (cp != &buf[ii])					/* Assuming we haven't yet */
#ifdef BUGGY_BDOS
		__BDOS(C_WRITESTR, cp);			/* Output the rest     */
#else
		__OSIF(C_WRITESTR, cp);			/* Output the rest     */
#endif

	return count;						/* return original count   */
}
