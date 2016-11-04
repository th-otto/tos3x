/* osinit.S - GEMDOS assembler interface   */

/*****************************************************************************
 *
 *   Global References
 *
 *****************************************************************************/

#include "config.h"
#include "bdosdefs.h"
#include "tosvars.inc"

    xref    _GSX_ENTRY
    xref    _osif
    xref    _xterm
    xref    _cinit
    xref    _run
    xref    _tikfrk

/*
 * BIOS functions
 */

cve     equ 5


    page
/******************************************************************************
 *
 *   __osinit - Initialize OS exeception vectors, etc.
 *
 ******************************************************************************/

    xdef    __osinit
__osinit:
/*   move.l  #_enter,$84     / * take over the handling of TRAP #1 */
    dc.w $21fc               /* workaround for buggy short address form of as68 */
    dc.l _enter
    dc.w $84
    
/*   move.l  $88,old_2       / * save old TRAP #2 handler */
    dc.w $23f8,$88
    dc.l old_2
    
/*   move.l  #ground_it,$88  / * take over the handling of TRAP #2 */
    dc.w    $21fc
    dc.l    ground_it
    dc.w $88
    
    move.w  sr,savesr
    ori.w   #$0700,sr
    move.l  #tikhnd,-(sp)    /* set clock vector */
    move.w  #$100,-(sp)      /* handle for clock */
    move.w  #cve,-(sp)       /* vector exchange */
    trap    #13
    addq.l  #8,sp
    move.l    d0,old_tik
    move.w    savesr,sr
    
    jsr _cinit
    rts

    page
/***************************************************************************
 *
 *  trap13 -
 *   perform trap 13 supervisor call on behalf of caller
 *
 ***************************************************************************/

    xdef    _trap13
_trap13:
    move.l  (sp)+,biosav
    trap    #13
    move.l  biosav,-(sp)
    rts

    xdef    _trap14
_trap14:
    move.l  (sp)+,biosav
    trap    #14
    move.l  biosav,-(sp)
    rts

/*
 *  ground_it - trap 2 entry point
 *
 * This minimal interface connects the entry point for a system where the AES
 * and VDI are already in memory.
 */

ground_it:              
    tst.w   d0
    beq oterm
    cmp.w     #$0073,d0
    bne novdi
    jsr       _GSX_ENTRY
    rte
novdi:
    cmp.w     #-1,d0
    bne       nodsp
    move.l    #_GSX_ENTRY,d0
    rte
nodsp:
    move.l  old_2,-(sp)
    rts

oterm:  move.l  fstrtend,sp
    clr.w   -(sp)
    bsr _xterm
    illegal
    rte

/***************************************************************************
 *
 * uc - utility routine to return the upper-case of character passed in
 *
 ***************************************************************************/

    xdef _uc
_uc:
    move.w    4(a7),d0
    cmp.b     #$61,d0
    blt.s     uc1
    cmp.b     #$7A,d0
    bgt.s     uc1
    bclr      #5,d0
uc1:
    rts

    page
/***************************************************************************
 *
 *  tikhnd -
 *   logical tick interrupt handler
 *
 ***************************************************************************/

tikhnd:
    move.w  4(sp),-(sp) /* get tick value.  ignore flags */
    jsr _tikfrk         /* call interrupt handler extension */
    addq.l  #2,sp       /* clear the stack */
    move.l    old_tik,-(a7)
    rts

    xdef _indcall
_indcall:
    move.l    4(a7),-(a7)
    rts

    page
/***************************************************************************
 *
 *  _swp68 - swap bytes in a word
 *
 ***************************************************************************/

    xdef    _swp68
_swp68:
    move.l  4(a7),a0
    move    (a0),d0
    ror #8,d0
    move    d0,(a0)
    rts

/***************************************************************************
 *
 *  _swp68l - swap word & bytes in long
 *
 ***************************************************************************/

    xdef    _swp68l
_swp68l:
    move.l  4(a7),a0
    move.l  (a0),d0
    ror #8,d0
    swap    d0
    ror #8,d0
    move.l  d0,(a0)
    rts


/***************************************************************************
 *
 *  _xsetjmp -
 *
 ***************************************************************************/

    xdef    _xsetjmp
_xsetjmp:
    link    a6,#0
    move.l  8(a6),a0      /* get jmp_buf */
    move.l  (a6),(a0)+    /* save a6 */
    lea 8(a6),a1
    move.l  a1,(a0)+      /* save sp */
    move.l  4(a6),(a0)    /* save return pc */
    clr.l   d0
    unlk    a6
    rts

/***************************************************************************
 *
 *  _xlongjmp -
 *
 ***************************************************************************/

    xdef    _xlongjmp
_xlongjmp:
    link    a6,#0
    move.l  12(a6),d0
    tst.l   d0
    bne okrc

    move.w  #1,d0
okrc:
    move.l  8(a6),a0      /* get jmp_buf */
    move.l  (a0)+,a6      /* restore a6 */
    move.l  (a0)+,a7      /* restore sp */
    move.l  (a0),-(sp)    /* get return pc */
    rts

/******************************************************************************
 *
 *   _enter - Front end of TRAP #1 handler
 *
 ******************************************************************************/

_enter:
    btst.b  #5,(sp)     /* are we in supervisor mode? */
    bne enter_1     /* yes, go check sup stack */

    move.l  usp,a0      /* no, check user stack */
    cmp.w   #$20,(a0)   /* toggle/inquire state? */
    beq x20_usr     /* yes, go perform function */

    bra enter_2

enter_1: ds.b 0
#if !P68010
    tst.w   _longframe
    bne     chklong
    cmp.w   #$20,6(sp)  /* toggle/inquire state? */
    beq x20_sup     /* yes, go perform function */
    bra enter_2
chklong:
#endif
    cmp.w   #$20,8(sp)  /* toggle/inquire state? */
    beq x20_sup     /* yes, go perform function */

enter_2:
    move.l  a6,-(sp)
    move.l  _run,a6
    movem.l d0/a3-a5,$68(a6)
    move.l  (sp)+,$78(a6)   /* olda6 */
    move.w  (sp)+,d0    /* status reg */
    move.l  (sp)+,a4    /* retadd */
#if !P68010
    tst.w   _longframe
    beq     noadj
#endif
    addq.l    #2,a7
noadj:
    btst    #13,d0      /* if he is in system state, use his stack */
    bne systk
    move.l  usp,a5
    movem.l d1-d7/a0-a2,-(a5)
    move.l  a4,-(a5)    /* retadd */
    move    d0,-(a5)    /* sr */
    move.l  sp,a0
    move.l  a0,-(a5)
    move.l  a5,$7c(a6)
/* now switch to fs stack */
    move.l  #fstack+2*1034,sp
    lea 50(a5),a0
    bra callos
/* he was using his own ssp */
systk:  movem.l d1-d7/a0-a2,-(sp)
    move.l  a4,-(sp)
    move    d0,-(sp)
    move.l  usp,a0
    move.l  a0,-(sp)
    move.l  sp,$7c(a6)
    lea 50(sp),a0
    move.l  #fstack+2*1034,sp
callos: move.l  a0,-(sp)    /* push parm pointer */
    jsr _osif
    add.l   #4,sp

/* ...fall into gouser.... */

/***************************************************************************
 *
 *  _gouser -
 *
 ***************************************************************************/

entusr:
    move.l  _run,a5
    move.l  d0,$68(a5)
    move.l  $7c(a5),a6  /* stack pointer (maybe usp, maybe ssp) */
    move.l  (a6)+,a4    /* other stack pointer */
    move.w  (a6)+,d0
    move.l  (a6)+,a3    /* retadd */
    movem.l (a6)+,d1-d7/a0-a2
    btst    #13,d0
    bne retsys          /* a6 is (user-supplied) system stack */
    move.l  a4,sp
    move.l  a6,usp
gousr: ds.b 0
#if !P68010
	tst.w   _longframe
	beq     noclr
#endif
    clr.w   -(a7)
noclr:
    move.l  a3,-(sp)
    move.w  d0,-(sp)
    movem.l $68(a5),d0/a3-a6
    rte

retsys: move.l  a6,sp
    move.l  a4,usp
    bra gousr

    xdef    _gouser
_gouser:
    movea.l  _run,a5
    move.l   104(a5),d0
    bra      entusr
    
    page
/******************************************************************************
 *
 * function 0x20 handler
 * toggle/inquire processor/stack state
 *
 ******************************************************************************/

x20_usr:            /* user mode entry point */
    move.l  2(a0),d1    /* get parameter */
    beq xu_0        /* already have new SSP from old USP, go handle 0L entry */

    subq.l  #1,d1       /* check for 1L entry */
    beq.s x20_inq     /* go handle inquiry */

    move.l  2(a0),a0    /* get new SSP from parameter */
xu_0: ds.b 0
#if P68010
    lea.l     -6(a0),a0
    move.w  (sp)+,d0    /* pop SR */
    move.l  (sp)+,(a0)  /* transport the return address */
    move.w    (a7)+,4(a0) /* transform format word */
#else
    move.w    (sp)+,d0  /* pop SR */
    move.l    (a7)+,d1  /* pop PC */
    tst.w    _longframe
    beq       usrnolong
    move.w    (a7)+,-(a0) /* transform format word */
usrnolong:
    move.l    d1,-(a0)    /* transport the return address */
#endif

    ori.w   #$2000,d0   /* set supervisor mode */
    move.w  d0,-(a0)    /* push SR */
    move.l  sp,d0       /* set return value (old SSP) */
    move.l  a0,sp       /* set new SSP */
    rte

/* supervisor mode entry point */

x20_sup: ds.b 0
#if !P68010
    tst.w   _longframe
    beq     supnolong
#endif
    move.l  10(sp),d1   /* get parameter */
    beq xs_0        /* go handle 0L entry */

    subq.l  #1,d1       /* check for 1L entry */
    beq x20_inq     /* go handle inquiry */

    move.l  10(sp),a1   /* point to new SSP */
    move.w  (sp)+,d0    /* pop SR */
    move.l  (sp)+,d1    /* pop the return address */
    move.w    (a7)+,-(a1) /* transport the format word */
    move.l  d1,-(a1)    /* transfer the return address */
#if !P68010
	bra     supcont
supnolong:
    move.l  8(sp),d1   /* get parameter */
    beq xs_0        /* go handle 0L entry */

    subq.l  #1,d1       /* check for 1L entry */
    beq x20_inq     /* go handle inquiry */
    move.l  8(sp),a1   /* point to new SSP */
    move.w  (sp)+,d0    /* pop SR */
    move.l  (sp)+,-(a1)    /* pop and transfer the return address */
#endif
supcont:
    move.w  d0,-(a1)    /* push SR */
    move.l  usp,a0      /* get USP */
    cmpa.l  a0,sp       /* already at old USP? */
    beq xs_usp      /* don't play with stack */

    move.l  (sp)+,-(a0) /* transport func # and parm */
    move.l  a1,sp       /* update SSP */
    bra xs_exit

xs_usp:
    move.l  a1,sp       /* update SSP */
    bra xs_scram

xs_0: ds.b 0
#if !P68010
    lea     6(sp),a0        /* create USP */
    tst.w   _longframe
    beq     xs_exit    
#endif
    lea     8(sp),a0        /* create USP */
xs_exit:
    move.l  a0,usp      /* update USP */
xs_scram:
    andi.w  #$DFFF,(sp) /* set user mode */
    rte

/* inquiry mode entry point */

x20_inq:
    move.l  #$2000,d0   /* set up test mask */
    and.w   (sp),d0     /* check the supervisor state */
    beq xi_exit     /* return 0L for user mode */

    moveq.l #-1,d0      /* return -1L for sup mode */

xi_exit:
    rte

/******************************************************************************
 *
 * call dosjr from within itself (or from linked-in shell)
 *
 ******************************************************************************/

    xdef    _oscall
_oscall:
    link    a6,#0
    move.l  a0,-(sp)
    lea 8(a6),a0
    move.l  a0,-(sp)
    jsr _osif
    addq.l  #4,sp
    move.l  (sp)+,a0
    unlk    a6
    rts

/******************************************************************************
 *
 * xbgettime - update dos time from BIOS
 *
 ******************************************************************************/

	xdef _xbgettime
	xref _date
	xref _time
_xbgettime:
	move.w    #$0017,-(a7)
	trap      #14
	addq.l    #2,a7
	move.w    sr,d1
	ori.w     #$0700,sr
	move.w    d0,_time
	swap      d0
	move.w    d0,_date
	move.w    d1,sr
	rts

/******************************************************************************
 *
 * xbsettime - update BIOS time from dos time
 *
 ******************************************************************************/

	xdef _xbsettime
	xref _date
	xref _time
_xbsettime:
    /* BUG: access must be made atomic, too */
	move.w    _time,-(a7)
	move.w    _date,-(a7)
	move.w    #$0016,-(a7)
	trap      #14
	addq.w    #6,a7
	rts

/******************************************************************************/

    bss
    even

	xdef biosav
	comm biosav,8
	xdef savesr
	comm savesr,2
	xdef old_tik
	comm old_tik,4
	xdef old_2
	comm old_2,4           /* old TRAP #2 handler */

	xdef fstack
	comm fstack,2*1034+4

    data
fstrtend: dc.l fstack+2*1034

