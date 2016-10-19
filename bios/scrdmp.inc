/*
 * XBIOS #14 - Scrdmp - Dump screen to printer.
 */
/* 306de: 00e01450 */
/* 206de: 00e0130c */
        xdef _scrdmp
_scrdmp:
        movea.l   _scr_dmp,a0
        jsr       (a0)
        move.w    #-1,_dumpflg
        rts

/*
 * default function for system variable scr_dmp/dump_vec
 */
/* 306de: 00e01462 */
/* 206de: 00e0131e */
	xdef _bscr_dump
_bscr_dump:
        move.l    (_v_bas_ad).w,(_pbdef).w      /* pbdef.pb_pb_scrptr */
        clr.w     (_pbdef+4).w                  /* pbdef.pb_offset */
        clr.w     d0
        move.b    (_sshiftmod).w,d0
        move.w    d0,(_pbdef+14).w              /* pbdef.pb_screz */
        add.w     d0,d0
        lea.l     pbdeftab(pc),a0
        move.w    0(a0,d0.w),(_pbdef+6).w       /* pbdef.pb_width */
        move.w    6(a0,d0.w),(_pbdef+8).w       /* pbdef.pb_height */
        clr.w     (_pbdef+10).w                 /* pbdef.pb_left */
        clr.w     (_pbdef+12).w                 /* pbdef.pb_right */
        move.l    #palette,(_pbdef+18).w        /* pbdef.pb_colptr */
        clr.w     (_pbdef+26).w                 /* pbdef.pb_mask; BUG: pb_mask is long (pointer) */
/* draft or final mode */
        move.w    (_prtconfig).w,d1
        lsr.w     #3,d1
        and.w     #$0001,d1
        move.w    d1,(_pbdef+16).w              /* pbdef.pb_prrez */
/* printer or rs232 port */
        move.w    (_prtconfig).w,d1
        move.w    d1,d0
        lsr.w     #4,d0
        and.w     #$0001,d0
        move.w    d0,(_pbdef+24).w              /* pbdef.pb_prport */
/* select printer flavor */
        and.w     #$0007,d1
        move.b    pbprtype(pc,d1.w),d0
        move.w    d0,(_pbdef+22)                /* pbdef.pb_prtype */
/* do it */
        pea.l     (_pbdef).w
        move.w    #$0001,(_dumpflg).w
        bsr       _prtblk
        move.w    #-1,_dumpflg
        addq.w    #4,a7
        rts

/* ---- screen resolution table (pixel) for printScreen */
/* BUG: bogus screen resolution table which is too short */
pbdeftab:
        dc.w 320,640,640
        dc.w 200,200,400
/* --- printer flavors (based on low 3 bits of pconfig) */
pbprtype:                            dc.b      0,2,1,-1,3,-1,-1,-1


