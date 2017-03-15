*******************************************************************************
*******************************************************************************
**
**	_vec_len
**
**		This routine computes the length of a vector using the
**		formula: SQRT( dX**2 + dY**2 )
**
**		input:	4(sp) = dX
**			6(sp) = dY
**
**		output: d0 = SQRT( dX**2 + dY**2 )
**
**		destroys: d0-d4
**
*******************************************************************************
*******************************************************************************

		xdef	_vec_len
_vec_len:
		move.w	4(sp),d0	; d0 <- dX
		muls	d0,d0		; d0 <- dX**2
		move.w	6(sp),d1	; d1 <- dY
		muls	d1,d1		; d1 <- dY**2
		add.l	d0,d1		; d1 <- dX**2 + dY**2
		beq	vl_out		; quit on null length

*   Compute an initial upper and lower bound on the square root. The lower
*   bound is the largest number that is no more than half the length of
*   The square; the upper bound is twice the lower bound.

		move.l	d1,d0		; d0 <- dX**2 + dY**2
		moveq.l	#0,d2		; d2 <- initial exponent: 2**00
		cmp.l	#$10000,d1	; significant digits in high word?
		bcs	bds_lp		; if so, concentrate on high word.

		swap	d1		; d1 <- high word of square
		moveq.l	#16,d2		; d2 <- initial exponent: 2**16
		
bds_lp:		cmp.w	#1,d1		; any significant digits left ?
		beq	bds_end		; no => time to calculate bounds

		addq.w	#1,d2		; d2 <- exponent +1
		lsr.w	#1,d1		; d1 <- square/2
		bra	bds_lp

bds_end:	asr.w	#1,d2		; d2 <- exponent/2  (square root)
		moveq.l	#1,d3
		asl.w	d2,d3		; d3 <- initial lower bound
		move.w	d3,d2
		add.w	d2,d2		; d2 <- initial upper bound
		bne	srch_lp		; check for overflow

		subq.w	#1,d2		; d2 <- maximum value: FFFF


*   Now, perform a binary search for the square root.

srch_lp:	move.w	d2,d1		; d1 <- upper bound
		sub.w	d3,d1		; d1 <- upper bound - lower bound
		cmp.w	#1,d1		;
		beq	srch_end	; quit when the delta is 1

		asr.w	#1,d1		; d1 <- half the difference
		add.w	d3,d1		; d1 <- midway 'tween the boundries
		move.w	d1,d4		; d4 <- copy of candidate
		mulu	d1,d1		; d1 <- candidate**2
		cmp.l	d0,d1		; compare with target
		bhi	hi_adjust	; if candidate too large, branch.
		bcs	lo_adjust	; if candidate too small, branch.

		move.w	d4,d0		; d0 <- exactly the square root
		rts


hi_adjust:	move.w	d4,d2		; d2 <- new upper bound
		bra	srch_lp

lo_adjust:	move.w	d4,d3		; d3 <- new lower bound
		bra	srch_lp

srch_end:	move.w	d3,d0		; no match: use the lower bound

vl_out:		rts

