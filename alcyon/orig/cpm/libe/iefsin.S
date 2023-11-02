         ttl       ieee format equivalent (iefsin/iefcos/ieftan)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*            iefsin iefcos ieftan               *
* ieee format equivalent  sine/cosine/tangent   *
*                                               *
*  input:   d7 - ieee format argument (radians) *
*                                               *
*  output:  d7 - ieee format function result    *
*                                               *
*     all other registers totally transparent   *
*                                               *
*        maximum stack used:   54 bytes         *
*                                               *
*  condition codes:                             *
*        z - set if result in d7 is zero        *
*        n - set if result in d7 is negative    *
*        c - undefined                          *
*        v - set if result is nan (not-a-number)*
*            (input magnitude too large or nan) *
*        x - undefined                          *
*                                               *
*  functions:                                   *
*             iefsin   -  sine result           *
*             iefcos   -  cosine result         *
*             ieftan   -  tangent result        *
*                                               *
*  notes:                                       *
*    1) input values are in radians.            *
*    2) input arguments larger than two pi      *
*       suffer reduced precision.  the larger   *
*       the argument, the smaller the precision.*
*       excessively large arguments which have  *
*       less than 5 bits of precision are       *
*       returned as a nan with the "v" bit set. *
*    3) the sign of tangents with infinite      *
*       value is undefined, however we return   *
*       a positive infinity.                    *
*    4) spot checks show relative errors bounded*
*       by 4 x 10**-7 but for arguments close to*
*       pi/2 intervals where 10**-5 is seen.    *
*                                               *
*************************************************
         page
*iefsin   .idnt  1,1 ieee format equivalent sine/cosine/tangent

         text

         xdef      iefsin,iefcos,ieftan     | entry points

         xref      ffpsin,ffpcos,ffptan  | ffp transcendentals
         xref      iefsop          | single operand front-ender
         xref      ieftieee        | back-end convert back to ieee format
         xref      iefrtnan        | back-end return ieee nan routine
         xref      ffpcpyrt        | copyright stub

vbit     equ       $0002     | condition code register "v" bit mask
ffpsign  equ.b     $80       | sign in fast floating point value

***********************
* tangent entry point *
***********************
ieftan:  jsr       iefsop    | parse the operand
         bra.s     ieftnrm   | +0 branch not infinity or nan
         jmp       iefrtnan  | +2 return nan for infinity

* perform tangent function with normalized number
ieftnrm: jsr       ffptan    | find tangent
         bra.s     iefcmn    | enter common exit code

**********************
* cosine entry point *
**********************
iefcos:  jsr       iefsop    | parse the operand
         bra.s     iefcnrm   | +0 branch not infinity or nan
         jmp       iefrtnan  | +2 return nan for infinity

* perform cosine function with normalized number
iefcnrm: jsr       ffpcos    | find cosine
         bra.s     iefcmn    | enter common exit code

********************
* sine entry point *
********************
iefsin:  jsr       iefsop    | parse the operand
         bra.s     iefsnrm   | +0 branch not infinity or nan
         jmp       iefrtnan  | +2 return nan for infinity

* perform sine function with normalized number
iefsnrm: jsr       ffpsin    | find sine
iefcmn:  bvc       ieftieee  | return if had enough precision
* overflow can mean true infinity result or not enough precision
* we can test for not enough precision by checking for largest possible value
         move.l    d7,d5     | copy over ffp format result
         or.b      #ffpsign,d5 | set sign bit to a one
         sub.l     #1,d5     | test for all one bits
*        bne       iefrtnan  | no, not enough precision, return a nan
         bne       dobranch  | for 16-bit non-MMU problem
         tst.b     d7        | reset ccr as it was
         ori       #vbit,ccr | and show overflow occured
         jmp       ieftieee  | restore with infinity of proper sign
dobranch:
         jmp       iefrtnan

         end
