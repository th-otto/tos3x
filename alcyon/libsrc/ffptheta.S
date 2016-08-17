         ttl       arctangent cordic table - ffptheta
* ffptheta idnt      1,1 ffp arctangent table
***************************************
* (c) copyright 1981 by motorola inc. *
***************************************
 
         text
 
         xdef      ffptheta            external definition
 
*********************************************************
*             arctangent table for cordic               *
*                                                       *
* the following table is used during cordic             *
* transcendental evaluations for sine, cosine, and      *
* tangent and represents arctangent values 2**-n where  *
* n ranges from 0 to 24.  the format is binary(31,29)   *
* precision (i.e. the binary point is between bits      *
* 28 and 27 giving two leading non-fraction bits.)      *
*********************************************************
 
ffptheta:
         dc.l      $1921fb54     $c90fdaa2>>3  arctan(2**0)
         dc.l      $76b19c15>>3  arctan(2**-1)
         dc.l      $3eb6ebf2>>3  arctan(2**-2)
         dc.l      $1fd5ba9a>>3  arctan(2**-3)
         dc.l      $0ffaaddb>>3  arctan(2**-4)
         dc.l      $07ff556e>>3  arctan(2**-5)
         dc.l      $03ffeaab>>3  arctan(2**-6)
         dc.l      $01fffd55>>3  arctan(2**-7)
         dc.l      $00ffffaa>>3  arctan(2**-8)
         dc.l      $007ffff5>>3  arctan(2**-9)
         dc.l      $003ffffe>>3  arctan(2**-10)
         dc.l      $001fffff>>3  arctan(2**-11)
         dc.l      $000fffff>>3  arctan(2**-12)
         dc.l      $0007ffff>>3  arctan(2**-13)
         dc.l      $0003ffff>>3  arctan(2**-14)
         dc.l      $0001ffff>>3  arctan(2**-15)
         dc.l      $0000ffff>>3  arctan(2**-16)
         dc.l      $00007fff>>3  arctan(2**-17)
         dc.l      $00003fff>>3  arctan(2**-18)
         dc.l      $00001fff>>3  arctan(2**-19)
         dc.l      $00000fff>>3  arctan(2**-20)
         dc.l      $000007ff>>3  arctan(2**-21)
         dc.l      $000003ff>>3  arctan(2**-22)
         dc.l      $000001ff>>3  arctan(2**-23)
         dc.l      $000000ff>>3  arctan(2**-24)
         dc.l      $0000007f>>3  arctan(2**-25)
         dc.l      $0000003f>>3  arctan(2**-26)
