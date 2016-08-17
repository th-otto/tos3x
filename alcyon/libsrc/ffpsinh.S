         ttl       fast floating point hyperbolics (ffpsinh)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************
 
*************************************************
*            ffpsinh/ffpcosh/ffptanh            *
*       fast floating point hyperbolics         *
*                                               *
*  input:   d7 - floating point argument        *
*                                               *
*  output:  d7 - hyperbolic result              *
*                                               *
*     all other registers are transparent       *
*                                               *
*  code size:  36 bytes   stack work: 50 bytes  *
*                                               *
*  calls: ffpexp, ffpdiv, ffpadd and ffpsub     *
*                                               *
*  condition codes:                             *
*        z - set if the result is zero          *
*        n - set if the result is negative      *
*        v - set if overflow occurred           *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*  notes:                                       *
*    1) an overflow will produce the maximum    *
*       signed value with the "v" bit set.      *
*    2) spot checks show at least seven digit   *
*       precision.                              *
*                                               *
*  time: (8mhz no wait states assumed)          *
*                                               *
*        sinh  623 microseconds                 *
*        cosh  601 microseconds                 *
*        tanh  623 microseconds                 *
*                                               *
*************************************************
*ffpsinh  idnt  1,2 ffp sinh cosh tanh
 
         text
 
         xdef      ffpsinh,ffpcosh,ffptanh       | entry points
 
         xref      ffpexp,ffpdiv,ffpadd,ffpsub | functions called
         xref      ffpcpyrt            | copyright stub
 
fpone    equ       $80000041           | floating one
 
**********************************
*            ffpcosh             *
*  this function is defined as   *
*            x    -x             *
*           e  + e               *
*           --------             *
*              2                 *
* we evaluate exactly as defined *
**********************************
 
ffpcosh: move.l    d6,-(sp)  | save our one work register
         and.b     #$7f,d7   | force positive (results same but exp faster)
         jsr       ffpexp    | evaluate e to the x
         bvs.s     fhcrtn    | return if overflow (result is highest number)
         move.l    d7,-(sp)  | save result
         move.l    d7,d6     | setup for divide into one
         move.l    #fpone,d7 | load floating point one
         jsr       ffpdiv    | compute e to -x as the inverse
         move.l    (sp)+,d6  | prepare to add together
         jsr       ffpadd    | create the numerator
         beq.s     fhcrtn    | return if zero result
         sub.b     #1,d7     | divide by two
         bvc.s     fhcrtn    | return if no underflow
         move.l    #0,d7     | return zero if underflow
fhcrtn:  movem.l   (sp)+,d6  | restore our work register
         rts                 | return to caller with answer
         page
**********************************
*            ffpsinh             *
*  this function is defined as   *
*            x    -x             *
*           e  - e               *
*           --------             *
*              2                 *
* however, we evaluate it via    *
* the cosh formula since its     *
* addition in the numerator      *
* is safer than our subtraction  *
*                                *
* thus the function becomes:     *
*            x                   *
*    sinh = e  - cosh            *
*                                *
**********************************
 
ffpsinh: move.l    d6,-(sp)  | save our one work register
         jsr       ffpexp    | evaluate e to the x
         bvs.s     fhsrtn    | return if overlow for maximum value
         move.l    d7,-(sp)  | save result
         move.l    d7,d6     | setup for divide into one
         move.l    #fpone,d7 | load floating point one
         jsr       ffpdiv    | compute e to -x as the inverse
         move.l    (sp),d6   | prepare to add together
         jsr       ffpadd    | create the numerator
         beq.s     fhszro    | branch if zero result
         sub.b     #1,d7     | divide by two
         bvc.s     fhszro    | branch if no underflow
         move.l    #0,d7     | zero if underflow
fhszro:  move.l    d7,d6     | move for final subtract
         move.l    (sp)+,d7  | reload e to x again and free
         jsr       ffpsub    | result is e to x minus cosh
fhsrtn:  movem.l   (sp)+,d6  | restore our work register
         rts                 | return to caller with answer
         page
**********************************
*            ffptanh             *
*  this function is defined as   *
*  sinh/cosh which reduces to:   *
*            2x                  *
*           e  - 1               *
*           ------               *
*            2x                  *
*           e  + 1               *
*                                *
* which we evaluate.             *
**********************************
 
ffptanh: move.l    d6,-(sp)  | save our one work register
         tst.b     d7        | ? zero
         beq.s     ffptrtn   | return true zero if so
         add.b     #1,d7     | x times two
         bvs.s     ffptovf   | branch if overflow/underflow
         jsr       ffpexp    | evaluate e to the 2x
         bvs.s     ffptovf2  | branch if too large
         move.l    d7,-(sp)  | save result
         move.l    #fpone,d6 | load floating point one
         jsr       ffpadd    | add 1 to e**2x
         move.l    d7,-(sp)  | save denominator
         move.l    4(sp),d7  | now prepare to subtract
         jsr       ffpsub    | create numerator
         move.l    (sp)+,d6  | restore denominator
         jsr       ffpdiv    | create result
         add.l     #4,sp     | free e**2x off of stack
ffptrtn: move.l    (sp)+,d6  | restore our work register
         rts                 | return to caller with answer
 
ffptovf: move.l    #$80000082,d7 | float one with exponent over to left
         roxr.b    #1,d7     | shift in correct sign
         bra.s     ffptrtn   | and return

ffptovf2: move.l   #fpone,d7 | return +1 as result
         bra.s     ffptrtn
