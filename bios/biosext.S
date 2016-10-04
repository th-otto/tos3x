*
* These functions are referenced by BIOS,
* but are defined elsewhere
*
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*
#include "config.h"
#include "ctrycodes.h"

* from VDI
		xdef line1010
		xdef setblitmode
		xdef getblitmode
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
		xdef _ui_start

#if OS_COUNTRY == CTRY_US
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $0000641C
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $00E1348A
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $00E078D2
setblitmode equ $00E07EC6
getblitmode equ $00E07EEE
vdireinit   equ $00E068F0
resetdev    equ $00E0699A
blink       equ $00E075C0
_cursconf   equ $00E075F2
_bcon5out   equ $00E07238
_bcon2out   equ $00E07244
#endif

#if OS_COUNTRY == CTRY_DE
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006476
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $00E134E4
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $00E0792C
setblitmode equ $00E07F20
getblitmode equ $00E07F48
vdireinit   equ $00E0694A
resetdev    equ $00E069F4
blink       equ $00E0761A
_cursconf   equ $00E0764C
_bcon5out   equ $00E07292
_bcon2out   equ $00E0729E
#endif

#if OS_COUNTRY == CTRY_FR
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006494
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $00E13502
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $00E0794A
setblitmode equ $00E07F3E
getblitmode equ $00E07F66
vdireinit   equ $00E06968
resetdev    equ $00E06A12
blink       equ $00E07638
_cursconf   equ $00E0766A
_bcon5out   equ $00E072B0
_bcon2out   equ $00E072BC
#endif

#if OS_COUNTRY == CTRY_IT
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006476
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $00E134E4
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $00E0792C
setblitmode equ $00E07F20
getblitmode equ $00E07F48
vdireinit   equ $00E0694A
resetdev    equ $00E069F4
blink       equ $00E0761A
_cursconf   equ $00E0764C
_bcon5out   equ $00E07292
_bcon2out   equ $00E0729E
#endif

#if OS_COUNTRY == CTRY_UK
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $0000641C
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $00E1348A
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $00E078D2
setblitmode equ $00E07EC6
getblitmode equ $00E07EEE
vdireinit   equ $00E068F0
resetdev    equ $00E0699A
blink       equ $00E075C0
_cursconf   equ $00E075F2
_bcon5out   equ $00E07238
_bcon2out   equ $00E07244
#endif

#if OS_COUNTRY == CTRY_SV
BIOSTBASE   equ $00E00000
BIOSTLEN    equ $00006494
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $00E13502
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $00E0794A
setblitmode equ $00E07F3E
getblitmode equ $00E07F66
vdireinit   equ $00E06968
resetdev    equ $00E06A12
blink       equ $00E07638
_cursconf   equ $00E0766A
_bcon5out   equ $00E072B0
_bcon2out   equ $00E072BC
#endif

#if OS_COUNTRY == CTRY_PL
BIOSTBASE   equ $00380000
BIOSTLEN    equ $00006476
BIOSDLEN    equ $000009D8
BDOSTBASE   equ $003934E4
BDOSTLEN    equ $000062AC
BDOSDLEN    equ $0000039E
line1010    equ $0038792C
setblitmode equ $00387F20
getblitmode equ $00387F48
vdireinit   equ $0038694A
resetdev    equ $003869F4
blink       equ $0038761A
_cursconf   equ $0038764C
_bcon5out   equ $00387292
_bcon2out   equ $0038729E
#endif

VDITBASE  equ BIOSTBASE+BIOSTLEN
AESTBASE  equ BDOSTBASE+BDOSTLEN

_ui_start equ AESTBASE
_osinit   equ BDOSTBASE+$5588
lmul      equ BDOSTBASE+$298
