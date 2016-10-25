#define __ASSEMBLER__ 1

#include "../common/config.h"
#include "aesdefs.h"
#include "../common/asmdefs.h"

	xdef _infdata
_infdata: ds.b 0
	xdef _XDeselect
_XDeselect: ds.b 0
	xdef _getcookie
_getcookie: ds.b 0
	xdef _scan_2
_scan_2: ds.b 0
	xdef _escan_str
_escan_str: ds.b 0
	xdef _save_2
_save_2: ds.b 0
	xdef _app_reschange
_app_reschange: ds.b 0
	xdef _g_name
_g_name: ds.b
	xdef _deskmain
_deskmain: ds.b 0
