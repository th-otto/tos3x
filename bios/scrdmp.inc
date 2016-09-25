/*
 * XBIOS #14 - Scrdmp - Dump screen to printer.
 */
/* 306de: 00e01450 */
        xdef _scrdmp
_scrdmp:
        movea.l   _scr_dmp,a0
        jsr       (a0)
        move.w    #-1,_dumpflg
        rts

/* 306de: 00e01462 */
		xdef _bscr_dump
_bscr_dump:
        rts

