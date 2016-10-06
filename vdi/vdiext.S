*
* These functions are referenced by VDI,
* but are defined elsewhere
*
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*
#include "../common/config.h"
#include "../bios/ctrycodes.h"
#include "../common/sections.inc"

* from BIOS
		xdef ringbell

* hm does not work because of PC-relative fixup overflow
* ringbell  equ BIOSTBASE+$3B2C+BIOSTLEN-$0000641C
