/* COLOR.RSH
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
#include "rsc\it.rsh"
#endif

#if SPAIN
#include "rsc\es.rsh"
#endif

#if SWEDEN
#include "rsc\sv.rsh"
#endif

static void fix_system_rsh(void)
{
	(*xcpb->rsh_fix) (NUM_OBS, NUM_FRSTR, NUM_FRIMG, NUM_TREE,
					  rs_object, rs_tedinfo, rs_strings, rs_iconblk,
					  rs_bitblk, rs_frstr, rs_frimg, rs_trindex, rs_imdope);
}

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

#define rs_object ws_object
#define rs_tedinfo ws_tedinfo
#define rs_strings ws_strings
#define rs_iconblk ws_iconblk
#define rs_bitblk ws_bitblk
#define rs_frstr ws_frstr
#define rs_frimg ws_frimg
#define rs_trindex ws_trindex
#define rs_imdope ws_imdope
#undef T0OBJ
#undef FREEBB
#undef FREEIMG
#undef FREESTR
#define pname pxname

#include "rsc\other.rsh"

static void fix_other_rsh(void)
{
	fix_rsh(NUM_OBS, NUM_FRSTR, NUM_FRIMG, NUM_TREE,
			rs_object, rs_tedinfo, rs_strings, rs_iconblk,
			rs_bitblk, rs_frstr, rs_frimg, rs_trindex, rs_imdope);
}

#undef rs_object
#undef rs_tedinfo
#undef rs_strings
#undef rs_iconblk
#undef rs_bitblk
#undef rs_frstr
#undef rs_frimg
#undef rs_trindex
#undef rs_imdope
#undef T0OBJ
#undef FREEBB
#undef FREEIMG
#undef FREESTR
#undef pname
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

#pragma warn .sig
#pragma warn .apt
#pragma warn .rpt
#pragma warn .sus
