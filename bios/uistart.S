*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*

	text
	
	xdef _ui_start
_ui_start equ GTSTART+GTLEN
	xdef _osinit
_osinit equ GTSTART+$5588
