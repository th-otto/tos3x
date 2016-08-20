*	ACCSTART.S	4/30/85			MIKE SCHMAL
*
*	DESK ACCESSORY STARTUP PROGRAM
*
* This version only contains what is necessary for a desk accessory to run
* All other garbage has been removed as not to waste memory.
*

	.text
	.globl	_main
	.globl	_ctrl_cnts

*
*  Must be first object file in link statement
*

	move.l	#ustk,a7	* get program stack
	jsr	_main		* go to program
	move.l	#0,(a7)		* back to gemdos
	trap	#1

	.bss
	.even
	.ds.l	256
ustk:	.ds.l	1
*
