*
* These functions are referenced by BIOS,
* but are defined elsewhere
*
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*
#include "../common/config.h"
#include "ctrycodes.h"

* from VDI
		xdef line1010
		xdef _SETBLT
		xdef _GETBLT
		xdef vdireinit
		xdef resetdev
		xdef blink
		xdef _cursconf
		xdef _bcon5out
		xdef _bcon2out
* from GEMDOS
		xdef _osinit
		xdef lmul
* from AES
		xdef gemstart

#if OS_COUNTRY == CTRY_US
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $0000641C
DESKTLEN    equ $0000FBCC
#endif

#if OS_COUNTRY == CTRY_DE
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006476
DESKTLEN    equ $0000FBA2
#endif

#if OS_COUNTRY == CTRY_FR
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006494
DESKTLEN    equ $0000FBA2
#endif

#if OS_COUNTRY == CTRY_ES
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $000064A2
DESKTLEN    equ $0000FBA2
#endif

#if OS_COUNTRY == CTRY_IT
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006484
DESKTLEN    equ $0000FBA2
#endif

#if OS_COUNTRY == CTRY_UK
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $0000641C
DESKTLEN    equ $0000FBCC
#endif

#if OS_COUNTRY == CTRY_SV
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006494
DESKTLEN    equ $0000FBA2
#endif

#if OS_COUNTRY == CTRY_PL
BIOSTBASE   equ $00380000
BIOSTLEN    equ $00006476
DESKTLEN    equ $0000FBA2
#endif


*
* these are identical in all versions
*
BIOSDLEN    equ $000009D8
VDITLEN     equ $0000D06E
VDIDLEN     equ $00006EFA
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
AESTLEN     equ $0000CCD6
AESDLEN     equ $000006D8
DESKDLEN    equ $00000000

*
* now calculate addresses from above
*
VDITBASE  equ BIOSTBASE+BIOSTLEN
BDOSTBASE equ VDITBASE+VDITLEN
AESTBASE  equ BDOSTBASE+BDOSTLEN
DESKTBASE equ AESTBASE+AESTLEN
ETEXT     equ DESKTBASE+DESKTLEN

BIOSDBASE equ ETEXT
VDIDBASE  equ BIOSDBASE+BIOSDLEN
BDOSDBASE equ VDIDBASE+VDIDLEN
AESDBASE  equ BDOSDBASE+BDOSDLEN
DESKDBASE equ AESDBASE+AESDLEN
EDATA     equ DESKDBASE+DESKDLEN

gemstart  equ AESTBASE

_osinit   equ BDOSTBASE+$5588
lmul      equ BDOSTBASE+$298

line1010    equ VDITBASE+$14B6
_SETBLT     equ VDITBASE+$1AAA
_GETBLT     equ VDITBASE+$1AD2
vdireinit   equ VDITBASE+$04D4
resetdev    equ VDITBASE+$057E
blink       equ VDITBASE+$11A4
_cursconf   equ VDITBASE+$11D6
_bcon5out   equ VDITBASE+$0E1C
_bcon2out   equ VDITBASE+$0E28
