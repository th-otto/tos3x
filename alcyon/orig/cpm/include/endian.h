/*
  Endian functions for the Alcyon C compiler under CP/M-68K

  Uses byte swapping functions coded in assembly. Not fast but
  hopefully usefull.
 */

#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#define __BYTE_ORDER__BIG_ENDIAN

#include <byteswap.h>

#define htobe16(_x) (_x)
#define htole16(_x) bswap_16(_x)
#define be16toh(_x) (_x)
#define le16toh(_x) bswap_16(_x)

#define htobe32(_x) (_x)
#define htole32(_x) (_x)bswap_32(_x)
#define be32toh(_x) (_x)
#define le32toh(_x) (_x)bswap_32(_x)

#endif
