       ttl     ieee format equivalent compare and test (iefcmp/ieftst)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************************
*                     iefcmp                                *
*  fast floating point ieee format equivalent compare       *
*                                                           *
*          (result is test of destination - source)         *
*                                                           *
*  input:  d6 - ieee format number (source)                 *
*          d7 - ieee format number (destination)            *
*                                                           *
*  output: the condition code register is set to directly   *
*           reflect the following results of the test:      *
*                                                           *
*                  eq         equal                         *
*                  ne         not equal                     *
*                  gt         greater than                  *
*                  ge         greater than or equal         *
*                  lt         less than                     *
*                  le         less than or equal            *
*                  cc         ordered                       *
*                  cs         unordered                     *
*                                                           *
*      condition codes:                                     *
*              n - set for proper arithmetic tests          *
*              z - set if result is zero                    *
*              v - set for proper arithmetic tests          *
*              c - set if result is unordered               *
*                  (not-a-number operand)                   *
*              x - undefined                                *
*                                                           *
*               all registers transparent                   *
*                                                           *
*            maximum stack usage:    32 bytes               *
*                                                           *
*  notes:                                                   *
*    1) the unordered condition results whenever any        *
*       argument is a nan (not-a-number).  the carry bit    *
*       will be returned on if this occurs.  this is        *
*       different from most of the other mc68344 ieee format*
*       equivalent operations in that they return the "v"   *
*       bit set which is handy for use of the "trapv"       *
*       instruction.  however, "v" must be used here for the*
*       signed arithmetic comparisons.                      *
*    2) iefcmp recognizes and properly handles all single-  *
*       precision ieee format values and data types.        *
*    3) infinities are handled in affine mode (plus and     *
*       minus infinities are allowed and operate with non-  *
*       infinities).                                        *
*                                                           *
*************************************************************
         page

*iefcmp .idnt    1,1  ieee format equivalent compare/test

         xref      iefdop  double argument conversion routine
       xref    ffpcpyrt        copyright notice

         text
         xdef      iefcmp    ieee format compare

ccrcbit  equ       $01       condition code register "c" bit mask

***********************
* compare entry point *
***********************
iefcmp:  move.l    d7,-(sp)  | save callers original d7
         jsr       iefcall   | call internal routine (nans will return here)
* if control returns here it will be from iefdopt detecting a nan
         move.l    (sp)+,d7  | restore original d7 in case arg1 nan and not arg2
         ori       #ccrcbit,ccr | set c-bit for unordered nan encntrd [vlh]
         rts                    | return to caller with "c" set

* internal subroutine.  split nan's away via call to iefdop.  iefdop will
* directly return to the code above if it detects either operand to be a nan.
iefcall: jsr       iefdop    | decode both operands
         bra       iefnrm    | +0 normalized return (or zero or denormalized)
         bra       iefinf2   | +2 arg2 infnity return
         bra       iefinf1   | +4 arg1 infnity return
*                                       +6 both infinity return

* both values are infinity.  we can substitute +1 and -1 values for plus and
* minus infinity respectively, and continue with a straight arithmetic compare.
         move.l    #30,d5    | setup shift count for sign propagation
         asr.l     d5,d7     | change to plus or minus one
         asr.l     d5,d6     | change to plus or minus one
         jmp       iefdocmp  | now finish with standard arithmetic compare

* arg2 is infinity and not arg1 - substitute $80000000 (lowest binary value)
* for negative infinity to force proper compare
iefinf2: tst.l     d7        | ? was this minus infinity
         bpl.s     iefinf2p  | branch if ok to compare
         lsl.l     #8,d7     | change to smallest negative number for compare
iefinf2p:jsr       ieffix1   | reload and fix arg1 negative zeroes
         jmp       iefdocmp  | now finish with compare

* arg1 is infinity and not arg2 - substitude $80000000 (lowest binary value)
* for negative infinity to force proper compare
iefinf1: tst.l     d6        | ? was this minus infinity
         bpl.s     iefdarg2  | branch if not, ok to use
         lsl.l     #8,d6     | set to smallest negative value
         jmp       iefdarg2  | reload and fix arg2 negative zeroes

* all normalized, denormalized, or zeroes return here.
* except for minus zeroes, a simple arithmetic compare can be
* done directly on the original ieee arguments.  minus zeroes are
* changed to true arithmetic zeroes.
iefnrm:  jsr       ieffix1   | load and fix negative zeroes for first argument
iefdarg2:move.l    16(sp),d7 | reload arg2
         add.l     d7,d7     | test for plus or minus zero
         beq.s     iefdocmp  | br zero, use true zero for compare
         move.l    16(sp),d7 | reload and use original value
iefdocmp:cmp.l     d6,d7     | perform the compare (destination minus source)
         andi      #$7f-ccrcbit,ccr | force c-bit off for ordered cmpr [vlh]
         movem.l   (sp)+,d3-d7 | restore callers original registers
         add.l     #8,sp     | skip over internal return address and saved d7
         rts                 | return with arithmetic compare condition code

* load and fix argument 1 for negative zeroes
ieffix1: move.l    12+4(sp),d6 | reload it back
         add.l     d6,d6     | check for plus or minus zero
         beq.s     ieffixr   | return if zero to use true zero
         move.l    12+4(sp),d6 | reload and use original value
ieffixr: rts                 | return to caller
         page
*************************************************************
*                     ieftst                                *
*  fast floating point ieee format equivalent test          *
*                                                           *
*          (result is test of destination minus zero)       *
*                                                           *
*  input:  d7 - ieee format number (destination)            *
*                                                           *
*  output: the condition code register is set to directly   *
*           reflect the following results of the test:      *
*                                                           *
*                  eq         equal zero                    *
*                  ne         not equal zero                *
*                  pl         positive value                *
*                  mi         negative value                *
*                  vc         not a nan (not-a-number)      *
*                  vs         nan (not-a-number)            *
*                                                           *
*      condition codes:                                     *
*              n - set if negative                          *
*              z - set if zero                              *
*              v - set if nan (not-a-number)                *
*              c - undefined                                *
*              x - undefined                                *
*                                                           *
*               all registers transparent                   *
*                                                           *
*            total stack usage:    24 bytes                 *
*                                                           *
*  notes:                                                   *
*    1) iefcmp recognizes and properly handles all single-  *
*       precision ieee format values and data types.        *
*                                                           *
*************************************************************
         page

         xref      iefsop  | single argument conversion routine
         xref      iefrtod7 | return original d7 to caller as result

         xdef      ieftst    | ieee format test

********************
* test entry point *
********************
ieftst:  jsr       iefsop    | separate out nans back to caller
         nop                 | +0 normalized zero or denormalized
*                                       +2 argument was infinity

* merely return with d7 as the result. ccr will be set properly
         jmp       iefrtod7   | return old original d7

         end
