         ttl       fast floating point exponent (ffpexp)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  ffpexp                       *
*       fast floating point exponent            *
*                                               *
*  input:   d7 - input argument                 *
*                                               *
*  output:  d7 - exponential result             *
*                                               *
*     all other registers are transparent       *
*                                               *
*  code size: 256 bytes   stack work: 34 bytes  *
*                                               *
*  condition codes:                             *
*        z - set if result in d7 is zero        *
*        n - cleared                            *
*        v - set if overlow occurred            *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*                                               *
*  notes:                                       *
*    1) an overflow returns the largest         *
*       magnitude number.                       *
*    2) spot checks show at least 6.8 digit     *
*       accuracy for all abs(arg) < 30.         *
*                                               *
*  time: (8mhz no wait states assumed)          *
*                                               *
*              488 microseconds                 *
*                                               *
*  logic:   1) find n = int(arg/ln 2).  this is *
*              added to the mantissa at the end.*
*           3) reduce argument to range by      *
*              finding arg = mod(arg, ln 2).    *
*           4) derive exp(arg) with cordic loop.*
*           5) add n to exponent giving result. *
*                                               *
*************************************************
*   	.idnt  1,2 ffp exp

         text

         xdef      ffpexp              | entry point

         xref      ffphthet            | hypertangent table

         xref      ffpmul2,ffpsub      | arithmetic primitives
         xref      ffptnorm            | transcendental normalize routine
         xref      ffpcpyrt            | copyright stub

ln2      equ       $b1721840           | ln 2 (base e)             .693147180
ln2inv    equ      $b8aa3b41           | inverse of ln 2 (base e) 1.44269504
cnjkhinv equ       $9a8f4441           | floating conjugate of k inverse
*                                      | corrected for the extra convergence
*                                      | during shifts for 4 and 13
kfctseed equ       $26a3d100           | k cordic seed


* overflow - return zero or highest value and "v" bit
fpeovflw: move.w    (sp)+,d6           | load sign word and work off stack
         tst.b     d6                  | ? was argument negative
         bpl.s     fpovnzro            | no, continue
         move.l    #0,d7               | return a zero
         bra.s     fpovrtn             | as result is too small
fpovnzro: move.l    #-1,d7             |  set all zeroes
         lsr.b     #1,d7               | zero sign bit
         ori       #$02,ccr            | set overflow bit
fpovrtn: movem.l   (sp)+,d1-d6/a0      | restore registers
         rts                           | return to caller

* return one for zero argument
ffpe1:   move.l    #$80000041,d7       | return a true one
         lea       7*4+2(sp),sp        | ignore stack saves
         tst.b     d7                  | set condition code properly
         rts                           | return to caller

**************
* exp entry  *
**************

* save work registers and insure positive argument
ffpexp:  movem.l   d1-d6/a0,-(sp)      | save all work registers
         move.w    d7,-(sp)            | save sign in low order byte for later
         beq.s     ffpe1               | return a true one for zero exponent
         and.b     #$7f,d7             | take absolute value

* divide by log 2 base e for partial result
fpepos:  move.l    d7,d2               | save original argument
         move.l    #ln2inv,d6          | load inverse to multiply (faster)
         jsr       ffpmul2             | obtain division thru multiply
         bvs       fpeovflw            | branch if too large
* convert quotient to both fixed and float integer
         move.b    d7,d5               | copy exponent over
         move.b    d7,d6               | copy exponent over
         sub.b     #64+32,d5           | find non-fractional precision
         neg.b     d5                  | make positive
         cmp.b     #24,d5              | ? insure not too large
         ble.s     fpeovflw            | branch too large
         cmp.b     #32,d5              | ? test upper range
         bge.s     fpesml              | branch less than one
         lsr.l     d5,d7               | shift to integer
         move.b    d7,(sp)             | place adjusted exponent with sign byte
         lsl.l     d5,d7               | back to normal without fraction
         move.b    d6,d7               | re-insert sign+exponent
         move.l    #ln2,d6             | multiply by ln2 to find residue
         jsr       ffpmul2             | multiply back out
         move.l    d7,d6               | setup to subtract multiple of ln 2
         move.l    d2,d7               | move argument in
         jsr       ffpsub              | find remainder of ln 2 divide
         move.l    d7,d2               | copy float argument
         bra.s     fpeadj              | adjust to fixed

* multiple less than one
fpesml:  clr.b     (sp)                | default initial multiply to zero
         move.l    d2,d7               | back to original argument

* convert argument to binary(31,29) precision
fpeadj:  clr.b     d7                  | clear sign and exponent
         sub.b     #64+3,d2            | obtain shift value
         neg.b     d2                  | for 2 non-fraction bits
         cmp.b     #31,d2              | insure not too small
         bls.s     fpeshf              | branch to shift if ok
         move.l    #0,d7               | force to zero
fpeshf:  lsr.l     d2,d7               | convert to fixed point

*****************************************
* cordic calculation registers:         *
* d1 - loop count   a0 - table pointer  *
* d2 - shift count                      *
* d3 - y'   d5 - y                      *
* d4 - x'   d6 - x                      *
* d7 - test argument                    *
*****************************************

* input within range, now start cordic setup
fpecom:  move.l    #0,d5               | y=0
         move.l    #kfctseed,d6        | x=1 with jkhinverse factored out
         lea       ffphthet,a0         | point to hperbolic tangent table
         move.l    #0,d2               | prime shift counter

* perform cordic loop repeating shifts 4 and 13 to guarantee convergence
* (ref. "a unified algorithm for elementary functions" j.s.walther
*        pg. 380 spring joint computer conference 1971)
         move.l    #3,d1               | do shifts 1 thru 4
         bsr.s     cordic              | first cordic loops
         sub.l     #4,a0               | redo table entry
         sub.w     #1,d2               | redo shift count
         move.l    #9,d1               | do four through 13
         bsr.s     cordic              | second cordic loops
         sub.l     #4,a0               | back to entry 13
         sub.w     #1,d2               | redo shift for 13
         move.l    #10,d1              | now 13 through 23
         bsr.s     cordic              | and finish up

* now finalize the result
         tst.b     1(sp)               | test original sign
         bpl.s     fsepos              | branch positive argument
         neg.l     d5                  | change y for subtraction
         neg.b     (sp)                | negate adjusted exponent to subtract
fsepos:  add.l     d5,d6               | add or subtract y to/from x
         jsr       ffptnorm            | float x
         move.l    d6,d7               | setup result
* add ln2 factor integer to the exponent
         add.b     (sp),d7             | add to exponent
         bmi       fpeovflw            | branch if too large
         beq       fpeovflw            | branch if too small
         add.l     #2,sp               | rid work data off stack
         movem.l   (sp)+,d1-d6/a0      | restore registers
         rts                           | return to caller

*************************
* cordic loop subroutine*
*************************
cordic:  add.w     #1,d2               | increment shift count
         move.l    d5,d3               | copy y
         move.l    d6,d4               | copy x
         asr.l     d2,d3               | shift for y'
         asr.l     d2,d4               | shift for x'
         tst.l     d7                  | test arg value
         bmi.s     febmi               | branch minus test
         add.l     d4,d5               | y=y+x'
         add.l     d3,d6               | x=x+y'
         sub.l     (a0)+,d7            | arg=arg-table(n)
         dbra      d1,cordic           | loop until done
         rts                           | return

febmi:   sub.l     d4,d5               | y=y-x'
         sub.l     d3,d6               | x=x-y'
         add.l     (a0)+,d7            | arg=arg+table(n)
         dbra      d1,cordic           | loop until done
         rts                           | return


         end
