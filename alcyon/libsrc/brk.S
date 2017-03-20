		xref	__break
		xdef	_brk
		text
_brk:		
		cmp.l	__break,sp	* compare current break with current stack
		bcs		gosovf		* actual stack overflow!
		movea.l	4(sp),a0	* New break?
		move.l	a0,d0		* compare with stack, including 256-byte chicken factor
		lea	$100(a0),a0
		cmpa.l	a0,sp		* if (sp < a0+256)
		bcs	badbrk			* 	bad break;
		move.l	d0,__break	* OK break: save the break
		clr.l	d0			* Set OK return
		rts

badbrk:
		move.l	#-1,d0		* Load return reg
		rts
gosovf:
		jmp		__sovf		* in crt0.o
