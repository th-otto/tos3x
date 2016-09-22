
        xdef _scrdmp
_scrdmp:
        movea.l   _scr_dmp,a0
        jsr       (a0)
        move.w    #-1,_dumpflg
        rts

		xdef _bscr_dump
_bscr_dump:
        rts
