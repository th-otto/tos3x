STACK=16384

MINSTACK=4096		* minimum stack+heap size.
FUDGE=512		* minimum space to leave ABOVE our stack

* BASEPAGE ADDRESSES:
p_lowtpa=$0		* Low TPA address (basepage address)
p_hitpa=$4		* High TPA address (and initial stack pointer)
p_tbase=$8		* ptr to Code segment start
p_tlen=$c		* Code segment length
p_dbase=$10		* ptr to Data segment start
p_dlen=$14		* Data segment length
p_bbase=$18		* ptr to Bss  segment start
p_blen=$1c		* Bss  segment length
p_dta=$20		* ptr to process's initial DTA
p_parent=$24		* ptr to process's parent's basepage
p_reserved=$28		* reserved pointer
p_env=$2c		* ptr to environment string for process
p_cmdlin=$80		* Command line image

*
* CONTROL VARIABLES (used in stack computations)
*
* GOTSTACK: a boolean which is set TRUE if STACK in [-1..4], meaning "don't
*	    assemble the code keeping or returning STACK bytes."
*
* DOSHRINK: a boolean which is set FALSE if STACK is -1, meaning "don't
*	    shrink any memory back to the operating system."
*
gotstack	.equ	0	* set to 1 if STACK in [-1..4]
doshrink	.equ	1	* default is 1; set to 0 if STACK = -1

* GEMDOS functions:
cconws=$09		* Cconws(string): write to console
fsetdta=$1a
mshrink=$4a		* Mshrink(newsize): shrink a block to a new size
pterm=$4c		* Pterm(code): exit, return code to parent

.globl	__start
.globl	__main
.globl	__exit
.globl	_brk
.globl	__break
.globl	___cpmrv
.globl	__base
.globl	__sovf
.globl	_crystal
.globl	_ctrl_cnts


	.text
*
*  Must be first object file in link statement
*

__start:
	move.l	a7,a5		* save our initial sp (used by ABORT)
	move.l	4(a5),a5	* a5 = basepage address
	move.l	a5,__base	* base = a0
	move.l	p_tlen(a5),d0
	add.l	p_dlen(a5),d0
	add.l	p_blen(a5),d0
	add.l	#STACK,d0
	move.l    d0,d1
	add.l     a5,d1
	and.l     #-2,d1
	movea.l   d1,a7
	move.l    d0,-(a7)
	move.l    a5,-(a7)
	clr.w     -(a7)
	move	#mshrink,-(sp)	* and the function code
	trap	#1		* Mshrink(0,__base,(sp-base))
	add.l	#12,sp		* clean the stack after ourselves


*************************************************************************
*									*
* Finally, the stack is set up. Now call _main(cmdline, length).	*
*									*
*************************************************************************

	move.l	__base,a0	* set up _main(cmdline,length)
    movea.l   p_bbase(a0),a1
    adda.l    p_blen(a0),a1
    move.l    a1,__break
    
	lea.l	p_cmdlin(a0),a2	* a2 now points to command line
	move.b	(a2)+,d0	* d0 = length; a2++;
	andi.l    #$000000FF,d0
	move.w	d0,-(a7)	* push length
	move.l	a2,-(a7)	* Push commnd
	suba.l    a6,a6		* Clear frame pointer
	jsr	__main		* call main routine	NEVER RETURNS
    jmp __exit

*
*	For GEMAES calls from AESBIND.ARC or cryslib.o
*
_crystal:
	move.l	4(a7),d1
	move.w	#200,d0
	trap	#2
	rts

*
*
_brk:		
	movea.l	4(sp),a0	* get new break
	move.l	a0,d0		* compare with stack, including 256-byte
	lea.l     256(a0),a0
	cmpa.l	a0,sp		* if (sp < a0+256)
	bcc	okbrk		* 	bad break;
badbrk:
	moveq.l	#-1,d0		* Load return reg
	rts			* Return
okbrk:
	move.l	d0,__break	* OK break: save the break
	clr.l	d0		* Set OK return
	rts			* return

*
*
		xdef	___BDOS
___BDOS:	link	a6,#0			*	link
		move.w	8(sp),d0		*	Load func code
		move.l	10(sp),d1		*	Load Paramter
		trap	#2			*	Enter BDOS
		cmpa.l	__break,sp		*	Check for stack ovf
		bcc	noovf			*	NO overflow, continue
__sovf:		move.w	#9,d0		*	String print
		lea	ovf,a0			*	a0-> message
		move.l	a0,d1			*	load proper reg
		trap	#2			*	Issue message
__exit:		move.w	#0,d0		*	Exit
		trap	#2			*		now
noovf:						*	Here if all OK
		unlk	a6			*
		rts				*	Back to caller
*
*	Block Fill function:
*
*	blkfill(dest,char,cnt);
*
*	BYTE	*dest;		* -> area to be filled
*	BYTE	char;		* =  char to fill
*	WORD	cnt;		* =  # bytes to fill
*
	.globl	_blkfill
	.globl	_memset
_blkfill:
_memset:
	move.l	4(a7),a0	* -> Output area
	move.w	8(a7),d1	* =  output char
	move.w	10(a7),d0	* =  output count
	ext.l	d0		* make it long
	subq.l	#1,d0		* decrement
	ble	filldone	* Done if le
fillit:	move.b	d1,(a0)+	* move a byte
	dbra	d0,fillit	* Continue
filldone: clr.l	d0		* always return 0
	rts			*

*
*	Index function to find out if a particular character is in a string.
*
	.globl	_index
	.globl	_strchr
_index:
_strchr:
	move.l	4(a7),a0	* a0 -> String
	move.w	8(a7),d0	* D0 = desired character
xindex:	tst.b	(a0)		* EOS?
	bne	notend		* No, continue to look
	clr.l	d0		* Not found
	rts			* Quit
notend:	cmp.b	(a0)+,d0	* check for character
	bne	xindex		*	
	move.l	a0,d0		* Found it
	subq.l	#1,d0		* set return pointer
	rts

*
*	control array for vdibind
*
	.data
    	.even
_ctrl_cnts:			 	*	Application Manager
	.dc.b	0, 1, 0			* func 010		
    	.dc.b	2, 1, 1			* func 011		
    	.dc.b	2, 1, 1 		* func 012		
	.dc.b	0, 1, 1			* func 013		
	.dc.b	2, 1, 1			* func 014		
	.dc.b	1, 1, 1			* func 015		
	.dc.b	0, 0, 0			* func 016		
	.dc.b	0, 0, 0			* func 017		
	.dc.b	0, 0, 0			* func 008		
	.dc.b	0, 1, 0			* func 019		
*							 Event Manager
	.dc.b	0, 1, 0			* func 020		
	.dc.b	3, 5, 0			* func 021		
	.dc.b	5, 5, 0			* func 022		
	.dc.b	0, 1, 1			* func 023		
	.dc.b	2, 1, 0			* func 024		
	.dc.b	16, 7, 1 		* func 025		
	.dc.b	2, 1, 0			* func 026		
	.dc.b	0, 0, 0			* func 027		
	.dc.b	0, 0, 0			* func 028		
	.dc.b	0, 0, 0			* func 009		
*							 Menu Manager
	.dc.b	1, 1, 1			* func 030		
	.dc.b	2, 1, 1			* func 031		
	.dc.b	2, 1, 1			* func 032		
	.dc.b	2, 1, 1			* func 033		
	.dc.b	1, 1, 2			* func 034		
	.dc.b	1, 1, 1			* func 005		
	.dc.b	0, 0, 0			* func 006		
	.dc.b	0, 0, 0			* func 007		
	.dc.b	0, 0, 0			* func 008		
	.dc.b	0, 0, 0			* func 009		
*							 Object Manager
	.dc.b	2, 1, 1			* func 040		
	.dc.b	1, 1, 1			* func 041		
  	.dc.b	6, 1, 1			* func 042		
	.dc.b	4, 1, 1			* func 043		
	.dc.b	1, 3, 1			* func 044		
	.dc.b	2, 1, 1			* func 045		
	.dc.b	4, 2, 1			* func 046		
	.dc.b	8, 1, 1			* func 047		
	.dc.b	0, 0, 0			* func 048		
	.dc.b	0, 0, 0			* func 049		
*							 Form Manager
	.dc.b	1, 1, 1			* func 050		
	.dc.b	9, 1, 1			* func 051		
	.dc.b	1, 1, 1			* func 002		
	.dc.b	1, 1, 0			* func 003		
	.dc.b	0, 5, 1			* func 004		
	.dc.b	0, 0, 0			* func 005		
	.dc.b	0, 0, 0			* func 006		
	.dc.b	0, 0, 0			* func 007		
	.dc.b	0, 0, 0			* func 008		
	.dc.b	0, 0, 0			* func 009		
*							 Dialog Manager
	.dc.b	0, 0, 0			* func 060		
	.dc.b	0, 0, 0			* func 061		
	.dc.b	0, 0, 0			* func 062		
	.dc.b	0, 0, 0			* func 003		
	.dc.b	0, 0, 0			* func 004		
	.dc.b	0, 0, 0			* func 005		
	.dc.b	0, 0, 0			* func 006		
	.dc.b	0, 0, 0			* func 007		
	.dc.b	0, 0, 0			* func 008		
	.dc.b	0, 0, 0			* func 009		
*							Graphics Manager
	.dc.b	4, 3, 0			* func 070		
	.dc.b	8, 3, 0			* func 071		
	.dc.b	6, 1, 0			* func 072		
	.dc.b	8, 1, 0			* func 073		
	.dc.b	8, 1, 0			* func 074		
	.dc.b	4, 1, 1			* func 075		
	.dc.b	3, 1, 1			* func 076		
	.dc.b	0, 5, 0			* func 077		
	.dc.b	1, 1, 1			* func 078		
	.dc.b	0, 5, 0			* func 009		
*							Scrap Manager
	.dc.b	0, 1, 1			* func 080		
	.dc.b	0, 1, 1			* func 081		
	.dc.b	0, 0, 0			* func 082		
	.dc.b	0, 0, 0			* func 083		
	.dc.b	0, 0, 0			* func 084		
	.dc.b	0, 0, 0			* func 005		
	.dc.b	0, 0, 0			* func 006		
	.dc.b	0, 0, 0			* func 007		
	.dc.b	0, 0, 0			* func 008		
	.dc.b	0, 0, 0			* func 009		
*							fseler Manager
	.dc.b	0, 2, 2			* func 090		
	.dc.b	0, 0, 0			* func 091		
	.dc.b	0, 0, 0			* func 092		
	.dc.b	0, 0, 0			* func 003		
	.dc.b	0, 0, 0			* func 004		
	.dc.b	0, 0, 0			* func 005		
	.dc.b	0, 0, 0			* func 006		
	.dc.b	0, 0, 0			* func 007		
	.dc.b	0, 0, 0			* func 008		
	.dc.b	0, 0, 0 		* func 009		
*							Window Manager
	.dc.b	5, 1, 0			* func 100		
	.dc.b	5, 1, 0			* func 101		
	.dc.b	1, 1, 0			* func 102		
	.dc.b	1, 1, 0			* func 103		
	.dc.b	2, 5, 0			* func 104		
	.dc.b	6, 1, 0			* func 105		
	.dc.b	2, 1, 0			* func 106		
	.dc.b	1, 1, 0			* func 107		
	.dc.b	6, 5, 0			* func 108		
	.dc.b	0, 0, 0 		* func 009		
*							Resource Manger
	.dc.b	0, 1, 1			* func 110		
	.dc.b	0, 1, 0			* func 111		
	.dc.b	2, 1, 0			* func 112		
	.dc.b	2, 1, 1			* func 113		
	.dc.b	1, 1, 1			* func 114		
	.dc.b	0, 0, 0			* func 115		
	.dc.b	0, 0, 0			* func 006
	.dc.b	0, 0, 0			* func 007
	.dc.b	0, 0, 0			* func 008
	.dc.b	0, 0, 0			* func 009
*							Shell Manager
	.dc.b	0, 1, 2			* func 120
	.dc.b	3, 1, 2			* func 121
	.dc.b	1, 1, 1			* func 122
	.dc.b	1, 1, 1			* func 123
	.dc.b	0, 1, 1			* func 124
	.dc.b	0, 1, 2			* func 125

*
*	Data area
*
	.data
	.globl	___pname	* Program Name
	.globl	___tname	* Terminal Name
	.globl	___lname	* List device name
	.globl	___xeof		* ^Z byte
ovf:		.dc.b	'Stack Overflow$'	* Overflow message
___pname:	.dc.b	'C runtime',0	* Program name
___tname:	.dc.b	'CON:',0	* Console name
___lname:	.dc.b	'LST:',0	* List device name
___xeof:	.dc.b	$1a		* Control-Z

**********************************************************************
*
* BSS AREA
**********************************************************************
	.bss
	.even
__base:		.ds.l	1	* -> Base Page
__break:	.ds.l	1	* Break location
___cpmrv:	.ds.w	1	* Last CP/M return val

	.end
