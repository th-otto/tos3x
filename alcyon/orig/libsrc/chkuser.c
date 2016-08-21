/***************************************************************************
 *									    
 *			_ c h k u s e r   F u n c t i o n		    
 *			---------------------------------		    
 *									    
 *	'_chkuser' and '_uchkuser' handle user numbers under CPM before	    
 *	version 4.   The user number is stored in the ccb structure;	    
 *	if a zero, the default user # is assumed, else the _chkuser()	    
 *	routine sets the user number to the desired user number (one less   
 *	than the stored user number, since user 0 is valid - similar to the 
 *	default drive spec for an FCB).					    
 *	'_uchkuser()' sets the user number back to its previous value.	    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"

int _chkuser(P(int) newu)
PP(int	newu;)				/* Desired user number	    */
{
#if CPM 
	register int prevu;				/* Previous user number	    */

	if (newu == 0)				/* Default user # desired?  */
		return 0;			/*   Yes, return	    */
	prevu = __OSIF(USER, 0xFF)+1;		/* Get prev and adjust	    */
	if (newu != prevu)			/* Are they different?	    */
		__OSIF(USER, newu-1);		/*   Yes, set to desired u# */
	return prevu;				/* Return the previous	    */
#else
	return 0;
#endif
}


int _uchkuser(P(int) newu, P(int) prevu)
PP(int newu;)				/* Desired user number	    */
PP(int prevu;)				/* Previous user number	    */
{
#if CPM
	if (newu == 0)				/* Default user #?	    */
		return;				/*   assume no change	    */
	if (newu != prevu)			/* Are they different?	    */
		__OSIF(USER, prevu-1);		/*   Yes, set to what it was*/
#else
	return 0;
#endif
}
