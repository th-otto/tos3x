/* SOUND.RSH
 *==========================================================================
 * DATE: April 3, 1990
 * DESCRIPTION: RSH file from RESOURCE
 */
 
#pragma warn -sus	/* 1 suspicious pointer conversion */
 
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

#if SWEDEN
#include "rsc\se.rsh" 
#endif

#pragma warn .sus
