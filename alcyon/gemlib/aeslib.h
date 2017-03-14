#include <stdint.h>
#include <obdefs.h>
#include <gemdefs.h>
#include <gembind.h>
#include "crysbind.h"

#define FALSE 0
#define TRUE  1

#ifndef NO_CONST
#  ifdef __GNUC__
#	 define NO_CONST(p) __extension__({ union { const void *cs; void *s; } x; x.cs = p; x.s; })
#  else
#    ifdef __ALCYON__ /* Alcyon parses the cast as function call??? */
#      define NO_CONST(p) p
#    else
#      define NO_CONST(p) ((void *)(p))
#    endif
#  endif
#endif

extern char const ctrl_cnts[];

typedef struct cblk
{
	int16_t *cb_pcontrol;
	int16_t *cb_pglobal;
	int16_t *cb_pintin;
	int16_t *cb_pintout;
	VOIDPTR *cb_padrin;
	VOIDPTR *cb_padrout;		
} CBLK;

extern int16_t control[C_SIZE];
extern int16_t global[G_SIZE];
extern int16_t int_in[I_SIZE];
extern int16_t int_out[O_SIZE];
extern VOIDPTR addr_in[AI_SIZE];
extern VOIDPTR addr_out[AO_SIZE];

int crystal PROTO((CBLK *));

int16_t crys_if PROTO((int16_t opcode));
