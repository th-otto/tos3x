         ttl       fast floating point log (ffplog)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************
*                  ffplog                       *
*       fast floating point logorithm           *
*                                               *
*  input:   d7 - input argument                 *
*                                               *
*  output:  d7 - logorithmic result to base e   *
*                                               *
*     all other registers totally transparent   *
*                                               *
*  code size: 184 bytes   stack work: 38 bytes  *
*                                               *
*  condition codes:                             *
*        z - set if the result is zero          *
*        n - set if result in is negative       *
*        v - set if invalid negative argument   *
*            or zero argument                   *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*                                               *
*  notes:                                       *
*    1) spot checks show errors bounded by      *
*       5 x 10**-8.                             *
*    2) negative arguments are illegal and cause*
*       the "v" bit to be set and the absolute  *
*       value used instead.                     *
*    3) a zero argument returns the largest     *
*       negative value possible with the "v" bit*
*       set.                                    *
*                                               *
*  time: (8mhz no wait states assumed)          *
*                                               *
*        times are very data sensitive with     *
*        samples ranging from 170 to 556        *
*        microseconds                           *
*                                               *
*************************************************
*ffplog   .idnt  1,2 ffp log

         text

         xdef      ffplog              | entry point

         xref      ffphthet            | hypertangent table
         xref      ffpadd,ffpdiv,ffpsub,ffpmul2 | arithmetic primitives
         xref      ffptnorm            | transcendental normalize routine
         xref      ffpcpyrt            | copyright stub

fpone    equ       $80000041           | floating value for one
log2     equ       $b1721840           | log(2) = .6931471805

**************
* log entry  *
**************

* insure argument positive
ffplog:  tst.b     d7                  | ? test sign
         beq.s     fplzro              | branch argument zero
         bpl.s     fplok               | branch alright

* argument is negative - use the absolute value and set the "v" bit
         and.b     #$7f,d7             | take absolute value
         bsr.s     fplok               | find log(abs(x))
fpsetv:  ori       #$02,ccr            | set overflow bit
         rts                           | return to caller

* argument is zero - return largest negative number with "v" bit
fplzro:  move.l    #-1,d7              | return largest negative
         jmp       fpsetv              | return with "v" bit set

* save work registers and strip exponent off
fplok:   movem.l   d1-d6/a0,-(sp)      | save all work registers
         move.b    d7,-(sp)            | save original exponent
         move.b    #64+1,d7            | force between 1 and 2
         move.l    #fpone,d6           | load up a one
         move.l    d7,d2               | copy argument
         jsr       ffpadd              | create arg+1
         exg       d7,d2               | swap result with argument
         jsr       ffpsub              | create arg-1
         move.l    d2,d6               | prepare for divide
         jsr       ffpdiv              | result is (arg-1)/(arg+1)
         beq.s     fplnocr             | zero so cordic not needed
* convert to bin(31,29) precision
         sub.b     #64+3,d7            | adjust exponent
         neg.b     d7                  | for shift necessary
         cmp.b     #31,d7              | ? insure not too small
         bls.s     fplshf              | no, go shift
         move.l    #0,d7               | force to zero
fplshf:  lsr.l     d7,d7               | shift to bin(31,29) precision

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
         lea       ffphthet,a0         | to inverse hyperbolic tangent table
         move.l    #22,d1              | loop 23 times
         move.l    #1,d2               | prime shift counter
         bra.s     cordic              | enter cordic loop

* cordic loop
fplpls:  asr.l     d2,d4               | shift(x')
         sub.l     d4,d5               | y = y - x'
         add.l     (a0),d6             | z = z + hypertan(i)
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

* now convert to float and add exponent*log(2) for final result
         move.l    #0,d7               | default zero if too small
         jsr       ffptnorm            | float z
         beq.s     fplnocr             | branch if too small
         add.b     #1,d6               | times two
         move.l    d6,d7               | setup in d7 in case exp=0
fplnocr: move.l    d7,d2               | save result
         move.l    #0,d6               | prepare original exponent load
         move.b    (sp)+,d6            | load it back
         sub.b     #64+1,d6            | convert exponent to binary
         beq.s     fplzpr              | branch zero partial here
         move.b    d6,d1               | save sign byte
         bpl.s     fplpos              | branch positive value
         neg.b     d6                  | force positive
fplpos:  ror.l     #8,d6               | prepare to convert to integer
         move.l    #$47,d5             | setup exponent mask
fplnorm: add.l     d6,d6               | shift to left
         dbmi      d5,fplnorm          | exp-1 and branch if not normalized
         move.b    d5,d6               | fix in exponent
         and.b     #$80,d1             | extract sign
         or.b      d1,d6               | insert sign in
         move.l    #log2,d7            | multiply exponent by log(2)
         jsr       ffpmul2             | multiply d6 and d7
         move.l    d2,d6               | now add cordic result
         jsr       ffpadd              | for final answer

fplzpr:  movem.l   (sp)+,d1-d6/a0      | restore registers
         rts                           | return to caller


         end
