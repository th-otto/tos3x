/* XCONTROL.RSH
 *==========================================================================
 * DATE: April 3, 1990
 * DESCRIPTION: RSH file from RESOURCE
 */
 
#pragma warn -apt	/* 1 Non-portable pointer assignment */
#pragma warn -rpt	/* 1 Non-portable pointer conversion */
#pragma warn -sig       /* 2 Conversion may lose significant digits */
 
#if USA
 #include "rsc\us.rsh" 
#endif

#if UK
 #include "rsc\us.rsh" 
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

#if SWEDEN
 #include "rsc\se.rsh" 
#endif

#pragma warn .sig
#pragma warn .apt
#pragma wanr .rpt



