/* SETENV.RSH
 *==========================================================================
 * DATE: MAY 3, 1990	(from C. Gee)
 * DESCRIPTION: RSH file from RESOURCE
 */

#pragma warn -sus
#pragma warn -apt	/* 1 Non-portable pointer assignment */
#pragma warn -rpt	/* 1 Non-portable pointer conversion */
#pragma warn -sig       /* 2 Conversion may lose significant digits */

 
#if USA
#include "rsc\en.rsh"
#endif

#if	UK
#include "rsc\en.rsh"
#endif

#if FRANCE
#include "rsc\fr.rsh" 
#endif

#if GERMANY
#include "rsc\de.rsh"
#endif

#if ITALY
#include "rsc\fr.rsh"
#endif

#if SPAIN
#include "rsc\es.rsh"
#endif

#if SWEDEN
#include "rsc\sv.rsh"
#endif

#pragma warn .sig
#pragma warn .apt
#pragma warn .rpt
#pragma warn .sus
