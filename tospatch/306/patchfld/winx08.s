[00e211cc] 5749                      subq.w    #3,a1
[00e211ce] 4e58                      unlk      a0
[00e211d0] 0220 1b51                 andi.b    #$51,-(a0)
[00e211d4] 205f                      movea.l   (a7)+,a0
[00e211d6] 4879 0000 94b6            pea.l     $000094B6
[00e211dc] 2f08                      move.l    a0,-(a7)
[00e211de] 2479 0000 e54a            movea.l   $0000E54A,a2
[00e211e4] 342a 001c                 move.w    28(a2),d2
[00e211e8] 43ef 0004                 lea.l     4(a7),a1
[00e211ec] 2059                      movea.l   (a1)+,a0
[00e211ee] 2f08                      move.l    a0,-(a7)
[00e211f0] 3f3c 0010                 move.w    #$0010,-(a7)
[00e211f4] 30d9                      move.w    (a1)+,(a0)+
[00e211f6] 3f19                      move.w    (a1)+,-(a7)
[00e211f8] 30c2                      move.w    d2,(a0)+
[00e211fa] 4258                      clr.w     (a0)+
[00e211fc] 30d9                      move.w    (a1)+,(a0)+
[00e211fe] 20d9                      move.l    (a1)+,(a0)+
[00e21200] 2091                      move.l    (a1),(a0)
[00e21202] 3f3c 0002                 move.w    #$0002,-(a7)
[00e21206] b46f 0002                 cmp.w     2(a7),d2
[00e2120a] 6608                      bne.s     $00E21214
[00e2120c] 0c6a 04f0 0094            cmpi.w    #$04F0,148(a2)
[00e21212] 6e06                      bgt.s     $00E2121A
[00e21214] 6100 88e8                 bsr       $00E19AFE
[00e21218] 4e71                      nop
[00e2121a] 4fef 000a                 lea.l     10(a7),a7
[00e2121e] 4e75                      rts
[00e21220] 2079 0000 aefc            movea.l   $0000AEFC,a0 /* winxvars.wx_mem */
[00e21226] 4e75                      rts
[00e21228] 2f00                      move.l    d0,-(a7)
[00e2122a] 61f4                      bsr.s     $00E21220
[00e2122c] c1fc 0122                 muls.w    #$0122,d0
[00e21230] d1c0                      adda.l    d0,a0
[00e21232] 201f                      move.l    (a7)+,d0
[00e21234] 4e75                      rts
[00e21236] 3039 0000 aefa            move.w    $0000AEFA,d0
[00e2123c] 6014                      bra.s     $00E21252
[00e2123e] 2079 0000 e54a            movea.l   $0000E54A,a0
[00e21244] 6008                      bra.s     $00E2124E
[00e21246] 3007                      move.w    d7,d0
[00e21248] 61de                      bsr.s     $00E21228
[00e2124a] 2068 0002                 movea.l   2(a0),a0
[00e2124e] 3028 0096                 move.w    150(a0),d0
[00e21252] 0300                      btst      d1,d0
[00e21254] 4e75                      rts
[00e21256] 61d0                      bsr.s     $00E21228
[00e21258] 4a41                      tst.w     d1
[00e2125a] 6c02                      bge.s     $00E2125E
[00e2125c] 4441                      neg.w     d1
[00e2125e] e749                      lsl.w     #3,d1
[00e21260] 41f0 1010                 lea.l     16(a0,d1.w),a0
[00e21264] 4e75                      rts
[00e21266] 7203                      moveq.l   #3,d1
[00e21268] 2f08                      move.l    a0,-(a7)
[00e2126a] 3f01                      move.w    d1,-(a7)
[00e2126c] 61e8                      bsr.s     $00E21256
[00e2126e] 2248                      movea.l   a0,a1
[00e21270] 321f                      move.w    (a7)+,d1
[00e21272] 2057                      movea.l   (a7),a0
[00e21274] 20d9                      move.l    (a1)+,(a0)+
[00e21276] 2091                      move.l    (a1),(a0)
[00e21278] 4a41                      tst.w     d1
[00e2127a] 6c0c                      bge.s     $00E21288
[00e2127c] 4a58                      tst.w     (a0)+
[00e2127e] 6708                      beq.s     $00E21288
[00e21280] 4a58                      tst.w     (a0)+
[00e21282] 6704                      beq.s     $00E21288
[00e21284] 5460                      addq.w    #2,-(a0)
[00e21286] 5460                      addq.w    #2,-(a0)
[00e21288] 205f                      movea.l   (a7)+,a0
[00e2128a] 4e75                      rts
[00e2128c] 7000                      moveq.l   #0,d0
[00e2128e] 41f9 0000 94a2            lea.l     $000094A2,a0
[00e21294] 43f9 0000 94ac            lea.l     $000094AC,a1
[00e2129a] 2448                      movea.l   a0,a2
[00e2129c] 2f09                      move.l    a1,-(a7)
[00e2129e] 7200                      moveq.l   #0,d1
[00e212a0] 6112                      bsr.s     $00E212B4
[00e212a2] 205f                      movea.l   (a7)+,a0
[00e212a4] 7203                      moveq.l   #3,d1
[00e212a6] 610c                      bsr.s     $00E212B4
[00e212a8] 7202                      moveq.l   #2,d1
[00e212aa] 6108                      bsr.s     $00E212B4
[00e212ac] 220a                      move.l    a2,d1
[00e212ae] 6710                      beq.s     $00E212C0
[00e212b0] 2041                      movea.l   d1,a0
[00e212b2] 7201                      moveq.l   #1,d1
[00e212b4] 2f08                      move.l    a0,-(a7)
[00e212b6] 619e                      bsr.s     $00E21256
[00e212b8] 2257                      movea.l   (a7),a1
[00e212ba] 20d9                      move.l    (a1)+,(a0)+
[00e212bc] 20d9                      move.l    (a1)+,(a0)+
[00e212be] 205f                      movea.l   (a7)+,a0
[00e212c0] 4e75                      rts
[00e212c2] 2f09                      move.l    a1,-(a7)
[00e212c4] 2f08                      move.l    a0,-(a7)
[00e212c6] 6100 4a46                 bsr       $00E25D0E
[00e212ca] 4e71                      nop
[00e212cc] 508f                      addq.l    #8,a7
[00e212ce] 4a40                      tst.w     d0
[00e212d0] 4e75                      rts
[00e212d2] 204d                      movea.l   a5,a0
[00e212d4] 3007                      move.w    d7,d0
[00e212d6] 6190                      bsr.s     $00E21268
[00e212d8] 224d                      movea.l   a5,a1
[00e212da] 41f9 0000 94a2            lea.l     $000094A2,a0
[00e212e0] 61e0                      bsr.s     $00E212C2
[00e212e2] 671e                      beq.s     $00E21302
[00e212e4] 200c                      move.l    a4,d0
[00e212e6] 6708                      beq.s     $00E212F0
[00e212e8] 224d                      movea.l   a5,a1
[00e212ea] 204c                      movea.l   a4,a0
[00e212ec] 61d4                      bsr.s     $00E212C2
[00e212ee] 6712                      beq.s     $00E21302
[00e212f0] 200b                      move.l    a3,d0
[00e212f2] 5280                      addq.l    #1,d0
[00e212f4] 660a                      bne.s     $00E21300
[00e212f6] 3007                      move.w    d7,d0
[00e212f8] 6100 ff2e                 bsr       $00E21228
[00e212fc] 2668 009e                 movea.l   158(a0),a3
[00e21300] 200b                      move.l    a3,d0
[00e21302] 4e75                      rts
[00e21304] 2f09                      move.l    a1,-(a7)
[00e21306] 2f08                      move.l    a0,-(a7)
[00e21308] 6100 4a6e                 bsr       $00E25D78
[00e2130c] 4e71                      nop
[00e2130e] 508f                      addq.l    #8,a7
[00e21310] 4e75                      rts
[00e21312] 2f08                      move.l    a0,-(a7)
[00e21314] 2f00                      move.l    d0,-(a7)
[00e21316] 6100 49aa                 bsr       $00E25CC2
[00e2131a] 4e71                      nop
[00e2131c] 508f                      addq.l    #8,a7
[00e2131e] 4e75                      rts
[00e21320] 6100 ff06                 bsr       $00E21228
[00e21324] 2068 0032                 movea.l   50(a0),a0
[00e21328] 3028 0030                 move.w    48(a0),d0
[00e2132c] 4e75                      rts
[00e2132e] 6100 fef8                 bsr       $00E21228
[00e21332] 2468 0032                 movea.l   50(a0),a2
[00e21336] 2268 0036                 movea.l   54(a0),a1
[00e2133a] 234a 0032                 move.l    a2,50(a1)
[00e2133e] 2549 0036                 move.l    a1,54(a2)
[00e21342] 42a8 0032                 clr.l     50(a0)
[00e21346] 42a8 0036                 clr.l     54(a0)
[00e2134a] 4e75                      rts
[00e2134c] 6100 feda                 bsr       $00E21228
[00e21350] 2248                      movea.l   a0,a1
[00e21352] 6100 fecc                 bsr       $00E21220
[00e21356] 2468 0036                 movea.l   54(a0),a2
[00e2135a] 2549 0032                 move.l    a1,50(a2)
[00e2135e] 234a 0036                 move.l    a2,54(a1)
[00e21362] 2348 0032                 move.l    a0,50(a1)
[00e21366] 2149 0036                 move.l    a1,54(a0)
[00e2136a] 4e75                      rts
[00e2136c] 3f01                      move.w    d1,-(a7)
[00e2136e] 6100 ffbe                 bsr.w     $00E2132E
[00e21372] 2248                      movea.l   a0,a1
[00e21374] 6100 feaa                 bsr       $00E21220
[00e21378] 4a5f                      tst.w     (a7)+
[00e2137a] 6604                      bne.s     $00E21380
[00e2137c] 2068 0032                 movea.l   50(a0),a0
[00e21380] 60d4                      bra.s     $00E21356
[00e21382] 2f00                      move.l    d0,-(a7)
[00e21384] 2f08                      move.l    a0,-(a7)
[00e21386] 6100 28fa                 bsr       $00E23C82
[00e2138a] 508f                      addq.l    #8,a7
[00e2138c] 4e75                      rts
[00e2138e] 2f09                      move.l    a1,-(a7)
[00e21390] 3f00                      move.w    d0,-(a7)
[00e21392] 2f08                      move.l    a0,-(a7)
[00e21394] 6100 2c54                 bsr       $00E23FEA
[00e21398] defc 000a                 adda.w    #$000A,a7
[00e2139c] 4e75                      rts
[00e2139e] 2f08                      move.l    a0,-(a7)
[00e213a0] 6100 305e                 bsr       $00E24400
[00e213a4] 4e71                      nop
[00e213a6] 588f                      addq.l    #4,a7
[00e213a8] 4e75                      rts
[00e213aa] 7001                      moveq.l   #1,d0
[00e213ac] be79 0000 95d6            cmp.w     $000095D6,d7
[00e213b2] 6708                      beq.s     $00E213BC
[00e213b4] 7200                      moveq.l   #0,d1
[00e213b6] 6100 fe8e                 bsr       $00E21246
[00e213ba] 56c0                      sne       d0
[00e213bc] 4880                      ext.w     d0
[00e213be] 4e75                      rts
[00e213c0] c07c 0004                 and.w     #$0004,d0
[00e213c4] 6708                      beq.s     $00E213CE
[00e213c6] 7208                      moveq.l   #8,d1
[00e213c8] 6100 fe6c                 bsr       $00E21236
[00e213cc] 6606                      bne.s     $00E213D4
[00e213ce] 61da                      bsr.s     $00E213AA
[00e213d0] 6714                      beq.s     $00E213E6
[00e213d2] 4e75                      rts
[00e213d4] 6100 fe4a                 bsr       $00E21220
[00e213d8] 2268 0036                 movea.l   54(a0),a1
[00e213dc] 3029 0030                 move.w    48(a1),d0
[00e213e0] 6712                      beq.s     $00E213F4
[00e213e2] be40                      cmp.w     d0,d7
[00e213e4] 6704                      beq.s     $00E213EA
[00e213e6] 7815                      moveq.l   #21,d4
[00e213e8] 600a                      bra.s     $00E213F4
[00e213ea] 7209                      moveq.l   #9,d1
[00e213ec] 6100 fe48                 bsr       $00E21236
[00e213f0] 6702                      beq.s     $00E213F4
[00e213f2] 7821                      moveq.l   #33,d4
[00e213f4] 7000                      moveq.l   #0,d0
[00e213f6] 4e75                      rts
[00e213f8] 2e00                      move.l    d0,d7
[00e213fa] 43fa fffc                 lea.l     $00E213F8(pc),a1
[00e213fe] 2079 0000 e54a            movea.l   $0000E54A,a0
[00e21404] 2f08                      move.l    a0,-(a7)
[00e21406] 2049                      movea.l   a1,a0
[00e21408] 7007                      moveq.l   #7,d0
[00e2140a] 6118                      bsr.s     $00E21424
[00e2140c] 205f                      movea.l   (a7)+,a0
[00e2140e] 660e                      bne.s     $00E2141E
[00e21410] 303c 0fff                 move.w    #$0FFF,d0
[00e21414] 0c11 002e                 cmpi.b    #$2E,(a1)
[00e21418] 6604                      bne.s     $00E2141E
[00e2141a] 303c 0fff                 move.w    #$0FFF,d0
[00e2141e] 3140 0096                 move.w    d0,150(a0)
[00e21422] 4e75                      rts
[00e21424] 2239 0000 af04            move.l    $0000AF04,d1
[00e2142a] 6712                      beq.s     $00E2143E
[00e2142c] 2f08                      move.l    a0,-(a7)
[00e2142e] 3f00                      move.w    d0,-(a7)
[00e21430] 2041                      movea.l   d1,a0
[00e21432] 4e90                      jsr       (a0)
[00e21434] 5c8f                      addq.l    #6,a7
[00e21436] 4a80                      tst.l     d0
[00e21438] 6b04                      bmi.s     $00E2143E
[00e2143a] 7201                      moveq.l   #1,d1
[00e2143c] 4e75                      rts
[00e2143e] 7200                      moveq.l   #0,d1
[00e21440] 4e75                      rts
[00e21442] 4a40                      tst.w     d0
[00e21444] 6604                      bne.s     $00E2144A
[00e21446] d2fc 002e                 adda.w    #$002E,a1
[00e2144a] 7016                      moveq.l   #22,d0
[00e2144c] 0c28 0016 0007            cmpi.b    #$16,7(a0)
[00e21452] 660a                      bne.s     $00E2145E
[00e21454] 2468 000c                 movea.l   12(a0),a2
[00e21458] 3559 0012                 move.w    (a1)+,18(a2)
[00e2145c] 6004                      bra.s     $00E21462
[00e2145e] 3159 000e                 move.w    (a1)+,14(a0)
[00e21462] d0fc 0018                 adda.w    #$0018,a0
[00e21466] 51c8 ffe4                 dbf       d0,$00E2144C
[00e2146a] 4e75                      rts
[00e2146c] 6100 fdba                 bsr       $00E21228
[00e21470] 3010                      move.w    (a0),d0
[00e21472] 2079 0000 9214            movea.l   $00009214,a0
[00e21478] 0800 0004                 btst      #4,d0
[00e2147c] 6704                      beq.s     $00E21482
[00e2147e] d0fc 0260                 adda.w    #$0260,a0
[00e21482] 4e75                      rts
[00e21484] 4e56 ffee                 link      a6,#$FFEE
[00e21488] 48e7 5f1c                 movem.l   d1/d3-d7/a3-a5,-(a7)
[00e2148c] 3c01                      move.w    d1,d6
[00e2148e] 2d79 0000 af08 ffee       move.l    $0000AF08,-18(a6)
[00e21496] 47ee fff2                 lea.l     -14(a6),a3
[00e2149a] 3007                      move.w    d7,d0
[00e2149c] 6100 fd8a                 bsr       $00E21228
[00e214a0] 2a48                      movea.l   a0,a5
[00e214a2] 6100 fda6                 bsr       $00E2124A
[00e214a6] 3d40 fffe                 move.w    d0,-2(a6)
[00e214aa] 3007                      move.w    d7,d0
[00e214ac] 61be                      bsr.s     $00E2146C
[00e214ae] 2848                      movea.l   a0,a4
[00e214b0] 4a46                      tst.w     d6
[00e214b2] 6704                      beq.s     $00E214B8
[00e214b4] 6100 0c44                 bsr       $00E220FA
[00e214b8] 6100 fef0                 bsr       $00E213AA
[00e214bc] 3d40 fffc                 move.w    d0,-4(a6)
[00e214c0] 6c02                      bge.s     $00E214C4
[00e214c2] 4240                      clr.w     d0
[00e214c4] 3d40 fffa                 move.w    d0,-6(a6)
[00e214c8] 70ff                      moveq.l   #-1,d0
[00e214ca] 2940 0002                 move.l    d0,2(a4)
[00e214ce] 3c2d 0006                 move.w    6(a5),d6
[00e214d2] 204b                      movea.l   a3,a0
[00e214d4] 3007                      move.w    d7,d0
[00e214d6] 6100 fd8e                 bsr       $00E21266
[00e214da] 41ec 0010                 lea.l     16(a4),a0
[00e214de] 224b                      movea.l   a3,a1
[00e214e0] 20d9                      move.l    (a1)+,(a0)+
[00e214e2] 2091                      move.l    (a1),(a0)
[00e214e4] 206d 0002                 movea.l   2(a5),a0
[00e214e8] 6100 11e0                 bsr       $00E226CA
[00e214ec] 3a00                      move.w    d0,d5
[00e214ee] 4840                      swap      d0
[00e214f0] 3800                      move.w    d0,d4
[00e214f2] 43ed 0042                 lea.l     66(a5),a1
[00e214f6] 302e fffa                 move.w    -6(a6),d0
[00e214fa] 204c                      movea.l   a4,a0
[00e214fc] 6100 ff44                 bsr       $00E21442
[00e21500] 4293                      clr.l     (a3)
[00e21502] 3e2e fff0                 move.w    -16(a6),d7
[00e21506] 5347                      subq.w    #1,d7
[00e21508] 3006                      move.w    d6,d0
[00e2150a] c07c 0007                 and.w     #$0007,d0
[00e2150e] 6700 007c                 beq.w     $00E2158C
[00e21512] 397c 0014 001e            move.w    #$0014,30(a4)
[00e21518] 7001                      moveq.l   #1,d0
[00e2151a] 6100 0190                 bsr       $00E216AC
[00e2151e] 20d3                      move.l    (a3),(a0)+
[00e21520] 30eb 0004                 move.w    4(a3),(a0)+
[00e21524] 362b 0004                 move.w    4(a3),d3
[00e21528] 0806 0001                 btst      #1,d6
[00e2152c] 6716                      beq.s     $00E21544
[00e2152e] 4a6e fffc                 tst.w     -4(a6)
[00e21532] 6710                      beq.s     $00E21544
[00e21534] 7022                      moveq.l   #34,d0
[00e21536] 6100 0174                 bsr       $00E216AC
[00e2153a] 20d3                      move.l    (a3),(a0)+
[00e2153c] 302e ffee                 move.w    -18(a6),d0
[00e21540] d153                      add.w     d0,(a3)
[00e21542] 9640                      sub.w     d0,d3
[00e21544] 0806 0002                 btst      #2,d6
[00e21548] 671a                      beq.s     $00E21564
[00e2154a] 4a6e fffc                 tst.w     -4(a6)
[00e2154e] 6714                      beq.s     $00E21564
[00e21550] 966e ffee                 sub.w     -18(a6),d3
[00e21554] 7024                      moveq.l   #36,d0
[00e21556] 6100 0154                 bsr       $00E216AC
[00e2155a] 3003                      move.w    d3,d0
[00e2155c] d053                      add.w     (a3),d0
[00e2155e] 30c0                      move.w    d0,(a0)+
[00e21560] 30eb 0002                 move.w    2(a3),(a0)+
[00e21564] 0806 0000                 btst      #0,d6
[00e21568] 6718                      beq.s     $00E21582
[00e2156a] 397c 0019 001e            move.w    #$0019,30(a4)
[00e21570] 206c 0054                 movea.l   84(a4),a0
[00e21574] 20ad 0008                 move.l    8(a5),(a0)
[00e21578] 7023                      moveq.l   #35,d0
[00e2157a] 6100 0130                 bsr       $00E216AC
[00e2157e] 20d3                      move.l    (a3),(a0)+
[00e21580] 30c3                      move.w    d3,(a0)+
[00e21582] 4253                      clr.w     (a3)
[00e21584] df6b 0002                 add.w     d7,2(a3)
[00e21588] 9f6b 0006                 sub.w     d7,6(a3)
[00e2158c] 362e ffee                 move.w    -18(a6),d3
[00e21590] 5343                      subq.w    #1,d3
[00e21592] 0806 0004                 btst      #4,d6
[00e21596] 671c                      beq.s     $00E215B4
[00e21598] 206c 0084                 movea.l   132(a4),a0
[00e2159c] 20ad 000c                 move.l    12(a5),(a0)
[00e215a0] 7005                      moveq.l   #5,d0
[00e215a2] 6100 0108                 bsr       $00E216AC
[00e215a6] 20d3                      move.l    (a3),(a0)+
[00e215a8] 30eb 0004                 move.w    4(a3),(a0)+
[00e215ac] df6b 0002                 add.w     d7,2(a3)
[00e215b0] 9f6b 0006                 sub.w     d7,6(a3)
[00e215b4] 7006                      moveq.l   #6,d0
[00e215b6] 6100 00f4                 bsr       $00E216AC
[00e215ba] 20db                      move.l    (a3)+,(a0)+
[00e215bc] 20db                      move.l    (a3)+,(a0)+
[00e215be] 5563                      subq.w    #2,-(a3)
[00e215c0] 4a44                      tst.w     d4
[00e215c2] 6702                      beq.s     $00E215C6
[00e215c4] 9f53                      sub.w     d7,(a3)
[00e215c6] 5563                      subq.w    #2,-(a3)
[00e215c8] 4a45                      tst.w     d5
[00e215ca] 6702                      beq.s     $00E215CE
[00e215cc] 9753                      sub.w     d3,(a3)
[00e215ce] 273c 0001 0001            move.l    #$00010001,-(a3)
[00e215d4] 303c 00c7                 move.w    #$00C7,d0
[00e215d8] 6100 00d2                 bsr       $00E216AC
[00e215dc] 20d3                      move.l    (a3),(a0)+
[00e215de] 20ab 0004                 move.l    4(a3),(a0)
[00e215e2] 0806 0005                 btst      #5,d6
[00e215e6] 6716                      beq.s     $00E215FE
[00e215e8] 4a45                      tst.w     d5
[00e215ea] 670a                      beq.s     $00E215F6
[00e215ec] 4a44                      tst.w     d4
[00e215ee] 660e                      bne.s     $00E215FE
[00e215f0] 9f6b 0006                 sub.w     d7,6(a3)
[00e215f4] 6008                      bra.s     $00E215FE
[00e215f6] 4a44                      tst.w     d4
[00e215f8] 6704                      beq.s     $00E215FE
[00e215fa] 976b 0004                 sub.w     d3,4(a3)
[00e215fe] 202b 0004                 move.l    4(a3),d0
[00e21602] d16b 0002                 add.w     d0,2(a3)
[00e21606] 4840                      swap      d0
[00e21608] d153                      add.w     d0,(a3)
[00e2160a] 0806 0005                 btst      #5,d6
[00e2160e] 56c7                      sne       d7
[00e21610] 4a45                      tst.w     d5
[00e21612] 671c                      beq.s     $00E21630
[00e21614] 4a44                      tst.w     d4
[00e21616] 6702                      beq.s     $00E2161A
[00e21618] 7e01                      moveq.l   #1,d7
[00e2161a] 322b 0006                 move.w    6(a3),d1
[00e2161e] 4257                      clr.w     (a7)
[00e21620] 3f13                      move.w    (a3),-(a7)
[00e21622] 2f2e fffc                 move.l    -4(a6),-(a7)
[00e21626] 45fa 006c                 lea.l     $00E21694(pc),a2
[00e2162a] 6100 00ae                 bsr       $00E216DA
[00e2162e] 5c8f                      addq.l    #6,a7
[00e21630] 4a44                      tst.w     d4
[00e21632] 6718                      beq.s     $00E2164C
[00e21634] 322b 0004                 move.w    4(a3),d1
[00e21638] 3eab 0002                 move.w    2(a3),(a7)
[00e2163c] 4267                      clr.w     -(a7)
[00e2163e] 2f2e fffc                 move.l    -4(a6),-(a7)
[00e21642] 45fa 0038                 lea.l     $00E2167C(pc),a2
[00e21646] 6100 0092                 bsr       $00E216DA
[00e2164a] 5c8f                      addq.l    #6,a7
[00e2164c] 4a07                      tst.b     d7
[00e2164e] 671e                      beq.s     $00E2166E
[00e21650] 303c 00c8                 move.w    #$00C8,d0
[00e21654] 6100 0056                 bsr.w     $00E216AC
[00e21658] 20d3                      move.l    (a3),(a0)+
[00e2165a] 7000                      moveq.l   #0,d0
[00e2165c] 4a6e fffc                 tst.w     -4(a6)
[00e21660] 6708                      beq.s     $00E2166A
[00e21662] 0806 0005                 btst      #5,d6
[00e21666] 6702                      beq.s     $00E2166A
[00e21668] 7006                      moveq.l   #6,d0
[00e2166a] 1940 00cc                 move.b    d0,204(a4)
[00e2166e] 4e71                      nop
[00e21670] 4e71                      nop
[00e21672] 204c                      movea.l   a4,a0
[00e21674] 4cdf 38fa                 movem.l   (a7)+,d1/d3-d7/a3-a5
[00e21678] 4e5e                      unlk      a6
[00e2167a] 4e75                      rts
[00e2167c] 0000                      dc.w      $0000
[00e2167e] 0150                      bchg      d0,(a0)
[00e21680] 00ce                      dc.w      $00CE ; illegal
[00e21682] 000a 01d0                 ori.b     #$D0,a2 ; apollo only
[00e21686] 01d6                      bset      d0,(a6)
[00e21688] 0009 01cf                 ori.b     #$CF,a1 ; apollo only
[00e2168c] 01d5                      bset      d0,(a5)
[00e2168e] 000b 01d1                 ori.b     #$D1,a3 ; apollo only
[00e21692] 0232 0002 00d8            andi.b    #$02,-40(a2,d0.w)
[00e21698] 00c9                      dc.w      $00C9 ; illegal
[00e2169a] 0007 012b                 ori.b     #$2B,d7
[00e2169e] 0134 0006                 btst      d0,6(a4,d0.w)
[00e216a2] 012a 0133                 btst      d0,307(a2)
[00e216a6] 0008 012c                 ori.b     #$2C,a0 ; apollo only
[00e216aa] 018d 3200                 movep.w   d0,12800(a5)
[00e216ae] c27c 001f                 and.w     #$001F,d1
[00e216b2] ea48                      lsr.w     #5,d0
[00e216b4] 4840                      swap      d0
[00e216b6] 3001                      move.w    d1,d0
[00e216b8] c3fc 0018                 muls.w    #$0018,d1
[00e216bc] 74ff                      moveq.l   #-1,d2
[00e216be] 2982 1802                 move.l    d2,2(a4,d1.l)
[00e216c2] 4874 1810                 pea.l     16(a4,d1.l)
[00e216c6] 204c                      movea.l   a4,a0
[00e216c8] 6100 fcb8                 bsr       $00E21382
[00e216cc] 205f                      movea.l   (a7)+,a0
[00e216ce] 4290                      clr.l     (a0)
[00e216d0] 2179 0000 af08 0004       move.l    $0000AF08,4(a0)
[00e216d8] 4e75                      rts
[00e216da] 48e7 5e1e                 movem.l   d1/d3-d6/a3-a6,-(a7)
[00e216de] 2c4a                      movea.l   a2,a6
[00e216e0] 3a01                      move.w    d1,d5
[00e216e2] 5445                      addq.w    #2,d5
[00e216e4] 361e                      move.w    (a6)+,d3
[00e216e6] 264c                      movea.l   a4,a3
[00e216e8] d6de                      adda.w    (a6)+,a3
[00e216ea] 377c 0014 0006            move.w    #$0014,6(a3)
[00e216f0] 301e                      move.w    (a6)+,d0
[00e216f2] 61b8                      bsr.s     $00E216AC
[00e216f4] 20af 002c                 move.l    44(a7),(a0)
[00e216f8] 3185 3004                 move.w    d5,4(a0,d3.w)
[00e216fc] 4a6f 0028                 tst.w     40(a7)
[00e21700] 6700 0102                 beq       $00E21804
[00e21704] 7800                      moveq.l   #0,d4
[00e21706] 321e                      move.w    (a6)+,d1
[00e21708] 301e                      move.w    (a6)+,d0
[00e2170a] 0306                      btst      d1,d6
[00e2170c] 672e                      beq.s     $00E2173C
[00e2170e] 619c                      bsr.s     $00E216AC
[00e21710] 9a70 3004                 sub.w     4(a0,d3.w),d5
[00e21714] 3185 3000                 move.w    d5,0(a0,d3.w)
[00e21718] 5245                      addq.w    #1,d5
[00e2171a] 322f 002a                 move.w    42(a7),d1
[00e2171e] 0801 0009                 btst      #9,d1
[00e21722] 6718                      beq.s     $00E2173C
[00e21724] 3016                      move.w    (a6),d0
[00e21726] 6184                      bsr.s     $00E216AC
[00e21728] 3030 3004                 move.w    4(a0,d3.w),d0
[00e2172c] 5640                      addq.w    #3,d0
[00e2172e] e248                      lsr.w     #1,d0
[00e21730] 3180 3004                 move.w    d0,4(a0,d3.w)
[00e21734] 9a40                      sub.w     d0,d5
[00e21736] 3185 3000                 move.w    d5,0(a0,d3.w)
[00e2173a] 5245                      addq.w    #1,d5
[00e2173c] 548e                      addq.l    #2,a6
[00e2173e] 321e                      move.w    (a6)+,d1
[00e21740] 301e                      move.w    (a6)+,d0
[00e21742] 0306                      btst      d1,d6
[00e21744] 674e                      beq.s     $00E21794
[00e21746] 6100 ff64                 bsr       $00E216AC
[00e2174a] 9a70 3004                 sub.w     4(a0,d3.w),d5
[00e2174e] 322f 002a                 move.w    42(a7),d1
[00e21752] 0801 0008                 btst      #8,d1
[00e21756] 670c                      beq.s     $00E21764
[00e21758] 3184 3000                 move.w    d4,0(a0,d3.w)
[00e2175c] d870 3004                 add.w     4(a0,d3.w),d4
[00e21760] 5344                      subq.w    #1,d4
[00e21762] 6004                      bra.s     $00E21768
[00e21764] 3185 3000                 move.w    d5,0(a0,d3.w)
[00e21768] 5245                      addq.w    #1,d5
[00e2176a] 0801 0008                 btst      #8,d1
[00e2176e] 6724                      beq.s     $00E21794
[00e21770] 0801 0009                 btst      #9,d1
[00e21774] 671e                      beq.s     $00E21794
[00e21776] 3016                      move.w    (a6),d0
[00e21778] 6100 ff32                 bsr       $00E216AC
[00e2177c] 3030 3004                 move.w    4(a0,d3.w),d0
[00e21780] 5640                      addq.w    #3,d0
[00e21782] e248                      lsr.w     #1,d0
[00e21784] 3180 3004                 move.w    d0,4(a0,d3.w)
[00e21788] 3184 3000                 move.w    d4,0(a0,d3.w)
[00e2178c] d840                      add.w     d0,d4
[00e2178e] 5344                      subq.w    #1,d4
[00e21790] 9a40                      sub.w     d0,d5
[00e21792] 5245                      addq.w    #1,d5
[00e21794] 548e                      addq.l    #2,a6
[00e21796] 321e                      move.w    (a6)+,d1
[00e21798] 0306                      btst      d1,d6
[00e2179a] 6768                      beq.s     $00E21804
[00e2179c] 377c 0019 0006            move.w    #$0019,6(a3)
[00e217a2] 301e                      move.w    (a6)+,d0
[00e217a4] 6100 ff06                 bsr       $00E216AC
[00e217a8] 3184 3000                 move.w    d4,0(a0,d3.w)
[00e217ac] 3185 3004                 move.w    d5,4(a0,d3.w)
[00e217b0] 301e                      move.w    (a6)+,d0
[00e217b2] 6100 fef8                 bsr       $00E216AC
[00e217b6] 3830 3004                 move.w    4(a0,d3.w),d4
[00e217ba] 343c 03e8                 move.w    #$03E8,d2
[00e217be] 3235 303e                 move.w    62(a5,d3.w),d1
[00e217c2] b27c ffff                 cmp.w     #$FFFF,d1
[00e217c6] 6708                      beq.s     $00E217D0
[00e217c8] 3005                      move.w    d5,d0
[00e217ca] 613e                      bsr.s     $00E2180A
[00e217cc] b044                      cmp.w     d4,d0
[00e217ce] 6c02                      bge.s     $00E217D2
[00e217d0] 3004                      move.w    d4,d0
[00e217d2] 3780 3074                 move.w    d0,116(a3,d3.w)
[00e217d6] 3205                      move.w    d5,d1
[00e217d8] 9240                      sub.w     d0,d1
[00e217da] 3035 303a                 move.w    58(a5,d3.w),d0
[00e217de] 612a                      bsr.s     $00E2180A
[00e217e0] 3780 3070                 move.w    d0,112(a3,d3.w)
[00e217e4] 7219                      moveq.l   #25,d1
[00e217e6] 3033 305c                 move.w    92(a3,d3.w),d0
[00e217ea] 9073 3074                 sub.w     116(a3,d3.w),d0
[00e217ee] 670c                      beq.s     $00E217FC
[00e217f0] 7207                      moveq.l   #7,d1
[00e217f2] 6100 fa42                 bsr       $00E21236
[00e217f6] 57c0                      seq       d0
[00e217f8] 5400                      addq.b    #2,d0
[00e217fa] 7214                      moveq.l   #20,d1
[00e217fc] 1740 0055                 move.b    d0,85(a3)
[00e21800] 3741 004e                 move.w    d1,78(a3)
[00e21804] 4cdf 787a                 movem.l   (a7)+,d1/d3-d6/a3-a6
[00e21808] 4e75                      rts
[00e2180a] 4a42                      tst.w     d2
[00e2180c] 6712                      beq.s     $00E21820
[00e2180e] d040                      add.w     d0,d0
[00e21810] c1c1                      muls.w    d1,d0
[00e21812] 81c2                      divs.w    d2,d0
[00e21814] 6b04                      bmi.s     $00E2181A
[00e21816] 5240                      addq.w    #1,d0
[00e21818] 6002                      bra.s     $00E2181C
[00e2181a] 5340                      subq.w    #1,d0
[00e2181c] e240                      asr.w     #1,d0
[00e2181e] 4e75                      rts
[00e21820] 7000                      moveq.l   #0,d0
[00e21822] 4e75                      rts
[00e21824] 206f 0004                 movea.l   4(a7),a0
[00e21828] 5468 0004                 addq.w    #2,4(a0)
[00e2182c] 5468 0006                 addq.w    #2,6(a0)
[00e21830] 7000                      moveq.l   #0,d0
[00e21832] 3f00                      move.w    d0,-(a7)
[00e21834] 2f08                      move.l    a0,-(a7)
[00e21836] 6100 0086                 bsr       $00E218BE
[00e2183a] 205f                      movea.l   (a7)+,a0
[00e2183c] 321f                      move.w    (a7)+,d1
[00e2183e] 7000                      moveq.l   #0,d0
[00e21840] 3401                      move.w    d1,d2
[00e21842] 48e7 470c                 movem.l   d1/d5-d7/a4-a5,-(a7)
[00e21846] 3e00                      move.w    d0,d7
[00e21848] 2a48                      movea.l   a0,a5
[00e2184a] 3c01                      move.w    d1,d6
[00e2184c] 3a02                      move.w    d2,d5
[00e2184e] 4a79 0000 913a            tst.w     $0000913A
[00e21854] 6662                      bne.s     $00E218B8
[00e21856] 224d                      movea.l   a5,a1
[00e21858] 41f9 0000 94a2            lea.l     $000094A2,a0
[00e2185e] 6100 fa62                 bsr       $00E212C2
[00e21862] 6100 3aec                 bsr       $00E25350
[00e21866] 4e71                      nop
[00e21868] 6100 f9b6                 bsr       $00E21220
[00e2186c] 2848                      movea.l   a0,a4
[00e2186e] 4a47                      tst.w     d7
[00e21870] 6608                      bne.s     $00E2187A
[00e21872] 226c 0032                 movea.l   50(a4),a1
[00e21876] 3e29 0030                 move.w    48(a1),d7
[00e2187a] 4a47                      tst.w     d7
[00e2187c] 6f34                      ble.s     $00E218B2
[00e2187e] 4a46                      tst.w     d6
[00e21880] 6608                      bne.s     $00E2188A
[00e21882] 226c 0036                 movea.l   54(a4),a1
[00e21886] 3c29 0030                 move.w    48(a1),d6
[00e2188a] bc45                      cmp.w     d5,d6
[00e2188c] 6710                      beq.s     $00E2189E
[00e2188e] 204d                      movea.l   a5,a0
[00e21890] 3006                      move.w    d6,d0
[00e21892] 6142                      bsr.s     $00E218D6
[00e21894] 72ff                      moveq.l   #-1,d1
[00e21896] 204d                      movea.l   a5,a0
[00e21898] 3006                      move.w    d6,d0
[00e2189a] 6100 00d2                 bsr       $00E2196E
[00e2189e] be46                      cmp.w     d6,d7
[00e218a0] 6710                      beq.s     $00E218B2
[00e218a2] 3006                      move.w    d6,d0
[00e218a4] 6100 f982                 bsr       $00E21228
[00e218a8] 2068 0036                 movea.l   54(a0),a0
[00e218ac] 3c28 0030                 move.w    48(a0),d6
[00e218b0] 60d8                      bra.s     $00E2188A
[00e218b2] 6100 3ab8                 bsr       $00E2536C
[00e218b6] 4e71                      nop
[00e218b8] 4cdf 30e2                 movem.l   (a7)+,d1/d5-d7/a4-a5
[00e218bc] 4e75                      rts
[00e218be] 226f 0004                 movea.l   4(a7),a1
[00e218c2] 3239 0000 94d8            move.w    $000094D8,d1
[00e218c8] 2079 0000 9156            movea.l   $00009156,a0
[00e218ce] 7000                      moveq.l   #0,d0
[00e218d0] 4267                      clr.w     -(a7)
[00e218d2] 600c                      bra.s     $00E218E0
[00e218d4] 91c8                      suba.l    a0,a0
[00e218d6] 7200                      moveq.l   #0,d1
[00e218d8] 2248                      movea.l   a0,a1
[00e218da] 91c8                      suba.l    a0,a0
[00e218dc] 3f3c ffff                 move.w    #$FFFF,-(a7)
[00e218e0] 74ff                      moveq.l   #-1,d2
[00e218e2] 6104                      bsr.s     $00E218E8
[00e218e4] 548f                      addq.l    #2,a7
[00e218e6] 4e75                      rts
[00e218e8] 4e56 fff0                 link      a6,#$FFF0
[00e218ec] 48e7 4f1c                 movem.l   d1/d4-d7/a3-a5,-(a7)
[00e218f0] 4a79 0000 913a            tst.w     $0000913A
[00e218f6] 666e                      bne.s     $00E21966
[00e218f8] 3e00                      move.w    d0,d7
[00e218fa] 6b6a                      bmi.s     $00E21966
[00e218fc] 3c01                      move.w    d1,d6
[00e218fe] 2a08                      move.l    a0,d5
[00e21900] 2849                      movea.l   a1,a4
[00e21902] 2642                      movea.l   d2,a3
[00e21904] 382e 0008                 move.w    8(a6),d4
[00e21908] 4bee fff0                 lea.l     -16(a6),a5
[00e2190c] 72fd                      moveq.l   #-3,d1
[00e2190e] 6100 f9c2                 bsr       $00E212D2
[00e21912] 6752                      beq.s     $00E21966
[00e21914] 49ee fff8                 lea.l     -8(a6),a4
[00e21918] 6048                      bra.s     $00E21962
[00e2191a] 224b                      movea.l   a3,a1
[00e2191c] 2659                      movea.l   (a1)+,a3
[00e2191e] 204c                      movea.l   a4,a0
[00e21920] 20d9                      move.l    (a1)+,(a0)+
[00e21922] 2091                      move.l    (a1),(a0)
[00e21924] 224c                      movea.l   a4,a1
[00e21926] 204d                      movea.l   a5,a0
[00e21928] 6100 f998                 bsr       $00E212C2
[00e2192c] 6734                      beq.s     $00E21962
[00e2192e] 2205                      move.l    d5,d1
[00e21930] 6606                      bne.s     $00E21938
[00e21932] 6100 fb50                 bsr       $00E21484
[00e21936] 2a08                      move.l    a0,d5
[00e21938] 4a44                      tst.w     d4
[00e2193a] 6712                      beq.s     $00E2194E
[00e2193c] 7800                      moveq.l   #0,d4
[00e2193e] 720b                      moveq.l   #11,d1
[00e21940] 6100 f8f4                 bsr       $00E21236
[00e21944] 6608                      bne.s     $00E2194E
[00e21946] 99cc                      suba.l    a4,a4
[00e21948] 70ff                      moveq.l   #-1,d0
[00e2194a] 2640                      movea.l   d0,a3
[00e2194c] 60be                      bra.s     $00E2190C
[00e2194e] 204c                      movea.l   a4,a0
[00e21950] 6100 fa4c                 bsr       $00E2139E
[00e21954] 3f3c 0008                 move.w    #$0008,-(a7)
[00e21958] 3f06                      move.w    d6,-(a7)
[00e2195a] 2f05                      move.l    d5,-(a7)
[00e2195c] 6100 2154                 bsr       $00E23AB2
[00e21960] 508f                      addq.l    #8,a7
[00e21962] 200b                      move.l    a3,d0
[00e21964] 66b4                      bne.s     $00E2191A
[00e21966] 4cdf 38f2                 movem.l   (a7)+,d1/d4-d7/a3-a5
[00e2196a] 4e5e                      unlk      a6
[00e2196c] 4e75                      rts
[00e2196e] 4e56 ffe8                 link      a6,#$FFE8
[00e21972] 48e7 011c                 movem.l   d7/a3-a5,-(a7)
[00e21976] 3e00                      move.w    d0,d7
[00e21978] 2641                      movea.l   d1,a3
[00e2197a] 4bee fff8                 lea.l     -8(a6),a5
[00e2197e] 2848                      movea.l   a0,a4
[00e21980] 7201                      moveq.l   #1,d1
[00e21982] 6100 f94e                 bsr       $00E212D2
[00e21986] 6724                      beq.s     $00E219AC
[00e21988] 601e                      bra.s     $00E219A8
[00e2198a] 204b                      movea.l   a3,a0
[00e2198c] 2658                      movea.l   (a0)+,a3
[00e2198e] 43ee fff0                 lea.l     -16(a6),a1
[00e21992] 22d8                      move.l    (a0)+,(a1)+
[00e21994] 22d8                      move.l    (a0)+,(a1)+
[00e21996] 5189                      subq.l    #8,a1
[00e21998] 204d                      movea.l   a5,a0
[00e2199a] 6100 f926                 bsr       $00E212C2
[00e2199e] 6708                      beq.s     $00E219A8
[00e219a0] 41ee fff0                 lea.l     -16(a6),a0
[00e219a4] 3007                      move.w    d7,d0
[00e219a6] 610c                      bsr.s     $00E219B4
[00e219a8] 200b                      move.l    a3,d0
[00e219aa] 66de                      bne.s     $00E2198A
[00e219ac] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[00e219b0] 4e5e                      unlk      a6
[00e219b2] 4e75                      rts
[00e219b4] 2f28 0004                 move.l    4(a0),-(a7)
[00e219b8] 2f10                      move.l    (a0),-(a7)
[00e219ba] 3f00                      move.w    d0,-(a7)
[00e219bc] 6100 f86a                 bsr       $00E21228
[00e219c0] 2068 0002                 movea.l   2(a0),a0
[00e219c4] 3f28 001c                 move.w    28(a0),-(a7)
[00e219c8] 3f3c 0014                 move.w    #$0014,-(a7)
[00e219cc] 2f3c 0000 94ee            move.l    #$000094EE,-(a7)
[00e219d2] 6100 f80a                 bsr       $00E211DE
[00e219d6] defc 0012                 adda.w    #$0012,a7
[00e219da] 4e75                      rts
[00e219dc] 6000 1406                 bra       $00E22DE4
[00e219e0] 6000 13e8                 bra       $00E22DCA
[00e219e4] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e219e8] 3e00                      move.w    d0,d7
[00e219ea] 2648                      movea.l   a0,a3
[00e219ec] 284a                      movea.l   a2,a4
[00e219ee] 4bf9 0000 94a2            lea.l     $000094A2,a5
[00e219f4] 7204                      moveq.l   #4,d1
[00e219f6] 6100 f84e                 bsr       $00E21246
[00e219fa] 6638                      bne.s     $00E21A34
[00e219fc] be79 0000 95d6            cmp.w     $000095D6,d7
[00e21a02] 662c                      bne.s     $00E21A30
[00e21a04] 2011                      move.l    (a1),d0
[00e21a06] 2229 0004                 move.l    4(a1),d1
[00e21a0a] 2415                      move.l    (a5),d2
[00e21a0c] d240                      add.w     d0,d1
[00e21a0e] d46d 0006                 add.w     6(a5),d2
[00e21a12] 906b 0002                 sub.w     2(a3),d0
[00e21a16] 6d04                      blt.s     $00E21A1C
[00e21a18] b242                      cmp.w     d2,d1
[00e21a1a] 6e14                      bgt.s     $00E21A30
[00e21a1c] 4840                      swap      d0
[00e21a1e] 4841                      swap      d1
[00e21a20] 4842                      swap      d2
[00e21a22] d240                      add.w     d0,d1
[00e21a24] d46d 0004                 add.w     4(a5),d2
[00e21a28] 9053                      sub.w     (a3),d0
[00e21a2a] 6d08                      blt.s     $00E21A34
[00e21a2c] b242                      cmp.w     d2,d1
[00e21a2e] 6f04                      ble.s     $00E21A34
[00e21a30] 7003                      moveq.l   #3,d0
[00e21a32] 6018                      bra.s     $00E21A4C
[00e21a34] 3007                      move.w    d7,d0
[00e21a36] 321b                      move.w    (a3)+,d1
[00e21a38] 9259                      sub.w     (a1)+,d1
[00e21a3a] 341b                      move.w    (a3)+,d2
[00e21a3c] 9459                      sub.w     (a1)+,d2
[00e21a3e] 204d                      movea.l   a5,a0
[00e21a40] 224c                      movea.l   a4,a1
[00e21a42] 95ca                      suba.l    a2,a2
[00e21a44] 4257                      clr.w     (a7)
[00e21a46] 6100 000a                 bsr.w     $00E21A52
[00e21a4a] 7000                      moveq.l   #0,d0
[00e21a4c] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e21a50] 4e75                      rts
[00e21a52] 4e56 ffe4                 link      a6,#$FFE4
[00e21a56] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e21a5a] 3e00                      move.w    d0,d7
[00e21a5c] 3c01                      move.w    d1,d6
[00e21a5e] 3a02                      move.w    d2,d5
[00e21a60] 2849                      movea.l   a1,a4
[00e21a62] 2d4a ffec                 move.l    a2,-20(a6)
[00e21a66] 47ee ffe4                 lea.l     -28(a6),a3
[00e21a6a] 224b                      movea.l   a3,a1
[00e21a6c] 22d8                      move.l    (a0)+,(a1)+
[00e21a6e] 22d8                      move.l    (a0)+,(a1)+
[00e21a70] 224b                      movea.l   a3,a1
[00e21a72] 41f9 0000 94a2            lea.l     $000094A2,a0
[00e21a78] 6100 f848                 bsr       $00E212C2
[00e21a7c] 3007                      move.w    d7,d0
[00e21a7e] 6100 f7a8                 bsr       $00E21228
[00e21a82] 2268 009e                 movea.l   158(a0),a1
[00e21a86] 41ee fff0                 lea.l     -16(a6),a0
[00e21a8a] 6100 15be                 bsr       $00E2304A
[00e21a8e] 4bee fff4                 lea.l     -12(a6),a5
[00e21a92] 4295                      clr.l     (a5)
[00e21a94] 602e                      bra.s     $00E21AC4
[00e21a96] 43ee fff8                 lea.l     -8(a6),a1
[00e21a9a] 41ec 0004                 lea.l     4(a4),a0
[00e21a9e] 22d8                      move.l    (a0)+,(a1)+
[00e21aa0] 22d8                      move.l    (a0)+,(a1)+
[00e21aa2] 5189                      subq.l    #8,a1
[00e21aa4] 204b                      movea.l   a3,a0
[00e21aa6] 6100 f81a                 bsr       $00E212C2
[00e21aaa] 6716                      beq.s     $00E21AC2
[00e21aac] 45ee fff8                 lea.l     -8(a6),a2
[00e21ab0] db6a 0002                 add.w     d5,2(a2)
[00e21ab4] dd52                      add.w     d6,(a2)
[00e21ab6] 224d                      movea.l   a5,a1
[00e21ab8] 41ee fff0                 lea.l     -16(a6),a0
[00e21abc] 6100 136e                 bsr       $00E22E2C
[00e21ac0] 2a40                      movea.l   d0,a5
[00e21ac2] 2854                      movea.l   (a4),a4
[00e21ac4] 200c                      move.l    a4,d0
[00e21ac6] 66ce                      bne.s     $00E21A96
[00e21ac8] 4bee fff4                 lea.l     -12(a6),a5
[00e21acc] 204d                      movea.l   a5,a0
[00e21ace] 6100 ff0c                 bsr       $00E219DC
[00e21ad2] 3205                      move.w    d5,d1
[00e21ad4] 3006                      move.w    d6,d0
[00e21ad6] 204d                      movea.l   a5,a0
[00e21ad8] 6100 151c                 bsr       $00E22FF6
[00e21adc] 204b                      movea.l   a3,a0
[00e21ade] 6100 f8be                 bsr       $00E2139E
[00e21ae2] 2855                      movea.l   (a5),a4
[00e21ae4] 6024                      bra.s     $00E21B0A
[00e21ae6] 2f2c 0008                 move.l    8(a4),-(a7)
[00e21aea] 202c 0004                 move.l    4(a4),d0
[00e21aee] 2f00                      move.l    d0,-(a7)
[00e21af0] 9045                      sub.w     d5,d0
[00e21af2] 4840                      swap      d0
[00e21af4] 9046                      sub.w     d6,d0
[00e21af6] 4840                      swap      d0
[00e21af8] 2f00                      move.l    d0,-(a7)
[00e21afa] 3f3c 0003                 move.w    #$0003,-(a7)
[00e21afe] 6100 2b7c                 bsr       $00E2467C
[00e21b02] 4e71                      nop
[00e21b04] defc 000e                 adda.w    #$000E,a7
[00e21b08] 2854                      movea.l   (a4),a4
[00e21b0a] 200c                      move.l    a4,d0
[00e21b0c] 66d8                      bne.s     $00E21AE6
[00e21b0e] 204d                      movea.l   a5,a0
[00e21b10] 6100 fece                 bsr       $00E219E0
[00e21b14] 4bee fff0                 lea.l     -16(a6),a5
[00e21b18] 204d                      movea.l   a5,a0
[00e21b1a] 6100 fec0                 bsr       $00E219DC
[00e21b1e] 4257                      clr.w     (a7)
[00e21b20] 4aae ffec                 tst.l     -20(a6)
[00e21b24] 660c                      bne.s     $00E21B32
[00e21b26] 5257                      addq.w    #1,(a7)
[00e21b28] 2215                      move.l    (a5),d1
[00e21b2a] 91c8                      suba.l    a0,a0
[00e21b2c] 3007                      move.w    d7,d0
[00e21b2e] 6100 fe3e                 bsr       $00E2196E
[00e21b32] 2415                      move.l    (a5),d2
[00e21b34] 224b                      movea.l   a3,a1
[00e21b36] 206e ffec                 movea.l   -20(a6),a0
[00e21b3a] 322e 0008                 move.w    8(a6),d1
[00e21b3e] 3007                      move.w    d7,d0
[00e21b40] 6100 fda6                 bsr       $00E218E8
[00e21b44] 204d                      movea.l   a5,a0
[00e21b46] 6100 fe98                 bsr       $00E219E0
[00e21b4a] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e21b4e] 4e5e                      unlk      a6
[00e21b50] 4e75                      rts
[00e21b52] 205f                      movea.l   (a7)+,a0
[00e21b54] 486e fff4                 pea.l     -12(a6)
[00e21b58] 2f08                      move.l    a0,-(a7)
[00e21b5a] 4a39 0000 af10            tst.b     $0000AF10
[00e21b60] 6618                      bne.s     $00E21B7A
[00e21b62] 3039 0000 95d6            move.w    $000095D6,d0
[00e21b68] 6100 f6be                 bsr       $00E21228
[00e21b6c] 4868 0018                 pea.l     24(a0)
[00e21b70] 2f28 0002                 move.l    2(a0),-(a7)
[00e21b74] 6100 9b04                 bsr       $00E1B67A
[00e21b78] 508f                      addq.l    #8,a7
[00e21b7a] 4e75                      rts
[00e21b7c] 2ad8                      move.l    (a0)+,(a5)+
[00e21b7e] 2ad8                      move.l    (a0)+,(a5)+
[00e21b80] 518d                      subq.l    #8,a5
[00e21b82] 3007                      move.w    d7,d0
[00e21b84] 6100 f6a2                 bsr       $00E21228
[00e21b88] 2648                      movea.l   a0,a3
[00e21b8a] 720b                      moveq.l   #11,d1
[00e21b8c] 6100 f6b8                 bsr       $00E21246
[00e21b90] 671c                      beq.s     $00E21BAE
[00e21b92] 204d                      movea.l   a5,a0
[00e21b94] 3039 0000 94a4            move.w    $000094A4,d0
[00e21b9a] 610a                      bsr.s     $00E21BA6
[00e21b9c] 202b 00a6                 move.l    166(a3),d0
[00e21ba0] 6100 0002                 bsr.w     $00E21BA4
[00e21ba4] 4840                      swap      d0
[00e21ba6] 5488                      addq.l    #2,a0
[00e21ba8] b050                      cmp.w     (a0),d0
[00e21baa] 6f02                      ble.s     $00E21BAE
[00e21bac] 3080                      move.w    d0,(a0)
[00e21bae] 4e75                      rts
[00e21bb0] 4e56 ffec                 link      a6,#$FFEC
[00e21bb4] 48e7 5f1c                 movem.l   d1/d3-d7/a3-a5,-(a7)
[00e21bb8] 204d                      movea.l   a5,a0
[00e21bba] 4bee fff0                 lea.l     -16(a6),a5
[00e21bbe] 61bc                      bsr.s     $00E21B7C
[00e21bc0] 49ee fff8                 lea.l     -8(a6),a4
[00e21bc4] 204c                      movea.l   a4,a0
[00e21bc6] 3007                      move.w    d7,d0
[00e21bc8] 6100 f69c                 bsr       $00E21266
[00e21bcc] 2015                      move.l    (a5),d0
[00e21bce] b094                      cmp.l     (a4),d0
[00e21bd0] 56c4                      sne       d4
[00e21bd2] 202d 0004                 move.l    4(a5),d0
[00e21bd6] b0ac 0004                 cmp.l     4(a4),d0
[00e21bda] 56c3                      sne       d3
[00e21bdc] 6606                      bne.s     $00E21BE4
[00e21bde] 4a04                      tst.b     d4
[00e21be0] 6700 0098                 beq       $00E21C7A
[00e21be4] 3007                      move.w    d7,d0
[00e21be6] 6100 f884                 bsr       $00E2146C
[00e21bea] 2155 0010                 move.l    (a5),16(a0)
[00e21bee] 6100 0fca                 bsr       $00E22BBA
[00e21bf2] 204d                      movea.l   a5,a0
[00e21bf4] 3007                      move.w    d7,d0
[00e21bf6] 6100 015e                 bsr       $00E21D56
[00e21bfa] 41eb 009e                 lea.l     158(a3),a0
[00e21bfe] 47ee ffec                 lea.l     -20(a6),a3
[00e21c02] 2690                      move.l    (a0),(a3)
[00e21c04] 4290                      clr.l     (a0)
[00e21c06] 6100 0182                 bsr       $00E21D8A
[00e21c0a] 6668                      bne.s     $00E21C74
[00e21c0c] 546d 0004                 addq.w    #2,4(a5)
[00e21c10] 546d 0006                 addq.w    #2,6(a5)
[00e21c14] 546c 0004                 addq.w    #2,4(a4)
[00e21c18] 546c 0006                 addq.w    #2,6(a4)
[00e21c1c] 4a03                      tst.b     d3
[00e21c1e] 6610                      bne.s     $00E21C30
[00e21c20] 2453                      movea.l   (a3),a2
[00e21c22] 224c                      movea.l   a4,a1
[00e21c24] 204d                      movea.l   a5,a0
[00e21c26] 3007                      move.w    d7,d0
[00e21c28] 6100 fdba                 bsr       $00E219E4
[00e21c2c] 3c00                      move.w    d0,d6
[00e21c2e] 6024                      bra.s     $00E21C54
[00e21c30] 7c03                      moveq.l   #3,d6
[00e21c32] 4a04                      tst.b     d4
[00e21c34] 661e                      bne.s     $00E21C54
[00e21c36] 302d 0004                 move.w    4(a5),d0
[00e21c3a] 906c 0004                 sub.w     4(a4),d0
[00e21c3e] 322d 0006                 move.w    6(a5),d1
[00e21c42] 926c 0006                 sub.w     6(a4),d1
[00e21c46] 6e08                      bgt.s     $00E21C50
[00e21c48] 4a40                      tst.w     d0
[00e21c4a] 6e04                      bgt.s     $00E21C50
[00e21c4c] 7c01                      moveq.l   #1,d6
[00e21c4e] 6004                      bra.s     $00E21C54
[00e21c50] 6100 0dd6                 bsr       $00E22A28
[00e21c54] e24e                      lsr.w     #1,d6
[00e21c56] 6408                      bcc.s     $00E21C60
[00e21c58] 204d                      movea.l   a5,a0
[00e21c5a] 3007                      move.w    d7,d0
[00e21c5c] 6100 fc78                 bsr       $00E218D6
[00e21c60] e24e                      lsr.w     #1,d6
[00e21c62] 6408                      bcc.s     $00E21C6C
[00e21c64] 204d                      movea.l   a5,a0
[00e21c66] 3007                      move.w    d7,d0
[00e21c68] 6100 00e2                 bsr       $00E21D4C
[00e21c6c] 204c                      movea.l   a4,a0
[00e21c6e] 3007                      move.w    d7,d0
[00e21c70] 6100 fbc0                 bsr       $00E21832
[00e21c74] 204b                      movea.l   a3,a0
[00e21c76] 6100 fd68                 bsr       $00E219E0
[00e21c7a] 4cdf 38fa                 movem.l   (a7)+,d1/d3-d7/a3-a5
[00e21c7e] 4e5e                      unlk      a6
[00e21c80] 4e75                      rts
[00e21c82] 4e56 fff0                 link      a6,#$FFF0
[00e21c86] 48e7 1f04                 movem.l   d3-d7/a5,-(a7)
[00e21c8a] 6100 f594                 bsr       $00E21220
[00e21c8e] 3801                      move.w    d1,d4
[00e21c90] 4441                      neg.w     d1
[00e21c92] e549                      lsl.w     #2,d1
[00e21c94] 2070 1032                 movea.l   50(a0,d1.w),a0
[00e21c98] be68 0030                 cmp.w     48(a0),d7
[00e21c9c] 6700 008e                 beq       $00E21D2C
[00e21ca0] 3007                      move.w    d7,d0
[00e21ca2] 6100 f67c                 bsr       $00E21320
[00e21ca6] 3600                      move.w    d0,d3
[00e21ca8] 3204                      move.w    d4,d1
[00e21caa] 3007                      move.w    d7,d0
[00e21cac] 6100 f6be                 bsr       $00E2136C
[00e21cb0] 4bee fff8                 lea.l     -8(a6),a5
[00e21cb4] 204d                      movea.l   a5,a0
[00e21cb6] 72fd                      moveq.l   #-3,d1
[00e21cb8] 3007                      move.w    d7,d0
[00e21cba] 6100 f5ac                 bsr       $00E21268
[00e21cbe] 3004                      move.w    d4,d0
[00e21cc0] 6100 00ca                 bsr       $00E21D8C
[00e21cc4] 6666                      bne.s     $00E21D2C
[00e21cc6] ba46                      cmp.w     d6,d5
[00e21cc8] 6754                      beq.s     $00E21D1E
[00e21cca] 3006                      move.w    d6,d0
[00e21ccc] 6100 fc06                 bsr       $00E218D4
[00e21cd0] 3005                      move.w    d5,d0
[00e21cd2] 6100 fc00                 bsr       $00E218D4
[00e21cd6] 3005                      move.w    d5,d0
[00e21cd8] 6100 f54e                 bsr       $00E21228
[00e21cdc] 0828 0002 0001            btst      #2,1(a0)
[00e21ce2] 6706                      beq.s     $00E21CEA
[00e21ce4] 6100 0058                 bsr.w     $00E21D3E
[00e21ce8] 6606                      bne.s     $00E21CF0
[00e21cea] 4a44                      tst.w     d4
[00e21cec] 663e                      bne.s     $00E21D2C
[00e21cee] 602c                      bra.s     $00E21D1C
[00e21cf0] 4a44                      tst.w     d4
[00e21cf2] 6720                      beq.s     $00E21D14
[00e21cf4] 41ee fff0                 lea.l     -16(a6),a0
[00e21cf8] 72fd                      moveq.l   #-3,d1
[00e21cfa] 3003                      move.w    d3,d0
[00e21cfc] 6100 f56a                 bsr       $00E21268
[00e21d00] 3005                      move.w    d5,d0
[00e21d02] 6100 0048                 bsr.w     $00E21D4C
[00e21d06] 3003                      move.w    d3,d0
[00e21d08] 6100 f616                 bsr       $00E21320
[00e21d0c] 3600                      move.w    d0,d3
[00e21d0e] ba43                      cmp.w     d3,d5
[00e21d10] 66e2                      bne.s     $00E21CF4
[00e21d12] 6018                      bra.s     $00E21D2C
[00e21d14] 204d                      movea.l   a5,a0
[00e21d16] 3005                      move.w    d5,d0
[00e21d18] 6100 0032                 bsr.w     $00E21D4C
[00e21d1c] 3605                      move.w    d5,d3
[00e21d1e] 3007                      move.w    d7,d0
[00e21d20] 6100 f5fe                 bsr       $00E21320
[00e21d24] 204d                      movea.l   a5,a0
[00e21d26] 3203                      move.w    d3,d1
[00e21d28] 6100 fb16                 bsr       $00E21840
[00e21d2c] 4cdf 20f8                 movem.l   (a7)+,d3-d7/a5
[00e21d30] 4e5e                      unlk      a6
[00e21d32] 4e75                      rts
[00e21d34] 3f00                      move.w    d0,-(a7)
[00e21d36] 6100 fb9c                 bsr       $00E218D4
[00e21d3a] 301f                      move.w    (a7)+,d0
[00e21d3c] 600c                      bra.s     $00E21D4A
[00e21d3e] 7203                      moveq.l   #3,d1
[00e21d40] 3005                      move.w    d5,d0
[00e21d42] 6100 f504                 bsr       $00E21248
[00e21d46] 660c                      bne.s     $00E21D54
[00e21d48] 3005                      move.w    d5,d0
[00e21d4a] 91c8                      suba.l    a0,a0
[00e21d4c] 72ff                      moveq.l   #-1,d1
[00e21d4e] 6100 fc1e                 bsr       $00E2196E
[00e21d52] 7000                      moveq.l   #0,d0
[00e21d54] 4e75                      rts
[00e21d56] 2248                      movea.l   a0,a1
[00e21d58] 6100 f4ce                 bsr       $00E21228
[00e21d5c] 45e8 0030                 lea.l     48(a0),a2
[00e21d60] 2162 0024                 move.l    -(a2),36(a0)
[00e21d64] 2162 0020                 move.l    -(a2),32(a0)
[00e21d68] 24d9                      move.l    (a1)+,(a2)+
[00e21d6a] 24d9                      move.l    (a1)+,(a2)+
[00e21d6c] 4868 0018                 pea.l     24(a0)
[00e21d70] 2f21                      move.l    -(a1),-(a7)
[00e21d72] 2f21                      move.l    -(a1),-(a7)
[00e21d74] 3f28 0006                 move.w    6(a0),-(a7)
[00e21d78] 3f3c 0001                 move.w    #$0001,-(a7)
[00e21d7c] 2068 0002                 movea.l   2(a0),a0
[00e21d80] 6100 08d6                 bsr       $00E22658
[00e21d84] defc 0010                 adda.w    #$0010,a7
[00e21d88] 4e75                      rts
[00e21d8a] 7000                      moveq.l   #0,d0
[00e21d8c] 6100 f492                 bsr       $00E21220
[00e21d90] 2068 0036                 movea.l   54(a0),a0
[00e21d94] 3a28 0030                 move.w    48(a0),d5
[00e21d98] 43f9 0000 95d6            lea.l     $000095D6,a1
[00e21d9e] 3c11                      move.w    (a1),d6
[00e21da0] 3285                      move.w    d5,(a1)
[00e21da2] 3f00                      move.w    d0,-(a7)
[00e21da4] 6702                      beq.s     $00E21DA8
[00e21da6] 3005                      move.w    d5,d0
[00e21da8] 6122                      bsr.s     $00E21DCC
[00e21daa] 6100 fdae                 bsr       $00E21B5A
[00e21dae] 301f                      move.w    (a7)+,d0
[00e21db0] 612e                      bsr.s     $00E21DE0
[00e21db2] 4a79 0000 913a            tst.w     $0000913A
[00e21db8] 4e75                      rts
[00e21dba] 23c0 0000 a6b2            move.l    d0,$0000A6B2
[00e21dc0] 4eb9 00e2 46ea            jsr       $00E246EA
[00e21dc6] 6100 f4c4                 bsr       $00E2128C
[00e21dca] 7000                      moveq.l   #0,d0
[00e21dcc] 6108                      bsr.s     $00E21DD6
[00e21dce] 6100 f550                 bsr       $00E21320
[00e21dd2] 6ef8                      bgt.s     $00E21DCC
[00e21dd4] 4e75                      rts
[00e21dd6] 3f00                      move.w    d0,-(a7)
[00e21dd8] 6100 1140                 bsr       $00E22F1A
[00e21ddc] 301f                      move.w    (a7)+,d0
[00e21dde] 4e75                      rts
[00e21de0] bc45                      cmp.w     d5,d6
[00e21de2] 673c                      beq.s     $00E21E20
[00e21de4] 4a00                      tst.b     d0
[00e21de6] 6706                      beq.s     $00E21DEE
[00e21de8] 721e                      moveq.l   #30,d1
[00e21dea] 3006                      move.w    d6,d0
[00e21dec] 6004                      bra.s     $00E21DF2
[00e21dee] 721f                      moveq.l   #31,d1
[00e21df0] 3005                      move.w    d5,d0
[00e21df2] 6f2c                      ble.s     $00E21E20
[00e21df4] 6100 f432                 bsr       $00E21228
[00e21df8] 2068 0002                 movea.l   2(a0),a0
[00e21dfc] 9efc 0010                 suba.w    #$0010,a7
[00e21e00] 224f                      movea.l   a7,a1
[00e21e02] 42a7                      clr.l     -(a7)
[00e21e04] 42a7                      clr.l     -(a7)
[00e21e06] 3f00                      move.w    d0,-(a7)
[00e21e08] 3f28 001c                 move.w    28(a0),-(a7)
[00e21e0c] 3f01                      move.w    d1,-(a7)
[00e21e0e] 2f09                      move.l    a1,-(a7)
[00e21e10] 7202                      moveq.l   #2,d1
[00e21e12] 6100 f434                 bsr       $00E21248
[00e21e16] 6704                      beq.s     $00E21E1C
[00e21e18] 6100 f3c4                 bsr       $00E211DE
[00e21e1c] defc 0022                 adda.w    #$0022,a7
[00e21e20] 4e75                      rts
[00e21e22] 48e7 041c                 movem.l   d5/a3-a5,-(a7)
[00e21e26] 3007                      move.w    d7,d0
[00e21e28] 6100 f3fe                 bsr       $00E21228
[00e21e2c] 2848                      movea.l   a0,a4
[00e21e2e] 266c 00a2                 movea.l   162(a4),a3
[00e21e32] ba7c 000b                 cmp.w     #$000B,d5
[00e21e36] 6622                      bne.s     $00E21E5A
[00e21e38] 266c 009e                 movea.l   158(a4),a3
[00e21e3c] 601c                      bra.s     $00E21E5A
[00e21e3e] 43eb 0004                 lea.l     4(a3),a1
[00e21e42] 204d                      movea.l   a5,a0
[00e21e44] 20d9                      move.l    (a1)+,(a0)+
[00e21e46] 2091                      move.l    (a1),(a0)
[00e21e48] 2653                      movea.l   (a3),a3
[00e21e4a] 294b 00a2                 move.l    a3,162(a4)
[00e21e4e] 224d                      movea.l   a5,a1
[00e21e50] 41ec 0018                 lea.l     24(a4),a0
[00e21e54] 6100 f46c                 bsr       $00E212C2
[00e21e58] 6608                      bne.s     $00E21E62
[00e21e5a] 200b                      move.l    a3,d0
[00e21e5c] 66e0                      bne.s     $00E21E3E
[00e21e5e] 429d                      clr.l     (a5)+
[00e21e60] 4295                      clr.l     (a5)
[00e21e62] 4cdf 3820                 movem.l   (a7)+,d5/a3-a5
[00e21e66] 4e75                      rts
[00e21e68] 4e56 0000                 link      a6,#$0000
[00e21e6c] 48e7 4304                 movem.l   d1/d6-d7/a5,-(a7)
[00e21e70] 7428                      moveq.l   #40,d2
[00e21e72] 7200                      moveq.l   #0,d1
[00e21e74] 6100 f3c0                 bsr       $00E21236
[00e21e78] 6602                      bne.s     $00E21E7C
[00e21e7a] 7408                      moveq.l   #8,d2
[00e21e7c] 7000                      moveq.l   #0,d0
[00e21e7e] 6100 f3a8                 bsr       $00E21228
[00e21e82] 2a48                      movea.l   a0,a5
[00e21e84] 082d 0000 0001            btst      #0,1(a5)
[00e21e8a] 670a                      beq.s     $00E21E96
[00e21e8c] 5240                      addq.w    #1,d0
[00e21e8e] b440                      cmp.w     d0,d2
[00e21e90] 6eec                      bgt.s     $00E21E7E
[00e21e92] 70ff                      moveq.l   #-1,d0
[00e21e94] 6024                      bra.s     $00E21EBA
[00e21e96] 3e00                      move.w    d0,d7
[00e21e98] 3c2e 0008                 move.w    8(a6),d6
[00e21e9c] 3006                      move.w    d6,d0
[00e21e9e] 204d                      movea.l   a5,a0
[00e21ea0] 6120                      bsr.s     $00E21EC2
[00e21ea2] 3007                      move.w    d7,d0
[00e21ea4] 206e 000a                 movea.l   10(a6),a0
[00e21ea8] 43f9 0000 95a4            lea.l     $000095A4,a1
[00e21eae] 95ca                      suba.l    a2,a2
[00e21eb0] 6100 f3ea                 bsr       $00E2129C
[00e21eb4] 6100 003e                 bsr.w     $00E21EF4
[00e21eb8] 3007                      move.w    d7,d0
[00e21eba] 4cdf 20c2                 movem.l   (a7)+,d1/d6-d7/a5
[00e21ebe] 4e5e                      unlk      a6
[00e21ec0] 4e75                      rts
[00e21ec2] 3140 0006                 move.w    d0,6(a0)
[00e21ec6] 2179 0000 e54a 0002       move.l    $0000E54A,2(a0)
[00e21ece] 30bc 0001                 move.w    #$0001,(a0)
[00e21ed2] 42a8 003a                 clr.l     58(a0)
[00e21ed6] 70ff                      moveq.l   #-1,d0
[00e21ed8] 2140 003e                 move.l    d0,62(a0)
[00e21edc] 41e8 0042                 lea.l     66(a0),a0
[00e21ee0] 2279 0000 aefc            movea.l   $0000AEFC,a1 /* winxvars.wx_mem */
[00e21ee6] 43e9 5558                 lea.l     21848(a1),a1
[00e21eea] 7016                      moveq.l   #22,d0
[00e21eec] 20d9                      move.l    (a1)+,(a0)+
[00e21eee] 51c8 fffc                 dbf       d0,$00E21EEC
[00e21ef2] 4e75                      rts
[00e21ef4] 48e7 1f0c                 movem.l   d3-d7/a4-a5,-(a7)
[00e21ef8] 2a39 0000 af08            move.l    $0000AF08,d5
[00e21efe] 4845                      swap      d5
[00e21f00] 7800                      moveq.l   #0,d4
[00e21f02] 49fa 0062                 lea.l     $00E21F66(pc),a4
[00e21f06] 7400                      moveq.l   #0,d2
[00e21f08] 6132                      bsr.s     $00E21F3C
[00e21f0a] 3607                      move.w    d7,d3
[00e21f0c] 206d 0002                 movea.l   2(a5),a0
[00e21f10] 6100 07b8                 bsr       $00E226CA
[00e21f14] 2800                      move.l    d0,d4
[00e21f16] 343c 0600                 move.w    #$0600,d2
[00e21f1a] 6120                      bsr.s     $00E21F3C
[00e21f1c] be43                      cmp.w     d3,d7
[00e21f1e] 6c02                      bge.s     $00E21F22
[00e21f20] 3e03                      move.w    d3,d7
[00e21f22] 4847                      swap      d7
[00e21f24] 4844                      swap      d4
[00e21f26] 4845                      swap      d5
[00e21f28] 343c 00c0                 move.w    #$00C0,d2
[00e21f2c] 610e                      bsr.s     $00E21F3C
[00e21f2e] 7407                      moveq.l   #7,d2
[00e21f30] 612a                      bsr.s     $00E21F5C
[00e21f32] 2b47 00a6                 move.l    d7,166(a5)
[00e21f36] 4cdf 30f8                 movem.l   (a7)+,d3-d7/a4-a5
[00e21f3a] 4e75                      rts
[00e21f3c] 3e3c 0002                 move.w    #$0002,d7
[00e21f40] 4a44                      tst.w     d4
[00e21f42] 6704                      beq.s     $00E21F48
[00e21f44] 528c                      addq.l    #1,a4
[00e21f46] 6118                      bsr.s     $00E21F60
[00e21f48] 101c                      move.b    (a4)+,d0
[00e21f4a] 6706                      beq.s     $00E21F52
[00e21f4c] 0106                      btst      d0,d6
[00e21f4e] 67f8                      beq.s     $00E21F48
[00e21f50] 60f4                      bra.s     $00E21F46
[00e21f52] 7209                      moveq.l   #9,d1
[00e21f54] 204d                      movea.l   a5,a0
[00e21f56] 6100 f2f2                 bsr       $00E2124A
[00e21f5a] 6708                      beq.s     $00E21F64
[00e21f5c] c446                      and.w     d6,d2
[00e21f5e] 6704                      beq.s     $00E21F64
[00e21f60] de45                      add.w     d5,d7
[00e21f62] 5347                      subq.w    #1,d7
[00e21f64] 4e75                      rts
[00e21f66] 0103                      btst      d0,d3
[00e21f68] 0302                      btst      d1,d2
[00e21f6a] 0005 090b                 ori.b     #$0B,d5
[00e21f6e] 0b0a 0005                 movep.w   5(a2),d5
[00e21f72] 0406 0808                 subi.b    #$08,d6
[00e21f76] 0700                      btst      d3,d0
[00e21f78] 7465                      moveq.l   #101,d2
[00e21f7a] 302f 0004                 move.w    4(a7),d0
[00e21f7e] 616a                      bsr.s     $00E21FEA
[00e21f80] 6756                      beq.s     $00E21FD8
[00e21f82] 08c0 0003                 bset      #3,d0
[00e21f86] 6650                      bne.s     $00E21FD8
[00e21f88] 3080                      move.w    d0,(a0)
[00e21f8a] 48e7 6714                 movem.l   d1-d2/d5-d7/a3/a5,-(a7)
[00e21f8e] 3e2f 0020                 move.w    32(a7),d7
[00e21f92] 6100 0638                 bsr       $00E225CC
[00e21f96] 3007                      move.w    d7,d0
[00e21f98] 6100 f3b2                 bsr       $00E2134C
[00e21f9c] 206f 0022                 movea.l   34(a7),a0
[00e21fa0] 2a4f                      movea.l   a7,a5
[00e21fa2] 6100 fbd8                 bsr       $00E21B7C
[00e21fa6] 204d                      movea.l   a5,a0
[00e21fa8] 3007                      move.w    d7,d0
[00e21faa] 6100 fdaa                 bsr       $00E21D56
[00e21fae] 70ff                      moveq.l   #-1,d0
[00e21fb0] 6100 fdda                 bsr       $00E21D8C
[00e21fb4] 660e                      bne.s     $00E21FC4
[00e21fb6] 3006                      move.w    d6,d0
[00e21fb8] 6f04                      ble.s     $00E21FBE
[00e21fba] 6100 f918                 bsr       $00E218D4
[00e21fbe] 3007                      move.w    d7,d0
[00e21fc0] 6100 fd72                 bsr       $00E21D34
[00e21fc4] 204d                      movea.l   a5,a0
[00e21fc6] 7202                      moveq.l   #2,d1
[00e21fc8] 3007                      move.w    d7,d0
[00e21fca] 6100 f2e8                 bsr       $00E212B4
[00e21fce] 6100 0600                 bsr       $00E225D0
[00e21fd2] 4cdf 28e6                 movem.l   (a7)+,d1-d2/d5-d7/a3/a5
[00e21fd6] 4e75                      rts
[00e21fd8] 6000 0aca                 bra       $00E22AA4
[00e21fdc] 7467                      moveq.l   #103,d2
[00e21fde] 302f 0004                 move.w    4(a7),d0
[00e21fe2] 6106                      bsr.s     $00E21FEA
[00e21fe4] 67f2                      beq.s     $00E21FD8
[00e21fe6] 4250                      clr.w     (a0)
[00e21fe8] 4e75                      rts
[00e21fea] 4a40                      tst.w     d0
[00e21fec] 6712                      beq.s     $00E22000
[00e21fee] b07c 0027                 cmp.w     #$0027,d0
[00e21ff2] 620c                      bhi.s     $00E22000
[00e21ff4] 6100 f232                 bsr       $00E21228
[00e21ff8] 3010                      move.w    (a0),d0
[00e21ffa] 0800 0000                 btst      #0,d0
[00e21ffe] 6602                      bne.s     $00E22002
[00e22000] 7000                      moveq.l   #0,d0
[00e22002] 4e75                      rts
[00e22004] 7466                      moveq.l   #102,d2
[00e22006] 302f 0004                 move.w    4(a7),d0
[00e2200a] 61de                      bsr.s     $00E21FEA
[00e2200c] 67ca                      beq.s     $00E21FD8
[00e2200e] 0880 0004                 bclr      #4,d0
[00e22012] 0880 0003                 bclr      #3,d0
[00e22016] 67c0                      beq.s     $00E21FD8
[00e22018] 3080                      move.w    d0,(a0)
[00e2201a] 4e56 fff8                 link      a6,#$FFF8
[00e2201e] 48e7 4f04                 movem.l   d1/d4-d7/a5,-(a7)
[00e22022] 3e2e 0008                 move.w    8(a6),d7
[00e22026] 4bee fff8                 lea.l     -8(a6),a5
[00e2202a] 6100 05a0                 bsr       $00E225CC
[00e2202e] 3007                      move.w    d7,d0
[00e22030] 6100 f2ee                 bsr       $00E21320
[00e22034] 3800                      move.w    d0,d4
[00e22036] 204d                      movea.l   a5,a0
[00e22038] 72fd                      moveq.l   #-3,d1
[00e2203a] 3007                      move.w    d7,d0
[00e2203c] 6100 f22a                 bsr       $00E21268
[00e22040] 6100 f2ec                 bsr       $00E2132E
[00e22044] 3007                      move.w    d7,d0
[00e22046] 6100 fd8e                 bsr       $00E21DD6
[00e2204a] 41f9 0000 95a4            lea.l     $000095A4,a0
[00e22050] 6100 fd04                 bsr       $00E21D56
[00e22054] 6100 fd34                 bsr       $00E21D8A
[00e22058] 6624                      bne.s     $00E2207E
[00e2205a] 4a44                      tst.w     d4
[00e2205c] 6618                      bne.s     $00E22076
[00e2205e] 3805                      move.w    d5,d4
[00e22060] 6f14                      ble.s     $00E22076
[00e22062] 3005                      move.w    d5,d0
[00e22064] 6100 f86e                 bsr       $00E218D4
[00e22068] 6100 fcd4                 bsr       $00E21D3E
[00e2206c] 6708                      beq.s     $00E22076
[00e2206e] 204d                      movea.l   a5,a0
[00e22070] 3005                      move.w    d5,d0
[00e22072] 6100 fcd8                 bsr       $00E21D4C
[00e22076] 204d                      movea.l   a5,a0
[00e22078] 3004                      move.w    d4,d0
[00e2207a] 6100 f7b6                 bsr       $00E21832
[00e2207e] 6100 0550                 bsr       $00E225D0
[00e22082] 4cdf 20f2                 movem.l   (a7)+,d1/d4-d7/a5
[00e22086] 4e5e                      unlk      a6
[00e22088] 4e75                      rts
[00e2208a] 6100 f19c                 bsr       $00E21228
[00e2208e] 4a40                      tst.w     d0
[00e22090] 6d2e                      blt.s     $00E220C0
[00e22092] 6726                      beq.s     $00E220BA
[00e22094] 2f08                      move.l    a0,-(a7)
[00e22096] 41e8 0018                 lea.l     24(a0),a0
[00e2209a] 3007                      move.w    d7,d0
[00e2209c] 4840                      swap      d0
[00e2209e] 3006                      move.w    d6,d0
[00e220a0] 6100 f270                 bsr       $00E21312
[00e220a4] 205f                      movea.l   (a7)+,a0
[00e220a6] 4a40                      tst.w     d0
[00e220a8] 6716                      beq.s     $00E220C0
[00e220aa] 0c6e 0002 0008            cmpi.w    #$0002,8(a6)
[00e220b0] 6708                      beq.s     $00E220BA
[00e220b2] 3010                      move.w    (a0),d0
[00e220b4] 0800 0008                 btst      #8,d0
[00e220b8] 6706                      beq.s     $00E220C0
[00e220ba] 2028 0002                 move.l    2(a0),d0
[00e220be] 4e75                      rts
[00e220c0] 2039 0000 e5c4            move.l    $0000E5C4,d0
[00e220c6] 4e75                      rts
[00e220c8] 43f9 00e3 e18a            lea.l     $00E3E18A,a1
[00e220ce] 1231 3066                 move.b    102(a1,d3.w),d1
[00e220d2] 67f2                      beq.s     $00E220C6
[00e220d4] 6b06                      bmi.s     $00E220DC
[00e220d6] 6100 f15e                 bsr       $00E21236
[00e220da] 67ea                      beq.s     $00E220C6
[00e220dc] 3203                      move.w    d3,d1
[00e220de] c3fc 0018                 muls.w    #$0018,d1
[00e220e2] 67e2                      beq.s     $00E220C6
[00e220e4] 41f4 180b                 lea.l     11(a4,d1.l),a0
[00e220e8] b410                      cmp.b     (a0),d2
[00e220ea] 67da                      beq.s     $00E220C6
[00e220ec] 1082                      move.b    d2,(a0)
[00e220ee] 93c9                      suba.l    a1,a1
[00e220f0] 3203                      move.w    d3,d1
[00e220f2] 204c                      movea.l   a4,a0
[00e220f4] 3007                      move.w    d7,d0
[00e220f6] 6000 f7e4                 bra       $00E218DC
[00e220fa] 45f9 00e3 e18a            lea.l     $00E3E18A,a2
[00e22100] 43ea 0018                 lea.l     24(a2),a1
[00e22104] 72ff                      moveq.l   #-1,d1
[00e22106] 7016                      moveq.l   #22,d0
[00e22108] 204c                      movea.l   a4,a0
[00e2210a] 20c1                      move.l    d1,(a0)+
[00e2210c] 30c1                      move.w    d1,(a0)+
[00e2210e] 4218                      clr.b     (a0)+
[00e22110] 10da                      move.b    (a2)+,(a0)+
[00e22112] 4298                      clr.l     (a0)+
[00e22114] 30d9                      move.w    (a1)+,(a0)+
[00e22116] 4258                      clr.w     (a0)+
[00e22118] 4298                      clr.l     (a0)+
[00e2211a] 4298                      clr.l     (a0)+
[00e2211c] 51c8 ffec                 dbf       d0,$00E2210A
[00e22120] 2409                      move.l    a1,d2
[00e22122] d2fc 001c                 adda.w    #$001C,a1
[00e22126] 600e                      bra.s     $00E22136
[00e22128] 2988 080c                 move.l    a0,12(a4,d0.l)
[00e2212c] 2442                      movea.l   d2,a2
[00e2212e] 720d                      moveq.l   #13,d1
[00e22130] 30da                      move.w    (a2)+,(a0)+
[00e22132] 51c9 fffc                 dbf       d1,$00E22130
[00e22136] 1019                      move.b    (a1)+,d0
[00e22138] 4880                      ext.w     d0
[00e2213a] c1fc 0018                 muls.w    #$0018,d0
[00e2213e] 6ce8                      bge.s     $00E22128
[00e22140] 206c 0084                 movea.l   132(a4),a0
[00e22144] 317c 0000 0010            move.w    #$0000,16(a0)
[00e2214a] 397c 0020 000a            move.w    #$0020,10(a4)
[00e22150] 4e75                      rts
[00e22152] 23ee 0004 0000 af0c       move.l    4(a6),$0000AF0C
[00e2215a] 41fa 0110                 lea.l     $00E2226C(pc),a0
[00e2215e] 2d48 0004                 move.l    a0,4(a6)
[00e22162] 41f9 0000 af08            lea.l     $0000AF08,a0
[00e22168] 30f9 0000 9504            move.w    $00009504,(a0)+
[00e2216e] 30f9 0000 92d4            move.w    $000092D4,(a0)+
[00e22174] 4e71                      nop
[00e22176] 4e71                      nop
[00e22178] 700b                      moveq.l   #11,d0
[00e2217a] 6100 f2a8                 bsr       $00E21424
[00e2217e] 2040                      movea.l   d0,a0
[00e22180] 6604                      bne.s     $00E22186
[00e22182] 41fa 0022                 lea.l     $00E221A6(pc),a0
[00e22186] 2f08                      move.l    a0,-(a7)
[00e22188] 3f3c 575a                 move.w    #$575A,-(a7)
[00e2218c] 4267                      clr.w     -(a7)
[00e2218e] 6100 02a4                 bsr       $00E22434
[00e22192] 508f                      addq.l    #8,a7
[00e22194] 2079 0000 aefc            movea.l   $0000AEFC,a0
[00e2219a] 4eb9 00e3 5dc0            jsr       $00E35DC0
[00e221a0] 6100 f258                 bsr       $00E213FA
[00e221a4] 600a                      bra.s     $00E221B0
[00e221a6] 01cc 0028                 movep.l   d0,40(a4)
[00e221aa] 03e8 07d0                 bset      d1,2000(a0)
[00e221ae] 4afc                      illegal
[00e221b0] 4e56 0000                 link      a6,#$0000
[00e221b4] 48e7 431c                 movem.l   d1/d6-d7/a3-a5,-(a7)
[00e221b8] 2a79 0000 aefc            movea.l   $0000AEFC,a5
[00e221be] 204d                      movea.l   a5,a0
[00e221c0] 303c 2d4f                 move.w    #$2D4F,d0
[00e221c4] 4218                      clr.b     (a0)+
[00e221c6] 51c8 fffc                 dbf       d0,$00E221C4
[00e221ca] 95ca                      suba.l    a2,a2
[00e221cc] 204d                      movea.l   a5,a0
[00e221ce] 7000                      moveq.l   #0,d0
[00e221d0] 3140 0030                 move.w    d0,48(a0)
[00e221d4] 43e8 00aa                 lea.l     170(a0),a1
[00e221d8] 7209                      moveq.l   #9,d1
[00e221da] 228a                      move.l    a2,(a1)
[00e221dc] 2449                      movea.l   a1,a2
[00e221de] d2fc 000c                 adda.w    #$000C,a1
[00e221e2] 51c9 fff6                 dbf       d1,$00E221DA
[00e221e6] d0fc 0122                 adda.w    #$0122,a0
[00e221ea] 5240                      addq.w    #1,d0
[00e221ec] b07c 0028                 cmp.w     #$0028,d0
[00e221f0] 66de                      bne.s     $00E221D0
[00e221f2] 23ca 0000 e552            move.l    a2,$0000E552
[00e221f8] 7000                      moveq.l   #0,d0
[00e221fa] 204d                      movea.l   a5,a0
[00e221fc] 6100 fcc4                 bsr       $00E21EC2
[00e22200] 6100 f08a                 bsr       $00E2128C
[00e22204] 4279 0000 95d6            clr.w     $000095D6
[00e2220a] 0055 0008                 ori.w     #$0008,(a5)
[00e2220e] 2b4d 0032                 move.l    a5,50(a5)
[00e22212] 2b4d 0036                 move.l    a5,54(a5)
[00e22216] 6100 fbb2                 bsr       $00E21DCA
[00e2221a] 47f9 0000 94a2            lea.l     $000094A2,a3
[00e22220] 4279 0000 94d8            clr.w     $000094D8
[00e22226] 2479 0000 8f4e            movea.l   $00008F4E,a2
[00e2222c] 23ca 0000 9156            move.l    a2,$00009156
[00e22232] 671c                      beq.s     $00E22250
[00e22234] 4aaa 0010                 tst.l     16(a2)
[00e22238] 6610                      bne.s     $00E2224A
[00e2223a] 2013                      move.l    (a3),d0
[00e2223c] 3540 0012                 move.w    d0,18(a2)
[00e22240] 4440                      neg.w     d0
[00e22242] 3540 002a                 move.w    d0,42(a2)
[00e22246] 3540 0042                 move.w    d0,66(a2)
[00e2224a] 356b 0006 0016            move.w    6(a3),22(a2)
[00e22250] 49ed 55b4                 lea.l     21940(a5),a4
[00e22254] 23cc 0000 9214            move.l    a4,$00009214
[00e2225a] 6100 fe9e                 bsr       $00E220FA
[00e2225e] 4279 0000 913a            clr.w     $0000913A
[00e22264] 4cdf 38c2                 movem.l   (a7)+,d1/d6-d7/a3-a5
[00e22268] 4e5e                      unlk      a6
[00e2226a] 4e75                      rts
[00e2226c] 7003                      moveq.l   #3,d0
[00e2226e] 6100 f1b4                 bsr       $00E21424
[00e22272] 2f39 0000 af0c            move.l    $0000AF0C,-(a7)
[00e22278] 4e75                      rts
[00e2227a] 3007                      move.w    d7,d0
[00e2227c] 6100 efaa                 bsr       $00E21228
[00e22280] 2268 0002                 movea.l   2(a0),a1
[00e22284] 3429 001c                 move.w    28(a1),d2
[00e22288] 70ff                      moveq.l   #-1,d0
[00e2228a] 78ff                      moveq.l   #-1,d4
[00e2228c] 3210                      move.w    (a0),d1
[00e2228e] e649                      lsr.w     #3,d1
[00e22290] c27c 0001                 and.w     #$0001,d1
[00e22294] 671c                      beq.s     $00E222B2
[00e22296] 2268 0032                 movea.l   50(a0),a1
[00e2229a] 3029 0030                 move.w    48(a1),d0
[00e2229e] 6602                      bne.s     $00E222A2
[00e222a0] 70ff                      moveq.l   #-1,d0
[00e222a2] 4a47                      tst.w     d7
[00e222a4] 6708                      beq.s     $00E222AE
[00e222a6] 2268 0036                 movea.l   54(a0),a1
[00e222aa] 3829 0030                 move.w    48(a1),d4
[00e222ae] 4840                      swap      d0
[00e222b0] 3004                      move.w    d4,d0
[00e222b2] 4e75                      rts
[00e222b4] 48e7 5d1c                 movem.l   d1/d3-d5/d7/a3-a5,-(a7)
[00e222b8] 3e2f 0024                 move.w    36(a7),d7
[00e222bc] 3a2f 0026                 move.w    38(a7),d5
[00e222c0] 2a6f 0028                 movea.l   40(a7),a5
[00e222c4] 4295                      clr.l     (a5)
[00e222c6] 42ad 0004                 clr.l     4(a5)
[00e222ca] 223c 0115 9bf0            move.l    #$01159BF0,d1
[00e222d0] 43fa 0026                 lea.l     $00E222F8(pc),a1
[00e222d4] 6000 0120                 bra       $00E223F6
[00e222d8] 0c0c 0c0c                 cmpi.b    #$0C,a4 ; apollo only
[00e222dc] 1a16                      move.b    (a6),d5
[00e222de] 181c                      move.b    (a4)+,d4
[00e222e0] 2c2a 3244                 move.l    12868(a2),d6
[00e222e4] 440c                      neg.b     a4 ; apollo only
[00e222e6] 5a28 264a                 addq.b    #5,9802(a0)
[00e222ea] 8a90                      or.l      (a0),d5
[00e222ec] 7e0c                      moveq.l   #12,d7
[00e222ee] 0c0c 686e                 cmpi.b    #$6E,a4 ; apollo only
[00e222f2] aaba d0da                 mac.w     d?.u,d?.u,0,acc3
[00e222f6] e400                      asr.b     #2,d0
[00e222f8] ba7c 0014                 cmp.w     #$0014,d5
[00e222fc] 6706                      beq.s     $00E22304
[00e222fe] 7468                      moveq.l   #104,d2
[00e22300] 6100 fcd6                 bsr       $00E21FD8
[00e22304] 7c00                      moveq.l   #0,d6
[00e22306] 3006                      move.w    d6,d0
[00e22308] 4cdf 38ba                 movem.l   (a7)+,d1/d3-d5/d7/a3-a5
[00e2230c] 4e75                      rts
[00e2230e] 5241                      addq.w    #1,d1
[00e22310] 5241                      addq.w    #1,d1
[00e22312] 5241                      addq.w    #1,d1
[00e22314] 204d                      movea.l   a5,a0
[00e22316] 3007                      move.w    d7,d0
[00e22318] 6100 ef4e                 bsr       $00E21268
[00e2231c] 60e8                      bra.s     $00E22306
[00e2231e] 5441                      addq.w    #2,d1
[00e22320] 5441                      addq.w    #2,d1
[00e22322] 5441                      addq.w    #2,d1
[00e22324] 3ab4 103a                 move.w    58(a4,d1.w),(a5)
[00e22328] 60dc                      bra.s     $00E22306
[00e2232a] 3e39 0000 95d6            move.w    $000095D6,d7
[00e22330] 6100 ff48                 bsr       $00E2227A
[00e22334] 3ac7                      move.w    d7,(a5)+
[00e22336] 3ac2                      move.w    d2,(a5)+
[00e22338] 3a80                      move.w    d0,(a5)
[00e2233a] 60ca                      bra.s     $00E22306
[00e2233c] 6100 fae4                 bsr       $00E21E22
[00e22340] 60c4                      bra.s     $00E22306
[00e22342] 486d 0004                 pea.l     4(a5)
[00e22346] 4855                      pea.l     (a5)
[00e22348] 6100 2d2a                 bsr       $00E25074
[00e2234c] 4e71                      nop
[00e2234e] 508f                      addq.l    #8,a7
[00e22350] 60ee                      bra.s     $00E22340
[00e22352] 2af9 0000 9156            move.l    $00009156,(a5)+
[00e22358] 3ab9 0000 94d8            move.w    $000094D8,(a5)
[00e2235e] 60e0                      bra.s     $00E22340
[00e22360] 1214                      move.b    (a4),d1
[00e22362] 3a81                      move.w    d1,(a5)
[00e22364] 60da                      bra.s     $00E22340
[00e22366] 7000                      moveq.l   #0,d0
[00e22368] 6100 efb6                 bsr       $00E21320
[00e2236c] 4a40                      tst.w     d0
[00e2236e] 6602                      bne.s     $00E22372
[00e22370] 70ff                      moveq.l   #-1,d0
[00e22372] 3a80                      move.w    d0,(a5)
[00e22374] 60ca                      bra.s     $00E22340
[00e22376] 6100 ff02                 bsr       $00E2227A
[00e2237a] 3ac2                      move.w    d2,(a5)+
[00e2237c] 3ac1                      move.w    d1,(a5)+
[00e2237e] 2a80                      move.l    d0,(a5)
[00e22380] 60be                      bra.s     $00E22340
[00e22382] 41ec 0042                 lea.l     66(a4),a0
[00e22386] 6004                      bra.s     $00E2238C
[00e22388] 41eb 5558                 lea.l     21848(a3),a0
[00e2238c] 226e 000e                 movea.l   14(a6),a1
[00e22390] 3029 0004                 move.w    4(a1),d0
[00e22394] 3ac0                      move.w    d0,(a5)+
[00e22396] d040                      add.w     d0,d0
[00e22398] d0c0                      adda.w    d0,a0
[00e2239a] 3ad0                      move.w    (a0),(a5)+
[00e2239c] 3aa8 002e                 move.w    46(a0),(a5)
[00e223a0] 609e                      bra.s     $00E22340
[00e223a2] 41f9 0000 aef6            lea.l     $0000AEF6,a0
[00e223a8] 2ad0                      move.l    (a0),(a5)+
[00e223aa] 2aa8 000e                 move.l    14(a0),(a5)
[00e223ae] 3c05                      move.w    d5,d6
[00e223b0] 608e                      bra.s     $00E22340
[00e223b2] 3afc 1fff                 move.w    #$1FFF,(a5)+
[00e223b6] 6100 ee7e                 bsr       $00E21236
[00e223ba] 3ac0                      move.w    d0,(a5)+
[00e223bc] 3afc 0fff                 move.w    #$0FFF,(a5)+
[00e223c0] 6100 ee7c                 bsr       $00E2123E
[00e223c4] 3ac0                      move.w    d0,(a5)+
[00e223c6] 60e6                      bra.s     $00E223AE
[00e223c8] 41eb 5550                 lea.l     21840(a3),a0
[00e223cc] 2ad8                      move.l    (a0)+,(a5)+
[00e223ce] 2ad8                      move.l    (a0)+,(a5)+
[00e223d0] 60dc                      bra.s     $00E223AE
[00e223d2] 3afc 007f                 move.w    #$007F,(a5)+
[00e223d6] 588d                      addq.l    #4,a5
[00e223d8] 525d                      addq.w    #1,(a5)+
[00e223da] 60c4                      bra.s     $00E223A0
[00e223dc] 7209                      moveq.l   #9,d1
[00e223de] 6100 ee56                 bsr       $00E21236
[00e223e2] 6704                      beq.s     $00E223E8
[00e223e4] 0055 0040                 ori.w     #$0040,(a5)
[00e223e8] 7202                      moveq.l   #2,d1
[00e223ea] 6100 ee52                 bsr       $00E2123E
[00e223ee] 6704                      beq.s     $00E223F4
[00e223f0] 0055 0006                 ori.w     #$0006,(a5)
[00e223f4] 60aa                      bra.s     $00E223A0
[00e223f6] 2679 0000 aefc            movea.l   $0000AEFC,a3
[00e223fc] 7019                      moveq.l   #25,d0
[00e223fe] ba40                      cmp.w     d0,d5
[00e22400] 621c                      bhi.s     $00E2241E
[00e22402] 0b01                      btst      d5,d1
[00e22404] 670a                      beq.s     $00E22410
[00e22406] 3007                      move.w    d7,d0
[00e22408] 6100 fbe4                 bsr       $00E21FEE
[00e2240c] 670e                      beq.s     $00E2241C
[00e2240e] 2848                      movea.l   a0,a4
[00e22410] 3005                      move.w    d5,d0
[00e22412] 7200                      moveq.l   #0,d1
[00e22414] 1031 00e0                 move.b    -32(a1,d0.w),d0
[00e22418] 4ef1 0000                 jmp       0(a1,d0.w)
[00e2241c] 4ed1                      jmp       (a1)
[00e2241e] 3005                      move.w    d5,d0
[00e22420] 907c 5758                 sub.w     #$5758,d0
[00e22424] b07c 0004                 cmp.w     #$0004,d0
[00e22428] 6206                      bhi.s     $00E22430
[00e2242a] d07c 001a                 add.w     #$001A,d0
[00e2242e] 60e2                      bra.s     $00E22412
[00e22430] 7000                      moveq.l   #0,d0
[00e22432] 60de                      bra.s     $00E22412
[00e22434] 4e56 fff8                 link      a6,#$FFF8
[00e22438] 48e7 4d1c                 movem.l   d1/d4-d5/d7/a3-a5,-(a7)
[00e2243c] 3e2e 0008                 move.w    8(a6),d7
[00e22440] 3a2e 000a                 move.w    10(a6),d5
[00e22444] 2a6e 000c                 movea.l   12(a6),a5
[00e22448] 6100 0182                 bsr       $00E225CC
[00e2244c] 223c 0305 872c            move.l    #$0305872C,d1
[00e22452] 43fa 0024                 lea.l     $00E22478(pc),a1
[00e22456] 609e                      bra.s     $00E223F6
[00e22458] 0606 1620                 addi.b    #$20,d6
[00e2245c] 0658 0606                 addi.w    #$0606,(a0)+
[00e22460] 766c                      moveq.l   #108,d3
[00e22462] 4406                      neg.b     d6
[00e22464] 06a0 b074 6a06            addi.l    #$B0746A06,-(a0)
[00e2246a] d4de                      adda.w    (a6)+,a2
[00e2246c] 0606 0606                 addi.b    #$06,d6
[00e22470] ce46                      and.w     d6,d7
[00e22472] 0606 e606                 addi.b    #$06,d6
[00e22476] 0600 7469                 addi.b    #$69,d0
[00e2247a] 6100 fb5c                 bsr       $00E21FD8
[00e2247e] 7c00                      moveq.l   #0,d6
[00e22480] 6100 014e                 bsr       $00E225D0
[00e22484] 4cdf 38b2                 movem.l   (a7)+,d1/d4-d5/d7/a3-a5
[00e22488] 4e5e                      unlk      a6
[00e2248a] 3006                      move.w    d6,d0
[00e2248c] 4e75                      rts
[00e2248e] 7803                      moveq.l   #3,d4
[00e22490] 2955 0008                 move.l    (a5),8(a4)
[00e22494] 7400                      moveq.l   #0,d2
[00e22496] 6008                      bra.s     $00E224A0
[00e22498] 7805                      moveq.l   #5,d4
[00e2249a] 2955 000c                 move.l    (a5),12(a4)
[00e2249e] 7404                      moveq.l   #4,d2
[00e224a0] 302c 0006                 move.w    6(a4),d0
[00e224a4] 0500                      btst      d2,d0
[00e224a6] 67d8                      beq.s     $00E22480
[00e224a8] 3014                      move.w    (a4),d0
[00e224aa] 0800 0003                 btst      #3,d0
[00e224ae] 67d0                      beq.s     $00E22480
[00e224b0] 91c8                      suba.l    a0,a0
[00e224b2] 3204                      move.w    d4,d1
[00e224b4] 3007                      move.w    d7,d0
[00e224b6] 6100 f420                 bsr       $00E218D8
[00e224ba] 60c4                      bra.s     $00E22480
[00e224bc] 72ff                      moveq.l   #-1,d1
[00e224be] 4a47                      tst.w     d7
[00e224c0] 67b6                      beq.s     $00E22478
[00e224c2] 082c 0003 0001            btst      #3,1(a4)
[00e224c8] 67ae                      beq.s     $00E22478
[00e224ca] 6100 f7b6                 bsr       $00E21C82
[00e224ce] 60b0                      bra.s     $00E22480
[00e224d0] 4a47                      tst.w     d7
[00e224d2] 67a4                      beq.s     $00E22478
[00e224d4] 082c 0003 0001            btst      #3,1(a4)
[00e224da] 679c                      beq.s     $00E22478
[00e224dc] 6100 f6d2                 bsr       $00E21BB0
[00e224e0] 609e                      bra.s     $00E22480
[00e224e2] 5841                      addq.w    #4,d1
[00e224e4] 5441                      addq.w    #2,d1
[00e224e6] 780c                      moveq.l   #12,d4
[00e224e8] 7408                      moveq.l   #8,d2
[00e224ea] 6006                      bra.s     $00E224F2
[00e224ec] 5841                      addq.w    #4,d1
[00e224ee] 7811                      moveq.l   #17,d4
[00e224f0] 740b                      moveq.l   #11,d2
[00e224f2] 41f4 103a                 lea.l     58(a4,d1.w),a0
[00e224f6] 3015                      move.w    (a5),d0
[00e224f8] b07c ffff                 cmp.w     #$FFFF,d0
[00e224fc] 6c02                      bge.s     $00E22500
[00e224fe] 70ff                      moveq.l   #-1,d0
[00e22500] b07c 03e8                 cmp.w     #$03E8,d0
[00e22504] 6f04                      ble.s     $00E2250A
[00e22506] 303c 03e8                 move.w    #$03E8,d0
[00e2250a] b050                      cmp.w     (a0),d0
[00e2250c] 67d2                      beq.s     $00E224E0
[00e2250e] 3080                      move.w    d0,(a0)
[00e22510] 6100 ee98                 bsr       $00E213AA
[00e22514] 668a                      bne.s     $00E224A0
[00e22516] 60c8                      bra.s     $00E224E0
[00e22518] 33c7 0000 913a            move.w    d7,$0000913A
[00e2251e] 66c0                      bne.s     $00E224E0
[00e22520] 2e8d                      move.l    a5,(a7)
[00e22522] 6100 f300                 bsr       $00E21824
[00e22526] 60b8                      bra.s     $00E224E0
[00e22528] 322d 0004                 move.w    4(a5),d1
[00e2252c] 2015                      move.l    (a5),d0
[00e2252e] 6608                      bne.s     $00E22538
[00e22530] 7200                      moveq.l   #0,d1
[00e22532] 2039 0000 8f4e            move.l    $00008F4E,d0
[00e22538] 23c0 0000 9156            move.l    d0,$00009156
[00e2253e] 33c1 0000 94d8            move.w    d1,$000094D8
[00e22544] 609a                      bra.s     $00E224E0
[00e22546] 3015                      move.w    (a5),d0
[00e22548] 1880                      move.b    d0,(a4)
[00e2254a] 60f8                      bra.s     $00E22544
[00e2254c] 41ec 0042                 lea.l     66(a4),a0
[00e22550] 6116                      bsr.s     $00E22568
[00e22552] 6000 ff54                 bra       $00E224A8
[00e22556] 41eb 5558                 lea.l     21848(a3),a0
[00e2255a] 610c                      bsr.s     $00E22568
[00e2255c] 60e6                      bra.s     $00E22544
[00e2255e] 41eb 5550                 lea.l     21840(a3),a0
[00e22562] 20dd                      move.l    (a5)+,(a0)+
[00e22564] 20dd                      move.l    (a5)+,(a0)+
[00e22566] 60dc                      bra.s     $00E22544
[00e22568] 381d                      move.w    (a5)+,d4
[00e2256a] 3004                      move.w    d4,d0
[00e2256c] d040                      add.w     d0,d0
[00e2256e] 7401                      moveq.l   #1,d2
[00e22570] 321d                      move.w    (a5)+,d1
[00e22572] 0c41 ffff                 cmpi.w    #$FFFF,d1
[00e22576] 6704                      beq.s     $00E2257C
[00e22578] 3181 0000                 move.w    d1,0(a0,d0.w)
[00e2257c] 3168 0014 0026            move.w    20(a0),38(a0)
[00e22582] 3168 0016 0028            move.w    22(a0),40(a0)
[00e22588] 3168 001e 002a            move.w    30(a0),42(a0)
[00e2258e] 3168 0020 002c            move.w    32(a0),44(a0)
[00e22594] d0fc 002e                 adda.w    #$002E,a0
[00e22598] 51ca ffd6                 dbf       d2,$00E22570
[00e2259c] 4e75                      rts
[00e2259e] 2f0d                      move.l    a5,-(a7)
[00e225a0] 6100 ec7e                 bsr       $00E21220
[00e225a4] 2a48                      movea.l   a0,a5
[00e225a6] 2a6d 0036                 movea.l   54(a5),a5
[00e225aa] 41ed 0028                 lea.l     40(a5),a0
[00e225ae] 202f 0008                 move.l    8(a7),d0
[00e225b2] 6100 ed5e                 bsr       $00E21312
[00e225b6] 4a40                      tst.w     d0
[00e225b8] 6706                      beq.s     $00E225C0
[00e225ba] 302d 0030                 move.w    48(a5),d0
[00e225be] 6008                      bra.s     $00E225C8
[00e225c0] 4a6d 0030                 tst.w     48(a5)
[00e225c4] 66e0                      bne.s     $00E225A6
[00e225c6] 70ff                      moveq.l   #-1,d0
[00e225c8] 2a5f                      movea.l   (a7)+,a5
[00e225ca] 4e75                      rts
[00e225cc] 7001                      moveq.l   #1,d0
[00e225ce] 6002                      bra.s     $00E225D2
[00e225d0] 7000                      moveq.l   #0,d0
[00e225d2] 6004                      bra.s     $00E225D8
[00e225d4] 302f 0004                 move.w    4(a7),d0
[00e225d8] 2079 0000 8f7e            movea.l   $00008F7E,a0
[00e225de] 2228 0002                 move.l    2(a0),d1
[00e225e2] b2b9 0000 e54a            cmp.l     $0000E54A,d1
[00e225e8] 57c1                      seq       d1
[00e225ea] 0880 0008                 bclr      #8,d0
[00e225ee] 670c                      beq.s     $00E225FC
[00e225f0] 4a50                      tst.w     (a0)
[00e225f2] 6708                      beq.s     $00E225FC
[00e225f4] 4a01                      tst.b     d1
[00e225f6] 6604                      bne.s     $00E225FC
[00e225f8] 4246                      clr.w     d6
[00e225fa] 6046                      bra.s     $00E22642
[00e225fc] 5540                      subq.w    #2,d0
[00e225fe] 6c30                      bge.s     $00E22630
[00e22600] 5440                      addq.w    #2,d0
[00e22602] 6720                      beq.s     $00E22624
[00e22604] 2f08                      move.l    a0,-(a7)
[00e22606] 6100 9c58                 bsr       $00E1C260
[00e2260a] 4e71                      nop
[00e2260c] 588f                      addq.l    #4,a7
[00e2260e] 4a40                      tst.w     d0
[00e22610] 6630                      bne.s     $00E22642
[00e22612] 2f39 0000 8f7e            move.l    $00008F7E,-(a7)
[00e22618] 3f3c 0004                 move.w    #$0004,-(a7)
[00e2261c] 6100 96f4                 bsr       $00E1BD12
[00e22620] 5c8f                      addq.l    #6,a7
[00e22622] 601e                      bra.s     $00E22642
[00e22624] 4a50                      tst.w     (a0)
[00e22626] 6f14                      ble.s     $00E2263C
[00e22628] 4a01                      tst.b     d1
[00e2262a] 6710                      beq.s     $00E2263C
[00e2262c] 6118                      bsr.s     $00E22646
[00e2262e] 6012                      bra.s     $00E22642
[00e22630] 3f00                      move.w    d0,-(a7)
[00e22632] 4eb9 00e1 b846            jsr       $00E1B846
[00e22638] 548f                      addq.l    #2,a7
[00e2263a] 6006                      bra.s     $00E22642
[00e2263c] 746b                      moveq.l   #107,d2
[00e2263e] 6100 f998                 bsr       $00E21FD8
[00e22642] 3006                      move.w    d6,d0
[00e22644] 4e75                      rts
[00e22646] 2f08                      move.l    a0,-(a7)
[00e22648] 6100 9c8c                 bsr       $00E1C2D6
[00e2264c] 4e71                      nop
[00e2264e] 588f                      addq.l    #4,a7
[00e22650] 4e75                      rts
[00e22652] 2079 0000 e54a            movea.l   $0000E54A,a0
[00e22658] 48e7 1f20                 movem.l   d3-d7/a2,-(a7)
[00e2265c] 3c2f 001e                 move.w    30(a7),d6
[00e22660] 7801                      moveq.l   #1,d4
[00e22662] 7e01                      moveq.l   #1,d7
[00e22664] 7601                      moveq.l   #1,d3
[00e22666] 7a01                      moveq.l   #1,d5
[00e22668] 3439 0000 af08            move.w    $0000AF08,d2
[00e2266e] 5342                      subq.w    #1,d2
[00e22670] 3239 0000 af0a            move.w    $0000AF0A,d1
[00e22676] 5341                      subq.w    #1,d1
[00e22678] 3006                      move.w    d6,d0
[00e2267a] c07c 0007                 and.w     #$0007,d0
[00e2267e] 6702                      beq.s     $00E22682
[00e22680] de41                      add.w     d1,d7
[00e22682] 0806 0004                 btst      #4,d6
[00e22686] 6702                      beq.s     $00E2268A
[00e22688] de41                      add.w     d1,d7
[00e2268a] 613e                      bsr.s     $00E226CA
[00e2268c] 4a40                      tst.w     d0
[00e2268e] 6702                      beq.s     $00E22692
[00e22690] d642                      add.w     d2,d3
[00e22692] 4840                      swap      d0
[00e22694] 4a40                      tst.w     d0
[00e22696] 6702                      beq.s     $00E2269A
[00e22698] da41                      add.w     d1,d5
[00e2269a] 4a6f 001c                 tst.w     28(a7)
[00e2269e] 6608                      bne.s     $00E226A8
[00e226a0] 4444                      neg.w     d4
[00e226a2] 4447                      neg.w     d7
[00e226a4] 4443                      neg.w     d3
[00e226a6] 4445                      neg.w     d5
[00e226a8] 41ef 0020                 lea.l     32(a7),a0
[00e226ac] 246f 0028                 movea.l   40(a7),a2
[00e226b0] 3498                      move.w    (a0)+,(a2)
[00e226b2] d95a                      add.w     d4,(a2)+
[00e226b4] 3498                      move.w    (a0)+,(a2)
[00e226b6] df5a                      add.w     d7,(a2)+
[00e226b8] 3498                      move.w    (a0)+,(a2)
[00e226ba] 9952                      sub.w     d4,(a2)
[00e226bc] 975a                      sub.w     d3,(a2)+
[00e226be] 3490                      move.w    (a0),(a2)
[00e226c0] 9f52                      sub.w     d7,(a2)
[00e226c2] 9b52                      sub.w     d5,(a2)
[00e226c4] 4cdf 04f8                 movem.l   (a7)+,d3-d7/a2
[00e226c8] 4e75                      rts
[00e226ca] 3f01                      move.w    d1,-(a7)
[00e226cc] 6100 eb80                 bsr       $00E2124E
[00e226d0] 3200                      move.w    d0,d1
[00e226d2] 3006                      move.w    d6,d0
[00e226d4] 4840                      swap      d0
[00e226d6] 3006                      move.w    d6,d0
[00e226d8] c0bc 0e00 01c0            and.l     #$0E0001C0,d0
[00e226de] 0806 0005                 btst      #5,d6
[00e226e2] 6710                      beq.s     $00E226F4
[00e226e4] 0801 0001                 btst      #1,d1
[00e226e8] 6604                      bne.s     $00E226EE
[00e226ea] 4680                      not.l     d0
[00e226ec] 6006                      bra.s     $00E226F4
[00e226ee] 4a80                      tst.l     d0
[00e226f0] 6602                      bne.s     $00E226F4
[00e226f2] 4640                      not.w     d0
[00e226f4] 321f                      move.w    (a7)+,d1
[00e226f6] 4e75                      rts
[00e226f8] 0c6e 0001 0008            cmpi.w    #$0001,8(a6)
[00e226fe] 670e                      beq.s     $00E2270E
[00e22700] 7201                      moveq.l   #1,d1
[00e22702] 6100 eb32                 bsr       $00E21236
[00e22706] 6708                      beq.s     $00E22710
[00e22708] 0c6e 0002 0008            cmpi.w    #$0002,8(a6)
[00e2270e] 4e75                      rts
[00e22710] 7001                      moveq.l   #1,d0
[00e22712] 4e75                      rts
[00e22714] 205f                      movea.l   (a7)+,a0
[00e22716] 7001                      moveq.l   #1,d0
[00e22718] e188                      lsl.l     #8,d0
[00e2271a] 1039 0000 af11            move.b    $0000AF11,d0
[00e22720] 5200                      addq.b    #1,d0
[00e22722] e188                      lsl.l     #8,d0
[00e22724] 2f00                      move.l    d0,-(a7)
[00e22726] 4ed0                      jmp       (a0)
[00e22728] 48e7 460c                 movem.l   d1/d5-d6/a4-a5,-(a7)
[00e2272c] 2a48                      movea.l   a0,a5
[00e2272e] 2849                      movea.l   a1,a4
[00e22730] 3ebc 0001                 move.w    #$0001,(a7)
[00e22734] 6100 2714                 bsr       $00E24E4A
[00e22738] 4e71                      nop
[00e2273a] 2a39 0000 8db0            move.l    $00008DB0,d5
[00e22740] 4845                      swap      d5
[00e22742] 9a6d 0004                 sub.w     4(a5),d5
[00e22746] 4845                      swap      d5
[00e22748] 9a6d 0006                 sub.w     6(a5),d5
[00e2274c] 2039 0000 8db0            move.l    $00008DB0,d0
[00e22752] 2200                      move.l    d0,d1
[00e22754] 7401                      moveq.l   #1,d2
[00e22756] 4840                      swap      d0
[00e22758] 4846                      swap      d6
[00e2275a] 4845                      swap      d5
[00e2275c] 9045                      sub.w     d5,d0
[00e2275e] b046                      cmp.w     d6,d0
[00e22760] 6c02                      bge.s     $00E22764
[00e22762] 3006                      move.w    d6,d0
[00e22764] 51ca fff0                 dbf       d2,$00E22756
[00e22768] 2b40 0004                 move.l    d0,4(a5)
[00e2276c] 2e81                      move.l    d1,(a7)
[00e2276e] 2f0c                      move.l    a4,-(a7)
[00e22770] 2f0d                      move.l    a5,-(a7)
[00e22772] 6100 2696                 bsr       $00E24E0A
[00e22776] 4e71                      nop
[00e22778] 508f                      addq.l    #8,a7
[00e2277a] 4a40                      tst.w     d0
[00e2277c] 66ce                      bne.s     $00E2274C
[00e2277e] 4cdf 3062                 movem.l   (a7)+,d1/d5-d6/a4-a5
[00e22782] 4e75                      rts
[00e22784] 6100 fe46                 bsr       $00E225CC
[00e22788] 6100 fa26                 bsr       $00E221B0
[00e2278c] 42b9 0000 9506            clr.l     $00009506
[00e22792] 600a                      bra.s     $00E2279E
[00e22794] 4279 0000 94c6            clr.w     $000094C6
[00e2279a] 6100 feaa                 bsr       $00E22646
[00e2279e] 2279 0000 e54a            movea.l   $0000E54A,a1
[00e227a4] 2079 0000 8f7e            movea.l   $00008F7E,a0
[00e227aa] 4a50                      tst.w     (a0)
[00e227ac] 6706                      beq.s     $00E227B4
[00e227ae] b3e8 0002                 cmpa.l    2(a0),a1
[00e227b2] 67e0                      beq.s     $00E22794
[00e227b4] 23c9 0000 950a            move.l    a1,$0000950A
[00e227ba] 7001                      moveq.l   #1,d0
[00e227bc] 4e75                      rts
[00e227be] 4eb9 00e2 62fa            jsr       $00E262FA
[00e227c4] 2a00                      move.l    d0,d5
[00e227c6] dab9 0000 8bd2            add.l     $00008BD2,d5
[00e227cc] 4e75                      rts
[00e227ce] 70ff                      moveq.l   #-1,d0
[00e227d0] 4eb9 00e1 b4d6            jsr       $00E1B4D6
[00e227d6] 4a40                      tst.w     d0
[00e227d8] 4e75                      rts
[00e227da] 7208                      moveq.l   #8,d1
[00e227dc] 6100 ea68                 bsr       $00E21246
[00e227e0] 6606                      bne.s     $00E227E8
[00e227e2] 377c 01f4 0006            move.w    #$01F4,6(a3)
[00e227e8] 4e75                      rts
[00e227ea] 6100 0124                 bsr       $00E22910
[00e227ee] ffec                      dc.w      $FFEC ; illegal
[00e227f0] 7800                      moveq.l   #0,d4
[00e227f2] 4e56 ffee                 link      a6,#$FFEE
[00e227f6] 48e7 5e1c                 movem.l   d1/d3-d6/a3-a5,-(a7)
[00e227fa] b67c 0013                 cmp.w     #$0013,d3
[00e227fe] 6c20                      bge.s     $00E22820
[00e22800] 7400                      moveq.l   #0,d2
[00e22802] 6100 f8c4                 bsr       $00E220C8
[00e22806] b67c 000f                 cmp.w     #$000F,d3
[00e2280a] 6d02                      blt.s     $00E2280E
[00e2280c] 5743                      subq.w    #3,d3
[00e2280e] d67c 0009                 add.w     #$0009,d3
[00e22812] 7208                      moveq.l   #8,d1
[00e22814] 6100 ea30                 bsr       $00E21246
[00e22818] 6606                      bne.s     $00E22820
[00e2281a] 5243                      addq.w    #1,d3
[00e2281c] c67c fffe                 and.w     #$FFFE,d3
[00e22820] 7401                      moveq.l   #1,d2
[00e22822] 6100 f8a4                 bsr       $00E220C8
[00e22826] 43ee fff0                 lea.l     -16(a6),a1
[00e2282a] 3003                      move.w    d3,d0
[00e2282c] 204c                      movea.l   a4,a0
[00e2282e] 6100 eb5e                 bsr       $00E2138E
[00e22832] 6100 03f4                 bsr       $00E22C28
[00e22836] 51ee ffee                 sf        -18(a6)
[00e2283a] 42ae fffc                 clr.l     -4(a6)
[00e2283e] 7a00                      moveq.l   #0,d5
[00e22840] 2639 0000 8db0            move.l    $00008DB0,d3
[00e22846] 2839 0000 af08            move.l    $0000AF08,d4
[00e2284c] 41ee fff0                 lea.l     -16(a6),a0
[00e22850] 7c02                      moveq.l   #2,d6
[00e22852] 7206                      moveq.l   #6,d1
[00e22854] 7402                      moveq.l   #2,d2
[00e22856] 4843                      swap      d3
[00e22858] 4844                      swap      d4
[00e2285a] 3003                      move.w    d3,d0
[00e2285c] 9050                      sub.w     (a0),d0
[00e2285e] 6d0a                      blt.s     $00E2286A
[00e22860] 9068 0004                 sub.w     4(a0),d0
[00e22864] 6f1c                      ble.s     $00E22882
[00e22866] 5281                      addq.l    #1,d1
[00e22868] 4440                      neg.w     d0
[00e2286a] 48c0                      ext.l     d0
[00e2286c] 81c4                      divs.w    d4,d0
[00e2286e] 5340                      subq.w    #1,d0
[00e22870] b268 000c                 cmp.w     12(a0),d1
[00e22874] 6702                      beq.s     $00E22878
[00e22876] 7c00                      moveq.l   #0,d6
[00e22878] 3141 0008                 move.w    d1,8(a0)
[00e2287c] 4841                      swap      d1
[00e2287e] 3200                      move.w    d0,d1
[00e22880] 6006                      bra.s     $00E22888
[00e22882] 7200                      moveq.l   #0,d1
[00e22884] 4268 0008                 clr.w     8(a0)
[00e22888] c342                      exg       d1,d2
[00e2288a] 5488                      addq.l    #2,a0
[00e2288c] 4244                      clr.w     d4
[00e2288e] 4a84                      tst.l     d4
[00e22890] 66c4                      bne.s     $00E22856
[00e22892] 4a81                      tst.l     d1
[00e22894] 660a                      bne.s     $00E228A0
[00e22896] 4a82                      tst.l     d2
[00e22898] 6606                      bne.s     $00E228A0
[00e2289a] 42ae fffc                 clr.l     -4(a6)
[00e2289e] 6034                      bra.s     $00E228D4
[00e228a0] 36bc 0018                 move.w    #$0018,(a3)
[00e228a4] 4a81                      tst.l     d1
[00e228a6] 670a                      beq.s     $00E228B2
[00e228a8] 4a82                      tst.l     d2
[00e228aa] 6708                      beq.s     $00E228B4
[00e228ac] 4a2e ffee                 tst.b     -18(a6)
[00e228b0] 6702                      beq.s     $00E228B4
[00e228b2] c342                      exg       d1,d2
[00e228b4] 2741 0006                 move.l    d1,6(a3)
[00e228b8] 2742 000a                 move.l    d2,10(a3)
[00e228bc] 7001                      moveq.l   #1,d0
[00e228be] 6100 ff10                 bsr       $00E227D0
[00e228c2] 6710                      beq.s     $00E228D4
[00e228c4] 462e ffee                 not.b     -18(a6)
[00e228c8] 2d6e fff8 fffc            move.l    -8(a6),-4(a6)
[00e228ce] 2006                      move.l    d6,d0
[00e228d0] 6100 feec                 bsr       $00E227BE
[00e228d4] 6100 039a                 bsr       $00E22C70
[00e228d8] 670c                      beq.s     $00E228E6
[00e228da] bab9 0000 8bd2            cmp.l     $00008BD2,d5
[00e228e0] 64f2                      bcc.s     $00E228D4
[00e228e2] 6000 ff5c                 bra       $00E22840
[00e228e6] 6100 0346                 bsr       $00E22C2E
[00e228ea] 4cdf 387a                 movem.l   (a7)+,d1/d3-d6/a3-a5
[00e228ee] 4e5e                      unlk      a6
[00e228f0] 4e75                      rts
[00e228f2] 74ff                      moveq.l   #-1,d2
[00e228f4] 6006                      bra.s     $00E228FC

[00e228f6] 0805 0002                 btst      #2,d5
[00e228fa] 57c2                      seq       d2
[00e228fc] 245f                      movea.l   (a7)+,a2
[00e228fe] b42e ffe8                 cmp.b     -24(a6),d2
[00e22902] 6704                      beq.s     $00E22908
[00e22904] 5497                      addq.l    #2,(a7)
[00e22906] 4e75                      rts
[00e22908] 2257                      movea.l   (a7),a1
[00e2290a] d2d1                      adda.w    (a1),a1
[00e2290c] 2e89                      move.l    a1,(a7)
[00e2290e] 4ed2                      jmp       (a2)
[00e22910] 61e0                      bsr.s     $00E228F2
[00e22912] 7200                      moveq.l   #0,d1
[00e22914] 183b 3008                 move.b    $00E2291E(pc,d3.w),d4
[00e22918] 6c06                      bge.s     $00E22920
[00e2291a] 4404                      neg.b     d4
[00e2291c] 323c 03e8                 move.w    #$03E8,d1
[00e22920] 4884                      ext.w     d4
[00e22922] 3741 0006                 move.w    d1,6(a3)
[00e22926] 4e75                      rts
[00e22928] 1ae6                      move.b    -(a6),(a5)+
[00e2292a] 0000                      dc.w      $0000
[00e2292c] 0019 e700                 ori.b     #$00,(a1)+
[00e22930] 00e6                      dc.w      $00E6 ; illegal
[00e22932] 1ae7                      move.b    -(a7),(a5)+
[00e22934] 1900                      move.b    d0,-(a4)
[00e22936] 7000                      moveq.l   #0,d0
[00e22938] 6002                      bra.s     $00E2293C
[00e2293a] 7001                      moveq.l   #1,d0
[00e2293c] 61b4                      bsr.s     $00E228F2
[00e2293e] 9640                      sub.w     d0,d3
[00e22940] 4eb9 00e1 b4aa            jsr       $00E1B4AA
[00e22946] 5243                      addq.w    #1,d3
[00e22948] 41ee fff8                 lea.l     -8(a6),a0
[00e2294c] 611a                      bsr.s     $00E22968
[00e2294e] 4841                      swap      d1
[00e22950] e249                      lsr.w     #1,d1
[00e22952] d350                      add.w     d1,(a0)
[00e22954] 4841                      swap      d1
[00e22956] e249                      lsr.w     #1,d1
[00e22958] d368 0002                 add.w     d1,2(a0)
[00e2295c] 6138                      bsr.s     $00E22996
[00e2295e] 3801                      move.w    d1,d4
[00e22960] 3740 0006                 move.w    d0,6(a3)
[00e22964] c9c5                      muls.w    d5,d4
[00e22966] 4e75                      rts
[00e22968] 2f08                      move.l    a0,-(a7)
[00e2296a] 2248                      movea.l   a0,a1
[00e2296c] 3003                      move.w    d3,d0
[00e2296e] 5340                      subq.w    #1,d0
[00e22970] 204c                      movea.l   a4,a0
[00e22972] 6100 ea1a                 bsr       $00E2138E
[00e22976] 518f                      subq.l    #8,a7
[00e22978] 224f                      movea.l   a7,a1
[00e2297a] 3003                      move.w    d3,d0
[00e2297c] 204c                      movea.l   a4,a0
[00e2297e] 6100 ea0e                 bsr       $00E2138E
[00e22982] 201f                      move.l    (a7)+,d0
[00e22984] 221f                      move.l    (a7)+,d1
[00e22986] 205f                      movea.l   (a7)+,a0
[00e22988] 4841                      swap      d1
[00e2298a] 9368 0004                 sub.w     d1,4(a0)
[00e2298e] 4841                      swap      d1
[00e22990] 9368 0006                 sub.w     d1,6(a0)
[00e22994] 4e75                      rts
[00e22996] 2039 0000 8db0            move.l    $00008DB0,d0
[00e2299c] b67c 0012                 cmp.w     #$0012,d3
[00e229a0] 6606                      bne.s     $00E229A8
[00e229a2] 7219                      moveq.l   #25,d1
[00e229a4] 4840                      swap      d0
[00e229a6] 6004                      bra.s     $00E229AC
[00e229a8] 721a                      moveq.l   #26,d1
[00e229aa] 5488                      addq.l    #2,a0
[00e229ac] 3f01                      move.w    d1,-(a7)
[00e229ae] 3428 0004                 move.w    4(a0),d2
[00e229b2] 9050                      sub.w     (a0),d0
[00e229b4] 6c02                      bge.s     $00E229B8
[00e229b6] 7000                      moveq.l   #0,d0
[00e229b8] b042                      cmp.w     d2,d0
[00e229ba] 6f02                      ble.s     $00E229BE
[00e229bc] 3002                      move.w    d2,d0
[00e229be] 323c 03e8                 move.w    #$03E8,d1
[00e229c2] 6100 ee46                 bsr       $00E2180A
[00e229c6] 321f                      move.w    (a7)+,d1
[00e229c8] 4e75                      rts

[00e229ca] 6100 ff2a                 bsr       $00E228F6
[00e229ce] 48e7 7e1c                 movem.l   d1-d6/a3-a5,-(a7)
[00e229d2] 204f                      movea.l   a7,a0
[00e229d4] 6192                      bsr.s     $00E22968
[00e229d6] 2439 0000 8db0            move.l    $00008DB0,d2
[00e229dc] 7a01                      moveq.l   #1,d5
[00e229de] 4840                      swap      d0
[00e229e0] 4841                      swap      d1
[00e229e2] 4842                      swap      d2
[00e229e4] 9440                      sub.w     d0,d2
[00e229e6] 6c02                      bge.s     $00E229EA
[00e229e8] 4242                      clr.w     d2
[00e229ea] b441                      cmp.w     d1,d2
[00e229ec] 6f02                      ble.s     $00E229F0
[00e229ee] 3401                      move.w    d1,d2
[00e229f0] d558                      add.w     d2,(a0)+
[00e229f2] 51cd ffea                 dbf       d5,$00E229DE
[00e229f6] 204f                      movea.l   a7,a0
[00e229f8] 619c                      bsr.s     $00E22996
[00e229fa] 3c00                      move.w    d0,d6
[00e229fc] 6100 022a                 bsr       $00E22C28
[00e22a00] 204f                      movea.l   a7,a0
[00e22a02] 6192                      bsr.s     $00E22996
[00e22a04] bc40                      cmp.w     d0,d6
[00e22a06] 6710                      beq.s     $00E22A18
[00e22a08] 3681                      move.w    d1,(a3)
[00e22a0a] 3740 0006                 move.w    d0,6(a3)
[00e22a0e] 6100 fdbe                 bsr       $00E227CE
[00e22a12] 6704                      beq.s     $00E22A18
[00e22a14] 3c2b 0006                 move.w    6(a3),d6
[00e22a18] 6100 0256                 bsr       $00E22C70
[00e22a1c] 66e2                      bne.s     $00E22A00
[00e22a1e] 6100 020e                 bsr       $00E22C2E
[00e22a22] 4cdf 387e                 movem.l   (a7)+,d1-d6/a3-a5
[00e22a26] 4e75                      rts
[00e22a28] 4e56 fff8                 link      a6,#$FFF8
[00e22a2c] 48e7 1810                 movem.l   d3-d4/a3,-(a7)
[00e22a30] 3600                      move.w    d0,d3
[00e22a32] 3801                      move.w    d1,d4
[00e22a34] 3007                      move.w    d7,d0
[00e22a36] 6100 e7f0                 bsr       $00E21228
[00e22a3a] 47e8 0018                 lea.l     24(a0),a3
[00e22a3e] 7205                      moveq.l   #5,d1
[00e22a40] 6100 e808                 bsr       $00E2124A
[00e22a44] 6756                      beq.s     $00E22A9C
[00e22a46] 4a43                      tst.w     d3
[00e22a48] 6f2c                      ble.s     $00E22A76
[00e22a4a] 41ee fff8                 lea.l     -8(a6),a0
[00e22a4e] 3013                      move.w    (a3),d0
[00e22a50] d06b 0004                 add.w     4(a3),d0
[00e22a54] 9043                      sub.w     d3,d0
[00e22a56] 3080                      move.w    d0,(a0)
[00e22a58] 316b 0002 0002            move.w    2(a3),2(a0)
[00e22a5e] 3143 0004                 move.w    d3,4(a0)
[00e22a62] 316b 0006 0006            move.w    6(a3),6(a0)
[00e22a68] 4a44                      tst.w     d4
[00e22a6a] 6f04                      ble.s     $00E22A70
[00e22a6c] 9968 0006                 sub.w     d4,6(a0)
[00e22a70] 3007                      move.w    d7,d0
[00e22a72] 6100 f2d8                 bsr       $00E21D4C
[00e22a76] 4a44                      tst.w     d4
[00e22a78] 6f20                      ble.s     $00E22A9A
[00e22a7a] 41ee fff8                 lea.l     -8(a6),a0
[00e22a7e] 2093                      move.l    (a3),(a0)
[00e22a80] 302b 0006                 move.w    6(a3),d0
[00e22a84] 9044                      sub.w     d4,d0
[00e22a86] d168 0002                 add.w     d0,2(a0)
[00e22a8a] 316b 0004 0004            move.w    4(a3),4(a0)
[00e22a90] 3144 0006                 move.w    d4,6(a0)
[00e22a94] 3007                      move.w    d7,d0
[00e22a96] 6100 f2b4                 bsr       $00E21D4C
[00e22a9a] 7c01                      moveq.l   #1,d6
[00e22a9c] 4cdf 0818                 movem.l   (a7)+,d3-d4/a3
[00e22aa0] 4e5e                      unlk      a6
[00e22aa2] 4e75                      rts
[00e22aa4] 7207                      moveq.l   #7,d1
[00e22aa6] 6100 e796                 bsr       $00E2123E
[00e22aaa] 673a                      beq.s     $00E22AE6
[00e22aac] 2079 0000 e54a            movea.l   $0000E54A,a0
[00e22ab2] 4267                      clr.w     -(a7)
[00e22ab4] 2f28 0010                 move.l    16(a0),-(a7)
[00e22ab8] 2f28 000c                 move.l    12(a0),-(a7)
[00e22abc] 3f02                      move.w    d2,-(a7)
[00e22abe] 486f 0002                 pea.l     2(a7)
[00e22ac2] 2f0f                      move.l    a7,-(a7)
[00e22ac4] 41f9 00e3 e18a            lea.l     $00E3E18A,a0
[00e22aca] 4868 00c8                 pea.l     200(a0)
[00e22ace] 4879 0000 ccfe            pea.l     $0000CCFE
[00e22ad4] 6100 3582                 bsr       $00E26058
[00e22ad8] 4e71                      nop
[00e22ada] 3f3c 0001                 move.w    #$0001,-(a7)
[00e22ade] 6100 9be8                 bsr       $00E1C6C8
[00e22ae2] defc 001e                 adda.w    #$001E,a7
[00e22ae6] 7c00                      moveq.l   #0,d6
[00e22ae8] 4e75                      rts
[00e22aea] 48e7 5004                 movem.l   d1/d3/a5,-(a7)
[00e22aee] 2a4c                      movea.l   a4,a5
[00e22af0] 7206                      moveq.l   #6,d1
[00e22af2] 302c 0006                 move.w    6(a4),d0
[00e22af6] 6100 e750                 bsr       $00E21248
[00e22afa] 673e                      beq.s     $00E22B3A
[00e22afc] 6010                      bra.s     $00E22B0E
[00e22afe] 0c55 0014                 cmpi.w    #$0014,(a5)
[00e22b02] 660a                      bne.s     $00E22B0E
[00e22b04] 302b 0006                 move.w    6(a3),d0
[00e22b08] b06d 0006                 cmp.w     6(a5),d0
[00e22b0c] 6712                      beq.s     $00E22B20
[00e22b0e] 7010                      moveq.l   #16,d0
[00e22b10] 322d 0004                 move.w    4(a5),d1
[00e22b14] 6f02                      ble.s     $00E22B18
[00e22b16] d041                      add.w     d1,d0
[00e22b18] dac0                      adda.w    d0,a5
[00e22b1a] b7cd                      cmpa.l    a5,a3
[00e22b1c] 6ee0                      bgt.s     $00E22AFE
[00e22b1e] 602c                      bra.s     $00E22B4C
[00e22b20] 224c                      movea.l   a4,a1
[00e22b22] 204b                      movea.l   a3,a0
[00e22b24] 6144                      bsr.s     $00E22B6A
[00e22b26] 671e                      beq.s     $00E22B46
[00e22b28] 2600                      move.l    d0,d3
[00e22b2a] 224d                      movea.l   a5,a1
[00e22b2c] 204b                      movea.l   a3,a0
[00e22b2e] 613a                      bsr.s     $00E22B6A
[00e22b30] 6714                      beq.s     $00E22B46
[00e22b32] 43ed 0008                 lea.l     8(a5),a1
[00e22b36] b680                      cmp.l     d0,d3
[00e22b38] 6e04                      bgt.s     $00E22B3E
[00e22b3a] 43ec 0008                 lea.l     8(a4),a1
[00e22b3e] 41eb 0008                 lea.l     8(a3),a0
[00e22b42] 6100 e7c0                 bsr       $00E21304
[00e22b46] 7e00                      moveq.l   #0,d7
[00e22b48] bbcc                      cmpa.l    a4,a5
[00e22b4a] 6718                      beq.s     $00E22B64
[00e22b4c] 224c                      movea.l   a4,a1
[00e22b4e] 204d                      movea.l   a5,a0
[00e22b50] 6118                      bsr.s     $00E22B6A
[00e22b52] 6610                      bne.s     $00E22B64
[00e22b54] 9e7c 0010                 sub.w     #$0010,d7
[00e22b58] 41ed 0010                 lea.l     16(a5),a0
[00e22b5c] 6002                      bra.s     $00E22B60
[00e22b5e] 3ad8                      move.w    (a0)+,(a5)+
[00e22b60] b7cd                      cmpa.l    a5,a3
[00e22b62] 66fa                      bne.s     $00E22B5E
[00e22b64] 4cdf 200a                 movem.l   (a7)+,d1/d3/a5
[00e22b68] 4e75                      rts
[00e22b6a] 48e7 4018                 movem.l   d1/a3-a4,-(a7)
[00e22b6e] 49e9 0008                 lea.l     8(a1),a4
[00e22b72] 47e8 0008                 lea.l     8(a0),a3
[00e22b76] 2f2c 0004                 move.l    4(a4),-(a7)
[00e22b7a] 2f14                      move.l    (a4),-(a7)
[00e22b7c] 224f                      movea.l   a7,a1
[00e22b7e] 204b                      movea.l   a3,a0
[00e22b80] 6100 e782                 bsr       $00E21304
[00e22b84] 302c 0004                 move.w    4(a4),d0
[00e22b88] c1ec 0006                 muls.w    6(a4),d0
[00e22b8c] 322b 0004                 move.w    4(a3),d1
[00e22b90] c3eb 0006                 muls.w    6(a3),d1
[00e22b94] d280                      add.l     d0,d1
[00e22b96] 2001                      move.l    d1,d0
[00e22b98] e488                      lsr.l     #2,d0
[00e22b9a] d280                      add.l     d0,d1
[00e22b9c] 302f 0004                 move.w    4(a7),d0
[00e22ba0] c1ef 0006                 muls.w    6(a7),d0
[00e22ba4] b081                      cmp.l     d1,d0
[00e22ba6] 6e0a                      bgt.s     $00E22BB2
[00e22ba8] 2897                      move.l    (a7),(a4)
[00e22baa] 296f 0004 0004            move.l    4(a7),4(a4)
[00e22bb0] 7000                      moveq.l   #0,d0
[00e22bb2] 508f                      addq.l    #8,a7
[00e22bb4] 4cdf 1802                 movem.l   (a7)+,d1/a3-a4
[00e22bb8] 4e75                      rts
[00e22bba] 2415                      move.l    (a5),d2
[00e22bbc] b494                      cmp.l     (a4),d2
[00e22bbe] 6742                      beq.s     $00E22C02
[00e22bc0] 3215                      move.w    (a5),d1
[00e22bc2] 9254                      sub.w     (a4),d1
[00e22bc4] 946c 0002                 sub.w     2(a4),d2
[00e22bc8] 3007                      move.w    d7,d0
[00e22bca] 6100 e65c                 bsr       $00E21228
[00e22bce] 2068 0002                 movea.l   2(a0),a0
[00e22bd2] 41e8 0090                 lea.l     144(a0),a0
[00e22bd6] 2250                      movea.l   (a0),a1
[00e22bd8] d2e8 0004                 adda.w    4(a0),a1
[00e22bdc] 2050                      movea.l   (a0),a0
[00e22bde] 6000 001e                 bra.w     $00E22BFE
[00e22be2] 0c50 0014                 cmpi.w    #$0014,(a0)
[00e22be6] 660e                      bne.s     $00E22BF6
[00e22be8] b068 0006                 cmp.w     6(a0),d0
[00e22bec] 6608                      bne.s     $00E22BF6
[00e22bee] d368 0008                 add.w     d1,8(a0)
[00e22bf2] d568 000a                 add.w     d2,10(a0)
[00e22bf6] d0e8 0004                 adda.w    4(a0),a0
[00e22bfa] d0fc 0010                 adda.w    #$0010,a0
[00e22bfe] b3c8                      cmpa.l    a0,a1
[00e22c00] 6ee0                      bgt.s     $00E22BE2
[00e22c02] 4e75                      rts
[00e22c04] 4a39 0000 af10            tst.b     $0000AF10
[00e22c0a] 670c                      beq.s     $00E22C18
[00e22c0c] 302e 0008                 move.w    8(a6),d0
[00e22c10] 5340                      subq.w    #1,d0
[00e22c12] 6d0a                      blt.s     $00E22C1E
[00e22c14] 670c                      beq.s     $00E22C22
[00e22c16] 611c                      bsr.s     $00E22C34
[00e22c18] 4a6e 0008                 tst.w     8(a6)
[00e22c1c] 4e75                      rts
[00e22c1e] 6120                      bsr.s     $00E22C40
[00e22c20] 6002                      bra.s     $00E22C24
[00e22c22] 6134                      bsr.s     $00E22C58
[00e22c24] 4e5e                      unlk      a6
[00e22c26] 4e75                      rts
[00e22c28] 6128                      bsr.s     $00E22C52
[00e22c2a] 6000 f9a4                 bra       $00E225D0
[00e22c2e] 6104                      bsr.s     $00E22C34
[00e22c30] 6000 f99a                 bra       $00E225CC
[00e22c34] 41f9 0000 af10            lea.l     $0000AF10,a0
[00e22c3a] 4a10                      tst.b     (a0)
[00e22c3c] 67e8                      beq.s     $00E22C26
[00e22c3e] 51d0                      sf        (a0)
[00e22c40] 6100 270e                 bsr       $00E25350
[00e22c44] 4e71                      nop
[00e22c46] 6100 3706                 bsr       $00E2634E
[00e22c4a] 4e71                      nop
[00e22c4c] 6000 271e                 bra       $00E2536C
[00e22c50] 4e71                      nop
[00e22c52] 50f9 0000 af10            st        $0000AF10
[00e22c58] 6100 36d6                 bsr       $00E26330
[00e22c5c] 4e71                      nop
[00e22c5e] 41f9 00e3 e18a            lea.l     $00E3E18A,a0
[00e22c64] 4868 007e                 pea.l     126(a0)
[00e22c68] 6100 b908                 bsr       $00E1E572
[00e22c6c] 588f                      addq.l    #4,a7
[00e22c6e] 4e75                      rts
[00e22c70] 6114                      bsr.s     $00E22C86
[00e22c72] 6710                      beq.s     $00E22C84
[00e22c74] 082d 0004 0001            btst      #4,1(a5)
[00e22c7a] 67f4                      beq.s     $00E22C70
[00e22c7c] 4a39 0000 af10            tst.b     $0000AF10
[00e22c82] 67ec                      beq.s     $00E22C70
[00e22c84] 4e75                      rts
[00e22c86] 4ef9 00e1 b1e4            jmp       $00E1B1E4
[00e22c8c] 6100 fc68                 bsr       $00E228F6
[00e22c90] 48e7 5e1c                 movem.l   d1/d3-d6/a3-a5,-(a7)
[00e22c94] 6100 ff92                 bsr.w     $00E22C28
[00e22c98] 3684                      move.w    d4,(a3)
[00e22c9a] 7000                      moveq.l   #0,d0
[00e22c9c] 6100 fb32                 bsr       $00E227D0
[00e22ca0] 670a                      beq.s     $00E22CAC
[00e22ca2] 70ec                      moveq.l   #-20,d0
[00e22ca4] d084                      add.l     d4,d0
[00e22ca6] d080                      add.l     d0,d0
[00e22ca8] 6100 fb14                 bsr       $00E227BE
[00e22cac] 6100 ffc2                 bsr.w     $00E22C70
[00e22cb0] 670c                      beq.s     $00E22CBE
[00e22cb2] bab9 0000 8bd2            cmp.l     $00008BD2,d5
[00e22cb8] 64f2                      bcc.s     $00E22CAC
[00e22cba] 70ff                      moveq.l   #-1,d0
[00e22cbc] 60de                      bra.s     $00E22C9C
[00e22cbe] 6100 ff6e                 bsr       $00E22C2E
[00e22cc2] 4cdf 387a                 movem.l   (a7)+,d1/d3-d6/a3-a5
[00e22cc6] 4e75                      rts
[00e22cc8] 6100 fc2c                 bsr       $00E228F6
[00e22ccc] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e22cd0] 2e2b 000a                 move.l    10(a3),d7
[00e22cd4] 2039 0000 8db0            move.l    $00008DB0,d0
[00e22cda] 4840                      swap      d0
[00e22cdc] 906b 000a                 sub.w     10(a3),d0
[00e22ce0] 4840                      swap      d0
[00e22ce2] 9047                      sub.w     d7,d0
[00e22ce4] 2a00                      move.l    d0,d5
[00e22ce6] 6100 ff40                 bsr       $00E22C28
[00e22cea] 2039 0000 8db0            move.l    $00008DB0,d0
[00e22cf0] 7401                      moveq.l   #1,d2
[00e22cf2] 4840                      swap      d0
[00e22cf4] 4846                      swap      d6
[00e22cf6] 4845                      swap      d5
[00e22cf8] 9045                      sub.w     d5,d0
[00e22cfa] b046                      cmp.w     d6,d0
[00e22cfc] 6c02                      bge.s     $00E22D00
[00e22cfe] 3006                      move.w    d6,d0
[00e22d00] 51ca fff0                 dbf       d2,$00E22CF2
[00e22d04] be80                      cmp.l     d0,d7
[00e22d06] 6712                      beq.s     $00E22D1A
[00e22d08] 36bc 001b                 move.w    #$001B,(a3)
[00e22d0c] 2740 000a                 move.l    d0,10(a3)
[00e22d10] 6100 fabc                 bsr       $00E227CE
[00e22d14] 6704                      beq.s     $00E22D1A
[00e22d16] 2e2b 000a                 move.l    10(a3),d7
[00e22d1a] 6100 ff54                 bsr       $00E22C70
[00e22d1e] 66ca                      bne.s     $00E22CEA
[00e22d20] 6100 ff0c                 bsr       $00E22C2E
[00e22d24] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e22d28] 4e75                      rts
[00e22d2a] 6100 fbca                 bsr       $00E228F6
[00e22d2e] 45ee fff8                 lea.l     -8(a6),a2
[00e22d32] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e22d36] 284a                      movea.l   a2,a4
[00e22d38] 2c2b 0006                 move.l    6(a3),d6
[00e22d3c] 2e2b 000a                 move.l    10(a3),d7
[00e22d40] 2039 0000 8db0            move.l    $00008DB0,d0
[00e22d46] 4840                      swap      d0
[00e22d48] 906b 0006                 sub.w     6(a3),d0
[00e22d4c] 4840                      swap      d0
[00e22d4e] 9046                      sub.w     d6,d0
[00e22d50] 2a00                      move.l    d0,d5
[00e22d52] 6100 fed4                 bsr       $00E22C28
[00e22d56] 2039 0000 8db0            move.l    $00008DB0,d0
[00e22d5c] 41ec 0000                 lea.l     0(a4),a0
[00e22d60] 7401                      moveq.l   #1,d2
[00e22d62] 4840                      swap      d0
[00e22d64] 4845                      swap      d5
[00e22d66] 4847                      swap      d7
[00e22d68] 9045                      sub.w     d5,d0
[00e22d6a] 3210                      move.w    (a0),d1
[00e22d6c] b041                      cmp.w     d1,d0
[00e22d6e] 6c02                      bge.s     $00E22D72
[00e22d70] 3001                      move.w    d1,d0
[00e22d72] d268 0004                 add.w     4(a0),d1
[00e22d76] 9247                      sub.w     d7,d1
[00e22d78] b041                      cmp.w     d1,d0
[00e22d7a] 6f02                      ble.s     $00E22D7E
[00e22d7c] 3001                      move.w    d1,d0
[00e22d7e] 5488                      addq.l    #2,a0
[00e22d80] 51ca ffe0                 dbf       d2,$00E22D62
[00e22d84] bc80                      cmp.l     d0,d6
[00e22d86] 6712                      beq.s     $00E22D9A
[00e22d88] 36bc 001c                 move.w    #$001C,(a3)
[00e22d8c] 2740 0006                 move.l    d0,6(a3)
[00e22d90] 6100 fa3c                 bsr       $00E227CE
[00e22d94] 6704                      beq.s     $00E22D9A
[00e22d96] 2c2b 0006                 move.l    6(a3),d6
[00e22d9a] 6100 fed4                 bsr       $00E22C70
[00e22d9e] 66b6                      bne.s     $00E22D56
[00e22da0] 6100 fe8c                 bsr       $00E22C2E
[00e22da4] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e22da8] 4e75                      rts
[00e22daa] 2079 0000 e552            movea.l   $0000E552,a0
[00e22db0] 2008                      move.l    a0,d0
[00e22db2] 67fe                      beq.s     $00E22DB2
[00e22db4] 23d0 0000 e552            move.l    (a0),$0000E552
[00e22dba] 4e75                      rts
[00e22dbc] c1fc 0122                 muls.w    #$0122,d0
[00e22dc0] 2079 0000 aefc            movea.l   $0000AEFC,a0
[00e22dc6] d1c0                      adda.l    d0,a0
[00e22dc8] 4e75                      rts
[00e22dca] 2248                      movea.l   a0,a1
[00e22dcc] 6002                      bra.s     $00E22DD0
[00e22dce] 2251                      movea.l   (a1),a1
[00e22dd0] 2011                      move.l    (a1),d0
[00e22dd2] 66fa                      bne.s     $00E22DCE
[00e22dd4] 22b9 0000 e552            move.l    $0000E552,(a1)
[00e22dda] 23d0 0000 e552            move.l    (a0),$0000E552
[00e22de0] 4290                      clr.l     (a0)
[00e22de2] 4e75                      rts
[00e22de4] 2050                      movea.l   (a0),a0
[00e22de6] 603e                      bra.s     $00E22E26
[00e22de8] 2248                      movea.l   a0,a1
[00e22dea] 6032                      bra.s     $00E22E1E
[00e22dec] 2028 0004                 move.l    4(a0),d0
[00e22df0] 2228 0008                 move.l    8(a0),d1
[00e22df4] d041                      add.w     d1,d0
[00e22df6] b0aa 0004                 cmp.l     4(a2),d0
[00e22dfa] 6620                      bne.s     $00E22E1C
[00e22dfc] 4841                      swap      d1
[00e22dfe] b26a 0008                 cmp.w     8(a2),d1
[00e22e02] 6618                      bne.s     $00E22E1C
[00e22e04] 302a 000a                 move.w    10(a2),d0
[00e22e08] d168 000a                 add.w     d0,10(a0)
[00e22e0c] 2292                      move.l    (a2),(a1)
[00e22e0e] 24b9 0000 e552            move.l    $0000E552,(a2)
[00e22e14] 23ca 0000 e552            move.l    a2,$0000E552
[00e22e1a] 6002                      bra.s     $00E22E1E
[00e22e1c] 224a                      movea.l   a2,a1
[00e22e1e] 2451                      movea.l   (a1),a2
[00e22e20] 200a                      move.l    a2,d0
[00e22e22] 66c8                      bne.s     $00E22DEC
[00e22e24] 2050                      movea.l   (a0),a0
[00e22e26] 2008                      move.l    a0,d0
[00e22e28] 66be                      bne.s     $00E22DE8
[00e22e2a] 4e75                      rts
[00e22e2c] 48e7 181e                 movem.l   d3-d4/a3-a6,-(a7)
[00e22e30] 2f0a                      move.l    a2,-(a7)
[00e22e32] 514f                      subq.w    #8,a7
[00e22e34] 2648                      movea.l   a0,a3
[00e22e36] 2c49                      movea.l   a1,a6
[00e22e38] 7600                      moveq.l   #0,d3
[00e22e3a] 2879 0000 e552            movea.l   $0000E552,a4
[00e22e40] 6000 00ae                 bra       $00E22EF0
[00e22e44] 41ed 0004                 lea.l     4(a5),a0
[00e22e48] 43d7                      lea.l     (a7),a1
[00e22e4a] 22d8                      move.l    (a0)+,(a1)+
[00e22e4c] 22d8                      move.l    (a0)+,(a1)+
[00e22e4e] 43d7                      lea.l     (a7),a1
[00e22e50] 206f 0008                 movea.l   8(a7),a0
[00e22e54] 6100 e46c                 bsr       $00E212C2
[00e22e58] 4a40                      tst.w     d0
[00e22e5a] 6606                      bne.s     $00E22E62
[00e22e5c] 264d                      movea.l   a5,a3
[00e22e5e] 6000 0090                 bra       $00E22EF0
[00e22e62] 7601                      moveq.l   #1,d3
[00e22e64] 302f 0002                 move.w    2(a7),d0
[00e22e68] 906d 0006                 sub.w     6(a5),d0
[00e22e6c] 6f0e                      ble.s     $00E22E7C
[00e22e6e] 6100 009e                 bsr       $00E22F0E
[00e22e72] 20ed 0004                 move.l    4(a5),(a0)+
[00e22e76] 30ed 0008                 move.w    8(a5),(a0)+
[00e22e7a] 3080                      move.w    d0,(a0)
[00e22e7c] 3217                      move.w    (a7),d1
[00e22e7e] 926d 0004                 sub.w     4(a5),d1
[00e22e82] 6f12                      ble.s     $00E22E96
[00e22e84] 6100 0088                 bsr       $00E22F0E
[00e22e88] 30ed 0004                 move.w    4(a5),(a0)+
[00e22e8c] 30ef 0002                 move.w    2(a7),(a0)+
[00e22e90] 30c1                      move.w    d1,(a0)+
[00e22e92] 30af 0006                 move.w    6(a7),(a0)
[00e22e96] 3417                      move.w    (a7),d2
[00e22e98] d46f 0004                 add.w     4(a7),d2
[00e22e9c] 322d 0004                 move.w    4(a5),d1
[00e22ea0] d26d 0008                 add.w     8(a5),d1
[00e22ea4] 9242                      sub.w     d2,d1
[00e22ea6] 6f0e                      ble.s     $00E22EB6
[00e22ea8] 6164                      bsr.s     $00E22F0E
[00e22eaa] 30c2                      move.w    d2,(a0)+
[00e22eac] 30ef 0002                 move.w    2(a7),(a0)+
[00e22eb0] 30c1                      move.w    d1,(a0)+
[00e22eb2] 30af 0006                 move.w    6(a7),(a0)
[00e22eb6] 342f 0002                 move.w    2(a7),d2
[00e22eba] d46f 0006                 add.w     6(a7),d2
[00e22ebe] 302d 0006                 move.w    6(a5),d0
[00e22ec2] d06d 000a                 add.w     10(a5),d0
[00e22ec6] 9042                      sub.w     d2,d0
[00e22ec8] 6f0e                      ble.s     $00E22ED8
[00e22eca] 6142                      bsr.s     $00E22F0E
[00e22ecc] 30ed 0004                 move.w    4(a5),(a0)+
[00e22ed0] 30c2                      move.w    d2,(a0)+
[00e22ed2] 30ed 0008                 move.w    8(a5),(a0)+
[00e22ed6] 3080                      move.w    d0,(a0)
[00e22ed8] 2695                      move.l    (a5),(a3)
[00e22eda] 200e                      move.l    a6,d0
[00e22edc] 670e                      beq.s     $00E22EEC
[00e22ede] 2c8d                      move.l    a5,(a6)
[00e22ee0] 2c4d                      movea.l   a5,a6
[00e22ee2] 41d7                      lea.l     (a7),a0
[00e22ee4] 429d                      clr.l     (a5)+
[00e22ee6] 2ad8                      move.l    (a0)+,(a5)+
[00e22ee8] 2ad8                      move.l    (a0)+,(a5)+
[00e22eea] 6004                      bra.s     $00E22EF0
[00e22eec] 2a8c                      move.l    a4,(a5)
[00e22eee] 284d                      movea.l   a5,a4
[00e22ef0] 2a53                      movea.l   (a3),a5
[00e22ef2] 200d                      move.l    a5,d0
[00e22ef4] 6600 ff4e                 bne       $00E22E44
[00e22ef8] 23cc 0000 e552            move.l    a4,$0000E552
[00e22efe] 200e                      move.l    a6,d0
[00e22f00] 6602                      bne.s     $00E22F04
[00e22f02] 2003                      move.l    d3,d0
[00e22f04] defc 000c                 adda.w    #$000C,a7
[00e22f08] 4cdf 7818                 movem.l   (a7)+,d3-d4/a3-a6
[00e22f0c] 4e75                      rts
[00e22f0e] 204c                      movea.l   a4,a0
[00e22f10] 2688                      move.l    a0,(a3)
[00e22f12] 67fe                      beq.s     $00E22F12
[00e22f14] 2648                      movea.l   a0,a3
[00e22f16] 2858                      movea.l   (a0)+,a4
[00e22f18] 4e75                      rts
[00e22f1a] 48e7 180c                 movem.l   d3-d4/a4-a5,-(a7)
[00e22f1e] 3600                      move.w    d0,d3
[00e22f20] 3003                      move.w    d3,d0
[00e22f22] 4eba fe98                 jsr       $00E22DBC(pc)
[00e22f26] 2848                      movea.l   a0,a4
[00e22f28] 41ec 009e                 lea.l     158(a4),a0
[00e22f2c] 4eba fe9c                 jsr       $00E22DCA(pc)
[00e22f30] 3014                      move.w    (a4),d0
[00e22f32] 0254 fff9                 andi.w    #$FFF9,(a4)
[00e22f36] 0800 0001                 btst      #1,d0
[00e22f3a] 6704                      beq.s     $00E22F40
[00e22f3c] 0054 0004                 ori.w     #$0004,(a4)
[00e22f40] 0800 0003                 btst      #3,d0
[00e22f44] 6756                      beq.s     $00E22F9C
[00e22f46] 486c 0028                 pea.l     40(a4)
[00e22f4a] 4eba fe5e                 jsr       $00E22DAA(pc)
[00e22f4e] 2948 009e                 move.l    a0,158(a4)
[00e22f52] 4298                      clr.l     (a0)+
[00e22f54] 2a48                      movea.l   a0,a5
[00e22f56] 225f                      movea.l   (a7)+,a1
[00e22f58] 20d9                      move.l    (a1)+,(a0)+
[00e22f5a] 2099                      move.l    (a1)+,(a0)
[00e22f5c] 5458                      addq.w    #2,(a0)+
[00e22f5e] 5450                      addq.w    #2,(a0)
[00e22f60] 7000                      moveq.l   #0,d0
[00e22f62] 6100 fe58                 bsr       $00E22DBC
[00e22f66] 2848                      movea.l   a0,a4
[00e22f68] 2068 0036                 movea.l   54(a0),a0
[00e22f6c] 3828 0030                 move.w    48(a0),d4
[00e22f70] 6024                      bra.s     $00E22F96
[00e22f72] 244d                      movea.l   a5,a2
[00e22f74] 93c9                      suba.l    a1,a1
[00e22f76] 41ec 009e                 lea.l     158(a4),a0
[00e22f7a] 4eba feb0                 jsr       $00E22E2C(pc)
[00e22f7e] 4a80                      tst.l     d0
[00e22f80] 6704                      beq.s     $00E22F86
[00e22f82] 0054 0002                 ori.w     #$0002,(a4)
[00e22f86] b644                      cmp.w     d4,d3
[00e22f88] 6608                      bne.s     $00E22F92
[00e22f8a] 41ec 009e                 lea.l     158(a4),a0
[00e22f8e] 4eba fe54                 jsr       $00E22DE4(pc)
[00e22f92] 286c 0032                 movea.l   50(a4),a4
[00e22f96] b66c 0030                 cmp.w     48(a4),d3
[00e22f9a] 66d6                      bne.s     $00E22F72
[00e22f9c] 4cdf 3018                 movem.l   (a7)+,d3-d4/a4-a5
[00e22fa0] 4e75                      rts
[00e22fa2] 48e7 1c00                 movem.l   d3-d5,-(a7)
[00e22fa6] 3429 0006                 move.w    6(a1),d2
[00e22faa] 3a28 0006                 move.w    6(a0),d5
[00e22fae] da68 000a                 add.w     10(a0),d5
[00e22fb2] b445                      cmp.w     d5,d2
[00e22fb4] 5cc5                      sge       d5
[00e22fb6] d469 000a                 add.w     10(a1),d2
[00e22fba] b468 0006                 cmp.w     6(a0),d2
[00e22fbe] 5fc4                      sle       d4
[00e22fc0] 4a41                      tst.w     d1
[00e22fc2] 6e02                      bgt.s     $00E22FC6
[00e22fc4] c945                      exg       d4,d5
[00e22fc6] 4a05                      tst.b     d5
[00e22fc8] 6620                      bne.s     $00E22FEA
[00e22fca] 3429 0004                 move.w    4(a1),d2
[00e22fce] 3628 0004                 move.w    4(a0),d3
[00e22fd2] 4a40                      tst.w     d0
[00e22fd4] 6e08                      bgt.s     $00E22FDE
[00e22fd6] d469 0008                 add.w     8(a1),d2
[00e22fda] b642                      cmp.w     d2,d3
[00e22fdc] 6006                      bra.s     $00E22FE4
[00e22fde] d668 0008                 add.w     8(a0),d3
[00e22fe2] b443                      cmp.w     d3,d2
[00e22fe4] 6d08                      blt.s     $00E22FEE
[00e22fe6] 4a04                      tst.b     d4
[00e22fe8] 6604                      bne.s     $00E22FEE
[00e22fea] 7001                      moveq.l   #1,d0
[00e22fec] 6002                      bra.s     $00E22FF0
[00e22fee] 4240                      clr.w     d0
[00e22ff0] 4cdf 0038                 movem.l   (a7)+,d3-d5
[00e22ff4] 4e75                      rts
[00e22ff6] 48e7 181e                 movem.l   d3-d4/a3-a6,-(a7)
[00e22ffa] 2648                      movea.l   a0,a3
[00e22ffc] 3800                      move.w    d0,d4
[00e22ffe] 3601                      move.w    d1,d3
[00e23000] 2850                      movea.l   (a0),a4
[00e23002] 240c                      move.l    a4,d2
[00e23004] 673e                      beq.s     $00E23044
[00e23006] 6036                      bra.s     $00E2303E
[00e23008] 3203                      move.w    d3,d1
[00e2300a] 3004                      move.w    d4,d0
[00e2300c] 224d                      movea.l   a5,a1
[00e2300e] 204c                      movea.l   a4,a0
[00e23010] 4eba ff90                 jsr       $00E22FA2(pc)
[00e23014] 4a40                      tst.w     d0
[00e23016] 6724                      beq.s     $00E2303C
[00e23018] 2c4b                      movea.l   a3,a6
[00e2301a] 6012                      bra.s     $00E2302E
[00e2301c] 3203                      move.w    d3,d1
[00e2301e] 3004                      move.w    d4,d0
[00e23020] 224d                      movea.l   a5,a1
[00e23022] 2056                      movea.l   (a6),a0
[00e23024] 4eba ff7c                 jsr       $00E22FA2(pc)
[00e23028] 4a40                      tst.w     d0
[00e2302a] 6606                      bne.s     $00E23032
[00e2302c] 2c56                      movea.l   (a6),a6
[00e2302e] b9d6                      cmpa.l    (a6),a4
[00e23030] 66ea                      bne.s     $00E2301C
[00e23032] 2054                      movea.l   (a4),a0
[00e23034] 2890                      move.l    (a0),(a4)
[00e23036] 2096                      move.l    (a6),(a0)
[00e23038] 2c88                      move.l    a0,(a6)
[00e2303a] 6002                      bra.s     $00E2303E
[00e2303c] 284d                      movea.l   a5,a4
[00e2303e] 2a54                      movea.l   (a4),a5
[00e23040] 200d                      move.l    a5,d0
[00e23042] 66c4                      bne.s     $00E23008
[00e23044] 4cdf 7818                 movem.l   (a7)+,d3-d4/a3-a6
[00e23048] 4e75                      rts
[00e2304a] 45f9 0000 e552            lea.l     $0000E552,a2
[00e23050] 2092                      move.l    (a2),(a0)
[00e23052] 6014                      bra.s     $00E23068
[00e23054] 2010                      move.l    (a0),d0
[00e23056] 67fe                      beq.s     $00E23056
[00e23058] 2040                      movea.l   d0,a0
[00e2305a] 2169 0004 0004            move.l    4(a1),4(a0)
[00e23060] 2169 0008 0008            move.l    8(a1),8(a0)
[00e23066] 2251                      movea.l   (a1),a1
[00e23068] 2009                      move.l    a1,d0
[00e2306a] 66e8                      bne.s     $00E23054
[00e2306c] 2490                      move.l    (a0),(a2)
[00e2306e] 4290                      clr.l     (a0)
[00e23070] 4e75                      rts
[00e23072] 4afc                      illegal
[00e23074] 4afc                      illegal
[00e23076] 4afc                      illegal
[00e23078] 4afc                      illegal
[00e2307a] 4afc                      illegal
[00e2307c] 4afc                      illegal
[00e2307e] 4afc                      illegal
[00e23080] 4afc                      illegal
[00e23082] 4afc                      illegal
[00e23084] 4afc                      illegal
[00e23086] 4afc                      illegal
[00e23088] 4afc                      illegal
[00e2308a] 4afc                      illegal
[00e2308c] 4afc                      illegal
[00e2308e] 4afc                      illegal
[00e23090] 4afc                      illegal
[00e23092] 4afc                      illegal
[00e23094] 4afc                      illegal
[00e23096] 4afc                      illegal
[00e23098] 4afc                      illegal
[00e2309a] 4afc                      illegal
[00e2309c] 4afc                      illegal
[00e2309e] 4afc                      illegal
[00e230a0] 4afc                      illegal
[00e230a2] 4afc                      illegal
