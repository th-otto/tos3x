         ttl       ffpieee conversions to/from ffp (ffptieee,ffpfieee)
******************************************
*  (c)  copyright 1981 by motorola inc.  *
******************************************
 
****************************************************
*        ffptieee and ffpfieee subroutines         *
*                                                  *
*   this module contains single-precision          *
*   conversion routines for ieee format floating   *
*   point (draft 8.0) to and from motorola fast    *
*   floating point (ffp) values.                   *
*   these can be used when large groups of numbers *
*   need to be converted between formats.  see     *
*   the mc68344 user's guide for a fuller          *
*   explanation of the various methods for ieee    *
*   format support.                                *
*                                                  *
*   the fast floating point (non-ieee format)      *
*   provides results as precise as those required  *
*   by the ieee specification.  however, this      *
*   format has some minor differences:             *
*    1) if the true result of an operation         *
*       is exactly between representable           *
*       values, the ffp round-to-nearest           *
*       function may round to either even or odd.  *
*    2) the ffp exponent allows half of the range  *
*       that the single-precision ieee format      *
*       provides (approx. 10 to the +-19 decimal). *
*    3) the ieee format specifies infinity,        *
*       not-a-number, and denormalized data        *
*       types that are not directly supported      *
*       by the ffp format.  however, they may be   *
*       added via customizing or used via the ieee *
*       format equivalent compatible calls         *
*       described in the mc68344 user's guide.     *
*    4) all zeroes are considered positive         *
*       in ffp format.                             *
*    5) the slightly higher precision multiply     *
*       routine "ffpmul2" should be substituted    *
*       for the default routine "ffpmul" for       *
*       completely equivalent precision.           *
*                                                  *
****************************************************
         page
*ffpieee  .idnt   1,1  ffp conversions to/from ieee format
 
         text
 
         xdef      ffptieee,ffpfieee
 
****************************************************
*               ffptieee                           *
*                                                  *
*  fast floating point to ieee format              *
*                                                  *
*   input:   d7 - fast floating point value        *
*                                                  *
*   output:  d7 - ieee format floating point value *
*                                                  *
*   condition codes:                               *
*            n - set if the result is negative     *
*            z - set if the result is zero         *
*            v - undefined                         *
*            c - undefined                         *
*            x - undefined                         *
*                                                  *
*   notes:                                         *
*     1) no work storage or registers required.    *
*     2) all zeroes will be converted positive.    *
*     3) no not-a-number, inifinity, denormalized, *
*        or indefinites generated. (unless         *
*        user customized.)                         *
*                                                  *
*   times (assuming in-line code):                 *
*           value zero      18 cycles              *
*           value not zero  66 cycles              *
*                                                  *
****************************************************
 
ffptieee:add.l     d7,d7     delete mantissa high bit
         beq.s     done1     branch zero as finished
         eor.b     #$80,d7   to twos complement exponent
         asr.b     #1,d7     form 8-bit exponent
         sub.b     #$82,d7   adjust 64 to 127 and excessize
         swap.w    d7        swap for high byte placement
         rol.l     #7,d7     set sign+exp in high byte
done1:
 
         rts                 return to caller
         page
************************************************************
*                     ffpfieee                             *
*         fast floating point from ieee format             *
*                                                          *
*   input:   d7 - ieee format floating point value         *
*   output:  d7 - ffp format floating point value          *
*                                                          *
*   condition codes:                                       *
*            n - undefined                                 *
*            z - set if the result is zero                 *
*            v - set if result overflowed ffp format       *
*            c - undefined                                 *
*            x - undefined                                 *
*                                                          *
*   notes:                                                 *
*     1) register d5 is used for work storage and not      *
*        transparent.                                      *
*     2) not-a-number, inifinity, and denormalized         *
*        types as well as an exponent outside of ffp range *
*        generate a branch to a specific part of the       *
*        routine.  customizing may easily be done there.   *
*                                                          *
*        the default actions for the various types are:    *
*      label      type         default substitution        *
*      -----      ----         --------------------        *
*       nan    not-a-number    zero                        *
*       inf    infinity        largest ffp value same sign *
*                              ("v" set in ccr)            *
*       denor  denormalized    zero                        *
*       exphi  exp too large   largest ffp value same sign *
*                              ("v" set in ccr)            *
*       explo  exp too small   zero                        *
*                                                          *
*   times (assuming in-line code):                         *
*           value zero      78 cycles                      *
*           value not zero  72 cycles                      *
*                                                          *
************************************************************
 
vbit     equ       $02       condition code register "v" bit mask
 
ffpfieee:swap.w    d7        swap word halves
         ror.l     #7,d7     exponent to low byte
         move.l    #-128,d5  load $80 mask in work register
         eor.b     d5,d7     convert from excess 127 to two's-complement
         add.b     d7,d7     from 8 to 7 bit exponent
         bvs.s     ffpovf    branch will not fit
         add.b     #2<<1+1,d7 adjust excess 127 to 64 and set mantissa high bit
         bvs.s     exphi     branch exponent too large (overflow)
         eor.b     d5,d7     back to excess 64
         ror.l     #1,d7     to fast float representation ("v" cleared)
done2:
 
         rts                 return to caller
         page
* overflow detected - caused by one of the following:
*        - false overflow due to difference between excess 127 and 64 format
*        - exponent too high or low to fit in 7 bits (exponent over/underflow)
*        - an exponent of $ff representing an infinity
*        - an exponent of $00 representing a zero, nan, or denormalized value
ffpovf:   bcc.s     ffpovlw   branch if overflow (exponent $ff or too large)
* overflow - check for possible false overflow due to different excess formats
         cmp.b     #$7c,d7   ? was original argument really in range
         beq.s     ffpovfls  yes, branch false overflow
         cmp.b     #$7e,d7   ? was original argument really in range
         bne.s     ffptovf   no, branch true overflow
ffpovfls: add.b     #$80+2<<1+1,d7  excess 64 adjustment and mantissa high bit
         ror.l     #1,d7     finalize to fast floating point format
         tst.b     d7        insure no illegal zero sign+exponent byte
         bne.s     done2     done if does not set s+exp all zeroes
         bra.s     explo     treat as underflowed exponent otherwise
* exponent low - check for zero, denormalized value, or too small an exponent
ffptovf: and.w     #$feff,d7 clear sign bit out
         tst.l     d7        ? entire value now zero
         beq.s     done2     branch if value is zero
         tst.b     d7        ? denormalized number (significant#0, exp=0)
         beq.s     denor     branch if denormalized
 
***************
*****explo - exponent to small for ffp format
***************
*  the sign bit will be bit 8.
explo:   move.l    #0,d7     default zero for this case ("v" cleared)
         bra.s     done2     return to mainline
 
***************
*****denor - denormalized number
***************
denor:   move.l    #0,d7     default is to return a zero ("v" cleared)
         bra.s     done2     return to mainline
 
* exponent high - check for exponent too high, infinity, or nan
ffpovlw: cmp.b     #$fe,d7   ? was original exponent $ff
         bne.s     exphi     no, branch exponent too large
         lsr.l     #8,d7     shift out exponent
         lsr.l     #1,d7     shift out sign
         bne.s     nan       branch not-a-number
 
***************
*****inf - infinity
***************
*  the carry and x bit represent the sign
inf:     move.l    #-1,d7    setup maximum ffp value
         roxr.b    #1,d7     shift in sign
         ori       #vbit,ccr show overflow occured [clh] was sr
         bra.s     done2     return with maximum same sign to mainline
 
***************
*****exphi - exponent to large for ffp format
***************
*  the sign bit will be bit 8.
exphi:   lsl.w     #8,d7     set x bit to sign
         move.l    #-1,d7    setup maximum number
         roxr.b    #1,d7     shift in sign
         ori       #vbit,ccr show overflow ocurred [vlh] was sr
         bra.s     done2     return maximum same sign to mainline
 
***************
*****nan - not-a-number
***************
* bits 0 thru 22 contain the nan data field
nan:     move.l    #0,d7     default to a zero ("v" bit cleared)
         bra.s     done2     return to mainline
 
         end
