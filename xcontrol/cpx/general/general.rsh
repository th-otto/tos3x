/* GENERAL.RSH
 *==========================================================================
 * DATE: April 3, 1990
 * DESCRIPTION: RSH file from RESOURCE
 */

#pragma warn -sus
#pragma warn -apt	/* 1 Non-portable pointer assignment */
#pragma warn -rpt	/* 1 Non-portable pointer conversion */
#pragma warn -sig       /* 2 Conversion may lose significant digits */
 
#if USA | UK
#include "en.rsh" 
#endif

#if ITALY
#include "it.rsh" 
#endif

#if GERMAN
#include "de.rsh" 
#endif

#if FRENCH
#include "fr.rsh" 
#endif

#if SPAIN
#include "es.rsh" 
#endif

#if SWEDEN
#include "sv.rsh" 
#endif

#pragma warn .sig
#pragma warn .apt
#pragma warn .rpt
#pragma warn .sus
