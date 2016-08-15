         ttl       fast floating point power (ffppwr)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************
 
*************************************************
*                  ffppwr                       *
*       fast floating point power function      *
*                                               *
*  input:   d6 - floating point exponent value  *
*           d7 - floating point argument value  *
*                                               *
*  output:  d7 - result of the value taken to   *
*                the power specified            *
*                                               *
*     all registers but d7 are transparent      *
*                                               *
*  code size:  36 bytes   stack work: 42 bytes  *
*                                               *
* calls subroutines: ffplog, ffpexp and ffpmul2  *
*                                               *
*  condition codes:                             *
*        z - set if the result is zero          *
*        n - cleared                            *
*        v - set if overflow occurred or base   *
*            value argument was negative        *
*        c - undefined                          *
*        x - undefined                          *
*                                               *
*  notes:                                       *
*    1) a negative base value will force the use*
*       if its absolute value.  the "v" bit will*
*       be set upon function return.            *
*    2) if the result overflows then the        *
*       maximum size value is returned with the *
*       "v" bit set in the condition code.      *
*    3) spot checks show at least six digit     *
*       precision for 80 percent of the cases.  *
*                                               *
*  time: (8mhz no wait states assumed)          *
*                                               *
*        the timing is very data sensitive with *
*        test samples ranging from 720 to       *
*        1206 microseconds                      *
*                                               *
*************************************************
*ffppwr   idnt  1,1 ffp power
 
         text
 
         xdef      ffppwr                        entry point
 
         xref      ffplog,ffpexp   exponent and log functions
         xref      ffpmul2            multiply function
         xref      ffpcpyrt            copyright stub
 
*****************
* power  entry  *
*****************
 
* take the logorithm of the base value
ffppwr:  tst.b     d7                  ? negative base value
         bpl.s     fpppos              branch positive
         and.b     #$7f,d7             take absolute value
         bsr.s     fpppos              find result using that
*        or.b      #$02,ccr            force "v" bit on for negative argument
         dc.l      $003c0002           *****assembler error*****
         rts                           return to caller
 
fpppos:  jsr       ffplog              find log of the number to be used
         movem.l   d3-d5,-(sp)         save multiply work registers
         jsr       ffpmul2              multiply by the exponent
         movem.l   (sp)+,d3-d5         restore multiply work registers
* if overflowed, ffpexp will set "v" bit and return desired result anyway
         jmp       ffpexp              result is exponent
