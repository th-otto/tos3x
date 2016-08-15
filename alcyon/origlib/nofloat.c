/* NOFLOAT: provides 'stubroutines' to satisfy floating point library	    */
/*	references from 'printf' et al.					    */

#include "lib.h"

VOID nofloat(NOTHING) { ; }					/* stubroutine 'tag'	    */

const char *__nofloat_msg = "floating point";

VOID _petoa(NOTHING) { _optoff(__nofloat_msg); }
VOID _pftoa(NOTHING) { _optoff(__nofloat_msg); }
VOID _pgtoa(NOTHING) { _optoff(__nofloat_msg); }
VOID _atof(NOTHING)   { _optoff(__nofloat_msg); }
