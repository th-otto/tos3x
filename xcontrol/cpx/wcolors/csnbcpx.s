        xref    _cpx_init
        xref    __VDIinit
        xref    __FPCinit
        xdef	__tos
        xdef	__clkstart
        xdef	__country

        text

		clr.l     -(a7)
		move.w    #$0020,-(a7)
		trap      #1
		move.l    ($000004BA).w,__clkstart
		movea.l   ($000004F2).w,a0
		movea.l   8(a0),a0
		move.w    2(a0),__tos
		move.w    28(a0),d1
		lsr.w     #1,d1
		move.w    d1,__country
		move.l    d0,2(a7)
		move.w    #$0020,(a7)
		trap      #1
		addq.w    #6,a7
		jsr       __VDIinit
		jsr       __FPCinit
		jmp       _cpx_init
		illegal

		bss

__savePC	ds.l	1
__saveD2A2	ds.l	2
__clkstart	ds.l	1
__msiz	ds.l	1
__mblk	ds.l	1
__maxblk	ds.w	1
__OSERR
__oserr	ds.l	1
__tos	ds.w	1
__country	ds.l	1

	    end
