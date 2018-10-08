/* COLOR.RSH
 *==========================================================================
 * DATE: MAY 3, 1990	(from C. Gee)
 * DESCRIPTION: RSH file from RESOURCE
 */
#ifdef __TURBOC__
#pragma warn -apt	/* 1 Non-portable pointer assignment */
#pragma warn -rpt	/* 1 Non-portable pointer conversion */
#pragma warn -sig       /* 2 Conversion may lose significant digits */
#endif

 
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

#ifdef __TURBOC__
#pragma warn .sig
#pragma warn .apt
#pragma wanr .rpt
#endif
