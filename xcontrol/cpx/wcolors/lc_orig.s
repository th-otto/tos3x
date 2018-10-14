; ph_branch = 0x601a
; ph_tlen = 0x00003588
; ph_dlen = 0x000017d8
; ph_blen = 0x00000398
; ph_slen = 0x00000000
; ph_res1 = 0x00000000
; ph_prgflags = 0x00000001
; ph_absflag = 0x0000
; first relocation = 0x0000000c
; relocation bytes = 0x00000329

; 0x00000001 bytes of junk at end of file

[00010000] 42a7                      clr.l     -(a7)
[00010002] 3f3c 0020                 move.w    #$0020,-(a7)
[00010006] 4e41                      trap      #1
[00010008] 23f8 04ba 0001 4d6c       move.l    ($000004BA).w,__clkstart
[00010010] 2078 04f2                 movea.l   ($000004F2).w,a0
[00010014] 2068 0008                 movea.l   8(a0),a0
[00010018] 33e8 0002 0001 4d80       move.w    2(a0),__tos
[00010020] 3228 001c                 move.w    28(a0),d1
[00010024] e249                      lsr.w     #1,d1
[00010026] 33c1 0001 4d82            move.w    d1,__country
[0001002c] 2f40 0002                 move.l    d0,2(a7)
[00010030] 3ebc 0020                 move.w    #$0020,(a7)
[00010034] 4e41                      trap      #1
[00010036] 5c4f                      addq.w    #6,a7
[00010038] 4eb9 0001 329c            jsr       __VDIinit
[0001003e] 4eb9 0001 3584            jsr       __FPCinit
[00010044] 4ef9 0001 2b44            jmp       cpx_init
[0001004a] 4afc                      illegal
[0001004c] 4ef9 0001 2b44            jmp       cpx_init
[00010052] 0000
init_tree:
[00010054] 48e7 2120                 movem.l   d2/d7/a2,-(a7)
[00010058] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00010060] 6c00 008a                 bge       $000100EC
[00010064] 2079 0001 4dcc            movea.l   _tree,a0
[0001006a] 08e8 0007 00e1            bset      #7,225(a0)
[00010070] 2079 0001 4dcc            movea.l   _tree,a0
[00010076] 08e8 0007 0171            bset      #7,369(a0)
[0001007c] 2079 0001 4dcc            movea.l   _tree,a0
[00010082] 08e8 0007 0189            bset      #7,393(a0)
[00010088] 2079 0001 4dcc            movea.l   _tree,a0
[0001008e] 08e8 0007 01a1            bset      #7,417(a0)
[00010094] 2079 0001 4dcc            movea.l   _tree,a0
[0001009a] 08e8 0007 01b9            bset      #7,441(a0)
[000100a0] 2079 0001 4dcc            movea.l   _tree,a0
[000100a6] 08e8 0007 01e9            bset      #7,489(a0)
[000100ac] 2079 0001 4dcc            movea.l   _tree,a0
[000100b2] 08e8 0007 0201            bset      #7,513(a0)
[000100b8] 2079 0001 4dcc            movea.l   _tree,a0
[000100be] 08e8 0007 0219            bset      #7,537(a0)
[000100c4] 2079 0001 4dcc            movea.l   _tree,a0
[000100ca] 08e8 0007 0231            bset      #7,561(a0)
[000100d0] 2079 0001 4dcc            movea.l   _tree,a0
[000100d6] 3168 0100 00d0            move.w    256(a0),208(a0)
[000100dc] 2079 0001 4dcc            movea.l   _tree,a0
[000100e2] 3168 0104 00d4            move.w    260(a0),212(a0)
[000100e8] 6000 01b8                 bra       $000102A2
[000100ec] 2079 0001 4dcc            movea.l   _tree,a0
[000100f2] 08a8 0001 0038            bclr      #1,56(a0)
[000100f8] 2079 0001 4dcc            movea.l   _tree,a0
[000100fe] 08a8 0002 0038            bclr      #2,56(a0)
[00010104] 2079 0001 4dcc            movea.l   _tree,a0
[0001010a] 08a8 0001 0068            bclr      #1,104(a0)
[00010110] 2079 0001 4dcc            movea.l   _tree,a0
[00010116] 08a8 0002 0068            bclr      #2,104(a0)
[0001011c] 2079 0001 4dcc            movea.l   _tree,a0
[00010122] 08a8 0001 0080            bclr      #1,128(a0)
[00010128] 2079 0001 4dcc            movea.l   _tree,a0
[0001012e] 08a8 0002 0080            bclr      #2,128(a0)
[00010134] 0c79 0008 0001 4de8       cmpi.w    #$0008,$00014DE8
[0001013c] 6f00 0164                 ble       $000102A2
[00010140] 2079 0001 4dcc            movea.l   _tree,a0
[00010146] 08e8 0001 0038            bset      #1,56(a0)
[0001014c] 2079 0001 4dcc            movea.l   _tree,a0
[00010152] 08e8 0002 0038            bset      #2,56(a0)
[00010158] 2079 0001 4dcc            movea.l   _tree,a0
[0001015e] 08e8 0001 0068            bset      #1,104(a0)
[00010164] 2079 0001 4dcc            movea.l   _tree,a0
[0001016a] 08e8 0002 0068            bset      #2,104(a0)
[00010170] 2079 0001 4dcc            movea.l   _tree,a0
[00010176] 08e8 0001 0080            bset      #1,128(a0)
[0001017c] 2079 0001 4dcc            movea.l   _tree,a0
[00010182] 08e8 0002 0080            bset      #2,128(a0)
[00010188] 2079 0001 4dcc            movea.l   _tree,a0
[0001018e] 5468 0040                 addq.w    #2,64(a0)
[00010192] 2079 0001 4dcc            movea.l   _tree,a0
[00010198] 5468 0042                 addq.w    #2,66(a0)
[0001019c] 2079 0001 4dcc            movea.l   _tree,a0
[000101a2] 5968 0044                 subq.w    #4,68(a0)
[000101a6] 2079 0001 4dcc            movea.l   _tree,a0
[000101ac] 5968 0046                 subq.w    #4,70(a0)
[000101b0] 2079 0001 4dcc            movea.l   _tree,a0
[000101b6] 5468 0070                 addq.w    #2,112(a0)
[000101ba] 2079 0001 4dcc            movea.l   _tree,a0
[000101c0] 5468 0072                 addq.w    #2,114(a0)
[000101c4] 2079 0001 4dcc            movea.l   _tree,a0
[000101ca] 5968 0074                 subq.w    #4,116(a0)
[000101ce] 2079 0001 4dcc            movea.l   _tree,a0
[000101d4] 5968 0076                 subq.w    #4,118(a0)
[000101d8] 2079 0001 4dcc            movea.l   _tree,a0
[000101de] 5468 0088                 addq.w    #2,136(a0)
[000101e2] 2079 0001 4dcc            movea.l   _tree,a0
[000101e8] 5468 008a                 addq.w    #2,138(a0)
[000101ec] 2079 0001 4dcc            movea.l   _tree,a0
[000101f2] 5968 008c                 subq.w    #4,140(a0)
[000101f6] 2079 0001 4dcc            movea.l   _tree,a0
[000101fc] 5968 008e                 subq.w    #4,142(a0)
[00010200] 7e00                      moveq.l   #0,d7
[00010202] 7013                      moveq.l   #19,d0
[00010204] be40                      cmp.w     d0,d7
[00010206] 6c00 009a                 bge       $000102A2
[0001020a] 2007                      move.l    d7,d0
[0001020c] 48c0                      ext.l     d0
[0001020e] d080                      add.l     d0,d0
[00010210] 41f9 0001 4d38            lea.l     $00014D38,a0
[00010216] 2248                      movea.l   a0,a1
[00010218] d3c0                      adda.l    d0,a1
[0001021a] 3211                      move.w    (a1),d1
[0001021c] 7418                      moveq.l   #24,d2
[0001021e] c3c2                      muls.w    d2,d1
[00010220] 2279 0001 4dcc            movea.l   _tree,a1
[00010226] 2449                      movea.l   a1,a2
[00010228] d5c1                      adda.l    d1,a2
[0001022a] 08ea 0001 0008            bset      #1,8(a2)
[00010230] 2248                      movea.l   a0,a1
[00010232] d3c0                      adda.l    d0,a1
[00010234] 3211                      move.w    (a1),d1
[00010236] c3c2                      muls.w    d2,d1
[00010238] 2279 0001 4dcc            movea.l   _tree,a1
[0001023e] 2449                      movea.l   a1,a2
[00010240] d5c1                      adda.l    d1,a2
[00010242] 08aa 0002 0008            bclr      #2,8(a2)
[00010248] 2248                      movea.l   a0,a1
[0001024a] d3c0                      adda.l    d0,a1
[0001024c] 3211                      move.w    (a1),d1
[0001024e] c3c2                      muls.w    d2,d1
[00010250] 2279 0001 4dcc            movea.l   _tree,a1
[00010256] 2449                      movea.l   a1,a2
[00010258] d5c1                      adda.l    d1,a2
[0001025a] 546a 0010                 addq.w    #2,16(a2)
[0001025e] 2248                      movea.l   a0,a1
[00010260] d3c0                      adda.l    d0,a1
[00010262] 3211                      move.w    (a1),d1
[00010264] c3c2                      muls.w    d2,d1
[00010266] 2279 0001 4dcc            movea.l   _tree,a1
[0001026c] 2449                      movea.l   a1,a2
[0001026e] d5c1                      adda.l    d1,a2
[00010270] 546a 0012                 addq.w    #2,18(a2)
[00010274] 2248                      movea.l   a0,a1
[00010276] d3c0                      adda.l    d0,a1
[00010278] 3211                      move.w    (a1),d1
[0001027a] c3c2                      muls.w    d2,d1
[0001027c] 2279 0001 4dcc            movea.l   _tree,a1
[00010282] 2449                      movea.l   a1,a2
[00010284] d5c1                      adda.l    d1,a2
[00010286] 596a 0014                 subq.w    #4,20(a2)
[0001028a] d1c0                      adda.l    d0,a0
[0001028c] 3010                      move.w    (a0),d0
[0001028e] c1c2                      muls.w    d2,d0
[00010290] 2079 0001 4dcc            movea.l   _tree,a0
[00010296] d1c0                      adda.l    d0,a0
[00010298] 5968 0016                 subq.w    #4,22(a0)
[0001029c] 5247                      addq.w    #1,d7
[0001029e] 6000 ff62                 bra       $00010202
[000102a2] 4cdf 0484                 movem.l   (a7)+,d2/d7/a2
[000102a6] 4e75                      rts
_wait_bup:
[000102a8] 4e56 fff8                 link      a6,#$FFF8
[000102ac] 486e fff8                 pea.l     -8(a6)
[000102b0] 4eb9 0001 3098            jsr       _Graf_mkstate
[000102b6] 584f                      addq.w    #4,a7
[000102b8] 4a6e fffc                 tst.w     -4(a6)
[000102bc] 66ee                      bne.s     $000102AC
[000102be] 4e5e                      unlk      a6
[000102c0] 4e75                      rts
_itoa2:
[000102c2] 48e7 2104                 movem.l   d2/d7/a5,-(a7)
[000102c6] 3e2f 0010                 move.w    16(a7),d7
[000102ca] 2a6f 0012                 movea.l   18(a7),a5
[000102ce] 2007                      move.l    d7,d0
[000102d0] 48c0                      ext.l     d0
[000102d2] 720a                      moveq.l   #10,d1
[000102d4] 81c1                      divs.w    d1,d0
[000102d6] 0640 0030                 addi.w    #$0030,d0
[000102da] 1a80                      move.b    d0,(a5)
[000102dc] 7430                      moveq.l   #48,d2
[000102de] b002                      cmp.b     d2,d0
[000102e0] 6702                      beq.s     $000102E4
[000102e2] 524d                      addq.w    #1,a5
[000102e4] 2007                      move.l    d7,d0
[000102e6] 48c0                      ext.l     d0
[000102e8] 81c1                      divs.w    d1,d0
[000102ea] 4840                      swap      d0
[000102ec] 0640 0030                 addi.w    #$0030,d0
[000102f0] 1ac0                      move.b    d0,(a5)+
[000102f2] 4215                      clr.b     (a5)
[000102f4] 4cdf 2084                 movem.l   (a7)+,d2/d7/a5
[000102f8] 4e75                      rts
_getrect:
[000102fa] 48e7 0114                 movem.l   d7/a3/a5,-(a7)
[000102fe] 2a6f 0010                 movea.l   16(a7),a5
[00010302] 3e2f 0014                 move.w    20(a7),d7
[00010306] 266f 0016                 movea.l   22(a7),a3
[0001030a] 2007                      move.l    d7,d0
[0001030c] c1fc 0018                 muls.w    #$0018,d0
[00010310] 204d                      movea.l   a5,a0
[00010312] d1c0                      adda.l    d0,a0
[00010314] 43e8 0010                 lea.l     16(a0),a1
[00010318] 204b                      movea.l   a3,a0
[0001031a] 20d9                      move.l    (a1)+,(a0)+
[0001031c] 20d9                      move.l    (a1)+,(a0)+
[0001031e] 41eb 0002                 lea.l     2(a3),a0
[00010322] 2f08                      move.l    a0,-(a7)
[00010324] 2f0b                      move.l    a3,-(a7)
[00010326] 3f07                      move.w    d7,-(a7)
[00010328] 2f0d                      move.l    a5,-(a7)
[0001032a] 4eb9 0001 31a0            jsr       _objc_offset
[00010330] 4fef 000e                 lea.l     14(a7),a7
[00010334] 4cdf 2880                 movem.l   (a7)+,d7/a3/a5
[00010338] 4e75                      rts
sl_x:
[0001033a] 4e56 ffe4                 link      a6,#$FFE4
[0001033e] 48e7 3f14                 movem.l   d2-d7/a3/a5,-(a7)
[00010342] 2a6e 0008                 movea.l   8(a6),a5
[00010346] 3e2e 000c                 move.w    12(a6),d7
[0001034a] 3c2e 000e                 move.w    14(a6),d6
[0001034e] 3a2e 0010                 move.w    16(a6),d5
[00010352] 382e 0012                 move.w    18(a6),d4
[00010356] 266e 0016                 movea.l   22(a6),a3
[0001035a] 486e fff8                 pea.l     -8(a6)
[0001035e] 3f07                      move.w    d7,-(a7)
[00010360] 2f0d                      move.l    a5,-(a7)
[00010362] 6196                      bsr.s     _getrect
[00010364] 486e fff0                 pea.l     -16(a6)
[00010368] 3f06                      move.w    d6,-(a7)
[0001036a] 2f0d                      move.l    a5,-(a7)
[0001036c] 618c                      bsr.s     _getrect
[0001036e] 4fef 0014                 lea.l     20(a7),a7
[00010372] 302e 0014                 move.w    20(a6),d0
[00010376] b045                      cmp.w     d5,d0
[00010378] 661e                      bne.s     $00010398
[0001037a] 2206                      move.l    d6,d1
[0001037c] 7418                      moveq.l   #24,d2
[0001037e] c3c2                      muls.w    d2,d1
[00010380] 362e fff4                 move.w    -12(a6),d3
[00010384] 2f41 0020                 move.l    d1,32(a7)
[00010388] 322e fffc                 move.w    -4(a6),d1
[0001038c] 9243                      sub.w     d3,d1
[0001038e] 262f 0020                 move.l    32(a7),d3
[00010392] 3b81 3810                 move.w    d1,16(a5,d3.l)
[00010396] 6066                      bra.s     $000103FE
[00010398] 2206                      move.l    d6,d1
[0001039a] c3fc 0018                 muls.w    #$0018,d1
[0001039e] 2405                      move.l    d5,d2
[000103a0] 9444                      sub.w     d4,d2
[000103a2] 48c2                      ext.l     d2
[000103a4] 362e fffc                 move.w    -4(a6),d3
[000103a8] 966e fff4                 sub.w     -12(a6),d3
[000103ac] 48c3                      ext.l     d3
[000103ae] 2003                      move.l    d3,d0
[000103b0] 2f41 0020                 move.l    d1,32(a7)
[000103b4] 223c 0000 03e8            move.l    #$000003E8,d1
[000103ba] 4eb9 0001 34bc            jsr       __CXM33
[000103c0] 2202                      move.l    d2,d1
[000103c2] 4eb9 0001 34bc            jsr       __CXM33
[000103c8] 322e 0014                 move.w    20(a6),d1
[000103cc] 9244                      sub.w     d4,d1
[000103ce] 5241                      addq.w    #1,d1
[000103d0] 48c1                      ext.l     d1
[000103d2] 2f40 0024                 move.l    d0,36(a7)
[000103d6] 203c 0000 03e8            move.l    #$000003E8,d0
[000103dc] 4eb9 0001 34bc            jsr       __CXM33
[000103e2] 222f 0024                 move.l    36(a7),d1
[000103e6] 2f40 0028                 move.l    d0,40(a7)
[000103ea] 2001                      move.l    d1,d0
[000103ec] 222f 0028                 move.l    40(a7),d1
[000103f0] 4eb9 0001 34dc            jsr       __CXD33
[000103f6] 222f 0020                 move.l    32(a7),d1
[000103fa] 3b80 1810                 move.w    d0,16(a5,d1.l)
[000103fe] 200b                      move.l    a3,d0
[00010400] 6702                      beq.s     $00010404
[00010402] 4e93                      jsr       (a3)
[00010404] 4cdf 28fc                 movem.l   (a7)+,d2-d7/a3/a5
[00010408] 4e5e                      unlk      a6
[0001040a] 4e75                      rts
_draw_slider:
[0001040c] 4e56 ffe4                 link      a6,#$FFE4
[00010410] 48e7 0714                 movem.l   d5-d7/a3/a5,-(a7)
[00010414] 2a6e 0008                 movea.l   8(a6),a5
[00010418] 3e2e 000c                 move.w    12(a6),d7
[0001041c] 3c2e 000e                 move.w    14(a6),d6
[00010420] 3a2e 0010                 move.w    16(a6),d5
[00010424] 266e 0016                 movea.l   22(a6),a3
[00010428] 486e fff0                 pea.l     -16(a6)
[0001042c] 3f06                      move.w    d6,-(a7)
[0001042e] 2f0d                      move.l    a5,-(a7)
[00010430] 6100 fec8                 bsr       _getrect
[00010434] 4fef 000a                 lea.l     10(a7),a7
[00010438] 7000                      moveq.l   #0,d0
[0001043a] 2206                      move.l    d6,d1
[0001043c] c3fc 0018                 muls.w    #$0018,d1
[00010440] 2075 180c                 movea.l   12(a5,d1.l),a0
[00010444] 3d40 ffe6                 move.w    d0,-26(a6)
[00010448] 3028 0016                 move.w    22(a0),d0
[0001044c] 4a40                      tst.w     d0
[0001044e] 6a08                      bpl.s     $00010458
[00010450] 3028 0016                 move.w    22(a0),d0
[00010454] 3d40 ffe6                 move.w    d0,-26(a6)
[00010458] 41ee fff0                 lea.l     -16(a6),a0
[0001045c] 2f08                      move.l    a0,-(a7)
[0001045e] 302e ffe6                 move.w    -26(a6),d0
[00010462] 3f00                      move.w    d0,-(a7)
[00010464] 3f00                      move.w    d0,-(a7)
[00010466] 2f08                      move.l    a0,-(a7)
[00010468] 4eb9 0001 2f02            jsr       _Rc_center
[0001046e] 204b                      movea.l   a3,a0
[00010470] 2e88                      move.l    a0,(a7)
[00010472] 3f2e 0014                 move.w    20(a6),-(a7)
[00010476] 3f2e 0012                 move.w    18(a6),-(a7)
[0001047a] 3f05                      move.w    d5,-(a7)
[0001047c] 3f06                      move.w    d6,-(a7)
[0001047e] 3f07                      move.w    d7,-(a7)
[00010480] 2f0d                      move.l    a5,-(a7)
[00010482] 6100 feb6                 bsr       sl_x
[00010486] 486e fff8                 pea.l     -8(a6)
[0001048a] 3f06                      move.w    d6,-(a7)
[0001048c] 2f0d                      move.l    a5,-(a7)
[0001048e] 6100 fe6a                 bsr       _getrect
[00010492] 41ee fff8                 lea.l     -8(a6),a0
[00010496] 2e88                      move.l    a0,(a7)
[00010498] 302e ffe6                 move.w    -26(a6),d0
[0001049c] 3f00                      move.w    d0,-(a7)
[0001049e] 3f00                      move.w    d0,-(a7)
[000104a0] 2f08                      move.l    a0,-(a7)
[000104a2] 4eb9 0001 2f02            jsr       _Rc_center
[000104a8] 486e fff8                 pea.l     -8(a6)
[000104ac] 7008                      moveq.l   #8,d0
[000104ae] 3f00                      move.w    d0,-(a7)
[000104b0] 3f06                      move.w    d6,-(a7)
[000104b2] 2f0d                      move.l    a5,-(a7)
[000104b4] 4eb9 0001 3034            jsr       _Objc_draw
[000104ba] 322e fff8                 move.w    -8(a6),d1
[000104be] b26e fff0                 cmp.w     -16(a6),d1
[000104c2] 5dc0                      slt       d0
[000104c4] 4400                      neg.b     d0
[000104c6] 4880                      ext.w     d0
[000104c8] 486e fff8                 pea.l     -8(a6)
[000104cc] 486e fff0                 pea.l     -16(a6)
[000104d0] 3d40 ffe4                 move.w    d0,-28(a6)
[000104d4] 4eb9 0001 2e44            jsr       _rc_intersect
[000104da] 4fef 0040                 lea.l     64(a7),a7
[000104de] 4a40                      tst.w     d0
[000104e0] 6716                      beq.s     $000104F8
[000104e2] 536e fffc                 subq.w    #1,-4(a6)
[000104e6] 302e fffc                 move.w    -4(a6),d0
[000104ea] 916e fff4                 sub.w     d0,-12(a6)
[000104ee] 4a6e ffe4                 tst.w     -28(a6)
[000104f2] 6704                      beq.s     $000104F8
[000104f4] d16e fff0                 add.w     d0,-16(a6)
[000104f8] 486e ffe8                 pea.l     -24(a6)
[000104fc] 3f07                      move.w    d7,-(a7)
[000104fe] 2f0d                      move.l    a5,-(a7)
[00010500] 6100 fdf8                 bsr       _getrect
[00010504] 486e ffe8                 pea.l     -24(a6)
[00010508] 486e fff0                 pea.l     -16(a6)
[0001050c] 4eb9 0001 2e44            jsr       _rc_intersect
[00010512] 486e ffe8                 pea.l     -24(a6)
[00010516] 7008                      moveq.l   #8,d0
[00010518] 3f00                      move.w    d0,-(a7)
[0001051a] 3f07                      move.w    d7,-(a7)
[0001051c] 2f0d                      move.l    a5,-(a7)
[0001051e] 4eb9 0001 3034            jsr       _Objc_draw
[00010524] 3d7c 0001 fff6            move.w    #$0001,-10(a6)
[0001052a] 486e fff0                 pea.l     -16(a6)
[0001052e] 7008                      moveq.l   #8,d0
[00010530] 3f00                      move.w    d0,-(a7)
[00010532] 7218                      moveq.l   #24,d1
[00010534] 3f01                      move.w    d1,-(a7)
[00010536] 2f0d                      move.l    a5,-(a7)
[00010538] 4eb9 0001 3034            jsr       _Objc_draw
[0001053e] 302e ffee                 move.w    -18(a6),d0
[00010542] 5240                      addq.w    #1,d0
[00010544] d16e fff2                 add.w     d0,-14(a6)
[00010548] 486e fff0                 pea.l     -16(a6)
[0001054c] 7008                      moveq.l   #8,d0
[0001054e] 3f00                      move.w    d0,-(a7)
[00010550] 7218                      moveq.l   #24,d1
[00010552] 3f01                      move.w    d1,-(a7)
[00010554] 2f0d                      move.l    a5,-(a7)
[00010556] 4eb9 0001 3034            jsr       _Objc_draw
[0001055c] 4fef 0036                 lea.l     54(a7),a7
[00010560] 3039 0001 4dbc            move.w    _curobj,d0
[00010566] 721d                      moveq.l   #29,d1
[00010568] b041                      cmp.w     d1,d0
[0001056a] 6606                      bne.s     $00010572
[0001056c] 7220                      moveq.l   #32,d1
[0001056e] be41                      cmp.w     d1,d7
[00010570] 671e                      beq.s     $00010590
[00010572] 720f                      moveq.l   #15,d1
[00010574] ba41                      cmp.w     d1,d5
[00010576] 6706                      beq.s     $0001057E
[00010578] 720e                      moveq.l   #14,d1
[0001057a] ba41                      cmp.w     d1,d5
[0001057c] 6628                      bne.s     $000105A6
[0001057e] 7234                      moveq.l   #52,d1
[00010580] b041                      cmp.w     d1,d0
[00010582] 670c                      beq.s     $00010590
[00010584] 7237                      moveq.l   #55,d1
[00010586] b041                      cmp.w     d1,d0
[00010588] 6706                      beq.s     $00010590
[0001058a] 7236                      moveq.l   #54,d1
[0001058c] b041                      cmp.w     d1,d0
[0001058e] 6616                      bne.s     $000105A6
[00010590] 42a7                      clr.l     -(a7)
[00010592] 7008                      moveq.l   #8,d0
[00010594] 3f00                      move.w    d0,-(a7)
[00010596] 723e                      moveq.l   #62,d1
[00010598] 3f01                      move.w    d1,-(a7)
[0001059a] 2f0d                      move.l    a5,-(a7)
[0001059c] 4eb9 0001 3034            jsr       _Objc_draw
[000105a2] 4fef 000c                 lea.l     12(a7),a7
[000105a6] 4cdf 28e0                 movem.l   (a7)+,d5-d7/a3/a5
[000105aa] 4e5e                      unlk      a6
[000105ac] 4e75                      rts
sl_dragx:
[000105ae] 4e56 ffd8                 link      a6,#$FFD8
[000105b2] 48e7 3f14                 movem.l   d2-d7/a3/a5,-(a7)
[000105b6] 2a6e 0008                 movea.l   8(a6),a5
[000105ba] 3e2e 000c                 move.w    12(a6),d7
[000105be] 3c2e 000e                 move.w    14(a6),d6
[000105c2] 3a2e 0010                 move.w    16(a6),d5
[000105c6] 266e 0014                 movea.l   20(a6),a3
[000105ca] 3013                      move.w    (a3),d0
[000105cc] 2206                      move.l    d6,d1
[000105ce] c3fc 0018                 muls.w    #$0018,d1
[000105d2] 2075 180c                 movea.l   12(a5,d1.l),a0
[000105d6] 43e8 0012                 lea.l     18(a0),a1
[000105da] 7200                      moveq.l   #0,d1
[000105dc] 2411                      move.l    (a1),d2
[000105de] 0282 0fff ffff            andi.l    #$0FFFFFFF,d2
[000105e4] 2282                      move.l    d2,(a1)
[000105e6] 91c8                      suba.l    a0,a0
[000105e8] 2f08                      move.l    a0,-(a7)
[000105ea] 3f2e 0012                 move.w    18(a6),-(a7)
[000105ee] 3f05                      move.w    d5,-(a7)
[000105f0] 3f00                      move.w    d0,-(a7)
[000105f2] 3f06                      move.w    d6,-(a7)
[000105f4] 3f07                      move.w    d7,-(a7)
[000105f6] 2f0d                      move.l    a5,-(a7)
[000105f8] 3d40 fff8                 move.w    d0,-8(a6)
[000105fc] 6100 fe0e                 bsr       _draw_slider
[00010600] 486e ffe8                 pea.l     -24(a6)
[00010604] 3f07                      move.w    d7,-(a7)
[00010606] 2f0d                      move.l    a5,-(a7)
[00010608] 6100 fcf0                 bsr       _getrect
[0001060c] 486e ffe0                 pea.l     -32(a6)
[00010610] 3f06                      move.w    d6,-(a7)
[00010612] 2f0d                      move.l    a5,-(a7)
[00010614] 6100 fce4                 bsr       _getrect
[00010618] 486e fff0                 pea.l     -16(a6)
[0001061c] 4eb9 0001 3098            jsr       _Graf_mkstate
[00010622] 4fef 002a                 lea.l     42(a7),a7
[00010626] 302e fff0                 move.w    -16(a6),d0
[0001062a] 906e ffe0                 sub.w     -32(a6),d0
[0001062e] 5340                      subq.w    #1,d0
[00010630] 3d40 fffc                 move.w    d0,-4(a6)
[00010634] 4a6e fff4                 tst.w     -12(a6)
[00010638] 6700 00d0                 beq       $0001070A
[0001063c] 302e fff0                 move.w    -16(a6),d0
[00010640] 906e fffc                 sub.w     -4(a6),d0
[00010644] 48ae 0001 fffa            movem.w   d0,-6(a6)
[0001064a] 322e ffe8                 move.w    -24(a6),d1
[0001064e] b041                      cmp.w     d1,d0
[00010650] 6e06                      bgt.s     $00010658
[00010652] 3d45 fffe                 move.w    d5,-2(a6)
[00010656] 6074                      bra.s     $000106CC
[00010658] 342e ffec                 move.w    -20(a6),d2
[0001065c] 2602                      move.l    d2,d3
[0001065e] d641                      add.w     d1,d3
[00010660] 382e ffe4                 move.w    -28(a6),d4
[00010664] 9644                      sub.w     d4,d3
[00010666] b043                      cmp.w     d3,d0
[00010668] 6d0a                      blt.s     $00010674
[0001066a] 362e 0012                 move.w    18(a6),d3
[0001066e] 3d43 fffe                 move.w    d3,-2(a6)
[00010672] 6058                      bra.s     $000106CC
[00010674] 362e 0012                 move.w    18(a6),d3
[00010678] 9645                      sub.w     d5,d3
[0001067a] 5243                      addq.w    #1,d3
[0001067c] 48c3                      ext.l     d3
[0001067e] 2003                      move.l    d3,d0
[00010680] 223c 0000 03e8            move.l    #$000003E8,d1
[00010686] 4eb9 0001 34bc            jsr       __CXM33
[0001068c] 322e fffa                 move.w    -6(a6),d1
[00010690] 926e ffe8                 sub.w     -24(a6),d1
[00010694] 48c1                      ext.l     d1
[00010696] 4eb9 0001 34bc            jsr       __CXM33
[0001069c] 9444                      sub.w     d4,d2
[0001069e] 48c2                      ext.l     d2
[000106a0] 2f40 0020                 move.l    d0,32(a7)
[000106a4] 2002                      move.l    d2,d0
[000106a6] 223c 0000 03e8            move.l    #$000003E8,d1
[000106ac] 4eb9 0001 34bc            jsr       __CXM33
[000106b2] 222f 0020                 move.l    32(a7),d1
[000106b6] 2f40 0024                 move.l    d0,36(a7)
[000106ba] 2001                      move.l    d1,d0
[000106bc] 222f 0024                 move.l    36(a7),d1
[000106c0] 4eb9 0001 34dc            jsr       __CXD33
[000106c6] d045                      add.w     d5,d0
[000106c8] 3d40 fffe                 move.w    d0,-2(a6)
[000106cc] 302e fffe                 move.w    -2(a6),d0
[000106d0] 322e fff8                 move.w    -8(a6),d1
[000106d4] b240                      cmp.w     d0,d1
[000106d6] 6722                      beq.s     $000106FA
[000106d8] 3680                      move.w    d0,(a3)
[000106da] 206e 0018                 movea.l   24(a6),a0
[000106de] 2f08                      move.l    a0,-(a7)
[000106e0] 3f2e 0012                 move.w    18(a6),-(a7)
[000106e4] 3f05                      move.w    d5,-(a7)
[000106e6] 3f00                      move.w    d0,-(a7)
[000106e8] 3f06                      move.w    d6,-(a7)
[000106ea] 3f07                      move.w    d7,-(a7)
[000106ec] 2f0d                      move.l    a5,-(a7)
[000106ee] 3d40 fff8                 move.w    d0,-8(a6)
[000106f2] 6100 fd18                 bsr       _draw_slider
[000106f6] 4fef 0012                 lea.l     18(a7),a7
[000106fa] 486e fff0                 pea.l     -16(a6)
[000106fe] 4eb9 0001 3098            jsr       _Graf_mkstate
[00010704] 584f                      addq.w    #4,a7
[00010706] 6000 ff2c                 bra       $00010634
[0001070a] 2006                      move.l    d6,d0
[0001070c] c1fc 0018                 muls.w    #$0018,d0
[00010710] 2075 080c                 movea.l   12(a5,d0.l),a0
[00010714] 43e8 0012                 lea.l     18(a0),a1
[00010718] 203c 1000 0000            move.l    #$10000000,d0
[0001071e] 2211                      move.l    (a1),d1
[00010720] 0281 0fff ffff            andi.l    #$0FFFFFFF,d1
[00010726] 8280                      or.l      d0,d1
[00010728] 2281                      move.l    d1,(a1)
[0001072a] 91c8                      suba.l    a0,a0
[0001072c] 2f08                      move.l    a0,-(a7)
[0001072e] 3f2e 0012                 move.w    18(a6),-(a7)
[00010732] 3f05                      move.w    d5,-(a7)
[00010734] 3f2e fff8                 move.w    -8(a6),-(a7)
[00010738] 3f06                      move.w    d6,-(a7)
[0001073a] 3f07                      move.w    d7,-(a7)
[0001073c] 2f0d                      move.l    a5,-(a7)
[0001073e] 6100 fccc                 bsr       _draw_slider
[00010742] 4cee 28fc ffb8            movem.l   -72(a6),d2-d7/a3/a5
[00010748] 4e5e                      unlk      a6
[0001074a] 4e75                      rts
_sl_pagex:
[0001074c] 4e56 ffe4                 link      a6,#$FFE4
[00010750] 48e7 3714                 movem.l   d2-d3/d5-d7/a3/a5,-(a7)
[00010754] 2a6e 0008                 movea.l   8(a6),a5
[00010758] 3e2e 000c                 move.w    12(a6),d7
[0001075c] 3c2e 000e                 move.w    14(a6),d6
[00010760] 3a2e 0010                 move.w    16(a6),d5
[00010764] 266e 0016                 movea.l   22(a6),a3
[00010768] 486e fff0                 pea.l     -16(a6)
[0001076c] 3f07                      move.w    d7,-(a7)
[0001076e] 2f0d                      move.l    a5,-(a7)
[00010770] 6100 fb88                 bsr       _getrect
[00010774] 486e ffe8                 pea.l     -24(a6)
[00010778] 3f06                      move.w    d6,-(a7)
[0001077a] 2f0d                      move.l    a5,-(a7)
[0001077c] 6100 fb7c                 bsr       _getrect
[00010780] 486e fff8                 pea.l     -8(a6)
[00010784] 4eb9 0001 3098            jsr       _Graf_mkstate
[0001078a] 4fef 0018                 lea.l     24(a7),a7
[0001078e] 302e ffec                 move.w    -20(a6),d0
[00010792] 6a02                      bpl.s     $00010796
[00010794] 5240                      addq.w    #1,d0
[00010796] e240                      asr.w     #1,d0
[00010798] 322e ffe8                 move.w    -24(a6),d1
[0001079c] d240                      add.w     d0,d1
[0001079e] 302e fff8                 move.w    -8(a6),d0
[000107a2] b041                      cmp.w     d1,d0
[000107a4] 6c04                      bge.s     $000107AA
[000107a6] 70ff                      moveq.l   #-1,d0
[000107a8] 6002                      bra.s     $000107AC
[000107aa] 7001                      moveq.l   #1,d0
[000107ac] 2205                      move.l    d5,d1
[000107ae] c3c0                      muls.w    d0,d1
[000107b0] 3413                      move.w    (a3),d2
[000107b2] d441                      add.w     d1,d2
[000107b4] 48ae 0001 ffe4            movem.w   d0,-28(a6)
[000107ba] 48ae 0004 ffe6            movem.w   d2,-26(a6)
[000107c0] 302e 0012                 move.w    18(a6),d0
[000107c4] b440                      cmp.w     d0,d2
[000107c6] 6d3c                      blt.s     $00010804
[000107c8] 322e 0014                 move.w    20(a6),d1
[000107cc] b441                      cmp.w     d1,d2
[000107ce] 6e34                      bgt.s     $00010804
[000107d0] 3682                      move.w    d2,(a3)
[000107d2] 206e 001a                 movea.l   26(a6),a0
[000107d6] 2f08                      move.l    a0,-(a7)
[000107d8] 3f01                      move.w    d1,-(a7)
[000107da] 3f00                      move.w    d0,-(a7)
[000107dc] 3f02                      move.w    d2,-(a7)
[000107de] 3f06                      move.w    d6,-(a7)
[000107e0] 3f07                      move.w    d7,-(a7)
[000107e2] 2f0d                      move.l    a5,-(a7)
[000107e4] 6100 fc26                 bsr       _draw_slider
[000107e8] 486e ffe8                 pea.l     -24(a6)
[000107ec] 3f06                      move.w    d6,-(a7)
[000107ee] 2f0d                      move.l    a5,-(a7)
[000107f0] 6100 fb08                 bsr       _getrect
[000107f4] 2eb9 0001 4dec            move.l    $00014DEC,(a7)
[000107fa] 4eb9 0001 30b4            jsr       Evnt_timer
[00010800] 4fef 001c                 lea.l     28(a7),a7
[00010804] 486e fff8                 pea.l     -8(a6)
[00010808] 4eb9 0001 3098            jsr       _Graf_mkstate
[0001080e] 584f                      addq.w    #4,a7
[00010810] 302e ffec                 move.w    -20(a6),d0
[00010814] 2200                      move.l    d0,d1
[00010816] 4a41                      tst.w     d1
[00010818] 6a02                      bpl.s     $0001081C
[0001081a] 5641                      addq.w    #3,d1
[0001081c] e441                      asr.w     #2,d1
[0001081e] 342e ffe8                 move.w    -24(a6),d2
[00010822] d242                      add.w     d2,d1
[00010824] 362e fff8                 move.w    -8(a6),d3
[00010828] b641                      cmp.w     d1,d3
[0001082a] 6d36                      blt.s     $00010862
[0001082c] d040                      add.w     d0,d0
[0001082e] d06e ffec                 add.w     -20(a6),d0
[00010832] 4a40                      tst.w     d0
[00010834] 6a02                      bpl.s     $00010838
[00010836] 5640                      addq.w    #3,d0
[00010838] e440                      asr.w     #2,d0
[0001083a] d440                      add.w     d0,d2
[0001083c] b642                      cmp.w     d2,d3
[0001083e] 6c22                      bge.s     $00010862
[00010840] 206e 001a                 movea.l   26(a6),a0
[00010844] 2f08                      move.l    a0,-(a7)
[00010846] 2f0b                      move.l    a3,-(a7)
[00010848] 3f2e 0014                 move.w    20(a6),-(a7)
[0001084c] 3f2e 0012                 move.w    18(a6),-(a7)
[00010850] 3f06                      move.w    d6,-(a7)
[00010852] 3f07                      move.w    d7,-(a7)
[00010854] 2f0d                      move.l    a5,-(a7)
[00010856] 6100 fd56                 bsr       _sl_dragx
[0001085a] 4fef 0014                 lea.l     20(a7),a7
[0001085e] 426e fffc                 clr.w     -4(a6)
[00010862] 4a6e fffc                 tst.w     -4(a6)
[00010866] 6726                      beq.s     $0001088E
[00010868] 486e fff0                 pea.l     -16(a6)
[0001086c] 3f2e fffa                 move.w    -6(a6),-(a7)
[00010870] 3f2e fff8                 move.w    -8(a6),-(a7)
[00010874] 4eb9 0001 3164            jsr       _rc_inside
[0001087a] 504f                      addq.w    #8,a7
[0001087c] 4a40                      tst.w     d0
[0001087e] 660e                      bne.s     $0001088E
[00010880] 486e fff8                 pea.l     -8(a6)
[00010884] 4eb9 0001 3098            jsr       _Graf_mkstate
[0001088a] 584f                      addq.w    #4,a7
[0001088c] 60d4                      bra.s     $00010862
[0001088e] 4a6e fffc                 tst.w     -4(a6)
[00010892] 6600 fefa                 bne       $0001078E
[00010896] 4cdf 28ec                 movem.l   (a7)+,d2-d3/d5-d7/a3/a5
[0001089a] 4e5e                      unlk      a6
[0001089c] 4e75                      rts
_draw_fills:
[0001089e] 48e7 0120                 movem.l   d7/a2,-(a7)
[000108a2] 7e28                      moveq.l   #40,d7
[000108a4] 702f                      moveq.l   #47,d0
[000108a6] be40                      cmp.w     d0,d7
[000108a8] 6e60                      bgt.s     $0001090A
[000108aa] 2007                      move.l    d7,d0
[000108ac] c1fc 0018                 muls.w    #$0018,d0
[000108b0] 2079 0001 4dcc            movea.l   _tree,a0
[000108b6] 2248                      movea.l   a0,a1
[000108b8] d3c0                      adda.l    d0,a1
[000108ba] 45e9 000c                 lea.l     12(a1),a2
[000108be] 202a 0002                 move.l    2(a2),d0
[000108c2] 4240                      clr.w     d0
[000108c4] 4840                      swap      d0
[000108c6] e088                      lsr.l     #8,d0
[000108c8] e888                      lsr.l     #4,d0
[000108ca] 0280 0000 000f            andi.l    #$0000000F,d0
[000108d0] 4a80                      tst.l     d0
[000108d2] 6632                      bne.s     $00010906
[000108d4] 45e9 000c                 lea.l     12(a1),a2
[000108d8] 203c 1000 0000            move.l    #$10000000,d0
[000108de] 222a 0002                 move.l    2(a2),d1
[000108e2] 0281 0fff ffff            andi.l    #$0FFFFFFF,d1
[000108e8] 8280                      or.l      d0,d1
[000108ea] 2541 0002                 move.l    d1,2(a2)
[000108ee] 42a7                      clr.l     -(a7)
[000108f0] 7008                      moveq.l   #8,d0
[000108f2] 3f00                      move.w    d0,-(a7)
[000108f4] 3f07                      move.w    d7,-(a7)
[000108f6] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000108fc] 4eb9 0001 3034            jsr       _Objc_draw
[00010902] 4fef 000c                 lea.l     12(a7),a7
[00010906] 5247                      addq.w    #1,d7
[00010908] 609a                      bra.s     $000108A4
[0001090a] 2039 0001 4dc4            move.l    _curcolor,d0
[00010910] 4240                      clr.w     d0
[00010912] 4840                      swap      d0
[00010914] e888                      lsr.l     #4,d0
[00010916] 0280 0000 0007            andi.l    #$00000007,d0
[0001091c] 2e00                      move.l    d0,d7
[0001091e] 0647 0028                 addi.w    #$0028,d7
[00010922] 2007                      move.l    d7,d0
[00010924] c1fc 0018                 muls.w    #$0018,d0
[00010928] 2079 0001 4dcc            movea.l   _tree,a0
[0001092e] 2248                      movea.l   a0,a1
[00010930] d3c0                      adda.l    d0,a1
[00010932] 45e9 000c                 lea.l     12(a1),a2
[00010936] 7000                      moveq.l   #0,d0
[00010938] 222a 0002                 move.l    2(a2),d1
[0001093c] 0281 0fff ffff            andi.l    #$0FFFFFFF,d1
[00010942] 2541 0002                 move.l    d1,2(a2)
[00010946] 42a7                      clr.l     -(a7)
[00010948] 7008                      moveq.l   #8,d0
[0001094a] 3f00                      move.w    d0,-(a7)
[0001094c] 3f07                      move.w    d7,-(a7)
[0001094e] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010954] 4eb9 0001 3034            jsr       _Objc_draw
[0001095a] 4fef 000c                 lea.l     12(a7),a7
[0001095e] 4cdf 0480                 movem.l   (a7)+,d7/a2
[00010962] 4e75                      rts
_draw_curborders:
[00010964] 48e7 2104                 movem.l   d2/d7/a5,-(a7)
[00010968] 2a6f 0010                 movea.l   16(a7),a5
[0001096c] 7eff                      moveq.l   #-1,d7
[0001096e] 3039 0001 4dbc            move.w    _curobj,d0
[00010974] 5140                      subq.w    #8,d0
[00010976] 6d00 011c                 blt       $00010A94
[0001097a] 0c40 0036                 cmpi.w    #$0036,d0
[0001097e] 6c00 0114                 bge       $00010A94
[00010982] d040                      add.w     d0,d0
[00010984] 303b 0006                 move.w    $0001098C(pc,d0.w),d0
[00010988] 4efb 0004                 jmp       $0001098E(pc,d0.w)
J1:
[0001098c] 008e                      dc.w $008e   ; $00010a1c-$0001098c-2
[0001098e] 008e                      dc.w $008e   ; $00010a1c-$0001098c-2
[00010990] 00b4                      dc.w $00b4   ; $00010a42-$0001098c-2
[00010992] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[00010994] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[00010996] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[00010998] 00b8                      dc.w $00b8   ; $00010a46-$0001098c-2
[0001099a] 0092                      dc.w $0092   ; $00010a20-$0001098c-2
[0001099c] 0096                      dc.w $0096   ; $00010a24-$0001098c-2
[0001099e] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109a0] 0092                      dc.w $0092   ; $00010a20-$0001098c-2
[000109a2] 00b8                      dc.w $00b8   ; $00010a46-$0001098c-2
[000109a4] 006a                      dc.w $006a   ; $000109f8-$0001098c-2
[000109a6] 0070                      dc.w $0070   ; $000109fe-$0001098c-2
[000109a8] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109aa] 006a                      dc.w $006a   ; $000109f8-$0001098c-2
[000109ac] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109ae] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109b0] 00bc                      dc.w $00bc   ; $00010a4a-$0001098c-2
[000109b2] 00bc                      dc.w $00bc   ; $00010a4a-$0001098c-2
[000109b4] 00bc                      dc.w $00bc   ; $00010a4a-$0001098c-2
[000109b6] 00c0                      dc.w $00c0   ; $00010a4e-$0001098c-2
[000109b8] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109ba] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109bc] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109be] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109c0] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109c2] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109c4] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109c6] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109c8] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109ca] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109cc] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109ce] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109d0] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109d2] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109d4] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109d6] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109d8] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109da] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109dc] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109de] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109e0] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109e2] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109e4] 00c4                      dc.w $00c4   ; $00010a52-$0001098c-2
[000109e6] 00c8                      dc.w $00c8   ; $00010a56-$0001098c-2
[000109e8] 00c4                      dc.w $00c4   ; $00010a52-$0001098c-2
[000109ea] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109ec] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109ee] 00e6                      dc.w $00e6   ; $00010a74-$0001098c-2
[000109f0] 00ea                      dc.w $00ea   ; $00010a78-$0001098c-2
[000109f2] 00e6                      dc.w $00e6   ; $00010a74-$0001098c-2
[000109f4] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109f6] 0106                      dc.w $0106   ; $00010a94-$0001098c-2
[000109f8] 7e16                      moveq.l   #22,d7
[000109fa] 6000 0098                 bra       $00010A94
[000109fe] 2f0d                      move.l    a5,-(a7)
[00010a00] 7008                      moveq.l   #8,d0
[00010a02] 3f00                      move.w    d0,-(a7)
[00010a04] 7217                      moveq.l   #23,d1
[00010a06] 3f01                      move.w    d1,-(a7)
[00010a08] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010a0e] 4eb9 0001 3034            jsr       _Objc_draw
[00010a14] 4fef 000c                 lea.l     12(a7),a7
[00010a18] 7e0d                      moveq.l   #13,d7
[00010a1a] 6078                      bra.s     $00010A94
[00010a1c] 7e0a                      moveq.l   #10,d7
[00010a1e] 6074                      bra.s     $00010A94
[00010a20] 7e11                      moveq.l   #17,d7
[00010a22] 6070                      bra.s     $00010A94
[00010a24] 2f0d                      move.l    a5,-(a7)
[00010a26] 7008                      moveq.l   #8,d0
[00010a28] 3f00                      move.w    d0,-(a7)
[00010a2a] 7212                      moveq.l   #18,d1
[00010a2c] 3f01                      move.w    d1,-(a7)
[00010a2e] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010a34] 4eb9 0001 3034            jsr       _Objc_draw
[00010a3a] 4fef 000c                 lea.l     12(a7),a7
[00010a3e] 7e0d                      moveq.l   #13,d7
[00010a40] 6052                      bra.s     $00010A94
[00010a42] 7e0e                      moveq.l   #14,d7
[00010a44] 604e                      bra.s     $00010A94
[00010a46] 7e0d                      moveq.l   #13,d7
[00010a48] 604a                      bra.s     $00010A94
[00010a4a] 7e1d                      moveq.l   #29,d7
[00010a4c] 6046                      bra.s     $00010A94
[00010a4e] 7e34                      moveq.l   #52,d7
[00010a50] 6042                      bra.s     $00010A94
[00010a52] 7e37                      moveq.l   #55,d7
[00010a54] 603e                      bra.s     $00010A94
[00010a56] 2f0d                      move.l    a5,-(a7)
[00010a58] 7008                      moveq.l   #8,d0
[00010a5a] 3f00                      move.w    d0,-(a7)
[00010a5c] 7236                      moveq.l   #54,d1
[00010a5e] 3f01                      move.w    d1,-(a7)
[00010a60] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010a66] 4eb9 0001 3034            jsr       _Objc_draw
[00010a6c] 4fef 000c                 lea.l     12(a7),a7
[00010a70] 7e3d                      moveq.l   #61,d7
[00010a72] 6020                      bra.s     $00010A94
[00010a74] 7e3c                      moveq.l   #60,d7
[00010a76] 601c                      bra.s     $00010A94
[00010a78] 2f0d                      move.l    a5,-(a7)
[00010a7a] 7008                      moveq.l   #8,d0
[00010a7c] 3f00                      move.w    d0,-(a7)
[00010a7e] 723b                      moveq.l   #59,d1
[00010a80] 3f01                      move.w    d1,-(a7)
[00010a82] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010a88] 4eb9 0001 3034            jsr       _Objc_draw
[00010a8e] 4fef 000c                 lea.l     12(a7),a7
[00010a92] 7e3d                      moveq.l   #61,d7
[00010a94] 70ff                      moveq.l   #-1,d0
[00010a96] be40                      cmp.w     d0,d7
[00010a98] 6718                      beq.s     $00010AB2
[00010a9a] 2f0d                      move.l    a5,-(a7)
[00010a9c] 7008                      moveq.l   #8,d0
[00010a9e] 3f00                      move.w    d0,-(a7)
[00010aa0] 3f07                      move.w    d7,-(a7)
[00010aa2] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010aa8] 4eb9 0001 3034            jsr       _Objc_draw
[00010aae] 4fef 000c                 lea.l     12(a7),a7
[00010ab2] 703e                      moveq.l   #62,d0
[00010ab4] 2079 0001 4dcc            movea.l   _tree,a0
[00010aba] b068 0090                 cmp.w     144(a0),d0
[00010abe] 6630                      bne.s     $00010AF0
[00010ac0] 3239 0001 4dbc            move.w    _curobj,d1
[00010ac6] 7434                      moveq.l   #52,d2
[00010ac8] b242                      cmp.w     d2,d1
[00010aca] 6738                      beq.s     $00010B04
[00010acc] 743c                      moveq.l   #60,d2
[00010ace] b242                      cmp.w     d2,d1
[00010ad0] 6732                      beq.s     $00010B04
[00010ad2] 743b                      moveq.l   #59,d2
[00010ad4] b242                      cmp.w     d2,d1
[00010ad6] 672c                      beq.s     $00010B04
[00010ad8] 743a                      moveq.l   #58,d2
[00010ada] b242                      cmp.w     d2,d1
[00010adc] 6726                      beq.s     $00010B04
[00010ade] 743d                      moveq.l   #61,d2
[00010ae0] b242                      cmp.w     d2,d1
[00010ae2] 6720                      beq.s     $00010B04
[00010ae4] 741b                      moveq.l   #27,d2
[00010ae6] b242                      cmp.w     d2,d1
[00010ae8] 671a                      beq.s     $00010B04
[00010aea] 741d                      moveq.l   #29,d2
[00010aec] b242                      cmp.w     d2,d1
[00010aee] 6714                      beq.s     $00010B04
[00010af0] 2f0d                      move.l    a5,-(a7)
[00010af2] 7208                      moveq.l   #8,d1
[00010af4] 3f01                      move.w    d1,-(a7)
[00010af6] 3f00                      move.w    d0,-(a7)
[00010af8] 2f08                      move.l    a0,-(a7)
[00010afa] 4eb9 0001 3034            jsr       _Objc_draw
[00010b00] 4fef 000c                 lea.l     12(a7),a7
[00010b04] 4cdf 2084                 movem.l   (a7)+,d2/d7/a5
[00010b08] 4e75                      rts
_draw_curobj:
[00010b0a] 4e56 ffdc                 link      a6,#$FFDC
[00010b0e] 48e7 2120                 movem.l   d2/d7/a2,-(a7)
[00010b12] 4a79 0001 4dc8            tst.w     _curIsTed
[00010b18] 6752                      beq.s     $00010B6C
[00010b1a] 3039 0001 4dbc            move.w    _curobj,d0
[00010b20] c1fc 0018                 muls.w    #$0018,d0
[00010b24] 2079 0001 4dcc            movea.l   _tree,a0
[00010b2a] d1c0                      adda.l    d0,a0
[00010b2c] 2268 000c                 movea.l   12(a0),a1
[00010b30] 41e9 0012                 lea.l     18(a1),a0
[00010b34] 2f08                      move.l    a0,-(a7)
[00010b36] 4879 0001 4dc4            pea.l     _curcolor
[00010b3c] 4eb9 0001 2e38            jsr       _FColor
[00010b42] 504f                      addq.w    #8,a7
[00010b44] 3039 0001 4dbc            move.w    _curobj,d0
[00010b4a] 2200                      move.l    d0,d1
[00010b4c] 7418                      moveq.l   #24,d2
[00010b4e] c3c2                      muls.w    d2,d1
[00010b50] 2079 0001 4dcc            movea.l   _tree,a0
[00010b56] 2248                      movea.l   a0,a1
[00010b58] d3c1                      adda.l    d1,a1
[00010b5a] 2469 000c                 movea.l   12(a1),a2
[00010b5e] 322a 0016                 move.w    22(a2),d1
[00010b62] 4a41                      tst.w     d1
[00010b64] 6a5c                      bpl.s     $00010BC2
[00010b66] 3e2a 0016                 move.w    22(a2),d7
[00010b6a] 6056                      bra.s     $00010BC2
[00010b6c] 3039 0001 4dbc            move.w    _curobj,d0
[00010b72] c1fc 0018                 muls.w    #$0018,d0
[00010b76] 2079 0001 4dcc            movea.l   _tree,a0
[00010b7c] d1c0                      adda.l    d0,a0
[00010b7e] 43e8 000c                 lea.l     12(a0),a1
[00010b82] 41e9 0002                 lea.l     2(a1),a0
[00010b86] 2f08                      move.l    a0,-(a7)
[00010b88] 4879 0001 4dc4            pea.l     _curcolor
[00010b8e] 4eb9 0001 2e38            jsr       _FColor
[00010b94] 504f                      addq.w    #8,a7
[00010b96] 3039 0001 4dbc            move.w    _curobj,d0
[00010b9c] 2200                      move.l    d0,d1
[00010b9e] c3fc 0018                 muls.w    #$0018,d1
[00010ba2] 2079 0001 4dcc            movea.l   _tree,a0
[00010ba8] 2248                      movea.l   a0,a1
[00010baa] d3c1                      adda.l    d1,a1
[00010bac] 45e9 000c                 lea.l     12(a1),a2
[00010bb0] 122a 0001                 move.b    1(a2),d1
[00010bb4] 4a01                      tst.b     d1
[00010bb6] 6a0a                      bpl.s     $00010BC2
[00010bb8] 45e9 000c                 lea.l     12(a1),a2
[00010bbc] 1e2a 0001                 move.b    1(a2),d7
[00010bc0] 4887                      ext.w     d7
[00010bc2] 486e fff8                 pea.l     -8(a6)
[00010bc6] 3f00                      move.w    d0,-(a7)
[00010bc8] 2f08                      move.l    a0,-(a7)
[00010bca] 6100 f72e                 bsr       _getrect
[00010bce] 41ee fff8                 lea.l     -8(a6),a0
[00010bd2] 2e88                      move.l    a0,(a7)
[00010bd4] 3f07                      move.w    d7,-(a7)
[00010bd6] 3f07                      move.w    d7,-(a7)
[00010bd8] 2f08                      move.l    a0,-(a7)
[00010bda] 4eb9 0001 2f02            jsr       _Rc_center
[00010be0] 4fef 0012                 lea.l     18(a7),a7
[00010be4] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00010bec] 6d1a                      blt.s     $00010C08
[00010bee] 0c79 0008 0001 4de8       cmpi.w    #$0008,$00014DE8
[00010bf6] 6f10                      ble.s     $00010C08
[00010bf8] 556e fff8                 subq.w    #2,-8(a6)
[00010bfc] 556e fffa                 subq.w    #2,-6(a6)
[00010c00] 586e fffc                 addq.w    #4,-4(a6)
[00010c04] 586e fffe                 addq.w    #4,-2(a6)
[00010c08] 3039 0001 4dbc            move.w    _curobj,d0
[00010c0e] 7213                      moveq.l   #19,d1
[00010c10] b041                      cmp.w     d1,d0
[00010c12] 6714                      beq.s     $00010C28
[00010c14] 7214                      moveq.l   #20,d1
[00010c16] b041                      cmp.w     d1,d0
[00010c18] 670e                      beq.s     $00010C28
[00010c1a] 7216                      moveq.l   #22,d1
[00010c1c] b041                      cmp.w     d1,d0
[00010c1e] 6708                      beq.s     $00010C28
[00010c20] 7217                      moveq.l   #23,d1
[00010c22] b041                      cmp.w     d1,d0
[00010c24] 6600 008a                 bne       $00010CB0
[00010c28] 41ee fff8                 lea.l     -8(a6),a0
[00010c2c] 43ee ffe8                 lea.l     -24(a6),a1
[00010c30] 2449                      movea.l   a1,a2
[00010c32] 24d8                      move.l    (a0)+,(a2)+
[00010c34] 24d8                      move.l    (a0)+,(a2)+
[00010c36] 41ee fff0                 lea.l     -16(a6),a0
[00010c3a] 20d9                      move.l    (a1)+,(a0)+
[00010c3c] 20d9                      move.l    (a1)+,(a0)+
[00010c3e] 486e ffe0                 pea.l     -32(a6)
[00010c42] 7018                      moveq.l   #24,d0
[00010c44] 3f00                      move.w    d0,-(a7)
[00010c46] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010c4c] 6100 f6ac                 bsr       _getrect
[00010c50] 486e ffe8                 pea.l     -24(a6)
[00010c54] 486e ffe0                 pea.l     -32(a6)
[00010c58] 4eb9 0001 2e44            jsr       _rc_intersect
[00010c5e] 302e ffee                 move.w    -18(a6),d0
[00010c62] 2200                      move.l    d0,d1
[00010c64] 5641                      addq.w    #3,d1
[00010c66] d36e fff2                 add.w     d1,-14(a6)
[00010c6a] 5640                      addq.w    #3,d0
[00010c6c] 916e fff6                 sub.w     d0,-10(a6)
[00010c70] 302e ffec                 move.w    -20(a6),d0
[00010c74] 2200                      move.l    d0,d1
[00010c76] 5641                      addq.w    #3,d1
[00010c78] d36e fff8                 add.w     d1,-8(a6)
[00010c7c] 5640                      addq.w    #3,d0
[00010c7e] 916e fffc                 sub.w     d0,-4(a6)
[00010c82] 302e fff6                 move.w    -10(a6),d0
[00010c86] 916e fffe                 sub.w     d0,-2(a6)
[00010c8a] 486e fff0                 pea.l     -16(a6)
[00010c8e] 7008                      moveq.l   #8,d0
[00010c90] 3f00                      move.w    d0,-(a7)
[00010c92] 3f39 0001 4dbc            move.w    _curobj,-(a7)
[00010c98] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010c9e] 4eb9 0001 3034            jsr       _Objc_draw
[00010ca4] 486e fff0                 pea.l     -16(a6)
[00010ca8] 6100 fcba                 bsr       _draw_curborders
[00010cac] 6000 0148                 bra       $00010DF6
[00010cb0] 7208                      moveq.l   #8,d1
[00010cb2] b041                      cmp.w     d1,d0
[00010cb4] 6706                      beq.s     $00010CBC
[00010cb6] 720a                      moveq.l   #10,d1
[00010cb8] b041                      cmp.w     d1,d0
[00010cba] 6646                      bne.s     $00010D02
[00010cbc] 41ee fff8                 lea.l     -8(a6),a0
[00010cc0] 43ee ffe8                 lea.l     -24(a6),a1
[00010cc4] 22d8                      move.l    (a0)+,(a1)+
[00010cc6] 22d8                      move.l    (a0)+,(a1)+
[00010cc8] 486e ffe0                 pea.l     -32(a6)
[00010ccc] 7018                      moveq.l   #24,d0
[00010cce] 3f00                      move.w    d0,-(a7)
[00010cd0] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010cd6] 6100 f622                 bsr       _getrect
[00010cda] 486e ffe8                 pea.l     -24(a6)
[00010cde] 486e ffe0                 pea.l     -32(a6)
[00010ce2] 4eb9 0001 2e44            jsr       _rc_intersect
[00010ce8] 4fef 0012                 lea.l     18(a7),a7
[00010cec] 302e ffec                 move.w    -20(a6),d0
[00010cf0] 2200                      move.l    d0,d1
[00010cf2] 5641                      addq.w    #3,d1
[00010cf4] d36e fff8                 add.w     d1,-8(a6)
[00010cf8] 5640                      addq.w    #3,d0
[00010cfa] 916e fffc                 sub.w     d0,-4(a6)
[00010cfe] 6000 00d4                 bra       $00010DD4
[00010d02] 7215                      moveq.l   #21,d1
[00010d04] b041                      cmp.w     d1,d0
[00010d06] 6600 00cc                 bne       $00010DD4
[00010d0a] 41ee fff8                 lea.l     -8(a6),a0
[00010d0e] 43ee ffe8                 lea.l     -24(a6),a1
[00010d12] 2449                      movea.l   a1,a2
[00010d14] 24d8                      move.l    (a0)+,(a2)+
[00010d16] 24d8                      move.l    (a0)+,(a2)+
[00010d18] 41ee fff0                 lea.l     -16(a6),a0
[00010d1c] 20d9                      move.l    (a1)+,(a0)+
[00010d1e] 20d9                      move.l    (a1)+,(a0)+
[00010d20] 486e ffe0                 pea.l     -32(a6)
[00010d24] 7018                      moveq.l   #24,d0
[00010d26] 3f00                      move.w    d0,-(a7)
[00010d28] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010d2e] 6100 f5ca                 bsr       _getrect
[00010d32] 302e ffe2                 move.w    -30(a6),d0
[00010d36] d06e ffe6                 add.w     -26(a6),d0
[00010d3a] 5640                      addq.w    #3,d0
[00010d3c] 3d40 fff2                 move.w    d0,-14(a6)
[00010d40] 486e fff0                 pea.l     -16(a6)
[00010d44] 7008                      moveq.l   #8,d0
[00010d46] 3f00                      move.w    d0,-(a7)
[00010d48] 3f39 0001 4dbc            move.w    _curobj,-(a7)
[00010d4e] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010d54] 4eb9 0001 3034            jsr       _Objc_draw
[00010d5a] 486e fff0                 pea.l     -16(a6)
[00010d5e] 6100 fc04                 bsr       _draw_curborders
[00010d62] 41ee fff8                 lea.l     -8(a6),a0
[00010d66] 43ee ffe8                 lea.l     -24(a6),a1
[00010d6a] 2449                      movea.l   a1,a2
[00010d6c] 24d8                      move.l    (a0)+,(a2)+
[00010d6e] 24d8                      move.l    (a0)+,(a2)+
[00010d70] 41ee fff0                 lea.l     -16(a6),a0
[00010d74] 20d9                      move.l    (a1)+,(a0)+
[00010d76] 20d9                      move.l    (a1)+,(a0)+
[00010d78] 486e ffe0                 pea.l     -32(a6)
[00010d7c] 7018                      moveq.l   #24,d0
[00010d7e] 3f00                      move.w    d0,-(a7)
[00010d80] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010d86] 6100 f572                 bsr       _getrect
[00010d8a] 302e ffe0                 move.w    -32(a6),d0
[00010d8e] d06e ffe4                 add.w     -28(a6),d0
[00010d92] 5640                      addq.w    #3,d0
[00010d94] 3d40 fff0                 move.w    d0,-16(a6)
[00010d98] 486e fff0                 pea.l     -16(a6)
[00010d9c] 7008                      moveq.l   #8,d0
[00010d9e] 3f00                      move.w    d0,-(a7)
[00010da0] 3f39 0001 4dbc            move.w    _curobj,-(a7)
[00010da6] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010dac] 4eb9 0001 3034            jsr       _Objc_draw
[00010db2] 486e fff0                 pea.l     -16(a6)
[00010db6] 6100 fbac                 bsr       _draw_curborders
[00010dba] 486e fff8                 pea.l     -8(a6)
[00010dbe] 7008                      moveq.l   #8,d0
[00010dc0] 3f00                      move.w    d0,-(a7)
[00010dc2] 7218                      moveq.l   #24,d1
[00010dc4] 3f01                      move.w    d1,-(a7)
[00010dc6] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010dcc] 4eb9 0001 3034            jsr       _Objc_draw
[00010dd2] 6022                      bra.s     $00010DF6
[00010dd4] 486e fff8                 pea.l     -8(a6)
[00010dd8] 7008                      moveq.l   #8,d0
[00010dda] 3f00                      move.w    d0,-(a7)
[00010ddc] 3f39 0001 4dbc            move.w    _curobj,-(a7)
[00010de2] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00010de8] 4eb9 0001 3034            jsr       _Objc_draw
[00010dee] 486e fff8                 pea.l     -8(a6)
[00010df2] 6100 fb70                 bsr       _draw_curborders
[00010df6] 4cee 0484 ffd0            movem.l   -48(a6),d2/d7/a2
[00010dfc] 4e5e                      unlk      a6
[00010dfe] 4e75                      rts
_fix_elev:
[00010e00] 48e7 2320                 movem.l   d2/d6-d7/a2,-(a7)
[00010e04] 3e2f 0014                 move.w    20(a7),d7
[00010e08] 3c2f 0016                 move.w    22(a7),d6
[00010e0c] 2007                      move.l    d7,d0
[00010e0e] c1fc 0018                 muls.w    #$0018,d0
[00010e12] 2079 0001 4dcc            movea.l   _tree,a0
[00010e18] d1c0                      adda.l    d0,a0
[00010e1a] 2268 000c                 movea.l   12(a0),a1
[00010e1e] 2f11                      move.l    (a1),-(a7)
[00010e20] 3f06                      move.w    d6,-(a7)
[00010e22] 6100 f49e                 bsr       _itoa2
[00010e26] 5c4f                      addq.w    #6,a7
[00010e28] 4a46                      tst.w     d6
[00010e2a] 662c                      bne.s     $00010E58
[00010e2c] 2007                      move.l    d7,d0
[00010e2e] 7218                      moveq.l   #24,d1
[00010e30] c1c1                      muls.w    d1,d0
[00010e32] 2079 0001 4dcc            movea.l   _tree,a0
[00010e38] 2248                      movea.l   a0,a1
[00010e3a] d3c0                      adda.l    d0,a1
[00010e3c] 2469 000c                 movea.l   12(a1),a2
[00010e40] 43ea 0012                 lea.l     18(a2),a1
[00010e44] 203c 0100 0000            move.l    #$01000000,d0
[00010e4a] 2411                      move.l    (a1),d2
[00010e4c] 0282 f0ff ffff            andi.l    #$F0FFFFFF,d2
[00010e52] 8480                      or.l      d0,d2
[00010e54] 2282                      move.l    d2,(a1)
[00010e56] 6024                      bra.s     $00010E7C
[00010e58] 2007                      move.l    d7,d0
[00010e5a] 7218                      moveq.l   #24,d1
[00010e5c] c1c1                      muls.w    d1,d0
[00010e5e] 2079 0001 4dcc            movea.l   _tree,a0
[00010e64] 2248                      movea.l   a0,a1
[00010e66] d3c0                      adda.l    d0,a1
[00010e68] 2469 000c                 movea.l   12(a1),a2
[00010e6c] 43ea 0012                 lea.l     18(a2),a1
[00010e70] 7000                      moveq.l   #0,d0
[00010e72] 2411                      move.l    (a1),d2
[00010e74] 0282 f0ff ffff            andi.l    #$F0FFFFFF,d2
[00010e7a] 2282                      move.l    d2,(a1)
[00010e7c] 2007                      move.l    d7,d0
[00010e7e] c1c1                      muls.w    d1,d0
[00010e80] 2079 0001 4dcc            movea.l   _tree,a0
[00010e86] d1c0                      adda.l    d0,a0
[00010e88] 2268 000c                 movea.l   12(a0),a1
[00010e8c] 41e9 0012                 lea.l     18(a1),a0
[00010e90] 2006                      move.l    d6,d0
[00010e92] 48c0                      ext.l     d0
[00010e94] 4840                      swap      d0
[00010e96] 4240                      clr.w     d0
[00010e98] 0280 000f 0000            andi.l    #$000F0000,d0
[00010e9e] 2210                      move.l    (a0),d1
[00010ea0] 0281 fff0 ffff            andi.l    #$FFF0FFFF,d1
[00010ea6] 8280                      or.l      d0,d1
[00010ea8] 2081                      move.l    d1,(a0)
[00010eaa] 4cdf 04c4                 movem.l   (a7)+,d2/d6-d7/a2
[00010eae] 4e75                      rts
_do_bordslid:
[00010eb0] 3039 0001 4dbe            move.w    _curcborder,d0
[00010eb6] 48c0                      ext.l     d0
[00010eb8] 4840                      swap      d0
[00010eba] 4240                      clr.w     d0
[00010ebc] e188                      lsl.l     #8,d0
[00010ebe] e988                      lsl.l     #4,d0
[00010ec0] 0280 f000 0000            andi.l    #$F0000000,d0
[00010ec6] 2239 0001 4dc4            move.l    _curcolor,d1
[00010ecc] 0281 0fff ffff            andi.l    #$0FFFFFFF,d1
[00010ed2] 8280                      or.l      d0,d1
[00010ed4] 23c1 0001 4dc4            move.l    d1,_curcolor
[00010eda] 3f39 0001 4dbe            move.w    _curcborder,-(a7)
[00010ee0] 7021                      moveq.l   #33,d0
[00010ee2] 3f00                      move.w    d0,-(a7)
[00010ee4] 6100 ff1a                 bsr       _fix_elev
[00010ee8] 6100 fc20                 bsr       _draw_curobj
[00010eec] 584f                      addq.w    #4,a7
[00010eee] 4e75                      rts
_do_textslid:
[00010ef0] 3039 0001 4dc0            move.w    _curctext,d0
[00010ef6] 48c0                      ext.l     d0
[00010ef8] 4840                      swap      d0
[00010efa] 4240                      clr.w     d0
[00010efc] e188                      lsl.l     #8,d0
[00010efe] 0280 0f00 0000            andi.l    #$0F000000,d0
[00010f04] 2239 0001 4dc4            move.l    _curcolor,d1
[00010f0a] 0281 f0ff ffff            andi.l    #$F0FFFFFF,d1
[00010f10] 8280                      or.l      d0,d1
[00010f12] 23c1 0001 4dc4            move.l    d1,_curcolor
[00010f18] 3f39 0001 4dc0            move.w    _curctext,-(a7)
[00010f1e] 7023                      moveq.l   #35,d0
[00010f20] 3f00                      move.w    d0,-(a7)
[00010f22] 6100 fedc                 bsr       _fix_elev
[00010f26] 6100 fbe2                 bsr       _draw_curobj
[00010f2a] 584f                      addq.w    #4,a7
[00010f2c] 4e75                      rts
_do_fillslid:
[00010f2e] 3039 0001 4dc2            move.w    $00014DC2,d0
[00010f34] 48c0                      ext.l     d0
[00010f36] 4840                      swap      d0
[00010f38] 4240                      clr.w     d0
[00010f3a] 0280 000f 0000            andi.l    #$000F0000,d0
[00010f40] 2239 0001 4dc4            move.l    _curcolor,d1
[00010f46] 0281 fff0 ffff            andi.l    #$FFF0FFFF,d1
[00010f4c] 8280                      or.l      d0,d1
[00010f4e] 23c1 0001 4dc4            move.l    d1,_curcolor
[00010f54] 3f39 0001 4dc2            move.w    $00014DC2,-(a7)
[00010f5a] 7025                      moveq.l   #37,d0
[00010f5c] 3f00                      move.w    d0,-(a7)
[00010f5e] 6100 fea0                 bsr       _fix_elev
[00010f62] 6100 fba6                 bsr       _draw_curobj
[00010f66] 584f                      addq.w    #4,a7
[00010f68] 4e75                      rts
_set_info:
[00010f6a] 2f02                      move.l    d2,-(a7)
[00010f6c] 4a79 0001 4dc8            tst.w     _curIsTed
[00010f72] 672c                      beq.s     $00010FA0
[00010f74] 3039 0001 4dbc            move.w    _curobj,d0
[00010f7a] c1fc 0018                 muls.w    #$0018,d0
[00010f7e] 2079 0001 4dcc            movea.l   _tree,a0
[00010f84] d1c0                      adda.l    d0,a0
[00010f86] 2268 000c                 movea.l   12(a0),a1
[00010f8a] 41e9 0012                 lea.l     18(a1),a0
[00010f8e] 3f10                      move.w    (a0),-(a7)
[00010f90] 4879 0001 4dc4            pea.l     _curcolor
[00010f96] 4eb9 0001 2e2c            jsr       ToColor
[00010f9c] 5c4f                      addq.w    #6,a7
[00010f9e] 6022                      bra.s     $00010FC2
[00010fa0] 3039 0001 4dbc            move.w    _curobj,d0
[00010fa6] c1fc 0018                 muls.w    #$0018,d0
[00010faa] 2079 0001 4dcc            movea.l   _tree,a0
[00010fb0] d1c0                      adda.l    d0,a0
[00010fb2] 43e8 000c                 lea.l     12(a0),a1
[00010fb6] 41e9 0002                 lea.l     2(a1),a0
[00010fba] 43f9 0001 4dc4            lea.l     _curcolor,a1
[00010fc0] 22d8                      move.l    (a0)+,(a1)+
[00010fc2] 6100 f8da                 bsr       _draw_fills
[00010fc6] 2279 0001 4dcc            movea.l   _tree,a1
[00010fcc] 2069 039c                 movea.l   924(a1),a0
[00010fd0] 43e8 0012                 lea.l     18(a0),a1
[00010fd4] 2039 0001 4dc4            move.l    _curcolor,d0
[00010fda] 4240                      clr.w     d0
[00010fdc] 4840                      swap      d0
[00010fde] ee88                      lsr.l     #7,d0
[00010fe0] 0280 0000 0001            andi.l    #$00000001,d0
[00010fe6] 4840                      swap      d0
[00010fe8] 4240                      clr.w     d0
[00010fea] ef88                      lsl.l     #7,d0
[00010fec] 0280 0080 0000            andi.l    #$00800000,d0
[00010ff2] 2211                      move.l    (a1),d1
[00010ff4] 0281 ff7f ffff            andi.l    #$FF7FFFFF,d1
[00010ffa] 8280                      or.l      d0,d1
[00010ffc] 2281                      move.l    d1,(a1)
[00010ffe] 42a7                      clr.l     -(a7)
[00011000] 7008                      moveq.l   #8,d0
[00011002] 3f00                      move.w    d0,-(a7)
[00011004] 7226                      moveq.l   #38,d1
[00011006] 3f01                      move.w    d1,-(a7)
[00011008] 2f39 0001 4dcc            move.l    _tree,-(a7)
[0001100e] 4eb9 0001 3034            jsr       _Objc_draw
[00011014] 2039 0001 4dc4            move.l    _curcolor,d0
[0001101a] 4240                      clr.w     d0
[0001101c] 4840                      swap      d0
[0001101e] e088                      lsr.l     #8,d0
[00011020] e888                      lsr.l     #4,d0
[00011022] 0280 0000 000f            andi.l    #$0000000F,d0
[00011028] 33c0 0001 4dbe            move.w    d0,_curcborder
[0001102e] 3e80                      move.w    d0,(a7)
[00011030] 7221                      moveq.l   #33,d1
[00011032] 3f01                      move.w    d1,-(a7)
[00011034] 6100 fdca                 bsr       _fix_elev
[00011038] 91c8                      suba.l    a0,a0
[0001103a] 2e88                      move.l    a0,(a7)
[0001103c] 700f                      moveq.l   #15,d0
[0001103e] 3f00                      move.w    d0,-(a7)
[00011040] 4267                      clr.w     -(a7)
[00011042] 3f39 0001 4dbe            move.w    _curcborder,-(a7)
[00011048] 7221                      moveq.l   #33,d1
[0001104a] 3f01                      move.w    d1,-(a7)
[0001104c] 7420                      moveq.l   #32,d2
[0001104e] 3f02                      move.w    d2,-(a7)
[00011050] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00011056] 6100 f3b4                 bsr       _draw_slider
[0001105a] 2039 0001 4dc4            move.l    _curcolor,d0
[00011060] 4240                      clr.w     d0
[00011062] 4840                      swap      d0
[00011064] e088                      lsr.l     #8,d0
[00011066] 0280 0000 000f            andi.l    #$0000000F,d0
[0001106c] 33c0 0001 4dc0            move.w    d0,_curctext
[00011072] 3e80                      move.w    d0,(a7)
[00011074] 7223                      moveq.l   #35,d1
[00011076] 3f01                      move.w    d1,-(a7)
[00011078] 6100 fd86                 bsr       _fix_elev
[0001107c] 91c8                      suba.l    a0,a0
[0001107e] 2e88                      move.l    a0,(a7)
[00011080] 700f                      moveq.l   #15,d0
[00011082] 3f00                      move.w    d0,-(a7)
[00011084] 4267                      clr.w     -(a7)
[00011086] 3f39 0001 4dc0            move.w    _curctext,-(a7)
[0001108c] 7223                      moveq.l   #35,d1
[0001108e] 3f01                      move.w    d1,-(a7)
[00011090] 7422                      moveq.l   #34,d2
[00011092] 3f02                      move.w    d2,-(a7)
[00011094] 2f39 0001 4dcc            move.l    _tree,-(a7)
[0001109a] 6100 f370                 bsr       _draw_slider
[0001109e] 2039 0001 4dc4            move.l    _curcolor,d0
[000110a4] 4240                      clr.w     d0
[000110a6] 4840                      swap      d0
[000110a8] 0280 0000 000f            andi.l    #$0000000F,d0
[000110ae] 33c0 0001 4dc2            move.w    d0,$00014DC2
[000110b4] 3e80                      move.w    d0,(a7)
[000110b6] 7225                      moveq.l   #37,d1
[000110b8] 3f01                      move.w    d1,-(a7)
[000110ba] 6100 fd44                 bsr       _fix_elev
[000110be] 91c8                      suba.l    a0,a0
[000110c0] 2e88                      move.l    a0,(a7)
[000110c2] 700f                      moveq.l   #15,d0
[000110c4] 3f00                      move.w    d0,-(a7)
[000110c6] 4267                      clr.w     -(a7)
[000110c8] 3f39 0001 4dc2            move.w    $00014DC2,-(a7)
[000110ce] 7225                      moveq.l   #37,d1
[000110d0] 3f01                      move.w    d1,-(a7)
[000110d2] 7424                      moveq.l   #36,d2
[000110d4] 3f02                      move.w    d2,-(a7)
[000110d6] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000110dc] 6100 f32e                 bsr       _draw_slider
[000110e0] 4fef 003c                 lea.l     60(a7),a7
[000110e4] 241f                      move.l    (a7)+,d2
[000110e6] 4e75                      rts
_check_tcolor:
[000110e8] 4e56 fff8                 link      a6,#$FFF8
[000110ec] 2f07                      move.l    d7,-(a7)
[000110ee] 3e2e 0008                 move.w    8(a6),d7
[000110f2] 3f07                      move.w    d7,-(a7)
[000110f4] 486e fffc                 pea.l     -4(a6)
[000110f8] 4eb9 0001 2e2c            jsr       ToColor
[000110fe] 5c4f                      addq.w    #6,a7
[00011100] 202e fffc                 move.l    -4(a6),d0
[00011104] 4240                      clr.w     d0
[00011106] 4840                      swap      d0
[00011108] 0280 0000 000f            andi.l    #$0000000F,d0
[0001110e] 3239 0001 4de8            move.w    $00014DE8,d1
[00011114] 48c1                      ext.l     d1
[00011116] b081                      cmp.l     d1,d0
[00011118] 6514                      bcs.s     $0001112E
[0001111a] 202e fffc                 move.l    -4(a6),d0
[0001111e] 4240                      clr.w     d0
[00011120] 4840                      swap      d0
[00011122] e088                      lsr.l     #8,d0
[00011124] 0280 0000 000f            andi.l    #$0000000F,d0
[0001112a] 5380                      subq.l    #1,d0
[0001112c] 6764                      beq.s     $00011192
[0001112e] 202e fffc                 move.l    -4(a6),d0
[00011132] 4240                      clr.w     d0
[00011134] 4840                      swap      d0
[00011136] e088                      lsr.l     #8,d0
[00011138] 0280 0000 000f            andi.l    #$0000000F,d0
[0001113e] 3239 0001 4de8            move.w    $00014DE8,d1
[00011144] 48c1                      ext.l     d1
[00011146] b081                      cmp.l     d1,d0
[00011148] 6512                      bcs.s     $0001115C
[0001114a] 202e fffc                 move.l    -4(a6),d0
[0001114e] 4240                      clr.w     d0
[00011150] 4840                      swap      d0
[00011152] 0280 0000 000f            andi.l    #$0000000F,d0
[00011158] 5380                      subq.l    #1,d0
[0001115a] 6736                      beq.s     $00011192
[0001115c] 202e fffc                 move.l    -4(a6),d0
[00011160] 4240                      clr.w     d0
[00011162] 4840                      swap      d0
[00011164] e088                      lsr.l     #8,d0
[00011166] 0280 0000 000f            andi.l    #$0000000F,d0
[0001116c] 3239 0001 4de8            move.w    $00014DE8,d1
[00011172] 48c1                      ext.l     d1
[00011174] b081                      cmp.l     d1,d0
[00011176] 652a                      bcs.s     $000111A2
[00011178] 202e fffc                 move.l    -4(a6),d0
[0001117c] 4240                      clr.w     d0
[0001117e] 4840                      swap      d0
[00011180] 0280 0000 000f            andi.l    #$0000000F,d0
[00011186] 3239 0001 4de8            move.w    $00014DE8,d1
[0001118c] 48c1                      ext.l     d1
[0001118e] b081                      cmp.l     d1,d0
[00011190] 6510                      bcs.s     $000111A2
[00011192] 7000                      moveq.l   #0,d0
[00011194] 222e fffc                 move.l    -4(a6),d1
[00011198] 0281 f0ff ffff            andi.l    #$F0FFFFFF,d1
[0001119e] 2d41 fffc                 move.l    d1,-4(a6)
[000111a2] 486e fffa                 pea.l     -6(a6)
[000111a6] 486e fffc                 pea.l     -4(a6)
[000111aa] 4eb9 0001 2e38            jsr       _FColor
[000111b0] 302e fffa                 move.w    -6(a6),d0
[000111b4] 2e2e fff4                 move.l    -12(a6),d7
[000111b8] 4e5e                      unlk      a6
[000111ba] 4e75                      rts
_check_elcolor:
[000111bc] 4e56 fff4                 link      a6,#$FFF4
[000111c0] 48e7 0300                 movem.l   d6-d7,-(a7)
[000111c4] 3e2e 0008                 move.w    8(a6),d7
[000111c8] 3c2e 000a                 move.w    10(a6),d6
[000111cc] 3f07                      move.w    d7,-(a7)
[000111ce] 486e fffc                 pea.l     -4(a6)
[000111d2] 4eb9 0001 2e2c            jsr       ToColor
[000111d8] 3e86                      move.w    d6,(a7)
[000111da] 486e fff8                 pea.l     -8(a6)
[000111de] 4eb9 0001 2e2c            jsr       ToColor
[000111e4] 4fef 000a                 lea.l     10(a7),a7
[000111e8] 202e fff8                 move.l    -8(a6),d0
[000111ec] 4240                      clr.w     d0
[000111ee] 4840                      swap      d0
[000111f0] 0280 0000 000f            andi.l    #$0000000F,d0
[000111f6] 6700 0090                 beq       $00011288
[000111fa] 3239 0001 4de8            move.w    $00014DE8,d1
[00011200] 48c1                      ext.l     d1
[00011202] b081                      cmp.l     d1,d0
[00011204] 6512                      bcs.s     $00011218
[00011206] 202e fffc                 move.l    -4(a6),d0
[0001120a] 4240                      clr.w     d0
[0001120c] 4840                      swap      d0
[0001120e] 0280 0000 000f            andi.l    #$0000000F,d0
[00011214] 5380                      subq.l    #1,d0
[00011216] 6760                      beq.s     $00011278
[00011218] 202e fffc                 move.l    -4(a6),d0
[0001121c] 4240                      clr.w     d0
[0001121e] 4840                      swap      d0
[00011220] 0280 0000 000f            andi.l    #$0000000F,d0
[00011226] 3239 0001 4de8            move.w    $00014DE8,d1
[0001122c] 48c1                      ext.l     d1
[0001122e] b081                      cmp.l     d1,d0
[00011230] 6512                      bcs.s     $00011244
[00011232] 202e fff8                 move.l    -8(a6),d0
[00011236] 4240                      clr.w     d0
[00011238] 4840                      swap      d0
[0001123a] 0280 0000 000f            andi.l    #$0000000F,d0
[00011240] 5380                      subq.l    #1,d0
[00011242] 6734                      beq.s     $00011278
[00011244] 202e fff8                 move.l    -8(a6),d0
[00011248] 4240                      clr.w     d0
[0001124a] 4840                      swap      d0
[0001124c] 0280 0000 000f            andi.l    #$0000000F,d0
[00011252] 3239 0001 4de8            move.w    $00014DE8,d1
[00011258] 48c1                      ext.l     d1
[0001125a] b081                      cmp.l     d1,d0
[0001125c] 652a                      bcs.s     $00011288
[0001125e] 202e fffc                 move.l    -4(a6),d0
[00011262] 4240                      clr.w     d0
[00011264] 4840                      swap      d0
[00011266] 0280 0000 000f            andi.l    #$0000000F,d0
[0001126c] 3239 0001 4de8            move.w    $00014DE8,d1
[00011272] 48c1                      ext.l     d1
[00011274] b081                      cmp.l     d1,d0
[00011276] 6510                      bcs.s     $00011288
[00011278] 7000                      moveq.l   #0,d0
[0001127a] 222e fff8                 move.l    -8(a6),d1
[0001127e] 0281 fff0 ffff            andi.l    #$FFF0FFFF,d1
[00011284] 2d41 fff8                 move.l    d1,-8(a6)
[00011288] 486e fff6                 pea.l     -10(a6)
[0001128c] 486e fff8                 pea.l     -8(a6)
[00011290] 4eb9 0001 2e38            jsr       _FColor
[00011296] 302e fff6                 move.w    -10(a6),d0
[0001129a] 4cee 00c0 ffec            movem.l   -20(a6),d6-d7
[000112a0] 4e5e                      unlk      a6
[000112a2] 4e75                      rts
_set_windows:
[000112a4] 48e7 0300                 movem.l   d6-d7,-(a7)
[000112a8] 2079 0001 4df2            movea.l   _wcolors,a0
[000112ae] 3f10                      move.w    (a0),-(a7)
[000112b0] 6100 fe36                 bsr       _check_tcolor
[000112b4] 2e00                      move.l    d0,d7
[000112b6] 2079 0001 4df2            movea.l   _wcolors,a0
[000112bc] 3f28 0002                 move.w    2(a0),-(a7)
[000112c0] 6100 fe26                 bsr       _check_tcolor
[000112c4] 2c00                      move.l    d0,d6
[000112c6] 3e86                      move.w    d6,(a7)
[000112c8] 3f07                      move.w    d7,-(a7)
[000112ca] 7003                      moveq.l   #3,d0
[000112cc] 3f00                      move.w    d0,-(a7)
[000112ce] 7213                      moveq.l   #19,d1
[000112d0] 3f01                      move.w    d1,-(a7)
[000112d2] 4267                      clr.w     -(a7)
[000112d4] 4eb9 0001 30d0            jsr       _wind_set
[000112da] 2079 0001 4df2            movea.l   _wcolors,a0
[000112e0] 3ea8 0004                 move.w    4(a0),(a7)
[000112e4] 6100 fe02                 bsr       _check_tcolor
[000112e8] 2e00                      move.l    d0,d7
[000112ea] 2079 0001 4df2            movea.l   _wcolors,a0
[000112f0] 3ea8 0006                 move.w    6(a0),(a7)
[000112f4] 6100 fdf2                 bsr       _check_tcolor
[000112f8] 2c00                      move.l    d0,d6
[000112fa] 3e86                      move.w    d6,(a7)
[000112fc] 3f07                      move.w    d7,-(a7)
[000112fe] 7005                      moveq.l   #5,d0
[00011300] 3f00                      move.w    d0,-(a7)
[00011302] 7213                      moveq.l   #19,d1
[00011304] 3f01                      move.w    d1,-(a7)
[00011306] 4267                      clr.w     -(a7)
[00011308] 4eb9 0001 30d0            jsr       _wind_set
[0001130e] 2079 0001 4df2            movea.l   _wcolors,a0
[00011314] 3ea8 0008                 move.w    8(a0),(a7)
[00011318] 6100 fdce                 bsr       _check_tcolor
[0001131c] 2e00                      move.l    d0,d7
[0001131e] 2079 0001 4df2            movea.l   _wcolors,a0
[00011324] 3ea8 000a                 move.w    10(a0),(a7)
[00011328] 6100 fdbe                 bsr       _check_tcolor
[0001132c] 2c00                      move.l    d0,d6
[0001132e] 3e86                      move.w    d6,(a7)
[00011330] 3f07                      move.w    d7,-(a7)
[00011332] 7008                      moveq.l   #8,d0
[00011334] 3f00                      move.w    d0,-(a7)
[00011336] 7213                      moveq.l   #19,d1
[00011338] 3f01                      move.w    d1,-(a7)
[0001133a] 4267                      clr.w     -(a7)
[0001133c] 4eb9 0001 30d0            jsr       _wind_set
[00011342] 7eff                      moveq.l   #-1,d7
[00011344] 2079 0001 4df2            movea.l   _wcolors,a0
[0001134a] 3c28 0014                 move.w    20(a0),d6
[0001134e] 3e86                      move.w    d6,(a7)
[00011350] 3f07                      move.w    d7,-(a7)
[00011352] 7009                      moveq.l   #9,d0
[00011354] 3f00                      move.w    d0,-(a7)
[00011356] 7213                      moveq.l   #19,d1
[00011358] 3f01                      move.w    d1,-(a7)
[0001135a] 4267                      clr.w     -(a7)
[0001135c] 4eb9 0001 30d0            jsr       _wind_set
[00011362] 2079 0001 4df2            movea.l   _wcolors,a0
[00011368] 3c28 0026                 move.w    38(a0),d6
[0001136c] 3e86                      move.w    d6,(a7)
[0001136e] 3f07                      move.w    d7,-(a7)
[00011370] 700e                      moveq.l   #14,d0
[00011372] 3f00                      move.w    d0,-(a7)
[00011374] 7213                      moveq.l   #19,d1
[00011376] 3f01                      move.w    d1,-(a7)
[00011378] 4267                      clr.w     -(a7)
[0001137a] 4eb9 0001 30d0            jsr       _wind_set
[00011380] 7cff                      moveq.l   #-1,d6
[00011382] 2079 0001 4df2            movea.l   _wcolors,a0
[00011388] 3168 0012 000e            move.w    18(a0),14(a0)
[0001138e] 2079 0001 4df2            movea.l   _wcolors,a0
[00011394] 3ea8 000c                 move.w    12(a0),(a7)
[00011398] 6100 fd4e                 bsr       _check_tcolor
[0001139c] 4fef 002c                 lea.l     44(a7),a7
[000113a0] 2e00                      move.l    d0,d7
[000113a2] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000113aa] 6d12                      blt.s     $000113BE
[000113ac] 2079 0001 4df2            movea.l   _wcolors,a0
[000113b2] 3f28 000e                 move.w    14(a0),-(a7)
[000113b6] 6100 fd30                 bsr       _check_tcolor
[000113ba] 544f                      addq.w    #2,a7
[000113bc] 2c00                      move.l    d0,d6
[000113be] 3f06                      move.w    d6,-(a7)
[000113c0] 3f07                      move.w    d7,-(a7)
[000113c2] 7002                      moveq.l   #2,d0
[000113c4] 3f00                      move.w    d0,-(a7)
[000113c6] 7213                      moveq.l   #19,d1
[000113c8] 3f01                      move.w    d1,-(a7)
[000113ca] 4267                      clr.w     -(a7)
[000113cc] 4eb9 0001 30d0            jsr       _wind_set
[000113d2] 2079 0001 4df2            movea.l   _wcolors,a0
[000113d8] 3ea8 0010                 move.w    16(a0),(a7)
[000113dc] 6100 fd0a                 bsr       _check_tcolor
[000113e0] 4fef 000a                 lea.l     10(a7),a7
[000113e4] 2e00                      move.l    d0,d7
[000113e6] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000113ee] 6d12                      blt.s     $00011402
[000113f0] 2079 0001 4df2            movea.l   _wcolors,a0
[000113f6] 3f28 0012                 move.w    18(a0),-(a7)
[000113fa] 6100 fcec                 bsr       _check_tcolor
[000113fe] 544f                      addq.w    #2,a7
[00011400] 2c00                      move.l    d0,d6
[00011402] 3f06                      move.w    d6,-(a7)
[00011404] 3f07                      move.w    d7,-(a7)
[00011406] 7004                      moveq.l   #4,d0
[00011408] 3f00                      move.w    d0,-(a7)
[0001140a] 7213                      moveq.l   #19,d1
[0001140c] 3f01                      move.w    d1,-(a7)
[0001140e] 4267                      clr.w     -(a7)
[00011410] 4eb9 0001 30d0            jsr       _wind_set
[00011416] 2079 0001 4df2            movea.l   _wcolors,a0
[0001141c] 3ea8 0016                 move.w    22(a0),(a7)
[00011420] 6100 fcc6                 bsr       _check_tcolor
[00011424] 4fef 000a                 lea.l     10(a7),a7
[00011428] 2e00                      move.l    d0,d7
[0001142a] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00011432] 6d12                      blt.s     $00011446
[00011434] 2079 0001 4df2            movea.l   _wcolors,a0
[0001143a] 3f28 0018                 move.w    24(a0),-(a7)
[0001143e] 6100 fca8                 bsr       _check_tcolor
[00011442] 544f                      addq.w    #2,a7
[00011444] 2c00                      move.l    d0,d6
[00011446] 3f06                      move.w    d6,-(a7)
[00011448] 3f07                      move.w    d7,-(a7)
[0001144a] 700a                      moveq.l   #10,d0
[0001144c] 3f00                      move.w    d0,-(a7)
[0001144e] 7213                      moveq.l   #19,d1
[00011450] 3f01                      move.w    d1,-(a7)
[00011452] 4267                      clr.w     -(a7)
[00011454] 4eb9 0001 30d0            jsr       _wind_set
[0001145a] 2079 0001 4df2            movea.l   _wcolors,a0
[00011460] 3ea8 001a                 move.w    26(a0),(a7)
[00011464] 6100 fc82                 bsr       _check_tcolor
[00011468] 4fef 000a                 lea.l     10(a7),a7
[0001146c] 2e00                      move.l    d0,d7
[0001146e] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00011476] 6d12                      blt.s     $0001148A
[00011478] 2079 0001 4df2            movea.l   _wcolors,a0
[0001147e] 3f28 001c                 move.w    28(a0),-(a7)
[00011482] 6100 fc64                 bsr       _check_tcolor
[00011486] 544f                      addq.w    #2,a7
[00011488] 2c00                      move.l    d0,d6
[0001148a] 3f06                      move.w    d6,-(a7)
[0001148c] 3f07                      move.w    d7,-(a7)
[0001148e] 700b                      moveq.l   #11,d0
[00011490] 3f00                      move.w    d0,-(a7)
[00011492] 7213                      moveq.l   #19,d1
[00011494] 3f01                      move.w    d1,-(a7)
[00011496] 4267                      clr.w     -(a7)
[00011498] 4eb9 0001 30d0            jsr       _wind_set
[0001149e] 2079 0001 4df2            movea.l   _wcolors,a0
[000114a4] 3ea8 0028                 move.w    40(a0),(a7)
[000114a8] 6100 fc3e                 bsr       _check_tcolor
[000114ac] 4fef 000a                 lea.l     10(a7),a7
[000114b0] 2e00                      move.l    d0,d7
[000114b2] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000114ba] 6d12                      blt.s     $000114CE
[000114bc] 2079 0001 4df2            movea.l   _wcolors,a0
[000114c2] 3f28 002a                 move.w    42(a0),-(a7)
[000114c6] 6100 fc20                 bsr       _check_tcolor
[000114ca] 544f                      addq.w    #2,a7
[000114cc] 2c00                      move.l    d0,d6
[000114ce] 3f06                      move.w    d6,-(a7)
[000114d0] 3f07                      move.w    d7,-(a7)
[000114d2] 700f                      moveq.l   #15,d0
[000114d4] 3f00                      move.w    d0,-(a7)
[000114d6] 7213                      moveq.l   #19,d1
[000114d8] 3f01                      move.w    d1,-(a7)
[000114da] 4267                      clr.w     -(a7)
[000114dc] 4eb9 0001 30d0            jsr       _wind_set
[000114e2] 2079 0001 4df2            movea.l   _wcolors,a0
[000114e8] 3ea8 002c                 move.w    44(a0),(a7)
[000114ec] 6100 fbfa                 bsr       _check_tcolor
[000114f0] 4fef 000a                 lea.l     10(a7),a7
[000114f4] 2e00                      move.l    d0,d7
[000114f6] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000114fe] 6d12                      blt.s     $00011512
[00011500] 2079 0001 4df2            movea.l   _wcolors,a0
[00011506] 3f28 002e                 move.w    46(a0),-(a7)
[0001150a] 6100 fbdc                 bsr       _check_tcolor
[0001150e] 544f                      addq.w    #2,a7
[00011510] 2c00                      move.l    d0,d6
[00011512] 3f06                      move.w    d6,-(a7)
[00011514] 3f07                      move.w    d7,-(a7)
[00011516] 7010                      moveq.l   #16,d0
[00011518] 3f00                      move.w    d0,-(a7)
[0001151a] 7213                      moveq.l   #19,d1
[0001151c] 3f01                      move.w    d1,-(a7)
[0001151e] 4267                      clr.w     -(a7)
[00011520] 4eb9 0001 30d0            jsr       _wind_set
[00011526] 4fef 000a                 lea.l     10(a7),a7
[0001152a] 2079 0001 4df2            movea.l   _wcolors,a0
[00011530] 3e28 001e                 move.w    30(a0),d7
[00011534] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[0001153c] 6d04                      blt.s     $00011542
[0001153e] 3c28 0020                 move.w    32(a0),d6
[00011542] 3f06                      move.w    d6,-(a7)
[00011544] 3f07                      move.w    d7,-(a7)
[00011546] 700c                      moveq.l   #12,d0
[00011548] 3f00                      move.w    d0,-(a7)
[0001154a] 7213                      moveq.l   #19,d1
[0001154c] 3f01                      move.w    d1,-(a7)
[0001154e] 4267                      clr.w     -(a7)
[00011550] 4eb9 0001 30d0            jsr       _wind_set
[00011556] 2079 0001 4df2            movea.l   _wcolors,a0
[0001155c] 3ea8 0022                 move.w    34(a0),(a7)
[00011560] 3f07                      move.w    d7,-(a7)
[00011562] 6100 fc58                 bsr       _check_elcolor
[00011566] 4fef 000c                 lea.l     12(a7),a7
[0001156a] 2e00                      move.l    d0,d7
[0001156c] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00011574] 6d14                      blt.s     $0001158A
[00011576] 2079 0001 4df2            movea.l   _wcolors,a0
[0001157c] 3f28 0024                 move.w    36(a0),-(a7)
[00011580] 3f06                      move.w    d6,-(a7)
[00011582] 6100 fc38                 bsr       _check_elcolor
[00011586] 584f                      addq.w    #4,a7
[00011588] 2c00                      move.l    d0,d6
[0001158a] 3f06                      move.w    d6,-(a7)
[0001158c] 3f07                      move.w    d7,-(a7)
[0001158e] 700d                      moveq.l   #13,d0
[00011590] 3f00                      move.w    d0,-(a7)
[00011592] 7213                      moveq.l   #19,d1
[00011594] 3f01                      move.w    d1,-(a7)
[00011596] 4267                      clr.w     -(a7)
[00011598] 4eb9 0001 30d0            jsr       _wind_set
[0001159e] 4fef 000a                 lea.l     10(a7),a7
[000115a2] 2079 0001 4df2            movea.l   _wcolors,a0
[000115a8] 3e28 0030                 move.w    48(a0),d7
[000115ac] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000115b4] 6d04                      blt.s     $000115BA
[000115b6] 3c28 0032                 move.w    50(a0),d6
[000115ba] 3f06                      move.w    d6,-(a7)
[000115bc] 3f07                      move.w    d7,-(a7)
[000115be] 7011                      moveq.l   #17,d0
[000115c0] 3f00                      move.w    d0,-(a7)
[000115c2] 7213                      moveq.l   #19,d1
[000115c4] 3f01                      move.w    d1,-(a7)
[000115c6] 4267                      clr.w     -(a7)
[000115c8] 4eb9 0001 30d0            jsr       _wind_set
[000115ce] 2079 0001 4df2            movea.l   _wcolors,a0
[000115d4] 3ea8 0034                 move.w    52(a0),(a7)
[000115d8] 3f07                      move.w    d7,-(a7)
[000115da] 6100 fbe0                 bsr       _check_elcolor
[000115de] 4fef 000c                 lea.l     12(a7),a7
[000115e2] 2e00                      move.l    d0,d7
[000115e4] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000115ec] 6d14                      blt.s     $00011602
[000115ee] 2079 0001 4df2            movea.l   _wcolors,a0
[000115f4] 3f28 0036                 move.w    54(a0),-(a7)
[000115f8] 3f07                      move.w    d7,-(a7)
[000115fa] 6100 fbc0                 bsr       _check_elcolor
[000115fe] 584f                      addq.w    #4,a7
[00011600] 2c00                      move.l    d0,d6
[00011602] 3f06                      move.w    d6,-(a7)
[00011604] 3f07                      move.w    d7,-(a7)
[00011606] 7012                      moveq.l   #18,d0
[00011608] 3f00                      move.w    d0,-(a7)
[0001160a] 7213                      moveq.l   #19,d1
[0001160c] 3f01                      move.w    d1,-(a7)
[0001160e] 4267                      clr.w     -(a7)
[00011610] 4eb9 0001 30d0            jsr       _wind_set
[00011616] 4fef 000a                 lea.l     10(a7),a7
[0001161a] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[0001161e] 4e75                      rts
_set_wobjects:
[00011620] 48e7 0024                 movem.l   a2/a5,-(a7)
[00011624] 2a6f 000c                 movea.l   12(a7),a5
[00011628] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00011630] 6d00 00f0                 blt       $00011722
[00011634] 2079 0001 4dcc            movea.l   _tree,a0
[0001163a] d0fc 00e4                 adda.w    #$00E4,a0
[0001163e] 43e8 0002                 lea.l     2(a0),a1
[00011642] 3f2d 0012                 move.w    18(a5),-(a7)
[00011646] 2f09                      move.l    a1,-(a7)
[00011648] 4eb9 0001 2e2c            jsr       ToColor
[0001164e] 2079 0001 4dcc            movea.l   _tree,a0
[00011654] d0fc 0174                 adda.w    #$0174,a0
[00011658] 43e8 0002                 lea.l     2(a0),a1
[0001165c] 3ead 0018                 move.w    24(a5),(a7)
[00011660] 2f09                      move.l    a1,-(a7)
[00011662] 4eb9 0001 2e2c            jsr       ToColor
[00011668] 2079 0001 4dcc            movea.l   _tree,a0
[0001166e] d0fc 018c                 adda.w    #$018C,a0
[00011672] 43e8 0002                 lea.l     2(a0),a1
[00011676] 3ead 001c                 move.w    28(a5),(a7)
[0001167a] 2f09                      move.l    a1,-(a7)
[0001167c] 4eb9 0001 2e2c            jsr       ToColor
[00011682] 2079 0001 4dcc            movea.l   _tree,a0
[00011688] d0fc 01bc                 adda.w    #$01BC,a0
[0001168c] 43e8 0002                 lea.l     2(a0),a1
[00011690] 3ead 0020                 move.w    32(a5),(a7)
[00011694] 2f09                      move.l    a1,-(a7)
[00011696] 4eb9 0001 2e2c            jsr       ToColor
[0001169c] 2079 0001 4dcc            movea.l   _tree,a0
[000116a2] d0fc 01a4                 adda.w    #$01A4,a0
[000116a6] 43e8 0002                 lea.l     2(a0),a1
[000116aa] 3ead 0024                 move.w    36(a5),(a7)
[000116ae] 2f09                      move.l    a1,-(a7)
[000116b0] 4eb9 0001 2e2c            jsr       ToColor
[000116b6] 2079 0001 4dcc            movea.l   _tree,a0
[000116bc] d0fc 01ec                 adda.w    #$01EC,a0
[000116c0] 43e8 0002                 lea.l     2(a0),a1
[000116c4] 3ead 002a                 move.w    42(a5),(a7)
[000116c8] 2f09                      move.l    a1,-(a7)
[000116ca] 4eb9 0001 2e2c            jsr       ToColor
[000116d0] 2079 0001 4dcc            movea.l   _tree,a0
[000116d6] d0fc 0204                 adda.w    #$0204,a0
[000116da] 43e8 0002                 lea.l     2(a0),a1
[000116de] 3ead 002e                 move.w    46(a5),(a7)
[000116e2] 2f09                      move.l    a1,-(a7)
[000116e4] 4eb9 0001 2e2c            jsr       ToColor
[000116ea] 2079 0001 4dcc            movea.l   _tree,a0
[000116f0] d0fc 0234                 adda.w    #$0234,a0
[000116f4] 43e8 0002                 lea.l     2(a0),a1
[000116f8] 3ead 0032                 move.w    50(a5),(a7)
[000116fc] 2f09                      move.l    a1,-(a7)
[000116fe] 4eb9 0001 2e2c            jsr       ToColor
[00011704] 2079 0001 4dcc            movea.l   _tree,a0
[0001170a] d0fc 021c                 adda.w    #$021C,a0
[0001170e] 43e8 0002                 lea.l     2(a0),a1
[00011712] 3ead 0036                 move.w    54(a5),(a7)
[00011716] 2f09                      move.l    a1,-(a7)
[00011718] 4eb9 0001 2e2c            jsr       ToColor
[0001171e] 4fef 0026                 lea.l     38(a7),a7
[00011722] 2279 0001 4dcc            movea.l   _tree,a1
[00011728] 2069 00cc                 movea.l   204(a1),a0
[0001172c] 45e8 0012                 lea.l     18(a0),a2
[00011730] 34ad 0002                 move.w    2(a5),(a2)
[00011734] 2279 0001 4dcc            movea.l   _tree,a1
[0001173a] 2069 00fc                 movea.l   252(a1),a0
[0001173e] 45e8 0012                 lea.l     18(a0),a2
[00011742] 34ad 0006                 move.w    6(a5),(a2)
[00011746] 2079 0001 4dcc            movea.l   _tree,a0
[0001174c] d0fc 0144                 adda.w    #$0144,a0
[00011750] 43e8 0002                 lea.l     2(a0),a1
[00011754] 3f2d 000a                 move.w    10(a5),-(a7)
[00011758] 2f09                      move.l    a1,-(a7)
[0001175a] 4eb9 0001 2e2c            jsr       ToColor
[00011760] 2079 0001 4dcc            movea.l   _tree,a0
[00011766] d0fc 015c                 adda.w    #$015C,a0
[0001176a] 43e8 0002                 lea.l     2(a0),a1
[0001176e] 3ead 0014                 move.w    20(a5),(a7)
[00011772] 2f09                      move.l    a1,-(a7)
[00011774] 4eb9 0001 2e2c            jsr       ToColor
[0001177a] 2079 0001 4dcc            movea.l   _tree,a0
[00011780] d0fc 01d4                 adda.w    #$01D4,a0
[00011784] 43e8 0002                 lea.l     2(a0),a1
[00011788] 3ead 0026                 move.w    38(a5),(a7)
[0001178c] 2f09                      move.l    a1,-(a7)
[0001178e] 4eb9 0001 2e2c            jsr       ToColor
[00011794] 2079 0001 4dcc            movea.l   _tree,a0
[0001179a] d0fc 027c                 adda.w    #$027C,a0
[0001179e] 43e8 0002                 lea.l     2(a0),a1
[000117a2] 3ead 000c                 move.w    12(a5),(a7)
[000117a6] 2f09                      move.l    a1,-(a7)
[000117a8] 4eb9 0001 2e2c            jsr       ToColor
[000117ae] 2079 0001 4dcc            movea.l   _tree,a0
[000117b4] d0fc 0294                 adda.w    #$0294,a0
[000117b8] 43e8 0002                 lea.l     2(a0),a1
[000117bc] 3ead 0010                 move.w    16(a5),(a7)
[000117c0] 2f09                      move.l    a1,-(a7)
[000117c2] 4eb9 0001 2e2c            jsr       ToColor
[000117c8] 2279 0001 4dcc            movea.l   _tree,a1
[000117ce] 2069 02ac                 movea.l   684(a1),a0
[000117d2] 45e8 0012                 lea.l     18(a0),a2
[000117d6] 3495                      move.w    (a5),(a2)
[000117d8] 2279 0001 4dcc            movea.l   _tree,a1
[000117de] 2069 02c4                 movea.l   708(a1),a0
[000117e2] 45e8 0012                 lea.l     18(a0),a2
[000117e6] 34ad 0004                 move.w    4(a5),(a2)
[000117ea] 2079 0001 4dcc            movea.l   _tree,a0
[000117f0] d0fc 04ec                 adda.w    #$04EC,a0
[000117f4] 43e8 0002                 lea.l     2(a0),a1
[000117f8] 3ead 0016                 move.w    22(a5),(a7)
[000117fc] 2f09                      move.l    a1,-(a7)
[000117fe] 4eb9 0001 2e2c            jsr       ToColor
[00011804] 2079 0001 4dcc            movea.l   _tree,a0
[0001180a] d0fc 0504                 adda.w    #$0504,a0
[0001180e] 43e8 0002                 lea.l     2(a0),a1
[00011812] 3ead 001a                 move.w    26(a5),(a7)
[00011816] 2f09                      move.l    a1,-(a7)
[00011818] 4eb9 0001 2e2c            jsr       ToColor
[0001181e] 2079 0001 4dcc            movea.l   _tree,a0
[00011824] d0fc 051c                 adda.w    #$051C,a0
[00011828] 43e8 0002                 lea.l     2(a0),a1
[0001182c] 3ead 001e                 move.w    30(a5),(a7)
[00011830] 2f09                      move.l    a1,-(a7)
[00011832] 4eb9 0001 2e2c            jsr       ToColor
[00011838] 2079 0001 4dcc            movea.l   _tree,a0
[0001183e] d0fc 0534                 adda.w    #$0534,a0
[00011842] 43e8 0002                 lea.l     2(a0),a1
[00011846] 3ead 0022                 move.w    34(a5),(a7)
[0001184a] 2f09                      move.l    a1,-(a7)
[0001184c] 4eb9 0001 2e2c            jsr       ToColor
[00011852] 2079 0001 4dcc            movea.l   _tree,a0
[00011858] d0fc 0564                 adda.w    #$0564,a0
[0001185c] 43e8 0002                 lea.l     2(a0),a1
[00011860] 3ead 0028                 move.w    40(a5),(a7)
[00011864] 2f09                      move.l    a1,-(a7)
[00011866] 4eb9 0001 2e2c            jsr       ToColor
[0001186c] 2079 0001 4dcc            movea.l   _tree,a0
[00011872] d0fc 057c                 adda.w    #$057C,a0
[00011876] 43e8 0002                 lea.l     2(a0),a1
[0001187a] 3ead 002c                 move.w    44(a5),(a7)
[0001187e] 2f09                      move.l    a1,-(a7)
[00011880] 4eb9 0001 2e2c            jsr       ToColor
[00011886] 2079 0001 4dcc            movea.l   _tree,a0
[0001188c] d0fc 0594                 adda.w    #$0594,a0
[00011890] 43e8 0002                 lea.l     2(a0),a1
[00011894] 3ead 0030                 move.w    48(a5),(a7)
[00011898] 2f09                      move.l    a1,-(a7)
[0001189a] 4eb9 0001 2e2c            jsr       ToColor
[000118a0] 2079 0001 4dcc            movea.l   _tree,a0
[000118a6] d0fc 05ac                 adda.w    #$05AC,a0
[000118aa] 43e8 0002                 lea.l     2(a0),a1
[000118ae] 3ead 0034                 move.w    52(a5),(a7)
[000118b2] 2f09                      move.l    a1,-(a7)
[000118b4] 4eb9 0001 2e2c            jsr       ToColor
[000118ba] 2079 0001 4dcc            movea.l   _tree,a0
[000118c0] d0fc 05c4                 adda.w    #$05C4,a0
[000118c4] 43e8 0002                 lea.l     2(a0),a1
[000118c8] 3ead 0008                 move.w    8(a5),(a7)
[000118cc] 2f09                      move.l    a1,-(a7)
[000118ce] 4eb9 0001 2e2c            jsr       ToColor
[000118d4] 4297                      clr.l     (a7)
[000118d6] 7008                      moveq.l   #8,d0
[000118d8] 3f00                      move.w    d0,-(a7)
[000118da] 4267                      clr.w     -(a7)
[000118dc] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000118e2] 4eb9 0001 3034            jsr       _Objc_draw
[000118e8] 4fef 0042                 lea.l     66(a7),a7
[000118ec] 6100 f67c                 bsr       _set_info
[000118f0] 4cdf 2400                 movem.l   (a7)+,a2/a5
[000118f4] 4e75                      rts
_set_wcolors:
[000118f6] 2f0a                      move.l    a2,-(a7)
[000118f8] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[00011900] 6d00 0138                 blt       $00011A3A
[00011904] 2079 0001 4dcc            movea.l   _tree,a0
[0001190a] d0fc 00e4                 adda.w    #$00E4,a0
[0001190e] 43e8 0002                 lea.l     2(a0),a1
[00011912] 2079 0001 4df2            movea.l   _wcolors,a0
[00011918] d0fc 0012                 adda.w    #$0012,a0
[0001191c] 2f08                      move.l    a0,-(a7)
[0001191e] 2f09                      move.l    a1,-(a7)
[00011920] 4eb9 0001 2e38            jsr       _FColor
[00011926] 2079 0001 4dcc            movea.l   _tree,a0
[0001192c] d0fc 0174                 adda.w    #$0174,a0
[00011930] 43e8 0002                 lea.l     2(a0),a1
[00011934] 2079 0001 4df2            movea.l   _wcolors,a0
[0001193a] d0fc 0018                 adda.w    #$0018,a0
[0001193e] 2e88                      move.l    a0,(a7)
[00011940] 2f09                      move.l    a1,-(a7)
[00011942] 4eb9 0001 2e38            jsr       _FColor
[00011948] 2079 0001 4dcc            movea.l   _tree,a0
[0001194e] d0fc 018c                 adda.w    #$018C,a0
[00011952] 43e8 0002                 lea.l     2(a0),a1
[00011956] 2079 0001 4df2            movea.l   _wcolors,a0
[0001195c] d0fc 001c                 adda.w    #$001C,a0
[00011960] 2e88                      move.l    a0,(a7)
[00011962] 2f09                      move.l    a1,-(a7)
[00011964] 4eb9 0001 2e38            jsr       _FColor
[0001196a] 2079 0001 4dcc            movea.l   _tree,a0
[00011970] d0fc 01bc                 adda.w    #$01BC,a0
[00011974] 43e8 0002                 lea.l     2(a0),a1
[00011978] 2079 0001 4df2            movea.l   _wcolors,a0
[0001197e] d0fc 0020                 adda.w    #$0020,a0
[00011982] 2e88                      move.l    a0,(a7)
[00011984] 2f09                      move.l    a1,-(a7)
[00011986] 4eb9 0001 2e38            jsr       _FColor
[0001198c] 2079 0001 4dcc            movea.l   _tree,a0
[00011992] d0fc 01a4                 adda.w    #$01A4,a0
[00011996] 43e8 0002                 lea.l     2(a0),a1
[0001199a] 2079 0001 4df2            movea.l   _wcolors,a0
[000119a0] d0fc 0024                 adda.w    #$0024,a0
[000119a4] 2e88                      move.l    a0,(a7)
[000119a6] 2f09                      move.l    a1,-(a7)
[000119a8] 4eb9 0001 2e38            jsr       _FColor
[000119ae] 2079 0001 4dcc            movea.l   _tree,a0
[000119b4] d0fc 01ec                 adda.w    #$01EC,a0
[000119b8] 43e8 0002                 lea.l     2(a0),a1
[000119bc] 2079 0001 4df2            movea.l   _wcolors,a0
[000119c2] d0fc 002a                 adda.w    #$002A,a0
[000119c6] 2e88                      move.l    a0,(a7)
[000119c8] 2f09                      move.l    a1,-(a7)
[000119ca] 4eb9 0001 2e38            jsr       _FColor
[000119d0] 2079 0001 4dcc            movea.l   _tree,a0
[000119d6] d0fc 0204                 adda.w    #$0204,a0
[000119da] 43e8 0002                 lea.l     2(a0),a1
[000119de] 2079 0001 4df2            movea.l   _wcolors,a0
[000119e4] d0fc 002e                 adda.w    #$002E,a0
[000119e8] 2e88                      move.l    a0,(a7)
[000119ea] 2f09                      move.l    a1,-(a7)
[000119ec] 4eb9 0001 2e38            jsr       _FColor
[000119f2] 2079 0001 4dcc            movea.l   _tree,a0
[000119f8] d0fc 0234                 adda.w    #$0234,a0
[000119fc] 43e8 0002                 lea.l     2(a0),a1
[00011a00] 2079 0001 4df2            movea.l   _wcolors,a0
[00011a06] d0fc 0032                 adda.w    #$0032,a0
[00011a0a] 2e88                      move.l    a0,(a7)
[00011a0c] 2f09                      move.l    a1,-(a7)
[00011a0e] 4eb9 0001 2e38            jsr       _FColor
[00011a14] 2079 0001 4dcc            movea.l   _tree,a0
[00011a1a] d0fc 021c                 adda.w    #$021C,a0
[00011a1e] 43e8 0002                 lea.l     2(a0),a1
[00011a22] 2079 0001 4df2            movea.l   _wcolors,a0
[00011a28] d0fc 0036                 adda.w    #$0036,a0
[00011a2c] 2e88                      move.l    a0,(a7)
[00011a2e] 2f09                      move.l    a1,-(a7)
[00011a30] 4eb9 0001 2e38            jsr       _FColor
[00011a36] 4fef 0028                 lea.l     40(a7),a7
[00011a3a] 2279 0001 4dcc            movea.l   _tree,a1
[00011a40] 2069 00cc                 movea.l   204(a1),a0
[00011a44] 45e8 0012                 lea.l     18(a0),a2
[00011a48] 2079 0001 4df2            movea.l   _wcolors,a0
[00011a4e] 3152 0002                 move.w    (a2),2(a0)
[00011a52] 2279 0001 4dcc            movea.l   _tree,a1
[00011a58] 2069 00fc                 movea.l   252(a1),a0
[00011a5c] 45e8 0012                 lea.l     18(a0),a2
[00011a60] 2079 0001 4df2            movea.l   _wcolors,a0
[00011a66] 3152 0006                 move.w    (a2),6(a0)
[00011a6a] 2079 0001 4dcc            movea.l   _tree,a0
[00011a70] d0fc 015c                 adda.w    #$015C,a0
[00011a74] 43e8 0002                 lea.l     2(a0),a1
[00011a78] 2079 0001 4df2            movea.l   _wcolors,a0
[00011a7e] d0fc 0014                 adda.w    #$0014,a0
[00011a82] 2f08                      move.l    a0,-(a7)
[00011a84] 2f09                      move.l    a1,-(a7)
[00011a86] 4eb9 0001 2e38            jsr       _FColor
[00011a8c] 2079 0001 4dcc            movea.l   _tree,a0
[00011a92] d0fc 01d4                 adda.w    #$01D4,a0
[00011a96] 43e8 0002                 lea.l     2(a0),a1
[00011a9a] 2079 0001 4df2            movea.l   _wcolors,a0
[00011aa0] d0fc 0026                 adda.w    #$0026,a0
[00011aa4] 2e88                      move.l    a0,(a7)
[00011aa6] 2f09                      move.l    a1,-(a7)
[00011aa8] 4eb9 0001 2e38            jsr       _FColor
[00011aae] 2079 0001 4dcc            movea.l   _tree,a0
[00011ab4] d0fc 0144                 adda.w    #$0144,a0
[00011ab8] 43e8 0002                 lea.l     2(a0),a1
[00011abc] 2079 0001 4df2            movea.l   _wcolors,a0
[00011ac2] d0fc 000a                 adda.w    #$000A,a0
[00011ac6] 2e88                      move.l    a0,(a7)
[00011ac8] 2f09                      move.l    a1,-(a7)
[00011aca] 4eb9 0001 2e38            jsr       _FColor
[00011ad0] 2079 0001 4dcc            movea.l   _tree,a0
[00011ad6] d0fc 027c                 adda.w    #$027C,a0
[00011ada] 43e8 0002                 lea.l     2(a0),a1
[00011ade] 2079 0001 4df2            movea.l   _wcolors,a0
[00011ae4] d0fc 000c                 adda.w    #$000C,a0
[00011ae8] 2e88                      move.l    a0,(a7)
[00011aea] 2f09                      move.l    a1,-(a7)
[00011aec] 4eb9 0001 2e38            jsr       _FColor
[00011af2] 2079 0001 4dcc            movea.l   _tree,a0
[00011af8] d0fc 0294                 adda.w    #$0294,a0
[00011afc] 43e8 0002                 lea.l     2(a0),a1
[00011b00] 2079 0001 4df2            movea.l   _wcolors,a0
[00011b06] d0fc 0010                 adda.w    #$0010,a0
[00011b0a] 2e88                      move.l    a0,(a7)
[00011b0c] 2f09                      move.l    a1,-(a7)
[00011b0e] 4eb9 0001 2e38            jsr       _FColor
[00011b14] 2279 0001 4dcc            movea.l   _tree,a1
[00011b1a] 2069 02ac                 movea.l   684(a1),a0
[00011b1e] 45e8 0012                 lea.l     18(a0),a2
[00011b22] 2079 0001 4df2            movea.l   _wcolors,a0
[00011b28] 3092                      move.w    (a2),(a0)
[00011b2a] 2279 0001 4dcc            movea.l   _tree,a1
[00011b30] 2069 02c4                 movea.l   708(a1),a0
[00011b34] 43e8 0012                 lea.l     18(a0),a1
[00011b38] 2079 0001 4df2            movea.l   _wcolors,a0
[00011b3e] 3151 0004                 move.w    (a1),4(a0)
[00011b42] 2079 0001 4dcc            movea.l   _tree,a0
[00011b48] d0fc 04ec                 adda.w    #$04EC,a0
[00011b4c] 43e8 0002                 lea.l     2(a0),a1
[00011b50] 2079 0001 4df2            movea.l   _wcolors,a0
[00011b56] d0fc 0016                 adda.w    #$0016,a0
[00011b5a] 2e88                      move.l    a0,(a7)
[00011b5c] 2f09                      move.l    a1,-(a7)
[00011b5e] 4eb9 0001 2e38            jsr       _FColor
[00011b64] 2079 0001 4dcc            movea.l   _tree,a0
[00011b6a] d0fc 0504                 adda.w    #$0504,a0
[00011b6e] 43e8 0002                 lea.l     2(a0),a1
[00011b72] 2079 0001 4df2            movea.l   _wcolors,a0
[00011b78] d0fc 001a                 adda.w    #$001A,a0
[00011b7c] 2e88                      move.l    a0,(a7)
[00011b7e] 2f09                      move.l    a1,-(a7)
[00011b80] 4eb9 0001 2e38            jsr       _FColor
[00011b86] 2079 0001 4dcc            movea.l   _tree,a0
[00011b8c] d0fc 051c                 adda.w    #$051C,a0
[00011b90] 43e8 0002                 lea.l     2(a0),a1
[00011b94] 2079 0001 4df2            movea.l   _wcolors,a0
[00011b9a] d0fc 001e                 adda.w    #$001E,a0
[00011b9e] 2e88                      move.l    a0,(a7)
[00011ba0] 2f09                      move.l    a1,-(a7)
[00011ba2] 4eb9 0001 2e38            jsr       _FColor
[00011ba8] 2079 0001 4dcc            movea.l   _tree,a0
[00011bae] d0fc 0534                 adda.w    #$0534,a0
[00011bb2] 43e8 0002                 lea.l     2(a0),a1
[00011bb6] 2079 0001 4df2            movea.l   _wcolors,a0
[00011bbc] d0fc 0022                 adda.w    #$0022,a0
[00011bc0] 2e88                      move.l    a0,(a7)
[00011bc2] 2f09                      move.l    a1,-(a7)
[00011bc4] 4eb9 0001 2e38            jsr       _FColor
[00011bca] 2079 0001 4dcc            movea.l   _tree,a0
[00011bd0] d0fc 0564                 adda.w    #$0564,a0
[00011bd4] 43e8 0002                 lea.l     2(a0),a1
[00011bd8] 2079 0001 4df2            movea.l   _wcolors,a0
[00011bde] d0fc 0028                 adda.w    #$0028,a0
[00011be2] 2e88                      move.l    a0,(a7)
[00011be4] 2f09                      move.l    a1,-(a7)
[00011be6] 4eb9 0001 2e38            jsr       _FColor
[00011bec] 2079 0001 4dcc            movea.l   _tree,a0
[00011bf2] d0fc 057c                 adda.w    #$057C,a0
[00011bf6] 43e8 0002                 lea.l     2(a0),a1
[00011bfa] 2079 0001 4df2            movea.l   _wcolors,a0
[00011c00] d0fc 002c                 adda.w    #$002C,a0
[00011c04] 2e88                      move.l    a0,(a7)
[00011c06] 2f09                      move.l    a1,-(a7)
[00011c08] 4eb9 0001 2e38            jsr       _FColor
[00011c0e] 2079 0001 4dcc            movea.l   _tree,a0
[00011c14] d0fc 0594                 adda.w    #$0594,a0
[00011c18] 43e8 0002                 lea.l     2(a0),a1
[00011c1c] 2079 0001 4df2            movea.l   _wcolors,a0
[00011c22] d0fc 0030                 adda.w    #$0030,a0
[00011c26] 2e88                      move.l    a0,(a7)
[00011c28] 2f09                      move.l    a1,-(a7)
[00011c2a] 4eb9 0001 2e38            jsr       _FColor
[00011c30] 2079 0001 4dcc            movea.l   _tree,a0
[00011c36] d0fc 05ac                 adda.w    #$05AC,a0
[00011c3a] 43e8 0002                 lea.l     2(a0),a1
[00011c3e] 2079 0001 4df2            movea.l   _wcolors,a0
[00011c44] d0fc 0034                 adda.w    #$0034,a0
[00011c48] 2e88                      move.l    a0,(a7)
[00011c4a] 2f09                      move.l    a1,-(a7)
[00011c4c] 4eb9 0001 2e38            jsr       _FColor
[00011c52] 2079 0001 4dcc            movea.l   _tree,a0
[00011c58] d0fc 05c4                 adda.w    #$05C4,a0
[00011c5c] 43e8 0002                 lea.l     2(a0),a1
[00011c60] 2079 0001 4df2            movea.l   _wcolors,a0
[00011c66] 5048                      addq.w    #8,a0
[00011c68] 2e88                      move.l    a0,(a7)
[00011c6a] 2f09                      move.l    a1,-(a7)
[00011c6c] 4eb9 0001 2e38            jsr       _FColor
[00011c72] 4fef 003c                 lea.l     60(a7),a7
[00011c76] 245f                      movea.l   (a7)+,a2
[00011c78] 4e75                      rts
_drag_curobj:
[00011c7a] 4e56 ffe8                 link      a6,#$FFE8
[00011c7e] 48e7 3720                 movem.l   d2-d3/d5-d7/a2,-(a7)
[00011c82] 42a7                      clr.l     -(a7)
[00011c84] 7004                      moveq.l   #4,d0
[00011c86] 3f00                      move.w    d0,-(a7)
[00011c88] 4eb9 0001 3218            jsr       _graf_mouse
[00011c8e] 5c4f                      addq.w    #6,a7
[00011c90] 3e39 0001 4dbc            move.w    _curobj,d7
[00011c96] 2c07                      move.l    d7,d6
[00011c98] 41f9 0001 4dc4            lea.l     _curcolor,a0
[00011c9e] 43ee ffea                 lea.l     -22(a6),a1
[00011ca2] 2449                      movea.l   a1,a2
[00011ca4] 24d8                      move.l    (a0)+,(a2)+
[00011ca6] 41ee ffee                 lea.l     -18(a6),a0
[00011caa] 20d9                      move.l    (a1)+,(a0)+
[00011cac] 3a39 0001 4dc8            move.w    _curIsTed,d5
[00011cb2] 3039 0001 4dbc            move.w    _curobj,d0
[00011cb8] b047                      cmp.w     d7,d0
[00011cba] 6700 0184                 beq       $00011E40
[00011cbe] 2007                      move.l    d7,d0
[00011cc0] 5140                      subq.w    #8,d0
[00011cc2] 6d00 014e                 blt       $00011E12
[00011cc6] 0c40 0036                 cmpi.w    #$0036,d0
[00011cca] 6c00 0146                 bge       $00011E12
[00011cce] d040                      add.w     d0,d0
[00011cd0] 303b 0006                 move.w    $00011CD8(pc,d0.w),d0
[00011cd4] 4efb 0004                 jmp       $00011CDA(pc,d0.w)
J2:
[00011cd8] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cda] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cdc] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cde] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011ce0] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011ce2] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011ce4] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011ce6] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011ce8] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cea] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cec] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cee] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cf0] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cf2] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cf4] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cf6] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cf8] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011cfa] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011cfc] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011cfe] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d00] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d02] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d04] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d06] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d08] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d0a] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d0c] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d0e] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d10] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d12] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d14] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d16] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d18] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d1a] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d1c] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d1e] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d20] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d22] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d24] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d26] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d28] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d2a] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d2c] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d2e] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d30] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d32] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d34] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d36] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d38] 0138                      dc.w $0138   ; $00011e12-$00011cd8-2
[00011d3a] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d3c] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d3e] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d40] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d42] 006a                      dc.w $006a   ; $00011d44-$00011cd8-2
[00011d44] 70ff                      moveq.l   #-1,d0
[00011d46] b079 0001 4dbc            cmp.w     _curobj,d0
[00011d4c] 671c                      beq.s     $00011D6A
[00011d4e] 41ee ffea                 lea.l     -22(a6),a0
[00011d52] 43f9 0001 4dc4            lea.l     _curcolor,a1
[00011d58] 22d8                      move.l    (a0)+,(a1)+
[00011d5a] 6100 edae                 bsr       _draw_curobj
[00011d5e] 41ee ffee                 lea.l     -18(a6),a0
[00011d62] 43f9 0001 4dc4            lea.l     _curcolor,a1
[00011d68] 22d8                      move.l    (a0)+,(a1)+
[00011d6a] 2007                      move.l    d7,d0
[00011d6c] 33c0 0001 4dbc            move.w    d0,_curobj
[00011d72] 7218                      moveq.l   #24,d1
[00011d74] c1c1                      muls.w    d1,d0
[00011d76] 2079 0001 4dcc            movea.l   _tree,a0
[00011d7c] 2248                      movea.l   a0,a1
[00011d7e] d3c0                      adda.l    d0,a1
[00011d80] 303c 00ff                 move.w    #$00FF,d0
[00011d84] c069 0006                 and.w     6(a1),d0
[00011d88] 7415                      moveq.l   #21,d2
[00011d8a] b042                      cmp.w     d2,d0
[00011d8c] 673a                      beq.s     $00011DC8
[00011d8e] 7416                      moveq.l   #22,d2
[00011d90] b042                      cmp.w     d2,d0
[00011d92] 6734                      beq.s     $00011DC8
[00011d94] 3039 0001 4dbc            move.w    _curobj,d0
[00011d9a] 2400                      move.l    d0,d2
[00011d9c] c5c1                      muls.w    d1,d2
[00011d9e] 2248                      movea.l   a0,a1
[00011da0] d3c2                      adda.l    d2,a1
[00011da2] 343c 00ff                 move.w    #$00FF,d2
[00011da6] c469 0006                 and.w     6(a1),d2
[00011daa] 761d                      moveq.l   #29,d3
[00011dac] b443                      cmp.w     d3,d2
[00011dae] 6718                      beq.s     $00011DC8
[00011db0] c1c1                      muls.w    d1,d0
[00011db2] 2248                      movea.l   a0,a1
[00011db4] d3c0                      adda.l    d0,a1
[00011db6] 303c 00ff                 move.w    #$00FF,d0
[00011dba] c069 0006                 and.w     6(a1),d0
[00011dbe] 741e                      moveq.l   #30,d2
[00011dc0] b042                      cmp.w     d2,d0
[00011dc2] 6704                      beq.s     $00011DC8
[00011dc4] 7000                      moveq.l   #0,d0
[00011dc6] 6002                      bra.s     $00011DCA
[00011dc8] 7001                      moveq.l   #1,d0
[00011dca] 33c0 0001 4dc8            move.w    d0,_curIsTed
[00011dd0] 6722                      beq.s     $00011DF4
[00011dd2] 3039 0001 4dbc            move.w    _curobj,d0
[00011dd8] c1c1                      muls.w    d1,d0
[00011dda] d1c0                      adda.l    d0,a0
[00011ddc] 2268 000c                 movea.l   12(a0),a1
[00011de0] 41e9 0012                 lea.l     18(a1),a0
[00011de4] 3f10                      move.w    (a0),-(a7)
[00011de6] 486e ffea                 pea.l     -22(a6)
[00011dea] 4eb9 0001 2e2c            jsr       ToColor
[00011df0] 5c4f                      addq.w    #6,a7
[00011df2] 6018                      bra.s     $00011E0C
[00011df4] 3039 0001 4dbc            move.w    _curobj,d0
[00011dfa] c1c1                      muls.w    d1,d0
[00011dfc] d1c0                      adda.l    d0,a0
[00011dfe] 43e8 000c                 lea.l     12(a0),a1
[00011e02] 41e9 0002                 lea.l     2(a1),a0
[00011e06] 43ee ffea                 lea.l     -22(a6),a1
[00011e0a] 22d8                      move.l    (a0)+,(a1)+
[00011e0c] 6100 ecfc                 bsr       _draw_curobj
[00011e10] 602e                      bra.s     $00011E40
[00011e12] 70ff                      moveq.l   #-1,d0
[00011e14] b079 0001 4dbc            cmp.w     _curobj,d0
[00011e1a] 6724                      beq.s     $00011E40
[00011e1c] 41ee ffea                 lea.l     -22(a6),a0
[00011e20] 43f9 0001 4dc4            lea.l     _curcolor,a1
[00011e26] 22d8                      move.l    (a0)+,(a1)+
[00011e28] 6100 ece0                 bsr       _draw_curobj
[00011e2c] 41ee ffee                 lea.l     -18(a6),a0
[00011e30] 43f9 0001 4dc4            lea.l     _curcolor,a1
[00011e36] 22d8                      move.l    (a0)+,(a1)+
[00011e38] 33fc ffff 0001 4dbc       move.w    #$FFFF,_curobj
[00011e40] 486e fff8                 pea.l     -8(a6)
[00011e44] 4eb9 0001 3098            jsr       _Graf_mkstate
[00011e4a] 3eae fffa                 move.w    -6(a6),(a7)
[00011e4e] 3f2e fff8                 move.w    -8(a6),-(a7)
[00011e52] 7008                      moveq.l   #8,d0
[00011e54] 3f00                      move.w    d0,-(a7)
[00011e56] 4267                      clr.w     -(a7)
[00011e58] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00011e5e] 4eb9 0001 31dc            jsr       objc_find
[00011e64] 4fef 000e                 lea.l     14(a7),a7
[00011e68] 2e00                      move.l    d0,d7
[00011e6a] 4a6e fffc                 tst.w     -4(a6)
[00011e6e] 6600 fe42                 bne       $00011CB2
[00011e72] 42a7                      clr.l     -(a7)
[00011e74] 4267                      clr.w     -(a7)
[00011e76] 4eb9 0001 3218            jsr       _graf_mouse
[00011e7c] 33c6 0001 4dbc            move.w    d6,_curobj
[00011e82] 33c5 0001 4dc8            move.w    d5,_curIsTed
[00011e88] 4cee 04ec ffd0            movem.l   -48(a6),d2-d3/d5-d7/a2
[00011e8e] 4e5e                      unlk      a6
[00011e90] 4e75                      rts
_do_form:
[00011e92] 4e56 ffd0                 link      a6,#$FFD0
[00011e96] 48e7 3720                 movem.l   d2-d3/d5-d7/a2,-(a7)
[00011e9a] 7000                      moveq.l   #0,d0
[00011e9c] 3f00                      move.w    d0,-(a7)
[00011e9e] 3f00                      move.w    d0,-(a7)
[00011ea0] 3d40 ffda                 move.w    d0,-38(a6)
[00011ea4] 4eb9 0001 3260            jsr       evnt_dclick
[00011eaa] 584f                      addq.w    #4,a7
[00011eac] 2200                      move.l    d0,d1
[00011eae] 48c1                      ext.l     d1
[00011eb0] e581                      asl.l     #2,d1
[00011eb2] 41f9 0001 4d24            lea.l     $00014D24,a0
[00011eb8] d1c1                      adda.l    d1,a0
[00011eba] 23d0 0001 4dec            move.l    (a0),$00014DEC
[00011ec0] 4a6e ffda                 tst.w     -38(a6)
[00011ec4] 6600 097e                 bne       $00012844
[00011ec8] 7000                      moveq.l   #0,d0
[00011eca] 2279 0001 4d88            movea.l   xcpb,a1
[00011ed0] 2069 0034                 movea.l   52(a1),a0
[00011ed4] 4879 0001 4dd0            pea.l     $00014DD0
[00011eda] 3f00                      move.w    d0,-(a7)
[00011edc] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00011ee2] 3d40 ffdc                 move.w    d0,-36(a6)
[00011ee6] 4e90                      jsr       (a0)
[00011ee8] 4fef 000a                 lea.l     10(a7),a7
[00011eec] 2e00                      move.l    d0,d7
[00011eee] 2079 0001 4dcc            movea.l   _tree,a0
[00011ef4] 2248                      movea.l   a0,a1
[00011ef6] d2fc 0010                 adda.w    #$0010,a1
[00011efa] 2039 0001 4de0            move.l    $00014DE0,d0
[00011f00] b091                      cmp.l     (a1),d0
[00011f02] 6612                      bne.s     $00011F16
[00011f04] 43e8 0010                 lea.l     16(a0),a1
[00011f08] 45e9 0004                 lea.l     4(a1),a2
[00011f0c] 2039 0001 4de4            move.l    $00014DE4,d0
[00011f12] b092                      cmp.l     (a2),d0
[00011f14] 6754                      beq.s     $00011F6A
[00011f16] 43e8 0010                 lea.l     16(a0),a1
[00011f1a] 41f9 0001 4de0            lea.l     $00014DE0,a0
[00011f20] 20d9                      move.l    (a1)+,(a0)+
[00011f22] 20d9                      move.l    (a1)+,(a0)+
[00011f24] 42a7                      clr.l     -(a7)
[00011f26] 7008                      moveq.l   #8,d0
[00011f28] 3f00                      move.w    d0,-(a7)
[00011f2a] 7220                      moveq.l   #32,d1
[00011f2c] 3f01                      move.w    d1,-(a7)
[00011f2e] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00011f34] 4eb9 0001 3034            jsr       _Objc_draw
[00011f3a] 4297                      clr.l     (a7)
[00011f3c] 7008                      moveq.l   #8,d0
[00011f3e] 3f00                      move.w    d0,-(a7)
[00011f40] 7222                      moveq.l   #34,d1
[00011f42] 3f01                      move.w    d1,-(a7)
[00011f44] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00011f4a] 4eb9 0001 3034            jsr       _Objc_draw
[00011f50] 4297                      clr.l     (a7)
[00011f52] 7008                      moveq.l   #8,d0
[00011f54] 3f00                      move.w    d0,-(a7)
[00011f56] 7224                      moveq.l   #36,d1
[00011f58] 3f01                      move.w    d1,-(a7)
[00011f5a] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00011f60] 4eb9 0001 3034            jsr       _Objc_draw
[00011f66] 4fef 001c                 lea.l     28(a7),a7
[00011f6a] 70ff                      moveq.l   #-1,d0
[00011f6c] be40                      cmp.w     d0,d7
[00011f6e] 6600 0166                 bne       $000120D6
[00011f72] 3039 0001 4dd0            move.w    $00014DD0,d0
[00011f78] 0440 0016                 subi.w    #$0016,d0
[00011f7c] 6710                      beq.s     $00011F8E
[00011f7e] 0440 0013                 subi.w    #$0013,d0
[00011f82] 6710                      beq.s     $00011F94
[00011f84] 0440 000c                 subi.w    #$000C,d0
[00011f88] 6710                      beq.s     $00011F9A
[00011f8a] 6000 014a                 bra       $000120D6
[00011f8e] 7e04                      moveq.l   #4,d7
[00011f90] 6000 0144                 bra       $000120D6
[00011f94] 7e05                      moveq.l   #5,d7
[00011f96] 6000 013e                 bra       $000120D6
[00011f9a] 3039 0001 4dd6            move.w    $00014DD6,d0
[00011fa0] 0440 1f00                 subi.w    #$1F00,d0
[00011fa4] 6700 00f4                 beq       $0001209A
[00011fa8] 0440 0300                 subi.w    #$0300,d0
[00011fac] 6700 00ae                 beq       $0001205C
[00011fb0] 0440 1900                 subi.w    #$1900,d0
[00011fb4] 676a                      beq.s     $00012020
[00011fb6] 0440 0100                 subi.w    #$0100,d0
[00011fba] 6764                      beq.s     $00012020
[00011fbc] 0440 0100                 subi.w    #$0100,d0
[00011fc0] 675e                      beq.s     $00012020
[00011fc2] 0440 0100                 subi.w    #$0100,d0
[00011fc6] 6758                      beq.s     $00012020
[00011fc8] 0440 0100                 subi.w    #$0100,d0
[00011fcc] 6752                      beq.s     $00012020
[00011fce] 0440 0100                 subi.w    #$0100,d0
[00011fd2] 674c                      beq.s     $00012020
[00011fd4] 0440 0100                 subi.w    #$0100,d0
[00011fd8] 6746                      beq.s     $00012020
[00011fda] 0440 0100                 subi.w    #$0100,d0
[00011fde] 6740                      beq.s     $00012020
[00011fe0] 0440 0100                 subi.w    #$0100,d0
[00011fe4] 673a                      beq.s     $00012020
[00011fe6] 0440 0100                 subi.w    #$0100,d0
[00011fea] 6734                      beq.s     $00012020
[00011fec] 0440 0300                 subi.w    #$0300,d0
[00011ff0] 671e                      beq.s     $00012010
[00011ff2] 0440 0b00                 subi.w    #$0B00,d0
[00011ff6] 6748                      beq.s     $00012040
[00011ff8] 0440 0f00                 subi.w    #$0F00,d0
[00011ffc] 6600 00d8                 bne       $000120D6
[00012000] 2f39 0001 4df2            move.l    _wcolors,-(a7)
[00012006] 6100 f618                 bsr       _set_wobjects
[0001200a] 584f                      addq.w    #4,a7
[0001200c] 6000 00c8                 bra       $000120D6
[00012010] 4879 0001 3698            pea.l     $00013698
[00012016] 6100 f608                 bsr       _set_wobjects
[0001201a] 584f                      addq.w    #4,a7
[0001201c] 6000 00b8                 bra       $000120D6
[00012020] 3039 0001 4dd6            move.w    $00014DD6,d0
[00012026] e040                      asr.w     #8,d0
[00012028] c1fc 0044                 muls.w    #$0044,d0
[0001202c] 41f9 0001 2730            lea.l     $00012730,a0
[00012032] d1c0                      adda.l    d0,a0
[00012034] 2f08                      move.l    a0,-(a7)
[00012036] 6100 f5e8                 bsr       _set_wobjects
[0001203a] 584f                      addq.w    #4,a7
[0001203c] 6000 0098                 bra       $000120D6
[00012040] 3039 0001 4dea            move.w    $00014DEA,d0
[00012046] c1fc 0044                 muls.w    #$0044,d0
[0001204a] 41f9 0001 3588            lea.l     _savecolors,a0
[00012050] d1c0                      adda.l    d0,a0
[00012052] 2f08                      move.l    a0,-(a7)
[00012054] 6100 f5ca                 bsr       _set_wobjects
[00012058] 584f                      addq.w    #4,a7
[0001205a] 607a                      bra.s     $000120D6
[0001205c] 7004                      moveq.l   #4,d0
[0001205e] 2079 0001 4d88            movea.l   xcpb,a0
[00012064] b068 0054                 cmp.w     84(a0),d0
[00012068] 666c                      bne.s     $000120D6
[0001206a] 2079 0001 4dcc            movea.l   _tree,a0
[00012070] 08e8 0000 003b            bset      #0,59(a0)
[00012076] 42a7                      clr.l     -(a7)
[00012078] 7008                      moveq.l   #8,d0
[0001207a] 3f00                      move.w    d0,-(a7)
[0001207c] 7202                      moveq.l   #2,d1
[0001207e] 3f01                      move.w    d1,-(a7)
[00012080] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00012086] 4eb9 0001 3034            jsr       _Objc_draw
[0001208c] 4fef 000c                 lea.l     12(a7),a7
[00012090] 7e02                      moveq.l   #2,d7
[00012092] 3d7c 0001 ffda            move.w    #$0001,-38(a6)
[00012098] 603c                      bra.s     $000120D6
[0001209a] 7004                      moveq.l   #4,d0
[0001209c] 2079 0001 4d88            movea.l   xcpb,a0
[000120a2] b068 0054                 cmp.w     84(a0),d0
[000120a6] 672e                      beq.s     $000120D6
[000120a8] 2079 0001 4dcc            movea.l   _tree,a0
[000120ae] 08e8 0000 003b            bset      #0,59(a0)
[000120b4] 42a7                      clr.l     -(a7)
[000120b6] 7008                      moveq.l   #8,d0
[000120b8] 3f00                      move.w    d0,-(a7)
[000120ba] 7202                      moveq.l   #2,d1
[000120bc] 3f01                      move.w    d1,-(a7)
[000120be] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000120c4] 4eb9 0001 3034            jsr       _Objc_draw
[000120ca] 4fef 000c                 lea.l     12(a7),a7
[000120ce] 7e02                      moveq.l   #2,d7
[000120d0] 3d7c 0001 ffda            move.w    #$0001,-38(a6)
[000120d6] 70ff                      moveq.l   #-1,d0
[000120d8] be40                      cmp.w     d0,d7
[000120da] 6700 fde4                 beq       $00011EC0
[000120de] 0887 000f                 bclr      #15,d7
[000120e2] 2007                      move.l    d7,d0
[000120e4] 5540                      subq.w    #2,d0
[000120e6] 6d00 fdd8                 blt       $00011EC0
[000120ea] 0c40 003d                 cmpi.w    #$003D,d0
[000120ee] 6c00 fdd0                 bge       $00011EC0
[000120f2] d040                      add.w     d0,d0
[000120f4] 303b 0006                 move.w    $000120FC(pc,d0.w),d0
[000120f8] 4efb 0004                 jmp       $000120FE(pc,d0.w)
J3:
[000120fc] 0078                      dc.w $0078   ; $00012176-$000120fc-2
[000120fe] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012100] 013c                      dc.w $013c   ; $0001223a-$000120fc-2
[00012102] 0144                      dc.w $0144   ; $00012242-$000120fc-2
[00012104] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012106] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012108] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[0001210a] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[0001210c] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[0001210e] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012110] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012112] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012114] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012116] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012118] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[0001211a] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[0001211c] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[0001211e] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012120] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012122] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012124] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012126] 0162                      dc.w $0162   ; $00012260-$000120fc-2
[00012128] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[0001212a] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[0001212c] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[0001212e] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012130] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012132] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012134] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012136] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012138] 0600                      dc.w $0600   ; $000126fe-$000120fc-2
[0001213a] 0630                      dc.w $0630   ; $0001272e-$000120fc-2
[0001213c] 066a                      dc.w $066a   ; $00012768-$000120fc-2
[0001213e] 069a                      dc.w $069a   ; $00012798-$000120fc-2
[00012140] 06d4                      dc.w $06d4   ; $000127d2-$000120fc-2
[00012142] 0704                      dc.w $0704   ; $00012802-$000120fc-2
[00012144] 04ec                      dc.w $04ec   ; $000125ea-$000120fc-2
[00012146] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012148] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[0001214a] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[0001214c] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[0001214e] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[00012150] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[00012152] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[00012154] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[00012156] 03b4                      dc.w $03b4   ; $000124b2-$000120fc-2
[00012158] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[0001215a] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[0001215c] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[0001215e] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[00012160] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012162] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012164] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012166] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012168] fdc2                      dc.w $fdc2   ; $00011ec0-$000120fc-2
[0001216a] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[0001216c] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[0001216e] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012170] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012172] 01a0                      dc.w $01a0   ; $0001229e-$000120fc-2
[00012174] 073e                      dc.w $073e   ; $0001283c-$000120fc-2
[00012176] 2279 0001 4d88            movea.l   xcpb,a1
[0001217c] 2069 0044                 movea.l   68(a1),a0
[00012180] 4267                      clr.w     -(a7)
[00012182] 4e90                      jsr       (a0)
[00012184] 544f                      addq.w    #2,a7
[00012186] 4a40                      tst.w     d0
[00012188] 6746                      beq.s     $000121D0
[0001218a] 6100 f76a                 bsr       _set_wcolors
[0001218e] 6100 f114                 bsr       _set_windows
[00012192] 3039 0001 4dea            move.w    $00014DEA,d0
[00012198] c1fc 0044                 muls.w    #$0044,d0
[0001219c] 41f9 0001 3588            lea.l     _savecolors,a0
[000121a2] d1c0                      adda.l    d0,a0
[000121a4] 2279 0001 4df2            movea.l   _wcolors,a1
[000121aa] 7007                      moveq.l   #7,d0
[000121ac] 20d9                      move.l    (a1)+,(a0)+
[000121ae] 20d9                      move.l    (a1)+,(a0)+
[000121b0] 51c8 fffa                 dbf       d0,$000121AC
[000121b4] 20d9                      move.l    (a1)+,(a0)+
[000121b6] 2279 0001 4d88            movea.l   xcpb,a1
[000121bc] 2069 0048                 movea.l   72(a1),a0
[000121c0] 4878 0110                 pea.l     ($00000110).w
[000121c4] 4879 0001 3588            pea.l     _savecolors
[000121ca] 4e90                      jsr       (a0)
[000121cc] 504f                      addq.w    #8,a7
[000121ce] 6004                      bra.s     $000121D4
[000121d0] 426e ffda                 clr.w     -38(a6)
[000121d4] 2079 0001 4dcc            movea.l   _tree,a0
[000121da] 08a8 0000 003b            bclr      #0,59(a0)
[000121e0] 2079 0001 4dcc            movea.l   _tree,a0
[000121e6] d0fc 0040                 adda.w    #$0040,a0
[000121ea] 43ee ffd2                 lea.l     -46(a6),a1
[000121ee] 22d8                      move.l    (a0)+,(a1)+
[000121f0] 22d8                      move.l    (a0)+,(a1)+
[000121f2] 486e ffd4                 pea.l     -44(a6)
[000121f6] 486e ffd2                 pea.l     -46(a6)
[000121fa] 7002                      moveq.l   #2,d0
[000121fc] 3f00                      move.w    d0,-(a7)
[000121fe] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00012204] 4eb9 0001 31a0            jsr       _objc_offset
[0001220a] 556e ffd2                 subq.w    #2,-46(a6)
[0001220e] 556e ffd4                 subq.w    #2,-44(a6)
[00012212] 586e ffd6                 addq.w    #4,-42(a6)
[00012216] 586e ffd8                 addq.w    #4,-40(a6)
[0001221a] 486e ffd2                 pea.l     -46(a6)
[0001221e] 7008                      moveq.l   #8,d0
[00012220] 3f00                      move.w    d0,-(a7)
[00012222] 7202                      moveq.l   #2,d1
[00012224] 3f01                      move.w    d1,-(a7)
[00012226] 2f39 0001 4dcc            move.l    _tree,-(a7)
[0001222c] 4eb9 0001 3034            jsr       _Objc_draw
[00012232] 4fef 001a                 lea.l     26(a7),a7
[00012236] 6000 fc88                 bra       $00011EC0
[0001223a] 6100 f6ba                 bsr       _set_wcolors
[0001223e] 6100 f064                 bsr       _set_windows
[00012242] 3d7c 0001 ffda            move.w    #$0001,-38(a6)
[00012248] 2007                      move.l    d7,d0
[0001224a] c1fc 0018                 muls.w    #$0018,d0
[0001224e] 2079 0001 4dcc            movea.l   _tree,a0
[00012254] d1c0                      adda.l    d0,a0
[00012256] 08a8 0000 000b            bclr      #0,11(a0)
[0001225c] 6000 fc62                 bra       $00011EC0
[00012260] 3d7c 0001 ffdc            move.w    #$0001,-36(a6)
[00012266] 2079 0001 4dcc            movea.l   _tree,a0
[0001226c] 317c 003e 0090            move.w    #$003E,144(a0)
[00012272] 7018                      moveq.l   #24,d0
[00012274] 2079 0001 4dcc            movea.l   _tree,a0
[0001227a] 3140 05d0                 move.w    d0,1488(a0)
[0001227e] 4268 0240                 clr.w     576(a0)
[00012282] 2079 0001 4dcc            movea.l   _tree,a0
[00012288] 3140 0004                 move.w    d0,4(a0)
[0001228c] 317c 0040 05d8            move.w    #$0040,1496(a0)
[00012292] 2079 0001 4dcc            movea.l   _tree,a0
[00012298] 317c 0020 0248            move.w    #$0020,584(a0)
[0001229e] 4a6e ffdc                 tst.w     -36(a6)
[000122a2] 663e                      bne.s     $000122E2
[000122a4] 2079 0001 4dcc            movea.l   _tree,a0
[000122aa] 317c 0018 0090            move.w    #$0018,144(a0)
[000122b0] 703e                      moveq.l   #62,d0
[000122b2] 2079 0001 4dcc            movea.l   _tree,a0
[000122b8] 3140 0240                 move.w    d0,576(a0)
[000122bc] 7200                      moveq.l   #0,d1
[000122be] 2079 0001 4dcc            movea.l   _tree,a0
[000122c4] 3141 05d0                 move.w    d1,1488(a0)
[000122c8] 2079 0001 4dcc            movea.l   _tree,a0
[000122ce] 3140 0004                 move.w    d0,4(a0)
[000122d2] 317c 0060 05d8            move.w    #$0060,1496(a0)
[000122d8] 2079 0001 4dcc            movea.l   _tree,a0
[000122de] 3141 0248                 move.w    d1,584(a0)
[000122e2] 486e fff8                 pea.l     -8(a6)
[000122e6] 703e                      moveq.l   #62,d0
[000122e8] 3f00                      move.w    d0,-(a7)
[000122ea] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000122f0] 6100 e008                 bsr       _getrect
[000122f4] 4fef 000a                 lea.l     10(a7),a7
[000122f8] 7a00                      moveq.l   #0,d5
[000122fa] 2079 0001 4dcc            movea.l   _tree,a0
[00012300] 2248                      movea.l   a0,a1
[00012302] d2fc 05dc                 adda.w    #$05DC,a1
[00012306] 1029 0001                 move.b    1(a1),d0
[0001230a] 4a00                      tst.b     d0
[0001230c] 6a0a                      bpl.s     $00012318
[0001230e] 43e8 05dc                 lea.l     1500(a0),a1
[00012312] 1a29 0001                 move.b    1(a1),d5
[00012316] 4885                      ext.w     d5
[00012318] 43ee fff8                 lea.l     -8(a6),a1
[0001231c] 2f09                      move.l    a1,-(a7)
[0001231e] 3f05                      move.w    d5,-(a7)
[00012320] 3f05                      move.w    d5,-(a7)
[00012322] 2f09                      move.l    a1,-(a7)
[00012324] 4eb9 0001 2f02            jsr       _Rc_center
[0001232a] 4fef 000c                 lea.l     12(a7),a7
[0001232e] 2007                      move.l    d7,d0
[00012330] 33c0 0001 4dbc            move.w    d0,_curobj
[00012336] 7218                      moveq.l   #24,d1
[00012338] c1c1                      muls.w    d1,d0
[0001233a] 2079 0001 4dcc            movea.l   _tree,a0
[00012340] 2248                      movea.l   a0,a1
[00012342] d3c0                      adda.l    d0,a1
[00012344] 303c 00ff                 move.w    #$00FF,d0
[00012348] c069 0006                 and.w     6(a1),d0
[0001234c] 7415                      moveq.l   #21,d2
[0001234e] b042                      cmp.w     d2,d0
[00012350] 673a                      beq.s     $0001238C
[00012352] 7416                      moveq.l   #22,d2
[00012354] b042                      cmp.w     d2,d0
[00012356] 6734                      beq.s     $0001238C
[00012358] 3039 0001 4dbc            move.w    _curobj,d0
[0001235e] 2400                      move.l    d0,d2
[00012360] c5c1                      muls.w    d1,d2
[00012362] 2248                      movea.l   a0,a1
[00012364] d3c2                      adda.l    d2,a1
[00012366] 343c 00ff                 move.w    #$00FF,d2
[0001236a] c469 0006                 and.w     6(a1),d2
[0001236e] 761d                      moveq.l   #29,d3
[00012370] b443                      cmp.w     d3,d2
[00012372] 6718                      beq.s     $0001238C
[00012374] c1c1                      muls.w    d1,d0
[00012376] 2248                      movea.l   a0,a1
[00012378] d3c0                      adda.l    d0,a1
[0001237a] 303c 00ff                 move.w    #$00FF,d0
[0001237e] c069 0006                 and.w     6(a1),d0
[00012382] 721e                      moveq.l   #30,d1
[00012384] b041                      cmp.w     d1,d0
[00012386] 6704                      beq.s     $0001238C
[00012388] 7000                      moveq.l   #0,d0
[0001238a] 6002                      bra.s     $0001238E
[0001238c] 7001                      moveq.l   #1,d0
[0001238e] 33c0 0001 4dc8            move.w    d0,_curIsTed
[00012394] 43e8 05e0                 lea.l     1504(a0),a1
[00012398] 2f09                      move.l    a1,-(a7)
[0001239a] 3f39 0001 4dbc            move.w    _curobj,-(a7)
[000123a0] 2f08                      move.l    a0,-(a7)
[000123a2] 6100 df56                 bsr       _getrect
[000123a6] 486e ffea                 pea.l     -22(a6)
[000123aa] 486e ffec                 pea.l     -20(a6)
[000123ae] 4267                      clr.w     -(a7)
[000123b0] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000123b6] 4eb9 0001 31a0            jsr       _objc_offset
[000123bc] 4fef 0018                 lea.l     24(a7),a7
[000123c0] 302e ffec                 move.w    -20(a6),d0
[000123c4] 2079 0001 4dcc            movea.l   _tree,a0
[000123ca] 9168 05e0                 sub.w     d0,1504(a0)
[000123ce] 302e ffea                 move.w    -22(a6),d0
[000123d2] 2079 0001 4dcc            movea.l   _tree,a0
[000123d8] 9168 05e2                 sub.w     d0,1506(a0)
[000123dc] 0c79 0330 0001 4df0       cmpi.w    #$0330,AES_Version
[000123e4] 6d4c                      blt.s     $00012432
[000123e6] 0c79 0008 0001 4de8       cmpi.w    #$0008,$00014DE8
[000123ee] 6f42                      ble.s     $00012432
[000123f0] 3039 0001 4dbc            move.w    _curobj,d0
[000123f6] c1fc 0018                 muls.w    #$0018,d0
[000123fa] 2079 0001 4dcc            movea.l   _tree,a0
[00012400] d1c0                      adda.l    d0,a0
[00012402] 0828 0001 0008            btst      #1,8(a0)
[00012408] 6728                      beq.s     $00012432
[0001240a] 2079 0001 4dcc            movea.l   _tree,a0
[00012410] 5568 05e0                 subq.w    #2,1504(a0)
[00012414] 2079 0001 4dcc            movea.l   _tree,a0
[0001241a] 5568 05e2                 subq.w    #2,1506(a0)
[0001241e] 2079 0001 4dcc            movea.l   _tree,a0
[00012424] 5868 05e4                 addq.w    #4,1508(a0)
[00012428] 2079 0001 4dcc            movea.l   _tree,a0
[0001242e] 5868 05e6                 addq.w    #4,1510(a0)
[00012432] 2079 0001 4dcc            movea.l   _tree,a0
[00012438] 2248                      movea.l   a0,a1
[0001243a] d2fc 05e0                 adda.w    #$05E0,a1
[0001243e] 45e8 05e0                 lea.l     1504(a0),a2
[00012442] 2f0a                      move.l    a2,-(a7)
[00012444] 70fd                      moveq.l   #-3,d0
[00012446] 3f00                      move.w    d0,-(a7)
[00012448] 3f00                      move.w    d0,-(a7)
[0001244a] 2f09                      move.l    a1,-(a7)
[0001244c] 4eb9 0001 2f02            jsr       _Rc_center
[00012452] 486e fff0                 pea.l     -16(a6)
[00012456] 703e                      moveq.l   #62,d0
[00012458] 3f00                      move.w    d0,-(a7)
[0001245a] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00012460] 6100 de98                 bsr       _getrect
[00012464] 41ee fff0                 lea.l     -16(a6),a0
[00012468] 2e88                      move.l    a0,(a7)
[0001246a] 3f05                      move.w    d5,-(a7)
[0001246c] 3f05                      move.w    d5,-(a7)
[0001246e] 2f08                      move.l    a0,-(a7)
[00012470] 4eb9 0001 2f02            jsr       _Rc_center
[00012476] 6100 eaf2                 bsr       _set_info
[0001247a] 486e fff8                 pea.l     -8(a6)
[0001247e] 7008                      moveq.l   #8,d0
[00012480] 3f00                      move.w    d0,-(a7)
[00012482] 4267                      clr.w     -(a7)
[00012484] 2f39 0001 4dcc            move.l    _tree,-(a7)
[0001248a] 4eb9 0001 3034            jsr       _Objc_draw
[00012490] 486e fff0                 pea.l     -16(a6)
[00012494] 7008                      moveq.l   #8,d0
[00012496] 3f00                      move.w    d0,-(a7)
[00012498] 4267                      clr.w     -(a7)
[0001249a] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000124a0] 4eb9 0001 3034            jsr       _Objc_draw
[000124a6] 6100 de00                 bsr       _wait_bup
[000124aa] 4fef 0036                 lea.l     54(a7),a7
[000124ae] 6000 fa10                 bra       $00011EC0
[000124b2] 486e ffea                 pea.l     -22(a6)
[000124b6] 486e ffec                 pea.l     -20(a6)
[000124ba] 3f07                      move.w    d7,-(a7)
[000124bc] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000124c2] 4eb9 0001 31a0            jsr       _objc_offset
[000124c8] 4fef 000e                 lea.l     14(a7),a7
[000124cc] 2c07                      move.l    d7,d6
[000124ce] 0446 0028                 subi.w    #$0028,d6
[000124d2] 2039 0001 4dc4            move.l    _curcolor,d0
[000124d8] 4240                      clr.w     d0
[000124da] 4840                      swap      d0
[000124dc] e888                      lsr.l     #4,d0
[000124de] 0280 0000 0007            andi.l    #$00000007,d0
[000124e4] 2206                      move.l    d6,d1
[000124e6] 48c1                      ext.l     d1
[000124e8] b081                      cmp.l     d1,d0
[000124ea] 6700 00a8                 beq       $00012594
[000124ee] 2006                      move.l    d6,d0
[000124f0] 48c0                      ext.l     d0
[000124f2] 4840                      swap      d0
[000124f4] 4240                      clr.w     d0
[000124f6] e988                      lsl.l     #4,d0
[000124f8] 0280 0070 0000            andi.l    #$00700000,d0
[000124fe] 2239 0001 4dc4            move.l    _curcolor,d1
[00012504] 0281 ff8f ffff            andi.l    #$FF8FFFFF,d1
[0001250a] 8280                      or.l      d0,d1
[0001250c] 23c1 0001 4dc4            move.l    d1,_curcolor
[00012512] 4a79 0001 4dc8            tst.w     _curIsTed
[00012518] 673c                      beq.s     $00012556
[0001251a] 3039 0001 4dbc            move.w    _curobj,d0
[00012520] 2200                      move.l    d0,d1
[00012522] 7418                      moveq.l   #24,d2
[00012524] c3c2                      muls.w    d2,d1
[00012526] 2079 0001 4dcc            movea.l   _tree,a0
[0001252c] 2248                      movea.l   a0,a1
[0001252e] d3c1                      adda.l    d1,a1
[00012530] 2469 000c                 movea.l   12(a1),a2
[00012534] 43ea 0012                 lea.l     18(a2),a1
[00012538] 2206                      move.l    d6,d1
[0001253a] 48c1                      ext.l     d1
[0001253c] 4841                      swap      d1
[0001253e] 4241                      clr.w     d1
[00012540] e989                      lsl.l     #4,d1
[00012542] 0281 0070 0000            andi.l    #$00700000,d1
[00012548] 2611                      move.l    (a1),d3
[0001254a] 0283 ff8f ffff            andi.l    #$FF8FFFFF,d3
[00012550] 8681                      or.l      d1,d3
[00012552] 2283                      move.l    d3,(a1)
[00012554] 6036                      bra.s     $0001258C
[00012556] 3039 0001 4dbc            move.w    _curobj,d0
[0001255c] c1fc 0018                 muls.w    #$0018,d0
[00012560] 2079 0001 4dcc            movea.l   _tree,a0
[00012566] d1c0                      adda.l    d0,a0
[00012568] 43e8 000c                 lea.l     12(a0),a1
[0001256c] 2006                      move.l    d6,d0
[0001256e] 48c0                      ext.l     d0
[00012570] 4840                      swap      d0
[00012572] 4240                      clr.w     d0
[00012574] e988                      lsl.l     #4,d0
[00012576] 0280 0070 0000            andi.l    #$00700000,d0
[0001257c] 2229 0002                 move.l    2(a1),d1
[00012580] 0281 ff8f ffff            andi.l    #$FF8FFFFF,d1
[00012586] 8280                      or.l      d0,d1
[00012588] 2341 0002                 move.l    d1,2(a1)
[0001258c] 6100 e310                 bsr       _draw_fills
[00012590] 6100 e578                 bsr       _draw_curobj
[00012594] 486e ffde                 pea.l     -34(a6)
[00012598] 4eb9 0001 3098            jsr       _Graf_mkstate
[0001259e] 3eae ffea                 move.w    -22(a6),(a7)
[000125a2] 3f2e ffde                 move.w    -34(a6),-(a7)
[000125a6] 7008                      moveq.l   #8,d0
[000125a8] 3f00                      move.w    d0,-(a7)
[000125aa] 7227                      moveq.l   #39,d1
[000125ac] 3f01                      move.w    d1,-(a7)
[000125ae] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000125b4] 4eb9 0001 31dc            jsr       objc_find
[000125ba] 4fef 000e                 lea.l     14(a7),a7
[000125be] 2e00                      move.l    d0,d7
[000125c0] 70ff                      moveq.l   #-1,d0
[000125c2] be40                      cmp.w     d0,d7
[000125c4] 6618                      bne.s     $000125DE
[000125c6] 2039 0001 4dc4            move.l    _curcolor,d0
[000125cc] 4240                      clr.w     d0
[000125ce] 4840                      swap      d0
[000125d0] e888                      lsr.l     #4,d0
[000125d2] 0280 0000 0007            andi.l    #$00000007,d0
[000125d8] 2e00                      move.l    d0,d7
[000125da] 0647 0028                 addi.w    #$0028,d7
[000125de] 4a6e ffe2                 tst.w     -30(a6)
[000125e2] 6600 fee8                 bne       $000124CC
[000125e6] 6000 f8d8                 bra       $00011EC0
[000125ea] 2279 0001 4dcc            movea.l   _tree,a1
[000125f0] 2069 039c                 movea.l   924(a1),a0
[000125f4] 45e8 0012                 lea.l     18(a0),a2
[000125f8] 082a 0007 0001            btst      #7,1(a2)
[000125fe] 670c                      beq.s     $0001260C
[00012600] 45e8 0012                 lea.l     18(a0),a2
[00012604] 08aa 0007 0001            bclr      #7,1(a2)
[0001260a] 600a                      bra.s     $00012616
[0001260c] 45e8 0012                 lea.l     18(a0),a2
[00012610] 08ea 0007 0001            bset      #7,1(a2)
[00012616] 2279 0001 4dcc            movea.l   _tree,a1
[0001261c] 2069 039c                 movea.l   924(a1),a0
[00012620] 45e8 0012                 lea.l     18(a0),a2
[00012624] 2012                      move.l    (a2),d0
[00012626] 4240                      clr.w     d0
[00012628] 4840                      swap      d0
[0001262a] ee88                      lsr.l     #7,d0
[0001262c] 0280 0000 0001            andi.l    #$00000001,d0
[00012632] 4840                      swap      d0
[00012634] 4240                      clr.w     d0
[00012636] ef88                      lsl.l     #7,d0
[00012638] 0280 0080 0000            andi.l    #$00800000,d0
[0001263e] 2239 0001 4dc4            move.l    _curcolor,d1
[00012644] 0281 ff7f ffff            andi.l    #$FF7FFFFF,d1
[0001264a] 8280                      or.l      d0,d1
[0001264c] 23c1 0001 4dc4            move.l    d1,_curcolor
[00012652] 4241                      clr.w     d1
[00012654] 4841                      swap      d1
[00012656] ee89                      lsr.l     #7,d1
[00012658] 0281 0000 0001            andi.l    #$00000001,d1
[0001265e] 4a79 0001 4dc8            tst.w     _curIsTed
[00012664] 6732                      beq.s     $00012698
[00012666] 3039 0001 4dbc            move.w    _curobj,d0
[0001266c] 2400                      move.l    d0,d2
[0001266e] 7618                      moveq.l   #24,d3
[00012670] c5c3                      muls.w    d3,d2
[00012672] 2049                      movea.l   a1,a0
[00012674] d1c2                      adda.l    d2,a0
[00012676] 2468 000c                 movea.l   12(a0),a2
[0001267a] 41ea 0012                 lea.l     18(a2),a0
[0001267e] 4841                      swap      d1
[00012680] 4241                      clr.w     d1
[00012682] ef89                      lsl.l     #7,d1
[00012684] 0281 0080 0000            andi.l    #$00800000,d1
[0001268a] 2410                      move.l    (a0),d2
[0001268c] 0282 ff7f ffff            andi.l    #$FF7FFFFF,d2
[00012692] 8481                      or.l      d1,d2
[00012694] 2082                      move.l    d2,(a0)
[00012696] 6040                      bra.s     $000126D8
[00012698] 3039 0001 4dbc            move.w    _curobj,d0
[0001269e] c1fc 0018                 muls.w    #$0018,d0
[000126a2] 2049                      movea.l   a1,a0
[000126a4] d1c0                      adda.l    d0,a0
[000126a6] 45e8 000c                 lea.l     12(a0),a2
[000126aa] 2039 0001 4dc4            move.l    _curcolor,d0
[000126b0] 4240                      clr.w     d0
[000126b2] 4840                      swap      d0
[000126b4] ee88                      lsr.l     #7,d0
[000126b6] 0280 0000 0001            andi.l    #$00000001,d0
[000126bc] 4840                      swap      d0
[000126be] 4240                      clr.w     d0
[000126c0] ef88                      lsl.l     #7,d0
[000126c2] 0280 0080 0000            andi.l    #$00800000,d0
[000126c8] 222a 0002                 move.l    2(a2),d1
[000126cc] 0281 ff7f ffff            andi.l    #$FF7FFFFF,d1
[000126d2] 8280                      or.l      d0,d1
[000126d4] 2541 0002                 move.l    d1,2(a2)
[000126d8] 42a7                      clr.l     -(a7)
[000126da] 7008                      moveq.l   #8,d0
[000126dc] 3f00                      move.w    d0,-(a7)
[000126de] 7226                      moveq.l   #38,d1
[000126e0] 3f01                      move.w    d1,-(a7)
[000126e2] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000126e8] 4eb9 0001 3034            jsr       _Objc_draw
[000126ee] 6100 e41a                 bsr       _draw_curobj
[000126f2] 6100 dbb4                 bsr       _wait_bup
[000126f6] 4fef 000c                 lea.l     12(a7),a7
[000126fa] 6000 f7c4                 bra       $00011EC0
[000126fe] 41fa e7b0                 lea.l     _do_bordslid(pc),a0
[00012702] 2f08                      move.l    a0,-(a7)
[00012704] 4879 0001 4dbe            pea.l     _curcborder
[0001270a] 700f                      moveq.l   #15,d0
[0001270c] 3f00                      move.w    d0,-(a7)
[0001270e] 4267                      clr.w     -(a7)
[00012710] 7201                      moveq.l   #1,d1
[00012712] 3f01                      move.w    d1,-(a7)
[00012714] 7421                      moveq.l   #33,d2
[00012716] 3f02                      move.w    d2,-(a7)
[00012718] 7620                      moveq.l   #32,d3
[0001271a] 3f03                      move.w    d3,-(a7)
[0001271c] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00012722] 6100 e028                 bsr       sl_pagex
[00012726] 4fef 0016                 lea.l     22(a7),a7
[0001272a] 6000 f794                 bra       $00011EC0
[0001272e] 3f39 0001 4dbe            move.w    _curcborder,-(a7)
[00012734] 7021                      moveq.l   #33,d0
[00012736] 3f00                      move.w    d0,-(a7)
[00012738] 6100 e6c6                 bsr       _fix_elev
[0001273c] 41fa e772                 lea.l     _do_bordslid(pc),a0
[00012740] 2e88                      move.l    a0,(a7)
[00012742] 4879 0001 4dbe            pea.l     _curcborder
[00012748] 700f                      moveq.l   #15,d0
[0001274a] 3f00                      move.w    d0,-(a7)
[0001274c] 4267                      clr.w     -(a7)
[0001274e] 7221                      moveq.l   #33,d1
[00012750] 3f01                      move.w    d1,-(a7)
[00012752] 7420                      moveq.l   #32,d2
[00012754] 3f02                      move.w    d2,-(a7)
[00012756] 2f39 0001 4dcc            move.l    _tree,-(a7)
[0001275c] 6100 de50                 bsr       sl_dragx
[00012760] 4fef 0014                 lea.l     20(a7),a7
[00012764] 6000 f75a                 bra       $00011EC0
[00012768] 41fa e786                 lea.l     _do_textslid(pc),a0
[0001276c] 2f08                      move.l    a0,-(a7)
[0001276e] 4879 0001 4dc0            pea.l     _curctext
[00012774] 700f                      moveq.l   #15,d0
[00012776] 3f00                      move.w    d0,-(a7)
[00012778] 4267                      clr.w     -(a7)
[0001277a] 7201                      moveq.l   #1,d1
[0001277c] 3f01                      move.w    d1,-(a7)
[0001277e] 7423                      moveq.l   #35,d2
[00012780] 3f02                      move.w    d2,-(a7)
[00012782] 7622                      moveq.l   #34,d3
[00012784] 3f03                      move.w    d3,-(a7)
[00012786] 2f39 0001 4dcc            move.l    _tree,-(a7)
[0001278c] 6100 dfbe                 bsr       sl_pagex
[00012790] 4fef 0016                 lea.l     22(a7),a7
[00012794] 6000 f72a                 bra       $00011EC0
[00012798] 3f39 0001 4dc0            move.w    _curctext,-(a7)
[0001279e] 7023                      moveq.l   #35,d0
[000127a0] 3f00                      move.w    d0,-(a7)
[000127a2] 6100 e65c                 bsr       _fix_elev
[000127a6] 41fa e748                 lea.l     _do_textslid(pc),a0
[000127aa] 2e88                      move.l    a0,(a7)
[000127ac] 4879 0001 4dc0            pea.l     _curctext
[000127b2] 700f                      moveq.l   #15,d0
[000127b4] 3f00                      move.w    d0,-(a7)
[000127b6] 4267                      clr.w     -(a7)
[000127b8] 7223                      moveq.l   #35,d1
[000127ba] 3f01                      move.w    d1,-(a7)
[000127bc] 7422                      moveq.l   #34,d2
[000127be] 3f02                      move.w    d2,-(a7)
[000127c0] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000127c6] 6100 dde6                 bsr       _sl_dragx
[000127ca] 4fef 0014                 lea.l     20(a7),a7
[000127ce] 6000 f6f0                 bra       $00011EC0
[000127d2] 41fa e75a                 lea.l     _do_fillslid(pc),a0
[000127d6] 2f08                      move.l    a0,-(a7)
[000127d8] 4879 0001 4dc2            pea.l     $00014DC2
[000127de] 700f                      moveq.l   #15,d0
[000127e0] 3f00                      move.w    d0,-(a7)
[000127e2] 4267                      clr.w     -(a7)
[000127e4] 7201                      moveq.l   #1,d1
[000127e6] 3f01                      move.w    d1,-(a7)
[000127e8] 7425                      moveq.l   #37,d2
[000127ea] 3f02                      move.w    d2,-(a7)
[000127ec] 7624                      moveq.l   #36,d3
[000127ee] 3f03                      move.w    d3,-(a7)
[000127f0] 2f39 0001 4dcc            move.l    _tree,-(a7)
[000127f6] 6100 df54                 bsr       sl_pagex
[000127fa] 4fef 0016                 lea.l     22(a7),a7
[000127fe] 6000 f6c0                 bra       $00011EC0
[00012802] 3f39 0001 4dc2            move.w    $00014DC2,-(a7)
[00012808] 7025                      moveq.l   #37,d0
[0001280a] 3f00                      move.w    d0,-(a7)
[0001280c] 6100 e5f2                 bsr       _fix_elev
[00012810] 41fa e71c                 lea.l     _do_fillslid(pc),a0
[00012814] 2e88                      move.l    a0,(a7)
[00012816] 4879 0001 4dc2            pea.l     $00014DC2
[0001281c] 700f                      moveq.l   #15,d0
[0001281e] 3f00                      move.w    d0,-(a7)
[00012820] 4267                      clr.w     -(a7)
[00012822] 7225                      moveq.l   #37,d1
[00012824] 3f01                      move.w    d1,-(a7)
[00012826] 7424                      moveq.l   #36,d2
[00012828] 3f02                      move.w    d2,-(a7)
[0001282a] 2f39 0001 4dcc            move.l    _tree,-(a7)
[00012830] 6100 dd7c                 bsr       sl_dragx
[00012834] 4fef 0014                 lea.l     20(a7),a7
[00012838] 6000 f686                 bra       $00011EC0
[0001283c] 6100 f43c                 bsr       _drag_curobj
[00012840] 6000 f67e                 bra       $00011EC0
[00012844] 4cdf 04ec                 movem.l   (a7)+,d2-d3/d5-d7/a2
[00012848] 4e5e                      unlk      a6
[0001284a] 4e75                      rts
_set_texts:
[0001284c] 48e7 0324                 movem.l   d6-d7/a2/a5,-(a7)
[00012850] 3e2f 0014                 move.w    20(a7),d7
[00012854] 2a79 0001 4d0e            movea.l   $00014D0E,a5
[0001285a] 2007                      move.l    d7,d0
[0001285c] 5340                      subq.w    #1,d0
[0001285e] 6d00 0286                 blt       $00012AE6
[00012862] 0c40 0006                 cmpi.w    #$0006,d0
[00012866] 6c00 027e                 bge       $00012AE6
[0001286a] d040                      add.w     d0,d0
[0001286c] 303b 0006                 move.w    $00012874(pc,d0.w),d0
[00012870] 4efb 0004                 jmp       $00012876(pc,d0.w)
J4:
[00012874] 000a                      dc.w $000a   ; $00012880-$00012874-2
[00012876] 0086                      dc.w $0086   ; $000128fc-$00012874-2
[00012878] 0270                      dc.w $0270   ; $00012ae6-$00012874-2
[0001287a] 0102                      dc.w $0102   ; $00012978-$00012874-2
[0001287c] 017e                      dc.w $017e   ; $000129f4-$00012874-2
[0001287e] 01f8                      dc.w $01f8   ; $00012a6e-$00012874-2
[00012880] 2279 0001 4dcc            movea.l   _tree,a1
[00012886] 2069 02ac                 movea.l   684(a1),a0
[0001288a] 20ad 01a4                 move.l    420(a5),(a0)
[0001288e] 2279 0001 4dcc            movea.l   _tree,a1
[00012894] 2069 02c4                 movea.l   708(a1),a0
[00012898] 20ad 01bc                 move.l    444(a5),(a0)
[0001289c] 2279 0001 4dcc            movea.l   _tree,a1
[000128a2] 2069 048c                 movea.l   1164(a1),a0
[000128a6] 20ad 01d4                 move.l    468(a5),(a0)
[000128aa] 2279 0001 4dcc            movea.l   _tree,a1
[000128b0] 2069 04a4                 movea.l   1188(a1),a0
[000128b4] 20ad 01ec                 move.l    492(a5),(a0)
[000128b8] 2279 0001 4dcc            movea.l   _tree,a1
[000128be] 2069 04bc                 movea.l   1212(a1),a0
[000128c2] 20ad 0204                 move.l    516(a5),(a0)
[000128c6] 2279 0001 4dcc            movea.l   _tree,a1
[000128cc] 2069 039c                 movea.l   924(a1),a0
[000128d0] 20ad 021c                 move.l    540(a5),(a0)
[000128d4] 2079 0001 4dcc            movea.l   _tree,a0
[000128da] 216d 015c 006c            move.l    348(a5),108(a0)
[000128e0] 2079 0001 4dcc            movea.l   _tree,a0
[000128e6] 216d 0174 0084            move.l    372(a5),132(a0)
[000128ec] 2079 0001 4dcc            movea.l   _tree,a0
[000128f2] 216d 018c 003c            move.l    396(a5),60(a0)
[000128f8] 6000 01ec                 bra       $00012AE6
[000128fc] 2279 0001 4dcc            movea.l   _tree,a1
[00012902] 2069 02ac                 movea.l   684(a1),a0
[00012906] 20ad 02ac                 move.l    684(a5),(a0)
[0001290a] 2279 0001 4dcc            movea.l   _tree,a1
[00012910] 2069 02c4                 movea.l   708(a1),a0
[00012914] 20ad 02c4                 move.l    708(a5),(a0)
[00012918] 2279 0001 4dcc            movea.l   _tree,a1
[0001291e] 2069 048c                 movea.l   1164(a1),a0
[00012922] 20ad 02dc                 move.l    732(a5),(a0)
[00012926] 2279 0001 4dcc            movea.l   _tree,a1
[0001292c] 2069 04a4                 movea.l   1188(a1),a0
[00012930] 20ad 02f4                 move.l    756(a5),(a0)
[00012934] 2279 0001 4dcc            movea.l   _tree,a1
[0001293a] 2069 04bc                 movea.l   1212(a1),a0
[0001293e] 20ad 030c                 move.l    780(a5),(a0)
[00012942] 2279 0001 4dcc            movea.l   _tree,a1
[00012948] 2069 039c                 movea.l   924(a1),a0
[0001294c] 20ad 0324                 move.l    804(a5),(a0)
[00012950] 2079 0001 4dcc            movea.l   _tree,a0
[00012956] 216d 0264 006c            move.l    612(a5),108(a0)
[0001295c] 2079 0001 4dcc            movea.l   _tree,a0
[00012962] 216d 027c 0084            move.l    636(a5),132(a0)
[00012968] 2079 0001 4dcc            movea.l   _tree,a0
[0001296e] 216d 0294 003c            move.l    660(a5),60(a0)
[00012974] 6000 0170                 bra       $00012AE6
[00012978] 2279 0001 4dcc            movea.l   _tree,a1
[0001297e] 2069 02ac                 movea.l   684(a1),a0
[00012982] 20ad 03b4                 move.l    948(a5),(a0)
[00012986] 2279 0001 4dcc            movea.l   _tree,a1
[0001298c] 2069 02c4                 movea.l   708(a1),a0
[00012990] 20ad 03cc                 move.l    972(a5),(a0)
[00012994] 2279 0001 4dcc            movea.l   _tree,a1
[0001299a] 2069 048c                 movea.l   1164(a1),a0
[0001299e] 20ad 03e4                 move.l    996(a5),(a0)
[000129a2] 2279 0001 4dcc            movea.l   _tree,a1
[000129a8] 2069 04a4                 movea.l   1188(a1),a0
[000129ac] 20ad 03fc                 move.l    1020(a5),(a0)
[000129b0] 2279 0001 4dcc            movea.l   _tree,a1
[000129b6] 2069 04bc                 movea.l   1212(a1),a0
[000129ba] 20ad 0414                 move.l    1044(a5),(a0)
[000129be] 2279 0001 4dcc            movea.l   _tree,a1
[000129c4] 2069 039c                 movea.l   924(a1),a0
[000129c8] 20ad 042c                 move.l    1068(a5),(a0)
[000129cc] 2079 0001 4dcc            movea.l   _tree,a0
[000129d2] 216d 036c 006c            move.l    876(a5),108(a0)
[000129d8] 2079 0001 4dcc            movea.l   _tree,a0
[000129de] 216d 0384 0084            move.l    900(a5),132(a0)
[000129e4] 2079 0001 4dcc            movea.l   _tree,a0
[000129ea] 216d 039c 003c            move.l    924(a5),60(a0)
[000129f0] 6000 00f4                 bra       $00012AE6
[000129f4] 2279 0001 4dcc            movea.l   _tree,a1
[000129fa] 2069 02ac                 movea.l   684(a1),a0
[000129fe] 20ad 04bc                 move.l    1212(a5),(a0)
[00012a02] 2279 0001 4dcc            movea.l   _tree,a1
[00012a08] 2069 02c4                 movea.l   708(a1),a0
[00012a0c] 20ad 04d4                 move.l    1236(a5),(a0)
[00012a10] 2279 0001 4dcc            movea.l   _tree,a1
[00012a16] 2069 048c                 movea.l   1164(a1),a0
[00012a1a] 20ad 04ec                 move.l    1260(a5),(a0)
[00012a1e] 2279 0001 4dcc            movea.l   _tree,a1
[00012a24] 2069 04a4                 movea.l   1188(a1),a0
[00012a28] 20ad 0504                 move.l    1284(a5),(a0)
[00012a2c] 2279 0001 4dcc            movea.l   _tree,a1
[00012a32] 2069 04bc                 movea.l   1212(a1),a0
[00012a36] 20ad 051c                 move.l    1308(a5),(a0)
[00012a3a] 2279 0001 4dcc            movea.l   _tree,a1
[00012a40] 2069 039c                 movea.l   924(a1),a0
[00012a44] 20ad 0534                 move.l    1332(a5),(a0)
[00012a48] 2079 0001 4dcc            movea.l   _tree,a0
[00012a4e] 216d 0474 006c            move.l    1140(a5),108(a0)
[00012a54] 2079 0001 4dcc            movea.l   _tree,a0
[00012a5a] 216d 048c 0084            move.l    1164(a5),132(a0)
[00012a60] 2079 0001 4dcc            movea.l   _tree,a0
[00012a66] 216d 04a4 003c            move.l    1188(a5),60(a0)
[00012a6c] 6078                      bra.s     $00012AE6
[00012a6e] 2279 0001 4dcc            movea.l   _tree,a1
[00012a74] 2069 02ac                 movea.l   684(a1),a0
[00012a78] 20ad 05c4                 move.l    1476(a5),(a0)
[00012a7c] 2279 0001 4dcc            movea.l   _tree,a1
[00012a82] 2069 02c4                 movea.l   708(a1),a0
[00012a86] 20ad 05dc                 move.l    1500(a5),(a0)
[00012a8a] 2279 0001 4dcc            movea.l   _tree,a1
[00012a90] 2069 048c                 movea.l   1164(a1),a0
[00012a94] 20ad 05f4                 move.l    1524(a5),(a0)
[00012a98] 2279 0001 4dcc            movea.l   _tree,a1
[00012a9e] 2069 04a4                 movea.l   1188(a1),a0
[00012aa2] 20ad 060c                 move.l    1548(a5),(a0)
[00012aa6] 2279 0001 4dcc            movea.l   _tree,a1
[00012aac] 2069 04bc                 movea.l   1212(a1),a0
[00012ab0] 20ad 0624                 move.l    1572(a5),(a0)
[00012ab4] 2279 0001 4dcc            movea.l   _tree,a1
[00012aba] 2069 039c                 movea.l   924(a1),a0
[00012abe] 20ad 063c                 move.l    1596(a5),(a0)
[00012ac2] 2079 0001 4dcc            movea.l   _tree,a0
[00012ac8] 216d 057c 006c            move.l    1404(a5),108(a0)
[00012ace] 2079 0001 4dcc            movea.l   _tree,a0
[00012ad4] 216d 0594 0084            move.l    1428(a5),132(a0)
[00012ada] 2079 0001 4dcc            movea.l   _tree,a0
[00012ae0] 216d 05ac 003c            move.l    1452(a5),60(a0)
[00012ae6] 2279 0001 4dcc            movea.l   _tree,a1
[00012aec] 2069 02ac                 movea.l   684(a1),a0
[00012af0] 2250                      movea.l   (a0),a1
[00012af2] 4a19                      tst.b     (a1)+
[00012af4] 66fc                      bne.s     $00012AF2
[00012af6] 5389                      subq.l    #1,a1
[00012af8] 93d0                      suba.l    (a0),a1
[00012afa] 2c09                      move.l    a1,d6
[00012afc] 2279 0001 4dcc            movea.l   _tree,a1
[00012b02] 2069 00cc                 movea.l   204(a1),a0
[00012b06] 2269 02ac                 movea.l   684(a1),a1
[00012b0a] 2451                      movea.l   (a1),a2
[00012b0c] d4c6                      adda.w    d6,a2
[00012b0e] 594a                      subq.w    #4,a2
[00012b10] 208a                      move.l    a2,(a0)
[00012b12] 2279 0001 4dcc            movea.l   _tree,a1
[00012b18] 2069 02c4                 movea.l   708(a1),a0
[00012b1c] 2250                      movea.l   (a0),a1
[00012b1e] 4a19                      tst.b     (a1)+
[00012b20] 66fc                      bne.s     $00012B1E
[00012b22] 5389                      subq.l    #1,a1
[00012b24] 93d0                      suba.l    (a0),a1
[00012b26] 2c09                      move.l    a1,d6
[00012b28] 2279 0001 4dcc            movea.l   _tree,a1
[00012b2e] 2069 00fc                 movea.l   252(a1),a0
[00012b32] 2269 02c4                 movea.l   708(a1),a1
[00012b36] 2451                      movea.l   (a1),a2
[00012b38] d4c6                      adda.w    d6,a2
[00012b3a] 594a                      subq.w    #4,a2
[00012b3c] 208a                      move.l    a2,(a0)
[00012b3e] 4cdf 24c0                 movem.l   (a7)+,d6-d7/a2/a5
[00012b42] 4e75                      rts
_cpx_init:
[00012b44] 4e56 ff8c                 link      a6,#$FF8C
[00012b48] 48e7 2004                 movem.l   d2/a5,-(a7)
[00012b4c] 2a6e 0008                 movea.l   8(a6),a5
[00012b50] 23cd 0001 4d88            move.l    a5,xcpb
[00012b56] 23fc 0001 5090 0001 5078  move.l    #_AEScontrol,__AESpb+0
[00012b60] 23fc 0001 509a 0001 507c  move.l    #_AESglobal,__AESpb+4
[00012b6a] 23fc 0001 50b8 0001 5080  move.l    #_AESintin,__AESpb+8
[00012b74] 23fc 0001 50d8 0001 5084  move.l    #_AESintout,__AESpb+12
[00012b7e] 23fc 0001 50e6 0001 5088  move.l    #_AESaddrin,__AESpb+16
[00012b88] 23fc 0001 50f2 0001 508c  move.l    #_AESaddrout,__AESpb+20
[00012b92] 7000                      moveq.l   #0,d0
[00012b94] 4eb9 0001 32c4            jsr       @_AESif ; appl_init
[00012b9a] 3239 0001 509a            move.w    _AESglobal,d1
[00012ba0] 0c41 0300                 cmpi.w    #$0300,d1
[00012ba4] 5cc0                      sge       d0
[00012ba6] 4400                      neg.b     d0
[00012ba8] 4880                      ext.w     d0
[00012baa] 33c0 0001 4dca            move.w    d0,$00014DCA
[00012bb0] 33c1 0001 4df0            move.w    d1,AES_Version
[00012bb6] 486e ff8e                 pea.l     -114(a6)
[00012bba] 4267                      clr.w     -(a7)
[00012bbc] 2079 0001 4d88            movea.l   xcpb,a0
[00012bc2] 3f10                      move.w    (a0),-(a7)
[00012bc4] 4eb9 0001 3114            jsr       vq_extnd
[00012bca] 504f                      addq.w    #8,a7
[00012bcc] 302e ffa8                 move.w    -88(a6),d0
[00012bd0] 33c0 0001 4de8            move.w    d0,$00014DE8
[00012bd6] 7000                      moveq.l   #0,d0
[00012bd8] 33c0 0001 4dea            move.w    d0,$00014DEA
[00012bde] 3039 0001 4de8            move.w    $00014DE8,d0
[00012be4] 7202                      moveq.l   #2,d1
[00012be6] b041                      cmp.w     d1,d0
[00012be8] 6f06                      ble.s     $00012BF0
[00012bea] 5279 0001 4dea            addq.w    #1,$00014DEA
[00012bf0] 7204                      moveq.l   #4,d1
[00012bf2] b041                      cmp.w     d1,d0
[00012bf4] 6f06                      ble.s     $00012BFC
[00012bf6] 5279 0001 4dea            addq.w    #1,$00014DEA
[00012bfc] 2279 0001 4d88            movea.l   xcpb,a1
[00012c02] 2069 004c                 movea.l   76(a1),a0
[00012c06] 4e90                      jsr       (a0)
[00012c08] 23c0 0001 4df2            move.l    d0,_wcolors
[00012c0e] 2079 0001 4d88            movea.l   xcpb,a0
[00012c14] 4a68 0002                 tst.w     2(a0)
[00012c18] 6732                      beq.s     $00012C4C
[00012c1a] 4a79 0001 4dca            tst.w     $00014DCA
[00012c20] 6700 018c                 beq       $00012DAE
[00012c24] 3239 0001 4dea            move.w    $00014DEA,d1
[00012c2a] c3fc 0044                 muls.w    #$0044,d1
[00012c2e] 41f9 0001 3588            lea.l     _savecolors,a0
[00012c34] d1c1                      adda.l    d1,a0
[00012c36] 2240                      movea.l   d0,a1
[00012c38] 7007                      moveq.l   #7,d0
[00012c3a] 22d8                      move.l    (a0)+,(a1)+
[00012c3c] 22d8                      move.l    (a0)+,(a1)+
[00012c3e] 51c8 fffa                 dbf       d0,$00012C3A
[00012c42] 22d8                      move.l    (a0)+,(a1)+
[00012c44] 6100 e65e                 bsr       _set_windows
[00012c48] 6000 0164                 bra       $00012DAE
[00012c4c] 4a68 0006                 tst.w     6(a0)
[00012c50] 6650                      bne.s     $00012CA2
[00012c52] 2068 0010                 movea.l   16(a0),a0
[00012c56] 4879 0001 4d12            pea.l     $00014D12
[00012c5c] 4879 0001 4d0a            pea.l     $00014D0A
[00012c62] 4879 0001 3f80            pea.l     $00013F80
[00012c68] 4879 0001 3f5a            pea.l     $00013F5A
[00012c6e] 4879 0001 3f72            pea.l     $00013F72
[00012c74] 4879 0001 3f84            pea.l     $00013F84
[00012c7a] 4879 0001 3dc2            pea.l     $00013DC2
[00012c80] 4879 0001 3fa6            pea.l     $00013FA6
[00012c86] 4879 0001 40da            pea.l     $000140DA
[00012c8c] 7002                      moveq.l   #2,d0
[00012c8e] 3f00                      move.w    d0,-(a7)
[00012c90] 4267                      clr.w     -(a7)
[00012c92] 7206                      moveq.l   #6,d1
[00012c94] 3f01                      move.w    d1,-(a7)
[00012c96] 343c 0082                 move.w    #$0082,d2
[00012c9a] 3f02                      move.w    d2,-(a7)
[00012c9c] 4e90                      jsr       (a0)
[00012c9e] 4fef 002c                 lea.l     44(a7),a7
[00012ca2] 4a79 0001 4dca            tst.w     $00014DCA
[00012ca8] 6750                      beq.s     $00012CFA
[00012caa] 23f9 0001 4d0a 0001 4dcc  move.l    $00014D0A,_tree
[00012cb4] 41fa 0108                 lea.l     _cpx_call(pc),a0
[00012cb8] 23c8 0001 4d8c            move.l    a0,$00014D8C
[00012cbe] 91c8                      suba.l    a0,a0
[00012cc0] 23c8 0001 4d90            move.l    a0,$00014D90
[00012cc6] 23c8 0001 4d94            move.l    a0,$00014D94
[00012ccc] 23c8 0001 4d98            move.l    a0,$00014D98
[00012cd2] 23c8 0001 4d9c            move.l    a0,$00014D9C
[00012cd8] 23c8 0001 4da0            move.l    a0,$00014DA0
[00012cde] 23c8 0001 4da4            move.l    a0,$00014DA4
[00012ce4] 23c8 0001 4da8            move.l    a0,$00014DA8
[00012cea] 23c8 0001 4dac            move.l    a0,$00014DAC
[00012cf0] 23c8 0001 4db0            move.l    a0,$00014DB0
[00012cf6] 6000 00b6                 bra       $00012DAE
[00012cfa] 2079 0001 4d88            movea.l   xcpb,a0
[00012d00] 3028 0054                 move.w    84(a0),d0
[00012d04] 5340                      subq.w    #1,d0
[00012d06] 6d00 008e                 blt       $00012D96
[00012d0a] 0c40 0006                 cmpi.w    #$0006,d0
[00012d0e] 6c00 0086                 bge       $00012D96
[00012d12] d040                      add.w     d0,d0
[00012d14] 303b 0006                 move.w    $00012D1C(pc,d0.w),d0
[00012d18] 4efb 0004                 jmp       $00012D1E(pc,d0.w)
J5:
[00012d1c] 000a                      dc.w $000a   ; $00012d28-$00012d1c-2
[00012d1e] 0020                      dc.w $0020   ; $00012d3e-$00012d1c-2
[00012d20] 0078                      dc.w $0078   ; $00012d96-$00012d1c-2
[00012d22] 0036                      dc.w $0036   ; $00012d54-$00012d1c-2
[00012d24] 004c                      dc.w $004c   ; $00012d6a-$00012d1c-2
[00012d26] 0062                      dc.w $0062   ; $00012d80-$00012d1c-2
[00012d28] 2079 0001 3f5e            movea.l   $00013F5E,a0
[00012d2e] 2f08                      move.l    a0,-(a7)
[00012d30] 7001                      moveq.l   #1,d0
[00012d32] 3f00                      move.w    d0,-(a7)
[00012d34] 4eb9 0001 323c            jsr       form_alert
[00012d3a] 5c4f                      addq.w    #6,a7
[00012d3c] 606c                      bra.s     $00012DAA
[00012d3e] 2079 0001 3f62            movea.l   $00013F62,a0
[00012d44] 2f08                      move.l    a0,-(a7)
[00012d46] 7001                      moveq.l   #1,d0
[00012d48] 3f00                      move.w    d0,-(a7)
[00012d4a] 4eb9 0001 323c            jsr       form_alert
[00012d50] 5c4f                      addq.w    #6,a7
[00012d52] 6056                      bra.s     $00012DAA
[00012d54] 2079 0001 3f66            movea.l   $00013F66,a0
[00012d5a] 2f08                      move.l    a0,-(a7)
[00012d5c] 7001                      moveq.l   #1,d0
[00012d5e] 3f00                      move.w    d0,-(a7)
[00012d60] 4eb9 0001 323c            jsr       form_alert
[00012d66] 5c4f                      addq.w    #6,a7
[00012d68] 6040                      bra.s     $00012DAA
[00012d6a] 2079 0001 3f6a            movea.l   $00013F6A,a0
[00012d70] 2f08                      move.l    a0,-(a7)
[00012d72] 7001                      moveq.l   #1,d0
[00012d74] 3f00                      move.w    d0,-(a7)
[00012d76] 4eb9 0001 323c            jsr       form_alert
[00012d7c] 5c4f                      addq.w    #6,a7
[00012d7e] 602a                      bra.s     $00012DAA
[00012d80] 2079 0001 3f6e            movea.l   $00013F6E,a0
[00012d86] 2f08                      move.l    a0,-(a7)
[00012d88] 7001                      moveq.l   #1,d0
[00012d8a] 3f00                      move.w    d0,-(a7)
[00012d8c] 4eb9 0001 323c            jsr       form_alert
[00012d92] 5c4f                      addq.w    #6,a7
[00012d94] 6014                      bra.s     $00012DAA
[00012d96] 2079 0001 3f5a            movea.l   $00013F5A,a0
[00012d9c] 2f08                      move.l    a0,-(a7)
[00012d9e] 7001                      moveq.l   #1,d0
[00012da0] 3f00                      move.w    d0,-(a7)
[00012da2] 4eb9 0001 323c            jsr       form_alert
[00012da8] 5c4f                      addq.w    #6,a7
[00012daa] 7000                      moveq.l   #0,d0
[00012dac] 6008                      bra.s     $00012DB6
[00012dae] 41f9 0001 4d8c            lea.l     $00014D8C,a0
[00012db4] 2008                      move.l    a0,d0
[00012db6] 4cdf 2004                 movem.l   (a7)+,d2/a5
[00012dba] 4e5e                      unlk      a6
[00012dbc] 4e75                      rts
_cpx_call:
[00012dbe] 2f0d                      move.l    a5,-(a7)
[00012dc0] 2a6f 0008                 movea.l   8(a7),a5
[00012dc4] 204d                      movea.l   a5,a0
[00012dc6] 43f9 0001 4de0            lea.l     $00014DE0,a1
[00012dcc] 22d8                      move.l    (a0)+,(a1)+
[00012dce] 22d8                      move.l    (a0)+,(a1)+
[00012dd0] 2079 0001 4dcc            movea.l   _tree,a0
[00012dd6] 3155 0010                 move.w    (a5),16(a0)
[00012dda] 2079 0001 4dcc            movea.l   _tree,a0
[00012de0] 316d 0002 0012            move.w    2(a5),18(a0)
[00012de6] 2079 0001 4d88            movea.l   xcpb,a0
[00012dec] 3f28 0054                 move.w    84(a0),-(a7)
[00012df0] 6100 fa5a                 bsr       _set_texts
[00012df4] 544f                      addq.w    #2,a7
[00012df6] 33fc 001a 0001 4dbc       move.w    #$001A,_curobj
[00012dfe] 4279 0001 4dc8            clr.w     _curIsTed
[00012e04] 2079 0001 4d88            movea.l   xcpb,a0
[00012e0a] 4a68 0006                 tst.w     6(a0)
[00012e0e] 6604                      bne.s     $00012E14
[00012e10] 6100 d242                 bsr       init_tree
[00012e14] 2f39 0001 4df2            move.l    _wcolors,-(a7)
[00012e1a] 6100 e804                 bsr       _set_wobjects
[00012e1e] 6100 f072                 bsr       _do_form
[00012e22] 584f                      addq.w    #4,a7
[00012e24] 7000                      moveq.l   #0,d0
[00012e26] 2a5f                      movea.l   (a7)+,a5
[00012e28] 4e75                      rts
[00012e2a] 0000
ToColor:
[00012e2c] 206f 0004                 movea.l   4(a7),a0
[00012e2e] 302f 0008                 ori.b     #$2F,d4
[00012e32] 3080                 ori.b     #$80,a0 ; apollo only
[00012e36] 4e75                      rts
_FColor:
[00012e38] 206f 0004                 movea.l   4(a7),a0
[00012e3c] 226f 0008                 movea.l   8(a7),a1
[00012e40] 3290                      move.w    (a0),(a1)
[00012e42] 4e75                      rts
_rc_intersect:
[00012e44] 4e56 fff8                 link      a6,#$FFF8
[00012e48] 48e7 2f04                 movem.l   d2/d4-d7/a5,-(a7)
[00012e4c] 2a6e 000c                 movea.l   12(a6),a5
[00012e50] 3015                      move.w    (a5),d0
[00012e52] d06d 0004                 add.w     4(a5),d0
[00012e56] 206e 0008                 movea.l   8(a6),a0
[00012e5a] 3210                      move.w    (a0),d1
[00012e5c] d268 0004                 add.w     4(a0),d1
[00012e60] b041                      cmp.w     d1,d0
[00012e62] 6d02                      blt.s     $00012E66
[00012e64] 2001                      move.l    d1,d0
[00012e66] 322d 0002                 move.w    2(a5),d1
[00012e6a] d26d 0006                 add.w     6(a5),d1
[00012e6e] 3428 0002                 move.w    2(a0),d2
[00012e72] d468 0006                 add.w     6(a0),d2
[00012e76] b242                      cmp.w     d2,d1
[00012e78] 6d02                      blt.s     $00012E7C
[00012e7a] 2202                      move.l    d2,d1
[00012e7c] 2c01                      move.l    d1,d6
[00012e7e] 3215                      move.w    (a5),d1
[00012e80] b250                      cmp.w     (a0),d1
[00012e82] 6e02                      bgt.s     $00012E86
[00012e84] 3210                      move.w    (a0),d1
[00012e86] 2a01                      move.l    d1,d5
[00012e88] 322d 0002                 move.w    2(a5),d1
[00012e8c] b268 0002                 cmp.w     2(a0),d1
[00012e90] 6e04                      bgt.s     $00012E96
[00012e92] 3228 0002                 move.w    2(a0),d1
[00012e96] 2801                      move.l    d1,d4
[00012e98] 9045                      sub.w     d5,d0
[00012e9a] 5ec1                      sgt       d1
[00012e9c] 4401                      neg.b     d1
[00012e9e] 4881                      ext.w     d1
[00012ea0] 2e01                      move.l    d1,d7
[00012ea2] 4a47                      tst.w     d7
[00012ea4] 671c                      beq.s     $00012EC2
[00012ea6] 9c44                      sub.w     d4,d6
[00012ea8] 5ec1                      sgt       d1
[00012eaa] 4401                      neg.b     d1
[00012eac] 4881                      ext.w     d1
[00012eae] 2e01                      move.l    d1,d7
[00012eb0] 4a47                      tst.w     d7
[00012eb2] 670e                      beq.s     $00012EC2
[00012eb4] 3a85                      move.w    d5,(a5)
[00012eb6] 3b44 0002                 move.w    d4,2(a5)
[00012eba] 3b40 0004                 move.w    d0,4(a5)
[00012ebe] 3b46 0006                 move.w    d6,6(a5)
[00012ec2] 2007                      move.l    d7,d0
[00012ec4] 4cdf 20f4                 movem.l   (a7)+,d2/d4-d7/a5
[00012ec8] 4e5e                      unlk      a6
[00012eca] 4e75                      rts
_rc_shrink:
[00012ecc] 4e56 0000                 link      a6,#$0000
[00012ed0] 302e 0008                 move.w    8(a6),d0
[00012ed4] 206e 000c                 movea.l   12(a6),a0
[00012ed8] 9168 0004                 sub.w     d0,4(a0)
[00012edc] 302e 000a                 move.w    10(a6),d0
[00012ee0] 9168 0006                 sub.w     d0,6(a0)
[00012ee4] 4e5e                      unlk      a6
[00012ee6] 4e75                      rts
_rc_inset:
[00012ee8] 4e56 0000                 link      a6,#$0000
[00012eec] 302e 0008                 move.w    8(a6),d0
[00012ef0] 206e 000c                 movea.l   12(a6),a0
[00012ef4] d150                      add.w     d0,(a0)
[00012ef6] 302e 000a                 move.w    10(a6),d0
[00012efa] d168 0002                 add.w     d0,2(a0)
[00012efe] 4e5e                      unlk      a6
[00012f00] 4e75                      rts
_Rc_center:
[00012f02] 4e56 0000                 link      a6,#$0000
[00012f06] 206e 0008                 movea.l   8(a6),a0
[00012f0a] 226e 0010                 movea.l   16(a6),a1
[00012f0e] 22d8                      move.l    (a0)+,(a1)+
[00012f10] 22d8                      move.l    (a0)+,(a1)+
[00012f12] 302e 000c                 move.w    12(a6),d0
[00012f16] d040                      add.w     d0,d0
[00012f18] 322e 000e                 move.w    14(a6),d1
[00012f1c] d241                      add.w     d1,d1
[00012f1e] 2f2e 0010                 move.l    16(a6),-(a7)
[00012f22] 3f01                      move.w    d1,-(a7)
[00012f24] 3f00                      move.w    d0,-(a7)
[00012f26] 61a4                      bsr.s     _rc_shrink
[00012f28] 2eae 0010                 move.l    16(a6),(a7)
[00012f2c] 3f2e 000e                 move.w    14(a6),-(a7)
[00012f30] 3f2e 000c                 move.w    12(a6),-(a7)
[00012f34] 61b2                      bsr.s     _rc_inset
[00012f36] 4e5e                      unlk      a6
[00012f38] 4e75                      rts

ObjcGrect:
[00012f3c] 4e56 0000                 link      a6,#0
[00012f40] 206e 000e                 movea.l   14(a6),a0
[00012f42] 2248                      movea.l   a0,a1
[00012f46] 5449                      addq.w    #2,a1
[00012f48] 2f09                      move.l    a1,-(a7)
[00012f4a] 2f08                      move.l    a0,-(a7)
[00012f4c] 3f2e 000c                 move.w    12(a6),-(a7)
[00012f50] 2f2e 0008                 move.l    8(a6),-(a7)
[00012f54] 4eba 024a                 jsr       _objc_offset(pc)
[00012f58] 302e 000c                 move.w    12(a6),d0
[00012f5c] c1fc 0018                 muls.w    #$0018,d0
[00012f60] 206e 000e                 movea.l   14(a6),a0
[00012f64] 226e 0008                 movea.l   8(a6),a1
[00012f68] 2171 0814 0004            move.l    20(a1,d0.l),4(a0)
[00012f6e] 4e5e                      unlk      a6
[00012f70] 4e75                      rts
ObjcXGrect:
[00012f72] 4e56 fff8                 link      a6,#$FFF8
[00012f76] 48e7 0314                 movem.l   d6-d7/a3/a5,-(a7)
[00012f7a] 2a6e 0008                 movea.l   8(a6),a5
[00012f7e] 3e2e 000c                 move.w    12(a6),d7
[00012f82] 266e 000e                 movea.l   14(a6),a3
[00012f86] 2f0b                      move.l    a3,-(a7)
[00012f88] 3f07                      move.w    d7,-(a7)
[00012f8a] 2f0d                      move.l    a5,-(a7)
[00012f8c] 61ae                      bsr.s     ObjcGrect
[00012f8e] 4fef 000a                 lea.l     10(a7),a7
[00012f92] 2007                      move.l    d7,d0
[00012f94] c1fc 0018                 muls.w    #$0018,d0
[00012f98] dbc0                      adda.l    d0,a5
[00012f9a] 082d 0000 0008            btst      #0,8(a5)
[00012fa0] 6708                      beq.s     $00012FAA
[00012fa2] 2d6d 000c fffc            move.l    12(a5),-4(a6)
[00012fa8] 6008                      bra.s     $00012FB2
[00012faa] 41ed 000c                 lea.l     12(a5),a0
[00012fae] 2d48 fffc                 move.l    a0,-4(a6)
[00012fb2] 302d 0006                 move.w    6(a5),d0
[00012fb6] 7215                      moveq.l   #21,d1
[00012fb8] b041                      cmp.w     d1,d0
[00012fba] 6712                      beq.s     $00012FCE
[00012fbc] 7216                      moveq.l   #22,d1
[00012fbe] b041                      cmp.w     d1,d0
[00012fc0] 670c                      beq.s     $00012FCE
[00012fc2] 721d                      moveq.l   #29,d1
[00012fc4] b041                      cmp.w     d1,d0
[00012fc6] 6706                      beq.s     $00012FCE
[00012fc8] 721e                      moveq.l   #30,d1
[00012fca] b041                      cmp.w     d1,d0
[00012fcc] 660c                      bne.s     $00012FDA
[00012fce] 226e fffc                 movea.l   -4(a6),a1
[00012fd2] 2051                      movea.l   (a1),a0
[00012fd4] 3e28 0016                 move.w    22(a0),d7
[00012fd8] 600a                      bra.s     $00012FE4
[00012fda] 206e fffc                 movea.l   -4(a6),a0
[00012fde] 1e28 0001                 move.b    1(a0),d7
[00012fe2] 4887                      ext.w     d7
[00012fe4] 4a47                      tst.w     d7
[00012fe6] 6b04                      bmi.s     $00012FEC
[00012fe8] 7c00                      moveq.l   #0,d6
[00012fea] 6004                      bra.s     $00012FF0
[00012fec] 2c07                      move.l    d7,d6
[00012fee] 4447                      neg.w     d7
[00012ff0] 082d 0004 000b            btst      #4,11(a5)
[00012ff6] 6708                      beq.s     $00013000
[00012ff8] 70fd                      moveq.l   #-3,d0
[00012ffa] bc40                      cmp.w     d0,d6
[00012ffc] 6f02                      ble.s     $00013000
[00012ffe] 2c00                      move.l    d0,d6
[00013000] 4a46                      tst.w     d6
[00013002] 6710                      beq.s     $00013014
[00013004] 2f0b                      move.l    a3,-(a7)
[00013006] 3f06                      move.w    d6,-(a7)
[00013008] 3f06                      move.w    d6,-(a7)
[0001300a] 2f0b                      move.l    a3,-(a7)
[0001300c] 4eba fef4                 jsr       _Rc_center(pc)
[00013010] 4fef 000c                 lea.l     12(a7),a7
[00013014] 082d 0005 000b            btst      #5,11(a5)
[0001301a] 6710                      beq.s     $0001302C
[0001301c] 4a47                      tst.w     d7
[0001301e] 670c                      beq.s     $0001302C
[00013020] 2007                      move.l    d7,d0
[00013022] d040                      add.w     d0,d0
[00013024] d16b 0004                 add.w     d0,4(a3)
[00013028] d16b 0006                 add.w     d0,6(a3)
[0001302c] 4cdf 28c0                 movem.l   (a7)+,d6-d7/a3/a5
[00013030] 4e5e                      unlk      a6
[00013032] 4e75                      rts
_Objc_draw:
[00013034] 4e56 fff4                 link      a6,#$FFF4
[00013038] 48e7 0114                 movem.l   d7/a3/a5,-(a7)
[0001303c] 2a6e 0008                 movea.l   8(a6),a5
[00013040] 3e2e 000c                 move.w    12(a6),d7
[00013044] 266e 0010                 movea.l   16(a6),a3
[00013048] 41f9 0001 50b8            lea.l     _AESintin,a0
[0001304e] 3087                      move.w    d7,(a0)
[00013050] 43e8 0004                 lea.l     4(a0),a1
[00013054] 316e 000e 0002            move.w    14(a6),2(a0)
[0001305a] 2d49 fffc                 move.l    a1,-4(a6)
[0001305e] 200b                      move.l    a3,d0
[00013060] 6614                      bne.s     $00013076
[00013062] 47ee fff4                 lea.l     -12(a6),a3
[00013066] 486e fff4                 pea.l     -12(a6)
[0001306a] 3f07                      move.w    d7,-(a7)
[0001306c] 2f0d                      move.l    a5,-(a7)
[0001306e] 6100 ff02                 bsr       ObjcXGrect
[00013072] 4fef 000a                 lea.l     10(a7),a7
[00013076] 204b                      movea.l   a3,a0
[00013078] 226e fffc                 movea.l   -4(a6),a1
[0001307c] 22d8                      move.l    (a0)+,(a1)+
[0001307e] 22d8                      move.l    (a0)+,(a1)+
[00013080] 23cd 0001 50e6            move.l    a5,_AESaddrin
[00013086] 303c 0080                 move.w    #$0080,d0 ; objc_draw
[0001308a] 4eba 0238                 jsr       @_AESif(pc)
[0001308e] 4cdf 2880                 movem.l   (a7)+,d7/a3/a5
[00013092] 4e5e                      unlk      a6
[00013094] 4e75                      rts
[00013096] 0000
_Graf_mkstate:
[00013098] 4e56 0000                 link      a6,#0
[0001309c] 303c 0114                 move.w    #$0114,d0
[000130a0] 4eba 0222                 jsr       @_AESif(pc)
[000130a4] 43e8 0002                 lea.l     2(a0),a1
[000130a8] 206e 0008                 movea.l   8(a6),a0
[000130ac] 20d9                      move.l    (a1)+,(a0)+
[000130ae] 20d9                      move.l    (a1)+,(a0)+
[000130b0] 4e5e                      unlk      a6
[000130b2] 4e75                      rts
Evnt_timer:
[000130b4] 4e56 0000                 link      a6,#$0000
[000130b8] 202e 0008                 move.l    8(a6),d0
[000130bc] 4840                      swap      d0
[000130be] 23c0 0001 50b8            move.l    d0,_AESintin
[000130c4] 7038                      moveq.l   #56,d0
[000130c6] 4eba 01fc                 jsr       @_AESif(pc)
[000130ca] 4e5e                      unlk      a6
[000130cc] 4e75                      rts
[000130ce] 0000
_wind_set:
[000130d0] 4e56 0000                 link      a6,#0
[000130d4] 33ee 0008 0001 50b8       move.w    8(a6),_AESintin
           33ee 000a 0001 50ba       move.w    10(a6),_AESintin+2
           33ee 000c 0001 50bc       move.w    12(a6),_AESintin+4
[000130ec] 33ee 000e 0001 50be       move.w    14(a6),_AESintin+6
[000130f4] 33ee 0010 0001 50c0       move.w    16(a6),_AESintin+8
[000130fc] 33ee 0012 0001 50c2       move.w    18(a6),_AESintin+10
[00013104] 303c 017c                 move.w    #$017C,d0
[00013108] 4eb9 0001 32c4            jsr       @_AESif
[0001310e] 4e5e                      unlk      a6
[00013110] 4e75                      rts
[00013112] 0000
vq_extnd:
[00013114] 4e56 0000                 link      a6,#0
[00013116] 33ee 000a 0001 4f4c       move.w    10(a6),__VDIintin
[00013120] 206e 000c                 movea.l   12(a6),a0
[00013124] 23c8 0001 5058            move.l    a0,__VDIpb+12
[0001312a] 43e8 005a                 lea.l     90(a0),a1
[0001312e] 23c9 0001 505c            move.l    a1,__VDIpb+16
[00013134] 302e 0008                 move.w    8(a6),d0
[00013138] 48c0                      ext.l     d0
[0001313a] 4840                      swap      d0
[0001313c] 4240                      clr.w     d0
[0001313e] 0040 0001                 ori.w     #$0001,d0
[00013142] 7266                      moveq.l   #102,d1
[00013144] 4841                      swap      d1
[00013146] 4eb9 0001 3280            jsr       _VDIif
[0001314c] 23fc 0001 4e4c 0001 5058  move.l    #__VDIintout,__VDIpb+12
[00013156] 23fc 0001 4e28 0001 505c  move.l    #__VDIptsout,__VDIpb+16
[00013160] 4e5e                      unlk      a6
[00013162] 4e75                      rts
rc_inside:
[00013164] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00013168] 3e2f 0010                 move.w    16(a7),d7
[0001316c] 3c2f 0012                 move.w    18(a7),d6
[00013170] 2a6f 0014                 movea.l   20(a7),a5
[00013174] 3015                      move.w    (a5),d0
[00013176] be40                      cmp.w     d0,d7
[00013178] 6d1c                      blt.s     $00013196
[0001317a] 322d 0002                 move.w    2(a5),d1
[0001317e] bc41                      cmp.w     d1,d6
[00013180] 6d14                      blt.s     $00013196
[00013182] d06d 0004                 add.w     4(a5),d0
[00013186] be40                      cmp.w     d0,d7
[00013188] 6c0c                      bge.s     $00013196
[0001318a] d26d 0006                 add.w     6(a5),d1
[0001318e] bc41                      cmp.w     d1,d6
[00013190] 6c04                      bge.s     $00013196
[00013192] 7001                      moveq.l   #1,d0
[00013194] 6002                      bra.s     $00013198
[00013196] 7000                      moveq.l   #0,d0
[00013198] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[0001319c] 4e75                      rts
[0001319e] 0000
_objc_offset:
[000131a0] 4e56 0000                 link      a6,#0
[000131a2] 23ee 0008 0001 50e6       move.l    8(a6),_AESaddrin
[000131ac] 33ee 000c 0001 50b8       move.w    12(a6),_AESintin
[000131b4] 303c 0088                 move.w    #$0088,d0
[000131b8] 4eb9 0001 32c4            jsr       @_AESif
[000131be] 206e 000e                 movea.l   14(a6),a0
[000131c2] 30b9 0001 50da            move.w    $000150DA,(a0)
[000131c8] 206e 0012                 movea.l   18(a6),a0
[000131cc] 30b9 0001 50dc            move.w    $000150DC,(a0)
[000131d2] 3039 0001 50d8            move.w    _AESintout,d0
[000131d8] 4e5e                      unlk      a6
[000131da] 4e75                      rts
objc_find:
[000131dc] 4e56 0000                 link      a6,#$0000
[000131e0] 23ee 0008 0001 50e6       move.l    8(a6),_AESaddrin
[000131e8] 33ee 000c 0001 50b8       move.w    12(a6),_AESintin
[000131f0] 33ee 000e 0001 50ba       move.w    14(a6),_AESintin+2
[000131f8] 33ee 0010 0001 50bc       move.w    16(a6),_AESintin+4
[00013200] 33ee 0012 0001 50be       move.w    18(a6),_AESintin+6
[00013208] 303c 0084                 move.w    #$0084,d0
[0001320c] 4eb9 0001 32c4            jsr       @_AESif
[00013212] 4e5e                      unlk      a6
[00013214] 4e75                      rts
[00013216] 0000
_graf_mouse:
[00013218] 4e56 0000                 link      a6,#$0000
[0001321a] 33ee 0008 0001 50b8       move.w    8(a6),_AESintin
[00013222] 23ee 000a 0001 50e6       move.l    10(a6),_AESaddrin
[0001322c] 303c 0110                 move.w    #$0110,d0
[00013230] 4eb9 0001 32c4            jsr       @_AESif
[00013236] 4e5e                      unlk      a6
[00013238] 4e75                      rts
[0001323a] 0000
form_alert:
[0001323c] 4e56 0000                 link      a6,#$0000
[0001323e] 23ee 000a 0001 50e6       move.l    10(a6),_AESaddrin
[00013248] 33ee 0008 0001 50b8       move.w    8(a6),_AESintin
[00013250] 303c 00a8                 move.w    #$00A8,d0
[00013254] 4eb9 0001 32c4            jsr       @_AESif
[0001325a] 4e5e                      unlk      a6
[0001325c] 4e75                      rts
[0001325e] 0000
evnt_dclick:
[00013260] 4e56 0000                 link      a6,#$0000
[00013264] 33ee 0008 0001 50b8       move.w    8(a6),_AESintin
[0001326c] 33ee 000a 0001 50ba       move.w    8(a6),_AESintin+2
[00013274] 7040                      moveq.l   #$40,d0
[00013276] 4eb9 0001 32c4            jsr       @_AESif
[0001327c] 4e5e                      unlk      a6
[0001327e] 4e75                      rts
@_VDIif:
[00013280] 41f9 0001 504c            lea.l     __VDIpb,a0
[00013286] 2250                      movea.l   (a0),a1
[00013288] 2281                      move.l    d1,(a1)
[0001328a] 3340 0006                 move.w    d0,6(a1)
[0001328e] 4840                      swap      d0
[00013290] 3340 000c                 move.w    d0,12(a1)
[00013294] 2208                      move.l    a0,d1
[00013296] 7073                      moveq.l   #115,d0
[00013298] 4e42                      trap      #2
[0001329a] 4e75                      rts
__VDIinit:
[0001329c] 41f9 0001 504c            lea.l     __VDIpb,a0
[000132a2] 20fc 0001 5060            move.l    #__VDIcontrl,(a0)+
[000132a8] 20fc 0001 4f4c            move.l    #__VDIintin,(a0)+
[000132ae] 20fc 0001 4e3c            move.l    #__VDIptsin,(a0)+
[000132b4] 20fc 0001 4e4c            move.l    #__VDIintout,(a0)+
[000132ba] 20fc 0001 4e28            move.l    #__VDIptsout,(a0)+
[000132c0] 4e75                      rts
[000132c2] 0000
@_AESif:
[000132c4] 41fa 0024                 lea.l     $000132ea(pc),a0
[000132c6] 2030 0000                 move.l    0(a0,d0.w),d0
[000132ca] 41f9 0001 5078            lea       __AESpb,a0
[000132d2] 2250                      movea.l   (a0),a1
[000132d4] 01c9 0001                 movep.l   d0,1(a1)
[000132d8] 2208                      move.l    a0,d1
[000132da] 303c 00c8                 move.w    #$00C8,d0
[000132de] 4e42                      trap      #2
[000132e0] 41f9 0001 50d8            lea.l     _AESintout,a0
[000132e6] 3010                      move.w    (a0),d0
[000132e8] 4e75                      rts
[000132ea] 0a00 0100                 eori.b    #$00,d0
[000132ee] 0b02                      btst      d5,d2
[000132f0] 0101                      btst      d0,d1
[000132f2] 0c02 0101                 cmpi.b    #$01,d2
[000132f6] 0d00                      btst      d6,d0
[000132f8] 0101                      btst      d0,d1
[000132fa] 0e02 0101                 abs.b     d2,d0 ; apollo only
[000132fe] 0f01                      btst      d7,d1
[00013300] 0101                      btst      d0,d1
[00013302] 1000                      move.b    d0,d0
[00013304] 0000 1100                 ori.b     #$00,d0
[00013308] 0000 1200                 ori.b     #$00,d0
[0001330c] 0000 1300                 ori.b     #$00,d0
[00013310] 0100                      btst      d0,d0
[00013312] 1400                      move.b    d0,d2
[00013314] 0100                      btst      d0,d0
[00013316] 1503                      move.b    d3,-(a2)
[00013318] 0500                      btst      d2,d0
[0001331a] 1605                      move.b    d5,d3
[0001331c] 0500                      btst      d2,d0
[0001331e] 1700                      move.b    d0,-(a3)
[00013320] 0101                      btst      d0,d1
[00013322] 1802                      move.b    d2,d4
[00013324] 0100                      btst      d0,d0
[00013326] 1910                      move.b    (a0),-(a4)
[00013328] 0701                      btst      d3,d1
[0001332a] 1a02                      move.b    d2,d5
[0001332c] 0100                      btst      d0,d0
[0001332e] 1b00                      move.b    d0,-(a5)
[00013330] 0000 1c00                 ori.b     #$00,d0
[00013334] 0000 1d00                 ori.b     #$00,d0
[00013338] 0000 1e01                 ori.b     #$01,d0
[0001333c] 0101                      btst      d0,d1
[0001333e] 1f02                      move.b    d2,-(a7)
[00013340] 0101                      btst      d0,d1
[00013342] 2002                      move.l    d2,d0
[00013344] 0101                      btst      d0,d1
[00013346] 2102                      move.l    d2,-(a0)
[00013348] 0101                      btst      d0,d1
[0001334a] 2201                      move.l    d1,d1
[0001334c] 0102                      btst      d0,d2
[0001334e] 2301                      move.l    d1,-(a1)
[00013350] 0101                      btst      d0,d1
[00013352] 2400                      move.l    d0,d2
[00013354] 0000 2500                 ori.b     #$00,d0
[00013358] 0000 2600                 ori.b     #$00,d0
[0001335c] 0000 2700                 ori.b     #$00,d0
[00013360] 0000 2802                 ori.b     #$02,d0
[00013364] 0101                      btst      d0,d1
[00013366] 2901                      move.l    d1,-(a4)
[00013368] 0101                      btst      d0,d1
[0001336a] 2a06                      move.l    d6,d5
[0001336c] 0101                      btst      d0,d1
[0001336e] 2b04                      move.l    d4,-(a5)
[00013370] 0101                      btst      d0,d1
[00013372] 2c01                      move.l    d1,d6
[00013374] 0301                      btst      d1,d1
[00013376] 2d02                      move.l    d2,-(a6)
[00013378] 0101                      btst      d0,d1
[0001337a] 2e04                      move.l    d4,d7
[0001337c] 0201 2f08                 andi.b    #$08,d1
[00013380] 0101                      btst      d0,d1
[00013382] 3000                      move.w    d0,d0
[00013384] 0000 3100                 ori.b     #$00,d0
[00013388] 0000 3201                 ori.b     #$01,d0
[0001338c] 0101                      btst      d0,d1
[0001338e] 3309                      move.w    a1,-(a1)
[00013390] 0100                      btst      d0,d0
[00013392] 3401                      move.w    d1,d2
[00013394] 0101                      btst      d0,d1
[00013396] 3501                      move.w    d1,-(a2)
[00013398] 0100                      btst      d0,d0
[0001339a] 3600                      move.w    d0,d3
[0001339c] 0501                      btst      d2,d1
[0001339e] 3703                      move.w    d3,-(a3)
[000133a0] 0301                      btst      d1,d1
[000133a2] 3802                      move.w    d2,d4
[000133a4] 0201 3900                 andi.b    #$00,d1
[000133a8] 0000 3a00                 ori.b     #$00,d0
[000133ac] 0000 3b00                 ori.b     #$00,d0
[000133b0] 0000 3c00                 ori.b     #$00,d0
[000133b4] 0000 3d00                 ori.b     #$00,d0
[000133b8] 0000 3e00                 ori.b     #$00,d0
[000133bc] 0000 3f00                 ori.b     #$00,d0
[000133c0] 0000 4000                 ori.b     #$00,d0
[000133c4] 0000 4100                 ori.b     #$00,d0
[000133c8] 0000 4200                 ori.b     #$00,d0
[000133cc] 0000 4300                 ori.b     #$00,d0
[000133d0] 0000 4400                 ori.b     #$00,d0
[000133d4] 0000 4500                 ori.b     #$00,d0
[000133d8] 0000 4604                 ori.b     #$04,d0
[000133dc] 0300                      btst      d1,d0
[000133de] 4708                      chk.l     a0,d3 ; 68020+ only
[000133e0] 0300                      btst      d1,d0
[000133e2] 4806                      nbcd      d6
[000133e4] 0100                      btst      d0,d0
[000133e6] 4908                      chk.l     a0,d4 ; 68020+ only
[000133e8] 0100                      btst      d0,d0
[000133ea] 4a08                      tst.b     a0
[000133ec] 0100                      btst      d0,d0
[000133ee] 4b04                      chk.l     d4,d5 ; 68020+ only
[000133f0] 0101                      btst      d0,d1
[000133f2] 4c03 0101                 mulu.l    d3,d0 ; 68020+ only
[000133f6] 4d00                      chk.l     d0,d6 ; 68020+ only
[000133f8] 0500                      btst      d2,d0
[000133fa] 4e01 0101                 cmpiw.l   #$0101,d1 ; apollo only
[000133fe] 4f00                      chk.l     d0,d7 ; 68020+ only
[00013400] 0500                      btst      d2,d0
[00013402] 5000                      addq.b    #8,d0
[00013404] 0101                      btst      d0,d1
[00013406] 5100                      subq.b    #8,d0
[00013408] 0101                      btst      d0,d1
[0001340a] 5200                      addq.b    #1,d0
[0001340c] 0000 5300                 ori.b     #$00,d0
[00013410] 0000 5400                 ori.b     #$00,d0
[00013414] 0000 5500                 ori.b     #$00,d0
[00013418] 0000 5600                 ori.b     #$00,d0
[0001341c] 0000 5700                 ori.b     #$00,d0
[00013420] 0000 5800                 ori.b     #$00,d0
[00013424] 0000 5900                 ori.b     #$00,d0
[00013428] 0000 5a00                 ori.b     #$00,d0
[0001342c] 0202 5b00                 andi.b    #$00,d2
[00013430] 0203 5c00                 andi.b    #$00,d3
[00013434] 0000 5d00                 ori.b     #$00,d0
[00013438] 0000 5e00                 ori.b     #$00,d0
[0001343c] 0000 5f00                 ori.b     #$00,d0
[00013440] 0000 6000                 ori.b     #$00,d0
[00013444] 0000 6100                 ori.b     #$00,d0
[00013448] 0000 6200                 ori.b     #$00,d0
[0001344c] 0000 6300                 ori.b     #$00,d0
[00013450] 0000 6405                 ori.b     #$05,d0
[00013454] 0100                      btst      d0,d0
[00013456] 6505                      bcs.s     $0001345D
[00013458] 0100                      btst      d0,d0
[0001345a] 6601                      bne.s     $0001345D
[0001345c] 0100                      btst      d0,d0
[0001345e] 6701                      beq.s     $00013461
[00013460] 0100                      btst      d0,d0
[00013462] 6802                      bvc.s     $00013466
[00013464] 0500                      btst      d2,d0
[00013466] 6906                      bvs.s     $0001346E
[00013468] 0100                      btst      d0,d0
[0001346a] 6a02                      bpl.s     $0001346E
[0001346c] 0100                      btst      d0,d0
[0001346e] 6b01                      bmi.s     $00013471
[00013470] 0100                      btst      d0,d0
[00013472] 6c06                      bge.s     $0001347A
[00013474] 0500                      btst      d2,d0
[00013476] 6d00 0000                 blt.w     $00013478
[0001347a] 6e00 0101                 bgt       $0001357D
[0001347e] 6f00 0100                 ble       $00013580
[00013482] 7002                      moveq.l   #2,d0
[00013484] 0100                      btst      d0,d0
[00013486] 7102                      ??? ; EMULOP M68K_EMUL_RESET
[00013488] 0101                      btst      d0,d1
[0001348a] 7201                      moveq.l   #1,d1
[0001348c] 0101                      btst      d0,d1
[0001348e] 7300                      ??? ; nf_id
[00013490] 0000 7400                 ori.b     #$00,d0
[00013494] 0000 7500                 ori.b     #$00,d0
[00013498] 0000 7600                 ori.b     #$00,d0
[0001349c] 0000 7700                 ori.b     #$00,d0
[000134a0] 0000 7800                 ori.b     #$00,d0
[000134a4] 0102                      btst      d0,d2
[000134a6] 7903                      ???
[000134a8] 0102                      btst      d0,d2
[000134aa] 7a01                      moveq.l   #1,d5
[000134ac] 0101                      btst      d0,d1
[000134ae] 7b01                      ???
[000134b0] 0101                      btst      d0,d1
[000134b2] 7c00                      moveq.l   #0,d6
[000134b4] 0101                      btst      d0,d1
[000134b6] 7d00                      ???
[000134b8] 0103                      btst      d0,d3
[000134ba] 0000
__CXM33:
[000134bc] 48e7                      movem.l   d2-d3,-(a7)
[000134c0] 2400                      move.l    d0,d2
[000134c2] 2601                      move.l    d1,d3
[000134c4] 4842                      swap      d2
[000134c6] 4843                      swap      d3
[000134c8] c4c1                      mulu.w    d1,d2
[000134ca] c6c0                      mulu.w    d0,d3
[000134cc] c0c1                      mulu.w    d1,d0
[000134ce] d443                      add.w     d3,d2
[000134d0] 4842                      swap      d2
[000134d2] 4242                      clr.w     d2
[000134d4] d082                      add.l     d2,d0
[000134d6] 4cdf 000c                 movem.l   (a7)+,d2-d3
[000134da] 4e75                      rts
__CXD33 :
[000134dc] 4a80                      tst.l     d0
[000134de] 6a16                      bpl.s     $000134F6
[000134e0] 4480                      neg.l     d0
[000134e2] 4a81                      tst.l     d1
[000134e4] 6a08                      bpl.s     $000134EE
[000134e6] 4481                      neg.l     d1
[000134e8] 6118                      bsr.s     $00013502
[000134ea] 4481                      neg.l     d1
[000134ec] 4e75                      rts
[000134ee] 6112                      bsr.s     $00013502
[000134f0] 4480                      neg.l     d0
[000134f2] 4481                      neg.l     d1
[000134f4] 4e75                      rts
[000134f6] 4a81                      tst.l     d1
[000134f8] 6a08                      bpl.s     $00013502
[000134fa] 4481                      neg.l     d1
[000134fc] 6104                      bsr.s     $00013502
[000134fe] 4480                      neg.l     d0
[00013500] 4e75                      rts
[00013502] 2f02                      move.l    d2,-(a7)
[00013504] 4841                      swap      d1
[00013506] 3401                      move.w    d1,d2
[00013508] 661e                      bne.s     $00013528
[0001350a] 4840                      swap      d0
[0001350c] 4841                      swap      d1
[0001350e] 4842                      swap      d2
[00013510] 3400                      move.w    d0,d2
[00013512] 6704                      beq.s     $00013518
[00013514] 84c1                      divu.w    d1,d2
[00013516] 3002                      move.w    d2,d0
[00013518] 4840                      swap      d0
[0001351a] 3400                      move.w    d0,d2
[0001351c] 84c1                      divu.w    d1,d2
[0001351e] 3002                      move.w    d2,d0
[00013520] 4842                      swap      d2
[00013522] 3202                      move.w    d2,d1
[00013524] 241f                      move.l    (a7)+,d2
[00013526] 4e75                      rts
[00013528] 2f03                      move.l    d3,-(a7)
[0001352a] 7610                      moveq.l   #16,d3
[0001352c] b27c 0080                 cmp.w     #$0080,d1
[00013530] 6404                      bcc.s     $00013536
[00013532] e199                      rol.l     #8,d1
[00013534] 5143                      subq.w    #8,d3
[00013536] b27c 0800                 cmp.w     #$0800,d1
[0001353a] 6404                      bcc.s     $00013540
[0001353c] e999                      rol.l     #4,d1
[0001353e] 5943                      subq.w    #4,d3
[00013540] b27c 2000                 cmp.w     #$2000,d1
[00013544] 6404                      bcc.s     $0001354A
[00013546] e599                      rol.l     #2,d1
[00013548] 5543                      subq.w    #2,d3
[0001354a] 4a41                      tst.w     d1
[0001354c] 6b04                      bmi.s     $00013552
[0001354e] e399                      rol.l     #1,d1
[00013550] 5343                      subq.w    #1,d3
[00013552] 3400                      move.w    d0,d2
[00013554] e6a8                      lsr.l     d3,d0
[00013556] 4842                      swap      d2
[00013558] 4242                      clr.w     d2
[0001355a] e6aa                      lsr.l     d3,d2
[0001355c] 4843                      swap      d3
[0001355e] 80c1                      divu.w    d1,d0
[00013560] 3600                      move.w    d0,d3
[00013562] 3002                      move.w    d2,d0
[00013564] 3403                      move.w    d3,d2
[00013566] 4841                      swap      d1
[00013568] c4c1                      mulu.w    d1,d2
[0001356a] 9082                      sub.l     d2,d0
[0001356c] 6404                      bcc.s     $00013572
[0001356e] 5343                      subq.w    #1,d3
[00013570] d081                      add.l     d1,d0
[00013572] 7200                      moveq.l   #0,d1
[00013574] 3203                      move.w    d3,d1
[00013576] 4843                      swap      d3
[00013578] e7b8                      rol.l     d3,d0
[0001357a] 4840                      swap      d0
[0001357c] c141                      exg       d0,d1
[0001357e] 261f                      move.l    (a7)+,d3
[00013580] 241f                      move.l    (a7)+,d2
[00013582] 4e75                      rts
__FPCinit:
[00013584] 4e75                      rts
[00013586] 0000                  ori.b     #$A1,d0

data:

savecolors[0]:
[00013588]                           dc.w $11a1
[0001358a]                           dc.w $1100
[0001358c]                           dc.w $1100
[0001358e]                           dc.w $1100
[00013590]                           dc.w $1101
[00013592]                           dc.w $1101
[00013594]                           dc.w $1101
[00013596]                           dc.w $1101
[00013598]                           dc.w $1101
[0001359a]                           dc.w $1101
[0001359c]                           dc.w $1101
[0001359e]                           dc.w $1101
[000135a0]                           dc.w $1101
[000135a2]                           dc.w $1101
[000135a4]                           dc.w $1101
[000135a6]                           dc.w $1111
[000135a8]                           dc.w $1111
[000135aa]                           dc.w $1101
[000135ac]                           dc.w $1101
[000135ae]                           dc.w $1101
[000135b0]                           dc.w $1101
[000135b2]                           dc.w $1101
[000135b4]                           dc.w $1101
[000135b6]                           dc.w $1101
[000135b8]                           dc.w $1111
[000135ba]                           dc.w $1111
[000135bc]                           dc.w $1101
[000135be]                           dc.w $1101
[000135c0]                           dc.w $0000
[000135c2]                           dc.w $0000
[000135c4]                           dc.w $0000
[000135c6]                           dc.w $0000
[000135c8]                           dc.w $0000
[000135ca]                           dc.w $0000
savecolors[1]:
[000135cc]                           dc.w $11a1
[000135ce]                           dc.w $1100
[000135d0]                           dc.w $1100
[000135d2]                           dc.w $1100
[000135d4]                           dc.w $1101
[000135d6]                           dc.w $1101
[000135d8]                           dc.w $1101
[000135da]                           dc.w $1101
[000135dc]                           dc.w $1101
[000135de]                           dc.w $1101
[000135e0]                           dc.w $1101
[000135e2]                           dc.w $1101
[000135e4]                           dc.w $1101
[000135e6]                           dc.w $1101
[000135e8]                           dc.w $1101
[000135ea]                           dc.w $1111
[000135ec]                           dc.w $1111
[000135ee]                           dc.w $1101
[000135f0]                           dc.w $1101
[000135f2]                           dc.w $1101
[000135f4]                           dc.w $1101
[000135f6]                           dc.w $1101
[000135f8]                           dc.w $1101
[000135fa]                           dc.w $1101
[000135fc]                           dc.w $1111
[000135fe]                           dc.w $1111
[00013600]                           dc.w $1101
[00013602]                           dc.w $1101
[00013604]                           dc.w $0000
[00013606]                           dc.w $0000
[00013608]                           dc.w $0000
[0001360a]                           dc.w $0000
[0001360c]                           dc.w $0000
[0001360e]                           dc.w $0000
savecolors[2]:
[00013610]                           dc.w $9178
[00013612]                           dc.w $9078
[00013614]                           dc.w $9948
[00013616]                           dc.w $9078
[00013618]                           dc.w $9178
[0001361a]                           dc.w $9078
[0001361c]                           dc.w $9178
[0001361e]                           dc.w $9078
[00013620]                           dc.w $9178
[00013622]                           dc.w $9078
[00013624]                           dc.w $8068
[00013626]                           dc.w $9178
[00013628]                           dc.w $9078
[0001362a]                           dc.w $9178
[0001362c]                           dc.w $9078
[0001362e]                           dc.w $9179
[00013630]                           dc.w $9079
[00013632]                           dc.w $9178
[00013634]                           dc.w $9078
[00013636]                           dc.w $8068
[00013638]                           dc.w $9178
[0001363a]                           dc.w $9078
[0001363c]                           dc.w $9178
[0001363e]                           dc.w $9078
[00013640]                           dc.w $9179
[00013642]                           dc.w $9079
[00013644]                           dc.w $9178
[00013646]                           dc.w $9078
[00013648]                           dc.w $0000
[0001364a]                           dc.w $0000
[0001364c]                           dc.w $0000
[0001364e]                           dc.w $0000
[00013650]                           dc.w $0000
[00013652]                           dc.w $0000
savecolors[3]:
[00013654]                           dc.w $11a1
[00013656]                           dc.w $1100
[00013658]                           dc.w $1100
[0001365a]                           dc.w $1100
[0001365c]                           dc.w $1101
[0001365e]                           dc.w $1101
[00013660]                           dc.w $1101
[00013662]                           dc.w $1101
[00013664]                           dc.w $1101
[00013666]                           dc.w $1101
[00013668]                           dc.w $1101
[0001366a]                           dc.w $1101
[0001366c]                           dc.w $1101
[0001366e]                           dc.w $1101
[00013670]                           dc.w $1101
[00013672]                           dc.w $1111
[00013674]                           dc.w $1111
[00013676]                           dc.w $1101
[00013678]                           dc.w $1101
[0001367a]                           dc.w $1101
[0001367c]                           dc.w $1101
[0001367e]                           dc.w $1101
[00013680]                           dc.w $1101
[00013682]                           dc.w $1101
[00013684]                           dc.w $1111
[00013686]                           dc.w $1111
[00013688]                           dc.w $1101
[0001368a]                           dc.w $1101
[0001368c]                           dc.w $0000
[0001368e]                           dc.w $0000
[00013690]                           dc.w $0000
[00013692]                           dc.w $0000
[00013694]                           dc.w $0000
[00013696]                           dc.w $0000
_defcolors:
[00013698]                           dc.w $9178
[0001369a]                           dc.w $9078
[0001369c]                           dc.w $9948
[0001369e]                           dc.w $9078
[000136a0]                           dc.w $9178
[000136a2]                           dc.w $9078
[000136a4]                           dc.w $9178
[000136a6]                           dc.w $9078
[000136a8]                           dc.w $9178
[000136aa]                           dc.w $9078
[000136ac]                           dc.w $8068
[000136ae]                           dc.w $9178
[000136b0]                           dc.w $9078
[000136b2]                           dc.w $9178
[000136b4]                           dc.w $9078
[000136b6]                           dc.w $9179
[000136b8]                           dc.w $9079
[000136ba]                           dc.w $9178
[000136bc]                           dc.w $9078
[000136be]                           dc.w $8068
[000136c0]                           dc.w $9178
[000136c2]                           dc.w $9078
[000136c4]                           dc.w $9178
[000136c6]                           dc.w $9078
[000136c8]                           dc.w $9179
[000136ca]                           dc.w $9079
[000136cc]                           dc.w $9178
[000136ce]                           dc.w $9078
[000136d0]                           dc.w $0000
[000136d2]                           dc.w $0000
[000136d4]                           dc.w $0000
[000136d6]                           dc.w $0000
[000136d8]                           dc.w $0000
[000136da]                           dc.w $0000
altcolors[0]: plain
[000136dc]                           dc.w $11a1
[000136de]                           dc.w $1100
[000136e0]                           dc.w $1100
[000136e2]                           dc.w $1100
[000136e4]                           dc.w $1101
[000136e6]                           dc.w $1101
[000136e8]                           dc.w $1101
[000136ea]                           dc.w $1101
[000136ec]                           dc.w $1101
[000136ee]                           dc.w $1101
[000136f0]                           dc.w $1101
[000136f2]                           dc.w $1101
[000136f4]                           dc.w $1101
[000136f6]                           dc.w $1101
[000136f8]                           dc.w $1101
[000136fa]                           dc.w $1111
[000136fc]                           dc.w $1111
[000136fe]                           dc.w $1101
[00013700]                           dc.w $1101
[00013702]                           dc.w $1101
[00013704]                           dc.w $1101
[00013706]                           dc.w $1101
[00013708]                           dc.w $1101
[0001370a]                           dc.w $1101
[0001370c]                           dc.w $1111
[0001370e]                           dc.w $1111
[00013710]                           dc.w $1101
[00013712]                           dc.w $1101
[00013714]                           dc.w $0000
[00013716]                           dc.w $0000
[00013718]                           dc.w $0000
[0001371a]                           dc.w $0000
[0001371c]                           dc.w $0000
[0001371e]                           dc.w $0000
altcolors[1]: green
[00013720]                           dc.w $837b
[00013722]                           dc.w $804b
[00013724]                           dc.w $914b
[00013726]                           dc.w $804b
[00013728]                           dc.w $837b
[0001372a]                           dc.w $804b
[0001372c]                           dc.w $837b
[0001372e]                           dc.w $804b
[00013730]                           dc.w $837b
[00013732]                           dc.w $804b
[00013734]                           dc.w $884b
[00013736]                           dc.w $837b
[00013738]                           dc.w $804b
[0001373a]                           dc.w $837b
[0001373c]                           dc.w $804b
[0001373e]                           dc.w $836b
[00013740]                           dc.w $806b
[00013742]                           dc.w $837b
[00013744]                           dc.w $804b
[00013746]                           dc.w $884b
[00013748]                           dc.w $837b
[0001374a]                           dc.w $804b
[0001374c]                           dc.w $837b
[0001374e]                           dc.w $804b
[00013750]                           dc.w $836b
[00013752]                           dc.w $806b
[00013754]                           dc.w $837b
[00013756]                           dc.w $804b
[00013758]                           dc.w $0000
[0001375a]                           dc.w $0000
[0001375c]                           dc.w $0000
[0001375e]                           dc.w $0000
[00013760]                           dc.w $0000
[00013762]                           dc.w $0000
altcolors[2]:
[00013764]                           dc.w $9574
[00013766]                           dc.w $906d
[00013768]                           dc.w $c475
[0001376a]                           dc.w $906d
[0001376c]                           dc.w $9574
[0001376e]                           dc.w $906d
[00013770]                           dc.w $9574
[00013772]                           dc.w $906d
[00013774]                           dc.w $9574
[00013776]                           dc.w $906d
[00013778]                           dc.w $c17d
[0001377a]                           dc.w $9574
[0001377c]                           dc.w $906d
[0001377e]                           dc.w $9574
[00013780]                           dc.w $906d
[00013782]                           dc.w $915c
[00013784]                           dc.w $915c
[00013786]                           dc.w $9574
[00013788]                           dc.w $906d
[0001378a]                           dc.w $c17d
[0001378c]                           dc.w $9574
[0001378e]                           dc.w $906d
[00013790]                           dc.w $9574
[00013792]                           dc.w $906d
[00013794]                           dc.w $915c
[00013796]                           dc.w $915c
[00013798]                           dc.w $9574
[0001379a]                           dc.w $906d
[0001379c]                           dc.w $0000
[0001379e]                           dc.w $0000
[000137a0]                           dc.w $0000
[000137a2]                           dc.w $0000
[000137a4]                           dc.w $0000
[000137a6]                           dc.w $0000
altcolors[3]:
[000137a8]                           dc.w $9477
[000137aa]                           dc.w $887f
[000137ac]                           dc.w $914a
[000137ae]                           dc.w $887f
[000137b0]                           dc.w $9477
[000137b2]                           dc.w $887f
[000137b4]                           dc.w $9477
[000137b6]                           dc.w $887f
[000137b8]                           dc.w $9477
[000137ba]                           dc.w $887f
[000137bc]                           dc.w $c47f
[000137be]                           dc.w $9477
[000137c0]                           dc.w $887f
[000137c2]                           dc.w $9477
[000137c4]                           dc.w $887f
[000137c6]                           dc.w $916a
[000137c8]                           dc.w $c46f
[000137ca]                           dc.w $9477
[000137cc]                           dc.w $887f
[000137ce]                           dc.w $c47f
[000137d0]                           dc.w $9477
[000137d2]                           dc.w $887f
[000137d4]                           dc.w $9477
[000137d6]                           dc.w $887f
[000137d8]                           dc.w $916a
[000137da]                           dc.w $c46f
[000137dc]                           dc.w $9477
[000137de]                           dc.w $887f
[000137e0]                           dc.w $0000
[000137e2]                           dc.w $0000
[000137e4]                           dc.w $0000
[000137e6]                           dc.w $0000
[000137e8]                           dc.w $0000
[000137ea]                           dc.w $0000
altcolors[4]: F5
[000137ec]                           dc.b 'Ft@,Dn@,F|@,F|@,F|@,'
[00013800]                           dc.w $8048
[00013802]                           dc.b 'Ft@,Ft@,F,@,F|F,'
[00013812]                           dc.w $8048
[00013814]                           dc.b 'Ft@,Ft@,F$@,F|@,',0
[00013825]                           dc.b $00
[00013826]                           dc.w $0000
[00013828]                           dc.w $0000
[0001382a]                           dc.w $0000
[0001382c]                           dc.w $0000
[0001382e]                           dc.w $0000
altcolors[5]: aqua
[00013830]                           dc.w $957d
[00013832]                           dc.w $904d
[00013834]                           dc.w $914d
[00013836]                           dc.w $904d
[00013838]                           dc.w $957d
[0001383a]                           dc.w $904d
[0001383c]                           dc.w $957d
[0001383e]                           dc.w $904d
[00013840]                           dc.w $957d
[00013842]                           dc.w $904d
[00013844]                           dc.w $8068
[00013846]                           dc.w $957d
[00013848]                           dc.w $904d
[0001384a]                           dc.w $957d
[0001384c]                           dc.w $904d
[0001384e]                           dc.w $9178
[00013850]                           dc.w $9078
[00013852]                           dc.w $957d
[00013854]                           dc.w $904d
[00013856]                           dc.w $8068
[00013858]                           dc.w $957d
[0001385a]                           dc.w $904d
[0001385c]                           dc.w $957d
[0001385e]                           dc.w $904d
[00013860]                           dc.w $9178
[00013862]                           dc.w $9078
[00013864]                           dc.w $957d
[00013866]                           dc.w $904d
[00013868]                           dc.w $0000
[0001386a]                           dc.w $0000
[0001386c]                           dc.w $0000
[0001386e]                           dc.w $0000
[00013870]                           dc.w $0000
[00013872]                           dc.w $0000
altcolors[6]: greys
[00013874]                           dc.w $8178
[00013876]                           dc.w $8968
[00013878]                           dc.w $9948
[0001387a]                           dc.w $8968
[0001387c]                           dc.w $9078
[0001387e]                           dc.w $8068
[00013880]                           dc.w $8078
[00013882]                           dc.w $8068
[00013884]                           dc.w $8078
[00013886]                           dc.w $8068
[00013888]                           dc.w $8068
[0001388a]                           dc.w $9078
[0001388c]                           dc.w $8068
[0001388e]                           dc.w $9078
[00013890]                           dc.w $8068
[00013892]                           dc.w $9169
[00013894]                           dc.w $8068
[00013896]                           dc.w $9178
[00013898]                           dc.w $8068
[0001389a]                           dc.w $8068
[0001389c]                           dc.w $9078
[0001389e]                           dc.w $8068
[000138a0]                           dc.w $9078
[000138a2]                           dc.w $8068
[000138a4]                           dc.w $9169
[000138a6]                           dc.w $8068
[000138a8]                           dc.w $9178
[000138aa]                           dc.w $8068
[000138ac]                           dc.w $0000
[000138ae]                           dc.w $0000
[000138b0]                           dc.w $0000
[000138b2]                           dc.w $0000
[000138b4]                           dc.w $0000
[000138b6]                           dc.w $0000
altcolors[7]: black
[000138b8]                           dc.w $1071
[000138ba]                           dc.w $1061
[000138bc]                           dc.w $1061
[000138be]                           dc.w $1041
[000138c0]                           dc.w $1071
[000138c2]                           dc.w $1041
[000138c4]                           dc.w $1071
[000138c6]                           dc.w $1041
[000138c8]                           dc.w $1071
[000138ca]                           dc.w $1041
[000138cc]                           dc.w $1041
[000138ce]                           dc.w $1071
[000138d0]                           dc.w $1041
[000138d2]                           dc.w $1071
[000138d4]                           dc.w $1041
[000138d6]                           dc.w $1161
[000138d8]                           dc.w $1041
[000138da]                           dc.w $0171
[000138dc]                           dc.w $1041
[000138de]                           dc.w $1041
[000138e0]                           dc.w $1071
[000138e2]                           dc.w $1041
[000138e4]                           dc.w $1071
[000138e6]                           dc.w $1041
[000138e8]                           dc.w $1161
[000138ea]                           dc.w $1041
[000138ec]                           dc.w $0171
[000138ee]                           dc.w $1041
[000138f0]                           dc.w $0000
[000138f2]                           dc.w $0000
[000138f4]                           dc.w $0000
[000138f6]                           dc.w $0000
[000138f8]                           dc.w $0000
[000138fa]                           dc.w $0000
altcolors[8]: greens
[000138fc]                           dc.w $c574
[000138fe]                           dc.b '@uDu@u'
[00013904]                           dc.w $c574
[00013906]                           dc.w $4075
[00013908]                           dc.w $c574
[0001390a]                           dc.w $4075
[0001390c]                           dc.w $c574
[0001390e]                           dc.w $4075
[00013910]                           dc.w $c17d
[00013912]                           dc.w $c574
[00013914]                           dc.w $4075
[00013916]                           dc.w $c574
[00013918]                           dc.w $4075
[0001391a]                           dc.w $c14c
[0001391c]                           dc.w $c14c
[0001391e]                           dc.w $c174
[00013920]                           dc.w $4075
[00013922]                           dc.w $c17d
[00013924]                           dc.w $c574
[00013926]                           dc.w $4075
[00013928]                           dc.w $c574
[0001392a]                           dc.w $4075
[0001392c]                           dc.w $c14c
[0001392e]                           dc.w $4075
[00013930]                           dc.w $c574
[00013932]                           dc.w $c14c
[00013934]                           dc.w $0000
[00013936]                           dc.w $0000
[00013938]                           dc.w $0000
[0001393a]                           dc.w $0000
[0001393c]                           dc.w $0000
[0001393e]                           dc.w $0000
altcolors[9]: default
[00013940]                           dc.w $9178
[00013942]                           dc.w $9078
[00013944]                           dc.w $9948
[00013946]                           dc.w $9078
[00013948]                           dc.w $9178
[0001394a]                           dc.w $9078
[0001394c]                           dc.w $9178
[0001394e]                           dc.w $9078
[00013950]                           dc.w $9178
[00013952]                           dc.w $9078
[00013954]                           dc.w $8068
[00013956]                           dc.w $9178
[00013958]                           dc.w $9078
[0001395a]                           dc.w $9178
[0001395c]                           dc.w $9078
[0001395e]                           dc.w $9179
[00013960]                           dc.w $9079
[00013962]                           dc.w $9178
[00013964]                           dc.w $9078
[00013966]                           dc.w $8068
[00013968]                           dc.w $9178
[0001396a]                           dc.w $9078
[0001396c]                           dc.w $9178
[0001396e]                           dc.w $9078
[00013970]                           dc.w $9179
[00013972]                           dc.w $9079
[00013974]                           dc.w $9178
[00013976]                           dc.w $9078
[00013978]                           dc.w $0000
[0001397a]                           dc.w $0000
[0001397c]                           dc.w $0000
[0001397e]                           dc.w $0000
[00013980]                           dc.w $0000
[00013982]                           dc.w $0000
resource:
[00013984]                           dc.b 'Save',0
[00013989]                           dc.b $00
[0001398a]                           dc.w $4f4b
[0001398c]                           dc.w $0000
[0001398e]                           dc.b 'Cancel',0
[00013995]                           dc.b $00
[00013996]                           dc.b 'dow',0
[0001399a]                           dc.w $0000
[0001399c]                           dc.w $0000
[0001399e]                           dc.b 'Info',0
[000139a3]                           dc.b $00
[000139a4]                           dc.w $0000
[000139a6]                           dc.w $0000
[000139a8]                           dc.b ' Window ',0
[000139b1]                           dc.b $00
[000139b2]                           dc.w $0000
[000139b4]                           dc.w $0000
[000139b6]                           dc.b ' Info',0
[000139bc]                           dc.w $0000
[000139be]                           dc.w $0000
[000139c0]                           dc.w $3133
[000139c2]                           dc.w $0000
[000139c4]                           dc.w $0000
[000139c6]                           dc.w $0000
[000139c8]                           dc.w $3133
[000139ca]                           dc.w $0000
[000139cc]                           dc.w $0000
[000139ce]                           dc.w $0000
[000139d0]                           dc.w $3133
[000139d2]                           dc.w $0000
[000139d4]                           dc.w $0000
[000139d6]                           dc.w $0000
[000139d8]                           dc.b ' Mode ',0
[000139df]                           dc.b $00
[000139e0]                           dc.w $0000
[000139e2]                           dc.w $0000
[000139e4]                           dc.b 'Border ',0
[000139ec]                           dc.w $0000
[000139ee]                           dc.w $0000
[000139f0]                           dc.b 'Text ',0
[000139f6]                           dc.w $0000
[000139f8]                           dc.w $0000
[000139fa]                           dc.b 'Fill ',0
[00013a00]                           dc.w $0000
[00013a02]                           dc.w $0000
[00013a04]                           dc.b 'USA',0
[00013a08]                           dc.w $4f4b
[00013a0a]                           dc.w $0000
[00013a0c]                           dc.b 'Cancel',0
[00013a13]                           dc.b $00
[00013a14]                           dc.b 'Save',0
[00013a19]                           dc.b $00
[00013a1a]                           dc.b ' Window ',0
[00013a23]                           dc.b $00
[00013a24]                           dc.b ' Info',0
[00013a2a]                           dc.b 'Border ',0
[00013a32]                           dc.b 'Text ',0
[00013a38]                           dc.b 'Fill ',0
[00013a3e]                           dc.b ' Mode ',0
[00013a45]                           dc.b $00
[00013a46]                           dc.b 'FRG',0
[00013a4a]                           dc.w $4f4b
[00013a4c]                           dc.w $0000
[00013a4e]                           dc.b 'Abbruch',0
[00013a56]                           dc.b 'Sichern',0
[00013a5e]                           dc.b ' Fenster ',0
[00013a68]                           dc.b ' Info',0
[00013a6e]                           dc.b 'Rand ',0
[00013a74]                           dc.b 'Text ',0
[00013a7a]                           dc.w $4681
[00013a7c]                           dc.b 'llen ',0
[00013a82]                           dc.b ' Modus ',0
[00013a8a]                           dc.b 'FRA',0
[00013a8e]                           dc.b 'CONFIRME',0
[00013a97]                           dc.b $00
[00013a98]                           dc.b 'Annule',0
[00013a9f]                           dc.b $00
[00013aa0]                           dc.b 'Sauve',0
[00013aa6]                           dc.b ' Fen'
[00013aaa]                           dc.w $8874
[00013aac]                           dc.b 're ',0
[00013ab0]                           dc.b ' Info',0
[00013ab6]                           dc.b 'Bord ',0
[00013abc]                           dc.b 'Texte ',0
[00013ac3]                           dc.b $00
[00013ac4]                           dc.b 'Motif ',0
[00013acb]                           dc.b $00
[00013acc]                           dc.b ' Mode ',0
[00013ad3]                           dc.b $00
[00013ad4]                           dc.b 'SPA',0
[00013ad8]                           dc.b 'CONFIRMAR',0
[00013ae2]                           dc.b 'Anular',0
[00013ae9]                           dc.b $00
[00013aea]                           dc.b 'Grabar',0
[00013af1]                           dc.b $00
[00013af2]                           dc.b ' Ventana ',0
[00013afc]                           dc.b ' Informaci'
[00013b06]                           dc.w $a26e
[00013b08]                           dc.w $0000
[00013b0a]                           dc.b 'Borde ',0
[00013b11]                           dc.b $00
[00013b12]                           dc.b 'Texto ',0
[00013b19]                           dc.b $00
[00013b1a]                           dc.b 'Llenado ',0
[00013b23]                           dc.b $00
[00013b24]                           dc.b ' Modo ',0
[00013b2b]                           dc.b $00
[00013b2c]                           dc.b 'ITA',0
[00013b30]                           dc.w $4f4b
[00013b32]                           dc.w $0000
[00013b34]                           dc.b 'Annulla',0
[00013b3c]                           dc.b 'Salva',0
[00013b42]                           dc.b ' Finestra ',0
[00013b4d]                           dc.b $00
[00013b4e]                           dc.b ' Info',0
[00013b54]                           dc.b 'Bordo ',0
[00013b5b]                           dc.b $00
[00013b5c]                           dc.b 'Testo ',0
[00013b63]                           dc.b $00
[00013b64]                           dc.b 'Retino ',0
[00013b6c]                           dc.b ' Modo ',0
[00013b73]                           dc.b $00
[00013b74]                           dc.b 'SWE',0
[00013b78]                           dc.w $4f4b
[00013b7a]                           dc.w $0000
[00013b7c]                           dc.b 'AVBRYT',0
[00013b83]                           dc.b $00
[00013b84]                           dc.b 'SPARA',0
[00013b8a]                           dc.w $2046
[00013b8c]                           dc.w $946e
[00013b8e]                           dc.b 'ster ',0
[00013b94]                           dc.b ' Info',0
[00013b9a]                           dc.b 'Ram ',0
[00013b9f]                           dc.b $00
[00013ba0]                           dc.b 'Text ',0
[00013ba6]                           dc.b 'Bakgrund ',0
[00013bb0]                           dc.w $204c
[00013bb2]                           dc.w $8467
[00013bb4]                           dc.w $6520
[00013bb6]                           dc.w $0000
[00013bb8]                           dc.b '[1][You can not set the|colors of windows|in this TOS version.][  OK  ]',0
[00013c00]                           dc.b '[1][In dieser TOS-Version|k'
[00013c1b]                           dc.b $94
[00013c1c]                           dc.b 'nnen keine Fenster-|farben gesetzt werden.][  OK  ]',0
[00013c50]                           dc.b '[1][Vous ne pouvez pas d'
[00013c68]                           dc.w $8266
[00013c6a]                           dc.b 'inir|les couleurs des fen'
[00013c83]                           dc.b $88
[00013c84]                           dc.b 'tres dans|cette version du TOS.][ CONFIRME ]',0
[00013cb1]                           dc.b $00
[00013cb2]                           dc.b '[1][No es posible configurar|los colores de las ventanas|con esta versi'
[00013cf9]                           dc.b $a2
[00013cfa]                           dc.b 'n de TOS.][CONFIRMAR]',0
[00013d10]                           dc.b '[1][Non '
[00013d18]                           dc.w $8a20
[00013d1a]                           dc.b 'possibile impostare|i colori delle finestre in|questa versione del TOS.][  OK  ]',0
[00013d6b]                           dc.b $00
[00013d6c]                           dc.b '[1][Det '
[00013d74]                           dc.w $8472
[00013d76]                           dc.b ' inte m'
[00013d7d]                           dc.b $94
[00013d7e]                           dc.b 'jligt att '
[00013d88]                           dc.w $846e
[00013d8a]                           dc.b 'dra|f'
[00013d8f]                           dc.b $84
[00013d90]                           dc.b 'rgerna p'
[00013d98]                           dc.w $8620
[00013d9a]                           dc.w $6694
[00013d9c]                           dc.b 'nstren i denna|TOS-version.][  OK  ]',0
[00013dc1]                           dc.b $00
[00013dc2]                           dc.l $00013984
[00013dc6]                           dc.l $0001398a
[00013dca]                           dc.l $0001398e
[00013dce]                           dc.l $00013996
[00013dd2]                           dc.l $0001399a
[00013dd6]                           dc.l $0001399c
[00013dda]                           dc.l $0001399e
[00013dde]                           dc.l $000139a4
[00013de2]                           dc.l $000139a6
[00013de6]                           dc.l $000139a8
[00013dea]                           dc.l $000139b2
[00013dee]                           dc.l $000139b4
[00013df2]                           dc.l $000139b6
[00013df6]                           dc.l $000139bc
[00013dfa]                           dc.l $000139be
[00013dfe]                           dc.l $000139c0
[00013e02]                           dc.l $000139c4
[00013e06]                           dc.l $000139c6
[00013e0a]                           dc.l $000139c8
[00013e0e]                           dc.l $000139cc
[00013e12]                           dc.l $000139ce
[00013e16]                           dc.l $000139d0
[00013e1a]                           dc.l $000139d4
[00013e1e]                           dc.l $000139d6
[00013e22]                           dc.l $000139d8
[00013e26]                           dc.l $000139e0
[00013e2a]                           dc.l $000139e2
[00013e2e]                           dc.l $000139e4
[00013e32]                           dc.l $000139ec
[00013e36]                           dc.l $000139ee
[00013e3a]                           dc.l $000139f0
[00013e3e]                           dc.l $000139f6
[00013e42]                           dc.l $000139f8
[00013e46]                           dc.l $000139fa
[00013e4a]                           dc.l $00013a00
[00013e4e]                           dc.l $00013a02
[00013e52]                           dc.l $00013a04
[00013e56]                           dc.l $00013a08
[00013e5a]                           dc.l $00013a0c
[00013e5e]                           dc.l $00013a14
[00013e62]                           dc.l $00013a1a
[00013e66]                           dc.l $00013a24
[00013e6a]                           dc.l $00013a2a
[00013e6e]                           dc.l $00013a32
[00013e72]                           dc.l $00013a38
[00013e76]                           dc.l $00013a3e
[00013e7a]                           dc.l $00013a46
[00013e7e]                           dc.l $00013a4a
[00013e82]                           dc.l $00013a4e
[00013e86]                           dc.l $00013a56
[00013e8a]                           dc.l $00013a5e
[00013e8e]                           dc.l $00013a68
[00013e92]                           dc.l $00013a6e
[00013e96]                           dc.l $00013a74
[00013e9a]                           dc.l $00013a7a
[00013e9e]                           dc.l $00013a82
[00013ea2]                           dc.l $00013a8a
[00013ea6]                           dc.l $00013a8e
[00013eaa]                           dc.l $00013a98
[00013eae]                           dc.l $00013aa0
[00013eb2]                           dc.l $00013aa6
[00013eb6]                           dc.l $00013ab0
[00013eba]                           dc.l $00013ab6
[00013ebe]                           dc.l $00013abc
[00013ec2]                           dc.l $00013ac4
[00013ec6]                           dc.l $00013acc
[00013eca]                           dc.l $00013ad4
[00013ece]                           dc.l $00013ad8
[00013ed2]                           dc.l $00013ae2
[00013ed6]                           dc.l $00013aea
[00013eda]                           dc.l $00013af2
[00013ede]                           dc.l $00013afc
[00013ee2]                           dc.l $00013b0a
[00013ee6]                           dc.l $00013b12
[00013eea]                           dc.l $00013b1a
[00013eee]                           dc.l $00013b24
[00013ef2]                           dc.l $00013b2c
[00013ef6]                           dc.l $00013b30
[00013efa]                           dc.l $00013b34
[00013efe]                           dc.l $00013b3c
[00013f02]                           dc.l $00013b42
[00013f06]                           dc.l $00013b4e
[00013f0a]                           dc.l $00013b54
[00013f0e]                           dc.l $00013b5c
[00013f12]                           dc.l $00013b64
[00013f16]                           dc.l $00013b6c
[00013f1a]                           dc.l $00013b74
[00013f1e]                           dc.l $00013b78
[00013f22]                           dc.l $00013b7c
[00013f26]                           dc.l $00013b84
[00013f2a]                           dc.l $00013b8a
[00013f2e]                           dc.l $00013b94
[00013f32]                           dc.l $00013b9a
[00013f36]                           dc.l $00013ba0
[00013f3a]                           dc.l $00013ba6
[00013f3e]                           dc.l $00013bb0
[00013f42]                           dc.l $00013bb8
[00013f46]                           dc.l $00013c00
[00013f4a]                           dc.l $00013c50
[00013f4e]                           dc.l $00013cb2
[00013f52]                           dc.l $00013d10
[00013f56]                           dc.l $00013d6c
[00013f5a]                           dc.w $0000
[00013f5c]                           dc.w $0060
[00013f5e]                           dc.w $0000
[00013f60]                           dc.w $0061
[00013f62]                           dc.w $0000
[00013f64]                           dc.w $0062
[00013f66]                           dc.w $0000
[00013f68]                           dc.w $0063
[00013f6a]                           dc.w $0000
[00013f6c]                           dc.w $0064
[00013f6e]                           dc.w $0000
[00013f70]                           dc.w $0065
[00013f72]                           dc.w $0000
[00013f74]                           dc.w $0000
[00013f76]                           dc.w $0000
[00013f78]                           dc.w $0000
[00013f7a]                           dc.w $0000
[00013f7c]                           dc.w $0000
[00013f7e]                           dc.w $0000
[00013f80]                           dc.w $0000
[00013f82]                           dc.w $0000
[00013f84]                           dc.w $0000
[00013f86]                           dc.w $0000
[00013f88]                           dc.w $0000
[00013f8a]                           dc.w $0000
[00013f8c]                           dc.w $0000
[00013f8e]                           dc.w $0000
[00013f90]                           dc.w $0000
[00013f92]                           dc.w $0000
[00013f94]                           dc.w $0000
[00013f96]                           dc.w $0000
[00013f98]                           dc.w $0000
[00013f9a]                           dc.w $0000
[00013f9c]                           dc.w $0000
[00013f9e]                           dc.w $0000
[00013fa0]                           dc.w $0000
[00013fa2]                           dc.w $0000
[00013fa4]                           dc.w $0000
[00013fa6]                           dc.w $0000
[00013fa8]                           dc.w $0003
[00013faa]                           dc.w $0000
[00013fac]                           dc.w $0004
[00013fae]                           dc.w $0000
[00013fb0]                           dc.w $0005
[00013fb2]                           dc.w $0003
[00013fb4]                           dc.w $0006
[00013fb6]                           dc.w $0000
[00013fb8]                           dc.w $11f0
[00013fba]                           dc.w $0000
[00013fbc]                           dc.w $ffff
[00013fbe]                           dc.w $0004
[00013fc0]                           dc.w $0001
[00013fc2]                           dc.w $0000
[00013fc4]                           dc.w $0006
[00013fc6]                           dc.w $0000
[00013fc8]                           dc.w $0007
[00013fca]                           dc.w $0000
[00013fcc]                           dc.w $0008
[00013fce]                           dc.w $0003
[00013fd0]                           dc.w $0006
[00013fd2]                           dc.w $0000
[00013fd4]                           dc.w $1180
[00013fd6]                           dc.w $0000
[00013fd8]                           dc.w $ffff
[00013fda]                           dc.w $0005
[00013fdc]                           dc.w $0001
[00013fde]                           dc.w $0000
[00013fe0]                           dc.w $0009
[00013fe2]                           dc.w $0000
[00013fe4]                           dc.w $000a
[00013fe6]                           dc.w $0000
[00013fe8]                           dc.w $000b
[00013fea]                           dc.w $0003
[00013fec]                           dc.w $0006
[00013fee]                           dc.w $0002
[00013ff0]                           dc.w $11a1
[00013ff2]                           dc.w $0000
[00013ff4]                           dc.w $ffff
[00013ff6]                           dc.w $0009
[00013ff8]                           dc.w $0001
[00013ffa]                           dc.w $0000
[00013ffc]                           dc.w $000c
[00013ffe]                           dc.w $0000
[00014000]                           dc.w $000d
[00014002]                           dc.w $0000
[00014004]                           dc.w $000e
[00014006]                           dc.w $0003
[00014008]                           dc.w $0006
[0001400a]                           dc.w $0000
[0001400c]                           dc.w $1180
[0001400e]                           dc.w $0000
[00014010]                           dc.w $ffff
[00014012]                           dc.w $0006
[00014014]                           dc.w $0001
[00014016]                           dc.w $0000
[00014018]                           dc.w $000f
[0001401a]                           dc.w $0000
[0001401c]                           dc.w $0010
[0001401e]                           dc.w $0000
[00014020]                           dc.w $0011
[00014022]                           dc.w $0003
[00014024]                           dc.w $0006
[00014026]                           dc.w $0002
[00014028]                           dc.w $1170
[0001402a]                           dc.w $0000
[0001402c]                           dc.w $ffff
[0001402e]                           dc.w $0003
[00014030]                           dc.w $0001
[00014032]                           dc.w $0000
[00014034]                           dc.w $0012
[00014036]                           dc.w $0000
[00014038]                           dc.w $0013
[0001403a]                           dc.w $0000
[0001403c]                           dc.w $0014
[0001403e]                           dc.w $0003
[00014040]                           dc.w $0006
[00014042]                           dc.w $0002
[00014044]                           dc.w $1170
[00014046]                           dc.w $0000
[00014048]                           dc.w $ffff
[0001404a]                           dc.w $0003
[0001404c]                           dc.w $0001
[0001404e]                           dc.w $0000
[00014050]                           dc.w $0015
[00014052]                           dc.w $0000
[00014054]                           dc.w $0016
[00014056]                           dc.w $0000
[00014058]                           dc.w $0017
[0001405a]                           dc.w $0003
[0001405c]                           dc.w $0006
[0001405e]                           dc.w $0002
[00014060]                           dc.w $1170
[00014062]                           dc.w $0000
[00014064]                           dc.w $ffff
[00014066]                           dc.w $0003
[00014068]                           dc.w $0001
[0001406a]                           dc.w $0000
[0001406c]                           dc.w $0018
[0001406e]                           dc.w $0000
[00014070]                           dc.w $0019
[00014072]                           dc.w $0000
[00014074]                           dc.w $001a
[00014076]                           dc.w $0003
[00014078]                           dc.w $0006
[0001407a]                           dc.w $0002
[0001407c]                           dc.w $11a1
[0001407e]                           dc.w $0000
[00014080]                           dc.w $ffff
[00014082]                           dc.w $0007
[00014084]                           dc.w $0001
[00014086]                           dc.w $0000
[00014088]                           dc.w $001b
[0001408a]                           dc.w $0000
[0001408c]                           dc.w $001c
[0001408e]                           dc.w $0000
[00014090]                           dc.w $001d
[00014092]                           dc.w $0003
[00014094]                           dc.w $0006
[00014096]                           dc.w $0001
[00014098]                           dc.w $1180
[0001409a]                           dc.w $0000
[0001409c]                           dc.w $ffff
[0001409e]                           dc.w $0008
[000140a0]                           dc.w $0001
[000140a2]                           dc.w $0000
[000140a4]                           dc.w $001e
[000140a6]                           dc.w $0000
[000140a8]                           dc.w $001f
[000140aa]                           dc.w $0000
[000140ac]                           dc.w $0020
[000140ae]                           dc.w $0003
[000140b0]                           dc.w $0006
[000140b2]                           dc.w $0001
[000140b4]                           dc.w $1180
[000140b6]                           dc.w $0000
[000140b8]                           dc.w $ffff
[000140ba]                           dc.w $0006
[000140bc]                           dc.w $0001
[000140be]                           dc.w $0000
[000140c0]                           dc.w $0021
[000140c2]                           dc.w $0000
[000140c4]                           dc.w $0022
[000140c6]                           dc.w $0000
[000140c8]                           dc.w $0023
[000140ca]                           dc.w $0003
[000140cc]                           dc.w $0006
[000140ce]                           dc.w $0001
[000140d0]                           dc.w $1180
[000140d2]                           dc.w $0000
[000140d4]                           dc.w $ffff
[000140d6]                           dc.w $0006
[000140d8]                           dc.w $0001
[000140da]                           dc.w $ffff
[000140dc]                           dc.w $0001
[000140de]                           dc.w $003e
[000140e0]                           dc.w $0014
[000140e2]                           dc.w $0000
[000140e4]                           dc.w $0000
[000140e6]                           dc.w $0000
[000140e8]                           dc.w $1101
[000140ea]                           dc.w $000c
[000140ec]                           dc.w $0602
[000140ee]                           dc.w $0020
[000140f0]                           dc.w $000b
[000140f2]                           dc.w $0003
[000140f4]                           dc.w $0002
[000140f6]                           dc.w $0002
[000140f8]                           dc.w $0014
[000140fa]                           dc.w $0000
[000140fc]                           dc.w $0000
[000140fe]                           dc.w $0000
[00014100]                           dc.w $1109
[00014102]                           dc.w $0100
[00014104]                           dc.w $0009
[00014106]                           dc.w $0709
[00014108]                           dc.w $0002
[0001410a]                           dc.w $0001
[0001410c]                           dc.w $ffff
[0001410e]                           dc.w $ffff
[00014110]                           dc.w $001a
[00014112]                           dc.w $0005
[00014114]                           dc.w $0000
[00014116]                           dc.w $0000
[00014118]                           dc.w $0000
[0001411a]                           dc.w $0001
[0001411c]                           dc.w $0800
[0001411e]                           dc.w $0008
[00014120]                           dc.w $0001
[00014122]                           dc.w $0006
[00014124]                           dc.w $0004
[00014126]                           dc.w $0005
[00014128]                           dc.w $0014
[0001412a]                           dc.w $0000
[0001412c]                           dc.w $0000
[0001412e]                           dc.w $0000
[00014130]                           dc.w $1108
[00014132]                           dc.w $000a
[00014134]                           dc.w $0009
[00014136]                           dc.w $0016
[00014138]                           dc.w $0002
[0001413a]                           dc.w $0005
[0001413c]                           dc.w $ffff
[0001413e]                           dc.w $ffff
[00014140]                           dc.w $001a
[00014142]                           dc.w $0007
[00014144]                           dc.w $0000
[00014146]                           dc.w $0000
[00014148]                           dc.w $0001
[0001414a]                           dc.w $0600
[0001414c]                           dc.w $0800
[0001414e]                           dc.w $0609
[00014150]                           dc.w $0001
[00014152]                           dc.w $0003
[00014154]                           dc.w $ffff
[00014156]                           dc.w $ffff
[00014158]                           dc.w $001a
[0001415a]                           dc.w $0005
[0001415c]                           dc.w $0000
[0001415e]                           dc.w $0000
[00014160]                           dc.w $0002
[00014162]                           dc.w $040b
[00014164]                           dc.w $0800
[00014166]                           dc.w $0609
[00014168]                           dc.w $0001
[0001416a]                           dc.w $0018
[0001416c]                           dc.w $0007
[0001416e]                           dc.w $000b
[00014170]                           dc.w $0019
[00014172]                           dc.w $0000
[00014174]                           dc.w $0020
[00014176]                           dc.w $00ff
[00014178]                           dc.w $1100
[0001417a]                           dc.w $0705
[0001417c]                           dc.w $0d00
[0001417e]                           dc.w $0019
[00014180]                           dc.w $0208
[00014182]                           dc.w $000a
[00014184]                           dc.w $0008
[00014186]                           dc.w $0009
[00014188]                           dc.w $0014
[0001418a]                           dc.w $0000
[0001418c]                           dc.w $0000
[0001418e]                           dc.w $00ff
[00014190]                           dc.w $1100
[00014192]                           dc.w $0000
[00014194]                           dc.w $0000
[00014196]                           dc.w $0019
[00014198]                           dc.w $0101
[0001419a]                           dc.w $0009
[0001419c]                           dc.w $ffff
[0001419e]                           dc.w $ffff
[000141a0]                           dc.w $0016
[000141a2]                           dc.w $0040
[000141a4]                           dc.w $0000
[000141a6]                           dc.w $0000
[000141a8]                           dc.w $0000
[000141aa]                           dc.w $0213
[000141ac]                           dc.w $0000
[000141ae]                           dc.w $0203
[000141b0]                           dc.w $0101
[000141b2]                           dc.w $0007
[000141b4]                           dc.w $ffff
[000141b6]                           dc.w $ffff
[000141b8]                           dc.w $001b
[000141ba]                           dc.w $0040
[000141bc]                           dc.w $0000
[000141be]                           dc.w $07ff
[000141c0]                           dc.w $1100
[000141c2]                           dc.w $0516
[000141c4]                           dc.w $0000
[000141c6]                           dc.w $0302
[000141c8]                           dc.w $0101
[000141ca]                           dc.w $000b
[000141cc]                           dc.w $ffff
[000141ce]                           dc.w $ffff
[000141d0]                           dc.w $0016
[000141d2]                           dc.w $0040
[000141d4]                           dc.w $0000
[000141d6]                           dc.w $0000
[000141d8]                           dc.w $0001
[000141da]                           dc.w $0014
[000141dc]                           dc.w $0201
[000141de]                           dc.w $0005
[000141e0]                           dc.w $0101
[000141e2]                           dc.w $0006
[000141e4]                           dc.w $000c
[000141e6]                           dc.w $0013
[000141e8]                           dc.w $0019
[000141ea]                           dc.w $0000
[000141ec]                           dc.w $0000
[000141ee]                           dc.w $0001
[000141f0]                           dc.w $1100
[000141f2]                           dc.w $0000
[000141f4]                           dc.w $0402
[000141f6]                           dc.w $0019
[000141f8]                           dc.w $0e05
[000141fa]                           dc.w $000d
[000141fc]                           dc.w $ffff
[000141fe]                           dc.w $ffff
[00014200]                           dc.w $0014
[00014202]                           dc.w $0000
[00014204]                           dc.w $0000
[00014206]                           dc.w $0000
[00014208]                           dc.w $1100
[0001420a]                           dc.w $0000
[0001420c]                           dc.w $0000
[0001420e]                           dc.w $0416
[00014210]                           dc.w $0c04
[00014212]                           dc.w $000e
[00014214]                           dc.w $ffff
[00014216]                           dc.w $ffff
[00014218]                           dc.w $001b
[0001421a]                           dc.w $0040
[0001421c]                           dc.w $0000
[0001421e]                           dc.w $06ff
[00014220]                           dc.w $1100
[00014222]                           dc.w $0516
[00014224]                           dc.w $0d04
[00014226]                           dc.w $0302
[00014228]                           dc.w $0101
[0001422a]                           dc.w $0013
[0001422c]                           dc.w $000f
[0001422e]                           dc.w $0012
[00014230]                           dc.w $0014
[00014232]                           dc.w $0000
[00014234]                           dc.w $0000
[00014236]                           dc.w $00ff
[00014238]                           dc.w $1100
[0001423a]                           dc.w $0516
[0001423c]                           dc.w $0000
[0001423e]                           dc.w $0302
[00014240]                           dc.w $0c04
[00014242]                           dc.w $0010
[00014244]                           dc.w $ffff
[00014246]                           dc.w $ffff
[00014248]                           dc.w $001b
[0001424a]                           dc.w $0040
[0001424c]                           dc.w $0000
[0001424e]                           dc.w $01ff
[00014250]                           dc.w $1100
[00014252]                           dc.w $0000
[00014254]                           dc.w $0000
[00014256]                           dc.w $0302
[00014258]                           dc.w $0101
[0001425a]                           dc.w $0011
[0001425c]                           dc.w $ffff
[0001425e]                           dc.w $ffff
[00014260]                           dc.w $001b
[00014262]                           dc.w $0040
[00014264]                           dc.w $0000
[00014266]                           dc.w $02ff
[00014268]                           dc.w $1100
[0001426a]                           dc.w $0000
[0001426c]                           dc.w $0b03
[0001426e]                           dc.w $0302
[00014270]                           dc.w $0101
[00014272]                           dc.w $0012
[00014274]                           dc.w $ffff
[00014276]                           dc.w $ffff
[00014278]                           dc.w $0014
[0001427a]                           dc.w $0040
[0001427c]                           dc.w $0000
[0001427e]                           dc.w $00ff
[00014280]                           dc.w $1100
[00014282]                           dc.w $0000
[00014284]                           dc.w $0201
[00014286]                           dc.w $0302
[00014288]                           dc.w $0301
[0001428a]                           dc.w $000e
[0001428c]                           dc.w $ffff
[0001428e]                           dc.w $ffff
[00014290]                           dc.w $0014
[00014292]                           dc.w $0040
[00014294]                           dc.w $0000
[00014296]                           dc.w $00ff
[00014298]                           dc.w $1111
[0001429a]                           dc.w $0000
[0001429c]                           dc.w $0602
[0001429e]                           dc.w $0302
[000142a0]                           dc.w $0401
[000142a2]                           dc.w $000b
[000142a4]                           dc.w $0014
[000142a6]                           dc.w $0017
[000142a8]                           dc.w $0014
[000142aa]                           dc.w $0040
[000142ac]                           dc.w $0000
[000142ae]                           dc.w $00ff
[000142b0]                           dc.w $1100
[000142b2]                           dc.w $0000
[000142b4]                           dc.w $0d04
[000142b6]                           dc.w $0416
[000142b8]                           dc.w $0101
[000142ba]                           dc.w $0015
[000142bc]                           dc.w $ffff
[000142be]                           dc.w $ffff
[000142c0]                           dc.w $001b
[000142c2]                           dc.w $0040
[000142c4]                           dc.w $0000
[000142c6]                           dc.w $04ff
[000142c8]                           dc.w $1100
[000142ca]                           dc.w $0000
[000142cc]                           dc.w $0000
[000142ce]                           dc.w $0302
[000142d0]                           dc.w $0101
[000142d2]                           dc.w $0016
[000142d4]                           dc.w $ffff
[000142d6]                           dc.w $ffff
[000142d8]                           dc.w $001b
[000142da]                           dc.w $0040
[000142dc]                           dc.w $0000
[000142de]                           dc.w $03ff
[000142e0]                           dc.w $1100
[000142e2]                           dc.w $0114
[000142e4]                           dc.w $0000
[000142e6]                           dc.w $0302
[000142e8]                           dc.w $0101
[000142ea]                           dc.w $0017
[000142ec]                           dc.w $ffff
[000142ee]                           dc.w $ffff
[000142f0]                           dc.w $0014
[000142f2]                           dc.w $0040
[000142f4]                           dc.w $0000
[000142f6]                           dc.w $00ff
[000142f8]                           dc.w $1100
[000142fa]                           dc.w $0402
[000142fc]                           dc.w $0000
[000142fe]                           dc.w $0009
[00014300]                           dc.w $0101
[00014302]                           dc.w $0013
[00014304]                           dc.w $ffff
[00014306]                           dc.w $ffff
[00014308]                           dc.w $0014
[0001430a]                           dc.w $0040
[0001430c]                           dc.w $0000
[0001430e]                           dc.w $00ff
[00014310]                           dc.w $1111
[00014312]                           dc.w $050b
[00014314]                           dc.w $0000
[00014316]                           dc.w $0308
[00014318]                           dc.w $0101
[0001431a]                           dc.w $003e
[0001431c]                           dc.w $0019
[0001431e]                           dc.w $001e
[00014320]                           dc.w $0019
[00014322]                           dc.w $0000
[00014324]                           dc.w $0020
[00014326]                           dc.w $00ff
[00014328]                           dc.w $1100
[0001432a]                           dc.w $0700
[0001432c]                           dc.w $0400
[0001432e]                           dc.w $0019
[00014330]                           dc.w $0208
[00014332]                           dc.w $001d
[00014334]                           dc.w $001a
[00014336]                           dc.w $001c
[00014338]                           dc.w $0014
[0001433a]                           dc.w $0000
[0001433c]                           dc.w $0000
[0001433e]                           dc.w $00ff
[00014340]                           dc.w $1100
[00014342]                           dc.w $0000
[00014344]                           dc.w $0000
[00014346]                           dc.w $0019
[00014348]                           dc.w $0101
[0001434a]                           dc.w $001b
[0001434c]                           dc.w $ffff
[0001434e]                           dc.w $ffff
[00014350]                           dc.w $001b
[00014352]                           dc.w $0040
[00014354]                           dc.w $0000
[00014356]                           dc.w $05ff
[00014358]                           dc.w $1100
[0001435a]                           dc.w $0000
[0001435c]                           dc.w $0000
[0001435e]                           dc.w $0102
[00014360]                           dc.w $0101
[00014362]                           dc.w $001c
[00014364]                           dc.w $ffff
[00014366]                           dc.w $ffff
[00014368]                           dc.w $001b
[0001436a]                           dc.w $0040
[0001436c]                           dc.w $0000
[0001436e]                           dc.w $07ff
[00014370]                           dc.w $1170
[00014372]                           dc.w $0516
[00014374]                           dc.w $0000
[00014376]                           dc.w $0302
[00014378]                           dc.w $0101
[0001437a]                           dc.w $0019
[0001437c]                           dc.w $ffff
[0001437e]                           dc.w $ffff
[00014380]                           dc.w $0016
[00014382]                           dc.w $0040
[00014384]                           dc.w $0000
[00014386]                           dc.w $0000
[00014388]                           dc.w $0002
[0001438a]                           dc.w $0302
[0001438c]                           dc.w $0000
[0001438e]                           dc.w $0014
[00014390]                           dc.w $0101
[00014392]                           dc.w $001e
[00014394]                           dc.w $ffff
[00014396]                           dc.w $ffff
[00014398]                           dc.w $0016
[0001439a]                           dc.w $0040
[0001439c]                           dc.w $0000
[0001439e]                           dc.w $0000
[000143a0]                           dc.w $0003
[000143a2]                           dc.w $0000
[000143a4]                           dc.w $0201
[000143a6]                           dc.w $0019
[000143a8]                           dc.w $0101
[000143aa]                           dc.w $0018
[000143ac]                           dc.w $001f
[000143ae]                           dc.w $003d
[000143b0]                           dc.w $0019
[000143b2]                           dc.w $0000
[000143b4]                           dc.w $0000
[000143b6]                           dc.w $0001
[000143b8]                           dc.w $1100
[000143ba]                           dc.w $0000
[000143bc]                           dc.w $0402
[000143be]                           dc.w $0019
[000143c0]                           dc.w $0e05
[000143c2]                           dc.w $0033
[000143c4]                           dc.w $0020
[000143c6]                           dc.w $0032
[000143c8]                           dc.w $0014
[000143ca]                           dc.w $0000
[000143cc]                           dc.w $0000
[000143ce]                           dc.w $0000
[000143d0]                           dc.w $1100
[000143d2]                           dc.w $0000
[000143d4]                           dc.w $0000
[000143d6]                           dc.w $0416
[000143d8]                           dc.w $0c04
[000143da]                           dc.w $0022
[000143dc]                           dc.w $0021
[000143de]                           dc.w $0021
[000143e0]                           dc.w $0014
[000143e2]                           dc.w $0040
[000143e4]                           dc.w $0000
[000143e6]                           dc.w $0003
[000143e8]                           dc.w $1151
[000143ea]                           dc.w $020c
[000143ec]                           dc.w $0100
[000143ee]                           dc.w $000a
[000143f0]                           dc.w $0001
[000143f2]                           dc.w $0020
[000143f4]                           dc.w $ffff
[000143f6]                           dc.w $ffff
[000143f8]                           dc.w $0016
[000143fa]                           dc.w $0040
[000143fc]                           dc.w $0000
[000143fe]                           dc.w $0000
[00014400]                           dc.w $0004
[00014402]                           dc.w $0006
[00014404]                           dc.w $0000
[00014406]                           dc.w $0003
[00014408]                           dc.w $0001
[0001440a]                           dc.w $0024
[0001440c]                           dc.w $0023
[0001440e]                           dc.w $0023
[00014410]                           dc.w $0014
[00014412]                           dc.w $0040
[00014414]                           dc.w $0000
[00014416]                           dc.w $0003
[00014418]                           dc.w $1151
[0001441a]                           dc.w $020c
[0001441c]                           dc.w $0301
[0001441e]                           dc.w $000a
[00014420]                           dc.w $0001
[00014422]                           dc.w $0022
[00014424]                           dc.w $ffff
[00014426]                           dc.w $ffff
[00014428]                           dc.w $0016
[0001442a]                           dc.w $0040
[0001442c]                           dc.w $0000
[0001442e]                           dc.w $0000
[00014430]                           dc.w $0005
[00014432]                           dc.w $0006
[00014434]                           dc.w $0000
[00014436]                           dc.w $0003
[00014438]                           dc.w $0001
[0001443a]                           dc.w $0026
[0001443c]                           dc.w $0025
[0001443e]                           dc.w $0025
[00014440]                           dc.w $0014
[00014442]                           dc.w $0040
[00014444]                           dc.w $0000
[00014446]                           dc.w $0003
[00014448]                           dc.w $1151
[0001444a]                           dc.w $020c
[0001444c]                           dc.w $0502
[0001444e]                           dc.w $000a
[00014450]                           dc.w $0001
[00014452]                           dc.w $0024
[00014454]                           dc.w $ffff
[00014456]                           dc.w $ffff
[00014458]                           dc.w $0016
[0001445a]                           dc.w $0040
[0001445c]                           dc.w $0000
[0001445e]                           dc.w $0000
[00014460]                           dc.w $0006
[00014462]                           dc.w $0006
[00014464]                           dc.w $0000
[00014466]                           dc.w $0003
[00014468]                           dc.w $0001
[0001446a]                           dc.w $0027
[0001446c]                           dc.w $ffff
[0001446e]                           dc.w $ffff
[00014470]                           dc.w $0016
[00014472]                           dc.w $0040
[00014474]                           dc.w $0000
[00014476]                           dc.w $0000
[00014478]                           dc.w $0007
[0001447a]                           dc.w $0300
[0001447c]                           dc.w $0803
[0001447e]                           dc.w $0709
[00014480]                           dc.w $0101
[00014482]                           dc.w $0030
[00014484]                           dc.w $0028
[00014486]                           dc.w $002f
[00014488]                           dc.w $0014
[0001448a]                           dc.w $0000
[0001448c]                           dc.w $0000
[0001448e]                           dc.w $00ff
[00014490]                           dc.w $1100
[00014492]                           dc.w $020c
[00014494]                           dc.w $0903
[00014496]                           dc.w $000a
[00014498]                           dc.w $0001
[0001449a]                           dc.w $0029
[0001449c]                           dc.w $ffff
[0001449e]                           dc.w $ffff
[000144a0]                           dc.w $0014
[000144a2]                           dc.w $0040
[000144a4]                           dc.w $0000
[000144a6]                           dc.w $0002
[000144a8]                           dc.w $1101
[000144aa]                           dc.w $0000
[000144ac]                           dc.w $0000
[000144ae]                           dc.w $0201
[000144b0]                           dc.w $0001
[000144b2]                           dc.w $002a
[000144b4]                           dc.w $ffff
[000144b6]                           dc.w $ffff
[000144b8]                           dc.w $0014
[000144ba]                           dc.w $0040
[000144bc]                           dc.w $0000
[000144be]                           dc.w $0002
[000144c0]                           dc.w $1111
[000144c2]                           dc.w $0201
[000144c4]                           dc.w $0000
[000144c6]                           dc.w $0201
[000144c8]                           dc.w $0001
[000144ca]                           dc.w $002b
[000144cc]                           dc.w $ffff
[000144ce]                           dc.w $ffff
[000144d0]                           dc.w $0014
[000144d2]                           dc.w $0040
[000144d4]                           dc.w $0000
[000144d6]                           dc.w $0002
[000144d8]                           dc.w $1121
[000144da]                           dc.w $0402
[000144dc]                           dc.w $0000
[000144de]                           dc.w $0201
[000144e0]                           dc.w $0001
[000144e2]                           dc.w $002c
[000144e4]                           dc.w $ffff
[000144e6]                           dc.w $ffff
[000144e8]                           dc.w $0014
[000144ea]                           dc.w $0040
[000144ec]                           dc.w $0000
[000144ee]                           dc.w $0002
[000144f0]                           dc.w $1131
[000144f2]                           dc.w $0603
[000144f4]                           dc.w $0000
[000144f6]                           dc.w $0201
[000144f8]                           dc.w $0001
[000144fa]                           dc.w $002d
[000144fc]                           dc.w $ffff
[000144fe]                           dc.w $ffff
[00014500]                           dc.w $0014
[00014502]                           dc.w $0040
[00014504]                           dc.w $0000
[00014506]                           dc.w $0002
[00014508]                           dc.w $1141
[0001450a]                           dc.w $0005
[0001450c]                           dc.w $0000
[0001450e]                           dc.w $0201
[00014510]                           dc.w $0001
[00014512]                           dc.w $002e
[00014514]                           dc.w $ffff
[00014516]                           dc.w $ffff
[00014518]                           dc.w $0014
[0001451a]                           dc.w $0040
[0001451c]                           dc.w $0000
[0001451e]                           dc.w $0002
[00014520]                           dc.w $1151
[00014522]                           dc.w $0206
[00014524]                           dc.w $0000
[00014526]                           dc.w $0201
[00014528]                           dc.w $0001
[0001452a]                           dc.w $002f
[0001452c]                           dc.w $ffff
[0001452e]                           dc.w $ffff
[00014530]                           dc.w $0014
[00014532]                           dc.w $0040
[00014534]                           dc.w $0000
[00014536]                           dc.w $0002
[00014538]                           dc.w $1161
[0001453a]                           dc.w $0407
[0001453c]                           dc.w $0000
[0001453e]                           dc.w $0201
[00014540]                           dc.w $0001
[00014542]                           dc.w $0027
[00014544]                           dc.w $ffff
[00014546]                           dc.w $ffff
[00014548]                           dc.w $0014
[0001454a]                           dc.w $0040
[0001454c]                           dc.w $0000
[0001454e]                           dc.w $0002
[00014550]                           dc.w $1171
[00014552]                           dc.w $0608
[00014554]                           dc.w $0000
[00014556]                           dc.w $0201
[00014558]                           dc.w $0001
[0001455a]                           dc.w $0031
[0001455c]                           dc.w $ffff
[0001455e]                           dc.w $ffff
[00014560]                           dc.w $0015
[00014562]                           dc.w $0000
[00014564]                           dc.w $0000
[00014566]                           dc.w $0000
[00014568]                           dc.w $0008
[0001456a]                           dc.w $0100
[0001456c]                           dc.w $0100
[0001456e]                           dc.w $000c
[00014570]                           dc.w $0001
[00014572]                           dc.w $0032
[00014574]                           dc.w $ffff
[00014576]                           dc.w $ffff
[00014578]                           dc.w $0015
[0001457a]                           dc.w $0000
[0001457c]                           dc.w $0000
[0001457e]                           dc.w $0000
[00014580]                           dc.w $0009
[00014582]                           dc.w $0100
[00014584]                           dc.w $0301
[00014586]                           dc.w $000c
[00014588]                           dc.w $0001
[0001458a]                           dc.w $001f
[0001458c]                           dc.w $ffff
[0001458e]                           dc.w $ffff
[00014590]                           dc.w $0015
[00014592]                           dc.w $0000
[00014594]                           dc.w $0000
[00014596]                           dc.w $0000
[00014598]                           dc.w $000a
[0001459a]                           dc.w $0100
[0001459c]                           dc.w $0502
[0001459e]                           dc.w $000c
[000145a0]                           dc.w $0001
[000145a2]                           dc.w $0038
[000145a4]                           dc.w $0034
[000145a6]                           dc.w $0037
[000145a8]                           dc.w $0014
[000145aa]                           dc.w $0000
[000145ac]                           dc.w $0000
[000145ae]                           dc.w $00ff
[000145b0]                           dc.w $1100
[000145b2]                           dc.w $0516
[000145b4]                           dc.w $0000
[000145b6]                           dc.w $0302
[000145b8]                           dc.w $0c04
[000145ba]                           dc.w $0035
[000145bc]                           dc.w $ffff
[000145be]                           dc.w $ffff
[000145c0]                           dc.w $001b
[000145c2]                           dc.w $0040
[000145c4]                           dc.w $0000
[000145c6]                           dc.w $01ff
[000145c8]                           dc.w $1100
[000145ca]                           dc.w $0000
[000145cc]                           dc.w $0000
[000145ce]                           dc.w $0302
[000145d0]                           dc.w $0201
[000145d2]                           dc.w $0036
[000145d4]                           dc.w $ffff
[000145d6]                           dc.w $ffff
[000145d8]                           dc.w $001b
[000145da]                           dc.w $0040
[000145dc]                           dc.w $0000
[000145de]                           dc.w $02ff
[000145e0]                           dc.w $1100
[000145e2]                           dc.w $0000
[000145e4]                           dc.w $0c03
[000145e6]                           dc.w $0302
[000145e8]                           dc.w $0001
[000145ea]                           dc.w $0037
[000145ec]                           dc.w $ffff
[000145ee]                           dc.w $ffff
[000145f0]                           dc.w $0014
[000145f2]                           dc.w $0040
[000145f4]                           dc.w $0000
[000145f6]                           dc.w $00ff
[000145f8]                           dc.w $1111
[000145fa]                           dc.w $0000
[000145fc]                           dc.w $0702
[000145fe]                           dc.w $0302
[00014600]                           dc.w $0401
[00014602]                           dc.w $0033
[00014604]                           dc.w $ffff
[00014606]                           dc.w $ffff
[00014608]                           dc.w $0014
[0001460a]                           dc.w $0040
[0001460c]                           dc.w $0000
[0001460e]                           dc.w $00ff
[00014610]                           dc.w $1100
[00014612]                           dc.w $0000
[00014614]                           dc.w $0301
[00014616]                           dc.w $0302
[00014618]                           dc.w $0301
[0001461a]                           dc.w $003d
[0001461c]                           dc.w $0039
[0001461e]                           dc.w $003c
[00014620]                           dc.w $0014
[00014622]                           dc.w $0000
[00014624]                           dc.w $0000
[00014626]                           dc.w $00ff
[00014628]                           dc.w $1100
[0001462a]                           dc.w $0000
[0001462c]                           dc.w $0d04
[0001462e]                           dc.w $0416
[00014630]                           dc.w $0101
[00014632]                           dc.w $003a
[00014634]                           dc.w $ffff
[00014636]                           dc.w $ffff
[00014638]                           dc.w $001b
[0001463a]                           dc.w $0040
[0001463c]                           dc.w $0000
[0001463e]                           dc.w $04ff
[00014640]                           dc.w $1100
[00014642]                           dc.w $0000
[00014644]                           dc.w $0000
[00014646]                           dc.w $0302
[00014648]                           dc.w $0101
[0001464a]                           dc.w $003b
[0001464c]                           dc.w $ffff
[0001464e]                           dc.w $ffff
[00014650]                           dc.w $001b
[00014652]                           dc.w $0040
[00014654]                           dc.w $0000
[00014656]                           dc.w $03ff
[00014658]                           dc.w $1100
[0001465a]                           dc.w $0114
[0001465c]                           dc.w $0000
[0001465e]                           dc.w $0302
[00014660]                           dc.w $0101
[00014662]                           dc.w $003c
[00014664]                           dc.w $ffff
[00014666]                           dc.w $ffff
[00014668]                           dc.w $0014
[0001466a]                           dc.w $0040
[0001466c]                           dc.w $0000
[0001466e]                           dc.w $00ff
[00014670]                           dc.w $1111
[00014672]                           dc.w $0508
[00014674]                           dc.w $0000
[00014676]                           dc.w $030b
[00014678]                           dc.w $0101
[0001467a]                           dc.w $0038
[0001467c]                           dc.w $ffff
[0001467e]                           dc.w $ffff
[00014680]                           dc.w $0014
[00014682]                           dc.w $0040
[00014684]                           dc.w $0000
[00014686]                           dc.w $00ff
[00014688]                           dc.w $1100
[0001468a]                           dc.w $0402
[0001468c]                           dc.w $0000
[0001468e]                           dc.w $0006
[00014690]                           dc.w $0101
[00014692]                           dc.w $001e
[00014694]                           dc.w $ffff
[00014696]                           dc.w $ffff
[00014698]                           dc.w $001b
[0001469a]                           dc.w $0040
[0001469c]                           dc.w $0000
[0001469e]                           dc.w $06ff
[000146a0]                           dc.w $1100
[000146a2]                           dc.w $0516
[000146a4]                           dc.w $0d04
[000146a6]                           dc.w $0302
[000146a8]                           dc.w $0101
[000146aa]                           dc.w $0000
[000146ac]                           dc.w $ffff
[000146ae]                           dc.w $ffff
[000146b0]                           dc.w $0019
[000146b2]                           dc.w $0060
[000146b4]                           dc.w $0000
[000146b6]                           dc.w $00ff
[000146b8]                           dc.w $1100
[000146ba]                           dc.w $0400
[000146bc]                           dc.w $0100
[000146be]                           dc.w $0702
[000146c0]                           dc.w $0701
[000146c2]                           dc.w $ffff
[000146c4]                           dc.w $0001
[000146c6]                           dc.w $0038
[000146c8]                           dc.w $0014
[000146ca]                           dc.w $0000
[000146cc]                           dc.w $0000
[000146ce]                           dc.w $0001
[000146d0]                           dc.w $1100
[000146d2]                           dc.w $0000
[000146d4]                           dc.w $0000
[000146d6]                           dc.w $004e
[000146d8]                           dc.w $0010
[000146da]                           dc.w $000c
[000146dc]                           dc.w $0002
[000146de]                           dc.w $000b
[000146e0]                           dc.w $0014
[000146e2]                           dc.w $0000
[000146e4]                           dc.w $0000
[000146e6]                           dc.w $00ff
[000146e8]                           dc.w $112a
[000146ea]                           dc.w $0002
[000146ec]                           dc.w $0001
[000146ee]                           dc.w $000b
[000146f0]                           dc.w $000e
[000146f2]                           dc.w $0003
[000146f4]                           dc.w $ffff
[000146f6]                           dc.w $ffff
[000146f8]                           dc.w $001a
[000146fa]                           dc.w $0001
[000146fc]                           dc.w $0000
[000146fe]                           dc.w $0000
[00014700]                           dc.w $0024
[00014702]                           dc.w $0000
[00014704]                           dc.w $0000
[00014706]                           dc.w $000b
[00014708]                           dc.w $0001
[0001470a]                           dc.w $0004
[0001470c]                           dc.w $ffff
[0001470e]                           dc.w $ffff
[00014710]                           dc.w $001a
[00014712]                           dc.w $0001
[00014714]                           dc.w $0000
[00014716]                           dc.w $0000
[00014718]                           dc.w $0025
[0001471a]                           dc.w $0001
[0001471c]                           dc.w $0002
[0001471e]                           dc.w $0002
[00014720]                           dc.w $0001
[00014722]                           dc.w $0005
[00014724]                           dc.w $ffff
[00014726]                           dc.w $ffff
[00014728]                           dc.w $001a
[0001472a]                           dc.w $0001
[0001472c]                           dc.w $0000
[0001472e]                           dc.w $0000
[00014730]                           dc.w $0026
[00014732]                           dc.w $0001
[00014734]                           dc.w $0003
[00014736]                           dc.w $0006
[00014738]                           dc.w $0001
[0001473a]                           dc.w $0006
[0001473c]                           dc.w $ffff
[0001473e]                           dc.w $ffff
[00014740]                           dc.w $001a
[00014742]                           dc.w $0001
[00014744]                           dc.w $0000
[00014746]                           dc.w $0000
[00014748]                           dc.w $0027
[0001474a]                           dc.w $0001
[0001474c]                           dc.w $0004
[0001474e]                           dc.w $0004
[00014750]                           dc.w $0001
[00014752]                           dc.w $0007
[00014754]                           dc.w $ffff
[00014756]                           dc.w $ffff
[00014758]                           dc.w $001a
[0001475a]                           dc.w $0001
[0001475c]                           dc.w $0000
[0001475e]                           dc.w $0000
[00014760]                           dc.w $0028
[00014762]                           dc.w $0001
[00014764]                           dc.w $0006
[00014766]                           dc.w $0008
[00014768]                           dc.w $0001
[0001476a]                           dc.w $0008
[0001476c]                           dc.w $ffff
[0001476e]                           dc.w $ffff
[00014770]                           dc.w $001a
[00014772]                           dc.w $0001
[00014774]                           dc.w $0000
[00014776]                           dc.w $0000
[00014778]                           dc.w $0029
[0001477a]                           dc.w $0001
[0001477c]                           dc.w $0007
[0001477e]                           dc.w $0005
[00014780]                           dc.w $0001
[00014782]                           dc.w $0009
[00014784]                           dc.w $ffff
[00014786]                           dc.w $ffff
[00014788]                           dc.w $001a
[0001478a]                           dc.w $0001
[0001478c]                           dc.w $0000
[0001478e]                           dc.w $0000
[00014790]                           dc.w $002a
[00014792]                           dc.w $0001
[00014794]                           dc.w $0009
[00014796]                           dc.w $0007
[00014798]                           dc.w $0001
[0001479a]                           dc.w $000a
[0001479c]                           dc.w $ffff
[0001479e]                           dc.w $ffff
[000147a0]                           dc.w $001a
[000147a2]                           dc.w $0001
[000147a4]                           dc.w $0000
[000147a6]                           dc.w $0000
[000147a8]                           dc.w $002b
[000147aa]                           dc.w $0001
[000147ac]                           dc.w $000a
[000147ae]                           dc.w $0005
[000147b0]                           dc.w $0001
[000147b2]                           dc.w $000b
[000147b4]                           dc.w $ffff
[000147b6]                           dc.w $ffff
[000147b8]                           dc.w $001a
[000147ba]                           dc.w $0001
[000147bc]                           dc.w $0000
[000147be]                           dc.w $0000
[000147c0]                           dc.w $002c
[000147c2]                           dc.w $0001
[000147c4]                           dc.w $000b
[000147c6]                           dc.w $0005
[000147c8]                           dc.w $0001
[000147ca]                           dc.w $0001
[000147cc]                           dc.w $ffff
[000147ce]                           dc.w $ffff
[000147d0]                           dc.w $001a
[000147d2]                           dc.w $0001
[000147d4]                           dc.w $0000
[000147d6]                           dc.w $0000
[000147d8]                           dc.w $002d
[000147da]                           dc.w $0001
[000147dc]                           dc.w $000c
[000147de]                           dc.w $0006
[000147e0]                           dc.w $0001
[000147e2]                           dc.w $0017
[000147e4]                           dc.w $000d
[000147e6]                           dc.w $0016
[000147e8]                           dc.w $0014
[000147ea]                           dc.w $0000
[000147ec]                           dc.w $0000
[000147ee]                           dc.w $00ff
[000147f0]                           dc.w $112a
[000147f2]                           dc.w $000e
[000147f4]                           dc.w $0001
[000147f6]                           dc.w $000b
[000147f8]                           dc.w $000e
[000147fa]                           dc.w $000e
[000147fc]                           dc.w $ffff
[000147fe]                           dc.w $ffff
[00014800]                           dc.w $001a
[00014802]                           dc.w $0000
[00014804]                           dc.w $0000
[00014806]                           dc.w $0000
[00014808]                           dc.w $002e
[0001480a]                           dc.w $0000
[0001480c]                           dc.w $0000
[0001480e]                           dc.w $000b
[00014810]                           dc.w $0001
[00014812]                           dc.w $000f
[00014814]                           dc.w $ffff
[00014816]                           dc.w $ffff
[00014818]                           dc.w $001a
[0001481a]                           dc.w $0000
[0001481c]                           dc.w $0000
[0001481e]                           dc.w $0000
[00014820]                           dc.w $002f
[00014822]                           dc.w $0001
[00014824]                           dc.w $0002
[00014826]                           dc.w $0002
[00014828]                           dc.w $0001
[0001482a]                           dc.w $0010
[0001482c]                           dc.w $ffff
[0001482e]                           dc.w $ffff
[00014830]                           dc.w $001a
[00014832]                           dc.w $0000
[00014834]                           dc.w $0000
[00014836]                           dc.w $0000
[00014838]                           dc.w $0030
[0001483a]                           dc.w $0001
[0001483c]                           dc.w $0003
[0001483e]                           dc.w $0007
[00014840]                           dc.w $0001
[00014842]                           dc.w $0011
[00014844]                           dc.w $ffff
[00014846]                           dc.w $ffff
[00014848]                           dc.w $001a
[0001484a]                           dc.w $0000
[0001484c]                           dc.w $0000
[0001484e]                           dc.w $0000
[00014850]                           dc.w $0031
[00014852]                           dc.w $0001
[00014854]                           dc.w $0004
[00014856]                           dc.w $0007
[00014858]                           dc.w $0001
[0001485a]                           dc.w $0012
[0001485c]                           dc.w $ffff
[0001485e]                           dc.w $ffff
[00014860]                           dc.w $001a
[00014862]                           dc.w $0000
[00014864]                           dc.w $0000
[00014866]                           dc.w $0000
[00014868]                           dc.w $0032
[0001486a]                           dc.w $0001
[0001486c]                           dc.w $0006
[0001486e]                           dc.w $0009
[00014870]                           dc.w $0001
[00014872]                           dc.w $0013
[00014874]                           dc.w $ffff
[00014876]                           dc.w $ffff
[00014878]                           dc.w $001a
[0001487a]                           dc.w $0000
[0001487c]                           dc.w $0000
[0001487e]                           dc.w $0000
[00014880]                           dc.w $0033
[00014882]                           dc.w $0001
[00014884]                           dc.w $0007
[00014886]                           dc.w $0005
[00014888]                           dc.w $0001
[0001488a]                           dc.w $0014
[0001488c]                           dc.w $ffff
[0001488e]                           dc.w $ffff
[00014890]                           dc.w $001a
[00014892]                           dc.w $0000
[00014894]                           dc.w $0000
[00014896]                           dc.w $0000
[00014898]                           dc.w $0034
[0001489a]                           dc.w $0001
[0001489c]                           dc.w $0009
[0001489e]                           dc.w $0005
[000148a0]                           dc.w $0001
[000148a2]                           dc.w $0015
[000148a4]                           dc.w $ffff
[000148a6]                           dc.w $ffff
[000148a8]                           dc.w $001a
[000148aa]                           dc.w $0000
[000148ac]                           dc.w $0000
[000148ae]                           dc.w $0000
[000148b0]                           dc.w $0035
[000148b2]                           dc.w $0001
[000148b4]                           dc.w $000a
[000148b6]                           dc.w $0005
[000148b8]                           dc.w $0001
[000148ba]                           dc.w $0016
[000148bc]                           dc.w $ffff
[000148be]                           dc.w $ffff
[000148c0]                           dc.w $001a
[000148c2]                           dc.w $0000
[000148c4]                           dc.w $0000
[000148c6]                           dc.w $0000
[000148c8]                           dc.w $0036
[000148ca]                           dc.w $0001
[000148cc]                           dc.w $000b
[000148ce]                           dc.w $0007
[000148d0]                           dc.w $0001
[000148d2]                           dc.w $000c
[000148d4]                           dc.w $ffff
[000148d6]                           dc.w $ffff
[000148d8]                           dc.w $001a
[000148da]                           dc.w $0000
[000148dc]                           dc.w $0000
[000148de]                           dc.w $0000
[000148e0]                           dc.w $0037
[000148e2]                           dc.w $0001
[000148e4]                           dc.w $000c
[000148e6]                           dc.w $0007
[000148e8]                           dc.w $0001
[000148ea]                           dc.w $0022
[000148ec]                           dc.w $0018
[000148ee]                           dc.w $0021
[000148f0]                           dc.w $0014
[000148f2]                           dc.w $0000
[000148f4]                           dc.w $0000
[000148f6]                           dc.w $00ff
[000148f8]                           dc.w $112a
[000148fa]                           dc.w $001a
[000148fc]                           dc.w $0001
[000148fe]                           dc.w $000b
[00014900]                           dc.w $000e
[00014902]                           dc.w $0019
[00014904]                           dc.w $ffff
[00014906]                           dc.w $ffff
[00014908]                           dc.w $001a
[0001490a]                           dc.w $0000
[0001490c]                           dc.w $0000
[0001490e]                           dc.w $0000
[00014910]                           dc.w $0038
[00014912]                           dc.w $0000
[00014914]                           dc.w $0000
[00014916]                           dc.w $000b
[00014918]                           dc.w $0001
[0001491a]                           dc.w $001a
[0001491c]                           dc.w $ffff
[0001491e]                           dc.w $ffff
[00014920]                           dc.w $001a
[00014922]                           dc.w $0000
[00014924]                           dc.w $0000
[00014926]                           dc.w $0000
[00014928]                           dc.w $0039
[0001492a]                           dc.w $0001
[0001492c]                           dc.w $0002
[0001492e]                           dc.w $0008
[00014930]                           dc.w $0001
[00014932]                           dc.w $001b
[00014934]                           dc.w $ffff
[00014936]                           dc.w $ffff
[00014938]                           dc.w $001a
[0001493a]                           dc.w $0000
[0001493c]                           dc.w $0000
[0001493e]                           dc.w $0000
[00014940]                           dc.w $003a
[00014942]                           dc.w $0001
[00014944]                           dc.w $0003
[00014946]                           dc.w $0006
[00014948]                           dc.w $0001
[0001494a]                           dc.w $001c
[0001494c]                           dc.w $ffff
[0001494e]                           dc.w $ffff
[00014950]                           dc.w $001a
[00014952]                           dc.w $0000
[00014954]                           dc.w $0000
[00014956]                           dc.w $0000
[00014958]                           dc.w $003b
[0001495a]                           dc.w $0001
[0001495c]                           dc.w $0004
[0001495e]                           dc.w $0005
[00014960]                           dc.w $0001
[00014962]                           dc.w $001d
[00014964]                           dc.w $ffff
[00014966]                           dc.w $ffff
[00014968]                           dc.w $001a
[0001496a]                           dc.w $0000
[0001496c]                           dc.w $0000
[0001496e]                           dc.w $0000
[00014970]                           dc.w $003c
[00014972]                           dc.w $0001
[00014974]                           dc.w $0006
[00014976]                           dc.w $0009
[00014978]                           dc.w $0001
[0001497a]                           dc.w $001e
[0001497c]                           dc.w $ffff
[0001497e]                           dc.w $ffff
[00014980]                           dc.w $001a
[00014982]                           dc.w $0000
[00014984]                           dc.w $0000
[00014986]                           dc.w $0000
[00014988]                           dc.w $003d
[0001498a]                           dc.w $0001
[0001498c]                           dc.w $0007
[0001498e]                           dc.w $0005
[00014990]                           dc.w $0001
[00014992]                           dc.w $001f
[00014994]                           dc.w $ffff
[00014996]                           dc.w $ffff
[00014998]                           dc.w $001a
[0001499a]                           dc.w $0000
[0001499c]                           dc.w $0000
[0001499e]                           dc.w $0000
[000149a0]                           dc.w $003e
[000149a2]                           dc.w $0001
[000149a4]                           dc.w $0009
[000149a6]                           dc.w $0005
[000149a8]                           dc.w $0001
[000149aa]                           dc.w $0020
[000149ac]                           dc.w $ffff
[000149ae]                           dc.w $ffff
[000149b0]                           dc.w $001a
[000149b2]                           dc.w $0000
[000149b4]                           dc.w $0000
[000149b6]                           dc.w $0000
[000149b8]                           dc.w $003f
[000149ba]                           dc.w $0001
[000149bc]                           dc.w $000a
[000149be]                           dc.w $0006
[000149c0]                           dc.w $0001
[000149c2]                           dc.w $0021
[000149c4]                           dc.w $ffff
[000149c6]                           dc.w $ffff
[000149c8]                           dc.w $001a
[000149ca]                           dc.w $0000
[000149cc]                           dc.w $0000
[000149ce]                           dc.w $0000
[000149d0]                           dc.w $0040
[000149d2]                           dc.w $0001
[000149d4]                           dc.w $000b
[000149d6]                           dc.w $0006
[000149d8]                           dc.w $0001
[000149da]                           dc.w $0017
[000149dc]                           dc.w $ffff
[000149de]                           dc.w $ffff
[000149e0]                           dc.w $001a
[000149e2]                           dc.w $0000
[000149e4]                           dc.w $0000
[000149e6]                           dc.w $0000
[000149e8]                           dc.w $0041
[000149ea]                           dc.w $0001
[000149ec]                           dc.w $000c
[000149ee]                           dc.w $0006
[000149f0]                           dc.w $0001
[000149f2]                           dc.w $002d
[000149f4]                           dc.w $0023
[000149f6]                           dc.w $002c
[000149f8]                           dc.w $0014
[000149fa]                           dc.w $0000
[000149fc]                           dc.w $0000
[000149fe]                           dc.w $00ff
[00014a00]                           dc.w $112a
[00014a02]                           dc.w $0026
[00014a04]                           dc.w $0001
[00014a06]                           dc.w $000e
[00014a08]                           dc.w $000e
[00014a0a]                           dc.w $0024
[00014a0c]                           dc.w $ffff
[00014a0e]                           dc.w $ffff
[00014a10]                           dc.w $001a
[00014a12]                           dc.w $0000
[00014a14]                           dc.w $0000
[00014a16]                           dc.w $0000
[00014a18]                           dc.w $0042
[00014a1a]                           dc.w $0000
[00014a1c]                           dc.w $0000
[00014a1e]                           dc.w $000e
[00014a20]                           dc.w $0001
[00014a22]                           dc.w $0025
[00014a24]                           dc.w $ffff
[00014a26]                           dc.w $ffff
[00014a28]                           dc.w $001a
[00014a2a]                           dc.w $0000
[00014a2c]                           dc.w $0000
[00014a2e]                           dc.w $0000
[00014a30]                           dc.w $0043
[00014a32]                           dc.w $0001
[00014a34]                           dc.w $0002
[00014a36]                           dc.w $0009
[00014a38]                           dc.w $0001
[00014a3a]                           dc.w $0026
[00014a3c]                           dc.w $ffff
[00014a3e]                           dc.w $ffff
[00014a40]                           dc.w $001a
[00014a42]                           dc.w $0000
[00014a44]                           dc.w $0000
[00014a46]                           dc.w $0000
[00014a48]                           dc.w $0044
[00014a4a]                           dc.w $0001
[00014a4c]                           dc.w $0003
[00014a4e]                           dc.w $0006
[00014a50]                           dc.w $0001
[00014a52]                           dc.w $0027
[00014a54]                           dc.w $ffff
[00014a56]                           dc.w $ffff
[00014a58]                           dc.w $001a
[00014a5a]                           dc.w $0000
[00014a5c]                           dc.w $0000
[00014a5e]                           dc.w $0000
[00014a60]                           dc.w $0045
[00014a62]                           dc.w $0001
[00014a64]                           dc.w $0004
[00014a66]                           dc.w $0006
[00014a68]                           dc.w $0001
[00014a6a]                           dc.w $0028
[00014a6c]                           dc.w $ffff
[00014a6e]                           dc.w $ffff
[00014a70]                           dc.w $001a
[00014a72]                           dc.w $0000
[00014a74]                           dc.w $0000
[00014a76]                           dc.w $0000
[00014a78]                           dc.w $0046
[00014a7a]                           dc.w $0001
[00014a7c]                           dc.w $0006
[00014a7e]                           dc.w $0009
[00014a80]                           dc.w $0001
[00014a82]                           dc.w $0029
[00014a84]                           dc.w $ffff
[00014a86]                           dc.w $ffff
[00014a88]                           dc.w $001a
[00014a8a]                           dc.w $0000
[00014a8c]                           dc.w $0000
[00014a8e]                           dc.w $0000
[00014a90]                           dc.w $0047
[00014a92]                           dc.w $0001
[00014a94]                           dc.w $0007
[00014a96]                           dc.w $000c
[00014a98]                           dc.w $0001
[00014a9a]                           dc.w $002a
[00014a9c]                           dc.w $ffff
[00014a9e]                           dc.w $ffff
[00014aa0]                           dc.w $001a
[00014aa2]                           dc.w $0000
[00014aa4]                           dc.w $0000
[00014aa6]                           dc.w $0000
[00014aa8]                           dc.w $0048
[00014aaa]                           dc.w $0001
[00014aac]                           dc.w $0009
[00014aae]                           dc.w $0006
[00014ab0]                           dc.w $0001
[00014ab2]                           dc.w $002b
[00014ab4]                           dc.w $ffff
[00014ab6]                           dc.w $ffff
[00014ab8]                           dc.w $001a
[00014aba]                           dc.w $0000
[00014abc]                           dc.w $0000
[00014abe]                           dc.w $0000
[00014ac0]                           dc.w $0049
[00014ac2]                           dc.w $0001
[00014ac4]                           dc.w $000a
[00014ac6]                           dc.w $0006
[00014ac8]                           dc.w $0001
[00014aca]                           dc.w $002c
[00014acc]                           dc.w $ffff
[00014ace]                           dc.w $ffff
[00014ad0]                           dc.w $001a
[00014ad2]                           dc.w $0000
[00014ad4]                           dc.w $0000
[00014ad6]                           dc.w $0000
[00014ad8]                           dc.w $004a
[00014ada]                           dc.w $0001
[00014adc]                           dc.w $000b
[00014ade]                           dc.w $0008
[00014ae0]                           dc.w $0001
[00014ae2]                           dc.w $0022
[00014ae4]                           dc.w $ffff
[00014ae6]                           dc.w $ffff
[00014ae8]                           dc.w $001a
[00014aea]                           dc.w $0000
[00014aec]                           dc.w $0000
[00014aee]                           dc.w $0000
[00014af0]                           dc.w $004b
[00014af2]                           dc.w $0001
[00014af4]                           dc.w $000c
[00014af6]                           dc.w $0006
[00014af8]                           dc.w $0001
[00014afa]                           dc.w $0038
[00014afc]                           dc.w $002e
[00014afe]                           dc.w $0037
[00014b00]                           dc.w $0014
[00014b02]                           dc.w $0000
[00014b04]                           dc.w $0000
[00014b06]                           dc.w $00ff
[00014b08]                           dc.w $112a
[00014b0a]                           dc.w $0035
[00014b0c]                           dc.w $0001
[00014b0e]                           dc.w $000c
[00014b10]                           dc.w $000e
[00014b12]                           dc.w $002f
[00014b14]                           dc.w $ffff
[00014b16]                           dc.w $ffff
[00014b18]                           dc.w $001a
[00014b1a]                           dc.w $0000
[00014b1c]                           dc.w $0000
[00014b1e]                           dc.w $0000
[00014b20]                           dc.w $004c
[00014b22]                           dc.w $0000
[00014b24]                           dc.w $0000
[00014b26]                           dc.w $000c
[00014b28]                           dc.w $0001
[00014b2a]                           dc.w $0030
[00014b2c]                           dc.w $ffff
[00014b2e]                           dc.w $ffff
[00014b30]                           dc.w $001a
[00014b32]                           dc.w $0000
[00014b34]                           dc.w $0000
[00014b36]                           dc.w $0000
[00014b38]                           dc.w $004d
[00014b3a]                           dc.w $0001
[00014b3c]                           dc.w $0002
[00014b3e]                           dc.w $0002
[00014b40]                           dc.w $0001
[00014b42]                           dc.w $0031
[00014b44]                           dc.w $ffff
[00014b46]                           dc.w $ffff
[00014b48]                           dc.w $001a
[00014b4a]                           dc.w $0000
[00014b4c]                           dc.w $0000
[00014b4e]                           dc.w $0000
[00014b50]                           dc.w $004e
[00014b52]                           dc.w $0001
[00014b54]                           dc.w $0003
[00014b56]                           dc.w $0007
[00014b58]                           dc.w $0001
[00014b5a]                           dc.w $0032
[00014b5c]                           dc.w $ffff
[00014b5e]                           dc.w $ffff
[00014b60]                           dc.w $001a
[00014b62]                           dc.w $0000
[00014b64]                           dc.w $0000
[00014b66]                           dc.w $0000
[00014b68]                           dc.w $004f
[00014b6a]                           dc.w $0001
[00014b6c]                           dc.w $0004
[00014b6e]                           dc.w $0005
[00014b70]                           dc.w $0001
[00014b72]                           dc.w $0033
[00014b74]                           dc.w $ffff
[00014b76]                           dc.w $ffff
[00014b78]                           dc.w $001a
[00014b7a]                           dc.w $0000
[00014b7c]                           dc.w $0000
[00014b7e]                           dc.w $0000
[00014b80]                           dc.w $0050
[00014b82]                           dc.w $0001
[00014b84]                           dc.w $0006
[00014b86]                           dc.w $000a
[00014b88]                           dc.w $0001
[00014b8a]                           dc.w $0034
[00014b8c]                           dc.w $ffff
[00014b8e]                           dc.w $ffff
[00014b90]                           dc.w $001a
[00014b92]                           dc.w $0000
[00014b94]                           dc.w $0000
[00014b96]                           dc.w $0000
[00014b98]                           dc.w $0051
[00014b9a]                           dc.w $0001
[00014b9c]                           dc.w $0007
[00014b9e]                           dc.w $0005
[00014ba0]                           dc.w $0001
[00014ba2]                           dc.w $0035
[00014ba4]                           dc.w $ffff
[00014ba6]                           dc.w $ffff
[00014ba8]                           dc.w $001a
[00014baa]                           dc.w $0000
[00014bac]                           dc.w $0000
[00014bae]                           dc.w $0000
[00014bb0]                           dc.w $0052
[00014bb2]                           dc.w $0001
[00014bb4]                           dc.w $0009
[00014bb6]                           dc.w $0006
[00014bb8]                           dc.w $0001
[00014bba]                           dc.w $0036
[00014bbc]                           dc.w $ffff
[00014bbe]                           dc.w $ffff
[00014bc0]                           dc.w $001a
[00014bc2]                           dc.w $0000
[00014bc4]                           dc.w $0000
[00014bc6]                           dc.w $0000
[00014bc8]                           dc.w $0053
[00014bca]                           dc.w $0001
[00014bcc]                           dc.w $000a
[00014bce]                           dc.w $0006
[00014bd0]                           dc.w $0001
[00014bd2]                           dc.w $0037
[00014bd4]                           dc.w $ffff
[00014bd6]                           dc.w $ffff
[00014bd8]                           dc.w $001a
[00014bda]                           dc.w $0000
[00014bdc]                           dc.w $0000
[00014bde]                           dc.w $0000
[00014be0]                           dc.w $0054
[00014be2]                           dc.w $0001
[00014be4]                           dc.w $000b
[00014be6]                           dc.w $0007
[00014be8]                           dc.w $0001
[00014bea]                           dc.w $002d
[00014bec]                           dc.w $ffff
[00014bee]                           dc.w $ffff
[00014bf0]                           dc.w $001a
[00014bf2]                           dc.w $0000
[00014bf4]                           dc.w $0000
[00014bf6]                           dc.w $0000
[00014bf8]                           dc.w $0055
[00014bfa]                           dc.w $0001
[00014bfc]                           dc.w $000c
[00014bfe]                           dc.w $0006
[00014c00]                           dc.w $0001
[00014c02]                           dc.w $0000
[00014c04]                           dc.w $0039
[00014c06]                           dc.w $0042
[00014c08]                           dc.w $0014
[00014c0a]                           dc.w $0000
[00014c0c]                           dc.w $0000
[00014c0e]                           dc.w $00ff
[00014c10]                           dc.w $112a
[00014c12]                           dc.w $0042
[00014c14]                           dc.w $0001
[00014c16]                           dc.w $000b
[00014c18]                           dc.w $000e
[00014c1a]                           dc.w $003a
[00014c1c]                           dc.w $ffff
[00014c1e]                           dc.w $ffff
[00014c20]                           dc.w $001a
[00014c22]                           dc.w $0000
[00014c24]                           dc.w $0000
[00014c26]                           dc.w $0000
[00014c28]                           dc.w $0056
[00014c2a]                           dc.w $0000
[00014c2c]                           dc.w $0000
[00014c2e]                           dc.w $000b
[00014c30]                           dc.w $0001
[00014c32]                           dc.w $003b
[00014c34]                           dc.w $ffff
[00014c36]                           dc.w $ffff
[00014c38]                           dc.w $001a
[00014c3a]                           dc.w $0000
[00014c3c]                           dc.w $0000
[00014c3e]                           dc.w $0000
[00014c40]                           dc.w $0057
[00014c42]                           dc.w $0001
[00014c44]                           dc.w $0002
[00014c46]                           dc.w $0002
[00014c48]                           dc.w $0001
[00014c4a]                           dc.w $003c
[00014c4c]                           dc.w $ffff
[00014c4e]                           dc.w $ffff
[00014c50]                           dc.w $001a
[00014c52]                           dc.w $0000
[00014c54]                           dc.w $0000
[00014c56]                           dc.w $0000
[00014c58]                           dc.w $0058
[00014c5a]                           dc.w $0001
[00014c5c]                           dc.w $0003
[00014c5e]                           dc.w $0006
[00014c60]                           dc.w $0001
[00014c62]                           dc.w $003d
[00014c64]                           dc.w $ffff
[00014c66]                           dc.w $ffff
[00014c68]                           dc.w $001a
[00014c6a]                           dc.w $0000
[00014c6c]                           dc.w $0000
[00014c6e]                           dc.w $0000
[00014c70]                           dc.w $0059
[00014c72]                           dc.w $0001
[00014c74]                           dc.w $0004
[00014c76]                           dc.w $0005
[00014c78]                           dc.w $0001
[00014c7a]                           dc.w $003e
[00014c7c]                           dc.w $ffff
[00014c7e]                           dc.w $ffff
[00014c80]                           dc.w $001a
[00014c82]                           dc.w $0000
[00014c84]                           dc.w $0000
[00014c86]                           dc.w $0000
[00014c88]                           dc.w $005a
[00014c8a]                           dc.w $0001
[00014c8c]                           dc.w $0006
[00014c8e]                           dc.w $0009
[00014c90]                           dc.w $0001
[00014c92]                           dc.w $003f
[00014c94]                           dc.w $ffff
[00014c96]                           dc.w $ffff
[00014c98]                           dc.w $001a
[00014c9a]                           dc.w $0000
[00014c9c]                           dc.w $0000
[00014c9e]                           dc.w $0000
[00014ca0]                           dc.w $005b
[00014ca2]                           dc.w $0001
[00014ca4]                           dc.w $0007
[00014ca6]                           dc.w $0005
[00014ca8]                           dc.w $0001
[00014caa]                           dc.w $0040
[00014cac]                           dc.w $ffff
[00014cae]                           dc.w $ffff
[00014cb0]                           dc.w $001a
[00014cb2]                           dc.w $0000
[00014cb4]                           dc.w $0000
[00014cb6]                           dc.w $0000
[00014cb8]                           dc.w $005c
[00014cba]                           dc.w $0001
[00014cbc]                           dc.w $0009
[00014cbe]                           dc.w $0004
[00014cc0]                           dc.w $0001
[00014cc2]                           dc.w $0041
[00014cc4]                           dc.w $ffff
[00014cc6]                           dc.w $ffff
[00014cc8]                           dc.w $001a
[00014cca]                           dc.w $0000
[00014ccc]                           dc.w $0000
[00014cce]                           dc.w $0000
[00014cd0]                           dc.w $005d
[00014cd2]                           dc.w $0001
[00014cd4]                           dc.w $000a
[00014cd6]                           dc.w $0005
[00014cd8]                           dc.w $0001
[00014cda]                           dc.w $0042
[00014cdc]                           dc.w $ffff
[00014cde]                           dc.w $ffff
[00014ce0]                           dc.w $001a
[00014ce2]                           dc.w $0000
[00014ce4]                           dc.w $0000
[00014ce6]                           dc.w $0000
[00014ce8]                           dc.w $005e
[00014cea]                           dc.w $0001
[00014cec]                           dc.w $000b
[00014cee]                           dc.w $0009
[00014cf0]                           dc.w $0001
[00014cf2]                           dc.w $0038
[00014cf4]                           dc.w $ffff
[00014cf6]                           dc.w $ffff
[00014cf8]                           dc.w $001a
[00014cfa]                           dc.w $0020
[00014cfc]                           dc.w $0000
[00014cfe]                           dc.w $0000
[00014d00]                           dc.w $005f
[00014d02]                           dc.w $0001
[00014d04]                           dc.w $000c
[00014d06]                           dc.w $0006
[00014d08]                           dc.w $0001
[00014d0a]                           dc.w $0000
[00014d0c]                           dc.w $0000
[00014d0e]                           dc.w $0000
[00014d10]                           dc.w $003f
[00014d12]                           dc.w $0000
[00014d14]                           dc.w $0000
[00014d16]                           dc.w $0000
[00014d18]                           dc.b 'WCOLORX.RSC',0
[00014d24]                           dc.w $0000
[00014d26]                           dc.w $01c2
[00014d28]                           dc.w $0000
[00014d2a]                           dc.w $014a
[00014d2c]                           dc.w $0000
[00014d2e]                           dc.w $0113
[00014d30]                           dc.w $0000
[00014d32]                           dc.w $00dc
[00014d34]                           dc.w $0000
[00014d36]                           dc.w $00a5
[00014d38]                           dc.w $003d
[00014d3a]                           dc.w $001a
[00014d3c]                           dc.w $001b
[00014d3e]                           dc.w $0034
[00014d40]                           dc.w $0035
[00014d42]                           dc.w $0037
[00014d44]                           dc.w $0039
[00014d46]                           dc.w $003a
[00014d48]                           dc.w $003c
[00014d4a]                           dc.w $000d
[00014d4c]                           dc.w $0009
[00014d4e]                           dc.w $000f
[00014d50]                           dc.w $0010
[00014d52]                           dc.w $0011
[00014d54]                           dc.w $0014
[00014d56]                           dc.w $0015
[00014d58]                           dc.w $0016
[00014d5a]                           dc.w $001c
[00014d5c]                           dc.w $0008
[00014d5e]                           dc.w $0000
