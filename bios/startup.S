/*
 * startup.S - TOS startup module
 *
 * Also contains most of the BIOS functions.
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */



#include "config.h"
#include "asmdefs.h"
#include "header.h"
#include "tosvars.inc"
#include "biosdefs.h"


/* ==== Defines ============================================================== */
cart_base  equ   $00fa0000 /* Base of ROM port cartridge */
GEM_MUPB_MAGIC equ $87654321

#define RR_MAGIC    $12123456
#define RR_CHKSUM   $5678

nlevels  equ 5
save_siz equ nlevels*46+8

/* ==== Definitions ========================================================== */

        xdef  _os_entry
        xdef  _run_cartridge_applications

/* ==== References =========================================================== */

/* BIOS C part */
/*        xref _biosmain     / * BIOS C entry point */

/* Overall memory map */
/*       xref __end         / * end of bss + comm sections (ld generated) */
/*       xref __etext       / * end of text section */
/*       xref __edata       / * end of data section */
        xref _endvdibss    /* end of VDI BSS */
        xref _endgembss    /* end of GEM BSS */
        xref __ebss        /* end of BSS */

		xref _supstk


/* ==== hardware addresses =================================================== */

scu_gp1   equ $FFFF8E09
memconf   equ $FFFF8001
#if STBOOK
tt_mcu    equ $00D00000
#else
tt_mcu    equ $FFD00000
#endif
v_syncmod equ $FFFF820A
sndmactl  equ $FFFF8900
mwmask    equ $FFFF8924
mwdata    equ $FFFF8922

v_bas_h   equ $FFFF8201
v_bas_m   equ $FFFF8203
v_bas_l   equ $FFFF820d

vme_mask  equ $FFFF8E0D
sys_mask  equ $FFFF8E01

palette   equ $FFFF8240
v_shf_mod equ $FFFF8260
shift_tt  equ $FFFF8262

spshift   equ $FFFF8266
sppalette equ $FFFF9800

STConfig  equ $FFFF9200

FPStat    equ $FFFFFA40

LcdPControl equ $FFFF827F


STLOW     equ 0
STMED     equ 1
STHIGH    equ 2
FALCONREZ equ 3
TTMED     equ 4
TTHIGH    equ 6
TTLOW     equ 7

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
        xref  _run

_os_entry:
    bra.s   _main       /* os_entry, branch to _main */
os_version:
#if STBOOK
    dc.w    $0206       /* wtf? why 2.06? */
#else
    dc.w    ATOSVERSION  /* os_version, TOS version */
#endif
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
	/* hack for polish version: apparently language code not patched */
    dc.w   (CTRY_UK<<1)+OS_PAL
#else
#if (OS_COUNTRY == CTRY_NL) & (TOSVERSION == 0x104)
	/* hack for dutch version: apparently wrong language code used */
    dc.w   (CTRY_SA<<1)+OS_PAL
#else
    dc.w   (OS_COUNTRY<<1)+OS_PAL
#endif
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

	xdef _main
_main:
/* disable the interrupts */
/* this is useless on cold boot, */
/* but it matters for software reset */
        move    #$2700,sr

/* special TT reset */
#if ((TOSVERSION >= 0x306) & (TOSVERSION < 0x400)) & !TP_50
        move.w #$0100,(DMA_CTRL).w /* reset ACSI chip... */
        move.w #$0000,(DMA_CTRL).w /* ...leave in read state. */
#endif
#if TOSVERSION >= 0x400
		move.w ($FFFF8006).w,d0 /* read to auto-config mem cntrlr */
#endif
#if TP_01 & !TP_50 /* KILL_RESET & !PAK */
		nop
#else
        reset                  /* reset all hardware */
#endif

#if TOSVERSION >= 0x400
		move.w ($FFFF8006).w,d0 /* read to auto-config mem cntrlr */
		move.w #7,($FFFF8940).w /* Added to fix problem in Sparrow hardware. GPIO has no pullup/down resist. */
#endif

/*
 * Check for diagnostic cartridge
 */
        cmp.l   #$fa52235f,cart_base    /* magic - is cartridge present? */
        bne.s   nodiag                  /* no -> go on */
        lea     nodiag(pc),a6           /* save return address */
        jmp     cart_base+4             /* execute diagnostic cartridge */
nodiag:

#if TP_50 /* PAK */
/*
 * Stack einrichten, um die Prozessorunabh„ngigkeiten in den Griff zu bekommen.
 * Aužerdem muž die MCU initialisiert werden, damit der Speicher berhaupt
 * benutzbar ist. Falls das TOS im ST-RAM ist, wird das jedoch unterlassen, da
 * es nur mit 4MB-Rechner funktionieren wrde und das RAM sowieso initialisiert
 * ist.
 *
 * Falls wir in Wirklichkeit einen Warmstart machen, so fhrt das zu keinen
 * Problemen, da kurz sp„ter bei gltigen memvalid, memval2 und memval3 die
 * MCU noch einmal mit dem Wert aus memcntrl ($424) gefttert wird.
 *
 * Dazu den Zugriff auf DMA entfernt und zuerst den Reset gemacht.
 */
		moveq.l   #1,d7
		moveq.l   #10,d0
		lea.l     memcret3(pc),a6
		bra       memchk
memcret3:
		bne       noconf
		moveq.l   #0,d7
		move.b    (memctrl).w,d0
noconf:
		move.b    d0,(memconf).w
		lea       $7fc,sp
#endif

/*
 * disable the instruction/data caches
 */
#if (TOSVERSION >= 0x300) | TP_50
        data
zero:
	    dc.l 0
		text

#if TP_50 /* PAK */
		lea     no020(pc),a0
		move.l  a0,(_illinst).w
#endif
        move.l  #$00000808,d0     /* clear & disable instruction/data caches on */
        D0_CACR              /*  68020-60 (even though bit usage differs!) */

        moveq   #0,d0
        D0_VBR           /* initialise VBR to zero */
no020:

/*
 * disable the MMU
 */
#if TP_50 /* PAK */
/*
 * Prozessorunabh„ngiges 3.06.
 * Initialisierung der 68030 MMU nach Reset.
 */
		lea    nommu(pc),a0
		move.l a0,(_illinst).w
		lea    nofpu1(pc),a0
		move.l a0,(_lineftrap).w
		move.l #zero,a0
		TO_TC_A0
		TO_TTR0_A0
		TO_TTR1_A0
nommu:
        FRESTORE(zero)
nofpu1:
		tst.l     d7
		bne.s     noreset
		moveq.l   #0,d7
#else
#if TP_60 /* NO_MMU_RESET */
		lea    _main(pc),a1
		cmpa.l #$e00000,a1
		bcs    nommur
#endif
        TO_TC(zero)          /*  we're done if we get a lineF exception ... */
nommur:
        TO_TTR0(zero)        /* first we initialise the TTRs (ACRs on a 68ec030) */
        TO_TTR1(zero)
#endif

#if !TP_50 /* PAK */
/*
 * c't 9/92:
 * SCU: General-Purpose-Register checken, ob es ein Power-On war.
 * Da wir das Register nicht haben, mssen wir immer die Power-On-Checks
 * durchfhren.
 */
#if (TOSVERSION >= 0x300) & (TOSVERSION < 0x400)
#if (TOSVERSION >= 0x306)
        FRESTORE(zero)
#endif
        btst      #0,(scu_gp1).w			/* memconfig valid? */
        beq.s     noreset
#endif
#endif
#endif

#if TOSVERSION >= 0x400
		move.w    #$0020,($FFFF828C).w
		move.w    #$0010,($FFFF8282).w
		ori.b     #$25,($FFFF8007).w
		btst      #6,($FFFF8007).w
		beq.s     noreset
#endif

#if !TP_50 /* for PAK, already done above */
        lea       memcret(pc),a6
        bra       memchk
memcret:
        bne.s      noreset
        move.b    (memctrl).w,(memconf).w  /* initialize memory controller */
#endif

resetvec:
/*
 * Check, if we should jump to a reset vector after warmboot
 * The high byte of resvector must be zero,
 * it must be even, and cannot be entirely zero.
 */

#if STBOOK
			move.w    (STConfig).w,d0           /* Configuration on ST Book */
            cmp.b     #$fc,d0                   /* power pressed while the ST Book is closed? */
            beq.s     reset2b                   /* yes => do not execute reset vector */
            move.w    tt_mcu+4,d0               /* ST Book: ??? */
            and.b     #6,d0                     /* check bit 1 & 2 */
            bne.s     reset2c
reset2b:    clr.l     resvalid
reset2c:
#endif

        cmpi.l   #$31415926,(resvalid).w  /* Jump to resetvector? */
        bne.s    noreset                /* No --> noreset */
        move.l   (resvector).w,d0       /* Yes: old resvec to d0 */
#if (TOSVERSION < 0x300) & !TP_50
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

        lea.l     (psgsel).w,a0
        move.b    #$07,(a0)  /* select mixer control */
        move.b    #$C0,2(a0) /* Port A+B = output */
        move.b    #$0E,(a0)  /* select Port A */
#if TP_58 & (TP_59 != 0) /* NEWVIDEO & SWITCH */
        move.b    #((1<<TP_59)+7),2(a0) /* deselect floppy drives */
#else
        move.b    #7,2(a0) /* deselect floppy drives */
#endif

#if TP_50
		bsr       VDOSkip1
#else
#if TOSVERSION < 0x400
#if TOSVERSION >= 0x300
#if TOSVERSION >= 0x306
#if TP_02 /* COLOR_60HZ */
        move.b    #0,(v_syncmod).w /* set video sync */
#else
        move.b    #1,(v_syncmod).w /* set video sync */
#endif
#else
        clr.w     (sndmactl).w
#endif
#else
        btst      #0,_os_conf+1             /* check bit: configured for 50hz? */
        beq.s     notpal                    /* (nope -- we're good ol' NTSC) */
        lea       ret_1b(pc),a6
        bra       waitvbl                   /* a short delay for PAL */
ret_1b:
#if TP_02 /* COLOR_60HZ */
        move.b    #0,(v_syncmod).w          /* yes -- twiddle to 60hz */
#else
        move.b    #2,(v_syncmod).w          /* yes -- twiddle to 50hz */
#endif
notpal:
#if STBOOK
		move.b    #STHIGH,(v_shf_mod).w     /* Switch to 640x400x1 */
        move.b    #$80,(LcdPControl).w      /* LCD display on */
        tst.w     tt_mcu                    /* ST Book: ??? */
#endif
#endif
#endif
#endif

/*
 * next, set up the palette(s)
 */

        lea.l     (palette).w,a1
#if TOSVERSION >= 0x400
        moveq     #16-1,d0
#else
        move.w    #16-1,d0
#endif
        lea.l     dflt_pal(pc),a0
initpal:
        move.w    (a0)+,(a1)+
        dbf       d0,initpal

#if TOSVERSION >= 0x400
		move.b    #$00,(spshift).w
		lea.l     (sppalette).w,a1
		moveq.l   #15,d0
initpal2:
		move.l    (a0)+,(a1)+
		dbf       d0,initpal2
		move.b    #STLOW,(v_shf_mod).w
#endif

/* set Video screen memory position to 0x10000 */
        move.b    #$01,(v_bas_h).w
        clr.b     (v_bas_m).w

#if TP_03 /* RAMRESET */
#include "../tospatch/ramres.S"
#else

#if (TOSVERSION >= 0x300) & (TOSVERSION < 0x400) & !TP_50
        btst      #0,(scu_gp1).w
        beq.s     domemchk
#endif

#if TOSVERSION < 0x400

        move.b    (memctrl).w,d6                /* d6 = memory controller configuration */
#if (TOSVERSION < 0x300) | TP_50
        move.b    d6,(memconf).w                /* setup memory controller configuration register */
#endif

        move.l    (_phystop).w,d5               /* d5 -> (possible) top of physical mem */
        lea       memcret2(pc),a6               /* load return address */
        bra       memchk                        /* check if the memory configuration is valid */
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
            move.l    d0,$300010
            move.l    d1,$300014
            cmp.l     $300010,d0                /* test if 4MB is actually installed */
            bne.s     ret_2a
            cmp.l     $300014,d1
            beq.s     ret_2b
ret_2a:     move.l    #$100000,d5               /* d5 -> physical top of memory (1Mb) */

ret_2b:     lea       $8000,sp

#else
#if (TOSVERSION < 0x300) | TP_50
/* First we try to configure the memory controller */

            clr.w     d6
            move.b    #$a,(memconf).w			/* default: setup controller for 2Mb/2Mb */

            movea.w   #$8,a0
            lea       $200008,a1				/* + 2Mb */
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
            bra       memchk0
chkmem3:    beq.s     chkmem7					/* bank is not working => */

            movea.l   d7,a0
            adda.l    d7,a0						/* + 1024/512Kb byte */
            addq.l    #8,a0
            lea       chkmem4(pc),a4
            bra       memchk0
chkmem4:    beq.s     chkmem6					/* bank has 512Kb of memory => */

            movea.w   #8,a0						/* + 0 bytes */
            lea       chkmem5(pc),a4
            bra       memchk0
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
#if TP_04 & !TP_03 /* MEM_INIT & !RAMRESET */
        movea.w   #$100,a0
#else
        movea.w   #sysvars_start,a0
#endif

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

#else

/* =========================================================================== */
/* ==== memconf for Falcon =================================================== */
/* =========================================================================== */

		bset      #6,($FFFF8007).w
		beq.s     domemchk
		cmpi.l    #$752019F3,(_memvalid).w
		bne.s     domemchk
		cmpi.l    #$237698AA,(_memval2).w
		bne.s     domemchk
		cmpi.l    #$5555AAAA,(_memval3).w
		beq       nomemchk
domemchk:
		move.w    ($FFFF8006).w,d0
		lsr.w     #8,d0
		lsr.b     #1,d0
		move.b    d0,d1
		and.b     #$01,d1
		lsr.b     #2,d0
		and.b     #$06,d0
		or.b      d0,d1
		move.l    #$00080000,d0
		asl.l     d1,d0
		cmp.b     #$05,d1
		bne.s     domem1
		move.l    #$00E00000,d0
domem1:
		move.l    d0,d5
		lea.l     (sysvars_start).w,a4
		move.l    d5,d4
		sub.l     a4,d4
		lsr.l     #8,d4
		subq.l    #1,d4
		lea.l     ($00000200).w,a5
		moveq.l   #0,d0
		movea.l   d0,a0
		moveq.l   #0,d1
		movea.l   d1,a1
		moveq.l   #0,d2
		movea.l   d2,a2
		moveq.l   #0,d3
		movea.l   d3,a3
		lea.l     256(a4),a4
clrmem:
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		movem.l   d0-d3/a0-a3,-(a4)
		adda.l    a5,a4
		dbf       d4,clrmem

#endif

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
#if ((TOSVERSION >= 0x300) & (TOSVERSION < 0x400)) | TP_50
        move.l  #refresh_done,(_buserror).w      /* bus error vector */
        move.w  #$0000,tt_mcu+224
refresh_done:

        /* Can we access FastRAM region? */

        move.l    #tt_ramsize_done,(_buserror).w
        move.w    #$FB55,d3           /* d3.w = difference between adjacent stored values */
        moveq.l   #0,d0
        move.l    #$00020000,d7       /* d7 = address gap between memory test zones */
        movea.l   #$01020000,a0       /* a0 = address of current test zone */
#if TP_50
		movea.l   a0,a2
		movea.l   d7,a1
		move.l    -(a0),d1
		not.l     d1
		move.l    d1,-(a1)
		sub.l     (a0)+,d1
		clr.l     (a1)+
		tst.l     d1
		beq.s     tt_ramsize_done
#endif
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
#if TP_50
        move.l    a0,-4(a0)
        cmpa.l    -4(a2),a0
        bne.s     next_tt
#else
        bra.s     next_tt
#endif
tt_ramsize_done:                    /* bus error or bad ram  */
        suba.l    d7,a0
        cmpa.l    #$01000000,a0
        bne.s     ttramok
        suba.l    a0,a0
ttramok:
        move.l    a0,d5
        move.l    d5,(_ramtop).w
        move.l    #$1357BD13,(_ramvalid).w
#if !TP_50
        bset      #0,(scu_gp1).w
#endif
#else
        clr.l     _ramtop                /* no FASTRAM available */
        move.l    #$1357bd13,(_ramvalid).w   /* ramtop is valid (ramvalid == RAMMAGIC) */
#endif

nomemchk:

#endif /* TP_03 */


#if TP_05 /* SHELL_P */
		clr.l     (_shell_p).w
        movea.w   #SUPSIZ*2+_supstk,a7
#else
        movea.l   #SUPSIZ*2+_supstk,a7
#endif

#if STBOOK | (TOSVERSION >= 0x400)
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
            lea       just_rte(pc),a3           /* a3 -> handy RTE */
            lea       bios_unimpl(pc),a4        /* a4 -> handy RTS */

/* --- setup 64 vectors: */
            lea       coldboot,a1               /* a1 -> during boot all exception trigger a coldboot, which erases all memory and resets */
            lea       (_buserror).w,a0          /* a0 -> interrupt RAM */
            move.w    #$3f,d0                   /* d0 = count */
sei1:       move.l    a1,(a0)+                  /* write vector */
            dbra      d0,sei1                   /* (loop to write more vectors) */
            move.l    a3,(_zerodiv).w           /* divide-by-zero vector -> rte */

            move.l    a3,(_lvl7vec).w           /* level #7 interrupt -> rte (power exception in a ST Book) */
            moveq     #6,d0
            lea       (_lvl1vec).w,a1
sei2:       move.l    #just_rte,(a1)+           /* level #1 ... level #6 to RTE */
            dbra      d0,sei2

/* --- install OS interrupt vectors: */
            move.l    #int_vbl,(_lvl4vec).w       /* vblank handler */
            move.l    #int_hbl,(_lvl2vec).w       /* hblank handler */
            move.l    a3,(_trap2vec).w            /* (empty) trap#2 handler */
            move.l    #_biostrap,(_trap13ve).w    /* trap #13 handler */
            move.l    #_xbiostrap,(_trap14vec).w  /* trap #14 handler */
            move.l    #line1010,(_lineatrap).w    /* line 1010 handler */
            move.l    a4,(_etv_timer).w           /* default timer-tick vector -> rts */
            move.l    #critret,(_etv_critic).w    /* default critical error handler */
            move.l    a4,(_etv_term).w            /* default terminal vector -> rts */

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
            movea.w   #_bconstat_vec,a1
            moveq     #31,d0
tconl:      move.l    (a0)+,(a1)+
            dbra      d0,tconl

            movea.l   (_buserror).w,a0
            movea.l   sp,a1
            move.l    #vmeinit,(_buserror).w
#if (TOSVERSION < 0x400)
            move.b    #$40,(vme_mask).w         /* Enable IRQ6 from VMEBUS/MFP */
            move.b    #$14,(sys_mask).w         /* VSYNC & HSYNC enable in the VME Bus System Control Unit */
#endif
vmeinit:    move.l    a0,(_buserror).w
            movea.l   a1,sp
#endif

#if (TOSVERSION < 0x400)

/* detect basic STe/TT DMA sound */

#if TP_05 /* SHELL_P */
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

#if (TOSVERSION < 0x300) | TP_50
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
#endif /* STBOOK */

#else /* STBOOK | (TOSVERSION >= 0x400) */

		move.b    ($FFFF8006).w,d0
		move.b    d0,d1
		lsl.b     #6,d1
		and.b     #$80,d1
		lsr.b     #6,d0
		or.b      d0,d1
		move.w    d1,($FFFF82C0).w
		st        _has_dmasound
		sf        STEFlag

#endif

#if TP_06 /* MSTE_CACHE_ON */
		tst.b     (STEFlag).w
		bne.s     nostec
		ori.b     #3,($ffff8e21).w /* turn on 16MHz and cache */
nostec:
#endif /* TP_06 */


/* clear the remainder of the BSS section */

        movea.l   #__ebss,a1
        movea.l   #__sbss,a0
        moveq.l   #0,d0
clrbss:
        move.w    d0,(a0)+
        cmpa.l    a0,a1
        bne.s     clrbss


/* initialize the MMU */
#if (TOSVERSION >= 0x300) | TP_50
        bsr       initmmu
#endif

/* clear screen memory */

#if (TOSVERSION >= 0x300) & (TOSVERSION < 0x400) & !TP_50
scrsize equ ((1280*960)/8+$100)
#else
scrsize equ ((640*400)/8+$300)
#endif
#if TP_58 /* NEWVIDEO */
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
        move.w    #STEP_RATE,(_seekrate).w         /* set default seek-rate */
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

#if STBOOK | (TOSVERSION >= 0x400)
            move.l    #$5F56444F,(a0)+          /* setup VDO cookie: Video hardware */
#if (TOSVERSION >= 0x400)
            move.l    #$30000,(a0)+             /* 3,0 => Atari Falcon, ST */
            move.l    #$5F4D4348,(a0)+          /* setup MCH cookie: Machine type */
            move.l    #$30000,(a0)+             /* 3,0 => Atari Falcon, ST */
#else
            move.l    #$10001,(a0)+             /* 1,1 => Atari STE, ST-Book */
            move.l    #$5F4D4348,(a0)+          /* setup MCH cookie: Machine type */
            move.l    #$10001,(a0)+             /* 1,1 => Atari STE, ST-Book */
#endif

            move.b    #$7f,d0
            tst.b     STEFlag
            bne.s     cooSTE
            move.l    #$5F535749,(a0)+          /* setup SWI cookie: DIP configuration switches */
            moveq     #0,d0
            move.w    (STConfig).w,d0
            lsr.w     #8,d0
            move.l    d0,(a0)+                  /* all DIP switches as a bit mask 0..7 */
cooSTE:     
#if (TOSVERSION >= 0x400)
            moveq     #31,d1                    /* bit 0: PSG, bit 1: 8-bit DMA, 2: CODEC, 3: DSP, 4: Multiplex */
#else
            moveq     #3,d1                     /* bit 0: PSG, bit 1: 8-bit DMA */
#endif
            move.l    #$5F534E44,(a0)+          /* setup SND cookie: Sound hardware */
            btst      #7,d0                     /* DIP switch 7 on? */
            bne.s     cooSND                    /* (punt) */
            bclr      #1,d1                     /* no 8-bit DMA sound */
cooSND:     move.l    d1,(a0)+
            btst      #6,d0                     /* DIP switch 6 on? */
            bne.s     cooFDC                    /* (punt - no HD floppy) */
            move.b    #8,dsb                    /* select HD density for drive A */
            move.l    #'_FDC',(a0)+             /* setup FDC cookie: Floppy disk controller */
            move.l    #$01415443,(a0)+          /* 'ATC' | (1 << 24) */
cooFDC:     move.l    #$5F465055,(a0)+          /* Setup FPU cookie: Type of the FPU */
            moveq     #0,d7                     /* 0 = no FPU */
            suba.w    #$24,sp
            move.l    (_lineftrap).w,(sp)
            move.l    (_coprovio).w,4(sp)
            move.l    #cooFPU,(_lineftrap).w
            move.l    #cooFPU,(_coprovio).w
            lea       8(sp),a1
            movea.w   #_cpunordered,a2
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
            FRESTSP
cooFPU2:    move.l    #$20000,d7                /* 0x20000 = 68881 or 68882 as co-processor (Exact type unknown) */
cooFPU:     movea.l   a2,sp
            addq.w    #4,sp
            move.l    (sp)+,(_lineftrap).w
            move.l    (sp)+,(_coprovio).w
            move.l    (sp)+,(_cpunordered).w
            move.l    (sp)+,(_cpinexact).w
            move.l    (sp)+,(_cpdivzero).w
            move.l    (sp)+,(_cpunderflow).w
            move.l    (sp)+,(_cpoperror).w
            move.l    (sp)+,(_cpinexact).w
            move.l    (sp)+,(_cpdivzero).w
            move.l    d7,(a0)+
            movea.l   (_buserror).w,a1
            movea.l   sp,a2
            move.l    #cooFPU3,(_buserror).w
            move.w    (FPStat).w,d0
            bset      #0,-3(a0)                 /* SFP004 present */
cooFPU3:    move.l    a1,(_buserror).w
            movea.l   a2,sp

#else

#if (TOSVERSION < 0x300) | TP_50
			tst.b     STEFlag                   /* no STE hardware available? */
			beq.s     cooVDO                    /* (correct) */
			move.l    #$5F56444F,(a0)+			/* '_VDO' */
			clr.l     (a0)+                     /* 0,0 = Atari ST (260 ST, 520 ST, 1040 ST, Mega ST, ...) */
			move.l    #$5F4D4348,(a0)+			/* '_MCH' */
			clr.l     (a0)+                     /* 0,0 = Atari ST */
			bra.s     cooSWI

cooVDO:	    move.l    #$5F56444F,(a0)+
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
			tst.b     ide_stat2
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

cooSWI: move.b    #$7F,d0
        tst.b     STEFlag
#if OS_COUNTRY == CTRY_CZ
        bne.s     nomicro-4 /* apparently buggy patch; jumps into middle of instruction */
#else
        bne.s     nomicro
#endif
        move.l    #$5F535749,(a0)+ /* '_SWI' */
        moveq.l   #0,d0
        move.w    (STConfig).w,d0
        lsr.w     #8,d0
#if OS_COUNTRY == CTRY_CZ
		bclr      #6,d0 /* WTF? */
#endif
        move.l    d0,(a0)+
nomicro:
        moveq.l   #3,d1
        move.l    #$5F534E44,(a0)+ /* '_SND' */
        btst      #7,d0
        bne.s     yesstereo
        bclr      #1,d1
yesstereo:
        move.l    d1,(a0)+
#if !TP_08 /* SET_FDC_COOKIE */
#if OS_COUNTRY == CTRY_CZ
		nop
#else
        btst      #6,d0
        bne.s     nofdc
#endif
        move.b    #$08,dsb+0 /* fd_drivetype */
#endif
        move.l    #$5F464443,(a0)+ /* '_FDC' */
        move.l    #FDC_COOKIE,(a0)+ /* FDC_1ATC */
nofdc:

/* detect FPU */

        move.l    #$5F465055,(a0)+ /* '_FPU' */
        movea.l   (_lineftrap).w,a1 /* save Line-F trap */
        movea.l   (_coprovio).w,a2
        movea.l   a7,a3         /* save SP */
        move.l    #nofpu,(_lineftrap).w
        move.l    #nofpu,(_coprovio).w
#if TP_50 /* PAK */
/*
 * SE: (29.10.93)
 * Cookie-Test der FPU pr„zisiert
 */
			moveq     #0,d1
			dc.w      $f327      /* fsave -(a7) */
			move.b    1(a7),d0
			cmpi.b    #$18,d0
			beq.s     is881
			cmpi.b    #$38,d0
			beq.s     is882
			cmpi.b    #$40,d0
			bne.s     linef_emu
			addq.w    #2,d1
is882:		addq.w    #2,d1
is881:		addq.w    #2,d1
linef_emu:	addq.w    #2,d1
nofpu:		swap      d1
			move.l    d1,(a0)+
#else

#if (OS_COUNTRY == CTRY_PL) | TP_61 /* FPU_TEST */
        D0_FP0
#else
        FP0_D0
#endif
        move.l    #$00020000,(a0)+             /* 6888x present */
        bra.s     fpudone
nofpu:
        clr.l     (a0)+
#endif /* TP_50 */

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

#if (TOSVERSION >= 0x300) | TP_50
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

#if !STBOOK & (TOSVERSION < 0x400)
        lea.l     just_rte(pc),a3
        lea.l     bios_unimpl(pc),a4
        lea.l     any_vec(pc),a1            /* default to exception handler displaying bombs */
#if !P68010
		adda.l    #$2000000,a1				/* exception number in top 8 bits of the address */
#endif
        lea.l     (2*4).w,a0
        move.w    #64-3,d0
initexc:
        move.l    a1,(a0)+
#if !P68010
		adda.l    #$1000000,a1				/* increment the exception number */
#endif
        dbf       d0,initexc

        move.l    a3,(_zerodiv).w      /* ignore zero divide */
        move.l    a3,(_lvl7vec).w      /* ignore NMI */

        moveq.l   #6,d0
        lea.l     (_lvl1vec).w,a1
initauto:
        move.l    #just_rte,(a1)+      /* ignore auto vectors */
        dbf       d0,initauto

        move.l    #int_vbl,(_lvl4vec).w      /* vblank handler */
        move.l    #int_hbl,(_lvl2vec).w      /* hblank handler */
        move.l    a3,(_trap2vec).w           /* ignore GEM trap for now */
        move.l    #_biostrap,(_trap13vec).w  
        move.l    #_xbiostrap,(_trap14vec).w
        move.l    #line1010,(_lineatrap).w
        move.l    a4,(_etv_timer).w          /* no timer yet */
        move.l    #critret,(_etv_critic).w   /* default critical error handler */
        move.l    a4,(_etv_term).w           /* no user defined function */
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
#if TOSVERSION >= 0x400
		moveq.l   #FALCONREZ,d1
		move.w    (falcon_videomode).w,d2
		bne.s     nocompat
		move.w    #$0082,d2
nocompat:
		move.w    d2,(falcon_videomode).w
#else
#if STBOOK
            bsr       scanextrom

            move.b    #$80,(LcdPControl).w      /* LCD display on */
            move.b    #$80,(LcdPShadow).w       /* LCD display shadow register */
            moveq     #STHIGH,d1                /* Switch to 640x400x1 */
            lea       setvb1(pc),a6
            bra       waitvbl
setvb1:     move.b    d1,(v_shf_mod).w          /* set rez hardware register */

#else
#if (TOSVERSION < 0x300) | TP_50 /* PAK */
			moveq     #STLOW,d1					/* Switch to 320x200x4 */
			btst      #7,(MFP_GPIP).w			/* monochrome monitor connected? */
			bne.s     nomonomon					/* (no) */
			moveq     #STHIGH,d1                /* Switch to 640x400x1 */
nomonomon:
#if TP_50
            bsr       altsync
#else
            lea       setvb1(pc),a6
            bra       waitvbl
setvb1:
#endif
            move.b    d1,(v_shf_mod).w          /* set rez hardware register */

#else
        moveq.l   #TTMED,d1                 /* assume color, video mode 640x480x16 */
        btst      #7,(MFP_GPIP).w			/* monochrome monitor connected? */
        bne.s     moncolor
        moveq.l   #TTHIGH,d1                /* Switch to 1280x960x1 */
moncolor:
        move.b    d1,(shift_tt).w   /* set TT shifter video mode */
#endif
#endif
        move.b    d1,(_sshiftmod).w  /* .. and also system var */

        bsr       blittest
        jsr       resetdev                /* linaA blitter/no-blitter table init */
        jsr       esc_init                /* clear screen, initialize cursor */
#endif

/*
 * initialize video
 */
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

#if TOSVERSION < 0x400
        bra       biosinit
#include "intpriv.inc"

biosinit:
		jsr       _osinit            /* initialize DOS */

#else

biosinit:
		
		move.l    (_phystop).w,(_memtop).w
		jsr       _osinit            /* initialize DOS */
		move.w    (falcon_videomode).w,-(a7)
		move.w    #$0003,-(a7)
		clr.l     -(a7)
		clr.l     -(a7)
		jsr       VsetScreen
		lea.l     12(a7),a7
		jsr       $00E0567E(pc)
		move.w    #$0001,-(a7)
		move.w    #$0000,-(a7)
		move.w    #$0000,-(a7)
		move.w    #$0008,-(a7)
		move.w    #$0000,-(a7)
		jsr       devconnect(pc)
		addq.w    #6,a7
		clr.w     (a7)
		jsr       setmode(pc)
		move.l    #$00020080,(a7)
		jsr       soundcmd(pc)
		move.w    #$0003,(a7)
		jsr       soundcmd(pc)
		move.l    #$00060003,(a7)
		jsr       soundcmd(pc)
		move.w    #$0004,(a7)
		jsr       soundcmd(pc)
		move.w    #$0005,(a7)
		jsr       soundcmd(pc)
		addq.w    #4,a7

#endif


/* --- set the current system time and date */
        move.w    _os_dosdate,_date  /* use BIOS time as current time */
#if (HD_WAIT == 1) | (HD_WAIT == 2) | (HD_WAIT == 3)
		jsr       hd_wait
#endif
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


#if TP_46 /* NOVA */
		bsr       nova
#endif

/*
 * Draw the Atari logo.
 */
/* 306xx: 00e0074a */
/* 206xx: 00e00762 */
        movea.l   #atarilogo,a0
        movea.l   (_v_bas_ad).w,a1
#if (OS_COUNTRY == CTRY_PL) | TP_62 | TP_58
		lea       (lineavars).w,a2
		move.w    (a2),d4   /* LA_PLANES */
		move.w    -2(a2),d5 /* V_BYTES_LIN */
		move.w    d5,d0
		mulu.w    #8,d0
		adda.w    d0,a1
        moveq.l   #86-1,d0         /* 86 lines of data */
logocol1:
        moveq.l   #6-1,d1          /* 12 bytes of data per line */
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
#if BINEXACT
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
#endif
#else
        move.b    (_sshiftmod).w,d0
        cmp.b     #STHIGH,d0       /* ST-High resolution? */
        beq.s     logomono
        cmp.b     #TTHIGH,d0
        beq.s     logomono         /* TT-High resolution? */
#if (TOSVERSION >= 0x300) & !TP_50 /* PAK */
        adda.w    #((640/8)*4)*4,a1  /* start at screen line 4 */
#else
        adda.w    #((320/8)*4)*4,a1  /* start at screen line 4 */
#endif
        move.w    #86-1,d0         /* 86 lines of data */
logocol1:
        moveq.l   #5,d1            /* 12 bytes of data per line */
logocol2:
        move.w    (a0)+,d2
        move.w    d2,(a1)+         /* write 4 plane words */
        move.w    d2,(a1)+
        move.w    d2,(a1)+
        move.w    d2,(a1)+
        dbf       d1,logocol2
#if (TOSVERSION >= 0x300) & !TP_50 /* PAK */
        adda.w    #(640/8-12)*4,a1
#else
        adda.w    #(320/8-12)*4,a1
#endif
        dbf       d0,logocol1
        bra.s     logodone
logomono:
#if (TOSVERSION >= 0x300) & !TP_50 /* PAK */
        adda.w    #(1280/8)*4,a1   /* start at screen line 4 */
#else
        adda.w    #(640/8)*4,a1    /* start at screen line 4 */
#endif
        move.w    #86-1,d0         /* 86 lines of data */
logomon1:
        moveq.l   #12-1,d1         /* 12 bytes of data per line */
logomon2:
        move.b    (a0)+,(a1)+
        dbf       d1,logomon2
#if (TOSVERSION >= 0x300) & !TP_50 /* PAK */
        adda.w    #(1280/8)-12,a1
#else
        adda.w    #(640/8)-12,a1
#endif
        dbf       d0,logomon1
#endif
logodone:
		text

        data

/* 306us: e35fe8 */
/* 306de: e36018 */
/* 306fr: e36036 */
/* 306uk: e35fe8 */
/* 306es: e36044 */
/* 306it: e36026 */
/* 306sv: e36036 */
/* 306sf: e36044 */
/* 306sg: e36044 */

/* 206us: e3257e */
/* 206de: e325ae */
/* 206fr: e325cc */
/* 206us: e3257e */
/* 206es: e325da */
/* 206it: e325bc */
/* 206sv: e325ae */
/* 206sf: e325da */
/* 206sg: e325da */

/* 96x86 image of atari logo displayed at boot time */
atarilogo:
#if TP_33 == 0
#include "../tospatch/fuji/atari.S"
#endif
#if TP_33 == 1
#include "../tospatch/fuji/pak3.S"
#endif
#if TP_33 == 2
#include "../tospatch/fuji/gib_dos.S"
#endif
#if TP_33 == 3
#include "../tospatch/fuji/gt_tos.S"
#endif
#if TP_33 == 4
#include "../tospatch/fuji/schegale.S"
#endif
#if TP_33 == 5
#include "../tospatch/fuji/ufp_logo.S"
#endif
#if TP_33 == 6
#include "../tospatch/fuji/x.S"
#endif
#if TP_33 == 7
#include "../tospatch/fuji/intl_out.S"
#endif

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
#if TP_09 /* SET_TIME_CRC */
        cmpi.l    #BOOT_TIME*200,_hz_200       /* system running for >80s? */
#else
        cmpi.l    #80*200,_hz_200       /* system running for >80s? */
#endif
        bcc       ptch_term             /* (then no ROM CRC check) */
#endif

/*
 * check the ROM crc
 */
#if STBOOK | (TOSVERSION >= 0x400)
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
#if STBOOK | (TOSVERSION >= 0x400)
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
#if TP_50 /* PAK */
        bra       crcok
#else
        bra.s     crcok
#endif
crcmsg: dc.b 'WARNING: BAD ROM CRC IN CHIP ',0
crcmsg2 dc.b '.',13,10,0
crcfail:
        move.l    a5,d5
        pea.l     crcmsg
        move.w    #9,-(a7)
        trap      #1
        move.b    #$45,d0            /* 'E' - even */
        btst      #0,d5
        beq.s     chipE
        move.b    #$4F,d0            /* 'O' - odd */
chipE:
        move.w    d0,2(a7)
        move.w    #2,(a7)
        trap      #1
#if TOSVERSION >= 0x300
        move.b    #$45,d0            /* 'E' - even */
        btst      #1,d5
        beq.s     chipE2
        move.b    #$4F,d0            /* 'O' - odd */
chipE2:
        move.w    d0,2(a7)
        move.w    #2,(a7)
        trap      #1
#endif
        move.l    #crcmsg2,2(a7)
        move.w    #9,(a7)
        trap      #1
        addq.w    #6,a7
        addq.l    #1,a5
#if TP_50 /* PAK */
/*
 * Wegen der Static-RAMs erh”htes Risiko fr zerschossenes TOS
 * daher nach der Warnung eine Endlosschleife.
 */
#if !TP_51 /* StaticRam */
		lea      _main(pc),a0
		cmp.l    #$e00000,a0
		beq.s    chipE4
#endif
chipE3: bra.s chipE3
chipE4:
#endif
        dbf       d6,crccheck
crcok:


/*
 * if no monochrome is active, holding down the alternate key forces
 * to 320x200x4 instead of 640x480x4 for ST compatibility
 * ??? a bit too late, after displaying error messages
 */
#if TOSVERSION >= 0x300
#if TP_50 /* PAK */
        cmpi.b    #STHIGH,(_sshiftmod).w         /* ST high? */
#else
        cmpi.b    #TTHIGH,(_sshiftmod).w         /* TT high? */
#endif
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
#if TP_13 /* BOOT_DEV */
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
 * Default environment string
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
/* 206de: 00e00962 */
/* 104de: 00fc0530 */
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
/* 206de: 00e0097a */
/* 104de: 00fc054c */
dmaboot:
#if TP_10 /* SET_TIME_RAM */
        move.l    #BOOT_TIME*200,d7
#else
        move.l    #80*200,d7
#endif
        cmp.l     _hz_200,d7                /* is the system running for > 80s? */
        bcs       dmadone                   /* (no memory test) */
        movea.w   #$0000,a5                 /* done status = false */
dmaloop:
        cmpa.w    #$0000,a5                 /* memtest done? */
        bne       skipsp                    /* (yes) */
        bsr       memtest
        movea.w   d0,a5                     /* memtest done? */
        beq       dmawait                   /* (no) */
#if HD_WAIT == 4
		jsr       hd_wait
        bra       dmadone
#endif
#if TP_11 /* SKIP_WAIT */
        bra       dmadone
#endif
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
	    moveq	  #16,d4     /* d4 = device (IDE0) */
#else
#if TP_50 /* PAK */
/*
 * c't 9/92 / SE (21.09.93):
 * Bootpreference nicht aus SCU holen, sondern direkt in D0 schreiben.
 * (korrigiert zu MOVEQ #$80,D0)
 */
        moveq     #$80,d0
#if TP_64 /* IDE_BOOT */
        moveq.l   #16,d4     /* d4 = device (IDE0) */
#else
        moveq.l   #8,d4     /* d4 = device (SCSI0) */
#endif
#else
        jsr       scsirst
#if TP_64 /* IDE_BOOT */
        moveq.l   #16,d4     /* d4 = device (IDE0) */
#else
        moveq.l   #8,d4      /* d4 = device (SCSI0) */
#endif
        move.b    (scu_gp1).w,d0
#endif
        and.w     #$00F8,d0
        move.w    d0,(_bootpref).w
        bne.s     confdone
#if !TP_50 /* PAK */
        pea.l     (_bootpref).w
        move.w    #2,-(a7)
        clr.l     -(a7)
        jsr       _nvmaccess /* read first 2 bytes of nvram (bootpref) */
        adda.w    #10,a7
        tst.w     d0
        beq.s     confdone
#endif
#endif
#endif

#if TP_14 /* CT_HD */
#include "../tospatch/ct_hd1.S"
#else
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
        adda.w    #12,a7
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
        /* cmpi.b    #$ff,dmaDevList(pc,d4.w)  / * end of the device list? */
        dc.w $0c3b,$00ff         /* work around as68 bug with addressing mode above */
        dc.w dmaDevList-*+2+$4000       
        bne.s     confdone                  /* no -> continue with the next one */
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
dmaDevList: dc.b      16,17,0,1,2,3,4,5,6,7,-1
        even
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

#endif /* TP_14 */


/*
 * void run_cartridge_applications(WORD typebit);
 *
 * Run all cartridge applications of the specified type.
 *
 * typebit(d0): application type bit number which must be set to 1 to match
 */
#if STBOOK
#define STBOOK_EXTROM _os_entry+$40000
scanextrom:
        lea STBOOK_EXTROM,a0
        bra.s     cartext2
#endif
/* 306de: 00e00bbc */
/* 206de: 00e00b68 */
/* 104de: 00fc0628 */
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
/* 206de: 00e00b9a */
/* 104de: 00fc0652 */
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
#if (TOSVERSION < 0x300) | TP_50
memchk0:
#endif
/* 306: unused */
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
/* 206de: 00e00bb2 */
#if (TOSVERSION >= 0x300) & !TP_50
memchk0:
#endif
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
/* 206de: 00e00bd0 */
/* 104de: 00fc068a */
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
 * hbl - force caller to IPL
 * Oh-well:      "Yeah, it sucks, but it works" (--lt)
 *
 * Note:         Hacks caller's IPL to 3 (if it was 0). This is
 *               a kludge against fascist programs and certain
 *               debuggers that insist on starting processes up
 *               at IPL 0.
 *
 */
/* 306de: 00e00c44 */
/* 206de: 00e00bf0 */
/* 104de: 00fc06aa */
/* 404: 00e00c9a */
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
/* 206de: 00e00c06 */
/* 208de: 00e00baa */
/* 104de: 00fc06c0 */
/* 404: 00e00cb0 */
int_vbl:
        addq.l    #1,_frclock         /* increase num of happened ints */
        subq.w    #1,_vblsem          /* check vbl semaphore */
        bmi       vbl_end             /* if VBl routine disabled -> end */
        movem.l   d0-d7/a0-a6,-(a7)   /* save registers */
        addq.l    #1,_vbclock         /* count number of VBL interrupts */

/* ------ Call deferred interrupt vectors */
/* vblqueue */
        move.w    _nvbls,d7                 /* d7 = # of deferred vblank vectors */
#if STBOOK
        beq.s     vbl_no_queue              /* (punt if no vectors) */
#else
        beq       vbl_no_queue              /* (punt if no vectors) */
#endif
        subq.l    #1,d7                     /* turn into DBRA count */
        movea.l   (_vblqueue).w,a0          /* a0 -> vectors */
vbl_loop:
        movea.l   (a0)+,a1                  /* a1 -> deferred vector */
        cmpa.w    #$0000,a1                 /* if(a1 == NULL) continue; */
        beq.s     vbl_next
        movem.l   d7/a0,-(a7)               /* save registers */
        jsr       (a1)                      /* call routine */
        movem.l   (a7)+,d7/a0               /* restore registers */
vbl_next:
        dbf       d7,vbl_loop               /* loop for more vectors */

#if !STBOOK
        move.b    (MFP_GPIP).w,d1
        tst.b     _has_dmasound
        beq.s     nomoncheck
/* Check for monitor switching, */
/* and jump to _swv_vec if necessary... */
        move.w    sr,-(a7)
        ori.w     #$0700,sr
moncheck:
#if TP_50 /* PAK ?why? */
		move.b    #0,d0
#else
        move.b    ((sndmactl)+1).w,d0
#endif
        move.b    (MFP_GPIP).w,d1
        btst      #7,d1
        sne       d1
        move.b    (MFP_GPIP).w,d2
        btst      #7,d2
        sne       d2
        cmp.b     d1,d2
        bne.s     moncheck
#if TP_50 /* PAK ?why? */
		move.b    #0,d0
#else
        cmp.b     ((sndmactl)+1).w,d0
#endif
        bne.s     moncheck
        move.w    (a7)+,sr
        btst      #0,d0
        beq.s     nomoncheck
        not.b     d1
nomoncheck:
#if !TP_65 /* EAGLE */
#if (TOSVERSION < 0x300) | TP_50
		move.b    (v_shf_mod).w,d0
		and.b     #3,d0
		cmp.b     #STHIGH,d0
		bge.s     vbl_wasmono
#else
        move.b    (shift_tt).w,d0
        and.b     #7,d0
        cmp.b     #TTHIGH,d0
        beq.s     vbl_wasmono
#endif
        btst      #7,d1
        bne.s     vbl_nomonchg
        bsr       dovsync
#if (TOSVERSION < 0x300) | TP_50
		move.b    #STHIGH,d0
#else
        move.b    #TTHIGH,d0
#endif
        bra.s     vbl_monchg
vbl_wasmono:
        btst      #7,d1
        beq.s     vbl_nomonchg
        move.b    (_defshiftmod).w,d0
#if (TOSVERSION < 0x300) | TP_50
		cmp.b     #STHIGH,d0
		blt.s     vbl_monchg
#else
        cmp.b     #TTHIGH,d0
        bne.s     vbl_monchg
#endif
        clr.b     d0 /* STLOW */
vbl_monchg:
        move.b    d0,(_sshiftmod).w
#if (TOSVERSION < 0x300) | TP_50
		move.b    d0,(v_shf_mod).w
#else
        move.b    (shift_tt).w,d1
        and.b     #$F8,d1
        or.b      d0,d1
        move.b    d1,(shift_tt).w
#endif
        movea.l   (_swv_vec).w,a0
        jsr       (a0)
#endif /* TP_65 */
vbl_nomonchg:
#endif /* !STBOOK */
        jsr       blink								/* blink cursor */
/* --- reload color palettes */

        tst.l     (_colorptr).w						/* reload color palettes, if(colorptr != NULL).... */
        beq.s     vbl_no_palette
        movea.l   (_colorptr).w,a0					/* a0 -> user's color base */
        lea.l     (palette).w,a1					/* a1 -> hardware palette base */
        move.w    #7,d1								/* d0 = count */
vbl_palette_loop:
        move.l    (a0)+,(a1)+
        dbf       d1,vbl_palette_loop
        clr.l     (_colorptr).w
vbl_no_palette:
/* --- reload display base register */
        tst.l     (_screenpt).w
        beq.s     vbl_no_screenpt
        move.l    (_screenpt).w,(_v_bas_ad).w
        move.b    (_v_bas_ad+2).w,(v_bas_m).w
        move.b    (_v_bas_ad+1).w,(v_bas_h).w
#if TOSVERSION >= 0x200
#if !BINEXACT
        tst.b     STEFlag
        bne.s     vbl_no_screenpt
#endif
        move.b    (_v_bas_ad+3).w,(v_bas_l).w       /* BUG: no test that 820d actually exists */
#endif
vbl_no_screenpt:
        bsr       _flopvbl
vbl_no_queue:
/* screen hardcopy */
        tst.w     (_dumpflg).w						/* monitor screen dump flag - printscreen active? */
        bne.s     vbl_no_dump
        bsr       _scrdmp
vbl_no_dump:
        movem.l   (a7)+,d0-d7/a0-a6					/* restore registers & return (and a handy RTE) */
vbl_end:
        addq.w    #1,_vblsem						/* V(vblsem) [release vblank] */

/* 306de: 00e00d78 */
/* 206de: 00e00d1a */
/* 104de: 00fc07a0 */
just_rte:
        rte

/*
 * XBIOS #37 - Vsync - wait for next vblank
 */
/* 306de: 00e00d7a */
/* 206de: 00e00d1c */
/* 104de: 00fc07a2 */
_vsync:
        move.w    sr,-(a7)
        andi.w    #$FBFF,sr							/* enable vbl interrupts */
        move.l    _frclock,d0						/* d0 = frame clock */
vsyncwait:
        cmp.l     _frclock,d0						/* wait for clock to change */
        beq.s     vsyncwait
        move.w    (a7)+,sr
        rts

/*
 * _callcrit - critical error handler binding for C
 * Falls-into:   critret
 * (screwy way to save two bytes....)
 *
 */
/* 306de: 00e00d92 */
/* 206de: 00e00d34 */
/* 104de: 00fc07ba */
		xdef _callcrit
_callcrit:
        move.l    _etv_critic,-(a7)
/*
 * critret - default critical error handler
 * Loads -1 into D0 and returns
 *
 */
critret:
        moveq.l   #-1,d0
        rts

/*
 * trp13h - GEMDOS BIOS trap handler (trap 13)
 * trp14h - Atari BIOS extensions (trap 14)
 * traph  - trap handler
 *
 * On the stack:
 *       From super-             From user
 *       visor mode:             mode:
 *       -----------             ------------
 *       N(sp) args              N(usp) args
 *       6(sp) func#             6(usp) func#
 *       2(sp) ret               2(ssp) ret
 *        (sp) SR                 (ssp) SR
 *
 * Returns:      anything in D0
 * Uses:         d0-d2/a0-a2
 * Keeps:        C registers
 *
 * Notes         BIOS traps are re-entrant to 'nlevels' (declared near the
 *               beginning of this file).  Attempts to recurse more than
 *               'nlevels' will probably result in a crash.
 *
 *               BIOS calls may be made from user mode.  (This differs from
 *               the current GEMDOS spec, which states that BIOS traps are
 *               available from supervisor mode only).
 *
 */
/* ==== Trap 14 - XBIOS entry point ========================================= */

/* 306de: 00e00d9c */
/* 206de: 00e00d3e */
/* 104de: 00fc07c4 */
/* 404: 00e00d76 */
_xbiostrap:
        lea.l     xbios_vecs(pc),a0
        bra.s     biosxbios

/* ==== Trap 13 - BIOS entry point ========================================== */

/* 306de: 00e00da2 */
/* 206de: 00e00d44 */
/* 104de: 00fc07ca */
/* 404: 00e00d7c */
_biostrap:
        lea.l     bios_vecs(pc),a0

/* ==== Trap 13+14 handler ================================================== */
biosxbios:
        movea.l   _savptr,a1
        move.w    (a7)+,d0               /* Status register -> d0 */
        move.w    d0,-(a1)               /* and save in save_area */
        move.l    (a7)+,-(a1)            /* save return address */
#if !P68010 | !BINEXACT
        tst.w     (_longframe).w
        beq.s     traph2
#endif
        tst.w     (a7)+                  /* skip format word */
traph2: movem.l   d3-d7/a3-a7,-(a1)      /* regs, including stack pointer */
        move.l    a1,_savptr
        btst      #13,d0                 /* were we in supervisor mode? */
        bne.s     bx_sp_ok               /* yes, the sp already points to the arguments */
        move.l    usp,a7                 /* no, the arguments were on the user stack */
bx_sp_ok:
        move.w    (a7)+,d0               /* remove the function number from stack */
        cmp.w     (a0)+,d0               /* Higher than highest number? */
        bge.s     bx_ret                 /* if not implemented, returns the func number */
        move.w    d0,d1
        lsl.w     #2,d1                  /* function table is 1 LW per number so multiply function number by 4 */
        move.l    0(a0,d1.w),d1          /* get function address */
#if (TOSVERSION >= 0x300) | TP_50
        bclr      #0,d1                  /* indirect call? */
        movea.l   d1,a0
        beq.s     bx_notindirect
#else
        movea.l   d1,a0                  /* (quick and dirty test-for-negative) */
        bpl.s     bx_notindirect         /* points to code */
#endif
        movea.l   (a0),a0                /* call indirect through system variable */
bx_notindirect:
        suba.l    a5,a5                  /* a5 -> zero page */
        jsr       (a0)                   /* call BIOS/XBIOS function */
bx_ret:
        movea.l   _savptr,a1
        movem.l   (a1)+,d3-d7/a3-a7      /* Get regs back, including sp */
#if !P68010 | !BINEXACT
        tst.w     (_longframe).w
        beq.s     trph3
#endif
        clr.w     -(a7)                  /* push NULL format word */
trph3:  move.l    (a1)+,-(a7)            /* return adress */
        move.w    (a1)+,-(a7)            /* sr */
        move.l    a1,_savptr
        rte                              /* return with return value in D0 */

/* ------ jump table for BIOS functions: */
/* 306de: 00e00dfa */
/* 206de: 00e00da4 */
/* 104de: 00fc0818 */
#if (TOSVERSION >= 0x300) | TP_50
#define INDIRECT 1
#else
#define INDIRECT $80000000
#endif
bios_vecs:
        dc.w 12
        dc.l _getmpb
        dc.l _bbconstat
        dc.l _bbconin
        dc.l _bbconout
        dc.l _hdv_rw+INDIRECT /* Rwabs */
        dc.l _setexc
        dc.l _tickcal
        dc.l _hdv_bpb+INDIRECT /* Getbpb */
        dc.l _bbcostat
        dc.l _hdv_mediach+INDIRECT /* Mediach */
        dc.l _drvmap
        dc.l _kbshift

/* ------ jump table for XBIOS functions: */
/* 306de: 00e00e2c */
/* 206de: 00e00dd6 */
/* 104de: 00fc084a */
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
#if NVRAM_SUPPORT
        dc.l _nvmaccess
#else
        dc.l bios_unimpl
#endif
#if MSTECLK
        dc.l _waketime
#else
        dc.l bios_unimpl
#endif
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
#if (TOSVERSION >= 0x300) & !TP_50
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
#endif
xbvecsend:

#if TOSVERSION >= 0x300
/* unknown: 00e00fae; seems to be unused */
_brwabs:
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
#endif


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
/* 206de: 00e00edc */
/* 104de: 00fc0950 */
_supexec:
        movea.l   4(a7),a0
        jmp       (a0)


/*
 * Character device I/O
 *
 * No check is made for "bogus" device numbers.  A wierd device
 * number will result in a crash.
 *
 */

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
/* 206de: 00e00ee2 */
/* 104de: 00fc0956 */
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
/* 206de: 00e00eea */
/* 104de: 00fc095c */
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
/* 206de: 00e00ef2 */
/* 104de: 00fc0962 */
_bbcostat:
        lea.l     (_bcostat_vec).w,a0
        moveq.l   #8,d1 /* offsetof(MAPTAB, Bcostat) */
        bra.s     doxconxx

/*
 * BIOS #3 - Bconout - Print character to output device
 */
/* 306de: 00e00fee */
/* 206de: 00e00efa */
/* 104de: 00fc0968 */
_bbconout:
        lea.l     (_bconout_vec).w,a0
        moveq.l   #12,d1 /* offsetof(MAPTAB, Bconout) */

doxconxx:
        move.w    4(a7),d0            /* get device number */
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
        lsl.w     #2,d0             /* turn into longword index */
        movea.l   0(a0,d0.w),a0     /* get address of handler */
        jmp       (a0)              /* jump to it */


/*
 * Jump tables for
 *       0 - lst: (printer)
 *       1 - aux: (rs232)
 *       2 - con: (screen)
 *       3 - Atari midi
 *       4 - Atari keyboard (output only)
 *       5 - raw console output (bypass vt52 pressure cooker)
 *
 * No range checking is performed.  If a bogus device number
 * is passed to the BIOS' character I/O handler, the system
 * will crash to become funky duex.
 *
 */
/* 306de: 00e01024 */
/* 206de: 00e00f30 */
/* 104de: 00fc0978 */
/* 404: 00e00e56 */
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
        dc.l      _bco4stat     /* BUG compatibility with early TOS versions: MIDI(3) and KBD(4) are swapped */
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
/* 206de: 00e00fb0 */
/* 104de: 00fc09f8 */
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
/* 206de: 00e00fb6 */
/* 104de: 00fc09fe */
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
/* 206de: 00e00fc8 */
/* 104de: 00fc0a10 */
_getmpb:
        movea.l   4(a7),a0
        lea.l     (_themd).w,a1
/* --- initialize MPB: */
        move.l    a1,(a0)            /* mp_mfl = &themd; */
        clr.l     4(a0)              /* mp_mal = NULL; */
        clr.l     8(a0)              /* mp_rover = NULL; */
/* ---- initialize MD: */
        clr.l     (a1)               /* themd.m_link = NULL; */
        move.l    (_membot).w,4(a1)  /* themd.m_start = membot; */
        move.l    (_memtop).w,d0
        sub.l     (_membot).w,d0
        move.l    d0,8(a1)           /* themd.m_length = memtop - membot; */
        clr.l     12(a1)             /* themd.m_own = NULL; */

#if (TOSVERSION >= 0x300) | TP_50
        cmpi.l    #$1357BD13,_ramvalid
        bne.s     getmpb1
        cmpi.l    #FASTRAMBASE,_ramtop
        bls.s     getmpb1
        lea.l     (_altmd).w,a2
        move.l    a2,(a1)            /* themd.m_link = &altmd; */
        clr.l     (a2)               /* altmd.m_link = NULL; */
        move.l    #1+FASTRAMBASE,4(a2)   /* altmd.m_start = (base address of fast mem) | M_ALTFLAG */
        move.l    _ramtop,d0
        sub.l     #FASTRAMBASE,d0
        move.l    d0,8(a2)           /* altmd.m_length = ramtop - base address of fast mem */
        clr.l     12(a2)             /* altmd.m_own = NULL; */
getmpb1:
#endif

        rts

/*
 * BIOS #5 - Setexc - set exception vector
 * Synopsis:     setexc(vecno, addr)
 *               If 'addr' < 0, the vector is not set.
 *
 *               Extended vectors ($100 though $107) are located in the
 *               first eight longwords of BSS, at $400. This is for
 *               convienience -- they could really be located anywhere.
 *
 * Returns:      D0.L = original vector value
 *
 */
/* 306de: 00e01124 */
/* 206de: 00e00ff4 */
/* 104de: 00fc0a3c */
_setexc:
        move.w    4(a7),d0          /* d0 = vector # */
        lsl.w     #2,d0             /* turn into longword index */
        suba.l    a0,a0
        lea.l     0(a0,d0.w),a0     /* a0 -> vector */
        move.l    (a0),d0           /* d0 = current vector address */
        move.l    6(a7),d1          /* d1 = what_to_change_it_to */
        bmi.s     setexc1           /* punt if (d1 < 0) */
        move.l    d1,(a0)           /* set vector address */
setexc1:
        rts

/*
 * BIOS #6 - Tickcal - return system timer calibration value (in ms)
 */
/* 306de: 00e0113c */
/* 206de: 00e0100c */
/* 104de: 00fc0a54 */
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
/* 206de: 00e01014 */
/* 104de: 00fc0a5c */
_physbase:
        moveq.l   #0,d0             /* cleanup pointer-to-be */
        move.b    (v_bas_h).w,d0    /* load and shift bits 16..23 */
        lsl.w     #8,d0
        move.b    (v_bas_m).w,d0    /* load and shift bits 8..15 */
        lsl.l     #8,d0
#if TOSVERSION >= 0x200
        tst.b     STEFlag
        bne.s     physbas1
#endif
#if TOSVERSION >= 0x106
        move.b    (v_bas_l).w,d0
#endif
physbas1:
        rts

/*
 * XBIOS #3 - Logbase - Get the screen's logical base, right away.
 *
 * This is the location that GSX uses when drawing to the screen.
 */
/* 306de: 00e01160 */
/* 206de: 00e01030 */
/* 104de: 00fc0a6c */
/* 106de: 00e00bfe */
_logbase:
        move.l    (_v_bas_ad).w,d0
        rts

/*
 * XBIOS #4 - Getrez - Get the screen's current resolution
 *
 * Returns 0, 1, 2, 4, 5 or 6.
 */
/* 306de: 00e01166 */
/* 206de: 00e01036 */
/* 104de: 00fc0a72 */
/* 106de: 00e00c0a */
_getrez:
        moveq.l   #0,d0
#if STBOOK
        move.b    (_sshiftmod).w,d0         /* get screen rezolution */
        and.b     #3,d0                     /* strip garbage bits */
#else
#if (TOSVERSION < 0x300) | TP_50
		move.b    (v_shf_mod).w,d0          /* get screen rezolution */
        and.b     #3,d0                     /* strip garbage bits */
#else
        move.b    (shift_tt).w,d0
        and.b     #7,d0
#endif
#endif
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
/* 206de: 00e01042 */
/* 104de: 00fc0a7e */
/* 106de: 00e00c10 */
_vsetscreen:
/* --- set logical location: */
        tst.l     4(a7)                      /* if(logloc < 0) then ignore it */
        bmi.s     nosetl
        move.l    4(a7),(_v_bas_ad).w
nosetl:
/* --- set physical location: */
        tst.l     8(a7)                      /* if(physloc < 0) then ignore it */
        bmi.s     nosetp
        move.b    9(a7),(v_bas_h).w
        move.b    10(a7),(v_bas_m).w
#if TOSVERSION >= 0x106
#if !BINEXACT
		/* BUG: no test for STEFlag here in original */
        tst.b     STEFlag
        bne.s     nosetp
#endif
        move.b    11(a7),(v_bas_l).w         /* BUG: no test that 820d actually exists */
#endif
nosetp:
/* --- set screen resolution (clears the screen, clobbers the cursor): */
        tst.w     12(a7)                     /* if(rez < 0) then ignore it */
        bmi.s     nosetr
        bsr       dovsync                    /* wait for start of vertical-blank */
        move.b    13(a7),(_sshiftmod).w      /* set software shadow */
#if (TOSVERSION >= 0x300) & !TP_50
        move.b    (shift_tt).w,d0            /* set hardware location */
        and.b     #$F8,d0
        or.b      13(a7),d0
        move.b    d0,(shift_tt).w
#else
        move.b    (_sshiftmod).w,(v_shf_mod).w /* set hardware location */
#endif
        clr.w     (_vblsem).w                /* disable vblank processing */
        jsr       esc_init                   /* re-initialize glass tty routines */
        move.w    #1,(_vblsem).w             /* re-enable vblanks */
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
/* 206de: 00e0108e */
/* 104de: 00fc0ac4 */
/* 106de: 00e00c5c */
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
/* 206de: 00e01096 */
/* 104de: 00fc0acc */
/* 106de: 00e00c64 */
_setcolor:
        move.w    4(a7),d1                   /* get color number */
        add.w     d1,d1                      /* turn into word index */
        and.w     #$001F,d1                  /* force color range (prevent buserr) */
        lea.l     (palette).w,a0             /* a0 -> base of palette memory */
        move.w    0(a0,d1.w),d0              /* return old color */
        tst.b     STEFlag
        beq.s     setcol1
        and.w     #$0777,d0                  /* mask dirty bits */
        bra.s     setcol2
setcol1:
        and.w     #$0FFF,d0
setcol2:
        tst.w     6(a7)                      /* if new color is <0, don't set it */
        bmi.s     setcol3                    /* (punt) */
        move.w    6(a7),0(a0,d1.w)           /* set new color */
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
/* 206de: 00e010c8 */
/* 104de: 00fc0af0 */
_puntaes:
        movea.l   _os_magic(pc),a0           /* get pointer to magic */
        cmpi.l    #$87654321,(a0)            /* is the magic still there? */
        bne.s     puntaes1                   /* no -- just return */
        cmpa.l    (_phystop).w,a0            /* is it in ROM? */
        bge.s     puntaes1                   /* yes -- we can't do anything about it */
        clr.l     (a0)                       /* clobber AES! */
        bra       _main                      /* restart the system */
puntaes1:
        rts


/*
 * _term - terminate current process
 * Called-by:    Uncaught traps (bus errors, and so on)
 * Saves:        processor state (in a bailout area)
 */
/* 306de: 00e0121c */
/* 206de: 00e010e2 */
/* 104de: 00fc0b0a */
any_vec:
#if !P68010
		jsr       savp_2                     /* stack PC */
savp_2:
		move.l    (sp)+,(_proc_enum).w       /* save bogus PC + exception number */
#endif
/* dump the registers, including SSP */
        movem.l   d0-d7/a0-a7,(_proc_dregs).w
#if P68010
        move.l    2(a7),(_proc_enum).w
/* get the exception number from the format word */
        move.w    6(a7),d0
        and.w     #$0FFF,d0
        asr.w     #2,d0
/* dump the exception number */
        move.b    d0,(_proc_enum).w
#endif
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

/* --- draw an appropriate number of 'shrooms on the screen: */
        moveq.l   #0,d1
        move.b    (_proc_enum).w,d1
        subq.w    #1,d1
        bsr.s     drawbombs
/* restore a sane stack (should have probably been done before calling subroutine above) */
        move.l    #save_beg+save_siz,(_savptr).w        /* clobber BIOS top level */
        move.w    #-1,-(a7)                             /* "error" return condition */
        move.w    #$004C,-(a7)                          /* Pterm */
        trap      #1
        bra       _main

/*
 * do_shroom - draw little mushroom clouds on the screen
 *  Passed:      d1.w = #shrooms to draw (DBRA count)
 *  Returns:     some shrooms on display
 *  Uses:        d0-d7/a0-a2
 *
 *  Discussion:  The graphics ain't all that great.   And this is silly.
 *
 */
/* 104de: 00fc0b56 */
/* 106de: 00e00cee */
drawbombs:
#if (TP_50 & TP_52) | TP_62 /* (PAK & Grafik_Bombs) | LOGO */
/*
 * Bombenausgabe auf Grafikkarten/Grafikerweiterungen u.„.
 *
 * Schwierigkeiten:
 * Auf Grafikkarten ohne Bitplanes (zB MegaVision300) werden die Bomben in
 * Farbe nicht korrekt ausgegeben.
 */
	lea       (lineavars).w,a2
	move.w    (a2),d7   /* LA_PLANES */
	subq.w    #1,d7
	moveq     #0,d4
	move.w    -2(a2),d4 /* V_BYTES_LIN */
	movea.l   (_v_bas_ad).w,a0
	move.w    -4(a2),d0 /* V_REZ_VT */
	lsr.w     #1,d0
	mulu      d4,d0
	adda.l    d0,a0
	lea       bombimage(pc),a1
	moveq     #$f,d6
drawbo4:
	move.w    d1,d2
	movea.l   a0,a2
drawbo5:
	move.w    d7,d5
drawbo6:
	move.w    (a1),(a2)+
	dbf       d5,drawbo6
	dbf       d2,drawbo5
	addq.w    #2,a1
	adda.w    d4,a0
	dbf       d6,drawbo4
	rts
#else
        move.b    (_sshiftmod).w,d7
#if (TOSVERSION >= 0x300) & !TP_50
        and.w     #7,d7
#else
        and.w     #3,d7
#endif
        add.w     d7,d7                      /* d7 = rez index */
        moveq.l   #0,d0
        move.b    (v_bas_h).w,d0
        lsl.w     #8,d0
        move.b    (v_bas_m).w,d0
        lsl.l     #8,d0
#if TOSVERSION >= 0x200
        tst.b     STEFlag
        bne.s     drawbo1
        move.b    (v_bas_l).w,d0
#endif
drawbo1:
        movea.l   d0,a0                      /* a0 -> base of mem to draw at */
        cmp.w     #TTHIGH,d7
        blt.s     drawbo2
        adda.l    #80*960,a0
        bra.s     drawbo3
drawbo2:
        adda.w    #50*320,a0
drawbo3:
        lea.l     bombimage,a1               /* a1 -> source from */
        move.w    #$000F,d6                  /* d6 = scanline count */
drawbo4:
        move.w    d1,d2                      /* d2 = # to draw on this line */
        movea.l   a0,a2                      /* safe ptr to beg of line */
drawbo5:
        move.w    dplanetab(pc,d7.w),d5      /* d5 = #words to replicate */
drawbo6:
        move.w    (a1),(a0)+
        dbf       d5,drawbo6
        dbf       d2,drawbo5                 /* another, on the same line */
        addq.w    #2,a1                      /* next source word */
        adda.w    dwidthtab(pc,d7.w),a2      /* next dest line */
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
#endif


/*
 * _fastcpy - "fast" 512-byte copy
 * Synopsis:     void cpy512(const void *src, void *dst)
 *
 *               Used by _rwabs to fake disk DMA to odd addresses.  Therefore,
 *               disk I/O on odd addresses is very slow.  Lose, lose.
 *
 */
		xdef _cpy512
_cpy512:
        movea.l   4(a7),a0
        movea.l   8(a7),a1
        move.w    #(512/8)-1,d0
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
 * _auto - exec auto-startup files in the appropriate subdirectory
 * _auto1 - exec (with filename args)
 * Passed:       a0 -> full filespec (pathname)
 *               a1 -> filename part of filespec
 *               _drvbits: bit vector of active drives
 *               _bootdev: contains device to exec from
 *
 * Returns:      nothing
 *
 * Note:         If _drvbits%%_bootdev is zero, _auto simply quits (since
 *               the device isn't active....)
 *
 * Uses:         everything
 */
/* 306de: 00e0133c */
/* 206de: 00e011f6 */
/* 104de: 00fc0bf4 */
/* 106de: 00e00d94 */
autoexec:
        move.l    #$000Bffff,-(a7)
        trap      #13                       /* Kbshift(-1) */
        addq.l    #4,a7
        btst      #2,d0                     /* Control pressed? */
        bne.s     autoexe2                  /* yes, skip autoexec */
#if TOSVERSION < 0x300
        move.l    (_drvbits).w,d0           /* d0 - active dev vector */
        move.w    _bootdev,d1               /* d1 - dev# to exec from */
        btst      d1,d0                     /* is the dev alive? */
#if STBOOK | !BINEXACT
        beq.s     autoexe2                  /* (no -- so punt) */
#else
        beq.s     autoexe1                  /* (no -- so punt) BUG: wrong label used here */
#endif
#endif
        lea.l     autopath(pc),a0
        lea.l     autoname(pc),a1
_auto1: move.l    (a7)+,savepc              /* return addr (used by execlr) */
        move.l    a0,(execpath).w
        move.l    a1,(execname).w
#if TOSVERSION >= 0x300
        move.l    (_drvbits).w,d0           /* d0 - active dev vector */
        move.w    _bootdev,d1               /* d1 - dev# to exec from */
        btst      d1,d0                     /* is the dev alive? */
        beq.s     autoexe1                  /* (no -- so punt) */
#endif
        lea.l     emptystr(pc),a0
        move.l    a0,-(a7)                  /* null environment */
        move.l    a0,-(a7)                  /* null command tail */
        move.l    a0,-(a7)                  /* null shell name */
        move.w    #$0005,-(a7)
        move.w    #$004B,-(a7)
        trap      #1                        /* Pexec(5) Create basepage */
        adda.w    #16,a7
        movea.l   d0,a0
        move.l    #autoscan,8(a0)           /* set bp->p_tbase */
        move.l    a3,-(a7)                  /* null environment */
        move.l    d0,-(a7)                  /* -> PSP */
        move.l    a3,-(a7)                  /* null shell name */
        move.w    #$0004,-(a7)
        move.w    #$004B,-(a7)
        trap      #1                        /* Pexec(Just go), execute the autoscan routine */
        adda.w    #16,a7
autoexe1:
        move.l    savepc,-(a7)
autoexe2:
        rts

/*
 * ST Book has another ROMDISK in the 2nd 256kb of the ROM
 * which is mapped as drive 'P'.
 */
#if STBOOK
autoerom:   lea       autopROM(pc),a0        /* -> path */
            lea       autopROM+8(pc),a1      /* -> filename */
            bra.s     _auto1

autopROM:   dc.b      'P:\AUTO\*.PRG',0
            dc.l      $12345678,$9abcdef0
#endif

/*
 * fauto - exec'd by _auto to do autostartup
 *
 * Passed:       pathname -> path part of filespec
 *               filename -> file path of filespec
 *
 */
autoscan:
        clr.l     -(a7)
        move.w    #$0020,-(a7)
        trap      #1              /* Super */
        addq.w    #6,a7
        movea.l   d0,a4

/* ---- free up some memory */
        movea.l   4(a7),a6                      /* get ptr to Basepage */
        lea.l     256(a6),a7                    /* set stack to end of commandline */
        move.l    #$00000100,-(a7)              /* sizeof(PD) */
        move.l    a6,-(a7)
        clr.w     -(a7)
        move.w    #$004A,-(a7)                  /* Mshrink to basepage only */
        trap      #1
#if TP_15 /* STACK_POINTER */
		adda.w    #12,a7
        tst.w     d0
        bne.s     autosca4                      /* if Mshrink failed, done */
        move.w    #7,-(a7)
        move.l    (execpath).w,-(a7)
#else
        addq.w    #6,a7
        tst.w     d0
        bne.s     autosca4                      /* if Mshrink failed, done */
        move.w    #7,-(a7)                      /* find r/o+hidden+system files */
        move.l    execpath,-(a7)                /* -> filename (on input) */
#endif
        move.w    #$004E,-(a7)                  /* Fsfirst */
        moveq.l   #8,d7
autosca1:
        pea.l     execdta                       /* setup DTA (for search) */
        move.w    #$001A,-(a7)
        trap      #1                            /* Fsetdta */
        addq.w    #6,a7
        trap      #1
        adda.w    d7,a7
        tst.w     d0                            /* Path found? */
        bne.s     autosca4                      /* no, done */
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
        pea.l     emptystr(pc)                  /* null enviroment */
        pea.l     emptystr(pc)                  /* no command tail */
        pea.l     execfname                     /* -> file to exec */
        clr.w     -(a7)
        move.w    #$004B,-(a7)
        trap      #1                            /* Pexec(Load&Go) */
        adda.w    #$0010,a7
        moveq.l   #2,d7
        move.w    #$004F,-(a7)                  /* Fsnext */
        bra.s     autosca1

/*
 * The first GEMDOS process can never terminate.
 * This is not a good feature.
 * Kludge around it - re-initialize the stack
 * and return to the guy who called us to begin with.
 *
 */
autosca4:
        lea.l     SUPSIZ*2+_supstk,a7          /* setup supervisor stack */
        move.l    savepc,-(a7)                 /* get return addr */
        rts


#include "scrdmp.inc"
		text

/* --- what it is: */
/* 306de: 00e014f6 */
/* 206de: 00e013b2 */
/* 104de: 00fc0da2 */
/* 106de: 00e00f3e */
bombimage:
#if TP_16 == 0 /* Bomb */
        dc.b $06,$00,$29,$00,$00,$80,$48,$40,$11,$f0,$01,$f0,$07,$fc,$0f,$fe
        dc.b $0d,$fe,$1f,$ff,$1f,$ef,$0f,$ee,$0f,$de,$07,$fc,$03,$f8,$00,$e0
#endif
#if TP_16 == 1 /* PILZE */
        dc.b $07,$c0,$1f,$f0,$3b,$f8,$77,$f4,$b7,$fa,$bb,$fa,$df,$f6,$66,$fc
        dc.b $32,$88,$02,$80,$04,$40,$04,$40,$05,$40,$05,$20,$09,$20,$09,$20
#endif
#if TP_16 == 2 /* Atari logo */
		dc.b $00,$00,$05,$a0,$05,$a0,$05,$a0,$05,$a0,$05,$a0,$05,$a0,$0d,$b0
		dc.b $0d,$b0,$1d,$b8,$3d,$bc,$f9,$9f,$f9,$9f,$f1,$8f,$e1,$87,$00,$00
#endif
#if TP_16 == 3 /* Pfeifenkopf */
		dc.b $07,$f0,$0f,$f8,$1f,$ec,$18,$04,$18,$04,$10,$04,$1e,$3c,$17,$54
		dc.b $11,$04,$0b,$28,$0d,$d8,$06,$28,$07,$d0,$2e,$10,$39,$e0,$38,$00
#endif
#if TP_16 == 4 /* Stinkefinger */
		dc.b $0c,$00,$12,$00,$11,$00,$09,$00,$09,$f0,$04,$a8,$1c,$a4,$26,$54
		dc.b $22,$04,$28,$04,$3c,$04,$44,$02,$40,$02,$30,$02,$0e,$02,$01,$01
#endif
#if TP_16 == 5 /* Skull */
		dc.b $07,$E0,$08,$10,$16,$68,$16,$68,$10,$08,$09,$90,$08,$10,$0B,$D0
		dc.b $04,$20,$33,$CC,$2C,$34,$13,$C8,$0C,$30,$13,$C8,$2C,$34,$30,$0C
#endif

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
/* 206de: 00e013d2 */
/* 104de: 00fc0dc2 */
/* 106de: 00e00f5e */
waitvbl:    lea       (MFP_TBDR).w,a0       /* a0 -> timer B data register */
            lea       (MFP_TBCR).w,a1       /* a1 -> timer B control register */
            bclr      #0,(MFP_IERA).w       /* disable IRQ of timer B */
            moveq     #1,d4                 /* wait for the timer to expire */
            clr.b     (a1)                  /* stop timer B */
            move.b    #$f0,(a0)             /* event every 240 scan lines */
            move.b    #8,(a1)               /* timer b: event count mode (HBL) */
waitvbl2:   cmp.b     (a0),d4               /* wait for HBL 239 scan lines to pass */
            bne.s     waitvbl2
waitvbl3:   move.b    (a0),d4
            move.w    #615,d3               /* wait till we are inside the vbl area */
waitvbl4:   cmp.b     (a0),d4
            bne.s     waitvbl3
            dbra      d3,waitvbl4
            move.b    #$10,(a1)             /* timer b: reset */
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
/* 206de: 00e01408 */
/* 104de: 00fc0dfe */
/* 106de: 00e00f9a */
run_reset_resident:
        movea.l   _phystop,a0                   /* start at the top of the address space */
run_res1:
        suba.w    #$0200,a0
        cmpa.w    #$0400,a0                     /* reach the lower bottom? */
        bls.s     run_res3                      /* (bail out) */
        cmpi.l    #RR_MAGIC,(a0)                /* check for magic */
        bne.s     run_res1                      /* (no magic -> next block) */
        cmpa.l    4(a0),a0                      /* second long is equal the base address of the block? */
        bne.s     run_res1                      /* (no -> next block) */
        clr.w     d0
        movea.l   a0,a1
        move.w    #$00FF,d1                     /* checksum over 256 words */
run_res2:
        add.w     (a1)+,d0
        dbf       d1,run_res2
        cmp.w     #RR_CHKSUM,d0                 /* magic checksum? */
        bne.s     run_res1                      /* (no -> next block) */
        move.l    a0,-(a7)                      /* save current address */
        jsr       8(a0)                         /* call code block */
        movea.l   (a7)+,a0                      /* continue with next block */
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
/* 206de: 00e01446 */
/* 104de: 00fc0e3e */
_gettime:
        lea.l     readRTCTime,a3
        lea.l     igetdt,a4
        bra.s     gsettime

/*
 * XBIOS #16 - Settime - Sets the intelligent keyboard's time and date.
 *
 * 'datetime' is a 32-bit DOS-format date and time (time in the low word,
 * date in the high word).
 */
/* 306de: 00e0157c */
/* 206de: 00e01454 */
/* 104de: 00fc0e4c */
_settime:
        move.w    4(a7),_date
        move.w    6(a7),_time
#if TP_17 /* LOCK_CLOCK */
        lea.l     retzero(pc),a3
#else
        lea.l     writeRTCTime,a3
#endif
        lea.l     isetdt,a4
gsettime:
        bsr       checkRTC
        bcc.s     gsettim1
        movea.l   a4,a3
gsettim1:
        jmp       (a3)


/*
 * copy sysbase into memory and patch it
 */
/* 306de: 00e015a2 */
/* 206de: 00e0147a */
/* 104de: 00fc0e62 */
/* 106de: 00e00ffe */
instoshdr:
        lea.l     _os_entry(pc),a0
        lea.l     oshdr+6,a1
        moveq.l   #47,d0
instos1:
        move.b    0(a0,d0.w),0(a1,d0.w)
        dbf       d0,instos1
        move.w    jmpop(pc),-6(a1)              /* copy JMP instruction just before */
        move.l    4(a1),-4(a1)                  /* copy ROM reseth into this JMP; BUG: the jmpop before is a short abs jump */
        move.w    braop(pc),(a1)                /* patch BRA at the beginning of sysbase copy to hit the JMP */
        move.w    30(a1),28(a1)                 /* copy os_date into os_conf WTF? */
        move.l    a1,_sysbase                   /* set new sysbase pointer */
        rts
jmpop:
        jmp       ($00000000).w
braop:
        bra.s     jmpop


/*
 * XBIOS #40 - Blitmode
 *
 * Returns: d0.w - bit 0 - blitter enabled
 *                 bit 1 - blitter available
 */
/* 306de: 00e015dc */
/* 206de: 00e014b4 */
/* 104de: 00fc0e9e */
/* 106de: 00e0103a */
_blitmode:
#if TP_18 /* IGNORE_BLITTER */
		moveq     #0,d0
#else
        bsr.s     blittest                      /* test if blitter is installed */
        move.w    d0,d4                         /* d4 = blitter status */
        move.w    d0,d5
        lsr.w     #1,d5
        or.w      #$FFFE,d5                     /* d5 = blitter enable mask */
        jsr       _GETBLT
        move.w    d0,d3                         /* d3 = blitter active */
        move.w    4(a7),d0                      /* new blitter status */
        bmi.s     blitmod1                      /* <0 just return the status */
        and.w     d5,d0                         /* mask blitter status out */
        or.w      d4,d0                         /* or'd blitter available status */
        jsr       _SETBLT                       /* disable/enable blitter */
blitmod1:
        move.w    d3,d0
#endif
        rts

/*
 * blittest - test if the blitter is installed
 */
/* 306de: 00e01604 */
/* 206de: 00e014dc */
/* 104de: 00fc0ec6 */
blittest:
        move.w    sr,d1
        move.w    #0,d0                         /* d0 = 0 -> blitter not installed */
        suba.l    a0,a0
        movea.l   a7,a2
        ori.w     #$0700,sr
        movea.l   _buserror(a0),a1
        move.l    #noblitter,_buserror(a0)
        tst.w     $ffff8a00(a0)
        moveq.l   #2,d0                         /* d0 = 2 -> blitter installed */
noblitter:
        move.l    a1,_buserror(a0)
        move.w    d1,sr
        movea.l   a2,a7
        rts

/*
 * delay for a little bit via timer a in the mfp, or timer c in the ttmfp
 */
/* 306de: 00e0162e */
/* 206de: 00e01506 */
	xdef mfpdelay
mfpdelay:

#if TP_50 & !TP_55 /* PAK & !HasTTMfp */

/*
 * SE: (13.10.93)
 * Delay-Routine:
 * Zeitmessung von Timer C im TT-MFP auf Timer C im ST-MFP verlegt.
 * Zur Messung wird Polling von TCDR verwendet (Aufl”sung 26 usec).
 *
 * Nicht n”tig, wenn ein TT-MFP existiert.
 *
 * FIXME: bad idea, Timer-C is also used for 200hz timer
 * could just use MFP Timer-A, as the 2.06 version
 */
      movem.l   a0/d0-d1,-(a7)
      moveq     #0,d1
      move.b    d0,d1
      bne.s     mfpdela1
      move.w    #$100,d1
mfpdela1:
	  lsr.w     #8,d0
      andi.w    #7,d0
      move.b    mfpdeltab(pc,d0.w),d0
      mulu      d0,d1
      lsr.l     #4,d1
      addq.l    #2,d1
      lea       (MFP_TCDR).w,a0
mfpdela2:
	  move.b    (a0),d0
mfpdela3:
	  cmp.b     (a0),d0
      beq.s     mfpdela3
      dbf       d1,mfpdela2
      movem.l   (a7)+,a0/d0-d1
      rts
mfpdeltab:
	  dc.b      0,1,3,4,13,16,25,50

#else

        bsr.s     timerc
mfpdela1:
#if TTMFP_SUPPORT
        btst      #5,(MFP_IPRB+(TTMFP_REGS-MFP_REGS)).w
        beq.s     mfpdela1
#else
        btst      #5,(MFP_IPRA).w               /* did timer a fire? */
        beq.s     mfpdela1                      /* wait more if not */
        clr.b     (MFP_TACR).w                  /* stop timer a */
#endif
        rts

/*
 * (re-)program TT-MFP Timer C or MFP Timer A
 * Input: D0: initial counter (low byte) + (divisor<<8)
 */
/* 306de: 00e0163a */
/* 206de: 00e01516 */
timerc:
#if TTMFP_SUPPORT
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
#else
		move      sr,-(sp)
        ori       #$700,sr
        clr.b     (MFP_TACR).w                  /* timer A control register */
        bclr      #5,(MFP_IERA).w               /* disable timer A */
        move.b    #$df,(MFP_IPRA).w             /* clear a pending timer A interrupt */
        bclr      #5,(MFP_IMRA).w               /* mask the timer A interrupt */
        bset      #5,(MFP_IERA).w               /* enable timer A */
        move      (sp)+,sr
        move.b    d0,(MFP_TADR).w               /* set timer a data register */
        ror.w     #8,d0
        move.b    d0,(MFP_TACR).w               /* set timer a control register */
        rol.w     #8,d0
#endif
        rts

#endif /* TP_50 & !TP_55 */

#include "zeromem.S"

#include "mmu030.inc"

#include "romcrc.inc"

		text

#if TP_50 /* PAK */
/*
 * Einen Teil der Initialisierung des Shifters verschoben.
 */
VDOSkip1:
		bsr.s altsync
#if TP_02 /* COLOR_60HZ */
        move.b    #0,(v_syncmod).w          /* yes -- twiddle to 60hz */
#else
        move.b    #2,(v_syncmod).w          /* yes -- twiddle to 50hz */
#endif
		rts

altsync:
#if !TP_57 /* Alt_Sync */
	  /* Synchronisierung mit DE-Signal an TBI */
      lea       (MFP_TBDR).w,a0
      lea       (MFP_TBCR).w,a1
      bclr      #0,(MFP_IERA).w
      moveq     #1,d4
      clr.b     (a1)
      move.b    #$f0,(a0)
      move.b    #8,(a1)
altsync1:
	  cmp.b     (a0),d4
      bne.s     altsync1
altsync2:
      move.b    (a0),d4
      move.w    #616-1,d3
altsync3:
	  cmp.b     (a0),d4
      bne.s     altsync2
      dbf       d3,altsync3
      move.b    #$10,(a1)
#else
      move.b    (v_bas_h).w,d3
      move.b    (v_bas_m).w,d4
      lea       ($ffff8205).w,a0
      lea       ($ffff8207).w,a1
altsync1:
      cmp.b     (a0),d3
      bne.s     altsync2
      cmp.b     (a1),d4
      beq.s     altsync1
altsync2:
      cmp.b     (a0),d3
      bne.s     altsync2
      cmp.b     (a1),d4
      bne.s     altsync2
#endif
		rts

/*
 * Initialisierung des DMA-Sound.
 */
#if 0 /* not needed for source patch; already done above */
InitDMASound:
      movea.l   a7,a6
      lea       initdma5(pc),a0
      move.l    a0,(_buserror).w
      clr.w     (sndmactl).w
      st        (_has_dmasound).w
      lea       initdma1(pc),a0
      move.w    (a0)+,(mwmask).w
      bra.s     initdma4
initdma1:
      dc.b      $0f,$fe,$09,$d1,$0a,$a9,$0a,$29
      dc.b      $09,$0d,$08,$8d,$08,$03,$00,$00
initdma2:
      move.w    d0,(mwdata).w
initdma3:
      tst.w     (mwdata).w
      bne.s     initdma3
initdma4:
      move.w    (a0)+,d0
      bne.s     initdma2
initdma5:
      movea.l   a6,a7
      move.b    #90,(v_bas_l).w
      tst.b     (v_bas_m).w
      move.b    (v_bas_l).w,d0
      cmp.b     #90,d0
      bne.s     initdma6
      clr.b     (v_bas_l).w
      tst.w     (palette).w
      tst.b     (v_bas_l).w
initdma6:
      sne       (STEFlag).w
      rts
#endif

/*
 * Cookie-Check _VDO und _MCU aus TOS 2.06.
 * Hope it works, bisher nicht getestet.
 */
#if 0 /* not needed for source patch; same functionality as in 2.06 */
MCHTest:
      move.l    #$5f56444f,(a0)+    /* "_vdo" */
      tst.b     (STEFlag).w
      beq.s     mchtest1
      clr.l     (a0)+
      clr.l     d0
      bra.s     mchtest4
mchtest1:
	  move.l    #$00010000,(a0)+
      move.l    #$00010000,d0
      movea.l   (_buserror).w,a1
      lea       mchtest2(pc),a2
      move.l    a2,(_buserror).w
      movea.l   a7,a2
      tst.b     (scu_gp1).w
      move.w    #$10,d0
      bra.s     mchtest3
mchtest2:
      clr.w     d0
      movea.l   a2,a7
      lea       mchtest3(pc),a3
      move.l    a3,(_buserror).w
      tst.b     $fff00039.l
      move.w    #8,d0
mchtest3:
      move.l    a1,(_buserror).w
      movea.l   a2,a7
mchtest4:
      move.l    #$5f4d4348,(a0)+    /* "_mch" */
      move.l    d0,(a0)+
      rts
#endif

#endif /* TP_50 */


/*
 * XBIOS #80 - EsetShift
 */
#if (TOSVERSION >= 0x300) & !TP_50
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
        move.w    #1,(_vblsem).w
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
        move.b    5(a7),(shift_tt+1).w
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
        lea.l     $ffff8400(a0),a0
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
#endif


#include "dma.inc"

        text

#if HD_WAIT == 1
#include "../tospatch/hdwait1.S"
#endif
#if HD_WAIT == 2
#include "../tospatch/hdwait2.S"
#endif
#if HD_WAIT == 3
#include "../tospatch/hdwait3.S"
#endif
#if HD_WAIT == 4
#include "../tospatch/hdwait4.S"
#endif

#if TP_46 /* NOVA */
#include "../tospatch/nova.S"
#endif

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

		xdef execname
		comm execname,4

		xdef execpath
		comm execpath,4

		xdef execfname
		comm execfname,32

		xdef _tim_c_sieve
		comm _tim_c_sieve,2
		
		xdef _shifty
		comm _shifty,1

		xdef execdta
		comm execdta,44

		xdef _sndtmp
		comm _sndtmp,1
		
		xdef _snddelay
		comm _snddelay,1
		
		xdef _sndtable
		comm _sndtable,4
		
		xdef _prtconfig
		comm _prtconfig,2
		
		xdef _bootpref
		comm _bootpref,2

#if (TOSVERSION >= 0x300) | TP_50
		xdef _altmd
		comm _altmd,16
#endif

		xdef dsb
		comm dsb,16

		xdef kbdbuf
		comm kbdbuf,7
		xdef mousebuf
		comm mousebuf,5
		xdef relmbuf
		comm relmbuf,3
		xdef clockbuf
		comm clockbuf,6
		xdef joybuf
		comm joybuf,3
		xdef buttonbuf
		comm buttonbuf,3
		xdef initmbuf
		comm initmbuf,17
		xdef kb_key_last
		comm kb_key_last,1
		xdef kb_ticks
		comm kb_ticks,1
		xdef kb_delay
		comm kb_delay,1
		xdef kb_initial
		comm kb_initial,1
		xdef kb_repeat
		comm kb_repeat,1
		xdef kb_altnum
		comm kb_altnum,2
		
		xdef savepc
		comm savepc,4

		xdef lst_timeout
		comm lst_timeout,4
		
#if TTMFP_SUPPORT
		xdef mdm3ibuf
		comm mdm3ibuf,SERIAL_BUFSIZE
		xdef mdm3obuf
		comm mdm3obuf,SERIAL_BUFSIZE
		xdef mdm3iorec
		comm mdm3iorec,36
#endif

		xdef mdm4ibuf
		comm mdm4ibuf,SERIAL_BUFSIZE
		xdef mdm4obuf
		comm mdm4obuf,SERIAL_BUFSIZE
		xdef mdm4iorec
		comm mdm4iorec,36

		xdef mdm2ibuf
		comm mdm2ibuf,SERIAL_BUFSIZE
		xdef mdm2obuf
		comm mdm2obuf,SERIAL_BUFSIZE
		xdef mdm2iorec
		comm mdm2iorec,36

		xdef rs232ibuf
		comm rs232ibuf,SERIAL_BUFSIZE
		xdef rs232obuf
		comm rs232obuf,SERIAL_BUFSIZE
		xdef rs232iorec
		comm rs232iorec,36

		xdef _curkeytbl
		comm _curkeytbl,12
