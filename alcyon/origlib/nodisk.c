/* NODISK: pulls out as much disk i/o as possible. */


nodisk() { ; }					/* stubroutine 'tag'	    */


char * __nodisk_io_msg = "disk i/o rtns";

_rdasc() { _optoff(__nodisk_io_msg); }
_rdbin() { _optoff(__nodisk_io_msg); }
_wrtasc() { _optoff(__nodisk_io_msg); }
_wrtbin() { _optoff(__nodisk_io_msg); }

