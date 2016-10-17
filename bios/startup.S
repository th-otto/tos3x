/*
 * startup.S - EmuTOS startup module
 *
 * Copyright (C) 2001-2016 by the EmuTOS development team
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */



#include "asmdefs.h"
#include "header.h"
#include "tosvars.inc"
#include "biosdefs.h"


/* ==== Defines ============================================================== */
cart_base  equ   $00fa0000 /* Base of ROM port cartridge */
GEM_MUPB_MAGIC equ $87654321

nvram_rtc_year_offset equ 1980-1968

#define RR_MAGIC    $12123456
#define RR_CHKSUM   $5678

save_siz equ 238

/* sizeof(IOREC) */
IOREC_SIZE equ 14

FASTRAMBASE equ $01000000

#define ACIA_IKBD_BASE $fffffc00
#define ACIA_MIDI_BASE $fffffc04

#define ACIA_CTRL 0
#define ACIA_DATA 2

#define SCCA_BASE $FFFF8C81
#define SCCA_CTRL SCCA_BASE+0
#define SCCA_DATA SCCA_BASE+2
#define SCCB_BASE $FFFF8C85
#define SCCB_CTRL SCCB_BASE+0
#define SCCB_DATA SCCN_BASE+2


/*
 * struct dsb {
 *    uint8_t drivetype;    == 8 for HD
 *    uint8_t pad;
 *    uint16_t curtrack;
 *    uint16_t density;     0 = SD, 3 = DD
 *    uint16_t seekrate;
 * };
 */
fd_drivetype equ 0
fd_curtrack equ 2
fd_density equ 4
fd_seekrate equ 6

/*
 * size of a raw sector for flopfmt:
 *    12 gap $00
 * +   3 id mark $f5
 * +   1 address mark $fe
 * +   1 track #
 * +   1 side #
 * +   1 sector #
 * +   1 sector size
 * +   1 checksum $f7
 * +  22 gap $4e
 * +  12 gap $00
 * +   3 id mark
 * +   1 address mark $fb
 * + 512 data
 * +   1 checksum $f7
 * +  40 gap $4e
 * = 612
 */
#define FD_RAWSEC 612
#define FD_RAWTRACK (60 + 18 * FD_RAWSEC + 1401)

#define DMA_DATA $ffff8604
#define DMA_CTRL $ffff8606

/* ==== Definitions ========================================================== */

        xdef  _os_entry
        xdef  _run_cartridge_applications

/* ==== Iorec offsets ======================================================== */

#define buf
size    equ 4
head    equ 6
tail    equ 8
low     equ 10
high    equ 12

rcv      equ 2*IOREC_SIZE+0
wr5      equ 2*IOREC_SIZE+1
rcts     equ 2*IOREC_SIZE+2
xoff     equ 2*IOREC_SIZE+3
ctr      equ 2*IOREC_SIZE+4
xonoffc  equ 2*IOREC_SIZE+5
baudrate equ 2*IOREC_SIZE+6
datamask equ 2*IOREC_SIZE+7
EXTIOREC_SIZE equ 2*IOREC_SIZE+8

BCONMAP_START_HANDLE equ 6

/* ==== References =========================================================== */

/* Memory */
        xref memchk
        
/* BIOS C part */
/*        xref _biosmain     / * BIOS C entry point */

/* Overall memory map */
/*       xref __end         / * end of bss + comm sections (ld generated) */
/*       xref __etext       / * end of text section */
/*       xref __edata       / * end of data section */
        xref _endvdibss    /* end of VDI BSS */
        xref _endgembss    /* end of GEM BSS */
        xref __ebss        /* end of BSS */

/* ==== Structures =========================================================== */

#define kbdvec    _kbdvecs+0
#define midivec   _kbdvecs+4
#define vkbderr   _kbdvecs+8
#define vmiderr   _kbdvecs+12
#define statvec   _kbdvecs+16
#define mousevec  _kbdvecs+20
#define clockvec  _kbdvecs+24
#define joyvec    _kbdvecs+28
#define midisys   _kbdvecs+32
#define ikbdsys   _kbdvecs+36
#define ikbdstate _kbdvecs+40
#define kbdlength _kbdvecs+41


/* ==== hardware addresses =================================================== */

scu_gp1   equ $FFFF8E09
memconf   equ $FFFF8001
tt_mcu    equ $FFD00000
v_syncmod equ $FFFF820A
sndmactl  equ $FFFF8900
mwmask    equ $FFFF8924
mwdata    equ $FFFF8922

v_bas_h   equ $FFFF8201
v_bas_m   equ $FFFF8203
v_bas_l   equ $FFFF820d

vme_mask  equ $FFFF8E0D
sys_mask  equ $FFFF8E01

v_shf_mod equ $FFFF8260
shift_tt  equ $FFFF8262

/* =========================================================================== */
/* ==== TEXT segment (TOS image) ============================================= */
/* =========================================================================== */

        text


/*
 * OSHEADER
 */

        xdef  _os_beg
        xdef  _os_magic
        xdef  _os_date
        xdef  _os_conf
        xdef  _os_dosdate

        xdef _ui_mupb

        xref  _root
        xref  _shifty
        xref  _run
		
_os_entry:
    bra.s   _main       /* os_entry, branch to _main */
os_version:
    dc.w    ATOSVERSION  /* os_version, TOS version */
reseth:
    dc.l    _main       /* reseth, pointer to reset handler */
_os_beg:
    dc.l    _os_entry   /* os_beg, base of os = _sysbase */
os_end:
    dc.l    _endvdibss  /* os_end, end of VDI BSS */
os_res1:
    dc.l    _main       /* os_res1, reserved */
_os_magic:
    dc.l    _ui_mupb    /* os_magic, pointer to GEM's MUPB */
_os_date:
    dc.l   OS_DATE      /* os_date, Date of system build */
_os_conf:               /* Flag for PAL version + country */
#ifdef CONF_MULTILANG
    dc.w   OS_CONF_MULTILANG
#else
#if OS_COUNTRY == CTRY_PL
    dc.w   (CTRY_UK<<1)+OS_PAL
#else
    dc.w   (OS_COUNTRY<<1)+OS_PAL
#endif
#endif
_os_dosdate:
    dc.w   OS_DOSDATE   /* os_date, Date of system build in GEMDOS format */
os_root:
    dc.l   _root        /* Pointer to the GEMDOS mem pool */
os_kbshift:
    dc.l   _shifty      /* Pointer to the keyboard shift keys states */
os_run:
    dc.l   _run         /* Pointer to a pointer to the actual basepage */
os_dummy:
    dc.l   0            /* _main should start at offset 0x30, shouldn't it? */


        data
_ui_mupb:
        dc.l GEM_MUPB_MAGIC
        dc.l _endgembss
        dc.l gemstart
zero:
	    dc.l 0

		text


/*
 * Get in supervisor mode and reset all Hardware
 *
 * The following considerations have been taken into account:
 * 1. for unknown reasons, real Falcons need an access to location $ffff8006
 *    before and after the reset instruction
 * 2. in order to run the same code on systems without a register at $ffff8006,
 *    the bus error vector must be intercepted prior to #1
 * 3. since non-68000 systems use the VBR to point to the start of the exception
 *    vectors, the VBR must be zeroed prior to #2 via MOVEC
 * 4. in order to run the same code on 68000 systems (which don't have MOVEC),
 *    the illegal instruction vector must be intercepted prior to #3.  for this,
 *    it doesn't matter if the VBR is non-zero because, if it is, the MOVEC
 *    instruction must be legal so the trap won't get taken ...
 */

_main:
/* disable the interrupts */
/* this is useless on cold boot, */
/* but it matters for software reset */
        move    #$2700,sr

/* special TT reset */
#if TOSVERSION >= 0x300
        move.w #$0100,($FFFF8606).w
        move.w #$0000,($FFFF8606).w
#endif
        reset                  /* reset all hardware */

/*
 * Check for diagnostic cartridge
 */
        cmp.l   #$fa52235f,cart_base    /* magic - is cartridge present? */
        bne.s   nodiag                  /* no -> go on */
        lea     nodiag(pc),a6           /* save return address */
        jmp     cart_base+4             /* execute diagnostig cartridge */
nodiag:

/*
 * disable the instruction/data caches
 */
#if TOSVERSION >= 0x300
        move.l  #$00000808,d0     /* clear & disable instruction/data caches on */
        D0_CACR              /*  68020-60 (even though bit usage differs!) */

        moveq   #0,d0
        D0_VBR           /* initialise VBR to zero */
/*
 * disable the MMU
 */
        TO_TC(zero)          /*  we're done if we get a lineF exception ... */
        TO_TTR0(zero)        /* first we initialise the TTRs (ACRs on a 68ec030) */
        TO_TTR1(zero)
        FRESTORE(zero)
        
        btst      #0,(scu_gp1).w			/* memconfig valid? */
        beq.s     noreset
#endif
        lea       memcret(pc),a6
        bra       memchk
memcret:
        bne.s      noreset
        move.b    (memctrl).w,(memconf).w  /* initialize memory controller */
        
#if STBOOK
			move.w    (STConfig).w,d0           /* Configuration on ST Book */
            cmp.b     #$fc,d0                   /* power pressed while the ST Book is closed? */
            beq.s     reset2b                   /* yes => do not execute reset vector */
            move.w    (tt_mcu+4).l,d0           /* ST Book: ??? */
            and.b     #6,d0                     /* check bit 1 & 2 */
            bne.s     reset2c
reset2b:    clr.l     (resvalid).l
reset2c:
#endif

/*
 * Check, if we should jump to a reset vector after warmboot
 * The high byte of resvector must be zero,
 * it must be even, and cannot be entirely zero.
 */

resetvec:
        cmpi.l   #$31415926,(resvalid).w  /* Jump to resetvector? */
        bne.s    noreset                /* No --> noreset */
        move.l   (resvector).w,d0       /* Yes: old resvec to d0 */
#if TOSVERSION < 0x300
        tst.b    (resvector).w          /* bits 24..31 must be zero */
        bne.s    noreset                /* (they aren't, so punt) */
#endif
        btst     #0,d0                  /* Address odd ? */
        bne.s    noreset                /* Yes --> noreset */
        movea.l  d0,a0                  /* resvec */
        lea      resetvec(pc),a6        /* save return address */
/* Note: the return address intentionally points to the resvalid test. */
/* So the reset routine must explicitly clear resvalid before returning */
/* to a6, otherwise there will be an infinite loop. */
        jmp      (a0)                   /* jump to resvec */
noreset:


/*
 * memory configuration
 */

/*
 * do/test memory config and clear it
 *  Initialize PSG output ports.
 *  Make port A and B output-only,
 *  initialize floppy select lines (so
 *  that none are selected)
 */
meminit:

        lea.l     ($FFFF8800).w,a0
        move.b    #$07,(a0)  /* select mixer control */
        move.b    #$C0,2(a0) /* Port A+B = output */
        move.b    #$0E,(a0)  /* select Port A */
        move.b    #$07,2(a0) /* deselect floppy drives */
        
#if TOSVERSION >= 0x300
        move.b    #$01,(v_syncmod).w /* set video sync */
#else
            btst      #0,_os_conf+1          /* check bit: configured for 50hz? */
            beq.s     notpal                    /* (nope -- we're good ol' NTSC) */
            lea       ret_1b(pc),a6
            bra       waitvbl                   /* a short delay for PAL */
ret_1b:     move.b    #2,(v_syncmod).w             /* yes -- twiddle to 50hz */
notpal:
#if STBOOK
		 	move.b    #2,(v_shf_mod).w          /* Switch to 640x400x1 */
            move.b    #$80,(lcdPowerControl).w  /* LCD display on */
            tst.w     (tt_mcu).l                /* ST Book: ??? */
#endif
#endif

/*
 * next, set up the palette(s)
 */

        lea.l     ($FFFF8240).w,a1
        move.w    #$000F,d0
        lea.l     dflt_pal(pc),a0
initpal:
        move.w    (a0)+,(a1)+
        dbf       d0,initpal

/* set Video screen memory position to 0x10000 */
        move.b    #$01,(v_bas_h).w
        clr.b     (v_bas_m).w

#if TOSVERSION >= 0x300
        btst      #0,(scu_gp1).w
        beq.s      domemchk
#endif

        move.b    (memctrl).w,d6

        move.l    (_phystop).w,d5
        lea       memcret2(pc),a6
        bra       memchk
memcret2:
        beq       nomemchk

#if STBOOK
/* --- init vars + hardware: */
            move.b    #%1010,d6                 /* setup controller for 2Mb/2Mb - only valid configuration on a ST Book */
            move.b    d6,(memconf).w            /* setup memory controller */

/* --- write test-pattern to determine memory configuration: */
            move.l    #$400000,d5               /* d5 -> physical top of memory (4Mb) */
            move.l    #$6161964,d0              /* 16th June 1964... */
            move.l    #$3251987,d1              /* 25th March 1987... */
            move.l    d0,($300010).l
            move.l    d1,($300014).l
            cmp.l     ($300010).l,d0            /* test if 4MB is actually installed */
            bne.s     ret_2a
            cmp.l     ($300014).l,d1
            beq.s     ret_2b
ret_2a:     move.l    #$100000,d5               /* d5 -> physical top of memory (1Mb) */

ret_2b:     lea       ($8000).l,sp

#else
#if TOSVERSION < 0x300
/* First we try to configure the memory controller */

            clr.w     d6
            move.b    #$a,(memconf).w			/* default: setup controller for 2Mb/2Mb */

            movea.w   #$8,a0
            lea       ($200008).l,a1			/* + 2Mb */
            clr.w     d0
chkpatloop: move.w    d0,(a0)+					/* fill 512-8 bytes with a test pattern */
            move.w    d0,(a1)+
            add.w     #$fa54,d0
            cmpa.w    #$200,a0
            bne.s     chkpatloop

            move.b    #90,(v_bas_l).w			/* wrote low byte of video address */
            tst.b     (v_bas_m).w				/* touch the middle byte (this should reset the low byte) */
            move.b    (v_bas_l).w,d0
            cmp.b     #90,d0					/* low byte not reset? */
            bne.s     chkmem1
            clr.b     (v_bas_l).w				/* try a different low byte value */
            tst.w     (palette).w				/* touch the color palette */
            tst.b     (v_bas_l).w				/* low byte changed? */
            bne.s     chkmem1
            move.l    #$40000,d7				/* 256Kb offset */
            bra.s     chkmem1b
chkmem1:    move.l    #$200,d7					/* 512 byte offset */
chkmem1b:   move.l    #$200000,d1				/* 2Mb = maximum size per bank */

chkmemloop: lsr.w     #2,d6						/* shift memory configuration down by a bank (bank 1 is in bits 0..1, bank 0 is in bits 2..3) */

            movea.l   d7,a0						/* + 512/256Kb bytes */
            addq.l    #8,a0
            lea       chkmem3(pc),a4
            bra       memchk
chkmem3:    beq.s     chkmem7					/* bank is not working => */

            movea.l   d7,a0
            adda.l    d7,a0						/* + 1024/512Kb byte */
            addq.l    #8,a0
            lea       chkmem4(pc),a4
            bra       memchk
chkmem4:    beq.s     chkmem6					/* bank has 512Kb of memory => */

            movea.w   #$8,a0					/* + 0 bytes */
            lea       chkmem5(pc),a4
            bra       memchk
chkmem5:    bne.s     chkmem7					/* bank is empty => */

            addq.w    #4,d6						/* 4+4 = 1000 2Mb bank size */
chkmem6:    addq.w    #4,d6						/* 4   = 0100 512Kb bank size */
chkmem7:    sub.l     #$200000,d1				/* - 2Mb */
            beq.s     chkmemloop
            move.b    d6,(memconf).w			/* set memory configuration */

#else
domemchk:


/* =========================================================================== */
/* ==== memconf for TT ======================================================= */
/* =========================================================================== */
/*
 * The following configurations are valid for ST-RAM in a TT, listed as
 * total RAM (motherboard RAM + optional daughterboard RAM):
 *  2MB (2MB + 0MB), 4MB (2MB + 2MB), 10MB (2MB + 8MB)
 * [According to the TT rev B MCU functional description, it is also
 * possible to have 8MB (8MB + 0MB) and 10MB (8MB + 2MB) configurations,
 * but I have not seen this.]  When an 8MB board is present, it is always
 * mapped to the first 8MB of memory.
 *
 * Systems with only 2MB boards require a memcntlr value of 0x05; systems
 * with an 8MB board require a memcntlr value of 0x0a.  We detect which
 * size of board is in low memory by assuming an 8MB board, then checking
 * for different memory addresses mapping to the same physical address.
 *
 * rfb (sep/2016)
 */
        move.w    #$0005,d6           /* assume 2MB board */
        move.b    #$0A,(memconf).w  /* set memory controller for 8MB */
        moveq.l   #0,d0               
        move.l    d0,($00001008).w    /* zero out test addresses */
        move.l    d0,($0000100C).w
        move.l    #$06161964,d0       /* write test value #1 to an address */
        move.l    d0,($00000008).w    /*  that may be mapped into test address #1 */
        cmp.l     ($00001008).w,d0    /* did test address #1 get updated? */
        bne.s     set_8MB             /* no, must be an 8MB board */
        move.l    #$04251987,d0       /* yes, repeat with test value #2 to be sure */
        move.l    d0,($0000000C).w    /* did test address #2 get updated? */
        cmp.l     ($0000100C).w,d0    /* yes, use the 2MB memcntlr value  */
        beq.s     set_2MB
set_8MB:
        move.w    #$000A,d6           /* set 8MB memcntlr value */
set_2MB:
        move.b    d6,(memconf).w    /* update memcntlr */
#endif
#endif

#if !STBOOK
/*
 * We now need to size the actual memory present.  We test memory,
 * starting at 128K and increasing by 128K each time.  For each area of
 * memory, we start at the current address, and work downwards, storing
 * a series of values; we then verify that the values have been stored
 * correctly.  An invalid value (or a bus error when we try to access a
 * memory location) causes us to stop and size the memory accordingly.
 */

        lea.l     $00008000,a7
        movea.l   (_buserror).w,a4
        lea.l     st_ramsize_done(pc),a0 /* bus error means end of ram */
        move.l    a0,(_buserror).w
        move.w    #$FB55,d3           /* d3.w = difference between adjacent stored values */
        move.l    #$00020000,d7       /* d7 = address of first memory test zone & gap between memory test zones */
                                        
        movea.l   d7,a0               /* a0 = address of current test zone */
next_zone:
        movea.l   a0,a1               /* a1 -> memory being tested */
        move.w    d0,d2               /* d2.w = test value */
        moveq.l   #42,d1              /* d1.w = loop count-1 */
store_mem:
        move.w    d2,-(a1)            /* store a value */
        add.w     d3,d2               /* update the value for next time */
        dbf       d1,store_mem        /* rinse & repeat */
/* now check the values we stored */
        movea.l   a0,a1               /* a1 -> test area */
        moveq.l   #42,d1              /* loop count */
check_mem:
        cmp.w     -(a1),d0            
        bne.s     st_ramsize_done   /* miscompare, ram invalid, exit */
        clr.w     (a1)                /* else zero it out */
        add.w     d3,d0               /* update for next compare */
        dbf       d1,check_mem        /* until we're done */
        adda.l    d7,a0               /* point to next test area */
        bra.s     next_zone
st_ramsize_done:                    /* bus error or bad ram  */
        suba.l    d7,a0               /* (a0 -> first zone that failed) */
        move.l    a0,d5               /* d5 = st ram size */
        move.l    a4,(_buserror).w
#endif

/* =========================================================================== */
/* ==== This part is executed only on first boot ============================= */
/* =========================================================================== */

        /* Clear the memory */
        movea.w   #sysvars_start,a0
        move.l    d5,d4
        moveq.l   #0,d0
clrmem:
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        cmpa.l    d4,a0
        bne.s     clrmem

        /* Store the ST-RAM parameters in the ST-RAM itself */
        move.b  d6,(memctrl).w             /* set copy of hw memory config */
        move.l  d5,(_phystop).w            /* highest address as phystop */

        move.l    #$752019F3,(_memvalid).w /* set memvalid to ok */
        move.l    #$237698AA,(_memval2).w  /* set memval2 to ok */
        move.l    #$5555AAAA,(_memval3).w  /* set memval3 to ok */

/* 
 * We must set the FastRAM refresh rate before we use it. The
 * rate is set by writing a "don't care" value to an address in
 * the range 0xffd00000-0xffd000ff; the low-order byte of the
 * address sets the refresh rate (specifically the number of
 * system clock cycles between each refresh request).  TT TOS
 * uses the value 0xe0; so do we.
 * NOTE: whether we actually have FastRAM or not, setting the
 * value always causes a bus error, so we need to handle it.
 */
#if TOSVERSION >= 0x300
        move.l  #refresh_done,(_buserror).w      /* bus error vector */
        move.w  #$0000,tt_mcu+224
refresh_done:

        /* Can we access FastRAM region? */

        move.l    #tt_ramsize_done,(_buserror).w
        move.w    #$FB55,d3           /* d3.w = difference between adjacent stored values */
        moveq.l   #0,d0
        move.l    #$00020000,d7       /* d7 = address gap between memory test zones */
        movea.l   #$01020000,a0       /* a0 = address of current test zone */
next_tt:
        movea.l   a0,a1               /* a1 -> memory being tested */
        move.w    d0,d2               /* d2.w = test value */
        moveq.l   #42,d1              /* d1.w = loop count-1 */
store_tt:
        move.w    d2,-(a1)            /* store a value */
        add.w     d3,d2               /* update the value for next time */
        dbf       d1,store_tt
/* now check the values we stored */
        movea.l   a0,a1               /* a1 -> test area */
        moveq.l   #42,d1              /* loop count */
check_tt:
        cmp.w     -(a1),d0
        bne.s     tt_ramsize_done
        clr.w     (a1)
        add.w     d3,d0
        dbf       d1,check_tt
        adda.l    d7,a0
        bra.s     next_tt
tt_ramsize_done:                    /* bus error or bad ram  */
        suba.l    d7,a0
        cmpa.l    #$01000000,a0
        bne.s     ttramok
        suba.l    a0,a0
ttramok:
        move.l    a0,d5
        move.l    d5,(_ramtop).w
        move.l    #$1357BD13,(_ramvalid).w
        bset      #0,(scu_gp1).w
#else
        clr.l     _ramtop                /* no FASTRAM available */
        move.l    #$1357bd13,(_ramvalid).w   /* ramtop is valid (ramvalid == RAMMAGIC) */
#endif

nomemchk:


/* detect basic STe/TT DMA sound */

#if OS_COUNTRY == CTRY_PL
		/* apparently a patch that was applied by the maker of the PL version */
		clr.l     (_shell_p).w
        movea.w   #_supstkend,a7
#else
        movea.l   #_supstkend,a7
#endif

#if STBOOK
/*
 *  Initialize interrupt vectors
 *
 *  The exception vectors are pointed to a cold boot (coldboot)
 *  during startup.
 *
 *  Trap 2 and Divide-by-zero are pointed at an rte
 *
 *  The HBLANK, VBLANK, line 1001 [someday: line 1111), trap 13, trap 14,
 *  and "extended" trap vectors are initialized appropriately.
 *
 */
            lea       _rte(pc),a3               /* a3 -> handy RTE */
            lea       _rts(pc),a4               /* a4 -> handy RTS */

/* --- setup 64 vectors: */
            lea       (coldboot).l,a1           /* a1 -> during boot all exception trigger a coldboot, which erases all memory and resets */
            lea       (_buserror).w,a0       /* a0 -> interrupt RAM */
            move.w    #$3f,d0                   /* d0 = count */
sei1:       move.l    a1,(a0)+                  /* write vector */
            dbra      d0,sei1                   /* (loop to write more vectors) */
            move.l    a3,(_zerodiv).w   /* divide-by-zero vector -> rte */

            move.l    a3,(_lvl7vec).w          /* level #7 interrupt -> rte (power exception in a ST Book) */
            moveq     #6,d0
            lea       (_lvl1vec).w,a1
sei2:       move.l    #_rte,(a1)+               /* level #1 ... level #6 to RTE */
            dbra      d0,sei2

/* --- install OS interrupt vectors: */
            move.l    #int_vbl,(_lvl4vec).w    /* vblank handler */
            move.l    #int_hbl,(_lvl2vec).w    /* hblank handler */
            move.l    a3,(_trap2vec).w        /* (empty) trap#2 handler */
            move.l    #_biostrap,(_trap13ve).w     /* trap #13 handler */
            move.l    #_xbiostrap,(_trap14vec).w    /* trap #14 handler */
            move.l    #line1010,(_lineatrap).w /*line 1010 handler */
            move.l    a4,(_etv_timer).w          /* default timer-tick vector -> rts */
            move.l    #_critret,(_etv_critic).w  /* default critical error handler */
            move.l    a4,(_etv_term).w           /* default terminal vector -> rts */

/*
 *  Setup the vblank deferred vector list.
 *  (This data structor is ugly,
 *   but we seem to be stuck with it).
 *
 */
            lea       (_vbl_list).w,a0          /* a0 -> default list of vbl locs */
            move.l    a0,(_vblqueue).w          /* install ptr to them */
            move.w    #7,d0                     /* clear vbl vectors */
avbl:       clr.l     (a0)+                     /* one at a time */
            dbra      d0,avbl

            lea       tconstat,a0
            movea.w   #xconstat,a1
            moveq     #31,d0
tconl:      move.l    (a0)+,(a1)+
            dbra      d0,tconl

            movea.l   (_buserror).w,a0
            movea.l   sp,a1
            move.l    #vmeinit,(_buserror).w
            move.b    #$40,(vme_mask).w         /* Enable IRQ6 from VMEBUS/MFP */
            move.b    #$14,(sys_mask).w         /* VSYNC & HSYNC enable in the VME Bus System Control Unit */
vmeinit:    move.l    a0,(_buserror).w
            movea.l   a1,sp
#endif

#if OS_COUNTRY == CTRY_PL
        clr.b     (_has_dmasound).w
#else
        clr.b     _has_dmasound
#endif

#if STBOOK
        sf        STEFlag               /* ST Book is not an Atari STE */
#else
        movea.l   a7,a6
        move.l    #nodmasound,(_buserror).w
        clr.w     (sndmactl).w
        st        _has_dmasound
        lea.l     microwtab(pc),a0
        move.w    (a0)+,(mwmask).w
        bra.s     checkmicro
microwtab:
        dc.w      $0ffe  /* value for microwire mask register */
        dc.w      $09d1                     /* 10-011-101000-1 = LCM - Master Volume - 0 db volume (max) - end-bit */
        dc.w      $0aa9                     /* 10-101-010100-1 = LCM - Left channel volume - 0 db volume (max) - end-bit */
        dc.w      $0a29                     /* 10-100-010100-1 = LCM - Right channel volume - 0 db volume (max) - end-bit */
        dc.w      $090d                     /* 10-010-000110-1 = LCM - Trebble control - 0 db (linear) - end-bit */
        dc.w      $088d                     /* 10-001-000110-1 = LCM - Bass control - 0 db (linear) - end-bit */
        dc.w      $0803                     /* 10-000-000001-1 = LCM - Mixer - DMA + YM2149 - end-bit */
        dc.w      0
nextmicro:
        move.w    d0,(mwdata).w
waitmicro:
        tst.w     (mwdata).w
        bne.s     waitmicro
checkmicro:
        move.w    (a0)+,d0
        bne.s     nextmicro
nodmasound:
        movea.l   a6,a7

#if TOSVERSION < 0x300
			move.b    #90,(v_bas_l).w           /* write low byte of video address */
			tst.b     (v_bas_m).w               /* access the medium byte */
			move.b    (v_bas_l).w,d0            /* (which should reset the low byte on an STE!) */
			cmp.b     #90,d0                    /* reset? */
			bne.s     nostedetect               /* yes => STE detected */
			clr.b     (v_bas_l).w               /* clear the low byte again */
			tst.w     (palette).w               /* access the color palette */
			tst.b     (v_bas_l).w
nostedetect:
            sne       STEFlag               /* <>0 => no STE hardware available */
#else
        sf        STEFlag
#endif
#endif


/* clear the remainder of the BSS section */

        movea.l   #__ebss,a1
        movea.l   #__sbss,a0
        moveq.l   #0,d0
clrbss:
        move.w    d0,(a0)+
        cmpa.l    a0,a1
        bne.s     clrbss


/* initialize the MMU */
#if TOSVERSION >= 0x300
        bsr       initmmu
#endif

/* clear screen memory */

#if TOSVERSION >= 0x300
scrsize equ ((1280*960)/8+$100)
#else
scrsize equ ((640*400)/8+$300)
#endif

        movea.l   (_phystop).w,a0
        suba.l    #scrsize,a0
        move.w    #(scrsize/16)-1,d1
        move.l    a0,(_v_bas_ad).w
        move.b    (_v_bas_ad+1).w,(v_bas_h).w
        move.b    (_v_bas_ad+2).w,(v_bas_m).w
        moveq.l   #0,d0
clrscr:
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        dbf       d1,clrscr

/* does os_magic indicate GEM in ROM? */
        movea.l   _os_magic(pc),a0
        cmpi.l    #GEM_MUPB_MAGIC,(a0)
        beq.s     gemok
        lea.l     _os_beg,a0
gemok:  
        move.l    4(a0),(_end_os).w
        move.l    8(a0),(_exec_os).w

/* =========================================================================== */
/* initialize system variables */
/* =========================================================================== */

/* --- Disk vectors: */
        move.l    #_bhdv_init,(_hdv_init).w
        move.l    #_bhdv_rwabs,(_hdv_rw).w
        move.l    #_bhdv_getbpb,(_hdv_bpb).w
        move.l    #_bhdv_mediach,(_hdv_mediach).w
        move.l    #_bhdv_boot,(_hdv_boot).w

/* --- device vectors: */
        move.l    #_bco0stat,(_prt_stat).w
        move.l    #_bcon0out,(_prt_vec).w
        move.l    #_bco6stat,(_aux_stat).w  /* BIOS device 1 (AUX) defaults to RS232 port */
        move.l    #_bcon6out,(_aux_vec).w   /* BIOS device 1 (AUX) defaults to RS232 port */
        move.l    #_bscr_dump,(_scr_dmp).w

/* --- Randoms: */
        move.l    (_v_bas_ad).w,(_memtop).w
        move.l    (_end_os).w,(_membot).w          /* set bottom of memory (for DOS) */
        move.w    #8,(_nvbls).w                    /* default number of vbl queue entries */
        st        (_fverify).w                     /* enable write-verify */
        move.w    #3,(_seekrate).w                 /* set default seek-rate */
        move.l    #_dskbuf,(_dskbufp).w            /* set pointer to disk buffer */
        move.w    #-1,(_dumpflg).w                 /* initialize print-count */
        move.l    #_os_entry,(_sysbase).w          /* unnessary; will be overwritten by instoshdr below */
        move.l    #save_beg+save_siz,(_savptr).w   /* register-save pointer for traps 13&14 */
        move.l    #bios_unimpl,(_swv_vec).w        /* ignore monitor changes for now */
        clr.l     (_drvbits).w                     /* remove all drives */
        move.l    #_do_bell,(_bell_hook).w
        move.l    #_do_keyclick,(_kcl_hook).w

        bsr       instoshdr

/* =========================================================================== */
/* initialize cookie jar */
/* =========================================================================== */

        lea.l     cookies,a0
        move.l    a0,(_p_cookies).w
        move.l    #$5F435055,(a0)+ /* '_CPU' */

/* detect CPU type */
/* (actually nonsense since 68030 instructions have already been used above) */
        moveq.l   #0,d1         /* assume 68000 */
        movea.w   #_illinst,a2
        movea.l   (a2),a3       /* save illegal instruction vector */
        movea.l   a7,a1         /* save SP */
        move.l    #cpuexit,(a2)
        CCR_D0                  /* move.w ccr,d0 legal on 68010+ */
        moveq.l   #10,d1        /* assume 68000 */
        dc.w $49c0              /* extb.l    d0 */
        moveq.l   #20,d1        /* assume 68020 */
        CACR_D0
        bset      #9,d0         /* set an unused bit */
        D0_CACR
        CACR_D0
        bclr      #9,d0
        beq.s     cpuexit
        moveq.l   #30,d1        /* assume 68030 */
        D0_CACR
cpuexit:
        movea.l   a1,a7         /* restore SP */
        move.l    a3,(a2)       /* save illegal instruction vector */
        move.l    d1,(a0)+      /* set _CPU cookie */
        sne       (_longframe+1).w

#if STBOOK
            move.l    #$5F56444F,(a0)+             /* setup VDO cookie: Video hardware */
            move.l    #$10001,(a0)+             /* 1,1 => Atari STE, ST-Book */
            move.l    #$5F4D4348,(a0)+             /* setup MCH cookie: Machine type */
            move.l    #$10001,(a0)+             /* 1,1 => Atari STE, ST-Book */

            move.b    #$7f,d0
            tst.b     (STEFlag).l
            bne.s     cooSTE
            move.l    #$5F535749,(a0)+             /* setup SWI cookie: DIP configuration switches */
            moveq     #0,d0
            move.w    (STConfig).w,d0
            lsr.w     #8,d0
            move.l    d0,(a0)+                  /* all DIP switches as a bit mask 0..7 */
cooSTE:  moveq     #3,d1                     /* bit 0: PSG, bit 1: 8-bit DMA */
            move.l    #$5F534E44,(a0)+             /* setup SND cookie: Sound hardware */
            btst      #7,d0                     /* DIP switch 7 on? */
            bne.s     cooSND                 /* (punt) */
            bclr      #1,d1                     /* no 8-bit DMA sound */
cooSND:  move.l    d1,(a0)+
            btst      #6,d0                     /* DIP switch 6 on? */
            bne.s     cooFDC                 /* (punt - no HD floppy) */
            move.b    #8,(dsb0).l               /* select HD density for drive A */
            move.l    #'_FDC',(a0)+             /* setup FDC cookie: Floppy disk controller */
            move.l    #$1415443,(a0)+           /* 'FDC' | (1 << 24) */
cooFDC:  move.l    #$5F465055,(a0)+             /* Setup FPU cookie: Type of the FPU */
            moveq     #0,d7                     /* 0 = no FPU */
            suba.w    #$24,sp
            move.l    (_lineftrap).w,(sp)
            move.l    (coprocexception).w,4(sp)
            move.l    #cooFPU,(_lineftrap).w
            move.l    #cooFPU,(coprocexception).w
            lea       8(sp),a1
            movea.w   #ffcp_unorderedcond,a2
            move.l    #cooFPU2,d0
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            move.l    (a2),(a1)+
            move.l    d0,(a2)+
            clr.l     -(sp)
            movea.l   sp,a2
            frestore  (sp)
cooFPU2: move.l    #$20000,d7                /* 0x20000 = 68881 or 68882 as co-processor (Exact type unknown) */
cooFPU:  movea.l   a2,sp
            addq.w    #4,sp
            move.l    (sp)+,(_lineftrap).w
            move.l    (sp)+,(coprocexception).w
            move.l    (sp)+,(ffcp_unorderedcond).w
            move.l    (sp)+,(ffcp_inexactresult).w
            move.l    (sp)+,(ffcp_divzero).w
            move.l    (sp)+,(ffcp_underflow).w
            move.l    (sp)+,(ffcp_operanderror).w
            move.l    (sp)+,(ffcp_inexactresult).w
            move.l    (sp)+,(ffcp_divzero).w
            move.l    d7,(a0)+
            movea.l   (_buserror).w,a1
            movea.l   sp,a2
            move.l    #cooFPU3,(_buserror).w
            move.w    (FPStat).w,d0
            bset      #0,-3(a0)                 /* SFP004 present */
cooFPU3: move.l    a1,(_buserror).w
            movea.l   a2,sp
#else

#if TOSVERSION < 0x300
			tst.b     (STEFlag).l               /* no STE hardware available? */
			beq.s     cooMCH                 /* (correct) */
			move.l    #$5F56444F,(a0)+
			clr.l     (a0)+                     /* 0,0 = Atari ST (260 ST, 520 ST, 1040 ST, Mega ST, ...) */
			move.l    #$5F4D4348,(a0)+
			clr.l     (a0)+                     /* 0,0 = Atari ST */
			bra.s     cooSWI

cooVDO:	move.l    #$5F56444F,(a0)+
			move.l    #$10000,(a0)+             /* 1,0 = STE Shifter */

			move.l    #$10000,d0                /* x = $00 = regular STE */
			movea.l   (_buserror).w,a1
			movea.l   sp,a2
			move.l    #cooMCH1,(_buserror).w
			tst.b     (scu_gp1).w
			move.w    #$10,d0                   /* x = $10 = Mega STE (with SCSI) */
			bra.s     cooMCH2
cooMCH1:	clr.w     d0
			movea.l   a2,sp
			move.l    #cooMCH2,(_buserror).w
			tst.b     (ide_stat2).l
			move.w    #8,d0                     /* x = $08 = STE with IDE (unknown machine) */
cooMCH2:	move.l    a1,(_buserror).w
			movea.l   a2,sp
			move.l    #$5F4D4348,(a0)+
			move.l    d0,(a0)+                  /* 1,x = STE (520, 1040, 2080, 4160, Mega STE, ST Book) */

#else
#if TOSVERSION < 0x400
        move.l    #$5F56444F,(a0)+ /* '_VDO' */
        move.l    #$00020000,(a0)+ /* 2,0 = TT Shifter */
        move.l    #$5F4D4348,(a0)+ /* '_MCH' */
        move.l    #$00020000,(a0)+ /* 2,0 = TT */
#endif
#endif

        move.b    #$7F,d0
        tst.b     STEFlag
        bne.s     nomicro
        move.l    #$5F535749,(a0)+ /* '_SWI' */
        moveq.l   #0,d0
        move.w    ($FFFF9200).w,d0
        lsr.w     #8,d0
        move.l    d0,(a0)+
nomicro:
        moveq.l   #3,d1
        move.l    #$5F534E44,(a0)+ /* '_SND' */
        btst      #7,d0
        bne.s     yesstereo
        bclr      #1,d1
yesstereo:
        move.l    d1,(a0)+
        btst      #6,d0
        bne.s     nofdc
        move.b    #$08,dsb+fd_drivetype
        move.l    #$5F464443,(a0)+ /* '_FDC' */
        move.l    #$01415443,(a0)+ /* FDC_1ATC */
nofdc:

/* detect FPU */

        move.l    #$5F465055,(a0)+ /* '_FPU' */
        movea.l   (_lineftrap).w,a1 /* save Line-F trap */
        movea.l   (_coprovio).w,a2
        movea.l   a7,a3         /* save SP */
        move.l    #nofpu,(_lineftrap).w
        move.l    #nofpu,(_coprovio).w
#if OS_COUNTRY == CTRY_PL
        D0_FP0
#else
        FP0_D0
#endif
        move.l    #$00020000,(a0)+             /* 6888x present */
        bra.s     fpudone
nofpu:
        clr.l     (a0)+
fpudone:
        move.l    a1,(_lineftrap).w /* restore Line-F trap */
        move.l    a2,(_coprovio).w
        movea.l   a3,a7         /* restore SP */

/* check memory-mapped FPU */
        movea.l   (_buserror).w,a1
        movea.l   a7,a2         /* save SP */
        move.l    #nosfp,(_buserror).w
        move.w    ($FFFFFA40).w,d0
        bset      #0,-3(a0)                 /* SFP004 present */
nosfp:
        move.l    a1,(_buserror).w
        movea.l   a2,a7         /* restore SP */

#if TOSVERSION >= 0x300
/* check wether fast ram buffer is needed */
        tst.l     (_ramtop).w
        beq.s     frbdone
        move.l    #$5F465242,(a0)+ /* '_FRB' */
        move.l    (_membot).w,d0
        move.l    d0,(a0)+
        add.l     #$00010000,d0
        move.l    d0,(_membot).w
        move.l    d0,(_end_os).w
frbdone:
#endif

#endif


/* terminate cookie jar */
        clr.l     (a0)+
        move.l    #NCOOKIES,(a0)+

/* =========================================================================== */
/* initialize exception vectors */
/* =========================================================================== */

#if !STBOOK
        lea.l     just_rte(pc),a3
        lea.l     bios_unimpl(pc),a4
        lea.l     any_vec(pc),a1       * default to exception handler displaying bombs
#if TOSVERSION < 0x300
			adda.l    #$2000000,a1				/* exception number in top 8 bits of the address */
#endif
        lea.l     (2*4).w,a0
        move.w    #64-3,d0
initexc:
        move.l    a1,(a0)+
#if TOSVERSION < 0x300
			adda.l    #$1000000,a1				/* increment the exception number */
#endif
        dbf       d0,initexc

        move.l    a3,(_zerodiv).w      * ignore zero divide
        move.l    a3,(_lvl7vec).w      * ignore NMI

        moveq.l   #6,d0
        lea.l     (_lvl1vec).w,a1
initauto:
        move.l    #just_rte,(a1)+      * ignore auto vectors
        dbf       d0,initauto

        move.l    #int_vbl,(_lvl4vec).w
        move.l    #int_hbl,(_lvl2vec).w
        move.l    a3,(_trap2vec).w     * ignore GEM trap for now
        move.l    #_biostrap,(_trap13vec).w
        move.l    #_xbiostrap,(_trap14vec).w
        move.l    #line1010,(_lineatrap).w
        move.l    a4,(_etv_timer).w    * no timer yet
        move.l    #critret,(_etv_critic).w
        move.l    a4,(_etv_term).w     * no user defined function
        lea.l     (_vbl_list).w,a0
        move.l    a0,(_vblqueue).w
        move.w    #7,d0
initvbl:
        clr.l     (a0)+
        dbf       d0,initvbl

/* init bios device vectors */
        lea.l     tconstat,a0
        movea.w   #_bconstat_vec,a1
        moveq.l   #31,d0
initcon:
        move.l    (a0)+,(a1)+
        dbf       d0,initcon

/* init VME bus */
        movea.l   (_buserror).w,a0
        movea.l   a7,a1
        move.l    #vmedone,(_buserror).w
        move.b    #$40,(vme_mask).w
        move.b    #$14,(sys_mask).w
vmedone:
        move.l    a0,(_buserror).w
        movea.l   a1,a7
#endif



/* initialize MFP */
        bsr       initmfp
        move.w    #(4<<8),d0 /* Delay Mode, /50 Prescale, data = 0 (about 10us delay) */
        bsr       mfpdelay

/* reset IKBD */
        move.l    #ikbdres,-(a7)
        move.w    #$0001,-(a7)
        jsr       _ikbdws
        addq.l    #6,a7
        move.w    #(7<<8),d0 /* Delay Mode, /200 Prescale, data = 0 (about 40us delay) */
        move.w    #$000E,d1  /* 15 * 40us = 600us delay */
ikbdwait:
        bsr       mfpdelay
        dbf       d1,ikbdwait

/* Run all boot applications from the application cartridge,
 * before video initialization.
 */
        moveq.l   #2,d0
        bsr       _run_cartridge_applications

/*
 * check monitor type, and set boot resolution
 */
#if STBOOK
            bsr       scanextrom

            move.b    #$80,(lcdPowerControl).w  /* LCD display on */
            move.b    #$80,(lcdPowerControlShadow).w /* LCD display shadow register */
            moveq     #2,d1                     /* Switch to 640x400x1 */
            lea       setvb1(pc),a6
            bra       waitvbl
setvb1:     move.b    d1,(v_shf_mod).w          /* set rez hardware register */

#else
#if TOSVERSION < 0x300
			moveq     #0,d1						/* Switch to 320x200x4 */
			btst      #7,(MFP_GPIP).w			/* monochrome monitor connected? */
			bne.s     nomonomon					/* (no) */
			moveq     #2,d1                     /* Switch to 640x400x1 */
nomonomon:
            lea       setvb1(pc),a6
            bra       waitvbl
setvb1:     move.b    d1,(v_shf_mod).w          /* set rez hardware register */

#else
        moveq.l   #4,d1                     /* assume color, video mode 640x480x16 */
        btst      #7,(MFP_GPIP).w			/* monochrome monitor connected? */
        bne.s     moncolor
        moveq.l   #6,d1                     /* Switch to 1280x960x1 */
moncolor:
        move.b    d1,(shift_tt).w   * set TT shifter video mode
#endif
#endif
        move.b    d1,(_sshiftmod).w  * .. and also system var

/*
 * initialize video
 */
        bsr       blittest                
        jsr       resetdev                /* linaA blitter/no-blitter table init */
        jsr       esc_init                /* clear screen, initialize cursor */
        move.l    #_main,(_swv_vec).w     /* RESET system on monitor change */
        move.w    #$0001,(_vblsem).w      /* enable vblank processing */

/* Run all boot applications from the application cartridge,
 * after hardware initialization.
 */
        clr.w     d0
        bsr       _run_cartridge_applications
#if STBOOK
        bsr       scanextrom
#endif

/* Run all boot applications from the application cartridge,
 * with interrupts enabled.
 */
        move.w    #$2300,sr
        moveq.l   #1,d0
        bsr       _run_cartridge_applications
#if STBOOK
            bsr       scanextrom

            move.l    (_hz_200).w,d0
            addq.l    #3,d0
resDelayL:  cmp.l     (_hz_200).w,d0            /* a short delay of 15-20ms */
            bhi.s     resDelayL
            clr.b     _shifty              /* reset keyboard shift state */
#endif

/*
 * continue with rest of bios initialization
 */
        move.l    #_int_priv,(_privvio).w
        bra       biosinit

/*
 * Flush instruction and data cache (68020+)
 */
flushCaches:
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        CACR_D0
        or.l      #$00000808,d0           /* CI (Clear Instruction Cache), DI (Clear Data Cache) */
        D0_CACR
        move.w    (a7)+,sr
        rts

/*
 * _int_priv - privileged instruction handler
 *
 * Convert a privileged 'MOVE SR,...' to 'MOVE CCR,...'
 */
/* 306de: 00e0060a */
_int_priv:
        movem.l   d0-d2,-(a7)      * saving some registers while we test
        move.l    a1,-(a7)
        move.l    a0,-(a7)
        movea.l   22(a7),a0        * the offending address
        move.w    (a0),d0          * the offending instruction
        move.w    d0,d1            * save for later tests
        and.w     #$FFC0,d0        * isolate move from SR opcode
        cmp.w     #$40C0,d0        * we don't care where "To" was
        bne       pi_back          * Not a MOVE SR,XX instruction
        move.l    #$30004E71,privinst.w   /* move.w d0,d0; nop */
        move.l    #$4E714E75,(privinst+4).w /* nop; rts */
        move.w    d1,d0
        and.w     #$0007,d0        /* <ea> register (bit 0..2) */
        lsl.w     #8,d0
        lsl.w     #1,d0            /* move into bit 9..11 of the destination <ea> */
        or.w      d0,privinst.w    /* insert the destination register */
        move.w    d1,d0
        and.w     #$0038,d0        /* <ea> mode (bit 3..5) */
        lsl.w     #3,d0
        or.w      d0,privinst.w    /* insert the destination addressing mode */
        moveq.l   #2,d2            * skip that many instruction bytes
        cmp.w     #$0180,d0        * is it indexed?
        beq       pi_back          /* not supported -> _term */
        tst.w     d0               * is it data-register direct?
        beq.s     int_p4           * yes go execute it
        cmp.w     #$0140,d0        * is it d(An)?
        beq.s     int_p2           * yes
        cmp.w     #$01C0,d0        * is it absolute?
        bne.s     int_p3
/* <ea> = (xxx).l or (xxx).w */
        and.w     #$0007,d1        * is it absolute short?
        beq.s     int_p1           * yes, skip
        addq.w    #2,d2            * 2 more bytes of instruction
        move.w    4(a0),(privinst+4).w * copy lower 16 bits of address
int_p1:
        addq.w    #2,d2            * 2 more bytes of instruction
        move.w    2(a0),(privinst+2).w * copy remaining 16 bits of address
        bra.s     int_p5
/* <ea> = d16(An) */
int_p2:
        addq.w    #2,d2            * 2 more bytes of instruction
        move.w    2(a0),(privinst+2).w * copy displacement
int_p3:
        and.w     #$0007,d1        * is USP affected?
        cmp.w     #$0007,d1
        bne.s     int_p5           /* no -> regular destination ea */
/* <ea> = d16(A7) or d8(A7,Dn) */
/* Special case because A7 has to be the USP, instead of the current SSP (in A7) */
        move.l    usp,a1           /* have to use the USP, otherwise we wouldn't have gotten the exception */
        andi.w    #$F3FF,privinst.w /* convert A7-relative into A1-relative destination <ea> */
        add.l     d2,22(a7)        /* pc += opcode size */
#if TOSVERSION >= 0x300
        bsr       flushCaches
#endif
        move.w    20(a7),d0        /* d0 = SR */
        jsr       privinst.w       /* execute: MOVE D0,d(A1,Dn); NOP; RTS or MOVE D0,d(A1); NOP; RTS */
        move.l    a1,usp           /* restore USP, but shouldn't have changed anyway */
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        movem.l   (a7)+,d0-d2
        rte
/* <ea> = Dn */
int_p4:
        add.l     d2,22(a7)           /* adjust return PC by number of bytes to skip */
        ori.w     #$0010,privinst.w   /* source ea = (A0) */
#if TOSVERSION >= 0x300
        bsr       flushCaches
#endif
        lea.l     20(a7),a0           * a0 = address of SR from stack frame
        movem.l   8(a7),d0-d2
        jsr       privinst.w          /* execute: MOVE (A0),Dn; NOP; NOP; RTS */
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        adda.w    #$000C,a7           /* skip d0-d2, because they have already been restored */
        rte
int_p5:
        add.l     d2,22(a7)           * adjust return PC by number of bytes to skip
#if TOSVERSION >= 0x300
        bsr       flushCaches
#endif
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        move.w    12(a7),d0
        jsr       privinst.w          /* execute: MOVE D0,<ea>; ...; RTS */
        movem.l   (a7)+,d0-d2
        rte
pi_back:
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        movem.l   (a7)+,d0-d2
        jmp       any_vec             /* illegal instruction => _term */

biosinit:
		jsr       _osinit            /* initialize DOS */

/* --- set the current system time and date */
        move.w    _os_dosdate,_date  /* use BIOS time as current time */
        jsr       ngetdt             /* set current time to RTC time */
        beq.s     clockdone
        bsr       igetdt             /* read time from the keyboard controller */
        swap      d0
        tst.b     d0
        beq.s     clockdone
        move.w    d0,_date
        swap      d0
        move.w    d0,_time
clockdone:
        clr.b     (MFP_TACR).w     /* stop Timer A */
        bclr      #5,(MFP_IERA).w  /* disable Timer A interrupt */

#if TOSVERSION >= 0x300
        move.l    #$00003111,d0
        D0_CACR
#endif


/* 
 * Draw the Atari logo.
 */
        movea.l   #atarilogo,a0
        movea.l   (_v_bas_a).w,a1
#if OS_COUNTRY == CTRY_PL
		/* apparently a patch that was applied by the maker of the PL version */
		lea (lineavars).w,a2
		move.w    (a2),d4   /* LA_PLANES */
		move.w    -2(a2),d5 /* V_BYTES_LIN */
		move.w    d5,d0
		mulu.w    #$0008,d0
		adda.w    d0,a1
        moveq.l   #$0055,d0        /* 86 lines of data */
logocol1:
        moveq.l   #5,d1            /* 12 bytes of data per line */
        movea.l   a1,a2
logocol2:
        move.l    d4,d2
        move.w    (a0)+,d3
logocol3:
        move.w    d3,(a1)+
        subq.w    #1,d2
        bne.s     logocol3
        dbf       d1,logocol2
        lea.l     0(a2,d5.w),a1
        dbf       d0,logocol1
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
#else
        move.b    (_sshiftmod).w,d0
        cmp.b     #$02,d0          /* ST-High resolution? */
        beq.s     logomono
        cmp.b     #$06,d0
        beq.s     logomono         /* TT-High resolution? */
#if TOSVERSION >= 0x300
        adda.w    #((640/8)*4)*4,a1  /* start at screen line 4 */
#else
        adda.w    #((320/8)*4)*4,a1  /* start at screen line 4 */
#endif
        move.w    #$0055,d0        /* 86 lines of data */
logocol1:
        moveq.l   #5,d1            /* 12 bytes of data per line */
logocol2:
        move.w    (a0)+,d2
        move.w    d2,(a1)+         /* write 4 plane words */
        move.w    d2,(a1)+
        move.w    d2,(a1)+
        move.w    d2,(a1)+
        dbf       d1,logocol2
#if TOSVERSION >= 0x300
        adda.w    #(640/8-12)*4,a1
#else
        adda.w    #(320/8-12)*4,a1
#endif
        dbf       d0,logocol1
        bra.s     logodone
logomono:
#if TOSVERSION >= 0x300
        adda.w    #(1280/8)*4,a1   /* start at screen line 4 */
#else
        adda.w    #(640/8)*4,a1   /* start at screen line 4 */
#endif
        move.w    #$0055,d0        /* 86 lines of data */
logomon1:
        moveq.l   #11,d1           /* 12 bytes of data per line */
logomon2:
        move.b    (a0)+,(a1)+
        dbf       d1,logomon2
#if TOSVERSION >= 0x300
        adda.w    #(1280/8)-12,a1
#else
        adda.w    #(640/8)-12,a1
#endif
        dbf       d0,logomon1
logodone:
#endif

        data

/* 96x86 image of atari logo displayed at boot time */
atarilogo:
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$79ff,$3c00,$0000,$0000
        dc.w $0000,$0000,$f9ff,$3e00,$0000,$0000
        dc.w $0000,$0000,$f9ff,$3e00,$0000,$0000
        dc.w $0000,$0000,$f9ff,$3e00,$0000,$0000
        dc.w $0000,$0000,$f9ff,$3e00,$0000,$0000
        dc.w $0000,$0000,$f9ff,$3e00,$0000,$0000
        dc.w $0000,$0000,$f9ff,$3e00,$0000,$0000
        dc.w $0000,$0001,$f9ff,$3f00,$0000,$0000
        dc.w $0000,$0001,$f9ff,$3f00,$0000,$0000
        dc.w $0000,$0001,$f9ff,$3f00,$0000,$0000
        dc.w $0000,$0001,$f9ff,$3f00,$0000,$0000
        dc.w $0000,$0003,$f9ff,$3f80,$0000,$0000
        dc.w $0000,$0003,$f9ff,$3f80,$0000,$0000
        dc.w $0000,$0003,$f9ff,$3f80,$0000,$0000
        dc.w $0000,$0007,$f1ff,$1fc0,$0000,$0000
        dc.w $0000,$0007,$f1ff,$1fc0,$0000,$0000
        dc.w $0000,$000f,$f1ff,$1fe0,$0000,$0000
        dc.w $0000,$000f,$f1ff,$1fe0,$0000,$0000
        dc.w $0000,$001f,$e1ff,$0ff0,$0000,$0000
        dc.w $0000,$003f,$e1ff,$0ff8,$0000,$0000
        dc.w $0000,$003f,$e1ff,$0ff8,$0000,$0000
        dc.w $0000,$007f,$c1ff,$07fc,$0000,$0000
        dc.w $0000,$00ff,$c1ff,$07fe,$0000,$0000
        dc.w $0000,$01ff,$81ff,$03ff,$0000,$0000
        dc.w $0000,$03ff,$81ff,$03ff,$8000,$0000
        dc.w $0000,$07ff,$01ff,$01ff,$c000,$0000
        dc.w $0000,$0ffe,$01ff,$00ff,$e000,$0000
        dc.w $0000,$1ffe,$01ff,$00ff,$f000,$0000
        dc.w $0000,$7ffc,$01ff,$007f,$fc00,$0000
        dc.w $0000,$fff8,$01ff,$003f,$fe00,$0000
        dc.w $0003,$fff0,$01ff,$001f,$ff80,$0000
        dc.w $001f,$ffe0,$01ff,$000f,$fff0,$0000
        dc.w $00ff,$ffc0,$01ff,$0007,$fffe,$0000
        dc.w $00ff,$ff80,$01ff,$0003,$fffe,$0000
        dc.w $00ff,$ff00,$01ff,$0001,$fffe,$0000
        dc.w $00ff,$fc00,$01ff,$0000,$7ffe,$0000
        dc.w $00ff,$f800,$01ff,$0000,$3ffe,$0000
        dc.w $00ff,$e000,$01ff,$0000,$0ffe,$0000
        dc.w $00ff,$8000,$01ff,$0000,$03fe,$0000
        dc.w $00fc,$0000,$01ff,$0000,$007e,$0000
        dc.w $00e0,$0000,$01ff,$0000,$000e,$0000
        dc.w $0000,$0000,$0000,$0000,$0000,$0000
        dc.w $0000,$0000,$0000,$0000,$0000,$0000
        dc.w $0000,$0000,$0000,$0000,$0000,$0000
        dc.w $0000,$0000,$0000,$0000,$0000,$0000
        dc.w $0000,$0000,$0000,$0000,$0000,$0000
        dc.w $0000,$c07f,$fe03,$0007,$c01e,$0700
        dc.w $0001,$e07f,$fe07,$801f,$f81e,$18c0
        dc.w $0003,$e07f,$fe0f,$803f,$fc1e,$1740
        dc.w $0003,$f07f,$fe0f,$c03f,$fe1e,$2520
        dc.w $0003,$f07f,$fe0f,$c03f,$fe1e,$2620
        dc.w $0007,$f803,$c01f,$e03c,$1f1e,$2520
        dc.w $0007,$f803,$c01f,$e03c,$0f1e,$1540
        dc.w $0007,$f803,$c01f,$e03c,$0f1e,$18c0
        dc.w $000f,$7c03,$c03d,$f03c,$0f1e,$0700
        dc.w $000f,$3c03,$c03c,$f03c,$0f1e,$0000
        dc.w $000f,$3c03,$c03c,$f03c,$1e1e,$0000
        dc.w $001e,$3e03,$c078,$f83c,$7e1e,$0000
        dc.w $001e,$1e03,$c078,$783d,$fc1e,$0000
        dc.w $001e,$1e03,$c078,$783d,$f81e,$0000
        dc.w $003e,$1f03,$c0f8,$7c3d,$e01e,$0000
        dc.w $003f,$ff03,$c0ff,$fc3d,$e01e,$0000
        dc.w $003f,$ff03,$c0ff,$fc3d,$e01e,$0000
        dc.w $007f,$ff83,$c1ff,$fe3c,$f01e,$0000
        dc.w $007f,$ff83,$c1ff,$fe3c,$f81e,$0000
        dc.w $0078,$0783,$c1e0,$1e3c,$781e,$0000
        dc.w $00f8,$07c3,$c3e0,$1f3c,$3c1e,$0000
        dc.w $00f0,$07c3,$c3c0,$1f3c,$3e1e,$0000
        dc.w $00f0,$03c3,$c3c0,$0f3c,$1e1e,$0000
        dc.w $01f0,$03e3,$c7c0,$0fbc,$1f1e,$0000
        dc.w $01e0,$01e3,$c780,$07b8,$0f1e,$0000

		text

/*
 * position VT52 cursor below logo
 */
        moveq.l   #32+7,d7
        tst.b     (_sshiftmod).w
        bne.s     notlow
        moveq.l   #32+12,d7
notlow:
        move.l    #$00030002,d6
        move.w    #$001B,-(a7)
        move.l    d6,-(a7)
        trap      #13             /* Bconout(2, ESC) */
        move.w    #$0059,4(a7)
        move.l    d6,(a7)
        trap      #13             /* Bconout(2, 'Y') */
        move.w    d7,4(a7)
        move.l    d6,(a7)
        trap      #13             /* Bconout(2, row) */
        move.w    #$0020,4(a7)
        move.l    d6,(a7)
        trap      #13             /* Bconout(2, col) */
        addq.w    #6,a7

#if TOSVERSION < 0x300 
        cmpi.l    #80*200,_hz_200        /* system running for >80s? */
        bcc       ptch_term             /* (then no ROM CRC check) */
#endif

/*
 * check the ROM crc
 */
#if STBOOK
banksize equ $3fffe
numbanks equ 1
#else
#if TOSVERSION < 0x300
banksize equ $1fffe
numbanks equ 2
#else
banksize equ $1fffe
numbanks equ 4
#endif
#endif

        move.l    #banksize,d7
        move.w    #numbanks-1,d6
        movea.l   #_os_entry,a5
crccheck:
        move.w    #numbanks,-(a7)    /* checksum over every nth byte */
        move.l    d7,-(a7)           /* number of bytes */
        move.l    a5,-(a7)           /* buffer address */
        bsr       check_rom_crc
        adda.w    #10,a7
        movea.l   a5,a0
#if STBOOK
        adda.l    d7,a0
#else
        adda.l    #banksize*numbanks,a0
#endif
        move.b    (a0),d1            /* high byte of CRC */
        lsl.w     #8,d1
        move.b    numbanks(a0),d1    /* low byte of CRC */
        cmp.w     d1,d0
        bne.s     crcfail
        addq.l    #1,a5
        dbf       d6,crccheck
        bra.s     crcok
crcmsg: dc.b 'WARNING: BAD ROM CRC IN CHIP ',0
crcmsg2 dc.b '.',13,10,0
crcfail:
        move.l    a5,d5
        pea.l     crcmsg
        move.w    #$0009,-(a7)
        trap      #1
        move.b    #$45,d0            /* 'E' - even */
        btst      #0,d5
        beq.s     chipE
        move.b    #$4F,d0            /* 'O' - odd */
chipE:
        move.w    d0,2(a7)
        move.w    #$0002,(a7)
        trap      #1
#if TOSVERSION >= 0x300
        move.b    #$45,d0            /* 'E' - even */
        btst      #1,d5
        beq.s     chipE2
        move.b    #$4F,d0            /* 'O' - odd */
chipE2:
        move.w    d0,2(a7)
        move.w    #$0002,(a7)
        trap      #1
#endif
        move.l    #crcmsg2,2(a7)
        move.w    #$0009,(a7)
        trap      #1
        addq.w    #6,a7
        addq.l    #1,a5
        dbf       d6,crccheck
crcok:


/*
 * if no monochrome is active, holding down the alternate key forces
 * to 320x200x4 instead of 640x480x4 for ST compatibility
 * ??? a bit too late, after displaying error messages
 */
#if TOSVERSION >= 0x300
        cmpi.b    #$06,(_sshiftmod).w         /* TT high? */
        beq.s     nosetrez
        move.l    #$000BFFFF,-(a7)
        trap      #13                         /* Kbshift(-1) */
        addq.w    #4,a7
        btst      #3,d0                       /* alternate pressed? */
        beq.s     nosetrez                    /* no */
        clr.w     -(a7)
        pea.l     ($FFFFFFFF).w
        pea.l     ($FFFFFFFF).w
        move.w    #$0005,-(a7)
        trap      #14                         /* Setscreen(-1L, -1L, 0) - switch to low rez */
        adda.w    #$000C,a7
        move.l    #$00000808,d0 /* BUG? this will be skipped too if in TT high rez */
        D0_CACR
nosetrez:
#endif

/* During boot till this point any exception triggers a coldboot, which */
/* erases the first MB and resets. From now own we point the exceptions */
/* to _term, which draws the bombs and terminates the currently running app */
ptch_term:

#if STBOOK
			move.l    #$1000000,d1             /* d1 -> exception number */
            lea       any_vec(pc),a0           /* new exception vector */
            adda.l    d1,a0                    /* add the exception number into the upper 8 bits (2 = bus error) */
            adda.l    d1,a0
            lea       (_buserror).w,a1         /* start with the bus error exception */
            move.w    #$3f,d0
            move.l    #coldboot,d2             /* old exception vector */
ptcht1:     cmp.l     (a1)+,d2                 /* is it pointing to coldboot? */
            bne.s     ptcht2                   /* no -> ignore It */
            move.l    a0,$fffc(a1)             /* point it to _term */
ptcht2:     adda.l    d1,a0                    /* increment the exception number in the upper 8 bits */
            dbra      d0,ptcht1                /* next vector -> */
#endif

/* boot eventually from a block device (floppy or harddisk) */
        bsr       diskboot                     
/* memory test and attempt to boot from SCSI/ACSI */
        bsr       dmaboot                      
/* run resident programs */
        bsr       run_reset_resident
        tst.w     _cmdload                     /* load shell from disk? */
        beq.s     nocmd
        bsr       autoexec
        move.l    #_os_entry,_sysbase          /* -> base of OS */
        pea.l     emptystr(pc)                 /* null environment string */
        pea.l     emptystr(pc)                 /* null argument string */
        pea.l     command_prg(pc)              /* push shell filename */
        clr.w     -(a7)                        /* Load&Go */
        bra.s     dopexec
nocmd:
/* --- bring up GEM: */
        bsr       autoexec
#if STBOOK
            bsr       autoerom
#endif
        move.l    #_os_entry,_sysbase
/* --- kludge up an enviroment string */
        lea.l     default_env(pc),a0
        movea.l   #gem_env,a1
copyenv:
        cmpi.b    #$23,(a0)                    /* look for drive# character */
        bne.s     nextenvc
        movea.l   a1,a2                        /* remember dest addr of drive character */
nextenvc:
        move.b    (a0)+,(a1)+
        bpl.s     copyenv
#if OS_COUNTRY == CTRY_PL
        move.w    _bootdev,d0
#else
        move.b    _bootdev,d0                  /* compute drive#, and shove it (infamous old bug accessing _bootdev as byte) */
#endif
        add.b     #$41,d0
        move.b    d0,(a2)                      
        pea.l     gem_env                      /* push address of environment string */
        pea.l     emptystr                     /* no arguments */
        pea.l     emptystr(pc)                 /* null shell name (in ROM, after all) */
        move.w    #$0005,-(a7)
        move.w    #$004B,-(a7)
        trap      #1                           /* Pexec(5) Create basepage */
        adda.w    #$000E,a7
        movea.l   d0,a0                        /* get pointer to PSP */
        move.l    _exec_os,8(a0)               /* set p_tbase of created PD */
        pea.l     gem_env
        move.l    a0,-(a7)
        pea.l     emptystr(pc)
        move.w    #$0004,-(a7)
dopexec:
        move.w    #$004B,-(a7)
        trap      #1                           /* Pexec(4) Just go: execute GEM */
        adda.w    #14,a7
/*
 * When startup fails (or if the exec returns,
 * which "cannot happen") fake a system reset:
 */
        jmp       _main

/*
 * Default enviroment string
 * Cannot be more than 20 chars long without modifying
 * the declaration for gem_env,
 * Any char >= $80 terminates the string (and is included in it)
 * The last '#' character is replaced by the boot drive's name (A, B, ...)
 */
default_env: dc.b 'PATH=',0,'#:',$5c,0,0,$ff
command_prg: dc.b 'COMMAND.PRG',0
gem_prg:     dc.b 'GEM.PRG'

emptystr: dc.b 0,0,0

ikbdres:
        dc.b $80,$01

/* Run all boot applications from the application cartridge.
 * Beware: Hatari features a special cartridge which is used
 * for GEMDOS drive emulation. It will hack drvbits and hook Pexec().
 * It will also hack Line A variables to enable extended VDI video modes.
 */
/* 306de: 00e0098a */
diskboot:
        moveq.l   #3,d0
        bsr       _run_cartridge_applications
#if STBOOK
        bsr       scanextrom
#endif
        movea.l   (_hdv_boot).w,a0          /* go through boot vector */
        jsr       (a0)
        tst.w     d0                        /* any errors? */
        bne.s     nodiskboot                /* (yes -- punt) */
        movea.l   (_dskbufp).w,a0
        jsr       (a0)                      /* execute boot sector (it might return) */
nodiskboot:
        rts


/* 306de: 00e009a2 */
dmaboot:
        move.l    #80*200,d7
        cmp.l     _hz_200,d7                /* is the system running for > 80s? */
        bcs       dmadone                   /* (no memory test) */
        movea.w   #$0000,a5                 /* done status = false */
dmaloop:
        cmpa.w    #$0000,a5                 /* memtest done? */
        bne       skipsp                    /* (yes) */
        bsr       memtest                   
        movea.w   d0,a5                     /* memtest done? */
        beq       dmawait                   /* (no) */
        move.w    #$001B,-(a7)
        move.l    #$00030002,-(a7)
        trap      #13                       /* Bconout(2, ESC) */
        move.w    #$0070,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, 'p') Reverse video */
        move.w    #$001B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, ESC) */
        move.w    #$0077,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, 'w') Discard end of line */
        move.l    #200,d5                   /* 200 = 1s */
        move.w    #$004F,d4
        tst.b     _sshiftmod
        bne.s     nolowres
        move.l    #400,d5                   /* 2s */
        moveq.l   #39,d4
nolowres:
        move.l    d5,d6
        /* d4+1 spaces */
        move.l    d4,d3
prspace:
        move.w    #$0020,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, ' ') */
        dbf       d4,prspace
        move.w    #$000D,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, '\r') */
        /* d4 spaces */
        subq.l    #1,d3
prspace2:
        move.w    #$0020,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, ' ') */
        dbf       d3,prspace2
        move.w    #$001B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, ESC) */
        move.w    #$0071,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, 'q') Normal video */
        addq.l    #6,a7
skipsp:
        cmp.l     _hz_200,d6
        bhi.s     dmawait
        move.w    #$001B,-(a7)
        move.l    #$00030002,-(a7)
        trap      #13                      /* Bconout(2, ESC) */
        move.w    #$004B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                      /* Bconout(2, 'K') Clear to eol */
        move.w    #$0008,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                      /* Bconout(2, '\b') */
        addq.w    #6,a7
        add.l     d5,d6
dmawait:
        cmpa.w    #$0000,a5                /* memtest done? */
        beq.s     dmakey                   /* (no) */
        cmp.l     _hz_200,d7
        bls.s     dmaline
dmakey:

#if STBOOK
	    move.w    tt_mcu+4,d0
        not.w     d0
        and.w     #$c,d0                    /* check bit 2 & 3 */
        bne.s     mtest7
#endif
        move.l    #$00010002,-(a7)
        trap      #13                       /* Bconstat(2) */
        addq.l    #4,a7
        tst.l     d0
        beq       dmaloop
        move.l    #$00020002,-(a7)
        trap      #13                       /* Bconin(2) */
        addq.l    #4,a7
mtest7: cmpa.w    #$0000,a5                 /* memtest done? */
        bne.s     dmaline                   /* (yes) */
        bsr       memtestabort
        move.l    d7,_hz_200
        bra.s     dmadone
dmaline:
        move.l    d7,_hz_200
        move.w    #$000D,-(a7)
        move.l    #$00030002,-(a7)
        trap      #13                       /* Bconout(2, '\r') */
        move.w    #$001B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, ESC) */
        move.w    #$004B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13                       /* Bconout(2, 'K') Clear to eol */
        addq.l    #6,a7

/* --- boot from DMA device */
dmadone:
#if STBOOK
		clr.w     gem_env               /* index to the dmaDevList */
#else
#if TOSVERSION < 0x300
	    moveq	  #$10,d4
#else
        jsr       scsirst
        moveq.l   #8,d4      /* d4 = device (SCSI0) */
        move.b    (scu_gp1).w,d0
        and.w     #$00F8,d0
        move.w    d0,(_bootpref).w
        bne.s     confdone
        pea.l     (_bootpref).w
        move.w    #$0002,-(a7)
        clr.l     -(a7)
        jsr       _nvmaccess /* read first 2 bytes of nvram (bootpref) */
        adda.w    #$000A,a7
        tst.w     d0
        beq.s     confdone
#endif
#endif
        clr.w     (_bootpref).w
confdone:
        move.w    #$0001,d1            /* d1 -> 2 tries per device */
dmadev:
        move.w    d1,-(a7)
#if STBOOK
        move.w    gem_env,d4
        move.b    dmaDevList(pc,d4.w),d4
#endif
        move.w    d4,-(a7)             /* pdev */
        move.l    (_dskbufp).w,-(a7)   /* buf = _dskbufp */
        move.w    #$0001,-(a7)         /* count = 1 */
        clr.l     -(a7)                /* sectnum = 0 */
        jsr       _dmaread             /* read first sector of this device */
        adda.w    #$000C,a7
        move.w    (a7)+,d1
        tst.l     d0                   /* read successful? */
        beq.s     dmaok                /* yes -> */
        addq.l    #1,d0                /* error == time out? */
        dbeq      d1,dmadev            /* timeout or another try left? -> */
        bra.s     nextdev              /* try next device */
dmaok:
        movea.l   (_dskbufp).w,a0
        move.w    #$00FF,d0            /* 256 word checksum over the boot sector */
        moveq.l   #0,d1
dmacrc:
        add.w     (a0)+,d1
        dbf       d0,dmacrc
        cmp.w     #$1234,d1            /* checksum == 0x1234? */
        beq.s     doboot               /* execute this valid boot sector -> */
nextdev:
#if STBOOK
	    move.w    gem_env,d4
        addq.w    #1,d4                     /* increment next device */
        move.w    d4,gem_env
        cmpi.b    #$ff,dmaDevList(pc,d4.w)  /* end of the device list? */
        bne.s     dmaBoot2                  /* no -> continue with the next one */
#else
        addq.w    #1,d4
        move.w    d4,d0
        and.w     #$0007,d0
        bne.s     confdone
        cmp.w     #$0008,d4
        beq.s     nodmaboot
        moveq.l   #0,d4       /* retry all again with ACSI devices */
        bra.s     confdone
#endif
nodmaboot:
        rts

#if STBOOK
/* boot order of DMA devices, $ff terminates the list */
dmaDevList: DC.B      $10,$11,$00,$01,$02,$03,$04,$05
            DC.B      $06,$07,$ff
#endif

doboot:
        movea.l   (_dskbufp).w,a0
        move.l    #$444D4172,d3          /* 'DMAr' */
        move.w    d4,d7                  /* d4 -> pdev */
        asl.w     #5,d7                  
        move.w    (_bootpref).w,d5
#if !STBOOK
        move.l    d4,-(a7)
#endif
        move.l    _hdv_rw,-(a7)          /* save read sector function pointer */
        jsr       (a0)                   /* execute boot sector */
        move.l    (a7)+,d0
#if !STBOOK
        move.l    (a7)+,d4
#endif
        cmp.l     (_hdv_rw).w,d0         /* did the read sector function change? */
        beq.s     nextdev                /* no -> no device driver was loaded -> continue to load boot sectors */
        rts

/*
 * void run_cartridge_applications(WORD typebit);
 *
 * Run all cartridge applications of the specified type.
 *
 * typebit(d0): application type bit number which must be set to 1 to match
 */
#if STBOOK
scanextrom:lea (STBOOK_EXTROM).l,a0
        bra.s     cartext2
#endif
/* 306de: 00e00bbc */
_run_cartridge_applications:
        lea     cart_base,a0
cartext2:
        cmp.l   #$ABCDEF42,(a0)+        /* is cartridge present? */
        bne.s   cartover                /* no -> cartover */
testtype:
        btst    d0,4(a0)                /* What type? */
        beq.s   nextapp

        movem.l d0-d7/a0-a6,-(sp)       /* save registers */
        move.l  4(a0),d0                /* d0 = address of cartridge init */
        and.l   #$00ffffff,d0           /* make it 24-bit clean */
        movea.l d0,a0
        jsr     (a0)                    /* execute app in cartridge */
        movem.l (sp)+,d0-d7/a0-a6       /* restore registers */
nextapp:
        tst.l   (a0)                    /* another application? */
        move.l  (a0),a0
        bne.s   testtype
cartover:
        rts

/* 306de: 00e00bee */
bios_unimpl:
		rts

/*
 * memchk - check pattern written to memory
 *       Passed:         d1.l = offset
 *                       a0 = base of pattern ($1f8 bytes long)
 *                       a5 -> return address
 *
 *       Returns:        EQ: the pattern matched
 *                       NE: the pattern didn't match
 *
 *       Uses:           d0.w, a1
 *       Called-by:      Coldstart memory-sizing routine.
 */
/* 306: unused */
memchk0:
        adda.l    d1,a0         /* a0 -> memory to check */
        clr.w     d0            /* zap pattern seed */
        lea.l     504(a0),a1    /* a1 -> ending address */
memchk1:
        cmp.w     (a0)+,d0      /* match? */
        bne.s     memchk2       /* (no -- return NE) */
        add.w     #$FA54,d0     /* yes -- bump pattern */
        cmpa.l    a0,a1         /* matched entire pattern? */
        bne.s     memchk1       /* (no) */
memchk2:
        jmp       (a4)          /* "return" to caller */

/*
 * test memory configuration validation
 *  Passed:      a6 -> return addressd
 *  Returns:     a5 -> 0 (quick zeropage)
 *               EQ: memory setup OK
 *               NE: memory never configured succesfully
 *
 */
/* 306de: 00e00c06 */
memchk:
        cmpi.l    #$752019F3,(_memvalid).w     /* test memory configuration validation, check first magic number */
        bne.s     memchk3                      /* (mismatched -- return NE) */
        cmpi.l    #$237698AA,(_memval2).w      /* check one more (for paranoia) */
        bne.s     memchk3                      /* (mismatched -- return NE) */
        cmpi.l    #$5555AAAA,(_memval3).w      /* check a third time (for more paranoia) */
memchk3:
        jmp       (a6)                         /* return EQ/NE */

/*
 * Default palette assignments.
 *  Sort of corresponding to the GSX spec.
 */
/* 306de: 00e00c24 */
dflt_pal:
		dc.w $0fff     /* 0 white */
		dc.w $0f00     /* 1 red */
		dc.w $00f0     /* 2 green */
		dc.w $0ff0     /* 3 yellow */
		dc.w $000f     /* 4 blue */
		dc.w $0f0f     /* 5 magenta */
		dc.w $00ff     /* 6 cyan */
		dc.w $0555     /* 7 "low white" */
		dc.w $0333     /* 8 grey */
		dc.w $0f33     /* 9 light red */
		dc.w $03f3     /* 10 light green */
		dc.w $0ff3     /* 11 light yellow */
		dc.w $033f     /* 12 light blue */
		dc.w $0f3f     /* 13 light magenta */
		dc.w $03ff     /* 14 light cyan */
		dc.w $0000     /* 15 black */

/*
 * Int 0x68 - HBL interrupt
 */
/* 306de: 00e00c44 */
int_hbl:
        move.w    d0,-(a7)            /* save d0 */
        move.w    2(a7),d0            /* get status register from stack */
        and.w     #$0700,d0           /* isolate just IPL-mask from sr */
        bne.s     is_ipl              /* if IPL is nonzero, then end */
        ori.w     #$0300,2(a7)        /* else set IPL3 in status register */
is_ipl:
        move.w    (a7)+,d0            /* restore d0 */
        rte

/*
 * int_vbl - Int 0x70 - VBL interrupt
 *
 * no video resolution change is done.
 */
/* 306de: 00e00c5a */
int_vbl:
        addq.l    #1,_frclock         /* increase num of happened ints */
        subq.w    #1,_vblsem          /* check vbl semaphore */
        bmi       vbl_end             /* if VBl routine disabled -> end */
        movem.l   d0-d7/a0-a6,-(a7)   /* save registers */
        addq.l    #1,_vbclock         /* count number of VBL interrupts */

/* vblqueue */
        move.w    _nvbls,d7
        beq       vbl_no_queue
        subq.l    #1,d7
        movea.l   (_vblqueue).w,a0
vbl_loop:
        movea.l   (a0)+,a1
        cmpa.w    #$0000,a1
        beq.s     vbl_next
        movem.l   d7/a0,-(a7)
        jsr       (a1)
        movem.l   (a7)+,d7/a0
vbl_next:
        dbf       d7,vbl_loop
        move.b    (MFP_GPIP).w,d1
        tst.b     _has_dmasound
        beq.s     nomoncheck
/* Check for monitor switching, */
/* and jump to _swv_vec if necessary... */
        move.w    sr,-(a7)
        ori.w     #$0700,sr
moncheck:
        move.b    ($FFFF8901).w,d0
        move.b    (MFP_GPIP).w,d1
        btst      #7,d1
        sne       d1
        move.b    (MFP_GPIP).w,d2
        btst      #7,d2
        sne       d2
        cmp.b     d1,d2
        bne.s     moncheck
        cmp.b     ($FFFF8901).w,d0
        bne.s     moncheck
        move.w    (a7)+,sr
        btst      #0,d0
        beq.s     nomoncheck
        not.b     d1
nomoncheck:
        move.b    (shift_tt).w,d0
        and.b     #$07,d0
        cmp.b     #$06,d0
        beq.s     vbl_wastthigh
        btst      #7,d1
        bne.s     vbl_nomonchg
        bsr       dovsync
        move.b    #$06,d0
        bra.s     vbl_monchg
vbl_wastthigh:
        btst      #7,d1
        beq.s     vbl_nomonchg
        move.b    (_defshiftmod).w,d0
        cmp.b     #$06,d0
        bne.s     vbl_monchg
        clr.b     d0
vbl_monchg:
        move.b    d0,(_sshiftmod).w
        move.b    (shift_tt).w,d1
        and.b     #$F8,d1
        or.b      d0,d1
        move.b    d1,(shift_tt).w
        movea.l   (_swv_vec).w,a0
        jsr       (a0)
vbl_nomonchg:
        jsr       blink
/* change palette registers */
        tst.l     (_colorptr).w
        beq.s     vbl_no_palette
        movea.l   (_colorptr).w,a0
        lea.l     ($FFFF8240).w,a1
        move.w    #$0007,d1
vbl_palette_loop:
        move.l    (a0)+,(a1)+
        dbf       d1,vbl_palette_loop
        clr.l     (_colorptr).w
vbl_no_palette:
/* set new video address */
        tst.l     (_screenpt).w
        beq.s     vbl_no_screenpt
        move.l    (_screenpt).w,(_v_bas_ad).w
        move.b    (_v_bas_ad+2).w,(v_bas_m).w
        move.b    (_v_bas_ad+1).w,(v_bas_h).w
        move.b    (_v_bas_ad+3).w,(v_bas_l).w         /* BUG: no test that 820d actually exists */
vbl_no_screenpt:
        bsr       _flopvbl
vbl_no_queue:
/* screen hardcopy */
        tst.w     (_dumpflg).w
        bne.s     vbl_no_dump
        bsr       _scrdmp
vbl_no_dump:
        movem.l   (a7)+,d0-d7/a0-a6
vbl_end:
        addq.w    #1,_vblsem

/* 306de: 00e00d78 */
just_rte:
        rte

/* 306de: 00e00d7a */
_vsync:
        move.w    sr,-(a7)
        andi.w    #$FBFF,sr
        move.l    _frclock,d0
vsyncwait:
        cmp.l     _frclock,d0
        beq.s     vsyncwait
        move.w    (a7)+,sr
        rts

/* 306de: 00e00d92 */
		xdef _callcrit
_callcrit:
        move.l    _etv_critic,-(a7)
critret:
        moveq.l   #-1,d0
        rts

/* ==== Trap 14 - XBIOS entry point ========================================= */

/* 306de: 00e00d9c */
_xbiostrap:
        lea.l     xbios_vecs(pc),a0
        bra.s     biosxbios

/* ==== Trap 13 - BIOS entry point ========================================== */

/* 306de: 00e00da2 */
_biostrap:
        lea.l     bios_vecs(pc),a0

/* ==== Trap 13+14 handler ================================================== */
biosxbios:
        movea.l   _savptr,a1
        move.w    (a7)+,d0               /* Status register -> d0 */
        move.w    d0,-(a1)               /* and save in save_area */
        move.l    (a7)+,-(a1)            /* save return address */
        tst.w     (a7)+                  /* skip format word; BUG: no test for _longframe */
        movem.l   d3-d7/a3-a7,-(a1)      /* regs, including stack pointer */
        move.l    a1,_savptr
        btst      #13,d0                 /* were we in user mode? */
        bne.s     bx_sp_ok               /* yes, the sp already points to the arguments */
        move.l    usp,a7                 /* no, the arguments were on the user stack */
bx_sp_ok:
        move.w    (a7)+,d0               /* remove the function number from stack */
        cmp.w     (a0)+,d0               /* Higher than highest number? */
        bge.s     bx_ret                 /* if not implemented, returns the func number */
        move.w    d0,d1                  
        lsl.w     #2,d1                  /* function table is 1 LW per number so multiply function number by 4 */
        move.l    0(a0,d1.w),d1          /* get function address */
        bclr      #0,d1                  /* indirect call? */
        movea.l   d1,a0
        beq.s     bx_notindirect         
        movea.l   (a0),a0                /* call indirect through system variable */
bx_notindirect:
        suba.l    a5,a5
        jsr       (a0)
bx_ret:
        movea.l   _savptr,a1
        movem.l   (a1)+,d3-d7/a3-a7      /* Get regs back, including sp */
        clr.w     -(a7)                  /* push NULL format word; BUG: no test for _longframe */
        move.l    (a1)+,-(a7)            /* return adress */
        move.w    (a1)+,-(a7)            /* sr */
        move.l    a1,_savptr
        rte                              /* return with return value in D0 */

bios_vecs:
        dc.w 12
        dc.l _getmpb
        dc.l _bbconstat
        dc.l _bbconin
        dc.l _bbconout
        dc.l _hdv_rw+1 /* Rwabs */
        dc.l _setexc
        dc.l _tickcal
        dc.l _hdv_bpb+1 /* Getbpb */
        dc.l _bbcostat
        dc.l _hdv_mediach+1 /* Mediach */
        dc.l _drvmap
        dc.l _kbshift

xbios_vecs:
        dc.w (xbvecsend-xbios_vecs)/4
        dc.l _initmouse
        dc.l bios_unimpl /* Ssbrk */
        dc.l _physbase
        dc.l _logbase
        dc.l _getrez
        dc.l _vsetscreen
        dc.l _setpalette
        dc.l _setcolor
        dc.l _floprd
        dc.l _flopwrt
        dc.l _flopfmt
        dc.l _dbmsg
        dc.l _midiws
        dc.l _mfpint
        dc.l _iorec
        dc.l _rsconf
        dc.l _keytbl
        dc.l _random
        dc.l _protobt
        dc.l _flopver
        dc.l _scrdmp
        dc.l _cursconf
        dc.l _settime
        dc.l _gettime
        dc.l _bioskeys
        dc.l _ikbdws
        dc.l _jdisint
        dc.l _jenabint
        dc.l _giaccess
        dc.l _offgibit
        dc.l _ongibit
        dc.l _xbtimer
        dc.l _dosound
        dc.l _setprt
        dc.l _kbdvbase
        dc.l _kbrate
        dc.l _prtblk
        dc.l _vsync
        dc.l _supexec
        dc.l _puntaes
        dc.l bios_unimpl
        dc.l _floprate
        dc.l _dmaread
        dc.l _dmawrite
        dc.l _bconmap
        dc.l bios_unimpl
        dc.l _nvmaccess
        dc.l bios_unimpl /* Waketime */
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l _blitmode
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l _esetshift
        dc.l _egetshift
        dc.l _esetbank
        dc.l _esetcolor
        dc.l _esetpalette
        dc.l _egetpalette
        dc.l _esetgray
        dc.l _esetsmear
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
        dc.l bios_unimpl
xbvecsend:

/* unknown: 00e00fae; seems to be unused */
        btst      #0,1(a7)
        bne.s     bx_notodd
        move.l    #rw_ret,(a7)
bx_notodd:
        movea.l   (_hdv_rw).w,a0
        jmp       (a0)
rw_ret:
        move.l    d0,-(a7)
#if TOSVERSION >= 0x300
        bsr       flushCaches
#endif
        move.l    (a7)+,d0
        jmp       bx_ret


/*
 * XBIOS #26 - Supexec - Start routine in supervisor-mode.
 *
 * 'codeptr' points to a piece of code, ending in an RTS, that is
 * executed in supervisor mode. The executed code cannot perform
 * BIOS or GEMDOS calls. This function is meant to allow programs
 * to hack hardware and protected locations without having to fiddle
 * with GEMDOS get/set supervisor mode call.
 */
/* 306de: 00e00fd0 */
_supexec:
        movea.l   4(a7),a0
        jmp       (a0)


/*
 * BIOS #1 - Bconstat - Status of input device
 *
 * Arguments:
 *   handle - device handle (0:PRT 1:AUX 2:CON)
 *
 *
 * Returns status in D0.L:
 *  -1  device is ready
 *   0  device is not ready
 */
/* 306de: 00e00fd6 */
_bbconstat:
        lea.l     (_bconstat_vec).w,a0
        moveq.l   #0,d1 /* offsetof(MAPTAB, Bconstat) */
        bra.s     doxconxx

/*
 * BIOS #2 - Bconin  - Get character from device
 *
 * Arguments:
 *   handle - device handle (0:PRT 1:AUX 2:CON)
 *
 * This function does not return until a character has been
 * input.  It returns the character value in D0.L, with the
 * high word set to zero.  For CON:, it returns the GSX 2.0
 * compatible scan code in the low byte of the high word, &
 * the ASCII character in the lower byte, or zero in the
 * lower byte if the character is non-ASCII.  For AUX:, it
 * returns the character in the low byte.
 */
/* 306de: 00e00fde */
_bbconin:
        lea.l     (_bconin_vec).w,a0
        moveq.l   #4,d1 /* offsetof(MAPTAB, Bconin) */
        bra.s     doxconxx

/*
 * BIOS #8 - Bcostat - Read status of output device
 *
 * Returns status in D0.L:
 * -1   device is ready
 * 0    device is not ready
 */
/* 306de: 00e00fe6 */
_bbcostat:
        lea.l     (_bcostat_vec).w,a0
        moveq.l   #8,d1 /* offsetof(MAPTAB, Bcostat) */
        bra.s     doxconxx

/*
 * BIOS #3 - Bconout - Print character to output device
 */
/* 306de: 00e00fee */
_bbconout:
        lea.l     (_bconout_vec).w,a0
        moveq.l   #12,d1 /* offsetof(MAPTAB, Bconout) */

doxconxx:
        move.w    4(a7),d0
        cmp.w     #$0005,d0
        bls.s     doxconstd
        subq.l    #6,d0
        cmp.w     (bcmap_root+4).w,d0 /* bcmap_root.maptabsize */
        bcc.s     doxconret
        movea.l   (bcmap_root).w,a0 /* bcmap_root.maptab */
        asl.w     #3,d0
        adda.w    d0,a0
        add.w     d0,d0
        adda.w    d0,a0
        movea.l   0(a0,d1.w),a0
        jmp       (a0)
doxconret:
        moveq.l   #0,d0
        rts
doxconstd:
        lsl.w     #2,d0
        movea.l   0(a0,d0.w),a0
        jmp       (a0)


/* 306de: 00e01024 */
tconstat:
        dc.l      bios_unimpl
        dc.l      _bcon6stat    /* BIOS device 1 (AUX) defaults to RS232 port */
        dc.l      _bcon2stat
        dc.l      _bcon3stat
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      _bcon0in
        dc.l      _bcon6in      /* BIOS device 1 (AUX) defaults to RS232 port */
        dc.l      _bcon2in
        dc.l      _bcon3in
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      _bco0stat
        dc.l      _bco6stat     /* BIOS device 1 (AUX) defaults to RS232 port */
        dc.l      _bco2stat
        dc.l      _bco4stat /* BUG compatibility with early TOS versions: MIDI(3) and KBD(4) are swapped */
        dc.l      _bco3stat
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      _bcon0out
        dc.l      _bcon6out     /* BIOS device 1 (AUX) defaults to RS232 port */
        dc.l      _bcon2out
        dc.l      _bcon3out
        dc.l      _bcon4out
        dc.l      _bcon5out
        dc.l      bios_unimpl
        dc.l      bios_unimpl

/*
 * BIOS #10 - Drvmap - Read drive bitmap
 *
 * Returns a long containing a bit map of logical drives on the system,
 * with bit 0, the least significant bit, corresponding to drive A.
 * Note that if the BIOS supports logical drives A and B on a single
 * physical drive, it should return both bits set if a floppy drive is
 * present.
 */
/* 306de: 00e010a4 */
_drvmap:
        move.l    (_drvbits).w,d0
        rts

/*
 *  BIOS #11 - Kbshift - Shift Key mode get/set.
 *
 *  two descriptions:
 *      o       If 'mode' is non-negative, sets the keyboard shift bits
 *              accordingly and returns the old shift bits.  If 'mode' is
 *              less than zero, returns the IBM0PC compatible state of the
 *              shift keys on the keyboard, as a bit vector in the low byte
 *              of D0
 *      o       The flag parameter is used to control the operation of
 *              this function.  If flag is not -1, it is copied into the
 *              state variable(s) for the shift, control and alt keys,
 *              and the previous key states are returned in D0.L.  If
 *              flag is -1, then only the inquiry is done.
 */
/* 306de: 00e010aa */
_kbshift:
        moveq.l   #0,d0
        move.b    (_shifty).w,d0
        move.w    4(a7),d1
        bmi.s     kbshift1
        move.b    d1,(_shifty).w
kbshift1:
        rts

/*
 * BIOS #0 - Getmpb - Load Memory parameter block
 *
 * Returns values of the initial memory parameter block, which contains the
 * start address and the length of the TPA.
 * Just executed one time, before GEMDOS is loaded.
 *
 * Arguments:
 *   mpb - first memory descriptor, filled from BIOS
 *
 */
/* 306de: 00e010bc */
_getmpb:
        movea.l   4(a7),a0
        lea.l     (_themd).w,a1
        move.l    a1,(a0)            /* mp_mfl = &themd; */
        clr.l     4(a0)              /* mp_mal = NULL; */
        clr.l     8(a0)              /* mp_rover = NULL; */
        clr.l     (a1)               /* themd.m_link = NULL; */
        move.l    (_membot).w,4(a1)  /* themd.m_start = membot; */
        move.l    (_memtop).w,d0
        sub.l     (_membot).w,d0
        move.l    d0,8(a1)           /* themd.m_length = memtop - membot; */
        clr.l     12(a1)             /* themd.m_own = NULL; */
        cmpi.l    #$1357BD13,_ramvalid
        bne.s     getmpb1
        cmpi.l    #FASTRAMBASE,_ramtop
        bls.s     getmpb1
        lea.l     (_altmd).w,a2
        move.l    a2,(a1)            /* altmd.m_link = &themd; */
        clr.l     (a2)               /* altmd.m_link = NULL; */
        move.l    #FASTRAMBASE+1,4(a2)   /* altmd.m_start = (base address of fast mem) | M_ALTFLAG */
        move.l    _ramtop,d0
        sub.l     #FASTRAMBASE,d0
        move.l    d0,8(a2)           /* altmd.m_length = ramtop - base address of fast mem */
        clr.l     12(a2)             /* altmd.m_own = NULL; */
getmpb1:
        rts

/*
 * BIOS #5 - Setexc - set exception vector
 */
/* 306de: 00e01124 */
_setexc:
        move.w    4(a7),d0
        lsl.w     #2,d0
        suba.l    a0,a0
        lea.l     0(a0,d0.w),a0
        move.l    (a0),d0
        move.l    6(a7),d1
        bmi.s     setexc1
        move.l    d1,(a0)
setexc1:
        rts

/*
 * BIOS #6 - Tickcal - Time between two systemtimer calls
 */
/* 306de: 00e0113c */
_tickcal:
        moveq.l   #0,d0
        move.w    (_timer_ms).w,d0
        rts

/*
 * XBIOS #2 - Physbase - Get the screen's physical base address
 *
 * (at the beginning of the next vblank).
 */
/* 306de: 00e01144 */
_physbase:
        moveq.l   #0,d0
        move.b    (v_bas_h).w,d0
        lsl.w     #8,d0
        move.b    (v_bas_m).w,d0
        lsl.l     #8,d0
        tst.b     STEFlag
        bne.s     physbas1
        move.b    (v_bas_l).w,d0
physbas1:
        rts

/*
 * XBIOS #3 - Logbase - Get the screen's logical base, right away.
 *
 * This is the location that GSX uses when drawing to the screen.
 */
/* 306de: 00e01160 */
_logbase:
        move.l    (_v_bas_a).w,d0
        rts

/*
 * XBIOS #4 - Getrez - Get the screen's current resolution
 *
 * Returns 0, 1, 2, 4, 5 or 6.
 */
/* 306de: 00e01166 */
_getrez:
        moveq.l   #0,d0
        move.b    (shift_tt).w,d0
        and.b     #$07,d0
        rts

/*
 * XBIOS #5 - Setscreen/VsetScreen - Set the screen locations
 *
 * Set the logical screen location (logLoc), the physical screen location
 * (physLoc), and the physical screen resolution. Negative parameters are
 * ignored (making it possible, for instance, to set screen resolution without
 * changing anything else). When resolution is changed, the screen is cleared,
 * the cursor is homed, and the VT52 terminal emulator state is reset.
 */
/* 306de: 00e01172 */
_vsetscreen:
        tst.l     4(a7)
        bmi.s     nosetl
        move.l    4(a7),(_v_bas_a).w
nosetl:
        tst.l     8(a7)
        bmi.s     nosetp
        move.b    9(a7),(v_bas_h).w
        move.b    10(a7),(v_bas_m).w
        move.b    11(a7),(v_bas_l).w         /* BUG: no test that 820d actually exists */
nosetp:
        tst.w     12(a7)
        bmi.s     nosetr
        bsr       dovsync
        move.b    13(a7),(_sshiftmod).w
        move.b    (shift_tt).w,d0
        and.b     #$F8,d0
        or.b      13(a7),d0
        move.b    d0,(shift_tt).w
        clr.w     (_vblsem).w
        jsr       esc_init
        move.w    #$0001,(_vblsem).w
nosetr:
        rts

/*
 * XBIOS #6 - Setpalette - Set the contents of the hardware palette register
 *
 * (all 16 color entries) from the 16 words pointed to by 'palettePtr'.
 * 'paletteptr' MUST be on a word boundary. The palette assignment takes
 * place at the beginning of the next vertical blank interrupt.
 */
/* 306de: 00e011c8 */
_setpalette:
        move.l    4(a7),(_colorptr).w     /* next VBL will do this */
        rts

/*
 * XBIOS #7 - Setcolor - Set the palette number
 *
 * Set the palette number 'colorNum' in the hardware palette table to the
 * specified 'color'. If 'color' is negative, the hardware register is not
 * changed.
 *
 * Return the old color.
 */
/* 306de: 00e011d0 */
_setcolor:
        move.w    4(a7),d1
        add.w     d1,d1
        and.w     #$001F,d1
        lea.l     ($FFFF8240).w,a0
        move.w    0(a0,d1.w),d0
        tst.b     STEFlag
        beq.s     setcol1
        and.w     #$0777,d0
        bra.s     setcol2
setcol1:
        and.w     #$0FFF,d0
setcol2:
        tst.w     6(a7)
        bmi.s     setcol3
        move.w    6(a7),0(a0,d1.w)
setcol3:
        rts

/*
 * XBIOS #27 - Puntaes - Throws away the AES, freeing up any memory it used.
 *
 * If the AES is still resident, it will be discarded and the system
 * will reboot. If the AES is not resident (if it was discarded earlier)
 * the function will return.
 */
/* 306de: 00e01202 */
_puntaes:
        movea.l   _os_magic(pc),a0
        cmpi.l    #$87654321,(a0)
        bne.s     puntaes1
        cmpa.l    (_phystop).w,a0
        bge.s     puntaes1
        clr.l     (a0)
        bra       _main
puntaes1:
        rts


/* 306de: 00e0121c */
any_vec:
/* dump the registers, including SSP */
        movem.l   d0-d7/a0-a7,(_proc_dregs).w
        move.l    2(a7),(_proc_enum).w
/* get the exception number from the format word */
        move.w    6(a7),d0
        and.w     #$0FFF,d0
        asr.w     #2,d0
/* dump the exception number */
        move.b    d0,(_proc_enum).w
/* dump the correct value for USP */
        move.l    usp,a0
        move.l    a0,(_proc_usp).w
/* dump 16 words from the stack */
        moveq.l   #15,d0
        lea.l     (_proc_stk).w,a0
        movea.l   a7,a1
any_vec1:
        move.w    (a1)+,(a0)+
        dbf       d0,any_vec1
/* magic value */
        move.l    #$12345678,(_proc_lives).w
        moveq.l   #0,d1
        move.b    (_proc_enum).w,d1
        subq.w    #1,d1
        bsr.s     drawbombs
/* restore a sane stack (should have probably been done before calling subroutine above) */
        move.l    #save_beg+save_siz,(_savptr).w
        move.w    #$FFFF,-(a7)
        move.w    #$004C,-(a7) /* Pterm */
        trap      #1
        bra       _main
drawbombs:
        move.b    (_sshiftmod).w,d7
        and.w     #$0007,d7
        add.w     d7,d7
        moveq.l   #0,d0
        move.b    (v_bas_h).w,d0
        lsl.w     #8,d0
        move.b    (v_bas_m).w,d0
        lsl.l     #8,d0
        tst.b     STEFlag
        bne.s     drawbo1
        move.b    (v_bas_l).w,d0
drawbo1:
        movea.l   d0,a0
        cmp.w     #$0006,d7
        blt.s     drawbo2
        adda.l    #80*960,a0
        bra.s     drawbo3
drawbo2:
        adda.w    #50*320,a0
drawbo3:
        lea.l     bombimage,a1
        move.w    #$000F,d6
drawbo4:
        move.w    d1,d2
        movea.l   a0,a2
drawbo5:
        move.w    dplanetab(pc,d7.w),d5
drawbo6:
        move.w    (a1),(a0)+
        dbf       d5,drawbo6
        dbf       d2,drawbo5
        addq.w    #2,a1
        adda.w    dwidthtab(pc,d7.w),a2
        movea.l   a2,a0
        dbf       d6,drawbo4
        moveq.l   #29,d7
drawbo7:
        bsr       _vsync
        dbf       d7,drawbo7
        rts
dplanetab:        /* number of planes - 1, indexed by resolution */
        dc.w      3,1,0,0,3,0,0,7
dwidthtab:        /* number of bytes per screen line, indexed by resolution */
        dc.w      320*4/8,640*2/8,640/8,0,640*4/8,0,1280/8,320*8/8


/* void cpy512(const void *src, void *dst) */
		xdef _cpy512
_cpy512:
        movea.l   4(a7),a0
        movea.l   8(a7),a1
        move.w    #$003F,d0
cpy512l:
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        move.b    (a0)+,(a1)+
        dbf       d0,cpy512l
        rts


/*
 * call the routine installed in system vector hdv_init.
 * By default, this will be bhdv_init
 */
		xdef _chdv_init
_chdv_init:
        move.l    _hdv_init,-(a7)
        rts

autopath:                      dc.b $5c,'AUTO',$5c
autoname:                      dc.b '*.PRG',0
        dc.l $12345678,$9abcdef0


/*
 * autoexec PRGs from AUTO folder
 */
/* 306de: 00e0133c */
autoexec:
        move.l    #$000Bffff,-(a7)
        trap      #13              /* Kbshift(-1) */
        addq.l    #4,a7     
        btst      #2,d0            /* Control pressed? */
        bne.s     autoexe2        /* yes, skip autoexec */
        lea.l     autopath(pc),a0
        lea.l     autoname(pc),a1
        move.l    (a7)+,savepc
        move.l    a0,(execpath).w
        move.l    a1,(execname).w
        move.l    (_drvbits).w,d0
        move.w    _bootdev,d1
        btst      d1,d0
        beq.s     autoexe1
        lea.l     emptystr(pc),a0
        move.l    a0,-(a7)
        move.l    a0,-(a7)
        move.l    a0,-(a7)
        move.w    #$0005,-(a7)
        move.w    #$004B,-(a7)
        trap      #1
        adda.w    #$0010,a7
        movea.l   d0,a0
        move.l    #autoscan,8(a0) /* set bp->p_tbase */
        move.l    a3,-(a7)
        move.l    d0,-(a7)
        move.l    a3,-(a7)
        move.w    #$0004,-(a7)
        move.w    #$004B,-(a7)
        trap      #1              /* Pexec(Just go), execute the autoscan routine */
        adda.w    #$0010,a7
autoexe1:
        move.l    savepc,-(a7)
autoexe2:
        rts

autoscan:
        clr.l     -(a7)
        move.w    #$0020,-(a7)
        trap      #1              /* Super */
        addq.w    #6,a7
        movea.l   d0,a4
        movea.l   4(a7),a6        /* get ptr to Basepage */
        lea.l     256(a6),a7      /* set stack to end of commandline */
        move.l    #$00000100,-(a7) /* sizeof(PD) */
        move.l    a6,-(a7)
        clr.w     -(a7)
        move.w    #$004A,-(a7)    /* Mshrink to basepage only */
        trap      #1
#if OS_COUNTRY == CTRY_PL
		/* apparently a patch that was applied by the maker of the PL version */
		adda.w    #$000C,a7
        tst.w     d0
        bne.s     autosca4        /* if Mshrink failed, done */
        move.w    #$0007,-(a7)
        move.l    (execpath.w),-(a7)
#else
        addq.w    #6,a7
        tst.w     d0
        bne.s     autosca4        /* if Mshrink failed, done */
        move.w    #$0007,-(a7)
        move.l    execpath,-(a7)
#endif
        move.w    #$004E,-(a7)    /* Fsfirst */
        moveq.l   #8,d7
autosca1:
        pea.l     execdta
        move.w    #$001A,-(a7)
        trap      #1              /* Fsetdta */
        addq.w    #6,a7
        trap      #1
        adda.w    d7,a7
        tst.w     d0              /* Path found? */
        bne.s     autosca4        /* no, done */
/* copy path of autofolder, and append found name to it */
        movea.l   execpath,a0
        movea.l   execname,a2
        lea.l     execfname,a1
autosca2:
        move.b    (a0)+,(a1)+
        cmpa.l    a0,a2
        bne.s     autosca2
        lea.l     execdta+30,a0
autosca3:
        move.b    (a0)+,(a1)+
        bne.s     autosca3
/* execute the program */
        pea.l     emptystr(pc)
        pea.l     emptystr(pc)
        pea.l     execfname
        clr.w     -(a7)
        move.w    #$004B,-(a7)
        trap      #1              /* Pexec(Load&Go) */
        adda.w    #$0010,a7
        moveq.l   #2,d7
        move.w    #$004F,-(a7)    /* Fsnext */
        bra.s     autosca1
autosca4:
        lea.l     _supstkend,a7
        move.l    savepc,-(a7)
        rts



#include "scrdmp.inc"
		text

/* --- what it is: */
bombimage:        dc.w $0600,$2900,$0080,$4840,$11f0,$01f0,$07fc,$0ffe,$0dfe,$1fff,$1fef,$0fee,$0fde,$07fc,$03f8,$00e0


/*
 * waitvbl - wait for the beam inside the vertical blank area
 */
#if TOSVERSION >= 0x300
/* 306de: e01516 (unused) */
waitvbl:
        clr.b     (MFP_TBCR).w      /* stop Timer B */
        clr.b     (MFP_TBDR).w      /* clear data register */
        move.b    #$08,(MFP_TBCR).w /* put timer B into event count mode */
waitev:
        tst.b     (MFP_TBDR).w
        beq.s     waitev
        jmp       (a6)
#else
waitvbl:    lea       (MFP_TBDR).w,a0               /* a0 -> timer B data register */
            lea       (MFP_TBCR).w,a1               /* a1 -> timer B control register */
            bclr      #0,(MFP_IERA).w               /* disable IRQ of timer B */
            moveq     #1,d4                     /* wait for the timer to expire */
            clr.b     (a1)                      /* stop timer B */
            move.b    #$f0,(a0)                 /* event every 240 scan lines */
            move.b    #8,(a1)                   /* timer b: event count mode (HBL) */
waitvbl2:   cmp.b     (a0),d4                   /* wait for HBL 239 scan lines to pass */
            bne.s     waitvbl2
waitvbl3:   move.b    (a0),d4
            move.w    #615,d3                   /* wait till we are inside the vbl area */
waitvbl4:   cmp.b     (a0),d4
            bne.s     waitvbl3
            dbra      d3,waitvbl4
            move.b    #$10,(a1)                 /* timer b: reset */
            jmp       (a6)
#endif


dovsync:
        bra       _vsync

/*
 * run_reset_resident - run "reset-resident" code
 *
 * "Reset-resident" code is code that has been loaded into RAM prior
 * to a warm boot.  It has a special header with a magic number, it
 * is 512 bytes long (aligned on a 512-byte boundary), and it has a
 * specific checksum (calculated on a word basis).
 *
 * Note: this is an undocumented feature of TOS that exists in all
 * versions of Atari TOS.
 */
/* 306de: 00e01530 */
run_reset_resident:
        movea.l   _phystop,a0
run_res1:
        suba.w    #$0200,a0
        cmpa.w    #$0400,a0
        bls.s     run_res3
        cmpi.l    #RR_MAGIC,(a0)
        bne.s     run_res1
        cmpa.l    4(a0),a0
        bne.s     run_res1
        clr.w     d0
        movea.l   a0,a1
        move.w    #$00FF,d1
run_res2:
        add.w     (a1)+,d0
        dbf       d1,run_res2
        cmp.w     #RR_CHKSUM,d0
        bne.s     run_res1
        move.l    a0,-(a7)
        jsr       8(a0)
        movea.l   (a7)+,a0
        bra.s     run_res1
run_res3:
        rts

/*
 * XBIOS #17 - Gettime - Gets intelligent keyboard's time and date
 *
 * Returns that value (in DOS format) as a 32-bit word.
 * (Time in the low word, date in the high word).
 */
/* 306de: 00e0156e */
_gettime:
        lea.l     rdttclock,a3
        lea.l     igetdt,a4
        bra.s     gsettime

/*
 * XBIOS #16 - Settime - Sets the intelligent keyboard's time and date.
 *
 * 'datetime' is a 32-bit DOS-format date and time (time in the low word,
 * date in the high word).
 */
/* 306de: 00e0157c */
_settime:
        move.w    4(a7),_date
        move.w    6(a7),_time
        lea.l     wrttclock,a3
        lea.l     isetdt,a4
gsettime:
        bsr       detect_nvram
        bcc.s     gsettim1
        movea.l   a4,a3
gsettim1:
        jmp       (a3)


/*
 * copy sysbase into memory and patch it
 */
/* 306de: 00e015a2 */
instoshdr:
        lea.l     _os_entry(pc),a0
        lea.l     oshdr+6,a1
        moveq.l   #47,d0
instos1:
        move.b    0(a0,d0.w),0(a1,d0.w)
        dbf       d0,instos1
        move.w    jmpop(pc),-6(a1) /* ? BUG? the jmpop there is short abs jump */
        move.l    4(a1),-4(a1)
        move.w    braop(pc),(a1)
        move.w    30(a1),28(a1) /* ??? oshdr.os_conf = oshdr.os_dosdate */
        move.l    a1,_sysbase
        rts
jmpop:
        jmp       ($00000000).w
braop:
        bra.s     jmpop


/*
 * XBIOS #40 - Blitmode
 */
/* 306de: 00e015dc */
_blitmode:
        bsr.s     blittest
        move.w    d0,d4
        move.w    d0,d5
        lsr.w     #1,d5
        or.w      #$FFFE,d5
        jsr       _GETBLT
        move.w    d0,d3
        move.w    4(a7),d0
        bmi.s     blitmod1
        and.w     d5,d0
        or.w      d4,d0
        jsr       _SETBLT
blitmod1:
        move.w    d3,d0
        rts

/*
 * blittest - test if the blitter is installed
 */
/* 306de: 00e01604 */
blittest:
        move.w    sr,d1
        move.w    #$0000,d0
        suba.l    a0,a0
        movea.l   a7,a2
        ori.w     #$0700,sr
        movea.l   _buserror(a0),a1
        move.l    #noblitter,_buserror(a0)
        tst.w     $ffff8a00(a0)
        moveq.l   #2,d0
noblitter:
        move.l    a1,_buserror(a0)
        move.w    d1,sr
        movea.l   a2,a7
        rts

/* 306de: 00e0162e */
mfpdelay:
        bsr.s     tttimerc
ttdelay1:
        btst      #5,(MFP_IPRB+(TTMFP_REGS-MFP_REGS)).w
        beq.s     ttdelay1
        rts

/*
 * (re-)program TT MFP Timer C
 * Input: D0: initial counter (low byte) + (divisor<<8)
 */
/* 306de: 00e0163a */
tttimerc:
        movem.w   d0-d1,-(a7)
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.b    (MFP_TCDCR+(TTMFP_REGS-MFP_REGS)).w,d1 /* save original Timer C+D mode */
        and.b     #$0F,d1
        move.b    d1,(MFP_TCDCR+(TTMFP_REGS-MFP_REGS)).w  Stop timer C
        bclr      #5,(MFP_IERB+(TTMFP_REGS-MFP_REGS)).w /* Disable Timer C interrupt */
        move.b    #$DF,(MFP_IPRB+(TTMFP_REGS-MFP_REGS)).w /* Clear pending Interrupt flag */
        bclr      #5,(MFP_IMRB+(TTMFP_REGS-MFP_REGS)).w /* Mask Timer C interrupt */
        bset      #5,(MFP_IERB+(TTMFP_REGS-MFP_REGS)).w /* Enable Timer C interrupt again */
        move.b    d0,(MFP_TCDR+(TTMFP_REGS-MFP_REGS)).w /* Set Timer C counter */
        lsr.w     #4,d0
        and.b     #$F0,d0
        or.b      d0,d1
        move.b    d1,(MFP_TCDCR+(TTMFP_REGS-MFP_REGS)).w /* Set Timer C mode */
        move.w    (a7)+,sr
        movem.w   (a7)+,d0-d1
        rts

/*
 *
 *  Quickly zero (lots of) memory.
 *  Copyright 1986 Atari Corp.
 *
 *  Synopsis:	zeromem(start, end)
 *		    LONG start;	    4(sp) -> first location
 *		    LONG end;	    8(sp) -> last location + 1
 *
 *    Uses:	C registers d0-d2/a0-a2
 *
 */
/* 306de: 00e01680 */
		xdef _zeromem
_zeromem:
        movea.l   4(a7),a0	/* a0 -> start */
        movea.l   8(a7),a1	/* a1 -> end+1 */
        movem.l   d3-d7/a3,-(a7)	/* save registers */
        moveq.l   #0,d1		/* store zero into d1-d7/a3 */
        moveq.l   #0,d2
        moveq.l   #0,d3
        moveq.l   #0,d4
        moveq.l   #0,d5
        moveq.l   #0,d6
        moveq.l   #0,d7
        movea.w   d7,a3
        move.l    a0,d0		/* word align first location */
        btst      #0,d0
        beq.s     zerom1
        move.b    d1,(a0)+
zerom1:
        move.l    a1,d0		/* d0 = ((a1 - a0) & ~0xff) */
        sub.l     a0,d0
        and.l     #$FFFFFF00,d0	/* mask fract bits, d0 = whole part */
        beq.s     zerom3		/* if (d0 == 0) do end-fraction; */
        lea.l     0(a0,d0.l),a0	/* a0 -> end of huge area */
        movea.l   a0,a2		/* a2 -> there, too */
        lsr.l     #8,d0		/* get 256-byte chunk count */
zerom2:
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        movem.l   d1-d7/a3,-(a2)	/* clear 32 bytes */
        subq.l    #1,d0			/* decrement count */
        bne.s     zerom2		/* while (d0) clear some more... */
zerom3:
        cmpa.l    a0,a1			/* while (a0 != a1) */
        beq.s     zerom4		/* (done) */
        move.b    d1,(a0)+		/* clear a byte */
        bra.s     zerom3
zerom4:
        movem.l   (a7)+,d3-d7/a3		/* restore registers */
        rts

#include "mmu030.inc"
		text

#include "romcrc.inc"
		text

/*
 * XBIOS #80 - EsetShift
 */
/* 306de: 00e01756 */
_esetshift:
        bsr       dovsync
        moveq.l   #0,d0
        move.w    (shift_tt).w,-(a7)
        move.w    6(a7),(shift_tt).w
        move.w    (shift_tt).w,d0
        and.w     #$0007,d0
        move.b    d0,(_sshiftmod).w
        clr.w     (_vblsem).w
        jsr       esc_init
        move.w    #$0001,(_vblsem).w
        move.w    (a7)+,d0
        rts

/*
 * XBIOS #81 - EgetShift
 */
/* 306de: 00e01786 */
_egetshift:
        moveq.l   #0,d0
        move.w    (shift_tt).w,d0
        rts

/*
 * XBIOS #82 - EsetBank - Read/modify TT shifter color bank number
 */
/* 306de: 00e0178e */
_esetbank:
        moveq.l   #0,d0
        move.w    (shift_tt).w,d0
        and.w     #$000F,d0
        tst.w     4(a7)
        bmi.s     esetb1
        move.b    5(a7),($FFFF8263).w
esetb1:
        rts

/*
 * XBIOS #83 - EsetColor - Read/modify TT palette color entry
 */
/* 306de: 00e017a6 */
_esetcolor:
        moveq.l   #0,d0
        lea.l     ($FFFF8400).w,a0
        move.w    4(a7),d0
        and.w     #$00FF,d0
        add.w     d0,d0
        adda.w    d0,a0
        move.w    (a0),d0
        and.w     #$0FFF,d0
        move.w    6(a7),d1
        bmi.s     esetcol1
        move.w    d1,(a0)
esetcol1:
        rts

/*
 * XBIOS #84 - EsetPalette - Set multiple TT palette color registers
 *
 * This function is defined by Atari to return void; however, if the TT
 * shifter is not present, it should return the function number in a WORD,
 * which is the de facto TOS standard for unimplemented xbios functions.
 * Therefore internally we make it return a WORD.
 */
/* 306de: 00e017c8 */
_esetpalette:
        move.w    4(a7),d0
        and.w     #$00FF,d0
        movea.w   d0,a0
        adda.w    a0,a0
        sub.w     #$0100,d0
        neg.w     d0
        move.w    6(a7),d1
        cmp.w     d0,d1
        ble.s     esetpal1
        move.w    d0,d1
esetpal1:
        movea.l   8(a7),a1
        lea.l     $ffff8400(a0),a0
        bra.s     esetpal3
esetpal2:
        move.w    (a1)+,(a0)+
esetpal3:
        dbf       d1,esetpal2
        rts

/*
 * XBIOS #85 - EgetPalette - Get multiple TT palette color registers
 *
 * See the comments for esetpalette() above
 */
/* 306de: 00e017f6 */
_egetpalette:
        move.w    4(a7),d0
        and.w     #$00FF,d0
        movea.w   d0,a0
        adda.w    a0,a0
        sub.w     #$0100,d0
        neg.w     d0
        move.w    6(a7),d1
        cmp.w     d0,d1
        ble.s     egetpal1
        move.w    d0,d1
egetpal1:
        movea.l   8(a7),a1
        lea.l     -31744(a0),a0
        bra.s     egetpal3
egetpal2:
        move.w    (a0)+,(a1)+
egetpal3:
        dbf       d1,egetpal2
        rts


/*
 * XBIOS #86 - EsetGray - Read/modify TT shifter grey mode bit
 */
/* 306de: 00e01824 */
_esetgray:
        moveq.l   #0,d0
        move.b    (shift_tt).w,d1
        move.b    d1,d0
        lsr.b     #4,d0
        and.b     #$01,d0
        bclr      #4,d1
        tst.w     4(a7)
        beq.s     esetgr1
        bmi.s     esetgr2
        bset      #4,d1
esetgr1:
        move.b    d1,(shift_tt).w
esetgr2:
        rts

/*
 * XBIOS #87 - EsetSmear - Read/modify TT shifter smear mode bit
 */
/* 306de: 00e01848 */
_esetsmear:
        moveq.l   #0,d0
        move.b    (shift_tt).w,d1
        move.b    d1,d0
        add.b     d0,d0
        subx.w    d0,d0
        neg.w     d0
        bclr      #7,d1
        tst.w     4(a7)
        beq.s     esetsm1
        bmi.s     esetsm2
        bset      #7,d1
esetsm1:
        move.b    d1,(shift_tt).w
esetsm2:
        rts


#include "dma.inc"

#include "nvram.inc"

#include "ikbdclock.inc"

/*
 * Bcostat function for device 3 (midi)
 */
/* 306de: 00e02266 */
_bco3stat:
        moveq.l   #-1,d0
        move.b    (ACIA_MIDI_BASE+ACIA_CTRL).w,d2
        btst      #1,d2
        bne.s     bco3sret
        moveq.l   #0,d0
bco3sret:
        rts

/*
 * Bconout function for device 3 (midi)
 */
/* 306de: 00e02276 */
_bcon3out:
        move.w    6(a7),d1
bcon3o1:
        lea.l     (ACIA_MIDI_BASE).w,a1
bcon3o2:
        move.b    (a1),d2
        btst      #1,d2
        beq.s     bcon3o2
        move.b    d1,ACIA_DATA(a1)
        rts


/*
 * XBIOS #12 - Midiws - Write string of characters to the MIDI port. 
 * VOID midiws(uint16_t cnt, const VOIDPTR ptr)
 */
/* 306de: 00e0228c */
		xdef _midiws
_midiws:
        moveq.l   #0,d3
        move.w    4(a7),d3
        movea.l   6(a7),a2
midiws1:
        move.b    (a2)+,d1
        bsr.s     bcon3o1
        dbf       d3,midiws1
        rts

/*
 * Bconstat function for device 3 (midi)
 */
/* 306de: 00e022a0 */
_bcon3stat:
        lea.l     (midiiorec).w,a0
        lea.l     (ACIA_MIDI_BASE).w,a1
        moveq.l   #-1,d0
        lea.l     head(a0),a2
        lea.l     tail(a0),a3
        cmpm.w    (a3)+,(a2)+
        bne.s     bcon3s1
        moveq.l   #0,d0
bcon3s1:
        rts

/*
 * Bconin function for device 3 (midi)
 */
/* 306de: 00e022ba */
_bcon3in:
        bsr.s     _bcon3stat
        tst.w     d0
        beq.s     _bcon3in
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.w    head(a0),d1
        cmp.w     tail(a0),d1
        beq.s     bcon3i2
        addq.w    #1,d1
        cmp.w     size(a0),d1
        bcs.s     bcon3i1
        moveq.l   #0,d1
bcon3i1:
        movea.l   buf(a0),a1
        and.l     #$0000FFFF,d1
        move.b    0(a1,d1.l),d0
        move.w    d1,head(a0)
bcon3i2:
        move.w    (a7)+,sr
        rts

/*
 * Bconout function for device 0 (printer)
 */
/* 306de: 00e022ee */
#if OS_COUNTRY == CTRY_PL
		/* apparently a patch that was applied by the maker of the PL version */
_bcon0out:
        btst      #4,(_prtconfig).w  /* Printer configured for output to RS232? */
        bne       _bcon6out
		lea.l     (_hz_200).w,a0
		move.l    (a0),d3
		move.l    d3,d2
		sub.l     (lst_timeout).w,d3
		cmpi.l    #1000,d3
        bcs.s     bcon0o2
bcon0o1:
        bsr.s     _bco0stat
        bmi.s     bcon0o3
        move.l    (a0),d3
        sub.l     d2,d3
        cmpi.l    #6000,d3
        bcs.s     bcon0o1
bcon0o2:
        dc.l      6000             /* == ori.b #$70,d0; most likely bytes forgotten to patch */
        moveq.l   #0,d0
        rts
bcon0o3:
        bsr.s     gpipisr7
        moveq.l   #7,d1
        moveq.l   #$c0,d0
        bsr.s     ongib
        move.b    #$0F,(a0)
        move.b    7(a7),(a1)
        bsr.s     strobeon
        move.w    (a0),d0
        move.w    (a0),d0
        bsr.s     ongib0
        move.w    d3,sr
        moveq.l   #-1,d0
        rts

strobeoff:
        moveq.l   #14,d1
ongib0:
        moveq.l   #$20,d0
ongib:
        move.b    d1,(a0)
        or.b      (a0),d0
        move.b    d0,(a1)
        rts

strobeon:
        moveq.l   #14,d1
offgib0:
        moveq.l   #$DF,d0
offgib:
        move.b    d1,(a0)
        and.b     (a0),d0
        move.b    d0,(a1)
        rts


/*
 * Bconin function for device 0 (printer)
 */
/* 306pl: 0038235a */
_bcon0in:
        bsr.s     gpipisr7
        moveq.l   #7,d1
        moveq.l   #127,d0
        bsr.s     offgib
        bsr.s     strobeoff
        move.w    d3,sr
bcon0in1:
        bsr.s     _bco0stat
        bmi.s     bcon0in1
        bsr.s     gpipisr7
        bsr.s     offgib0
        move.b    #$0F,(a0)
        move.b    (a0),d0
        move.w    d3,sr
        rts

/*
 * Bcostat function for device 0 (printer)
 */
/* 306pl: 00382378 */
_bco0stat:
        andi.b    #$FE,(MFP_DDR).w
        moveq.l   #0,d0
        btst      d0,(MFP_GPIP).w
        bne.s     bco0st1
        moveq.l   #-1,d0
bco0st1:
        rts

strobeo0:
        bsr.s     gpipisr7
        bsr.s     strobeoff
        move.w    d3,sr
        rts

gpipisr7:
        lea.l     ($FFFF8800).w,a0
        lea.l     2(a0),a1
        move.w    sr,d3
        ori.w     #$0700,sr
        rts
		dc.w -1

#else

/*
 * Bconout function for device 0 (printer)
 */
/* 306de: 00e022ee */
_bcon0out:
        btst      #4,(_prtconfig).w  /* Printer configured for output to RS232? */
        bne       _bcon6out
        move.l    (_hz_200).w,d2
        sub.l     (lst_timeout).w,d2
        cmpi.l    #1000,d2
        bcs.s     bcon0o2
        move.l    (_hz_200).w,d2
bcon0o1:
        bsr       _bco0stat
        tst.w     d0
        bne.s     bcon0o3
        move.l    (_hz_200).w,d3
        sub.l     d2,d3
        cmpi.l    #6000,d3
        blt.s     bcon0o1
bcon0o2:
        moveq.l   #0,d0
        move.l    (_hz_200).w,(lst_timeout).w
        rts
bcon0o3:
        move.w    sr,d3
        ori.w     #$0700,sr
        moveq.l   #7,d1
        bsr       giaccess
        ori.b     #$80,d0
        move.b    #$87,d1
        bsr       giaccess
        move.w    d3,sr
        move.w    6(a7),d0
        move.b    #$8F,d1
        bsr       giaccess
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        bsr.s     strobeon
        bsr.s     strobeon
        bsr.s     strobeoff
        move.w    (a7)+,sr
        moveq.l   #-1,d0
        rts

strobeoff:
        moveq.l   #32,d2
        bra       ongibit

strobeon:
        move.b    #$DF,d2
        bra       offgibit


/*
 * Bconin function for device 0 (printer)
 */
/* 306de: 00e02372 */
_bcon0in:
        moveq.l   #7,d1
        bsr       giaccess
        andi.b    #$7F,d0
        move.b    #$87,d1
        bsr       giaccess
        bsr.s     strobeoff
bcon0in1:
        bsr.s     _bco0stat
        tst.w     d0
        bne.s     bcon0in1
        bsr.s     strobeon
        moveq.l   #15,d1
        bra       giaccess

/*
 * Bcostat function for device 0 (printer)
 */
/* 306de: 00e02394 */
_bco0stat:
        lea.l     (MFP_GPIP).w,a0
        moveq.l   #-1,d0
        btst      #0,(a0)
        beq.s     bco0st1
        moveq.l   #0,d0
bco0st1:
        rts

#endif

/*
 * Bconstat function for device 6 (rs232 driven by MFP)
 */
/* 306de: 00e023a4 */
_bcon6stat:
        lea.l     (rs232iorec).w,a0
bcon6st1:
        moveq.l   #-1,d0
        lea.l     head(a0),a1
        lea.l     tail(a0),a0
        cmpm.w    (a0)+,(a1)+
        bne.s     bcon6st2
        moveq.l   #0,d0
bcon6st2:
        rts

/*
 * Bconin function for device 6 (rs232 driven by MFP)
 */
/* 306de: 00e023ba */
_bcon6in:
        lea.l     (rs232iorec).w,a0
        lea.l     (MFP_REGS).w,a2
bcon6in1:
        bsr       iorecget
        move.w    d0,-(a7)
        tst.b     ctr(a0)
        beq.s     bcon6in3
        move.w    tail(a0),d0
        sub.w     head(a0),d0
        bpl.s     bcon6in2
        add.w     size(a0),d0
bcon6in2:
        cmp.w     low(a0),d0
        bgt.s     bcon6in3
        tst.b     rcts(a0)
        beq.s     bcon6in3
        bsr.s     rs232cont
bcon6in3:
        move.w    (a7)+,d0
        rts

rs232cont:
        clr.b     rcts(a0)
        btst      #0,ctr(a0)
        bne.s     rs232co1
        bra       rtson
rs232co1:
        move.b    #$11,xonoffc(a0)
        bra.s     bcon6o1

/*
 * Bcostat function for device 6 (rs232 driven by MFP)
 */
/* 306de: 00e02406 */
_bco6stat:
        lea.l     (rs232iorec+IOREC_SIZE).w,a0    /* rs232iorec.out */
bco6s1:
        move.w    tail(a0),d1             /* rs232iorec.out.tail */
        bsr       wrapin
        moveq.l   #-1,d0
        cmp.w     head(a0),d1
        bne.s     _bco6sret
        moveq.l   #0,d0
_bco6sret:
        rts


/*
 * Bconout function for device 6 (rs232 driven by MFP)
 */
/* 306de: 00e0241e */
_bcon6out:
        move.w    6(a7),d0
        lea.l     (rs232iorec+IOREC_SIZE).w,a0    /* rs232iorec.out */
        bsr       iorecput
        lea.l     (rs232iorec).w,a0       /* rs232iorec.in */
        lea.l     (MFP_REGS).w,a2
bcon6o1:
        tst.b     MFP_TSR-MFP_REGS(a2)
        bpl.s     bcon6o2
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        bsr       iorecout
        move.w    (a7)+,sr
bcon6o2:
        rts


/*
 * Bcostat function for device 4 (keyboard)
 */
/* 306de: 00e02446 */
_bco4stat:
        moveq.l   #-1,d0
        move.b    (ACIA_IKBD_BASE+ACIA_CTRL).w,d2
        btst      #1,d2
        bne.s     bco4st1
        moveq.l   #0,d0
bco4st1:
        rts

/*
 * Bconout function for device 4 (keyboard)
 */
/* 306de: 00e02456 */
_bcon4out:
        move.w    6(a7),d1
ikbd_writeb:
        lea.l     (ACIA_IKBD_BASE).w,a1
ikbd_wr1:
        move.b    (a1),d2
        btst      #1,d2
        beq.s     ikbd_wr1
        move.w    #$0400,d0
        bsr       mfpdelay
        move.b    d1,ACIA_DATA(a1)
        rts

/*
 * XBIOS #25 - Ikbdws - write string to keyboard
 * VOID ikbdws(uint16_t cnt, VOIDPTR ptr);
 */
/* 306de: 00e02474 */
		xdef _ikbdws
_ikbdws:
        moveq.l   #0,d3
        move.w    4(a7),d3
        movea.l   6(a7),a2
ikbdws1:
        move.b    (a2)+,d1
        bsr.s     ikbd_writeb
        dbf       d3,ikbdws1
        rts

/*
 * Bconstat function for device 2 (console)
 */
/* 306de: 00e02488 */
_bcon2stat:
        lea.l     (ikbdiorec).w,a0
        moveq.l   #-1,d0
        lea.l     head(a0),a2
        lea.l     tail(a0),a3
        cmpm.w    (a3)+,(a2)+
        bne.s     bcon2st1
        moveq.l   #0,d0
bcon2st1:
        rts

/*
 * Bconin function for device 2 (console)
 */
/* 306de: 00e0249e */
_bcon2in:
        bsr.s     _bcon2stat
        tst.w     d0
        beq.s     _bcon2in
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.w    head(a0),d1
        cmp.w     tail(a0),d1
        beq.s     bcon2in2
        addq.w    #4,d1
        cmp.w     size(a0),d1
        bcs.s     bcon2in1
        moveq.l   #0,d1
bcon2in1:
        movea.l   (a0),a1
        and.l     #$0000FFFF,d1
        move.l    0(a1,d1.l),d0
        move.w    d1,head(a0)
bcon2in2:
        move.w    (a7)+,sr
        rts

/*
 * Bcostat function for device 2 (console)
 */
/* 306de: 00e024d2 */
_bco2stat:
        moveq.l   #-1,d0
        rts



/* 306de: 00e024d6 */
initmfp:
        lea.l     (MFP_REGS).w,a0
        moveq.l   #0,d0
        movep.l   d0,0(a0)    /* clear gpip thru iera */
        movep.l   d0,8(a0)    /* clear ierb thru isrb */
        movep.l   d0,16(a0)   /* clear isrb thru vr */
        move.b    #$48,MFP_VR-MFP_REGS(a0) /* set mfp autovector to $100 and s-bit */
        bset      #2,MFP_AER-MFP_REGS(a0)  /* set cts to low to high transition */
        lea.l     (TTMFP_REGS).w,a0
        moveq.l   #0,d0
        movep.l   d0,0(a0)    /* clear gpip thru iera */
        movep.l   d0,8(a0)    /* clear ierb thru isrb */
        movep.l   d0,16(a0)   /* clear isrb thru vr */
        move.b    #$58,MFP_VR-MFP_REGS(a0) /* set mfp autovector to $140 and s-bit */
        clr.b     (_privvio).w /* ??? */
        move.w    #$1111,(_tim_c_sieve).w
        move.w    #20,(_timer_ms).w
        moveq.l   #2,d0
        moveq.l   #80,d1
        move.w    #$00C0,d2
        bsr       settimer    /* initialize Timer C (200hz timer) */
        lea.l     timercint,a2
        moveq.l   #5,d0
        bsr       initint
        moveq.l   #3,d0
        moveq.l   #1,d1
        moveq.l   #2,d2
        bsr       settimer
        move.b    #$01,(rs232iorec+baudrate).w
        lea.l     (MFP_REGS).w,a0
        move.l    #$00880105,d0
        movep.l   d0,MFP_SCR-MFP_REGS(a0)
        lea.l     (TTMFP_REGS).w,a0
        move.l    #$00880105,d0
        movep.l   d0,MFP_SCR-MFP_REGS(a0)
        move.b    #$01,(MFP_TCDCR+(TTMFP_REGS-MFP_REGS)).w
        move.b    #$02,(MFP_TDDR+(TTMFP_REGS-MFP_REGS)).w
        move.b    #$01,(mdm3iorec+baudrate).w
        tst.b     STEFlag
        bne.s     initmfp2
        bsr       initscc
initmfp2:
        bsr       dtron
        bsr       rtson
        lea.l     (rs232iorec).w,a0
        lea.l     rs232table,a1
        moveq.l   #EXTIOREC_SIZE-1,d0
        bsr       copymem
        lea.l     (mdm3iorec).w,a0
        lea.l     mdm3table,a1
        moveq.l   #EXTIOREC_SIZE-1,d0
        bsr       copymem
        lea.l     (midiiorec).w,a0
        lea.l     miditable,a1
        moveq.l   #IOREC_SIZE-1,d0
        bsr       copymem
        move.w    #$FFFF,kb_altnum
        move.l    #aciasys2,d0
        move.l    d0,(vkbderr).w
        move.l    d0,(vmiderr).w
        move.l    #_midivec,(midivec).w
        move.l    #_midisys,(midisys).w
        move.l    #_ikbdsys,(ikbdsys).w
        move.l    #_kbdvec,(kbdvec).w
/* reset midi acia */
        move.b    #$03,(ACIA_MIDI_BASE+ACIA_CTRL).w
/* init the acia to divide by 16x clock, 8 bit data, 1 stop bit, no parity, */
/* rts low, transmitting interrupt disabled, receiving interrupt enabled */
        move.b    #$95,(ACIA_MIDI_BASE+ACIA_CTRL).w
        move.b    #$07,(_conterm).w
        move.l    #_clockvec,(clockvec).w
        move.l    #dummyrts,d0
        move.l    d0,(statvec).w
        move.l    d0,(mousevec).w
        move.l    d0,(joyvec).w
        bsr       initdevstables
        moveq.l   #0,d0
        move.l    d0,(_sndtable).w
        move.b    d0,(_snddelay).w
        move.b    d0,(_sndtmp).w
        move.l    d0,(lst_timeout).w
#if OS_COUNTRY == CTRY_PL
        bsr       strobeo0
#else
        bsr       strobeoff
#endif
        move.b    #$0F,(kb_initial).w
        move.b    #$02,(kb_repeat).w
        lea.l     (ikbdiorec).w,a0
        lea.l     ikbdtable,a1
        moveq.l   #IOREC_SIZE-1,d0
        bsr.s     copymem
        bsr       _bioskeys
/* reset ikbd acia */
        move.b    #$03,(ACIA_IKBD_BASE+ACIA_CTRL).w
/* init the acia do divide by 64 clock, 8 bit data, 1 stop bit, no parity, */
/* rts low, transmitting interrupt disabled, receiving interrupt enabled */
        move.b    #$96,(ACIA_IKBD_BASE+ACIA_CTRL).w
        movea.l   #mfpvectr,a3
        moveq.l   #3,d1
initmfp3:
        move.l    d1,d2
        move.l    d1,d0
        addi.b    #$09,d0
        asl.l     #2,d2
        movea.l   0(a3,d2.w),a2
        bsr       initint
        dbf       d1,initmfp3
        lea.l     _int_acia,a2
        moveq.l   #6,d0
        bsr       initint
        lea.l     ctsint,a2
        moveq.l   #2,d0
        bsr       initint
        movea.l   #ttmfpvectr,a3
        movea.w   #TTMFP9,a0
        move.l    (a3)+,(a0)+
        move.l    (a3)+,(a0)+
        move.l    (a3)+,(a0)+
        move.l    (a3)+,(a0)+
        ori.b     #$1E,(MFP_IERA+(TTMFP_REGS-MFP_REGS)).w
        ori.b     #$1E,(MFP_IMRA+(TTMFP_REGS-MFP_REGS)).w
dummyrts:
        rts

		data
ttmfpvectr:
        dc.l tttxerror
        dc.l tttxrint
        dc.l ttrxerror
        dc.l ttrcvrint

/*
 * initial EXT_IOREC for device 8 (modem3)
 */
/* 306de: 00e36744 */
mdm3table:
        dc.l mdm3ibuf
        dc.w SERIAL_BUFSIZE,0,0,128,192
        dc.l mdm3obuf
        dc.w SERIAL_BUFSIZE,0,0,128,192
        dc.b 0,0,0,0,1,0,1,-1
		text


/* 306de: 00e026b2 */
copymem:
        move.b    (a1)+,(a0)+
        dbf       d0,copymem
        rts

ikbdtable:                
        dc.l      ikbdbuf
        dc.w      SERIAL_BUFSIZE,0,0,64,192
miditable:
        dc.l      midibuf
        dc.w      128,0,0,32,96


/*
 * initial EXT_IOREC for device 6 (rs232)
 */
rs232table:
        dc.l      rs232ibuf
        dc.w      SERIAL_BUFSIZE,0,0,128,192
        dc.l      rs232obuf
        dc.w      SERIAL_BUFSIZE,0,0,128,192
        dc.b      0,0,0,0,1,0,1,-1

mfpvectr:
        dc.l txerror
        dc.l txrint
        dc.l rxerror
        dc.l rcvrint

settimer:
        movem.l   d0-d4/a0-a3,-(a7)
        movea.w   #MFP_REGS,a0
        movea.l   #imrt,a3
        movea.l   #imrmt,a2
        bsr.s     mskreg
        movea.l   #iert,a3
        movea.l   #imrmt,a2
        bsr.s     mskreg
        movea.l   #iprt,a3
        movea.l   #imrmt,a2
        bsr.s     mskreg
        movea.l   #isrt,a3
        movea.l   #imrmt,a2
        bsr.s     mskreg
        movea.l   #tcrtab,a3
        movea.l   #tcrmsk,a2
        bsr.s     mskreg
        exg       a3,a1
        lea.l     tdrtab,a3
        moveq.l   #0,d3
        move.b    0(a3,d0.w),d3
settim1:
        move.b    d2,0(a0,d3.w)
        cmp.b     0(a0,d3.w),d2
        bne.s     settim1
        exg       a3,a1
        or.b      d1,(a3)
        movem.l   (a7)+,d0-d4/a0-a3
        rts

mskreg:
        bsr.s     getmask
        move.b    (a2),d3
        and.b     d3,(a3)
        rts

getmask:
        moveq.l   #0,d3
        adda.w    d0,a3
        move.b    (a3),d3
        add.l     a0,d3
        movea.l   d3,a3
        adda.w    d0,a2
        rts

iert:    dc.b $06,$06,$08,$08
iprt:    dc.b $0a,$0a,$0c,$0c
isrt:    dc.b $0e,$0e,$10,$10
imrt:    dc.b $12,$12,$14,$14
imrmt:   dc.b $df,$fe,$df,$ef
tcrtab:  dc.b $18,$1a,$1c,$1c
tcrmsk:  dc.b $00,$00,$8f,$f8
tdrtab:  dc.b $1e,$20,$22,$24

/*
 * XBIOS #13 - Mfpint - Set the MFP interrupt number
 *
 * Set the MFP interrupt number 'interno' (0 to 15) to 'vector'.
 * The old vector is written over (and thus unrecoverable).
 */
/* 306de: 00e027b0 */
_mfpint:
        move.w    4(a7),d0
        movea.l   6(a7),a2
        andi.l    #$0000000F,d0
initint:
        movem.l   d0-d2/a0-a2,-(a7)
        bsr.s     jdisint1
        move.l    d0,d2
        asl.w     #2,d2
        addi.l    #$00000100,d2
        movea.l   d2,a1
        move.l    a2,(a1)
        bsr.s     jenabin1
        movem.l   (a7)+,d0-d2/a0-a2
        rts

/*
 * XBIOS #26 - Jdisint - Disable interrupt number 'intno' on the 68901
 */
/* 306de: 00e027da */
_jdisint:
        move.w    4(a7),d0
        andi.l    #$0000000F,d0
jdisint1:
        movem.l   d0-d1/a0-a1,-(a7)
        lea.l     (MFP_REGS).w,a0
        lea.l     MFP_IMRA-MFP_REGS(a0),a1
        bsr.s     mfpea
        bclr      d1,(a1)
        lea.l     MFP_IERA-MFP_REGS(a0),a1
        bsr.s     mfpea
        bclr      d1,(a1)
        lea.l     MFP_ISRA-MFP_REGS(a0),a1
        bsr.s     mfpea
        move.b    #$FE,d0
        rol.b     d1,d0
        move.b    d0,(a1)
        movem.l   (a7)+,d0-d1/a0-a1
        rts

/*
 * XBIOS #27 - Jenabint - Enable interrupt number 'intno' on the 68901,
 */
/* 306de: 00e02810 */
_jenabint:
        move.w    4(a7),d0
        andi.l    #$0000000F,d0
jenabin1:
        movem.l   d0-d1/a0-a1,-(a7)
        lea.l     (MFP_REGS).w,a0
        lea.l     MFP_IERA-MFP_REGS(a0),a1
        bsr.s     mfpea
        bset      d1,(a1)
        lea.l     MFP_IMRA-MFP_REGS(a0),a1
        bsr.s     mfpea
        bset      d1,(a1)
        movem.l   (a7)+,d0-d1/a0-a1
        rts

/*
 * helper function for above.
 * calculates the bit number and the MFP register address.
 */
mfpea:
        move.b    d0,d1
        cmpi.b    #$08,d1
        blt.s     mfpea1
        subq.w    #8,d1
        rts
mfpea1:
        addq.l    #MFP_IERB-MFP_IERA,a1
        rts

/*************************************************************************
 *
 *               receiver buffer full interrupt routine
 *
 *               grabs data from the rs-232 receiver port
 *
 *************************************************************************/
/* 306de: 00e02848 */
rcvrint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (rs232iorec).w,a0
        lea.l     (MFP_REGS).w,a2
rcvrint1:
        move.b    MFP_RSR-MFP_REGS(a2),rcv(a0)
        move.b    MFP_UDR-MFP_REGS(a2),d0
        btst      #0,ctr(a0)
        beq.s     rcvrint3
        cmpi.b    #$13,d0
        bne.s     rcvrint2
        move.b    #-1,xoff(a0)
        bra.s     rcvrint7
rcvrint2:
        cmpi.b    #$11,d0
        bne.s     rcvrint3
        clr.b     xoff(a0)
        bra.s     rcvrint6
rcvrint3:
        move.w    tail(a0),d1
        bsr       wrapin
        cmp.w     head(a0),d1
        beq.s     rcvrint7
        bsr       iorecput
        tst.b     ctr(a0)
        beq.s     rcvrint7
        move.w    tail(a0),d0
        sub.w     head(a0),d0
        bpl.s     rcvrint4
        add.w     size(a0),d0
rcvrint4:
        cmp.w     high(a0),d0
        blt.s     rcvrint7
        tst.b     rcts(a0)
        bne.s     rcvrint7
        move.b    #-1,rcts(a0)
        btst      #0,ctr(a0)
        bne.s     rcvrint5
        bsr       rtsoff
        bra.s     rcvrint7
rcvrint5:
        move.b    #$13,xonoffc(a0)
rcvrint6:
        tst.b     MFP_TSR-MFP_REGS(a2)
        bpl.s     rcvrint7
        bsr       iorecout
rcvrint7:
        move.b    #$EF,MFP_ISRA-MFP_REGS(a2)
        movem.l   (a7)+,d0-d1/a0-a2
        rte

/*************************************************************************
 *
 *               transmit buffer empty interrupt routine
 *
 *************************************************************************/
/* 306de: 00e028e2 */
txrint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (MFP_REGS).w,a2
        lea.l     (rs232iorec).w,a0
txrint1:
        bsr       iorecout
        move.b    #$FB,MFP_ISRA-MFP_REGS(a2)
        movem.l   (a7)+,d0-d1/a0-a2
        rte

/*************************************************************************
 *
 *               Clear-To-Send interrupt routine
 *
 *************************************************************************/
/* 306de: 00e028fe */
ctsint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (rs232iorec).w,a0
        lea.l     (MFP_REGS).w,a2
        btst      #1,ctr(a0)
        beq.s     ctsint2
        btst      #2,(a2)
        bne.s     ctsint1
        clr.b     xoff(a0)
        bset      #2,MFP_AER-MFP_REGS(a2)
        tst.b     MFP_TSR-MFP_REGS(a2)
        bpl.s     ctsint2
        bsr.s     iorecout
        bra.s     ctsint2
ctsint1:
        move.b    #-1,xoff(a0)
        bclr      #2,MFP_AER-MFP_REGS(a2)
ctsint2:
        move.b    #$FB,MFP_ISRB-MFP_REGS(a2)
        movem.l   (a7)+,d0-d1/a0-a2
        rte


/*************************************************************************
 *               routines to handle tx or rx errors
 *************************************************************************/
/* 306de: 00e02944 */
rxerror:
        movem.l   d0/a0,-(a7)
        lea.l     (MFP_REGS).w,a0
rxerror1:
        move.b    MFP_RSR-MFP_REGS(a0),rs232iorec+rcv
        move.b    MFP_UDR-MFP_REGS(a0),d0
        move.b    #$F7,MFP_ISRA-MFP_REGS(a0)
        movem.l   (a7)+,d0/a0
        rte

/* 306de: 00e02964 */
txerror:
        move.l    a0,-(a7)
        lea.l     (MFP_REGS).w,a0
txerror1:
        move.b    MFP_TSR-MFP_REGS(a0),rs232iorec+wr5
        move.b    #$FD,MFP_ISRA-MFP_REGS(a0)
        movea.l   (a7)+,a0
        tst.b     (MFP_TSR).w
        rte

/*************************************************************************
 * write a character from the ring buffer to the serial port
 *************************************************************************/
/* 306de: 00e02980 */
iorecout:
        move.l    a0,-(a7)
        move.b    xonoffc(a0),d0
        beq.s     ioreco1
        clr.b     xonoffc(a0)
        bra.s     ioreco2
ioreco1:
        move.b    ctr(a0),d0
        and.b     xoff(a0),d0
        bne.s     ioreco3
        adda.w    #IOREC_SIZE,a0
        move.w    head(a0),d0
        cmp.w     tail(a0),d0
        beq.s     ioreco3
        bsr.s     iorecget
ioreco2:
        tst.b     MFP_TSR-MFP_REGS(a2)
        bpl.s     ioreco2
        move.b    MFP_TSR-MFP_REGS(a2),$00000EC3
        move.b    d0,MFP_UDR-MFP_REGS(a2)
ioreco3:
        movea.l   (a7)+,a0
        rts

/*************************************************************************
 * put a character into the ring buffer
 *************************************************************************/
/* 306de: 00e029be */
iorecput:
        move.w    tail(a0),d1
        bsr.s     wrapin
iorecp1:
        cmp.w     head(a0),d1
        beq.s     iorecp1
        movea.l   buf(a0),a1
        and.l     #$0000FFFF,d1
        move.b    d0,0(a1,d1.l)
        move.w    d1,tail(a0)
        rts

/*************************************************************************
 * get a character from the ring buffer
 *************************************************************************/
/* 306de: 00e029dc */
iorecget:
        move.w    head(a0),d1
        cmp.w     tail(a0),d1
        beq.s     iorecget
        bsr.s     wrapin
        movea.l   buf(a0),a1
        moveq.l   #0,d0
        and.l     #$0000FFFF,d1
        move.b    0(a1,d1.l),d0
        move.w    d1,head(a0)
        rts

/*************************************************************************
 * set RTS signal
 *************************************************************************/
/* 306de: 00e029fc */
rtson:
        lea.l     ($FFFF8800).w,a1
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.b    #$0E,(a1)
        move.b    (a1),d1
        and.b     #$F7,d1
        move.b    d1,2(a1)
        move.w    (a7)+,sr
        rts

/*************************************************************************
 * clear RTS signal
 *************************************************************************/
/* 306de: 00e02a18 */
rtsoff:
        lea.l     ($FFFF8800).w,a1
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.b    #$0E,(a1)
        move.b    (a1),d1
        ori.b     #$08,d1
        move.b    d1,2(a1)
        move.w    (a7)+,sr
        rts

/*************************************************************************
 * get next index for ringbuffer into D1
 *************************************************************************/
wrapin:
        addq.w    #1,d1
        cmp.w     size(a0),d1
        bcs.s     wrapin1
        moveq.l   #0,d1
wrapin1:
        rts


/*
 * XBIOS #14 - Iorec - Returns pointer to a serial device's input buffer record.
 */
/* 306de: 00e02a40 */
_iorec:
        move.w    4(a7),d1
        beq.s     iorec1               /* for device 0 (rs232), return bconmap configuration */
        asl.l     #2,d1
        move.l    iorectab(pc,d1.w),d0
        rts
iorec1:
        move.l    (bcmap_root+12).w,d0 /* bcmap_root.iorec */
        rts
iorectab:
        dc.l rs232iorec
        dc.l ikbdiorec
        dc.l midiiorec


/*
 * XBIOS #15 - Rsconf - Configure RS-232 port.
 *
 * If any parameter is -1 ($FFFF), the corresponding hardware register
 * is not set.
 *
 * speed  - the port's baud rate
 * flow   - the flow control
 * ucr    - 68901 register
 * rsr    - 68901 register
 * tsr    - 68901 register
 * scr    - 68901 register
 */
/* 306de: 00e02a60 */
_rsconf:
        movea.l   (bcmap_root+8).w,a0 /* bcmap_root.rscon */
        jmp       (a0)

/*
 * Rsconf function for device 6 (rs232 driven by MFP)
 */
/* 306de: 00e02a66 */
_rs6conf:
        lea.l     (rs232iorec).w,a0
        lea.l     (MFP_REGS).w,a2
rs6conf1:
        moveq.l   #0,d0
        cmpi.w    #-2,4(a7)
        bne.s     rs6conf2
        move.b    baudrate(a0),d0
        rts
rs6conf2:
        ori.w     #$0700,sr
        movep.l   MFP_UCR-MFP_REGS(a2),d7
        move.w    6(a7),d0
        cmp.w     #$0003,d0
        bhi.s     rs6conf6
        bne.s     rs6conf3
        moveq.l   #1,d0
rs6conf3:
        cmp.b     ctr(a0),d0
        beq.s     rs6conf6
        move.w    d0,-(a7)
        tst.b     xoff(a0)
        beq.s     rs6conf4
        clr.b     xoff(a0)
        bsr       iorecout
rs6conf4:
        tst.b     rcts(a0)
        beq.s     rs6conf5
        bsr       rs232cont
rs6conf5:
        move.w    (a7)+,d0
        move.b    d0,ctr(a0)
rs6conf6:
        tst.w     4(a7)
        bmi.s     rs6conf7
        bclr      #0,MFP_RSR-MFP_REGS(a2)
        bclr      #0,MFP_TSR-MFP_REGS(a2)
        move.w    4(a7),d1
        move.b    d1,baudrate(a0)
        move.b    baudctrl(pc,d1.w),d0
        move.b    bauddata(pc,d1.w),d2
        andi.b    #$70,MFP_TCDCR-MFP_REGS(a2)
        move.b    d2,MFP_TDDR-MFP_REGS(a2)
        or.b      d0,MFP_TCDCR-MFP_REGS(a2)
        bset      #0,MFP_RSR-MFP_REGS(a2)
        bset      #0,MFP_TSR-MFP_REGS(a2)
rs6conf7:
        tst.w     8(a7)
        bmi.s     rs6conf8
        move.b    9(a7),MFP_UCR-MFP_REGS(a2)
rs6conf8:
        tst.w     10(a7)
        bmi.s     rs6conf9
        move.b    11(a7),MFP_RSR-MFP_REGS(a2)
rs6conf9:
        tst.w     12(a7)
        bmi.s     rs6con10
        move.b    13(a7),MFP_TSR-MFP_REGS(a2)
rs6con10:
        tst.w     14(a7)
        bmi.s     rs6con11
        move.b    15(a7),MFP_SCR-MFP_REGS(a2)
rs6con11:
        move.l    d7,d0
        rts

baudctrl: dc.b $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$02,$02
bauddata: dc.b $01,$02,$04,$05,$08,$0a,$0b,$10,$20,$40,$60,$80,$8f,$af,$40,$60

/*
 * initial values for 4 extra MAPTAB structures
 */
/* 306de: 00e02b4a */
bcondevs:
        dc.l      _bcon6stat
        dc.l      _bcon6in
        dc.l      _bco6stat
        dc.l      _bcon6out
        dc.l      _rs6conf
        dc.l      rs232iorec
        dc.l      _bcon7stat
        dc.l      _bcon7in
        dc.l      _bco7stat
        dc.l      _bcon7out
        dc.l      _rs7conf
        dc.l      mdm2iorec
        dc.l      _bcon8stat
        dc.l      _bcon8in
        dc.l      _bco8stat
        dc.l      _bcon8out
        dc.l      _rs8conf
        dc.l      mdm3iorec
        dc.l      _bcon9stat
        dc.l      _bcon9in
        dc.l      _bco9stat
        dc.l      _bcon9out
        dc.l      _rs9conf
        dc.l      mdm4iorec

/*
 * XBIOS #44 - Bconmap - Select a default port.
 */
/* 306de: 00e02baa */
_bconmap:
        moveq.l   #0,d0
        move.w    4(a7),d1
        move.w    (bcmap_root+6).w,d0 /* bcmap_root.auxnr */
        cmp.w     #$FFFF,d1
        beq.s     bconmap1
        move.l    #bcmap_root,d0
        cmp.w     #$FFFE,d1
        beq.s     bconmap1
        moveq.l   #0,d0
        subq.w    #BCONMAP_START_HANDLE,d1
        bmi.s     bconmap1
        cmp.w     (bcmap_root+4).w,d1 /* bcmap_root.maptabsize */
        bcc.s     bconmap1
        move.w    (bcmap_root+6).w,d1 /* bcmap_root.auxnr */
        subq.w    #6,d1
        asl.w     #3,d1 /* calculate d1 * 24 */
        move.w    d1,d2
        add.w     d1,d1
        add.w     d1,d2 /* d2 now sizeof(MAPTAB) */
        movea.l   (bcmap_root).w,a0 /* bcmap_root.maptab */
        adda.w    d2,a0
/* build active MAPTAB from system variables for device 1 (aux) */
        lea.l     (_bconstat_vec+1*4).w,a1
        move.l    (a1),(a0)+
        move.l    32(a1),(a0)+
        move.l    64(a1),(a0)+
        move.l    96(a1),(a0)+
        move.l    bcmap_root+8,(a0)+ /* bcmap_root.rscon */
        move.l    (bcmap_root+12).w,(a0)+ /* bcmap_root.iorec */
        move.w    4(a7),d1
        subq.w    #BCONMAP_START_HANDLE,d1
        asl.w     #3,d1
        move.w    d1,d2
        add.w     d1,d1
        add.w     d1,d2
        movea.l   (bcmap_root).w,a0 /* bcmap_root.maptab */
        adda.w    d2,a0
/* update system variables for device 1 (aux) */
        move.l    (a0)+,(a1)
        move.l    (a0)+,32(a1)
        move.l    (a0)+,64(a1)
        move.l    (a0)+,96(a1)
        move.l    (a0)+,(bcmap_root+8).w /* bcmap_root.rscon */
        move.l    (a0)+,(bcmap_root+12).w /* bcmap_root.iorec */
        move.w    (bcmap_root+6).w,d0 /* bcmap_root.auxnr */
        move.w    4(a7),(bcmap_root+6).w
bconmap1:
        rts


initdevstables:
        lea.l     (bcmap_devs).w,a0        
        move.l    a0,(bcmap_root+0).w     /* bcmap_root.maptab */
        move.w    #$0004,(bcmap_root+4).w /* bcmap_root.maptabsize */
        lea.l     bcondevs(pc),a1
        move.w    #4*6-1,d0
initdev1:
        move.l    (a1)+,(a0)+
        dbf       d0,initdev1
        move.w    #$0006,(bcmap_root+6).w /* bcmap_root.auxnr */
/* install device 6 vectors also in system variables for device 1 (aux) */
        lea.l     bcondevs,a1
        lea.l     (_bconstat_vec+1*4).w,a0
        move.l    (a1)+,(a0)
        move.l    (a1)+,32(a0)
        move.l    (a1)+,64(a0)
        move.l    (a1)+,96(a0)
        move.l    (a1)+,(bcmap_root+8).w /* bcmap_root.rscon */
        move.l    (a1)+,(bcmap_root+12).w /* bcmap_root.iorec */
        rts

/*
 * Bconstat function for device 8 (TTMFP serial port)
 */
_bcon8stat:
        lea.l     (mdm3iorec).w,a0
        bra       bcon6st1

/*
 * Bconin function for device 8 (TTMFP serial port)
 */
_bcon8in:
        lea.l     (mdm3iorec).w,a0
        lea.l     (TTMFP_REGS).w,a2
        bra       bcon6in1

/*
 * Bcostat function for device 8 (TTMFP serial port)
 */
_bco8stat:
        lea.l     (mdm3iorec+IOREC_SIZE).w,a0
        bra       bco6s1


/*
 * Bconout function for device 8 (TTMFP serial port)
 */
_bcon8out:
        move.w    6(a7),d0
        lea.l     (mdm3iorec+IOREC_SIZE).w,a0
        bsr       iorecput
        lea.l     (mdm3iorec).w,a0
        lea.l     (TTMFP_REGS).w,a2
        bra       bcon6o1

/*************************************************************************
 * receiver buffer full interrupt routine for TTMFP
 *************************************************************************/
/* 306de: 00e02cb0 */
ttrcvrint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (mdm3iorec).w,a0
        lea.l     (TTMFP_REGS).w,a2
        bra       rcvrint1

/*************************************************************************
 * transmit buffer empty interrupt routine fo TTFMP
 *************************************************************************/
tttxrint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (TTMFP_REGS).w,a2
        lea.l     (mdm3iorec).w,a0
        bra       txrint1

/*************************************************************************
 * routines to handle tx or rx errors for TTMFP
 *************************************************************************/
ttrxerror:
        movem.l   d0/a0,-(a7)
        lea.l     (TTMFP_REGS).w,a0
        bra       rxerror1

tttxerror:
        move.l    a0,-(a7)
        lea.l     (TTMFP_REGS).w,a0
        bra       txerror1

/*
 * Rsconf function for device 8 (TTMFP serial port)
 */
/* 306de: 00e02ce6 */
_rs8conf:
        lea.l     (mdm3iorec).w,a0
        lea.l     (TTMFP_REGS).w,a2
        bra       rs6conf1


sccinit:
        move.b    (a1)+,d0
        bmi.s     sccinit1
        move.b    d0,(a0)
        move.b    (a1)+,(a0)
        bra.s     sccinit
sccinit1:
        rts


scctbl:
        dc.b $04,$44            /* x16 clock mode, 1 stop bit, no parity */
        dc.b $01,$04            /* 'parity is special condition' */
        dc.b $02,$60            /* interrupt vector #s start at 0x60 (lowmem 0x180) */
        dc.b $03,$c0            /* Rx 8 bits/char, disabled */
        dc.b $05,$e2            /* Tx 8 bits/char, disabled, DTR, RTS */
        dc.b $06,$00            /* SDLC (n/a) */
        dc.b $07,$00            /* SDLC (n/a) */
        dc.b $09,$01            /* status low, vector includes status */
        dc.b $0a,$00            /* misc flags */
        dc.b $0b,$50            /* Rx/Tx clocks from baudrate generator output */
        dc.b $0c,$18            /* time const low = 24 | so rate = (24+2)*2/BR clock period */
        dc.b $0d,$00            /* time const hi = 0   | = 52/(8053976/16) => 9680 bps */
        dc.b $0e,$02            /* baudrate generator source = PCLK (8MHz) */
        dc.b $0e,$03            /* ditto + enable baudrate generator */
        dc.b $03,$c1            /* Rx 8 bits/char, enabled */
        dc.b $05,$ea            /* Tx 8 bits/char, enabled, DTR, RTS */
        dc.b $0f,$20            /* CTS interrupt enable */
        dc.b $00,$10            /* reset external/status interrupts */
        dc.b $00,$10            /* reset again (necessary, see manual) */
        dc.b $01,$17            /* interrupts for Rx, Tx, special condition; parity is special */
        dc.b $09,$09            /* status low, master interrupt enable */
        dc.b $ff,$00            /* end of table */


/* disconnect LAN from port a */
nolan:
        move.w    sr,d1
        ori.w     #$0700,sr
        move.b    #$0E,($FFFF8800).w
        move.b    ($FFFF8800).w,d0
        bset      #7,d0
        move.b    #$0E,($FFFF8800).w
        move.b    d0,($FFFF8802).w
        move.w    d1,sr
        rts

/* 306de: 00e02d4c */
initscc:
        movea.l   (_buserror).w,a0
        movea.l   a7,a1
        move.l    #initscc2,(_buserror).w
        tst.b     (SCCA_DATA).w
        move.l    a0,(_buserror).w
        lea.l     ($00000180).w,a0
        lea.l     sccvect(pc),a1
        moveq.l   #4*4-1,d0
initscc1:
        move.l    (a1)+,(a0)+
        dbf       d0,initscc1
        clr.w     ($FFFF8C14).w
        bsr.s     nolan
        lea.l     (mdm4iorec).w,a0
        lea.l     mdm2table(pc),a1
        moveq.l   #EXTIOREC_SIZE+1,d0 /* BUG: should be -1 */
        bsr       copymem
        lea.l     (mdm2iorec).w,a0
        lea.l     mdm2table(pc),a1
        moveq.l   #EXTIOREC_SIZE+1,d0 /* BUG: should be -1 */
        bsr       copymem
        move.l    #mdm2ibuf,(mdm2iorec).w
        move.l    #mdm2obuf,(mdm2iorec+IOREC_SIZE).w
        lea.l     (SCCA_BASE).w,a2
        move.b    #$09,(a2)
        move.b    #$C0,(a2)
        move.w    #$0104,d0
        jsr       mfpdelay
        lea.l     (SCCA_BASE).w,a0
        lea.l     scctbl(pc),a1
        bsr       sccinit
        lea.l     (SCCB_BASE).w,a0
        lea.l     scctbl(pc),a1
        bsr       sccinit
        bset      #5,(vme_mask).w
        rts
initscc2:
        move.l    a0,(_buserror).w
        movea.l   a1,a7
        rts

/*************************************************************************
 * transmit buffer empty interrupt routine for SCC channel B
 *************************************************************************/
sccbempty:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCB_BASE).w,a2
        lea.l     (mdm2iorec).w,a0
        bra       sccempty

/*************************************************************************
 * status interrupt routine for SCC channel B
 *************************************************************************/
sccbstat:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCB_BASE).w,a2
        lea.l     (mdm2iorec).w,a0
        bra       sccstat

/*************************************************************************
 * receiver buffer full interrupt routine for SCC channel B
 *************************************************************************/
sccbrxint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCB_BASE).w,a2
        lea.l     (mdm2iorec).w,a0
        bra.s     sccrxint

/*************************************************************************
 * RX/TX error interrupt routine for SCC channel B (unused)
 *************************************************************************/
sccbrxerr:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCB_BASE).w,a2
        lea.l     mdm2iorec,a0
        bra       sccrxerr

/*************************************************************************
 * transmit buffer empty interrupt routine for SCC channel A
 *************************************************************************/
sccaempty:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCA_BASE).w,a2
        lea.l     (mdm4iorec).w,a0
        bra       sccempty

/*************************************************************************
 * status interrupt routine for SCC channel A
 *************************************************************************/
sccastat:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCA_BASE).w,a2
        lea.l     (mdm4iorec).w,a0
        bra       sccstat

/*************************************************************************
 * receiver buffer full interrupt routine for SCC channel A
 *************************************************************************/
sccarxint:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCA_BASE).w,a2
        lea.l     (mdm4iorec).w,a0
        bra.s     sccrxint

/*************************************************************************
 * RX/TX error interrupt routine for SCC channel A (unused)
 *************************************************************************/
sccarxerr:
        movem.l   d0-d1/a0-a2,-(a7)
        lea.l     (SCCA_BASE).w,a2
        lea.l     mdm4iorec,a0
        bra       sccrxerr

/*************************************************************************
 * receiver buffer full interrupt routine for SCC
 *************************************************************************/
/* 306de: 00e02e62 */
sccrxint:
        move.b    #$08,(a2)
        move.b    (a2),d0
        and.b     datamask(a0),d0
        btst      #0,ctr(a0)
        beq.s     sccrxin2
        cmp.b     #$13,d0
        bne.s     sccrxin1
        st        xoff(a0)
        bra.s     sccrxin6
sccrxin1:
        cmp.b     #$11,d0
        bne.s     sccrxin2
        tst.b     xoff(a0)
        sf        xoff(a0)
        bne.s     sccrxin5
sccrxin2:
        move.w    tail(a0),d1
        bsr       wrapin
        cmp.w     head(a0),d1
        beq.s     sccrxin6
        bsr       iorecput
        tst.b     ctr(a0)
        beq.s     sccrxin6
        tst.b     rcts(a0)
        bne.s     sccrxin6
        move.w    tail(a0),d0
        sub.w     head(a0),d0
        bpl.s     sccrxin3
        add.w     size(a0),d0
sccrxin3:
        cmp.w     high(a0),d0
        blt.s     sccrxin6
        st        rcts(a0)
        btst      #0,ctr(a0)
        bne.s     sccrxin4
        move.b    wr5(a0),d0
        bclr      #1,d0
        move.b    d0,wr5(a0)
        move.b    #$05,(a2)
        move.b    d0,(a2)
        bra.s     sccrxin6
sccrxin4:
        move.b    #$13,xonoffc(a0)
sccrxin5:
        move.b    #$00,(a2)
        move.b    (a2),d0
        btst      #2,d0
        beq.s     sccrxin6
        bsr.s     sccout
sccrxin6:
        move.b    #$00,(a2)
        move.b    #$38,(a2)
        movem.l   (a7)+,d0-d1/a0-a2
        rte

/*************************************************************************
 * RX/TX error interrupt routine for SCC
 *************************************************************************/
sccrxerr:
        move.b    #$01,(a2)
        move.b    (a2),d0
        move.b    #$08,(a2)
        move.b    (a2),d0
        move.b    #$00,(a2)
        move.b    #$30,(a2)
        bra.s     sccrxin6

/*************************************************************************
 * write a character from the ring buffer to the SCC
 *************************************************************************/
/* 306de: 00e02f1a */
sccout:
        move.l    a0,-(a7)
        tst.b     xonoffc(a0)
        bne.s     sccout1
        move.b    ctr(a0),d0
        and.b     xoff(a0),d0
        bne.s     sccout4
sccout1:
        move.b    #$00,(a2)
        move.b    (a2),d0
        btst      #2,d0
        beq.s     sccout4
        move.b    xonoffc(a0),d0
        beq.s     sccout2
        clr.b     xonoffc(a0)
        bra.s     sccout3
sccout2:
        adda.w    #$000E,a0
        move.w    head(a0),d0
        cmp.w     tail(a0),d0
        beq.s     sccout4
        bsr       iorecget
sccout3:
        move.b    #$08,(a2)
        move.b    d0,(a2)
sccout4:
        movea.l   (a7)+,a0
        rts

/*************************************************************************
 * transmit buffer empty interrupt routine for SCC
 *************************************************************************/
/* 306de: 00e02f60 */
sccempty:
        move.b    #$00,(a2)
        move.b    #$28,(a2)
        move.b    #$00,(a2)
        move.b    #$38,(a2)
        bsr.s     sccout
        movem.l   (a7)+,d0-d1/a0-a2
        rte

/*************************************************************************
 * status interrupt routine for SCC
 *************************************************************************/
/* 306de: 00e02f78 */
sccstat:
        btst      #1,ctr(a0)
        beq.s     sccstat1
        move.b    #$00,(a2)
        move.b    (a2),d0
        btst      #5,d0
        seq       xoff(a0)
        beq.s     sccstat1
        bsr.s     sccout
sccstat1:
        move.b    #$00,(a2)
        move.b    #$10,(a2)
        move.b    #$00,(a2)
        move.b    #$38,(a2)
        movem.l   (a7)+,d0-d1/a0-a2
        rte

/*
 * Bconstat function for device 7 (SCC channel B)
 */
/* 306de: 00e02fa8 */
_bcon7stat:
        lea.l     (mdm2iorec).w,a0
        lea.l     (SCCB_BASE).w,a2
        bra.s     sccistat

/*
 * Bconin function for device 7 (SCC channel B)
 */
/* 306de: 00e02fb2 */
_bcon7in:
        lea.l     (mdm2iorec).w,a0
        lea.l     (SCCB_BASE).w,a2
        bra.s     sccin

/*
 * Bcostat function for device 7 (SCC channel B)
 */
/* 306de: 00e02fbc */
_bco7stat:
        lea.l     (mdm2iorec).w,a0
        lea.l     (SCCB_BASE).w,a2
        bra.s     sccostat

/*
 * Bconout function for device 7 (SCC channel B)
 */
/* 306de: 00e02fc6 */
_bcon7out:
        lea.l     (mdm2iorec).w,a0
        lea.l     (SCCB_BASE).w,a2
        bra       sccxout

/*
 * Bconstat function for device 9 (SCC channel A)
 */
/* 306de: 00e02fd2 */
_bcon9stat:
        lea.l     (mdm4iorec).w,a0
        lea.l     (SCCA_BASE).w,a2
        bra.s     sccistat

/*
 * Bconin function for device 9 (SCC channel A)
 */
/* 306de: 00e02fdc */
_bcon9in:
        lea.l     (mdm4iorec).w,a0
        lea.l     (SCCA_BASE).w,a2
        bra.s     sccin

/*
 * Bcostat function for device 9 (SCC channel A)
 */
/* 306de: 00e02fe6 */
_bco9stat:
        lea.l     (mdm4iorec).w,a0
        lea.l     (SCCA_BASE).w,a2
        bra.s     sccostat

/*
 * Bconout function for device 9 (SCC channel A)
 */
/* 306de: 00e02ff0 */
_bcon9out:
        lea.l     (mdm4iorec).w,a0
        lea.l     (SCCA_BASE).w,a2
        bra.s     sccxout

sccistat:
        moveq.l   #0,d0
        lea.l     head(a0),a1
        lea.l     tail(a0),a0
        cmpm.w    (a0)+,(a1)+
        beq.s     sccist1
        moveq.l   #-1,d0
sccist1:
        rts

sccostat:
        move.w    tail(a0),d1
        bsr       wrapin
        cmp.w     head(a0),d1
        beq.s     sccost1
        moveq.l   #-1,d0
        rts
sccost1:
        moveq.l   #0,d0
        rts

sccin:
        bsr       iorecget
        move.w    d0,-(a7)
        tst.b     ctr(a0)
        beq.s     sccin2
        tst.b     rcts(a0)
        beq.s     sccin2
        move.w    tail(a0),d0
        sub.w     head(a0),d0
        bpl.s     sccin1
        add.w     size(a0),d0
sccin1:
        cmp.w     low(a0),d0
        bgt.s     sccin2
        bsr.s     scccont
sccin2:
        move.w    (a7)+,d0
        rts

scccont:
        clr.b     rcts(a0)
        btst      #0,ctr(a0)
        beq.s     scccont1
        move.b    #$11,xonoffc(a0)
        bra.s     sccxout0
scccont1:
        move.b    wr5(a0),d0
        bset      #1,d0
        move.b    d0,wr5(a0)
        move.b    #$05,(a2)
        move.b    d0,(a2)
        rts

sccxout:
        move.w    6(a7),d0
        adda.w    #IOREC_SIZE,a0
        bsr       iorecput
        suba.w    #IOREC_SIZE,a0
sccxout0:
        move.b    #$00,(a2)
        move.b    (a2),d0
        btst      #2,d0
        beq.s     sccxout1
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        bsr       sccout
        move.w    (a7)+,sr
sccxout1:
        rts


/*
 * Rsconf function for device 9 (SCC channel A)
 */
/* 306de: 00e030a0 */
_rs9conf:
        lea.l     (mdm4iorec).w,a0
        lea.l     (SCCA_BASE).w,a2
        bra.s     sccconf

/*
 * Rsconf function for device 7 (SCC channel B)
 */
/* 306de: 00e030aa */
_rs7conf:
        lea.l     (mdm2iorec).w,a0
        lea.l     (SCCB_BASE).w,a2
sccconf:
        moveq.l   #0,d0
        cmpi.w    #$FFFE,4(a7)
        bne.s     sccconf1
        move.b    baudrate(a0),d0
        rts
sccconf1:
        ori.w     #$0700,sr
        moveq.l   #0,d7
        move.b    rcv(a0),d7
        asl.w     #8,d7
        swap      d7
        move.b    wr5(a0),d7
        lsr.b     #1,d7
        and.b     #$04,d7
        asl.w     #8,d7
        move.w    6(a7),d0
        cmp.w     #$0003,d0
        bhi.s     sccconf5
        bne.s     sccconf2
        moveq.l   #1,d0
sccconf2:
        cmp.b     ctr(a0),d0
        beq.s     sccconf5
        tst.b     xoff(a0)
        beq.s     sccconf3
        clr.b     xoff(a0)
        bsr       sccout
sccconf3:
        tst.b     rcts(a0)
        beq.s     sccconf4
        move.w    d0,-(a7)
        bsr       scccont
        move.w    (a7)+,d0
sccconf4:
        move.b    d0,ctr(a0)
sccconf5:
        move.w    4(a7),d0
        cmp.w     #$000F,d0
        bhi.s     sccconf6
        move.b    d0,baudrate(a0)
        asl.w     #1,d0
        lea.l     sccbaudtab,a1
        move.w    0(a1,d0.w),d0
        move.b    #$0C,(a2)
        move.b    d0,(a2)
        lsr.w     #8,d0
        move.b    #$0D,(a2)
        move.b    d0,(a2)
sccconf6:
        move.w    8(a7),d0
        bmi.s     scccon11
        move.b    d0,rcv(a0)
        move.b    d0,d1
        and.b     #$60,d1
        lsr.b     #5,d1
        moveq.l   #-1,d2
        lsr.b     d1,d2
        move.b    d2,datamask(a0)
        move.b    d0,d1
        and.b     #$60,d1
        beq.s     sccconf7
        cmp.b     #$60,d1
        bne.s     sccconf8
sccconf7:
        eori.b    #$60,d1
sccconf8:
        move.b    wr5(a0),d2
        and.b     #$9F,d2
        or.b      d1,d2
        move.b    d2,wr5(a0)
        move.b    #$05,(a2)
        move.b    d2,(a2)
        asl.b     #1,d1
        or.b      #$01,d1
        move.b    #$03,(a2)
        move.b    d1,(a2)
        move.b    d0,d1
        and.b     #$1E,d1
        lsr.b     #1,d1
        bclr      #1,d1
        sne       d2
        bclr      #0,d1
        bne.s     sccconf9
        bclr      #1,d2
        bra.s     scccon10
sccconf9:
        bset      #1,d2
scccon10:
        and.b     #$03,d2
        or.b      d2,d1
        or.b      #$40,d1
        move.b    #$04,(a2)
        move.b    d1,(a2)
scccon11:
        move.w    10(a7),d0
        bmi.s     scccon13
        btst      #3,d0
        beq.s     scccon12
        bset      #4,wr5(a0)
        bne.s     scccon13
        move.b    #$05,(a2)
        move.b    wr5(a0),(a2)
        bra.s     scccon13
scccon12:
        bclr      #4,wr5(a0)
        beq.s     scccon13
        move.b    #$05,(a2)
        move.b    wr5(a0),(a2)
scccon13:
        move.l    d7,d0
        rts

/* 306de: 00e031e4 */
sccbaudtab: dc.w $000b,$0018,$0032,$0044,$0067,$007c,$008a,$00d0
            dc.w $01a1,$0345,$04e8,$068c,$074d,$08ee,$0d1a,$13a8


/*
 * interrupt vectors for SCC A&B copied to 0x180
 */
sccvect:
       dc.l      sccbempty
       dc.l      0
       dc.l      sccbstat
       dc.l      0
       dc.l      sccbrxint
       dc.l      0
       dc.l      sccbrxint
       dc.l      0
       dc.l      sccaempty
       dc.l      0
       dc.l      sccastat
       dc.l      0
       dc.l      sccarxint
       dc.l      0
       dc.l      sccarxint
       dc.l      0

/*
 * initial EXT_IOREC for device 7 (modem2) & device 9 (modem4)
 */
mdm2table:
        dc.l mdm4ibuf
        dc.w SERIAL_BUFSIZE,0,0,128,192
        dc.l mdm4obuf
        dc.w SERIAL_BUFSIZE,0,0,128,192
        dc.b 8,$ea,0,0,1,0,1,$ff


/*************************************************************************
 * receiver buffer full interrupt handler for IKBD/MIDI ACIA
 *************************************************************************/
/* 306de: 00e03268 */
_int_acia:
        movem.l   d0-d3/a0-a3,-(a7)
intacia1:
        movea.l   (midisys).w,a2
        jsr       (a2)
        movea.l   (ikbdsys).w,a2
        jsr       (a2)
        btst      #4,(MFP_GPIP).w      /* ACIA receiver buffer empty? */
        beq.s     intacia1             /* no, read more bytes */
        move.b    #$BF,(MFP_ISRB).w    /* clear in-service bit */
        movem.l   (a7)+,d0-d3/a0-a3
        rte

_midisys:
        lea.l     (midiiorec).w,a0
        lea.l     (ACIA_MIDI_BASE).w,a1
        movea.l   (vmiderr).w,a2
        bra.s     _aciasys
_ikbdsys:
        lea.l     (ikbdiorec).w,a0
        lea.l     (ACIA_IKBD_BASE).w,a1
        movea.l   (vkbderr).w,a2
_aciasys:
        move.b    (a1),d2
        btst      #7,d2        /* interrupt request? */
        beq.s     aciasys2     /* no, ignore */
        btst      #0,d2        /* receiver data full? */
        beq.s     aciasys1     /* no, skip processing */
        movem.l   d2/a0-a2,-(a7)
        bsr.s     aciaread
        movem.l   (a7)+,d2/a0-a2
aciasys1:
        andi.b    #$20,d2      /* receiver overrun? */
        beq.s     aciasys2     /* no, we are done */
        move.b    ACIA_DATA(a1),d0     /* read data byte */
        jmp       (a2)         /* yes, call error function */
aciasys2:
        rts


/*
 * ==== IKBD stuff ================
 *
 * Packets received from the IKBD are accumulated into the kbdbuf buffer.
 * The packet header (F6 to FF) determines the packet length and the
 * action to be taken once the packet has been received completely.
 * During the reception of a packet, variable ikbdstate contains the
 * action number, and variable kbdlength contains the number of bytes
 * not received yet.
 *
 * action <--whole IKBD packet-->  Comment
 * number    <-given to routine->
 *
 *  1     F6 a1 a2 a3 a4 a5 a6 a7 (miscellaneous, 7 bytes)
 *  2     F7 0b xh xl yh yl       (absolute mouse)
 *  3     F8 dx dy                (relative mouse, no button)
 *  3     F9 dx dy                (relative mouse, button 1)
 *  3     FA dx dy                (relative mouse, button 2)
 *  3     FB dx dy                (relative mouse, both buttons)
 *  4     FC yy MM dd hh mm ss    (date and time)
 *  5     FD j0 j1                (both joysticks)
 *  6     FE bj                   (joystick 0)
 *  7     FF bj                   (joystick 1)
 */

aciaread:
        move.b    ACIA_DATA(a1),d0      /* read data byte */
        cmpa.l    #ikbdiorec,a0
        bne       midiread
        tst.b     (ikbdstate).w         /* inside a multi-byte packet? */
        bne.s     in_packet             /* ikbdstate != 0 => go and add to the packet */
        cmpi.b    #$F6,d0               /* is byte a packet header? */
        bcc.s     begin_packet          /* byte >= 0xf6 => go begin receiving a packet */
        move.l    (kbdvec).w,-(a7)      /* ordinary key byte in d0. jump in vector */
        rts
begin_packet:
        subi.b    #$F6,d0               /* generate table index */
        andi.l    #$000000FF,d0
        lea.l     ikbd_state_table,a3   
        move.b    0(a3,d0.w),(ikbdstate).w /* set ikbd state */
        lea.l     kbd_length_table,a3
        move.b    0(a3,d0.w),(kbdlength).w /* set number expected bytes to follow */
        addi.w    #$00F6,d0             /* reconstruct input byte */
        cmpi.b    #$F8,d0               /* mouse position record? */
        blt.s     begin_p1              /* no */
        cmpi.b    #$FB,d0               /* mouse position record? */
        bgt.s     begin_p1              /* no */
        move.b    d0,(relmbuf).w        /* store first byte with button states */
        rts
begin_p1:
        cmpi.b    #$FD,d0               /* joystick record? */
        blt.s     begin_p2              /* no */
        move.b    d0,(joybuf).w         /* store joystick byte */
begin_p2:
        rts

ikbd_state_table:                     dc.b 1,2,3,3,3,3,4,5,6,7
kbd_length_table:                     dc.b 7,5,2,2,2,2,6,2,1,1

in_packet:
        cmpi.b    #$06,(ikbdstate).w  /* joystick packet? */
        bcc       kbd_joy             /* yes, handle separately */
        lea.l     ikbdbaddr,a2
        moveq.l   #0,d2
        move.b    (ikbdstate).w,d2
        subq.b    #1,d2
        asl.w     #1,d2
        add.b     (ikbdstate).w,d2
        subq.b    #1,d2
        asl.w     #2,d2 /* d2 now (ikbdstate - 1) * 12 */
        movea.l   0(a2,d2.w),a0       /* get start of buffer */
        movea.l   4(a2,d2.w),a1       /* get end of buffer */
        movea.l   8(a2,d2.w),a2       /* get packet handler address */
        movea.l   (a2),a2             /* get packet handler */
        moveq.l   #0,d2
        move.b    (kbdlength).w,d2
        suba.l    d2,a1
        move.b    d0,(a1)             /* store input byte */
        subq.b    #1,(kbdlength).w    /* decrement the number of bytes in the package */
        tst.b     (kbdlength).w       /* packet complete? */
        bne.s     in_pack2            /* no */
in_pack1:
        move.l    a0,-(a7)            
        jsr       (a2)                /* call packet handler */
        addq.w    #4,a7
        clr.b     (ikbdstate).w       /* reset ikbd state */
in_pack2:
        rts

ikbdbaddr:
        dc.l kbdbuf,kbdbuf+7,statvec
        dc.l mousebuf,mousebuf+5,mousevec
        dc.l relmbuf,relmbuf+3,mousevec
        dc.l clockbuf,clockbuf+6,clockvec
        dc.l joybuf,joybuf+2,joyvec

/* handle joystick packet $FE,$FF */
kbd_joy:
        move.l    #joybuf+1,d1
        add.b     (ikbdstate).w,d1              /* ikbdstate reflects joy0 or joy1 state */
        subq.b    #6,d1
        movea.l   d1,a2
        move.b    d0,(a2)
        movea.l   (joyvec).w,a2                 /* get user defined handler */
        lea.l     (joybuf).w,a0
        bra.s     in_pack1                      /* and go call it */

/********************************
 * handler for ordinary keys
 ********************************/
/* 306de: 00e033ea */
_kbdvec:
        move.b    (_shifty).w,d1                /* get current shift state */
        cmpi.b    #$2A,d0                       /* left shift pressed? */
        bne.s     kbdvec1
        bset      #1,d1
        bra       kbdvec10
kbdvec1:
        cmpi.b    #$AA,d0                       /* left shift released? */
        bne.s     kbdvec2
        bclr      #1,d1
        bra       kbdvec10
kbdvec2:
        cmpi.b    #$36,d0                       /* right shift pressed? */
        bne.s     kbdvec3
        bset      #0,d1
        bra.s     kbdvec10
kbdvec3:
        cmpi.b    #$B6,d0                       /* right shift released? */
        bne.s     kbdvec4
        bclr      #0,d1
        bra.s     kbdvec10
kbdvec4:
        cmpi.b    #$1D,d0                       /* control pressed? */
        bne.s     kbdvec5
        bset      #2,d1
        bra.s     kbdvec10
kbdvec5:
        cmpi.b    #$9D,d0                       /* control released? */
        bne.s     kbdvec6
        bclr      #2,d1
        bra.s     kbdvec10
kbdvec6:
        cmpi.b    #$38,d0                       /* ALT pressed? */
        bne.s     kbdvec7
        bset      #3,d1
        bra.s     kbdvec10
kbdvec7:
        cmpi.b    #$B8,d0                       /* ALT released? */
        bne.s     kbdvec8
        bclr      #3,d1
        tst.w     kb_altnum                     /* ascii code via numpad active? */
        bmi.s     kbdvec10
        move.b    d1,(_shifty).w
        move.l    a0,-(a7)
        moveq.l   #0,d1                         /* clear ascii code reg */
        move.w    d1,d0         
        move.b    kb_altnum+1,d0                /* get entered ascii code */
        move.w    #$FFFF,kb_altnum
        bra       kbdvec44
kbdvec8:
        cmpi.b    #$3A,d0                       /* CAPS LOCK pressed? */
        bne.s     kbdvec11
        btst      #0,(_conterm).w               /* should click sound be produced? */
        beq.s     kbdvec9
        movem.l   d0-d2/a0-a2,-(a7)
        movea.l   (_kcl_hook).w,a0
        jsr       (a0)
        movem.l   (a7)+,d0-d2/a0-a2
kbdvec9:
        bchg      #4,d1                         /* toggle CAPS LOCK state */
kbdvec10:
        move.b    d1,(_shifty).w                /* store new shift state */
        rts
kbdvec11:
        btst      #7,d0                         /* was key released? */
        bne.s     kbdvec12
        move.b    d0,(kb_key_last).w            /* save for key repeat */
        move.b    kb_initial,(kb_ticks).w
        move.b    kb_repeat,(kb_delay).w
        bra.s     kbdvec15
kbdvec12:
        move.b    d0,d1
        bclr      #7,d1
        cmp.b     (kb_key_last).w,d1
        bne.s     kbdvec13
        moveq.l   #0,d1
        move.b    d1,(kb_key_last).w
        move.b    d1,(kb_ticks).w
        move.b    d1,(kb_delay).w
kbdvec13:
        cmpi.b    #$C7,d0                       /* was it HOME? */
        beq.s     kbdvec14
        cmpi.b    #$D2,d0                       /* was it INSERT? */
        bne       kbdvec47
kbdvec14:
        btst      #3,(_shifty).w                /* ... and ALT active? */
        beq       kbdvec47
kbdvec15:
        btst      #0,(_conterm).w               /* should click sound be produced? */
        beq.s     kbdvec16
        movem.l   d0-d2/a0-a2,-(a7)
        movea.l   (_kcl_hook).w,a1
        jsr       (a1)
        movem.l   (a7)+,d0-d2/a0-a2
kbdvec16:
        move.l    a0,-(a7)
        moveq.l   #0,d1
        move.b    d0,d1
        movea.l   (_curkeytbl).w,a0
        andi.w    #$007F,d0
        btst      #4,(_shifty).w                /* caps-lock active? */
        beq.s     kbdvec17
        movea.l   (_curkeytbl+8).w,a0           /* use caps-lock table */
kbdvec17:
        btst      #0,(_shifty).w                /* left shift active? */
        bne.s     kbdvec18
        btst      #1,(_shifty).w                /* right shift active? */
        beq.s     kbdvec20
kbdvec18:
        cmpi.b    #$3B,d0                       /* was it a function key? */
        bcs.s     kbdvec19                      /* no */
        cmpi.b    #$44,d0
        bhi.s     kbdvec19                      /* no */
        addi.w    #$0019,d1                     /* add to change to GSX standard */
        moveq.l   #0,d0
        bra       kbdvec44
kbdvec19:
        movea.l   (_curkeytbl+4).w,a0           /* use the shift table */
kbdvec20:
        move.b    0(a0,d0.w),d0                 /* get ascii code from key table */
        btst      #2,(_shifty).w                /* control active? */
        beq.s     kbdvec27
        cmpi.b    #$0D,d0                       /* control-CR, convert to LF */
        bne.s     kbdvec21
        moveq.l   #10,d0
        beq.s     kbdvec24
kbdvec21:
        cmpi.b    #$47,d1                       /* convert CONTROL-home to gsx standard */
        bne.s     kbdvec22
        addi.w    #$0030,d1
        bra       kbdvec44
kbdvec22:
        cmpi.b    #$4B,d1                       /* convert CONTROL-left arrow to gsx standard */
        bne.s     kbdvec23
        moveq.l   #$73,d1
        moveq.l   #0,d0
        bra       kbdvec44
kbdvec23:
        cmpi.b    #$4D,d1                       /* convert CONTROL-right arrow to gsx standard */
        bne.s     kbdvec24
        moveq.l   #$74,d1
        moveq.l   #0,d0
        bra       kbdvec44
kbdvec24:
        cmpi.b    #$32,d0                       /* convert control-shift-down arrow to gsx standard */
        bne.s     kbdvec25
        moveq.l   #0,d0
        bra       kbdvec44
kbdvec25:
        cmpi.b    #$36,d0                       /* convert control-shift-right arrow to gsx standard */
        bne.s     kbdvec26
        moveq.l   #$1e,d0
        bra       kbdvec44
kbdvec26:
        cmpi.b    #$2D,d0                       /* convert control-shift-kp-minus to gsx standard */
        bne.s     kbdvec27
        moveq.l   #$1f,d0
        bra       kbdvec44
kbdvec27:
        btst      #3,(_shifty).w                /* is alt active? */
        beq       kbdvec42
        cmp.b     #$67,d1                       /* keypad key 0 - 9? */
        bcs.s     kbdvec29                      /* no */
        cmp.b     #$70,d1
        bhi.s     kbdvec29                      /* no */
        move.w    kb_altnum,d0
        bpl.s     kbdvec28
        moveq.l   #0,d0
kbdvec28:
        mulu.w    #$000A,d0
        ext.w     d1
        move.b    0(a0,d1.w),d1                 /* get ascii code of keypad key */
        sub.b     #$30,d1
        add.b     d1,d0
        move.w    d0,kb_altnum                  /* store as new number code */
        movea.l   (a7)+,a0
        rts
kbdvec29:

/*
 * Country-dependent key translations
 */
/* 306any: 00e035e0 */
/* 205any: 00e028a0 */
#if OS_COUNTRY == CTRY_US
/* nothing to do */
#endif

#if OS_COUNTRY == CTRY_DE
        cmpi.b    #$1A,d1                       /* convert ALT-udiaresis to @ or backslash */
        bne.s     kbdde1
        move.b    #$40,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$5C,d0
        bra       kbdvec42
kbdde1:
        cmpi.b    #$27,d1                       /* convert ALT-odiaresis to [ or { */
        bne.s     kbdde2
        move.b    #$5B,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7B,d0
        bra       kbdvec42
kbdde2:
        cmpi.b    #$28,d1                       /* convert ALT-adiaresis to ] or } */
        bne.s     kbdde3
        move.b    #$5D,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7D,d0
        bra       kbdvec42
kbdde3:
#endif

#if OS_COUNTRY == CTRY_FR
        cmpi.b    #$1A,d1                       /* convert ALT-? to [ or { */
        bne.s     kbdfr1
        move.b    #$5B,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7B,d0
        bra       kbdvec42
kbdfr1:
        cmpi.b    #$1B,d1                       /* convert ALT-? to ] or } */
        bne.s     kbdfr2
        move.b    #$5D,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7D,d0
        bra       kbdvec42
kbdfr2:
        cmpi.b    #$28,d1                       /* convert ALT-? to backslash or 0 */
        bne.s     kbdfr3
        move.b    #$5C,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$00,d0
        bra       kbdvec42
kbdfr3:
        cmpi.b    #$2B,d1                       /* convert ALT-? to @ or tilde */
        bne.s     kbdfr4
        move.b    #$40,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7E,d0
        bra       kbdvec42
kbdfr4:
#endif

#if OS_COUNTRY == CTRY_UK
/* nothing to do */
#endif

#if OS_COUNTRY == CTRY_ES
        cmpi.b    #$1A,d1                       /* convert ALT-? to [ or { */
        bne.s     kbdes1
        move.b    #$5B,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7B,d0
        bra       kbdvec42
kbdes1:
        cmpi.b    #$1B,d1                       /* convert ALT-? to ] or } */
        bne.s     kbdes2
        move.b    #$5D,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7D,d0
        bra       kbdvec42
kbdes2:
        cmpi.b    #$2B,d1                       /* convert ALT-? to # or @ */
        bne.s     kbdes3
        move.b    #$23,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$40,d0
        bra       kbdvec42
kbdes3:
        cmpi.b    #$28,d1                       /* convert ALT-? to # or @ */
        bne.s     kbdes4
        move.b    #$81,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$00,d0
        bra       kbdvec42
kbdes4:
        cmpi.b    #$27,d1                       /* convert ALT-? to # or @ */
        bne.s     kbdes5
        move.b    #$00,d0
        bra       kbdvec42
kbdes5:
#endif

#if OS_COUNTRY == CTRY_IT
        cmpi.b    #$1A,d1                       /* convert ALT-? to [ or { */
        bne.s     kbdit1
        move.b    #$5B,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7B,d0
        bra       kbdvec42
kbdit1:
        cmpi.b    #$1B,d1                       /* convert ALT-? to ] or } */
        bne.s     kbdit2
        move.b    #$5D,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7D,d0
        bra       kbdvec42
kbdit2:
        cmpi.b    #$2B,d1                       /* convert ALT-? to degree or tilde */
        bne.s     kbdit3
        move.b    #$F8,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7E,d0
        bra       kbdvec42
kbdit3:
        cmpi.b    #$60,d1
        bne.s     kbdit4
        move.b    #$60,d0
        bra       kbdvec42
kbdit4:
#endif

#if OS_COUNTRY == CTRY_SV
        cmpi.b    #$1A,d1                       /* convert ALT-? to [ or { */
        bne.s     kbdsv1
        move.b    #$5B,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7B,d0
        bra       kbdvec42
kbdsv1:
        cmpi.b    #$1B,d1                       /* convert ALT-? to ] or } */
        bne.s     kbdsv2
        move.b    #$5D,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7D,d0
        bra       kbdvec42
kbdsv2:
        cmpi.b    #$28,d1                       /* convert ALT-? to grave accent or tilde */
        bne.s     kbdsv3
        move.b    #$60,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7e,d0
        bra       kbdvec42
kbdsv3:
        cmpi.b    #$2B,d1                       /* convert ALT-? to ^ or @ */
        bne.s     kbdsv4
        move.b    #$5E,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$40,d0
        bra       kbdvec42
kbdsv4:
#endif

#if OS_COUNTRY == CTRY_PL
        cmpi.b    #$1A,d1                       /* convert ALT-udiaresis to @ or backslash */
        bne.s     kbdpl1
        move.b    #$40,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$5C,d0
        bra       kbdvec42
kbdpl1:
        cmpi.b    #$27,d1                       /* convert ALT-odiaresis to [ or { */
        bne.s     kbdpl2
        move.b    #$5B,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7B,d0
        bra       kbdvec42
kbdpl2:
        cmpi.b    #$28,d1                       /* convert ALT-adiaresis to ] or } */
        bne.s     kbdpl3
        move.b    #$5D,d0
        move.b    (_shifty).w,d2
        andi.b    #$03,d2
        beq       kbdvec42
        move.b    #$7D,d0
        bra       kbdvec42
kbdpl3:
#endif

        cmpi.b    #$62,d1                       /* is it an "alt help" signal to dump the screen? */
        bne.s     kbdvec33
        addq.w    #1,(_dumpflg).w
        movea.l   (a7)+,a0
        bra       kbdvec47
kbdvec33:
        lea.l     mousekey,a2                   /* get pointer to first alt. mouse scancode table */
        moveq.l   #3,d2
kbdvec34:
        cmp.b     0(a2,d2.w),d1
        beq       keymouse
        dbf       d2,kbdvec34
        cmpi.b    #$48,d1                       /* is it an up arrow? */
        bne.s     kbdvec35
        move.b    #$00,d1
        move.b    #-$08,d2
        move.b    (_shifty).w,d0
        andi.b    #$03,d0
        beq       keymous4
        move.b    #-1,d2
        bra       keymous4
kbdvec35:
        cmpi.b    #$4B,d1                       /* is it an left arrow? */
        bne.s     kbdvec36
        move.b    #$00,d2
        move.b    #-$08,d1
        move.b    (_shifty).w,d0
        andi.b    #$03,d0
        beq       keymous4
        move.b    #-1,d1
        bra       keymous4
kbdvec36:
        cmpi.b    #$4D,d1                       /* is it an right arrow? */
        bne.s     kbdvec37
        move.b    #$08,d1
        move.b    #$00,d2
        move.b    (_shifty).w,d0
        andi.b    #$03,d0
        beq       keymous4
        move.b    #$01,d1
        bra       keymous4
kbdvec37:
        cmpi.b    #$50,d1                       /* is it an down arrow? */
        bne.s     kbdvec38
        move.b    #$00,d1
        move.b    #$08,d2
        move.b    (_shifty).w,d0
        andi.b    #$03,d0
        beq       keymous4
        move.b    #$01,d2
        bra       keymous4
kbdvec38:
        btst      #2,(_shifty).w                /* is control active? */
        bne.s     kbdvec43                      /* yes, skip */
        cmpi.b    #$02,d1                       /* is scancode between '1' key and '=' key? */
        bcs.s     kbdvec39
        cmpi.b    #$0D,d1
        bhi.s     kbdvec39
        addi.b    #$76,d1
        bra.s     kbdvec40
kbdvec39:
        cmpi.b    #$41,d0                       /* is it an ascii character 'A' -'Z'? */
        bcs.s     kbdvec41
        cmpi.b    #$5A,d0
        bhi.s     kbdvec41
kbdvec40:
        moveq.l   #0,d0
        bra.s     kbdvec44
kbdvec41:
        cmpi.b    #$61,d0                       /* is it an ascii character 'a' -'z'? */
        bcs.s     kbdvec44
        cmpi.b    #$7A,d0
        bhi.s     kbdvec44
        bra.s     kbdvec40
kbdvec42:
        btst      #2,(_shifty).w
        beq.s     kbdvec44
kbdvec43:
        andi.w    #$001F,d0
kbdvec44:
        asl.w     #8,d1                         /* put scancode to high byte */
        add.w     d1,d0                         /* insert ascii code */
        movea.l   (a7)+,a0
        move.w    tail(a0),d1
        addq.w    #4,d1
        cmp.w     size(a0),d1                   /* room for 4 bytes? */
        bcs.s     kbdvec45
        moveq.l   #0,d1
kbdvec45:
        cmp.w     head(a0),d1                   /* keyboard buffer full? */
        beq.s     kbdvec47                      /* yes, drop it */
        movea.l   buf(a0),a2
        swap      d0                            /* put shift state to high word */
        clr.w     d0
        move.b    (_shifty).w,d0
        swap      d0
        lsl.l     #8,d0
        lsr.w     #8,d0
        move.l    d0,d2                         /* save keycode for tests */
        bclr      #28,d2                        /* clear capslock state */
        swap      d2
        cmp.w     #$0C53,d2                     /* control+alt+delete? */
        beq       _main                         /* yes, reset */
        cmp.w     #$0D53,d2                     /* control+alt+leftshift-delete? */
        beq       coldboot
        btst      #3,(_conterm).w
        bne.s     kbdvec46
        andi.l    #$00FFFFFF,d0
kbdvec46:
        and.l     #$0000FFFF,d1
        move.l    d0,0(a2,d1.l)
        move.w    d1,tail(a0)
kbdvec47:
        rts

/*
 * default function for system variable kcl_hook
 */
/* 306de: 00e0378a */
_do_keyclick:
        move.l    #_clicksnd,(_sndtable).w
        move.b    #$00,(_snddelay).w
        rts

/* 306de: 00e0379a */
midiread:
        movea.l   (midivec).w,a2
        jmp       (a2)

/* 306de: 00e037a0 */
_midivec:
        move.w    tail(a0),d1
        addq.w    #1,d1
        cmp.w     size(a0),d1
        bcs.s     midivec1
        moveq.l   #0,d1
midivec1:
        cmp.w     head(a0),d1
        beq.s     midivec2
        movea.l   buf(a0),a2
        and.l     #$0000FFFF,d1
        move.b    d0,0(a2,d1.l)
        move.w    d1,tail(a0)
midivec2:
        rts


/* 306de: 00e037c6 */
keymouse:
        moveq.l   #5,d3
        btst      #4,d1
        beq.s     keymous1
        moveq.l   #6,d3
keymous1:
        btst      #7,d1
        beq.s     keymous2
        bclr      d3,(_shifty).w
        bra.s     keymous3
keymous2:
        bset      d3,(_shifty).w
keymous3:
        moveq.l   #0,d1
        moveq.l   #0,d2
keymous4:
        lea.l     (buttonbuf).w,a0
        movea.l   (mousevec).w,a2
        clr.l     d0
        move.b    (_shifty).w,d0
        lsr.b     #5,d0
        addi.b    #$F8,d0
        move.b    d0,(a0)
        move.b    d1,1(a0)
        move.b    d2,2(a0)
        jsr       (a2)
        movea.l   (a7)+,a0
        rts

/* scancode that send mouse packets */
mousekey:  dc.b      $47,$c7,$52,$d2


coldboot:
        move.w    #$2700,sr                 /* disable all IRQSR */
        
#if STBOOK
            move.l    ($0004).w,(_buserror).w /*a bus error triggers a reset */
            movea.w   #$000c,a0         /* start erasing from $c on */
            moveq     #0,d0
            move.l    #$3fffc,d1                /* erase 1MB */
coldboo2: move.l    d0,(a0)+
            dbra      d1,coldboo2
            movea.l   ($0004).w,a0              /* jump into the ROM to reset the system */
            jmp       (a0)

#else
#if TOSVERSION < 0x300
			move.w    #5,d0
			lea       coldboor(pc),a0
			movea.w   #$000c,a1
coldboo1:	move.l    (a0)+,(a1)+				/* copy erase routine to $000c... */
			dbra      d0,coldboo1
			jmp       ($000c).w

coldboor:	move.l    ($0004).w,(_buserror).w
			lea       coldbooe(pc),a0
			moveq     #0,d0
coldbool:	move.l    d0,(a0)+
			move.l    d0,(a0)+
			move.l    d0,(a0)+
			move.l    d0,(a0)+
			bra.s     coldbool
coldbooe:nop

#else
        move.l    #$00000808,d0
        D0_CACR
        moveq.l   #0,d0
        D0_VBR
        TO_TC(zero2)   /* disable PMMU and address translation */
        TO_TTR0(zero2)
        TO_TTR1(zero2)
/* copy erase routine to low memory */
        move.w    #(coldbend-coldboo2)/4-1,d0
        lea.l     coldboo2(pc),a0
        movea.w   #$00c,a1
coldboo1:
        move.l    (a0)+,(a1)+
        dbf       d0,coldboo1
        jmp       ($0000000C).w
/* clear ST memory until buserror */
coldboo2:
        lea.l     coldboo4(pc),a0
        move.l    a0,(_buserror).w
        lea.l     coldboo6(pc),a0
        moveq.l   #0,d0
coldboo3:
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        bra.s     coldboo3
/* clear FASTRAM until buserror */
coldboo4:
        move.l    ($00000004).w,(_buserror).w
        lea.l     FASTRAMBASE,a0
coldboo5:
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        bra.s     coldboo5
coldboo6:
        nop
coldbend:  ds.b 0

        data
zero2:  dc.l 0
		text
#endif
#endif


/*
 * XBIOS #28 - Giaccess - Permit access to the registers of the GI sound chip. 
 */
/* 306de: 00e0387e */
_giaccess:
        move.w    4(a7),d0
        move.w    6(a7),d1
giaccess:
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        movem.l   d1-d2/a0,-(a7)
        lea.l     ($FFFF8800).w,a0
        move.b    d1,d2
        andi.b    #$0F,d1
        move.b    d1,(a0)
        asl.b     #1,d2
        bcc.s     giacces1
        move.b    d0,2(a0)
giacces1:
        moveq.l   #0,d0
        move.b    (a0),d0
        movem.l   (a7)+,d1-d2/a0
        move.w    (a7)+,sr
        rts


dtron:
        move.b    #$EF,d2
        bra.s     offgibit

/*
 * XBIOS #30 - Ongibit - Set individual bits in the port A register of the GI sound chip. 
 */
/* 306de: 00e038b6 */
_ongibit:
        moveq.l   #0,d2
        move.w    4(a7),d2
ongibit:
        movem.l   d0-d2,-(a7)
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        moveq.l   #14,d1
        move.l    d2,-(a7)
        bsr.s     giaccess
        move.l    (a7)+,d2
        or.b      d2,d0
        move.b    #$8E,d1
        bsr.s     giaccess
        move.w    (a7)+,sr
        movem.l   (a7)+,d0-d2
        rts


/*
 * XBIOS #29 - Offgibit - Clear individual bits in the port A register of the GI sound chip. 
 */
/* 306de: 00e038de */
_offgibit:
        moveq.l   #0,d2
        move.w    4(a7),d2
offgibit:
        movem.l   d0-d2,-(a7)
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        moveq.l   #14,d1
        move.l    d2,-(a7)
        bsr.s     giaccess
        move.l    (a7)+,d2
        and.b     d2,d0
        move.b    #$8E,d1
        bsr.s     giaccess
        move.w    (a7)+,sr
        movem.l   (a7)+,d0-d2
        rts


/*
 * XBIOS #0 - Initmouse - Initializes the mouse handler. 
 */
/* 306de: 00e03906 */
_initmouse:
        tst.w     4(a7)
        beq.s     initmou1
        move.l    10(a7),(mousevec).w
        movea.l   6(a7),a3
        cmpi.w    #$0001,4(a7)
        beq.s     initmou2
        cmpi.w    #$0002,4(a7)
        beq.s     initmou3
        cmpi.w    #$0004,4(a7)
        beq.s     initmou4
        moveq.l   #0,d0
        rts
initmou1:
        moveq.l   #18,d1
        bsr       ikbd_writeb
        move.l    #imrts,(mousevec).w
        bra.s     initmou5
initmou2:
        lea.l     (initmbuf).w,a2
        move.b    #$08,(a2)+
        move.b    #$0B,(a2)+
        bsr.s     setmouse
        moveq.l   #6,d3
        lea.l     (initmbuf).w,a2
        bsr       ikbdws1
        bra.s     initmou5
initmou3:
        lea.l     (initmbuf).w,a2
        move.b    #$09,(a2)+
        move.b    4(a3),(a2)+
        move.b    5(a3),(a2)+
        move.b    6(a3),(a2)+
        move.b    7(a3),(a2)+
        move.b    #$0C,(a2)+
        bsr.s     setmouse
        move.b    #$0E,(a2)+
        move.b    #$00,(a2)+
        move.b    8(a3),(a2)+
        move.b    9(a3),(a2)+
        move.b    10(a3),(a2)+
        move.b    11(a3),(a2)+
        moveq.l   #16,d3
        lea.l     (initmbuf).w,a2
        bsr       ikbdws1
        bra.s     initmou5
initmou4:
        lea.l     (initmbuf).w,a2
        move.b    #$0A,(a2)+
        bsr.s     setmouse
        moveq.l   #5,d3
        lea.l     (initmbuf).w,a2
        bsr       ikbdws1
initmou5:
        moveq.l   #-1,d0
        rts

setmouse:
        move.b    2(a3),(a2)+
        move.b    3(a3),(a2)+
        moveq.l   #16,d1
        sub.b     (a3),d1
        move.b    d1,(a2)+
        move.b    #$07,(a2)+
        move.b    1(a3),(a2)+
        rts

/*
 * XBIOS #31 - Xbtimer - Initialize the timer in the MFP chip.
 */
/* 306de: 00e039ce */
_xbtimer:
        moveq.l   #0,d0
        moveq.l   #0,d1
        moveq.l   #0,d2
        move.w    4(a7),d0
        move.w    6(a7),d1
        move.w    8(a7),d2
        bsr       settimer
        tst.l     10(a7)
        bmi.s     imrts
        movea.l   10(a7),a2
        moveq.l   #0,d1
        lea.l     xbtimtab(pc),a1
        andi.l    #$000000FF,d0
        move.b    0(a1,d0.w),d0
        bsr       initint
imrts:
        rts

xbtimtab:  dc.b 13,8,5,4


/*
 * XBIOS #16 - Keytbl - Define the mapping table for the keyboard scancodes to key-codes.
 */
/* 306de: 00e03a08 */
_keytbl:
        tst.l     4(a7)
        bmi.s     _keytbl1
        move.l    4(a7),(_curkeytbl).w
_keytbl1:
        tst.l     8(a7)
        bmi.s     _keytbl2
        move.l    8(a7),(_curkeytbl+4).w
_keytbl2:
        tst.l     12(a7)
        bmi.s     _keytbl3
        move.l    12(a7),(_curkeytbl+8).w
_keytbl3:
        move.l    #_curkeytbl,d0
        rts


/*
 * XBIOS #24 - Bioskeys - Reinitialize keyboard.
 */
/* 306de: 00e03a08 */
_bioskeys:
        move.l    #_keytblnorm,(_curkeytbl).w
        move.l    #_keytblshift,(_curkeytblxx+4).w
        move.l    #_keytblcaps,(_curkeytbl+8).w
        rts

/*
 * XBIOS #32 - Dosound - Program the sound generator (PSG). 
 */
/* 306de: 00e03a4e */
_dosound:
        move.l    (_sndtable).w,d0
        move.l    4(a7),d1
        bmi.s     dosound1
        move.l    d1,(_sndtable).w
        clr.b     (_snddelay).w
dosound1:
        rts


/*
 * XBIOS #33 - Setprt - Initialize the printer.
 */
/* 306de: 00e03a62 */
_setprt:
        move.w    (_prtconfig).w,d0
        tst.w     4(a7)
        bmi.s     setprt1
        move.w    4(a7),(_prtconfig).w
setprt1:
        rts


/*
 * XBIOS #35 - Kbrate - Set the auto-repeat rate of the keyboard.
 */
/* 306de: 00e03a74 */
_kbrate:
        move.w    (kb_initial).w,d0
        tst.w     4(a7)
        bmi.s     kbrate1
        move.w    4(a7),d1
        move.b    d1,(kb_initial).w
        tst.w     6(a7)
        bmi.s     kbrate1
        move.w    6(a7),d1
        move.b    d1,(kb_repeat).w
kbrate1:
        rts


/*
 * XBIOS #34 - Kbdvbase - Return a pointer to the IKBD (intelligent keyboard), and MIDI interrupt vectors.
 */
/* 306de: 00e03a96 */
_kbdvbase:
        move.l    #_kbdvecs+4,d0
        rts

/***********************
 * Timer C interrupt
 ***********************/
/* 306de: 00e03a9e */
timercint:
        addq.l    #1,_hz_200
/*        rol.b     _tim_c_sieve */
/* workaround for as68 which does not handle rol.b ea */
        dc.w $e7f9
        dc.l _tim_c_sieve
        bpl.s     timerci3
        movem.l   d0-d7/a0-a6,-(a7)
        bsr.s     soundirq
        btst      #1,(_conterm).w
        beq.s     timerci2
        tst.b     (kb_key_last).w
        beq.s     timerci2
        tst.b     (kb_ticks).w
        beq.s     timerci1
        subq.b    #1,(kb_ticks).w
        bne.s     timerci2
timerci1:
        subq.b    #1,(kb_delay).w
        bne.s     timerci2
        move.b    (kb_repeat).w,(kb_delay).w
        move.b    (kb_key_last).w,d0
        lea.l     (ikbdiorec).w,a0
        bsr       kbdvec15
timerci2:
        move.w    (_timer_ms).w,-(a7)
        movea.l   (_etv_timer).w,a0
        jsr       (a0)
        addq.w    #2,a7
        movem.l   (a7)+,d0-d7/a0-a6
timerci3:
        move.b    #$DF,($FFFFFA11).w
        rte

/* Sound interrupt routine */
/* Called from timer C irq */
soundirq:
        movem.l   d0-d1/a0,-(a7)
        move.l    (_sndtable).w,d0
        beq.s     soundir7
        movea.l   d0,a0
        move.b    (_snddelay).w,d0
        beq.s     soundir1
        subq.b    #1,d0
        move.b    d0,(_snddelay).w
        bra.s     soundir7
soundir1:
        move.b    (a0)+,d0
        bmi.s     soundir3
        move.b    d0,($FFFF8800).w
        cmpi.b    #$07,d0
        bne.s     soundir2
        move.b    (a0)+,d1
        andi.b    #$3F,d1
        move.b    ($FFFF8800).w,d0
        andi.b    #$C0,d0
        or.b      d1,d0
        move.b    d0,($FFFF8802).w
        bra.s     soundir1
soundir2
        move.b    (a0)+,($FFFF8802).w
        bra.s     soundir1
soundir3:
        addq.b    #1,d0
        bpl.s     soundir5
        cmpi.b    #$81,d0
        bne.s     soundir4
        move.b    (a0)+,(_sndtmp).w
        bra.s     soundir1
soundir4:
        cmpi.b    #$82,d0
        bne.s     soundir5
        move.b    (a0)+,($FFFF8800).w
        move.b    (a0)+,d0
        add.b     d0,(_sndtmp).w
        move.b    (a0)+,d0
        move.b    (_sndtmp).w,($FFFF8802).w
        cmp.b     (_sndtmp).w,d0
        beq.s     soundir6
        subq.w    #4,a0
        bra.s     soundir6
soundir5:
        move.b    (a0)+,(_snddelay).w
        bne.s     soundir6
        movea.w   #$0000,a0
soundir6:
        move.l    a0,(_sndtable).w
soundir7:
        movem.l   (a7)+,d0-d1/a0
        rts

/*********************************************************
 * VT52 emulator callback for the bell sound
 *********************************************************/
/* 306de: 00e03b86 */
ringbell:
        btst      #2,(_conterm).w
        beq.s     soundir8
        movea.l   _bell_hook,a0
        jsr       (a0)
        rts

/*
 * default function for system variable bell_hook
 */
/* 306de: 00e03b98 */
_do_bell:
        move.l    #_bellsnd,(_sndtable).w
        move.b    #$00,(_snddelay).w
soundir8:
        rts


#include "floppy.inc"


		bss
		
		xdef gem_env
		comm gem_env,12
		
		xdef save_beg
		comm save_beg,save_siz
		
		xdef oshdr
		comm oshdr,70 /* 6+48+16 */

		xdef cookies
cooksize equ NCOOKIES*2*4
		comm cookies,cooksize

		xdef _has_dmasound
		comm _has_dmasound,1
		
		xdef STEFlag
		comm STEFlag,1
		
		xdef privinst
		comm privinst,8

		xdef _pbdef
		comm _pbdef,30
		
		xdef midiiorec
		comm midiiorec,14
		xdef midibuf
		comm midibuf,SERIAL_BUFSIZE
		
		xdef ikbdiorec
		comm ikbdiorec,14
		xdef ikbdbuf
		comm ikbdbuf,128
		
		xdef _kbdvecs
		comm _kbdvecs,42

		xdef bcmap_root
		comm bcmap_root,16

		xdef bcmap_devs
		comm bcmap_devs,96
