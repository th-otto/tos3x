         ttl       ieee format equivalent back-end routines (iefback)
*iefback  .idnt   1,1  ieee format equivalent back-end routines
******************************************
*  (c)  copyright 1981 by motorola inc.  *
******************************************

         text

****************************************************************
*              iefrtnan (internal routine)                     *
*  ieee format equivalent fast floating point return nan       *
*                                                              *
*  input: sp -> +0  original callers d3-d7 registers           *
*              +20  original callers return address            *
*                                                              *
*  output: d7 - a newly created nan (not-a-number)             *
*          ccr - the "v" bit is forced on                      *
*                                                              *
*          and direct return to the original caller            *
*                                                              *
*  purpose:  called whenever the result of an operation        *
*          is illegal or undefined and a nan result must       *
*          be generated as the final result.                   *
*                                                              *
*   the ieee format defined nan is determined by an exponent   *
*   of all one's and a non-zero significand.  the sign bit     *
*   is a don't care.  the ieee standard leaves up to each      *
*   implementation what is placed in the significand.  here    *
*   we will generate the low order 23 bits of the original     *
*   caller's return address.  however, this may not be         *
*   sufficient - if all 23 bits happen to be zero or the       *
*   address is larger than 23 bits this would lead to an       *
*   incorrect result.  therfore, if this happens only the low  *
*   order significand bit is set on with the rest zeroes.      *
*   this represents an odd address (illegal with current m68000*
*   instruction alignment restrictions) and any interested     *
*   party can tell if such a substitution has taken place.     *
*   also, if this was illegally assumed to be an address and   *
*   used, an address exception trap would ensue thus not       *
*   allowing its use as a valid address.                       *
*                                                              *
****************************************************************

signmsk  equ       $80000000 | ieee format sign isolation mask
expmsk   equ       $7f800000 | ieee format exponent mask
vbit     equ       $0002     | condition code "v" bit mask
zbit     equ       $0004     | condition code "z" bit mask

         xdef      iefrtnan  | return nan result routine

iefrtnan:movem.l   (sp)+,d3-d7         | restore callers registers
         move.l    (sp),d7             | load up return address
         and.l     #signmsk+expmsk,d7  | verify not larger than 23 bits
         bne.s     iefnone             | it is, cannot use it - return a one
         move.l    (sp),d7             | load up return address
         and.l     #$007fffff,d7       | isolate address bits required
         bne.s     iefnzro             | branch if not zero
iefnone: move.l    #1,d7               | set only low bit on
iefnzro: or.l      #expmsk,d7          | force exponent all ones
         or.b      #vbit,ccr           | return with "v" bit set [vlh]
         rts                           | return to original caller
         page
**********************************************************
*           ieftieee (internal subroutine)               *
*  ieee format compatible convert ffp to ieee format     *
*                                                        *
*  input: d7 - result of fast floating point operation   *
*         ccr - set for above result                     *
*         sp -> +0  original callers saved d3-d7         *
*              +20  original callers return address      *
*                                                        *
*  output: d7 - ieee format equivalent of the result     *
*                                                        *
*  condition code:                                       *
*                                                        *
*              n - set if the result is negative         *
*              z - set if the result is zero             *
*              v - cleared (not nan)                     *
*              c - cleared                               *
*              x - undefined                             *
*                                                        *
*    all fast floating point numbers have an exact       *
*    ieee format representation.  since the fast         *
*    floating point routines always set the "v" bit      *
*    for overflows and returns the proper sign, we       *
*    can easily change the result to an ieee infinity    *
*    and unflag the "v" bit.                             *
*                                                        *
**********************************************************

         xdef      ieftieee  | return ieee result to original caller

ieftieee:bvs.s     iefvset   | branch if overflow ffp result
         add.l     d7,d7     | delete mantissa high bit
         beq.s     ieftrtn   | branch zero as finished
         eor.b     #$80,d7   | to twos complement exponent
         asr.b     #1,d7     | form 8-bit exponent
         sub.b     #$82,d7   | adjust 64 to 127 and excessize
         swap.w    d7        | swap for high byte placement
         rol.l     #7,d7     | set sign+exp in high byte
ieftrtn: tst.l     d7        | test "z" and "n", clear "v" and "c" in ccr
         movem.l   (sp)+,d3-d6 | restore d3 thru d6 callers registers
         add.l     #4,sp     | skip original d7
         rts                 | return to original caller with result

* overflow - set to proper ieee format infinity
iefvset: add.b     d7,d7     | save sign bit in "x"
         move.l    #expmsk<<1,d7 | set exponent of ones shifted left
         roxr.l    #1,d7     | insert proper sign
         jmp       ieftrtn   | and return to original caller
         page
*******************************************************************
*  general purpose return routines                                *
*                                                                 *
*  the following routines return a specific final result          *
*  to the original caller with the proper condition codes         *
*  set as follows:                                                *
*                                                                 *
*              n - the result is negative                         *
*              z - the result is a zero                           *
*              v - cleared (not a nan)                            *
*              c - undefined                                      *
*              x - undefined                                      *
*                                                                 *
*  the routines are as follows:                                   *
*                                                                 *
*   iefrtd7  - return the current contents of d7                  *
*   iefrtod7 - return the original contents of d7                 *
*   iefrtsz  - return a signed zero (sign is bit 31 of d7)        *
*   iefrtie  - return infinity with sign exclusive or of          *
*              original argument signs                            *
*   iefrtsze - return signed zero with sign exclusiv or           *
*              of original argument signs                         *
*                                                                 *
*******************************************************************

         xdef      iefrtd7,iefrtsz,iefrtod7,iefrtie,iefrtsze

**********************
* return original d7 *
* (cant be neg zero) *
**********************
iefrtod7:move.l    16(sp),d7  load original d7 into d7

*********************
* return current d7 *
* (cant be neg zero)*
*********************
iefrtd7: movem.l   (sp)+,d3-d6         | load all but d7 registers back up
         add.l     #4,sp     | skip original d7 on stack
         add.l     d7,d7     | check for signed zero
         beq.s     iefwasz   | branch if was a zero
         roxr.l    #1,d7     | value back into position set ccr ("v" clear)
         rts                 | return to caller with ccr and result

**************************************
* return signed zero with sign being *
* eor of the original operands       *
**************************************
iefrtsze:movem.l   12(sp),d6-d7 | load original arguments
         eor.l     d6,d7     | produce proper sign

**********************
* return signed zero *
* d7 bit31 has sign  *
**********************
iefrtsz: movem.l   (sp)+,d3-d6 | load all but d7 back up
         add.l     #4,sp     | skip original d7 on stack
         add.l     d7,d7     | set sign bit into carry
         move.l    #0,d7     | zero d7
iefwasz: roxr.l    #1,d7     | set sign bit back in ("v" cleared)
         ori       #zbit,ccr | force zero bit on in ccr [vlh]
         rts

*********************************
* return infinity with eor sign *
* of original arguments         *
*********************************
iefrtie: movem.l   (sp)+,d3-d7 | restore original arguments
         eor.l     d6,d7     | produce proper sign
         add.l     d7,d7     | shift sign out
         move.l    #expmsk<<1,d7 | setup infinity (exponent all ones)
         roxr.l    #1,d7     | set sign back in ("v" cleared)
         rts                 | return with result and ccr set

         end
