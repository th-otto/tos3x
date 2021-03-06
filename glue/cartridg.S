/*
 * cartridge.S - ROM cartridge for ST-Book
 */

#include "asmdefs.h"
#include "tosvars.inc"

CARTDRIVE equ 15 /* 'P' - 'A' */

#define SECTOR_SIZE 512

#define IMAGE_SIZE 252

bpb         equ $0800
#define B_16 1

oldbpb      equ $0816
oldrw       equ $081a
oldmediach  equ $081e

    text

cart_beg:
	dc.l $abcdef42
	dc.l second              /* next program header (0 = no other program) */
	dc.l init+$08000000      /* cartridge init (24 bit, upper byte = type) */
	dc.l start               /* program to execute */
	dc.w $1776               /* date 22.11.1991 */
	dc.w $69B6               /* time 13:13:44 */ /* BUG: time&date are swapped here */
	dc.l cart_end-init       /* calculated program length */
	dc.b 'BOOKROM.PRG',0
	even
	dc.w $ffff


init:	
start:
	bra.s install
	
	dc.l $014c
	dc.w 0,0,0,0,0,0,0,0,0,0,0

/*
 * fake a boot sector:
 *   0x00 bra[2] = 0x601c
 *   0x02 loader[6] = 
 *   0x08 serial[3] = 
 *   0x0b bps[2] = 512
 *   0x0d spc    = 1
 *   0x0e res[2] = 1
 *   0x10 fat    = 2
 *   0x11 dir[2] = 16
 *   0x13 sec[2] = 0x1f8
 *   0x15 media  = 0xf9
 *   0x16 spf[2] = 2
 *   0x18 spt[2] = 1
 *   0x1a sides[2] = 1
 *   0x1c hid[2] = 0
 */
install:

		move.w    #IMAGE_SIZE,d0          /* size of the image */
		move.w    #SECTOR_SIZE,(bpb+0).w /* sector size */
		move.w    #$0001,(bpb+2).w /* cluster size */
		move.w    #SECTOR_SIZE,(bpb+4).w /* cluster size in bytes */
		move.w    #$0001,(bpb+6).w /* root directory length */
		move.w    #B_16,(bpb+16).w /* flags: 16-bit fat */
		asl.w     #1,d0
		move.w    d0,d1
		move.w    d1,(bpb+14).w    /* number of clusters */
		add.w     #$00FF,d1
		asr.w     #8,d1
		move.w    d1,(bpb+8).w     /* fat size */
		move.w    d1,(bpb+10).w    /* fat record of 2nd fat */
		move.w    d1,(bpb+12).w
		add.w     d1,(bpb+12).w    /* first data record */
		dc.w $0678,$0001,bpb+12            /* addi.w    #$0001,(bpb+12).w */
		move.w    #CARTDRIVE,d0
		moveq.l   #1,d7
		lsl.l     d0,d7
		or.l      d7,(_drvbits).w
		move.l    (_hdv_bpb).w,(oldbpb).w
		move.l    (_hdv_rw).w,(oldrw).w
		move.l    (_hdv_mediach).w,(oldmediach).w
		lea.l     newbpb(pc),a0
		move.l    a0,(_hdv_bpb).w
		lea.l     newrw(pc),a0
		move.l    a0,(_hdv_rw).w
		lea.l     newmediach(pc),a0
		move.l    a0,(_hdv_mediach).w
		pea.l     msg(pc)
		move.w    #$0009,-(a7)
		trap      #1
		addq.w    #6,a7
		rts
newbpb:
		move.w    4(a7),d0
		movea.l   (oldbpb).w,a0
		lea.l     cartbpb(pc),a1
		bra.w     checkdrv
newrw:
		move.w    14(a7),d0
		movea.l   (oldrw).w,a0
		lea.l     cartrw(pc),a1
		bra.w     checkdrv
newmediach:
		move.w    4(a7),d0
		movea.l   (oldmediach).w,a0
		lea.l     cartmediach(pc),a1
checkdrv:
		cmp.w     #CARTDRIVE,d0
		bne.w     notours
		movea.l   a1,a0
notours:
		jmp       (a0)

cartbpb:
		move.l    #bpb,d0
		rts

cartrw:
		movea.l   #secstart,a0
		moveq.l   #0,d0
		move.w    12(a7),d0 /* get starting sector # */
		lsl.l     #8,d0
		lsl.l     #1,d0
		adda.l    d0,a0
		moveq.l   #0,d2
		move.w    10(a7),d2 /* get number of sectors */
		lsl.l     #4,d2
		movea.l   6(a7),a1 /* get buffer ptr */
		move.l    a1,d0
		btst      #0,5(a7) /* write request? */
		beq.s     cartread /* no, go ahead */
		moveq.l   #-13,d0  /* E_WRPRO */
		rts
cartread:
		btst      #0,d0
		bne.s     odd
fastcopy:
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		subq.l    #1,d2
		bmi.s     copyend
		bne.s     fastcopy
		bra.s     copyend
odd:
		lsl.w     #3,d2
slowcopy:
		move.b    (a0)+,(a1)+
		move.b    (a0)+,(a1)+
		move.b    (a0)+,(a1)+
		move.b    (a0)+,(a1)+
		subq.l    #1,d2
		bmi.s     copyend
		bne.s     slowcopy
copyend:
		clr.l     d0
		rts


cartmediach:
		clr.l     d0
		rts


msg:	dc.b '0253K ROMDISK installed as ',CARTDRIVE+'A',':',13,10,0
        even
        dc.l 0


cart_end: ds.b 0

 		dcb.b 512-(*-cart_beg),255


second:	
	dc.l 0                   /* next program header (0 = no other program) */
	dc.l secinit             /* cartridge init (24 bit, upper byte = type) */
	dc.l secstart            /* program to execute */
	dc.w $1869               /* date 09.03.1992 */
	dc.w $99C6               /* time 19:14:12 */ /* BUG: time&date are swapped here */
	dc.l IMAGE_SIZE*1024+$A00      /* calculated program length */
	dc.b 'CARTROM.IMG',0
	even
	dc.w $FFFF

secinit: ds.b 0
secstart: ds.b 0
