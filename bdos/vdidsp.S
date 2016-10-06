*
* These 2 functions are referenced by GEMDOS,
* but are defined elsewhere
*
#include "../common/config.h"
#include "../bios/ctrycodes.h"

	xdef _zeromem
	xdef _GSX_ENTRY

#if OS_COUNTRY == CTRY_US
_zeromem equ $00E01680
_GSX_ENTRY   equ $00E0785A
#endif

#if OS_COUNTRY == CTRY_DE
_zeromem equ $00E01680
_GSX_ENTRY   equ $00E078B4
#endif

#if OS_COUNTRY == CTRY_FR
_zeromem equ $00E01680
_GSX_ENTRY   equ $00E078D2
#endif

#if OS_COUNTRY == CTRY_UK
_zeromem equ $00E01680
_GSX_ENTRY   equ $00E0785A
#endif

#if OS_COUNTRY == CTRY_SV
_zeromem equ $00E01680
_GSX_ENTRY   equ $00E078D2
#endif

#if OS_COUNTRY == CTRY_PL
_zeromem equ $00381680
_GSX_ENTRY   equ $003878B4
#endif
