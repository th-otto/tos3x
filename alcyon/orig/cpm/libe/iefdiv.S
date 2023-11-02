       ttl     ieee format equivalent divide (iefdiv)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************************
*                        iefdiv                             *
*     fast floating point ieee format equivalent divide     *
*                                                           *
*  input:  d6 - ieee format number divisor (source)         *
*          d7 - ieee format number dividend (destination)   *
*                                                           *
*  output: d7 - ieee format floating result of register d6  *
*               divided into register d7                    *
*                                                           *
*  condition codes:                                         *
*          n - result is negative                           *
*          z - result is zero                               *
*          v - result is nan (not-a-number)                 *
*          c - undefined                                    *
*          x - undefined                                    *
*                                                           *
*               all registers transparent                   *
*                                                           *
*            maximum stack usage:  24 bytes                 *
*                                                           *
*  result matrix:              arg 2                        *
*                  others    zero    infinity      nan      *
*     arg 1      ****************************************   *
*   others       *   a    *    b    *    c     *    f   *   *
*   zero         *   c    *    d    *    c     *    f   *   *
*   infinity     *   b    *    b    *    d     *    f   *   *
*   nan          *   e    *    e    *    e     *    f   *   *
*                ****************************************   *
*       a = return divide result, overflowing to infinity,  *
*           underflowing to zero with proper sign           *
*       b = return zero with proper sign                    *
*       c = return infinity with proper sign                *
*       d = return newly created nan (not-a-number)         *
*       e = return arg1 (nan) unchanged                     *
*       f = return arg2 (nan) unchanged                     *
*                                                           *
*  notes:                                                   *
*    1) see the mc68344 user's guide for a description of   *
*       the possible differences between the results        *
*       returned here versus those required by the          *
*       ieee standard.                                      *
*                                                           *
*************************************************************
         page
*iefdiv -idnt    1,1  ieee format equivalent divide

         xdef      iefdiv    | ieee format equivalent divide

         xref      iefdop   | double argument conversion routine
         xref      iefrtnan | create and return nan result routine
         xref      ieftieee | return and convert back to ieee format
         xref      iefrtsze | return signed zero with exclusive or signs
         xref      iefrtie  | return infinity with exclusive or signs
         xref      ffpdiv   | reference fast floating point divide routine
         xref      ffpcpyrt | copyright notice

         text

**********************
* divide entry point *
**********************
iefdiv:  jsr       iefdop    | decode both operands
         bra       iefnrm    | +0 branch normalized
         bra       iefrtinf  | +2 branch arg2 infinity
         bra       iefrtzro  | +4 branch arg1 infinity
* both are infinity - return a nan      +6 both are infinity
         jmp       iefrtnan  | return a nan for infinity into infinity

* arg1 infinity - return zero with proper sign
iefrtzro:jmp       iefrtsze  | return zero with exclusive or'ed sign

* arg2 infinity but not arg1 - return infinity with proper sign
iefrtinf:jmp       iefrtie   | return infinity with exclusive or'ed sign

* normalized numbers - test for zeroes
iefnrm:  tst.l     d7        | ? dividend zero (arg2)
         bne.s     ief2nz    | no, go test divisor for zero
         tst.l     d6        | ? are both zero
         bne.s     iefrtzro  | no, just dividend - return a zero
         jmp       iefrtnan  | return a nan for zero into zero

* dividend (arg2) not zero
ief2nz:  tst.l     d6        | ? divisor zero (arg1) but not dividend (arg2)
         beq.s     iefrtinf  | yes, return infinity with proper sign

* both arguments non zero and normalized - do the divide
         jsr       ffpdiv    | do fast floating point divide
         beq       iefrtzro  | if result is zero return a proper sign zero
         jmp       ieftieee  | convert result back to ieee format

         end
