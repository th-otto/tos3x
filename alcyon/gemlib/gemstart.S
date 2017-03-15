******************************************************************************
*
* gemstart.s -startup code for the C runtime library, aesbind, vdibind
*	
*	IMPORTANT: SEE THE DESCRIPTION OF THE "_stksize" VARIABLE, BELOW.
*
*	This is the startup code for any application running on the Atari ST.
*	This includes gemlib, vdi, and the aes bindings.
*
*	Look carefully at the comments below; they will determine the nature
*	of this startup code (stack size, AES & VDI usage, etc.).
*
*	This must be the first object file in a LINK command. When the
*	operating system gives it control, our process has ALL of memory
*	allocated to it, and our stack pointer is at the top.
*	This code (conditionally) gives some of that memory back
*	to the operating system, places its stack pointer at the top
*	of the memory it still owns, sets up some global variables,
*	and calls __main, the run-time library startup routine (which
*	parses the command line into argc/argv, opens stdin and stdout,
*	etc., before calling the programmer's _main).
*
*	This object file also includes __exit, which is the procedure the
*	runtime library calls to exit back to the operating system.
*	As a new feature with this release, the argument passed to __exit
*	is NOT ignored; it is passed to the operating system using the Pterm
*	function call, which returns it as the value of the Pexec() call
*	which invoked this process.
*
******************************************************************************
*
*	CONFIGURING THIS STARTUP FILE
*
*	There are several memory models which this startup file will
*	assemble for, selected by the assembly variable STACK.
*
*	When your process gets control, it owns its basepage, its text
*	(program) segment, its data segment, its bss segment (uninitialized
*	data), and all the "free memory" from there to the top of the TPA.
*	A process can choose to keep all that free memory for its stack
*	(used by function calls and local variables) and heap (used by
*	malloc()), or it can return some or all of that space to the
*	operating system.
*
*	The memory between the top of your bss segment and your stack pointer
*	is used both for a heap and for the stack.  The line between heap
*	and stack is called the "break".  When malloc() uses up all the
*	memory below the break, it calls _brk() (in this file) to move the 
*	break up.  If the break gets too close to the stack, _brk() returns 
*	an error code, and malloc() will return NULL because it couldn't
*	satisfy the request.  If the stack actually overflows the break,
*	_brk() prints an error message and exits, returning 1 to its parent.
*
*	If you are using the AES or VDI, you must return at least a little of
*	the free memory to the operating system, because they need it.
*	About 8K should be enough. The AES uses this memory for blt buffers,
*	and the VDI uses it for virtual workstation data.
*	Also, if your program uses the Pexec() call, you will need to return
*	some space to make room for the child process.
*
*	It is usually a good idea to keep only as much memory as you will
*	use, because some programs depend on processes returning some 
*	memory to the operating system.
*
*************************************************************************
*
*	Here are the memory models you can set up with the _stksize variable:
*
*	_stksize
*	value	Meaning
*	------	---------------------------------------------------------
*	-1	Keep all the memory for this process. Return NONE of it
*		to the operating system.  This model gives you the most
*		memory.
*
*		WARNING: IF YOU REQUEST ALL OF MEMORY (with STACK = -1),
*		YOU MUST NOT USE THE AES, THE VDI, OR THE BIOS PEXEC() 
*		FUNCTION. PEXEC WILL RETURN A CORRECT ERROR CODE (-39, 
*		ENSMEM), BUT IT WILL ALSO BREAK THE MEMORY MANAGEMENT SYSTEM.
*
*	 0	Return all but a minimal amount (MINSTACK) of the free
*		space to the operating system.  This is a good idea if
*		you know you won't be using malloc() much, or if you
*		will be using Pexec() and your program doesn't need much
*		stack space.  Remember, though, that some library functions,
*		especially fopen(), use malloc() and will use your heap
*		space.
*
*	 1	Keep 1/4 of the free space.  This is a good model if
*		you will be using malloc() a lot, but also want to use 
*		Pexec() to spawn subprocesses.
*
*	 2	Keep 2/4 (1/2) of the free space.  This is good if you
*		use malloc() a lot, but don't want to be too much of a 
*		memory hog.
*
*	 3	Keep 3/4 of the free space.  This is a good choice for
*		programs which use the AES or VDI, because it gives you plenty
*		of room for using malloc(), but leaves enough for the
*		AES and VDI to allocate their buffers, too.
*
*   An example using __stksize C is:
*
*	/* outside all function blocks */
*	long _stksize = 32767;	/* 32K stack+heap */
*		or
*	long _stksize = -8192;	/* keep all but 8K */
*
*	Note that in C, all variables get an underscore stuck on
*	the front, so you just use one underscore in your program.
*
*	Any other POSITIVE value of _stksize will be taken as the number of
*	bytes you want to KEEP for your stack and heap.
*
*	Any other NEGATIVE value of STACK will be taken as the number of
*	bytes you want to give back to the operating system.
*
*	Note that if you give back less than 512 bytes, you still shouldn't
*	use Pexec(), and if you give back less than (about) 4K, you shouldn't
*	use the AES or VDI.
*
*	In all cases, a minimum stack size is enforced.  This minimum is
*	set by the variable MINSTACK in this assembly file.  This value
*	should be at least 256 bytes, but should be more like 4K. If
*	the stack size from the STACK model you choose or the _stksize
*	variable in your program is less than MINSTACK, you'll get
*	MINSTACK bytes.  If there aren't MINSTACK bytes free past the end
*	of your BSS, the program will abort with an error message.
*
*************************************************************************
*
* STACK variable summary:
*	-1=keep all
*	 0=keep MINSTACK bytes
*	 1=keep 1/4 of free memory
*	 2=keep 2/4
*	 3=keep 3/4
*    other=keep that many bytes (positive) or give back that many (negative)

MINSTACK equ 4096		* minimum stack+heap size.
FUDGE equ 512		* minimum space to leave ABOVE our stack

* BASEPAGE ADDRESSES:
p_lowtpa equ $0		* Low TPA address (basepage address)
p_hitpa equ $4		* High TPA address (and initial stack pointer)
p_tbase equ $8		* ptr to Code segment start
p_tlen equ $c		* Code segment length
p_dbase equ $10		* ptr to Data segment start
p_dlen equ $14		* Data segment length
p_bbase equ $18		* ptr to Bss  segment start
p_blen equ $1c		* Bss  segment length
p_dta equ $20		* ptr to process's initial DTA
p_parent equ $24	* ptr to process's parent's basepage
p_reserved equ $28	* reserved pointer
p_env equ $2c		* ptr to environment string for process
p_cmdlin equ $80	* Command line image

*
* CONTROL VARIABLES (used in stack computations)
*
* GOTSTACK: a boolean which is set TRUE if STACK in [-1..4], meaning "don't
*	    assemble the code keeping or returning STACK bytes."
*
* DOSHRINK: a boolean which is set FALSE if STACK is -1, meaning "don't
*	    shrink any memory back to the operating system."
*

* GEMDOS functions:
cconws equ $09		* Cconws(string): write to console
fsetdta equ $1a
mshrink equ $4a		* Mshrink(newsize): shrink a block to a new size
pterm equ $4c		* Pterm(code): exit, return code to parent

	xdef	__start
	xdef	__main
	xdef	__exit
	xdef	_brk
	xdef	__break
	xdef	__base
	xdef	__BasPag
	xdef	__sovf
	xdef	__app
	xdef	__PgmSize
	
	xref    __stksize

	text
*
*  Must be first object file in link statement
*

__start:
	move.l  a0,a3
    move.l  a3,d0
    bne     isacc
    move.l  4(a7),a0	* a0 = basepage address
    moveq.l #1,d0
    bra     isapp
isacc:
	moveq.l #0,d0
isapp:
   	move.l	sp,a1		* save our initial sp (used by ABORT)
	move.l	a0,__base	* base = a0
    move.w  d0,__app
	move.l	p_bbase(a0),d0	* d0 = bss seg start
	add.l	p_blen(a0),d0	* d0 += bss length  (d0 now = start of heap)
	move.l	d0,__break	* __break = first byte of heap
    move.l  d0,d1
    sub.l   a0,d1
    move.l  d1,__PgmSize
    
*************************************************************************
*									*
*	Compute stack size based on MINSTACK, p_hitpa(a0),
*	and __stksize, as appropriate.  Place the SP where you want	*
*	your stack to be. Note that a0 == __base, d0 == __break		*
*									*
*************************************************************************

*************************************************************************
*		STACK = -1: keep all of memory				*
*************************************************************************

	move.l  __stksize,d7
	cmp.l   #-1,d7		* if (STACK == -1)
	bne     minstack
	move.l	p_hitpa(a0),sp	* place stack at top of tpa.
	move.l	d0,d1		* check against MINSTACK
	add.l	#MINSTACK,d1	* d1 = __break + MINSTACK;
	cmp.l	sp,d1		* if (sp < __break + MINSTACK)
	bhi	    abort		*	goto abort;
	bra     gotstack    *   skips doshrink

*************************************************************************
*		STACK = 0: keep only MINSTACK bytes			*
*************************************************************************

minstack:
    tst.l    d7
    bne      stak1
	move.l	#MINSTACK,sp	*	sp = __break+MINSTACK;
	add.l	d0,sp
	bra     doshrink

*************************************************************************
*		STACK = 1: keep 1/4 of available memory			*
*************************************************************************

stak1:
    cmp.l    #1,d7
    bne      stak2
	move.l	p_hitpa(a0),d1	*	d1 = p_hitpa;
	sub.l	d0,d1		*	d1 -= __break; /* d1 = free ram size */
	lsr.l	#2,d1		*	d1 /= 4;
	add.l	d0,d1		*	d1 += __break;     /* d1 = new sp */
	move.l	d1,sp		* 	sp = d1;
	bra     doshrink

*************************************************************************
*		STACK = 2: keep 2/4 of available memory			*
*************************************************************************

stak2:
    cmp.l    #2,d7
    bne      stak3
	move.l	p_hitpa(a0),d1	*	d1 = p_hitpa;
	sub.l	d0,d1		*	d1 -= __break; /* d1 = free ram size */
	lsr.l	#1,d1		*	d1 /= 2;
	add.l	d0,d1		*	d1 += __break; /* d1 = new sp */
	move.l	d1,sp		* 	sp = d1;
	bra     doshrink

*************************************************************************
*		STACK = 3: keep 3/4 of available memory			*
*************************************************************************

stak3:
    cmp.l    #3,d7
    bne      stak4
	move.l	p_hitpa(a0),d1	*	d1 = p_hitpa;
	sub.l	d0,d1		*	d1 -= __break; /* d1 = free ram size */
	lsr.l	#2,d1		*	d1 /= 4;
	move.l	d1,d2		*	d2 = d1
	add.l	d2,d1		*	d1 += d2;
	add.l	d2,d1		*	d1 += d2; /* now d1 = 3*(d1/4) */
	add.l	d0,d1		*	d1 += __break;     /* d1 = new sp */
	move.l	d1,sp		* 	sp = d1;
	bra     doshrink

*************************************************************************
*		STACK is something else: keep (if STACK>0) or give	*
*			back (if STACK<0) STACK bytes			*
*************************************************************************

stak4:
	move.l d7,d1
	bmi	giveback	*	if (__stksize < 0) goto giveback;
	add.l	d0,d1		*	d1 = __base+__STKSIZ; /* new sp */
	bra	gotd1

giveback:
	add.l	p_hitpa(a0),d1	*	d1 += hitpa;

gotd1:	move.l	d1,sp		* gotd1: sp = d1;

*************************************************************************
*									*
* DOSHRINK: take SP as a requested stack pointer. Place it		*
* between (__break+MINSTACK) and (p_hitpa(a0)-FUDGE).  If we can't,	*
* abort. Otherwise, we return the remaining memory back to the o.s.	*
* The reason we always shrink by at least FUDGE bytes is to work around	*
* a bug in the XBIOS Malloc() routine: when there are fewer than 512	*
* bytes in the largest free block, attempting a Pexec() breaks the	*
* memory management system.  Since all models except -1 permit Pexec()	*
* calls, we have to make sure they don't break, even if the Pexec()	*
* fails.  Thus, FUDGE must be at least 512.				*
*									*
*************************************************************************
*
* PSEUDOCODE:
* doshrink(sp)
* {
*	/* if too low, bump it up */
*	if (sp < (__break + MINSTACK))
*		sp = (__break + MINSTACK);
*
*	/* if too high, bump it down */
*	if (sp > (hitpa - FUDGE)) {
*		sp = (hitpa - FUDGE);
*
*		/* if now too low, there's not enough memory */
*		if (sp < (__break + MINSTACK))
*			goto abort;
*	}
*	Mshrink(0,__base,(sp - __base));
* }
*
*************************************************************************

doshrink:
	move.l	d0,d1		*     d1 = __break;
	add.l	#MINSTACK,d1	*     d1 += MINSTACK;
	cmp.l	d1,sp		*     if ((__break+MINSTACK) < sp)
	bhi	minok		* 	goto minok;
	move.l	d1,sp		*     else sp = (__break+MINSTACK)
minok:				* minok:
	move.l	p_hitpa(a0),d2	*     d2 = hitpa;
	sub.l	#FUDGE,d2	*     d2 -= FUDGE;
	cmp.l	d2,sp		*     if ((hitpa - FUDGE) > sp)
	bcs	maxok		*	goto maxok;
*				*     else {
	move.l	d2,sp		*	sp = (hitpa - FUDGE);
	cmp.l	d1,d2		* 	if ((__break+MINSTACK) > (hitpa-FUDGE))
	bcs	abort		*	    goto abort;	/* BAD NEWS */
*				*     }
maxok:

*************************************************************************
* STACK LOCATION HAS BEEN DETERMINED. Return unused memory to the o.s.	*
*************************************************************************

	move.l	sp,d1		*     d1 = sp;
	and.l	#-2,d1		*     /* ensure d1 is even */
	move.l	d1,sp		*     sp = d1;
	sub.l	a0,d1		*     d1 -= __base; /* d1 == size to keep */

	move.l	d1,-(sp)	* push the size to keep
	move.l	a0,-(sp)	* and start of this block (our basepage)
	clr.w	-(sp)		* and a junk word
	move	#mshrink,-(sp)	* and the function code
	trap	#1		* Mshrink(0,__base,(sp-base))
	add.l	#12,sp		* clean the stack after ourselves

*************************************************************************
*									*
* Finally, the stack is set up. Now call _main(cmdline, length).	*
*									*
*************************************************************************

gotstack:
	move.l	__base,a0	* set up _main(cmdline,length)
	lea.l	p_cmdlin(a0),a2	* a2 now points to command line
	move.b	(a2)+,d0	* d0 = length; a2++;
	ext.w	d0		* extend byte count into d0.w
	move.w	d0,-(a7)	* push length
	move.l	a2,-(a7)	* Push commnd
	suba.l	a6,a6		* Clear frame pointer
	jsr	__main		* call main routine	NEVER RETURNS
    move.w d0,-(a7)
    bsr __exit

*
* stack overflow! This external is called by salloc in gemlib as well as above
*
__sovf:
	lea	ovf(pc),a0			*	a0-> message
	move.l	a0,-(a7)			*	load proper reg
	move.w	#cconws,-(sp)	* push fn code
	trap	#1		* Issue message

	moveq   #-1,d0
	move.w  d0,4(a7)

***********************************************************************
*
* _exit(code)	Terminate process, return code to the parent.
*
***********************************************************************

__exit:
	addq.l   #4,a7		* drop return PC off the stack, leaving code
	move.w	#pterm,-(a7)	* push function number
_halt:
	trap	#1		* and trap.
	bra.s _halt

*
* abort: used if the stack setup above fails. Restores the initial sp,
* prints a message, and quits with the error ENSMEM.
*
abort:				* print an abortive message and quit
	move.l	a1,sp		* restore initial sp
	pea.l	abortmsg(pc)	* push string address
	move.w	#cconws,-(a7)	* and function code
	trap	#1		* and trap to print message
	addq.l	#6,a7		* clean off stack
	move.w	#-39,-(a7)	* push error number -39: ENSMEM
	jsr	__exit		* and exit with it.
*
*
_brk:		
	cmp.l	__break,sp	* compare current break with current stack
	bcs	__sovf		* actual stack overflow!
	movea.l	4(sp),a0	* get new break
	move.l	a0,d0		* compare with stack, including 256-byte
	adda.l	#$100,a0	* chicken factor
	cmpa.l	a0,sp		* if (sp < a0+256)
	bcs	badbrk		* 	bad break;
	move.l	d0,__break	* OK break: save the break
	clr.l	d0		* Set OK return
	rts			* return

badbrk:
	move.l	#-1,d0		* Load return reg
	rts			* Return

ovf:		dc.b	'Stack Overflow',13,10,0	* Overflow message
abortmsg:	dc.b	'Cannot initialize stack',13,10,0	* abort message
    even

*
*	Data area
*
	data
	xdef	___pname	* Program Name
___pname:	dc.b	'runtime',0	* Program name

**********************************************************************
*
* BSS AREA
**********************************************************************
	bss
	even
__BasPag:
__base:		ds.l	1	* -> Base Page
__break:	ds.l	1	* Break location
__PgmSize:  ds.l    1
__app:		ds.w	1
