         ttl       ieee format equivalent arctangent (iefatan)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  iefatan                      *
*     ieee format equivalent arctangent         *
*                                               *
*  input:   d7 - ieee format argument           *
*                                               *
*  output:  d7 - ieee format arctangent radian  *
*                result                         *
*                                               *
*     all other registers totally transparent   *
*                                               *
*  condition codes:                             *
*        z - set if result is zero              *
*        n - cleared                            *
*        v - set if result is nan (not-a-number)*
*            (set if the input argument was nan)
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*          all other registers transparent      *
*                                               *
*          maximum stack used:   54 bytes       *
*                                               *
*                                               *
*  notes:                                       *
*    1) see the mc68344 user's guide for details*
*       on ieee format range limitations.       *
*    2) spot checks show at least six digit     *
*       precision on all sampled cases.         *
*                                               *
*************************************************
         page
*iefatan  idnt  1,1 ieee format equivalent arctangent

         text

         xdef      iefatan           | entry point

         xref      ffpatan           | fast floating point arctangent
         xref      iefsop            | front-end single operand convert
         xref      ieftieee          | back-end return ieee format
         xref      ffpfieee          | ffp to ieee convert routine
         xref      ffpcpyrt          | copyright stub

********************
* arctangent entry *
********************

* save registers and convert argument
iefatan_ jsr       iefsop    | convert the single argument
         bra.s     iefnrm    | +0 branch normalized
* argument was infinite      | +2 infinity
         jsr       ffpfieee  | convert to maximum values allowed in ffp format

* argument was normalized
iefnrm:  jsr       ffpatan   | call ffp arctangent routine
         jmp       ieftieee  | return in ieee format

         end
