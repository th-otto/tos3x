	xdef	_MUL_DIV
_MUL_DIV: ds.b 0
	xdef	_mul_div
_mul_div: ds.b 0

	move.w	6(sp),d0	;	 m2
	add.w	d0,d0		;      2*m2
	muls	4(sp),d0	;   m1*2*m2
	divs	8(sp),d0	;  (m1*2*m2)/d1
	bmi	md_1

	addq.w	#1,d0		;  ((m1*2*m2)/d1)+1
	asr.w	#1,d0		; (((m1*2*m2)/d1)+1)/2
	rts

md_1:	subq.w	#1,d0		;  ((m1*2*m2)/d1)-1
	asr.w	#1,d0		; (((m1*2*m2)/d1)-1)/2
	rts

