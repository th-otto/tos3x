/* Attribute controller: index and data write */
#define ATC_IW  $3C0
/* Misc Output Write Register */
#define MISC_W  $3C2
/* Video Subsystem register */
#define VIDSUB  $3C3
/* Timing Sequencer index and data ports */
#define TS_I    $3C4
#define TS_D    $3C5
/* RAMDAC pixel mask */
#define DAC_PEL $3C6
/* RAMDAC write index */
#define DAC_IW  $3C8
/* RAMDAC palette data */
#define DAC_D   $3C9
/* GDC segement select, index and data ports */
#define GDC_SEG $3CD
#define GDC_I   $3CE
#define GDC_D   $3CF
/* CRT Controller index and data ports */
#define CRTC_I  $3D4
#define CRTC_D  $3D5
/* Input Status Register 1: color emulation */
#define IS1_RC  $3DA


#define _v_bas_ad  $44e
#define _sshiftmod $44c
#define _hz_200 $4ba
#define _buserror 8

		dc.l $e007b6 /* XXX */
		dc.l $e00770

nova:
		move.l    a0,-(a7) /* XXX */
			bsr       initno
			tst.l     d0
			beq       nova2
			movea.l   d0,a0
			lea.l     ndevtab(pc),a1
			move.w    14(a1),d1 /* vertical res-1 */
			addq.w    #1,d1
			mulu.w    4(a1),d1  /* * bytes/line */
			lsr.l     #2,d1
			moveq.l   #0,d2
nova1:
			move.l    d2,(a0)+
			dbf       d1,nova1
			move.l    d0,(_v_bas_ad).w
			move.b    #2,(_sshiftmod).w
			dc.w      $a000 /* linea init */
			move.w    (_v_bas_ad).w,-34(a0) /* v_cur_ad ???? why move.w? */
			lea.l     ndevtab(pc),a1
			move.w    6(a1),(a0) /* v_planes */
			move.w    4(a1),d0
			move.w    d0,2(a0) /* v_lin_wr */
			move.w    d0,-2(a0) /* bytes_lin */
			lsl.w     #4,d0
			move.w    d0,-40(a0) /* v_cel_wr */
			move.w    14(a1),d0
			addq.w    #1,d0
			move.w    d0,-4(a0) /* v_vt_rez */
			lsr.w     #4,d0
			subq.w    #1,d0
			move.w    d0,-42(a0) /* v_celmy */
			move.w    12(a1),d0
			addq.w    #1,d0
			move.w    d0,-12(a0) /* v_hz_rez */
			lsr.w     #3,d0
			subq.w    #1,d0
			move.w    d0,-44(a0) /* v_celmx */
			movea.l   (_v_bas_ad).w,a1
			lea.l     novalogo(pc),a0
			moveq.l   #86-1,d0       /* 86 lines of data */
logonov1:
			moveq.l   #12-1,d1       /* 12 bytes of data per line */
logonov2:
			move.b    (a0)+,(a1)+
			dbf       d1,logonov2
			adda.w    #80-12,a1
			dbf       d0,logonov1
			addq.l    #4,a7
			/* bra       logodone XXX */
			move.l    nova-8(pc),-(a7) /* XXX */
			rts /* XXX */
nova2:
			movea.l   (_v_bas_ad).w,a1 /* XXX */
			move.b    ($0000044C).w,d0 /* XXX */
			movea.l   (a7)+,a0 /* XXX */
			move.l    nova-4(pc),-(a7) /* XXX */
			rts

initno:
			movem.l   d3-d7/a2-a6,-(a7)
			bsr       detnova
			tst.l     d0
			beq       initno_5
			move.l    d0,-(a7)
			movea.l   a0,a6
			move.w    $12ee(a6),d0
			cmp.w     #$FFFF,d0
			bne.s     initno_3
			/* Enables VGA mode and selects MCLK 1 */
			move.b    #$01,VIDSUB(a6)
			move.b    #$E3,MISC_W(a6)
			bsr       unlock_et4000
			move.b    #$34,CRTC_I(a6)
			move.b    #0,CRTC_D(a6)
			move.b    #$31,CRTC_I(a6)
			move.b    #0,CRTC_D(a6)
			bsr       x03FE
			bsr       x0372
			moveq.l   #0,d7
			move.l    (_hz_200).w,d6
			add.l     #200,d6
initno_1:
			bsr       nwaitvbl
			addq.w    #1,d7
			cmp.l     (_hz_200).w,d6
			bhi.s     initno_1
			cmp.w     #$003C,d7
			bhi.s     initno_2
			move.b    #$07,TS_I(a6)
			move.b    #$A4,TS_D(a6)
initno_2:
			bra.s     initno_4
initno_3:
			addi.l    #$000A0000,(a7)
			bsr.w     x0166
			bsr.w     x01A6
initno_4:
			move.l    (a7)+,d0
			movea.l   d0,a0
			move.b    #0,(a0)
			tst.b     (a0)
			bne.s     initno_6
			move.b    #$55,(a0)
			cmpi.b    #$55,(a0)
			bne.s     initno_6
			movem.l   (a7)+,d3-d7/a2-a6
			rts
initno_5:
			movem.l   (a7)+,d3-d7/a2-a6
			moveq.l   #0,d0
			rts
initno_6:
			sub.l     #$00100000,d0
			movem.l   (a7)+,d3-d7/a2-a6
			rts

x0166:
			move.b    #$A9,$36ee(a6)
			move.b    #$90,$36ef(a6)
			move.b    #$0E,$6aee(a6)
			move.b    #$00,$42ee(a6)
			move.w    #$0202,$5eee(a6)
			move.w    #$0040,$52ee(a6)
			move.w    #0,$56ee(a6)
			move.b    #$03,$4ae8(a6)
			move.w    #$0090,$42e8(a6)
			move.w    #$0050,$42e8(a6)
			bra       x0328

x01A6:
			move.w    #$5002,$4aee(a6)
			bsr       x0328
			move.b    #$10,$46e8(a6)
			move.b    #$01,$0102(a6)
			move.b    #$08,$46e8(a6)
			move.b    #$01,VIDSUB(a6)
			move.b    #$50,GDC_I(a6)
			move.b    #$CE,GDC_D(a6)
			move.b    #$51,GDC_I(a6)
			move.b    #$81,GDC_D(a6)
			bsr       x057A
			lea.l     ndevtab(pc),a1
			moveq.l   #4-1,d1
			moveq.l   #1,d0
			lea.l     et4K_ts-ndevtab(a1),a2
			bsr       set_multiple_idxreg
			move.b    et4k_MISC_W-ndevtab(a1),MISC_W(a6)
			lea.l     CRTC_I(a6),a0
			move.b    #$11,(a0)
			move.b    #0,CRTC_D-CRTC_I(a0)
			move.w    #25-1,d1
			moveq.l   #0,d0
			lea.l     o56-ndevtab(a1),a2
			bsr       set_multiple_idxreg
			lea.l     $01ce(a6),a0
			move.b    #$86,(a0)
			move.b    #$7A,1(a0)
			move.b    #$A3,(a0)
			move.b    #0,1(a0)
			move.b    #$AD,(a0)
			move.b    #0,1(a0)
			move.b    #$AE,(a0)
			move.b    #0,1(a0)
			move.b    #$B0,(a0)
			move.b    #8,1(a0)
			move.b    #$B1,(a0)
			move.b    #0,1(a0)
			move.b    #$B2,(a0)
			move.b    #0,1(a0)
			move.b    #$B3,(a0)
			move.b    #0,1(a0)
			move.b    #$B4,(a0)
			move.b    #0,1(a0)
			move.b    #$B5,(a0)
			move.b    #0,1(a0)
			move.b    #$B6,(a0)
			move.b    #1,1(a0)
			move.b    #$B8,(a0)
			move.b    #0,1(a0)
			move.b    #$BD,(a0)
			move.b    #4,1(a0)
			move.b    #$BE,(a0)
			move.b    #8,1(a0)
			move.b    #$BF,(a0)
			move.b    #1,1(a0)
			lea.l     GDC_I(a6),a0
			move.w    #9-1,d1
			moveq.l   #0,d0
			lea.l     et4K_crt-ndevtab(a1),a2
			bsr       set_multiple_idxreg
			move.b    IS1_RC(a6),d0
			lea.l     ATC_IW(a6),a0
			lea.l     et4k_atc-ndevtab(a1),a2
			moveq.l   #0,d1
			moveq.l   #21-1,d2
x01A6_1:
			move.b    d1,(a0)
			move.b    (a2)+,(a0)
			addq.b    #1,d1
			dbf       d2,x01A6_1
			move.b    #$20,(a0)
			move.b    #$00,TS_I(a6)
			move.b    #$01,TS_D(a6)
			move.b    #$B9,$01ce(a6)
			move.b    #$42,$01cf(a6)
			move.b    #$B8,$01ce(a6)
			move.b    #$40,$01cf(a6)
			move.b    #$BE,$01ce(a6)
			move.b    #$00,$01cf(a6)
			move.b    et4k_MISC_W-ndevtab(a1),MISC_W(a6)
			move.b    #0,TS_I(a6)
			move.b    #3,TS_D(a6)
			bsr.w     nsetpal
			rts

x0328:
			move.w    #$1A20,$7aee(a6)
			move.b    #$00,$02ed(a6)
			move.b    #$30,$02ea(a6)
			move.b    #$2D,$02eb(a6)
			moveq.l   #12,d0
			bsr.w     x035A
			move.w    #$1A40,$7aee(a6)
			move.w    #$0000,$62ee(a6)
			move.b    #$FF,$02ea(a6)
			rts

x035A:
			movea.l   a6,a0
			adda.l    #$00008000,a0
			move.b    $12ef(a0),d1
			and.b     #$F0,d1
			or.b      d0,d1
			move.b    d1,$7eef(a6)
			rts

x0372:
			bsr       x059E
			tst.w     d0
			beq.s     x0372_1
			moveq.l   #0,d0
			bsr       rdhicolor
x0372_1:
			lea.l     ndevtab(pc),a1
			bsr       x04B2
			bsr       x049c
			bsr.w     nsetpal
			bsr       x048C
			rts

nsetpal:
			move.b    #255,DAC_PEL(a6)  /* DAC pixel mask */
			/* Load colors 0 and 1 */
			move.b    #0,DAC_IW(a6)
			lea.l     DAC_D(a6),a0
			moveq.l   #1,d7
			lea.l     npalette(pc),a1
nsetpa1:
			move.b    (a1)+,d0
			move.b    (a1)+,d1
			move.b    (a1)+,d2
			bsr.s     nsetcol
			dbf       d7,nsetpa1
			/* Load color 255 */
			move.b    #255,DAC_IW(a6)
			move.b    (a1)+,d0
			move.b    (a1)+,d1
			move.b    (a1)+,d2
nsetcol:
			move.b    d0,(a0)
			nop
			nop
			move.b    d1,(a0)
			nop
			nop
			move.b    d2,(a0)
			rts

nunk:
			lea.l     DAC_IW(a6),a0
			move.b    #0,(a0)+
			move.w    #256*3-1,d1
nunk1:
			dc.w $6000 /* ??? thats a branch instruction */
			nop
			move.b    d0,(a0)
			dbf       d1,nunk1
			rts

/* Unlocks access to the extended registers of ET4000 */
unlock_et4000:
			move.b    #$03,$3BF(a6)
			bsr       nsdelay
			move.b    #$A0,$3D8(a6)
			rts

x03FE:
			lea.l     GDC_I(a6),a0
			move.b    #$02,TS_I-GDC_I(a0)
			move.b    #$0F,TS_D-GDC_I(a0)
			move.b    #$04,TS_I-GDC_I(a0)
			move.b    #$0C,TS_D-GDC_I(a0)
			move.b    #$01,(a0)
			move.b    #0,GDC_D-GDC_I(a0)
			move.b    #$03,(a0)
			move.b    #0,GDC_D-GDC_I(a0)
			move.b    #$06,(a0)
			move.b    #5,GDC_D-GDC_I(a0)
			move.b    #$08,(a0)
			move.b    #255,GDC_D-GDC_I(a0)
			move.b    #0,GDC_SEG-GDC_I(a0)
			addq.w    #CRTC_I-GDC_I,a0
			move.b    #$32,(a0)
			move.b    #$28,CRTC_D-CRTC_I(a0)
			move.b    #$36,(a0)
			move.b    #$F3,CRTC_D-CRTC_I(a0)
			move.b    #$37,(a0)
			move.b    #$0F,CRTC_D-CRTC_I(a0)
			moveq.l   #0,d0
			rts

x046C:
			move.b    IS1_RC(a6),d0
			lea.l     ATC_IW(a6),a0
			lea.l     et4k_atc-ndevtab(a1),a2
			moveq.l   #0,d1
			moveq.l   #23-1,d2
x046C_1:
			move.b    d1,(a0)
			move.b    (a2)+,(a0)
			addq.b    #1,d1
			dbf       d2,x046C_1
			move.b    #$20,(a0)
			rts

x048C:
			lea.l     TS_I(a6),a0
			move.b    #1,(a0)
			move.b    et4K_ts(pc),TS_D-TS_I(a0)
			rts

x049c:
			lea.l     TS_I(a6),a0
			move.b    #1,(a0)
			move.b    et4K_ts(pc),d0
			or.b      #$20,d0
			move.b    d0,TS_D-TS_I(a0)
			rts

x04B2:
			bsr       x057A
			bsr       unlock_et4000
			moveq.l   #4-1,d1
			moveq.l   #1,d0
			lea.l     et4K_ts-ndevtab(a1),a2
			bsr       set_multiple_idxreg
			moveq.l   #3-1,d1
			moveq.l   #6,d0
			bsr       set_multiple_idxreg
			move.b    et4k_MISC_W-ndevtab(a1),MISC_W(a6)
			lea.l     CRTC_I(a6),a0
			move.b    #$11,(a0)
			bsr       nsdelay
			move.b    #$00,CRTC_D-CRTC_I(a0)
			move.w    #25-1,d1
			moveq.l   #0,d0
			lea.l     o56-ndevtab(a1),a2
			bsr.s     set_multiple_idxreg
			bsr       nsdelay
			move.b    #$33,(a0)
			bsr       nsdelay
			move.b    (a2)+,CRTC_D-CRTC_I(a0)
			bsr       nsdelay
			move.b    #$34,(a0)
			bsr       nsdelay
			move.b    (a2)+,CRTC_D-CRTC_I(a0)
			bsr       nsdelay
			move.b    #$35,(a0)
			bsr       nsdelay
			move.b    (a2)+,CRTC_D-CRTC_I(a0)
			bsr       nsdelay
			move.b    #$11,(a0)
			bsr       nsdelay
			move.b    CRTC_D-CRTC_I(a0),d0
			bsr       nsdelay
			or.b      #$80,d0
			move.b    d0,CRTC_D-CRTC_I(a0)
			bsr       x046C
			lea.l     GDC_I(a6),a0
			move.w    #9-1,d1
			moveq.l   #0,d0
			lea.l     et4K_crt-ndevtab(a1),a2
			bsr.s     set_multiple_idxreg
			move.b    #$36,CRTC_I(a6)
			bsr       nsdelay
			move.b    #$53,CRTC_D(a6)
			rts

set_multiple_idxreg:
			bsr       nsdelay
			move.b    d0,(a0)
			bsr       nsdelay
			move.b    (a2)+,TS_D-TS_I(a0)
			addq.b    #1,d0
			dbf       d1,set_multiple_idxreg
			rts

x057A:
			lea.l     TS_I(a6),a0
			move.b    #$00,(a0)
			bsr.w     nsdelay
			move.b    #1,TS_D-TS_I(a0)
			bsr.w     nsdelay
			move.b    #0,(a0)
			nop
			move.b    #3,TS_D-TS_I(a0)
			rts

x059E:
			moveq.l   #0,d0
			bsr.w     rddac
			bsr.w     dac2cmd
			move.b    DAC_PEL(a6),d1
			not.b     d1
			bsr.w     rddac
			move.b    DAC_PEL(a6),d2
			move.b    d1,DAC_PEL(a6)
			bsr.w     rddac
			bsr.w     dac2cmd
			move.b    DAC_PEL(a6),d2
			cmp.b     d1,d2
			beq.s     x059E_1
			moveq.l   #1,d0
x059E_1:
			rts

dac2cmd:
			move.b    DAC_PEL(a6),d0
			bsr.w     nsdelay
			move.b    DAC_PEL(a6),d0
			bsr.w     nsdelay
			move.b    DAC_PEL(a6),d0
			bsr.w     nsdelay
			move.b    DAC_PEL(a6),d0
			bsr.w     nsdelay
			rts

rddac:
			move.b    DAC_IW(a6),d0
			rts

nsdelay:
			move.l    d0,-(a7)
			move.w    #10000,d0
nsdel1:
			nop
			dbf       d0,nsdel1
			move.l    (a7)+,d0
			rts

/* Switches HiColor RAMDAC to palette mode. */
rdhicolor:
			move.w    d0,-(a7)
			bsr.s     rddac
			bsr.s     dac2cmd
			move.w    (a7)+,d0
			move.b    d0,DAC_PEL(a6)
			bsr.s     rddac
			rts

/*
 * detect_nova:
 *   returns register base in a0, and memory base in d0
 */
detnova:
			move.l    (_buserror).w,d7
			movea.l   a7,a6
			lea.l     novabuserr(pc),a0
			move.l    a0,(_buserror).w
			lea.l     detnov1(pc),a1
			lea.l     $FE900000,a0
			move.l    #$FE800000,d0
			move.b    #1,VIDSUB(a0)
			bra.s     detnov4
detnov1:
			lea.l     detnov2(pc),a1
			lea.l     $00DC0000,a0
			move.l    #$00C00000,d0
			move.b    #1,VIDSUB(a0)
			bra.s     detnov4
detnov2:
			lea.l     detnov3(pc),a1
			lea.l     $00CC0000,a0 /* BUG: could be normal memory */
			move.b    #1,VIDSUB(a0)
			bra.s     detnov4
detnov3:
			lea.l     detnov5(pc),a1
			lea.l     $FEDC0000,a0
			move.l    #$FEC00000,d0
			move.b    #1,VIDSUB(a0)
detnov4:
			move.l    d7,(_buserror).w
			rts
detnov5:
			moveq.l   #0,d0
			move.l    d7,(_buserror).w
			rts
novabuserr:
			movea.l   a6,a7
			jmp       (a1)

nwaitvbl:
			moveq.l   #10,d1
nwaitv1:
			moveq.l   #-1,d2
nwaitv2:
			move.b    IS1_RC(a6),d0
			and.w     #8,d0
			dbne      d2,nwaitv2
			dbne      d1,nwaitv1
			moveq.l   #10,d1
nwaitv3:
			moveq.l   #-1,d2
nwaitv4:
			move.b    IS1_RC(a6),d0
			and.w     #8,d0
			dbeq      d2,nwaitv4
			dbeq      d1,nwaitv3
			rts

npalette:
			dc.b $ff,$ff,$ff
			dc.b $00,$00,$00
			dc.b $00,$00,$00
			dc.b 0 /* XXX even */

ndevtab:
            dc.w 125
            dc.w 1
            dc.w 80 /* bytes/line */
            dc.w 1  /* planes */
            dc.w 2
            dc.w 1
            dc.w 640-1       /* horizontal resolution */
            dc.w 400-1       /* vertical resolution */

/* Attribute Controller: registers 0 - 0x16 */
et4k_atc:
			dc.b $00,$01,$02,$03,$04,$05,$06,$07
			dc.b $08,$09,$0a,$0b,$0c,$0d,$0e,$0f
			dc.b $01,$ff,$01,$10,$00
            dc.b $05,$00

/* Misc Output Write Register */
et4k_MISC_W:
            dc.b $63

/* Timing Sequencer registers 1...8 */
et4K_ts:
            dc.b $01
            dc.b $01
            dc.b $00
            dc.b $06
            dc.b $00
            dc.b $f4
			dc.b $03

/* CRT Controller: registers 0 - 0x18 */
et4K_crt:
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $01
			dc.b $0f
			dc.b $ff
o56:
			dc.b $5f
			dc.b $4f
			dc.b $50
			dc.b $82
			dc.b $54
			dc.b $80
			dc.b $bf
			dc.b $1f
			dc.b $00
			dc.b $40
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $00
			dc.b $9c
			dc.b $0e
			dc.b $8f
			dc.b $28
			dc.b $00
			dc.b $e7
			dc.b $04
			dc.b $c3
			dc.b $ff
			dc.b $00
			dc.b $00
			dc.b $00

novalogo:
#include "../tospatch/fuji/nova.S"

 dc.l 0 /* XXX */
