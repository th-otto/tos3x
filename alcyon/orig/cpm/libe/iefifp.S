      ttl     ieee format equivalent integer to float (iefifp)
************************************
* (c) copyright 1981 motorla inc.  *
************************************

***********************************************************
*    iefifp - ieee format equivalent integer to float     *
*                                                         *
*      input: d7 = fixed point integer (2's complement)   *
*      output: d7 = ieee format floating point equivalent *
*                                                         *
*      condition codes:                                   *
*                n - set if result is negative            *
*                z - set if result is zero                *
*                v - cleared (not a nan)                  *
*                c - undefined                            *
*                x - undefined                            *
*                                                         *
*            registers are transparent                    *
*                                                         *
*          maximum stack used:    28 bytes                *
*                                                         *
*  notes:                                                 *
*   1) since a longword binary value contains 31 bits of  *
*      precision which is more than the effective 24 bits *
*      available with the ieee single-precision format,   *
*      integers of greater than 24 bit magnitude will be  *
*      rounded and imprecise.                             *
*                                                         *
***********************************************************
         page
         xdef    iefifp     | external name

         xref      ffpifp   | fast floating point integer convert
         xref      ieftieee | convert ffp to ieee and return
         xref      ffpcpyrt | copyright notice

*iefifp .idnt    1,1  ieee format equivalent integer to float

         text

iefifp:  movem.l   d3-d7,-(sp) | save work registers
         bsr       ffpifp    | convert to fast floating point format
         bra       ieftieee  | to ieee format, test, and return

         end
