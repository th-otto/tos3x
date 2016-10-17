*
* These 2 functions are referenced by GEMDOS,
* but are defined elsewhere
*
#include "../common/config.h"
#include "../common/ctrycodes.h"
#include "../common/sections.inc"

	xdef _zeromem
	xdef _GSX_ENTRY

#if TOSVERSION == 0x306
_zeromem equ BIOSTBASE+$1680
_GSX_ENTRY   equ VDITBASE+$143E
#endif

