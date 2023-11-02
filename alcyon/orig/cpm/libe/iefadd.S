       ttl     ieee format equivalent add/subtract (iefadd/iefsub)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************************
*                     iefadd/iefsub                         *
*  fast floating point ieee format equivalent add/subtract  *
*                                                           *
*  iefadd - ieee format equivalent floating point addition  *
*                                                           *
*  input:  d6 - ieee format number addend (source)          *
*          d7 - ieee format number adder  (destination)     *
*                                                           *
*  iefsub - ieee format equivalent floating point subtract  *
*                                                           *
*  input:  d6 - ieee format number subtrahend (source)      *
*          d7 - ieee format number minuend (destination)    *
*                                                           *
*  output: d7 - ieee format floating result of register d6  *
*               added or subtracted from register d7        *
*                                                           *
*  condition codes:                                         *
*          n - result is negative                           *
*          z - result is zero                               *
*          v - result is nan (not-a-number)                 *
*          c - undefined                                    *
*          x - undefined                                    *
*                                                           *
*           all registers transparent                       *
*                                                           *
*        maximum used stack:    28 bytes                    *
*                                                           *
*  result matrix:            arg 2                          *
*                  others    +inf      -inf        nan      *
*     arg 1      ****************************************   *
*   others       *   a    *    b    *    c     *    f   *   *
*   +infinity    *   b    *    b    *    d     *    f   *   *
*   -infinity    *   c    *    d    *    c     *    f   *   *
*   nan          *   e    *    e    *    e     *    f   *   *
*                ****************************************   *
*       a = return addition or subtraction result,          *
*           overflowing to infinity, underflowing to zero   *
*       b = return plus infinity                            *
*       c = return minus infinity                           *
*       d = return newly created nan (not-a-number)         *
*       e = return arg1 (nan) unchanged                     *
*       f = return arg2 (nan) unchanged                     *
*                                                           *
*  notes:                                                   *
*    1) for subtraction, the sign of the source is          *
*       inverted and then the operation is treated as       *
*       an addition using the decision matrix above.        *
*    2) see the mc68344 user's guide for a description of   *
*       the possible differences between the results        *
*       returned here versus those required by the          *
*       ieee standard.                                      *
*                                                           *
*************************************************************
         page
*iefadd .idnt    1,1  ieee format equivalent add/subtract

         xdef      iefadd    | ieee format addition
         xdef      iefsub    | ieee format subtraction

         xref      iefdop   | double argument conversion routine
         xref      iefrtnan | create and return nan result routine
         xref      iefrtd7  | return contents of d7 as the result
         xref      iefrtsz  | return signed zero with sign of d7
         xref      ieftieee | return and convert back to ieee format
         xref      ffpadd   | reference fast floating point add routine
         xref      ffpcpyrt | copyright notice

         text

************************
* subtract entry point *
************************
iefsub:  bchg.l    #31,d6    | invert sign of second argument for subtract
         jsr       iefadd    | and call add routine
         move.w    sr,-(sp)  | save ccr of result on the stack [vlh] was sr
         bchg.l    #31,d6    | revert sign back to original condition
         rtr                 | return with result and condition codes

*******************
* add entry point *
*******************
iefadd:  jsr       iefdop    | decode both operands
         bra       iefnrm    | +0 branch normalized
         bra       iefinf2   | +2 branch arg2 infinity
         bra       iefinf1   | +4 branch arg1 infinity
* test for opposite signs               +6 both are infinity
         eor.l     d6,d7     | exclusive or signs
*        bmi       iefrtnan  | opposite signs - go return a nan
		 bmi	   GOTHERE
		 jmp       iefinf1
GOTHERE: jmp       iefrtnan  |  loader problem

*                                      otherwise both same and return same

* arg1 infinity - return it
iefinf1: move.l    d6,d7     | return arg1
* arg2 infinity - return it (already in d7)
iefinf2: jmp       iefrtd7   | return d7 as our result

* normalized numbers - do the addition
iefnrm:  jsr       ffpadd    | do fast floating point add
*        bne       ieftieee  | convert result back to ieee format
		 bne       DOIT
		 jmp	   NOPE
DOIT:	 jmp       ieftieee
* the above few lines had to be changed because of the loader problem

* result is zero - return with proper sign for rn (round-to-nearest)
NOPE:
         movem.l   (sp),d3-d7  | reload arguments (and rest of registers)
         and.l     d6,d7     | return minus only if both minus
         jmp       iefrtsz   | return signed zero with sign of d7

         end
