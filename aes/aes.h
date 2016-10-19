#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
#endif

#include "../common/config.h"
#include "aesdefs.h"

#if TOSVERSION >= 0x0100
#undef AESVERSION
#define AESVERSION 0x0120
#endif

#if TOSVERSION >= 0x0104
#undef AESVERSION
#define AESVERSION 0x0140
#endif

#if TOSVERSION >= 0x0201
#undef AESVERSION
#define AESVERSION 0x0310
#endif

#if TOSVERSION >= 0x0206
#undef AESVERSION
#define AESVERSION 0x0320
#endif

#if TOSVERSION >= 0x0301
#undef AESVERSION
#define AESVERSION 0x0300
#endif

#if TOSVERSION >= 0x0306
#undef AESVERSION
#define AESVERSION 0x0320
#endif

#if TOSVERSION >= 0x0400
#undef AESVERSION
#define AESVERSION 0x0340
#endif
