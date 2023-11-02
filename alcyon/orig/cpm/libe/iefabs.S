       ttl     ieee format equivalent abs and neg (iefabs/iefneg)
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

*************************************************************
*                     iefabs                                *
*  fast floating point ieee format equivalent absolute value*
*                                                           *
*  input:  d7 - ieee format number argument                 *
*                                                           *
*  output: d7 - ieee format number absolute value result    *
*                                                           *
*      condition codes:                                     *
*              n - cleared                                  *
*              z - set if result is zero                    *
*              v - set if result is nan (not-a-number)      *
*                  (occurs only if input argument is nan)   *
*              c - undefined                                *
*              x - undefined                                *
*                                                           *
*               all registers transparent                   *
*                                                           *
*            maximum stack used:   24 bytes                 *
*                                                           *
*  notes:                                                   *
*    1) this routine properly handles all ieee floating     *
*       point values and number types.                      *
*    2) if the input argument is a nan (not-a-number) then  *
*       it will be returned as the result with the "v" bit  *
*       set in the condition code register.                 *
*                                                           *
*************************************************************
         page
*iefabs .idnt    1,1  ieee format equivalent abs and neg

         xdef      iefabs    | ieee format absolute value

         xref      iefsop   | single argument conversion routine
         xref      iefrtod7 | return caller's original d7 as result
         xref      ffpcpyrt | copyright notice

         text

******************************
* absolute value entry point *
******************************
iefabs:  jsr       iefsop    | direct return to caller if nan encountered
         nop                 | +0 normalized return (or zero or denormalized)
*                                       +2 infinity return

* all values may be absolutized by forcing a plus sign on the original value
         bclr.b    #7,16(sp)  | clear sign bit of original value
         jmp       iefrtod7   | and return original value altered a wee bit
         page
*************************************************************
*                     iefneg                                *
*  fast floating point ieee format equivalent negate        *
*                                                           *
*  input:  d7 - ieee format number argument                 *
*                                                           *
*  output: d7 - ieee format number negated result           *
*                                                           *
*      condition codes:                                     *
*              n - set if result is negative                *
*              z - set if result is zero                    *
*              v - set if result is nan (not-a-number)      *
*                  (occurs only if input argument is nan)   *
*              c - undefined                                *
*              x - undefined                                *
*                                                           *
*               all registers transparent                   *
*                                                           *
*           maximum stack used:     24 bytes                *
*                                                           *
*  notes:                                                   *
*    1) this routine properly handles all ieee floating     *
*       point values and number types.                      *
*    2) if the input argument is a nan (not-a-number) then  *
*       it will be returned as the result with the "v" bit  *
*       set in the condition code register.                 *
*                                                           *
*************************************************************
         page
         xdef      iefneg    | ieee format negate

**********************
* negate entry point *
**********************
iefneg:  jsr       iefsop    | direct return to caller if nan encountered
         nop                 | +0 normalized return (or zero or denormalized)
*                                       +6 both infinity return

* all values may be negated by inverting the sign bit of the original value
         bchg.b    #7,16(sp) | negate original sign bit
         jmp       iefrtod7  | and return this modified original register

         end
