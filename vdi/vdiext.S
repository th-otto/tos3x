/*
 * These functions are referenced by VDI,
 * but are defined elsewhere
 *
 *
 * This file is only used when linking a single module,
 * to get the same addresses as when linking the whole ROM
 */
#define __ASSEMBLER__ 1

#include "config.h"
#include "vdidefs.h"
#include "ctrycodes.h"
#include "sections.inc"

/* from BIOS */
		xdef ringbell
		xdef _zeromem
		text
ringbell ds.b 0

#if BINEXACT
#  ifdef __GNUC__
#    undef BINEXACT
#    define BINEXACT 0
#  endif
#endif

#if BINEXACT
/*
 * This strange looking equate is needed because the symbol
 * is accessed by a relative branch
 */
#if TOSVERSION == 0x306
ringbell  equ $3B2C+(BIOSTLEN-BTLEN_US)-(VDITBASE-BIOSTBASE)
_zeromem equ BIOSTBASE+$1680
#endif

#if TOSVERSION == 0x206
ringbell  equ $374A+(BIOSTLEN-BTLEN_US)-(VDITBASE-BIOSTBASE)
_zeromem equ BIOSTBASE+$1548
#endif
#endif
