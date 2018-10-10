/* COLRSC.H
 *==========================================================================
 * DATE: April 3, 1990
 * DESCRIPTION: INCLUDE.H file from RESOURCE
 */
 
 
#if USA
#include "rsc\en.h"
#endif

#if	UK
#include "rsc\en.h"
#endif


#if FRANCE
#include "rsc\fr.h"
#endif

#if GERMANY
#include "rsc\de.h"
#endif

#if ITALY
#include "rsc\it.h"
#endif

#if SPAIN
#include "rsc\es.h"
#endif

#if SWEDEN
#include "rsc\sv.h"
#endif

#undef NUM_STRINGS
#undef NUM_FRSTR
#undef NUM_UD
#undef NUM_IMAGES
#undef NUM_BB
#undef NUM_FRIMG
#undef NUM_IB
#undef NUM_CIB
#undef NUM_TI
#undef NUM_OBS
#undef NUM_TREE
#include "rsc\other.h"
#undef NUM_STRINGS
#undef NUM_FRSTR
#undef NUM_UD
#undef NUM_IMAGES
#undef NUM_BB
#undef NUM_FRIMG
#undef NUM_IB
#undef NUM_CIB
#undef NUM_TI
#undef NUM_OBS
#undef NUM_TREE
