         ttl       ffp transcendental normalize internal routine (ffptnorm)
* ffptnorm idnt      1,2 ffp transcendental internal normalize
 
         xdef      ffptnorm
         text
 
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************
 
******************************
*        ffptnorm            *
* normalize bin(29,31) value *
*   and convert to float     *
*                            *
* input: d6 - internal fixed *
* output: d6 - ffp float     *
*         cc - reflect value *
* notes:                     *
*  1) d4 is destroyed.       *
*                            *
* time: (8mhz no wait state) *
*       zero  4.0 microsec.  *
*   avg else 17.0 microsec.  *
*                            *
******************************
 
 
ffptnorm: move.l    #$42,d4            | setup initial exponent
         tst.l     d6                  | test for non-negative
         beq.s     fsfrtn              | return if zero
         bpl.s     fsfpls              | branch is >= 0
         neg.l     d6                  | absolutize input
         move.b    #$c2,d4             | setup initial negative exponent
fsfpls:  cmp.l     #$00007fff,d6       | test for a small number
         bhi.s     fsfcont             | branch if not small
         swap.w    d6                  | swap halves
         sub.b     #16,d4              | offset by 16 shifts
fsfcont: add.l     d6,d6               | shift another bit
         dbmi      d4,fsfcont          | shift left until normalized
         tst.b     d6                  | ? should we round up
         bpl.s     fsfnrm              | no, branch rounded
         add.l     #$100,d6            | round up
         bcc.s     fsfnrm              | branch no overflow
         roxr.l    #1,d6               | adjust back for bit in 31
         add.b     #1,d4               | make up for last shift right
fsfnrm:  move.b    d4,d6               | insert sign+exponent
fsfrtn:  rts                           | return to caller
