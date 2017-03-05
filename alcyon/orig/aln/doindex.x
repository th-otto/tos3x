; ph_branch = 0x601a
; ph_tlen = 0x000035d4
; ph_dlen = 0x000008e8
; ph_blen = 0x000023c0
; ph_slen = 0x00000000
; ph_res1 = 0x00000000
; ph_prgflags = 0x00000000
; ph_absflag = 0x0000
; first relocation = 0x00000008
; relocation bytes = 0x00000221

[00010000] 224f                      movea.l   a7,a1
[00010002] 206f 0004                 movea.l   4(a7),a0
[00010006] 23c8 0001 3ebc            move.l    a0,__base
[0001000c] 2028 0018                 move.l    24(a0),d0
[00010010] d0a8 001c                 add.l     28(a0),d0
[00010014] 23c0 0001 3ec0            move.l    d0,__break
[0001001a] 223c ffff e000            move.l    #$FFFFE000,d1
[00010020] 6b00 0008                 bmi       $0001002A
[00010024] d280                      add.l     d0,d1
[00010026] 6000 0006                 bra       $0001002E
[0001002a] d2a8 0004                 add.l     4(a0),d1
[0001002e] 2e41                      movea.l   d1,a7
[00010030] 2200                      move.l    d0,d1
[00010032] d2bc 0000 1000            add.l     #$00001000,d1
[00010038] bfc1                      cmpa.l    d1,a7
[0001003a] 6200 0004                 bhi       $00010040
[0001003e] 2e41                      movea.l   d1,a7
[00010040] 2428 0004                 move.l    4(a0),d2
[00010044] 94bc 0000 0200            sub.l     #$00000200,d2
[0001004a] bfc2                      cmpa.l    d2,a7
[0001004c] 6500 000a                 bcs       $00010058
[00010050] 2e42                      movea.l   d2,a7
[00010052] b481                      cmp.l     d1,d2
[00010054] 6500 0042                 bcs       abort
[00010058] 220f                      move.l    a7,d1
[0001005a] c2bc ffff fffe            and.l     #$FFFFFFFE,d1
[00010060] 2e41                      movea.l   d1,a7
[00010062] 9288                      sub.l     a0,d1
[00010064] 2f01                      move.l    d1,-(a7)
[00010066] 2f08                      move.l    a0,-(a7)
[00010068] 4267                      clr.w     -(a7)
[0001006a] 3f3c 004a                 move.w    #$004A,-(a7)
[0001006e] 4e41                      trap      #1
[00010070] dffc 0000 000c            adda.l    #$0000000C,a7
[00010076] 2079 0001 3ebc            movea.l   __base,a0
[0001007c] 45e8 0080                 lea.l     128(a0),a2
[00010080] 101a                      move.b    (a2)+,d0
[00010082] 4880                      ext.w     d0
[00010084] 3f00                      move.w    d0,-(a7)
[00010086] 2f0a                      move.l    a2,-(a7)
[00010088] 9dce                      suba.l    a6,a6
[0001008a] 4eb9 0001 14c2            jsr       __main
__exit:
[00010090] 4a9f                      tst.l     (a7)+
[00010092] 3f3c 004c                 move.w    #$004C,-(a7)
[00010096] 4e41                      trap      #1
abort:
[00010098] 2e49                      movea.l   a1,a7
[0001009a] 4879 0001 35f8            pea.l     $000135F8
[000100a0] 3f3c 0009                 move.w    #$0009,-(a7)
[000100a4] 4e41                      trap      #1
[000100a6] 5c8f                      addq.l    #6,a7
[000100a8] 3f3c ffd9                 move.w    #$FFD9,-(a7)
[000100ac] 4eb9 0001 0090            jsr       __exit
_brk:		
[000100b2] bff9 0001 3ec0            cmpa.l    __break,a7
[000100b8] 6500 003e                 bcs       $000100F8
[000100bc] 206f 0004                 movea.l   4(a7),a0
[000100c0] 2008                      move.l    a0,d0
[000100c2] d1fc 0000 0100            adda.l    #$00000100,a0
[000100c8] bfc8                      cmpa.l    a0,a7
[000100ca] 6500 000c                 bcs       $000100D8
[000100ce] 23c0 0001 3ec0            move.l    d0,__break
[000100d4] 4280                      clr.l     d0
[000100d6] 4e75                      rts
[000100d8] 70ff                      moveq.l   #-1,d0
[000100da] 4e75                      rts
___BDOS:
[000100dc] 4e56 0000                 link      a6,#$0000
[000100e0] 302f 0008                 move.w    8(a7),d0
[000100e4] 222f 000a                 move.l    10(a7),d1
[000100e8] 4e42                      trap      #2
[000100ea] bff9 0001 3ec0            cmpa.l    __break,a7
[000100f0] 6500 0006                 bcs       $000100F8
[000100f4] 4e5e                      unlk      a6
[000100f6] 4e75                      rts
[000100f8] 2f3c 0001 35d4            move.l    #$000135D4,-(a7)
[000100fe] 3f3c 0009                 move.w    #$0009,-(a7)
[00010102] 4e41                      trap      #1
[00010104] 3f3c 0001                 move.w    #$0001,-(a7)
[00010108] 303c 004c                 move.w    #$004C,d0
[0001010c] 4e41                      trap      #1
_blkfill:
[0001010e] 206f 0004                 movea.l   4(a7),a0
[00010112] 322f 0008                 move.w    8(a7),d1
[00010116] 302f 000a                 move.w    10(a7),d0
[0001011a] 48c0                      ext.l     d0
[0001011c] 5380                      subq.l    #1,d0
[0001011e] 6f00 0008                 ble       $00010128
[00010122] 10c1                      move.b    d1,(a0)+
[00010124] 51c8 fffc                 dbf       d0,$00010122
[00010128] 4280                      clr.l     d0
[0001012a] 4e75                      rts
_index:
_strchr:
[0001012c] 206f 0004                 movea.l   4(a7),a0
[00010130] 302f 0008                 move.w    8(a7),d0
[00010134] 4a10                      tst.b     (a0)
[00010136] 6600 0006                 bne       $0001013E
[0001013a] 4280                      clr.l     d0
[0001013c] 4e75                      rts
[0001013e] b018                      cmp.b     (a0)+,d0
[00010140] 66f2                      bne.s     $00010134
[00010142] 2008                      move.l    a0,d0
[00010144] 5380                      subq.l    #1,d0
[00010146] 4e75                      rts
_crystal:
[00010148] 222f 0004                 move.l    4(a7),d1
[0001014c] 303c 00c8                 move.w    #$00C8,d0
[00010150] 4e42                      trap      #2
[00010152] 4e75                      rts

[00010154] 4e56 fffc                 link      a6,#$FFFC
[00010158] 48e7 011c                 movem.l   d7/a3-a5,-(a7)
[0001015c] 9bcd                      suba.l    a5,a5
[0001015e] 97cb                      suba.l    a3,a3
[00010160] 4279 0001 3ec6            clr.w     $00013EC6
[00010166] 4279 0001 3ec8            clr.w     $00013EC8
[0001016c] 3eae 0008                 move.w    8(a6),(a7)
[00010170] 6100 0104                 bsr       $00010276
[00010174] 2840                      movea.l   d0,a4
[00010176] 200c                      move.l    a4,d0
[00010178] 6716                      beq.s     $00010190
[0001017a] 5279 0001 3ec8            addq.w    #1,$00013EC8
[00010180] 200d                      move.l    a5,d0
[00010182] 6604                      bne.s     $00010188
[00010184] 2a4c                      movea.l   a4,a5
[00010186] 6004                      bra.s     $0001018C
[00010188] 274c 0010                 move.l    a4,16(a3)
[0001018c] 264c                      movea.l   a4,a3
[0001018e] 60dc                      bra.s     $0001016C
[00010190] 4a6e 000a                 tst.w     10(a6)
[00010194] 6706                      beq.s     $0001019C
[00010196] 2e8d                      move.l    a5,(a7)
[00010198] 6100 0746                 bsr       $000108E0
[0001019c] 3039 0001 3ec8            move.w    $00013EC8,d0
[000101a2] 5e40                      addq.w    #7,d0
[000101a4] e640                      asr.w     #3,d0
[000101a6] 33c0 0001 3eca            move.w    d0,$00013ECA
[000101ac] 3039 0001 3ec8            move.w    $00013EC8,d0
[000101b2] c1fc 0014                 muls.w    #$0014,d0
[000101b6] 3239 0001 3ec6            move.w    $00013EC6,d1
[000101bc] c3fc 000c                 muls.w    #$000C,d1
[000101c0] d041                      add.w     d1,d0
[000101c2] 3239 0001 3ec8            move.w    $00013EC8,d1
[000101c8] c3f9 0001 3eca            muls.w    $00013ECA,d1
[000101ce] d041                      add.w     d1,d0
[000101d0] 5c40                      addq.w    #6,d0
[000101d2] 48c0                      ext.l     d0
[000101d4] 2e80                      move.l    d0,(a7)
[000101d6] 2f3c 0000 0001            move.l    #$00000001,-(a7)
[000101dc] 4eb9 0001 132a            jsr       $0001132A
[000101e2] 588f                      addq.l    #4,a7
[000101e4] 2d40 fffc                 move.l    d0,-4(a6)
[000101e8] 660a                      bne.s     $000101F4
[000101ea] 2ebc 0001 3776            move.l    #$00013776,(a7)
[000101f0] 6100 09f2                 bsr       $00010BE4
[000101f4] 4240                      clr.w     d0
[000101f6] 2eae fffc                 move.l    -4(a6),(a7)
[000101fa] 6100 061c                 bsr       $00010818
[000101fe] 4240                      clr.w     d0
[00010200] 202e fffc                 move.l    -4(a6),d0
[00010204] 5c80                      addq.l    #6,d0
[00010206] 23c0 0001 3ed0            move.l    d0,$00013ED0
[0001020c] 2e8d                      move.l    a5,(a7)
[0001020e] 2f2e fffc                 move.l    -4(a6),-(a7)
[00010212] 5c97                      addq.l    #6,(a7)
[00010214] 6100 062c                 bsr       $00010842
[00010218] 588f                      addq.l    #4,a7
[0001021a] 3039 0001 3ec8            move.w    $00013EC8,d0
[00010220] c1fc 0014                 muls.w    #$0014,d0
[00010224] d0ae fffc                 add.l     -4(a6),d0
[00010228] 5c80                      addq.l    #6,d0
[0001022a] 23c0 0001 3ecc            move.l    d0,$00013ECC
[00010230] 4240                      clr.w     d0
[00010232] 2e8d                      move.l    a5,(a7)
[00010234] 6100 0244                 bsr       $0001047A
[00010238] 4240                      clr.w     d0
[0001023a] 3039 0001 3ec8            move.w    $00013EC8,d0
[00010240] c1fc 0014                 muls.w    #$0014,d0
[00010244] 3239 0001 3ec6            move.w    $00013EC6,d1
[0001024a] c3fc 000c                 muls.w    #$000C,d1
[0001024e] d081                      add.l     d1,d0
[00010250] d0ae fffc                 add.l     -4(a6),d0
[00010254] 2e80                      move.l    d0,(a7)
[00010256] 5c97                      addq.l    #6,(a7)
[00010258] 2f0d                      move.l    a5,-(a7)
[0001025a] 6100 0394                 bsr       $000105F0
[0001025e] 588f                      addq.l    #4,a7
[00010260] 2e8d                      move.l    a5,(a7)
[00010262] 6100 061a                 bsr       $0001087E
[00010266] 4240                      clr.w     d0
[00010268] 202e fffc                 move.l    -4(a6),d0
[0001026c] 4a9f                      tst.l     (a7)+
[0001026e] 4cdf 3800                 movem.l   (a7)+,a3-a5
[00010272] 4e5e                      unlk      a6
[00010274] 4e75                      rts

[00010276] 4e56 ffe0                 link      a6,#$FFE0
[0001027a] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001027e] 3e2e 0008                 move.w    8(a6),d7
[00010282] 3ebc 0001                 move.w    #$0001,(a7)
[00010286] 3f07                      move.w    d7,-(a7)
[00010288] 42a7                      clr.l     -(a7)
[0001028a] 3f3c 0042                 move.w    #$0042,-(a7)
[0001028e] 4eb9 0001 14b2            jsr       _gemdos
[00010294] 508f                      addq.l    #8,a7
[00010296] 2d40 ffe0                 move.l    d0,-32(a6)
[0001029a] 4240                      clr.w     d0
[0001029c] 2e8e                      move.l    a6,(a7)
[0001029e] 0697 ffff ffe4            addi.l    #$FFFFFFE4,(a7)
[000102a4] 3f07                      move.w    d7,-(a7)
[000102a6] 4eb9 0001 0f30            jsr       $00010F30
[000102ac] 548f                      addq.l    #2,a7
[000102ae] 4a40                      tst.w     d0
[000102b0] 6706                      beq.s     $000102B8
[000102b2] 4280                      clr.l     d0
[000102b4] 6000 00aa                 bra       $00010360
[000102b8] 2ebc 0000 0020            move.l    #$00000020,(a7)
[000102be] 4eb9 0001 1026            jsr       $00011026
[000102c4] 2a40                      movea.l   d0,a5
[000102c6] 200d                      move.l    a5,d0
[000102c8] 660a                      bne.s     $000102D4
[000102ca] 2ebc 0001 3784            move.l    #$00013784,(a7)
[000102d0] 6100 0912                 bsr       $00010BE4
[000102d4] 3ebc 000e                 move.w    #$000E,(a7)
[000102d8] 2f0e                      move.l    a6,-(a7)
[000102da] 0697 ffff ffe4            addi.l    #$FFFFFFE4,(a7)
[000102e0] 2f0d                      move.l    a5,-(a7)
[000102e2] 4eb9 0001 1b2c            jsr       _strncpy
[000102e8] 508f                      addq.l    #8,a7
[000102ea] 2b6e ffe0 0014            move.l    -32(a6),20(a5)
[000102f0] 42ad 0010                 clr.l     16(a5)
[000102f4] 42ad 0018                 clr.l     24(a5)
[000102f8] 42ad 001c                 clr.l     28(a5)
[000102fc] 2eae fffa                 move.l    -6(a6),(a7)
[00010300] 4eb9 0001 1026            jsr       $00011026
[00010306] 2840                      movea.l   d0,a4
[00010308] 200c                      move.l    a4,d0
[0001030a] 6616                      bne.s     $00010322
[0001030c] 2ebc 0001 3792            move.l    #$00013792,(a7)
[00010312] 4eb9 0001 3592            jsr       _printf
[00010318] 3ebc ffff                 move.w    #$FFFF,(a7)
[0001031c] 4eb9 0001 3352            jsr       _exit
[00010322] 2e8c                      move.l    a4,(a7)
[00010324] 2f2e fffa                 move.l    -6(a6),-(a7)
[00010328] 3f07                      move.w    d7,-(a7)
[0001032a] 3f3c 003f                 move.w    #$003F,-(a7)
[0001032e] 4eb9 0001 14b2            jsr       _gemdos
[00010334] 508f                      addq.l    #8,a7
[00010336] b0ae fffa                 cmp.l     -6(a6),d0
[0001033a] 670a                      beq.s     $00010346
[0001033c] 2ebc 0001 37a1            move.l    #$000137A1,(a7)
[00010342] 6100 08a0                 bsr       $00010BE4
[00010346] 2e8c                      move.l    a4,(a7)
[00010348] 2f0d                      move.l    a5,-(a7)
[0001034a] 611e                      bsr.s     $0001036A
[0001034c] 588f                      addq.l    #4,a7
[0001034e] 4a40                      tst.w     d0
[00010350] 6704                      beq.s     $00010356
[00010352] 4280                      clr.l     d0
[00010354] 600a                      bra.s     $00010360
[00010356] 2e8c                      move.l    a4,(a7)
[00010358] 4eb9 0001 11a0            jsr       $000111A0
[0001035e] 200d                      move.l    a5,d0
[00010360] 4a9f                      tst.l     (a7)+
[00010362] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00010366] 4e5e                      unlk      a6
[00010368] 4e75                      rts

[0001036a] 4e56 fffa                 link      a6,#$FFFA
[0001036e] 48e7 0f1c                 movem.l   d4-d7/a3-a5,-(a7)
[00010372] 2a6e 0008                 movea.l   8(a6),a5
[00010376] 286e 000c                 movea.l   12(a6),a4
[0001037a] 2c2c 0002                 move.l    2(a4),d6
[0001037e] dcac 0006                 add.l     6(a4),d6
[00010382] dcbc 0000 001c            add.l     #$0000001C,d6
[00010388] 202c 000e                 move.l    14(a4),d0
[0001038c] 81fc 000e                 divs.w    #$000E,d0
[00010390] 48c0                      ext.l     d0
[00010392] 3e00                      move.w    d0,d7
[00010394] d9c6                      adda.l    d6,a4
[00010396] 3a07                      move.w    d7,d5
[00010398] 6000 00ce                 bra       $00010468
[0001039c] 302c 0008                 move.w    8(a4),d0
[000103a0] c07c 2800                 and.w     #$2800,d0
[000103a4] 3d40 fffe                 move.w    d0,-2(a6)
[000103a8] 2d6c 000a fffa            move.l    10(a4),-6(a6)
[000103ae] 0c6e 2000 fffe            cmpi.w    #$2000,-2(a6)
[000103b4] 670e                      beq.s     $000103C4
[000103b6] 0c6e 2800 fffe            cmpi.w    #$2800,-2(a6)
[000103bc] 664e                      bne.s     $0001040C
[000103be] 4aae fffa                 tst.l     -6(a6)
[000103c2] 6748                      beq.s     $0001040C
[000103c4] 4240                      clr.w     d0
[000103c6] 5279 0001 3ec6            addq.w    #1,$00013EC6
[000103cc] 266d 001c                 movea.l   28(a5),a3
[000103d0] 2ebc 0000 000e            move.l    #$0000000E,(a7)
[000103d6] 4eb9 0001 1026            jsr       $00011026
[000103dc] 2b40 001c                 move.l    d0,28(a5)
[000103e0] 6612                      bne.s     $000103F4
[000103e2] 2ebc 0001 37bf            move.l    #$000137BF,(a7)
[000103e8] 4eb9 0001 3592            jsr       _printf
[000103ee] 7001                      moveq.l   #1,d0
[000103f0] 6000 007e                 bra       $00010470
[000103f4] 2e8c                      move.l    a4,(a7)
[000103f6] 2f2d 001c                 move.l    28(a5),-(a7)
[000103fa] 4eb9 0001 0fb0            jsr       $00010FB0
[00010400] 588f                      addq.l    #4,a7
[00010402] 206d 001c                 movea.l   28(a5),a0
[00010406] 214b 000a                 move.l    a3,10(a0)
[0001040a] 6054                      bra.s     $00010460
[0001040c] 0c6e 0800 fffe            cmpi.w    #$0800,-2(a6)
[00010412] 670e                      beq.s     $00010422
[00010414] 0c6e 2800 fffe            cmpi.w    #$2800,-2(a6)
[0001041a] 6644                      bne.s     $00010460
[0001041c] 4aae fffa                 tst.l     -6(a6)
[00010420] 663e                      bne.s     $00010460
[00010422] 4240                      clr.w     d0
[00010424] 266d 0018                 movea.l   24(a5),a3
[00010428] 2ebc 0000 000e            move.l    #$0000000E,(a7)
[0001042e] 4eb9 0001 1026            jsr       $00011026
[00010434] 2b40 0018                 move.l    d0,24(a5)
[00010438] 6610                      bne.s     $0001044A
[0001043a] 2ebc 0001 37ce            move.l    #$000137CE,(a7)
[00010440] 4eb9 0001 3592            jsr       _printf
[00010446] 7001                      moveq.l   #1,d0
[00010448] 6026                      bra.s     $00010470
[0001044a] 2e8c                      move.l    a4,(a7)
[0001044c] 2f2d 0018                 move.l    24(a5),-(a7)
[00010450] 4eb9 0001 0fb0            jsr       $00010FB0
[00010456] 588f                      addq.l    #4,a7
[00010458] 206d 0018                 movea.l   24(a5),a0
[0001045c] 214b 000a                 move.l    a3,10(a0)
[00010460] d9fc 0000 000e            adda.l    #$0000000E,a4
[00010466] 5345                      subq.w    #1,d5
[00010468] 4a45                      tst.w     d5
[0001046a] 6600 ff30                 bne       $0001039C
[0001046e] 4240                      clr.w     d0
[00010470] 4a9f                      tst.l     (a7)+
[00010472] 4cdf 38e0                 movem.l   (a7)+,d5-d7/a3-a5
[00010476] 4e5e                      unlk      a6
[00010478] 4e75                      rts

[0001047a] 4e56 0000                 link      a6,#$0000
[0001047e] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[00010482] 2a6e 0008                 movea.l   8(a6),a5
[00010486] 4247                      clr.w     d7
[00010488] 2879 0001 3ecc            movea.l   $00013ECC,a4
[0001048e] 602e                      bra.s     $000104BE
[00010490] 4240                      clr.w     d0
[00010492] 266d 001c                 movea.l   28(a5),a3
[00010496] 601c                      bra.s     $000104B4
[00010498] 4240                      clr.w     d0
[0001049a] 2e8b                      move.l    a3,(a7)
[0001049c] 2f0c                      move.l    a4,-(a7)
[0001049e] 4eb9 0001 0fb0            jsr       $00010FB0
[000104a4] 588f                      addq.l    #4,a7
[000104a6] 3947 000a                 move.w    d7,10(a4)
[000104aa] d9fc 0000 000c            adda.l    #$0000000C,a4
[000104b0] 266b 000a                 movea.l   10(a3),a3
[000104b4] 200b                      move.l    a3,d0
[000104b6] 66e0                      bne.s     $00010498
[000104b8] 5247                      addq.w    #1,d7
[000104ba] 2a6d 0010                 movea.l   16(a5),a5
[000104be] 200d                      move.l    a5,d0
[000104c0] 66ce                      bne.s     $00010490
[000104c2] 2ebc 0001 1b74            move.l    #_strcmp,(a7)
[000104c8] 3f3c 000c                 move.w    #$000C,-(a7)
[000104cc] 3f39 0001 3ec6            move.w    $00013EC6,-(a7)
[000104d2] 2f39 0001 3ecc            move.l    $00013ECC,-(a7)
[000104d8] 4eb9 0001 33c6            jsr       _qsort
[000104de] 508f                      addq.l    #8,a7
[000104e0] 610a                      bsr.s     $000104EC
[000104e2] 4a9f                      tst.l     (a7)+
[000104e4] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[000104e8] 4e5e                      unlk      a6
[000104ea] 4e75                      rts

[000104ec] 4e56 0000                 link      a6,#$0000
[000104f0] 48e7 071c                 movem.l   d5-d7/a3-a5,-(a7)
[000104f4] 4246                      clr.w     d6
[000104f6] 3039 0001 3ec6            move.w    $00013EC6,d0
[000104fc] c1fc 000c                 muls.w    #$000C,d0
[00010500] 2a40                      movea.l   d0,a5
[00010502] dbf9 0001 3ecc            adda.l    $00013ECC,a5
[00010508] 2879 0001 3ecc            movea.l   $00013ECC,a4
[0001050e] 6000 00d0                 bra       $000105E0
[00010512] 3e2c 000a                 move.w    10(a4),d7
[00010516] 47ec 000c                 lea.l     12(a4),a3
[0001051a] 4246                      clr.w     d6
[0001051c] 6000 0078                 bra       $00010596
[00010520] 4a79 0001 3772            tst.w     $00013772
[00010526] 6758                      beq.s     $00010580
[00010528] 4a46                      tst.w     d6
[0001052a] 6636                      bne.s     $00010562
[0001052c] 302b 000a                 move.w    10(a3),d0
[00010530] c1fc 0014                 muls.w    #$0014,d0
[00010534] d0b9 0001 3ed0            add.l     $00013ED0,d0
[0001053a] 2e80                      move.l    d0,(a7)
[0001053c] 3007                      move.w    d7,d0
[0001053e] c1fc 0014                 muls.w    #$0014,d0
[00010542] d0b9 0001 3ed0            add.l     $00013ED0,d0
[00010548] 2f00                      move.l    d0,-(a7)
[0001054a] 2f0c                      move.l    a4,-(a7)
[0001054c] 2f3c 0001 37dd            move.l    #$000137DD,-(a7)
[00010552] 4eb9 0001 3592            jsr       _printf
[00010558] dffc 0000 000c            adda.l    #$0000000C,a7
[0001055e] 7c01                      moveq.l   #1,d6
[00010560] 601e                      bra.s     $00010580
[00010562] 302b 000a                 move.w    10(a3),d0
[00010566] c1fc 0014                 muls.w    #$0014,d0
[0001056a] d0b9 0001 3ed0            add.l     $00013ED0,d0
[00010570] 2e80                      move.l    d0,(a7)
[00010572] 2f3c 0001 37ff            move.l    #$000137FF,-(a7)
[00010578] 4eb9 0001 3592            jsr       _printf
[0001057e] 588f                      addq.l    #4,a7
[00010580] be6b 000a                 cmp.w     10(a3),d7
[00010584] 6f04                      ble.s     $0001058A
[00010586] 3007                      move.w    d7,d0
[00010588] 6004                      bra.s     $0001058E
[0001058a] 302b 000a                 move.w    10(a3),d0
[0001058e] 3e00                      move.w    d0,d7
[00010590] d7fc 0000 000c            adda.l    #$0000000C,a3
[00010596] 2e8b                      move.l    a3,(a7)
[00010598] 2f0c                      move.l    a4,-(a7)
[0001059a] 4eb9 0001 1b74            jsr       _strcmp
[000105a0] 588f                      addq.l    #4,a7
[000105a2] 4a40                      tst.w     d0
[000105a4] 6700 ff7a                 beq       $00010520
[000105a8] 4a46                      tst.w     d6
[000105aa] 6724                      beq.s     $000105D0
[000105ac] 4a79 0001 3772            tst.w     $00013772
[000105b2] 671c                      beq.s     $000105D0
[000105b4] 3007                      move.w    d7,d0
[000105b6] c1fc 0014                 muls.w    #$0014,d0
[000105ba] d0b9 0001 3ed0            add.l     $00013ED0,d0
[000105c0] 2e80                      move.l    d0,(a7)
[000105c2] 2f3c 0001 3807            move.l    #$00013807,-(a7)
[000105c8] 4eb9 0001 3592            jsr       _printf
[000105ce] 588f                      addq.l    #4,a7
[000105d0] 600a                      bra.s     $000105DC
[000105d2] 3947 000a                 move.w    d7,10(a4)
[000105d6] d9fc 0000 000c            adda.l    #$0000000C,a4
[000105dc] b9cb                      cmpa.l    a3,a4
[000105de] 66f2                      bne.s     $000105D2
[000105e0] b9cd                      cmpa.l    a5,a4
[000105e2] 6500 ff2e                 bcs       $00010512
[000105e6] 4a9f                      tst.l     (a7)+
[000105e8] 4cdf 38c0                 movem.l   (a7)+,d6-d7/a3-a5
[000105ec] 4e5e                      unlk      a6
[000105ee] 4e75                      rts

[000105f0] 4e56 fff8                 link      a6,#$FFF8
[000105f4] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[000105f8] 2a6e 0008                 movea.l   8(a6),a5
[000105fc] 2d6e 000c fffc            move.l    12(a6),-4(a6)
[00010602] 426e fffa                 clr.w     -6(a6)
[00010606] 3d7c 0001 fff8            move.w    #$0001,-8(a6)
[0001060c] 264d                      movea.l   a5,a3
[0001060e] 6066                      bra.s     $00010676
[00010610] 4240                      clr.w     d0
[00010612] 286b 0018                 movea.l   24(a3),a4
[00010616] 604a                      bra.s     $00010662
[00010618] 4240                      clr.w     d0
[0001061a] 3eb9 0001 3ec6            move.w    $00013EC6,(a7)
[00010620] 2f39 0001 3ecc            move.l    $00013ECC,-(a7)
[00010626] 2f0c                      move.l    a4,-(a7)
[00010628] 4eb9 0001 0ecc            jsr       $00010ECC
[0001062e] 508f                      addq.l    #8,a7
[00010630] 3e00                      move.w    d0,d7
[00010632] be7c ffff                 cmp.w     #$FFFF,d7
[00010636] 6604                      bne.s     $0001063C
[00010638] 4240                      clr.w     d0
[0001063a] 6022                      bra.s     $0001065E
[0001063c] 4240                      clr.w     d0
[0001063e] 303c 0080                 move.w    #$0080,d0
[00010642] 3207                      move.w    d7,d1
[00010644] c27c 0007                 and.w     #$0007,d1
[00010648] e260                      asr.w     d1,d0
[0001064a] 3f00                      move.w    d0,-(a7)
[0001064c] 3007                      move.w    d7,d0
[0001064e] e640                      asr.w     #3,d0
[00010650] 3040                      movea.w   d0,a0
[00010652] d1ee fffc                 adda.l    -4(a6),a0
[00010656] 1010                      move.b    (a0),d0
[00010658] 4880                      ext.w     d0
[0001065a] 805f                      or.w      (a7)+,d0
[0001065c] 1080                      move.b    d0,(a0)
[0001065e] 286c 000a                 movea.l   10(a4),a4
[00010662] 200c                      move.l    a4,d0
[00010664] 66b2                      bne.s     $00010618
[00010666] 3039 0001 3eca            move.w    $00013ECA,d0
[0001066c] 48c0                      ext.l     d0
[0001066e] d1ae fffc                 add.l     d0,-4(a6)
[00010672] 266b 0010                 movea.l   16(a3),a3
[00010676] 200b                      move.l    a3,d0
[00010678] 6696                      bne.s     $00010610
[0001067a] 2eae 000c                 move.l    12(a6),(a7)
[0001067e] 3f39 0001 3ec8            move.w    $00013EC8,-(a7)
[00010684] 610c                      bsr.s     $00010692
[00010686] 548f                      addq.l    #2,a7
[00010688] 4a9f                      tst.l     (a7)+
[0001068a] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[0001068e] 4e5e                      unlk      a6
[00010690] 4e75                      rts

[00010692] 4e56 0000                 link      a6,#$0000
[00010696] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[0001069a] 3e2e 0008                 move.w    8(a6),d7
[0001069e] 2a6e 000a                 movea.l   10(a6),a5
[000106a2] 23cd 0001 3ed6            move.l    a5,$00013ED6
[000106a8] 33c7 0001 3ec8            move.w    d7,$00013EC8
[000106ae] 3007                      move.w    d7,d0
[000106b0] 5e40                      addq.w    #7,d0
[000106b2] e640                      asr.w     #3,d0
[000106b4] 33c0 0001 3eca            move.w    d0,$00013ECA
[000106ba] 3079 0001 3eca            movea.w   $00013ECA,a0
[000106c0] 2e88                      move.l    a0,(a7)
[000106c2] 3079 0001 3ec8            movea.w   $00013EC8,a0
[000106c8] 2f08                      move.l    a0,-(a7)
[000106ca] 4eb9 0001 132a            jsr       $0001132A
[000106d0] 588f                      addq.l    #4,a7
[000106d2] 23c0 0001 3eda            move.l    d0,$00013EDA
[000106d8] 660a                      bne.s     $000106E4
[000106da] 2ebc 0001 3831            move.l    #$00013831,(a7)
[000106e0] 6100 0502                 bsr       $00010BE4
[000106e4] 4246                      clr.w     d6
[000106e6] 6030                      bra.s     $00010718
[000106e8] 4240                      clr.w     d0
[000106ea] 2079 0001 3eda            movea.l   $00013EDA,a0
[000106f0] 3206                      move.w    d6,d1
[000106f2] e641                      asr.w     #3,d1
[000106f4] 48c1                      ext.l     d1
[000106f6] 1030 1800                 move.b    0(a0,d1.l),d0
[000106fa] 4880                      ext.w     d0
[000106fc] 323c 0080                 move.w    #$0080,d1
[00010700] 3406                      move.w    d6,d2
[00010702] c47c 0007                 and.w     #$0007,d2
[00010706] e461                      asr.w     d2,d1
[00010708] c041                      and.w     d1,d0
[0001070a] 660a                      bne.s     $00010716
[0001070c] 4279 0001 3ed4            clr.w     $00013ED4
[00010712] 3e86                      move.w    d6,(a7)
[00010714] 6114                      bsr.s     $0001072A
[00010716] 5246                      addq.w    #1,d6
[00010718] bc79 0001 3ec8            cmp.w     $00013EC8,d6
[0001071e] 6dc8                      blt.s     $000106E8
[00010720] 4a9f                      tst.l     (a7)+
[00010722] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00010726] 4e5e                      unlk      a6
[00010728] 4e75                      rts

[0001072a] 4e56 0000                 link      a6,#$0000
[0001072e] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00010732] 3e2e 0008                 move.w    8(a6),d7
[00010736] 2079 0001 3eda            movea.l   $00013EDA,a0
[0001073c] 3207                      move.w    d7,d1
[0001073e] e641                      asr.w     #3,d1
[00010740] 48c1                      ext.l     d1
[00010742] 1030 1800                 move.b    0(a0,d1.l),d0
[00010746] 4880                      ext.w     d0
[00010748] 323c 0080                 move.w    #$0080,d1
[0001074c] 3407                      move.w    d7,d2
[0001074e] c47c 0007                 and.w     #$0007,d2
[00010752] e461                      asr.w     d2,d1
[00010754] c041                      and.w     d1,d0
[00010756] 6600 0094                 bne       $000107EC
[0001075a] 5279 0001 3ed4            addq.w    #1,$00013ED4
[00010760] 4240                      clr.w     d0
[00010762] 303c 0080                 move.w    #$0080,d0
[00010766] 3207                      move.w    d7,d1
[00010768] c27c 0007                 and.w     #$0007,d1
[0001076c] e260                      asr.w     d1,d0
[0001076e] 3f00                      move.w    d0,-(a7)
[00010770] 3007                      move.w    d7,d0
[00010772] e640                      asr.w     #3,d0
[00010774] 3040                      movea.w   d0,a0
[00010776] d1f9 0001 3eda            adda.l    $00013EDA,a0
[0001077c] 1010                      move.b    (a0),d0
[0001077e] 4880                      ext.w     d0
[00010780] 805f                      or.w      (a7)+,d0
[00010782] 1080                      move.b    d0,(a0)
[00010784] 3039 0001 3eca            move.w    $00013ECA,d0
[0001078a] c1c7                      muls.w    d7,d0
[0001078c] 2a40                      movea.l   d0,a5
[0001078e] dbf9 0001 3ed6            adda.l    $00013ED6,a5
[00010794] 4246                      clr.w     d6
[00010796] 6046                      bra.s     $000107DE
[00010798] 3006                      move.w    d6,d0
[0001079a] e640                      asr.w     #3,d0
[0001079c] 48c0                      ext.l     d0
[0001079e] 1035 0800                 move.b    0(a5,d0.l),d0
[000107a2] 4880                      ext.w     d0
[000107a4] 323c 0080                 move.w    #$0080,d1
[000107a8] 3406                      move.w    d6,d2
[000107aa] c47c 0007                 and.w     #$0007,d2
[000107ae] e461                      asr.w     d2,d1
[000107b0] c041                      and.w     d1,d0
[000107b2] 6728                      beq.s     $000107DC
[000107b4] 4240                      clr.w     d0
[000107b6] 3e86                      move.w    d6,(a7)
[000107b8] 6100 ff70                 bsr       $0001072A
[000107bc] 3eb9 0001 3eca            move.w    $00013ECA,(a7)
[000107c2] 3039 0001 3eca            move.w    $00013ECA,d0
[000107c8] c1c6                      muls.w    d6,d0
[000107ca] d0b9 0001 3ed6            add.l     $00013ED6,d0
[000107d0] 2f00                      move.l    d0,-(a7)
[000107d2] 2f0d                      move.l    a5,-(a7)
[000107d4] 4eb9 0001 0ea0            jsr       $00010EA0
[000107da] 508f                      addq.l    #8,a7
[000107dc] 5246                      addq.w    #1,d6
[000107de] bc79 0001 3ec8            cmp.w     $00013EC8,d6
[000107e4] 6db2                      blt.s     $00010798
[000107e6] 5379 0001 3ed4            subq.w    #1,$00013ED4
[000107ec] 4a9f                      tst.l     (a7)+
[000107ee] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[000107f2] 4e5e                      unlk      a6
[000107f4] 4e75                      rts

[000107f6] 4e56 fffc                 link      a6,#$FFFC
[000107fa] 600c                      bra.s     $00010808
[000107fc] 2ebc 0001 383f            move.l    #$0001383F,(a7)
[00010802] 4eb9 0001 3592            jsr       _printf
[00010808] 302e 0008                 move.w    8(a6),d0
[0001080c] 536e 0008                 subq.w    #1,8(a6)
[00010810] 4a40                      tst.w     d0
[00010812] 66e8                      bne.s     $000107FC
[00010814] 4e5e                      unlk      a6
[00010816] 4e75                      rts

[00010818] 4e56 0000                 link      a6,#$0000
[0001081c] 48e7 0104                 movem.l   d7/a5,-(a7)
[00010820] 2a6e 0008                 movea.l   8(a6),a5
[00010824] 3abc ff75                 move.w    #$FF75,(a5)
[00010828] 3b79 0001 3ec6 0002       move.w    $00013EC6,2(a5)
[00010830] 3b79 0001 3ec8 0004       move.w    $00013EC8,4(a5)
[00010838] 4a9f                      tst.l     (a7)+
[0001083a] 4cdf 2000                 movem.l   (a7)+,a5
[0001083e] 4e5e                      unlk      a6
[00010840] 4e75                      rts

[00010842] 4e56 0000                 link      a6,#$0000
[00010846] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0001084a] 2a6e 0008                 movea.l   8(a6),a5
[0001084e] 286e 000c                 movea.l   12(a6),a4
[00010852] 601c                      bra.s     $00010870
[00010854] 2e8c                      move.l    a4,(a7)
[00010856] 2f0d                      move.l    a5,-(a7)
[00010858] 4eb9 0001 1b0a            jsr       _strcpy
[0001085e] 588f                      addq.l    #4,a7
[00010860] 2b6c 0014 0010            move.l    20(a4),16(a5)
[00010866] dbfc 0000 0014            adda.l    #$00000014,a5
[0001086c] 286c 0010                 movea.l   16(a4),a4
[00010870] 200c                      move.l    a4,d0
[00010872] 66e0                      bne.s     $00010854
[00010874] 4a9f                      tst.l     (a7)+
[00010876] 4cdf 3000                 movem.l   (a7)+,a4-a5
[0001087a] 4e5e                      unlk      a6
[0001087c] 4e75                      rts

[0001087e] 4e56 fffc                 link      a6,#$FFFC
[00010882] 48e7 011c                 movem.l   d7/a3-a5,-(a7)
[00010886] 2a6e 0008                 movea.l   8(a6),a5
[0001088a] 6046                      bra.s     $000108D2
[0001088c] 286d 001c                 movea.l   28(a5),a4
[00010890] 6012                      bra.s     $000108A4
[00010892] 2d6c 000a fffc            move.l    10(a4),-4(a6)
[00010898] 2e8c                      move.l    a4,(a7)
[0001089a] 4eb9 0001 11a0            jsr       $000111A0
[000108a0] 286e fffc                 movea.l   -4(a6),a4
[000108a4] 200c                      move.l    a4,d0
[000108a6] 66ea                      bne.s     $00010892
[000108a8] 286d 0018                 movea.l   24(a5),a4
[000108ac] 6012                      bra.s     $000108C0
[000108ae] 2d6c 000a fffc            move.l    10(a4),-4(a6)
[000108b4] 2e8c                      move.l    a4,(a7)
[000108b6] 4eb9 0001 11a0            jsr       $000111A0
[000108bc] 286e fffc                 movea.l   -4(a6),a4
[000108c0] 200c                      move.l    a4,d0
[000108c2] 66ea                      bne.s     $000108AE
[000108c4] 266d 0010                 movea.l   16(a5),a3
[000108c8] 2e8d                      move.l    a5,(a7)
[000108ca] 4eb9 0001 11a0            jsr       $000111A0
[000108d0] 2a4b                      movea.l   a3,a5
[000108d2] 200d                      move.l    a5,d0
[000108d4] 66b6                      bne.s     $0001088C
[000108d6] 4a9f                      tst.l     (a7)+
[000108d8] 4cdf 3800                 movem.l   (a7)+,a3-a5
[000108dc] 4e5e                      unlk      a6
[000108de] 4e75                      rts

[000108e0] 4e56 0000                 link      a6,#$0000
[000108e4] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[000108e8] 2a6e 0008                 movea.l   8(a6),a5
[000108ec] 4246                      clr.w     d6
[000108ee] 6000 00d8                 bra       $000109C8
[000108f2] 2ead 0014                 move.l    20(a5),(a7)
[000108f6] 2f0d                      move.l    a5,-(a7)
[000108f8] 2f3c 0001 3844            move.l    #$00013844,-(a7)
[000108fe] 4eb9 0001 3592            jsr       _printf
[00010904] 508f                      addq.l    #8,a7
[00010906] 5246                      addq.w    #1,d6
[00010908] 4aad 001c                 tst.l     28(a5)
[0001090c] 6758                      beq.s     $00010966
[0001090e] 2ebc 0001 3866            move.l    #$00013866,(a7)
[00010914] 4eb9 0001 3592            jsr       _printf
[0001091a] 7e12                      moveq.l   #18,d7
[0001091c] 286d 001c                 movea.l   28(a5),a4
[00010920] 6034                      bra.s     $00010956
[00010922] be7c 0046                 cmp.w     #$0046,d7
[00010926] 6f0e                      ble.s     $00010936
[00010928] 2ebc 0001 3871            move.l    #$00013871,(a7)
[0001092e] 4eb9 0001 3592            jsr       _printf
[00010934] 7e12                      moveq.l   #18,d7
[00010936] 2e8c                      move.l    a4,(a7)
[00010938] 2f3c 0001 3876            move.l    #$00013876,-(a7)
[0001093e] 4eb9 0001 3592            jsr       _printf
[00010944] 588f                      addq.l    #4,a7
[00010946] 2e8c                      move.l    a4,(a7)
[00010948] 4eb9 0001 1ae4            jsr       _strlen
[0001094e] 5240                      addq.w    #1,d0
[00010950] de40                      add.w     d0,d7
[00010952] 286c 000a                 movea.l   10(a4),a4
[00010956] 200c                      move.l    a4,d0
[00010958] 66c8                      bne.s     $00010922
[0001095a] 2ebc 0001 387a            move.l    #$0001387A,(a7)
[00010960] 4eb9 0001 3592            jsr       _printf
[00010966] 4aad 0018                 tst.l     24(a5)
[0001096a] 6758                      beq.s     $000109C4
[0001096c] 2ebc 0001 387c            move.l    #$0001387C,(a7)
[00010972] 4eb9 0001 3592            jsr       _printf
[00010978] 7e12                      moveq.l   #18,d7
[0001097a] 286d 0018                 movea.l   24(a5),a4
[0001097e] 6034                      bra.s     $000109B4
[00010980] be7c 0046                 cmp.w     #$0046,d7
[00010984] 6f0e                      ble.s     $00010994
[00010986] 2ebc 0001 3887            move.l    #$00013887,(a7)
[0001098c] 4eb9 0001 3592            jsr       _printf
[00010992] 7e12                      moveq.l   #18,d7
[00010994] 2e8c                      move.l    a4,(a7)
[00010996] 2f3c 0001 388c            move.l    #$0001388C,-(a7)
[0001099c] 4eb9 0001 3592            jsr       _printf
[000109a2] 588f                      addq.l    #4,a7
[000109a4] 2e8c                      move.l    a4,(a7)
[000109a6] 4eb9 0001 1ae4            jsr       _strlen
[000109ac] 5240                      addq.w    #1,d0
[000109ae] de40                      add.w     d0,d7
[000109b0] 286c 000a                 movea.l   10(a4),a4
[000109b4] 200c                      move.l    a4,d0
[000109b6] 66c8                      bne.s     $00010980
[000109b8] 2ebc 0001 3890            move.l    #$00013890,(a7)
[000109be] 4eb9 0001 3592            jsr       _printf
[000109c4] 2a6d 0010                 movea.l   16(a5),a5
[000109c8] 200d                      move.l    a5,d0
[000109ca] 6600 ff26                 bne       $000108F2
[000109ce] 4a9f                      tst.l     (a7)+
[000109d0] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[000109d4] 4e5e                      unlk      a6
[000109d6] 4e75                      rts

[000109d8] 4e56 0000                 link      a6,#$0000
[000109dc] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[000109e0] 2a6e 0008                 movea.l   8(a6),a5
[000109e4] 3e15                      move.w    (a5),d7
[000109e6] 3c2d 0002                 move.w    2(a5),d6
[000109ea] 3a2d 0004                 move.w    4(a5),d5
[000109ee] 5c8d                      addq.l    #6,a5
[000109f0] 4240                      clr.w     d0
[000109f2] 3e86                      move.w    d6,(a7)
[000109f4] 3f05                      move.w    d5,-(a7)
[000109f6] 2f3c 0001 3892            move.l    #$00013892,-(a7)
[000109fc] 4eb9 0001 3592            jsr       _printf
[00010a02] 5c8f                      addq.l    #6,a7
[00010a04] 3e85                      move.w    d5,(a7)
[00010a06] 2f0d                      move.l    a5,-(a7)
[00010a08] 6100 0106                 bsr       $00010B10
[00010a0c] 588f                      addq.l    #4,a7
[00010a0e] 3005                      move.w    d5,d0
[00010a10] c1fc 0014                 muls.w    #$0014,d0
[00010a14] 48c0                      ext.l     d0
[00010a16] dbc0                      adda.l    d0,a5
[00010a18] 2ebc 0001 38c8            move.l    #$000138C8,(a7)
[00010a1e] 4eb9 0001 3592            jsr       _printf
[00010a24] 603c                      bra.s     $00010A62
[00010a26] 3ead 000a                 move.w    10(a5),(a7)
[00010a2a] 2f0d                      move.l    a5,-(a7)
[00010a2c] 4eb9 0001 1ae4            jsr       _strlen
[00010a32] 588f                      addq.l    #4,a7
[00010a34] b07c 0008                 cmp.w     #$0008,d0
[00010a38] 6c08                      bge.s     $00010A42
[00010a3a] 2f3c 0001 38dc            move.l    #$000138DC,-(a7)
[00010a40] 6006                      bra.s     $00010A48
[00010a42] 2f3c 0001 38de            move.l    #$000138DE,-(a7)
[00010a48] 2f0d                      move.l    a5,-(a7)
[00010a4a] 2f3c 0001 38d3            move.l    #$000138D3,-(a7)
[00010a50] 4eb9 0001 3592            jsr       _printf
[00010a56] dffc 0000 000c            adda.l    #$0000000C,a7
[00010a5c] dbfc 0000 000c            adda.l    #$0000000C,a5
[00010a62] 3006                      move.w    d6,d0
[00010a64] 5346                      subq.w    #1,d6
[00010a66] 4a40                      tst.w     d0
[00010a68] 66bc                      bne.s     $00010A26
[00010a6a] 2ebc 0001 38df            move.l    #$000138DF,(a7)
[00010a70] 4eb9 0001 3592            jsr       _printf
[00010a76] 2e8d                      move.l    a5,(a7)
[00010a78] 610a                      bsr.s     $00010A84
[00010a7a] 4a9f                      tst.l     (a7)+
[00010a7c] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00010a80] 4e5e                      unlk      a6
[00010a82] 4e75                      rts

[00010a84] 4e56 0000                 link      a6,#$0000
[00010a88] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00010a8c] 2a6e 0008                 movea.l   8(a6),a5
[00010a90] 4247                      clr.w     d7
[00010a92] 606a                      bra.s     $00010AFE
[00010a94] 4245                      clr.w     d5
[00010a96] 4246                      clr.w     d6
[00010a98] 6044                      bra.s     $00010ADE
[00010a9a] 3006                      move.w    d6,d0
[00010a9c] e640                      asr.w     #3,d0
[00010a9e] 48c0                      ext.l     d0
[00010aa0] 1035 0800                 move.b    0(a5,d0.l),d0
[00010aa4] 4880                      ext.w     d0
[00010aa6] 323c 0080                 move.w    #$0080,d1
[00010aaa] 3406                      move.w    d6,d2
[00010aac] c47c 0007                 and.w     #$0007,d2
[00010ab0] e461                      asr.w     d2,d1
[00010ab2] c041                      and.w     d1,d0
[00010ab4] 6726                      beq.s     $00010ADC
[00010ab6] 4a45                      tst.w     d5
[00010ab8] 6612                      bne.s     $00010ACC
[00010aba] 7a01                      moveq.l   #1,d5
[00010abc] 3e87                      move.w    d7,(a7)
[00010abe] 2f3c 0001 38f4            move.l    #$000138F4,-(a7)
[00010ac4] 4eb9 0001 3592            jsr       _printf
[00010aca] 588f                      addq.l    #4,a7
[00010acc] 3e86                      move.w    d6,(a7)
[00010ace] 2f3c 0001 38f9            move.l    #$000138F9,-(a7)
[00010ad4] 4eb9 0001 3592            jsr       _printf
[00010ada] 588f                      addq.l    #4,a7
[00010adc] 5246                      addq.w    #1,d6
[00010ade] bc79 0001 3ec8            cmp.w     $00013EC8,d6
[00010ae4] 6db4                      blt.s     $00010A9A
[00010ae6] 4a45                      tst.w     d5
[00010ae8] 670c                      beq.s     $00010AF6
[00010aea] 2ebc 0001 38fd            move.l    #$000138FD,(a7)
[00010af0] 4eb9 0001 3592            jsr       _printf
[00010af6] daf9 0001 3eca            adda.w    $00013ECA,a5
[00010afc] 5247                      addq.w    #1,d7
[00010afe] be79 0001 3ec8            cmp.w     $00013EC8,d7
[00010b04] 6d8e                      blt.s     $00010A94
[00010b06] 4a9f                      tst.l     (a7)+
[00010b08] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00010b0c] 4e5e                      unlk      a6
[00010b0e] 4e75                      rts

[00010b10] 4e56 0000                 link      a6,#$0000
[00010b14] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00010b18] 2a6e 0008                 movea.l   8(a6),a5
[00010b1c] 3e2e 000c                 move.w    12(a6),d7
[00010b20] 4246                      clr.w     d6
[00010b22] 6022                      bra.s     $00010B46
[00010b24] 2e8d                      move.l    a5,(a7)
[00010b26] 2f2d 0010                 move.l    16(a5),-(a7)
[00010b2a] 3f06                      move.w    d6,-(a7)
[00010b2c] 2f3c 0001 38ff            move.l    #$000138FF,-(a7)
[00010b32] 4eb9 0001 3592            jsr       _printf
[00010b38] dffc 0000 000a            adda.l    #$0000000A,a7
[00010b3e] dbfc 0000 0014            adda.l    #$00000014,a5
[00010b44] 5246                      addq.w    #1,d6
[00010b46] bc47                      cmp.w     d7,d6
[00010b48] 6dda                      blt.s     $00010B24
[00010b4a] 4a9f                      tst.l     (a7)+
[00010b4c] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00010b50] 4e5e                      unlk      a6
[00010b52] 4e75                      rts

[00010b54] 4e56 0000                 link      a6,#$0000
[00010b58] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00010b5c] 2a79 0001 3ecc            movea.l   $00013ECC,a5
[00010b62] 3e39 0001 3ec6            move.w    $00013EC6,d7
[00010b68] 601a                      bra.s     $00010B84
[00010b6a] 3ead 000a                 move.w    10(a5),(a7)
[00010b6e] 2f0d                      move.l    a5,-(a7)
[00010b70] 2f3c 0001 3917            move.l    #$00013917,-(a7)
[00010b76] 4eb9 0001 3592            jsr       _printf
[00010b7c] 508f                      addq.l    #8,a7
[00010b7e] dbfc 0000 000c            adda.l    #$0000000C,a5
[00010b84] 3007                      move.w    d7,d0
[00010b86] 5347                      subq.w    #1,d7
[00010b88] 4a40                      tst.w     d0
[00010b8a] 66de                      bne.s     $00010B6A
[00010b8c] 4a9f                      tst.l     (a7)+
[00010b8e] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00010b92] 4e5e                      unlk      a6
[00010b94] 4e75                      rts
usage:
[00010b96] 4e56 fffc                 link      a6,#$FFFC
[00010b9a] 2ebc 0001 391e            move.l    #$0001391E,(a7)
[00010ba0] 4eb9 0001 3546            jsr       _puts
[00010ba6] 2ebc 0001 3940            move.l    #$00013940,(a7)
[00010bac] 4eb9 0001 3546            jsr       _puts
[00010bb2] 2ebc 0001 3971            move.l    #$00013971,(a7)
[00010bb8] 4eb9 0001 3546            jsr       _puts
[00010bbe] 2ebc 0001 39a5            move.l    #$000139A5,(a7)
[00010bc4] 4eb9 0001 3546            jsr       _puts
[00010bca] 2ebc 0001 39db            move.l    #$000139DB,(a7)
[00010bd0] 4eb9 0001 3546            jsr       _puts
[00010bd6] 3ebc 0001                 move.w    #$0001,(a7)
[00010bda] 4eb9 0001 3352            jsr       _exit
[00010be0] 4e5e                      unlk      a6
[00010be2] 4e75                      rts

[00010be4] 4e56 fffc                 link      a6,#$FFFC
[00010be8] 2eae 0008                 move.l    8(a6),(a7)
[00010bec] 2f3c 0001 3a07            move.l    #$00013A07,-(a7)
[00010bf2] 4eb9 0001 3592            jsr       _printf
[00010bf8] 588f                      addq.l    #4,a7
[00010bfa] 3ebc 0001                 move.w    #$0001,(a7)
[00010bfe] 4eb9 0001 3352            jsr       _exit
[00010c04] 4e5e                      unlk      a6
[00010c06] 4e75                      rts
_main:
[00010c08] 4e56 fff2                 link      a6,#$FFF2
[00010c0c] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00010c10] 426e fff8                 clr.w     -8(a6)
[00010c14] 426e fff6                 clr.w     -10(a6)
[00010c18] 0c6e 0001 0008            cmpi.w    #$0001,8(a6)
[00010c1e] 6604                      bne.s     $00010C24
[00010c20] 6100 ff74                 bsr       $00010B96
[00010c24] 7e01                      moveq.l   #1,d7
[00010c26] 605c                      bra.s     $00010C84
[00010c28] 206e 000a                 movea.l   10(a6),a0
[00010c2c] 3247                      movea.w   d7,a1
[00010c2e] d3c9                      adda.l    a1,a1
[00010c30] d3c9                      adda.l    a1,a1
[00010c32] 2070 9800                 movea.l   0(a0,a1.l),a0
[00010c36] 1028 0001                 move.b    1(a0),d0
[00010c3a] 4880                      ext.w     d0
[00010c3c] 602a                      bra.s     $00010C68
[00010c3e] 3d7c 0001 fff8            move.w    #$0001,-8(a6)
[00010c44] 603c                      bra.s     $00010C82
[00010c46] 33fc 0001 0001 3774       move.w    #$0001,$00013774
[00010c4e] 6032                      bra.s     $00010C82
[00010c50] 3d7c 0001 fff6            move.w    #$0001,-10(a6)
[00010c56] 602a                      bra.s     $00010C82
[00010c58] 33fc 0001 0001 3772       move.w    #$0001,$00013772
[00010c60] 6020                      bra.s     $00010C82
[00010c62] 6100 ff32                 bsr       $00010B96
[00010c66] 601a                      bra.s     $00010C82
[00010c68] b07c 0064                 cmp.w     #$0064,d0
[00010c6c] 67d0                      beq.s     $00010C3E
[00010c6e] b07c 0065                 cmp.w     #$0065,d0
[00010c72] 67d2                      beq.s     $00010C46
[00010c74] b07c 0069                 cmp.w     #$0069,d0
[00010c78] 67d6                      beq.s     $00010C50
[00010c7a] b07c 0077                 cmp.w     #$0077,d0
[00010c7e] 67d8                      beq.s     $00010C58
[00010c80] 60e0                      bra.s     $00010C62
[00010c82] 5247                      addq.w    #1,d7
[00010c84] 206e 000a                 movea.l   10(a6),a0
[00010c88] 3247                      movea.w   d7,a1
[00010c8a] d3c9                      adda.l    a1,a1
[00010c8c] d3c9                      adda.l    a1,a1
[00010c8e] 2070 9800                 movea.l   0(a0,a1.l),a0
[00010c92] 0c10 002d                 cmpi.b    #$2D,(a0)
[00010c96] 6790                      beq.s     $00010C28
[00010c98] 4257                      clr.w     (a7)
[00010c9a] 206e 000a                 movea.l   10(a6),a0
[00010c9e] 3207                      move.w    d7,d1
[00010ca0] 48c1                      ext.l     d1
[00010ca2] e581                      asl.l     #2,d1
[00010ca4] 2f30 1800                 move.l    0(a0,d1.l),-(a7)
[00010ca8] 3f3c 003d                 move.w    #$003D,-(a7)
[00010cac] 4eb9 0001 14b2            jsr       _gemdos
[00010cb2] 5c8f                      addq.l    #6,a7
[00010cb4] 3c00                      move.w    d0,d6
[00010cb6] 6c0a                      bge.s     $00010CC2
[00010cb8] 2ebc 0001 3a12            move.l    #$00013A12,(a7)
[00010cbe] 6100 ff24                 bsr       $00010BE4
[00010cc2] 2e8e                      move.l    a6,(a7)
[00010cc4] 5597                      subq.l    #2,(a7)
[00010cc6] 2f3c 0000 0002            move.l    #$00000002,-(a7)
[00010ccc] 3f06                      move.w    d6,-(a7)
[00010cce] 3f3c 003f                 move.w    #$003F,-(a7)
[00010cd2] 4eb9 0001 14b2            jsr       _gemdos
[00010cd8] 508f                      addq.l    #8,a7
[00010cda] b0bc 0000 0002            cmp.l     #$00000002,d0
[00010ce0] 670a                      beq.s     $00010CEC
[00010ce2] 2ebc 0001 3a28            move.l    #$00013A28,(a7)
[00010ce8] 6100 fefa                 bsr       $00010BE4
[00010cec] 0c6e ff65 fffe            cmpi.w    #$FF65,-2(a6)
[00010cf2] 6720                      beq.s     $00010D14
[00010cf4] 206e 000a                 movea.l   10(a6),a0
[00010cf8] 3207                      move.w    d7,d1
[00010cfa] 48c1                      ext.l     d1
[00010cfc] e581                      asl.l     #2,d1
[00010cfe] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[00010d02] 2f3c 0001 3a41            move.l    #$00013A41,-(a7)
[00010d08] 4eb9 0001 3592            jsr       _printf
[00010d0e] 588f                      addq.l    #4,a7
[00010d10] 6100 fe84                 bsr       $00010B96
[00010d14] 3eae fff6                 move.w    -10(a6),(a7)
[00010d18] 3f06                      move.w    d6,-(a7)
[00010d1a] 6100 f438                 bsr       $00010154
[00010d1e] 548f                      addq.l    #2,a7
[00010d20] 2a40                      movea.l   d0,a5
[00010d22] 200d                      move.l    a5,d0
[00010d24] 6620                      bne.s     $00010D46
[00010d26] 206e 000a                 movea.l   10(a6),a0
[00010d2a] 3207                      move.w    d7,d1
[00010d2c] 48c1                      ext.l     d1
[00010d2e] e581                      asl.l     #2,d1
[00010d30] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[00010d34] 2f3c 0001 3a5b            move.l    #$00013A5B,-(a7)
[00010d3a] 4eb9 0001 3592            jsr       _printf
[00010d40] 588f                      addq.l    #4,a7
[00010d42] 6100 fe52                 bsr       $00010B96
[00010d46] 3e86                      move.w    d6,(a7)
[00010d48] 3f3c 003e                 move.w    #$003E,-(a7)
[00010d4c] 4eb9 0001 14b2            jsr       _gemdos
[00010d52] 548f                      addq.l    #2,a7
[00010d54] 4a80                      tst.l     d0
[00010d56] 670a                      beq.s     $00010D62
[00010d58] 2ebc 0001 3a80            move.l    #$00013A80,(a7)
[00010d5e] 6100 fe84                 bsr       $00010BE4
[00010d62] 4a6e fff8                 tst.w     -8(a6)
[00010d66] 6706                      beq.s     $00010D6E
[00010d68] 2e8d                      move.l    a5,(a7)
[00010d6a] 6100 fc6c                 bsr       $000109D8
[00010d6e] 206e 000a                 movea.l   10(a6),a0
[00010d72] 3207                      move.w    d7,d1
[00010d74] 48c1                      ext.l     d1
[00010d76] e581                      asl.l     #2,d1
[00010d78] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[00010d7c] 4eb9 0001 1ae4            jsr       _strlen
[00010d82] 48c0                      ext.l     d0
[00010d84] 2e80                      move.l    d0,(a7)
[00010d86] 5a97                      addq.l    #5,(a7)
[00010d88] 4eb9 0001 1026            jsr       $00011026
[00010d8e] 2d40 fffa                 move.l    d0,-6(a6)
[00010d92] 660a                      bne.s     $00010D9E
[00010d94] 2ebc 0001 3a99            move.l    #$00013A99,(a7)
[00010d9a] 6100 fe48                 bsr       $00010BE4
[00010d9e] 206e 000a                 movea.l   10(a6),a0
[00010da2] 3207                      move.w    d7,d1
[00010da4] 48c1                      ext.l     d1
[00010da6] e581                      asl.l     #2,d1
[00010da8] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[00010dac] 2f2e fffa                 move.l    -6(a6),-(a7)
[00010db0] 4eb9 0001 1b0a            jsr       _strcpy
[00010db6] 588f                      addq.l    #4,a7
[00010db8] 3ebc 002e                 move.w    #$002E,(a7)
[00010dbc] 2f2e fffa                 move.l    -6(a6),-(a7)
[00010dc0] 4eb9 0001 33a0            jsr       _rindex
[00010dc6] 588f                      addq.l    #4,a7
[00010dc8] 4a80                      tst.l     d0
[00010dca] 6714                      beq.s     $00010DE0
[00010dcc] 3ebc 002e                 move.w    #$002E,(a7)
[00010dd0] 2f2e fffa                 move.l    -6(a6),-(a7)
[00010dd4] 4eb9 0001 33a0            jsr       _rindex
[00010dda] 588f                      addq.l    #4,a7
[00010ddc] 2040                      movea.l   d0,a0
[00010dde] 4210                      clr.b     (a0)
[00010de0] 2ebc 0001 3aa7            move.l    #$00013AA7,(a7)
[00010de6] 2f2e fffa                 move.l    -6(a6),-(a7)
[00010dea] 4eb9 0001 1bb6            jsr       _strcat
[00010df0] 588f                      addq.l    #4,a7
[00010df2] 4257                      clr.w     (a7)
[00010df4] 2f2e fffa                 move.l    -6(a6),-(a7)
[00010df8] 3f3c 003c                 move.w    #$003C,-(a7)
[00010dfc] 4eb9 0001 14b2            jsr       _gemdos
[00010e02] 5c8f                      addq.l    #6,a7
[00010e04] 3a00                      move.w    d0,d5
[00010e06] 6c0a                      bge.s     $00010E12
[00010e08] 2ebc 0001 3aac            move.l    #$00013AAC,(a7)
[00010e0e] 6100 fdd4                 bsr       $00010BE4
[00010e12] 33ed 0002 0001 3ec6       move.w    2(a5),$00013EC6
[00010e1a] 33ed 0004 0001 3ec8       move.w    4(a5),$00013EC8
[00010e22] 3039 0001 3ec8            move.w    $00013EC8,d0
[00010e28] c1fc 0014                 muls.w    #$0014,d0
[00010e2c] 3239 0001 3ec6            move.w    $00013EC6,d1
[00010e32] c3fc 000c                 muls.w    #$000C,d1
[00010e36] d041                      add.w     d1,d0
[00010e38] 3239 0001 3ec8            move.w    $00013EC8,d1
[00010e3e] c3f9 0001 3eca            muls.w    $00013ECA,d1
[00010e44] d041                      add.w     d1,d0
[00010e46] 5c40                      addq.w    #6,d0
[00010e48] 48c0                      ext.l     d0
[00010e4a] 2d40 fff2                 move.l    d0,-14(a6)
[00010e4e] 2e8d                      move.l    a5,(a7)
[00010e50] 2f2e fff2                 move.l    -14(a6),-(a7)
[00010e54] 3f05                      move.w    d5,-(a7)
[00010e56] 3f3c 0040                 move.w    #$0040,-(a7)
[00010e5a] 4eb9 0001 14b2            jsr       _gemdos
[00010e60] 508f                      addq.l    #8,a7
[00010e62] b0ae fff2                 cmp.l     -14(a6),d0
[00010e66] 670a                      beq.s     $00010E72
[00010e68] 2ebc 0001 3ac3            move.l    #$00013AC3,(a7)
[00010e6e] 6100 fd74                 bsr       $00010BE4
[00010e72] 3e85                      move.w    d5,(a7)
[00010e74] 3f3c 003e                 move.w    #$003E,-(a7)
[00010e78] 4eb9 0001 14b2            jsr       _gemdos
[00010e7e] 548f                      addq.l    #2,a7
[00010e80] 4a80                      tst.l     d0
[00010e82] 670a                      beq.s     $00010E8E
[00010e84] 2ebc 0001 3adc            move.l    #$00013ADC,(a7)
[00010e8a] 6100 fd58                 bsr       $00010BE4
[00010e8e] 2e8d                      move.l    a5,(a7)
[00010e90] 4eb9 0001 11a0            jsr       $000111A0
[00010e96] 4a9f                      tst.l     (a7)+
[00010e98] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00010e9c] 4e5e                      unlk      a6
[00010e9e] 4e75                      rts

[00010ea0] 4e56 0000                 link      a6,#$0000
[00010ea4] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00010ea8] 2a6e 0008                 movea.l   8(a6),a5
[00010eac] 286e 000c                 movea.l   12(a6),a4
[00010eb0] 3e2e 0010                 move.w    16(a6),d7
[00010eb4] 6008                      bra.s     $00010EBE
[00010eb6] 101c                      move.b    (a4)+,d0
[00010eb8] 4880                      ext.w     d0
[00010eba] 811d                      or.b      d0,(a5)+
[00010ebc] 5347                      subq.w    #1,d7
[00010ebe] 4a47                      tst.w     d7
[00010ec0] 66f4                      bne.s     $00010EB6
[00010ec2] 4a9f                      tst.l     (a7)+
[00010ec4] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00010ec8] 4e5e                      unlk      a6
[00010eca] 4e75                      rts

[00010ecc] 4e56 0000                 link      a6,#$0000
[00010ed0] 48e7 1f1c                 movem.l   d3-d7/a3-a5,-(a7)
[00010ed4] 2a6e 0008                 movea.l   8(a6),a5
[00010ed8] 286e 000c                 movea.l   12(a6),a4
[00010edc] 7c01                      moveq.l   #1,d6
[00010ede] 3a2e 0010                 move.w    16(a6),d5
[00010ee2] ba46                      cmp.w     d6,d5
[00010ee4] 6c04                      bge.s     $00010EEA
[00010ee6] 70ff                      moveq.l   #-1,d0
[00010ee8] 603c                      bra.s     $00010F26
[00010eea] 3e06                      move.w    d6,d7
[00010eec] de45                      add.w     d5,d7
[00010eee] e247                      asr.w     #1,d7
[00010ef0] 3007                      move.w    d7,d0
[00010ef2] 5340                      subq.w    #1,d0
[00010ef4] c1fc 000c                 muls.w    #$000C,d0
[00010ef8] 2640                      movea.l   d0,a3
[00010efa] d7cc                      adda.l    a4,a3
[00010efc] 2e8b                      move.l    a3,(a7)
[00010efe] 2f0d                      move.l    a5,-(a7)
[00010f00] 4eb9 0001 1b74            jsr       _strcmp
[00010f06] 588f                      addq.l    #4,a7
[00010f08] 3800                      move.w    d0,d4
[00010f0a] 4a44                      tst.w     d4
[00010f0c] 6c06                      bge.s     $00010F14
[00010f0e] 3a07                      move.w    d7,d5
[00010f10] 5345                      subq.w    #1,d5
[00010f12] 6010                      bra.s     $00010F24
[00010f14] 4a44                      tst.w     d4
[00010f16] 6608                      bne.s     $00010F20
[00010f18] 302b 000a                 move.w    10(a3),d0
[00010f1c] 6008                      bra.s     $00010F26
[00010f1e] 6004                      bra.s     $00010F24
[00010f20] 3c07                      move.w    d7,d6
[00010f22] 5246                      addq.w    #1,d6
[00010f24] 60bc                      bra.s     $00010EE2
[00010f26] 4a9f                      tst.l     (a7)+
[00010f28] 4cdf 38f0                 movem.l   (a7)+,d4-d7/a3-a5
[00010f2c] 4e5e                      unlk      a6
[00010f2e] 4e75                      rts

[00010f30] 4e56 ffe4                 link      a6,#$FFE4
[00010f34] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00010f38] 2eae 000a                 move.l    10(a6),(a7)
[00010f3c] 2f3c 0000 001c            move.l    #$0000001C,-(a7)
[00010f42] 3f2e 0008                 move.w    8(a6),-(a7)
[00010f46] 3f3c 003f                 move.w    #$003F,-(a7)
[00010f4a] 4eb9 0001 14b2            jsr       _gemdos
[00010f50] 508f                      addq.l    #8,a7
[00010f52] 3e00                      move.w    d0,d7
[00010f54] be7c 001c                 cmp.w     #$001C,d7
[00010f58] 673e                      beq.s     $00010F98
[00010f5a] 4a47                      tst.w     d7
[00010f5c] 6606                      bne.s     $00010F64
[00010f5e] 7001                      moveq.l   #1,d0
[00010f60] 6044                      bra.s     $00010FA6
[00010f62] 6034                      bra.s     $00010F98
[00010f64] 4a47                      tst.w     d7
[00010f66] 6f20                      ble.s     $00010F88
[00010f68] 206e 000a                 movea.l   10(a6),a0
[00010f6c] 4a10                      tst.b     (a0)
[00010f6e] 6606                      bne.s     $00010F76
[00010f70] 7001                      moveq.l   #1,d0
[00010f72] 6032                      bra.s     $00010FA6
[00010f74] 6010                      bra.s     $00010F86
[00010f76] 2ebc 0001 3af6            move.l    #$00013AF6,(a7)
[00010f7c] 4eb9 0001 3592            jsr       _printf
[00010f82] 7001                      moveq.l   #1,d0
[00010f84] 6020                      bra.s     $00010FA6
[00010f86] 6010                      bra.s     $00010F98
[00010f88] 2ebc 0001 3b19            move.l    #$00013B19,(a7)
[00010f8e] 4eb9 0001 3592            jsr       _printf
[00010f94] 7001                      moveq.l   #1,d0
[00010f96] 600e                      bra.s     $00010FA6
[00010f98] 206e 000a                 movea.l   10(a6),a0
[00010f9c] 4a10                      tst.b     (a0)
[00010f9e] 6604                      bne.s     $00010FA4
[00010fa0] 7001                      moveq.l   #1,d0
[00010fa2] 6002                      bra.s     $00010FA6
[00010fa4] 4240                      clr.w     d0
[00010fa6] 4a9f                      tst.l     (a7)+
[00010fa8] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00010fac] 4e5e                      unlk      a6
[00010fae] 4e75                      rts

[00010fb0] 4e56 0000                 link      a6,#$0000
[00010fb4] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00010fb8] 2a6e 0008                 movea.l   8(a6),a5
[00010fbc] 286e 000c                 movea.l   12(a6),a4
[00010fc0] 2adc                      move.l    (a4)+,(a5)+
[00010fc2] 2adc                      move.l    (a4)+,(a5)+
[00010fc4] 4215                      clr.b     (a5)
[00010fc6] 4a9f                      tst.l     (a7)+
[00010fc8] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00010fcc] 4e5e                      unlk      a6
[00010fce] 4e75                      rts

[00010fd0] 4e56 0000                 link      a6,#$0000
[00010fd4] 48e7 0f00                 movem.l   d4-d7,-(a7)
[00010fd8] 3e2e 0008                 move.w    8(a6),d7
[00010fdc] 3ebc 0001                 move.w    #$0001,(a7)
[00010fe0] 3f07                      move.w    d7,-(a7)
[00010fe2] 42a7                      clr.l     -(a7)
[00010fe4] 3f3c 0042                 move.w    #$0042,-(a7)
[00010fe8] 4eb9 0001 14b2            jsr       _gemdos
[00010fee] 508f                      addq.l    #8,a7
[00010ff0] 2c00                      move.l    d0,d6
[00010ff2] 3ebc 0002                 move.w    #$0002,(a7)
[00010ff6] 3f07                      move.w    d7,-(a7)
[00010ff8] 42a7                      clr.l     -(a7)
[00010ffa] 3f3c 0042                 move.w    #$0042,-(a7)
[00010ffe] 4eb9 0001 14b2            jsr       _gemdos
[00011004] 508f                      addq.l    #8,a7
[00011006] 2a00                      move.l    d0,d5
[00011008] 4257                      clr.w     (a7)
[0001100a] 3f07                      move.w    d7,-(a7)
[0001100c] 2f06                      move.l    d6,-(a7)
[0001100e] 3f3c 0042                 move.w    #$0042,-(a7)
[00011012] 4eb9 0001 14b2            jsr       _gemdos
[00011018] 508f                      addq.l    #8,a7
[0001101a] 2005                      move.l    d5,d0
[0001101c] 4a9f                      tst.l     (a7)+
[0001101e] 4cdf 00e0                 movem.l   (a7)+,d5-d7
[00011022] 4e5e                      unlk      a6
[00011024] 4e75                      rts

[00011026] 4e56 0000                 link      a6,#$0000
[0001102a] 48e7 0f0c                 movem.l   d4-d7/a4-a5,-(a7)
[0001102e] 2e2e 0008                 move.l    8(a6),d7
[00011032] bebc ffff ffff            cmp.l     #$FFFFFFFF,d7
[00011038] 6636                      bne.s     $00011070
[0001103a] 2079 0001 3b38            movea.l   $00013B38,a0
[00011040] 2850                      movea.l   (a0),a4
[00011042] 2079 0001 3b38            movea.l   $00013B38,a0
[00011048] 2a28 0004                 move.l    4(a0),d5
[0001104c] 2079 0001 3b38            movea.l   $00013B38,a0
[00011052] 2850                      movea.l   (a0),a4
[00011054] 600c                      bra.s     $00011062
[00011056] baac 0004                 cmp.l     4(a4),d5
[0001105a] 6c04                      bge.s     $00011060
[0001105c] 2a2c 0004                 move.l    4(a4),d5
[00011060] 2854                      movea.l   (a4),a4
[00011062] b9f9 0001 3b38            cmpa.l    $00013B38,a4
[00011068] 66ec                      bne.s     $00011056
[0001106a] 2005                      move.l    d5,d0
[0001106c] e780                      asl.l     #3,d0
[0001106e] 602c                      bra.s     $0001109C
[00011070] 2f3c 0000 0008            move.l    #$00000008,-(a7)
[00011076] 2f07                      move.l    d7,-(a7)
[00011078] 5e97                      addq.l    #7,(a7)
[0001107a] 4eb9 0001 1862            jsr       ldiv
[00011080] 508f                      addq.l    #8,a7
[00011082] 2c00                      move.l    d0,d6
[00011084] 5286                      addq.l    #1,d6
[00011086] 2e86                      move.l    d6,(a7)
[00011088] 611c                      bsr.s     $000110A6
[0001108a] 2a40                      movea.l   d0,a5
[0001108c] 200d                      move.l    a5,d0
[0001108e] 6604                      bne.s     $00011094
[00011090] 4280                      clr.l     d0
[00011092] 6008                      bra.s     $0001109C
[00011094] 2e86                      move.l    d6,(a7)
[00011096] 2f0d                      move.l    a5,-(a7)
[00011098] 6152                      bsr.s     $000110EC
[0001109a] 588f                      addq.l    #4,a7
[0001109c] 4a9f                      tst.l     (a7)+
[0001109e] 4cdf 30e0                 movem.l   (a7)+,d5-d7/a4-a5
[000110a2] 4e5e                      unlk      a6
[000110a4] 4e75                      rts

[000110a6] 4e56 0000                 link      a6,#$0000
[000110aa] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000110ae] 2e2e 0008                 move.l    8(a6),d7
[000110b2] 2879 0001 3b38            movea.l   $00013B38,a4
[000110b8] 2a54                      movea.l   (a4),a5
[000110ba] 6024                      bra.s     $000110E0
[000110bc] bead 0004                 cmp.l     4(a5),d7
[000110c0] 6e04                      bgt.s     $000110C6
[000110c2] 200c                      move.l    a4,d0
[000110c4] 601c                      bra.s     $000110E2
[000110c6] bbf9 0001 3b38            cmpa.l    $00013B38,a5
[000110cc] 660e                      bne.s     $000110DC
[000110ce] 2e87                      move.l    d7,(a7)
[000110d0] 6168                      bsr.s     $0001113A
[000110d2] 2a40                      movea.l   d0,a5
[000110d4] 200d                      move.l    a5,d0
[000110d6] 6604                      bne.s     $000110DC
[000110d8] 4280                      clr.l     d0
[000110da] 6006                      bra.s     $000110E2
[000110dc] 284d                      movea.l   a5,a4
[000110de] 2a55                      movea.l   (a5),a5
[000110e0] 60da                      bra.s     $000110BC
[000110e2] 4a9f                      tst.l     (a7)+
[000110e4] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000110e8] 4e5e                      unlk      a6
[000110ea] 4e75                      rts

[000110ec] 4e56 0000                 link      a6,#$0000
[000110f0] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[000110f4] 2a6e 0008                 movea.l   8(a6),a5
[000110f8] 2e2e 000c                 move.l    12(a6),d7
[000110fc] 2855                      movea.l   (a5),a4
[000110fe] 2007                      move.l    d7,d0
[00011100] 5880                      addq.l    #4,d0
[00011102] b0ac 0004                 cmp.l     4(a4),d0
[00011106] 6d04                      blt.s     $0001110C
[00011108] 2a94                      move.l    (a4),(a5)
[0001110a] 601a                      bra.s     $00011126
[0001110c] 2007                      move.l    d7,d0
[0001110e] e780                      asl.l     #3,d0
[00011110] 2640                      movea.l   d0,a3
[00011112] d7cc                      adda.l    a4,a3
[00011114] 202c 0004                 move.l    4(a4),d0
[00011118] 9087                      sub.l     d7,d0
[0001111a] 2740 0004                 move.l    d0,4(a3)
[0001111e] 2694                      move.l    (a4),(a3)
[00011120] 2a8b                      move.l    a3,(a5)
[00011122] 2947 0004                 move.l    d7,4(a4)
[00011126] 23cd 0001 3b38            move.l    a5,$00013B38
[0001112c] 200c                      move.l    a4,d0
[0001112e] 5080                      addq.l    #8,d0
[00011130] 4a9f                      tst.l     (a7)+
[00011132] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[00011136] 4e5e                      unlk      a6
[00011138] 4e75                      rts

[0001113a] 4e56 0000                 link      a6,#$0000
[0001113e] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011142] 2e2e 0008                 move.l    8(a6),d7
[00011146] 2f3c 0000 0040            move.l    #$00000040,-(a7)
[0001114c] 2f07                      move.l    d7,-(a7)
[0001114e] 0697 0000 003f            addi.l    #$0000003F,(a7)
[00011154] 4eb9 0001 1862            jsr       ldiv
[0001115a] 508f                      addq.l    #8,a7
[0001115c] 2c00                      move.l    d0,d6
[0001115e] ed86                      asl.l     #6,d6
[00011160] 2006                      move.l    d6,d0
[00011162] e780                      asl.l     #3,d0
[00011164] 2e80                      move.l    d0,(a7)
[00011166] 6100 014e                 bsr       $000112B6
[0001116a] 2a40                      movea.l   d0,a5
[0001116c] bbfc ffff ffff            cmpa.l    #$FFFFFFFF,a5
[00011172] 6604                      bne.s     $00011178
[00011174] 4280                      clr.l     d0
[00011176] 601e                      bra.s     $00011196
[00011178] 200d                      move.l    a5,d0
[0001117a] 5280                      addq.l    #1,d0
[0001117c] c0bc ffff fffe            and.l     #$FFFFFFFE,d0
[00011182] 2a40                      movea.l   d0,a5
[00011184] 284d                      movea.l   a5,a4
[00011186] 2946 0004                 move.l    d6,4(a4)
[0001118a] 2e8c                      move.l    a4,(a7)
[0001118c] 5097                      addq.l    #8,(a7)
[0001118e] 6110                      bsr.s     $000111A0
[00011190] 2039 0001 3b38            move.l    $00013B38,d0
[00011196] 4a9f                      tst.l     (a7)+
[00011198] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[0001119c] 4e5e                      unlk      a6
[0001119e] 4e75                      rts

[000111a0] 4e56 0000                 link      a6,#$0000
[000111a4] 48e7 011c                 movem.l   d7/a3-a5,-(a7)
[000111a8] 2a6e 0008                 movea.l   8(a6),a5
[000111ac] 49ed fff8                 lea.l     -8(a5),a4
[000111b0] 2679 0001 3b38            movea.l   $00013B38,a3
[000111b6] b9cb                      cmpa.l    a3,a4
[000111b8] 6504                      bcs.s     $000111BE
[000111ba] b9d3                      cmpa.l    (a3),a4
[000111bc] 6510                      bcs.s     $000111CE
[000111be] b7d3                      cmpa.l    (a3),a3
[000111c0] 6508                      bcs.s     $000111CA
[000111c2] b9d3                      cmpa.l    (a3),a4
[000111c4] 6308                      bls.s     $000111CE
[000111c6] b9cb                      cmpa.l    a3,a4
[000111c8] 6404                      bcc.s     $000111CE
[000111ca] 2653                      movea.l   (a3),a3
[000111cc] 60e8                      bra.s     $000111B6
[000111ce] b9cb                      cmpa.l    a3,a4
[000111d0] 651e                      bcs.s     $000111F0
[000111d2] 202c 0004                 move.l    4(a4),d0
[000111d6] e780                      asl.l     #3,d0
[000111d8] d08c                      add.l     a4,d0
[000111da] 222b 0004                 move.l    4(a3),d1
[000111de] e781                      asl.l     #3,d1
[000111e0] d28b                      add.l     a3,d1
[000111e2] b081                      cmp.l     d1,d0
[000111e4] 620a                      bhi.s     $000111F0
[000111e6] 23cb 0001 3b38            move.l    a3,$00013B38
[000111ec] 4240                      clr.w     d0
[000111ee] 6040                      bra.s     $00011230
[000111f0] 202c 0004                 move.l    4(a4),d0
[000111f4] e780                      asl.l     #3,d0
[000111f6] d08c                      add.l     a4,d0
[000111f8] b093                      cmp.l     (a3),d0
[000111fa] 6610                      bne.s     $0001120C
[000111fc] 2053                      movea.l   (a3),a0
[000111fe] 2028 0004                 move.l    4(a0),d0
[00011202] d1ac 0004                 add.l     d0,4(a4)
[00011206] 2053                      movea.l   (a3),a0
[00011208] 2890                      move.l    (a0),(a4)
[0001120a] 6002                      bra.s     $0001120E
[0001120c] 2893                      move.l    (a3),(a4)
[0001120e] 202b 0004                 move.l    4(a3),d0
[00011212] e780                      asl.l     #3,d0
[00011214] d08b                      add.l     a3,d0
[00011216] b9c0                      cmpa.l    d0,a4
[00011218] 660c                      bne.s     $00011226
[0001121a] 202c 0004                 move.l    4(a4),d0
[0001121e] d1ab 0004                 add.l     d0,4(a3)
[00011222] 2694                      move.l    (a4),(a3)
[00011224] 6002                      bra.s     $00011228
[00011226] 268c                      move.l    a4,(a3)
[00011228] 23cb 0001 3b38            move.l    a3,$00013B38
[0001122e] 4240                      clr.w     d0
[00011230] 4a9f                      tst.l     (a7)+
[00011232] 4cdf 3800                 movem.l   (a7)+,a3-a5
[00011236] 4e5e                      unlk      a6
[00011238] 4e75                      rts

[0001123a] 4e56 fffc                 link      a6,#$FFFC
[0001123e] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011242] 2a6e 0008                 movea.l   8(a6),a5
[00011246] 2e2e 000c                 move.l    12(a6),d7
[0001124a] 2e8d                      move.l    a5,(a7)
[0001124c] 6100 ff52                 bsr       $000111A0
[00011250] 2f3c 0000 0008            move.l    #$00000008,-(a7)
[00011256] 2f07                      move.l    d7,-(a7)
[00011258] 5e97                      addq.l    #7,(a7)
[0001125a] 4eb9 0001 1862            jsr       ldiv
[00011260] 508f                      addq.l    #8,a7
[00011262] 2c00                      move.l    d0,d6
[00011264] 5286                      addq.l    #1,d6
[00011266] 2e86                      move.l    d6,(a7)
[00011268] 6100 fe3c                 bsr       $000110A6
[0001126c] 2d40 fffc                 move.l    d0,-4(a6)
[00011270] 6604                      bne.s     $00011276
[00011272] 4280                      clr.l     d0
[00011274] 6036                      bra.s     $000112AC
[00011276] 206e fffc                 movea.l   -4(a6),a0
[0001127a] 2850                      movea.l   (a0),a4
[0001127c] 508c                      addq.l    #8,a4
[0001127e] bbcc                      cmpa.l    a4,a5
[00011280] 671e                      beq.s     $000112A0
[00011282] b9cd                      cmpa.l    a5,a4
[00011284] 640c                      bcc.s     $00011292
[00011286] 6004                      bra.s     $0001128C
[00011288] 18dd                      move.b    (a5)+,(a4)+
[0001128a] 5387                      subq.l    #1,d7
[0001128c] 4a87                      tst.l     d7
[0001128e] 66f8                      bne.s     $00011288
[00011290] 600e                      bra.s     $000112A0
[00011292] d9c7                      adda.l    d7,a4
[00011294] dbc7                      adda.l    d7,a5
[00011296] 6004                      bra.s     $0001129C
[00011298] 1925                      move.b    -(a5),-(a4)
[0001129a] 5387                      subq.l    #1,d7
[0001129c] 4a87                      tst.l     d7
[0001129e] 66f8                      bne.s     $00011298
[000112a0] 2e86                      move.l    d6,(a7)
[000112a2] 2f2e fffc                 move.l    -4(a6),-(a7)
[000112a6] 6100 fe44                 bsr       $000110EC
[000112aa] 588f                      addq.l    #4,a7
[000112ac] 4a9f                      tst.l     (a7)+
[000112ae] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[000112b2] 4e5e                      unlk      a6
[000112b4] 4e75                      rts

[000112b6] 4e56 fffc                 link      a6,#$FFFC
[000112ba] 48e7 0300                 movem.l   d6-d7,-(a7)
[000112be] 2e2e 0008                 move.l    8(a6),d7
[000112c2] 2d79 0001 3ec0 fffc       move.l    __break,-4(a6)
[000112ca] 0807 0000                 btst      #0,d7
[000112ce] 6702                      beq.s     $000112D2
[000112d0] 5287                      addq.l    #1,d7
[000112d2] 2039 0001 3ec0            move.l    __break,d0
[000112d8] d087                      add.l     d7,d0
[000112da] 2e80                      move.l    d0,(a7)
[000112dc] 4eb9 0001 00b2            jsr       _brk
[000112e2] 4a40                      tst.w     d0
[000112e4] 6704                      beq.s     $000112EA
[000112e6] 70ff                      moveq.l   #-1,d0
[000112e8] 6004                      bra.s     $000112EE
[000112ea] 202e fffc                 move.l    -4(a6),d0
[000112ee] 4a9f                      tst.l     (a7)+
[000112f0] 4cdf 0080                 movem.l   (a7)+,d7
[000112f4] 4e5e                      unlk      a6
[000112f6] 4e75                      rts

[000112f8] 4e56 0000                 link      a6,#$0000
[000112fc] 48e7 0104                 movem.l   d7/a5,-(a7)
[00011300] 2eae 0008                 move.l    8(a6),(a7)
[00011304] 6100 fd20                 bsr       $00011026
[00011308] 2a40                      movea.l   d0,a5
[0001130a] 200d                      move.l    a5,d0
[0001130c] 6604                      bne.s     $00011312
[0001130e] 4280                      clr.l     d0
[00011310] 600e                      bra.s     $00011320
[00011312] 2eae 0008                 move.l    8(a6),(a7)
[00011316] 4267                      clr.w     -(a7)
[00011318] 2f0d                      move.l    a5,-(a7)
[0001131a] 6136                      bsr.s     $00011352
[0001131c] 5c8f                      addq.l    #6,a7
[0001131e] 200d                      move.l    a5,d0
[00011320] 4a9f                      tst.l     (a7)+
[00011322] 4cdf 2000                 movem.l   (a7)+,a5
[00011326] 4e5e                      unlk      a6
[00011328] 4e75                      rts

[0001132a] 4e56 0000                 link      a6,#$0000
[0001132e] 48e7 0300                 movem.l   d6-d7,-(a7)
[00011332] 2f2e 0008                 move.l    8(a6),-(a7)
[00011336] 2f2e 000c                 move.l    12(a6),-(a7)
[0001133a] 4eb9 0001 18e4            jsr       lmul
[00011340] 508f                      addq.l    #8,a7
[00011342] 2e00                      move.l    d0,d7
[00011344] 2e87                      move.l    d7,(a7)
[00011346] 61b0                      bsr.s     $000112F8
[00011348] 4a9f                      tst.l     (a7)+
[0001134a] 4cdf 0080                 movem.l   (a7)+,d7
[0001134e] 4e5e                      unlk      a6
[00011350] 4e75                      rts

[00011352] 4e56 0000                 link      a6,#$0000
[00011356] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[0001135a] 2a6e 0008                 movea.l   8(a6),a5
[0001135e] 1e2e 000d                 move.b    13(a6),d7
[00011362] 2c2e 000e                 move.l    14(a6),d6
[00011366] 6002                      bra.s     $0001136A
[00011368] 1ac7                      move.b    d7,(a5)+
[0001136a] 2006                      move.l    d6,d0
[0001136c] 5386                      subq.l    #1,d6
[0001136e] 4a80                      tst.l     d0
[00011370] 66f6                      bne.s     $00011368
[00011372] 4a9f                      tst.l     (a7)+
[00011374] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00011378] 4e5e                      unlk      a6
[0001137a] 4e75                      rts

[0001137c] 4e56 0000                 link      a6,#$0000
[00011380] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00011384] 2e2e 0008                 move.l    8(a6),d7
[00011388] 2e87                      move.l    d7,(a7)
[0001138a] 4eb9 0001 1026            jsr       $00011026
[00011390] 2a40                      movea.l   d0,a5
[00011392] 4a79 0001 376e            tst.w     $0001376E
[00011398] 6712                      beq.s     $000113AC
[0001139a] 2e8d                      move.l    a5,(a7)
[0001139c] 2f07                      move.l    d7,-(a7)
[0001139e] 2f3c 0001 3b3c            move.l    #$00013B3C,-(a7)
[000113a4] 4eb9 0001 3592            jsr       _printf
[000113aa] 508f                      addq.l    #8,a7
[000113ac] 200d                      move.l    a5,d0
[000113ae] 4a9f                      tst.l     (a7)+
[000113b0] 4cdf 2080                 movem.l   (a7)+,d7/a5
[000113b4] 4e5e                      unlk      a6
[000113b6] 4e75                      rts

[000113b8] 4e56 0000                 link      a6,#$0000
[000113bc] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[000113c0] 2e2e 0008                 move.l    8(a6),d7
[000113c4] 2c2e 000c                 move.l    12(a6),d6
[000113c8] 2e86                      move.l    d6,(a7)
[000113ca] 2f07                      move.l    d7,-(a7)
[000113cc] 4eb9 0001 132a            jsr       $0001132A
[000113d2] 588f                      addq.l    #4,a7
[000113d4] 2a40                      movea.l   d0,a5
[000113d6] 4a79 0001 376e            tst.w     $0001376E
[000113dc] 6718                      beq.s     $000113F6
[000113de] 2e8d                      move.l    a5,(a7)
[000113e0] 2f06                      move.l    d6,-(a7)
[000113e2] 2f07                      move.l    d7,-(a7)
[000113e4] 2f3c 0001 3b50            move.l    #$00013B50,-(a7)
[000113ea] 4eb9 0001 3592            jsr       _printf
[000113f0] dffc 0000 000c            adda.l    #$0000000C,a7
[000113f6] 200d                      move.l    a5,d0
[000113f8] 4a9f                      tst.l     (a7)+
[000113fa] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[000113fe] 4e5e                      unlk      a6
[00011400] 4e75                      rts

[00011402] 4e56 0000                 link      a6,#$0000
[00011406] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001140a] 2a6e 0008                 movea.l   8(a6),a5
[0001140e] 2e2e 000c                 move.l    12(a6),d7
[00011412] 2e87                      move.l    d7,(a7)
[00011414] 2f0d                      move.l    a5,-(a7)
[00011416] 4eb9 0001 123a            jsr       $0001123A
[0001141c] 588f                      addq.l    #4,a7
[0001141e] 2840                      movea.l   d0,a4
[00011420] 4a79 0001 376e            tst.w     $0001376E
[00011426] 6718                      beq.s     $00011440
[00011428] 2e8c                      move.l    a4,(a7)
[0001142a] 2f07                      move.l    d7,-(a7)
[0001142c] 2f0d                      move.l    a5,-(a7)
[0001142e] 2f3c 0001 3b68            move.l    #$00013B68,-(a7)
[00011434] 4eb9 0001 3592            jsr       _printf
[0001143a] dffc 0000 000c            adda.l    #$0000000C,a7
[00011440] 200c                      move.l    a4,d0
[00011442] 4a9f                      tst.l     (a7)+
[00011444] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00011448] 4e5e                      unlk      a6
[0001144a] 4e75                      rts

[0001144c] 4e56 0000                 link      a6,#$0000
[00011450] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00011454] 2a6e 0008                 movea.l   8(a6),a5
[00011458] 4a79 0001 376e            tst.w     $0001376E
[0001145e] 6710                      beq.s     $00011470
[00011460] 2e8d                      move.l    a5,(a7)
[00011462] 2f3c 0001 3b81            move.l    #$00013B81,-(a7)
[00011468] 4eb9 0001 3592            jsr       _printf
[0001146e] 588f                      addq.l    #4,a7
[00011470] 4a79 0001 3770            tst.w     $00013770
[00011476] 6704                      beq.s     $0001147C
[00011478] 4247                      clr.w     d7
[0001147a] 600a                      bra.s     $00011486
[0001147c] 2e8d                      move.l    a5,(a7)
[0001147e] 4eb9 0001 11a0            jsr       $000111A0
[00011484] 3e00                      move.w    d0,d7
[00011486] 3007                      move.w    d7,d0
[00011488] 4a9f                      tst.l     (a7)+
[0001148a] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0001148e] 4e5e                      unlk      a6
[00011490] 4e75                      rts
_xbios:
[00011492] 23df 0001 3ede            move.l    (a7)+,$00013EDE
[00011498] 4e4e                      trap      #14
[0001149a] 2f39 0001 3ede            move.l    $00013EDE,-(a7)
[000114a0] 4e75                      rts
_bios:
[000114a2] 23df 0001 3ede            move.l    (a7)+,$00013EDE
[000114a8] 4e4d                      trap      #13
[000114aa] 2f39 0001 3ede            move.l    $00013EDE,-(a7)
[000114b0] 4e75                      rts
_gemdos:
[000114b2] 23df 0001 3ede            move.l    (a7)+,$00013EDE
[000114b8] 4e41                      trap      #1
[000114ba] 2f39 0001 3ede            move.l    $00013EDE,-(a7)
[000114c0] 4e75                      rts
__main:
[000114c2] 4e56 0000                 link      a6,#$0000
[000114c6] 48e7 0104                 movem.l   d7/a5,-(a7)
[000114ca] 4eb9 0001 2178            jsr       __chinit
[000114d0] 4257                      clr.w     (a7)
[000114d2] 2f3c 0001 35ed            move.l    #$000135ED,-(a7)
[000114d8] 4eb9 0001 3154            jsr       _open
[000114de] 588f                      addq.l    #4,a7
[000114e0] 3ebc 0001                 move.w    #$0001,(a7)
[000114e4] 2f3c 0001 35ed            move.l    #$000135ED,-(a7)
[000114ea] 4eb9 0001 3154            jsr       _open
[000114f0] 588f                      addq.l    #4,a7
[000114f2] 3ebc 0001                 move.w    #$0001,(a7)
[000114f6] 2f3c 0001 35ed            move.l    #$000135ED,-(a7)
[000114fc] 4eb9 0001 3154            jsr       _open
[00011502] 588f                      addq.l    #4,a7
[00011504] 306e 000c                 movea.w   12(a6),a0
[00011508] d1ee 0008                 adda.l    8(a6),a0
[0001150c] 4210                      clr.b     (a0)
[0001150e] 3eae 000c                 move.w    12(a6),(a7)
[00011512] 2f2e 0008                 move.l    8(a6),-(a7)
[00011516] 4eb9 0001 1528            jsr       ___main
[0001151c] 588f                      addq.l    #4,a7
[0001151e] 4a9f                      tst.l     (a7)+
[00011520] 4cdf 2000                 movem.l   (a7)+,a5
[00011524] 4e5e                      unlk      a6
[00011526] 4e75                      rts
___main:
[00011528] 4e56 ffdc                 link      a6,#$FFDC
[0001152c] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011530] 4257                      clr.w     (a7)
[00011532] 4eb9 0001 2306            jsr       _sbrk
[00011538] 23c0 0001 3ee4            move.l    d0,$00013EE4
[0001153e] 23c0 0001 3ee8            move.l    d0,$00013EE8
[00011544] 4279 0001 3ee2            clr.w     $00013EE2
[0001154a] 2ebc 0001 35e5            move.l    #$000135E5,(a7)
[00011550] 6100 02bc                 bsr       _addargv
[00011554] 2a6e 0008                 movea.l   8(a6),a5
[00011558] 6000 01f4                 bra       $0001174E
[0001155c] 6002                      bra.s     $00011560
[0001155e] 528d                      addq.l    #1,a5
[00011560] 4a15                      tst.b     (a5)
[00011562] 6714                      beq.s     $00011578
[00011564] 1015                      move.b    (a5),d0
[00011566] 4880                      ext.w     d0
[00011568] 48c0                      ext.l     d0
[0001156a] d0bc 0001 3c4e            add.l     #$00013C4E,d0
[00011570] 2040                      movea.l   d0,a0
[00011572] 0810 0005                 btst      #5,(a0)
[00011576] 66e6                      bne.s     $0001155E
[00011578] 1c15                      move.b    (a5),d6
[0001157a] 4a06                      tst.b     d6
[0001157c] 6700 01d6                 beq       $00011754
[00011580] 1006                      move.b    d6,d0
[00011582] 4880                      ext.w     d0
[00011584] 3e80                      move.w    d0,(a7)
[00011586] 2f3c 0001 3c12            move.l    #$00013C12,-(a7)
[0001158c] 4eb9 0001 012c            jsr       _strchr
[00011592] 588f                      addq.l    #4,a7
[00011594] 4a80                      tst.l     d0
[00011596] 6740                      beq.s     $000115D8
[00011598] 1006                      move.b    d6,d0
[0001159a] 4880                      ext.w     d0
[0001159c] 3e80                      move.w    d0,(a7)
[0001159e] 2f0d                      move.l    a5,-(a7)
[000115a0] 5297                      addq.l    #1,(a7)
[000115a2] 4eb9 0001 012c            jsr       _strchr
[000115a8] 588f                      addq.l    #4,a7
[000115aa] 2840                      movea.l   d0,a4
[000115ac] 200c                      move.l    a4,d0
[000115ae] 660e                      bne.s     $000115BE
[000115b0] 2ebc 0001 3c15            move.l    #$00013C15,(a7)
[000115b6] 2f0d                      move.l    a5,-(a7)
[000115b8] 6100 01f0                 bsr       __err
[000115bc] 588f                      addq.l    #4,a7
[000115be] 200c                      move.l    a4,d0
[000115c0] 908d                      sub.l     a5,d0
[000115c2] 3e00                      move.w    d0,d7
[000115c4] 3047                      movea.w   d7,a0
[000115c6] d1cd                      adda.l    a5,a0
[000115c8] 4210                      clr.b     (a0)
[000115ca] 5247                      addq.w    #1,d7
[000115cc] 2e8d                      move.l    a5,(a7)
[000115ce] 5297                      addq.l    #1,(a7)
[000115d0] 6100 023c                 bsr       _addargv
[000115d4] 6000 0176                 bra       $0001174C
[000115d8] 4247                      clr.w     d7
[000115da] 6002                      bra.s     $000115DE
[000115dc] 5247                      addq.w    #1,d7
[000115de] 3047                      movea.w   d7,a0
[000115e0] d1cd                      adda.l    a5,a0
[000115e2] 4a10                      tst.b     (a0)
[000115e4] 671a                      beq.s     $00011600
[000115e6] 3007                      move.w    d7,d0
[000115e8] 48c0                      ext.l     d0
[000115ea] 1035 0800                 move.b    0(a5,d0.l),d0
[000115ee] 4880                      ext.w     d0
[000115f0] 48c0                      ext.l     d0
[000115f2] d0bc 0001 3c4e            add.l     #$00013C4E,d0
[000115f8] 2040                      movea.l   d0,a0
[000115fa] 0810 0005                 btst      #5,(a0)
[000115fe] 67dc                      beq.s     $000115DC
[00011600] 3007                      move.w    d7,d0
[00011602] 48c0                      ext.l     d0
[00011604] 4a35 0800                 tst.b     0(a5,d0.l)
[00011608] 6708                      beq.s     $00011612
[0001160a] 3047                      movea.w   d7,a0
[0001160c] d1cd                      adda.l    a5,a0
[0001160e] 4210                      clr.b     (a0)
[00011610] 5247                      addq.w    #1,d7
[00011612] 3ebc 003f                 move.w    #$003F,(a7)
[00011616] 2f0d                      move.l    a5,-(a7)
[00011618] 4eb9 0001 012c            jsr       _strchr
[0001161e] 588f                      addq.l    #4,a7
[00011620] 4a80                      tst.l     d0
[00011622] 6614                      bne.s     $00011638
[00011624] 3ebc 002a                 move.w    #$002A,(a7)
[00011628] 2f0d                      move.l    a5,-(a7)
[0001162a] 4eb9 0001 012c            jsr       _strchr
[00011630] 588f                      addq.l    #4,a7
[00011632] 4a80                      tst.l     d0
[00011634] 6700 0110                 beq       $00011746
[00011638] 2d7c 0001 45a4 fffc       move.l    #$000145A4,-4(a6)
[00011640] 2eae fffc                 move.l    -4(a6),(a7)
[00011644] 0697 0000 0038            addi.l    #$00000038,(a7)
[0001164a] 3f3c 001a                 move.w    #$001A,-(a7)
[0001164e] 4eb9 0001 1938            jsr       _trap
[00011654] 548f                      addq.l    #2,a7
[00011656] 3ebc 0011                 move.w    #$0011,(a7)
[0001165a] 2f0d                      move.l    a5,-(a7)
[0001165c] 3f3c 0003                 move.w    #$0003,-(a7)
[00011660] 4eb9 0001 1be0            jsr       ___open
[00011666] 5c8f                      addq.l    #6,a7
[00011668] 4880                      ext.w     d0
[0001166a] 1c00                      move.b    d0,d6
[0001166c] bc3c 00ff                 cmp.b     #$FF,d6
[00011670] 660e                      bne.s     $00011680
[00011672] 2ebc 0001 3c27            move.l    #$00013C27,(a7)
[00011678] 2f0d                      move.l    a5,-(a7)
[0001167a] 6100 012e                 bsr       __err
[0001167e] 588f                      addq.l    #4,a7
[00011680] 2e8d                      move.l    a5,(a7)
[00011682] 4eb9 0001 1ae4            jsr       _strlen
[00011688] 3d40 ffdc                 move.w    d0,-36(a6)
[0001168c] 6026                      bra.s     $000116B4
[0001168e] 302e ffdc                 move.w    -36(a6),d0
[00011692] 5340                      subq.w    #1,d0
[00011694] 48c0                      ext.l     d0
[00011696] 1035 0800                 move.b    0(a5,d0.l),d0
[0001169a] 4880                      ext.w     d0
[0001169c] 3e80                      move.w    d0,(a7)
[0001169e] 2f3c 0001 3c32            move.l    #$00013C32,-(a7)
[000116a4] 4eb9 0001 012c            jsr       _strchr
[000116aa] 588f                      addq.l    #4,a7
[000116ac] 4a80                      tst.l     d0
[000116ae] 660a                      bne.s     $000116BA
[000116b0] 536e ffdc                 subq.w    #1,-36(a6)
[000116b4] 4a6e ffdc                 tst.w     -36(a6)
[000116b8] 66d4                      bne.s     $0001168E
[000116ba] 6000 0080                 bra       $0001173C
[000116be] 2e8e                      move.l    a6,(a7)
[000116c0] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[000116c6] 1006                      move.b    d6,d0
[000116c8] 4880                      ext.w     d0
[000116ca] 3f00                      move.w    d0,-(a7)
[000116cc] 2f2e fffc                 move.l    -4(a6),-(a7)
[000116d0] 6100 016a                 bsr       __toasc
[000116d4] 5c8f                      addq.l    #6,a7
[000116d6] 2e8e                      move.l    a6,(a7)
[000116d8] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[000116de] 4eb9 0001 1ae4            jsr       _strlen
[000116e4] 3e80                      move.w    d0,(a7)
[000116e6] 302e ffdc                 move.w    -36(a6),d0
[000116ea] d157                      add.w     d0,(a7)
[000116ec] 5257                      addq.w    #1,(a7)
[000116ee] 4eb9 0001 1a3c            jsr       __salloc
[000116f4] 2840                      movea.l   d0,a4
[000116f6] 3eae ffdc                 move.w    -36(a6),(a7)
[000116fa] 2f0d                      move.l    a5,-(a7)
[000116fc] 2f0c                      move.l    a4,-(a7)
[000116fe] 4eb9 0001 1b2c            jsr       _strncpy
[00011704] 508f                      addq.l    #8,a7
[00011706] 306e ffdc                 movea.w   -36(a6),a0
[0001170a] d1cc                      adda.l    a4,a0
[0001170c] 4210                      clr.b     (a0)
[0001170e] 2e8e                      move.l    a6,(a7)
[00011710] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[00011716] 2f0c                      move.l    a4,-(a7)
[00011718] 4eb9 0001 1bb6            jsr       _strcat
[0001171e] 588f                      addq.l    #4,a7
[00011720] 2e8c                      move.l    a4,(a7)
[00011722] 6100 00ea                 bsr       _addargv
[00011726] 3ebc 0012                 move.w    #$0012,(a7)
[0001172a] 2f0d                      move.l    a5,-(a7)
[0001172c] 3f3c 0003                 move.w    #$0003,-(a7)
[00011730] 4eb9 0001 1be0            jsr       ___open
[00011736] 5c8f                      addq.l    #6,a7
[00011738] 4880                      ext.w     d0
[0001173a] 1c00                      move.b    d0,d6
[0001173c] bc3c 00ff                 cmp.b     #$FF,d6
[00011740] 6600 ff7c                 bne       $000116BE
[00011744] 6006                      bra.s     $0001174C
[00011746] 2e8d                      move.l    a5,(a7)
[00011748] 6100 00c4                 bsr       _addargv
[0001174c] dac7                      adda.w    d7,a5
[0001174e] 4a15                      tst.b     (a5)
[00011750] 6600 fe0a                 bne       $0001155C
[00011754] 4297                      clr.l     (a7)
[00011756] 6100 00b6                 bsr       _addargv
[0001175a] 5379 0001 3ee2            subq.w    #1,$00013EE2
[00011760] 2eb9 0001 3ee8            move.l    $00013EE8,(a7)
[00011766] 4eb9 0001 00b2            jsr       _brk
[0001176c] b07c ffff                 cmp.w     #$FFFF,d0
[00011770] 6610                      bne.s     $00011782
[00011772] 2ebc 0001 3c44            move.l    #$00013C44,(a7)
[00011778] 2f3c 0001 3c35            move.l    #$00013C35,-(a7)
[0001177e] 612a                      bsr.s     __err
[00011780] 588f                      addq.l    #4,a7
[00011782] 4297                      clr.l     (a7)
[00011784] 2f39 0001 3ee4            move.l    $00013EE4,-(a7)
[0001178a] 3f39 0001 3ee2            move.w    $00013EE2,-(a7)
[00011790] 4eb9 0001 0c08            jsr       $00010C08
[00011796] 5c8f                      addq.l    #6,a7
[00011798] 3e80                      move.w    d0,(a7)
[0001179a] 4eb9 0001 3352            jsr       _exit
[000117a0] 4a9f                      tst.l     (a7)+
[000117a2] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[000117a6] 4e5e                      unlk      a6
[000117a8] 4e75                      rts
__err:
[000117aa] 4e56 ff7c                 link      a6,#$FF7C
[000117ae] 2eae 0008                 move.l    8(a6),(a7)
[000117b2] 2f0e                      move.l    a6,-(a7)
[000117b4] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[000117ba] 4eb9 0001 1b0a            jsr       _strcpy
[000117c0] 588f                      addq.l    #4,a7
[000117c2] 2eae 000c                 move.l    12(a6),(a7)
[000117c6] 2f0e                      move.l    a6,-(a7)
[000117c8] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[000117ce] 4eb9 0001 1bb6            jsr       _strcat
[000117d4] 588f                      addq.l    #4,a7
[000117d6] 2ebc 0001 3c45            move.l    #$00013C45,(a7)
[000117dc] 2f0e                      move.l    a6,-(a7)
[000117de] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[000117e4] 4eb9 0001 1bb6            jsr       _strcat
[000117ea] 588f                      addq.l    #4,a7
[000117ec] 2e8e                      move.l    a6,(a7)
[000117ee] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[000117f4] 3f3c 0009                 move.w    #$0009,-(a7)
[000117f8] 4eb9 0001 1938            jsr       _trap
[000117fe] 548f                      addq.l    #2,a7
[00011800] 3ebc ffff                 move.w    #$FFFF,(a7)
[00011804] 4eb9 0001 3352            jsr       _exit
[0001180a] 4e5e                      unlk      a6
[0001180c] 4e75                      rts
_addargv:
[0001180e] 4e56 0000                 link      a6,#$0000
[00011812] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00011816] 2a6e 0008                 movea.l   8(a6),a5
[0001181a] 200d                      move.l    a5,d0
[0001181c] 6708                      beq.s     $00011826
[0001181e] 2079 0001 3ee8            movea.l   $00013EE8,a0
[00011824] 208d                      move.l    a5,(a0)
[00011826] 58b9 0001 3ee8            addq.l    #4,$00013EE8
[0001182c] 5279 0001 3ee2            addq.w    #1,$00013EE2
[00011832] 4a9f                      tst.l     (a7)+
[00011834] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00011838] 4e5e                      unlk      a6
[0001183a] 4e75                      rts
__toasc:
[0001183c] 4e56 0000                 link      a6,#$0000
[00011840] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[00011844] 2a6e 0008                 movea.l   8(a6),a5
[00011848] 1e2e 000d                 move.b    13(a6),d7
[0001184c] 286e 000e                 movea.l   14(a6),a4
[00011850] 47ed 0056                 lea.l     86(a5),a3
[00011854] 18db                      move.b    (a3)+,(a4)+
[00011856] 66fc                      bne.s     $00011854
[00011858] 4a9f                      tst.l     (a7)+
[0001185a] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[0001185e] 4e5e                      unlk      a6
[00011860] 4e75                      rts
ldiv:
[00011862] 2043                      movea.l   d3,a0
[00011864] 222f 0004                 move.l    4(a7),d1
[00011868] 6c02                      bge.s     $0001186C
[0001186a] 4481                      neg.l     d1
[0001186c] 262f 0008                 move.l    8(a7),d3
[00011870] 6e0e                      bgt.s     $00011880
[00011872] 6d0a                      blt.s     $0001187E
[00011874] 203c 8000 0000            move.l    #$80000000,d0
[0001187a] 2200                      move.l    d0,d1
[0001187c] 605c                      bra.s     $000118DA
[0001187e] 4483                      neg.l     d3
[00011880] b6bc 0000 0001            cmp.l     #$00000001,d3
[00011886] 6606                      bne.s     $0001188E
[00011888] 2001                      move.l    d1,d0
[0001188a] 7200                      moveq.l   #0,d1
[0001188c] 603a                      bra.s     $000118C8
[0001188e] 7000                      moveq.l   #0,d0
[00011890] b681                      cmp.l     d1,d3
[00011892] 6d08                      blt.s     $0001189C
[00011894] 6e32                      bgt.s     $000118C8
[00011896] 7001                      moveq.l   #1,d0
[00011898] 7200                      moveq.l   #0,d1
[0001189a] 602c                      bra.s     $000118C8
[0001189c] 7402                      moveq.l   #2,d2
[0001189e] b2bc 0001 0000            cmp.l     #$00010000,d1
[000118a4] 6c0c                      bge.s     $000118B2
[000118a6] 82c3                      divu.w    d3,d1
[000118a8] 3001                      move.w    d1,d0
[000118aa] 4241                      clr.w     d1
[000118ac] 4841                      swap      d1
[000118ae] 6018                      bra.s     $000118C8
[000118b0] d482                      add.l     d2,d2
[000118b2] d683                      add.l     d3,d3
[000118b4] b283                      cmp.l     d3,d1
[000118b6] 64f8                      bcc.s     $000118B0
[000118b8] 6008                      bra.s     $000118C2
[000118ba] b283                      cmp.l     d3,d1
[000118bc] 6504                      bcs.s     $000118C2
[000118be] 8082                      or.l      d2,d0
[000118c0] 9283                      sub.l     d3,d1
[000118c2] e28b                      lsr.l     #1,d3
[000118c4] e28a                      lsr.l     #1,d2
[000118c6] 66f2                      bne.s     $000118BA
[000118c8] 4a6f 0004                 tst.w     4(a7)
[000118cc] 6a04                      bpl.s     $000118D2
[000118ce] 4480                      neg.l     d0
[000118d0] 4481                      neg.l     d1
[000118d2] 4a6f 0008                 tst.w     8(a7)
[000118d6] 6a02                      bpl.s     $000118DA
[000118d8] 4480                      neg.l     d0
[000118da] 23c1 0001 3ef4            move.l    d1,$00013EF4
[000118e0] 2608                      move.l    a0,d3
[000118e2] 4e75                      rts
lmul:
[000118e4] 4e56 fffc                 link      a6,#$FFFC
[000118e8] 4242                      clr.w     d2
[000118ea] 4aae 0008                 tst.l     8(a6)
[000118ee] 6c06                      bge.s     $000118F6
[000118f0] 44ae 0008                 neg.l     8(a6)
[000118f4] 5242                      addq.w    #1,d2
[000118f6] 4aae 000c                 tst.l     12(a6)
[000118fa] 6c06                      bge.s     $00011902
[000118fc] 44ae 000c                 neg.l     12(a6)
[00011900] 5242                      addq.w    #1,d2
[00011902] 302e 000a                 move.w    10(a6),d0
[00011906] c0ee 000e                 mulu.w    14(a6),d0
[0001190a] 2d40 fffc                 move.l    d0,-4(a6)
[0001190e] 302e 0008                 move.w    8(a6),d0
[00011912] c0ee 000e                 mulu.w    14(a6),d0
[00011916] 322e 000c                 move.w    12(a6),d1
[0001191a] c2ee 000a                 mulu.w    10(a6),d1
[0001191e] d041                      add.w     d1,d0
[00011920] d06e fffc                 add.w     -4(a6),d0
[00011924] 3d40 fffc                 move.w    d0,-4(a6)
[00011928] 202e fffc                 move.l    -4(a6),d0
[0001192c] 0802 0000                 btst      #0,d2
[00011930] 6702                      beq.s     $00011934
[00011932] 4480                      neg.l     d0
[00011934] 4e5e                      unlk      a6
[00011936] 4e75                      rts
_trap:
[00011938] 23df 0001 3eec            move.l    (a7)+,$00013EEC
[0001193e] 4e41                      trap      #1
[00011940] 2f39 0001 3eec            move.l    $00013EEC,-(a7)
[00011946] 4e75                      rts
__pc_rea:
[00011948] 4e56 fffa                 link      a6,#$FFFA
[0001194c] 206e 0008                 movea.l   8(a6),a0
[00011950] 3d68 fff2 fffe            move.w    -14(a0),-2(a6)
[00011956] 2eae 0010                 move.l    16(a6),(a7)
[0001195a] 306e 0016                 movea.w   22(a6),a0
[0001195e] 2f08                      move.l    a0,-(a7)
[00011960] 306e 0014                 movea.w   20(a6),a0
[00011964] 2f08                      move.l    a0,-(a7)
[00011966] 4eb9 0001 18e4            jsr       lmul
[0001196c] 508f                      addq.l    #8,a7
[0001196e] 2f00                      move.l    d0,-(a7)
[00011970] 3f2e fffe                 move.w    -2(a6),-(a7)
[00011974] 3f3c 003f                 move.w    #$003F,-(a7)
[00011978] 4eb9 0001 1938            jsr       _trap
[0001197e] 508f                      addq.l    #8,a7
[00011980] 4e5e                      unlk      a6
[00011982] 4e75                      rts
__pc_wri:
[00011984] 4e56 fffa                 link      a6,#$FFFA
[00011988] 206e 0008                 movea.l   8(a6),a0
[0001198c] 3d68 fff2 fffe            move.w    -14(a0),-2(a6)
[00011992] 2eae 0010                 move.l    16(a6),(a7)
[00011996] 306e 0016                 movea.w   22(a6),a0
[0001199a] 2f08                      move.l    a0,-(a7)
[0001199c] 306e 0014                 movea.w   20(a6),a0
[000119a0] 2f08                      move.l    a0,-(a7)
[000119a2] 4eb9 0001 18e4            jsr       lmul
[000119a8] 508f                      addq.l    #8,a7
[000119aa] 2f00                      move.l    d0,-(a7)
[000119ac] 3f2e fffe                 move.w    -2(a6),-(a7)
[000119b0] 3f3c 0040                 move.w    #$0040,-(a7)
[000119b4] 4eb9 0001 1938            jsr       _trap
[000119ba] 508f                      addq.l    #8,a7
[000119bc] 4e5e                      unlk      a6
[000119be] 4e75                      rts
_uldiv:
[000119c0] 4e56 0000                 link      a6,#$0000
[000119c4] 48e7 1f00                 movem.l   d3-d7,-(a7)
[000119c8] 2e2e 0008                 move.l    8(a6),d7
[000119cc] 2c2e 000c                 move.l    12(a6),d6
[000119d0] 4a86                      tst.l     d6
[000119d2] 6616                      bne.s     $000119EA
[000119d4] 23fc 8000 0000 0001 3c4a  move.l    #$80000000,$00013C4A
[000119de] 203c 8000 0000            move.l    #$80000000,d0
[000119e4] 80fc 0000                 divu.w    #$0000,d0
[000119e8] 6048                      bra.s     $00011A32
[000119ea] bc87                      cmp.l     d7,d6
[000119ec] 630a                      bls.s     $000119F8
[000119ee] 23c7 0001 3c4a            move.l    d7,$00013C4A
[000119f4] 4280                      clr.l     d0
[000119f6] 603a                      bra.s     $00011A32
[000119f8] be86                      cmp.l     d6,d7
[000119fa] 6606                      bne.s     $00011A02
[000119fc] 7a01                      moveq.l   #1,d5
[000119fe] 4287                      clr.l     d7
[00011a00] 6028                      bra.s     $00011A2A
[00011a02] 7801                      moveq.l   #1,d4
[00011a04] be86                      cmp.l     d6,d7
[00011a06] 650e                      bcs.s     $00011A16
[00011a08] 2006                      move.l    d6,d0
[00011a0a] e380                      asl.l     #1,d0
[00011a0c] bc80                      cmp.l     d0,d6
[00011a0e] 6206                      bhi.s     $00011A16
[00011a10] e386                      asl.l     #1,d6
[00011a12] e384                      asl.l     #1,d4
[00011a14] 60ee                      bra.s     $00011A04
[00011a16] 4285                      clr.l     d5
[00011a18] 4a84                      tst.l     d4
[00011a1a] 670e                      beq.s     $00011A2A
[00011a1c] be86                      cmp.l     d6,d7
[00011a1e] 6504                      bcs.s     $00011A24
[00011a20] 8a84                      or.l      d4,d5
[00011a22] 9e86                      sub.l     d6,d7
[00011a24] e28c                      lsr.l     #1,d4
[00011a26] e28e                      lsr.l     #1,d6
[00011a28] 60ee                      bra.s     $00011A18
[00011a2a] 23c7 0001 3c4a            move.l    d7,$00013C4A
[00011a30] 2005                      move.l    d5,d0
[00011a32] 4a9f                      tst.l     (a7)+
[00011a34] 4cdf 00f0                 movem.l   (a7)+,d4-d7
[00011a38] 4e5e                      unlk      a6
[00011a3a] 4e75                      rts
__salloc:
[00011a3c] 205f                      movea.l   (a7)+,a0
[00011a3e] 4280                      clr.l     d0
[00011a40] 301f                      move.w    (a7)+,d0
[00011a42] 5a80                      addq.l    #5,d0
[00011a44] 0880 0000                 bclr      #0,d0
[00011a48] 2279 0001 3ec0            movea.l   __break,a1
[00011a4e] 43e9 0100                 lea.l     256(a1),a1
[00011a52] 43f1 0800                 lea.l     0(a1,d0.l),a1
[00011a56] bfc9                      cmpa.l    a1,a7
[00011a58] 6206                      bhi.s     $00011A60
[00011a5a] 4ef9 0001 00f8            jmp       $000100F8
[00011a60] 9fc0                      suba.l    d0,a7
[00011a62] 43ef 0004                 lea.l     4(a7),a1
[00011a66] 2009                      move.l    a1,d0
[00011a68] 4ed0                      jmp       (a0)
__strcmp:
[00011a6a] 4e56 0000                 link      a6,#$0000
[00011a6e] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011a72] 2a6e 0008                 movea.l   8(a6),a5
[00011a76] 286e 000c                 movea.l   12(a6),a4
[00011a7a] 6024                      bra.s     $00011AA0
[00011a7c] 101d                      move.b    (a5)+,d0
[00011a7e] 4880                      ext.w     d0
[00011a80] 3e80                      move.w    d0,(a7)
[00011a82] 6130                      bsr.s     $00011AB4
[00011a84] 1e00                      move.b    d0,d7
[00011a86] 101c                      move.b    (a4)+,d0
[00011a88] 4880                      ext.w     d0
[00011a8a] 3e80                      move.w    d0,(a7)
[00011a8c] 6126                      bsr.s     $00011AB4
[00011a8e] 1c00                      move.b    d0,d6
[00011a90] be06                      cmp.b     d6,d7
[00011a92] 6f04                      ble.s     $00011A98
[00011a94] 7001                      moveq.l   #1,d0
[00011a96] 6012                      bra.s     $00011AAA
[00011a98] be06                      cmp.b     d6,d7
[00011a9a] 6c04                      bge.s     $00011AA0
[00011a9c] 70ff                      moveq.l   #-1,d0
[00011a9e] 600a                      bra.s     $00011AAA
[00011aa0] 4a15                      tst.b     (a5)
[00011aa2] 66d8                      bne.s     $00011A7C
[00011aa4] 4a14                      tst.b     (a4)
[00011aa6] 66d4                      bne.s     $00011A7C
[00011aa8] 4240                      clr.w     d0
[00011aaa] 4a9f                      tst.l     (a7)+
[00011aac] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00011ab0] 4e5e                      unlk      a6
[00011ab2] 4e75                      rts
[00011ab4] 4e56 0000                 link      a6,#$0000
[00011ab8] 48e7 0300                 movem.l   d6-d7,-(a7)
[00011abc] 3e2e 0008                 move.w    8(a6),d7
[00011ac0] be7c 0061                 cmp.w     #$0061,d7
[00011ac4] 6d0a                      blt.s     $00011AD0
[00011ac6] be7c 007a                 cmp.w     #$007A,d7
[00011aca] 6e04                      bgt.s     $00011AD0
[00011acc] de7c ffe0                 add.w     #$FFE0,d7
[00011ad0] 3007                      move.w    d7,d0
[00011ad2] 4a9f                      tst.l     (a7)+
[00011ad4] 4cdf 0080                 movem.l   (a7)+,d7
[00011ad8] 4e5e                      unlk      a6
[00011ada] 4e75                      rts
___fdecl:
[00011adc] 4e56 fffc                 link      a6,#$FFFC
[00011ae0] 4e5e                      unlk      a6
[00011ae2] 4e75                      rts
_strlen:
[00011ae4] 4e56 0000                 link      a6,#$0000
[00011ae8] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00011aec] 2a6e 0008                 movea.l   8(a6),a5
[00011af0] 284d                      movea.l   a5,a4
[00011af2] 6002                      bra.s     $00011AF6
[00011af4] 528c                      addq.l    #1,a4
[00011af6] 4a14                      tst.b     (a4)
[00011af8] 66fa                      bne.s     $00011AF4
[00011afa] 200c                      move.l    a4,d0
[00011afc] 48c0                      ext.l     d0
[00011afe] 908d                      sub.l     a5,d0
[00011b00] 4a9f                      tst.l     (a7)+
[00011b02] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00011b06] 4e5e                      unlk      a6
[00011b08] 4e75                      rts
_strcpy:
[00011b0a] 4e56 0000                 link      a6,#$0000
[00011b0e] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00011b12] 2a6e 000c                 movea.l   12(a6),a5
[00011b16] 286e 0008                 movea.l   8(a6),a4
[00011b1a] 18dd                      move.b    (a5)+,(a4)+
[00011b1c] 66fc                      bne.s     $00011B1A
[00011b1e] 202e 0008                 move.l    8(a6),d0
[00011b22] 4a9f                      tst.l     (a7)+
[00011b24] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00011b28] 4e5e                      unlk      a6
[00011b2a] 4e75                      rts
_strncpy:
[00011b2c] 4e56 0000                 link      a6,#$0000
[00011b30] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00011b34] 2a6e 000c                 movea.l   12(a6),a5
[00011b38] 286e 0008                 movea.l   8(a6),a4
[00011b3c] 6004                      bra.s     $00011B42
[00011b3e] 528c                      addq.l    #1,a4
[00011b40] 528d                      addq.l    #1,a5
[00011b42] 302e 0010                 move.w    16(a6),d0
[00011b46] 536e 0010                 subq.w    #1,16(a6)
[00011b4a] 4a40                      tst.w     d0
[00011b4c] 6f04                      ble.s     $00011B52
[00011b4e] 1895                      move.b    (a5),(a4)
[00011b50] 66ec                      bne.s     $00011B3E
[00011b52] 526e 0010                 addq.w    #1,16(a6)
[00011b56] 6002                      bra.s     $00011B5A
[00011b58] 421c                      clr.b     (a4)+
[00011b5a] 302e 0010                 move.w    16(a6),d0
[00011b5e] 536e 0010                 subq.w    #1,16(a6)
[00011b62] 4a40                      tst.w     d0
[00011b64] 66f2                      bne.s     $00011B58
[00011b66] 202e 0008                 move.l    8(a6),d0
[00011b6a] 4a9f                      tst.l     (a7)+
[00011b6c] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00011b70] 4e5e                      unlk      a6
[00011b72] 4e75                      rts
_strcmp:
[00011b74] 4e56 0000                 link      a6,#$0000
[00011b78] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011b7c] 2a6e 0008                 movea.l   8(a6),a5
[00011b80] 286e 000c                 movea.l   12(a6),a4
[00011b84] 6014                      bra.s     $00011B9A
[00011b86] 1e1d                      move.b    (a5)+,d7
[00011b88] 1c1c                      move.b    (a4)+,d6
[00011b8a] be06                      cmp.b     d6,d7
[00011b8c] 6f04                      ble.s     $00011B92
[00011b8e] 7001                      moveq.l   #1,d0
[00011b90] 601a                      bra.s     $00011BAC
[00011b92] be06                      cmp.b     d6,d7
[00011b94] 6c04                      bge.s     $00011B9A
[00011b96] 70ff                      moveq.l   #-1,d0
[00011b98] 6012                      bra.s     $00011BAC
[00011b9a] 4a15                      tst.b     (a5)
[00011b9c] 6704                      beq.s     $00011BA2
[00011b9e] 4a14                      tst.b     (a4)
[00011ba0] 66e4                      bne.s     $00011B86
[00011ba2] 1015                      move.b    (a5),d0
[00011ba4] 4880                      ext.w     d0
[00011ba6] 1214                      move.b    (a4),d1
[00011ba8] 4881                      ext.w     d1
[00011baa] 9041                      sub.w     d1,d0
[00011bac] 4a9f                      tst.l     (a7)+
[00011bae] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00011bb2] 4e5e                      unlk      a6
[00011bb4] 4e75                      rts
_strcat:
[00011bb6] 4e56 0000                 link      a6,#$0000
[00011bba] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00011bbe] 2a6e 000c                 movea.l   12(a6),a5
[00011bc2] 286e 0008                 movea.l   8(a6),a4
[00011bc6] 6002                      bra.s     $00011BCA
[00011bc8] 528c                      addq.l    #1,a4
[00011bca] 4a14                      tst.b     (a4)
[00011bcc] 66fa                      bne.s     $00011BC8
[00011bce] 18dd                      move.b    (a5)+,(a4)+
[00011bd0] 66fc                      bne.s     $00011BCE
[00011bd2] 202e 0008                 move.l    8(a6),d0
[00011bd6] 4a9f                      tst.l     (a7)+
[00011bd8] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00011bdc] 4e5e                      unlk      a6
[00011bde] 4e75                      rts
___open:
[00011be0] 4e56 fffa                 link      a6,#$FFFA
[00011be4] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[00011be8] 2a6e 000a                 movea.l   10(a6),a5
[00011bec] 302e 0008                 move.w    8(a6),d0
[00011bf0] c1fc 0238                 muls.w    #$0238,d0
[00011bf4] d0bc 0001 3efc            add.l     #$00013EFC,d0
[00011bfa] 2d40 fffc                 move.l    d0,-4(a6)
[00011bfe] 7eff                      moveq.l   #-1,d7
[00011c00] 2e8d                      move.l    a5,(a7)
[00011c02] 6100 00ec                 bsr       $00011CF0
[00011c06] 264d                      movea.l   a5,a3
[00011c08] 6002                      bra.s     $00011C0C
[00011c0a] 528b                      addq.l    #1,a3
[00011c0c] 4a13                      tst.b     (a3)
[00011c0e] 6706                      beq.s     $00011C16
[00011c10] 0c13 003a                 cmpi.b    #$3A,(a3)
[00011c14] 66f4                      bne.s     $00011C0A
[00011c16] 4a13                      tst.b     (a3)
[00011c18] 671c                      beq.s     $00011C36
[00011c1a] 1d63 fffa                 move.b    -(a3),-6(a6)
[00011c1e] 0c2e 0041 fffa            cmpi.b    #$41,-6(a6)
[00011c24] 6d0c                      blt.s     $00011C32
[00011c26] 0c2e 004f fffa            cmpi.b    #$4F,-6(a6)
[00011c2c] 6e04                      bgt.s     $00011C32
[00011c2e] 2a4b                      movea.l   a3,a5
[00011c30] 6004                      bra.s     $00011C36
[00011c32] 4beb 0002                 lea.l     2(a3),a5
[00011c36] 302e 000e                 move.w    14(a6),d0
[00011c3a] 6000 0090                 bra       $00011CCC
[00011c3e] 4257                      clr.w     (a7)
[00011c40] 2f0d                      move.l    a5,-(a7)
[00011c42] 3f3c 003c                 move.w    #$003C,-(a7)
[00011c46] 4eb9 0001 1938            jsr       _trap
[00011c4c] 5c8f                      addq.l    #6,a7
[00011c4e] 3e00                      move.w    d0,d7
[00011c50] 6f0a                      ble.s     $00011C5C
[00011c52] 206e fffc                 movea.l   -4(a6),a0
[00011c56] 3147 0004                 move.w    d7,4(a0)
[00011c5a] 4247                      clr.w     d7
[00011c5c] 6000 0086                 bra       $00011CE4
[00011c60] 4257                      clr.w     (a7)
[00011c62] 2f0d                      move.l    a5,-(a7)
[00011c64] 3f3c 004e                 move.w    #$004E,-(a7)
[00011c68] 4eb9 0001 1938            jsr       _trap
[00011c6e] 5c8f                      addq.l    #6,a7
[00011c70] 4a80                      tst.l     d0
[00011c72] 6604                      bne.s     $00011C78
[00011c74] 4240                      clr.w     d0
[00011c76] 6004                      bra.s     $00011C7C
[00011c78] 303c 00ff                 move.w    #$00FF,d0
[00011c7c] 3e00                      move.w    d0,d7
[00011c7e] 6064                      bra.s     $00011CE4
[00011c80] 3ebc 004f                 move.w    #$004F,(a7)
[00011c84] 4eb9 0001 1938            jsr       _trap
[00011c8a] 4a80                      tst.l     d0
[00011c8c] 6604                      bne.s     $00011C92
[00011c8e] 4240                      clr.w     d0
[00011c90] 6004                      bra.s     $00011C96
[00011c92] 303c 00ff                 move.w    #$00FF,d0
[00011c96] 3e00                      move.w    d0,d7
[00011c98] 604a                      bra.s     $00011CE4
[00011c9a] 2e8d                      move.l    a5,(a7)
[00011c9c] 3f3c 003d                 move.w    #$003D,-(a7)
[00011ca0] 4eb9 0001 1938            jsr       _trap
[00011ca6] 548f                      addq.l    #2,a7
[00011ca8] 3e00                      move.w    d0,d7
[00011caa] 6f0a                      ble.s     $00011CB6
[00011cac] 206e fffc                 movea.l   -4(a6),a0
[00011cb0] 3147 0004                 move.w    d7,4(a0)
[00011cb4] 4247                      clr.w     d7
[00011cb6] 602c                      bra.s     $00011CE4
[00011cb8] 2e8d                      move.l    a5,(a7)
[00011cba] 3f3c 0041                 move.w    #$0041,-(a7)
[00011cbe] 4eb9 0001 1938            jsr       _trap
[00011cc4] 548f                      addq.l    #2,a7
[00011cc6] 3e00                      move.w    d0,d7
[00011cc8] 601a                      bra.s     $00011CE4
[00011cca] 6018                      bra.s     $00011CE4
[00011ccc] 907c 000f                 sub.w     #$000F,d0
[00011cd0] b07c 0007                 cmp.w     #$0007,d0
[00011cd4] 620e                      bhi.s     $00011CE4
[00011cd6] e540                      asl.w     #2,d0
[00011cd8] 3040                      movea.w   d0,a0
[00011cda] d1fc 0001 3cd2            adda.l    #$00013CD2,a0
[00011ce0] 2050                      movea.l   (a0),a0
[00011ce2] 4ed0                      jmp       (a0)
[00011ce4] 3007                      move.w    d7,d0
[00011ce6] 4a9f                      tst.l     (a7)+
[00011ce8] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[00011cec] 4e5e                      unlk      a6
[00011cee] 4e75                      rts
[00011cf0] 4e56 fffc                 link      a6,#$FFFC
[00011cf4] 206e 0008                 movea.l   8(a6),a0
[00011cf8] 0c10 0061                 cmpi.b    #$61,(a0)
[00011cfc] 6d12                      blt.s     $00011D10
[00011cfe] 206e 0008                 movea.l   8(a6),a0
[00011d02] 0c10 007a                 cmpi.b    #$7A,(a0)
[00011d06] 6e08                      bgt.s     $00011D10
[00011d08] 206e 0008                 movea.l   8(a6),a0
[00011d0c] 0410 0020                 subi.b    #$20,(a0)
[00011d10] 206e 0008                 movea.l   8(a6),a0
[00011d14] 1010                      move.b    (a0),d0
[00011d16] 4880                      ext.w     d0
[00011d18] 52ae 0008                 addq.l    #1,8(a6)
[00011d1c] 4a00                      tst.b     d0
[00011d1e] 66d4                      bne.s     $00011CF4
[00011d20] 4e5e                      unlk      a6
[00011d22] 4e75                      rts
__ttyout:
[00011d24] 4e56 fffa                 link      a6,#$FFFA
[00011d28] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00011d2c] 2a6e 0008                 movea.l   8(a6),a5
[00011d30] 101d                      move.b    (a5)+,d0
[00011d32] 4880                      ext.w     d0
[00011d34] 3d40 fffe                 move.w    d0,-2(a6)
[00011d38] 204d                      movea.l   a5,a0
[00011d3a] 326e fffe                 movea.w   -2(a6),a1
[00011d3e] d1c9                      adda.l    a1,a0
[00011d40] 10bc 0024                 move.b    #$24,(a0)
[00011d44] 4247                      clr.w     d7
[00011d46] 2d4d fffa                 move.l    a5,-6(a6)
[00011d4a] 6048                      bra.s     $00011D94
[00011d4c] 204d                      movea.l   a5,a0
[00011d4e] 3247                      movea.w   d7,a1
[00011d50] d1c9                      adda.l    a1,a0
[00011d52] 0c10 0024                 cmpi.b    #$24,(a0)
[00011d56] 663a                      bne.s     $00011D92
[00011d58] 206e fffa                 movea.l   -6(a6),a0
[00011d5c] 0c10 0024                 cmpi.b    #$24,(a0)
[00011d60] 6710                      beq.s     $00011D72
[00011d62] 2eae fffa                 move.l    -6(a6),(a7)
[00011d66] 3f3c 0009                 move.w    #$0009,-(a7)
[00011d6a] 4eb9 0001 00dc            jsr       ___BDOS
[00011d70] 548f                      addq.l    #2,a7
[00011d72] 2ebc 0000 0024            move.l    #$00000024,(a7)
[00011d78] 3f3c 0002                 move.w    #$0002,-(a7)
[00011d7c] 4eb9 0001 00dc            jsr       ___BDOS
[00011d82] 548f                      addq.l    #2,a7
[00011d84] 200d                      move.l    a5,d0
[00011d86] 3207                      move.w    d7,d1
[00011d88] 48c1                      ext.l     d1
[00011d8a] d081                      add.l     d1,d0
[00011d8c] 5280                      addq.l    #1,d0
[00011d8e] 2d40 fffa                 move.l    d0,-6(a6)
[00011d92] 5247                      addq.w    #1,d7
[00011d94] be6e fffe                 cmp.w     -2(a6),d7
[00011d98] 6db2                      blt.s     $00011D4C
[00011d9a] 200d                      move.l    a5,d0
[00011d9c] 3207                      move.w    d7,d1
[00011d9e] 48c1                      ext.l     d1
[00011da0] d081                      add.l     d1,d0
[00011da2] b0ae fffa                 cmp.l     -6(a6),d0
[00011da6] 6710                      beq.s     $00011DB8
[00011da8] 2eae fffa                 move.l    -6(a6),(a7)
[00011dac] 3f3c 0009                 move.w    #$0009,-(a7)
[00011db0] 4eb9 0001 00dc            jsr       ___BDOS
[00011db6] 548f                      addq.l    #2,a7
[00011db8] 302e fffe                 move.w    -2(a6),d0
[00011dbc] 4a9f                      tst.l     (a7)+
[00011dbe] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00011dc2] 4e5e                      unlk      a6
[00011dc4] 4e75                      rts
__lstout:
[00011dc6] 4e56 fffe                 link      a6,#$FFFE
[00011dca] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00011dce] 2a6e 0008                 movea.l   8(a6),a5
[00011dd2] 1e1d                      move.b    (a5)+,d7
[00011dd4] 4887                      ext.w     d7
[00011dd6] ce7c 00ff                 and.w     #$00FF,d7
[00011dda] 3d47 fffe                 move.w    d7,-2(a6)
[00011dde] 6010                      bra.s     $00011DF0
[00011de0] 101d                      move.b    (a5)+,d0
[00011de2] 4880                      ext.w     d0
[00011de4] 3e80                      move.w    d0,(a7)
[00011de6] 4267                      clr.w     -(a7)
[00011de8] 3f3c 0003                 move.w    #$0003,-(a7)
[00011dec] 611c                      bsr.s     $00011E0A
[00011dee] 588f                      addq.l    #4,a7
[00011df0] 3007                      move.w    d7,d0
[00011df2] 5347                      subq.w    #1,d7
[00011df4] 4a40                      tst.w     d0
[00011df6] 6ee8                      bgt.s     $00011DE0
[00011df8] 302e fffe                 move.w    -2(a6),d0
[00011dfc] 4a9f                      tst.l     (a7)+
[00011dfe] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00011e02] 4e5e                      unlk      a6
[00011e04] 4e75                      rts
[00011e06] 4e56 fffc                 link      a6,#$FFFC
[00011e0a] 23df 0001 3ef8            move.l    (a7)+,$00013EF8
[00011e10] 4e4d                      trap      #13
[00011e12] 2f39 0001 3ef8            move.l    $00013EF8,-(a7)
[00011e18] 4e75                      rts
[00011e1a] 4e5e                      unlk      a6
[00011e1c] 4e75                      rts
__wrtchr:
[00011e1e] 4e56 fde8                 link      a6,#$FDE8
[00011e22] 3d7c 0001 fdf2            move.w    #$0001,-526(a6)
[00011e28] 426e fdf0                 clr.w     -528(a6)
[00011e2c] 206e 0008                 movea.l   8(a6),a0
[00011e30] 0828 0001 0001            btst      #1,1(a0)
[00011e36] 670a                      beq.s     $00011E42
[00011e38] 2d7c 0001 1d24 fdf4       move.l    #__ttyout,-524(a6)
[00011e40] 6014                      bra.s     $00011E56
[00011e42] 206e 0008                 movea.l   8(a6),a0
[00011e46] 0828 0002 0001            btst      #2,1(a0)
[00011e4c] 6708                      beq.s     $00011E56
[00011e4e] 2d7c 0001 1dc6 fdf4       move.l    #__lstout,-524(a6)
[00011e56] 206e 0008                 movea.l   8(a6),a0
[00011e5a] 0828 0007 0001            btst      #7,1(a0)
[00011e60] 671e                      beq.s     $00011E80
[00011e62] 202e 0008                 move.l    8(a6),d0
[00011e66] d0bc 0000 0012            add.l     #$00000012,d0
[00011e6c] 2d40 fdec                 move.l    d0,-532(a6)
[00011e70] 206e fdec                 movea.l   -532(a6),a0
[00011e74] 3028 0004                 move.w    4(a0),d0
[00011e78] c07c 0c00                 and.w     #$0C00,d0
[00011e7c] 3d40 fdf0                 move.w    d0,-528(a6)
[00011e80] 426e fdfa                 clr.w     -518(a6)
[00011e84] 3d6e 0010 fffe            move.w    16(a6),-2(a6)
[00011e8a] 6000 00ea                 bra       $00011F76
[00011e8e] 3d7c 0001 fffc            move.w    #$0001,-4(a6)
[00011e94] 6000 00b6                 bra       $00011F4C
[00011e98] 206e 000c                 movea.l   12(a6),a0
[00011e9c] 0c10 000a                 cmpi.b    #$0A,(a0)
[00011ea0] 662e                      bne.s     $00011ED0
[00011ea2] 3d7c ffff fdfa            move.w    #$FFFF,-518(a6)
[00011ea8] 4a6e fdf2                 tst.w     -526(a6)
[00011eac] 6720                      beq.s     $00011ECE
[00011eae] 302e fffc                 move.w    -4(a6),d0
[00011eb2] 5240                      addq.w    #1,d0
[00011eb4] b07c 0200                 cmp.w     #$0200,d0
[00011eb8] 6c00 00a2                 bge       $00011F5C
[00011ebc] 204e                      movea.l   a6,a0
[00011ebe] 326e fffc                 movea.w   -4(a6),a1
[00011ec2] d1c9                      adda.l    a1,a0
[00011ec4] 117c 000d fdfc            move.b    #$0D,-516(a0)
[00011eca] 526e fffc                 addq.w    #1,-4(a6)
[00011ece] 605c                      bra.s     $00011F2C
[00011ed0] 206e 000c                 movea.l   12(a6),a0
[00011ed4] 0c10 0009                 cmpi.b    #$09,(a0)
[00011ed8] 6652                      bne.s     $00011F2C
[00011eda] 4a6e fdf0                 tst.w     -528(a6)
[00011ede] 674c                      beq.s     $00011F2C
[00011ee0] 7008                      moveq.l   #8,d0
[00011ee2] 322e fdfa                 move.w    -518(a6),d1
[00011ee6] c27c 0007                 and.w     #$0007,d1
[00011eea] 9041                      sub.w     d1,d0
[00011eec] 3d40 fdf8                 move.w    d0,-520(a6)
[00011ef0] 302e fffc                 move.w    -4(a6),d0
[00011ef4] d06e fdf8                 add.w     -520(a6),d0
[00011ef8] b07c 0200                 cmp.w     #$0200,d0
[00011efc] 6c5e                      bge.s     $00011F5C
[00011efe] 302e fdf8                 move.w    -520(a6),d0
[00011f02] d16e fdfa                 add.w     d0,-518(a6)
[00011f06] 6012                      bra.s     $00011F1A
[00011f08] 204e                      movea.l   a6,a0
[00011f0a] 326e fffc                 movea.w   -4(a6),a1
[00011f0e] d1c9                      adda.l    a1,a0
[00011f10] 117c 0020 fdfc            move.b    #$20,-516(a0)
[00011f16] 526e fffc                 addq.w    #1,-4(a6)
[00011f1a] 302e fdf8                 move.w    -520(a6),d0
[00011f1e] 536e fdf8                 subq.w    #1,-520(a6)
[00011f22] 4a40                      tst.w     d0
[00011f24] 66e2                      bne.s     $00011F08
[00011f26] 52ae 000c                 addq.l    #1,12(a6)
[00011f2a] 6020                      bra.s     $00011F4C
[00011f2c] 204e                      movea.l   a6,a0
[00011f2e] 326e fffc                 movea.w   -4(a6),a1
[00011f32] d1c9                      adda.l    a1,a0
[00011f34] 226e 000c                 movea.l   12(a6),a1
[00011f38] 1151 fdfc                 move.b    (a1),-516(a0)
[00011f3c] 52ae 000c                 addq.l    #1,12(a6)
[00011f40] 526e fffc                 addq.w    #1,-4(a6)
[00011f44] 536e fffe                 subq.w    #1,-2(a6)
[00011f48] 526e fdfa                 addq.w    #1,-518(a6)
[00011f4c] 0c6e 0200 fffc            cmpi.w    #$0200,-4(a6)
[00011f52] 6c08                      bge.s     $00011F5C
[00011f54] 4a6e fffe                 tst.w     -2(a6)
[00011f58] 6600 ff3e                 bne       $00011E98
[00011f5c] 302e fffc                 move.w    -4(a6),d0
[00011f60] 5340                      subq.w    #1,d0
[00011f62] 1d40 fdfc                 move.b    d0,-516(a6)
[00011f66] 2f0e                      move.l    a6,-(a7)
[00011f68] 0697 ffff fdfc            addi.l    #$FFFFFDFC,(a7)
[00011f6e] 206e fdf4                 movea.l   -524(a6),a0
[00011f72] 4e90                      jsr       (a0)
[00011f74] 588f                      addq.l    #4,a7
[00011f76] 4a6e fffe                 tst.w     -2(a6)
[00011f7a] 6600 ff12                 bne       $00011E8E
[00011f7e] 4240                      clr.w     d0
[00011f80] 302e 0010                 move.w    16(a6),d0
[00011f84] 4e5e                      unlk      a6
[00011f86] 4e75                      rts
__wrtbin:
[00011f88] 4e56 fffe                 link      a6,#$FFFE
[00011f8c] 48e7 0104                 movem.l   d7/a5,-(a7)
[00011f90] 2a6e 0008                 movea.l   8(a6),a5
[00011f94] 3ebc 0001                 move.w    #$0001,(a7)
[00011f98] 3f2e 0010                 move.w    16(a6),-(a7)
[00011f9c] 2f2e 000c                 move.l    12(a6),-(a7)
[00011fa0] 2f2d 0006                 move.l    6(a5),-(a7)
[00011fa4] 2f0d                      move.l    a5,-(a7)
[00011fa6] 0697 0000 0012            addi.l    #$00000012,(a7)
[00011fac] 4eb9 0001 1984            jsr       __pc_wri
[00011fb2] dffc 0000 000e            adda.l    #$0000000E,a7
[00011fb8] 3d40 fffe                 move.w    d0,-2(a6)
[00011fbc] 4a6e fffe                 tst.w     -2(a6)
[00011fc0] 6616                      bne.s     $00011FD8
[00011fc2] 33fc 0005 0001 3cce       move.w    #$0005,$00013CCE
[00011fca] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[00011fd4] 70ff                      moveq.l   #-1,d0
[00011fd6] 6020                      bra.s     $00011FF8
[00011fd8] 4280                      clr.l     d0
[00011fda] 302e fffe                 move.w    -2(a6),d0
[00011fde] d1ad 0006                 add.l     d0,6(a5)
[00011fe2] 202d 0006                 move.l    6(a5),d0
[00011fe6] b0ad 000e                 cmp.l     14(a5),d0
[00011fea] 6f06                      ble.s     $00011FF2
[00011fec] 2b6d 0006 000e            move.l    6(a5),14(a5)
[00011ff2] 4240                      clr.w     d0
[00011ff4] 302e fffe                 move.w    -2(a6),d0
[00011ff8] 4a9f                      tst.l     (a7)+
[00011ffa] 4cdf 2000                 movem.l   (a7)+,a5
[00011ffe] 4e5e                      unlk      a6
[00012000] 4e75                      rts
__wrtasc:
[00012002] 4e56 fffa                 link      a6,#$FFFA
[00012006] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001200a] 2a6e 0008                 movea.l   8(a6),a5
[0001200e] 286e 000c                 movea.l   12(a6),a4
[00012012] 3e2e 0010                 move.w    16(a6),d7
[00012016] 4240                      clr.w     d0
[00012018] 3d40 fffa                 move.w    d0,-6(a6)
[0001201c] 3d40 fffc                 move.w    d0,-4(a6)
[00012020] 6000 00c8                 bra       $000120EA
[00012024] 6004                      bra.s     $0001202A
[00012026] 526e fffa                 addq.w    #1,-6(a6)
[0001202a] be6e fffa                 cmp.w     -6(a6),d7
[0001202e] 6310                      bls.s     $00012040
[00012030] 204c                      movea.l   a4,a0
[00012032] 93c9                      suba.l    a1,a1
[00012034] 326e fffa                 movea.w   -6(a6),a1
[00012038] d1c9                      adda.l    a1,a0
[0001203a] 0c10 000a                 cmpi.b    #$0A,(a0)
[0001203e] 66e6                      bne.s     $00012026
[00012040] 4240                      clr.w     d0
[00012042] 302e fffa                 move.w    -6(a6),d0
[00012046] 906e fffc                 sub.w     -4(a6),d0
[0001204a] 6366                      bls.s     $000120B2
[0001204c] 3ebc 0001                 move.w    #$0001,(a7)
[00012050] 3f2e fffa                 move.w    -6(a6),-(a7)
[00012054] 4240                      clr.w     d0
[00012056] 302e fffc                 move.w    -4(a6),d0
[0001205a] 9157                      sub.w     d0,(a7)
[0001205c] 200c                      move.l    a4,d0
[0001205e] 4281                      clr.l     d1
[00012060] 322e fffc                 move.w    -4(a6),d1
[00012064] d081                      add.l     d1,d0
[00012066] 2f00                      move.l    d0,-(a7)
[00012068] 2f2d 0006                 move.l    6(a5),-(a7)
[0001206c] 2f0d                      move.l    a5,-(a7)
[0001206e] 0697 0000 0012            addi.l    #$00000012,(a7)
[00012074] 4eb9 0001 1984            jsr       __pc_wri
[0001207a] dffc 0000 000e            adda.l    #$0000000E,a7
[00012080] 3d40 fffe                 move.w    d0,-2(a6)
[00012084] 4280                      clr.l     d0
[00012086] 302e fffe                 move.w    -2(a6),d0
[0001208a] d1ad 0006                 add.l     d0,6(a5)
[0001208e] 4a6e fffe                 tst.w     -2(a6)
[00012092] 6616                      bne.s     $000120AA
[00012094] 33fc 0005 0001 3cce       move.w    #$0005,$00013CCE
[0001209c] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[000120a6] 70ff                      moveq.l   #-1,d0
[000120a8] 605e                      bra.s     $00012108
[000120aa] 3d6e fffa fffc            move.w    -6(a6),-4(a6)
[000120b0] 6038                      bra.s     $000120EA
[000120b2] 526e fffa                 addq.w    #1,-6(a6)
[000120b6] 3ebc 0001                 move.w    #$0001,(a7)
[000120ba] 3f3c 0001                 move.w    #$0001,-(a7)
[000120be] 2f3c 0001 3cf2            move.l    #$00013CF2,-(a7)
[000120c4] 2f2d 0006                 move.l    6(a5),-(a7)
[000120c8] 2f0d                      move.l    a5,-(a7)
[000120ca] 0697 0000 0012            addi.l    #$00000012,(a7)
[000120d0] 4eb9 0001 1984            jsr       __pc_wri
[000120d6] dffc 0000 000e            adda.l    #$0000000E,a7
[000120dc] 3d40 fffe                 move.w    d0,-2(a6)
[000120e0] 4280                      clr.l     d0
[000120e2] 302e fffe                 move.w    -2(a6),d0
[000120e6] d1ad 0006                 add.l     d0,6(a5)
[000120ea] be6e fffc                 cmp.w     -4(a6),d7
[000120ee] 6200 ff34                 bhi       $00012024
[000120f2] 202d 0006                 move.l    6(a5),d0
[000120f6] b0ad 000e                 cmp.l     14(a5),d0
[000120fa] 6f06                      ble.s     $00012102
[000120fc] 2b6d 0006 000e            move.l    6(a5),14(a5)
[00012102] 4240                      clr.w     d0
[00012104] 302e fffc                 move.w    -4(a6),d0
[00012108] 4a9f                      tst.l     (a7)+
[0001210a] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[0001210e] 4e5e                      unlk      a6
[00012110] 4e75                      rts
__allocc:
[00012112] 4e56 0000                 link      a6,#$0000
[00012116] 48e7 0700                 movem.l   d5-d7,-(a7)
[0001211a] 7c01                      moveq.l   #1,d6
[0001211c] 4247                      clr.w     d7
[0001211e] 6018                      bra.s     $00012138
[00012120] 2006                      move.l    d6,d0
[00012122] c0b9 0001 3cf4            and.l     $00013CF4,d0
[00012128] 660a                      bne.s     $00012134
[0001212a] 8db9 0001 3cf4            or.l      d6,$00013CF4
[00012130] 3007                      move.w    d7,d0
[00012132] 601e                      bra.s     $00012152
[00012134] e386                      asl.l     #1,d6
[00012136] 5247                      addq.w    #1,d7
[00012138] be7c 0010                 cmp.w     #$0010,d7
[0001213c] 6de2                      blt.s     $00012120
[0001213e] 33fc 0018 0001 3cce       move.w    #$0018,$00013CCE
[00012146] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[00012150] 70ff                      moveq.l   #-1,d0
[00012152] 4a9f                      tst.l     (a7)+
[00012154] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[00012158] 4e5e                      unlk      a6
[0001215a] 4e75                      rts
__freec:
[0001215c] 4e56 fffc                 link      a6,#$FFFC
[00012160] 7001                      moveq.l   #1,d0
[00012162] 322e 0008                 move.w    8(a6),d1
[00012166] e360                      asl.w     d1,d0
[00012168] 4640                      not.w     d0
[0001216a] 48c0                      ext.l     d0
[0001216c] c1b9 0001 3cf4            and.l     d0,$00013CF4
[00012172] 4240                      clr.w     d0
[00012174] 4e5e                      unlk      a6
[00012176] 4e75                      rts
__chinit:
[00012178] 4e56 0000                 link      a6,#$0000
[0001217c] 48e7 0300                 movem.l   d6-d7,-(a7)
[00012180] 4247                      clr.w     d7
[00012182] 6006                      bra.s     $0001218A
[00012184] 3e87                      move.w    d7,(a7)
[00012186] 6112                      bsr.s     ___chini
[00012188] 5247                      addq.w    #1,d7
[0001218a] be7c 0010                 cmp.w     #$0010,d7
[0001218e] 6df4                      blt.s     $00012184
[00012190] 4a9f                      tst.l     (a7)+
[00012192] 4cdf 0080                 movem.l   (a7)+,d7
[00012196] 4e5e                      unlk      a6
[00012198] 4e75                      rts
___chini:
[0001219a] 4e56 0000                 link      a6,#$0000
[0001219e] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[000121a2] 302e 0008                 move.w    8(a6),d0
[000121a6] c1fc 0238                 muls.w    #$0238,d0
[000121aa] 2a40                      movea.l   d0,a5
[000121ac] dbfc 0001 3efc            adda.l    #$00013EFC,a5
[000121b2] 302e 0008                 move.w    8(a6),d0
[000121b6] 1b40 0003                 move.b    d0,3(a5)
[000121ba] 4255                      clr.w     (a5)
[000121bc] 422d 0002                 clr.b     2(a5)
[000121c0] 2b7c ffff ffff 000a       move.l    #$FFFFFFFF,10(a5)
[000121c8] 42ad 0006                 clr.l     6(a5)
[000121cc] 42ad 000e                 clr.l     14(a5)
[000121d0] 3ebc 0020                 move.w    #$0020,(a7)
[000121d4] 4267                      clr.w     -(a7)
[000121d6] 2f0d                      move.l    a5,-(a7)
[000121d8] 0697 0000 0012            addi.l    #$00000012,(a7)
[000121de] 4eb9 0001 010e            jsr       _blkfill
[000121e4] 5c8f                      addq.l    #6,a7
[000121e6] 3ebc 000b                 move.w    #$000B,(a7)
[000121ea] 3f3c 0020                 move.w    #$0020,-(a7)
[000121ee] 2f0d                      move.l    a5,-(a7)
[000121f0] 0697 0000 0013            addi.l    #$00000013,(a7)
[000121f6] 4eb9 0001 010e            jsr       _blkfill
[000121fc] 5c8f                      addq.l    #6,a7
[000121fe] 4a9f                      tst.l     (a7)+
[00012200] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00012204] 4e5e                      unlk      a6
[00012206] 4e75                      rts
__chkc:
[00012208] 4e56 0000                 link      a6,#$0000
[0001220c] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00012210] 3e2e 0008                 move.w    8(a6),d7
[00012214] be7c 0010                 cmp.w     #$0010,d7
[00012218] 6516                      bcs.s     $00012230
[0001221a] 33fc 0009 0001 3cce       move.w    #$0009,$00013CCE
[00012222] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[0001222c] 4280                      clr.l     d0
[0001222e] 6030                      bra.s     $00012260
[00012230] 4240                      clr.w     d0
[00012232] 3007                      move.w    d7,d0
[00012234] c0fc 0238                 mulu.w    #$0238,d0
[00012238] 2a40                      movea.l   d0,a5
[0001223a] dbfc 0001 3efc            adda.l    #$00013EFC,a5
[00012240] 082d 0000 0001            btst      #0,1(a5)
[00012246] 6616                      bne.s     $0001225E
[00012248] 33fc 0009 0001 3cce       move.w    #$0009,$00013CCE
[00012250] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[0001225a] 4280                      clr.l     d0
[0001225c] 6002                      bra.s     $00012260
[0001225e] 200d                      move.l    a5,d0
[00012260] 4a9f                      tst.l     (a7)+
[00012262] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00012266] 4e5e                      unlk      a6
[00012268] 4e75                      rts
_write:
[0001226a] 4e56 0000                 link      a6,#$0000
[0001226e] 48e7 0104                 movem.l   d7/a5,-(a7)
[00012272] 3eae 0008                 move.w    8(a6),(a7)
[00012276] 4eb9 0001 2208            jsr       __chkc
[0001227c] 2a40                      movea.l   d0,a5
[0001227e] 200d                      move.l    a5,d0
[00012280] 6606                      bne.s     $00012288
[00012282] 70ff                      moveq.l   #-1,d0
[00012284] 6000 0076                 bra       $000122FC
[00012288] 4a6e 000e                 tst.w     14(a6)
[0001228c] 6604                      bne.s     $00012292
[0001228e] 4240                      clr.w     d0
[00012290] 606a                      bra.s     $000122FC
[00012292] 082d 0003 0001            btst      #3,1(a5)
[00012298] 6716                      beq.s     $000122B0
[0001229a] 33fc 0009 0001 3cce       move.w    #$0009,$00013CCE
[000122a2] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[000122ac] 70ff                      moveq.l   #-1,d0
[000122ae] 604c                      bra.s     $000122FC
[000122b0] 3015                      move.w    (a5),d0
[000122b2] c07c 0204                 and.w     #$0204,d0
[000122b6] 6714                      beq.s     $000122CC
[000122b8] 3eae 000e                 move.w    14(a6),(a7)
[000122bc] 2f2e 000a                 move.l    10(a6),-(a7)
[000122c0] 2f0d                      move.l    a5,-(a7)
[000122c2] 4eb9 0001 1e1e            jsr       __wrtchr
[000122c8] 508f                      addq.l    #8,a7
[000122ca] 6030                      bra.s     $000122FC
[000122cc] 082d 0004 0001            btst      #4,1(a5)
[000122d2] 6716                      beq.s     $000122EA
[000122d4] 3eae 000e                 move.w    14(a6),(a7)
[000122d8] 2f2e 000a                 move.l    10(a6),-(a7)
[000122dc] 2f0d                      move.l    a5,-(a7)
[000122de] 4eb9 0001 2002            jsr       __wrtasc
[000122e4] 508f                      addq.l    #8,a7
[000122e6] 6014                      bra.s     $000122FC
[000122e8] 6012                      bra.s     $000122FC
[000122ea] 3eae 000e                 move.w    14(a6),(a7)
[000122ee] 2f2e 000a                 move.l    10(a6),-(a7)
[000122f2] 2f0d                      move.l    a5,-(a7)
[000122f4] 4eb9 0001 1f88            jsr       __wrtbin
[000122fa] 508f                      addq.l    #8,a7
[000122fc] 4a9f                      tst.l     (a7)+
[000122fe] 4cdf 2000                 movem.l   (a7)+,a5
[00012302] 4e5e                      unlk      a6
[00012304] 4e75                      rts
_sbrk:
[00012306] 4e56 0000                 link      a6,#$0000
[0001230a] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001230e] 3e2e 0008                 move.w    8(a6),d7
[00012312] 48c7                      ext.l     d7
[00012314] cebc 0000 ffff            and.l     #$0000FFFF,d7
[0001231a] 0807 0000                 btst      #0,d7
[0001231e] 6702                      beq.s     $00012322
[00012320] 5287                      addq.l    #1,d7
[00012322] 2a79 0001 3ec0            movea.l   __break,a5
[00012328] 2847                      movea.l   d7,a4
[0001232a] d9f9 0001 3ec0            adda.l    __break,a4
[00012330] 2e8c                      move.l    a4,(a7)
[00012332] 4eb9 0001 00b2            jsr       _brk
[00012338] b07c ffff                 cmp.w     #$FFFF,d0
[0001233c] 6616                      bne.s     $00012354
[0001233e] 33fc 000c 0001 3cce       move.w    #$000C,$00013CCE
[00012346] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[00012350] 70ff                      moveq.l   #-1,d0
[00012352] 6012                      bra.s     $00012366
[00012354] 3eae 0008                 move.w    8(a6),(a7)
[00012358] 4267                      clr.w     -(a7)
[0001235a] 2f0d                      move.l    a5,-(a7)
[0001235c] 4eb9 0001 010e            jsr       _blkfill
[00012362] 5c8f                      addq.l    #6,a7
[00012364] 200d                      move.l    a5,d0
[00012366] 4a9f                      tst.l     (a7)+
[00012368] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[0001236c] 4e5e                      unlk      a6
[0001236e] 4e75                      rts
___prtsh:
[00012370] 4e56 fffc                 link      a6,#$FFFC
[00012374] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00012378] 206e 0008                 movea.l   8(a6),a0
[0001237c] 3010                      move.w    (a0),d0
[0001237e] 48c0                      ext.l     d0
[00012380] 2d40 fffc                 move.l    d0,-4(a6)
[00012384] 202e fffc                 move.l    -4(a6),d0
[00012388] c0bc 0000 ffff            and.l     #$0000FFFF,d0
[0001238e] 3e00                      move.w    d0,d7
[00012390] 2a6e 0014                 movea.l   20(a6),a5
[00012394] 3c2e 0010                 move.w    16(a6),d6
[00012398] 4a6e 0012                 tst.w     18(a6)
[0001239c] 671c                      beq.s     $000123BA
[0001239e] 4a47                      tst.w     d7
[000123a0] 6c18                      bge.s     $000123BA
[000123a2] 4240                      clr.w     d0
[000123a4] 3007                      move.w    d7,d0
[000123a6] 4440                      neg.w     d0
[000123a8] 3e00                      move.w    d0,d7
[000123aa] 206e 000c                 movea.l   12(a6),a0
[000123ae] 2050                      movea.l   (a0),a0
[000123b0] 10bc 002d                 move.b    #$2D,(a0)
[000123b4] 226e 000c                 movea.l   12(a6),a1
[000123b8] 5291                      addq.l    #1,(a1)
[000123ba] 6012                      bra.s     $000123CE
[000123bc] 4280                      clr.l     d0
[000123be] 3007                      move.w    d7,d0
[000123c0] 80c6                      divu.w    d6,d0
[000123c2] 4840                      swap      d0
[000123c4] 1ac0                      move.b    d0,(a5)+
[000123c6] 4280                      clr.l     d0
[000123c8] 3007                      move.w    d7,d0
[000123ca] 80c6                      divu.w    d6,d0
[000123cc] 3e00                      move.w    d0,d7
[000123ce] 4a47                      tst.w     d7
[000123d0] 66ea                      bne.s     $000123BC
[000123d2] 200d                      move.l    a5,d0
[000123d4] 4a9f                      tst.l     (a7)+
[000123d6] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[000123da] 4e5e                      unlk      a6
[000123dc] 4e75                      rts
___prtld:
[000123de] 4e56 fffc                 link      a6,#$FFFC
[000123e2] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[000123e6] 2d7c 0001 3c4a fffc       move.l    #$00013C4A,-4(a6)
[000123ee] 2a6e 0014                 movea.l   20(a6),a5
[000123f2] 3c2e 0010                 move.w    16(a6),d6
[000123f6] 48c6                      ext.l     d6
[000123f8] 206e 0008                 movea.l   8(a6),a0
[000123fc] 2e10                      move.l    (a0),d7
[000123fe] 0c6e 0010 0010            cmpi.w    #$0010,16(a6)
[00012404] 661a                      bne.s     $00012420
[00012406] 7a08                      moveq.l   #8,d5
[00012408] 600c                      bra.s     $00012416
[0001240a] 2007                      move.l    d7,d0
[0001240c] c07c 000f                 and.w     #$000F,d0
[00012410] 1ac0                      move.b    d0,(a5)+
[00012412] e887                      asr.l     #4,d7
[00012414] 5345                      subq.w    #1,d5
[00012416] 4a87                      tst.l     d7
[00012418] 6704                      beq.s     $0001241E
[0001241a] 4a45                      tst.w     d5
[0001241c] 66ec                      bne.s     $0001240A
[0001241e] 6068                      bra.s     $00012488
[00012420] 0c6e 0008 0010            cmpi.w    #$0008,16(a6)
[00012426] 6624                      bne.s     $0001244C
[00012428] 7a0b                      moveq.l   #11,d5
[0001242a] 600c                      bra.s     $00012438
[0001242c] 2007                      move.l    d7,d0
[0001242e] c07c 0007                 and.w     #$0007,d0
[00012432] 1ac0                      move.b    d0,(a5)+
[00012434] e687                      asr.l     #3,d7
[00012436] 5345                      subq.w    #1,d5
[00012438] 4a87                      tst.l     d7
[0001243a] 6704                      beq.s     $00012440
[0001243c] 4a45                      tst.w     d5
[0001243e] 66ec                      bne.s     $0001242C
[00012440] 4a45                      tst.w     d5
[00012442] 6606                      bne.s     $0001244A
[00012444] 022d 0003 ffff            andi.b    #$03,-1(a5)
[0001244a] 603c                      bra.s     $00012488
[0001244c] 4a6e 0012                 tst.w     18(a6)
[00012450] 671a                      beq.s     $0001246C
[00012452] 4a87                      tst.l     d7
[00012454] 6c16                      bge.s     $0001246C
[00012456] 206e 000c                 movea.l   12(a6),a0
[0001245a] 2050                      movea.l   (a0),a0
[0001245c] 10bc 002d                 move.b    #$2D,(a0)
[00012460] 226e 000c                 movea.l   12(a6),a1
[00012464] 5291                      addq.l    #1,(a1)
[00012466] 2007                      move.l    d7,d0
[00012468] 4480                      neg.l     d0
[0001246a] 2e00                      move.l    d0,d7
[0001246c] 6016                      bra.s     $00012484
[0001246e] 2e86                      move.l    d6,(a7)
[00012470] 2f07                      move.l    d7,-(a7)
[00012472] 4eb9 0001 19c0            jsr       _uldiv
[00012478] 588f                      addq.l    #4,a7
[0001247a] 2e00                      move.l    d0,d7
[0001247c] 206e fffc                 movea.l   -4(a6),a0
[00012480] 1ae8 0003                 move.b    3(a0),(a5)+
[00012484] 4a87                      tst.l     d7
[00012486] 66e6                      bne.s     $0001246E
[00012488] 200d                      move.l    a5,d0
[0001248a] 4a9f                      tst.l     (a7)+
[0001248c] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00012490] 4e5e                      unlk      a6
[00012492] 4e75                      rts
___prtin:
[00012494] 4e56 fff0                 link      a6,#$FFF0
[00012498] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001249c] 2f0e                      move.l    a6,-(a7)
[0001249e] 0697 ffff fff0            addi.l    #$FFFFFFF0,(a7)
[000124a4] 3f2e 0012                 move.w    18(a6),-(a7)
[000124a8] 3f2e 0010                 move.w    16(a6),-(a7)
[000124ac] 2f0e                      move.l    a6,-(a7)
[000124ae] 0697 0000 000c            addi.l    #$0000000C,(a7)
[000124b4] 2f2e 0008                 move.l    8(a6),-(a7)
[000124b8] 206e 0014                 movea.l   20(a6),a0
[000124bc] 4e90                      jsr       (a0)
[000124be] dffc 0000 0010            adda.l    #$00000010,a7
[000124c4] 2a40                      movea.l   d0,a5
[000124c6] 200e                      move.l    a6,d0
[000124c8] d0bc ffff fff0            add.l     #$FFFFFFF0,d0
[000124ce] bbc0                      cmpa.l    d0,a5
[000124d0] 6602                      bne.s     $000124D4
[000124d2] 421d                      clr.b     (a5)+
[000124d4] 286e 000c                 movea.l   12(a6),a4
[000124d8] 6012                      bra.s     $000124EC
[000124da] 1e25                      move.b    -(a5),d7
[000124dc] 4887                      ext.w     d7
[000124de] de7c 0030                 add.w     #$0030,d7
[000124e2] be7c 0039                 cmp.w     #$0039,d7
[000124e6] 6f02                      ble.s     $000124EA
[000124e8] 5e47                      addq.w    #7,d7
[000124ea] 18c7                      move.b    d7,(a4)+
[000124ec] 200e                      move.l    a6,d0
[000124ee] d0bc ffff fff0            add.l     #$FFFFFFF0,d0
[000124f4] bbc0                      cmpa.l    d0,a5
[000124f6] 66e2                      bne.s     $000124DA
[000124f8] 4214                      clr.b     (a4)
[000124fa] 200c                      move.l    a4,d0
[000124fc] 4a9f                      tst.l     (a7)+
[000124fe] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00012502] 4e5e                      unlk      a6
[00012504] 4e75                      rts
_malloc_:
[00012506] 4e56 fffc                 link      a6,#$FFFC
[0001250a] 4e5e                      unlk      a6
[0001250c] 4e75                      rts
__errmal:
[0001250e] 4e56 fffc                 link      a6,#$FFFC
[00012512] 4e5e                      unlk      a6
[00012514] 4e75                      rts
_malloc:
[00012516] 4e56 0000                 link      a6,#$0000
[0001251a] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[0001251e] 3e2e 0008                 move.w    8(a6),d7
[00012522] 5e47                      addq.w    #7,d7
[00012524] e64f                      lsr.w     #3,d7
[00012526] 5247                      addq.w    #1,d7
[00012528] 3e87                      move.w    d7,(a7)
[0001252a] 611e                      bsr.s     $0001254A
[0001252c] 2a40                      movea.l   d0,a5
[0001252e] 200d                      move.l    a5,d0
[00012530] 6604                      bne.s     $00012536
[00012532] 4280                      clr.l     d0
[00012534] 600a                      bra.s     $00012540
[00012536] 3e87                      move.w    d7,(a7)
[00012538] 2f0d                      move.l    a5,-(a7)
[0001253a] 6100 0086                 bsr       $000125C2
[0001253e] 588f                      addq.l    #4,a7
[00012540] 4a9f                      tst.l     (a7)+
[00012542] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00012546] 4e5e                      unlk      a6
[00012548] 4e75                      rts
[0001254a] 4e56 0000                 link      a6,#$0000
[0001254e] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00012552] 2879 0001 3d00            movea.l   $00013D00,a4
[00012558] 2a54                      movea.l   (a4),a5
[0001255a] 605a                      bra.s     $000125B6
[0001255c] 4240                      clr.w     d0
[0001255e] 302d 0004                 move.w    4(a5),d0
[00012562] 4241                      clr.w     d1
[00012564] 322d 0006                 move.w    6(a5),d1
[00012568] b340                      eor.w     d1,d0
[0001256a] 4640                      not.w     d0
[0001256c] 4a40                      tst.w     d0
[0001256e] 670e                      beq.s     $0001257E
[00012570] 3ebc 0001                 move.w    #$0001,(a7)
[00012574] 4eb9 0001 250e            jsr       __errmal
[0001257a] 4280                      clr.l     d0
[0001257c] 603a                      bra.s     $000125B8
[0001257e] 4240                      clr.w     d0
[00012580] 302d 0004                 move.w    4(a5),d0
[00012584] b06e 0008                 cmp.w     8(a6),d0
[00012588] 6504                      bcs.s     $0001258E
[0001258a] 200c                      move.l    a4,d0
[0001258c] 602a                      bra.s     $000125B8
[0001258e] bbf9 0001 3d00            cmpa.l    $00013D00,a5
[00012594] 661c                      bne.s     $000125B2
[00012596] 3eae 0008                 move.w    8(a6),(a7)
[0001259a] 6100 00a6                 bsr       $00012642
[0001259e] 2a40                      movea.l   d0,a5
[000125a0] 200d                      move.l    a5,d0
[000125a2] 660e                      bne.s     $000125B2
[000125a4] 3ebc 0002                 move.w    #$0002,(a7)
[000125a8] 4eb9 0001 250e            jsr       __errmal
[000125ae] 4280                      clr.l     d0
[000125b0] 6006                      bra.s     $000125B8
[000125b2] 284d                      movea.l   a5,a4
[000125b4] 2a55                      movea.l   (a5),a5
[000125b6] 60a4                      bra.s     $0001255C
[000125b8] 4a9f                      tst.l     (a7)+
[000125ba] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000125be] 4e5e                      unlk      a6
[000125c0] 4e75                      rts
[000125c2] 4e56 0000                 link      a6,#$0000
[000125c6] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[000125ca] 206e 0008                 movea.l   8(a6),a0
[000125ce] 2a50                      movea.l   (a0),a5
[000125d0] 4240                      clr.w     d0
[000125d2] 302e 000c                 move.w    12(a6),d0
[000125d6] 5840                      addq.w    #4,d0
[000125d8] b06d 0004                 cmp.w     4(a5),d0
[000125dc] 6508                      bcs.s     $000125E6
[000125de] 206e 0008                 movea.l   8(a6),a0
[000125e2] 2095                      move.l    (a5),(a0)
[000125e4] 6046                      bra.s     $0001262C
[000125e6] 284d                      movea.l   a5,a4
[000125e8] 4240                      clr.w     d0
[000125ea] 302e 000c                 move.w    12(a6),d0
[000125ee] e748                      lsl.w     #3,d0
[000125f0] 4840                      swap      d0
[000125f2] 4240                      clr.w     d0
[000125f4] 4840                      swap      d0
[000125f6] d9c0                      adda.l    d0,a4
[000125f8] 4240                      clr.w     d0
[000125fa] 302d 0004                 move.w    4(a5),d0
[000125fe] 906e 000c                 sub.w     12(a6),d0
[00012602] 3940 0004                 move.w    d0,4(a4)
[00012606] 4240                      clr.w     d0
[00012608] 302c 0004                 move.w    4(a4),d0
[0001260c] 4640                      not.w     d0
[0001260e] 3940 0006                 move.w    d0,6(a4)
[00012612] 2895                      move.l    (a5),(a4)
[00012614] 206e 0008                 movea.l   8(a6),a0
[00012618] 208c                      move.l    a4,(a0)
[0001261a] 3b6e 000c 0004            move.w    12(a6),4(a5)
[00012620] 4240                      clr.w     d0
[00012622] 302d 0004                 move.w    4(a5),d0
[00012626] 4640                      not.w     d0
[00012628] 3b40 0006                 move.w    d0,6(a5)
[0001262c] 23ee 0008 0001 3d00       move.l    8(a6),$00013D00
[00012634] 200d                      move.l    a5,d0
[00012636] 5080                      addq.l    #8,d0
[00012638] 4a9f                      tst.l     (a7)+
[0001263a] 4cdf 3000                 movem.l   (a7)+,a4-a5
[0001263e] 4e5e                      unlk      a6
[00012640] 4e75                      rts
[00012642] 4e56 0000                 link      a6,#$0000
[00012646] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001264a] 3e2e 0008                 move.w    8(a6),d7
[0001264e] de7c 003f                 add.w     #$003F,d7
[00012652] ec47                      asr.w     #6,d7
[00012654] ed47                      asl.w     #6,d7
[00012656] 3007                      move.w    d7,d0
[00012658] e740                      asl.w     #3,d0
[0001265a] 3e80                      move.w    d0,(a7)
[0001265c] 4eb9 0001 2306            jsr       _sbrk
[00012662] 2a40                      movea.l   d0,a5
[00012664] bbfc ffff ffff            cmpa.l    #$FFFFFFFF,a5
[0001266a] 6604                      bne.s     $00012670
[0001266c] 4280                      clr.l     d0
[0001266e] 602a                      bra.s     $0001269A
[00012670] 200d                      move.l    a5,d0
[00012672] 5280                      addq.l    #1,d0
[00012674] c0bc ffff fffe            and.l     #$FFFFFFFE,d0
[0001267a] 2a40                      movea.l   d0,a5
[0001267c] 284d                      movea.l   a5,a4
[0001267e] 3947 0004                 move.w    d7,4(a4)
[00012682] 4240                      clr.w     d0
[00012684] 302c 0004                 move.w    4(a4),d0
[00012688] 4640                      not.w     d0
[0001268a] 3940 0006                 move.w    d0,6(a4)
[0001268e] 2e8c                      move.l    a4,(a7)
[00012690] 5097                      addq.l    #8,(a7)
[00012692] 6110                      bsr.s     _free
[00012694] 2039 0001 3d00            move.l    $00013D00,d0
[0001269a] 4a9f                      tst.l     (a7)+
[0001269c] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000126a0] 4e5e                      unlk      a6
[000126a2] 4e75                      rts
_free:
[000126a4] 4e56 0000                 link      a6,#$0000
[000126a8] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[000126ac] 2a6e 0008                 movea.l   8(a6),a5
[000126b0] 518d                      subq.l    #8,a5
[000126b2] 4240                      clr.w     d0
[000126b4] 302d 0004                 move.w    4(a5),d0
[000126b8] 4241                      clr.w     d1
[000126ba] 322d 0006                 move.w    6(a5),d1
[000126be] b340                      eor.w     d1,d0
[000126c0] 4640                      not.w     d0
[000126c2] 4a40                      tst.w     d0
[000126c4] 6710                      beq.s     $000126D6
[000126c6] 3ebc 0003                 move.w    #$0003,(a7)
[000126ca] 4eb9 0001 250e            jsr       __errmal
[000126d0] 70ff                      moveq.l   #-1,d0
[000126d2] 6000 00c4                 bra       $00012798
[000126d6] 2879 0001 3d00            movea.l   $00013D00,a4
[000126dc] bbcc                      cmpa.l    a4,a5
[000126de] 6504                      bcs.s     $000126E4
[000126e0] bbd4                      cmpa.l    (a4),a5
[000126e2] 6510                      bcs.s     $000126F4
[000126e4] b9d4                      cmpa.l    (a4),a4
[000126e6] 6508                      bcs.s     $000126F0
[000126e8] bbd4                      cmpa.l    (a4),a5
[000126ea] 6308                      bls.s     $000126F4
[000126ec] bbcc                      cmpa.l    a4,a5
[000126ee] 6404                      bcc.s     $000126F4
[000126f0] 2854                      movea.l   (a4),a4
[000126f2] 60e8                      bra.s     $000126DC
[000126f4] bbcc                      cmpa.l    a4,a5
[000126f6] 6532                      bcs.s     $0001272A
[000126f8] 200d                      move.l    a5,d0
[000126fa] 4241                      clr.w     d1
[000126fc] 322d 0004                 move.w    4(a5),d1
[00012700] e749                      lsl.w     #3,d1
[00012702] 4841                      swap      d1
[00012704] 4241                      clr.w     d1
[00012706] 4841                      swap      d1
[00012708] d081                      add.l     d1,d0
[0001270a] 220c                      move.l    a4,d1
[0001270c] 4242                      clr.w     d2
[0001270e] 342c 0004                 move.w    4(a4),d2
[00012712] e74a                      lsl.w     #3,d2
[00012714] 4842                      swap      d2
[00012716] 4242                      clr.w     d2
[00012718] 4842                      swap      d2
[0001271a] d282                      add.l     d2,d1
[0001271c] b081                      cmp.l     d1,d0
[0001271e] 620a                      bhi.s     $0001272A
[00012720] 23cc 0001 3d00            move.l    a4,$00013D00
[00012726] 4240                      clr.w     d0
[00012728] 606e                      bra.s     $00012798
[0001272a] 200d                      move.l    a5,d0
[0001272c] 4241                      clr.w     d1
[0001272e] 322d 0004                 move.w    4(a5),d1
[00012732] e749                      lsl.w     #3,d1
[00012734] 4841                      swap      d1
[00012736] 4241                      clr.w     d1
[00012738] 4841                      swap      d1
[0001273a] d081                      add.l     d1,d0
[0001273c] b094                      cmp.l     (a4),d0
[0001273e] 661c                      bne.s     $0001275C
[00012740] 2054                      movea.l   (a4),a0
[00012742] 3028 0004                 move.w    4(a0),d0
[00012746] d16d 0004                 add.w     d0,4(a5)
[0001274a] 4240                      clr.w     d0
[0001274c] 302d 0004                 move.w    4(a5),d0
[00012750] 4640                      not.w     d0
[00012752] 3b40 0006                 move.w    d0,6(a5)
[00012756] 2054                      movea.l   (a4),a0
[00012758] 2a90                      move.l    (a0),(a5)
[0001275a] 6002                      bra.s     $0001275E
[0001275c] 2a94                      move.l    (a4),(a5)
[0001275e] 200c                      move.l    a4,d0
[00012760] 4241                      clr.w     d1
[00012762] 322c 0004                 move.w    4(a4),d1
[00012766] e749                      lsl.w     #3,d1
[00012768] 4841                      swap      d1
[0001276a] 4241                      clr.w     d1
[0001276c] 4841                      swap      d1
[0001276e] d081                      add.l     d1,d0
[00012770] bbc0                      cmpa.l    d0,a5
[00012772] 661a                      bne.s     $0001278E
[00012774] 4240                      clr.w     d0
[00012776] 302d 0004                 move.w    4(a5),d0
[0001277a] d16c 0004                 add.w     d0,4(a4)
[0001277e] 4240                      clr.w     d0
[00012780] 302c 0004                 move.w    4(a4),d0
[00012784] 4640                      not.w     d0
[00012786] 3940 0006                 move.w    d0,6(a4)
[0001278a] 2895                      move.l    (a5),(a4)
[0001278c] 6002                      bra.s     $00012790
[0001278e] 288d                      move.l    a5,(a4)
[00012790] 23cc 0001 3d00            move.l    a4,$00013D00
[00012796] 4240                      clr.w     d0
[00012798] 4a9f                      tst.l     (a7)+
[0001279a] 4cdf 3000                 movem.l   (a7)+,a4-a5
[0001279e] 4e5e                      unlk      a6
[000127a0] 4e75                      rts
_realloc:
[000127a2] 4e56 fffc                 link      a6,#$FFFC
[000127a6] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000127aa] 2a6e 0008                 movea.l   8(a6),a5
[000127ae] 2e8d                      move.l    a5,(a7)
[000127b0] 6100 fef2                 bsr       _free
[000127b4] 3e2e 000c                 move.w    12(a6),d7
[000127b8] 5e47                      addq.w    #7,d7
[000127ba] e64f                      lsr.w     #3,d7
[000127bc] 5247                      addq.w    #1,d7
[000127be] 3e87                      move.w    d7,(a7)
[000127c0] 6100 fd88                 bsr       $0001254A
[000127c4] 2d40 fffc                 move.l    d0,-4(a6)
[000127c8] 6604                      bne.s     $000127CE
[000127ca] 4280                      clr.l     d0
[000127cc] 604a                      bra.s     $00012818
[000127ce] 206e fffc                 movea.l   -4(a6),a0
[000127d2] 2850                      movea.l   (a0),a4
[000127d4] 508c                      addq.l    #8,a4
[000127d6] bbcc                      cmpa.l    a4,a5
[000127d8] 6732                      beq.s     $0001280C
[000127da] b9cd                      cmpa.l    a5,a4
[000127dc] 6410                      bcc.s     $000127EE
[000127de] 6006                      bra.s     $000127E6
[000127e0] 18dd                      move.b    (a5)+,(a4)+
[000127e2] 536e 000c                 subq.w    #1,12(a6)
[000127e6] 4a6e 000c                 tst.w     12(a6)
[000127ea] 66f4                      bne.s     $000127E0
[000127ec] 601e                      bra.s     $0001280C
[000127ee] 4280                      clr.l     d0
[000127f0] 302e 000c                 move.w    12(a6),d0
[000127f4] d9c0                      adda.l    d0,a4
[000127f6] 4280                      clr.l     d0
[000127f8] 302e 000c                 move.w    12(a6),d0
[000127fc] dbc0                      adda.l    d0,a5
[000127fe] 6006                      bra.s     $00012806
[00012800] 1925                      move.b    -(a5),-(a4)
[00012802] 536e 000c                 subq.w    #1,12(a6)
[00012806] 4a6e 000c                 tst.w     12(a6)
[0001280a] 66f4                      bne.s     $00012800
[0001280c] 3e87                      move.w    d7,(a7)
[0001280e] 2f2e fffc                 move.l    -4(a6),-(a7)
[00012812] 6100 fdae                 bsr       $000125C2
[00012816] 588f                      addq.l    #4,a7
[00012818] 4a9f                      tst.l     (a7)+
[0001281a] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[0001281e] 4e5e                      unlk      a6
[00012820] 4e75                      rts
_isatty:
[00012822] 4e56 0000                 link      a6,#$0000
[00012826] 48e7 0104                 movem.l   d7/a5,-(a7)
[0001282a] 3eae 0008                 move.w    8(a6),(a7)
[0001282e] 4eb9 0001 2208            jsr       __chkc
[00012834] 2a40                      movea.l   d0,a5
[00012836] 200d                      move.l    a5,d0
[00012838] 6604                      bne.s     $0001283E
[0001283a] 4240                      clr.w     d0
[0001283c] 600e                      bra.s     $0001284C
[0001283e] 082d 0001 0001            btst      #1,1(a5)
[00012844] 6604                      bne.s     $0001284A
[00012846] 4240                      clr.w     d0
[00012848] 6002                      bra.s     $0001284C
[0001284a] 7001                      moveq.l   #1,d0
[0001284c] 4a9f                      tst.l     (a7)+
[0001284e] 4cdf 2000                 movem.l   (a7)+,a5
[00012852] 4e5e                      unlk      a6
[00012854] 4e75                      rts
_isdev:
[00012856] 4e56 0000                 link      a6,#$0000
[0001285a] 48e7 0104                 movem.l   d7/a5,-(a7)
[0001285e] 3eae 0008                 move.w    8(a6),(a7)
[00012862] 4eb9 0001 2208            jsr       __chkc
[00012868] 2a40                      movea.l   d0,a5
[0001286a] 200d                      move.l    a5,d0
[0001286c] 6604                      bne.s     $00012872
[0001286e] 4240                      clr.w     d0
[00012870] 6006                      bra.s     $00012878
[00012872] 3015                      move.w    (a5),d0
[00012874] c07c 0106                 and.w     #$0106,d0
[00012878] 4a9f                      tst.l     (a7)+
[0001287a] 4cdf 2000                 movem.l   (a7)+,a5
[0001287e] 4e5e                      unlk      a6
[00012880] 4e75                      rts
_ttyname:
[00012882] 4e56 fffc                 link      a6,#$FFFC
[00012886] 3eae 0008                 move.w    8(a6),(a7)
[0001288a] 6196                      bsr.s     _isatty
[0001288c] 4a40                      tst.w     d0
[0001288e] 6708                      beq.s     $00012898
[00012890] 203c 0001 35ed            move.l    #$000135ED,d0
[00012896] 6002                      bra.s     $0001289A
[00012898] 4280                      clr.l     d0
[0001289a] 4e5e                      unlk      a6
[0001289c] 4e75                      rts
__flsbuf:
[0001289e] 4e56 fffe                 link      a6,#$FFFE
[000128a2] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[000128a6] 1e2e 0009                 move.b    9(a6),d7
[000128aa] 2a6e 000a                 movea.l   10(a6),a5
[000128ae] 4246                      clr.w     d6
[000128b0] 3a06                      move.w    d6,d5
[000128b2] 082d 0001 0003            btst      #1,3(a5)
[000128b8] 6606                      bne.s     $000128C0
[000128ba] 70ff                      moveq.l   #-1,d0
[000128bc] 6000 0124                 bra       $000129E2
[000128c0] 4aad 0004                 tst.l     4(a5)
[000128c4] 6656                      bne.s     $0001291C
[000128c6] 082d 0003 0003            btst      #3,3(a5)
[000128cc] 664e                      bne.s     $0001291C
[000128ce] 3ebc 0200                 move.w    #$0200,(a7)
[000128d2] 4eb9 0001 2516            jsr       _malloc
[000128d8] 2b40 0004                 move.l    d0,4(a5)
[000128dc] 2b40 0008                 move.l    d0,8(a5)
[000128e0] 6608                      bne.s     $000128EA
[000128e2] 006d 0008 0002            ori.w     #$0008,2(a5)
[000128e8] 6032                      bra.s     $0001291C
[000128ea] 006d 0004 0002            ori.w     #$0004,2(a5)
[000128f0] 3e95                      move.w    (a5),(a7)
[000128f2] 4eb9 0001 2822            jsr       _isatty
[000128f8] 4a40                      tst.w     d0
[000128fa] 6708                      beq.s     $00012904
[000128fc] 006d 0040 0002            ori.w     #$0040,2(a5)
[00012902] 6018                      bra.s     $0001291C
[00012904] 3b7c 01fe 000c            move.w    #$01FE,12(a5)
[0001290a] 1007                      move.b    d7,d0
[0001290c] 4880                      ext.w     d0
[0001290e] 226d 0008                 movea.l   8(a5),a1
[00012912] 1280                      move.b    d0,(a1)
[00012914] 52ad 0008                 addq.l    #1,8(a5)
[00012918] 6000 00c8                 bra       $000129E2
[0001291c] 082d 0003 0003            btst      #3,3(a5)
[00012922] 670e                      beq.s     $00012932
[00012924] 41ee fffe                 lea.l     -2(a6),a0
[00012928] 2b48 0004                 move.l    a0,4(a5)
[0001292c] 2008                      move.l    a0,d0
[0001292e] 2b40 0008                 move.l    d0,8(a5)
[00012932] 206d 0008                 movea.l   8(a5),a0
[00012936] 1087                      move.b    d7,(a0)
[00012938] 52ad 0008                 addq.l    #1,8(a5)
[0001293c] 082d 0003 0003            btst      #3,3(a5)
[00012942] 671a                      beq.s     $0001295E
[00012944] 7a01                      moveq.l   #1,d5
[00012946] 3e85                      move.w    d5,(a7)
[00012948] 2f2d 0004                 move.l    4(a5),-(a7)
[0001294c] 3f15                      move.w    (a5),-(a7)
[0001294e] 4eb9 0001 226a            jsr       _write
[00012954] 5c8f                      addq.l    #6,a7
[00012956] 3c00                      move.w    d0,d6
[00012958] 426d 000c                 clr.w     12(a5)
[0001295c] 606e                      bra.s     $000129CC
[0001295e] 082d 0006 0003            btst      #6,3(a5)
[00012964] 673e                      beq.s     $000129A4
[00012966] be3c 000a                 cmp.b     #$0A,d7
[0001296a] 6710                      beq.s     $0001297C
[0001296c] 202d 0004                 move.l    4(a5),d0
[00012970] d0bc 0000 0200            add.l     #$00000200,d0
[00012976] b0ad 0008                 cmp.l     8(a5),d0
[0001297a] 6222                      bhi.s     $0001299E
[0001297c] 202d 0008                 move.l    8(a5),d0
[00012980] 90ad 0004                 sub.l     4(a5),d0
[00012984] 3a00                      move.w    d0,d5
[00012986] 3e85                      move.w    d5,(a7)
[00012988] 2f2d 0004                 move.l    4(a5),-(a7)
[0001298c] 3f15                      move.w    (a5),-(a7)
[0001298e] 4eb9 0001 226a            jsr       _write
[00012994] 5c8f                      addq.l    #6,a7
[00012996] 3c00                      move.w    d0,d6
[00012998] 2b6d 0004 0008            move.l    4(a5),8(a5)
[0001299e] 426d 000c                 clr.w     12(a5)
[000129a2] 6028                      bra.s     $000129CC
[000129a4] 202d 0008                 move.l    8(a5),d0
[000129a8] 90ad 0004                 sub.l     4(a5),d0
[000129ac] 3a00                      move.w    d0,d5
[000129ae] 3e85                      move.w    d5,(a7)
[000129b0] 2f2d 0004                 move.l    4(a5),-(a7)
[000129b4] 3f15                      move.w    (a5),-(a7)
[000129b6] 4eb9 0001 226a            jsr       _write
[000129bc] 5c8f                      addq.l    #6,a7
[000129be] 3c00                      move.w    d0,d6
[000129c0] 3b7c 01ff 000c            move.w    #$01FF,12(a5)
[000129c6] 2b6d 0004 0008            move.l    4(a5),8(a5)
[000129cc] ba46                      cmp.w     d6,d5
[000129ce] 670a                      beq.s     $000129DA
[000129d0] 006d 0010 0002            ori.w     #$0010,2(a5)
[000129d6] 70ff                      moveq.l   #-1,d0
[000129d8] 6008                      bra.s     $000129E2
[000129da] 1007                      move.b    d7,d0
[000129dc] 4880                      ext.w     d0
[000129de] c07c 00ff                 and.w     #$00FF,d0
[000129e2] 4a9f                      tst.l     (a7)+
[000129e4] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[000129e8] 4e5e                      unlk      a6
[000129ea] 4e75                      rts
_fputc:
[000129ec] 4e56 0000                 link      a6,#$0000
[000129f0] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000129f4] 1e2e 0009                 move.b    9(a6),d7
[000129f8] 2a6e 000a                 movea.l   10(a6),a5
[000129fc] 536d 000c                 subq.w    #1,12(a5)
[00012a00] 6d16                      blt.s     $00012A18
[00012a02] 1007                      move.b    d7,d0
[00012a04] 4880                      ext.w     d0
[00012a06] 226d 0008                 movea.l   8(a5),a1
[00012a0a] 1280                      move.b    d0,(a1)
[00012a0c] c07c 00ff                 and.w     #$00FF,d0
[00012a10] 52ad 0008                 addq.l    #1,8(a5)
[00012a14] 6012                      bra.s     $00012A28
[00012a16] 6010                      bra.s     $00012A28
[00012a18] 2e8d                      move.l    a5,(a7)
[00012a1a] 1007                      move.b    d7,d0
[00012a1c] 4880                      ext.w     d0
[00012a1e] 3f00                      move.w    d0,-(a7)
[00012a20] 4eb9 0001 289e            jsr       __flsbuf
[00012a26] 548f                      addq.l    #2,a7
[00012a28] 4a9f                      tst.l     (a7)+
[00012a2a] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00012a2e] 4e5e                      unlk      a6
[00012a30] 4e75                      rts
_fputn:
[00012a32] 4e56 0000                 link      a6,#$0000
[00012a36] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00012a3a] 2a6e 0008                 movea.l   8(a6),a5
[00012a3e] 3e2e 000c                 move.w    12(a6),d7
[00012a42] 286e 000e                 movea.l   14(a6),a4
[00012a46] 082c 0003 0003            btst      #3,3(a4)
[00012a4c] 6724                      beq.s     $00012A72
[00012a4e] 426c 000c                 clr.w     12(a4)
[00012a52] 3e87                      move.w    d7,(a7)
[00012a54] 2f0d                      move.l    a5,-(a7)
[00012a56] 3f14                      move.w    (a4),-(a7)
[00012a58] 4eb9 0001 226a            jsr       _write
[00012a5e] 5c8f                      addq.l    #6,a7
[00012a60] b047                      cmp.w     d7,d0
[00012a62] 670a                      beq.s     $00012A6E
[00012a64] 006c 0010 0002            ori.w     #$0010,2(a4)
[00012a6a] 70ff                      moveq.l   #-1,d0
[00012a6c] 602a                      bra.s     $00012A98
[00012a6e] 4240                      clr.w     d0
[00012a70] 6026                      bra.s     $00012A98
[00012a72] 601a                      bra.s     $00012A8E
[00012a74] 2e8c                      move.l    a4,(a7)
[00012a76] 101d                      move.b    (a5)+,d0
[00012a78] 4880                      ext.w     d0
[00012a7a] 3f00                      move.w    d0,-(a7)
[00012a7c] 4eb9 0001 29ec            jsr       _fputc
[00012a82] 548f                      addq.l    #2,a7
[00012a84] b07c ffff                 cmp.w     #$FFFF,d0
[00012a88] 6604                      bne.s     $00012A8E
[00012a8a] 70ff                      moveq.l   #-1,d0
[00012a8c] 600a                      bra.s     $00012A98
[00012a8e] 3007                      move.w    d7,d0
[00012a90] 5347                      subq.w    #1,d7
[00012a92] 4a40                      tst.w     d0
[00012a94] 66de                      bne.s     $00012A74
[00012a96] 4240                      clr.w     d0
[00012a98] 4a9f                      tst.l     (a7)+
[00012a9a] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00012a9e] 4e5e                      unlk      a6
[00012aa0] 4e75                      rts
__pftoa:
[00012aa2] 4e56 fff8                 link      a6,#$FFF8
[00012aa6] 4a6e 0010                 tst.w     16(a6)
[00012aaa] 6c04                      bge.s     $00012AB0
[00012aac] 7006                      moveq.l   #6,d0
[00012aae] 6004                      bra.s     $00012AB4
[00012ab0] 302e 0010                 move.w    16(a6),d0
[00012ab4] 3d40 0010                 move.w    d0,16(a6)
[00012ab8] 206e 0008                 movea.l   8(a6),a0
[00012abc] 2010                      move.l    (a0),d0
[00012abe] 2d40 fffc                 move.l    d0,-4(a6)
[00012ac2] 3eae 0010                 move.w    16(a6),(a7)
[00012ac6] 2f2e 000c                 move.l    12(a6),-(a7)
[00012aca] 2f2e fffc                 move.l    -4(a6),-(a7)
[00012ace] 4eb9 0000 0000            jsr       $00000000
[00012ad4] 508f                      addq.l    #8,a7
[00012ad6] 4e5e                      unlk      a6
[00012ad8] 4e75                      rts
__petoa:
[00012ada] 4e56 fff8                 link      a6,#$FFF8
[00012ade] 4a6e 0010                 tst.w     16(a6)
[00012ae2] 6c04                      bge.s     $00012AE8
[00012ae4] 7006                      moveq.l   #6,d0
[00012ae6] 6004                      bra.s     $00012AEC
[00012ae8] 302e 0010                 move.w    16(a6),d0
[00012aec] 3d40 0010                 move.w    d0,16(a6)
[00012af0] 206e 0008                 movea.l   8(a6),a0
[00012af4] 2010                      move.l    (a0),d0
[00012af6] 2d40 fffc                 move.l    d0,-4(a6)
[00012afa] 3eae 0010                 move.w    16(a6),(a7)
[00012afe] 2f2e 000c                 move.l    12(a6),-(a7)
[00012b02] 2f2e fffc                 move.l    -4(a6),-(a7)
[00012b06] 4eb9 0000 0000            jsr       $00000000
[00012b0c] 508f                      addq.l    #8,a7
[00012b0e] 4e5e                      unlk      a6
[00012b10] 4e75                      rts
__pgtoa:
[00012b12] 4e56 fff8                 link      a6,#$FFF8
[00012b16] 3eae 0010                 move.w    16(a6),(a7)
[00012b1a] 2f2e 000c                 move.l    12(a6),-(a7)
[00012b1e] 2f2e 0008                 move.l    8(a6),-(a7)
[00012b22] 6100 ff7e                 bsr       __pftoa
[00012b26] 508f                      addq.l    #8,a7
[00012b28] 2d40 fffc                 move.l    d0,-4(a6)
[00012b2c] 2eae 000c                 move.l    12(a6),(a7)
[00012b30] 4eb9 0001 1ae4            jsr       _strlen
[00012b36] 322e 0010                 move.w    16(a6),d1
[00012b3a] 5e41                      addq.w    #7,d1
[00012b3c] b041                      cmp.w     d1,d0
[00012b3e] 6f14                      ble.s     $00012B54
[00012b40] 3eae 0010                 move.w    16(a6),(a7)
[00012b44] 2f2e 000c                 move.l    12(a6),-(a7)
[00012b48] 2f2e 0008                 move.l    8(a6),-(a7)
[00012b4c] 618c                      bsr.s     __petoa
[00012b4e] 508f                      addq.l    #8,a7
[00012b50] 2d40 fffc                 move.l    d0,-4(a6)
[00012b54] 202e fffc                 move.l    -4(a6),d0
[00012b58] 4e5e                      unlk      a6
[00012b5a] 4e75                      rts
__doprt:
[00012b5c] 4e56 fde4                 link      a6,#$FDE4
[00012b60] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00012b64] 2a6e 0010                 movea.l   16(a6),a5
[00012b68] 426e fdec                 clr.w     -532(a6)
[00012b6c] 4aae 000c                 tst.l     12(a6)
[00012b70] 6700 0468                 beq       $00012FDA
[00012b74] 6000 0460                 bra       $00012FD6
[00012b78] 4245                      clr.w     d5
[00012b7a] 2d6e 000c fff2            move.l    12(a6),-14(a6)
[00012b80] 6006                      bra.s     $00012B88
[00012b82] 52ae fff2                 addq.l    #1,-14(a6)
[00012b86] 5245                      addq.w    #1,d5
[00012b88] 206e fff2                 movea.l   -14(a6),a0
[00012b8c] 4a10                      tst.b     (a0)
[00012b8e] 670a                      beq.s     $00012B9A
[00012b90] 206e fff2                 movea.l   -14(a6),a0
[00012b94] 0c10 0025                 cmpi.b    #$25,(a0)
[00012b98] 66e8                      bne.s     $00012B82
[00012b9a] 4a45                      tst.w     d5
[00012b9c] 6f1c                      ble.s     $00012BBA
[00012b9e] 2eae 0008                 move.l    8(a6),(a7)
[00012ba2] 3f05                      move.w    d5,-(a7)
[00012ba4] 2f2e 000c                 move.l    12(a6),-(a7)
[00012ba8] 4eb9 0001 2a32            jsr       _fputn
[00012bae] 5c8f                      addq.l    #6,a7
[00012bb0] 2d6e fff2 000c            move.l    -14(a6),12(a6)
[00012bb6] db6e fdec                 add.w     d5,-532(a6)
[00012bba] 206e 000c                 movea.l   12(a6),a0
[00012bbe] 0c10 0025                 cmpi.b    #$25,(a0)
[00012bc2] 40c0                      move.w    sr,d0
[00012bc4] 52ae 000c                 addq.l    #1,12(a6)
[00012bc8] 44c0                      move.b    d0,ccr
[00012bca] 6600 040e                 bne       $00012FDA
[00012bce] 426e fdea                 clr.w     -534(a6)
[00012bd2] 206e 000c                 movea.l   12(a6),a0
[00012bd6] 1010                      move.b    (a0),d0
[00012bd8] 4880                      ext.w     d0
[00012bda] 1e00                      move.b    d0,d7
[00012bdc] b07c 002d                 cmp.w     #$002D,d0
[00012be0] 40c0                      move.w    sr,d0
[00012be2] 52ae 000c                 addq.l    #1,12(a6)
[00012be6] 44c0                      move.b    d0,ccr
[00012be8] 660e                      bne.s     $00012BF8
[00012bea] 206e 000c                 movea.l   12(a6),a0
[00012bee] 1e10                      move.b    (a0),d7
[00012bf0] 52ae 000c                 addq.l    #1,12(a6)
[00012bf4] 526e fdea                 addq.w    #1,-534(a6)
[00012bf8] 1d7c 0020 fff6            move.b    #$20,-10(a6)
[00012bfe] be3c 0030                 cmp.b     #$30,d7
[00012c02] 660e                      bne.s     $00012C12
[00012c04] 1d47 fff6                 move.b    d7,-10(a6)
[00012c08] 206e 000c                 movea.l   12(a6),a0
[00012c0c] 1e10                      move.b    (a0),d7
[00012c0e] 52ae 000c                 addq.l    #1,12(a6)
[00012c12] 3d7c ffff fdf0            move.w    #$FFFF,-528(a6)
[00012c18] be3c 002a                 cmp.b     #$2A,d7
[00012c1c] 661a                      bne.s     $00012C38
[00012c1e] 2d4d fff8                 move.l    a5,-8(a6)
[00012c22] 206e fff8                 movea.l   -8(a6),a0
[00012c26] 3d50 fdf0                 move.w    (a0),-528(a6)
[00012c2a] 548d                      addq.l    #2,a5
[00012c2c] 206e 000c                 movea.l   12(a6),a0
[00012c30] 1e10                      move.b    (a0),d7
[00012c32] 52ae 000c                 addq.l    #1,12(a6)
[00012c36] 6038                      bra.s     $00012C70
[00012c38] 602a                      bra.s     $00012C64
[00012c3a] 4a6e fdf0                 tst.w     -528(a6)
[00012c3e] 6c04                      bge.s     $00012C44
[00012c40] 426e fdf0                 clr.w     -528(a6)
[00012c44] 1007                      move.b    d7,d0
[00012c46] 4880                      ext.w     d0
[00012c48] 322e fdf0                 move.w    -528(a6),d1
[00012c4c] c3fc 000a                 muls.w    #$000A,d1
[00012c50] d041                      add.w     d1,d0
[00012c52] d07c ffd0                 add.w     #$FFD0,d0
[00012c56] 3d40 fdf0                 move.w    d0,-528(a6)
[00012c5a] 206e 000c                 movea.l   12(a6),a0
[00012c5e] 1e10                      move.b    (a0),d7
[00012c60] 52ae 000c                 addq.l    #1,12(a6)
[00012c64] be3c 0030                 cmp.b     #$30,d7
[00012c68] 6d06                      blt.s     $00012C70
[00012c6a] be3c 0039                 cmp.b     #$39,d7
[00012c6e] 6fca                      ble.s     $00012C3A
[00012c70] 7cff                      moveq.l   #-1,d6
[00012c72] be3c 002e                 cmp.b     #$2E,d7
[00012c76] 660c                      bne.s     $00012C84
[00012c78] 4246                      clr.w     d6
[00012c7a] 206e 000c                 movea.l   12(a6),a0
[00012c7e] 1e10                      move.b    (a0),d7
[00012c80] 52ae 000c                 addq.l    #1,12(a6)
[00012c84] be3c 002a                 cmp.b     #$2A,d7
[00012c88] 6618                      bne.s     $00012CA2
[00012c8a] 2d4d fff8                 move.l    a5,-8(a6)
[00012c8e] 206e fff8                 movea.l   -8(a6),a0
[00012c92] 3c10                      move.w    (a0),d6
[00012c94] 548d                      addq.l    #2,a5
[00012c96] 206e 000c                 movea.l   12(a6),a0
[00012c9a] 1e10                      move.b    (a0),d7
[00012c9c] 52ae 000c                 addq.l    #1,12(a6)
[00012ca0] 602a                      bra.s     $00012CCC
[00012ca2] 601c                      bra.s     $00012CC0
[00012ca4] 1007                      move.b    d7,d0
[00012ca6] 4880                      ext.w     d0
[00012ca8] 3206                      move.w    d6,d1
[00012caa] c3fc 000a                 muls.w    #$000A,d1
[00012cae] d041                      add.w     d1,d0
[00012cb0] 3c00                      move.w    d0,d6
[00012cb2] dc7c ffd0                 add.w     #$FFD0,d6
[00012cb6] 206e 000c                 movea.l   12(a6),a0
[00012cba] 1e10                      move.b    (a0),d7
[00012cbc] 52ae 000c                 addq.l    #1,12(a6)
[00012cc0] be3c 0030                 cmp.b     #$30,d7
[00012cc4] 6d06                      blt.s     $00012CCC
[00012cc6] be3c 0039                 cmp.b     #$39,d7
[00012cca] 6fd8                      ble.s     $00012CA4
[00012ccc] 426e fde8                 clr.w     -536(a6)
[00012cd0] be3c 006c                 cmp.b     #$6C,d7
[00012cd4] 6706                      beq.s     $00012CDC
[00012cd6] be3c 004c                 cmp.b     #$4C,d7
[00012cda] 660e                      bne.s     $00012CEA
[00012cdc] 526e fde8                 addq.w    #1,-536(a6)
[00012ce0] 206e 000c                 movea.l   12(a6),a0
[00012ce4] 1e10                      move.b    (a0),d7
[00012ce6] 52ae 000c                 addq.l    #1,12(a6)
[00012cea] 41ee fdf2                 lea.l     -526(a6),a0
[00012cee] 2d48 fff2                 move.l    a0,-14(a6)
[00012cf2] 1007                      move.b    d7,d0
[00012cf4] 4880                      ext.w     d0
[00012cf6] 6000 020c                 bra       $00012F04
[00012cfa] 526e fde8                 addq.w    #1,-536(a6)
[00012cfe] 4a6e fde8                 tst.w     -536(a6)
[00012d02] 6708                      beq.s     $00012D0C
[00012d04] 203c 0001 23de            move.l    #___prtld,d0
[00012d0a] 6006                      bra.s     $00012D12
[00012d0c] 203c 0001 2370            move.l    #___prtsh,d0
[00012d12] 23c0 0001 3ef0            move.l    d0,$00013EF0
[00012d18] 2eb9 0001 3ef0            move.l    $00013EF0,(a7)
[00012d1e] 3f3c 0001                 move.w    #$0001,-(a7)
[00012d22] 3f3c 000a                 move.w    #$000A,-(a7)
[00012d26] 2f0e                      move.l    a6,-(a7)
[00012d28] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012d2e] 2f0d                      move.l    a5,-(a7)
[00012d30] 4eb9 0001 2494            jsr       ___prtin
[00012d36] dffc 0000 000c            adda.l    #$0000000C,a7
[00012d3c] 4a6e fde8                 tst.w     -536(a6)
[00012d40] 6704                      beq.s     $00012D46
[00012d42] 7004                      moveq.l   #4,d0
[00012d44] 6002                      bra.s     $00012D48
[00012d46] 7002                      moveq.l   #2,d0
[00012d48] 48c0                      ext.l     d0
[00012d4a] dbc0                      adda.l    d0,a5
[00012d4c] 6000 01ce                 bra       $00012F1C
[00012d50] 526e fde8                 addq.w    #1,-536(a6)
[00012d54] 4a6e fde8                 tst.w     -536(a6)
[00012d58] 6708                      beq.s     $00012D62
[00012d5a] 203c 0001 23de            move.l    #___prtld,d0
[00012d60] 6006                      bra.s     $00012D68
[00012d62] 203c 0001 2370            move.l    #___prtsh,d0
[00012d68] 23c0 0001 3ef0            move.l    d0,$00013EF0
[00012d6e] 2eb9 0001 3ef0            move.l    $00013EF0,(a7)
[00012d74] 4267                      clr.w     -(a7)
[00012d76] 3f3c 000a                 move.w    #$000A,-(a7)
[00012d7a] 2f0e                      move.l    a6,-(a7)
[00012d7c] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012d82] 2f0d                      move.l    a5,-(a7)
[00012d84] 4eb9 0001 2494            jsr       ___prtin
[00012d8a] dffc 0000 000c            adda.l    #$0000000C,a7
[00012d90] 4a6e fde8                 tst.w     -536(a6)
[00012d94] 6704                      beq.s     $00012D9A
[00012d96] 7004                      moveq.l   #4,d0
[00012d98] 6002                      bra.s     $00012D9C
[00012d9a] 7002                      moveq.l   #2,d0
[00012d9c] 48c0                      ext.l     d0
[00012d9e] dbc0                      adda.l    d0,a5
[00012da0] 6000 017a                 bra       $00012F1C
[00012da4] 526e fde8                 addq.w    #1,-536(a6)
[00012da8] 4a6e fde8                 tst.w     -536(a6)
[00012dac] 6708                      beq.s     $00012DB6
[00012dae] 203c 0001 23de            move.l    #___prtld,d0
[00012db4] 6006                      bra.s     $00012DBC
[00012db6] 203c 0001 2370            move.l    #___prtsh,d0
[00012dbc] 23c0 0001 3ef0            move.l    d0,$00013EF0
[00012dc2] 2eb9 0001 3ef0            move.l    $00013EF0,(a7)
[00012dc8] 4267                      clr.w     -(a7)
[00012dca] 3f3c 0008                 move.w    #$0008,-(a7)
[00012dce] 2f0e                      move.l    a6,-(a7)
[00012dd0] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012dd6] 2f0d                      move.l    a5,-(a7)
[00012dd8] 4eb9 0001 2494            jsr       ___prtin
[00012dde] dffc 0000 000c            adda.l    #$0000000C,a7
[00012de4] 4a6e fde8                 tst.w     -536(a6)
[00012de8] 6704                      beq.s     $00012DEE
[00012dea] 7004                      moveq.l   #4,d0
[00012dec] 6002                      bra.s     $00012DF0
[00012dee] 7002                      moveq.l   #2,d0
[00012df0] 48c0                      ext.l     d0
[00012df2] dbc0                      adda.l    d0,a5
[00012df4] 6000 0126                 bra       $00012F1C
[00012df8] 526e fde8                 addq.w    #1,-536(a6)
[00012dfc] 4a6e fde8                 tst.w     -536(a6)
[00012e00] 6708                      beq.s     $00012E0A
[00012e02] 203c 0001 23de            move.l    #___prtld,d0
[00012e08] 6006                      bra.s     $00012E10
[00012e0a] 203c 0001 2370            move.l    #___prtsh,d0
[00012e10] 23c0 0001 3ef0            move.l    d0,$00013EF0
[00012e16] 2eb9 0001 3ef0            move.l    $00013EF0,(a7)
[00012e1c] 4267                      clr.w     -(a7)
[00012e1e] 3f3c 0010                 move.w    #$0010,-(a7)
[00012e22] 2f0e                      move.l    a6,-(a7)
[00012e24] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012e2a] 2f0d                      move.l    a5,-(a7)
[00012e2c] 4eb9 0001 2494            jsr       ___prtin
[00012e32] dffc 0000 000c            adda.l    #$0000000C,a7
[00012e38] 4a6e fde8                 tst.w     -536(a6)
[00012e3c] 6704                      beq.s     $00012E42
[00012e3e] 7004                      moveq.l   #4,d0
[00012e40] 6002                      bra.s     $00012E44
[00012e42] 7002                      moveq.l   #2,d0
[00012e44] 48c0                      ext.l     d0
[00012e46] dbc0                      adda.l    d0,a5
[00012e48] 6000 00d2                 bra       $00012F1C
[00012e4c] 2d4d fffc                 move.l    a5,-4(a6)
[00012e50] 206e fffc                 movea.l   -4(a6),a0
[00012e54] 2d50 fff2                 move.l    (a0),-14(a6)
[00012e58] 588d                      addq.l    #4,a5
[00012e5a] 6000 00c0                 bra       $00012F1C
[00012e5e] 2d4d fff8                 move.l    a5,-8(a6)
[00012e62] 206e fff8                 movea.l   -8(a6),a0
[00012e66] 3010                      move.w    (a0),d0
[00012e68] c07c 00ff                 and.w     #$00FF,d0
[00012e6c] 1d40 fdf2                 move.b    d0,-526(a6)
[00012e70] 422e fdf3                 clr.b     -525(a6)
[00012e74] 548d                      addq.l    #2,a5
[00012e76] 6000 00a4                 bra       $00012F1C
[00012e7a] 1007                      move.b    d7,d0
[00012e7c] 4880                      ext.w     d0
[00012e7e] 3e80                      move.w    d0,(a7)
[00012e80] 3f06                      move.w    d6,-(a7)
[00012e82] 2f0e                      move.l    a6,-(a7)
[00012e84] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012e8a] 2f0d                      move.l    a5,-(a7)
[00012e8c] 4eb9 0001 2ada            jsr       __petoa
[00012e92] dffc 0000 000a            adda.l    #$0000000A,a7
[00012e98] 588d                      addq.l    #4,a5
[00012e9a] 7cff                      moveq.l   #-1,d6
[00012e9c] 6000 007e                 bra       $00012F1C
[00012ea0] 1007                      move.b    d7,d0
[00012ea2] 4880                      ext.w     d0
[00012ea4] 3e80                      move.w    d0,(a7)
[00012ea6] 3f06                      move.w    d6,-(a7)
[00012ea8] 2f0e                      move.l    a6,-(a7)
[00012eaa] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012eb0] 2f0d                      move.l    a5,-(a7)
[00012eb2] 4eb9 0001 2aa2            jsr       __pftoa
[00012eb8] dffc 0000 000a            adda.l    #$0000000A,a7
[00012ebe] 588d                      addq.l    #4,a5
[00012ec0] 7cff                      moveq.l   #-1,d6
[00012ec2] 6058                      bra.s     $00012F1C
[00012ec4] 1007                      move.b    d7,d0
[00012ec6] 4880                      ext.w     d0
[00012ec8] 3e80                      move.w    d0,(a7)
[00012eca] 3f06                      move.w    d6,-(a7)
[00012ecc] 2f0e                      move.l    a6,-(a7)
[00012ece] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012ed4] 2f0d                      move.l    a5,-(a7)
[00012ed6] 4eb9 0001 2b12            jsr       __pgtoa
[00012edc] dffc 0000 000a            adda.l    #$0000000A,a7
[00012ee2] 588d                      addq.l    #4,a5
[00012ee4] 7cff                      moveq.l   #-1,d6
[00012ee6] 6034                      bra.s     $00012F1C
[00012ee8] 2eae 0008                 move.l    8(a6),(a7)
[00012eec] 1007                      move.b    d7,d0
[00012eee] 4880                      ext.w     d0
[00012ef0] 3f00                      move.w    d0,-(a7)
[00012ef2] 4eb9 0001 29ec            jsr       _fputc
[00012ef8] 548f                      addq.l    #2,a7
[00012efa] 526e fdec                 addq.w    #1,-532(a6)
[00012efe] 6000 00d6                 bra       $00012FD6
[00012f02] 6018                      bra.s     $00012F1C
[00012f04] 907c 0043                 sub.w     #$0043,d0
[00012f08] b07c 0035                 cmp.w     #$0035,d0
[00012f0c] 62da                      bhi.s     $00012EE8
[00012f0e] e540                      asl.w     #2,d0
[00012f10] 3040                      movea.w   d0,a0
[00012f12] d1fc 0001 3d04            adda.l    #$00013D04,a0
[00012f18] 2050                      movea.l   (a0),a0
[00012f1a] 4ed0                      jmp       (a0)
[00012f1c] 2eae fff2                 move.l    -14(a6),(a7)
[00012f20] 4eb9 0001 1ae4            jsr       _strlen
[00012f26] 3a00                      move.w    d0,d5
[00012f28] bc45                      cmp.w     d5,d6
[00012f2a] 6c06                      bge.s     $00012F32
[00012f2c] 4a46                      tst.w     d6
[00012f2e] 6d02                      blt.s     $00012F32
[00012f30] 3a06                      move.w    d6,d5
[00012f32] 302e fdf0                 move.w    -528(a6),d0
[00012f36] 9045                      sub.w     d5,d0
[00012f38] 3d40 fdee                 move.w    d0,-530(a6)
[00012f3c] 4a6e fdea                 tst.w     -534(a6)
[00012f40] 6658                      bne.s     $00012F9A
[00012f42] 0c2e 0030 fff6            cmpi.b    #$30,-10(a6)
[00012f48] 662a                      bne.s     $00012F74
[00012f4a] 206e fff2                 movea.l   -14(a6),a0
[00012f4e] 0c10 002d                 cmpi.b    #$2D,(a0)
[00012f52] 6620                      bne.s     $00012F74
[00012f54] 5345                      subq.w    #1,d5
[00012f56] 2eae 0008                 move.l    8(a6),(a7)
[00012f5a] 206e fff2                 movea.l   -14(a6),a0
[00012f5e] 1010                      move.b    (a0),d0
[00012f60] 4880                      ext.w     d0
[00012f62] 3f00                      move.w    d0,-(a7)
[00012f64] 4eb9 0001 29ec            jsr       _fputc
[00012f6a] 548f                      addq.l    #2,a7
[00012f6c] 52ae fff2                 addq.l    #1,-14(a6)
[00012f70] 526e fdec                 addq.w    #1,-532(a6)
[00012f74] 6018                      bra.s     $00012F8E
[00012f76] 2eae 0008                 move.l    8(a6),(a7)
[00012f7a] 102e fff6                 move.b    -10(a6),d0
[00012f7e] 4880                      ext.w     d0
[00012f80] 3f00                      move.w    d0,-(a7)
[00012f82] 4eb9 0001 29ec            jsr       _fputc
[00012f88] 548f                      addq.l    #2,a7
[00012f8a] 526e fdec                 addq.w    #1,-532(a6)
[00012f8e] 302e fdee                 move.w    -530(a6),d0
[00012f92] 536e fdee                 subq.w    #1,-530(a6)
[00012f96] 4a40                      tst.w     d0
[00012f98] 6edc                      bgt.s     $00012F76
[00012f9a] 2eae 0008                 move.l    8(a6),(a7)
[00012f9e] 3f05                      move.w    d5,-(a7)
[00012fa0] 2f2e fff2                 move.l    -14(a6),-(a7)
[00012fa4] 4eb9 0001 2a32            jsr       _fputn
[00012faa] 5c8f                      addq.l    #6,a7
[00012fac] db6e fdec                 add.w     d5,-532(a6)
[00012fb0] 6018                      bra.s     $00012FCA
[00012fb2] 2eae 0008                 move.l    8(a6),(a7)
[00012fb6] 102e fff6                 move.b    -10(a6),d0
[00012fba] 4880                      ext.w     d0
[00012fbc] 3f00                      move.w    d0,-(a7)
[00012fbe] 4eb9 0001 29ec            jsr       _fputc
[00012fc4] 548f                      addq.l    #2,a7
[00012fc6] 526e fdec                 addq.w    #1,-532(a6)
[00012fca] 302e fdee                 move.w    -530(a6),d0
[00012fce] 536e fdee                 subq.w    #1,-530(a6)
[00012fd2] 4a40                      tst.w     d0
[00012fd4] 6edc                      bgt.s     $00012FB2
[00012fd6] 6000 fba0                 bra       $00012B78
[00012fda] 302e fdec                 move.w    -532(a6),d0
[00012fde] 4a9f                      tst.l     (a7)+
[00012fe0] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00012fe4] 4e5e                      unlk      a6
[00012fe6] 4e75                      rts
_lseek:
[00012fe8] 4e56 0000                 link      a6,#$0000
[00012fec] 48e7 0104                 movem.l   d7/a5,-(a7)
[00012ff0] 3eae 0008                 move.w    8(a6),(a7)
[00012ff4] 4eb9 0001 2208            jsr       __chkc
[00012ffa] 2a40                      movea.l   d0,a5
[00012ffc] 200d                      move.l    a5,d0
[00012ffe] 6616                      bne.s     $00013016
[00013000] 33fc 0009 0001 3cce       move.w    #$0009,$00013CCE
[00013008] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[00013012] 70ff                      moveq.l   #-1,d0
[00013014] 6024                      bra.s     $0001303A
[00013016] 3eae 000e                 move.w    14(a6),(a7)
[0001301a] 3f2d 0004                 move.w    4(a5),-(a7)
[0001301e] 2f2e 000a                 move.l    10(a6),-(a7)
[00013022] 3f3c 0042                 move.w    #$0042,-(a7)
[00013026] 4eb9 0001 1938            jsr       _trap
[0001302c] 508f                      addq.l    #8,a7
[0001302e] 2b40 0006                 move.l    d0,6(a5)
[00013032] 0255 ffdf                 andi.w    #$FFDF,(a5)
[00013036] 202d 0006                 move.l    6(a5),d0
[0001303a] 4a9f                      tst.l     (a7)+
[0001303c] 4cdf 2000                 movem.l   (a7)+,a5
[00013040] 4e5e                      unlk      a6
[00013042] 4e75                      rts
_tell:
[00013044] 4e56 fffc                 link      a6,#$FFFC
[00013048] 3ebc 0001                 move.w    #$0001,(a7)
[0001304c] 42a7                      clr.l     -(a7)
[0001304e] 3f2e 0008                 move.w    8(a6),-(a7)
[00013052] 6194                      bsr.s     _lseek
[00013054] 5c8f                      addq.l    #6,a7
[00013056] 4e5e                      unlk      a6
[00013058] 4e75                      rts
__open:
[0001305a] 4e56 0000                 link      a6,#$0000
[0001305e] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00013062] 4eb9 0001 2112            jsr       __allocc
[00013068] 3e00                      move.w    d0,d7
[0001306a] be7c ffff                 cmp.w     #$FFFF,d7
[0001306e] 6606                      bne.s     $00013076
[00013070] 70ff                      moveq.l   #-1,d0
[00013072] 6000 00d6                 bra       $0001314A
[00013076] 3e87                      move.w    d7,(a7)
[00013078] 4eb9 0001 219a            jsr       ___chini
[0001307e] 3007                      move.w    d7,d0
[00013080] c1fc 0238                 muls.w    #$0238,d0
[00013084] 2a40                      movea.l   d0,a5
[00013086] dbfc 0001 3efc            adda.l    #$00013EFC,a5
[0001308c] 4a6e 000c                 tst.w     12(a6)
[00013090] 6604                      bne.s     $00013096
[00013092] 0055 0008                 ori.w     #$0008,(a5)
[00013096] 4a6e 000e                 tst.w     14(a6)
[0001309a] 6604                      bne.s     $000130A0
[0001309c] 0055 0010                 ori.w     #$0010,(a5)
[000130a0] 2ebc 0001 35ed            move.l    #$000135ED,(a7)
[000130a6] 2f2e 0008                 move.l    8(a6),-(a7)
[000130aa] 4eb9 0001 1a6a            jsr       __strcmp
[000130b0] 588f                      addq.l    #4,a7
[000130b2] 4a40                      tst.w     d0
[000130b4] 6612                      bne.s     $000130C8
[000130b6] 0055 0003                 ori.w     #$0003,(a5)
[000130ba] 3b6e 000c 0004            move.w    12(a6),4(a5)
[000130c0] 3007                      move.w    d7,d0
[000130c2] 6000 0086                 bra       $0001314A
[000130c6] 601e                      bra.s     $000130E6
[000130c8] 2ebc 0001 35f2            move.l    #$000135F2,(a7)
[000130ce] 2f2e 0008                 move.l    8(a6),-(a7)
[000130d2] 4eb9 0001 1a6a            jsr       __strcmp
[000130d8] 588f                      addq.l    #4,a7
[000130da] 4a40                      tst.w     d0
[000130dc] 6608                      bne.s     $000130E6
[000130de] 0055 0005                 ori.w     #$0005,(a5)
[000130e2] 3007                      move.w    d7,d0
[000130e4] 6064                      bra.s     $0001314A
[000130e6] 3ebc 000f                 move.w    #$000F,(a7)
[000130ea] 2f2e 0008                 move.l    8(a6),-(a7)
[000130ee] 3f07                      move.w    d7,-(a7)
[000130f0] 4eb9 0001 1be0            jsr       ___open
[000130f6] 5c8f                      addq.l    #6,a7
[000130f8] 4a40                      tst.w     d0
[000130fa] 671e                      beq.s     $0001311A
[000130fc] 3e87                      move.w    d7,(a7)
[000130fe] 4eb9 0001 215c            jsr       __freec
[00013104] 33fc 0002 0001 3cce       move.w    #$0002,$00013CCE
[0001310c] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[00013116] 70ff                      moveq.l   #-1,d0
[00013118] 6030                      bra.s     $0001314A
[0001311a] 0055 0001                 ori.w     #$0001,(a5)
[0001311e] 3ebc 0002                 move.w    #$0002,(a7)
[00013122] 42a7                      clr.l     -(a7)
[00013124] 102d 0003                 move.b    3(a5),d0
[00013128] 4880                      ext.w     d0
[0001312a] 3f00                      move.w    d0,-(a7)
[0001312c] 4eb9 0001 2fe8            jsr       _lseek
[00013132] 5c8f                      addq.l    #6,a7
[00013134] 4257                      clr.w     (a7)
[00013136] 42a7                      clr.l     -(a7)
[00013138] 102d 0003                 move.b    3(a5),d0
[0001313c] 4880                      ext.w     d0
[0001313e] 3f00                      move.w    d0,-(a7)
[00013140] 4eb9 0001 2fe8            jsr       _lseek
[00013146] 5c8f                      addq.l    #6,a7
[00013148] 3007                      move.w    d7,d0
[0001314a] 4a9f                      tst.l     (a7)+
[0001314c] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00013150] 4e5e                      unlk      a6
[00013152] 4e75                      rts
_open:
[00013154] 4e56 fffc                 link      a6,#$FFFC
[00013158] 4257                      clr.w     (a7)
[0001315a] 3f2e 000c                 move.w    12(a6),-(a7)
[0001315e] 2f2e 0008                 move.l    8(a6),-(a7)
[00013162] 6100 fef6                 bsr       __open
[00013166] 5c8f                      addq.l    #6,a7
[00013168] 4e5e                      unlk      a6
[0001316a] 4e75                      rts
_opena:
[0001316c] 4e56 fffc                 link      a6,#$FFFC
[00013170] 4257                      clr.w     (a7)
[00013172] 3f2e 000c                 move.w    12(a6),-(a7)
[00013176] 2f2e 0008                 move.l    8(a6),-(a7)
[0001317a] 6100 fede                 bsr       __open
[0001317e] 5c8f                      addq.l    #6,a7
[00013180] 4e5e                      unlk      a6
[00013182] 4e75                      rts
_openb:
[00013184] 4e56 fffc                 link      a6,#$FFFC
[00013188] 3ebc 0001                 move.w    #$0001,(a7)
[0001318c] 3f2e 000c                 move.w    12(a6),-(a7)
[00013190] 2f2e 0008                 move.l    8(a6),-(a7)
[00013194] 6100 fec4                 bsr       __open
[00013198] 5c8f                      addq.l    #6,a7
[0001319a] 4e5e                      unlk      a6
[0001319c] 4e75                      rts
_fflush:
[0001319e] 4e56 0000                 link      a6,#$0000
[000131a2] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[000131a6] 2a6e 0008                 movea.l   8(a6),a5
[000131aa] 302d 0002                 move.w    2(a5),d0
[000131ae] c07c 000a                 and.w     #$000A,d0
[000131b2] b07c 0002                 cmp.w     #$0002,d0
[000131b6] 662c                      bne.s     $000131E4
[000131b8] 202d 0008                 move.l    8(a5),d0
[000131bc] 90ad 0004                 sub.l     4(a5),d0
[000131c0] 3c00                      move.w    d0,d6
[000131c2] 6f20                      ble.s     $000131E4
[000131c4] 3e86                      move.w    d6,(a7)
[000131c6] 2f2d 0004                 move.l    4(a5),-(a7)
[000131ca] 3f15                      move.w    (a5),-(a7)
[000131cc] 4eb9 0001 226a            jsr       _write
[000131d2] 5c8f                      addq.l    #6,a7
[000131d4] 3e00                      move.w    d0,d7
[000131d6] bc47                      cmp.w     d7,d6
[000131d8] 670a                      beq.s     $000131E4
[000131da] 006d 0010 0002            ori.w     #$0010,2(a5)
[000131e0] 70ff                      moveq.l   #-1,d0
[000131e2] 604a                      bra.s     $0001322E
[000131e4] 082d 0001 0003            btst      #1,3(a5)
[000131ea] 671e                      beq.s     $0001320A
[000131ec] 4aad 0004                 tst.l     4(a5)
[000131f0] 6716                      beq.s     $00013208
[000131f2] 082d 0003 0003            btst      #3,3(a5)
[000131f8] 6708                      beq.s     $00013202
[000131fa] 3b7c 0001 000c            move.w    #$0001,12(a5)
[00013200] 6006                      bra.s     $00013208
[00013202] 3b7c 01ff 000c            move.w    #$01FF,12(a5)
[00013208] 601c                      bra.s     $00013226
[0001320a] 3ebc 0001                 move.w    #$0001,(a7)
[0001320e] 302d 000c                 move.w    12(a5),d0
[00013212] 4440                      neg.w     d0
[00013214] 48c0                      ext.l     d0
[00013216] 2f00                      move.l    d0,-(a7)
[00013218] 3f15                      move.w    (a5),-(a7)
[0001321a] 4eb9 0001 2fe8            jsr       _lseek
[00013220] 5c8f                      addq.l    #6,a7
[00013222] 426d 000c                 clr.w     12(a5)
[00013226] 2b6d 0004 0008            move.l    4(a5),8(a5)
[0001322c] 4240                      clr.w     d0
[0001322e] 4a9f                      tst.l     (a7)+
[00013230] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00013234] 4e5e                      unlk      a6
[00013236] 4e75                      rts
[00013238] 4e56 fffc                 link      a6,#$FFFC
[0001323c] 4e5e                      unlk      a6
[0001323e] 4e75                      rts
 _close:
[00013240] 4e56 0000                 link      a6,#$0000
[00013244] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00013248] 3e2e 0008                 move.w    8(a6),d7
[0001324c] 3e87                      move.w    d7,(a7)
[0001324e] 4eb9 0001 2208            jsr       __chkc
[00013254] 2a40                      movea.l   d0,a5
[00013256] 200d                      move.l    a5,d0
[00013258] 6616                      bne.s     $00013270
[0001325a] 33fc 0009 0001 3cce       move.w    #$0009,$00013CCE
[00013262] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[0001326c] 70ff                      moveq.l   #-1,d0
[0001326e] 604e                      bra.s     $000132BE
[00013270] 4246                      clr.w     d6
[00013272] 3015                      move.w    (a5),d0
[00013274] c07c 0006                 and.w     #$0006,d0
[00013278] 6616                      bne.s     $00013290
[0001327a] 3ead 0004                 move.w    4(a5),(a7)
[0001327e] 3f3c 003e                 move.w    #$003E,-(a7)
[00013282] 4eb9 0001 1938            jsr       _trap
[00013288] 548f                      addq.l    #2,a7
[0001328a] 3c00                      move.w    d0,d6
[0001328c] 6c02                      bge.s     $00013290
[0001328e] 7cff                      moveq.l   #-1,d6
[00013290] 3e87                      move.w    d7,(a7)
[00013292] 4eb9 0001 219a            jsr       ___chini
[00013298] 3e87                      move.w    d7,(a7)
[0001329a] 4eb9 0001 215c            jsr       __freec
[000132a0] 4a46                      tst.w     d6
[000132a2] 6606                      bne.s     $000132AA
[000132a4] 3006                      move.w    d6,d0
[000132a6] 6016                      bra.s     $000132BE
[000132a8] 6014                      bra.s     $000132BE
[000132aa] 33fc 0005 0001 3cce       move.w    #$0005,$00013CCE
[000132b2] 33f9 0001 3ec4 0001 3cd0  move.w    $00013EC4,$00013CD0
[000132bc] 70ff                      moveq.l   #-1,d0
[000132be] 4a9f                      tst.l     (a7)+
[000132c0] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[000132c4] 4e5e                      unlk      a6
[000132c6] 4e75                      rts
_fclose:
[000132c8] 4e56 0000                 link      a6,#$0000
[000132cc] 48e7 0104                 movem.l   d7/a5,-(a7)
[000132d0] 2a6e 0008                 movea.l   8(a6),a5
[000132d4] 302d 0002                 move.w    2(a5),d0
[000132d8] c07c 0003                 and.w     #$0003,d0
[000132dc] 672a                      beq.s     $00013308
[000132de] 2e8d                      move.l    a5,(a7)
[000132e0] 4eb9 0001 319e            jsr       _fflush
[000132e6] 082d 0002 0003            btst      #2,3(a5)
[000132ec] 670a                      beq.s     $000132F8
[000132ee] 2ead 0004                 move.l    4(a5),(a7)
[000132f2] 4eb9 0001 26a4            jsr       _free
[000132f8] 4240                      clr.w     d0
[000132fa] 48c0                      ext.l     d0
[000132fc] 2b40 0008                 move.l    d0,8(a5)
[00013300] 2b40 0004                 move.l    d0,4(a5)
[00013304] 426d 000c                 clr.w     12(a5)
[00013308] 026d ff80 0002            andi.w    #$FF80,2(a5)
[0001330e] 3e95                      move.w    (a5),(a7)
[00013310] 4eb9 0001 3240            jsr        _close
[00013316] 4a9f                      tst.l     (a7)+
[00013318] 4cdf 2000                 movem.l   (a7)+,a5
[0001331c] 4e5e                      unlk      a6
[0001331e] 4e75                      rts
__cleanu:
[00013320] 4e56 0000                 link      a6,#$0000
[00013324] 48e7 0300                 movem.l   d6-d7,-(a7)
[00013328] 4247                      clr.w     d7
[0001332a] 6016                      bra.s     $00013342
[0001332c] 3007                      move.w    d7,d0
[0001332e] c1fc 000e                 muls.w    #$000E,d0
[00013332] d0bc 0001 3ddc            add.l     #$00013DDC,d0
[00013338] 2e80                      move.l    d0,(a7)
[0001333a] 4eb9 0001 32c8            jsr       _fclose
[00013340] 5247                      addq.w    #1,d7
[00013342] be7c 0010                 cmp.w     #$0010,d7
[00013346] 6de4                      blt.s     $0001332C
[00013348] 4a9f                      tst.l     (a7)+
[0001334a] 4cdf 0080                 movem.l   (a7)+,d7
[0001334e] 4e5e                      unlk      a6
[00013350] 4e75                      rts
_exit:
[00013352] 4e56 fffc                 link      a6,#$FFFC
[00013356] 4eb9 0001 3320            jsr       __cleanu
[0001335c] 3eae 0008                 move.w    8(a6),(a7)
[00013360] 4eb9 0001 0090            jsr       __exit
[00013366] 4e5e                      unlk      a6
[00013368] 4e75                      rts
_strrchr:
[0001336a] 4e56 0000                 link      a6,#$0000
[0001336e] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00013372] 2a6e 0008                 movea.l   8(a6),a5
[00013376] 1e2e 000d                 move.b    13(a6),d7
[0001337a] 284d                      movea.l   a5,a4
[0001337c] 6002                      bra.s     $00013380
[0001337e] 528c                      addq.l    #1,a4
[00013380] 4a14                      tst.b     (a4)
[00013382] 66fa                      bne.s     $0001337E
[00013384] 600a                      bra.s     $00013390
[00013386] b9cd                      cmpa.l    a5,a4
[00013388] 6604                      bne.s     $0001338E
[0001338a] 4280                      clr.l     d0
[0001338c] 6008                      bra.s     $00013396
[0001338e] 538c                      subq.l    #1,a4
[00013390] be14                      cmp.b     (a4),d7
[00013392] 66f2                      bne.s     $00013386
[00013394] 200c                      move.l    a4,d0
[00013396] 4a9f                      tst.l     (a7)+
[00013398] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[0001339c] 4e5e                      unlk      a6
[0001339e] 4e75                      rts
_rindex:
[000133a0] 4e56 0000                 link      a6,#$0000
[000133a4] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000133a8] 2a6e 0008                 movea.l   8(a6),a5
[000133ac] 1e2e 000d                 move.b    13(a6),d7
[000133b0] 1007                      move.b    d7,d0
[000133b2] 4880                      ext.w     d0
[000133b4] 3e80                      move.w    d0,(a7)
[000133b6] 2f0d                      move.l    a5,-(a7)
[000133b8] 61b0                      bsr.s     _strrchr
[000133ba] 588f                      addq.l    #4,a7
[000133bc] 4a9f                      tst.l     (a7)+
[000133be] 4cdf 2080                 movem.l   (a7)+,d7/a5
[000133c2] 4e5e                      unlk      a6
[000133c4] 4e75                      rts
_qsort:
[000133c6] 4e56 0000                 link      a6,#$0000
[000133ca] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[000133ce] 0c6e 0001 000c            cmpi.w    #$0001,12(a6)
[000133d4] 6f00 012e                 ble       $00013504
[000133d8] 4247                      clr.w     d7
[000133da] 3c2e 000c                 move.w    12(a6),d6
[000133de] 5346                      subq.w    #1,d6
[000133e0] 3006                      move.w    d6,d0
[000133e2] c1ee 000e                 muls.w    14(a6),d0
[000133e6] 48c0                      ext.l     d0
[000133e8] 2a40                      movea.l   d0,a5
[000133ea] dbee 0008                 adda.l    8(a6),a5
[000133ee] 6002                      bra.s     $000133F2
[000133f0] 5247                      addq.w    #1,d7
[000133f2] be46                      cmp.w     d6,d7
[000133f4] 6c1c                      bge.s     $00013412
[000133f6] 2f0d                      move.l    a5,-(a7)
[000133f8] 3007                      move.w    d7,d0
[000133fa] c1ee 000e                 muls.w    14(a6),d0
[000133fe] 48c0                      ext.l     d0
[00013400] d0ae 0008                 add.l     8(a6),d0
[00013404] 2f00                      move.l    d0,-(a7)
[00013406] 206e 0010                 movea.l   16(a6),a0
[0001340a] 4e90                      jsr       (a0)
[0001340c] 508f                      addq.l    #8,a7
[0001340e] 4a40                      tst.w     d0
[00013410] 6fde                      ble.s     $000133F0
[00013412] 6002                      bra.s     $00013416
[00013414] 5346                      subq.w    #1,d6
[00013416] bc47                      cmp.w     d7,d6
[00013418] 6f1c                      ble.s     $00013436
[0001341a] 2f0d                      move.l    a5,-(a7)
[0001341c] 3006                      move.w    d6,d0
[0001341e] c1ee 000e                 muls.w    14(a6),d0
[00013422] 48c0                      ext.l     d0
[00013424] d0ae 0008                 add.l     8(a6),d0
[00013428] 2f00                      move.l    d0,-(a7)
[0001342a] 206e 0010                 movea.l   16(a6),a0
[0001342e] 4e90                      jsr       (a0)
[00013430] 508f                      addq.l    #8,a7
[00013432] 4a40                      tst.w     d0
[00013434] 6cde                      bge.s     $00013414
[00013436] be46                      cmp.w     d6,d7
[00013438] 6c26                      bge.s     $00013460
[0001343a] 3eae 000e                 move.w    14(a6),(a7)
[0001343e] 3006                      move.w    d6,d0
[00013440] c1ee 000e                 muls.w    14(a6),d0
[00013444] 48c0                      ext.l     d0
[00013446] d0ae 0008                 add.l     8(a6),d0
[0001344a] 2f00                      move.l    d0,-(a7)
[0001344c] 3007                      move.w    d7,d0
[0001344e] c1ee 000e                 muls.w    14(a6),d0
[00013452] 48c0                      ext.l     d0
[00013454] d0ae 0008                 add.l     8(a6),d0
[00013458] 2f00                      move.l    d0,-(a7)
[0001345a] 6100 00b4                 bsr       $00013510
[0001345e] 508f                      addq.l    #8,a7
[00013460] be46                      cmp.w     d6,d7
[00013462] 6d8a                      blt.s     $000133EE
[00013464] 3eae 000e                 move.w    14(a6),(a7)
[00013468] 2f0d                      move.l    a5,-(a7)
[0001346a] 3007                      move.w    d7,d0
[0001346c] c1ee 000e                 muls.w    14(a6),d0
[00013470] 48c0                      ext.l     d0
[00013472] d0ae 0008                 add.l     8(a6),d0
[00013476] 2f00                      move.l    d0,-(a7)
[00013478] 6100 0096                 bsr       $00013510
[0001347c] 508f                      addq.l    #8,a7
[0001347e] 302e 000c                 move.w    12(a6),d0
[00013482] 5340                      subq.w    #1,d0
[00013484] 9047                      sub.w     d7,d0
[00013486] be40                      cmp.w     d0,d7
[00013488] 6c3e                      bge.s     $000134C8
[0001348a] 2eae 0010                 move.l    16(a6),(a7)
[0001348e] 3f2e 000e                 move.w    14(a6),-(a7)
[00013492] 3f07                      move.w    d7,-(a7)
[00013494] 2f2e 0008                 move.l    8(a6),-(a7)
[00013498] 6100 ff2c                 bsr       _qsort
[0001349c] 508f                      addq.l    #8,a7
[0001349e] 2eae 0010                 move.l    16(a6),(a7)
[000134a2] 3f2e 000e                 move.w    14(a6),-(a7)
[000134a6] 3f2e 000c                 move.w    12(a6),-(a7)
[000134aa] 5357                      subq.w    #1,(a7)
[000134ac] 3007                      move.w    d7,d0
[000134ae] 9157                      sub.w     d0,(a7)
[000134b0] 3007                      move.w    d7,d0
[000134b2] 5240                      addq.w    #1,d0
[000134b4] c1ee 000e                 muls.w    14(a6),d0
[000134b8] 48c0                      ext.l     d0
[000134ba] d0ae 0008                 add.l     8(a6),d0
[000134be] 2f00                      move.l    d0,-(a7)
[000134c0] 6100 ff04                 bsr       _qsort
[000134c4] 508f                      addq.l    #8,a7
[000134c6] 603c                      bra.s     $00013504
[000134c8] 2eae 0010                 move.l    16(a6),(a7)
[000134cc] 3f2e 000e                 move.w    14(a6),-(a7)
[000134d0] 3f2e 000c                 move.w    12(a6),-(a7)
[000134d4] 5357                      subq.w    #1,(a7)
[000134d6] 3007                      move.w    d7,d0
[000134d8] 9157                      sub.w     d0,(a7)
[000134da] 3007                      move.w    d7,d0
[000134dc] 5240                      addq.w    #1,d0
[000134de] c1ee 000e                 muls.w    14(a6),d0
[000134e2] 48c0                      ext.l     d0
[000134e4] d0ae 0008                 add.l     8(a6),d0
[000134e8] 2f00                      move.l    d0,-(a7)
[000134ea] 6100 feda                 bsr       _qsort
[000134ee] 508f                      addq.l    #8,a7
[000134f0] 2eae 0010                 move.l    16(a6),(a7)
[000134f4] 3f2e 000e                 move.w    14(a6),-(a7)
[000134f8] 3f07                      move.w    d7,-(a7)
[000134fa] 2f2e 0008                 move.l    8(a6),-(a7)
[000134fe] 6100 fec6                 bsr       _qsort
[00013502] 508f                      addq.l    #8,a7
[00013504] 4240                      clr.w     d0
[00013506] 4a9f                      tst.l     (a7)+
[00013508] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[0001350c] 4e5e                      unlk      a6
[0001350e] 4e75                      rts

[00013510] 4e56 0000                 link      a6,#$0000
[00013514] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00013518] 2a6e 0008                 movea.l   8(a6),a5
[0001351c] 286e 000c                 movea.l   12(a6),a4
[00013520] 3e2e 0010                 move.w    16(a6),d7
[00013524] bbcc                      cmpa.l    a4,a5
[00013526] 6714                      beq.s     $0001353C
[00013528] 600a                      bra.s     $00013534
[0001352a] 1c15                      move.b    (a5),d6
[0001352c] 1a94                      move.b    (a4),(a5)
[0001352e] 1886                      move.b    d6,(a4)
[00013530] 528d                      addq.l    #1,a5
[00013532] 528c                      addq.l    #1,a4
[00013534] 3007                      move.w    d7,d0
[00013536] 5347                      subq.w    #1,d7
[00013538] 4a40                      tst.w     d0
[0001353a] 6eee                      bgt.s     $0001352A
[0001353c] 4a9f                      tst.l     (a7)+
[0001353e] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00013542] 4e5e                      unlk      a6
[00013544] 4e75                      rts
_puts:
[00013546] 4e56 0000                 link      a6,#$0000
[0001354a] 48e7 0104                 movem.l   d7/a5,-(a7)
[0001354e] 2a6e 0008                 movea.l   8(a6),a5
[00013552] 601e                      bra.s     $00013572
[00013554] 2ebc 0001 3dea            move.l    #$00013DEA,(a7)
[0001355a] 101d                      move.b    (a5)+,d0
[0001355c] 4880                      ext.w     d0
[0001355e] 3f00                      move.w    d0,-(a7)
[00013560] 4eb9 0001 29ec            jsr       _fputc
[00013566] 548f                      addq.l    #2,a7
[00013568] b07c ffff                 cmp.w     #$FFFF,d0
[0001356c] 6604                      bne.s     $00013572
[0001356e] 70ff                      moveq.l   #-1,d0
[00013570] 6016                      bra.s     $00013588
[00013572] 4a15                      tst.b     (a5)
[00013574] 66de                      bne.s     $00013554
[00013576] 2ebc 0001 3dea            move.l    #$00013DEA,(a7)
[0001357c] 3f3c 000a                 move.w    #$000A,-(a7)
[00013580] 4eb9 0001 29ec            jsr       _fputc
[00013586] 548f                      addq.l    #2,a7
[00013588] 4a9f                      tst.l     (a7)+
[0001358a] 4cdf 2000                 movem.l   (a7)+,a5
[0001358e] 4e5e                      unlk      a6
[00013590] 4e75                      rts
_printf:
[00013592] 4e56 fffc                 link      a6,#$FFFC
[00013596] 2e8e                      move.l    a6,(a7)
[00013598] 0697 0000 000c            addi.l    #$0000000C,(a7)
[0001359e] 2f2e 0008                 move.l    8(a6),-(a7)
[000135a2] 2f3c 0001 3dea            move.l    #$00013DEA,-(a7)
[000135a8] 4eb9 0001 2b5c            jsr       __doprt
[000135ae] 508f                      addq.l    #8,a7
[000135b0] 4e5e                      unlk      a6
[000135b2] 4e75                      rts
_fprintf:
[000135b4] 4e56 fffc                 link      a6,#$FFFC
[000135b8] 2e8e                      move.l    a6,(a7)
[000135ba] 0697 0000 0010            addi.l    #$00000010,(a7)
[000135c0] 2f2e 000c                 move.l    12(a6),-(a7)
[000135c4] 2f2e 0008                 move.l    8(a6),-(a7)
[000135c8] 4eb9 0001 2b5c            jsr       __doprt
[000135ce] 508f                      addq.l    #8,a7
[000135d0] 4e5e                      unlk      a6
[000135d2] 4e75                      rts

data:
[000135d4] 5374 6163 6b20 4f76 6572  subq.w    #1,([$6B20,a4],$4F766572) ; 68020+ only
[000135de] 666c                      bne.s     $0001364C
[000135e0] 6f77                      ble.s     $00013659
[000135e2] 0d0a 0072                 movep.w   114(a2),d6
[000135e6] 756e 7469                 mvs.w     29801(a6),d2 ; ColdFire only
[000135ea] 6d65                      blt.s     $00013651
[000135ec] 0043 4f4e                 ori.w     #$4F4E,d3
[000135f0] 3a00                      move.w    d0,d5
[000135f2] 4c53 543a                 divu.l    (a3),d2:d5 ; 68020+ only
[000135f6] 001a 4361                 ori.b     #$61,(a2)+
[000135fa] 6e6e                      bgt.s     $0001366A
[000135fc] 6f74                      ble.s     $00013672
[000135fe] 2069 6e69                 movea.l   28265(a1),a0
[00013602] 7469                      moveq.l   #105,d2
[00013604] 616c                      bsr.s     $00013672
[00013606] 697a                      bvs.s     $00013682
[00013608] 6520                      bcs.s     $0001362A
[0001360a] 7374 6163 6b0d 0a00 0001  mvs.w     ([$6B0D,a4],$0A000001),d1 ; ColdFire only
[00013614] 0002 0101                 ori.b     #$01,d2
[00013618] 0201 0100                 andi.b    #$00,d1
[0001361c] 0101                      btst      d0,d1
[0001361e] 0201 0101                 andi.b    #$01,d1
[00013622] 0101                      btst      d0,d1
[00013624] 0000 0000                 ori.b     #$00,d0
[00013628] 0000 0000                 ori.b     #$00,d0
[0001362c] 0000 0100                 ori.b     #$00,d0
[00013630] 0001 0003                 ori.b     #$03,d1
[00013634] 0500                      btst      d2,d0
[00013636] 0505                      btst      d2,d5
[00013638] 0000 0101                 ori.b     #$01,d0
[0001363c] 0201 0010                 andi.b    #$10,d1
[00013640] 0701                      btst      d3,d1
[00013642] 0201 0000                 andi.b    #$00,d1
[00013646] 0000 0000                 ori.b     #$00,d0
[0001364a] 0000 0000                 ori.b     #$00,d0
[0001364e] 0101                      btst      d0,d1
[00013650] 0102                      btst      d0,d2
[00013652] 0101                      btst      d0,d1
[00013654] 0201 0102                 andi.b    #$02,d1
[00013658] 0101                      btst      d0,d1
[0001365a] 0101                      btst      d0,d1
[0001365c] 0201 0101                 andi.b    #$01,d1
[00013660] 0000 0000                 ori.b     #$00,d0
[00013664] 0000 0000                 ori.b     #$00,d0
[00013668] 0000 0000                 ori.b     #$00,d0
[0001366c] 0201 0101                 andi.b    #$01,d1
[00013670] 0101                      btst      d0,d1
[00013672] 0601 0104                 addi.b    #$04,d1
[00013676] 0101                      btst      d0,d1
[00013678] 0103                      btst      d0,d3
[0001367a] 0102                      btst      d0,d2
[0001367c] 0101                      btst      d0,d1
[0001367e] 0402 0108                 subi.b    #$08,d2
[00013682] 0101                      btst      d0,d1
[00013684] 0000 0000                 ori.b     #$00,d0
[00013688] 0000 0101                 ori.b     #$01,d0
[0001368c] 0109 0101                 movep.w   257(a1),d0
[00013690] 0101                      btst      d0,d1
[00013692] 0101                      btst      d0,d1
[00013694] 0100                      btst      d0,d0
[00013696] 0005 0100                 ori.b     #$00,d5
[0001369a] 0000 0000                 ori.b     #$00,d0
[0001369e] 0000 0000                 ori.b     #$00,d0
[000136a2] 0000 0000                 ori.b     #$00,d0
[000136a6] 0000 0000                 ori.b     #$00,d0
[000136aa] 0000 0000                 ori.b     #$00,d0
[000136ae] 0000 0000                 ori.b     #$00,d0
[000136b2] 0000 0000                 ori.b     #$00,d0
[000136b6] 0000 0000                 ori.b     #$00,d0
[000136ba] 0000 0000                 ori.b     #$00,d0
[000136be] 0000 0000                 ori.b     #$00,d0
[000136c2] 0000 0000                 ori.b     #$00,d0
[000136c6] 0403 0008                 subi.b    #$08,d3
[000136ca] 0300                      btst      d1,d0
[000136cc] 0601 0008                 addi.b    #$08,d1
[000136d0] 0100                      btst      d0,d0
[000136d2] 0801 0004                 btst      #4,d1
[000136d6] 0101                      btst      d0,d1
[000136d8] 0301                      btst      d1,d1
[000136da] 0100                      btst      d0,d0
[000136dc] 0500                      btst      d2,d0
[000136de] 0101                      btst      d0,d1
[000136e0] 0100                      btst      d0,d0
[000136e2] 0500                      btst      d2,d0
[000136e4] 0001 0100                 ori.b     #$00,d1
[000136e8] 0101                      btst      d0,d1
[000136ea] 0000 0000                 ori.b     #$00,d0
[000136ee] 0000 0000                 ori.b     #$00,d0
[000136f2] 0000 0000                 ori.b     #$00,d0
[000136f6] 0000 0000                 ori.b     #$00,d0
[000136fa] 0000 0000                 ori.b     #$00,d0
[000136fe] 0000 0000                 ori.b     #$00,d0
[00013702] 0002 0200                 ori.b     #$00,d2
[00013706] 0000 0000                 ori.b     #$00,d0
[0001370a] 0000 0000                 ori.b     #$00,d0
[0001370e] 0000 0000                 ori.b     #$00,d0
[00013712] 0000 0000                 ori.b     #$00,d0
[00013716] 0000 0000                 ori.b     #$00,d0
[0001371a] 0000 0000                 ori.b     #$00,d0
[0001371e] 0000 0501                 ori.b     #$01,d0
[00013722] 0005 0100                 ori.b     #$00,d5
[00013726] 0101                      btst      d0,d1
[00013728] 0001 0100                 ori.b     #$00,d1
[0001372c] 0205 0006                 andi.b    #$06,d5
[00013730] 0100                      btst      d0,d0
[00013732] 0201 0001                 andi.b    #$01,d1
[00013736] 0100                      btst      d0,d0
[00013738] 0605 0000                 addi.b    #$00,d5
[0001373c] 0000 0001                 ori.b     #$01,d0
[00013740] 0100                      btst      d0,d0
[00013742] 0100                      btst      d0,d0
[00013744] 0201 0002                 andi.b    #$02,d1
[00013748] 0101                      btst      d0,d1
[0001374a] 0101                      btst      d0,d1
[0001374c] 0100                      btst      d0,d0
[0001374e] 0000 0000                 ori.b     #$00,d0
[00013752] 0000 0000                 ori.b     #$00,d0
[00013756] 0000 0000                 ori.b     #$00,d0
[0001375a] 0000 0001                 ori.b     #$01,d0
[0001375e] 0203 0102                 andi.b    #$02,d3
[00013762] 0101                      btst      d0,d1
[00013764] 0101                      btst      d0,d1
[00013766] 0101                      btst      d0,d1
[00013768] 0001 0100                 ori.b     #$00,d1
[0001376c] 0102                      btst      d0,d2
[0001376e] 0000 0000                 ori.b     #$00,d0
[00013772] 0000 0000                 ori.b     #$00,d0
[00013776] 4f75 7420                 lea.l     32(a5,d7.w*4),a7 ; 68020+ only
[0001377a] 6f66                      ble.s     $000137E2
[0001377c] 206d 656d                 movea.l   25965(a5),a0
[00013780] 6f72                      ble.s     $000137F4
[00013782] 7900                      mvs.b     d0,d4 ; ColdFire only
[00013784] 4f75 7420                 lea.l     32(a5,d7.w*4),a7 ; 68020+ only
[00013788] 6f66                      ble.s     $000137F0
[0001378a] 206d 656d                 movea.l   25965(a5),a0
[0001378e] 6f72                      ble.s     $00013802
[00013790] 7900                      mvs.b     d0,d4 ; ColdFire only
[00013792] 4f75 7420                 lea.l     32(a5,d7.w*4),a7 ; 68020+ only
[00013796] 6f66                      ble.s     $000137FE
[00013798] 206d 656d                 movea.l   25965(a5),a0
[0001379c] 6f72                      ble.s     $00013810
[0001379e] 790a                      mvs.b     a2,d4 ; ColdFire only
[000137a0] 0045 7272                 ori.w     #$7272,d5
[000137a4] 6f72                      ble.s     $00013818
[000137a6] 2072 6561 6469            movea.l   ([$6469,a2,zd6.w*4]),a0 ; 68020+ only
[000137ac] 6e67                      bgt.s     $00013815
[000137ae] 2061                      movea.l   -(a1),a0
[000137b0] 7263                      moveq.l   #99,d1
[000137b2] 6869                      bvc.s     $0001381D
[000137b4] 7665                      moveq.l   #101,d3
[000137b6] 6420                      bcc.s     $000137D8
[000137b8] 6d6f                      blt.s     $00013829
[000137ba] 6475                      bcc.s     $00013831
[000137bc] 6c65                      bge.s     $00013823
[000137be] 004f 7574                 ori.w     #$7574,a7
[000137c2] 206f 6620                 movea.l   26144(a7),a0
[000137c6] 6d65                      blt.s     $0001382D
[000137c8] 6d6f                      blt.s     $00013839
[000137ca] 7279                      moveq.l   #121,d1
[000137cc] 0a00 4f75                 eori.b    #$75,d0
[000137d0] 7420                      moveq.l   #32,d2
[000137d2] 6f66                      ble.s     $0001383A
[000137d4] 206d 656d                 movea.l   25965(a5),a0
[000137d8] 6f72                      ble.s     $0001384C
[000137da] 790a                      mvs.b     a2,d4 ; ColdFire only
[000137dc] 0057 6172                 ori.w     #$6172,(a7)
[000137e0] 6e69                      bgt.s     $0001384B
[000137e2] 6e67                      bgt.s     $0001384B
[000137e4] 3a20                      move.w    -(a0),d5
[000137e6] 2573 2065 7870            move.l    101(a3,d2.w),30832(a2)
[000137ec] 6f72                      ble.s     $00013860
[000137ee] 7465                      moveq.l   #101,d2
[000137f0] 6420                      bcc.s     $00013812
[000137f2] 6279                      bhi.s     $0001386D
[000137f4] 2025                      move.l    -(a5),d0
[000137f6] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[000137f8] 616e                      bsr.s     $00013868
[000137fa] 6420                      bcc.s     $0001381C
[000137fc] 2573 0020 616e            move.l    32(a3,d0.w),24942(a2)
[00013802] 6420                      bcc.s     $00013824
[00013804] 2573 002e 0a09            move.l    46(a3,d0.w),2569(a2)
[0001380a] 5661                      addq.w    #3,-(a1)
[0001380c] 6c75                      bge.s     $00013883
[0001380e] 6520                      bcs.s     $00013830
[00013810] 6672                      bne.s     $00013884
[00013812] 6f6d                      ble.s     $00013881
[00013814] 2025                      move.l    -(a5),d0
[00013816] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00013818] 7573 6564 3b20            mvs.w     ([$3B20,a3],zd6.w*4),d2 ; ColdFire only; reserved OD=0
[0001381e] 6f74                      ble.s     $00013894
[00013820] 6865                      bvc.s     $00013887
[00013822] 7273                      moveq.l   #115,d1
[00013824] 2064                      movea.l   -(a4),a0
[00013826] 6973                      bvs.s     $0001389B
[00013828] 6361                      bls.s     $0001388B
[0001382a] 7264                      moveq.l   #100,d1
[0001382c] 6564                      bcs.s     $00013892
[0001382e] 2e0a                      move.l    a2,d7
[00013830] 004f 7574                 ori.w     #$7574,a7
[00013834] 206f 6620                 movea.l   26144(a7),a0
[00013838] 6d65                      blt.s     $0001389F
[0001383a] 6d6f                      blt.s     $000138AB
[0001383c] 7279                      moveq.l   #121,d1
[0001383e] 0020 2020                 ori.b     #$20,-(a0)
[00013842] 2000                      move.l    d0,d0
[00013844] 4669 6c65                 not.w     27749(a1)
[00013848] 2025                      move.l    -(a5),d0
[0001384a] 733b 206f                 mvs.b     $000138BB(pc,d2.w),d1 ; ColdFire only
[0001384e] 6666                      bne.s     $000138B6
[00013850] 7365                      mvs.w     -(a5),d1 ; ColdFire only
[00013852] 7420                      moveq.l   #32,d2
[00013854] 3078 256c                 movea.w   ($0000256C).w,a0
[00013858] 7820                      moveq.l   #32,d4
[0001385a] 696e                      bvs.s     $000138CA
[0001385c] 2061                      movea.l   -(a1),a0
[0001385e] 7263                      moveq.l   #99,d1
[00013860] 6869                      bvc.s     $000138CB
[00013862] 7665                      moveq.l   #101,d3
[00013864] 0a00 0945                 eori.b    #$45,d0
[00013868] 5850                      addq.w    #4,(a0)
[0001386a] 4f52                      lea.l     (a2),a7
[0001386c] 5453                      addq.w    #2,(a3)
[0001386e] 3a20                      move.w    -(a0),d5
[00013870] 000a 0909                 ori.b     #$09,a2
[00013874] 2000                      move.l    d0,d0
[00013876] 2573 2000 0a00            move.l    0(a3,d2.w),2560(a2)
[0001387c] 0949 4d50                 movep.l   19792(a1),d4
[00013880] 4f52                      lea.l     (a2),a7
[00013882] 5453                      addq.w    #2,(a3)
[00013884] 3a20                      move.w    -(a0),d5
[00013886] 000a 0909                 ori.b     #$09,a2
[0001388a] 2000                      move.l    d0,d0
[0001388c] 2573 2000 0a00            move.l    0(a3,d2.w),2560(a2)
[00013892] 4152                      lea.l     (a2),a0
[00013894] 4348                      lea.l     a0,a1
[00013896] 4956                      lea.l     (a6),a4
[00013898] 4520                      chk.l     -(a0),d2 ; 68020+ only
[0001389a] 4649                      not.w     a1
[0001389c] 4c45 2043                 divul.l   d5,d3:d2 ; remu.l for ColdFire
[000138a0] 4f4e                      lea.l     a6,a7
[000138a2] 5441                      addq.w    #2,d1
[000138a4] 494e                      lea.l     a6,a4
[000138a6] 5320                      subq.b    #1,-(a0)
[000138a8] 2564 2046                 move.l    -(a4),8262(a2)
[000138ac] 494c                      lea.l     a4,a4
[000138ae] 4553                      lea.l     (a3),a2
[000138b0] 2045                      movea.l   d5,a0
[000138b2] 5850                      addq.w    #4,(a0)
[000138b4] 4f52                      lea.l     (a2),a7
[000138b6] 5449                      addq.w    #2,a1
[000138b8] 4e47                      trap      #7
[000138ba] 2025                      move.l    -(a5),d0
[000138bc] 6420                      bcc.s     $000138DE
[000138be] 5359                      subq.w    #1,(a1)+
[000138c0] 4d42                      lea.l     d2,a6
[000138c2] 4f4c                      lea.l     a4,a7
[000138c4] 530a                      subq.b    #1,a2
[000138c6] 0a00 0a45                 eori.b    #$45,d0
[000138ca] 5850                      addq.w    #4,(a0)
[000138cc] 4f52                      lea.l     (a2),a7
[000138ce] 5453                      addq.w    #2,(a3)
[000138d0] 0a0a 0025                 eori.b    #$25,a2
[000138d4] 7325                      mvs.b     -(a5),d1 ; ColdFire only
[000138d6] 7309                      mvs.b     a1,d1 ; ColdFire only
[000138d8] 2564 0a00                 move.l    -(a4),2560(a2)
[000138dc] 0900                      btst      d4,d0
[000138de] 000a 4649                 ori.b     #$49,a2
[000138e2] 4c45 3a09                 divsl.l   d5,d1:d3 ; rems.l for ColdFire
[000138e6] 4445                      neg.w     d5
[000138e8] 5045                      addq.w    #8,d5
[000138ea] 4e44                      trap      #4
[000138ec] 5320                      subq.b    #1,-(a0)
[000138ee] 4f4e                      lea.l     a6,a7
[000138f0] 3a0a                      move.w    a2,d5
[000138f2] 0a00 2564                 eori.b    #$64,d0
[000138f6] 3a09                      move.w    a1,d5
[000138f8] 0025 6420                 ori.b     #$20,-(a5)
[000138fc] 000a 0025                 ori.b     #$25,a2
[00013900] 3464                      movea.w   -(a4),a2
[00013902] 3a20                      move.w    -(a0),d5
[00013904] 6f66                      ble.s     $0001396C
[00013906] 6673                      bne.s     $0001397B
[00013908] 6574                      bcs.s     $0001397E
[0001390a] 2030 7825                 move.l    37(a0,d7.l),d0
[0001390e] 3035 6c78                 move.w    120(a5,d6.l*4),d0 ; 68020+ only
[00013912] 2025                      move.l    -(a5),d0
[00013914] 730a                      mvs.b     a2,d1 ; ColdFire only
[00013916] 0025 7320                 ori.b     #$20,-(a5)
[0001391a] 2564 0a00                 move.l    -(a4),2560(a2)
[0001391e] 5573 6167 653a 2064 6f69  subq.w    #2,([$653A,a3],$20646F69) ; 68020+ only; reserved OD=3
[00013928] 7420                      moveq.l   #32,d2
[0001392a] 5b2d 695d                 subq.b    #5,26973(a5)
[0001392e] 205b                      movea.l   (a3)+,a0
[00013930] 2d64 5d20                 move.l    -(a4),23840(a6)
[00013934] 5b2d 7a5d                 subq.b    #5,31325(a5)
[00013938] 2066                      movea.l   -(a6),a0
[0001393a] 6e61                      bgt.s     $0001399D
[0001393c] 6d65                      blt.s     $000139A3
[0001393e] 0a00 2d64                 eori.b    #$64,d0
[00013942] 3a20                      move.w    -(a0),d5
[00013944] 6469                      bcc.s     $000139AF
[00013946] 7370 6c61                 mvs.w     97(a0,d6.l*4),d1 ; ColdFire only
[0001394a] 7920                      mvs.b     -(a0),d4 ; ColdFire only
[0001394c] 7468                      moveq.l   #104,d2
[0001394e] 6520                      bcs.s     $00013970
[00013950] 6172                      bsr.s     $000139C4
[00013952] 6368                      bls.s     $000139BC
[00013954] 6976                      bvs.s     $000139CC
[00013956] 6520                      bcs.s     $00013978
[00013958] 696e                      bvs.s     $000139C8
[0001395a] 6465                      bcc.s     $000139C1
[0001395c] 7820                      moveq.l   #32,d4
[0001395e] 7665                      moveq.l   #101,d3
[00013960] 7262                      moveq.l   #98,d1
[00013962] 6174                      bsr.s     $000139D8
[00013964] 696d                      bvs.s     $000139D3
[00013966] 2028 6475                 move.l    25717(a0),d0
[0001396a] 6d70                      blt.s     $000139DC
[0001396c] 2069 7429                 movea.l   29737(a1),a0
[00013970] 002d 693a 2064            ori.b     #$3A,8292(a5)
[00013976] 6973                      bvs.s     $000139EB
[00013978] 706c                      moveq.l   #108,d0
[0001397a] 6179                      bsr.s     $000139F5
[0001397c] 2061                      movea.l   -(a1),a0
[0001397e] 6e20                      bgt.s     $000139A0
[00013980] 696d                      bvs.s     $000139EF
[00013982] 706f                      moveq.l   #111,d0
[00013984] 7274                      moveq.l   #116,d1
[00013986] 732f 6578                 mvs.b     25976(a7),d1 ; ColdFire only
[0001398a] 706f                      moveq.l   #111,d0
[0001398c] 7274                      moveq.l   #116,d1
[0001398e] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00013990] 696e                      bvs.s     $00013A00
[00013992] 6465                      bcc.s     $000139F9
[00013994] 7820                      moveq.l   #32,d4
[00013996] 6f66                      ble.s     $000139FE
[00013998] 2074 6865                 movea.l   101(a4,d6.l),a0
[0001399c] 2061                      movea.l   -(a1),a0
[0001399e] 7263                      moveq.l   #99,d1
[000139a0] 6869                      bvc.s     $00013A0B
[000139a2] 7665                      moveq.l   #101,d3
[000139a4] 002d 773a 2064            ori.b     #$3A,8292(a5)
[000139aa] 6973                      bvs.s     $00013A1F
[000139ac] 706c                      moveq.l   #108,d0
[000139ae] 6179                      bsr.s     $00013A29
[000139b0] 2077 6172 6e69 6e67 7320  movea.l   ([$6E696E67,a7],$7320),a0 ; 68020+ only
[000139ba] 6162                      bsr.s     $00013A1E
[000139bc] 6f75                      ble.s     $00013A33
[000139be] 7420                      moveq.l   #32,d2
[000139c0] 6d75                      blt.s     $00013A37
[000139c2] 6c74                      bge.s     $00013A38
[000139c4] 6970                      bvs.s     $00013A36
[000139c6] 6c79                      bge.s     $00013A41
[000139c8] 2d64 6566                 move.l    -(a4),25958(a6)
[000139cc] 696e                      bvs.s     $00013A3C
[000139ce] 6564                      bcs.s     $00013A34
[000139d0] 2065                      movea.l   -(a5),a0
[000139d2] 7874                      moveq.l   #116,d4
[000139d4] 6572                      bcs.s     $00013A48
[000139d6] 6e61                      bgt.s     $00013A39
[000139d8] 6c73                      bge.s     $00013A4D
[000139da] 002d 7a3a 2064            ori.b     #$3A,8292(a5)
[000139e0] 6973                      bvs.s     $00013A55
[000139e2] 706c                      moveq.l   #108,d0
[000139e4] 6179                      bsr.s     $00013A5F
[000139e6] 2028 6c6f                 move.l    27759(a0),d0
[000139ea] 7473                      moveq.l   #115,d2
[000139ec] 206f 6629                 movea.l   26153(a7),a0
[000139f0] 2064                      movea.l   -(a4),a0
[000139f2] 6562                      bcs.s     $00013A56
[000139f4] 7567                      mvs.w     -(a7),d2 ; ColdFire only
[000139f6] 6769                      beq.s     $00013A61
[000139f8] 6e67                      bgt.s     $00013A61
[000139fa] 2069 6e66                 movea.l   28262(a1),a0
[000139fe] 6f72                      ble.s     $00013A72
[00013a00] 6d61                      blt.s     $00013A63
[00013a02] 7469                      moveq.l   #105,d2
[00013a04] 6f6e                      ble.s     $00013A74
[00013a06] 0050 616e                 ori.w     #$616E,(a0)
[00013a0a] 6963                      bvs.s     $00013A6F
[00013a0c] 3a20                      move.w    -(a0),d5
[00013a0e] 2573 0a00 4361            move.l    0(a3,d0.l*2),17249(a2) ; 68020+ only
[00013a14] 6e27                      bgt.s     $00013A3D
[00013a16] 7420                      moveq.l   #32,d2
[00013a18] 6f70                      ble.s     $00013A8A
[00013a1a] 656e                      bcs.s     $00013A8A
[00013a1c] 2069 6e70                 movea.l   28272(a1),a0
[00013a20] 7574 2066                 mvs.w     102(a4,d2.w),d2 ; ColdFire only
[00013a24] 696c                      bvs.s     $00013A92
[00013a26] 6500 4572                 bcs       $00017F9A
[00013a2a] 726f                      moveq.l   #111,d1
[00013a2c] 7220                      moveq.l   #32,d1
[00013a2e] 7265                      moveq.l   #101,d1
[00013a30] 6164                      bsr.s     $00013A96
[00013a32] 696e                      bvs.s     $00013AA2
[00013a34] 6720                      beq.s     $00013A56
[00013a36] 696e                      bvs.s     $00013AA6
[00013a38] 7075                      moveq.l   #117,d0
[00013a3a] 7420                      moveq.l   #32,d2
[00013a3c] 6669                      bne.s     $00013AA7
[00013a3e] 6c65                      bge.s     $00013AA5
[00013a40] 0046 696c                 ori.w     #$696C,d6
[00013a44] 6520                      bcs.s     $00013A66
[00013a46] 2573 2069 7320            move.l    105(a3,d2.w),29472(a2)
[00013a4c] 6e6f                      bgt.s     $00013ABD
[00013a4e] 7420                      moveq.l   #32,d2
[00013a50] 616e                      bsr.s     $00013AC0
[00013a52] 2061                      movea.l   -(a1),a0
[00013a54] 7263                      moveq.l   #99,d1
[00013a56] 6869                      bvc.s     $00013AC1
[00013a58] 7665                      moveq.l   #101,d3
[00013a5a] 0043 6f75                 ori.w     #$6F75,d3
[00013a5e] 6c64                      bge.s     $00013AC4
[00013a60] 6e27                      bgt.s     $00013A89
[00013a62] 7420                      moveq.l   #32,d2
[00013a64] 6275                      bhi.s     $00013ADB
[00013a66] 696c                      bvs.s     $00013AD4
[00013a68] 6420                      bcc.s     $00013A8A
[00013a6a] 616e                      bsr.s     $00013ADA
[00013a6c] 2069 6e64                 movea.l   28260(a1),a0
[00013a70] 6578                      bcs.s     $00013AEA
[00013a72] 2066                      movea.l   -(a6),a0
[00013a74] 696c                      bvs.s     $00013AE2
[00013a76] 6520                      bcs.s     $00013A98
[00013a78] 666f                      bne.s     $00013AE9
[00013a7a] 7220                      moveq.l   #32,d1
[00013a7c] 2573 0a00 4572            move.l    0(a3,d0.l*2),17778(a2) ; 68020+ only
[00013a82] 726f                      moveq.l   #111,d1
[00013a84] 7220                      moveq.l   #32,d1
[00013a86] 636c                      bls.s     $00013AF4
[00013a88] 6f73                      ble.s     $00013AFD
[00013a8a] 696e                      bvs.s     $00013AFA
[00013a8c] 6720                      beq.s     $00013AAE
[00013a8e] 696e                      bvs.s     $00013AFE
[00013a90] 7075                      moveq.l   #117,d0
[00013a92] 7420                      moveq.l   #32,d2
[00013a94] 6669                      bne.s     $00013AFF
[00013a96] 6c65                      bge.s     $00013AFD
[00013a98] 004f 7574                 ori.w     #$7574,a7
[00013a9c] 206f 6620                 movea.l   26144(a7),a0
[00013aa0] 6d65                      blt.s     $00013B07
[00013aa2] 6d6f                      blt.s     $00013B13
[00013aa4] 7279                      moveq.l   #121,d1
[00013aa6] 002e 6e64 7800            ori.b     #$64,30720(a6)
[00013aac] 4361                      lea.l     -(a1),a1
[00013aae] 6e27                      bgt.s     $00013AD7
[00013ab0] 7420                      moveq.l   #32,d2
[00013ab2] 6f70                      ble.s     $00013B24
[00013ab4] 656e                      bcs.s     $00013B24
[00013ab6] 206f 7574                 movea.l   30068(a7),a0
[00013aba] 7075                      moveq.l   #117,d0
[00013abc] 7420                      moveq.l   #32,d2
[00013abe] 6669                      bne.s     $00013B29
[00013ac0] 6c65                      bge.s     $00013B27
[00013ac2] 0045 7272                 ori.w     #$7272,d5
[00013ac6] 6f72                      ble.s     $00013B3A
[00013ac8] 2077 7269                 movea.l   105(a7,d7.w*2),a0 ; 68020+ only
[00013acc] 7469                      moveq.l   #105,d2
[00013ace] 6e67                      bgt.s     $00013B37
[00013ad0] 2069 6e64                 movea.l   28260(a1),a0
[00013ad4] 6578                      bcs.s     $00013B4E
[00013ad6] 2066                      movea.l   -(a6),a0
[00013ad8] 696c                      bvs.s     $00013B46
[00013ada] 6500 4572                 bcs       $0001804E
[00013ade] 726f                      moveq.l   #111,d1
[00013ae0] 7220                      moveq.l   #32,d1
[00013ae2] 636c                      bls.s     $00013B50
[00013ae4] 6f73                      ble.s     $00013B59
[00013ae6] 696e                      bvs.s     $00013B56
[00013ae8] 6720                      beq.s     $00013B0A
[00013aea] 696e                      bvs.s     $00013B5A
[00013aec] 6465                      bcc.s     $00013B53
[00013aee] 7820                      moveq.l   #32,d4
[00013af0] 6669                      bne.s     $00013B5B
[00013af2] 6c65                      bge.s     $00013B59
[00013af4] 0000 496e                 ori.b     #$6E,d0
[00013af8] 636f                      bls.s     $00013B69
[00013afa] 6d70                      blt.s     $00013B6C
[00013afc] 6c65                      bge.s     $00013B63
[00013afe] 7465                      moveq.l   #101,d2
[00013b00] 2066                      movea.l   -(a6),a0
[00013b02] 696c                      bvs.s     $00013B70
[00013b04] 6520                      bcs.s     $00013B26
[00013b06] 6865                      bvc.s     $00013B6D
[00013b08] 6164                      bsr.s     $00013B6E
[00013b0a] 6572                      bcs.s     $00013B7E
[00013b0c] 2069 6e20                 movea.l   28192(a1),a0
[00013b10] 6172                      bsr.s     $00013B84
[00013b12] 6368                      bls.s     $00013B7C
[00013b14] 6976                      bvs.s     $00013B8C
[00013b16] 650a                      bcs.s     $00013B22
[00013b18] 0045 7272                 ori.w     #$7272,d5
[00013b1c] 6f72                      ble.s     $00013B90
[00013b1e] 2072 6561 6469            movea.l   ([$6469,a2,zd6.w*4]),a0 ; 68020+ only
[00013b24] 6e67                      bgt.s     $00013B8D
[00013b26] 2061                      movea.l   -(a1),a0
[00013b28] 7263                      moveq.l   #99,d1
[00013b2a] 6869                      bvc.s     $00013B95
[00013b2c] 7665                      moveq.l   #101,d3
[00013b2e] 0a00 0001                 eori.b    #$01,d0
[00013b32] 3b30 0000                 move.w    0(a0,d0.w),-(a5)
[00013b36] 0000 0001                 ori.b     #$01,d0
[00013b3a] 3b30 4d61 6c6c            move.w    ([$6C6C,a0,zd4.l*4]),-(a5) ; 68020+ only
[00013b40] 6f63                      ble.s     $00013BA5
[00013b42] 2825                      move.l    -(a5),d4
[00013b44] 6c78                      bge.s     $00013BBE
[00013b46] 2920                      move.l    -(a0),-(a4)
[00013b48] 3d3e                      move.w    ???,-(a6)
[00013b4a] 2025                      move.l    -(a5),d0
[00013b4c] 6c78                      bge.s     $00013BC6
[00013b4e] 0a00 4361                 eori.b    #$61,d0
[00013b52] 6c6c                      bge.s     $00013BC0
[00013b54] 6f63                      ble.s     $00013BB9
[00013b56] 2825                      move.l    -(a5),d4
[00013b58] 6c78                      bge.s     $00013BD2
[00013b5a] 2c25                      move.l    -(a5),d6
[00013b5c] 6c78                      bge.s     $00013BD6
[00013b5e] 2920                      move.l    -(a0),-(a4)
[00013b60] 3d3e                      move.w    ???,-(a6)
[00013b62] 2025                      move.l    -(a5),d0
[00013b64] 6c78                      bge.s     $00013BDE
[00013b66] 0a00 5245                 eori.b    #$45,d0
[00013b6a] 616c                      bsr.s     $00013BD8
[00013b6c] 6c6f                      bge.s     $00013BDD
[00013b6e] 6328                      bls.s     $00013B98
[00013b70] 256c 782c 256c            move.l    30764(a4),9580(a2)
[00013b76] 7829                      moveq.l   #41,d4
[00013b78] 203d                      move.l    ???,d0
[00013b7a] 3e20                      move.w    -(a0),d7
[00013b7c] 256c 780a 0046            move.l    30730(a4),70(a2)
[00013b82] 7265                      moveq.l   #101,d1
[00013b84] 6528                      bcs.s     $00013BAE
[00013b86] 256c 7829 203d            move.l    30761(a4),8253(a2)
[00013b8c] 3e20                      move.w    -(a0),d7
[00013b8e] 0000 0020                 ori.b     #$20,d0
[00013b92] 2020                      move.l    -(a0),d0
[00013b94] 2020                      move.l    -(a0),d0
[00013b96] 2020                      move.l    -(a0),d0
[00013b98] 2020                      move.l    -(a0),d0
[00013b9a] 0808 0808                 btst      #2056,a0
[00013b9e] 0820 2020                 btst      #8224,-(a0)
[00013ba2] 2020                      move.l    -(a0),d0
[00013ba4] 2020                      move.l    -(a0),d0
[00013ba6] 2020                      move.l    -(a0),d0
[00013ba8] 2020                      move.l    -(a0),d0
[00013baa] 2020                      move.l    -(a0),d0
[00013bac] 2020                      move.l    -(a0),d0
[00013bae] 2020                      move.l    -(a0),d0
[00013bb0] 2018                      move.l    (a0)+,d0
[00013bb2] 1010                      move.b    (a0),d0
[00013bb4] 1010                      move.b    (a0),d0
[00013bb6] 1010                      move.b    (a0),d0
[00013bb8] 1010                      move.b    (a0),d0
[00013bba] 1010                      move.b    (a0),d0
[00013bbc] 1010                      move.b    (a0),d0
[00013bbe] 1010                      move.b    (a0),d0
[00013bc0] 1004                      move.b    d4,d0
[00013bc2] 0404 0404                 subi.b    #$04,d4
[00013bc6] 0404 0404                 subi.b    #$04,d4
[00013bca] 0410 1010                 subi.b    #$10,(a0)
[00013bce] 1010                      move.b    (a0),d0
[00013bd0] 1010                      move.b    (a0),d0
[00013bd2] 4141                      lea.l     d1,a0
[00013bd4] 4141                      lea.l     d1,a0
[00013bd6] 4141                      lea.l     d1,a0
[00013bd8] 0101                      btst      d0,d1
[00013bda] 0101                      btst      d0,d1
[00013bdc] 0101                      btst      d0,d1
[00013bde] 0101                      btst      d0,d1
[00013be0] 0101                      btst      d0,d1
[00013be2] 0101                      btst      d0,d1
[00013be4] 0101                      btst      d0,d1
[00013be6] 0101                      btst      d0,d1
[00013be8] 0101                      btst      d0,d1
[00013bea] 0101                      btst      d0,d1
[00013bec] 1010                      move.b    (a0),d0
[00013bee] 1010                      move.b    (a0),d0
[00013bf0] 1010                      move.b    (a0),d0
[00013bf2] 4242                      clr.w     d2
[00013bf4] 4242                      clr.w     d2
[00013bf6] 4242                      clr.w     d2
[00013bf8] 0202 0202                 andi.b    #$02,d2
[00013bfc] 0202 0202                 andi.b    #$02,d2
[00013c00] 0202 0202                 andi.b    #$02,d2
[00013c04] 0202 0202                 andi.b    #$02,d2
[00013c08] 0202 0202                 andi.b    #$02,d2
[00013c0c] 1010                      move.b    (a0),d0
[00013c0e] 1010                      move.b    (a0),d0
[00013c10] 2000                      move.l    d0,d0
[00013c12] 2227                      move.l    -(a7),d1
[00013c14] 003a 2075 6e6d            ori.b     #$75,$0001AA83(pc)
[00013c1a] 6174                      bsr.s     $00013C90
[00013c1c] 6368                      bls.s     $00013C86
[00013c1e] 6564                      bcs.s     $00013C84
[00013c20] 2071 756f 7465 003a 204e  movea.l   ([$7465,a1],zd7.w*4,$003A204E),a0 ; 68020+ only; reserved OD=3
[00013c2a] 6f20                      ble.s     $00013C4C
[00013c2c] 6d61                      blt.s     $00013C8F
[00013c2e] 7463                      moveq.l   #99,d2
[00013c30] 6800 5c3a                 bvc       $0001986C
[00013c34] 0053 7461                 ori.w     #$7461,(a3)
[00013c38] 636b                      bls.s     $00013CA5
[00013c3a] 204f                      movea.l   a7,a0
[00013c3c] 7665                      moveq.l   #101,d3
[00013c3e] 7266                      moveq.l   #102,d1
[00013c40] 6c6f                      bge.s     $00013CB1
[00013c42] 7700                      mvs.b     d0,d3 ; ColdFire only
[00013c44] 000d 0a00                 ori.b     #$00,a5
[00013c48] 0000 0000                 ori.b     #$00,d0
[00013c4c] 0000 0101                 ori.b     #$01,d0
[00013c50] 0101                      btst      d0,d1
[00013c52] 0101                      btst      d0,d1
[00013c54] 0101                      btst      d0,d1
[00013c56] 0121                      btst      d0,-(a1)
[00013c58] 2101                      move.l    d1,-(a0)
[00013c5a] 2121                      move.l    -(a1),-(a0)
[00013c5c] 0101                      btst      d0,d1
[00013c5e] 0101                      btst      d0,d1
[00013c60] 0101                      btst      d0,d1
[00013c62] 0101                      btst      d0,d1
[00013c64] 0101                      btst      d0,d1
[00013c66] 0101                      btst      d0,d1
[00013c68] 0101                      btst      d0,d1
[00013c6a] 0101                      btst      d0,d1
[00013c6c] 0101                      btst      d0,d1
[00013c6e] 2202                      move.l    d2,d1
[00013c70] 0202 0202                 andi.b    #$02,d2
[00013c74] 0202 0202                 andi.b    #$02,d2
[00013c78] 0202 0202                 andi.b    #$02,d2
[00013c7c] 0202 0404                 andi.b    #$04,d2
[00013c80] 0404 0404                 subi.b    #$04,d4
[00013c84] 0404 0404                 subi.b    #$04,d4
[00013c88] 0202 0202                 andi.b    #$02,d2
[00013c8c] 0202 0208                 andi.b    #$08,d2
[00013c90] 0808 0808                 btst      #2056,a0
[00013c94] 0808 0808                 btst      #2056,a0
[00013c98] 0808 0808                 btst      #2056,a0
[00013c9c] 0808 0808                 btst      #2056,a0
[00013ca0] 0808 0808                 btst      #2056,a0
[00013ca4] 0808 0808                 btst      #2056,a0
[00013ca8] 0802 0202                 btst      #514,d2
[00013cac] 0202 0210                 andi.b    #$10,d2
[00013cb0] 1010                      move.b    (a0),d0
[00013cb2] 1010                      move.b    (a0),d0
[00013cb4] 1010                      move.b    (a0),d0
[00013cb6] 1010                      move.b    (a0),d0
[00013cb8] 1010                      move.b    (a0),d0
[00013cba] 1010                      move.b    (a0),d0
[00013cbc] 1010                      move.b    (a0),d0
[00013cbe] 1010                      move.b    (a0),d0
[00013cc0] 1010                      move.b    (a0),d0
[00013cc2] 1010                      move.b    (a0),d0
[00013cc4] 1010                      move.b    (a0),d0
[00013cc6] 1010                      move.b    (a0),d0
[00013cc8] 1002                      move.b    d2,d0
[00013cca] 0202 0201                 andi.b    #$01,d2
[00013cce] 0000 0000                 ori.b     #$00,d0
[00013cd2] 0001 1c9a                 ori.b     #$9A,d1
[00013cd6] 0001 1ce4                 ori.b     #$E4,d1
[00013cda] 0001 1c60                 ori.b     #$60,d1
[00013cde] 0001 1c80                 ori.b     #$80,d1
[00013ce2] 0001 1cb8                 ori.b     #$B8,d1
[00013ce6] 0001 1ce4                 ori.b     #$E4,d1
[00013cea] 0001 1ce4                 ori.b     #$E4,d1
[00013cee] 0001 1c3e                 ori.b     #$3E,d1
[00013cf2] 0d00                      btst      d6,d0
[00013cf4] 0000 0000                 ori.b     #$00,d0
[00013cf8] 0001 3cf8                 ori.b     #$F8,d1
[00013cfc] 0000 ffff                 ori.b     #$FF,d0
[00013d00] 0001 3cf8                 ori.b     #$F8,d1
[00013d04] 0001 2e5e                 ori.b     #$5E,d1
[00013d08] 0001 2cfa                 ori.b     #$FA,d1
[00013d0c] 0001 2e7a                 ori.b     #$7A,d1
[00013d10] 0001 2ea0                 ori.b     #$A0,d1
[00013d14] 0001 2ec4                 ori.b     #$C4,d1
[00013d18] 0001 2ee8                 ori.b     #$E8,d1
[00013d1c] 0001 2ee8                 ori.b     #$E8,d1
[00013d20] 0001 2ee8                 ori.b     #$E8,d1
[00013d24] 0001 2ee8                 ori.b     #$E8,d1
[00013d28] 0001 2ee8                 ori.b     #$E8,d1
[00013d2c] 0001 2ee8                 ori.b     #$E8,d1
[00013d30] 0001 2ee8                 ori.b     #$E8,d1
[00013d34] 0001 2da4                 ori.b     #$A4,d1
[00013d38] 0001 2ee8                 ori.b     #$E8,d1
[00013d3c] 0001 2ee8                 ori.b     #$E8,d1
[00013d40] 0001 2ee8                 ori.b     #$E8,d1
[00013d44] 0001 2e4c                 ori.b     #$4C,d1
[00013d48] 0001 2ee8                 ori.b     #$E8,d1
[00013d4c] 0001 2d50                 ori.b     #$50,d1
[00013d50] 0001 2ee8                 ori.b     #$E8,d1
[00013d54] 0001 2ee8                 ori.b     #$E8,d1
[00013d58] 0001 2df8                 ori.b     #$F8,d1
[00013d5c] 0001 2ee8                 ori.b     #$E8,d1
[00013d60] 0001 2ee8                 ori.b     #$E8,d1
[00013d64] 0001 2ee8                 ori.b     #$E8,d1
[00013d68] 0001 2ee8                 ori.b     #$E8,d1
[00013d6c] 0001 2ee8                 ori.b     #$E8,d1
[00013d70] 0001 2ee8                 ori.b     #$E8,d1
[00013d74] 0001 2ee8                 ori.b     #$E8,d1
[00013d78] 0001 2ee8                 ori.b     #$E8,d1
[00013d7c] 0001 2ee8                 ori.b     #$E8,d1
[00013d80] 0001 2ee8                 ori.b     #$E8,d1
[00013d84] 0001 2e5e                 ori.b     #$5E,d1
[00013d88] 0001 2cfe                 ori.b     #$FE,d1
[00013d8c] 0001 2e7a                 ori.b     #$7A,d1
[00013d90] 0001 2ea0                 ori.b     #$A0,d1
[00013d94] 0001 2ec4                 ori.b     #$C4,d1
[00013d98] 0001 2ee8                 ori.b     #$E8,d1
[00013d9c] 0001 2ee8                 ori.b     #$E8,d1
[00013da0] 0001 2ee8                 ori.b     #$E8,d1
[00013da4] 0001 2ee8                 ori.b     #$E8,d1
[00013da8] 0001 2ee8                 ori.b     #$E8,d1
[00013dac] 0001 2ee8                 ori.b     #$E8,d1
[00013db0] 0001 2ee8                 ori.b     #$E8,d1
[00013db4] 0001 2da8                 ori.b     #$A8,d1
[00013db8] 0001 2ee8                 ori.b     #$E8,d1
[00013dbc] 0001 2ee8                 ori.b     #$E8,d1
[00013dc0] 0001 2ee8                 ori.b     #$E8,d1
[00013dc4] 0001 2e4c                 ori.b     #$4C,d1
[00013dc8] 0001 2ee8                 ori.b     #$E8,d1
[00013dcc] 0001 2d54                 ori.b     #$54,d1
[00013dd0] 0001 2ee8                 ori.b     #$E8,d1
[00013dd4] 0001 2ee8                 ori.b     #$E8,d1
[00013dd8] 0001 2dfc                 ori.b     #$FC,d1
[00013ddc] 0000 0001                 ori.b     #$01,d0
[00013de0] 0000 0000                 ori.b     #$00,d0
[00013de4] 0000 0000                 ori.b     #$00,d0
[00013de8] 0000 0001                 ori.b     #$01,d0
[00013dec] 000a 0000                 ori.b     #$00,a2
[00013df0] 0000 0000                 ori.b     #$00,d0
[00013df4] 0000 0000                 ori.b     #$00,d0
[00013df8] 0002 000a                 ori.b     #$0A,d2
[00013dfc] 0000 0000                 ori.b     #$00,d0
[00013e00] 0000 0000                 ori.b     #$00,d0
[00013e04] 0000 0003                 ori.b     #$03,d0
[00013e08] 0000 0000                 ori.b     #$00,d0
[00013e0c] 0000 0000                 ori.b     #$00,d0
[00013e10] 0000 0000                 ori.b     #$00,d0
[00013e14] 0004 0000                 ori.b     #$00,d4
[00013e18] 0000 0000                 ori.b     #$00,d0
[00013e1c] 0000 0000                 ori.b     #$00,d0
[00013e20] 0000 0005                 ori.b     #$05,d0
[00013e24] 0000 0000                 ori.b     #$00,d0
[00013e28] 0000 0000                 ori.b     #$00,d0
[00013e2c] 0000 0000                 ori.b     #$00,d0
[00013e30] 0006 0000                 ori.b     #$00,d6
[00013e34] 0000 0000                 ori.b     #$00,d0
[00013e38] 0000 0000                 ori.b     #$00,d0
[00013e3c] 0000 0007                 ori.b     #$07,d0
[00013e40] 0000 0000                 ori.b     #$00,d0
[00013e44] 0000 0000                 ori.b     #$00,d0
[00013e48] 0000 0000                 ori.b     #$00,d0
[00013e4c] 0008 0000                 ori.b     #$00,a0
[00013e50] 0000 0000                 ori.b     #$00,d0
[00013e54] 0000 0000                 ori.b     #$00,d0
[00013e58] 0000 0009                 ori.b     #$09,d0
[00013e5c] 0000 0000                 ori.b     #$00,d0
[00013e60] 0000 0000                 ori.b     #$00,d0
[00013e64] 0000 0000                 ori.b     #$00,d0
[00013e68] 000a 0000                 ori.b     #$00,a2
[00013e6c] 0000 0000                 ori.b     #$00,d0
[00013e70] 0000 0000                 ori.b     #$00,d0
[00013e74] 0000 000b                 ori.b     #$0B,d0
[00013e78] 0000 0000                 ori.b     #$00,d0
[00013e7c] 0000 0000                 ori.b     #$00,d0
[00013e80] 0000 0000                 ori.b     #$00,d0
[00013e84] 000c 0000                 ori.b     #$00,a4
[00013e88] 0000 0000                 ori.b     #$00,d0
[00013e8c] 0000 0000                 ori.b     #$00,d0
[00013e90] 0000 000d                 ori.b     #$0D,d0
[00013e94] 0000 0000                 ori.b     #$00,d0
[00013e98] 0000 0000                 ori.b     #$00,d0
[00013e9c] 0000 0000                 ori.b     #$00,d0
[00013ea0] 000e 0000                 ori.b     #$00,a6
[00013ea4] 0000 0000                 ori.b     #$00,d0
[00013ea8] 0000 0000                 ori.b     #$00,d0
[00013eac] 0000 000f                 ori.b     #$0F,d0
[00013eb0] 0000 0000                 ori.b     #$00,d0
[00013eb4] 0000 0000                 ori.b     #$00,d0
[00013eb8] 0000 0000                 ori.b     #$00,d0

../bin/doindex.ttp: text 0x000035d4, data 0x000008e8, bss 0x000023c0
0x00000000 - 0x00000154 gemst8k.o
             0x00001492 <unknown>
0x00001492 - 0x000014a2 xbios.o
             0x00001492 _xbios
0x000014a2 - 0x000014b2 bios.o
             0x000014a2 _bios
0x000014b2 - 0x000014c2 gemdos.o
             0x000014b2 _gemdos
000114c2   - 00011528   xmain.o
             000114c2   __main
00011528   - 00011862   xomain.o
             00011528   ___main

00011862   - 000118e4   ldiv.o
             00011862   ldiv

0x000018e4 - 0x00001938 lmul.o
             0x000018e4 lmul
0x00001938 - 0x00001948 trap.o
             0x00001938 _trap
0x00001948 - 0x00001984 xread.o
             0x00001948 __pc_rea
0x00001984 - 0x000019c0 xwrite.o
             0x00001984 __pc_wri
0x000019c0 - 0x00001a3c uldiv.o
             0x000019c0 _uldiv
0x00001a3c - 0x00001a6a salloc.o
             0x00001a3c __salloc
0x00001a6a - 0x00001adc stricmp.o
             0x00001a6a __strcmp
0x00001adc - 0x00001ae4 fdecls.o
             0x00001adc ___fdecl
0x00001ae4 - 0x00001b0a strlen.o
             0x00001ae4 _strlen
0x00001b0a - 0x00001b2c strcpy.o
             0x00001b0a _strcpy
0x00001b2c - 0x00001b74 strncpy.o
             0x00001b2c _strncpy
0x00001b74 - 0x00001bb6 strcmp.o
             0x00001b74 _strcmp
0x00001bb6 - 0x00001be0 strcat.o
             0x00001bb6 _strcat
0x00001be0 - 0x00001d24 fixxopen.o
             0x00001be0 ___open
             0x00001cf0 _ucase
0x00001d24 - 0x00001dc6 ttyout.o
             0x00001d24 __ttyout
0x00001dc6 - 0x00001e1e fixlst.o
             0x00001dc6 __lstout
             0x00001e06 _j30june
0x00001e1e - 0x00001f88 fixwrtc.o
             0x00001e1e __wrtchr
0x00001f88 - 0x00002002 writebin.o
             0x00001f88 __wrtbin
0x00002002 - 0x00002112 writeasc.o
             0x00002002 __wrtasc
0x00002112 - 0x00002178 allocc.o
             0x00002112 __allocc
             0x0000215c __freec
0x00002178 - 0x00002208 chinit.o
             0x00002178 __chinit
             0x0000219a ___chini
0x00002208 - 0x0000226a chkc.o
             0x00002208 __chkc
0x0000226a - 0x00002306 fixwrite.o
             0x0000226a _write
0x00002306 - 0x00002370 sbrk.o
             0x00002306 _sbrk
0x00002370 - 0x000023de prtshort.o
             0x00002370 ___prtsh
0x000023de - 0x00002494 prtld.o
             0x000023de ___prtld
0x00002494 - 0x00002506 prtint.o
             0x00002494 ___prtin
0x00002506 - 0x00002516 mallocnd.o
             0x00002506 _malloc_
             0x0000250e __errmal
0x00002516 - 0x00002822 malloc.o
             0x00002516 _malloc
             0x000026a4 _free
             0x000027a2 _realloc
0x00002822 - 0x0000289e isatty.o
             0x00002822 _isatty
             0x00002856 _isdev
             0x00002882 _ttyname
0x0000289e - 0x000029ec flsbuf.o
             0x0000289e __flsbuf
0x000029ec - 0x00002a32 fputc.o
             0x000029ec _fputc
0x00002a32 - 0x00002aa2 fputn.o
             0x00002a32 _fputn
0x00002aa2 - 0x00002b5c xdoprtf.o !! todo
0x00002b5c - 0x00002fe8 doprt.o
             0x00002b5c __doprt
0x00002fe8 - 0x0000305a lseek.o
             0x00002fe8 _lseek
             0x00003044 _tell
0x0000305a - 0x0000319e open.o
             0x0000305a __open
             0x00003154 _open
             0x0000316c _opena
             0x00003184 _openb
0x0000319e - 0x00003238 fflush.o
             0x0000319e _fflush
0x00003238 - 0x00003240 ctype.o
             0x00003238 ____atab
0x00003240 - 0x000032c8 close.o
             0x00003240  _close
0x000032c8 - 0x00003320 fclose.o
             0x000032c8 _fclose
0x00003320 - 0x00003352 cleanup.o
             0x00003320 __cleanu
0x00003352 - 0x0000336a exit.o
             0x00003352 _exit
0x0000336a - 0x000033c6 strrchr.o
             0x0000336a _strrchr
             0x000033a0 _rindex
0x000033c6 - 0x00003546 qsort.o
0x00003546 - 0x00003592 puts.o
             0x00003546 _puts
0x00003592 - 0x000035d4 printf.o
             0x00003592 _printf
             0x000035b4 _fprintf

00013EBC __base
00013EC0 __break
00013EE2 _argc
00013EE4 _argv
00013EE8 _argv2
