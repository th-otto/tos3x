
ide_datar  equ $FFF00000
ide_param  equ $FFF00005
ide_seccn  equ $FFF00009
ide_stars  equ $FFF0000D
ide_zylow  equ $FFF00011
ide_zyhigh equ $FFF00015
ide_headn  equ $FFF00019
ide_comst  equ $FFF0001D
ide_stat2  equ $FFF00039

	text

/**************************************************************************
 * DMAREAD.S from the AHDI source                                         *
 **************************************************************************/
/*
 * dmaread() - read from a DMA device.
 * dmawrite() - write to a DMA device.
 *
 * LONG	sectnum		$4(sp).l
 * WORD	count		$8(sp).w
 * BYTE	*buf		$a(sp).l	$b(sp)=high $c(sp)=mid $d(sp)=low
 * WORD	pdev		$e(sp).w
 *
 * Returns 0 if successful.
 * Returns a negative number if failure:
 *	EUNDEV (-15L) if unknown device (bad dev number)
 *	EREADF (-11L) if read failure
 *	EWRITF (-10L) if write failure
 *	       (-1L)  if timed-out
 *
 * Comments:
 *	ACSI, SCSI and IDE-AT drives are supported.
 */
/*
 * XBIOS #42 - DMAread
 */
/* 306de: 00e0186c */
/* 206de: 00e015e6 */
_dmaread:
        move.b    #$00,dmarwflg
        move.w    #$0008,d1     /* d1 = opcode for READ */
        bra.s     dmarw
/*
 * XBIOS #43 - DMAwrite
 */
/* 306de: 00e0187a */
/* 206de: 00e015f4 */
_dmawrite:
        move.b    #$01,dmarwflg
        move.w    #$000A,d1     /* d1 = opcode for WRITE */
dmarw:
        cmpi.w    #$0010,14(a7) /* unit# > IDE-AT unit# 16? */
        bhi       dmaundev      /* if so, return unknown device */
        cmpi.w    #$000F,14(a7) /* an IDE-AT unit? */
        bhi.s     dmarw2        /* if so, talk IDE-AT */
        lea.l     scsicmd,a0    /* a0 -> beginning of command block */
        move.b    d1,(a0)+      /* byte 0 = opcode */
        move.b    5(a7),(a0)+   /* byte 1 = msb of logical block addr */
        move.b    6(a7),(a0)+   /* byte 2 = logical block addr */
        move.b    7(a7),(a0)+   /* byte 3 = lsb of logical block addr */
        move.b    9(a7),(a0)+   /* byte 4 = transfer length (in blocks) */
        clr.b     (a0)          /* byte 5 = control byte */
        move.w    14(a7),d0     /* d0 = physical unit number */
#if TOSVERSION >= 0x300
        moveq.l   #0,d1
        move.w    8(a7),d1      /* get count */
        lsl.l     #8,d1
        add.l     d1,d1         /* d1 = transfer size (sector count * 512) */
#endif
        moveq.l   #6,d2         /* d2 = length of command */
        movea.l   10(a7),a0     /* a0 = buffer */
        tst.b     dmarwflg      /* read or write? */
        bne.s     dmarw1        /* it's a write */
        bsr       dodmaread     /* send a receive data command */
        bra       dmarw7
dmarw1:
        bsr       dodmawrite    /* send a write data command */
        bra       dmarw7        /* branch will take place for non-IDE drives */
/*
 * Before doing anything with the IDE bus, make sure it's there.
 * (The label noide is before at just because it fits better.)
 */
noide:
        movea.l   a1,a7         /* got the bus error on IDEASR; */
        move.l    a0,(_buserror).w  /* restore vector & sp, return EUNDEV */
        bra       dmaundev
dmarw2:
        movea.l   (_buserror).w,a0
        movea.l   a7,a1
        move.l    #noide,(_buserror).w
        tst.b     ide_stat2     /* read a register to probe for IDE bus */
        movea.l   a1,a7         /* hey! no bus error! */
        move.l    a0,(_buserror).w /* restore vector & sp and continue. */
        move.b    15(a7),d0     /* IDE unit# */
        bsr       iderdy        /* wait for drive to be ready */
#if P68030
        beq       dmaundev      /* if drive never gets ready, return with unknown device else */
#else
        beq.s     dmaundev      /* if drive never gets ready, return with unknown device else */
#endif
        moveq.l   #2,d0         /* D_IDENTIFY delay required by Conner drives */
        add.l     (_hz_200).w,d0 /* between power on and identify() */
dmarw3:
        cmp.l     (_hz_200).w,d0
        bcc.s     dmarw3

        pea.l     idebuf        /* scratch buffer for drive parameters */
        move.w    18(a7),-(a7)  /* IDE unit# */
        bsr       ideidentify   /* identify() */
        addq.w    #6,a7
        tst.w     d0            /* successful? */
#if TOSVERSION >= 0x300
        bmi       dmaret        /* if timed-out, return */
#else
        bmi.s     dmaret        /* if timed-out, return */
#endif
        bne.s     dmarw8        /* if error, return with error code else can do read or write */
        moveq.l   #2,d0         /* D_IDENTIFY delay required by Conner drives */
        add.l     (_hz_200).w,d0 /* between identify() and r/w */
dmarw4:
        cmp.l     (_hz_200).w,d0
        bcc.s     dmarw4

#if TOSVERSION >= 0x300
        lea.l     idebuf,a0
        move.w    12(a0),-(a7)  /* # of sectors per track */
        move.w    6(a0),-(a7)   /* # of heads */
        move.w    18(a7),-(a7)  /* physical unit # */
        bsr       idecount
        addq.w    #6,a7
        tst.w     d0
        bmi.s     dmaret
        bne.s     dmarw8
        lea.l     idebuf,a0
#else
	pea       idebuf        /* beginning of _identify() data */
	bsr       _gcparm       /* get drive current parameters */
	addq.w    #4,sp         /* clean up stack */
#endif
        move.w    14(a7),-(a7)   /* physical unit # */
        move.l    12(a7),-(a7)   /* buffer */
        move.w    14(a7),-(a7)   /* count */
        move.l    12(a7),-(a7)   /* logical block address */
#if TOSVERSION >= 0x300
        move.w    12(a0),-(a7)   /* device number of sectors */
        move.w    6(a0),-(a7)    /* device number of heads */
#else
        move.w    d2,-(sp)       /* # sectors per track */
        move.w    d1,-(sp)       /* # data heads */
#endif
        tst.b     dmarwflg       /* read or write? */
        bne.s     dmarw5
        bsr       ideread        /* read sectors */
        bra.s     dmarw6
dmarw5:
        bsr       idewrite       /* write sectors */
dmarw6:
        adda.w    #16,a7
dmarw7:
#if P68030
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        CACR_D1
        ori.w     #$0808,d1
        D1_CACR
        move.w    (a7)+,sr
#endif
        tst.w     d0             /* successful? */
        ble.s     dmaret         /* if no error or timed-out, return else */
dmarw8:
        moveq.l   #-11,d0        /* assume it's a read error E_READF */
        tst.b     dmarwflg
        beq.s     dmaret
        moveq.l   #-10,d0        /* else, it's a write error E_WRITF */
dmaret:
        rts
dmaundev:
        moveq.l   #-15,d0        /* E_UNDEV */
        bra.s     dmaret

/*
 * dorcmd() - send a command which will receive data from the target
 *
 * Passed:
 *	d0.w = physical unit number
 *	d2.w = command length (NCMD or LCMD)
 *	a0.l = buffer address
 */
/* 306de: 00e019ba */
/* 206de: 00e016f6 */
dodmaread:
#if TOSVERSION >= 0x300
        cmp.w     #$0007,d0
        bls.s     domdmar1
        bsr       scsiread       /* it's an SCSI device */
        rts
domdmar1:
        bsr       rcvacsi        /* it's an ACSI device */
#else
        bsr.s     rcvacsi        /* it's an ACSI device */
#endif
        rts

/*
 * dowcmd() - send a command which will write data to the target
 *
 * Passed:
 *	d0.w = physical unit number
 *	d2.w = command length (NCMD or LCMD)
 *	a0.l = buffer address
 */
/* 306de: 00e019cc */
/* 206de: 00e016fa */
dodmawrite:
#if TOSVERSION >= 0x300
        cmp.w     #$0007,d0
        bls.s     dodmaw1
        bsr       scsiwrite       /* it's an SCSI device */
        rts
dodmaw1:
#endif
        bsr       wrtacsi        /* it's an ACSI device */
        rts

/*
 * LONG _qdone() - Wait for command byte handshake
 * LONG _fdone() - Wait for operation complete
 * Passed:	nothing
 *
 * Returns:	EQ: no time-out
 *		MI: time-out condition
 *
 * Uses:		D0
 *
 */
/* 306de: 00e019de */
/* 206de: 00e01700 */
_fdone:
        move.l    #600,d0                   /* ACLTMOUT */
        bra.s     dmadel2
_qdone:
        moveq.l   #20,d0                    /* ACSTMOUT */
dmadel2:
#if TOSVERSION < 0x300
	move.l    d0,-(sp)                  /* save timeout value */
        moveq     #2,d0                     /* busy-wait delay for slow ACSI */
        add.l     (_hz_200).w,d0            /* minimum 20 microsec. */
sdelay: cmp.l     (_hz_200).w,d0
        bge.s     sdelay
        move.l    (sp)+,d0                  /* restore timeout value */
#endif
        add.l     (_hz_200).w,d0
dmadel3:
        cmp.l     (_hz_200).w,d0            /* time-out? */
        bcs.s     dmaerr                    /* (i give up, return NE) */
        btst      #5,(MFP_GPIP).w           /* interrupt? */
        bne.s     dmadel3                   /* (not yet) */
        moveq.l   #0,d0                     /* return EQ (no time-out) */
        rts
dmaerr:
        moveq.l   #-1,d0
        rts

/*
 * Wait for end of SASI command
 *
 * Passed:	d1 value to be written to wdl
 *
 * Returns:	EQ: success (error code in D0.W)
 *		MI: time-out (-1 in D0.W)
 *		NE: failure (SASI error code in D0.W)
 *
 * Uses:		d0
 */
/* 306de: 00e01a02 */
/* 206de: 00e01734 */
_endcmd:
        bsr.s     _fdone                    /* wait for operation complete */
        bmi.s     acsiab1                   /* (timed-out, so complain) */
#if P68030
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        CACR_D0
        ori.w     #$0800,d0
        D0_CACR
        move.w    (a7)+,sr
#endif
        move.w    d1,($FFFF8606).w
        move.w    ($FFFF8604).w,d0          /* get the result */
        and.w     #$00FF,d0                 /* (clean it up) if non-0 should do a RequestSense command to learn more */
acsiab1:
        move.l    (_hz_200).w,lastacstm     /* update controller last accessed time */
        addq.l    #2,lastacstm              /* lastacstm = _hz_200 + 2; */
        rts

/*
 *  Handle command time-out
 *  Unlock DMA chip and return completion status
 */
/* 306de: 00e01a36 */
/* 206de: 00e01754 */
_hdone:
        move.w    #$0080,($FFFF8606).w      /* Landon's code seems to presume we put $80 there */
#if (TOSVERSION >= 0x300) | !BINEXACT
        clr.w     (_flock).w                /* NOW, signal that we are done */
#else
        sf        (_flock).w                /* NOW, signal that we are done */
#endif
        rts

/*
 * delay()
 *	5 - 10ms kludge delay for message byte sent back by controller.
 */
/* 306de: 00e01a42 */
/* 206de: 00e01760 */
acsibeg:
        move.l    d0,-(a7)                  /* preserve d0 */
        move.l    lastacstm,d0              /* d0 = controller last accessed time */
acsibeg1:
        cmp.l     (_hz_200).w,d0            /* while (_hz_200 <= lastacstm) */
        bcc.s     acsibeg1                  /* wait() */
        move.l    (a7)+,d0                  /* restore d0 */
        rts

/*
 * rcvacsi() - send a ACSI command which receives data from target.
 *
 * Passed:
 *	d0.w = physical unit number
 *	d2.w = command length (NCMD or LCMD)
 *	a0.l = buffer address
 */
/* 306de: 00e01a54 */
/* 206de: 00e01772 */
rcvacsi:
        movea.l   (_buserror).w,a1          /* check to see if the ACSI bus actually exists! Needed for prototype PADs */
        movea.l   a7,a2
        move.l    #acsierr,(_buserror).w
        tst.w     ($FFFF8606).w             /* harmless or bus erorr */
        move.l    a1,(_buserror).w          /* no bus error - restore & continue */
        movea.l   a2,a7
        st        (_flock).w                /* lock FIFO */
        bsr.s     acsibeg                   /* delay if necessary */
        movea.w   #$8604,a1                 /* a1 = pointer to DMA chip */
        bsr       setdmaaddr                /* set DMA pointer */
        move.w    #$0190,2(a1)              /* WDL	; toggle DMA chip to direction */
        bsr       shortdelay
        move.w    #$0090,2(a1)              /* WDL	;  for receiving data */
        bsr       shortdelay
        bsr       setacnt                   /* set DMA count */
        lea.l     scsicmd,a0                /* a0 = address of command block */
        moveq.l   #0,d1                     /* direction of DMA is IN */
        bsr.s     sblkacsi                  /* send the command block */
        bra.s     _hdone                    /* cleanup after IRQ */

/*
 * wrtacsi() - send an ACSI command which will write data to the target
 *
 * Passed:
 *	d0.w = physical unit number
 *	d2.w = command length (NCMD or LCMD)
 *	a0.l = buffer address
 */
/* 306de: 00e01a9e */
/* 206de: 00e017bc */
wrtacsi:
        movea.l   (_buserror).w,a1          /* check to see if the ACSI bus actually exists! Needed for prototype PADs */
        movea.l   a7,a2
        move.l    #acsierr,(_buserror).w
        tst.w     ($FFFF8604).w             /* harmless or bus erorr */
        move.l    a1,(_buserror).w          /* no bus error - restore & continue */
        movea.l   a2,a7
        st        (_flock).w                /* lock FIFO */
        bsr.s     acsibeg
        movea.w   #$8604,a1                 /* a1 = pointer to DMA chip */
        bsr.s     setdmaaddr                /* set DMA pointer */
        move.w    #$0090,2(a1)              /* WDL	; toggle DMA chip for "send" */
        bsr       shortdelay
        move.w    #$0190,2(a1)              /* WDL */
#if TOSVERSION >= 0x300
        bsr       shortdelay
#else
        bsr.s     shortdelay
#endif
        bsr.s     setacnt                   /* set DMA count */
        move.l    #$00000100,d1             /* d1 = direction of DMA is OUT */
        bsr.s     sblkacsi                  /* send the command block */
        bra       _hdone                    /* cleanup after IRQ */

/*
 * You get to this label if there is a bus error when probing for
 * the ACSI bus: it's meant for prototype PADs, which don't
 * have an ACSI DMA chip.
 */
/* 306de: 00e01ae4 */
/* 206de: 00e01800 */
acsierr:
        moveq.l   #-15,d0                   /* return E_UNDEV */
        movea.l   a2,a7
        move.l    a1,(_buserror).w
        rts

/*
 * sblkacsi() - send command block
 *
 * Passed:
 *	d0.w = physical unit number
 *	d1.l = direction of DMA ($0000 for IN or $0100 for OUT)
 *	d2.w = command length (NCMD or LCMD)
 *	a1.l = pointer to DMA chip
 *
 * Returns:
 *	d0.l =  0 if successful
 *	d0.l = -1 if time-out
 *
 * Trashes:
 *	d0, d1, d2, a2
 */
/* 306de: 00e01aee */
/* 206de: 00e0180a */
sblkacsi:
        move.b    #$88,d1                       /* next byte is the opcode */
        move.w    d1,2(a1)                      /* WDL */
        move.b    #$8A,d1                       /* following bytes are operands */
        lea.l     scsicmd,a2
        lsl.b     #5,d0                         /* shift unit number into place */
        or.b      d0,(a2)                       /* first command byte = unit # | opcode */
        subq.w    #2,d2
acsista1:
        swap      d1                            /* d1.hw = operand */
        move.b    (a2)+,d1                      /* d1.lw = tells controller next byte */
        swap      d1
        move.l    d1,(a1)                       /* WDCWDL */
        bsr       _qdone
        bmi.s     acsista2                      /* if time-out, returns */
        dbf       d2,acsista1                   /* else send rest of command block */
        move.w    d1,2(a1)                      /* WDL - get ready to send control byte */
        move.b    #$00,d1                       /* signal sending control byte */
        swap      d1                            /* d1.hw = control byte */
        move.b    (a2),d1                       /* d1.lw = tells controller it's end */
        swap      d1
        move.l    d1,(a1)
        move.b    #$8A,d1                       /* d1 = wdl value */
        bsr       _endcmd                       /* wait for command completion */
acsista2:
        rts

/*
 * setadma() - set the ACSI DMA pointer
 *
 * Passed:
 *	a0.l = buffer address
 */
/* 306de: 00e01b32 */
/* 206de: 00e0184e */
setdmaaddr:
        move.l    a0,-(a7)
        move.b    3(a7),($FFFF860D).w
        move.b    2(a7),($FFFF860B).w
        move.b    1(a7),($FFFF8609).w
        addq.l    #4,a7
        rts

/*
 * setacnt() - set the ACSI DMA counter
 *
 * Passed:
 *	a1.l = pointer to DMA chip
 */
/* 306de: 00e01b4a */
/* 206de: 00e01866 */
setacnt:
#if TOSVERSION >= 0x300
        move.l    d1,-(a7)
        lsr.l     #8,d1
        add.l     d1,d1
        move.w    d1,(a1)
        move.l    (a7)+,d1
#else
	move.w    #$ff,(a1)                 /* set the ACSI DMA counter to 255 512-byte-blocks */
#endif
        rts

/*
 * After talking to the DMA chip in a way that may reset it,
 * we need a 8 8Mhz clocks (ie. 1 microsec) delay, before we can
 * talk to the chip again.
 */
/* 306de: 00e01b56 */
/* 206de: 00e0186c */
shortdelay:
        tst.b     (MFP_GPIP).w              /* delay for 1 microsec */
        tst.b     (MFP_GPIP).w              /* this amounts to 16 16Mhz clocks */
        tst.b     (MFP_GPIP).w
        tst.b     (MFP_GPIP).w
        rts


#if TOSVERSION >= 0x300
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
        btst      #3,10(a2) /* phase match? */
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
        btst      #3,10(a2) /* phase match? */
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
        btst      #6,($FFFF8789).w /* BSY set? */
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
        btst      #6,($FFFF8789).w /* BSY set? */
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

/* 306de: 00e01cb8 */
scsibsy:
        btst      #5,($FFFF8789).w  /* data transfer pending? */
        bne.s     scsibsy1
        cmp.l     (a0),d1
        bhi.s     scsibsy
        moveq.l   #-1,d0
        bra.s     scsibsy2
scsibsy1:
        moveq.l   #0,d0
scsibsy2:
        rts

/* 306de: 00e01ccc */
scsiack:
        ori.b     #$11,($FFFF8783).w /* assert ACK */
scsiack1:
        btst      #5,($FFFF8789).w  /* data transfer pending? */
        beq.s     scsiack2
        cmp.l     (a0),d1
        bhi.s     scsiack1
        moveq.l   #-1,d0
        bra.s     scsiack3
scsiack2:
        moveq.l   #0,d0
scsiack3:
        andi.b    #$EF,($FFFF8783).w /* de-assert ACK */
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
        move.b    #$80,($FFFF8783).w  /* assert RST */
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
#endif


/*
 * Wait for status to come back
 */
/* 306de: 00e01d30 */
/* 206de: 00e0187e */
idestat:
        move.l    #2000,d0                      /* d0 = time-out limit (D_WORST) */
        add.l     (_hz_200).w,d0                /* d0 = expiration time */
idestat1:
        btst      #5,(MFP_GPIP).w               /* interrupt? */
        beq.s     idestat2                      /* if so, out of the loop */
        cmp.l     (_hz_200).w,d0                /* time-out? */
        bhi.s     idestat1                      /* if not, wait some more */
        moveq.l   #-1,d0                        /* else, return time-out */
        bra.s     idestat4
idestat2:
        moveq.l   #0,d0
        move.b    ide_comst,d0                  /* read status register */
        btst      #0,d0                         /* error occured? */
        bne.s     idestat3                      /* yes, ho read error */
        btst      #3,d0                         /* data request ready? */
        bne.s     idestat4
        moveq.l   #0,d0
        bra.s     idestat4
idestat3:
        move.b    ide_param,d0 /* read error register */
idestat4:
        rts

/*
 * ideread() - reads from 1 to 256 sectors as specified in the Task File,
 *		beginning at the specified sector.
 *	   - sector count equal to 0 requests 256 sectors.
 *
 * ideread(nhd, nspt, sectnum, count, buf, pdev)
 * WORD	nhd/*		4(sp).w		; # of data heads on pdev
 * WORD	nspt/*		6(sp).w		; # of physical sectors per track
 * LONG	sectnum/*	8(sp).l		; logical block address
 * WORD	count/*		$c(sp).w	; # of sectors to read
 * BYTE	*buf/*		$e(sp).l	; $f(sp)=high $10(sp)=mid $11(sp)=low
 * WORD	pdev/*		$12(sp).w	; physical device number
 */
/* 306de: 00e01d6c */
/* 206de: 00e018ba */
ideread:
        bsr       idechs                    /* set physical address */
        movea.l   14(a7),a0                 /* a0 -> buffer to read into */
        move.b    13(a7),ide_seccn          /* set sector count */
        move.w    12(a7),d1                 /* d1.w = # of sectors to read */
        subq.w    #1,d1                     /* dbra likes one less */
        move.b    #$00,ide_stat2            /* enable interrupt */
        move.b    #$20,ide_comst            /* set command code IDEREAD */
iderd1:
        bsr.s     idestat                   /* wait for interrupt */
        tst.w     d0                        /* successful? */
        bmi.s     iderd2                    /* if timed-out, return */
        btst      #3,d0                     /* DRQ? */
        beq.s     iderd2                    /* if not, return */
        bsr       iderdsec                  /* fill sector buffer */
        dbf       d1,iderd1                 /* more to read? */
        moveq.l   #0,d0                     /* everything's fine */
iderd2:
        rts

/*
 * idewrite() - writes from 1 to 256 sectors as specified in the Task File,
 *		beginning at the specified sector.
 *	    - sector count equal to 0 requests 256 sectors.
 *
 * idewrite(nhd, nspt, sectnum, count, buf, pdev)
 * WORD	nhd  		4(sp).w		; # of data heads on pdev
 * WORD	nspt		6(sp).w		; # of physical sectors per track
 * LONG	sectnum		8(sp).l		; logical block address
 * WORD	count		$c(sp).w	; # sectors to read
 * BYTE	*buf		$e(sp).l	; $f(sp)=high $10(sp)=mid $11(sp)=low
 * WORD	pdev		$12(sp).w	; physical device number
 */
/* 306de: 00e01daa */
/* 206de: 00e018f8 */
idewrite:
        bsr.s     idechs                    /* set physical address */
        movea.l   14(a7),a0                 /* a0 -> buffer to write from */
        move.b    13(a7),ide_seccn          /* set sector count */
        move.w    12(a7),d1                 /* d1.w = # of sectors to read */
        subq.w    #1,d1                     /* dbra likes one less */
        move.b    #$00,ide_stat2            /* enable interrupt */
        move.b    #$30,ide_comst            /* set command code IDEWRITE */
idewr1:
        btst      #3,ide_stat2              /* DRQ? */
        beq.s     idewr1                    /* if not, wait longer */
idewr2:
        bsr       idewrsec                  /* fill sector buffer */
        bsr       idestat                   /* wait for interrupt */
        tst.w     d0                        /* successful? */
        bmi.s     idewr3                    /* if timed-out, return */
        btst      #3,d0                     /* DRQ? */
        beq.s     idewr3                    /* if not, return */
        dbf       d1,idewr2                 /* else go transfer data */
        moveq.l   #0,d0                     /* everything's fine */
idewr3:
        rts

/*
 * set_dhcs() - convert a logical block address into a physical address.
 *	     - set drive #, head #, cylinder # and sector # in task file.
 *
 * Passed:
 *	8(sp).w = nhd = # of data heads
 *	$a(sp).w = nspt = # of physical sectors per track
 *	$c(sp).l = logical block address
 *	$16(sp).w = physical unit #
 */
/* 306de: 00e01df2 */
/* 206de: 00e01940 */
idechs:
        move.l    12(a7),d1        /* get sector number */
        move.w    8(a7),d2         /* get number of heads */
        move.w    10(a7),d0        /* get number of sectors */
        mulu.w    d0,d2            /* calc number of sectors / cylinder */
        divu.w    d2,d1            /* calc cylinder # */
        move.b    d1,ide_zylow     /* write cylinder low */
        lsr.l     #8,d1
        move.b    d1,ide_zyhigh    /* write cylinder high */
        lsr.l     #8,d1            /* d1 now remainder of division above */
        divu.w    d0,d1            /* calc head # */
        move.w    22(a7),d0        /* get device number */
        andi.b    #$07,d0
        lsl.b     #4,d0
        or.b      d0,d1
        move.b    d1,ide_headn     /* select device & head */
        swap      d1
        addq.w    #1,d1
        move.b    d1,ide_stars     /* write sector number */
        rts

#if TOSVERSION >= 0x300
/* 306de: 00e01e32 */
idecount:
        move.w    4(a7),d0         /* d0 = physical unit # */
        andi.b    #$07,d0          /* mask off flags (if any) */
        lsl.w     #4,d0            /* shift unit # to place */
        move.b    d0,ide_headn     /* select device */
        move.w    6(a7),d0
        subq.b    #1,d0
        or.b      d0,ide_headn     /* select head */
        move.b    9(a7),ide_seccn  /* select sector count */
        move.b    #$00,ide_stat2   /* enable interrupt */
        move.b    #$91,ide_comst   /* set command code INITIALIZE DEVICE PARAMETERS */
        bra       idestat
#endif

/*
 * identify() - allows the Host to receive parameter information from
 *	       the drive.
 *
 * identify(pdev, buf)
 * WORD	pdev/*	4(sp).w		; physical unit #
 * BYTE	*buf/*	6(sp).l		; buffer to put data
 */
/* 306de: 00e01e6a */
/* 206de: 00e01980 */
ideidentify:
        move.w    4(a7),d0         /* get device */
        andi.b    #$07,d0          /* mask off flags (if any) */
        lsl.b     #4,d0            /* shift unit # to place */
        move.b    d0,ide_headn     /* select device */
        movea.l   6(a7),a0
        move.b    #$00,ide_stat2   /* enable interrupt */
        move.b    #$EC,ide_comst   /* send ATA IDENTIFY command */
        bsr       idestat          /* wait for interrupt */
        tst.w     d0               /* successful? */
        bmi.s     ideiden1         /* if timed-out, return */
        btst      #3,d0            /* DRQ? */
        beq.s     ideiden1         /* if not, return with error */
        bsr.s     iderdsec         /* read the identify packet */
        moveq.l   #0,d0
ideiden1:
        rts

/*
 * readbuf() - reads 512 bytes (128 longs) of data into sector buffer.
 *
 * Passed:
 *	a0.l = buffer to store data read from sector buffer
 */
/* 306de: 00e01ea2 */
/* 206de: 00e019b8 */
iderdsec:
#if TOSVERSION >= 0x300
        moveq.l   #127,d0
        lea.l     ide_datar,a1
iderds1:
        move.l    (a1),(a0)+
        dbf       d0,iderds1
#else
	moveq     #$1f,d0                   /* d0 = (# of words of data to read / 8) - 1 */
        lea       (ide_datar).l,a1          /* a1 -> data bus */
rb0:    move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        move.w    (a1),(a0)+                /* read data from bus */
        dbra      d0,rb0                    /* repeat until all done */
#endif
        rts

/*
 * wrtbuf() - writes 512 bytes (128 longs) of data to sector buffer.
 *
 * Passed:
 *	a0.l = buffer with data to write to sector buffer
 */
/* 306de: 00e01eb2 */
/* 206de: 00e019d6 */
idewrsec:
#if TOSVERSION >= 0x300
        moveq.l   #127,d0
        lea.l     ide_datar,a1
idewrs1:
        move.l    (a0)+,(a1)
        dbf       d0,idewrs1
#else
 	moveq     #$1f,d0                   /* d0 = (# of words of data to read / 8) - 1 */
        lea       ide_datar,a1              /* a1 -> data bus */
wb0:    move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        move.w    (a0)+,(a1)                /* write data to bus */
        dbra      d0,wb0                    /* repeat until all done */
#endif
        rts

/*
 * check for IDE ready, with timeout of ~5 sec
 *
 * Passed:
 *	d0.b = IDE drive unit #
 *
 * Returns: 0 - if drive is NOT ready
 *	   1 - if drive is ready
 */
/* 306de: 00e01ec2 */
/* 206de: 00e019f4 */
iderdy:
        andi.b    #$07,d0          /* mask off flags (if any) */
        lsl.b     #4,d0            /* shift unit # to place */
        move.b    d0,ide_headn     /* select device */
#if TOSVERSION >= 0x300
        move.l    #1000,d0        /* set up timer IDERDY */
        add.l     (_hz_200).w,d0
iderdy1:
        btst      #6,ide_stat2     /* status drive ready? */
        bne.s     iderdy2          /* yes, report ok */
#else
        move.b    #$50,d1          /* ready status */
        move.l    #1000,d0         /* set up timer IDERDY */
        add.l     (_hz_200).w,d0
iderdy1:
        cmp.b     ide_stat2,d1     /* is drive ready and not busy? */
        beq.s     iderdy2          /* if so, return with drive ready */
#endif
        cmp.l     (_hz_200).w,d0   /* timeout reached? */
        bcc.s     iderdy1          /* no, try again */
        moveq.l   #0,d0            /* report failure */
        rts
iderdy2:
        moveq.l   #1,d0
        rts


/*
 * gcparm() - get current drive parameters
 *
 * gcparm(buf)
 * char	*buf/*	$4(sp).l    -> data returned by identify()
 *
 * Returns:
 *	d0.w = # of default cylinders
 *	d1.w = # of default heads
 *	d2.w = # of default sectors per track
 */
#if TOSVERSION < 0x300
_gcparm:    movea.l   4(sp),a0                  /* a0 -> data buffer */
            adda.w    #$50,a0                   /* a0 -> where Conner model number is (CONMDL) */
            move.l    a0,-(sp)
            pea       cp2024
            move.w    #6,-(sp)
            bsr.s     strcmp                    /* compare model# with "CP2024" */
            adda.w    #$a,sp                    /* clean up stack */
            tst.w     d0                        /* is unit the CP2024 (Kato 20Mb)? */
            bne.s     gcp0                      /* if not, handle the normal way else return default values of CP2024 */
            move.w    #$267,d0                  /* d0.w = # of cylinders (CP20NCYL) */
            move.w    #4,d1                     /* d1.w = # of heads (CP20NHEAD) */
            move.w    #$11,d2                   /* d2.w = # of spt (CP20NSPT) */
            bra.s     gcpend
gcp0:       movea.l   4(sp),a0
            move.w    2(a0),d0                  /* d0.w = # of cylinders */
            move.w    6(a0),d1                  /* d1.w = # of heads */
            move.w    $c(a0),d2                 /* d2.w = # of sectors per track */
gcpend:     rts

conner:     dc.b      'Conner',0
            dc.b      $00
cp2024:     dc.b      'CP2024',0
            dc.b      $00


/*
 * strcmp() - compare two strings
 *
 * Passed:
 *	4(sp).w  = n (# of bytes to compare)
 *	6(sp).l  = address of first string
 *	10(sp).l = address of second string
 *
 * Returns:
 *	d0.w = 0	if first n bytes of the 2 strings are the same
 *	     = non-0    otherwise
 */
strcmp:     movem.l   d1/a0-a1,-(sp)            /* save registers d1, a0 and a1 */
            move.w    $10(sp),d1                /* d1 = byte count */
            subq.w    #1,d1                     /* dbra likes one less */
            movea.l   $12(sp),a0                /* a0 -> string 1 */
            movea.l   $16(sp),a1                /* a1 -> string 2 */
            moveq     #1,d0                     /* assume strings are not the same */
str0:       cmpm.b    (a0)+,(a1)+               /* characters the same? */
            bne.s     str1                      /* if not, return */
            dbra      d1,str0                   /* else compare next character */
            moveq     #0,d0                     /* the strings are the same */
str1:       movem.l   (sp)+,d1/a0-a1            /* restore registers d1, a0 and a1 */
            rts

#endif

	bss

	xdef dmarwflg
	comm dmarwflg,1

	xdef scsicmd
	comm scsicmd,10

	xdef idebuf
	comm idebuf,SECTOR_SIZE

	xdef lastacstm
	comm lastacstm,4
