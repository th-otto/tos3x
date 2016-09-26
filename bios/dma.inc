/*
 * XBIOS #42 - DMAread
 */
/* 306de: 00e0186c */
_dmaread:
        move.b    #$00,dmarwflg
        move.w    #$0008,d1
        bra.s     dmarw
/*
 * XBIOS #43 - DMAwrite
 */
/* 306de: 00e0187a */
_dmawrite:
        move.b    #$01,dmarwflg
        move.w    #$000A,d1
dmarw:
        cmpi.w    #$0010,14(a7) ; devno <= 16?
        bhi       dmaundev      ; no, fail
        cmpi.w    #$000F,14(a7) ; devno <= 15?
        bhi.s     dmarw2        ; no, goto ide case
        lea.l     scsicmd,a0
        move.b    d1,(a0)+      ; command
        move.b    5(a7),(a0)+   ; sector>>16
        move.b    6(a7),(a0)+   ; sector>>8
        move.b    7(a7),(a0)+   ; sector
        move.b    9(a7),(a0)+   ; count
        clr.b     (a0)          ; flags
        move.w    14(a7),d0     ; get device
        moveq.l   #0,d1
        move.w    8(a7),d1      ; get count
        lsl.l     #8,d1
        add.l     d1,d1         ; d1 = transfer size (sector count * 512)
        moveq.l   #6,d2
        movea.l   10(a7),a0
        tst.b     dmarwflg
        bne.s     dmarw1
        bsr       dodmaread
        bra       dmarw7
dmarw1:
        bsr       dodmawrite
        bra       dmarw7
dmabuserr:
        movea.l   a1,a7
        move.l    a0,(_buserror).w
        bra       dmaundev
dmarw2:
        movea.l   (_buserror).w,a0
        movea.l   a7,a1
        move.l    #dmabuserr,(_buserror).w
        tst.b     $FFF00039
        movea.l   a1,a7
        move.l    a0,(_buserror).w
        move.b    15(a7),d0
        bsr       iderdy
        beq       dmaundev
        moveq.l   #2,d0
        add.l     (_hz_200).w,d0
dmarw3:
        cmp.l     (_hz_200).w,d0
        bcc.s     dmarw3
        pea.l     idebuf
        move.w    18(a7),-(a7)
        bsr       ideidentify
        addq.w    #6,a7
        tst.w     d0
        bmi       dmaret
        bne.s     dmarw8
        moveq.l   #2,d0
        add.l     (_hz_200).w,d0
dmarw4:
        cmp.l     (_hz_200).w,d0
        bcc.s     dmarw4
        lea.l     idebuf,a0
        move.w    12(a0),-(a7)
        move.w    6(a0),-(a7)
        move.w    18(a7),-(a7)
        bsr       idecount
        addq.w    #6,a7
        tst.w     d0
        bmi.s     dmaret
        bne.s     dmarw8
        lea.l     idebuf,a0
        move.w    14(a7),-(a7)   ; device number
        move.l    12(a7),-(a7)   ; buffer
        move.w    14(a7),-(a7)   ; count
        move.l    12(a7),-(a7)   ; start sector number
        move.w    12(a0),-(a7)   ; device number of sectors
        move.w    6(a0),-(a7)    ; device number of heads
        tst.b     dmarwflg
        bne.s     dmarw5
        bsr       ideread
        bra.s     dmarw6
dmarw5:
        bsr       idewrite
dmarw6:
        adda.w    #$0010,a7
dmarw7:
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        CACR_D1
        ori.w     #$0808,d1
        D1_CACR
        move.w    (a7)+,sr
        tst.w     d0
        ble.s     dmaret
dmarw8:
        moveq.l   #-11,d0 ; E_READF
        tst.b     dmarwflg
        beq.s     dmaret
        moveq.l   #-10,d0 ; E_WRITF
dmaret:
        rts
dmaundev:
        moveq.l   #-15,d0 ; E_UNDEV
        bra.s     dmaret

/* 306de: 00e019ba */
dodmaread:
        cmp.w     #$0007,d0
        bls.s     domdmar1
        bsr       scsiread
        rts
domdmar1:
        bsr       acsiread
        rts

/* 306de: 00e019cc */
dodmawrite:
        cmp.w     #$0007,d0
        bls.s     dodmaw1
        bsr       scsiwrite
        rts
dodmaw1:
        bsr       acsiwrite
        rts

/* 306de: 00e019de */
dmadelay:
        move.l    #$00000258,d0
        bra.s     dmadel2
dmadel1:
        moveq.l   #20,d0
dmadel2:
        add.l     (_hz_200).w,d0
dmadel3:
        cmp.l     (_hz_200).w,d0
        bcs.s     dmaerr
        btst      #5,(MFP_GPIP).w
        bne.s     dmadel3
        moveq.l   #0,d0
        rts
dmaerr:
        moveq.l   #-1,d0
        rts

/* 306de: 00e01a02 */
acsiabort:
        bsr.s     dmadelay
        bmi.s     acsiab1
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        CACR_D0
        ori.w     #$0800,d0
        D0_CACR
        move.w    (a7)+,sr
        move.w    d1,($FFFF8606).w
        move.w    ($FFFF8604).w,d0
        and.w     #$00FF,d0
acsiab1:
        move.l    (_hz_200).w,nextacsi
        addq.l    #2,nextacsi
        rts

/* 306de: 00e01a36 */
acsiend:
        move.w    #$0080,($FFFF8606).w
        clr.w     (_flock).w
        rts

/* 00e01a42 */
acsibeg:
        move.l    d0,-(a7)
        move.l    nextacsi,d0
acsibeg1:
        cmp.l     (_hz_200).w,d0
        bcc.s     acsibeg1
        move.l    (a7)+,d0
        rts

/* 306de: 00e01a54 */
acsiread:
        movea.l   (_buserror).w,a1
        movea.l   a7,a2
        move.l    #acsierr,(_buserror).w
        tst.w     ($FFFF8606).w
        move.l    a1,(_buserror).w
        movea.l   a2,a7
        st        (_flock).w
        bsr.s     acsibeg
        movea.w   #$8604,a1
        bsr       setdmaaddr
        move.w    #$0190,2(a1)
        bsr       shortdelay
        move.w    #$0090,2(a1)
        bsr       shortdelay
        bsr       acsisize
        lea.l     scsicmd,a0
        moveq.l   #0,d1
        bsr.s     acsistat
        bra.s     acsiend

/* 306de: 00e01a9e */
acsiwrite:
        movea.l   (_buserror).w,a1
        movea.l   a7,a2
        move.l    #acsierr,(_buserror).w
        tst.w     ($FFFF8604).w
        move.l    a1,(_buserror).w
        movea.l   a2,a7
        st        (_flock).w
        bsr.s     acsibeg
        movea.w   #$8604,a1
        bsr.s     setdmaaddr
        move.w    #$0090,2(a1)
        bsr       shortdelay
        move.w    #$0190,2(a1)
        bsr       shortdelay
        bsr.s     acsisize
        move.l    #$00000100,d1
        bsr.s     acsistat
        bra       acsiend

/* 306de: 00e01ae4 */
acsierr:
        moveq.l   #-15,d0 ; E_UNDEV
        movea.l   a2,a7
        move.l    a1,(_buserror).w
        rts

/* 00e01aee */
acsistat:
        move.b    #$88,d1
        move.w    d1,2(a1)
        move.b    #$8A,d1
        lea.l     scsicmd,a2
        lsl.b     #5,d0
        or.b      d0,(a2)
        subq.w    #2,d2
acsista1:
        swap      d1
        move.b    (a2)+,d1
        swap      d1
        move.l    d1,(a1)
        bsr       dmadel1
        bmi.s     acsista2
        dbf       d2,acsista1
        move.w    d1,2(a1)
        move.b    #$00,d1
        swap      d1
        move.b    (a2),d1
        swap      d1
        move.l    d1,(a1)
        move.b    #$8A,d1
        bsr       acsiabort
acsista2:
        rts

/* 306de: 00e01b32 */
setdmaaddr:
        move.l    a0,-(a7)
        move.b    3(a7),($FFFF860D).w
        move.b    2(a7),($FFFF860B).w
        move.b    1(a7),($FFFF8609).w
        addq.l    #4,a7
        rts

/* 306de: 00e01b4a */
acsisize:
        move.l    d1,-(a7)
        lsr.l     #8,d1
        add.l     d1,d1
        move.w    d1,(a1)
        move.l    (a7)+,d1
        rts

/* 306de: 00e01b56 */
shortdelay:
        tst.b     (MFP_REGS).w
        tst.b     (MFP_REGS).w
        tst.b     (MFP_REGS).w
        tst.b     (MFP_REGS).w
        rts


/* 306de: 00e01b68 */
scsiread:
        move.l    a0,-(a7)
        andi.w    #$0007,d0
        bsr.s     scsisend
        movea.l   (a7)+,a0
        tst.w     d0
        bmi.s     scsird2
        movea.l   a0,a1
        movea.w   #$8781,a2
        move.b    #$01,($FFFF8787).w
        move.b    ($FFFF878F).w,d0
scsird1:
        bsr       sdelaytime
        bsr       scsibsy
        bmi.s     scsird2
        btst      #3,10(a2) ; phase match?
        beq       scsiabort
        move.b    (a2),(a1)+
        bsr       scsiack
        bra.s     scsird1
scsird2:
        rts

/* 306de: 00e01ba4 */
scsiwrite:
        andi.w    #$0007,d0
        move.l    a0,-(a7)
        move.w    d2,-(a7)
        bsr.s     scsisend
        move.w    (a7)+,d2
        movea.l   (a7)+,a0
        tst.w     d0
        bmi.s     scsiwr2
        movea.l   a0,a1
        movea.w   #$8781,a2
        move.b    #$00,($FFFF8787).w
        move.b    ($FFFF878F).w,d0
scsiwr1:
        bsr       sdelaytime
        bsr       scsibsy
        bmi.s     scsiwr2
        btst      #3,10(a2) ; phase match?
        beq       scsiabort
        move.b    (a1)+,(a2)
        bsr       scsiack
        bra.s     scsiwr1
scsiwr2:
        rts

/* 306de: 00e01be4 */
scsisend:
        move.l    d2,-(a7)
        move.w    d0,-(a7)
        bsr.s     scsiphase
        addq.w    #2,a7
        move.l    (a7)+,d2
        tst.w     d0
        bmi.s     scsisen2
        move.b    #$02,($FFFF8787).w
        move.b    #$01,($FFFF8783).w
        lea.l     scsicmd,a1
        subq.w    #1,d2
        bsr       sdelaytime
scsisen1:
        move.b    (a1)+,d0
        bsr       scsiwrbyte
        tst.w     d0
        bmi.s     scsisen2
        dbf       d2,scsisen1
        moveq.l   #0,d0
scsisen2:
        rts

/* 306de: 00e01c1c */
scsiphase:
        bsr       sdelaytime
scsipha1:
        btst      #6,($FFFF8789).w ; BSY set?
        beq.s     scsipha2
        cmp.l     (a0),d1
        bhi.s     scsipha1
        bra.s     scsipha4
scsipha2:
        move.b    #$00,($FFFF8787).w
        move.b    #$00,($FFFF8789).w
        move.b    #$0C,($FFFF8783).w
        clr.w     d0
        move.w    4(a7),d1
        bset      d1,d0
        move.b    d0,($FFFF8781).w
        move.b    #$0D,($FFFF8783).w
        andi.b    #$FE,($FFFF8785).w
        andi.b    #$F7,($FFFF8783).w
        bsr       sdelaytime
scsipha3:
        btst      #6,($FFFF8789).w ; BSY set?
        bne.s     scsipha5
        cmp.l     (a0),d1
        bhi.s     scsipha3
scsipha4:
        moveq.l   #-1,d0
        bra.s     scsipha6
scsipha5:
        clr.w     d0
scsipha6:
        move.b    #$00,($FFFF8783).w
        rts

/* 306de: 00e01c7c */
scsiabort:
        bsr       sdelaytime
        move.b    #$03,($FFFF8787).w
        move.b    ($FFFF878F).w,d0
        bsr.s     scsibsy
        bmi.s     scsiab3
        moveq.l   #0,d0
        move.b    ($FFFF8781).w,d0
        bsr       sdelaytime
        move.l    d0,-(a7)
        bsr.s     scsiack
        tst.w     d0
        beq.s     scsiab2
scsiab1:
        addq.l    #4,a7
        bra.s     scsiab3
scsiab2:
        bsr.s     sdelaytime
        bsr.s     scsibsy
        bmi.s     scsiab1
        move.b    ($FFFF8781).w,d0
        bsr.s     scsiack
        tst.w     d0
        bmi.s     scsiab1
        move.l    (a7)+,d0
scsiab3:
        rts

/* 00e01cb8 */
scsibsy:
        btst      #5,($FFFF8789).w  ; data transfer pending?
        bne.s     scsibsy1
        cmp.l     (a0),d1
        bhi.s     scsibsy
        moveq.l   #-1,d0
        bra.s     scsibsy2
scsibsy1:
        moveq.l   #0,d0
scsibsy2:
        rts

/* 00e01ccc */
scsiack:
        ori.b     #$11,($FFFF8783).w ; assert ACK
scsiack1:
        btst      #5,($FFFF8789).w  ; data transfer pending?
        beq.s     scsiack2
        cmp.l     (a0),d1
        bhi.s     scsiack1
        moveq.l   #-1,d0
        bra.s     scsiack3
scsiack2:
        moveq.l   #0,d0
scsiack3:
        andi.b    #$EF,($FFFF8783).w ; de-assert ACK
        rts

/* 306de: 00e01cec */
scsiwrbyte:
        move.w    d0,-(a7)
        bsr.s     scsibsy
        bmi.s     scsiwrb1
        move.b    1(a7),($FFFF8781).w
        bsr.s     scsiack
scsiwrb1:
        addq.l    #2,a7
        rts

/* 306de: 00e01cfe */
scsirst:
        move.b    #$80,($FFFF8783).w  ; assert RST
        bsr.s     sdelaytime
scsirst1:
        cmp.l     (a0),d1
        bhi.s     scsirst1
        move.b    #$00,($FFFF8783).w
        bsr.s     ldelaytime
scsirst2:
        cmp.l     (a0),d1
        bhi.s     scsirst2
        rts

/* 306de: 00e01d18 */
sdelaytime:
        movea.w   #_hz_200,a0
        moveq.l   #51,d1
        add.l     (a0),d1
        rts

/* 306de: 00e01d22 */
ldelaytime:
        movea.w   #_hz_200,a0
        move.l    #201,d1
        add.l     (a0),d1
        rts

/* 306de: 00e01d30 */
idestat:
        move.l    #2000,d0
        add.l     (_hz_200).w,d0
idestat1:
        btst      #5,(MFP_GPIP).w
        beq.s     idestat2
        cmp.l     (_hz_200).w,d0
        bhi.s     idestat1
        moveq.l   #-1,d0
        bra.s     idestat4
idestat2:
        moveq.l   #0,d0
        move.b    $FFF0001D,d0 ; read status register
        btst      #0,d0        ; error occured?
        bne.s     idestat3     ; yes, ho read error
        btst      #3,d0        ; data request ready?
        bne.s     idestat4
        moveq.l   #0,d0
        bra.s     idestat4
idestat3:
        move.b    $FFF00005,d0 ; read error register
idestat4:
        rts

/* 306de: 00e01d6c */
ideread:
        bsr       idechs
        movea.l   14(a7),a0
        move.b    13(a7),$FFF00009
        move.w    12(a7),d1
        subq.w    #1,d1
        move.b    #$00,$FFF00039
        move.b    #$20,$FFF0001D
iderd1:
        bsr.s     idestat
        tst.w     d0
        bmi.s     iderd2
        btst      #3,d0
        beq.s     iderd2
        bsr       iderdsec
        dbf       d1,iderd1
        moveq.l   #0,d0
iderd2:
        rts

/* 306de: 00e01daa */
idewrite:
        bsr.s     idechs
        movea.l   14(a7),a0
        move.b    13(a7),$FFF00009
        move.w    12(a7),d1
        subq.w    #1,d1
        move.b    #$00,$FFF00039
        move.b    #$30,$FFF0001D
idewr1:
        btst      #3,$FFF00039
        beq.s     idewr1
idewr2:
        bsr       idewrsec
        bsr       idestat
        tst.w     d0
        bmi.s     idewr3
        btst      #3,d0
        beq.s     idewr3
        dbf       d1,idewr2
        moveq.l   #0,d0
idewr3:
        rts

/* 306de: 00e01df2 */
idechs:
        move.l    12(a7),d1        ; get sector number
        move.w    8(a7),d2         ; get number of heads
        move.w    10(a7),d0        ; get number of sectors
        mulu.w    d0,d2            ; calc number of sectors / cylinder
        divu.w    d2,d1            ; calc cylinder #
        move.b    d1,$FFF00011     ; write cylinder low
        lsr.l     #8,d1
        move.b    d1,$FFF00015     ; write cylinder high
        lsr.l     #8,d1            ; d1 now remainder of division above
        divu.w    d0,d1            ; calc head #
        move.w    22(a7),d0        ; get device number
        andi.b    #$07,d0
        lsl.b     #4,d0
        or.b      d0,d1
        move.b    d1,$FFF00019     ; select device & head
        swap      d1
        addq.w    #1,d1
        move.b    d1,$FFF0000D     ; write sector number
        rts

/* 306de: 00e01e32 */
idecount:
        move.w    4(a7),d0
        andi.b    #$07,d0
        lsl.w     #4,d0
        move.b    d0,$FFF00019     ; select device
        move.w    6(a7),d0
        subq.b    #1,d0
        or.b      d0,$FFF00019     ; select head
        move.b    9(a7),$FFF00009  ; select sector count
        move.b    #$00,$FFF00039
        move.b    #$91,$FFF0001D
        bra       idestat

/* 306de: 00e01e6a */
ideidentify:
        move.w    4(a7),d0         ; get device
        andi.b    #$07,d0
        lsl.b     #4,d0
        move.b    d0,$FFF00019     ; select device
        movea.l   6(a7),a0
        move.b    #$00,$FFF00039
        move.b    #$EC,$FFF0001D   ; send ATA IDENTIFY command
        bsr       idestat
        tst.w     d0
        bmi.s     ideiden1
        btst      #3,d0
        beq.s     ideiden1
        bsr.s     iderdsec         ; read the identify packet
        moveq.l   #0,d0
ideiden1:
        rts

/*
 * read 1 sector from IDE data register
 */
/* 306de: 00e01ea2 */
iderdsec:
        moveq.l   #127,d0
        lea.l     $FFF00000,a1
iderds1:
        move.l    (a1),(a0)+
        dbf       d0,iderds1
        rts

/*
 * write 1 sector to IDE data register
 */
/* 306de: 00e01eb2 */
idewrsec:
        moveq.l   #127,d0
        lea.l     $FFF00000,a1
idewrs1:
        move.l    (a0)+,(a1)
        dbf       d0,idewrs1
        rts

/*
 * check for IDE ready, with timeout of ~5 sec
 */
/* 306de: 00e01ec2 */
iderdy:
        andi.b    #$07,d0
        lsl.b     #4,d0
        move.b    d0,$FFF00019     ; select device
        move.l    #1000,d0
        add.l     (_hz_200).w,d0
iderdy1:
        btst      #6,$FFF00039     ; status drive ready?
        bne.s     iderdy2          ; yes, report ok
        cmp.l     (_hz_200).w,d0   ; timeout reached?
        bcc.s     iderdy1          ; no, try again
        moveq.l   #0,d0            ; report failure
        rts
iderdy2:
        moveq.l   #1,d0
        rts
