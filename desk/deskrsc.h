#include "ctrycode.h"

#if TOSVERSION >= 0x400
/*
 * assumes MULTILANG_SUPPORT & POPUP_SUPPORT;
 * layout of individual resource files
 * must be identical
 */
#if !MULTILANG_SUPPORT
error
#endif
#if !POPUP_SUPPORT
error
#endif
#include "rsc/404/deskus.h"
#endif

/*
 * Must include the specific resource header file;
 * the layout is actually different.
 * We can use the same header files for 2.06, 2.08 & 3.06, though.
 */
#if (TOSVERSION == 0x306) | (TOSVERSION == 0x206) | (TOSVERSION == 0x208)
#if OS_COUNTRY == CTRY_US
#include "rsc/306/deskus.h"
#endif
#if OS_COUNTRY == CTRY_UK
#include "rsc/306/deskuk.h"
#endif
#if (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SG)
#include "rsc/306/deskde.h"
#endif
#if (OS_COUNTRY == CTRY_FR) | (OS_COUNTRY == CTRY_SF)
#include "rsc/306/deskfr.h"
#endif
#if OS_COUNTRY == CTRY_ES
#include "rsc/306/deskes.h"
#endif
#if OS_COUNTRY == CTRY_IT
#include "rsc/306/deskit.h"
#endif
#if OS_COUNTRY == CTRY_SE
#include "rsc/306/deskse.h"
#endif
#if OS_COUNTRY == CTRY_TR
#include "rsc/306/desktr.h"
#endif
#if OS_COUNTRY == CTRY_FI
#include "rsc/306/deskfi.h"
#endif
#if OS_COUNTRY == CTRY_NO
#include "rsc/306/deskno.h"
#endif
#if OS_COUNTRY == CTRY_DK
#include "rsc/306/deskdk.h"
#endif
#if OS_COUNTRY == CTRY_PL
#include "rsc/306/deskpl.h"
#endif
#endif

/*
 * TOS versions before 0x400 have some language dependent strings
 * hardcoded instead of begin placed in the resource file
 */
#if !STR_IN_RSC

extern char const More[];
extern char const Endfile[];
extern char const Cart[]; /* unused; taken from desktop.inf instead */
extern char const Cpystr[];
extern char const Delstr[];
extern char const Fileinfo[];
extern char const Foldinfo[];
extern char const Movestr[];
extern char const Readerror[];
extern char const Cantopen[];
extern char const Nomem[];
extern char const Cachetxt[];
extern char const Blttxt[];
extern char const Nconflict[];
extern char const Rname[];
extern char const Crenstr[];
extern char const Mrenstr[];
extern char const Istr[];
extern char const Isels[];
extern char const Isel[];
extern char const Istrs[];
extern char const Device[];
extern char const Sinf[];
extern char const Litem[];

#endif
