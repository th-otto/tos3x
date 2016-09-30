*
* flopini - initialize floppies
* int flopini(void *buffer, DSB *dsb, int devno)
*
* Returns:       EQ if initialization succeeded (drive attached).
*                NE if initialization failed (no drive attached).
*
/* 306de: 00e03ba8 */
_flopini:
        clr.l     fd_frb
        lea.l     dsb,a1
        tst.w     12(a7)
        beq.s     flopini1
        lea.l     dsb+8,a1
flopini1:
        move.w    _seekrat,fd_seekrate(a1)
        move.w    #$0003,fd_density(a1)
        moveq.l   #-1,d0
        clr.w     fd_curtrack(a1)
        clr.w     fd_cscnt
        bsr       floplock
        bsr       select
        move.w    #$FF00,fd_curtrack(a1)
        bsr       restore
        beq.s     flopini2
        moveq.l   #10,d7
        bsr       hardsee1
        bne.s     flopini3
        bsr       restore
flopini2:
        beq       flopok
flopini3:
        bra       flopfail
        move.l    a0,($00000008).w
        movea.l   a1,a7
        movem.l   (a7)+,d7/a6
flopini4:
        moveq.l   #-15,d0 ; E_UNDEV
        rts


/*
 * XBIOS #8 - Floprd - Read individual sectors from floppy disk.
 */
/* 306de: 00e03c10 */
_floprd:
        tst.w     (_nflops).w
        beq.s     flopini4
        bsr       fdchange
        moveq.l   #-11,d0 ; E_READF
        bsr       frbrdcheck
        bsr       floplock
floprd1:
        bsr       select
        bsr       go2track
        bcs       flopfail
        bne       floprd6
floprd2:
        move.w    #-1,fd_curerr ; E_ERR
        move.w    #$0190,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0090,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0001,(DMA_DATA).w
        move.w    #$0080,(a6)
        move.w    #$0080,d7
        bsr       wrfdcd7
        move.l    (_hz_200).w,d7
        add.l     #$0000012C,d7
floprd3:
        btst      #5,(MFP_GPIP).w
        beq.s     floprd4
        cmp.l     (_hz_200).w,d7
        bhi.s     floprd3
        move.w    #-2,(fd_curerr).w
        bsr       fdcreset
        bra.s     floprd6
floprd4:
        move.w    #$0090,(a6)
        move.w    (a6),d0
        btst      #0,d0
        beq.s     floprd6
        move.w    #$0080,(a6)
        bsr       rdfdcd0
        and.b     #$1C,d0
        bne.s     floprd5
        move.w    #$0002,(fd_retry).w
        addq.w    #1,(fd_sect).w
        addi.l    #$00000200,(fd_buffer).w
        subq.w    #1,(fd_scount).w
        beq       flopok
        bsr       select1
        bra       floprd2
floprd5:
        bsr.s     fdcerr
floprd6:
        cmpi.w    #$0001,(fd_retry).w
        bne.s     floprd7
        bsr       reseek
floprd7:
        subq.w    #1,(fd_retry).w
        bpl       floprd1
        bra       flopfail

fdcerr:
        moveq.l   #-13,d1 ; E_WRPRO
        btst      #6,d0
        bne.s     fdcerr1
        moveq.l   #-8,d1  ; E_SEEK
        btst      #4,d0
        bne.s     fdcerr1
        moveq.l   #-4,d1  ; E_CRC
        btst      #3,d0
        bne.s     fdcerr1
        move.w    (fd_deferr).w,d1
fdcerr1:
        move.w    d1,(fd_curerr).w
        rts


/*
 * XBIOS #9 - Flopwr - Write individual sectors on a floppy disk.
 */
/* 306de: 00e03d12 */
_flopwrt:
        tst.w     (_nflops).w
        beq.w     flopini4
        bsr       fdchange
        moveq.l   #-10,d0 ; E_WRITF
        bsr       frbwrcheck
        bsr       floplock
        move.w    (fd_sect).w,d0
        subq.w    #1,d0
        or.w      (fd_track).w,d0
        or.w      (fd_side).w,d0
        bne.s     flopwrt1
        moveq.l   #2,d0
        bsr       setdchg
flopwrt1:
        bsr       select
        bsr       go2track
        bcs       flopfail
        bne       flopwrt6
flopwrt2:
        move.w    #-1,(fd_curerr).w
        move.w    #$0090,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0190,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0001,d7
        bsr       wrfdcd7
        move.w    #$0180,(a6)
        move.w    #$00A0,d7
        bsr       wrfdcd7
        move.l    (_hz_200).w,d7
        add.l     #$0000012C,d7
flopwrt3:
        btst      #5,(MFP_GPIP).w
        beq.s     flopwrt4
        cmp.l     (_hz_200).w,d7
        bhi.s     flopwrt3
        bsr       fdcreset
        bra.s     flopwrt5
flopwrt4:
        move.w    #$0180,(a6)
        bsr       rdfdcd0
        bsr       fdcerr
        btst      #6,d0
        bne       flopfail
        and.b     #$5C,d0
        bne.s     flopwrt5
        move.w    #$0002,(fd_retry).w
        addq.w    #1,(fd_sect).w
        addi.l    #$00000200,(fd_buffer).w
        subq.w    #1,(fd_scount).w
        beq       flopok
        bsr       select1
        bra       flopwrt2
flopwrt5:
        cmpi.w    #$0001,(fd_retry).w
        bne.s     flopwrt7
flopwrt6:
        bsr       reseek
flopwrt7:
        subq.w    #1,(fd_retry).w
        bpl       flopwrt1
        bra       flopfail

/*
 * XBIOS #10 - Flopfmt - Format specific tracks on a floppy 
 */
/* 306de: 00e03e02 */
_flopfmt:
        cmpi.l    #$87654321,22(a7)
        bne       flopfail
        tst.w     _nflops
        beq       flopini4
        bsr       fdchange
        moveq.l   #-1,d0
        bsr       frbfmtcheck
        bsr       floplock
        bsr       select
        move.w    14(a7),(fd_spt).w
        move.w    20(a7),(fd_interlv).w
        move.w    26(a7),(fd_virgin).w
        move.l    8(a7),(fd_secmap).w
        moveq.l   #2,d0
        bsr       setdchg
        moveq.l   #3,d0
        cmpi.w    #$000D,(fd_spt).w
        bcc.s     flopfmt1
        moveq.l   #0,d0
flopfmt1:
        move.w    d0,($FFFF860E).w
        move.w    d0,fd_density(a1)
        bsr       hardseek
        bne       flopfail
        move.w    (fd_track).w,fd_curtrack(a1)
        move.w    #-1,(fd_curerr).w
        bsr.s     fmtrack
        bne       flopfail
        move.w    (fd_spt).w,(fd_scount).w
        move.w    #$0001,(fd_sect).w
        bsr       verify1
        movea.l   (fd_buffer).w,a2
        tst.w     (a2)
        beq       flopok
        move.w    #-16,(fd_curerr).w
        bra       flopfail

fmtrack:
        move.w    #-10,(fd_deferr).w ; E_WRITF
        movea.l   (fd_buffer).w,a2
        movea.l   (fd_secmap).w,a3
        moveq.l   #119,d1
        cmpi.w    #$000D,(fd_spt).w
        bcc.s     fmtrack1
        moveq.l   #59,d1
fmtrack1:
        moveq.l   #78,d0
        bsr       wmult
        clr.w     d3
        tst.w     (fd_interlv).w
        bmi       fmtrack9
        moveq.l   #1,d3
fmtrack2:
        move.w    d3,d4
fmtrack3:
        moveq.l   #11,d1
        clr.b     d0
        bsr       wmult
        moveq.l   #2,d1
        moveq.l   #-11,d0
        bsr       wmult
        move.b    #$FE,(a2)+
        move.b    (fd_track+1).w,(a2)+
        move.b    (fd_side+1).w,(a2)+
        move.b    d4,(a2)+
        move.b    #$02,(a2)+
        move.b    #$F7,(a2)+
        moveq.l   #21,d1
        moveq.l   #78,d0
        bsr       wmult
        moveq.l   #11,d1
        clr.b     d0
        bsr       wmult
        moveq.l   #2,d1
        moveq.l   #-11,d0
        bsr       wmult
        move.b    #$FB,(a2)+
        move.w    #$00FF,d1
fmtrack4:
        move.b    (fd_virgin).w,(a2)+
        move.b    (fd_virgin+1).w,(a2)+
        dbf       d1,fmtrack4
        move.b    #$F7,(a2)+
        moveq.l   #39,d1
        moveq.l   #78,d0
        bsr       wmult
        tst.w     (fd_interlv).w
        bmi       fmtrack9
        add.w     (fd_interlv).w,d4
        cmp.w     (fd_spt).w,d4
        ble.s     fmtrack3
        addq.w    #1,d3
        cmp.w     (fd_interlv).w,d3
        ble.s     fmtrack2
fmtrack5:
        move.w    #$0AF0,d1
        cmpi.w    #$000D,(fd_spt).w
        bcc.s     fmtrack6
        move.w    #$0578,d1
fmtrack6:
        moveq.l   #78,d0
        bsr       wmult
        move.b    (fd_buffer+3).w,($FFFF860D).w
        move.b    (fd_buffer+2).w,($FFFF860B).w
        move.b    (fd_buffer+1).w,($FFFF8609).w
        move.w    #$0090,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0190,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        moveq.l   #96,d7
        bsr       wrfdcd7
        move.w    #$0180,(a6)
        move.w    #$00F0,d7
        bsr       wrfdcd7
        move.l    (_hz_200).w,d7
        add.l     #$0000012C,d7
fmtrack7:
        btst      #5,(MFP_GPIP).w
        beq.s     fmtrac10
        cmp.l     (_hz_200).w,d7
        bhi.s     fmtrack7
        bsr       fdcreset
fmtrack8:
        moveq.l   #1,d7
        rts
fmtrack9:
        cmp.w     (fd_spt).w,d3
        beq       fmtrack5
        move.w    d3,d6
        add.w     d6,d6
        move.w    0(a3,d6.w),d4
        addq.w    #1,d3
        bra       fmtrack3
fmtrac10:
        move.w    #$0190,(a6)
        move.w    (a6),d0
        btst      #0,d0
        beq.s     fmtrack8
        move.w    #$0180,(a6)
        bsr       rdfdcd0
        bsr       fdcerr
        and.b     #$44,d0
        rts

wmult:
        move.b    d0,(a2)+
        dbf       d1,wmult
        rts


/*
 * XBIOS #19 - Flopver - Verify sectors of a floppy disk.
 */
/* 306de: 00e03ffe */
_flopver:
        tst.w     (_nflops).w
        beq       flopini4
        bsr       fdchange
        moveq.l   #-11,d0 ; E_READF
        bsr       frbfmtcheck
        bsr       floplock
        bsr       select
        bsr       go2track
        bne       flopfail
        bsr.s     verify1
        bra       flopok

verify1:
        move.w    #-11,(fd_deferr).w ; E_READF
        movea.l   (fd_buffer).w,a2
        addi.l    #$00000200,(fd_buffer).w
verify2:
        move.w    #$0002,(fd_retry).w
        move.w    #$0084,(a6)
        move.w    (fd_sect).w,d7
        bsr       wrfdcd7
verify3:
        move.b    (fd_buffer+3).w,($FFFF860D).w
        move.b    (fd_buffer+2).w,($FFFF860B).w
        move.b    (fd_buffer+1).w,($FFFF8609).w
        move.w    #$0190,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0090,(a6)
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        move.w    #$0001,d7
        bsr       wrfdcd7
        move.w    #$0080,(a6)
        move.w    #$0080,d7
        bsr       wrfdcd7
        move.l    (_hz_200).w,d7
        add.l     #$0000012C,d7
verify4:
        btst      #5,(MFP_GPIP).w
        beq.s     verify5
        cmp.l     (_hz_200).w,d7
        bhi.s     verify4
        bsr       fdcreset
        bra.s     verify7
verify5:
        move.w    #$0090,(a6)
        move.w    (a6),d0
        btst      #0,d0
        beq.s     verify7
        move.w    #$0080,(a6)
        bsr       rdfdcd0
        bsr       fdcerr
        and.b     #$1C,d0
        bne.s     verify7
verify6:
        addq.w    #1,(fd_sect).w
        subq.w    #1,(fd_scount).w
        bne       verify2
        subi.l    #$00000200,(fd_buffer).w
        clr.w     (a2)
        rts
verify7:
        cmpi.w    #$0001,(fd_retry).w
        bne.s     verify8
        bsr       reseek
verify8:
        subq.w    #1,(fd_retry).w
        bpl       verify3
        move.w    (fd_sect).w,(a2)+
        bra.s     verify6

/* 306de: 00e04106 */
_flopvbl:
        tst.w     (_nflops).w
        beq       flopvbl5
        lea.l     (DMA_CTRL).w,a6
        st        (fd_motoron).w
        tst.w     (_flock).w
        bne.s     flopvbl5
        move.l    (_frclock).w,d0
        move.b    d0,d1
        and.b     #$07,d1
        bne.s     flopvbl2
        move.w    #$0080,(a6)
        lsr.b     #3,d0
        and.w     #$0001,d0
        lea.l     (_fd_wp).w,a0
        adda.w    d0,a0
        cmp.w     (_nflops).w,d0
        bne.s     flopvbl1
        clr.w     d0
flopvbl1:
        addq.b    #1,d0
        lsl.b     #1,d0
        eori.b    #$07,d0
        bsr       setporta
        bsr       rdfdcd0
        move.w    d0,d1
        btst      #6,d1
        sne       (a0)
        move.b    d2,d0
        bsr       setporta
flopvbl2:
        move.w    (_fd_wp).w,d0
        or.w      d0,(_fd_latch).w
        tst.w     (fd_desel).w
        bne.s     flopvbl4
        move.l    (_hz_200).w,d0
        cmp.l     fd_timeout,d0
        bcc.s     flopvbl3
        bsr       rdfdcd0
        btst      #7,d0
        bne.s     flopvbl5
flopvbl3:
        move.b    #$07,d0
        bsr       setporta
        move.w    #$0001,(fd_desel).w
flopvbl4:
        clr.w     (fd_motoron).w
flopvbl5:
        rts


/* 306de: 00e04196 */
floplock:
        movem.l   d3-d7/a3-a6,flopsregs
        lea.l     (DMA_CTRL).w,a6
        st        (fd_motoron).w
        move.w    d0,(fd_deferr).w
        move.w    d0,(fd_curerr).w
        move.w    #$0001,(_flock).w
        move.l    8(a7),(fd_buffer).w
        move.w    16(a7),(fd_dev).w
        move.w    18(a7),(fd_sect).w
        move.w    20(a7),(fd_track).w
        move.w    22(a7),(fd_side).w
        move.w    24(a7),(fd_scount).w
        move.w    #$0002,(fd_retry).w
        lea.l     (dsb).w,a1
        tst.w     (fd_dev).w
        beq.s     floploc1
        lea.l     (dsb+8).w,a1
floploc1:
        tst.w     fd_curtrack(a1)
        bpl.s     floploc3
        bsr       select
        clr.w     fd_curtrack(a1)
        bsr       restore
        beq.s     floploc3
        moveq.l   #10,d7
        bsr       hardsee1
        bne.s     floploc2
        bsr       restore
        beq.s     floploc3
floploc2:
        move.w    #$FF00,fd_curtrack(a1)
floploc3:
        rts

/* 306de: 00e04216 */
flopfail:
        moveq.l   #1,d0
        bsr       setdchg
        move.w    (fd_curerr).w,d0
        ext.l     d0
        bra.s     flopok1

flopok:
        clr.l     d0
flopok1:
        move.l    d0,-(a7)
        bsr       flush_cache_all_030
        move.w    #$0086,(a6)
        move.w    fd_curtrack(a1),d7
        bsr       wrfdcd7
        move.w    #$0010,d6
        bsr       flopcmds
        move.l    _hz_200,d0
        add.l     #1000,d0
        move.l    d0,fd_timeout
        move.w    (fd_dev).w,d0
        lsl.w     #2,d0
        lea.l     (_fd_lastacc).w,a0
        move.l    (_frclock).w,0(a0,d0.w)
        cmpi.w    #1,(_nflops).w
        bne.s     flopok2
        move.l    (_frclock).w,4(a0)
flopok2:
        move.l    (a7)+,d0
        movem.l   (flopsregs).w,d3-d7/a3-a6
        clr.w     _flock
        bsr       frbbackcopy
        rts

hardseek:
        move.w    fd_track,d7
hardsee1:
        move.w    #-6,fd_curerr ; E_SEEK
        move.w    #$0086,(a6)
        bsr       wrfdcd7
        move.w    #$0010,d6
        bra       flopcmds

reseek:
        move.w    #-6,fd_curerr ; E_SEEK
        bsr.s     restore
        bne.s     go2trret
        clr.w     fd_curtrack(a1)
        move.w    #$0082,(a6)
        clr.w     d7
        bsr       wrfdcd7
        move.w    #$0086,(a6)
        move.w    #5,d7
        bsr       wrfdcd7
        move.w    #$0010,d6
        bsr.s     flopcmds
        bne.s     go2trret
        move.w    #5,fd_curtrack(a1)

go2track:
        move.w    #$0001,fd_rtryseek
go2trac1:
        move.w    #-6,fd_curerr
        move.w    #$0086,(a6)
        move.w    (fd_track).w,d7
        bsr       wrfdcd7
        moveq.l   #20,d6
        bsr.s     flopcmds
        bcs.s     go2trret
        bne.s     go2trac2
        and.b     #$18,d7
        beq.s     go2trac3
go2trac2:
        move.w    fd_density(a1),d0
        and.w     #$0003,d0
        eori.w    #$0003,d0
        move.w    d0,($FFFF860E).w
        move.w    d0,fd_density(a1)
        subq.w    #1,fd_rtryseek
        bne.s     go2trret
        bsr.s     restore
        bra.s     go2trac1
go2trac3:
        move.w    (fd_track).w,fd_curtrack(a1)
        clr.w     d7
go2trret:
        rts

restore:
        clr.w     d6
        bsr.s     flopcmds
        bne.s     restore1
        btst      #2,d7
        eori.b    #$04,ccr
        bne.s     restore1
        clr.w     fd_curtrack(a1)
restore1:
        rts

*--- seek rate conversion table for HD mode
* HD is using twice the clock speed (16MHz instead of 8MHz) for the FDC,
* so the FDC has to seek "slower"
dseekrt:      dc.b 1,1,0,0


flopcmds:
        move.w    fd_seekrate(a1),d0
        and.w     #3,d0
        tst.w     fd_density(a1)
        beq.s     flopcms1
        lea.l     dseekrt,a0
        move.b    0(a0,d0.w),d0
flopcms1:
        or.b      d0,d6
        or.b      (a1),d6
        move.w    fd_density(a1),($FFFF860E).w
flopcmd:
        move.l    (_hz_200).w,d7
        add.l     #300,d7
        move.w    #$0080,(a6)
        bsr       rdfdcd0
        btst      #7,d0
        bne.s     flopcmd1
        move.l    (_hz_200).w,d7
        add.l     #$00000258,d7
flopcmd1:
        bsr       wrfdcd6
flopcmd2:
        cmp.l     (_hz_200).w,d7
        bcs.s     flopcmd3
        btst      #5,(MFP_GPIP).w
        bne.s     flopcmd2
        bsr       rdfdcd7
        clr.w     d6
        rts
flopcmd3:
        bsr.s     fdcreset
        moveq.l   #0,d6
        subq.w    #1,d6
        rts

fdcreset:
        move.w    #$0080,(a6)
        move.w    #$00D0,d7
        bsr.s     wrfdcd7
        move.w    d0,-(a7)
        move.w    #$0114,d0
        bsr       ttdelayc
        move.w    (a7)+,d0
        bsr.s     rdfdcd7
        rts

/* 306de: 00e043c4 */
select:
        clr.w     (fd_desel).w
        move.w    (fd_dev).w,d0
        addq.b    #1,d0
        lsl.b     #1,d0
        or.w      (fd_side).w,d0
        eori.b    #$07,d0
        and.b     #$07,d0
        bsr.s     setporta
        move.w    #$0082,(a6)
        move.w    fd_curtrack(a1),d7
        bsr.s     wrfdcd7
select1:
        move.w    #$0084,(a6)
        move.w    (fd_sect).w,d7
        bsr.s     wrfdcd7
        move.b    (fd_buffer+3).w,($FFFF860D).w
        move.b    (fd_buffer+2).w,($FFFF860B).w
        move.b    (fd_buffer+1).w,($FFFF8609).w
        rts

/* 306de: 00e04406 */
setporta:
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.b    #$0E,($FFFF8800).w
        move.b    ($FFFF8800).w,d1
        move.b    d1,d2
        and.b     #$F8,d1
        or.b      d0,d1
        move.b    d1,($FFFF8802).w
        move.w    (a7)+,sr
        rts

/* write d6 to selected FDC register */
/* 306de: 00e04426 */
wrfdcd6:
        bsr.s     rwdelay
        move.w    d6,(DMA_DATA).w
        rts

/* write d7 to selected FDC register */
/* 306de: 00e0442e */
wrfdcd7:
        bsr.s     rwdelay
        move.w    d7,(DMA_DATA).w
        rts

/* read selected FDC register into d7 */
/* 306de: 00e04436 */
rdfdcd7:
        bsr.s     rwdelay
        move.w    (DMA_DATA).w,d7
        rts

/* read selected FDC register into d0 */
/* 306de: 00e0443e */
rdfdcd0:
        bsr.s     rwdelay
        move.w    (DMA_DATA).w,d0
        rts

rwdelay:
        move.w    d0,-(a7)
        move.w    #$0119,d0
        bsr       ttdelayc
        move.w    (a7)+,d0
        rts

/* 306de: 00e04454 */
fdchange:
        cmpi.w    #1,_nflops
        bne.s     fdchang2
        move.w    16(a7),d0
        cmp.w     _curflop,d0
        beq.s     fdchang1
        move.w    d0,-(a7)
        move.w    #$FFEF,-(a7)
        bsr       _callcrit
        addq.w    #4,a7
        move.w    #$FFFF,_fd_latch
        lea.l     _fd_lastacc,a0
        clr.l     (a0)+
        clr.l     (a0)
        move.w    16(a7),_curflop
fdchang1:
        clr.w     16(a7)
fdchang2:
        rts

setdchg:
        lea.l     (_drivechange).w,a0
        move.b    d0,-(a7)
        move.w    (fd_dev).w,d0
        cmpi.w    #1,_nflops
        bne.s     setdchg1
        move.w    _curflop,d0
setdchg1:
        move.b    (a7)+,0(a0,d0.w)
        rts


/*
 * XBIOS #41 - Floprate - Read or set the seek rate of a floppy drive.
 */
/* 306de: 00e044b6 */
_floprate:
        lea.l     dsb,a1
        tst.w     4(a7)
        beq.s     floprat1
        lea.l     dsb+8,a1
floprat1:
        move.w    fd_seekrate(a1),d0
        move.w    6(a7),d1
        cmp.w     #-1,d1
        beq.s     floprat2
        cmp.w     #-2,d1
        beq.s     floprat3
        cmp.w     #-3,d1
        beq.s     floprat4
        cmp.w     #-4,d1
        beq.s     floprat5
        move.w    d1,fd_seekrate(a1)
floprat2:
        ext.l     d0
        rts

* Floprate(dev, -2): set HD drivetype
floprat3:
        move.b    #$08,(a1)
        moveq.l   #0,d0
        rts
* Floprate(dev, -3): set DD drivetype
floprat4:
        clr.b     (a1)
        moveq.l   #0,d0
        rts
* Floprate(dev, -4): query drivetype
floprat5:
        tst.b     (a1)
        sne       d0
        ext.w     d0
        ext.l     d0
        rts

/* 306de: 00e04508 */
frbbackcopy:
        move.w    fd_cscnt,d1
        beq.s     frbback2
        move.l    d0,-(a7)
        bsr       flush_cache_all_030
        move.l    (a7)+,d0
        clr.w     fd_cscnt
        movea.l   fd_copyp,a0
        movea.l   fd_frb,a1
frbback0:
        asl.w     #5,d1
        subq.w    #1,d1
frbback1:
        move.l    (a1)+,(a0)+
        move.l    (a1)+,(a0)+
        move.l    (a1)+,(a0)+
        move.l    (a1)+,(a0)+
        dbf       d1,frbback1
frbback2:
        rts

frbrdcheck:
        cmpi.l    #$00A00000,8(a7)
        bcs.s     frbback2
        bsr.s     getfrb
        move.l    8(a7),fd_copyp
        move.l    fd_frb,8(a7)
        move.w    24(a7),fd_cscnt
        rts

frbwrcheck:
        cmpi.l    #$00A00000,8(a7)
        bcs.s     frbback2
        bsr.s     getfrb
        movea.l   8(a7),a1
        movea.l   fd_frb,a0
        move.l    a0,8(a7)
        move.w    24(a7),d1
        bra.s     frbback0

frbfmtcheck:
        cmpi.l    #$00A00000,8(a7)
        bcs.s     frbback2
        bsr.s     getfrb
        move.l    8(a7),fd_copyp
        move.l    fd_frb,8(a7)
        move.w    #1,fd_cscnt
        rts

getfrb:
        tst.l     fd_frb
        bne.s     getfrb3
        movea.l   (_p_cookies).w,a0
        cmpa.w    #$0000,a0
        beq.s     getfrb4
getfrb1:
        tst.l     (a0)
        beq.s     getfrb4
        cmpi.l    #$5F465242,(a0)+ ; '_FRB'
        beq.s     getfrb2
        addq.l    #4,a0
        bra.s     getfrb1
getfrb2:
        move.l    (a0)+,fd_frb
getfrb3:
        rts
getfrb4:
        addq.l    #8,a7
        moveq.l   #-12,d0 ; E_GENRL
        rts


