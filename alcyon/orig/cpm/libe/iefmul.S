       ttl     ieee format equivalent multiply (iefmul)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************************
*                       iefmul                              *
*  fast floating point ieee format equivalent multiply      *
*                                                           *
*  iefmul - ieee format equivalent floating point multiply  *
*                                                           *
*  input:  d6 - ieee format number multiplier (source)      *
*          d7 - ieee format number multiplican (destination)*
*                                                           *
*  output: d7 - ieee format floating result of register d6  *
*               multiplied by register d7                   *
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
*        maximum stack used:   24 bytes                     *
*                                                           *
*  result matrix:            arg 2                          *
*                normalized  zero       inf        nan      *
*     arg 1      ****************************************   *
*   normalized   *   a    *    b    *    c     *    f   *   *
*   zero         *   b    *    b    *    d     *    f   *   *
*   infinity     *   c    *    d    *    c     *    f   *   *
*   nan          *   e    *    e    *    e     *    f   *   *
*                ****************************************   *
*               (denormalized values are treated as zeroes) *
*       a = return multiply result, overflow to infinity,   *
*           underflow to zero                               *
*       b = return zero                                     *
*       c = return infinity                                 *
*       d = return newly created nan (not-a-number) for     *
*           illegal operation
*       e = return arg1 (nan) unchanged                     *
*       f = return arg2 (nan) unchanged                     *
*                                                           *
*  notes:                                                   *
*    1) zeroes and infinities are returned with proper      *
*       sign (exclusive or of input argument sign bits).    *
*    2) see the mc68344 user's guide for a description of   *
*       the possible differences between the results        *
*       returned here versus those required by the          *
*       ieee standard.                                      *
*                                                           *
*************************************************************
         page
*iefmul .idnt    1,1  ieee format equivalent multiply

         xdef      iefmul    | ieee format equivalent multiply

         xref      iefdop   | double argument conversion routine
         xref      iefrtnan | create and return nan result routine
         xref      ieftieee | return and convert back to ieee format
         xref      iefrtie  | return signed infinity exclusive or'ed
         xref      iefrtsze | return signed zero exclusive or'ed
         xref      ffpmul2  | reference ffp perfect precision mult routine
         xref      ffpcpyrt | copyright notice

         text

***********************************************
* ieee format equivalent multiply entry point *
***********************************************
iefmul:  jsr       iefdop    | decode both operands
         bra.s     iefnrm    | +0 branch normalized
         bra.s     iefinf2   | +2 branch only arg2 infinity
         bra.s     iefinf1   | +4 branch only arg1 infinity
* both infinity, return proper sign     +6 both are infinity
iefrtinf:jmp       iefrtie   | returne infinity with proper sign

* arg1 infinity - swap arguments and treat as arg2
iefinf1: exg.l     d6,d7     | swap for next code portion

* arg2 infinity - if opposite argument is zero than illegal and return nan
iefinf2: tst.l     d6        | ? is opposite argument a zero
         bne.s     iefrtinf  | no, go return infinity with proper sign
         jmp       iefrtnan  | yes, return a nan for this illegal operation

* normalized numbers(or zero) - do the multiply
iefnrm:  jsr       ffpmul2   | do fast floating point add
*        bne       ieftieee  | convert result back to ieee format
         bne       DOIT
         jmp       NOPE
DOIT:    jmp       ieftieee  | loader problems again

* result is zero so return zero with proper sign
NOPE:    jmp       iefrtsze  | return zero with exclusively or'ed signed

         end
