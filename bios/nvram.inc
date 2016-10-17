/* 306de: 00e01ef0 */
ngetdt:
        bsr       detect_nvram
        bcs.s     inittc2
        move.b    #$0D,(NVRAM_ADDR_REG).w /* select register D */
        move.b    (NVRAM_DATA_REG).w,d0
        btst      #7,d0                   /* is RAM & time valid? */
        bne.s     inittc1                 /* yes */
        move.l    #$12C80000,-(a7)        /* (9<<9)+(6<<5)+8 */
        bsr       wrttclock
        addq.w    #4,a7
inittc1:
        bsr.s     rdttclock
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
rdttclock:
        bsr       detect_nvram
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
wrttclock:
        bsr       detect_nvram
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
detect_nvram:
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
 * XBIOS +42 - NVMaccess
 */
/* 306de: 00e0208c */
_nvmaccess:
        moveq.l   #-5,d0       /* return E_BADRQ in case of error */
        move.w    4(a7),d1
        beq.s     nvmread
        cmp.w     #$0002,d1
        beq.s     nvminit
        bhi.s     nvmret
nvmwrite:
        bsr.s     nvmcrc
        tst.w     d0
        bne.s     nvmret
        movea.l   10(a7),a0
        bra.s     nvmwr2
nvmwr1:
        move.b    d1,(a1)
        move.b    (a0)+,(a2)
        addq.w    #1,d1
nvmwr2:
        dbf       d2,nvmwr1
        bsr       nvmsum
        move.b    #$3F,(a1)
        move.b    d0,(a2)
        not.b     d0
        move.b    #$3E,(a1)
        move.b    d0,(a2)
        moveq.l   #0,d0        /* return E_OK */
nvmret:
        rts
nvmread:
        bsr.s     nvmcrc
        cmp.w     #-5,d0       /* bad arguments? */
        beq.s     nvmret
        movea.l   10(a7),a0
        bra.s     nvmrd2
nvmrd1:
        move.b    d1,(a1)
        move.b    (a2),(a0)+
        addq.w    #1,d1
nvmrd2:
        dbf       d2,nvmrd1
        rts

nvminit:
        lea.l     (NVRAM_ADDR_REG).w,a1
        lea.l     (NVRAM_DATA_REG).w,a2
        moveq.l   #0,d0
        moveq.l   #14,d1
        moveq.l   #49,d2
nvminit1:
        move.b    d1,(a1)
        move.b    d0,(a2)
        addq.w    #1,d1
        dbf       d2,nvminit1
        move.b    #$3E,(a1)
        move.b    #$FF,(a2)
        rts

/*
 * check nvram crc and NVMaccess parameters
 */
nvmcrc:
        bsr.s     nvmsum
        move.b    d0,d1
        moveq.l   #-12,d0     /* return E_GENRL for CRC errors */
        move.b    #$3F,(a1)
        cmp.b     (a2),d1     /* crc2 ok? */
        bne.s     nvmcrc1     /* no */
        not.b     d1
        move.b    #$3E,(a1)
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


