         ttl       ieee format equivalent exponent (iefexp)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  iefexp                       *
* ieee format equivalent exponent function      *
*                                               *
*  input:   d7 - ieee floating point argument   *
*                                               *
*  output:  d7 - ieee floating point exponential*
*                result                         *
*                                               *
*     all other registers are transparent       *
*                                               *
*       maximum stack used:   54 bytes          *
*                                               *
*  condition codes:                             *
*        z - set if result in d7 is zero        *
*        n - cleared                            *
*        v - set if result is nan (not-a-number)*
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*                                               *
*  notes:                                       *
*    1) if the input argument is a nan (not-a-  *
*       number) then the "v" bit will be set    *
*       and the argument unchanged upon return. *
*    2) see the mc68344 user's guide for details*
*       on the range of ieee normalized values  *
*       supported.                              *
*    2) spot checks show at least 6.8 digit     *
*       accuracy for all abs(arg) < 30.         *
*                                               *
*************************************************
         page
*iefexp   .idnt  1,1 ieee format equivalent exp

         text

         xdef      iefexp            | entry point

         xref      ffpexp            | fast floating point exponent
         xref      iefsop            | front-end single argument routine
         xref      ieftieee          | back-end return to ieee format
         xref      iefrtd7           | return to caller argument in d7
         xref      ffpcpyrt          | copyright stub

************************
* exponent entry point *
************************
iefexp:  jsr       iefsop    | convert argument to ffp format
         bra.s     iefnrm    | branch normalized
* argument is an infinity
         bpl.s     iefdrtn   | branch if plus - return itself
         move.l    #0,d7     | return plus zero if was negative infinity
iefdrtn: jmp       iefrtd7   | return the result that is in d7

* argument is normalized
iefnrm:  jsr       ffpexp    | call fast floating point exponent function
         jmp       ieftieee  | and return in ieee format

         end
