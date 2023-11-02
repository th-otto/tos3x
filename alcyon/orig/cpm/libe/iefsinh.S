         ttl       ieee format equivalent hyperbolics (iefsinh)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*            iefsinh/iefcosh/ieftanh            *
* ieee equivalent floating point hyperbolics    *
*                                               *
*  input:   d7 - ieee format argument           *
*                                               *
*  output:  d7 - ieee format hyperbolic result  *
*                                               *
*     all other registers are transparent       *
*                                               *
*      maximum stack useage:    54 bytes        *
*                                               *
*  calls: iefexp, iefdiv, iefadd and iefsub     *
*                                               *
*  condition codes:                             *
*        z - set if result is zero              *
*        n - set if result is negative          *
*        v - set if result is nan (not-a-number)*
*            (returned if input was nan)        *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*  notes:                                       *
*    1) see the mc68344 user's guide for the    *
*       limitations on ieee format normalized   *
*       values.                                 *
*    2) if the input argument is a nan, it will *
*       be returned with the "v" bit set.       *
*    3) spot checks show at least seven digit   *
*       precision.                              *
*                                               *
*************************************************
         page
*iefsinh  idnt  1,1 ieee format equivalent hyperbolics

         text

         xdef      iefsinh,iefcosh,ieftanh       | entry points

         xref      iefexp,iefmul,iefdiv,iefadd,iefsub | called
         xref      ffpcpyrt            | copyright stub

fpone    equ       $3f800000           | floating one
fptwo    equ       $40000000           | floating two

**********************************
*            iefcosh             *
*  this function is defined as   *
*            x    -x             *
*           e  + e               *
*           --------             *
*              2                 *
* we evaluate exactly as defined *
**********************************

iefcosh: move.l    d6,-(sp)  | save our work register
         jsr       iefexp    | evaluate e to the x
         move.l    d7,-(sp)  | save result
         move.l    d7,d6     | setup for divide into one
         move.l    #fpone,d7 | load floating point one
         jsr       iefdiv    | compute e to -x as the inverse
         move.l    (sp)+,d6  | prepare to add together
         jsr       iefadd    | create the numerator
         move.l    #fptwo,d6 | prepare to divide by two
         jsr       iefdiv    | divide by two
         movem.l   (sp)+,d6  | restore work register
         rts                 | return to caller
         page
**********************************
*            iefsinh             *
*  this function is defined as   *
*            x    -x             *
*           e  - e               *
*           --------             *
*              2                 *
* we evaluate exactly as defined *
**********************************

iefsinh: move.l    d6,-(sp)  | save our work register
         jsr       iefexp    | evaluate e to the x
         move.l    d7,-(sp)  | save result
         move.l    d7,d6     | setup for divide into one
         move.l    #fpone,d7 | load floating point one
         jsr       iefdiv    | compute e to -x as the inverse
         move.l    d7,d6     | prepare to subtract
         move.l    (sp)+,d7  | from first calculation
         jsr       iefsub    | create the numerator
         move.l    #fptwo,d6 | prepare to divide by two
         jsr       iefdiv    | divide by two
         movem.l   (sp)+,d6  | restore work register
         rts                 | return to caller
         page
**********************************
*            ieftanh             *
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

ieftanh: move.l    d6,-(sp)  | save our one work register
         move.l    #fptwo,d6 | prepare to multiply times two
         jsr       iefmul    | get argument times two
         jsr       iefexp    | evaluate e to the 2x
         move.l    d7,-(sp)  | save result
         move.l    #fpone,d6 | load floating point one
         jsr       iefadd    | add 1 to e**2x
         move.l    d7,-(sp)  | save denominator
         move.l    4(sp),d7  | now prepare to subtract
         jsr       iefsub    | create numerator
         movem.l    (sp)+,d6 |  restore denominator (keep ccr same)
         bvs.s     ieftrtn   | branch if nan here
         cmp.l     d6,d7     | ***allow infinity by infinity to be one***
         bne.s     ieftdod   | branch to do divide if not same value
         move.l    #fpone,d7 | result is a one
         bra.s     ieftrtn   | and return
ieftdod: jsr       iefdiv    | create result
ieftrtn: add.l     #4,sp     | free e**2x off of stack
         movem.l   (sp)+,d6  | restore our work register
         rts                 | return to caller with answer and ccr set

         end
