/**************************************************************************
 *                                                                        *
 *        convert ikbd real-time clock format to jdos format              *
 *                                                                        *
 **************************************************************************/
/* 306de: 00e02160 */
/* 206de: 00e01e86 */
_clockvec:
        lea.l     (clockbuf).w,a0
        bsr       bcdget
        subi.b    #80,d0                        /* adjust so that 1980 => 0 for time base */
        move.b    d0,d2
        asl.l     #4,d2
        bsr       bcdget
        add.b     d0,d2
        asl.l     #5,d2
        bsr       bcdget
        add.b     d0,d2
        asl.l     #5,d2
        bsr       bcdget
        add.b     d0,d2
        asl.l     #6,d2
        bsr       bcdget
        add.b     d0,d2
        asl.l     #5,d2
        bsr       bcdget
        lsr.b     #1,d0                         /* adjust to provide two second increments... */
        add.b     d0,d2                         /* ...another @!#%@#$% kludge, thank you ! */
        move.l    d2,(iclkrtime).w
        move.b    #$00,(iclk_ready).w           /* clear handshaking flag */
        rts


/* 306de: 00e021a4 */
/* 206de: 00e01eca */
igetdt:
        move.b    #-1,(iclk_ready).w
        move.b    #$1C,d1    /* send read clock command */
        bsr       ikbd_writeb
        movea.l   (_hz_200).w,a0
        adda.w    #$00C8,a0
        moveq.l   #0,d0
igetdt1:
        cmpa.l    (_hz_200).w,a0
        bcs.s     igetdt2
        tst.b     (iclk_ready).w
        bne.s     igetdt1
        move.l    (iclkrtime).w,d0
igetdt2:
        rts

/* 306de: 00e021ce */
/* 206de: 00e01ef4 */
isetdt:
        move.l    4(a7),(iclkwtime).w
        lea.l     iclkbuf+6,a0
        move.l    (iclkwtime).w,d2
        move.b    d2,d0
        andi.b    #$1F,d0  /* write seconds */
        asl.b     #1,d0
        bsr.s     bcdput
        lsr.l     #5,d2    /* write minutes */
        move.b    d2,d0
        andi.b    #$3F,d0
        bsr.s     bcdput
        lsr.l     #6,d2    /* write hour */
        move.b    d2,d0
        andi.b    #$1F,d0
        bsr.s     bcdput
        lsr.l     #5,d2    /* write day of month */
        move.b    d2,d0
        andi.b    #$1F,d0
        bsr.s     bcdput
        lsr.l     #5,d2    /* write month */
        move.b    d2,d0
        andi.b    #$0F,d0
        bsr.s     bcdput
        lsr.l     #4,d2    /* write year */
        move.b    d2,d0
        andi.b    #$7F,d0
        bsr.s     bcdput
        addi.b    #$80,(a0)
        move.b    #$1B,d1    /* send set clock command */
        bsr       ikbd_writeb
        moveq.l   #5,d3
        lea.l     iclkbuf,a2
        bsr       ikbdws1
        move.b    #$1C,d1    /* send read clock command */
        bsr       ikbd_writeb
        rts

/**************************************************************************
 *                                                                        *
 *                  convert a byte from binary to bcd format              *
 *                                                                        *
 *       entry:     d0.l  - value                                         *
 *                                                                        *
 **************************************************************************/
bcdput:
        moveq.l   #0,d1
        move.b    d0,d1
        divs.w    #10,d1
        asl.w     #4,d1
        move.w    d1,d0
        swap      d1
        add.w     d1,d0
        move.b    d0,-(a0)                      /* transfer to output clock buffer */
        rts

/**************************************************************************
 *                                                                        *
 *                  convert a byte from bcd format to binary              *
 *                                                                        *
 *       entry:     a0.l  - pointer to byte                               *
 *                                                                        *
 **************************************************************************/
bcdget:
        move.b    (a0)+,d0                      /* get bcd byte */
        move.b    d0,d1
        and.w     #$000F,d0                     /* dump high nibble */
        and.w     #$00F0,d1                     /* isolate high nibble */
        asr.w     #4,d1                         /* dump low nibble */
        mulu.w    #10,d1                        /* high nibble * 10 */
        add.w     d1,d0                         /* + low nibble */
        rts

