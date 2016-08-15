         ttl       ffp sine cosine tangent (ffpsin/ffpcos/ffptan/ffpsincs)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************
 
*************************************************
*        ffpsin ffpcos ffptan ffpsincs          *
*     fast floating point sine/cosine/tangent   *
*                                               *
*  input:   d7 - input argument (radian)        *
*                                               *
*  output:  d7 - function result                *
*           (ffpsincs also returns d6)          *
*                                               *
*     all other registers totally transparent   *
*                                               *
*  code size: 334 bytes   stack work: 38 bytes  *
*                                               *
*  condition codes:                             *
*        z - set if result in d7 is zero        *
*        n - set if result in d7 is negative    *
*        c - undefined                          *
*        v - set if result is meaningless       *
*            (input magnitude too large)        *
*        x - undefined                          *
*                                               *
*  functions:                                   *
*             ffpsin   -  sine result           *
*             ffpcos   -  cosine result         *
*             ffptan   -  tangent result        *
*             ffpsincs -  both sine and cosine  *
*                         d6 - sin, d7 - cosine *
*                                               *
*  notes:                                       *
*    1) input values are in radians.            *
*    2) function ffpsincs returns both sine     *
*       and cosine twice as fast as calculating *
*       the two functions independently for     *
*       the same value.  this is handy for      *
*       graphics processing.                    *
*    2) input arguments larger than two pi      *
*       suffer reduced precision.  the larger   *
*       the argument, the smaller the precision.*
*       excessively large arguments which have  *
*       less than 5 bits of precision are       *
*       returned unchanged with the "v" bit set.*
*    3) for tangent angles of infinite value    *
*       the largest possible positive number    *
*       is returned ($ffffff7f). this still     *
*       gives results well within single        *
*       precision calculation.                  *
*    4) spot checks show errors bounded by      *
*       4 x 10**-7 but for arguments close to   *
*       pi/2 intervals where 10**-5 is seen.    *
*                                               *
*  time: (8mhz no wait states and argument      *
*         assumed within +-pi)                  *
*                                               *
*           ffpsin       413 microseconds       *
*           ffpcos       409 microseconds       *
*           ffptan       501 microseconds       *
*           ffpsincs     420 microseconds       *
*************************************************
* ffpsin   idnt  1,2 ffp sine cosine tangent
 
         text
 
         xdef      ffpsin,ffpcos,ffptan,ffpsincs entry points
 
         xref      ffptheta           | inverse tangent table
 
         xref      ffpmul2,ffpdiv,ffpsub    | multiply, divide and subtract
         xref      ffptnorm           | transcendental normalize routine
         xref      ffpcpyrt           | copyright stub
 
pi       equ       $c90fdb42          | floating constant pi
fixedpi  equ       $c90fdaa2          | pi skeleton to 32 bits precision
inv2pi   equ       $a2f9833e          | inverse of two-pi
kinv     equ       $9b74ee40          | floating k inverse
nkfact   equ       $ec916240          | negative k inverse
 
********************************************
* entry for returning both sine and cosine *
********************************************
ffpsincs: move.w    #-2,-(sp)           | flag both sine and cosine wanted
         bra.s     fpscom              | enter common code
 
**********************
* tangent entry point*
**********************
ffptan:  move.w    #-1,-(sp)           | flag tangent with minus value
         bra.s     fpschl              | check very small values
 
**************************
* cosine only entry point*
**************************
ffpcos:  move.w    #1,-(sp)            | flag cosine with positive value
         bra.s     fpscom              | enter common code
 
* negative sine/tangent small value check
fpschm:  cmp.b     #$80+$40-8,d7       | ? less or same as -2**-9
         bhi.s     fpscom              | continue if not too small
* return argument
fpsrti:  add.l     #2,sp               | rid internal parameter
         tst.b     d7                  | set condition codes
         rts                           | return to caller
 
************************
* sine only entry point*
************************
ffpsin:  clr.w     -(sp)               | flag sine with zero
* sine and tangent values < 2**-9 return identities
fpschl:  tst.b     d7                  | test sign
         bmi.s     fpschm              | branch minus
         cmp.b     #$40-8,d7           | ? less or same than 2**-9
         bls.s     fpsrti              | return identity

* save registers and insure input within + or - pi range
fpscom:  movem.l   d1-d6/a0,-(sp)      | save all work registers
         move.l    d7,d2               | copy input over
         add.b     d7,d7               | rid sign bit
         cmp.b     #(64+5)<<1,d7       | ? abs(arg) < 2**6 (32)
         bls.s     fpsnlr              | branch yes, not too large
* argument is too large to subtract to within range
         cmp.b     #(64+20)<<1,d7      | ? test excessive size (>2**20)
         bls.s     fpsgpr              | no, go ahead and use
* error - argument so large result has no precision
*        or.b      #$02,ccr            | force v bit on
         dc.l      $003c0002           | *****assembler error*****
         movem.l   (sp)+,d1-d6/a0      | restore registers
         add.l     #2,sp               | clean internal argument off stack
         rts                           | return to caller

* we must find mod(arg,twopi) since argument is too large for subtractions
fpsgpr:  move.l    #inv2pi,d6          | load up 2*pi inverse constant
         move.l    d2,d7               | copy over input argument
         jsr       ffpmul2             | divide by 2pi (via multiply inverse)
* convert quotient to float integer
         move.b    d7,d5               | copy exponent over
         and.b     #$7f,d5             | rid sign from exponent
         sub.b     #64+24,d5           | find fractional precision
         neg.b     d5                  | make positive
         move.l    #-1,d4              | setup mask of all ones
         clr.b     d4                  | start zeroes at low byte
         lsl.l     d5,d4               | shift zeroes into fractional part
         or.b      #$ff,d4             | do not remove sign and exponent
         and.l     d4,d7               | strip fractional bits entirely
         move.l    #pi+1,d6            | load up 2*pi constant
         jsr       ffpmul2             | multiply back out
         move.l    d7,d6               | setup to subtract multiple of twopi
         move.l    d2,d7               | move argument in
         jsr       ffpsub              | find remainder of twopi divide
         move.l    d7,d2               | use it as new input argument

* convert argument to binary(31,26) precision for reduction within +-pi
fpsnlr:  move.l    #$0c90fdaa,d4       | fixedpi>>4 load pi
         move.l    d2,d7               | copy float argument
         clr.b     d7                  | clear sign and exponent
         tst.b     d2                  | test sign
         bmi.s     fpsnmi              | branch negative
         sub.b     #64+6,d2            | obtain shift value
         neg.b     d2                  | for 5 bit non-fraction bits
         cmp.b     #31,d2              | ? very small number
         bls.s     fpssh1              | no, go ahead and shift
         move.l    #0,d7               | force to zero
fpssh1:  lsr.l     d2,d7               | convert to fixed point
* force to +pi or below
fpspck:  cmp.l     d4,d7               | ? greater than pi
         ble.s     fpsckm              | branch not
         sub.l     d4,d7               | subtract
         sub.l     d4,d7               | .  twopi
         bra.s     fpspck              | and check again
 
fpsnmi:  sub.b     #$80+64+6,d2        | rid sign and get shift value
         neg.b     d2                  | for 5 non-fractional bits
         cmp.b     #31,d2              | ? very small number
         bls.s     fpssh2              | no, go ahead and shift
         move.l    #0,d7               | force to zero
fpssh2:  lsr.l     d2,d7               | convert to fixed point
         neg.l     d7                  | make negative
         neg.l     d4                  | make -pi
* force to -pi or above
fpsnck:  cmp.l     d4,d7               | ? less than -pi
         bge.s     fpsckm              | branch not
         sub.l     d4,d7               | add
         sub.l     d4,d7               | .  twopi
         bra.s     fpsnck              | and check again

*****************************************
* cordic calculation registers:         *
* d1 - loop count   a0 - table pointer  *
* d2 - shift count                      *
* d3 - x'   d5 - x                      *
* d4 - y'   d6 - y                      *
* d7 - test argument                    *
*****************************************
 
* input within range, now start cordic setup
fpsckm:  move.l    #0,d5               | x=0
         move.l    #nkfact,d6          | y=negative inverse k factor seed
         move.l    #$3243f6a8,d4       | fixedpi>>2, setup fixed pi/2 constant
         asl.l     #3,d7               | now to binary(31,29) precision
         bmi.s     fpsap2              | branch if minus to add pi/2
         neg.l     d6                  | y=positive inverse k factor seed
         neg.l     d4                  | subtract pi/2 for positive argument
fpsap2   add.l     d4,d7               | add constant
         lea       ffptheta,a0         | load arctangent table
         move.l    #23,d1              | loop 24 times
         move.l    #-1,d2              | prime shift counter
* cordic loop
fsinlp:  add.w     #1,d2               | increment shift count
         move.l    d5,d3               | copy x
         move.l    d6,d4               | copy y
         asr.l     d2,d3               | shift for x'
         asr.l     d2,d4               | shift for y'
         tst.l     d7                  | test arg value
         bmi.s     fsbmi               | branch minus test
         sub.l     d4,d5               | x=x-y'
         add.l     d3,d6               | y=y+x'
         sub.l     (a0)+,d7            | arg=arg-table(n)
         dbra      d1,fsinlp           | loop until done
         bra.s     fscom               | enter common code
fsbmi:   add.l     d4,d5               | x=x+y'
         sub.l     d3,d6               | y=y-x'
         add.l     (a0)+,d7            | arg=arg+table(n)
         dbra      d1,fsinlp           | loop until done

* now split up tangent and ffpsincs from sine and cosine
fscom:   move.w    7*4(sp),d1          | reload internal parameter
         bpl.s     fssincos            | branch for sine or cosine
 
         add.b     #1,d1               | see if was -1 for tangent
         bne.s     fsdual              | no, must be both sin and cosine
* tangent finish
         bsr.s     fsfloat             | float y (sin)
         move.l    d6,d7               | setup for divide into
         move.l    d5,d6               | prepare x
         bsr.s     fsfloat             | float x (cos)
         beq.s     fstinf              | branch infinite result
         jsr       ffpdiv              | tangent = sin/cos
fsinfrt: movem.l   (sp)+,d1-d6/a0      | restore registers
         add.l     #2,sp               | delete internal parameter
         rts                           | return to caller
* tangent is infinite. return maximum positive number.
fstinf:  move.l    #$ffffff7f,d7       | largest ffp number
         bra.s     fsinfrt             | and clean up
 
* sine and cosine
fssincos: beq.s     fssine             | branch if sine
         move.l    d5,d6               | use x for cosine
fssine:  bsr.s     fsfloat             | convert to float
         move.l    d6,d7               | return result
         tst.b     d7                  | and condition code test
         movem.l   (sp)+,d1-d6/a0      | restore registers
         add.l     #2,sp               | delete internal parameter
         rts                           | return to caller
 
* both sine and cosine
fsdual:  move.l    d5,-(sp)            | save cosine derivitive
         bsr.s     fsfloat             | convert sine derivitive to float
         move.l    d6,6*4(sp)          | place sine into saved d6
         move.l    (sp)+,d6            | restore cosine derivitive
         bra.s     fssine              | and continue restoring sine on the sly
 
* fsfloat - float internal precision but truncate to zero if < 2**-21
fsfloat: move.l    d6,d4               | copy internal precision value
         bmi.s     fsfneg              | branch negative
         cmp.l     #$000000ff,d6       | ? test magnitude
*        bhi       ffptnorm            | normalize if not too small
		bhi			dobranch
fsfzro:  move.l    #0,d6               | return a zero
         rts                           | return to caller
fsfneg   asr.l     #8,d4               | see if all ones bits 8-31
         add.l     #1,d4               | ? goes to zero
*        bne       ffptnorm            | normalize if not too small
		bne			dobranch
         bra.s     fsfzro              | return zero
 
dobranch:
		jmp		ffptnorm			   | 16-bit no-MMU problem
