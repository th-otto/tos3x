#include "../common/config.h"
#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
#endif

typedef int16_t BOOLEAN;
#define FALSE 0
#define TRUE 1

/* ERROR - error return code */
typedef	int32_t ERROR;
 
#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#include "mobdefs.h"
#include "defines.h"
#include "window.h"
#include "gemdefs.h"
#include "deskusa.h"
#include "osbind.h"
#include "extern.h"



/*
 * extern references from AES
 */
extern int16_t st_lang;		/* Language code    */
extern uint16_t st_time;		/* time code        */
extern uint16_t st_date;
extern uint16_t st_dchar;
extern int16_t st_keybd;
