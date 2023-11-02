         ttl       ieee format equivalent power function(iefpwr)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  iefpwr                       *
*   ieee format equivalent  power function      *
*                                               *
*  input:   d6 - ieee format exponent value     *
*           d7 - ieee format argument value     *
*                                               *
*  output:  d7 - result of the value taken to   *
*                the power specified            *
*                                               *
*     all registers are transparent             *
*                                               *
*    maximum stack used:    52 bytes            *
*                                               *
*  condition codes:                             *
*        z - set if result is zero              *
*        n - cleared                            *
*        v - set if result is nan (not-a-number)*
*            (input argument negative or nan)   *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*  notes:                                       *
*    1) see the mc68344 user's guide for details*
*       concerning limitations on normalized    *
*       argument ranges.                        *
*    2) a negative base value, or zero to a     *
*       negative power is invalid and returns   *
*       a nan with the "v" bit set.             *
*    3) spot checks show at least six digit     *
*       precision for 80 percent of the cases.  *
*                                               *
*************************************************
         page
*iefpwr   .idnt  1,1 ieee format equivalent power function

         text

         xdef      iefpwr            | entry point

         xref      ieflog            | ieee log function
         xref      iefmul            | ieee multiply routine
         xref      iefexp            | ieee exponent function
         xref      ffpcpyrt          | copyright stub

*****************
* power  entry  *
*****************
iefpwr:  jsr       ieflog              | find low of base value
         jsr       iefmul              | multiply by the power
         jmp       iefexp              | finish with exponent result

         end
