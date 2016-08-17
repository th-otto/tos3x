      ttl    fast floating point square root (ffpsqrt)
*******************************************
* (c)  copyright 1981 by motorola inc.    *
*******************************************
 
********************************************
*           ffpsqrt subroutine             *
*                                          *
* input:                                   *
*          d7 - floating point argument    *
*                                          *
* output:                                  *
*          d7 - floating point square root *
*                                          *
* condition codes:                         *
*                                          *
*          n - cleared                     *
*          z - set if result is zero       *
*          v - set if argument was negative*
*          c - cleared                     *
*          x - undefined                   *
*                                          *
*    registers d3 thru d6 are volatile     *
*                                          *
* code: 194 bytes    stack work: 4 bytes   *
*                                          *
* notes:                                   *
*   1) no overflows or underflows can      *
*      occur.                              *
*   2) a negative argument causes the      *
*      absolute value to be used and the   *
*      "v" bit set to indicate that a      *
*      negative square root was attempted. *
*                                          *
* times:                                   *
* argument zero         3.50 microseconds  *
* minimum time > 0    187.50 microseconds  *
* average time > 0    193.75 microseconds  *
* maximum time > 0    200.00 microseconds  *
********************************************
* ffpsqrt  idnt 1,1  ffp square root
 
       text
 
      xdef   ffpsqrt   | entry point
      xref   ffpcpyrt  | copyright notice
 
* negative argument handler
fpsinv:  and.b     #$7f,d7   | take absolute value
         bsr.s     ffpsqrt   | find sqrt(abs(x))
*        or.b      $02,ccr   | set "v" bit
         dc.l      $003c0002 | **assembler error**
         rts                 | return to caller
 
*********************
* square root entry *
*********************
ffpsqrt: move.b    d7,d3     | copy s+exponent over
         beq.s     fpsrtn    | return zero if zero argument
         bmi.s     fpsinv    | negative, reject with special condition codes
         lsr.b     #1,d3     | divide exponent by two
         bcc.s     fpseven   | branch exponent was even
         add.b     #1,d3     | adjust odd values up by one
         lsr.l     #1,d7     | offset odd exponent's mantissa one bit
fpseven: add.b     #$20,d3   | renormalize exponent
         swap.w    d3        | save result s+exp for final move
         move.w    #23,d3    | setup loop for 24 bit generation
         lsr.l     #7,d7     | prepare first test value
         move.l    d7,d4     | d4 - previous value during loop
         move.l    d7,d5     | d5 - new test value during loop
         move.l    a0,d6     | save address register
         lea       fpstbl(pc),a0 | load table address
         move.l    #$00800000,d7 | d7 - initial result (must be a one)
         sub.l     d7,d4     | preset old value in case zero bit next
         sub.l     #$01200000,d5 | combine first loop calculations
         bra.s     fpsent    | go enter loop calculations
 
*                   square root calculation
* this is an optimized scheme for the recursive square root algorithm:
*
*  step n+1:
*     test value <= .0  0  0  r  r  r  0 1  then generate a one in result r
*                     n  2  1  n  2  1        else a zero in result r      n+1
*                                                                    n+1
* precalculations are done such that the entry is midway into step 2
 
fpsone:  bset      d3,d7     | insert a one into this position
         move.l    d5,d4     | update new test value
fpszero: add.l     d4,d4     | multiply test result by two
         move.l    d4,d5     | copy in case next bit zero
         sub.l     (a0)+,d5  | subtract the '01' ending pattern
         sub.l     d7,d5     | subtract result bits collected so far
fpsent:  dbmi      d3,fpsone | branch if a one generated in the result
         dbpl      d3,fpszero | branch if a zero generated
 
* all 24 bits calculated. now test result of 25th bit
         bls.s     fpsfin    | branch next bit zero, no rounding
         add.l     #1,d7     | round up (cannot overflow)
fpsfin:  lsl.l     #8,d7     | normalize result
         move.l    d6,a0     | restore address register
         swap.w    d3        | restore s+exp save
         move.b    d3,d7     | move in final sign+exponent
fpsrtn:  rts                 | return to caller
 
* table to furnish '01' shifts during the algorithm loop
fpstbl:  dc.l      1<<20,1<<19,1<<18,1<<17,1<<16,1<<15
         dc.l      1<<14,1<<13,1<<12,1<<11,1<<10,1<<9,1<<8
         dc.l      1<<7,1<<6,1<<5,1<<4,1<<3,1<<2,1<<1,1<<0
         dc.l      0,0
