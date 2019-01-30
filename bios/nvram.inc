	text

/* NVRAM_ADDR_REG  equ $ffff8961 */
/* NVRAM_DATA_REG  equ $ffff8963 */

rtc_sec         equ $fffffc20


/* 306de: 00e01ef0 */
#if NVRAM_SUPPORT
ngetdt:
        bsr       checkRTC
        bcs.s     inittc2
        move.b    #$0D,(NVRAM_ADDR_REG).w /* select register D */
        move.b    (NVRAM_DATA_REG).w,d0
        btst      #7,d0                   /* is RAM & time valid? */
        bne.s     inittc1                 /* yes */
        move.l    #$12C80000,-(a7)        /* (9<<9)+(6<<5)+8 */
        bsr       writeRTCTime
        addq.w    #4,a7
inittc1:
        bsr.s     readRTCTime
        cmp.l     #$FFFFFFFF,d0
        beq.s     inittc2
        moveq.l   #0,d0
inittc2:
        rts

/*
 * reads date & time from TT clock,
 * and updates GEMDOS variables from it
 */
/* 306de: 00e01f20 */
readRTCTime:
        bsr       checkRTC
        bcs.s     inittc2
        move.b    #$0D,(NVRAM_ADDR_REG).w /* select register D */
        btst      #7,(NVRAM_DATA_REG).w   /* is RAM & time valid? */
        beq       rdttc2                  /* no, fail */
        move.w    sr,d2
        move.w    d2,d0
        or.w      #$0700,d0
rdttc1:
        move.b    #$0A,(NVRAM_ADDR_REG).w /* select register A */
        btst      #7,(NVRAM_DATA_REG).w   /* is update in progress? */
        bne.s     rdttc1                  /* yes, wait */
        moveq.l   #0,d0
        move.l    d0,d1
        move.b    #$00,(NVRAM_ADDR_REG).w /* select register 0 (seconds) */
        move.b    (NVRAM_DATA_REG).w,d0
        asr.w     #1,d0
        move.b    #$02,(NVRAM_ADDR_REG).w /* select register 2 (minutes) */
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$1546                  /* bfins     d1,d0{21:6} */
        move.b    #$04,(NVRAM_ADDR_REG).w /* select register 4 (hours) */
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$1405                  /* bfins     d1,d0{16:5} */
        move.b    #$07,(NVRAM_ADDR_REG).w /* select register 7 (day of month) */
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$12c5                  /* bfins     d1,d0{11:5} */
        move.b    #$08,(NVRAM_ADDR_REG).w /* select register 8 (month) */
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$11c4                  /* bfins     d1,d0{7:4} */
        move.b    #$09,(NVRAM_ADDR_REG).w /* select register 9 (year) */
        move.b    (NVRAM_DATA_REG).w,d1
        sub.b     #nvram_rtc_year_offset,d1
        dc.w $efc0,$1007                  /* bfins     d1,d0{0:7} */
        move.w    d2,sr
        move.w    sr,d2
        ori.w     #$0700,sr
        move.w    d0,_time
        swap      d0
        move.w    d0,_date
        swap      d0
        move.w    d2,sr
        rts
rdttc2:
        moveq.l   #-1,d0
        rts

/*
 * write date & time to TT clock
 */
/* 306de: 00e01fc6 */
writeRTCTime:
        bsr       checkRTC
        bcs       inittc2
        move.l    4(a7),d0
        move.b    #$0B,(NVRAM_ADDR_REG).w /* select register B */
        move.b    #$80,(NVRAM_DATA_REG).w /* abort update cycle */
        move.b    #$0A,(NVRAM_ADDR_REG).w /* select register A */
        move.b    #$2A,(NVRAM_DATA_REG).w /* divider = 2, rate = 10 */
        move.b    #$0B,(NVRAM_ADDR_REG).w /* select register B */
        move.b    #$8E,(NVRAM_DATA_REG).w /* enable data mode binary, 24hr, daylight savings */
        move.b    #$00,(NVRAM_ADDR_REG).w /* select register 0 (seconds) */
        dc.w $e9c0,$16c5    /* bfextu    d0{27:5},d1  */
        add.b     d1,d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$02,(NVRAM_ADDR_REG).w /* select register 2 (minutes) */
        dc.w $e9c0,$1546    /* bfextu    d0{21:6},d1 */
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$04,(NVRAM_ADDR_REG).w /* select register 4 (hours) */
        dc.w $e9c0,$1405    /* bfextu    d0{16:5},d1 */
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$07,(NVRAM_ADDR_REG).w /* select register 7 (day of month) */
        dc.w $e9c0,$12c5    /* bfextu    d0{11:5},d1 */
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$08,(NVRAM_ADDR_REG).w /* select register 8 (month) */
        dc.w $e9c0,$11c4    /* bfextu    d0{7:4},d1 */
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$09,(NVRAM_ADDR_REG).w /* select register 9 (year) */
        dc.w $e9c0,$1007    /* bfextu    d0{0:7},d1 */
        add.b     #nvram_rtc_year_offset,d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$0B,(NVRAM_ADDR_REG).w
        move.b    #$0E,(NVRAM_DATA_REG).w
        rts

/* 306de: 00e0205e */
checkRTC:
        movea.l   a7,a0
        movea.l   (_buserror).w,a1
        move.l    #nvramfail,(_buserror).w
        move.b    #$00,(NVRAM_ADDR_REG).w
        move.b    (NVRAM_DATA_REG).w,d0
        move.l    a1,(_buserror).w
        andi.b    #$FE,ccr                /* clear carry bit */
        rts
nvramfail:
        movea.l   a0,a7
        move.l    a1,(_buserror).w
        ori.b     #$01,ccr                /* set carry bit */
        rts

/*
 * XBIOS #42 - NVMaccess
 */
/* 306de: 00e0208c */
_nvmaccess:
        moveq.l   #-5,d0       /* return E_BADRQ in case of error */
        move.w    4(a7),d1
        beq.s     nvmread      /* op = 0: read NVRAM */
        cmp.w     #$0002,d1
        beq.s     nvminit      /* op = 2: initialize NVRAM and update checksum */
        bhi.s     nvmret       /* (unknown op) */
/* op = 1: write into NVRAM and update the checksum */
nvmwrite:
        bsr.s     nvmcrc
        tst.w     d0
        bne.s     nvmret
        movea.l   10(a7),a0
        bra.s     nvmwr2
nvmwr1:
        move.b    d1,(a1)      /* select register */
        move.b    (a0)+,(a2)   /* write value */
        addq.w    #1,d1        /* next reg */
nvmwr2:
        dbf       d2,nvmwr1
        bsr       nvmsum       /* calculate the checksum */
        move.b    #$3F,(a1)    /* checksum into reg 63 */
        move.b    d0,(a2)
        not.b     d0
        move.b    #$3E,(a1)    /* inverted checksum into reg 62 */
        move.b    d0,(a2)
        moveq.l   #0,d0        /* return E_OK */
nvmret:
        rts
/* read from NVRAM and also validate the checksum */
nvmread:
        bsr.s     nvmcrc
        cmp.w     #-5,d0       /* bad arguments? */
        beq.s     nvmret
        movea.l   10(a7),a0
        bra.s     nvmrd2
nvmrd1:
        move.b    d1,(a1)      /* select register */
        move.b    (a2),(a0)+   /* read value */
        addq.w    #1,d1        /* next reg */
nvmrd2:
        dbf       d2,nvmrd1
        rts
/* erase NVRAM and update the checksum */
nvminit:
        lea.l     (NVRAM_ADDR_REG).w,a1
        lea.l     (NVRAM_DATA_REG).w,a2
        moveq.l   #0,d0
        moveq.l   #14,d1
        moveq.l   #49,d2       /* fill 48 bytes plus checksum */
nvminit1:
        move.b    d1,(a1)      /* select register */
        move.b    d0,(a2)      /* clear */
        addq.w    #1,d1        /* next reg */
        dbf       d2,nvminit1
        move.b    #$3E,(a1)
        move.b    #$FF,(a2)    /* inverted checksum of 0 is 0xFF */
        rts

/*
 * check nvram crc and NVMaccess parameters
 */
nvmcrc:
        bsr.s     nvmsum
        move.b    d0,d1
        moveq.l   #-12,d0     /* return E_GENRL for CRC errors */
        move.b    #$3F,(a1)   /* register 63 contains the checksum */
        cmp.b     (a2),d1     /* crc2 ok? */
        bne.s     nvmcrc1     /* no */
        not.b     d1
        move.b    #$3E,(a1)   /* register 62 contains the inverted checksum */
        cmp.b     (a2),d1     /* crc1 ok? */
        bne.s     nvmcrc1     /* no */
        moveq.l   #-5,d0      /* return E_BADRQ for argument errors */
        move.w    10(a7),d1
        cmp.w     #$0030,d1   /* start in range? */
        bcc.s     nvmcrc1
        move.w    12(a7),d2
        bmi.s     nvmcrc1
        add.w     d1,d2
        cmp.w     #$0030,d2   /* start+count in range? */
        bhi.s     nvmcrc1
        moveq.l   #0,d0       /* return E_OK */
nvmcrc1:
        move.w    12(a7),d2
        move.w    10(a7),d1
        add.w     #$000E,d1   /* first data byte starts at offset 14 */
        rts

/*
 * sum up nvm data 14-62
 */
nvmsum:
        lea.l     (NVRAM_ADDR_REG).w,a1
        lea.l     (NVRAM_DATA_REG).w,a2
        moveq.l   #0,d0
        moveq.l   #14,d1
        moveq.l   #47,d2
nvmsum1:
        move.b    d1,(a1)
        add.b     (a2),d0
        addq.w    #1,d1
        dbf       d2,nvmsum1
        rts

#endif


#if MSTECLK

/* 206de: 00e01a96 */
ngetdt: bsr.s     checkRTC
        bcs.s     ngetdt2
        bsr.s     readRTCTime
        cmp.l     #-1,d0
        beq.s     ngetdt2
        moveq     #0,d0
ngetdt2:
        rts

#if STBOOK
checkRTC:
	movea.w   #rtc_sec,a0
        move.b    #$00,29(a0)               /* Clear RTC Test register */
        move.b    #$0c,31(a0)               /* Reset 1 Hz & 16 Hz alarm pulse */
        move.b    #$08,27(a0)               /* Clock Start, Alarm off, Bank 0 */
        moveq     #0,d0                     /* The RTC is always available */
        rts
#else
/* 206de: 00e01aa8 */
checkRTC:   movea.w   #rtc_sec,a0
            move.l    (_buserror).w,d2
            movea.l   sp,a2
            move.l    #chkRTC2,(_buserror).w
            bset      #0,27(a0)
            move.l    d2,(_buserror).w
#if !BINEXACT
/* preserve old value */
            movep.w   5(a0),d0
            move.w    d0,-(a7)
#endif
            move.w    #$a05,d0
            movep.w   d0,5(a0)
            movep.w   5(a0),d1
            and.w     #$f0f,d1
            cmp.w     d0,d1
#if BINEXACT
            bne.s     chkRTC3
#else
            bne.s     chkRTC2
/* restore old value */
            move.w    (a7)+,d0
            movep.w   d0,5(a0)
#endif
            move.b    #1,1(a0)
            bclr      #0,27(a0)
            move.b    #0,29(a0)
            rts
chkRTC2:    movea.l   a2,sp
            move.l    d2,(_buserror).w
chkRTC3:    ori       #1,ccr                    /* RTC not available */
            rts
#endif /* STBOOK */

/* 206de: 00e01af8 */
readRTCTime:bsr.s     checkRTC
            bcs       rdRTCErr
            lea       (rtcbufa).w,a1
            lea       (rtcbufb).w,a2
            bsr       rdRTCMask
rdRTC2:     exg       a1,a2
            bsr       rdRTCMask
            moveq     #$c,d0
rdRTC3:     move.b    (a1,d0.w),d1
            cmp.b     (a2,d0.w),d1
            bne.s     rdRTC2
            dbra      d0,rdRTC3
            moveq     #0,d0
            move.b    $b(a1),d0
            mulu.w    #10,d0
            add.b     $c(a1),d0
            asr.w     #1,d0
            move.w    d0,d1
            moveq     #0,d0
            move.b    9(a1),d0
            mulu.w    #10,d0
            add.b     $a(a1),d0
            asl.w     #5,d0
            add.w     d0,d1
            moveq     #0,d0
            move.b    7(a1),d0
            mulu.w    #10,d0
            add.b     8(a1),d0
            asl.w     #8,d0
            asl.w     #3,d0
            add.w     d0,d1
            swap      d1
            moveq     #0,d0
            move.b    4(a1),d0
            mulu.w    #10,d0
            add.b     5(a1),d0
            move.w    d0,d1
            moveq     #0,d0
            move.b    2(a1),d0
            mulu.w    #10,d0
            add.b     3(a1),d0
            asl.w     #5,d0
            add.w     d0,d1
            moveq     #0,d0
            move.b    (a1),d0
            mulu.w    #10,d0
            add.b     1(a1),d0
            asl.w     #8,d0
            asl.w     #1,d0
            add.w     d0,d1
            move      sr,d2
            ori       #$700,sr
            move.w    d1,_date
            swap      d1
            move.w    d1,_time
            move      d2,sr
            move.l    d1,d0
            rts

rdRTCMask:  moveq     #$c,d0
            moveq     #1,d1
rdRTCMk2:   move.b    (a0,d1.w),d2
            and.b     #$f,d2
            move.b    d2,(a1,d0.w)
            addq.w    #2,d1
            dbra      d0,rdRTCMk2
            rts

rdRTCErr:   moveq  #-1,d0
            rts

writeRTCTime:bsr      checkRTC
            bcs       no_RTC_found
            lea       (rtcbufa).w,a1
            movea.w   4(sp),a2
            bsr       rtcwday
            move.b    d0,6(a1)
            move.w    6(sp),d1
            move.w    d1,d0
            and.l     #$1f,d0
            add.w     d0,d0
            divu.w    #10,d0
            move.b    d0,11(a1)
            swap      d0
            move.b    d0,12(a1)
            move.w    d1,d0
            lsr.w     #5,d0
            and.l     #$3f,d0
            divu.w    #10,d0
            move.b    d0,9(a1)
            swap      d0
            move.b    d0,10(a1)
            lsr.w     #8,d1
            lsr.w     #3,d1
            ext.l     d1
            divu.w    #10,d1
            move.b    d1,7(a1)
            swap      d1
            move.b    d1,8(a1)
            move.w    4(sp),d1
            move.w    d1,d0
            and.l     #$1f,d0
            divu.w    #10,d0
            move.b    d0,4(a1)
            swap      d0
            move.b    d0,5(a1)
            move.w    d1,d0
            lsr.w     #5,d0
            and.l     #$f,d0
            divu.w    #10,d0
            move.b    d0,2(a1)
            swap      d0
            move.b    d0,3(a1)
            lsr.w     #1,d1
            lsr.w     #8,d1
            ext.l     d1
            move.l    d1,d2
            divu.w    #10,d1
            move.b    d1,(a1)
            swap      d1
            move.b    d1,1(a1)
            divu.w    #4,d2
            swap      d2
            move.b    #$e,$1f(a0)
            bset      #0,$1b(a0) /* select bank 1 */
            move.b    #1,$15(a0) /* set 24h mode */
            move.b    d2,$17(a0) /* set learyear register */
            bclr      #0,$1b(a0) /* select bank 0 */
            bclr      #3,$1b(a0) /* stop the clock */
            moveq     #$c,d0
            moveq     #1,d1
wrRTC2:     move.b    (a1,d0.w),(a0,d1.w)
            addq.w    #2,d1
            dbra      d0,wrRTC2
            bset      #3,$1b(a0) /* start the clock */
            moveq     #0,d0
            rts

no_RTC_found:moveq    #-1,d0
            rts

rtcwday:    moveq     #2,d2
            move.w    a2,d0
            lsr.w     #8,d0
            lsr.w     #1,d0
            add.w     d0,d2
            move.w    d0,d1
            lsr.w     #2,d1
            add.w     d1,d2
            move.w    a2,d1
            lsr.w     #5,d1
            and.w     #$f,d1
            and.w     #3,d0
            bne.s     rtcwday2
            cmp.w     #2,d1
            bhi.s     rtcwday2
            subq.w    #1,d2
rtcwday2:   subq.w #1,d1
            add.w     d1,d1
            add.w     daysOffsetToMonths(pc,d1.w),d2
            move.w    a2,d1
            and.w     #$1f,d1
            add.w     d1,d2
            divu.w    #7,d2
            swap      d2
            moveq     #0,d0
            move.w    d2,d0
            rts

daysOffsetToMonths:DC.W $0000,$001f,$003b,$005a
            DC.W      $0078,$0097,$00b5,$00d4
            DC.W      $00f3,$0111,$0130,$014e

#endif /* MSTECLK */


#if MSTECLK
/*
 * XBIOS #47 - Waketime
 */
/* 206de: 00e01d0a */
_waketime:  move.l    4(sp),d0
            beq       clrwtim
            cmp.l     #-1,d0
            beq       readRTCWaketime
            cmp.l     #1,d0
            beq       enawtim
            bsr       readRTCTime
            cmp.l     #-1,d0
            beq       enawtimDone
            move.l    4(sp),d1
            and.b     #$e0,d0
            and.b     #$e0,d1
            cmp.l     d1,d0                     /* is the new wakeup time already in the past? */
            bcc.s     waketim5
            move.l    waketimeAlarm,d2
            beq.s     waketim2
            cmp.l     d2,d0
            bhi.s     waketim2
            cmp.l     d2,d1
            bcc.s     waketim6
waketim2:   movea.l   d0,a0
            move.l    d1,d2
            and.l     #$1fffff,d2
            and.l     #$1fffff,d0
            cmp.l     d2,d0                     /* new wakeup time is _now_? */
            beq.s     waketim3
            move.l    d1,waketimeAlarm
            moveq     #0,d0
            rts

waketim3: and.l     #$1f0000,d2
            cmp.l     #$10000,d2
            beq.s     waketim4
            and.l     #$ffe0ffff,d1
            or.l      #$10000,d1
            move.l    d1,waketimeAlarm
            moveq     #4,d0
            rts

waketim4:   move.l a0,d0
            add.l     #$10000,d0
            move.l    d0,waketimeAlarm
            moveq     #5,d0
            rts

waketim5:   moveq  #1,d0
            rts

waketim6:   moveq  #2,d0
            rts

readRTCWaketime:move.l waketimeAlarm,d0
            rts

clrwtim:    bsr       checkRTC
            bcs       no_RTC_found
            bclr      #2,$1b(a0)                /* RTC Alarm off */
            move.b    #$d,$1f(a0)               /* Reset 1Hz, 16 Hz and Alarm */
            clr.l     waketimeAlarm
            moveq     #3,d0
            rts

enawtim:    bsr       readRTCTime
            cmp.l     #-1,d0
            beq       enawtimDone
            move.l    waketimeAlarm,d1
            beq       enawtimDone
            and.b     #$e0,d1
            move.l    d1,d2
            and.b     #$e0,d0
            cmp.l     d1,d0                     /* is the new wakeup time already in the past? */
            bhi.s     enawtimDone
            bne.s     enawtim2
            and.b     #$1f,d2
            cmp.b     #7,d2
            bcs.s     enawtim3
enawtim2:   movea.w   #rtc_sec,a0
            bset      #0,$1b(a0)                /* RTC select Bank 1 */
            move.l    waketimeAlarm,d1
            move.w    d1,d0
            lsr.w     #5,d0                     /* Minutes */
            and.l     #$3f,d0
            divu.w    #10,d0
            move.b    d0,7(a0)                  /* RTC 1-minute alarm register */
            swap      d0
            move.b    d0,5(a0)                  /* RTC 10-minute alarm register */
            move.w    d1,d0
            lsr.w     #8,d0
            lsr.w     #3,d0                     /* Hours */
            ext.l     d0
            divu.w    #10,d0
            move.b    d0,$b(a0)                 /* RTC 1-hour alarm register */
            swap      d0
            move.b    d0,9(a0)                  /* RTC 10-hour alarm register */
            move.l    d1,d0
            swap      d0
            and.w     #$1f,d0                   /* Days */
            ext.l     d0
            divu.w    #10,d0
            move.b    d0,$11(a0)                /* RTC 1-hour alarm register */
            swap      d0
            move.b    d0,$f(a0)                 /* RTC 10-day alarm register */
            movea.w   waketimeAlarm,a2
            bsr       rtcwday
            move.b    d0,$d(a0)                 /* RTC day-of-the-week alarm register */
            bset      #2,$1b(a0)                /* RTC Alarm on */
            bclr      #0,$1b(a0)                /* RTC select Bank 0 */
enawtimDone:moveq     #0,d0
            rts

enawtim3:   clr.l     waketimeAlarm
            moveq     #-1,d0
            rts
#endif /* MSTECLK */

#if 0 /* original functions from tospatch; no longer needed */

/*
 ********************************************************************************
 * RTC-Patches:
 * Die Megauhr soll ja wohl auch ausgelesen werden, oder?
 * Da das 3.06 natrlich nichts von der Mega-Uhr weiž, mssen hier andere
 * Routinen rein.
 *******************************************************************************
 */
xngetdt:
      bsr       checkRTC
      bcs.s     ngetdt1
      bclr      #0,$1b(a0)
      clr.l     d0
      moveq     #1,d1
      bsr.s     ngetdt2
      asr.l     #1,d1
      bfins     d1,d0{27:5}
      moveq     #5,d1
      bsr.s     ngetdt2
      bfins     d1,d0{21:6}
      moveq     #9,d1
      bsr.s     ngetdt2
      bfins     d1,d0{16:5}
      moveq     #$f,d1
      bsr.s     ngetdt2
      bfins     d1,d0{11:5}
      moveq     #$13,d1
      bsr.s     ngetdt2
      bfins     d1,d0{7:4}
      moveq     #$17,d1
      bsr.s     ngetdt2
      bfins     d1,d0{0:7}
      move      sr,d2
      ori.w     #$700,sr
      move.w    d0,_time.w
      swap      d0
      move.w    d0,_date
      swap      d0
      move      d2,sr
ngetdt1:
      rts
ngetdt2:
      move.b    0(a0,d1.w),d2
      bsr.s     ngetdt3
      andi.b    #$f,d2
      move.b    2(a0,d1.w),d1
      andi.b    #$f,d1
      mulu      #$a,d1
      add.b     d2,d1
ngetdt3:
      move.l    d0,-(a7)
      move.w    #$101,d0
      jsr       mfpdelay
      move.l    (a7)+,d0
      rts
ngetdt4:
      movea.l   a2,a7
ngetdt5:
      move.l    a1,(_buserror).w
      ori.b     #1,ccr
      rts
ngetdt6:
      move.l    a1,(_buserror).w
      move.b    #1,1(a0)
      move.b    #0,$1d(a0)
      andi.b    #-2,ccr
      rts
	  dc.b 0,0,0,0


/*
 *****************************************************************
 * se: (20.09.93)
 * rtc auf neue zeit einstellen.
 * erharlt die zeit im gemdos-format auf dem stack (4(sp)).
 */

writeRTCTime:
      bsr       checkRTC
      bcs       writeR9
      bclr      #0,$1b(a0)
      move.l    4(a7),d0
      bfextu    d0{27:5},d1
      andi.l    #$1f,d1
      asl.l     #1,d1
      moveq     #1,d2
      bsr.s     writeR8
      bfextu    d0{21:6},d1
      andi.l    #$3f,d1
      moveq     #5,d2
      bsr.s     writeR8
      bfextu    d0{16:5},d1
      andi.l    #$1f,d1
      moveq     #9,d2
      bsr.s     writeR8
      bfextu    d0{11:5},d1
      andi.l    #$1f,d1
      moveq     #$f,d2
      bsr.s     writeR8
      bfextu    d0{7:4},d1
      andi.l    #$f,d1
      moveq     #$13,d2
      bsr.s     writeR8
      bfextu    d0{0:7},d1
      andi.l    #$7f,d1
      moveq     #$17,d2
writeR8:
      move.l    d3,-(a7)
      clr.l     d3
      move.b    d1,d3
      divu      #$a,d3
      mulu      #$a,d3
      sub.b     d3,d1
      divu      #$a,d3
      move.b    d1,0(a0,d2.w)
      move.b    d3,2(a0,d2.w)
      move.l    (a7)+,d3
writeR9:
      rts
writeR10:
      cmp.w     d1,d2
      bne       ngetdt5
      movep.w   d0,5(a0)
      bra       ngetdt6
      dc.b 0,0,0,0

/*
 *******************************************************************************
 * se: (20.09.93)
 * check, ob die uhr da ist.
 */
checkRTC:
      movea.l   a7,a2
      movea.l   (_buserror).w,a1
      lea       ngetdt4(pc),a0          
      move.l    a0,(_buserror).w
      movea.w   #rtc_sec,a0
      bset      #0,$1b(a0)
      movep.w   5(a0),d0
      move.w    #$a05,d1
      movep.w   d1,5(a0)
      movep.w   5(a0),d2
      andi.w    #$f0f,d2
      bra.s     writeR10
#endif

	bss

#if MSTECLK
	xdef rtcbufa
	comm rtcbufa,13
	xdef rtcbufb
	comm rtcbufb,13
	xdef waketimeAlarm
	comm waketimeAlarm,4
#endif
