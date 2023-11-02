         ttl       fast floating point cordic hyperbolic table (ffphthet)
*ffphthet .idnt  1,1 ffp inverse hyperbolic table

***************************************
* (c) copyright 1981 by motorola inc. *
***************************************

         text

         xdef      ffphthet     external definition

*********************************************************
*     inverse hyperbolic tangent table for cordic       *
*                                                       *
* the following table is used during cordic             *
* transcendental evaluations for log and exp. it has    *
* inverse hyperbolic tangent for 2**-n where n ranges   *
* from 1 to 24.  the format is binary(31,29)            *
* precision (i.e. the binary point is assumed between   *
* bits 27 and 28 with three leading non-fraction bits.) *
*********************************************************

ffphthet: dc.l      $1193ea7a    $8c9f53d0>>3 harctan(2**-1)   .549306144
         dc.l      $4162bbe8>>3 harctan(2**-2)   .255412812
         dc.l      $202b1238>>3 harctan(2**-3)
         dc.l      $10055888>>3 harctan(2**-4)
         dc.l      $0800aac0>>3 harctan(2**-5)
         dc.l      $04001550>>3 harctan(2**-6)
         dc.l      $020002a8>>3 harctan(2**-7)
         dc.l      $01000050>>3 harctan(2**-8)
         dc.l      $00800008>>3 harctan(2**-9)
         dc.l      $00400000>>3 harctan(2**-10)
         dc.l      $00200000>>3 harctan(2**-11)
         dc.l      $00100000>>3 harctan(2**-12)
         dc.l      $00080000>>3 harctan(2**-13)
         dc.l      $00040000>>3 harctan(2**-14)
         dc.l      $00020000>>3 harctan(2**-15)
         dc.l      $00010000>>3 harctan(2**-16)
         dc.l      $00008000>>3 harctan(2**-17)
         dc.l      $00004000>>3 harctan(2**-18)
         dc.l      $00002000>>3 harctan(2**-19)
         dc.l      $00001000>>3 harctan(2**-20)
         dc.l      $00000800>>3 harctan(2**-21)
         dc.l      $00000400>>3 harctan(2**-22)
         dc.l      $00000200>>3 harctan(2**-23)
         dc.l      $00000100>>3 harctan(2**-24)

         end
