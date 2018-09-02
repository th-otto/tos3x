/* Prototypes for functions defined in loadbind.c */

#ifndef __COMPILER_H__
#include <compiler.h>
#endif

VOID Dsp_Init PROTO((void));
VOID DspFlushSubroutines PROTO((void));
int DspLdSubroutine PROTO((const void *codeptr, long size, int ability));
long DspLodToBinary PROTO((const char *file, char *dspbuff));
int DspLdProg PROTO((const char *file, int ability, void *codeptr));
VOID DspExProg PROTO((const char *codeptr, long codesize, int ability));

VOID vector_copy PROTO((void));
VOID DspExBoot PROTO((const char *codeptr, long codesize, int ability));
VOID DspBlkHandShake PROTO((const char *codeptr, long codesize, char *data_out, long size_out));
VOID DspDoBlock PROTO((const char *codeptr, long codesize, char *data_out, long size_out));

VOID Dsp_Move_Block PROTO((long from, long to, long size));
VOID dsp_set_vector PROTO((long addr, int vector));
VOID dsp_load_vectors PROTO((void));
VOID Dsp_Run_SubLoader PROTO((void));
VOID Send_Load_Info PROTO((long addr, long size));
VOID dsp_send_vectors PROTO((void));
