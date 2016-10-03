*
* These functions are referenced by BIOS,
* but are defined elsewhere
*
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*
#include "config.h"
#include "../bios/ctrycodes.h"

* from VDI
		xdef line1010
* from GEMDOS
		xdef _ui_start
		xdef _osinit
		xdef lmul

#if OS_COUNTRY == CTRY_DE
line1010 equ $00E0792C
#endif

_ui_start equ GTSTART+GTLEN
_osinit   equ GTSTART+$5588
lmul      equ GTSTART+$298
