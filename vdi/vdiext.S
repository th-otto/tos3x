*
* These functions are referenced by VDI,
* but are defined elsewhere
*
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*
#include "../common/config.h"
#include "vdidefs.h"
#include "../common/ctrycodes.h"
#include "../common/sections.inc"

* from BIOS
		xdef ringbell
* This strange looking equate is needed because the symbol
* is accessed by a relative branch
		text
ringbell ds.b 0
ringbell  equ $3B2C+(BIOSTLEN-BTLEN_US)-(VDITBASE-BIOSTBASE)
		
		xdef _zeromem
_zeromem equ BIOSTBASE+$1680
