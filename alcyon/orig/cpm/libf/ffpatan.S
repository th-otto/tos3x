         ttl       fast floating point arctangent (ffpatan)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  ffpatan                      *
*       fast floating point arctangent          *
*                                               *
*  input:   d7 - input argument                 *
*                                               *
*  output:  d7 - arctangent radian result       *
*                                               *
*     all other registers totally transparent   *
*                                               *
*  code size: 132 bytes   stack work: 32 bytes  *
*                                               *
*  condition codes:                             *
*        z - set if the result is zero          *
*        n - cleared                            *
*        v - cleared                            *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*                                               *
*  notes:                                       *
*    1) spot checks show at least six digit     *
*       precision on all sampled cases.         *
*                                               *
*  time: (8mhz no wait states assumed)          *
*                                               *
*        the time is very data sensitive with   *
*        sample values ranging from 238 to      *
*        465 microseconds                       *
*                                               *
*************************************************
*ffpatan  .idnt  1,2 ffp arctangent

         text

         xdef      ffpatan             | entry point

         xref      ffptheta            | arctangent table

         xref      ffpdiv,ffpsub       | arithmetic primitives
         xref      ffptnorm            | transcendental normalize routine
         xref      ffpcpyrt            | copyright stub

piov2    equ       $c90fdb41           | float pi/2
fpone    equ       $80000041           | float 1

********************
* arctangent entry *
********************

* save registers and perform argument reduction
ffpatan: movem.l   d1-d6/a0,-(sp)      | save caller's registers
         move.b    d7,-(sp)            | save original sign on stack
         and.b     #$7f,d7             | take absolute value of arg
* insure less than one for cordic loop
         move.l    #fpone,d6           | load up 1
         clr.b     -(sp)               | default no inverse required
         cmp.b     d6,d7               | ? less than one
         bcs.s     fpainrg             | branch in range
         bhi.s     fpardc              | higher - must reduce
         cmp.l     d6,d7               | ? less or equal to one
         bls.s     fpainrg             | branch yes, is in range
* argument > 1:  atan(1/x) =  pi/2 - atan(x)
fpardc:  not.b     (sp)                | flag inverse taken
         exg       d6,d7               | take inverse of argument
         jsr       ffpdiv              | perform divide

* perform cordic function
* convert to bin(31,29) precision
fpainrg: sub.b     #64+3,d7            | adjust exponent
         neg.b     d7                  | for shift necessary
         cmp.b     #31,d7              | ? too small to worry about
         bls.s     fpanotz             | branch if not too small
         move.l    #0,d6               | convert to a zero
         bra.s     fpazro              | branch if zero
fpanotz: lsr.l     d7,d7               | shift to bin(31,29) precision

*****************************************
* cordic calculation registers:         *
* d1 - loop count   a0 - table pointer  *
* d2 - shift count                      *
* d3 - y'   d5 - y                      *
* d4 - x'   d6 - z                      *
* d7 - x                                *
*****************************************

         move.l    #0,d6               | z=0
         move.l    #1<<29,d5           | y=1
         lea       ffptheta+4,a0       | to arctangent table
         move.l    #24,d1              | loop 25 times
         move.l    #1,d2               | prime shift counter
         bra.s     cordic              | enter cordic loop

* cordic loop
fplpls:  asr.l     d2,d4               | shift(x')
         add.l     d4,d5               | y = y + x'
         add.l     (a0),d6             | z = z + arctan(i)
cordic:  move.l    d7,d4               | x' = x
         move.l    d5,d3               | y' = y
         asr.l     d2,d3               | shift(y')
fplnlp:  sub.l     d3,d7               | x = x - y'
         bpl.s     fplpls              | branch negative
         move.l    d4,d7               | restore x
         add.l     #4,a0               | to next table entry
         add.b     #1,d2               | increment shift count
         lsr.l     #1,d3               | shift(y')
         dbra      d1,fplnlp           | and loop until done

* now convert to float and reconstruct the result
         jsr       ffptnorm            | float z
fpazro:  move.l    d6,d7               | copy answer to d7
         tst.b     (sp)+               | ? was inverse taken
         beq.s     fpaninv             | branch if not
         move.l    #piov2,d7           | take away from pi over two
         jsr       ffpsub              | subtract
fpaninv: move.b    (sp)+,d6            | load original sign
         tst.b     d7                  | ? result zero
         beq.s     fpartn              | return if so
         and.b     #$80,d6             | clear exponent portion
         or.b      d6,d7               | if minus, give minus result
fpartn:  movem.l   (sp)+,d1-d6/a0      | restore caller's registers
         rts                           | return to caller

         end
