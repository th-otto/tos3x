         ttl       ieee format equivalent log (ieflog)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  ieflog                       *
*       fast floating point logorithm           *
*                                               *
*  input:   d7 - ieee format input argument     *
*                                               *
*  output:  d7 - ieee format logorithmic result *
*                                               *
*     all other registers totally transparent   *
*                                               *
*  condition codes:                             *
*        z - set if result is zero              *
*        n - set if result is negative          *
*        v - set if result is nan (not-a-number)*
*            (negative or nan argument)         *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*         all other registers transparent       *
*                                               *
*         maximum stack used:   54 bytes        *
*                                               *
*  notes:                                       *
*    1) see the mc68344 user's guide for details*
*       concerning ieee format normalized range *
*       support limitations.                    *
*    2) spot checks show relative errors bounded*
*       by 5 x 10**-8.                          *
*    2) negative arguments are illegal and cause*
*       a nan (not-a-number) to be returned.    *
*    3) a zero argument returns minus infinity. *
*                                               *
*************************************************
         page
*ieflog   .idnt  1,1 ieee format equivalent logorithm

         text

         xdef      ieflog            | entry point

         xref      ffplog            | ffp logorithm routine
         xref      iefsop            | front-end operand conversion routine
         xref      ieftieee          | back-end convert to ieee and return
         xref      iefrtnan          | back-end return nan routine
         xref      iefrtod7          | return original d7 from the caller
         xref      ffpcpyrt          | copyright stub

**************
* log entry  *
**************
ieflog:  jsr       iefsop    | convert the operand
         bra.s     iefnrm    | +0  branch normalized value
* input argument is infinity               +8
         bmi       iefrtnan  | return a nan for a negative argument
         jmp       iefrtod7  | return plus infinity as the result

* argument is normalized
iefnrm:  bmi       iefrtnan  | return a nan if argument is negative
         jsr       ffplog    | call fast floating point log routine
         jmp       ieftieee  | and return result in ieee format

         end
