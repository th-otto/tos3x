/*
  Standard int definitions for CP/M-68K and Alcyon C.

  Not many because not all are available. But this should be a bit
  better than the definitions in portab.h

 */

#ifndef __STDINT_H_
#define __STDINT_H_

typedef char          int8_t;
typedef int           int16_t;
typedef long          int32_t;

/* There is no unsigned 8 bit type available */
typedef unsigned int uint16_t;
/* There is no unsigned 32 bit type available */
#endif
