       ttl     fast floating point cmp/tst (ffpcmp/ffptst)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************
 
*************************************************************
*                      ffpcmp                               *
*              fast floating point compare                  *
*                                                           *
*  input:  d6 - fast floating point argument (source)       *
*          d7 - fast floating point argument (destination)  *
*                                                           *
*  output: condition code reflecting the following branches *
*          for the result of comparing the destination      *
*          minus the source:                                *
*                                                           *
*                  gt - destination greater                 *
*                  ge - destination greater or equal to     *
*                  eq - destination equal                   *
*                  ne - destination not equal               *
*                  lt - destination less than               *
*                  le - destination less than or equal to   *
*                                                           *
*      condition codes:                                     *
*              n - cleared                                  *
*              z - set if result is zero                    *
*              v - cleared                                  *
*              c - undefined                                *
*              x - undefined                                *
*                                                           *
*               all registers transparent                   *
*                                                           *
*************************************************************
* ffpcmp idnt    1,1  ffp cmp/tst
 
         xdef      ffpcmp    | fast floating point compare
 
         xref    ffpcpyrt        | copyright notice
 
         text
 
***********************
* compare entry point *
***********************
ffpcmp:
         cmp.b     d6,d7     | compare sign and exponent only first
         bne.s     ffpcrtn   | return if that is sufficient
         cmp.l     d6,d7     | no, compare full longwords then
ffpcrtn: rts                 | and return to the caller


         xdef     ffptst     | fast floating point test
 
********************
* test entry point *
********************
ffptst:  tst.b     d7        | return tested condition code
         rts                 | to caller
