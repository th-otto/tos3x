#include "asmdefs.h"
#include "header.h"
#include "tosvars.inc"

/* 306de: 00e0694a */
	xdef vdireinit
vdireinit:
	rts

/* 306de: 00e069f4 */
	xdef resetdev
resetdev:
	rts

/* 306de: 00e0761a */
	xdef _blink
_blink:
	rts


/*
 * XBIOS #21 - Cursconf - Sets the working of the VT52 cursor
 */
/* 306: 00e0764c */
	xdef _cursconf
_cursconf:
		rts


/* 306: 00e07f20 */
	xdef setblitmode
setblitmode:
        move.w    d0,LA_BLTMODE
        movea.l   LA_CURDEV,a0
        btst      #0,d0
        beq.s     setblit1
        movea.l   LA_ROUTINES,a0
setblit1:
        lea.l     LA_USERDEVINIT,a1
        moveq.l   #9,d0
setblit2:
        move.l    (a0)+,(a1)+
        dbf       d0,setblit2
        rts

/* 306: 00e07f48 */
	xdef getblitmode
getblitmode:
        move.w    LA_BLTMODE,d0
        rts

