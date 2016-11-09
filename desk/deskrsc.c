#include "desktop.h"

#if !STR_IN_RSC

#include "ctrycodes.h"

#if OS_COUNTRY == CTRY_US
#include "rsc/306/strus.h"
#endif

#if OS_COUNTRY == CTRY_UK
#include "rsc/306/strus.h"
#endif

#if (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SG)
#include "rsc/306/strde.h"
#endif

#if (OS_COUNTRY == CTRY_FR) | (OS_COUNTRY == CTRY_SF)
#include "rsc/306/strfr.h"
#endif

#if OS_COUNTRY == CTRY_ES
#include "rsc/306/stres.h"
#endif

#if OS_COUNTRY == CTRY_IT
#include "rsc/306/strit.h"
#endif

#if OS_COUNTRY == CTRY_SV
#include "rsc/306/strsv.h"
#endif

#if OS_COUNTRY == CTRY_TR
#include "rsc/306/strtr.h"
#endif

#if OS_COUNTRY == CTRY_FI
#include "rsc/306/strfi.h"
#endif

#if OS_COUNTRY == CTRY_NO
#include "rsc/306/strno.h"
#endif

#if OS_COUNTRY == CTRY_DK
#include "rsc/306/strdk.h"
#endif

#if OS_COUNTRY == CTRY_PL
#include "rsc/306/strpl.h"
#endif

#endif
