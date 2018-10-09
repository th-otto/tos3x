/* PRINTER.RSH
 *==========================================================================
 * DATE: April 3, 1990
 * DESCRIPTION: RSH file from RESOURCE
 */
 
#pragma warn -sus
#pragma warn -apt	/* 1 Non-portable pointer assignment */
#pragma warn -rpt	/* 1 Non-portable pointer conversion */
#pragma warn -sig       /* 2 Conversion may lose significant digits */
 
#if USA | UK
#include "rsc\en.rsh" 
#endif

#if ITALY
#include "rsc\it.rsh" 
#endif

#if GERMAN
#include "rsc\de.rsh" 
#endif

#if FRENCH
#include "rsc\fr.rsh" 
#endif

#if SPAIN
#include "rsc\es.rsh" 
#endif

#if TURKEY
#include "rsc\tr.rsh" 
#endif

#if SWEDEN
#include "rsc\sv.rsh" 
#endif

#if FINLAND
#include "rsc\fi.rsh" 
#endif

#if NORWAY
#include "rsc\no.rsh" 
#endif

#if DENMARK
#include "rsc\da.rsh" 
#endif

#if ARABIC
#include "rsc\ar.rsh" 
#endif

#pragma warn .sig
#pragma warn .apt
#pragma warn .rpt
#pragma warn .sus
