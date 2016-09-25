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
#include "tosvars.inc"


* ==== Defines ==============================================================
TOSVERSION equ $306
mc68010 equ 1

cart_base  equ   $00fa0000 * Base of ROM port cartridge
GEM_MUPB_MAGIC equ $87654321

NCOOKIES equ 16
nvram_rtc_year_offset equ 1980-1968

#define RR_MAGIC    $12123456
#define RR_CHKSUM   $5678

save_siz equ 238

#define IOREC_SIZE 14
#define EXTIOREC_SIZE 2*14+8

* ==== Definitions ==========================================================

        xdef  _os_entry
        xdef  _run_cartridge_applications

* ==== Iorec offsets ========================================================

buf     equ 0
size    equ 4
head    equ 6
tail    equ 8
low     equ 10
high    equ 12

* ==== References ===========================================================

* Memory
        xref memchk
        
* BIOS C part
*        xref _biosmain     * BIOS C entry point

* Overall memory map
*       xref __end         * end of bss + comm sections (ld generated)
*       xref __etext       * end of text section
*       xref __edata       * end of data section
        xref _endvdibss    * end of VDI BSS
        xref _endgembss    * end of GEM BSS
        xref __ebss        * end of BSS

* ==== Structures ===========================================================

#define kbdvec    _kbdvecs+0
#define midivec   _kbdvecs+4
#define vkbderr   _kbdvecs+8
#define vmiderr   _kbdvecs+12
#define statvec   _kbdvecs+16
#define mousevec  _kbdvecs+20
#define clockvec  _kbdvecs+24
#define joyvec    _kbdvecs+28
#define ikbdsys   _kbdvecs+32
#define midisys   _kbdvecs+36
#define ikbdstate _kbdvecs+40
#define kbdlength _kbdvecs+41


* ===========================================================================
* ==== TEXT segment (TOS image) =============================================
* ===========================================================================

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
    bra.s   _main       * os_entry, branch to _main
os_version:
    dc.w    TOSVERSION  * os_version, TOS version
reseth:
    dc.l    _main       * reseth, pointer to reset handler
_os_beg:
    dc.l    _os_entry   * os_beg, base of os = _sysbase
os_end:
    dc.l    _endvdibss  * os_end, end of VDI BSS
os_res1:
    dc.l    _main       * os_res1, reserved
_os_magic:
    dc.l    _ui_mupb    * os_magic, pointer to GEM's MUPB
_os_date:
    dc.l   OS_DATE      * os_date, Date of system build
_os_conf:               * Flag for PAL version + country
#ifdef CONF_MULTILANG
    dc.w   OS_CONF_MULTILANG
#else
    dc.w   (OS_COUNTRY<<1)+OS_PAL
#endif
_os_dosdate:
    dc.w   OS_DOSDATE   * os_date, Date of system build in GEMDOS format
os_root:
    dc.l   _root        * Pointer to the GEMDOS mem pool
os_kbshift:
    dc.l   _shifty      * Pointer to the keyboard shift keys states
os_run:
    dc.l   _run         * Pointer to a pointer to the actual basepage
os_dummy:
    dc.l   0            * _main should start at offset 0x30, shouldn't it?




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
* disable the interrupts
* this is useless on cold boot,
* but it matters for software reset
        move    #$2700,sr

* special TT reset
        move.w #$0100,($FFFF8606).w
        move.w #$0000,($FFFF8606).w
        reset                  * reset all hardware

/*
 * Check for diagnostic cartridge
 */
        cmp.l   #$fa52235f,cart_base    * magic - is cartridge present?
        bne.s   nodiag                  * no -> go on
        lea     nodiag(pc),a6           * save return address
        jmp     cart_base+4             * execute diagnostig cartridge
nodiag:

/*
 * disable the instruction/data caches
 */
        move.l  #$00000808,d0     * clear & disable instruction/data caches on
        D0_CACR              *  68020-60 (even though bit usage differs!)

        moveq   #0,d0
        D0_VBR           * initialise VBR to zero
/*
 * disable the MMU
 */
        TO_TC(zero)          *  we're done if we get a lineF exception ...
        TO_TTR0(zero)        * first we initialise the TTRs (ACRs on a 68ec030)
        TO_TTR1(zero)
        FRESTORE(zero)
        
        btst      #0,($FFFF8E09).w
        beq.s     noreset
        lea       memcret(pc),a6
        bra       memchk
memcret:
        bne.s      noreset
        move.b    (memctrl).w,($FFFF8001).w
        
/*
 * Check, if we should jump to a reset vector after warmboot
 */

resetvec:
        cmpi.l   #$31415926,(resvalid).w  * Jump to resetvector?
        bne.s    noreset                * No --> noreset
        move.l   (resvector).w,d0       * Yes: old resvec to d0
        btst     #0,d0                  * Address odd ?
        bne.s    noreset                * Yes --> noreset
        movea.l  d0,a0                  * resvec
        lea      resetvec(pc),a6        * save return address
* Note: the return address intentionally points to the resvalid test.
* So the reset routine must explicitly clear resvalid before returning
* to a6, otherwise there will be an infinite loop.
        jmp      (a0)                   * jump to resvec
noreset:


/*
 * memory configuration
 */

*
* do/test memory config and clear it
*
meminit:

        lea.l     ($FFFF8800).w,a0
        move.b    #$07,(a0)  ; select mixer control
        move.b    #$C0,2(a0) ; Port A+B = output
        move.b    #$0E,(a0)  ; select Port A
        move.b    #$07,2(a0) ; deselect floppy drives
        
        move.b    #$01,($FFFF820A).w ; set video sync

/*
 * next, set up the palette(s)
 */

        lea.l     ($FFFF8240).w,a1
        move.w    #$000F,d0
        lea.l     dflt_pal(pc),a0
initpal:
        move.w    (a0)+,(a1)+
        dbf       d0,initpal

* set Video screen memory position to 0x10000
        move.b    #$01,($FFFF8201).w
        clr.b     ($FFFF8203).w

        btst      #0,($FFFF8E09).w
        beq.s      domemchk
        move.b    (memctrl).w,d6
        move.l    (_phystop).w,d5
        lea       memcret2(pc),a6
        bra       memchk
memcret2:
        beq       nomemchk
domemchk:

* ===========================================================================
* ==== memconf for TT =======================================================
* ===========================================================================
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
        move.w    #$0005,d6           * assume 2MB board
        move.b    #$0A,($FFFF8001).w  * set memory controller for 8MB
        moveq.l   #0,d0               
        move.l    d0,($00001008).w    * zero out test addresses
        move.l    d0,($0000100C).w
        move.l    #$06161964,d0       * write test value #1 to an address
        move.l    d0,($00000008).w    *  that may be mapped into test address #1
        cmp.l     ($00001008).w,d0    * did test address #1 get updated?
        bne.s     set_8MB             * no, must be an 8MB board
        move.l    #$04251987,d0       * yes, repeat with test value #2 to be sure
        move.l    d0,($0000000C).w    * did test address #2 get updated?
        cmp.l     ($0000100C).w,d0    * yes, use the 2MB memcntlr value 
        beq.s     set_2MB
set_8MB:
        move.w    #$000A,d6           * set 8MB memcntlr value
set_2MB:
        move.b    d6,($FFFF8001).w    * update memcntlr

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
        lea.l     st_ramsize_done(pc),a0 * bus error means end of ram
        move.l    a0,(_buserror).w
        move.w    #$FB55,d3           * d3.w = difference between adjacent stored values
        move.l    #$00020000,d7       * d7 = address of first memory test zone & gap between memory test zones
                                        
        movea.l   d7,a0               * a0 = address of current test zone
next_zone:
        movea.l   a0,a1               * a1 -> memory being tested
        move.w    d0,d2               * d2.w = test value
        moveq.l   #42,d1              * d1.w = loop count-1
store_mem:
        move.w    d2,-(a1)            * store a value
        add.w     d3,d2               * update the value for next time
        dbf       d1,store_mem        * rinse & repeat
* now check the values we stored
        movea.l   a0,a1               * a1 -> test area
        moveq.l   #42,d1              * loop count
check_mem:
        cmp.w     -(a1),d0            
        bne.s     st_ramsize_done   * miscompare, ram invalid, exit
        clr.w     (a1)                * else zero it out
        add.w     d3,d0               * update for next compare
        dbf       d1,check_mem        * until we're done
        adda.l    d7,a0               * point to next test area
        bra.s     next_zone
st_ramsize_done:                    * bus error or bad ram 
        suba.l    d7,a0               * (a0 -> first zone that failed)
        move.l    a0,d5               * d5 = st ram size
        move.l    a4,(_buserror).w

* ===========================================================================
* ==== This part is executed only on first boot =============================
* ===========================================================================

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
        move.b  d6,(memctrl).w             * set copy of hw memory config
        move.l  d5,(_phystop).w            * highest address as phystop

        move.l    #$752019F3,(_memvalid).w * set memvalid to ok
        move.l    #$237698AA,(_memval2).w  * set memval2 to ok
        move.l    #$5555AAAA,(_memval3).w  * set memval3 to ok

* We must set the FastRAM refresh rate before we use it.  The
* rate is set by writing a "don't care" value to an address in
* the range 0xffd00000-0xffd000ff; the low-order byte of the
* address sets the refresh rate (specifically the number of
* system clock cycles between each refresh request).  TT TOS
* uses the value 0xe0; so do we.
* NOTE: whether we actually have FastRAM or not, setting the
* value always causes a bus error, so we need to handle it.
        move.l  #refresh_done,(_buserror).w      * bus error vector
        move.w  #$0000,$ffd000e0
refresh_done:

        /* Can we access FastRAM region? */

        move.l    #tt_ramsize_done,(_buserror).w
        move.w    #$FB55,d3           * d3.w = difference between adjacent stored values
        moveq.l   #0,d0
        move.l    #$00020000,d7       * d7 = address gap between memory test zones
        movea.l   #$01020000,a0       * a0 = address of current test zone
next_tt:
        movea.l   a0,a1               * a1 -> memory being tested
        move.w    d0,d2               * d2.w = test value
        moveq.l   #42,d1              * d1.w = loop count-1
store_tt:
        move.w    d2,-(a1)            * store a value
        add.w     d3,d2               * update the value for next time
        dbf       d1,store_tt
* now check the values we stored
        movea.l   a0,a1               * a1 -> test area
        moveq.l   #42,d1              * loop count
check_tt:
        cmp.w     -(a1),d0
        bne.s     tt_ramsize_done
        clr.w     (a1)
        add.w     d3,d0
        dbf       d1,check_tt
        adda.l    d7,a0
        bra.s     next_tt
tt_ramsize_done:                    * bus error or bad ram 
        suba.l    d7,a0
        cmpa.l    #$01000000,a0
        bne.s     ttramok
        suba.l    a0,a0
ttramok:
        move.l    a0,d5
        move.l    d5,(_ramtop).w
        move.l    #$1357BD13,(_ramvalid).w
        bset      #0,($FFFF8E09).w
nomemchk:


* detect basic STe/TT DMA sound

        movea.l   #_supstkend,a7
        clr.b     _has_dmasound
        movea.l   a7,a6
        move.l    #nodmasound,(_buserror).w
        clr.w     ($FFFF8900).w
        st        _has_dmasound
        lea.l     microwtab(pc),a0
        move.w    (a0)+,($FFFF8924).w
        bra.s     checkmicro
microwtab:
        dc.w      $0ffe  ; value for microwire mask register
        dc.w      $09d1,$0aa9,$0a29,$090d,$088d,$0803,0  ; values for microwire data register
nextmicro:
        move.w    d0,($FFFF8922).w
waitmicro:
        tst.w     ($FFFF8922).w
        bne.s     waitmicro
checkmicro:
        move.w    (a0)+,d0
        bne.s     nextmicro
nodmasound:
        movea.l   a6,a7
        sf        _has_microwire


* clear the remainder of the BSS section

        movea.l   #__ebss,a1
        movea.l   #__sbss,a0
        moveq.l   #0,d0
clrbss:
        move.w    d0,(a0)+
        cmpa.l    a0,a1
        bne.s     clrbss


* initialize the MMU
        bsr       initmmu

* clear screen memory

scrsize equ ((1280*960)/8+$100)

        movea.l   (_phystop).w,a0
        suba.l    #scrsize,a0
        move.w    #(scrsize/16)-1,d1
        move.l    a0,(_v_bas_ad).w
        move.b    (_v_bas_ad+1).w,($FFFF8201).w
        move.b    (_v_bas_ad+2).w,($FFFF8203).w
        moveq.l   #0,d0
clrscr:
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        move.l    d0,(a0)+
        dbf       d1,clrscr

* does os_magic indicate GEM in ROM?
        movea.l   _os_magic(pc),a0
        cmpi.l    #GEM_MUPB_MAGIC,(a0)
        beq.s     gemok
        lea.l     _os_beg,a0
gemok:  
        move.l    4(a0),(_end_os).w
        move.l    8(a0),(_exec_os).w

* ===========================================================================
* initialize system variables
* ===========================================================================

        move.l    #_bhdv_init,(_hdv_init).w
        move.l    #_bhdv_rwabs,(_hdv_rw).w
        move.l    #_bhdv_getbpb,(_hdv_bpb).w
        move.l    #_bhdv_mediach,(_hdv_mediach).w
        move.l    #_bhdv_boot,(_hdv_boot).w

        move.l    #_bco0stat,(_prt_stat).w
        move.l    #_bcon0out,(_prt_vec).w
        move.l    #_bco1stat,(_aux_stat).w
        move.l    #_bcon1out,(_aux_vec).w
        move.l    #_bscr_dump,(_scr_dmp).w

        move.l    (_v_bas_ad).w,(_memtop).w
        move.l    (_end_os).w,(_membot).w
        move.w    #8,(_nvbls).w
        st        (_fverify).w
        move.w    #3,(_seekrate).w
        move.l    #_dskbuf,(_dskbufp).w
        move.w    #-1,(_dumpflg).w
        move.l    #_os_entry,(_sysbase).w          ; unnessary; will be overwritten by instoshdr below
        move.l    #save_beg+save_siz,(_savptr).w
        move.l    #bios_unimpl,(_swv_vec).w
        clr.l     (_drvbits).w
        move.l    #_do_bell,(_bell_hook).w
        move.l    #_do_keyclick,(_kcl_hook).w

        bsr       instoshdr

* ===========================================================================
* initialize cookie jar
* ===========================================================================

        lea.l     cookies,a0
        move.l    a0,(_p_cookies).w
        move.l    #$5F435055,(a0)+ ; '_CPU'

* detect CPU type
* (actually nonsense since 68030 instructions have already been used above)
        moveq.l   #0,d1         * assume 68000
        movea.w   #_illinst,a2
        movea.l   (a2),a3       * save illegal instruction vector
        movea.l   a7,a1         * save SP
        move.l    #cpuexit,(a2)
        CCR_D0                  * move.w ccr,d0 legal on 68010+
        moveq.l   #10,d1        * assume 68000
        dc.w $49c0              * extb.l    d0
        moveq.l   #20,d1        * assume 68020
        CACR_D0
        bset      #9,d0         * set an unused bit
        D0_CACR
        CACR_D0
        bclr      #9,d0
        beq.s     cpuexit
        moveq.l   #30,d1        * assume 68030
        D0_CACR
cpuexit:
        movea.l   a1,a7         * restore SP
        move.l    a3,(a2)       * save illegal instruction vector
        move.l    d1,(a0)+      * set _CPU cookie
        sne       (_longframe+1).w
        move.l    #$5F56444F,(a0)+ ; '_VDO'
        move.l    #$00020000,(a0)+
        move.l    #$5F4D4348,(a0)+ ; '_MCH'
        move.l    #$00020000,(a0)+
        move.b    #$7F,d0
        tst.b     _has_microwire
        bne.s     nomicro
        move.l    #$5F535749,(a0)+ ; '_SWI'
        moveq.l   #0,d0
        move.w    ($FFFF9200).w,d0
        lsr.w     #8,d0
        move.l    d0,(a0)+
nomicro:
        moveq.l   #3,d1
        move.l    #$5F534E44,(a0)+ ; '_SND'
        btst      #7,d0
        bne.s     yesstereo
        bclr      #1,d1
yesstereo:
        move.l    d1,(a0)+
        btst      #6,d0
        bne.s     nofdc
        move.b    #$08,$00001820
        move.l    #$5F464443,(a0)+ ; '_FDC'
        move.l    #$01415443,(a0)+ ; FDC_1ATC
nofdc:

* detect FPU

        move.l    #$5F465055,(a0)+ ; '_FPU'
        movea.l   (_lineftrap).w,a1 ; save Line-F trap
        movea.l   (_coprovio).w,a2
        movea.l   a7,a3         * save SP
        move.l    #nofpu,(_lineftrap).w
        move.l    #nofpu,(_coprovio).w
        FP0_D0
        move.l    #$00020000,(a0)+
        bra.s     fpudone
nofpu:
        clr.l     (a0)+
fpudone:
        move.l    a1,(_lineftrap).w ; restore Line-F trap
        move.l    a2,(_coprovio).w
        movea.l   a3,a7         * restore SP

* check memory-mapped FPU
        movea.l   (_buserror).w,a1
        movea.l   a7,a2         * save SP
        move.l    #nosfp,(_buserror).w
        move.w    ($FFFFFA40).w,d0
        bset      #0,-3(a0)
nosfp:
        move.l    a1,(_buserror).w
        movea.l   a2,a7         * restore SP

* check wether fast ram buffer is needed
        tst.l     (_ramtop).w
        beq.s     frbdone
        move.l    #$5F465242,(a0)+ ; '_FRB'
        move.l    (_membot).w,d0
        move.l    d0,(a0)+
        add.l     #$00010000,d0
        move.l    d0,(_membot).w
        move.l    d0,(_end_os).w
frbdone:

* terminate cookie jar
        clr.l     (a0)+
        move.l    #NCOOKIES,(a0)+

* ===========================================================================
* initialize exception vectors
* ===========================================================================

        lea.l     just_rte(pc),a3
        lea.l     bios_unimpl(pc),a4
        lea.l     any_vec(pc),a1       * default to exception handler displaying bombs
        lea.l     (2*4).w,a0
        move.w    #64-3,d0
initexc:
        move.l    a1,(a0)+
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
        move.w    #$0007,d0
initvbl:
        clr.l     (a0)+
        dbf       d0,initvbl

* init bios device vectors
        lea.l     conxxinit,a0
        movea.w   #_bconstat_vec,a1
        moveq.l   #31,d0
initcon:
        move.l    (a0)+,(a1)+
        dbf       d0,initcon

* init VME bus
        movea.l   (_buserror).w,a0
        movea.l   a7,a1
        move.l    #vmedone,(_buserror).w
        move.b    #$40,($FFFF8E0D).w
        move.b    #$14,($FFFF8E01).w
vmedone:
        move.l    a0,(_buserror).w
        movea.l   a1,a7

* initialize MFP
        bsr       initmfp
        move.w    #(4<<8),d0 ; Divisor 50 for Timer C
        bsr       ttdelayc

* reset IKBD
        move.l    #ikbdres,-(a7)
        move.w    #$0001,-(a7)
        jsr       _ikbdws
        addq.l    #6,a7
        move.w    #(7<<8),d0 ; Divisor 200 for Timer C
        move.w    #$000E,d1
ikbdwait:
        bsr       ttdelayc
        dbf       d1,ikbdwait

/* Run all boot applications from the application cartridge,
 * before video initialization.
 */
        moveq.l   #2,d0
        bsr       _run_cartridge_applications

/*
 * check monitor type, and set boot resolution
 */
        moveq.l   #4,d1              * assume color, video mode 640x480x16
        btst      #7,(MFP_LPT).w
        bne.s     moncolor
        moveq.l   #6,d1
moncolor:
        move.b    d1,($FFFF8262).w   * set TT shifter video mode
        move.b    d1,(_sshiftmod).w  * .. and also system var

/*
 * initialize video
 */
        bsr       ckblitter
        jsr       resetdev
        jsr       vdireinit
        move.l    #_main,(_swv_vec).w
        move.w    #$0001,(_vblsem).w

/* Run all boot applications from the application cartridge,
 * after hardware initialization.
 */
        clr.w     d0
        bsr       _run_cartridge_applications

/* Run all boot applications from the application cartridge,
 * with interrupts enabled.
 */
        move.w    #$2300,sr
        moveq.l   #1,d0
        bsr       _run_cartridge_applications

/*
 * continue with rest of bios initialization
 */
        move.l    #_int_priv,(_privvio).w
        bra       biosinit

flush_cache_all_030:
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        CACR_D0
        or.l      #$00000808,d0
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
        move.l    #$30004E71,privinst.w   ; move.w d0,d0; nop
        move.l    #$4E714E75,(privinst+4).w ; nop; rts
        move.w    d1,d0
        and.w     #$0007,d0
        lsl.w     #8,d0
        lsl.w     #1,d0
        or.w      d0,privinst.w    * insert the destination register
        move.w    d1,d0
        and.w     #$0038,d0
        lsl.w     #3,d0
        or.w      d0,privinst.w    * insert the destination addressing mode
        moveq.l   #2,d2            * skip that many instruction bytes
        cmp.w     #$0180,d0        * is it indexed?
        beq       pi_back          * yes, fail
        tst.w     d0               * is it data-register direct?
        beq.s     int_p4           * yes go execute it
        cmp.w     #$0140,d0        * is it d(An)?
        beq.s     int_p2           * yes
        cmp.w     #$01C0,d0        * is it absolute?
        bne.s     int_p3
        and.w     #$0007,d1        * is it absolute short?
        beq.s     int_p1           * yes, skip
        addq.w    #2,d2            * 2 more bytes of instruction
        move.w    4(a0),(privinst+4).w * copy lower 16 bits of address
int_p1:
        addq.w    #2,d2            * 2 more bytes of instruction
        move.w    2(a0),(privinst+2).w * copy remaining 16 bits of address
        bra.s     int_p5
int_p2:
        addq.w    #2,d2            * 2 more bytes of instruction
        move.w    2(a0),(privinst+2).w * copy displacement
int_p3:
        and.w     #$0007,d1        * is USP affected?
        cmp.w     #$0007,d1
        bne.s     int_p5
        move.l    usp,a1
        andi.w    #$F3FF,privinst.w
        add.l     d2,22(a7)
        bsr       flush_cache_all_030
        move.w    20(a7),d0
        jsr       privinst.w
        move.l    a1,usp
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        movem.l   (a7)+,d0-d2
        rte
int_p4:
        add.l     d2,22(a7)           * adjust return PC by number of bytes to skip
        ori.w     #$0010,privinst.w
        bsr       flush_cache_all_030
        lea.l     20(a7),a0           * a0 = address of SR from stack frame
        movem.l   8(a7),d0-d2
        jsr       privinst.w
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        adda.w    #$000C,a7
        rte
int_p5:
        add.l     d2,22(a7)           * adjust return PC by number of bytes to skip
        bsr       flush_cache_all_030
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        move.w    12(a7),d0
        jsr       privinst.w
        movem.l   (a7)+,d0-d2
        rte
pi_back:
        movea.l   (a7)+,a0
        movea.l   (a7)+,a1
        movem.l   (a7)+,d0-d2
        jmp       any_vec

biosinit:
		jsr       _osinit
        move.w    _os_dosdate,_date

        jsr       ngetdt
        beq.s     clockdone
        bsr       igetdt
        swap      d0
        tst.b     d0
        beq.s     clockdone
        move.w    d0,_date
        swap      d0
        move.w    d0,_time
clockdone:
        clr.b     (MFP_TACR).w     ; stop Timer A
        bclr      #5,(MFP_IERA).w  ; disable Timer A interrupt
        move.l    #$00003111,d0
        D0_CACR


/* 
 * Draw the Atari logo.
 */
        movea.l   #atarilogo,a0
        movea.l   (_v_bas_a).w,a1
        move.b    (_sshiftmod).w,d0
        cmp.b     #$02,d0          ; ST-High resolution?
        beq.s     logomono
        cmp.b     #$06,d0
        beq.s     logomono         ; TT-High resolution?
        adda.w    #((640/8)*4)*4,a1  ; start at screen line 4
        move.w    #$0055,d0        ; 86 lines of data
logocol1:
        moveq.l   #5,d1            ; 12 bytes of data per line
logocol2:
        move.w    (a0)+,d2
        move.w    d2,(a1)+         ; write 4 plane words
        move.w    d2,(a1)+
        move.w    d2,(a1)+
        move.w    d2,(a1)+
        dbf       d1,logocol2
        adda.w    #(640/8-12)*4,a1
        dbf       d0,logocol1
        bra.s     logodone
logomono:
        adda.w    #(1280/8)*4,a1   ; start at screen line 4
        move.w    #$0055,d0        ; 86 lines of data
logomon1:
        moveq.l   #11,d1           ; 12 bytes of data per line
logomon2:
        move.b    (a0)+,(a1)+
        dbf       d1,logomon2
        adda.w    #(1280/8)-12,a1
        dbf       d0,logomon1
logodone:

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
        trap      #13             ; Bconout(2, ESC)
        move.w    #$0059,4(a7)
        move.l    d6,(a7)
        trap      #13             ; Bconout(2, 'Y')
        move.w    d7,4(a7)
        move.l    d6,(a7)
        trap      #13             ; Bconout(2, row)
        move.w    #$0020,4(a7)
        move.l    d6,(a7)
        trap      #13             ; Bconout(2, col)
        addq.w    #6,a7

/*
 * check the ROM crc
 */
        move.l    #$0001FFFE,d7
        move.w    #$0003,d6
        movea.l   #_os_entry,a5
crccheck:
        move.w    #$0004,-(a7)
        move.l    d7,-(a7)
        move.l    a5,-(a7)
        bsr       check_rom_crc
        adda.w    #$000A,a7
        movea.l   a5,a0
        adda.l    #$0007FFF8,a0
        move.b    (a0),d1
        lsl.w     #8,d1
        move.b    4(a0),d1
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
        move.b    #$45,d0
        btst      #0,d5
        beq.s     chipE
        move.b    #$4F,d0
chipE:
        move.w    d0,2(a7)
        move.w    #$0002,(a7)
        trap      #1
        move.b    #$45,d0
        btst      #1,d5
        beq.s     chipE2
        move.b    #$4F,d0
chipE2:
        move.w    d0,2(a7)
        move.w    #$0002,(a7)
        trap      #1
        move.l    #crcmsg2,2(a7)
        move.w    #$0009,(a7)
        trap      #1
        addq.w    #6,a7
        addq.l    #1,a5
        dbf       d6,crccheck
crcok:


/*
 * switch to ST low res unless in TT high res
 * ??? why? a bit too late, after displaying error messages
 */
        cmpi.b    #$06,(_sshiftmod).w
        beq.s     nosetrez
        move.l    #$000BFFFF,-(a7)
        trap      #13          ; Kbshift(-1)
        addq.w    #4,a7
        btst      #3,d0
        beq.s     nosetrez
        clr.w     -(a7)
        pea.l     ($FFFFFFFF).w
        pea.l     ($FFFFFFFF).w
        move.w    #$0005,-(a7)
        trap      #14         ; Setscreen(-1L, -1L, 0)
        adda.w    #$000C,a7
        move.l    #$00000808,d0 ; BUG? this will be skipped too if in TT high rez
        D0_CACR
nosetrez:

/* boot eventually from a block device (floppy or harddisk) */
        bsr       diskboot
/* boot eventually from a SCSI/ACSI */
        bsr       dmaboot
/* run resident programs */
        bsr       run_reset_resident
        tst.w     _cmdload
        beq.s     nocmd
        bsr       autoexec
        move.l    #_os_entry,_sysbase
        pea.l     emptystr(pc)
        pea.l     emptystr(pc)
        pea.l     command_prg(pc)
        clr.w     -(a7)        ; Load&Go
        bra.s     dopexec
nocmd:
        bsr       autoexec
        move.l    #_os_entry,_sysbase
        lea.l     default_env(pc),a0
        movea.l   #gem_env,a1
copyenv:
        cmpi.b    #$23,(a0)
        bne.s     nextenvc
        movea.l   a1,a2            ; remember dest addr of drive character
nextenvc:
        move.b    (a0)+,(a1)+
        bpl.s     copyenv
        move.b    _bootdev,d0      ; infamous old bug accessing _bootdev as byte
        add.b     #$41,d0
        move.b    d0,(a2)
        pea.l     gem_env
        pea.l     emptystr
        pea.l     emptystr(pc)
        move.w    #$0005,-(a7)
        move.w    #$004B,-(a7)
        trap      #1               ; Pexec(5) Create basepage
        adda.w    #$000E,a7
        movea.l   d0,a0
        move.l    _exec_os,8(a0)   ; set p_tbase of created PD
        pea.l     gem_env
        move.l    a0,-(a7)
        pea.l     emptystr(pc)
        move.w    #$0004,-(a7)
dopexec:
        move.w    #$004B,-(a7)
        trap      #1               ; Pexec(4) Just go: execute GEM
        adda.w    #$000E,a7
        jmp       _main

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
        movea.l   (_hdv_boot).w,a0
        jsr       (a0)
        tst.w     d0
        bne.s     nodiskboot
        movea.l   (_dskbufp).w,a0
        jsr       (a0)
nodiskboot:
        rts


/* 306de: 00e009a2 */
dmaboot:
        move.l    #16000,d7
        cmp.l     _hz_200,d7
        bcs       dmadone
        movea.w   #$0000,a5
dmaloop:
        cmpa.w    #$0000,a5
        bne       skipsp
        bsr       x00E05FA4
        movea.w   d0,a5
        beq       dmawait
        move.w    #$001B,-(a7)
        move.l    #$00030002,-(a7)
        trap      #13               ; Bconout(2, ESC)
        move.w    #$0070,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, 'p')
        move.w    #$001B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, ESC)
        move.w    #$0077,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, 'w')
        move.l    #200,d5
        move.w    #$004F,d4
        tst.b     _sshiftmod
        bne.s     nolowres
        move.l    #400,d5
        moveq.l   #39,d4
nolowres:
        move.l    d5,d6
        move.l    d4,d3
prspace:
        move.w    #$0020,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, ' ')
        dbf       d4,prspace
        move.w    #$000D,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, '\r')
        subq.l    #1,d3
prspace2:
        move.w    #$0020,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, ' ')
        dbf       d3,prspace2
        move.w    #$001B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, ESC)
        move.w    #$0071,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, 'q')
        addq.l    #6,a7
skipsp:
        cmp.l     _hz_200,d6
        bhi.s     dmawait
        move.w    #$001B,-(a7)
        move.l    #$00030002,-(a7)
        trap      #13               ; Bconout(2, ESC)
        move.w    #$004B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, 'K')
        move.w    #$0008,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, '\b')
        addq.w    #6,a7
        add.l     d5,d6
dmawait:
        cmpa.w    #$0000,a5
        beq.s     dmakey
        cmp.l     _hz_200,d7
        bls.s     dmaline
dmakey:
        move.l    #$00010002,-(a7)
        trap      #13               ; Bconstat(2)
        addq.l    #4,a7
        tst.l     d0
        beq       dmaloop
        move.l    #$00020002,-(a7)
        trap      #13               ; Bconin(2)
        addq.l    #4,a7
        cmpa.w    #$0000,a5
        bne.s     dmaline
        bsr       x00E061F2
        move.l    d7,_hz_200
        bra.s     dmadone
dmaline:
        move.l    d7,_hz_200
        move.w    #$000D,-(a7)
        move.l    #$00030002,-(a7)
        trap      #13               ; Bconout(2, '\r')
        move.w    #$001B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, ESC)
        move.w    #$004B,4(a7)
        move.l    #$00030002,(a7)
        trap      #13               ; Bconout(2, 'K')
        addq.l    #6,a7
dmadone:
        jsr       x00E01CFE
        moveq.l   #8,d4      ; d4 = device (SCSI0)
        move.b    ($FFFF8E09).w,d0
        and.w     #$00F8,d0
        move.w    d0,(_bootpref).w
        bne.s     confdone
        pea.l     (_bootpref).w
        move.w    #$0002,-(a7)
        clr.l     -(a7)
        jsr       _nvmaccess ; read first 2 bytes of nvram (bootpref)
        adda.w    #$000A,a7
        tst.w     d0
        beq.s     confdone
        clr.w     (_bootpref).w
confdone:
        move.w    #$0001,d1
dmadev:
        move.w    d1,-(a7)
        move.w    d4,-(a7)
        move.l    (_dskbufp).w,-(a7)
        move.w    #$0001,-(a7)
        clr.l     -(a7)
        jsr       _dmaread
        adda.w    #$000C,a7
        move.w    (a7)+,d1
        tst.l     d0
        beq.s     dmaok
        addq.l    #1,d0
        dbeq      d1,dmadev
        bra.s     nextdev
dmaok:
        movea.l   (_dskbufp).w,a0
        move.w    #$00FF,d0
        moveq.l   #0,d1
dmacrc:
        add.w     (a0)+,d1
        dbf       d0,dmacrc
        cmp.w     #$1234,d1
        beq.s     doboot
nextdev:
        addq.w    #1,d4
        move.w    d4,d0
        and.w     #$0007,d0
        bne.s     confdone
        cmp.w     #$0008,d4
        beq.s     nodmaboot
        moveq.l   #0,d4       ; retry all again with ACSI devices
        bra.s     confdone
nodmaboot:
        rts
doboot:
        movea.l   (_dskbufp).w,a0
        move.l    #$444D4172,d3   ; 'DMAr'
        move.w    d4,d7
        asl.w     #5,d7
        move.w    (_bootpref).w,d5
        move.l    d4,-(a7)
        move.l    _hdv_rw,-(a7)
        jsr       (a0)
        move.l    (a7)+,d0
        move.l    (a7)+,d4
        cmp.l     (_hdv_rw).w,d0
        beq.s     nextdev
        rts

/*
 * void run_cartridge_applications(WORD typebit);
 *
 * Run all cartridge applications of the specified type.
 *
 * typebit(d0): application type bit number which must be set to 1 to match
 */
/* 306de: 00e00bbc */
_run_cartridge_applications:
        lea     cart_base,a0
        cmp.l   #$ABCDEF42,(a0)+        * is cartridge present?
        bne.s   cartover                * no -> cartover
testtype:
        btst    d0,4(a0)                * What type?
        beq.s   nextapp

        movem.l d0-d7/a0-a6,-(sp)       * save registers
        move.l  4(a0),d0                * d0 = address of cartridge init
        and.l   #$00ffffff,d0           * make it 24-bit clean
        movea.l d0,a0
        jsr     (a0)                    * execute app in cartridge
        movem.l (sp)+,d0-d7/a0-a6       * restore registers
nextapp:
        tst.l   (a0)                    * another application?
        move.l  (a0),a0
        bne.s   testtype
cartover:
        rts

/* 306de: 00e00bee */
bios_unimpl:
		rts

unknown:
        adda.l    d1,a0
        clr.w     d0
        lea.l     504(a0),a1
unknown1:
        cmp.w     (a0)+,d0
        bne.s     unknown2
        add.w     #$FA54,d0
        cmpa.l    a0,a1
        bne.s     unknown1
unknown2:
        jmp       (a4)

*
* check for valid memory configuration.
* Return address passed in A6
*
/* 306de: 00e00c06 */
memchk:
        cmpi.l    #$752019F3,(_memvalid).w
        bne.s     memchk1
        cmpi.l    #$237698AA,(_memval2).w
        bne.s     memchk1
        cmpi.l    #$5555AAAA,(_memval3).w
memchk1:
        jmp       (a6)

/* 306de: 00e00c24 */
dflt_pal:
		dc.w $0fff,$0f00,$00f0,$0ff0
		dc.w $000f,$0f0f,$00ff,$0555
		dc.w $0333,$0f33,$03f3,$0ff3
		dc.w $033f,$0f3f,$03ff,$0000

/*
 * Int 0x68 - HBL interrupt
 */
/* 306de: 00e00c44 */
int_hbl:
        move.w    d0,-(a7)            * save d0
        move.w    2(a7),d0            * get status register from stack
        and.w     #$0700,d0           * isolate just IPL-mask from sr
        bne.s     is_ipl              * if IPL is nonzero, then end
        ori.w     #$0300,2(a7)        * else set IPL3 in status register
is_ipl:
        move.w    (a7)+,d0            * restore d0
        rte

/*
 * int_vbl - Int 0x70 - VBL interrupt
 *
 * no video resolution change is done.
 */
/* 306de: 00e00c5a */
int_vbl:
        addq.l    #1,_frclock         * increase num of happened ints
        subq.w    #1,_vblsem          * check vbl semaphore
        bmi       vbl_end             * if VBl routine disabled -> end
        movem.l   d0-d7/a0-a6,-(a7)   * save registers
        addq.l    #1,_vbclock         * count number of VBL interrupts

* vblqueue
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
        move.b    (MFP_LPT).w,d1
        tst.b     _has_dmasound
        beq.s     nomoncheck
* Check for monitor switching,
* and jump to _swv_vec if necessary...
        move.w    sr,-(a7)
        ori.w     #$0700,sr
moncheck:
        move.b    ($FFFF8901).w,d0
        move.b    (MFP_LPT).w,d1
        btst      #7,d1
        sne       d1
        move.b    (MFP_LPT).w,d2
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
        move.b    ($FFFF8262).w,d0
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
        move.b    ($FFFF8262).w,d1
        and.b     #$F8,d1
        or.b      d0,d1
        move.b    d1,($FFFF8262).w
        movea.l   (_swv_vec).w,a0
        jsr       (a0)
vbl_nomonchg:
        jsr       _blink
* change palette registers
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
* set new video address
        tst.l     (_screenpt).w
        beq.s     vbl_no_screenpt
        move.l    (_screenpt).w,(_v_bas_ad).w
        move.b    (_v_bas_ad+2).w,($FFFF8203).w
        move.b    (_v_bas_ad+1).w,($FFFF8201).w
        move.b    (_v_bas_ad+3).w,($FFFF820D).w         ; BUG: no test that 820d actually exists
vbl_no_screenpt:
        bsr       _flopvbl
vbl_no_queue:
* screen hardcopy
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

* ==== Trap 14 - XBIOS entry point =========================================

/* 306de: 00e00d9c */
_xbiostrap:
        lea.l     xbios_vecs(pc),a0
        bra.s     biosxbios

* ==== Trap 13 - BIOS entry point ==========================================

/* 306de: 00e00da2 */
_biostrap:
        lea.l     bios_vecs(pc),a0

* ==== Trap 13+14 handler ==================================================
biosxbios:
        movea.l   _savptr,a1
        move.w    (a7)+,d0               * Status register -> d0
        move.w    d0,-(a1)               * and save in save_area
        move.l    (a7)+,-(a1)            * save return address
        tst.w     (a7)+                  * skip format word; BUG: no test for _longframe
        movem.l   d3-d7/a3-a7,-(a1)      * regs, including stack pointer
        move.l    a1,_savptr
        btst      #13,d0                 * were we in user mode?
        bne.s     bx_sp_ok               * yes, the sp already points to the arguments
        move.l    usp,a7                 * no, the arguments were on the user stack
bx_sp_ok:
        move.w    (a7)+,d0               * remove the function number from stack
        cmp.w     (a0)+,d0               * Higher than highest number?
        bge.s     bx_sp_ok               * if not implemented, returns the func number
        move.w    d0,d1                  
        lsl.w     #2,d1                  * function table is 1 LW per number so multiply function number by 4
        move.l    0(a0,d1.w),d1          * get function address
        bclr      #0,d1                  * indirect call?
        movea.l   d1,a0
        beq.s     bx_notindirect         
        movea.l   (a0),a0                * call indirect through system variable
bx_notindirect:
        suba.l    a5,a5
        jsr       (a0)
bx_ret:
        movea.l   _savptr,a1
        movem.l   (a1)+,d3-d7/a3-a7      * Get regs back, including sp
        clr.w     -(a7)                  * push NULL format word; BUG: no test for _longframe
        move.l    (a1)+,-(a7)            * return adress
        move.w    (a1)+,-(a7)            * sr
        move.l    a1,_savptr
        rte                              * return with return value in D0

bios_vecs:
        dc.w 12
        dc.l _getmpb
        dc.l _bbconstat
        dc.l _bbconin
        dc.l _bbconout
        dc.l _hdv_rw+1 ; Rwabs
        dc.l _setexc
        dc.l _tickcal
        dc.l _hdv_bpb+1 ; Getbpb
        dc.l _bbcostat
        dc.l _hdv_mediach+1 ; Mediach
        dc.l _drvmap
        dc.l _kbshift

xbios_vecs:
        dc.w (xbvecsend-xbios_vecs)/4
        dc.l _initmouse
        dc.l bios_unimpl ; Ssbrk
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
        dc.l bios_unimpl ; Waketime
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

/* unknown: 00e00fae */
        btst      #0,1(a7)
        bne.s     bx_notodd
        move.l    #rw_ret,(a7)
bx_notodd:
        movea.l   (_hdv_rw).w,a0
        jmp       (a0)
rw_ret:
        move.l    d0,-(a7)
        bsr       flush_cache_all_030
        move.l    (a7)+,d0
        jmp       bx_ret


/*
 * XBIOS #26 - Supexec
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


/**
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
        moveq.l   #0,d1 ; offsetof(MAPTAB, Bconstat)
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
        moveq.l   #4,d1 ; offsetof(MAPTAB, Bconin)
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
        moveq.l   #8,d1 ; offsetof(MAPTAB, Bcostat)
        bra.s     doxconxx

/*
 * BIOS #3 - Bconout - Print character to output device
 */
/* 306de: 00e00fee */
_bbconout:
        lea.l     (_bconout_vec).w,a0
        moveq.l   #12,d1 ; offsetof(MAPTAB, Bconout)

doxconxx:
        move.w    4(a7),d0
        cmp.w     #$0005,d0
        bls.s     doxconstd
        subq.l    #6,d0
        cmp.w     (bcmap_root+4).w,d0 ; bcmap_root.maptabsize
        bcc.s     doxconret
        movea.l   (bcmap_root).w,a0 ; bcmap_root.maptab
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
conxxinit:
        dc.l      bios_unimpl
        dc.l      _bcon1stat
        dc.l      _bcon2stat
        dc.l      _bcon3stat
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      _bcon0in
        dc.l      _bcon1in
        dc.l      _bcon2in
        dc.l      _bcon3in
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      _bco0stat
        dc.l      _bco1stat
        dc.l      _bco2stat
        dc.l      _bco4stat ; BUG compatibility with early TOS versions: MIDI(3) and KBD(4) are swapped
        dc.l      _bco3stat
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      bios_unimpl
        dc.l      _bcon0out
        dc.l      _bcon1out
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
        move.l    a1,(a0)
        clr.l     4(a0)
        clr.l     8(a0)
        clr.l     (a1)
        move.l    (_membot).w,4(a1)
        move.l    (_memtop).w,d0
        sub.l     (_membot).w,d0
        move.l    d0,8(a1)
        clr.l     12(a1)
        cmpi.l    #$1357BD13,_ramvalid
        bne.s     getmpb1
        cmpi.l    #$01000000,_ramtop
        bls.s     getmpb1
        lea.l     (_altmd).w,a2
        move.l    a2,(a1)
        clr.l     (a2)
        move.l    #$01000001,4(a2)
        move.l    _ramtop,d0
        sub.l     #$01000000,d0
        move.l    d0,8(a2)
        clr.l     12(a2)
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
        move.b    ($FFFF8201).w,d0
        lsl.w     #8,d0
        move.b    ($FFFF8203).w,d0
        lsl.l     #8,d0
        tst.b     _has_microwire
        bne.s     physbas1
        move.b    ($FFFF820D).w,d0
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
        move.b    ($FFFF8262).w,d0
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
        move.b    9(a7),($FFFF8201).w
        move.b    10(a7),($FFFF8203).w
        move.b    11(a7),($FFFF820D).w         ; BUG: no test that 820d actually exists
nosetp:
        tst.w     12(a7)
        bmi.s     nosetr
        bsr       dovsync
        move.b    13(a7),(_sshiftmod).w
        move.b    ($FFFF8262).w,d0
        and.b     #$F8,d0
        or.b      13(a7),d0
        move.b    d0,($FFFF8262).w
        clr.w     (_vblsem).w
        jsr       vdireinit
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
        move.l    4(a7),(_colorptr).w     * next VBL will do this
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
        tst.b     _has_microwire
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
* dump the registers, including SSP
        movem.l   d0-d7/a0-a7,(_proc_dregs).w
        move.l    2(a7),(_proc_enum).w
* get the exception number from the format word
        move.w    6(a7),d0
        and.w     #$0FFF,d0
        asr.w     #2,d0
* dump the exception number
        move.b    d0,(_proc_enum).w
* dump the correct value for USP
        move.l    usp,a0
        move.l    a0,(_proc_usp).w
* dump 16 words from the stack
        moveq.l   #15,d0
        lea.l     (_proc_stk).w,a0
        movea.l   a7,a1
any_vec1:
        move.w    (a1)+,(a0)+
        dbf       d0,any_vec1
* magic value
        move.l    #$12345678,(_proc_lives).w
        moveq.l   #0,d1
        move.b    (_proc_enum).w,d1
        subq.w    #1,d1
        bsr.s     drawbombs
* restore a sane stack (should have probably been done before calling subroutine above)
        move.l    #save_beg+save_siz,(_savptr).w
        move.w    #$FFFF,-(a7)
        move.w    #$004C,-(a7) ; Pterm
        trap      #1
        bra       _main
drawbombs:
        move.b    (_sshiftmod).w,d7
        and.w     #$0007,d7
        add.w     d7,d7
        moveq.l   #0,d0
        move.b    ($FFFF8201).w,d0
        lsl.w     #8,d0
        move.b    ($FFFF8203).w,d0
        lsl.l     #8,d0
        tst.b     _has_microwire
        bne.s     drawbo1
        move.b    ($FFFF820D).w,d0
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


* cpy512(void *src, void *dst)
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
chdv_init:
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
        trap      #13              ; Kbshift(-1)
        addq.l    #4,a7     
        btst      #2,d0            ; Control pressed?
        bne.s     autoexe2        ; yes, skip autoexec
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
        move.l    #autoscan,8(a0) ; set bp->p_tbase
        move.l    a3,-(a7)
        move.l    d0,-(a7)
        move.l    a3,-(a7)
        move.w    #$0004,-(a7)
        move.w    #$004B,-(a7)
        trap      #1              ; Pexec(Just go), execute the autoscan routine
        adda.w    #$0010,a7
autoexe1:
        move.l    savepc,-(a7)
autoexe2:
        rts

autoscan:
        clr.l     -(a7)
        move.w    #$0020,-(a7)
        trap      #1              ; Super
        addq.w    #6,a7
        movea.l   d0,a4
        movea.l   4(a7),a6        ; get ptr to Basepage
        lea.l     256(a6),a7      ; set stack to end of commandline
        move.l    #$00000100,-(a7) ; sizeof(PD)
        move.l    a6,-(a7)
        clr.w     -(a7)
        move.w    #$004A,-(a7)    ; Mshrink to basepage only
        trap      #1
        addq.w    #6,a7
        tst.w     d0
        bne.s     autosca4        ; if Mshrink failed, done
        move.w    #$0007,-(a7)
        move.l    execpath,-(a7)
        move.w    #$004E,-(a7)    ; Fsfirst
        moveq.l   #8,d7
autosca1:
        pea.l     execdta
        move.w    #$001A,-(a7)
        trap      #1              ; Fsetdta
        addq.w    #6,a7
        trap      #1
        adda.w    d7,a7
        tst.w     d0              ; Path found?
        bne.s     autosca4        ; no, done
* copy path of autofolder, and append found name to it
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
* execute the program
        pea.l     emptystr(pc)
        pea.l     emptystr(pc)
        pea.l     execfname
        clr.w     -(a7)
        move.w    #$004B,-(a7)
        trap      #1              ; Pexec(Load&Go)
        adda.w    #$0010,a7
        moveq.l   #2,d7
        move.w    #$004F,-(a7)    ; Fsnext
        bra.s     autosca1
autosca4:
        lea.l     _supstkend,a7
        move.l    savepc,-(a7)
        rts



#include "scrdmp.inc"
		text

bombimage:        dc.w $0600,$2900,$0080,$4840,$11f0,$01f0,$07fc,$0ffe,$0dfe,$1fff,$1fef,$0fee,$0fde,$07fc,$03f8,$00e0


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

/* 306de: 00e015a2 */
instoshdr:
        lea.l     _os_entry(pc),a0
        lea.l     oshdr+6,a1
        moveq.l   #47,d0
instos1:
        move.b    0(a0,d0.w),0(a1,d0.w)
        dbf       d0,instos1
        move.w    jmpop(pc),-6(a1) ; ? BUG? the jmpop there is short abs jump
        move.l    4(a1),-4(a1)
        move.w    braop(pc),(a1)
        move.w    30(a1),28(a1) ; ??? oshdr.os_conf = oshdr.os_dosdate
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
        bsr.s     ckblitter
        move.w    d0,d4
        move.w    d0,d5
        lsr.w     #1,d5
        or.w      #$FFFE,d5
        jsr       getblitmode
        move.w    d0,d3
        move.w    4(a7),d0
        bmi.s     blitmod1
        and.w     d5,d0
        or.w      d4,d0
        jsr       setblitmode
blitmod1:
        move.w    d3,d0
        rts

/* 306de: 00e01604 */
ckblitter:
        move.w    sr,d1
        move.w    #$0000,d0
        suba.l    a0,a0
        movea.l   a7,a2
        ori.w     #$0700,sr
        movea.l   _buserror(a0),a1
        move.l    #noblitter,_buserror(a0)
        tst.w     $8a00(a0)
        moveq.l   #2,d0
noblitter:
        move.l    a1,_buserror(a0)
        move.w    d1,sr
        movea.l   a2,a7
        rts

/* 306de: 00e0162e */
ttdelayc:
        bsr.s     tttimerc
ttdelay1:
        btst      #5,(TTMFP_REGS+MFP_IPRB).w
        beq.s     ttdelay1
        rts

*
* (re-)program TT MFP Timer C
* Input: D0: initial counter (low byte) + (divisor<<8)
*
/* 306de: 00e0163a */
tttimerc:
        movem.w   d0-d1,-(a7)
        move.w    sr,-(a7)
        ori.w     #$0700,sr
        move.b    (TTMFP_REGS+MFP_TCDCR).w,d1 ; save original Timer C+D mode
        and.b     #$0F,d1
        move.b    d1,(TTMFP_REGS+MFP_TCDCR).w  Stop timer C
        bclr      #5,(TTMFP_REGS+MFP_IERB).w ; Disable Timer C interrupt
        move.b    #$DF,(TTMFP_REGS+MFP_IPRB).w ; Clear pending Interrupt flag
        bclr      #5,(TTMFP_REGS+MFP_IMRB).w ; Mask Timer C interrupt
        bset      #5,(TTMFP_REGS+MFP_IERB).w ; Enable Timer C interrupt again
        move.b    d0,(TTMFP_REGS+MFP_TCDR).w ; Set Timer C counter
        lsr.w     #4,d0
        and.b     #$F0,d0
        or.b      d0,d1
        move.b    d1,(TTMFP_REGS+MFP_TCDCR).w ; Set Timer C mode
        move.w    (a7)+,sr
        movem.w   (a7)+,d0-d1
        rts

/*
 * _zeromem(void *start, void *end)
 */
/* 306de: 00e01680 */
		xdef _zeromem
_zeromem:
        movea.l   4(a7),a0
        movea.l   8(a7),a1
        movem.l   d3-d7/a3,-(a7)
        moveq.l   #0,d1
        moveq.l   #0,d2
        moveq.l   #0,d3
        moveq.l   #0,d4
        moveq.l   #0,d5
        moveq.l   #0,d6
        moveq.l   #0,d7
        movea.w   d7,a3
        move.l    a0,d0
        btst      #0,d0
        beq.s     zerom1
        move.b    d1,(a0)+
zerom1:
        move.l    a1,d0
        sub.l     a0,d0
        and.l     #$FFFFFF00,d0
        beq.s     zerom3
        lea.l     0(a0,d0.l),a0
        movea.l   a0,a2
        lsr.l     #8,d0
zerom2:
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        movem.l   d1-d7/a3,-(a2)
        subq.l    #1,d0
        bne.s     zerom2
zerom3:
        cmpa.l    a0,a1
        beq.s     zerom4
        move.b    d1,(a0)+
        bra.s     zerom3
zerom4:
        movem.l   (a7)+,d3-d7/a3
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
        move.w    ($FFFF8262).w,-(a7)
        move.w    6(a7),($FFFF8262).w
        move.w    ($FFFF8262).w,d0
        and.w     #$0007,d0
        move.b    d0,(_sshiftmod).w
        clr.w     (_vblsem).w
        jsr       vdireinit
        move.w    #$0001,(_vblsem).w
        move.w    (a7)+,d0
        rts

/*
 * XBIOS #81 - EgetShift
 */
/* 306de: 00e01786 */
_egetshift:
        moveq.l   #0,d0
        move.w    ($FFFF8262).w,d0
        rts

/*
 * XBIOS #82 - EsetBank - Read/modify TT shifter color bank number
 */
/* 306de: 00e0178e */
_esetbank:
        moveq.l   #0,d0
        move.w    ($FFFF8262).w,d0
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
        lea.l     $8400(a0),a0
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
        move.b    ($FFFF8262).w,d1
        move.b    d1,d0
        lsr.b     #4,d0
        and.b     #$01,d0
        bclr      #4,d1
        tst.w     4(a7)
        beq.s     esetgr1
        bmi.s     esetgr2
        bset      #4,d1
esetgr1:
        move.b    d1,($FFFF8262).w
esetgr2:
        rts

/*
 * XBIOS #87 - EsetSmear - Read/modify TT shifter smear mode bit
 */
/* 306de: 00e01848 */
_esetsmear:
        moveq.l   #0,d0
        move.b    ($FFFF8262).w,d1
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
        move.b    d1,($FFFF8262).w
esetsm2:
        rts

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

x00E01CFE:
	rts

/* 306de: 00e01ef0 */
ngetdt:
        bsr       detect_nvram
        bcs.s     inittc2
        move.b    #$0D,(NVRAM_ADDR_REG).w ; select register D
        move.b    (NVRAM_DATA_REG).w,d0
        btst      #7,d0                   ; is RAM & time valid?
        bne.s     inittc1                 ; yes
        move.l    #$12C80000,-(a7)        ; (9<<9)+(6<<5)+8
        bsr       wrttclock
        addq.w    #4,a7
inittc1:
        bsr.s     rdttclock
        cmp.l     #$FFFFFFFF,d0
        beq.s     inittc2
        moveq.l   #0,d0
inittc2:
        rts

*
* reads date & time from TT clock,
* and updates GEMDOS variables from it
*
/* 306de: 00e01f20 */
rdttclock:
        bsr       detect_nvram
        bcs.s     inittc2
        move.b    #$0D,(NVRAM_ADDR_REG).w ; select register D
        btst      #7,(NVRAM_DATA_REG).w   ; is RAM & time valid?
        beq       rdttc2                  ; no, fail
        move.w    sr,d2
        move.w    d2,d0
        or.w      #$0700,d0
rdttc1:
        move.b    #$0A,(NVRAM_ADDR_REG).w ; select register A
        btst      #7,(NVRAM_DATA_REG).w   ; is update in progress?
        bne.s     rdttc1                  ; yes, wait
        moveq.l   #0,d0
        move.l    d0,d1
        move.b    #$00,(NVRAM_ADDR_REG).w ; select register 0 (seconds)
        move.b    (NVRAM_DATA_REG).w,d0
        asr.w     #1,d0
        move.b    #$02,(NVRAM_ADDR_REG).w ; select register 2 (minutes)
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$1546                  ; bfins     d1,d0{21:6}
        move.b    #$04,(NVRAM_ADDR_REG).w ; select register 4 (hours)
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$1405                  ; bfins     d1,d0{16:5}
        move.b    #$07,(NVRAM_ADDR_REG).w ; select register 7 (day of month)
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$12c5                  ; bfins     d1,d0{11:5}
        move.b    #$08,(NVRAM_ADDR_REG).w ; select register 8 (month)
        move.b    (NVRAM_DATA_REG).w,d1
        dc.w $efc0,$11c4                  ; bfins     d1,d0{7:4}
        move.b    #$09,(NVRAM_ADDR_REG).w ; select register 9 (year)
        move.b    (NVRAM_DATA_REG).w,d1
        sub.b     #nvram_rtc_year_offset,d1
        dc.w $efc0,$1007                  ; bfins     d1,d0{0:7}
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

*
* write date & time to TT clock
*
/* 306de: 00e01fc6 */
wrttclock:
        bsr       detect_nvram
        bcs       inittc2
        move.l    4(a7),d0
        move.b    #$0B,(NVRAM_ADDR_REG).w ; select register B
        move.b    #$80,(NVRAM_DATA_REG).w ; abort update cycle
        move.b    #$0A,(NVRAM_ADDR_REG).w ; select register A
        move.b    #$2A,(NVRAM_DATA_REG).w ; divider = 2, rate = 10
        move.b    #$0B,(NVRAM_ADDR_REG).w ; select register B
        move.b    #$8E,(NVRAM_DATA_REG).w ; enable data mode binary, 24hr, daylight savings
        move.b    #$00,(NVRAM_ADDR_REG).w ; select register 0 (seconds)
        dc.w $e9c0,$16c5    ; bfextu    d0{27:5},d1 
        add.b     d1,d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$02,(NVRAM_ADDR_REG).w ; select register 2 (minutes)
        dc.w $e9c0,$1546    ; bfextu    d0{21:6},d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$04,(NVRAM_ADDR_REG).w ; select register 4 (hours)
        dc.w $e9c0,$1405    ; bfextu    d0{16:5},d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$07,(NVRAM_ADDR_REG).w ; select register 7 (day of month)
        dc.w $e9c0,$12c5    ; bfextu    d0{11:5},d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$08,(NVRAM_ADDR_REG).w ; select register 8 (month)
        dc.w $e9c0,$11c4    ; bfextu    d0{7:4},d1
        move.b    d1,(NVRAM_DATA_REG).w
        move.b    #$09,(NVRAM_ADDR_REG).w ; select register 9 (year)
        dc.w $e9c0,$1007    ; bfextu    d0{0:7},d1
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
        andi.b    #$FE,ccr                ; clear carry bit
        rts
nvramfail:
        movea.l   a0,a7
        move.l    a1,(_buserror).w
        ori.b     #$01,ccr                ; set carry bit
        rts

/*
 * XBIOS +42 - NVMaccess
 */
/* 306de: 00e0208c */
_nvmaccess:
	rts

/* 306de: 00e021a4 */
igetdt:
		rts

/* 306de: 00e021ce */
isetdt:
		rts

/* 306de: 00e024d6 */
initmfp:
		rts

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

mfpea:
        move.b    d0,d1
        cmpi.b    #$08,d1
        blt.s     mfpea1
        subq.w    #8,d1
        rts
mfpea1:
        addq.l    #MFP_IERB-MFP_IERA,a1
        rts

/*
 * XBIOS #14 - Iorec - Returns pointer to a serial device's input buffer record.
 */
_iorec:
/* 306de: 00e02a40 */
        move.w    4(a7),d1
        beq.s     iorec1
        asl.l     #2,d1
        move.l    iorectab(pc,d1.w),d0
        rts
iorec1:
        move.l    (bcmap_root+12).w,d0 ; bcmap_root.iorec
        rts
iorectab:
        dc.l rs232iorec
        dc.l midiiorec
        dc.l ikbdiorec

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
        movea.l   (bcmap_root+8).w,a0 ; bcmap_root.rscon
        jmp       (a0)

/*
 * XBIOS #44 - Bconmap - Select a default port.
 */
/* 306de: 00e02baa */
_bconmap:
		rts


/* 306de: 00e0378a */
_do_keyclick:
		rts

/*
 * XBIOS #28 - Giaccess - Permit access to the registers of the GI sound chip. 
 */
/* 306de: 00e0387e */
_giaccess:
		rts


/*
 * XBIOS #30 - Ongibit - Set individual bits in the port A register of the GI sound chip. 
 */
/* 306de: 00e038b6 */
_ongibit:
		rts


/*
 * XBIOS #29 - Offgibit - Clear individual bits in the port A register of the GI sound chip. 
 */
/* 306de: 00e038de */
_offgibit:
		rts


/*
 * XBIOS #0 - Initmouse - Initializes the mouse handler. 
 */
/* 306de: 00e03906 */
_initmouse:
		rts


/*
 * XBIOS #31 - Xbtimer - Initialize the timer in the MFP chip.
 */
/* 306de: 00e039ce */
_xbtimer:
		rts


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
        move.l    #_keytblshift,(_curkeytbl+4).w
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


/* 306de: 00e03b98 */
_do_bell:
		rts

/*
 * XBIOS #8 - Floprd - Read individual sectors from floppy disk.
 */
/* 306de: 00e03c10 */
_floprd:
		rts

/*
 * XBIOS #9 - Flopwr - Write individual sectors on a floppy disk.
 */
/* 306de: 00e03d12 */
_flopwrt:
		rts

/*
 * XBIOS #10 - Flopfmt - Format specific tracks on a floppy 
 */
/* 306de: 00e03e02 */
_flopfmt:
		rts

/*
 * XBIOS #19 - Flopver - Verify sectors of a floppy disk.
 */
/* 306de: 00e03ffe */
_flopver:
		rts

/* 306de: 00e04106 */
_flopvbl:
	rts

/*
 * XBIOS #41 - Floprate - Read or set the seek rate of a floppy drive.
 */
/* 306de: 00e044b6 */
_floprate:
		rts

/*
 * XBIOS #36 - Prtblk - Hardcopy print-out 
 */
/* 306de: 00e045d8 */
_prtblk:
	rts


/*
 * XBIOS #17 - Random - Random number generator
 */
/* 306de: 00e05d38 */
_random:
	rts


/*
 * XBIOS #18 - Protobt - Generate a standard boot-sector for a floppy disk.
 */
/* 306de: 00e05e04 */
_protobt:
		rts

x00E05FA4:
	rts
x00E061F2:
	rts

/* 306de: 00e0694a */
vdireinit:
	rts

/* 306de: 00e069f4 */
resetdev:
	rts

/* 306de: 00e0761a */
_blink:
	rts


/*
 * XBIOS #21 - Cursconf - Sets the working of the VT52 cursor
 */
/* 306: 00e0764c */
_cursconf:
		rts


/* 306: 00e07f20 */
setblitmode:
        move.w    d0,LA_BLTMODE
        movea.l   LA_CURDEV,a0
        btst      #0,d0
        beq.s     setblit1
        movea.l   LA_ROUTINES,a0
setblit1:
        lea.l     LA_USERDEVINIT,a1
        moveq.l   #9,d0
setblit2:
        move.l    (a0)+,(a1)+
        dbf       d0,setblit2
        rts

	xdef getblitmode
/* 306: 00e07f48 */
getblitmode:
        move.w    LA_BLTMODE,d0
        rts

        data
_ui_mupb:
        dc.l GEM_MUPB_MAGIC
        dc.l _endgembss
        dc.l _ui_start
zero:
	    dc.l 0

* 96x86 image of atari logo displayed at boot time
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

		bss
		
		xdef gem_env
		comm gem_env,12
		
		xdef save_beg
		comm save_beg,save_siz
		
		xdef oshdr
		comm oshdr,70 ; 6+48+16

		xdef cookies
cooksize equ NCOOKIES*2*4
		comm cookies,cooksize

		xdef _bootpref
		comm _bootpref,2
		
		xdef _altmd
		comm _altmd,16
		
		xdef _has_dmasound
		comm _has_dmasound,1
		
		xdef _has_microwire
		comm _has_microwire,1
		
		xdef privinst
		comm privinst,8

		xdef _pbdef
		comm _pbdef,30
		
		xdef midiiorec
		comm midiiorec,14
		xdef midibuf
		comm midibuf,256
		
		xdef ikbdiorec
		comm ikbdiorec,14
		xdef ikbdbuf
		comm ikbdbuf,128
		
		xdef _kbdvecs
		comm _kbdvecs,42

		xdef bcmap_root
		comm bcmap_root,16
