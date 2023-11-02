        ttl       ieee format equivalent float to integer (ieffpi)
**************************************
* (c) copyright 1981 by motorla inc. *
**************************************

***********************************************************
*    ieffpi - ieee format equivalent float to integer     *
*                                                         *
*      input:  d7 = ieee format number                    *
*      output: d7 = fixed point longword integer          *
*                            (2's complement)             *
*                                                         *
*  condition codes:                                       *
*             n - set if result is negative               *
*             z - set if result is zero                   *
*             v - set if result overflowed integer        *
*                 representation or input was a nan       *
*                 (not-a-number)                          *
*             c - undefined                               *
*             x - undefined                               *
*                                                         *
*     registers are transparent over this routine         *
*                                                         *
*              stack used:    54 bytes                    *
*                                                         *
*  the range provided with 2's complement longword binary *
*  is:   -2,147,483,649 < value < +2,147,483,648          *
*                                                         *
*  notes:                                                 *
*   1) since ieee single precision format holds 24 bits   *
*      of precision which is less than the 31 bits of     *
*      longword binary integer arithmetic, results of     *
*      over 24 bit integer magnitude will be imprecise    *
*      and have low-end zeroes.                           *
*   2) if the input argument is too large to be contained *
*      in the longword fixed point binary format, the     *
*      largest possible magnitude value is returned with  *
*      the "v" bit set in the condition code register.    *
*   3) if a nan (not-a-number) is the input argument,     *
*      it will be returned as is with the "v" bit set.    *
*   4) since the "v" bit is set for two possible          *
*      conditions (overflow or nan) they can be separated *
*      by testing the significand (bits 0 thru 22).  if   *
*      these bits are all ones, then the value probably   *
*      was an overflow.  any other pattern indicates the  *
*      input argument was nan.                            *
*                                                         *
***********************************************************
         page
         xdef      ieffpi   | entry point

        xref      ffpfpi    | fast floating point float to integer
        xref      ffpfieee  | ffp conversion of ieee to ffp format
        xref      iefsop    | ieee single operand convert internal routine
        xref      ffpcpyrt  | copyright notice

*ieffpi  .idnt      1,1  ieee format equivalent float to integer

        text

*************************
* ieee float to integer *
*************************
ieffpi:  bsr       iefsop    | convert to fast floating point or reject nan
         bra.s     iefnrm    | +0 normalized value, denormalized value or zero
*                                       +2 argument infinity

* infinity - convert to ffp format's largest magnitude
         bsr       ffpfieee  | ffp's conversion will force to highest possible

* normalized value, denormalized or zero
iefnrm:  bsr       ffpfpi    | convert from fast floating point to integer
         movem.l   (sp)+,d3-d6 | reload callers registers
         add.l     #4,sp     | skip original argument
         rts                 | return to caller with integer in d7 and ccr set

         end
