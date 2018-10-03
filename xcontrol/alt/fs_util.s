		globl fsel_init
		globl fsel_name
		
		xref Supexec
		xref Dgetdrv
		xref Dgetpath
		xref strrchr
		xref strchr
		xref strcpy
		xref strcat
		xref fsel_input
		xref fsel_exinput
				
get_vars:
		moveq.l   #2,d0
		add.l     ($000004F2).w,d0
		movea.l   d0,a0
		move.w    (a0),TOS_version
		moveq.l   #65,d1
		add.b     ($00000447).w,d1
		move.b    d1,boot_drive
		rts
fsel_init:
		move.l    a2,-(a7)
		lea.l     get_vars(pc),a0
		jsr       Supexec(pc)
		lea.l     boot_path,a2
		jsr       Dgetdrv(pc)
		add.b     #$41,d0
		move.b    d0,(a2)+
		move.b    #$3A,(a2)+
		clr.w     d0
		movea.l   a2,a0
		jsr       Dgetpath(pc)
		movea.l   (a7)+,a2
		rts

fsel_name:
		movem.l   d3-d4/a2-a6,-(a7)
		lea.l     -148(a7),a7
		move.w    d0,d4
		movea.l   a0,a2
		move.l    a1,144(a7)
		movea.l   180(a7),a5
		lea.l     16(a7),a4
		lea.l     2(a7),a6
		move.b    (a0),d1
		beq       fsnam3
		moveq.l   #92,d0
		jsr       strrchr(pc)
		movea.l   a0,a3
		move.l    a3,d0
		beq       fsnam1
		clr.b     (a3)+
		movea.l   a2,a1
		movea.l   a4,a0
		jsr       strcpy(pc)
		bra       fsnam2
fsnam1:
		lea.l     boot_path,a1
		movea.l   a4,a0
		jsr       strcpy(pc)
		movea.l   a4,a3
fsnam2:
		movea.l   a3,a1
		movea.l   a6,a0
		jsr       strcpy(pc)
		moveq.l   #46,d0
		movea.l   a6,a0
		jsr       strchr(pc)
		movea.l   a0,a3
		move.l    a3,d0
		bne       fsnam5
		bra       fsnam4
fsnam3:
		lea.l     boot_path,a1
		movea.l   a4,a0
		jsr       strcpy(pc)
		clr.b     (a6)
fsnam4:
		lea.l     dotstar,a3
fsnam5:
		lea.l     backslash,a1
		movea.l   a4,a0
		jsr       strcat(pc)
		move.l    144(a7),d0
		beq       fsnam6
		movea.l   d0,a1
		movea.l   a4,a0
		jsr       strcat(pc)
		bra       fsnam7
fsnam6:
		lea.l     dotstar+1,a1
		movea.l   a4,a0
		jsr       strcat(pc)
		movea.l   a3,a1
		movea.l   a4,a0
		jsr       strcat(pc)
fsnam7:
		move.l    a5,d0
		beq       fsnam8
		cmpi.w    #$0104,TOS_version
		bcs       fsnam8
		move.l    d0,-(a7)
		pea.l     4(a7)
		movea.l   a6,a1
		movea.l   a4,a0
		jsr       fsel_exinput(pc)
		addq.w    #8,a7
		move.w    d0,d3
		bra       fsnam9
fsnam8:
		pea.l     (a7)
		movea.l   a6,a1
		movea.l   a4,a0
		jsr       fsel_input(pc)
		addq.w    #4,a7
		move.w    d0,d3
fsnam9:
		tst.w     d3
		ble       fsnam10
		move.w    (a7),d3
		bne       fsnam11
fsnam10:
		clr.b     (a2)
		bra       fsnam15
fsnam11:
		moveq.l   #92,d0
		movea.l   a4,a0
		jsr       strrchr(pc)
		movea.l   a0,a3
		clr.b     (a0)
		movea.l   a4,a1
		movea.l   a2,a0
		jsr       strcpy(pc)
		lea.l     backslash,a1
		movea.l   a2,a0
		jsr       strcat(pc)
		movea.l   a6,a1
		movea.l   a2,a0
		jsr       strcat(pc)
		moveq.l   #92,d0
		movea.l   a2,a0
		jsr       strrchr(pc)
		movea.l   a0,a3
		moveq.l   #46,d0
		movea.l   a2,a0
		jsr       strrchr(pc)
		movea.l   a0,a5
		move.l    a5,d0
		beq       fsnam12
		cmpa.l    d0,a3
		bcs       fsnam13
fsnam12:
		moveq.l   #46,d0
		movea.l   144(a7),a0
		jsr       strrchr(pc)
		movea.l   a0,a5
		move.l    a5,d0
		beq       fsnam13
		movea.l   a5,a1
		movea.l   a2,a0
		jsr       strcat(pc)
fsnam13:
		tst.w     d4
		beq       fsnam14
		movea.l   a4,a1
		lea.l     boot_path,a0
		jsr       strcpy(pc)
fsnam14:
		move.b    (a6),d0
		bne       fsnam15
		clr.w     d3
fsnam15:
		move.w    d3,d0
		lea.l     148(a7),a7
		movem.l   (a7)+,d3-d4/a2-a6
		rts

		data
		
dotstar:
		dc.b '.','*',0,0
backslash:
		dc.b $5c, 0

		bss

TOS_version: ds.w 1
boot_drive: ds.b 1
boot_path: ds.b 129
