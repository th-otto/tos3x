/*
 * cartridge.S - ROM cartridge for ST-Book
 */

    text

cart_beg:
	dc.l 0xabcdef42
	dc.l second              /* next program header (0 = no other program) */
	dc.l init+0x08000000     /* cartridge init (24 bit, upper byte = type) */
	dc.l start               /* program to execute */
	dc.w $1776               /* date 22.11.1991 */
	dc.w $69B6               /* time 13:13:44 */ /* BUG: time&date are swapped here */
	dc.l cart_end-init       /* calculated program length */
	dc.b "BOOKROM.PRG",0
	even
	dc.w $ffff


init:	
start:	
	rts


cart_end: ds.b 0


second:	
	dc.l 0                   /* next program header (0 = no other program) */
	dc.l secinit             /* cartridge init (24 bit, upper byte = type) */
	dc.l secstart            /* program to execute */
	dc.w $1869               /* date 09.03.1992 */
	dc.w $99C6               /* time 19:14:12 */ /* BUG: time&date are swapped here */
	dc.l secend-secinit      /* calculated program length */
	dc.b "CARTROM.IMG",0
	even
	dc.w $FFFF

secinit:
secstart:
	rts


secend: ds.b 0
