
_vblqueue equ $456
_sysbase equ $4f2
_p_cookies equ $5a0

entry:
				bra       start
				dc.b      'K.Isakovic'

basepage equ entry-256
cmdline equ basepage+128

respart:

virt_work:
				dc.l	  0
dispatcher:
				dc.l	  0
ret_pc:
				dc.l	  0

				dc.l      $58425241 /* 'XBRA' */
				dc.l      $56464958 /* 'VFIX' */
oldtrap:
				dc.l      0
newtrap:
				move.l    a0,dispatcher-respart+cmdline
				cmpi.w    #$0073,d0
				bne.s     novdi
				movea.l   d1,a0
				movea.l   (a0),a0
				cmpi.w    #1,(a0)   /* v_opnwk? */
				beq.s     dofix
				cmpi.w    #100,(a0) /* v_opnvwk? */
				bne.s     go_old
dofix:
				lea.l     ret_pc(pc),a0
				move.l    2(a7),(a0)
				lea.l     ret_from_rom(pc),a0
				move.l    a0,2(a7)
go_old:
				movea.l   dispatcher(pc),a0
				move.l    oldtrap(pc),-(a7)
				rts
novdi:
				cmpi.w    #-1,d0
				bne.s     go_old
				lea.l     newdisp(pc),a0
				move.l    a0,d0
				movea.l   dispatcher(pc),a0
				rte

ret_from_rom:
				bsr.s     fixhandle
				move.l    ret_pc(pc),-(a7)
				rts

				dc.l      $58425241 /* 'XBRA' */
				dc.l      $56464958 /* 'VFIX' */
olddisp:
				dc.l      0

newdisp:
				move.l    a0,dispatcher-respart+cmdline
				movea.l   d1,a0
				movea.l   (a0),a0
				cmpi.w    #$0001,(a0) /* v_opnwk? */
				beq.s     callvdi
				cmpi.w    #$0064,(a0) /* v_opnvwk? */
				beq.s     callvdi
				movea.l   dispatcher(pc),a0
				move.l    olddisp(pc),-(a7)
				rts

callvdi:
				movea.l   olddisp(pc),a0
				jsr       (a0)
				movea.l   dispatcher(pc),a0
fixhandle:
				movem.l   d0/a0-a2,-(a7)
				movea.l   virt_work(pc),a0
				movea.l   64(a0),a1 /* get next_work ptr */
				move.l    a1,d0
				beq.s     nofix     /* if last, no need to fix */
				movea.l   64(a1),a2
fixloop:
				move.l    a2,d0
				beq.s     nofix
				move.w    40(a1),d0 /* get handle of current ws */
				cmp.w     40(a2),d0 /* compare to handle of next ws */
				bgt.s     fixswap
				movea.l   a1,a0
				movea.l   a2,a1
				movea.l   64(a2),a2
				bra.s     fixloop
fixswap:
				move.l    a2,64(a0)
				move.l    64(a2),64(a1)
				move.l    a1,64(a2)
nofix:
				movem.l   (a7)+,d0/a0-a2
				rts
resend:

instfix:
				move.w    #$FFFF,d0
				trap      #2
				move.l    d0,olddisp

				lea       respart(pc),a0
				lea       cmdline(pc),a1
				move.l    #((resend-respart+3)/4)-1,d0
copyres:
				move.l    (a0)+,(a1)+
				dbf       d0,copyres
				lea       oldtrap-respart+cmdline(pc),a0
				move.l    ($00000088).w,(a0)+
				move.l    a0,($00000088).w
				rts

start:
				lea.l     basepage(pc),a3
				move.l    a3,base
				movea.l   12(a3),a0
				adda.l    20(a3),a0
				adda.l    28(a3),a0
				adda.w    #$0100,a0
				move.l    a3,d0
				add.l     a0,d0
				and.b     #$FC,d0
				movea.l   d0,a7
				move.l    a0,-(a7)
				move.l    a3,-(a7)
				move.w    #$0000,-(a7)
				move.w    #$004A,-(a7) /* Mshrink */
				trap      #1
				lea.l     12(a7),a7
				bra.w     main

printstr:
				movem.l   a1-a2,-(a7)
				move.l    a0,-(a7)
				move.w    #$0009,-(a7) /* Cconws */
				trap      #1
				addq.l    #6,a7
				movem.l   (a7)+,a1-a2
				rts

main:
				lea.l     checkinst(pc),a0
				jsr       supexec(pc)
				move.l    virt_work,d0
				ble.s     fail
				clr.w     d1
				lea.l     resend,a0
				move.l    a0,d0
				sub.l     base,d0
				sub.l     #$0000008C,d0
				jsr       ptermres(pc)
				rts
fail:
				moveq.l   #-1,d0
				cmp.l     virt_work,d0
				beq.s     done
				lea.l     key_msg,a0
				jsr       printstr(pc)
				jsr       cnecin(pc)
				moveq.l   #-1,d0
				jsr       exit(pc)
				rts

done:
				clr.w     d0
				jsr       exit(pc)
				rts

checkinst:
				movem.l   d3/a2-a4,-(a7)
				lea.l     virt_work,a2
				clr.l     (a2)
				lea.l     key_msg,a3
				lea.l     cpry_msg-key_msg(a3),a0
				jsr       printstr(pc)
				movea.l   (_vblqueue).w,a4
				move.l    (a4),d0
				beq.s     novbl
				lea.l     notinst1-key_msg(a3),a0
				jsr       printstr(pc)
				bra       noinst
novbl:
				movea.l   (_sysbase).w,a4
				movea.l   8(a4),a4
				cmpi.w    #$0102,2(a4)
				beq.s     nobug
				cmpi.w    #$0301,2(a4)
				blt.s     check106
nobug:
				lea.l     notinst2-key_msg(a3),a0
				jsr       printstr(pc)
				lea.l     notinst3-key_msg(a3),a0
				jsr       printstr(pc)
				bra       noinst
check106:
				cmpi.w    #$0106,2(a4)
				bge.s     check100
				moveq.l   #84,d0 /* Vsize */
				move.w    d0,-(a7)
				jsr       xbios(pc)
				addq.w    #2,a7
				cmp.w     #$0201,d0
				blt.s     check100
				lea.l     notinst4-key_msg(a3),a0
				jsr       printstr(pc)
				lea.l     notinst5-key_msg(a3),a0
				jsr       printstr(pc)
				bra       skipinst
check100:
				cmpi.l    #$00000100,(a4)
				bne.s     checkdate
				move.l    #$0000578C,(a2)
				bra       foundtos
checkdate:
				cmpi.l    #$08081988,24(a4) /* Beta 1.4 ram-tos */
				beq.s     isbeta
				cmpi.l    #$05181988,24(a4) /* Developer 1.4 ram-tos */
				bne.s     checkd2
isbeta:
				move.l    #$00003A9C,(a2)
				bra.s     foundtos
checkd2:
				cmpi.l    #$04061989,24(a4) /* Rainbow ram-tos */
				beq.s     israin
				cmpi.l    #$02221989,24(a4) /* C-BIT rom-tos */
				bne.s     checkd3
israin:
				move.l    #$00003BAA,(a2)
				bra.s     foundtos
checkd3:
				cmpi.l    #$07291989,24(a4) /* STE 1.6 */
				bne.s     checkd4
				move.l    #$00003BEA,(a2)
				bra.s     foundtos
checkd4:
				cmpi.l    #$05231989,24(a4) /* TT TOS 030 */
				bne.s     checkd5
				move.l    #$000052D6,(a2)
				bra.s     foundtos
checkd5:
				cmpi.l    #$12051990,24(a4) /* MEGA STE 2.05 */
				bne.s     checkd6
				move.l    #$00003FFC,(a2)
				bra.s     foundtos
checkd6:
				lea.l     notinst6-key_msg(a3),a0
				jsr       printstr(pc)
				lea.l     notinst7-key_msg(a3),a0
				jsr       printstr(pc)
				bra.s     noinst
foundtos:
				movea.l   (_p_cookies).w,a4
				move.l    a4,d0
				beq.s     nojar
				clr.w     d3
				bra.s     nextjar
jarloop:
				move.w    d3,d0
				add.w     d0,d0
				lsl.w     #2,d0
				cmpi.l    #$4E564449,0(a4,d0.w) /* 'NVDI' */
				bne.s     searchjar
				lea.l     notinst8-key_msg(a3),a0
				jsr       printstr(pc)
skipinst:
				move.l    #-1,(a2)
				bra.s     noinst
searchjar:
				addq.w    #1,d3
nextjar:
				move.w    d3,d0
				add.w     d0,d0
				lsl.w     #2,d0
				move.l    0(a4,d0.w),d1
				bne.s     jarloop
				move.w    d3,d2
				ext.l     d2
				cmp.l     4(a4,d0.w),d2
				bge.s     nojar
				clr.l     8(a4,d0.w)
				move.w    d3,d1
				add.w     d1,d1
				lsl.w     #2,d1
				move.l    4(a4,d0.w),12(a4,d1.w)
				move.l    #$56464958,0(a4,d0.w)  /* 'VFIX' */
				move.l    #$0001000A,4(a4,d0.w)
nojar:
				jsr       instfix(pc)
				lea.l     installed-key_msg(a3),a0
				jsr       printstr(pc)
noinst:
				movem.l   (a7)+,d3/a2-a4
				rts

cnecin:
				move.w    #$0008,-(a7) /* Cnecin */
				trap      #1
				addq.w    #2,a7
				rts

exit:
				move.w    d0,-(a7)
				move.w    #$004C,-(a7) /* Pterm */
				trap      #1
				addq.w    #4,a7
				rts

ptermres:
				move.w    d1,-(a7)
				move.l    d0,-(a7)
				move.w    #$0031,-(a7) /* Ptermres */
				trap      #1

xbios:
				move.l    (a7)+,save_pc
				move.l    a2,save_a2
				trap      #14
				movea.l   save_a2,a2
				movea.l   save_pc,a0
				jmp       (a0)

supexec:
				move.l    a2,-(a7)
				move.l    a0,-(a7)
				move.w    #$0026,-(a7) /* Supexec */
				trap      #14
				addq.w    #6,a7
				movea.l   (a7)+,a2
				rts

				data

key_msg:
				dc.b $f5,'    Please press RETURN             ',$f5,10,13,10,13,0

cpry_msg:
				dc.b 10,13,$f4,' VDI-FIX 1.10  K.Isakovic, 30.01.91 ',$f4,10,13,0

notinst1:
				dc.b '| NOT installed.   Auto folder only !|',$0a,$0d,0
notinst2:
				dc.b '|      This TOS doesn',39,'t have the     |',$0a,$0d,0
notinst3:
				dc.b '|    ',39,'v_opnwk',39,'- & ',39,'v_opnvwk',39,'- Bug.   |',$0a,$0d,0
notinst4:
				dc.b '|  The ',39,'v_opnwk',39,'- & ',39,'v_opnvwk',39,'- Bug  |',$0a,$0d,0
notinst5:
				dc.b $f5,'   is already fixed by OVERSCAN !   ',$f5,10,13,10,13,0
notinst6:
				dc.b '|  This Version of TOS 1.0, 1.4, 1.6 |',$0a,$0d,0
notinst7:
				dc.b '|    2.0x or 3.00 is not supported.  |',$0a,$0d,0
notinst8:
				dc.b $f5,'    NVDI does not need this patch.  ',$f5,10,13,0
installed:
				dc.b $f5,' now installed.                     ',$f5,10,13,10,13,0

				bss

base:			ds.l 1
save_pc:		ds.l 1
save_a2:		ds.l 1
stack:			ds.b 256
