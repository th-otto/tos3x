      ttl    ieee format equivalent square root (iefsqrt)
*******************************************
* (c)  copyright 1981 by motorola inc.    *
*******************************************

*************************************************
*                iefsqrt                        *
*      ieee format equivalent square root       *
*                                               *
* input:                                        *
*          d7 - ieee format argument            *
*                                               *
* output:                                       *
*          d7 - ieee format square root         *
*                                               *
* condition codes:                              *
*                                               *
*   if the result is valid -                    *
*          n - cleared                          *
*          z - set if result is zero            *
*          v - cleared                          *
*          c - undefined                        *
*          x - undefined                        *
*                                               *
*   if the result is invalid due to a           *
*   negative non-zero or nan input argument-    *
*          n - undefined                        *
*          z - cleared                          *
*          v - set                              *
*          c - undefined                        *
*          x - undefined                        *
*                                               *
*        all registers are transparent          *
*                                               *
*        maximum stack used:   24 bytes         *
*                                               *
* notes:                                        *
*   1) valid results are obtained unless        *
*      the input argument was a negative        *
*      non-zero number or nan (not-a-           *
*      number) which can be determined by       *
*      the "v" bit setting in the ccr.          *
*   2) see the mc68344 user's guide for         *
*      details on the ranges handled by         *
*      the fast floating point equivalent       *
*      routines.                                *
*                                               *
*************************************************
         page
iefsqrt  idnt 1,1  ieee format equivalent square root

       text

      xdef   iefsqrt   | entry point

         xref      ffpsqrt | fast floating point square root routine
         xref      iefsop  | single operand front-end handler
         xref      ieftieee | back-end handler to return ieee format
         xref      iefrtnan | error handler to return a nan (not-a-number)
         xref      iefrtod7 | return original caller's d7
      xref   ffpcpyrt  | copyright notice

*********************
* square root entry *
*********************
iefsqrt: jsr       iefsop    | convert the operand
         bra.s     iefnrm    | branch both normalized
* argument was infinity - return a nan if minus
*        bmi       iefrtnan  | return nan if it is minus infinity
         bmi       dobranch
         jmp       iefrtod7  | just return input argument if plus infinity

* argument was normalized
* iefnrm:  bmi       iefrtnan  | return nan for invalid negative argument
iefnrm:  bmi       dobranch
         move.l    16(sp),d5 | insure was not a negative very small number
         bpl.s     iefsdoit  | branch was positive
         add.l     d5,d5     | rid sign byte to check if was negative zero
*        bne       iefrtnan  | return nan for very small negative numbers also
         bne       dobranch
iefsdoit:jsr       ffpsqrt   | perform square root
         jmp       ieftieee  | and return ieee format back to caller
dobranch:
         jmp       iefrtnan  | 16-bit no MMU stuff

         end
