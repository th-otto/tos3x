******************************************************************************
*
*	C runtime startup for CP/M-68k.
*
******************************************************************************
*
ltpa equ 0			* Low TPA address
htpa equ 4			* High TPA address
lcode equ 8			* Code segment start
codelen equ 12		* Code segment length
ldata equ 16		* Data segment start
datalen equ 20		* Data segment length
lbss equ 24			* Bss  segment start
bsslen equ 28		* Bss  segment length
freelen equ 32		* free segment length
resvd equ 36		* Reserved
command equ 128		* Command tail
prtstr equ 9		* Cconws Print string GEMDOS Call
exit equ 76			* Pterm GEMDOS exit call
		xref	__main
		xdef	__exit
		xdef	__break
		xdef	__start
		xdef	__base
		xdef	__BasPag
		xdef	__PgmSize
	    xdef	__sovf

		text
__start:
		bra	start			*	Branch around ID
		dc.b 'CLEAR68K V02.00, Copyright(c) 1984, Digital Research '
serial:		dc.b	'XXXX-0000-654321'	*	serial number
		even
start:		move.l	4(a7),a0		*	a0 -> Base page
		move.l	a0,__base		*	Load C external
		move.l	lbss(a0),a1		*	a1 -> bss region
		adda.l	bsslen(a0),a1		*	a1 -> 1st heap loc
		move.l	a1,__break		*	Put in "break" loc
		suba.l  a0,a1
		move.l  a1,__PgmSize
		lea.l	command(a0),a2		*	a2 -> command line
		moveq	#0,d0			*	clear junk
		move.b	(a2)+,d0		*	d0 = byte count
		move.w	d0,-(a7)		*	push length
		move.l	a2,-(a7)		*	Push commnd
		suba.l	a6,a6			*	Clear frame pointer
		jsr	__main			*	call main routine
		move.w d0,-(a7)
		bsr.s	__exit			*	call "exit"
*
*
		bss
__BasPag:
__base:		ds.l	1			*	-> Base Page
__break:	ds.l	1			*	Break function
__PgmSize:  ds.l    1

		text
*
*
__sovf:
		lea	ovf(pc),a0			*	a0-> message
		move.l	a0,-(a7)			*	load proper reg
		move.w	#prtstr,-(a7)		*	String print
		trap	#1			*	Issue message
		moveq   #-1,d0
		move.w  d0,4(a7)

__exit:
		addq.l   #4,a7			* pop return pc; we dont return to caller
		move.w	#exit,-(a7)		*	Exit
_halt:
		trap	#1			*		now
		bra.s _halt

ovf:		dc.b	'Stack Overflow'	*	Error message
    dc.b 13,10,0
    even
*
*	Data area
*
		data
		xdef	___pname		*	Program Name

___pname:	dc.b	'C runtime',0		*	Program name
