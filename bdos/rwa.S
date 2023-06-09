*	rwa.a - GEMDOS assembler interface	*

*
*
*  NOTE:	This MUST be assembled with AS68's "-t" option.
*
* Originally written by JSL.
*
* MODIFICATION HISTORY
*
*	11 Mar 85	SCC	Added xgetsup() functionality as a hack in
*				_entry().
*				Removed get sup functionality from exit().
*	11 Mar 85	JSL	Changed timer vector number.
*	12 Mar 85	SCC	Modified xgetsup() functionality in _entry().
*	13 Mar 85	SCC	Changed 'xgetsup()' type functionality to
*				'toggle/inquire processor/stack state' type
*				functionality.
*	25 Mar 85	SCC	Modified 'tikhnd:'.  It didn't need to save
*				the registers it was saving.
*	 1 Apr 85	SCC	Modified 'tikhnd:'.  It needed to get the
*				number of milliseconds since last tick into
*				the right place for the stack frame for call
*				to tikfrk().
*	10 Apr 85	SCC	Modified longjmp() to use long return value
*				on stack.
*	12 Apr 85	SCC	Modified exit() to check for function -1
*				(return address of screen driver.
*	14 Apr 85	SCC	Modified osinit() to disable interrupts while
*				initializing the tick vector.
*	19 Apr 85	SCC	Modified osinit() to preserve the previous
*				state of the SR.
*	22 Apr 85	SCC	Modified the saving of SR to store it
*				temporarily in a static.
*
*	31 May 85	EWF	Added in conditional assembly for 68010-based
*				systems.
*
*	19 Jun 85	EWF	Added additional conditional assemblies for
*				68010-based systems.
*
*	 9 Aug 85	SCC	Modified osinit() to get tick vector from
*				clock device handle.
*
*				NOTE:	STACK LOOKS DIFFERENT THAT ATARI'S ON
*					INVOCATION OF TICK HANDLER!
*
*				Modified tikhnd: to work with new clock format
*				(tick value in long above long flag value on
*				stack)
*
*	15 Aug 85	SCC	Modified tikhnd:.  It was picking up the
*				number of milliseconds from the wrong location
*				on the stack.
*
*	18 Aug 85	SCC	Added copyright message.
*
*	 1 Nov 85	SCC	Converted to runtime determination of
*				processor type (68000 v. 68010)
*
*				Added 68010 'MOVE SR,...' to 'MOVE CCR,...'
*				illegal instruction handler
*
*				Converted all exception vector handling to
*				use BIOS calls, rather than direct addresses.
*
*	 4 Nov 85	SCC	Cleaned up a stack imbalance problem in
*				'ii_handler'.  It was not POPing D0/A0 off
*				the stack before going off to the OEM's
*				Illegal Instruction handler.
*
*	11 Nov 85	KTB	put trap2 handler back in for the nonce
*
*	14 May 86	KTB	removed dojmp
*
*	02 Jul 86	KTB	M01.01a.0702.01 osinit now returns address of 
*				date/time stamp for bdos
*
*	23 Jul 86	KTB	M01.01.0723.01 osinit needs to save stack ptr
*				in some other reg besides d1.
*
*	24 Jul 86	KTB	M01.01.0724.01 osinit no longer returns addr
*				of date/time stamp.  this is all handled in 
*				startup.a
*
*	04 Aug 86	KTB	M01.01.0804.02 osinit => _osinit
*
*	28 Aug 86	SCC	M01.01.0828.01 changed TRAP #2 handler back
*				similar to the way it was in the olden days
*				for Atari's benefit in bringing up a system
*				where the VDI and AES are already in memory.
*
*	19 May 87	ACH   	The 68070 has an exception stack frame similar
*				to the 68010 but it does not impliment the
*				"move ccr,d0" instruction. Hence the 68000 vs.
*				68010 detection would give the oposite result
*				to the one desired. Force mc68010 flag to true.
*
*	19 Nov 87	ACH	Moved _lbmove, _bfill, _bmove into this module.
*
* NAMES
*
*	EWF	Eric W. Fleischman
*	JSL	Jason S. Loveman
*	SCC	Steven C. Cavender
*	ACH	Anthony C. Hay
*
******************************************************************************

_68070		equ	1		* 1=running on 68070, 0=68000/68010


*****************************************************************************
*
*	Global References
*
*****************************************************************************

	xref	_s68
	xref	_s68l
	xref	_xsetjmp
	xref	_xlongjmp
	xref	_gouser
	xref	_oscall
	xref	_trap13
	xref	__osinit
	xref	mc68010

	xdef	_osif
	xdef	_xterm
	xdef	_cinit
	xdef	_run
	xdef	_tikfrk
	xdef	fstrt

	xref	_lbmove
	xref	_bfill
	xref	_bmove


**
** BIOS functions
**

x_vector	equ	5
cve		equ	16

BFHCLK		equ	-1		* BIOS handle for tick


	text
	page
******************************************************************************
*
*	__osinit - Initialize OS exeception vectors, etc.
*
******************************************************************************

	text

	ifeq	_68070

__osinit:
*
* First of all:  Are we executing on a 68000 or a 68010?
*	We find out by taking over the illegal instruction trap and then
*	executing a 'MOVE CCR,D0' which is not implemented on the 68000.
*	We will set a flag for use later on as we enter and exit the BDOS.
*	We also will set up a handler on the 68010 to convert 'MOVE SR,...'
*	to 'MOVE CCR,...' at execution time, so that we can support one
*	release of 68XXX software.

	move.l	sp,ii_stksav		* save the current sp
	clr.w	mc68010			* pre-clear processor flag
	move.l	#ii_move,-(sp)		* exchange illegal instruction vector
	move.w	#4,-(sp)
	move.w	#x_vector,-(sp)
	trap	#13
	move.l	d0,ii_save
	move.l	d0,ii_old

	move	ccr,d0			* So, can we execute this?

	move.w	#1,mc68010		* yes, flag that we're on a 68010
	move.l	#ii_handler,ii_save	* and set in our handler

ii_move:
	movea.l	ii_stksav,sp		* restore sp
	move.l	ii_save,-(sp)		* we now either restore the old vector
	move.w	#4,-(sp)		* or install our 68010 conversion
	move.w	#x_vector,-(sp)		* handler
	trap	#13
	endc

	ifne	_68070
__osinit:
	move.w	#1,mc68010		* flag that we're on a 68010 (68070)
	subq.l	#8,sp
	endc

	move.l	#_enter,4(sp)		* take over the handling of TRAP #1
	move.w	#$21,2(sp)
	move.w	#x_vector,(sp)
	trap	#13

	move.l	#ground_it,4(sp)	* take over the handling of TRAP #2
	move.w	#$22,2(sp)
	move.w	#x_vector,(sp)
	trap	#13
	move.l	d0,old_2		* save old TRAP #2 handler

	move.l	#tikhnd,4(sp)		* set clock vector
	move.w	#BFHCLK,2(sp)		* handle for clock
	move.w	#cve,(sp)		* character vector exchange
	trap	#13
	add.l	#8,sp

	jsr	_cinit
	rts

*	dc.b	"Copyright 1985, Digital Research, Inc.",0
	even

*
*  ground_it - trap 2 entry point
*
* This minimal interface connects the entry point for a system where the AES
* and VDI are already in memory.
*

ground_it:				
	tst.w	d0
	beq	oterm

	move.l	old_2,-(sp)
	rts

oterm:	move.l	#fstrt,sp
	clr	-(sp)
	jsr	_xterm
	rte

ii_handler:
	movem.l	d0/a0,-(sp)		* saving d0 and a0 while we test
	movea.l	10(sp),a0		* the offending address
	move.w	(a0),d0			* the offending instruction
	andi.w	#$ffc0,d0		* isolate move from SR opcode
	cmpi.w	#$40c0,d0		* we don't care where "To" was
	bne	not_SR			* Not a MOVE SR,XX instruction

	ori.w	#$200,(a0)		* make it MOVE CCR,XX
	movem.l	(sp)+,d0/a0
	rte

not_SR:
	movem.l	(sp)+,d0/a0
	move.l	ii_old,-(sp)		* let OEM handle it
	rts

	bss
	even

mc68010:				* 0 = 68000, 1 = 68010
	ds.w	1

old_2:
	ds.l	1			* old TRAP #2 handler

ii_save:				* routine to set up as illegal
	ds.l	1			* instruction handler
ii_old:
	ds.l	1
ii_stksav:
	ds.l	1			* at osinit: save sp here

	page
***************************************************************************
*
*  trap13 -
*	perform trap 13 supervisor call on behalf of caller
*
***************************************************************************

	text
_trap13:
	move.l	(sp)+,biosav
	trap	#13
	move.l	biosav,-(sp)
	rts

	bss
	even
biosav:
	ds.l	1

***************************************************************************
*
*  tikhnd -
*	logical tick interrupt handler
*
***************************************************************************

	text
tikhnd:
	move.w	$A(sp),-(sp)	* get tick value.  ignore flags
	jsr	_tikfrk		* call interrupt handler extension
	addq.l	#2,sp		* clear the stack
	rts

	page
***************************************************************************
*
*  _s68 - swap bytes in a word
*
***************************************************************************

	text
_s68:
	link	a6,#0
	move.l	8(a6),a0
	move	(a0),d0
	ror	#8,d0
	move	d0,(a0)
	unlk	a6
	rts

***************************************************************************
*
*  _s68l - swap word & bytes in long
*
***************************************************************************

	text
_s68l:
	link	a6,#0
	move.l	8(a6),a0
	move.l	(a0),d0
	ror	#8,d0
	swap	d0
	ror	#8,d0
	move.l	d0,(a0)
	unlk	a6
	rts


_bfill:
*
* Entry
*	4(sp)	-> first byte in block
*	8(sp)	=  BYTE value to fill with
*	10(sp)	=  UWORD number of bytes to fill
* Exit
*	none

	
	move.l	4(sp),a0		* a0 -> block
	move	8(sp),d0		* d0.b = byte to fill with
	move	10(sp),d1		* d1.w = byte count
	bra	f_test

f_loop:
	move.b	d0,(a0)+
f_test:
	dbf	d1,f_loop

	rts



_bmove:
*
* Entry
*	4(sp)	-> first byte in source block
*	8(sp)	-> first byte in destination block
*	C(sp)	=  UWORD number of bytes to copy
* Exit
*	none

	
	move.l	4(sp),a0		* a0 -> source block
	move.l	8(sp),a1		* a1 -> destination block
	move	12(sp),d0		* d0.w = byte count
	bra	b_test

b_loop:
	move.b	(a0)+,(a1)+
b_test:
	dbf	d0,b_loop

	rts



_lbmove:
*
* Entry
*	4(sp)	-> first byte in source block
*	8(sp)	-> first byte in destination block
*	C(sp)	=  LONG number of bytes to copy
* Exit
*	none

	
	move.l	4(sp),a0		* a0 -> source block
	move.l	8(sp),a1		* a1 -> destination block
	move.l	12(sp),d0
	move	d0,d1			* d1.W = ls 16 bits of count
	swap	d0			* d0.W = ms 16 bits of count
	bra	lb_test

lb_loop:
	move.b	(a0)+,(a1)+
lb_test:
	dbf	d1,lb_loop
	dbf	d0,lb_loop

	rts




***************************************************************************
*
*  _xsetjmp -
*
***************************************************************************

	text
_xsetjmp:
	link	a6,#0
	move.l	8(a6),a0
	move.l	0(a6),(a0)+
	lea	8(a6),a1
	move.l	a1,(a0)+
	move.l	4(a6),(a0)
	clr.l	d0
	unlk	a6
	rts

***************************************************************************
*
*  _xlongjmp -
*
***************************************************************************

	text
_xlongjmp:
	link	a6,#0
	move.l	12(a6),d0
	tst.l	d0
	bne	okrc

	move	#1,d0
okrc:
	move.l	8(a6),a0
	move.l	(a0)+,a6
	move.l	(a0)+,a7
	move.l	(a0),-(sp)
	rts

	page
******************************************************************************
*
*	_enter - Front end of TRAP #1 handler
*
******************************************************************************

	text
_enter:
	bsr	_fix_trap

	btst.b	#5,(sp)		* are we in supervisor mode?
	bne	enter_1		* yes, go check sup stack

	move.l	usp,a0		* no, check user stack
	cmp.w	#$20,(a0)	* toggle/inquire state?
	beq	x20_usr		* yes, go perform function

	bra	enter_2

enter_1:
				* next line is 68000 specific
	cmp.w	#$20,6(sp)	* toggle/inquire state?
	beq	x20_sup		* yes, go perform function

enter_2:
	move.l	a6,-(sp)
	move.l	_run,a6
	movem.l	d0/a3-a5,$68(a6)
	move.l	(sp)+,$78(a6)	* olda6
	move	(sp)+,d0	* status reg
	move.l	(sp)+,a4	* retadd
	btst	#13,d0		* if he is in system state, use his stack
	bne	systk
	move.l	usp,a5
	movem.l	d1-d7/a0-a2,-(a5)
	move.l	a4,-(a5)	* retadd
	move	d0,-(a5)	* sr
	move.l	sp,a0
	move.l	a0,-(a5)
	move.l	a5,$7c(a6)
* now switch to fs stack
	move.l	#fstrt,sp
	lea	50(a5),a0
	bra	callos
* he was using his own ssp
systk:	movem.l	d1-d7/a0-a2,-(sp)
	move.l	a4,-(sp)
	move	d0,-(sp)
	move.l	usp,a0
	move.l	a0,-(sp)
	move.l	sp,$7c(a6)
	lea	50(sp),a0
	move.l	#fstrt,sp
callos:	move.l	a0,-(sp)	* push parm pointer
	jsr	_osif
	add.l	#4,sp

*...fall into gouser....

***************************************************************************
*
*  _gouser -
*
***************************************************************************

_gouser:
	move.l	_run,a5
	move.l	d0,$68(a5)
	move.l	$7c(a5),a6	* stack pointer (maybe usp, maybe ssp)
	move.l	(a6)+,a4	* other stack pointer
	move	(a6)+,d0
	move.l	(a6)+,a3	* retadd
	movem.l	(a6)+,d1-d7/a0-a2
	btst	#13,d0
	bne	retsys		* a6 is (user-supplied) system stack
	move.l	a4,sp
	move.l	a6,usp
gousr:	move.l	a3,-(sp)
	move	d0,-(sp)
	movem.l	$68(a5),d0/a3-a6

	bra	_fix_rte

retsys:	move.l	a6,sp
	move.l	a4,usp
	bra	gousr

	bss
	even

	ds.w	1000
fstrt:
	ds.l	1

	page
******************************************************************************
*
* function 0x20 handler
* toggle/inquire processor/stack state
*
******************************************************************************

	text
x20_usr:			* user mode entry point
	move.l	2(a0),d1	* get parameter
	beq	xu_0		* already have new SSP from
*				* old USP, go handle 0L entry

	addq.l	#1,d1		* check for -1L entry
	beq	x20_inq		* go handle inquiry

	move.l	2(a0),a0	* get new SSP from parameter
xu_0:
	move.w	(sp)+,d0	* pop SR
	move.l	(sp)+,-(a0)	* transport the return address
	ori.w	#$2000,d0	* set supervisor mode
	move.w	d0,-(a0)	* push SR
	move.l	sp,d0		* set return value (old SSP)
	move.l	a0,sp		* set new SSP

	bra	_fix_rte

* supervisor mode entry point

x20_sup:
	move.l	8(sp),d1	* get parameter
	beq	xs_0		* go handle 0L entry

	addq.l	#1,d1		* check for -1L entry
	beq	x20_inq		* go handle inquiry

	move.l	8(sp),a1	* point to new SSP
	move.w	(sp)+,d0	* pop SR
	move.l	(sp)+,-(a1)	* transport the return address
	move.w	d0,-(a1)	* push SR
	move.l	usp,a0		* get USP
	cmpa.l	a0,sp		* already at old USP?
	beq	xs_usp		* don't play with stack

	move.l	(sp)+,-(a0)	* transport func # and parm
	move.l	a1,sp		* update SSP
	bra	xs_exit

xs_usp:
	move.l	a1,sp		* update SSP
	bra	xs_scram

xs_0:
	move.l	sp,a0		* create USP
	addq.l	#6,a0		* this is 68000 specific
xs_exit:
	move.l	a0,usp		* update USP
xs_scram:
	andi.w	#$DFFF,(sp)	* set user mode

	bra	_fix_rte

* inquiry mode entry point

x20_inq:
	move.l	#$2000,d0	* set up test mask
	and.w	(sp),d0		* check the supervisor state
	beq	xi_exit		* return 0L for user mode

	move.l	#-1,d0		* return -1L for sup mode

xi_exit:
	bra	_fix_rte

	page
******************************************************************************
*
* call dosjr from within itself (or from linked-in shell)
*
******************************************************************************

	text
_oscall:
	link	a6,#0
	move.l	a0,-(sp)
	lea	8(a6),a0
	move.l	a0,-(sp)
	jsr	_osif
	addq.l	#4,sp
	move.l	(sp)+,a0
	unlk	a6
	rts

	page
******************************************************************************
*
* _fix_trap - make 68010 exception stack frame look like a 68000 frame
*
******************************************************************************

	text
_fix_trap:
	tst.w	mc68010		* check processor type
	beq	ft_exit

	move.w	8(sp),10(sp)	* lsw of return address of TRAP
	move.w	6(sp),8(sp)	* msw of return address of TRAP
	move.w	4(sp),6(sp)	* sr
	move.w	2(sp),4(sp)	* lsw of return address of BSR
	move.w	(sp),2(sp)	* msw of return address of BSR
	add.l	#2,sp		* discard the missing word
ft_exit:
	rts

******************************************************************************
*
* _fix_rte - restore a 68010 exception stack frame
*
******************************************************************************

	text
_fix_rte:
	tst.w	mc68010		* check processor type
	beq	fr_exit

	sub.l	#2,sp		* resverve a hole on top of the stack
	move.w	2(sp),(sp)	* move SR
	move.w	4(sp),2(sp)	* msw of return address of TRAP
	move.w	6(sp),4(sp)	* lsw of return address of TRAP
	clr.w	6(sp)		* store a bogus exception frame type
fr_exit:
	rte
