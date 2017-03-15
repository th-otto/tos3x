	xdef	_umul_div
_umul_div:

	move.w	6(sp),d0	;         m2
	add.w	d0,d0		;       2*m2
	mulu	4(sp),d0	;    m1*2*m2
	divu	8(sp),d0	;   (m1*2*m2)/d1
	addq.w	#1,d0		;  ((m1*2*m2)/d1)+1
	asr.w	#1,d0		; (((m1*2*m2)/d1)+1)/2
	rts
