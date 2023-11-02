         ttl       ieee format front-end routines (ieffront)
*ieffront .idnt   1,1  ieee format equivalent front-end routines
******************************************
*  (c)  copyright 1981 by motorola inc.  *
******************************************

*******************************************************
*               idfsop (internal subroutine)          *
*     ieee format equivalent process single operand   *
*                                                     *
*  input:   d7 - ieee format number argument2         *
*           sp -> +0 return address to caller         *
*                 +4 original caller's return address *
*                                                     *
*  output:  d6 converted to fast floating point       *
*           format with user's original registers     *
*           d3-d7 stacked or a direct return bypassing*
*           the first-line routine if either          *
*           parameter was a nan                       *
*                                                     *
*                                                     *
*     return is via vectored branch with offset added *
*     to the address on the stack.  this allows easy  *
*     type descrimination by the caller for selected  *
*     data types:                                     *
*                                                     *
*   return  +0   if the argument is normalized        *
*                (including zero and denormalized)    *
*           +2   if argument is an infinity           *
*                                                     *
*   the stack appears:  s+0  original d3-d7 upon entry*
*                       s+20 original caller's return *
*                                                     *
*  condition codes:                                   *
*                                                     *
*       (only if bypassed return done)                *
*                                                     *
*            n - undefined                            *
*            z - cleared                              *
*            v - set (result is a nan)                *
*            c - undefined                            *
*            x - undefined                            *
*                                                     *
*       (only if return to immediate caller)          *
*                                                     *
*            n - set if value is negative             *
*            z - set if value is a zero               *
*            v - undeinfed                            *
*            c - undefined                            *
*            x - undefined                            *
*                                                     *
*  this routine is a front end for the ieee format    *
*  capatible routines (ief routines).  it may         *
*  bypass the caller to directly return to the user   *
*  code if the argument is a nan (not-a-number)       *
*  since the result must be a nan (the same).         *
*  the call to this routine must be the               *
*  first instruction of the level-1 ief routine,      *
*  since it may return directly to the original       *
*  caller.  also registers must be unaltered before   *
*  the call.                                          *
*                                                     *
*  logic:  the following decision tree shows the      *
*          processing for this routine and what       *
*          values are returned for the argument       *
*                                                     *
*   if arg is nan then                                *
*         set the "v" bit in the ccr and bypass the   *
*         caller returning with arg (d7) unchanged    *
*                                                     *
*   if arg is an infinity then adjust return address  *
*         and return with ccr set for plus or minus   *
*         at offset +2                                *
*                                                     *
*   if arg is denormalized then set it to zero        *
*        else it is normalized - convert to ffp format*
*                                                     *
*   return to caller at +0 offset                     *
*                                                     *
*  notes:                                             *
*  1) during the conversion of normalized ieee format *
*     numbers to ffp format, the exponent may be too  *
*     large for ffp magnitudes.  when this is true    *
*     then the value is converted to an infinity with *
*     the proper sign.  if the exponent is too small  *
*     than a zero is returned.  see the mc68344 user's*
*     guide for complete details of the range handling*
*     of the fast floating point format conversion.   *
*  2) all zeroes are treated as positive values.      *
*  3) denormalized values are treated as zeroes.      *
*                                                     *
*******************************************************

         text

         xdef      iefsop    | single operand convert

expmsk   equ       $7f800000 | ieee format exponent mask
vbit     equ       $0002     | condition code "v" bit mask
cbit     equ       $0001     | condition code "c" bit mask

* call internal subroutine to parse argument (d7)
iefsop:  lea       -16(sp),sp | adjust stack for new return address position
         move.l    16(sp),-(sp) | move return address to new location
         movem.l   d3-d7,4(sp) | save original caller's registers
         bsr.s     iefprse   | convert argument two
         bcc.s     iefargs   | branch not infinity
         move.w    sr,d5     | save ccr [vlh] was sr
         add.l     #2,(sp)   | adjust return address
         move.w    d5,ccr    | restore ccr
iefargs: rts                 | return to caller at proper offset
         page
*******************************************************
*               idfdop (internal subroutine)          *
*     ieee format equivalent process double operand   *
*                                                     *
*  input:   d6 - ieee format number argument1         *
*           d7 - ieee format number argument2         *
*           sp -> +0 return address to caller         *
*                 +4 original caller's return address *
*                                                     *
*  output:  d6/d7 converted to fast floating point    *
*           format with user's original registers     *
*           d3-d7 stacked or a direct return bypassing*
*           the first-line routine if either          *
*           parameter was a nan                       *
*                                                     *
*                                                     *
*     return is via vectored branch with offset added *
*     to the address on the stack.  this allows easy  *
*     type descrimination by the caller for selected  *
*     data types:                                     *
*                                                     *
*   return  +0   if both arguments normalized         *
*                (including zero and denormalized)    *
*           +2   if arg2 is an infinity               *
*           +4   if arg1 is an infinity               *
*           +6   if both arguments are infinities     *
*                                                     *
*   the stack appears:  s+0  original d3-d7 upon entry*
*                       s+20 original caller's return *
*                                                     *
*  condition codes:                                   *
*                                                     *
*      (only if bypassed return done)                 *
*                                                     *
*            n - undefined                            *
*            z - cleared                              *
*            v - set (result is a nan)                *
*            c - undefined                            *
*            x - undefined                            *
*                                                     *
*      (only if return directly to immediate caller)  *
*                                                     *
*            n - set if arg1 is negative              *
*            z - set if arg1 is zero                  *
*            v - undefined                            *
*            c - set if arg1 is an infinity           *
*            x - undefined                            *
*                                                     *
*  this routine is a front end for the ieee format    *
*  capatible routines (ief routines).  it may         *
*  bypass the caller to directly return to the user   *
*  code if an argument is a nan (not-a-number)        *
*  since the result must be a nan.  it must be the    *
*  first instruction of the level-1 ief routine,      *
*  since it may return directly to the original       *
*  caller.  also registers must be unaltered before   *
*  the call.                                          *
*                                                     *
*  logic:  the following decision tree shows the      *
*          processing for this routine and what       *
*          values are returned for arg1 and arg2      *
*                                                     *
*   if arg2 is nan then                               *
*         set the "v" bit in the ccr and bypass the   *
*         caller returning with arg2 (d7) unchanged   *
*                                                     *
*   if arg1 is nan then                               *
*         set the "v" bit in the ccr and bypass the   *
*         caller returning with arg1 copied to arg2   *
*                                                     *
*   if arg2 is an infinity then adjust return address *
*                                                     *
*   if arg1 is an infinity then adjust return address *
*                                                     *
*   if arg2 is denormalized then set it to zero       *
*        else it is normalized - convert to ffp format*
*                                                     *
*   if arg1 is denormalized then set it to zero       *
*        else it is normalized - convert to ffp format*
*                                                     *
*   return to caller at proper offset                 *
*                                                     *
*  notes:                                             *
*  1) during the conversion of normalized ieee format *
*     numbers to ffp format, the exponent may be too  *
*     large for ffp magnitudes.  when this is true    *
*     then the value is converted to an infinity with *
*     the proper sign.  if the exponent is too small  *
*     than a zero is returned.  see the mc68344 user's*
*     guide for complete details of the range         *
*     treatment by the fast floating point conversion.*
*  2) all zeroes are treated as positive values.      *
*  3) denormalized values are treated as zeroes.      *
*                                                     *
*******************************************************

         text

         xdef      iefdop    | dual operand convert

* call internal subroutine to parse argument 2 (d7)
iefdop:  lea       -16(sp),sp | reset stack for register storage
         move.l    16(sp),-(sp) | move return address to new bottom of stack
         movem.l   d3-d7,4(sp) | save original registers above return address
         bsr.s     iefprse   | convert argument two
         bcc.s     iefarg2   | branch not infinity
         add.l     #2,(sp)   | adjust return address
iefarg2: exg.l     d6,d7     | swap arguments to convert arg1 (nan returns arg1)
         bsr.s     iefprse   | convert second argument
         bcc.s     iefnoti   | branch not infinity
         move.w    sr,d5     | save ccr [vlh] was sr
         add.l     #4,(sp)   | adjust return address
         move.w    d5,ccr    | restore ccr
iefnoti: exg.l     d6,d7     | swap arguments back into place
         rts                 | return to caller
         page
*
* internal convert subroutine
*   convert the argument in d7 to fast floating point format and return
*   ccr set for test against sign and zero
*
* output:
*         if nan - direct return bypassing caller with nan in d7 and "v" set
*           else return with converted value and "c" bit if is an infnity
*
iefprse: move.l    d7,d5     | save original argument
         swap.w    d7        | swap word halves
         ror.l     #7,d7     | exponent to low byte
         eor.b     #$80,d7   | convert from excess 127 to two's-complement
         add.b     d7,d7     | from 8 to 7 bit exponent
         bvs.s     iefovf    | branch will not fit
         add.b     #2<<1+1,d7 | adjust excess 127 to 64 and set mantissa high bit
         bvs.s     iefexh    | branch exponent too large (overflow)
         eor.b     #$80,d7   | back to excess 64
         ror.l     #1,d7     | to fast float representation
         tst.b     d7        | clear carry
         rts                 | return to caller

* overflow detected - caused by one of the following:
*        - false exponent overflow due to difference in excess notations
*        - exponent too high or low to fit in 7 bits (exponent over/underflow)
*        - an exponent of $ff representing an infinity
*        - an exponent of $00 representing a zero, nan, or denormalized value
iefovf:  bcc.s     iefovlw   branch if overflow (exponent $ff or too large)
* check for false overflow
         cmp.b     #$7c,d7   | ? will corrected value be ok
         beq.s     ieffov    | yes, branch if false overflow
         cmp.b     #$7e,d7   | ? will corrected value be in range
         bne.s     iefnotf   | no, branch not false overflow
ieffov:  add.b     #$80+2<<1+1,d7 | back to excess 64 and set mantissa high bit
         ror.l     #1,d7     | to fast floating point representation
         tst.b     d7        | insure not illegal zero sign+exponent byte
         bne.s     iefcrt    | no, is ok so return "c" cleared
* exponent low - is zero, denormalized value, or too small an exponent
iefnotf: move.l    #0,d7     | return zero for all of these cases ("c" cleared)
iefcrt:  rts                 | return to caller

* exponent high - check for exponent too high, infinity, or nan
iefovlw: cmp.b     #$fe,d7   | ? was original exponent $ff
         bne.s     iefexh    | no, branch exponent too large
         lsr.l     #8,d7     | shift out exponent
         lsr.l     #1,d7     | shift sign out into "x" bit
         beq.s     iefexhi   | branch if true infinity

* arg2 is a nan - bypass caller and return as the result with "v" bit set
         move.l    d5,d7     | return original argument
         add.l     #8,sp     | skip internal and caller return addresses
         movem.l   (sp)+,d3-d6 | return registers intact
         add.l     #4,sp     | skip original d7
         ori       #vbit,ccr | set v bit on [vlh]
         rts                 | return to original caller

* arg2 is infinity or exponent too large so force to infinity (sign is bit 8)
iefexh:  lsl.w     #8,d7     | set "x" ccr bit same as sign
iefexhi: move.l    #expmsk<<1,d7 | set exponent all ones for infinity and over 1
         roxr.l    #1,d7     | shift sign and and finished result
         ori       #cbit,ccr | set "c" bit on for infinity notice [vlh]
         rts                 | return to conversion routine

         end
