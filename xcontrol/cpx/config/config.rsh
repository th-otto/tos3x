/* COLOR.RSH
 *==========================================================================
 * DATE: MAY 3, 1990	(from C. Gee)
 * DESCRIPTION: RSH file from RESOURCE
 */

#pragma warn -sus
 
#if USA
#include "en.rsh" 
#endif

#if	UK
#include "en_GB.rsh" 
#endif

#if FRANCE
#include "fr.rsh" 
#endif

#if GERMANY
#include "de.rsh" 
#endif

#if ITALY
#include "it.rsh" 
#endif

#if SPAIN
#include "es.rsh" 
#endif

#if SWEDEN
#include "sv.rsh" 
#endif

#pragma warn .sus
