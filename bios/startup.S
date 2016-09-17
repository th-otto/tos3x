*
* startup.S - EmuTOS startup module
*
* Copyright (C) 2001-2016 by the EmuTOS development team
*
* This file is distributed under the GPL, version 2 or at your
* option any later version.  See doc/license.txt for details.
*



#include "asmdefs.h"
#include "header.h"


// ==== Defines ==============================================================

cart_base  equ   $00fa0000 // Base of ROM port cartridge

// ==== Definitions ==========================================================

        xdef  _os_entry
        xdef  _run_cartridge_applications

// ==== References ===========================================================

        // TOS System variables
        xref resvalid
        xref resvector
        xref _stktop

        // Memory
        xref memchk
        
        // BIOS C part
        xref _biosmain     // BIOS C entry point

        // Overall memory map
        xref __end         // end of bss + comm sections (ld generated)
        xref __etext       // end of text section
        xref __edata       // end of data section
        xref _endvdibss    // end of VDI BSS
        xref _endgembss    // end of GEM BSS
        xref __ebss        // end of BSS

// ===========================================================================
// ==== TEXT segment (TOS image) =============================================
// ===========================================================================

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
    bra.s   _main       // os_entry, branch to _main
os_version:
    dc.w    TOS_VERSION // os_version, TOS version
reseth:
    dc.l    _main       // reseth, pointer to reset handler
_os_beg:
    dc.l    _os_entry   // os_beg, base of os = _sysbase
os_end:
    dc.l    _endvdibss  // os_end, end of VDI BSS
os_res1:
    dc.l    _main       // os_res1, reserved
_os_magic:
    dc.l    _ui_mupb    // os_magic, pointer to GEM's MUPB
_os_date:
    dc.l   OS_DATE     // os_date, Date of system build
_os_conf:               // Flag for PAL version + country
#ifdef CONF_MULTILANG
    dc.w   OS_CONF_MULTILANG
#else
    dc.w   (OS_COUNTRY<<1)+OS_PAL
#endif
_os_dosdate:
    dc.w   OS_DOSDATE  // os_date, Date of system build in GEMDOS format
os_root:
    dc.l   _root       // Pointer to the GEMDOS mem pool
os_kbshift:
    dc.l   _shifty     // Pointer to the keyboard shift keys states
os_run:
    dc.l   _run        // Pointer to a pointer to the actual basepage
os_dummy:
    dc.l   0           // _main should start at offset 0x30, shouldn't it?




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
        // disable the interrupts
        // this is useless on cold boot,
        // but it matters for software reset
        move    #$2700,sr

        // special TT reset
        move.w #$0100,($FFFF8606).w
        move.w #$0000,($FFFF8606).w
        reset                  // reset all hardware

/*
 * Check for diagnostic cartridge
 */

        cmp.l   #$fa52235f,cart_base  // magic - is cartridge present?
        bne     nodiag                  // no -> go on
        lea     nodiag(pc), a6          // save return address
        jmp     cart_base+4             // execute diagnostig cartridge
nodiag:

/*
 * disable the instruction/data caches
 */
        move.l  #$00000808,d0     // clear & disable instruction/data caches on
        MOVEC_D0_CACR              //  68020-60 (even though bit usage differs!)

        moveq   #0,d0
        MOVEC_D0_VBR           // initialise VBR to zero
/*
 * disable the MMU
 */
        PMOVE_TO_TC(zero)          //  we're done if we get a lineF exception ...
        PMOVE_TO_TTR0(zero)        // first we initialise the TTRs (ACRs on a 68ec030)
        PMOVE_TO_TTR1(zero)
        FRESTORE
        dc.l zero
        
        btst      #0,($FFFF8E09).w
        beq noreset
        lea memcret(pc),a6
        bra memchk
memcret:
        bne noreset
        move.b    (memctrl).w,($FFFF8001).w
        
/*
 * Check, if we should jump to a reset vector after warmboot
 */

resetvec:
       cmpi.l   #$31415926,(resvalid).w   // Jump to resetvector?
       bne.s    noreset                 // No --> noreset
       move.l   (resvector).w,d0           // Yes: old resvec to d0
       beq.s    noreset                 // Address null? Yes --> noreset
       btst     #0,d0                  // Address odd ?
       bne.s    noreset                 // Yes --> noreset
       movea.l  d0,a0                  // resvec
       lea      resetvec(pc),a6        // save return address
       // Note: the return address intentionally points to the resvalid test.
       // So the reset routine must explicitly clear resvalid before returning
       // to a6, otherwise there will be an infinite loop.
       jmp      (a0)                    // jump to resvec
noreset:


/*
 * memory configuration
 */

*
* do/test memory config and clear it
*
meminit:


/*
 * From now, the stack is allowed, and C code can be used.
 * The BIOS startup goes on in bios.c
 */

        jmp     _biosmain


/*
 * void run_cartridge_applications(WORD typebit);
 *
 * Run all cartridge applications of the specified type.
 *
 * typebit: application type bit number which must be set to 1 to match
 */
_run_cartridge_applications:
        move.w  4(sp),d0                // application bit number
        lea     cart_base,a0
        cmp.l   #$ABCDEF42,(a0)+      // is cartridge present?
        bne     cartover                // no -> cartover
testtype:
        btst    d0,4(a0)               // What type?
        beq     nextapp

        movem.l d0-d7/a0-a6,-(sp)      // save registers
        move.l  4(a0),d0                // d0 = address of cartridge init
        andi.l  #$00ffffff,d0          // make it 24-bit clean
        movea.l d0,a0
        jsr     (a0)                    // execute app in cartridge
        movem.l (sp)+,d0-d7/a0-a6      // restore registers
nextapp:
        tst.l   (a0)                    // another application?
        move.l  (a0), a0
        bne     testtype
cartover:
        rts


        data
_ui_mupb:
        dc.l $87654321
        dc.l _endgembss
        dc.l _ui_start
zero:
	
