[00e1d766] 5749                      subq.w    #3,a1
[00e1d768] 4e58                      unlk      a0
[00e1d76a] 0220 1b51                 andi.b    #$51,-(a0)
[00e1d76e] 205f                      movea.l   (a7)+,a0
[00e1d770] 4879 0000 7a6c            pea.l     $00007A6C
[00e1d776] 2f08                      move.l    a0,-(a7)
[00e1d778] 2479 0000 cb00            movea.l   $0000CB00,a2
[00e1d77e] 342a 001c                 move.w    28(a2),d2
[00e1d782] 43ef 0004                 lea.l     4(a7),a1
[00e1d786] 2059                      movea.l   (a1)+,a0
[00e1d788] 2f08                      move.l    a0,-(a7)
[00e1d78a] 3f3c 0010                 move.w    #$0010,-(a7)
[00e1d78e] 30d9                      move.w    (a1)+,(a0)+
[00e1d790] 3f19                      move.w    (a1)+,-(a7)
[00e1d792] 30c2                      move.w    d2,(a0)+
[00e1d794] 4258                      clr.w     (a0)+
[00e1d796] 30d9                      move.w    (a1)+,(a0)+
[00e1d798] 20d9                      move.l    (a1)+,(a0)+
[00e1d79a] 2091                      move.l    (a1),(a0)
[00e1d79c] 3f3c 0002                 move.w    #$0002,-(a7)
[00e1d7a0] b46f 0002                 cmp.w     2(a7),d2
[00e1d7a4] 6608                      bne.s     $00E1D7AE
[00e1d7a6] 0c6a 04f0 0094            cmpi.w    #$04F0,148(a2)
[00e1d7ac] 6e06                      bgt.s     $00E1D7B4
[00e1d7ae] 6100 88e8                 bsr       $00E16098
[00e1d7b2] 4e71                      nop
[00e1d7b4] 4fef 000a                 lea.l     10(a7),a7
[00e1d7b8] 4e75                      rts
[00e1d7ba] 2079 0000 94b2            movea.l   $000094B2,a0
[00e1d7c0] 4e75                      rts
[00e1d7c2] 2f00                      move.l    d0,-(a7)
[00e1d7c4] 61f4                      bsr.s     $00E1D7BA
[00e1d7c6] c1fc 0122                 muls.w    #$0122,d0
[00e1d7ca] d1c0                      adda.l    d0,a0
[00e1d7cc] 201f                      move.l    (a7)+,d0
[00e1d7ce] 4e75                      rts
[00e1d7d0] 3039 0000 94b0            move.w    $000094B0,d0
[00e1d7d6] 6014                      bra.s     $00E1D7EC
[00e1d7d8] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e1d7de] 6008                      bra.s     $00E1D7E8
[00e1d7e0] 3007                      move.w    d7,d0
[00e1d7e2] 61de                      bsr.s     $00E1D7C2
[00e1d7e4] 2068 0002                 movea.l   2(a0),a0
[00e1d7e8] 3028 0096                 move.w    150(a0),d0
[00e1d7ec] 0300                      btst      d1,d0
[00e1d7ee] 4e75                      rts
[00e1d7f0] 61d0                      bsr.s     $00E1D7C2
[00e1d7f2] 4a41                      tst.w     d1
[00e1d7f4] 6c02                      bge.s     $00E1D7F8
[00e1d7f6] 4441                      neg.w     d1
[00e1d7f8] e749                      lsl.w     #3,d1
[00e1d7fa] 41f0 1010                 lea.l     16(a0,d1.w),a0
[00e1d7fe] 4e75                      rts
[00e1d800] 7203                      moveq.l   #3,d1
[00e1d802] 2f08                      move.l    a0,-(a7)
[00e1d804] 3f01                      move.w    d1,-(a7)
[00e1d806] 61e8                      bsr.s     $00E1D7F0
[00e1d808] 2248                      movea.l   a0,a1
[00e1d80a] 321f                      move.w    (a7)+,d1
[00e1d80c] 2057                      movea.l   (a7),a0
[00e1d80e] 20d9                      move.l    (a1)+,(a0)+
[00e1d810] 2091                      move.l    (a1),(a0)
[00e1d812] 4a41                      tst.w     d1
[00e1d814] 6c0c                      bge.s     $00E1D822
[00e1d816] 4a58                      tst.w     (a0)+
[00e1d818] 6708                      beq.s     $00E1D822
[00e1d81a] 4a58                      tst.w     (a0)+
[00e1d81c] 6704                      beq.s     $00E1D822
[00e1d81e] 5460                      addq.w    #2,-(a0)
[00e1d820] 5460                      addq.w    #2,-(a0)
[00e1d822] 205f                      movea.l   (a7)+,a0
[00e1d824] 4e75                      rts
[00e1d826] 7000                      moveq.l   #0,d0
[00e1d828] 41f9 0000 7a58            lea.l     $00007A58,a0
[00e1d82e] 43f9 0000 7a62            lea.l     $00007A62,a1
[00e1d834] 2448                      movea.l   a0,a2
[00e1d836] 2f09                      move.l    a1,-(a7)
[00e1d838] 7200                      moveq.l   #0,d1
[00e1d83a] 6112                      bsr.s     $00E1D84E
[00e1d83c] 205f                      movea.l   (a7)+,a0
[00e1d83e] 7203                      moveq.l   #3,d1
[00e1d840] 610c                      bsr.s     $00E1D84E
[00e1d842] 7202                      moveq.l   #2,d1
[00e1d844] 6108                      bsr.s     $00E1D84E
[00e1d846] 220a                      move.l    a2,d1
[00e1d848] 6710                      beq.s     $00E1D85A
[00e1d84a] 2041                      movea.l   d1,a0
[00e1d84c] 7201                      moveq.l   #1,d1
[00e1d84e] 2f08                      move.l    a0,-(a7)
[00e1d850] 619e                      bsr.s     $00E1D7F0
[00e1d852] 2257                      movea.l   (a7),a1
[00e1d854] 20d9                      move.l    (a1)+,(a0)+
[00e1d856] 20d9                      move.l    (a1)+,(a0)+
[00e1d858] 205f                      movea.l   (a7)+,a0
[00e1d85a] 4e75                      rts
[00e1d85c] 2f09                      move.l    a1,-(a7)
[00e1d85e] 2f08                      move.l    a0,-(a7)
[00e1d860] 6100 4a46                 bsr       $00E222A8
[00e1d864] 4e71                      nop
[00e1d866] 508f                      addq.l    #8,a7
[00e1d868] 4a40                      tst.w     d0
[00e1d86a] 4e75                      rts
[00e1d86c] 204d                      movea.l   a5,a0
[00e1d86e] 3007                      move.w    d7,d0
[00e1d870] 6190                      bsr.s     $00E1D802
[00e1d872] 224d                      movea.l   a5,a1
[00e1d874] 41f9 0000 7a58            lea.l     $00007A58,a0
[00e1d87a] 61e0                      bsr.s     $00E1D85C
[00e1d87c] 671e                      beq.s     $00E1D89C
[00e1d87e] 200c                      move.l    a4,d0
[00e1d880] 6708                      beq.s     $00E1D88A
[00e1d882] 224d                      movea.l   a5,a1
[00e1d884] 204c                      movea.l   a4,a0
[00e1d886] 61d4                      bsr.s     $00E1D85C
[00e1d888] 6712                      beq.s     $00E1D89C
[00e1d88a] 200b                      move.l    a3,d0
[00e1d88c] 5280                      addq.l    #1,d0
[00e1d88e] 660a                      bne.s     $00E1D89A
[00e1d890] 3007                      move.w    d7,d0
[00e1d892] 6100 ff2e                 bsr       $00E1D7C2
[00e1d896] 2668 009e                 movea.l   158(a0),a3
[00e1d89a] 200b                      move.l    a3,d0
[00e1d89c] 4e75                      rts
[00e1d89e] 2f09                      move.l    a1,-(a7)
[00e1d8a0] 2f08                      move.l    a0,-(a7)
[00e1d8a2] 6100 4a6e                 bsr       $00E22312
[00e1d8a6] 4e71                      nop
[00e1d8a8] 508f                      addq.l    #8,a7
[00e1d8aa] 4e75                      rts
[00e1d8ac] 2f08                      move.l    a0,-(a7)
[00e1d8ae] 2f00                      move.l    d0,-(a7)
[00e1d8b0] 6100 49aa                 bsr       $00E2225C
[00e1d8b4] 4e71                      nop
[00e1d8b6] 508f                      addq.l    #8,a7
[00e1d8b8] 4e75                      rts
[00e1d8ba] 6100 ff06                 bsr       $00E1D7C2
[00e1d8be] 2068 0032                 movea.l   50(a0),a0
[00e1d8c2] 3028 0030                 move.w    48(a0),d0
[00e1d8c6] 4e75                      rts
[00e1d8c8] 6100 fef8                 bsr       $00E1D7C2
[00e1d8cc] 2468 0032                 movea.l   50(a0),a2
[00e1d8d0] 2268 0036                 movea.l   54(a0),a1
[00e1d8d4] 234a 0032                 move.l    a2,50(a1)
[00e1d8d8] 2549 0036                 move.l    a1,54(a2)
[00e1d8dc] 42a8 0032                 clr.l     50(a0)
[00e1d8e0] 42a8 0036                 clr.l     54(a0)
[00e1d8e4] 4e75                      rts
[00e1d8e6] 6100 feda                 bsr       $00E1D7C2
[00e1d8ea] 2248                      movea.l   a0,a1
[00e1d8ec] 6100 fecc                 bsr       $00E1D7BA
[00e1d8f0] 2468 0036                 movea.l   54(a0),a2
[00e1d8f4] 2549 0032                 move.l    a1,50(a2)
[00e1d8f8] 234a 0036                 move.l    a2,54(a1)
[00e1d8fc] 2348 0032                 move.l    a0,50(a1)
[00e1d900] 2149 0036                 move.l    a1,54(a0)
[00e1d904] 4e75                      rts
[00e1d906] 3f01                      move.w    d1,-(a7)
[00e1d908] 6100 ffbe                 bsr.w     $00E1D8C8
[00e1d90c] 2248                      movea.l   a0,a1
[00e1d90e] 6100 feaa                 bsr       $00E1D7BA
[00e1d912] 4a5f                      tst.w     (a7)+
[00e1d914] 6604                      bne.s     $00E1D91A
[00e1d916] 2068 0032                 movea.l   50(a0),a0
[00e1d91a] 60d4                      bra.s     $00E1D8F0
[00e1d91c] 2f00                      move.l    d0,-(a7)
[00e1d91e] 2f08                      move.l    a0,-(a7)
[00e1d920] 6100 28fa                 bsr       $00E2021C
[00e1d924] 508f                      addq.l    #8,a7
[00e1d926] 4e75                      rts
[00e1d928] 2f09                      move.l    a1,-(a7)
[00e1d92a] 3f00                      move.w    d0,-(a7)
[00e1d92c] 2f08                      move.l    a0,-(a7)
[00e1d92e] 6100 2c54                 bsr       $00E20584
[00e1d932] defc 000a                 adda.w    #$000A,a7
[00e1d936] 4e75                      rts
[00e1d938] 2f08                      move.l    a0,-(a7)
[00e1d93a] 6100 305e                 bsr       $00E2099A
[00e1d93e] 4e71                      nop
[00e1d940] 588f                      addq.l    #4,a7
[00e1d942] 4e75                      rts
[00e1d944] 7001                      moveq.l   #1,d0
[00e1d946] be79 0000 7b8c            cmp.w     $00007B8C,d7
[00e1d94c] 6708                      beq.s     $00E1D956
[00e1d94e] 7200                      moveq.l   #0,d1
[00e1d950] 6100 fe8e                 bsr       $00E1D7E0
[00e1d954] 56c0                      sne       d0
[00e1d956] 4880                      ext.w     d0
[00e1d958] 4e75                      rts
[00e1d95a] c07c 0004                 and.w     #$0004,d0
[00e1d95e] 6708                      beq.s     $00E1D968
[00e1d960] 7208                      moveq.l   #8,d1
[00e1d962] 6100 fe6c                 bsr       $00E1D7D0
[00e1d966] 6606                      bne.s     $00E1D96E
[00e1d968] 61da                      bsr.s     $00E1D944
[00e1d96a] 6714                      beq.s     $00E1D980
[00e1d96c] 4e75                      rts
[00e1d96e] 6100 fe4a                 bsr       $00E1D7BA
[00e1d972] 2268 0036                 movea.l   54(a0),a1
[00e1d976] 3029 0030                 move.w    48(a1),d0
[00e1d97a] 6712                      beq.s     $00E1D98E
[00e1d97c] be40                      cmp.w     d0,d7
[00e1d97e] 6704                      beq.s     $00E1D984
[00e1d980] 7815                      moveq.l   #21,d4
[00e1d982] 600a                      bra.s     $00E1D98E
[00e1d984] 7209                      moveq.l   #9,d1
[00e1d986] 6100 fe48                 bsr       $00E1D7D0
[00e1d98a] 6702                      beq.s     $00E1D98E
[00e1d98c] 7821                      moveq.l   #33,d4
[00e1d98e] 7000                      moveq.l   #0,d0
[00e1d990] 4e75                      rts
[00e1d992] 2e00                      move.l    d0,d7
[00e1d994] 43fa fffc                 lea.l     $00E1D992(pc),a1
[00e1d998] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e1d99e] 2f08                      move.l    a0,-(a7)
[00e1d9a0] 2049                      movea.l   a1,a0
[00e1d9a2] 7007                      moveq.l   #7,d0
[00e1d9a4] 6118                      bsr.s     $00E1D9BE
[00e1d9a6] 205f                      movea.l   (a7)+,a0
[00e1d9a8] 660e                      bne.s     $00E1D9B8
[00e1d9aa] 303c 0fff                 move.w    #$0FFF,d0
[00e1d9ae] 0c11 002e                 cmpi.b    #$2E,(a1)
[00e1d9b2] 6604                      bne.s     $00E1D9B8
[00e1d9b4] 303c 0fff                 move.w    #$0FFF,d0
[00e1d9b8] 3140 0096                 move.w    d0,150(a0)
[00e1d9bc] 4e75                      rts
[00e1d9be] 2239 0000 94ba            move.l    $000094BA,d1
[00e1d9c4] 6712                      beq.s     $00E1D9D8
[00e1d9c6] 2f08                      move.l    a0,-(a7)
[00e1d9c8] 3f00                      move.w    d0,-(a7)
[00e1d9ca] 2041                      movea.l   d1,a0
[00e1d9cc] 4e90                      jsr       (a0)
[00e1d9ce] 5c8f                      addq.l    #6,a7
[00e1d9d0] 4a80                      tst.l     d0
[00e1d9d2] 6b04                      bmi.s     $00E1D9D8
[00e1d9d4] 7201                      moveq.l   #1,d1
[00e1d9d6] 4e75                      rts
[00e1d9d8] 7200                      moveq.l   #0,d1
[00e1d9da] 4e75                      rts
[00e1d9dc] 4a40                      tst.w     d0
[00e1d9de] 6604                      bne.s     $00E1D9E4
[00e1d9e0] d2fc 002e                 adda.w    #$002E,a1
[00e1d9e4] 7016                      moveq.l   #22,d0
[00e1d9e6] 0c28 0016 0007            cmpi.b    #$16,7(a0)
[00e1d9ec] 660a                      bne.s     $00E1D9F8
[00e1d9ee] 2468 000c                 movea.l   12(a0),a2
[00e1d9f2] 3559 0012                 move.w    (a1)+,18(a2)
[00e1d9f6] 6004                      bra.s     $00E1D9FC
[00e1d9f8] 3159 000e                 move.w    (a1)+,14(a0)
[00e1d9fc] d0fc 0018                 adda.w    #$0018,a0
[00e1da00] 51c8 ffe4                 dbf       d0,$00E1D9E6
[00e1da04] 4e75                      rts
[00e1da06] 6100 fdba                 bsr       $00E1D7C2
[00e1da0a] 3010                      move.w    (a0),d0
[00e1da0c] 2079 0000 77ca            movea.l   $000077CA,a0
[00e1da12] 0800 0004                 btst      #4,d0
[00e1da16] 6704                      beq.s     $00E1DA1C
[00e1da18] d0fc 0260                 adda.w    #$0260,a0
[00e1da1c] 4e75                      rts
[00e1da1e] 4e56 ffee                 link      a6,#$FFEE
[00e1da22] 48e7 5f1c                 movem.l   d1/d3-d7/a3-a5,-(a7)
[00e1da26] 3c01                      move.w    d1,d6
[00e1da28] 2d79 0000 94be ffee       move.l    $000094BE,-18(a6)
[00e1da30] 47ee fff2                 lea.l     -14(a6),a3
[00e1da34] 3007                      move.w    d7,d0
[00e1da36] 6100 fd8a                 bsr       $00E1D7C2
[00e1da3a] 2a48                      movea.l   a0,a5
[00e1da3c] 6100 fda6                 bsr       $00E1D7E4
[00e1da40] 3d40 fffe                 move.w    d0,-2(a6)
[00e1da44] 3007                      move.w    d7,d0
[00e1da46] 61be                      bsr.s     $00E1DA06
[00e1da48] 2848                      movea.l   a0,a4
[00e1da4a] 4a46                      tst.w     d6
[00e1da4c] 6704                      beq.s     $00E1DA52
[00e1da4e] 6100 0c44                 bsr       $00E1E694
[00e1da52] 6100 fef0                 bsr       $00E1D944
[00e1da56] 3d40 fffc                 move.w    d0,-4(a6)
[00e1da5a] 6c02                      bge.s     $00E1DA5E
[00e1da5c] 4240                      clr.w     d0
[00e1da5e] 3d40 fffa                 move.w    d0,-6(a6)
[00e1da62] 70ff                      moveq.l   #-1,d0
[00e1da64] 2940 0002                 move.l    d0,2(a4)
[00e1da68] 3c2d 0006                 move.w    6(a5),d6
[00e1da6c] 204b                      movea.l   a3,a0
[00e1da6e] 3007                      move.w    d7,d0
[00e1da70] 6100 fd8e                 bsr       $00E1D800
[00e1da74] 41ec 0010                 lea.l     16(a4),a0
[00e1da78] 224b                      movea.l   a3,a1
[00e1da7a] 20d9                      move.l    (a1)+,(a0)+
[00e1da7c] 2091                      move.l    (a1),(a0)
[00e1da7e] 206d 0002                 movea.l   2(a5),a0
[00e1da82] 6100 11e0                 bsr       $00E1EC64
[00e1da86] 3a00                      move.w    d0,d5
[00e1da88] 4840                      swap      d0
[00e1da8a] 3800                      move.w    d0,d4
[00e1da8c] 43ed 0042                 lea.l     66(a5),a1
[00e1da90] 302e fffa                 move.w    -6(a6),d0
[00e1da94] 204c                      movea.l   a4,a0
[00e1da96] 6100 ff44                 bsr       $00E1D9DC
[00e1da9a] 4293                      clr.l     (a3)
[00e1da9c] 3e2e fff0                 move.w    -16(a6),d7
[00e1daa0] 5347                      subq.w    #1,d7
[00e1daa2] 3006                      move.w    d6,d0
[00e1daa4] c07c 0007                 and.w     #$0007,d0
[00e1daa8] 6700 007c                 beq.w     $00E1DB26
[00e1daac] 397c 0014 001e            move.w    #$0014,30(a4)
[00e1dab2] 7001                      moveq.l   #1,d0
[00e1dab4] 6100 0190                 bsr       $00E1DC46
[00e1dab8] 20d3                      move.l    (a3),(a0)+
[00e1daba] 30eb 0004                 move.w    4(a3),(a0)+
[00e1dabe] 362b 0004                 move.w    4(a3),d3
[00e1dac2] 0806 0001                 btst      #1,d6
[00e1dac6] 6716                      beq.s     $00E1DADE
[00e1dac8] 4a6e fffc                 tst.w     -4(a6)
[00e1dacc] 6710                      beq.s     $00E1DADE
[00e1dace] 7022                      moveq.l   #34,d0
[00e1dad0] 6100 0174                 bsr       $00E1DC46
[00e1dad4] 20d3                      move.l    (a3),(a0)+
[00e1dad6] 302e ffee                 move.w    -18(a6),d0
[00e1dada] d153                      add.w     d0,(a3)
[00e1dadc] 9640                      sub.w     d0,d3
[00e1dade] 0806 0002                 btst      #2,d6
[00e1dae2] 671a                      beq.s     $00E1DAFE
[00e1dae4] 4a6e fffc                 tst.w     -4(a6)
[00e1dae8] 6714                      beq.s     $00E1DAFE
[00e1daea] 966e ffee                 sub.w     -18(a6),d3
[00e1daee] 7024                      moveq.l   #36,d0
[00e1daf0] 6100 0154                 bsr       $00E1DC46
[00e1daf4] 3003                      move.w    d3,d0
[00e1daf6] d053                      add.w     (a3),d0
[00e1daf8] 30c0                      move.w    d0,(a0)+
[00e1dafa] 30eb 0002                 move.w    2(a3),(a0)+
[00e1dafe] 0806 0000                 btst      #0,d6
[00e1db02] 6718                      beq.s     $00E1DB1C
[00e1db04] 397c 0019 001e            move.w    #$0019,30(a4)
[00e1db0a] 206c 0054                 movea.l   84(a4),a0
[00e1db0e] 20ad 0008                 move.l    8(a5),(a0)
[00e1db12] 7023                      moveq.l   #35,d0
[00e1db14] 6100 0130                 bsr       $00E1DC46
[00e1db18] 20d3                      move.l    (a3),(a0)+
[00e1db1a] 30c3                      move.w    d3,(a0)+
[00e1db1c] 4253                      clr.w     (a3)
[00e1db1e] df6b 0002                 add.w     d7,2(a3)
[00e1db22] 9f6b 0006                 sub.w     d7,6(a3)
[00e1db26] 362e ffee                 move.w    -18(a6),d3
[00e1db2a] 5343                      subq.w    #1,d3
[00e1db2c] 0806 0004                 btst      #4,d6
[00e1db30] 671c                      beq.s     $00E1DB4E
[00e1db32] 206c 0084                 movea.l   132(a4),a0
[00e1db36] 20ad 000c                 move.l    12(a5),(a0)
[00e1db3a] 7005                      moveq.l   #5,d0
[00e1db3c] 6100 0108                 bsr       $00E1DC46
[00e1db40] 20d3                      move.l    (a3),(a0)+
[00e1db42] 30eb 0004                 move.w    4(a3),(a0)+
[00e1db46] df6b 0002                 add.w     d7,2(a3)
[00e1db4a] 9f6b 0006                 sub.w     d7,6(a3)
[00e1db4e] 7006                      moveq.l   #6,d0
[00e1db50] 6100 00f4                 bsr       $00E1DC46
[00e1db54] 20db                      move.l    (a3)+,(a0)+
[00e1db56] 20db                      move.l    (a3)+,(a0)+
[00e1db58] 5563                      subq.w    #2,-(a3)
[00e1db5a] 4a44                      tst.w     d4
[00e1db5c] 6702                      beq.s     $00E1DB60
[00e1db5e] 9f53                      sub.w     d7,(a3)
[00e1db60] 5563                      subq.w    #2,-(a3)
[00e1db62] 4a45                      tst.w     d5
[00e1db64] 6702                      beq.s     $00E1DB68
[00e1db66] 9753                      sub.w     d3,(a3)
[00e1db68] 273c 0001 0001            move.l    #$00010001,-(a3)
[00e1db6e] 303c 00c7                 move.w    #$00C7,d0
[00e1db72] 6100 00d2                 bsr       $00E1DC46
[00e1db76] 20d3                      move.l    (a3),(a0)+
[00e1db78] 20ab 0004                 move.l    4(a3),(a0)
[00e1db7c] 0806 0005                 btst      #5,d6
[00e1db80] 6716                      beq.s     $00E1DB98
[00e1db82] 4a45                      tst.w     d5
[00e1db84] 670a                      beq.s     $00E1DB90
[00e1db86] 4a44                      tst.w     d4
[00e1db88] 660e                      bne.s     $00E1DB98
[00e1db8a] 9f6b 0006                 sub.w     d7,6(a3)
[00e1db8e] 6008                      bra.s     $00E1DB98
[00e1db90] 4a44                      tst.w     d4
[00e1db92] 6704                      beq.s     $00E1DB98
[00e1db94] 976b 0004                 sub.w     d3,4(a3)
[00e1db98] 202b 0004                 move.l    4(a3),d0
[00e1db9c] d16b 0002                 add.w     d0,2(a3)
[00e1dba0] 4840                      swap      d0
[00e1dba2] d153                      add.w     d0,(a3)
[00e1dba4] 0806 0005                 btst      #5,d6
[00e1dba8] 56c7                      sne       d7
[00e1dbaa] 4a45                      tst.w     d5
[00e1dbac] 671c                      beq.s     $00E1DBCA
[00e1dbae] 4a44                      tst.w     d4
[00e1dbb0] 6702                      beq.s     $00E1DBB4
[00e1dbb2] 7e01                      moveq.l   #1,d7
[00e1dbb4] 322b 0006                 move.w    6(a3),d1
[00e1dbb8] 4257                      clr.w     (a7)
[00e1dbba] 3f13                      move.w    (a3),-(a7)
[00e1dbbc] 2f2e fffc                 move.l    -4(a6),-(a7)
[00e1dbc0] 45fa 006c                 lea.l     $00E1DC2E(pc),a2
[00e1dbc4] 6100 00ae                 bsr       $00E1DC74
[00e1dbc8] 5c8f                      addq.l    #6,a7
[00e1dbca] 4a44                      tst.w     d4
[00e1dbcc] 6718                      beq.s     $00E1DBE6
[00e1dbce] 322b 0004                 move.w    4(a3),d1
[00e1dbd2] 3eab 0002                 move.w    2(a3),(a7)
[00e1dbd6] 4267                      clr.w     -(a7)
[00e1dbd8] 2f2e fffc                 move.l    -4(a6),-(a7)
[00e1dbdc] 45fa 0038                 lea.l     $00E1DC16(pc),a2
[00e1dbe0] 6100 0092                 bsr       $00E1DC74
[00e1dbe4] 5c8f                      addq.l    #6,a7
[00e1dbe6] 4a07                      tst.b     d7
[00e1dbe8] 671e                      beq.s     $00E1DC08
[00e1dbea] 303c 00c8                 move.w    #$00C8,d0
[00e1dbee] 6100 0056                 bsr.w     $00E1DC46
[00e1dbf2] 20d3                      move.l    (a3),(a0)+
[00e1dbf4] 7000                      moveq.l   #0,d0
[00e1dbf6] 4a6e fffc                 tst.w     -4(a6)
[00e1dbfa] 6708                      beq.s     $00E1DC04
[00e1dbfc] 0806 0005                 btst      #5,d6
[00e1dc00] 6702                      beq.s     $00E1DC04
[00e1dc02] 7006                      moveq.l   #6,d0
[00e1dc04] 1940 00cc                 move.b    d0,204(a4)
[00e1dc08] 4e71                      nop
[00e1dc0a] 4e71                      nop
[00e1dc0c] 204c                      movea.l   a4,a0
[00e1dc0e] 4cdf 38fa                 movem.l   (a7)+,d1/d3-d7/a3-a5
[00e1dc12] 4e5e                      unlk      a6
[00e1dc14] 4e75                      rts
[00e1dc16] 0000                      dc.w      $0000
[00e1dc18] 0150                      bchg      d0,(a0)
[00e1dc1a] 00ce                      dc.w      $00CE ; illegal
[00e1dc1c] 000a 01d0                 ori.b     #$D0,a2 ; apollo only
[00e1dc20] 01d6                      bset      d0,(a6)
[00e1dc22] 0009 01cf                 ori.b     #$CF,a1 ; apollo only
[00e1dc26] 01d5                      bset      d0,(a5)
[00e1dc28] 000b 01d1                 ori.b     #$D1,a3 ; apollo only
[00e1dc2c] 0232 0002 00d8            andi.b    #$02,-40(a2,d0.w)
[00e1dc32] 00c9                      dc.w      $00C9 ; illegal
[00e1dc34] 0007 012b                 ori.b     #$2B,d7
[00e1dc38] 0134 0006                 btst      d0,6(a4,d0.w)
[00e1dc3c] 012a 0133                 btst      d0,307(a2)
[00e1dc40] 0008 012c                 ori.b     #$2C,a0 ; apollo only
[00e1dc44] 018d 3200                 movep.w   d0,12800(a5)
[00e1dc48] c27c 001f                 and.w     #$001F,d1
[00e1dc4c] ea48                      lsr.w     #5,d0
[00e1dc4e] 4840                      swap      d0
[00e1dc50] 3001                      move.w    d1,d0
[00e1dc52] c3fc 0018                 muls.w    #$0018,d1
[00e1dc56] 74ff                      moveq.l   #-1,d2
[00e1dc58] 2982 1802                 move.l    d2,2(a4,d1.l)
[00e1dc5c] 4874 1810                 pea.l     16(a4,d1.l)
[00e1dc60] 204c                      movea.l   a4,a0
[00e1dc62] 6100 fcb8                 bsr       $00E1D91C
[00e1dc66] 205f                      movea.l   (a7)+,a0
[00e1dc68] 4290                      clr.l     (a0)
[00e1dc6a] 2179 0000 94be 0004       move.l    $000094BE,4(a0)
[00e1dc72] 4e75                      rts
[00e1dc74] 48e7 5e1e                 movem.l   d1/d3-d6/a3-a6,-(a7)
[00e1dc78] 2c4a                      movea.l   a2,a6
[00e1dc7a] 3a01                      move.w    d1,d5
[00e1dc7c] 5445                      addq.w    #2,d5
[00e1dc7e] 361e                      move.w    (a6)+,d3
[00e1dc80] 264c                      movea.l   a4,a3
[00e1dc82] d6de                      adda.w    (a6)+,a3
[00e1dc84] 377c 0014 0006            move.w    #$0014,6(a3)
[00e1dc8a] 301e                      move.w    (a6)+,d0
[00e1dc8c] 61b8                      bsr.s     $00E1DC46
[00e1dc8e] 20af 002c                 move.l    44(a7),(a0)
[00e1dc92] 3185 3004                 move.w    d5,4(a0,d3.w)
[00e1dc96] 4a6f 0028                 tst.w     40(a7)
[00e1dc9a] 6700 0102                 beq       $00E1DD9E
[00e1dc9e] 7800                      moveq.l   #0,d4
[00e1dca0] 321e                      move.w    (a6)+,d1
[00e1dca2] 301e                      move.w    (a6)+,d0
[00e1dca4] 0306                      btst      d1,d6
[00e1dca6] 672e                      beq.s     $00E1DCD6
[00e1dca8] 619c                      bsr.s     $00E1DC46
[00e1dcaa] 9a70 3004                 sub.w     4(a0,d3.w),d5
[00e1dcae] 3185 3000                 move.w    d5,0(a0,d3.w)
[00e1dcb2] 5245                      addq.w    #1,d5
[00e1dcb4] 322f 002a                 move.w    42(a7),d1
[00e1dcb8] 0801 0009                 btst      #9,d1
[00e1dcbc] 6718                      beq.s     $00E1DCD6
[00e1dcbe] 3016                      move.w    (a6),d0
[00e1dcc0] 6184                      bsr.s     $00E1DC46
[00e1dcc2] 3030 3004                 move.w    4(a0,d3.w),d0
[00e1dcc6] 5640                      addq.w    #3,d0
[00e1dcc8] e248                      lsr.w     #1,d0
[00e1dcca] 3180 3004                 move.w    d0,4(a0,d3.w)
[00e1dcce] 9a40                      sub.w     d0,d5
[00e1dcd0] 3185 3000                 move.w    d5,0(a0,d3.w)
[00e1dcd4] 5245                      addq.w    #1,d5
[00e1dcd6] 548e                      addq.l    #2,a6
[00e1dcd8] 321e                      move.w    (a6)+,d1
[00e1dcda] 301e                      move.w    (a6)+,d0
[00e1dcdc] 0306                      btst      d1,d6
[00e1dcde] 674e                      beq.s     $00E1DD2E
[00e1dce0] 6100 ff64                 bsr       $00E1DC46
[00e1dce4] 9a70 3004                 sub.w     4(a0,d3.w),d5
[00e1dce8] 322f 002a                 move.w    42(a7),d1
[00e1dcec] 0801 0008                 btst      #8,d1
[00e1dcf0] 670c                      beq.s     $00E1DCFE
[00e1dcf2] 3184 3000                 move.w    d4,0(a0,d3.w)
[00e1dcf6] d870 3004                 add.w     4(a0,d3.w),d4
[00e1dcfa] 5344                      subq.w    #1,d4
[00e1dcfc] 6004                      bra.s     $00E1DD02
[00e1dcfe] 3185 3000                 move.w    d5,0(a0,d3.w)
[00e1dd02] 5245                      addq.w    #1,d5
[00e1dd04] 0801 0008                 btst      #8,d1
[00e1dd08] 6724                      beq.s     $00E1DD2E
[00e1dd0a] 0801 0009                 btst      #9,d1
[00e1dd0e] 671e                      beq.s     $00E1DD2E
[00e1dd10] 3016                      move.w    (a6),d0
[00e1dd12] 6100 ff32                 bsr       $00E1DC46
[00e1dd16] 3030 3004                 move.w    4(a0,d3.w),d0
[00e1dd1a] 5640                      addq.w    #3,d0
[00e1dd1c] e248                      lsr.w     #1,d0
[00e1dd1e] 3180 3004                 move.w    d0,4(a0,d3.w)
[00e1dd22] 3184 3000                 move.w    d4,0(a0,d3.w)
[00e1dd26] d840                      add.w     d0,d4
[00e1dd28] 5344                      subq.w    #1,d4
[00e1dd2a] 9a40                      sub.w     d0,d5
[00e1dd2c] 5245                      addq.w    #1,d5
[00e1dd2e] 548e                      addq.l    #2,a6
[00e1dd30] 321e                      move.w    (a6)+,d1
[00e1dd32] 0306                      btst      d1,d6
[00e1dd34] 6768                      beq.s     $00E1DD9E
[00e1dd36] 377c 0019 0006            move.w    #$0019,6(a3)
[00e1dd3c] 301e                      move.w    (a6)+,d0
[00e1dd3e] 6100 ff06                 bsr       $00E1DC46
[00e1dd42] 3184 3000                 move.w    d4,0(a0,d3.w)
[00e1dd46] 3185 3004                 move.w    d5,4(a0,d3.w)
[00e1dd4a] 301e                      move.w    (a6)+,d0
[00e1dd4c] 6100 fef8                 bsr       $00E1DC46
[00e1dd50] 3830 3004                 move.w    4(a0,d3.w),d4
[00e1dd54] 343c 03e8                 move.w    #$03E8,d2
[00e1dd58] 3235 303e                 move.w    62(a5,d3.w),d1
[00e1dd5c] b27c ffff                 cmp.w     #$FFFF,d1
[00e1dd60] 6708                      beq.s     $00E1DD6A
[00e1dd62] 3005                      move.w    d5,d0
[00e1dd64] 613e                      bsr.s     $00E1DDA4
[00e1dd66] b044                      cmp.w     d4,d0
[00e1dd68] 6c02                      bge.s     $00E1DD6C
[00e1dd6a] 3004                      move.w    d4,d0
[00e1dd6c] 3780 3074                 move.w    d0,116(a3,d3.w)
[00e1dd70] 3205                      move.w    d5,d1
[00e1dd72] 9240                      sub.w     d0,d1
[00e1dd74] 3035 303a                 move.w    58(a5,d3.w),d0
[00e1dd78] 612a                      bsr.s     $00E1DDA4
[00e1dd7a] 3780 3070                 move.w    d0,112(a3,d3.w)
[00e1dd7e] 7219                      moveq.l   #25,d1
[00e1dd80] 3033 305c                 move.w    92(a3,d3.w),d0
[00e1dd84] 9073 3074                 sub.w     116(a3,d3.w),d0
[00e1dd88] 670c                      beq.s     $00E1DD96
[00e1dd8a] 7207                      moveq.l   #7,d1
[00e1dd8c] 6100 fa42                 bsr       $00E1D7D0
[00e1dd90] 57c0                      seq       d0
[00e1dd92] 5400                      addq.b    #2,d0
[00e1dd94] 7214                      moveq.l   #20,d1
[00e1dd96] 1740 0055                 move.b    d0,85(a3)
[00e1dd9a] 3741 004e                 move.w    d1,78(a3)
[00e1dd9e] 4cdf 787a                 movem.l   (a7)+,d1/d3-d6/a3-a6
[00e1dda2] 4e75                      rts
[00e1dda4] 4a42                      tst.w     d2
[00e1dda6] 6712                      beq.s     $00E1DDBA
[00e1dda8] d040                      add.w     d0,d0
[00e1ddaa] c1c1                      muls.w    d1,d0
[00e1ddac] 81c2                      divs.w    d2,d0
[00e1ddae] 6b04                      bmi.s     $00E1DDB4
[00e1ddb0] 5240                      addq.w    #1,d0
[00e1ddb2] 6002                      bra.s     $00E1DDB6
[00e1ddb4] 5340                      subq.w    #1,d0
[00e1ddb6] e240                      asr.w     #1,d0
[00e1ddb8] 4e75                      rts
[00e1ddba] 7000                      moveq.l   #0,d0
[00e1ddbc] 4e75                      rts
[00e1ddbe] 206f 0004                 movea.l   4(a7),a0
[00e1ddc2] 5468 0004                 addq.w    #2,4(a0)
[00e1ddc6] 5468 0006                 addq.w    #2,6(a0)
[00e1ddca] 7000                      moveq.l   #0,d0
[00e1ddcc] 3f00                      move.w    d0,-(a7)
[00e1ddce] 2f08                      move.l    a0,-(a7)
[00e1ddd0] 6100 0086                 bsr       $00E1DE58
[00e1ddd4] 205f                      movea.l   (a7)+,a0
[00e1ddd6] 321f                      move.w    (a7)+,d1
[00e1ddd8] 7000                      moveq.l   #0,d0
[00e1ddda] 3401                      move.w    d1,d2
[00e1dddc] 48e7 470c                 movem.l   d1/d5-d7/a4-a5,-(a7)
[00e1dde0] 3e00                      move.w    d0,d7
[00e1dde2] 2a48                      movea.l   a0,a5
[00e1dde4] 3c01                      move.w    d1,d6
[00e1dde6] 3a02                      move.w    d2,d5
[00e1dde8] 4a79 0000 76f0            tst.w     $000076F0
[00e1ddee] 6662                      bne.s     $00E1DE52
[00e1ddf0] 224d                      movea.l   a5,a1
[00e1ddf2] 41f9 0000 7a58            lea.l     $00007A58,a0
[00e1ddf8] 6100 fa62                 bsr       $00E1D85C
[00e1ddfc] 6100 3aec                 bsr       $00E218EA
[00e1de00] 4e71                      nop
[00e1de02] 6100 f9b6                 bsr       $00E1D7BA
[00e1de06] 2848                      movea.l   a0,a4
[00e1de08] 4a47                      tst.w     d7
[00e1de0a] 6608                      bne.s     $00E1DE14
[00e1de0c] 226c 0032                 movea.l   50(a4),a1
[00e1de10] 3e29 0030                 move.w    48(a1),d7
[00e1de14] 4a47                      tst.w     d7
[00e1de16] 6f34                      ble.s     $00E1DE4C
[00e1de18] 4a46                      tst.w     d6
[00e1de1a] 6608                      bne.s     $00E1DE24
[00e1de1c] 226c 0036                 movea.l   54(a4),a1
[00e1de20] 3c29 0030                 move.w    48(a1),d6
[00e1de24] bc45                      cmp.w     d5,d6
[00e1de26] 6710                      beq.s     $00E1DE38
[00e1de28] 204d                      movea.l   a5,a0
[00e1de2a] 3006                      move.w    d6,d0
[00e1de2c] 6142                      bsr.s     $00E1DE70
[00e1de2e] 72ff                      moveq.l   #-1,d1
[00e1de30] 204d                      movea.l   a5,a0
[00e1de32] 3006                      move.w    d6,d0
[00e1de34] 6100 00d2                 bsr       $00E1DF08
[00e1de38] be46                      cmp.w     d6,d7
[00e1de3a] 6710                      beq.s     $00E1DE4C
[00e1de3c] 3006                      move.w    d6,d0
[00e1de3e] 6100 f982                 bsr       $00E1D7C2
[00e1de42] 2068 0036                 movea.l   54(a0),a0
[00e1de46] 3c28 0030                 move.w    48(a0),d6
[00e1de4a] 60d8                      bra.s     $00E1DE24
[00e1de4c] 6100 3ab8                 bsr       $00E21906
[00e1de50] 4e71                      nop
[00e1de52] 4cdf 30e2                 movem.l   (a7)+,d1/d5-d7/a4-a5
[00e1de56] 4e75                      rts
[00e1de58] 226f 0004                 movea.l   4(a7),a1
[00e1de5c] 3239 0000 7a8e            move.w    $00007A8E,d1
[00e1de62] 2079 0000 770c            movea.l   $0000770C,a0
[00e1de68] 7000                      moveq.l   #0,d0
[00e1de6a] 4267                      clr.w     -(a7)
[00e1de6c] 600c                      bra.s     $00E1DE7A
[00e1de6e] 91c8                      suba.l    a0,a0
[00e1de70] 7200                      moveq.l   #0,d1
[00e1de72] 2248                      movea.l   a0,a1
[00e1de74] 91c8                      suba.l    a0,a0
[00e1de76] 3f3c ffff                 move.w    #$FFFF,-(a7)
[00e1de7a] 74ff                      moveq.l   #-1,d2
[00e1de7c] 6104                      bsr.s     $00E1DE82
[00e1de7e] 548f                      addq.l    #2,a7
[00e1de80] 4e75                      rts
[00e1de82] 4e56 fff0                 link      a6,#$FFF0
[00e1de86] 48e7 4f1c                 movem.l   d1/d4-d7/a3-a5,-(a7)
[00e1de8a] 4a79 0000 76f0            tst.w     $000076F0
[00e1de90] 666e                      bne.s     $00E1DF00
[00e1de92] 3e00                      move.w    d0,d7
[00e1de94] 6b6a                      bmi.s     $00E1DF00
[00e1de96] 3c01                      move.w    d1,d6
[00e1de98] 2a08                      move.l    a0,d5
[00e1de9a] 2849                      movea.l   a1,a4
[00e1de9c] 2642                      movea.l   d2,a3
[00e1de9e] 382e 0008                 move.w    8(a6),d4
[00e1dea2] 4bee fff0                 lea.l     -16(a6),a5
[00e1dea6] 72fd                      moveq.l   #-3,d1
[00e1dea8] 6100 f9c2                 bsr       $00E1D86C
[00e1deac] 6752                      beq.s     $00E1DF00
[00e1deae] 49ee fff8                 lea.l     -8(a6),a4
[00e1deb2] 6048                      bra.s     $00E1DEFC
[00e1deb4] 224b                      movea.l   a3,a1
[00e1deb6] 2659                      movea.l   (a1)+,a3
[00e1deb8] 204c                      movea.l   a4,a0
[00e1deba] 20d9                      move.l    (a1)+,(a0)+
[00e1debc] 2091                      move.l    (a1),(a0)
[00e1debe] 224c                      movea.l   a4,a1
[00e1dec0] 204d                      movea.l   a5,a0
[00e1dec2] 6100 f998                 bsr       $00E1D85C
[00e1dec6] 6734                      beq.s     $00E1DEFC
[00e1dec8] 2205                      move.l    d5,d1
[00e1deca] 6606                      bne.s     $00E1DED2
[00e1decc] 6100 fb50                 bsr       $00E1DA1E
[00e1ded0] 2a08                      move.l    a0,d5
[00e1ded2] 4a44                      tst.w     d4
[00e1ded4] 6712                      beq.s     $00E1DEE8
[00e1ded6] 7800                      moveq.l   #0,d4
[00e1ded8] 720b                      moveq.l   #11,d1
[00e1deda] 6100 f8f4                 bsr       $00E1D7D0
[00e1dede] 6608                      bne.s     $00E1DEE8
[00e1dee0] 99cc                      suba.l    a4,a4
[00e1dee2] 70ff                      moveq.l   #-1,d0
[00e1dee4] 2640                      movea.l   d0,a3
[00e1dee6] 60be                      bra.s     $00E1DEA6
[00e1dee8] 204c                      movea.l   a4,a0
[00e1deea] 6100 fa4c                 bsr       $00E1D938
[00e1deee] 3f3c 0008                 move.w    #$0008,-(a7)
[00e1def2] 3f06                      move.w    d6,-(a7)
[00e1def4] 2f05                      move.l    d5,-(a7)
[00e1def6] 6100 2154                 bsr       $00E2004C
[00e1defa] 508f                      addq.l    #8,a7
[00e1defc] 200b                      move.l    a3,d0
[00e1defe] 66b4                      bne.s     $00E1DEB4
[00e1df00] 4cdf 38f2                 movem.l   (a7)+,d1/d4-d7/a3-a5
[00e1df04] 4e5e                      unlk      a6
[00e1df06] 4e75                      rts
[00e1df08] 4e56 ffe8                 link      a6,#$FFE8
[00e1df0c] 48e7 011c                 movem.l   d7/a3-a5,-(a7)
[00e1df10] 3e00                      move.w    d0,d7
[00e1df12] 2641                      movea.l   d1,a3
[00e1df14] 4bee fff8                 lea.l     -8(a6),a5
[00e1df18] 2848                      movea.l   a0,a4
[00e1df1a] 7201                      moveq.l   #1,d1
[00e1df1c] 6100 f94e                 bsr       $00E1D86C
[00e1df20] 6724                      beq.s     $00E1DF46
[00e1df22] 601e                      bra.s     $00E1DF42
[00e1df24] 204b                      movea.l   a3,a0
[00e1df26] 2658                      movea.l   (a0)+,a3
[00e1df28] 43ee fff0                 lea.l     -16(a6),a1
[00e1df2c] 22d8                      move.l    (a0)+,(a1)+
[00e1df2e] 22d8                      move.l    (a0)+,(a1)+
[00e1df30] 5189                      subq.l    #8,a1
[00e1df32] 204d                      movea.l   a5,a0
[00e1df34] 6100 f926                 bsr       $00E1D85C
[00e1df38] 6708                      beq.s     $00E1DF42
[00e1df3a] 41ee fff0                 lea.l     -16(a6),a0
[00e1df3e] 3007                      move.w    d7,d0
[00e1df40] 610c                      bsr.s     $00E1DF4E
[00e1df42] 200b                      move.l    a3,d0
[00e1df44] 66de                      bne.s     $00E1DF24
[00e1df46] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[00e1df4a] 4e5e                      unlk      a6
[00e1df4c] 4e75                      rts
[00e1df4e] 2f28 0004                 move.l    4(a0),-(a7)
[00e1df52] 2f10                      move.l    (a0),-(a7)
[00e1df54] 3f00                      move.w    d0,-(a7)
[00e1df56] 6100 f86a                 bsr       $00E1D7C2
[00e1df5a] 2068 0002                 movea.l   2(a0),a0
[00e1df5e] 3f28 001c                 move.w    28(a0),-(a7)
[00e1df62] 3f3c 0014                 move.w    #$0014,-(a7)
[00e1df66] 2f3c 0000 7aa4            move.l    #$00007AA4,-(a7)
[00e1df6c] 6100 f80a                 bsr       $00E1D778
[00e1df70] defc 0012                 adda.w    #$0012,a7
[00e1df74] 4e75                      rts
[00e1df76] 6000 1406                 bra       $00E1F37E
[00e1df7a] 6000 13e8                 bra       $00E1F364
[00e1df7e] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e1df82] 3e00                      move.w    d0,d7
[00e1df84] 2648                      movea.l   a0,a3
[00e1df86] 284a                      movea.l   a2,a4
[00e1df88] 4bf9 0000 7a58            lea.l     $00007A58,a5
[00e1df8e] 7204                      moveq.l   #4,d1
[00e1df90] 6100 f84e                 bsr       $00E1D7E0
[00e1df94] 6638                      bne.s     $00E1DFCE
[00e1df96] be79 0000 7b8c            cmp.w     $00007B8C,d7
[00e1df9c] 662c                      bne.s     $00E1DFCA
[00e1df9e] 2011                      move.l    (a1),d0
[00e1dfa0] 2229 0004                 move.l    4(a1),d1
[00e1dfa4] 2415                      move.l    (a5),d2
[00e1dfa6] d240                      add.w     d0,d1
[00e1dfa8] d46d 0006                 add.w     6(a5),d2
[00e1dfac] 906b 0002                 sub.w     2(a3),d0
[00e1dfb0] 6d04                      blt.s     $00E1DFB6
[00e1dfb2] b242                      cmp.w     d2,d1
[00e1dfb4] 6e14                      bgt.s     $00E1DFCA
[00e1dfb6] 4840                      swap      d0
[00e1dfb8] 4841                      swap      d1
[00e1dfba] 4842                      swap      d2
[00e1dfbc] d240                      add.w     d0,d1
[00e1dfbe] d46d 0004                 add.w     4(a5),d2
[00e1dfc2] 9053                      sub.w     (a3),d0
[00e1dfc4] 6d08                      blt.s     $00E1DFCE
[00e1dfc6] b242                      cmp.w     d2,d1
[00e1dfc8] 6f04                      ble.s     $00E1DFCE
[00e1dfca] 7003                      moveq.l   #3,d0
[00e1dfcc] 6018                      bra.s     $00E1DFE6
[00e1dfce] 3007                      move.w    d7,d0
[00e1dfd0] 321b                      move.w    (a3)+,d1
[00e1dfd2] 9259                      sub.w     (a1)+,d1
[00e1dfd4] 341b                      move.w    (a3)+,d2
[00e1dfd6] 9459                      sub.w     (a1)+,d2
[00e1dfd8] 204d                      movea.l   a5,a0
[00e1dfda] 224c                      movea.l   a4,a1
[00e1dfdc] 95ca                      suba.l    a2,a2
[00e1dfde] 4257                      clr.w     (a7)
[00e1dfe0] 6100 000a                 bsr.w     $00E1DFEC
[00e1dfe4] 7000                      moveq.l   #0,d0
[00e1dfe6] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e1dfea] 4e75                      rts
[00e1dfec] 4e56 ffe4                 link      a6,#$FFE4
[00e1dff0] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e1dff4] 3e00                      move.w    d0,d7
[00e1dff6] 3c01                      move.w    d1,d6
[00e1dff8] 3a02                      move.w    d2,d5
[00e1dffa] 2849                      movea.l   a1,a4
[00e1dffc] 2d4a ffec                 move.l    a2,-20(a6)
[00e1e000] 47ee ffe4                 lea.l     -28(a6),a3
[00e1e004] 224b                      movea.l   a3,a1
[00e1e006] 22d8                      move.l    (a0)+,(a1)+
[00e1e008] 22d8                      move.l    (a0)+,(a1)+
[00e1e00a] 224b                      movea.l   a3,a1
[00e1e00c] 41f9 0000 7a58            lea.l     $00007A58,a0
[00e1e012] 6100 f848                 bsr       $00E1D85C
[00e1e016] 3007                      move.w    d7,d0
[00e1e018] 6100 f7a8                 bsr       $00E1D7C2
[00e1e01c] 2268 009e                 movea.l   158(a0),a1
[00e1e020] 41ee fff0                 lea.l     -16(a6),a0
[00e1e024] 6100 15be                 bsr       $00E1F5E4
[00e1e028] 4bee fff4                 lea.l     -12(a6),a5
[00e1e02c] 4295                      clr.l     (a5)
[00e1e02e] 602e                      bra.s     $00E1E05E
[00e1e030] 43ee fff8                 lea.l     -8(a6),a1
[00e1e034] 41ec 0004                 lea.l     4(a4),a0
[00e1e038] 22d8                      move.l    (a0)+,(a1)+
[00e1e03a] 22d8                      move.l    (a0)+,(a1)+
[00e1e03c] 5189                      subq.l    #8,a1
[00e1e03e] 204b                      movea.l   a3,a0
[00e1e040] 6100 f81a                 bsr       $00E1D85C
[00e1e044] 6716                      beq.s     $00E1E05C
[00e1e046] 45ee fff8                 lea.l     -8(a6),a2
[00e1e04a] db6a 0002                 add.w     d5,2(a2)
[00e1e04e] dd52                      add.w     d6,(a2)
[00e1e050] 224d                      movea.l   a5,a1
[00e1e052] 41ee fff0                 lea.l     -16(a6),a0
[00e1e056] 6100 136e                 bsr       $00E1F3C6
[00e1e05a] 2a40                      movea.l   d0,a5
[00e1e05c] 2854                      movea.l   (a4),a4
[00e1e05e] 200c                      move.l    a4,d0
[00e1e060] 66ce                      bne.s     $00E1E030
[00e1e062] 4bee fff4                 lea.l     -12(a6),a5
[00e1e066] 204d                      movea.l   a5,a0
[00e1e068] 6100 ff0c                 bsr       $00E1DF76
[00e1e06c] 3205                      move.w    d5,d1
[00e1e06e] 3006                      move.w    d6,d0
[00e1e070] 204d                      movea.l   a5,a0
[00e1e072] 6100 151c                 bsr       $00E1F590
[00e1e076] 204b                      movea.l   a3,a0
[00e1e078] 6100 f8be                 bsr       $00E1D938
[00e1e07c] 2855                      movea.l   (a5),a4
[00e1e07e] 6024                      bra.s     $00E1E0A4
[00e1e080] 2f2c 0008                 move.l    8(a4),-(a7)
[00e1e084] 202c 0004                 move.l    4(a4),d0
[00e1e088] 2f00                      move.l    d0,-(a7)
[00e1e08a] 9045                      sub.w     d5,d0
[00e1e08c] 4840                      swap      d0
[00e1e08e] 9046                      sub.w     d6,d0
[00e1e090] 4840                      swap      d0
[00e1e092] 2f00                      move.l    d0,-(a7)
[00e1e094] 3f3c 0003                 move.w    #$0003,-(a7)
[00e1e098] 6100 2b7c                 bsr       $00E20C16
[00e1e09c] 4e71                      nop
[00e1e09e] defc 000e                 adda.w    #$000E,a7
[00e1e0a2] 2854                      movea.l   (a4),a4
[00e1e0a4] 200c                      move.l    a4,d0
[00e1e0a6] 66d8                      bne.s     $00E1E080
[00e1e0a8] 204d                      movea.l   a5,a0
[00e1e0aa] 6100 fece                 bsr       $00E1DF7A
[00e1e0ae] 4bee fff0                 lea.l     -16(a6),a5
[00e1e0b2] 204d                      movea.l   a5,a0
[00e1e0b4] 6100 fec0                 bsr       $00E1DF76
[00e1e0b8] 4257                      clr.w     (a7)
[00e1e0ba] 4aae ffec                 tst.l     -20(a6)
[00e1e0be] 660c                      bne.s     $00E1E0CC
[00e1e0c0] 5257                      addq.w    #1,(a7)
[00e1e0c2] 2215                      move.l    (a5),d1
[00e1e0c4] 91c8                      suba.l    a0,a0
[00e1e0c6] 3007                      move.w    d7,d0
[00e1e0c8] 6100 fe3e                 bsr       $00E1DF08
[00e1e0cc] 2415                      move.l    (a5),d2
[00e1e0ce] 224b                      movea.l   a3,a1
[00e1e0d0] 206e ffec                 movea.l   -20(a6),a0
[00e1e0d4] 322e 0008                 move.w    8(a6),d1
[00e1e0d8] 3007                      move.w    d7,d0
[00e1e0da] 6100 fda6                 bsr       $00E1DE82
[00e1e0de] 204d                      movea.l   a5,a0
[00e1e0e0] 6100 fe98                 bsr       $00E1DF7A
[00e1e0e4] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e1e0e8] 4e5e                      unlk      a6
[00e1e0ea] 4e75                      rts
[00e1e0ec] 205f                      movea.l   (a7)+,a0
[00e1e0ee] 486e fff4                 pea.l     -12(a6)
[00e1e0f2] 2f08                      move.l    a0,-(a7)
[00e1e0f4] 4a39 0000 94c6            tst.b     $000094C6
[00e1e0fa] 6618                      bne.s     $00E1E114
[00e1e0fc] 3039 0000 7b8c            move.w    $00007B8C,d0
[00e1e102] 6100 f6be                 bsr       $00E1D7C2
[00e1e106] 4868 0018                 pea.l     24(a0)
[00e1e10a] 2f28 0002                 move.l    2(a0),-(a7)
[00e1e10e] 6100 9b04                 bsr       $00E17C14
[00e1e112] 508f                      addq.l    #8,a7
[00e1e114] 4e75                      rts
[00e1e116] 2ad8                      move.l    (a0)+,(a5)+
[00e1e118] 2ad8                      move.l    (a0)+,(a5)+
[00e1e11a] 518d                      subq.l    #8,a5
[00e1e11c] 3007                      move.w    d7,d0
[00e1e11e] 6100 f6a2                 bsr       $00E1D7C2
[00e1e122] 2648                      movea.l   a0,a3
[00e1e124] 720b                      moveq.l   #11,d1
[00e1e126] 6100 f6b8                 bsr       $00E1D7E0
[00e1e12a] 671c                      beq.s     $00E1E148
[00e1e12c] 204d                      movea.l   a5,a0
[00e1e12e] 3039 0000 7a5a            move.w    $00007A5A,d0
[00e1e134] 610a                      bsr.s     $00E1E140
[00e1e136] 202b 00a6                 move.l    166(a3),d0
[00e1e13a] 6100 0002                 bsr.w     $00E1E13E
[00e1e13e] 4840                      swap      d0
[00e1e140] 5488                      addq.l    #2,a0
[00e1e142] b050                      cmp.w     (a0),d0
[00e1e144] 6f02                      ble.s     $00E1E148
[00e1e146] 3080                      move.w    d0,(a0)
[00e1e148] 4e75                      rts
[00e1e14a] 4e56 ffec                 link      a6,#$FFEC
[00e1e14e] 48e7 5f1c                 movem.l   d1/d3-d7/a3-a5,-(a7)
[00e1e152] 204d                      movea.l   a5,a0
[00e1e154] 4bee fff0                 lea.l     -16(a6),a5
[00e1e158] 61bc                      bsr.s     $00E1E116
[00e1e15a] 49ee fff8                 lea.l     -8(a6),a4
[00e1e15e] 204c                      movea.l   a4,a0
[00e1e160] 3007                      move.w    d7,d0
[00e1e162] 6100 f69c                 bsr       $00E1D800
[00e1e166] 2015                      move.l    (a5),d0
[00e1e168] b094                      cmp.l     (a4),d0
[00e1e16a] 56c4                      sne       d4
[00e1e16c] 202d 0004                 move.l    4(a5),d0
[00e1e170] b0ac 0004                 cmp.l     4(a4),d0
[00e1e174] 56c3                      sne       d3
[00e1e176] 6606                      bne.s     $00E1E17E
[00e1e178] 4a04                      tst.b     d4
[00e1e17a] 6700 0098                 beq       $00E1E214
[00e1e17e] 3007                      move.w    d7,d0
[00e1e180] 6100 f884                 bsr       $00E1DA06
[00e1e184] 2155 0010                 move.l    (a5),16(a0)
[00e1e188] 6100 0fca                 bsr       $00E1F154
[00e1e18c] 204d                      movea.l   a5,a0
[00e1e18e] 3007                      move.w    d7,d0
[00e1e190] 6100 015e                 bsr       $00E1E2F0
[00e1e194] 41eb 009e                 lea.l     158(a3),a0
[00e1e198] 47ee ffec                 lea.l     -20(a6),a3
[00e1e19c] 2690                      move.l    (a0),(a3)
[00e1e19e] 4290                      clr.l     (a0)
[00e1e1a0] 6100 0182                 bsr       $00E1E324
[00e1e1a4] 6668                      bne.s     $00E1E20E
[00e1e1a6] 546d 0004                 addq.w    #2,4(a5)
[00e1e1aa] 546d 0006                 addq.w    #2,6(a5)
[00e1e1ae] 546c 0004                 addq.w    #2,4(a4)
[00e1e1b2] 546c 0006                 addq.w    #2,6(a4)
[00e1e1b6] 4a03                      tst.b     d3
[00e1e1b8] 6610                      bne.s     $00E1E1CA
[00e1e1ba] 2453                      movea.l   (a3),a2
[00e1e1bc] 224c                      movea.l   a4,a1
[00e1e1be] 204d                      movea.l   a5,a0
[00e1e1c0] 3007                      move.w    d7,d0
[00e1e1c2] 6100 fdba                 bsr       $00E1DF7E
[00e1e1c6] 3c00                      move.w    d0,d6
[00e1e1c8] 6024                      bra.s     $00E1E1EE
[00e1e1ca] 7c03                      moveq.l   #3,d6
[00e1e1cc] 4a04                      tst.b     d4
[00e1e1ce] 661e                      bne.s     $00E1E1EE
[00e1e1d0] 302d 0004                 move.w    4(a5),d0
[00e1e1d4] 906c 0004                 sub.w     4(a4),d0
[00e1e1d8] 322d 0006                 move.w    6(a5),d1
[00e1e1dc] 926c 0006                 sub.w     6(a4),d1
[00e1e1e0] 6e08                      bgt.s     $00E1E1EA
[00e1e1e2] 4a40                      tst.w     d0
[00e1e1e4] 6e04                      bgt.s     $00E1E1EA
[00e1e1e6] 7c01                      moveq.l   #1,d6
[00e1e1e8] 6004                      bra.s     $00E1E1EE
[00e1e1ea] 6100 0dd6                 bsr       $00E1EFC2
[00e1e1ee] e24e                      lsr.w     #1,d6
[00e1e1f0] 6408                      bcc.s     $00E1E1FA
[00e1e1f2] 204d                      movea.l   a5,a0
[00e1e1f4] 3007                      move.w    d7,d0
[00e1e1f6] 6100 fc78                 bsr       $00E1DE70
[00e1e1fa] e24e                      lsr.w     #1,d6
[00e1e1fc] 6408                      bcc.s     $00E1E206
[00e1e1fe] 204d                      movea.l   a5,a0
[00e1e200] 3007                      move.w    d7,d0
[00e1e202] 6100 00e2                 bsr       $00E1E2E6
[00e1e206] 204c                      movea.l   a4,a0
[00e1e208] 3007                      move.w    d7,d0
[00e1e20a] 6100 fbc0                 bsr       $00E1DDCC
[00e1e20e] 204b                      movea.l   a3,a0
[00e1e210] 6100 fd68                 bsr       $00E1DF7A
[00e1e214] 4cdf 38fa                 movem.l   (a7)+,d1/d3-d7/a3-a5
[00e1e218] 4e5e                      unlk      a6
[00e1e21a] 4e75                      rts
[00e1e21c] 4e56 fff0                 link      a6,#$FFF0
[00e1e220] 48e7 1f04                 movem.l   d3-d7/a5,-(a7)
[00e1e224] 6100 f594                 bsr       $00E1D7BA
[00e1e228] 3801                      move.w    d1,d4
[00e1e22a] 4441                      neg.w     d1
[00e1e22c] e549                      lsl.w     #2,d1
[00e1e22e] 2070 1032                 movea.l   50(a0,d1.w),a0
[00e1e232] be68 0030                 cmp.w     48(a0),d7
[00e1e236] 6700 008e                 beq       $00E1E2C6
[00e1e23a] 3007                      move.w    d7,d0
[00e1e23c] 6100 f67c                 bsr       $00E1D8BA
[00e1e240] 3600                      move.w    d0,d3
[00e1e242] 3204                      move.w    d4,d1
[00e1e244] 3007                      move.w    d7,d0
[00e1e246] 6100 f6be                 bsr       $00E1D906
[00e1e24a] 4bee fff8                 lea.l     -8(a6),a5
[00e1e24e] 204d                      movea.l   a5,a0
[00e1e250] 72fd                      moveq.l   #-3,d1
[00e1e252] 3007                      move.w    d7,d0
[00e1e254] 6100 f5ac                 bsr       $00E1D802
[00e1e258] 3004                      move.w    d4,d0
[00e1e25a] 6100 00ca                 bsr       $00E1E326
[00e1e25e] 6666                      bne.s     $00E1E2C6
[00e1e260] ba46                      cmp.w     d6,d5
[00e1e262] 6754                      beq.s     $00E1E2B8
[00e1e264] 3006                      move.w    d6,d0
[00e1e266] 6100 fc06                 bsr       $00E1DE6E
[00e1e26a] 3005                      move.w    d5,d0
[00e1e26c] 6100 fc00                 bsr       $00E1DE6E
[00e1e270] 3005                      move.w    d5,d0
[00e1e272] 6100 f54e                 bsr       $00E1D7C2
[00e1e276] 0828 0002 0001            btst      #2,1(a0)
[00e1e27c] 6706                      beq.s     $00E1E284
[00e1e27e] 6100 0058                 bsr.w     $00E1E2D8
[00e1e282] 6606                      bne.s     $00E1E28A
[00e1e284] 4a44                      tst.w     d4
[00e1e286] 663e                      bne.s     $00E1E2C6
[00e1e288] 602c                      bra.s     $00E1E2B6
[00e1e28a] 4a44                      tst.w     d4
[00e1e28c] 6720                      beq.s     $00E1E2AE
[00e1e28e] 41ee fff0                 lea.l     -16(a6),a0
[00e1e292] 72fd                      moveq.l   #-3,d1
[00e1e294] 3003                      move.w    d3,d0
[00e1e296] 6100 f56a                 bsr       $00E1D802
[00e1e29a] 3005                      move.w    d5,d0
[00e1e29c] 6100 0048                 bsr.w     $00E1E2E6
[00e1e2a0] 3003                      move.w    d3,d0
[00e1e2a2] 6100 f616                 bsr       $00E1D8BA
[00e1e2a6] 3600                      move.w    d0,d3
[00e1e2a8] ba43                      cmp.w     d3,d5
[00e1e2aa] 66e2                      bne.s     $00E1E28E
[00e1e2ac] 6018                      bra.s     $00E1E2C6
[00e1e2ae] 204d                      movea.l   a5,a0
[00e1e2b0] 3005                      move.w    d5,d0
[00e1e2b2] 6100 0032                 bsr.w     $00E1E2E6
[00e1e2b6] 3605                      move.w    d5,d3
[00e1e2b8] 3007                      move.w    d7,d0
[00e1e2ba] 6100 f5fe                 bsr       $00E1D8BA
[00e1e2be] 204d                      movea.l   a5,a0
[00e1e2c0] 3203                      move.w    d3,d1
[00e1e2c2] 6100 fb16                 bsr       $00E1DDDA
[00e1e2c6] 4cdf 20f8                 movem.l   (a7)+,d3-d7/a5
[00e1e2ca] 4e5e                      unlk      a6
[00e1e2cc] 4e75                      rts
[00e1e2ce] 3f00                      move.w    d0,-(a7)
[00e1e2d0] 6100 fb9c                 bsr       $00E1DE6E
[00e1e2d4] 301f                      move.w    (a7)+,d0
[00e1e2d6] 600c                      bra.s     $00E1E2E4
[00e1e2d8] 7203                      moveq.l   #3,d1
[00e1e2da] 3005                      move.w    d5,d0
[00e1e2dc] 6100 f504                 bsr       $00E1D7E2
[00e1e2e0] 660c                      bne.s     $00E1E2EE
[00e1e2e2] 3005                      move.w    d5,d0
[00e1e2e4] 91c8                      suba.l    a0,a0
[00e1e2e6] 72ff                      moveq.l   #-1,d1
[00e1e2e8] 6100 fc1e                 bsr       $00E1DF08
[00e1e2ec] 7000                      moveq.l   #0,d0
[00e1e2ee] 4e75                      rts
[00e1e2f0] 2248                      movea.l   a0,a1
[00e1e2f2] 6100 f4ce                 bsr       $00E1D7C2
[00e1e2f6] 45e8 0030                 lea.l     48(a0),a2
[00e1e2fa] 2162 0024                 move.l    -(a2),36(a0)
[00e1e2fe] 2162 0020                 move.l    -(a2),32(a0)
[00e1e302] 24d9                      move.l    (a1)+,(a2)+
[00e1e304] 24d9                      move.l    (a1)+,(a2)+
[00e1e306] 4868 0018                 pea.l     24(a0)
[00e1e30a] 2f21                      move.l    -(a1),-(a7)
[00e1e30c] 2f21                      move.l    -(a1),-(a7)
[00e1e30e] 3f28 0006                 move.w    6(a0),-(a7)
[00e1e312] 3f3c 0001                 move.w    #$0001,-(a7)
[00e1e316] 2068 0002                 movea.l   2(a0),a0
[00e1e31a] 6100 08d6                 bsr       $00E1EBF2
[00e1e31e] defc 0010                 adda.w    #$0010,a7
[00e1e322] 4e75                      rts
[00e1e324] 7000                      moveq.l   #0,d0
[00e1e326] 6100 f492                 bsr       $00E1D7BA
[00e1e32a] 2068 0036                 movea.l   54(a0),a0
[00e1e32e] 3a28 0030                 move.w    48(a0),d5
[00e1e332] 43f9 0000 7b8c            lea.l     $00007B8C,a1
[00e1e338] 3c11                      move.w    (a1),d6
[00e1e33a] 3285                      move.w    d5,(a1)
[00e1e33c] 3f00                      move.w    d0,-(a7)
[00e1e33e] 6702                      beq.s     $00E1E342
[00e1e340] 3005                      move.w    d5,d0
[00e1e342] 6122                      bsr.s     $00E1E366
[00e1e344] 6100 fdae                 bsr       $00E1E0F4
[00e1e348] 301f                      move.w    (a7)+,d0
[00e1e34a] 612e                      bsr.s     $00E1E37A
[00e1e34c] 4a79 0000 76f0            tst.w     $000076F0
[00e1e352] 4e75                      rts
[00e1e354] 23c0 0000 8c68            move.l    d0,$00008C68
[00e1e35a] 4eb9 00e2 0c84            jsr       $00E20C84
[00e1e360] 6100 f4c4                 bsr       $00E1D826
[00e1e364] 7000                      moveq.l   #0,d0
[00e1e366] 6108                      bsr.s     $00E1E370
[00e1e368] 6100 f550                 bsr       $00E1D8BA
[00e1e36c] 6ef8                      bgt.s     $00E1E366
[00e1e36e] 4e75                      rts
[00e1e370] 3f00                      move.w    d0,-(a7)
[00e1e372] 6100 1140                 bsr       $00E1F4B4
[00e1e376] 301f                      move.w    (a7)+,d0
[00e1e378] 4e75                      rts
[00e1e37a] bc45                      cmp.w     d5,d6
[00e1e37c] 673c                      beq.s     $00E1E3BA
[00e1e37e] 4a00                      tst.b     d0
[00e1e380] 6706                      beq.s     $00E1E388
[00e1e382] 721e                      moveq.l   #30,d1
[00e1e384] 3006                      move.w    d6,d0
[00e1e386] 6004                      bra.s     $00E1E38C
[00e1e388] 721f                      moveq.l   #31,d1
[00e1e38a] 3005                      move.w    d5,d0
[00e1e38c] 6f2c                      ble.s     $00E1E3BA
[00e1e38e] 6100 f432                 bsr       $00E1D7C2
[00e1e392] 2068 0002                 movea.l   2(a0),a0
[00e1e396] 9efc 0010                 suba.w    #$0010,a7
[00e1e39a] 224f                      movea.l   a7,a1
[00e1e39c] 42a7                      clr.l     -(a7)
[00e1e39e] 42a7                      clr.l     -(a7)
[00e1e3a0] 3f00                      move.w    d0,-(a7)
[00e1e3a2] 3f28 001c                 move.w    28(a0),-(a7)
[00e1e3a6] 3f01                      move.w    d1,-(a7)
[00e1e3a8] 2f09                      move.l    a1,-(a7)
[00e1e3aa] 7202                      moveq.l   #2,d1
[00e1e3ac] 6100 f434                 bsr       $00E1D7E2
[00e1e3b0] 6704                      beq.s     $00E1E3B6
[00e1e3b2] 6100 f3c4                 bsr       $00E1D778
[00e1e3b6] defc 0022                 adda.w    #$0022,a7
[00e1e3ba] 4e75                      rts
[00e1e3bc] 48e7 041c                 movem.l   d5/a3-a5,-(a7)
[00e1e3c0] 3007                      move.w    d7,d0
[00e1e3c2] 6100 f3fe                 bsr       $00E1D7C2
[00e1e3c6] 2848                      movea.l   a0,a4
[00e1e3c8] 266c 00a2                 movea.l   162(a4),a3
[00e1e3cc] ba7c 000b                 cmp.w     #$000B,d5
[00e1e3d0] 6622                      bne.s     $00E1E3F4
[00e1e3d2] 266c 009e                 movea.l   158(a4),a3
[00e1e3d6] 601c                      bra.s     $00E1E3F4
[00e1e3d8] 43eb 0004                 lea.l     4(a3),a1
[00e1e3dc] 204d                      movea.l   a5,a0
[00e1e3de] 20d9                      move.l    (a1)+,(a0)+
[00e1e3e0] 2091                      move.l    (a1),(a0)
[00e1e3e2] 2653                      movea.l   (a3),a3
[00e1e3e4] 294b 00a2                 move.l    a3,162(a4)
[00e1e3e8] 224d                      movea.l   a5,a1
[00e1e3ea] 41ec 0018                 lea.l     24(a4),a0
[00e1e3ee] 6100 f46c                 bsr       $00E1D85C
[00e1e3f2] 6608                      bne.s     $00E1E3FC
[00e1e3f4] 200b                      move.l    a3,d0
[00e1e3f6] 66e0                      bne.s     $00E1E3D8
[00e1e3f8] 429d                      clr.l     (a5)+
[00e1e3fa] 4295                      clr.l     (a5)
[00e1e3fc] 4cdf 3820                 movem.l   (a7)+,d5/a3-a5
[00e1e400] 4e75                      rts
[00e1e402] 4e56 0000                 link      a6,#$0000
[00e1e406] 48e7 4304                 movem.l   d1/d6-d7/a5,-(a7)
[00e1e40a] 7428                      moveq.l   #40,d2
[00e1e40c] 7200                      moveq.l   #0,d1
[00e1e40e] 6100 f3c0                 bsr       $00E1D7D0
[00e1e412] 6602                      bne.s     $00E1E416
[00e1e414] 7408                      moveq.l   #8,d2
[00e1e416] 7000                      moveq.l   #0,d0
[00e1e418] 6100 f3a8                 bsr       $00E1D7C2
[00e1e41c] 2a48                      movea.l   a0,a5
[00e1e41e] 082d 0000 0001            btst      #0,1(a5)
[00e1e424] 670a                      beq.s     $00E1E430
[00e1e426] 5240                      addq.w    #1,d0
[00e1e428] b440                      cmp.w     d0,d2
[00e1e42a] 6eec                      bgt.s     $00E1E418
[00e1e42c] 70ff                      moveq.l   #-1,d0
[00e1e42e] 6024                      bra.s     $00E1E454
[00e1e430] 3e00                      move.w    d0,d7
[00e1e432] 3c2e 0008                 move.w    8(a6),d6
[00e1e436] 3006                      move.w    d6,d0
[00e1e438] 204d                      movea.l   a5,a0
[00e1e43a] 6120                      bsr.s     $00E1E45C
[00e1e43c] 3007                      move.w    d7,d0
[00e1e43e] 206e 000a                 movea.l   10(a6),a0
[00e1e442] 43f9 0000 7b5a            lea.l     $00007B5A,a1
[00e1e448] 95ca                      suba.l    a2,a2
[00e1e44a] 6100 f3ea                 bsr       $00E1D836
[00e1e44e] 6100 003e                 bsr.w     $00E1E48E
[00e1e452] 3007                      move.w    d7,d0
[00e1e454] 4cdf 20c2                 movem.l   (a7)+,d1/d6-d7/a5
[00e1e458] 4e5e                      unlk      a6
[00e1e45a] 4e75                      rts
[00e1e45c] 3140 0006                 move.w    d0,6(a0)
[00e1e460] 2179 0000 cb00 0002       move.l    $0000CB00,2(a0)
[00e1e468] 30bc 0001                 move.w    #$0001,(a0)
[00e1e46c] 42a8 003a                 clr.l     58(a0)
[00e1e470] 70ff                      moveq.l   #-1,d0
[00e1e472] 2140 003e                 move.l    d0,62(a0)
[00e1e476] 41e8 0042                 lea.l     66(a0),a0
[00e1e47a] 2279 0000 94b2            movea.l   $000094B2,a1
[00e1e480] 43e9 5558                 lea.l     21848(a1),a1
[00e1e484] 7016                      moveq.l   #22,d0
[00e1e486] 20d9                      move.l    (a1)+,(a0)+
[00e1e488] 51c8 fffc                 dbf       d0,$00E1E486
[00e1e48c] 4e75                      rts
[00e1e48e] 48e7 1f0c                 movem.l   d3-d7/a4-a5,-(a7)
[00e1e492] 2a39 0000 94be            move.l    $000094BE,d5
[00e1e498] 4845                      swap      d5
[00e1e49a] 7800                      moveq.l   #0,d4
[00e1e49c] 49fa 0062                 lea.l     $00E1E500(pc),a4
[00e1e4a0] 7400                      moveq.l   #0,d2
[00e1e4a2] 6132                      bsr.s     $00E1E4D6
[00e1e4a4] 3607                      move.w    d7,d3
[00e1e4a6] 206d 0002                 movea.l   2(a5),a0
[00e1e4aa] 6100 07b8                 bsr       $00E1EC64
[00e1e4ae] 2800                      move.l    d0,d4
[00e1e4b0] 343c 0600                 move.w    #$0600,d2
[00e1e4b4] 6120                      bsr.s     $00E1E4D6
[00e1e4b6] be43                      cmp.w     d3,d7
[00e1e4b8] 6c02                      bge.s     $00E1E4BC
[00e1e4ba] 3e03                      move.w    d3,d7
[00e1e4bc] 4847                      swap      d7
[00e1e4be] 4844                      swap      d4
[00e1e4c0] 4845                      swap      d5
[00e1e4c2] 343c 00c0                 move.w    #$00C0,d2
[00e1e4c6] 610e                      bsr.s     $00E1E4D6
[00e1e4c8] 7407                      moveq.l   #7,d2
[00e1e4ca] 612a                      bsr.s     $00E1E4F6
[00e1e4cc] 2b47 00a6                 move.l    d7,166(a5)
[00e1e4d0] 4cdf 30f8                 movem.l   (a7)+,d3-d7/a4-a5
[00e1e4d4] 4e75                      rts
[00e1e4d6] 3e3c 0002                 move.w    #$0002,d7
[00e1e4da] 4a44                      tst.w     d4
[00e1e4dc] 6704                      beq.s     $00E1E4E2
[00e1e4de] 528c                      addq.l    #1,a4
[00e1e4e0] 6118                      bsr.s     $00E1E4FA
[00e1e4e2] 101c                      move.b    (a4)+,d0
[00e1e4e4] 6706                      beq.s     $00E1E4EC
[00e1e4e6] 0106                      btst      d0,d6
[00e1e4e8] 67f8                      beq.s     $00E1E4E2
[00e1e4ea] 60f4                      bra.s     $00E1E4E0
[00e1e4ec] 7209                      moveq.l   #9,d1
[00e1e4ee] 204d                      movea.l   a5,a0
[00e1e4f0] 6100 f2f2                 bsr       $00E1D7E4
[00e1e4f4] 6708                      beq.s     $00E1E4FE
[00e1e4f6] c446                      and.w     d6,d2
[00e1e4f8] 6704                      beq.s     $00E1E4FE
[00e1e4fa] de45                      add.w     d5,d7
[00e1e4fc] 5347                      subq.w    #1,d7
[00e1e4fe] 4e75                      rts
[00e1e500] 0103                      btst      d0,d3
[00e1e502] 0302                      btst      d1,d2
[00e1e504] 0005 090b                 ori.b     #$0B,d5
[00e1e508] 0b0a 0005                 movep.w   5(a2),d5
[00e1e50c] 0406 0808                 subi.b    #$08,d6
[00e1e510] 0700                      btst      d3,d0
[00e1e512] 7465                      moveq.l   #101,d2
[00e1e514] 302f 0004                 move.w    4(a7),d0
[00e1e518] 616a                      bsr.s     $00E1E584
[00e1e51a] 6756                      beq.s     $00E1E572
[00e1e51c] 08c0 0003                 bset      #3,d0
[00e1e520] 6650                      bne.s     $00E1E572
[00e1e522] 3080                      move.w    d0,(a0)
[00e1e524] 48e7 6714                 movem.l   d1-d2/d5-d7/a3/a5,-(a7)
[00e1e528] 3e2f 0020                 move.w    32(a7),d7
[00e1e52c] 6100 0638                 bsr       $00E1EB66
[00e1e530] 3007                      move.w    d7,d0
[00e1e532] 6100 f3b2                 bsr       $00E1D8E6
[00e1e536] 206f 0022                 movea.l   34(a7),a0
[00e1e53a] 2a4f                      movea.l   a7,a5
[00e1e53c] 6100 fbd8                 bsr       $00E1E116
[00e1e540] 204d                      movea.l   a5,a0
[00e1e542] 3007                      move.w    d7,d0
[00e1e544] 6100 fdaa                 bsr       $00E1E2F0
[00e1e548] 70ff                      moveq.l   #-1,d0
[00e1e54a] 6100 fdda                 bsr       $00E1E326
[00e1e54e] 660e                      bne.s     $00E1E55E
[00e1e550] 3006                      move.w    d6,d0
[00e1e552] 6f04                      ble.s     $00E1E558
[00e1e554] 6100 f918                 bsr       $00E1DE6E
[00e1e558] 3007                      move.w    d7,d0
[00e1e55a] 6100 fd72                 bsr       $00E1E2CE
[00e1e55e] 204d                      movea.l   a5,a0
[00e1e560] 7202                      moveq.l   #2,d1
[00e1e562] 3007                      move.w    d7,d0
[00e1e564] 6100 f2e8                 bsr       $00E1D84E
[00e1e568] 6100 0600                 bsr       $00E1EB6A
[00e1e56c] 4cdf 28e6                 movem.l   (a7)+,d1-d2/d5-d7/a3/a5
[00e1e570] 4e75                      rts
[00e1e572] 6000 0aca                 bra       $00E1F03E
[00e1e576] 7467                      moveq.l   #103,d2
[00e1e578] 302f 0004                 move.w    4(a7),d0
[00e1e57c] 6106                      bsr.s     $00E1E584
[00e1e57e] 67f2                      beq.s     $00E1E572
[00e1e580] 4250                      clr.w     (a0)
[00e1e582] 4e75                      rts
[00e1e584] 4a40                      tst.w     d0
[00e1e586] 6712                      beq.s     $00E1E59A
[00e1e588] b07c 0027                 cmp.w     #$0027,d0
[00e1e58c] 620c                      bhi.s     $00E1E59A
[00e1e58e] 6100 f232                 bsr       $00E1D7C2
[00e1e592] 3010                      move.w    (a0),d0
[00e1e594] 0800 0000                 btst      #0,d0
[00e1e598] 6602                      bne.s     $00E1E59C
[00e1e59a] 7000                      moveq.l   #0,d0
[00e1e59c] 4e75                      rts
[00e1e59e] 7466                      moveq.l   #102,d2
[00e1e5a0] 302f 0004                 move.w    4(a7),d0
[00e1e5a4] 61de                      bsr.s     $00E1E584
[00e1e5a6] 67ca                      beq.s     $00E1E572
[00e1e5a8] 0880 0004                 bclr      #4,d0
[00e1e5ac] 0880 0003                 bclr      #3,d0
[00e1e5b0] 67c0                      beq.s     $00E1E572
[00e1e5b2] 3080                      move.w    d0,(a0)
[00e1e5b4] 4e56 fff8                 link      a6,#$FFF8
[00e1e5b8] 48e7 4f04                 movem.l   d1/d4-d7/a5,-(a7)
[00e1e5bc] 3e2e 0008                 move.w    8(a6),d7
[00e1e5c0] 4bee fff8                 lea.l     -8(a6),a5
[00e1e5c4] 6100 05a0                 bsr       $00E1EB66
[00e1e5c8] 3007                      move.w    d7,d0
[00e1e5ca] 6100 f2ee                 bsr       $00E1D8BA
[00e1e5ce] 3800                      move.w    d0,d4
[00e1e5d0] 204d                      movea.l   a5,a0
[00e1e5d2] 72fd                      moveq.l   #-3,d1
[00e1e5d4] 3007                      move.w    d7,d0
[00e1e5d6] 6100 f22a                 bsr       $00E1D802
[00e1e5da] 6100 f2ec                 bsr       $00E1D8C8
[00e1e5de] 3007                      move.w    d7,d0
[00e1e5e0] 6100 fd8e                 bsr       $00E1E370
[00e1e5e4] 41f9 0000 7b5a            lea.l     $00007B5A,a0
[00e1e5ea] 6100 fd04                 bsr       $00E1E2F0
[00e1e5ee] 6100 fd34                 bsr       $00E1E324
[00e1e5f2] 6624                      bne.s     $00E1E618
[00e1e5f4] 4a44                      tst.w     d4
[00e1e5f6] 6618                      bne.s     $00E1E610
[00e1e5f8] 3805                      move.w    d5,d4
[00e1e5fa] 6f14                      ble.s     $00E1E610
[00e1e5fc] 3005                      move.w    d5,d0
[00e1e5fe] 6100 f86e                 bsr       $00E1DE6E
[00e1e602] 6100 fcd4                 bsr       $00E1E2D8
[00e1e606] 6708                      beq.s     $00E1E610
[00e1e608] 204d                      movea.l   a5,a0
[00e1e60a] 3005                      move.w    d5,d0
[00e1e60c] 6100 fcd8                 bsr       $00E1E2E6
[00e1e610] 204d                      movea.l   a5,a0
[00e1e612] 3004                      move.w    d4,d0
[00e1e614] 6100 f7b6                 bsr       $00E1DDCC
[00e1e618] 6100 0550                 bsr       $00E1EB6A
[00e1e61c] 4cdf 20f2                 movem.l   (a7)+,d1/d4-d7/a5
[00e1e620] 4e5e                      unlk      a6
[00e1e622] 4e75                      rts
[00e1e624] 6100 f19c                 bsr       $00E1D7C2
[00e1e628] 4a40                      tst.w     d0
[00e1e62a] 6d2e                      blt.s     $00E1E65A
[00e1e62c] 6726                      beq.s     $00E1E654
[00e1e62e] 2f08                      move.l    a0,-(a7)
[00e1e630] 41e8 0018                 lea.l     24(a0),a0
[00e1e634] 3007                      move.w    d7,d0
[00e1e636] 4840                      swap      d0
[00e1e638] 3006                      move.w    d6,d0
[00e1e63a] 6100 f270                 bsr       $00E1D8AC
[00e1e63e] 205f                      movea.l   (a7)+,a0
[00e1e640] 4a40                      tst.w     d0
[00e1e642] 6716                      beq.s     $00E1E65A
[00e1e644] 0c6e 0002 0008            cmpi.w    #$0002,8(a6)
[00e1e64a] 6708                      beq.s     $00E1E654
[00e1e64c] 3010                      move.w    (a0),d0
[00e1e64e] 0800 0008                 btst      #8,d0
[00e1e652] 6706                      beq.s     $00E1E65A
[00e1e654] 2028 0002                 move.l    2(a0),d0
[00e1e658] 4e75                      rts
[00e1e65a] 2039 0000 cb7a            move.l    $0000CB7A,d0
[00e1e660] 4e75                      rts
[00e1e662] 43f9 00e3 5f20            lea.l     $00E35F20,a1
[00e1e668] 1231 3066                 move.b    102(a1,d3.w),d1
[00e1e66c] 67f2                      beq.s     $00E1E660
[00e1e66e] 6b06                      bmi.s     $00E1E676
[00e1e670] 6100 f15e                 bsr       $00E1D7D0
[00e1e674] 67ea                      beq.s     $00E1E660
[00e1e676] 3203                      move.w    d3,d1
[00e1e678] c3fc 0018                 muls.w    #$0018,d1
[00e1e67c] 67e2                      beq.s     $00E1E660
[00e1e67e] 41f4 180b                 lea.l     11(a4,d1.l),a0
[00e1e682] b410                      cmp.b     (a0),d2
[00e1e684] 67da                      beq.s     $00E1E660
[00e1e686] 1082                      move.b    d2,(a0)
[00e1e688] 93c9                      suba.l    a1,a1
[00e1e68a] 3203                      move.w    d3,d1
[00e1e68c] 204c                      movea.l   a4,a0
[00e1e68e] 3007                      move.w    d7,d0
[00e1e690] 6000 f7e4                 bra       $00E1DE76
[00e1e694] 45f9 00e3 5f20            lea.l     $00E35F20,a2
[00e1e69a] 43ea 0018                 lea.l     24(a2),a1
[00e1e69e] 72ff                      moveq.l   #-1,d1
[00e1e6a0] 7016                      moveq.l   #22,d0
[00e1e6a2] 204c                      movea.l   a4,a0
[00e1e6a4] 20c1                      move.l    d1,(a0)+
[00e1e6a6] 30c1                      move.w    d1,(a0)+
[00e1e6a8] 4218                      clr.b     (a0)+
[00e1e6aa] 10da                      move.b    (a2)+,(a0)+
[00e1e6ac] 4298                      clr.l     (a0)+
[00e1e6ae] 30d9                      move.w    (a1)+,(a0)+
[00e1e6b0] 4258                      clr.w     (a0)+
[00e1e6b2] 4298                      clr.l     (a0)+
[00e1e6b4] 4298                      clr.l     (a0)+
[00e1e6b6] 51c8 ffec                 dbf       d0,$00E1E6A4
[00e1e6ba] 2409                      move.l    a1,d2
[00e1e6bc] d2fc 001c                 adda.w    #$001C,a1
[00e1e6c0] 600e                      bra.s     $00E1E6D0
[00e1e6c2] 2988 080c                 move.l    a0,12(a4,d0.l)
[00e1e6c6] 2442                      movea.l   d2,a2
[00e1e6c8] 720d                      moveq.l   #13,d1
[00e1e6ca] 30da                      move.w    (a2)+,(a0)+
[00e1e6cc] 51c9 fffc                 dbf       d1,$00E1E6CA
[00e1e6d0] 1019                      move.b    (a1)+,d0
[00e1e6d2] 4880                      ext.w     d0
[00e1e6d4] c1fc 0018                 muls.w    #$0018,d0
[00e1e6d8] 6ce8                      bge.s     $00E1E6C2
[00e1e6da] 206c 0084                 movea.l   132(a4),a0
[00e1e6de] 317c 0000 0010            move.w    #$0000,16(a0)
[00e1e6e4] 397c 0020 000a            move.w    #$0020,10(a4)
[00e1e6ea] 4e75                      rts
[00e1e6ec] 23ee 0004 0000 94c2       move.l    4(a6),$000094C2
[00e1e6f4] 41fa 0110                 lea.l     $00E1E806(pc),a0
[00e1e6f8] 2d48 0004                 move.l    a0,4(a6)
[00e1e6fc] 41f9 0000 94be            lea.l     $000094BE,a0
[00e1e702] 30f9 0000 7aba            move.w    $00007ABA,(a0)+
[00e1e708] 30f9 0000 788a            move.w    $0000788A,(a0)+
[00e1e70e] 4e71                      nop
[00e1e710] 4e71                      nop
[00e1e712] 700b                      moveq.l   #11,d0
[00e1e714] 6100 f2a8                 bsr       $00E1D9BE
[00e1e718] 2040                      movea.l   d0,a0
[00e1e71a] 6604                      bne.s     $00E1E720
[00e1e71c] 41fa 0022                 lea.l     $00E1E740(pc),a0
[00e1e720] 2f08                      move.l    a0,-(a7)
[00e1e722] 3f3c 575a                 move.w    #$575A,-(a7)
[00e1e726] 4267                      clr.w     -(a7)
[00e1e728] 6100 02a4                 bsr       $00E1E9CE
[00e1e72c] 508f                      addq.l    #8,a7
[00e1e72e] 2079 0000 94b2            movea.l   $000094B2,a0
[00e1e734] 4eb9 00e3 235a            jsr       $00E3235A
[00e1e73a] 6100 f258                 bsr       $00E1D994
[00e1e73e] 600a                      bra.s     $00E1E74A
[00e1e740] 01cc 0028                 movep.l   d0,40(a4)
[00e1e744] 03e8 07d0                 bset      d1,2000(a0)
[00e1e748] 4afc                      illegal
[00e1e74a] 4e56 0000                 link      a6,#$0000
[00e1e74e] 48e7 431c                 movem.l   d1/d6-d7/a3-a5,-(a7)
[00e1e752] 2a79 0000 94b2            movea.l   $000094B2,a5
[00e1e758] 204d                      movea.l   a5,a0
[00e1e75a] 303c 2d4f                 move.w    #$2D4F,d0
[00e1e75e] 4218                      clr.b     (a0)+
[00e1e760] 51c8 fffc                 dbf       d0,$00E1E75E
[00e1e764] 95ca                      suba.l    a2,a2
[00e1e766] 204d                      movea.l   a5,a0
[00e1e768] 7000                      moveq.l   #0,d0
[00e1e76a] 3140 0030                 move.w    d0,48(a0)
[00e1e76e] 43e8 00aa                 lea.l     170(a0),a1
[00e1e772] 7209                      moveq.l   #9,d1
[00e1e774] 228a                      move.l    a2,(a1)
[00e1e776] 2449                      movea.l   a1,a2
[00e1e778] d2fc 000c                 adda.w    #$000C,a1
[00e1e77c] 51c9 fff6                 dbf       d1,$00E1E774
[00e1e780] d0fc 0122                 adda.w    #$0122,a0
[00e1e784] 5240                      addq.w    #1,d0
[00e1e786] b07c 0028                 cmp.w     #$0028,d0
[00e1e78a] 66de                      bne.s     $00E1E76A
[00e1e78c] 23ca 0000 cb08            move.l    a2,$0000CB08
[00e1e792] 7000                      moveq.l   #0,d0
[00e1e794] 204d                      movea.l   a5,a0
[00e1e796] 6100 fcc4                 bsr       $00E1E45C
[00e1e79a] 6100 f08a                 bsr       $00E1D826
[00e1e79e] 4279 0000 7b8c            clr.w     $00007B8C
[00e1e7a4] 0055 0008                 ori.w     #$0008,(a5)
[00e1e7a8] 2b4d 0032                 move.l    a5,50(a5)
[00e1e7ac] 2b4d 0036                 move.l    a5,54(a5)
[00e1e7b0] 6100 fbb2                 bsr       $00E1E364
[00e1e7b4] 47f9 0000 7a58            lea.l     $00007A58,a3
[00e1e7ba] 4279 0000 7a8e            clr.w     $00007A8E
[00e1e7c0] 2479 0000 7504            movea.l   $00007504,a2
[00e1e7c6] 23ca 0000 770c            move.l    a2,$0000770C
[00e1e7cc] 671c                      beq.s     $00E1E7EA
[00e1e7ce] 4aaa 0010                 tst.l     16(a2)
[00e1e7d2] 6610                      bne.s     $00E1E7E4
[00e1e7d4] 2013                      move.l    (a3),d0
[00e1e7d6] 3540 0012                 move.w    d0,18(a2)
[00e1e7da] 4440                      neg.w     d0
[00e1e7dc] 3540 002a                 move.w    d0,42(a2)
[00e1e7e0] 3540 0042                 move.w    d0,66(a2)
[00e1e7e4] 356b 0006 0016            move.w    6(a3),22(a2)
[00e1e7ea] 49ed 55b4                 lea.l     21940(a5),a4
[00e1e7ee] 23cc 0000 77ca            move.l    a4,$000077CA
[00e1e7f4] 6100 fe9e                 bsr       $00E1E694
[00e1e7f8] 4279 0000 76f0            clr.w     $000076F0
[00e1e7fe] 4cdf 38c2                 movem.l   (a7)+,d1/d6-d7/a3-a5
[00e1e802] 4e5e                      unlk      a6
[00e1e804] 4e75                      rts
[00e1e806] 7003                      moveq.l   #3,d0
[00e1e808] 6100 f1b4                 bsr       $00E1D9BE
[00e1e80c] 2f39 0000 94c2            move.l    $000094C2,-(a7)
[00e1e812] 4e75                      rts
[00e1e814] 3007                      move.w    d7,d0
[00e1e816] 6100 efaa                 bsr       $00E1D7C2
[00e1e81a] 2268 0002                 movea.l   2(a0),a1
[00e1e81e] 3429 001c                 move.w    28(a1),d2
[00e1e822] 70ff                      moveq.l   #-1,d0
[00e1e824] 78ff                      moveq.l   #-1,d4
[00e1e826] 3210                      move.w    (a0),d1
[00e1e828] e649                      lsr.w     #3,d1
[00e1e82a] c27c 0001                 and.w     #$0001,d1
[00e1e82e] 671c                      beq.s     $00E1E84C
[00e1e830] 2268 0032                 movea.l   50(a0),a1
[00e1e834] 3029 0030                 move.w    48(a1),d0
[00e1e838] 6602                      bne.s     $00E1E83C
[00e1e83a] 70ff                      moveq.l   #-1,d0
[00e1e83c] 4a47                      tst.w     d7
[00e1e83e] 6708                      beq.s     $00E1E848
[00e1e840] 2268 0036                 movea.l   54(a0),a1
[00e1e844] 3829 0030                 move.w    48(a1),d4
[00e1e848] 4840                      swap      d0
[00e1e84a] 3004                      move.w    d4,d0
[00e1e84c] 4e75                      rts
[00e1e84e] 48e7 5d1c                 movem.l   d1/d3-d5/d7/a3-a5,-(a7)
[00e1e852] 3e2f 0024                 move.w    36(a7),d7
[00e1e856] 3a2f 0026                 move.w    38(a7),d5
[00e1e85a] 2a6f 0028                 movea.l   40(a7),a5
[00e1e85e] 4295                      clr.l     (a5)
[00e1e860] 42ad 0004                 clr.l     4(a5)
[00e1e864] 223c 0115 9bf0            move.l    #$01159BF0,d1
[00e1e86a] 43fa 0026                 lea.l     $00E1E892(pc),a1
[00e1e86e] 6000 0120                 bra       $00E1E990
[00e1e872] 0c0c 0c0c                 cmpi.b    #$0C,a4 ; apollo only
[00e1e876] 1a16                      move.b    (a6),d5
[00e1e878] 181c                      move.b    (a4)+,d4
[00e1e87a] 2c2a 3244                 move.l    12868(a2),d6
[00e1e87e] 440c                      neg.b     a4 ; apollo only
[00e1e880] 5a28 264a                 addq.b    #5,9802(a0)
[00e1e884] 8a90                      or.l      (a0),d5
[00e1e886] 7e0c                      moveq.l   #12,d7
[00e1e888] 0c0c 686e                 cmpi.b    #$6E,a4 ; apollo only
[00e1e88c] aaba d0da                 mac.w     d?.u,d?.u,0,acc3
[00e1e890] e400                      asr.b     #2,d0
[00e1e892] ba7c 0014                 cmp.w     #$0014,d5
[00e1e896] 6706                      beq.s     $00E1E89E
[00e1e898] 7468                      moveq.l   #104,d2
[00e1e89a] 6100 fcd6                 bsr       $00E1E572
[00e1e89e] 7c00                      moveq.l   #0,d6
[00e1e8a0] 3006                      move.w    d6,d0
[00e1e8a2] 4cdf 38ba                 movem.l   (a7)+,d1/d3-d5/d7/a3-a5
[00e1e8a6] 4e75                      rts
[00e1e8a8] 5241                      addq.w    #1,d1
[00e1e8aa] 5241                      addq.w    #1,d1
[00e1e8ac] 5241                      addq.w    #1,d1
[00e1e8ae] 204d                      movea.l   a5,a0
[00e1e8b0] 3007                      move.w    d7,d0
[00e1e8b2] 6100 ef4e                 bsr       $00E1D802
[00e1e8b6] 60e8                      bra.s     $00E1E8A0
[00e1e8b8] 5441                      addq.w    #2,d1
[00e1e8ba] 5441                      addq.w    #2,d1
[00e1e8bc] 5441                      addq.w    #2,d1
[00e1e8be] 3ab4 103a                 move.w    58(a4,d1.w),(a5)
[00e1e8c2] 60dc                      bra.s     $00E1E8A0
[00e1e8c4] 3e39 0000 7b8c            move.w    $00007B8C,d7
[00e1e8ca] 6100 ff48                 bsr       $00E1E814
[00e1e8ce] 3ac7                      move.w    d7,(a5)+
[00e1e8d0] 3ac2                      move.w    d2,(a5)+
[00e1e8d2] 3a80                      move.w    d0,(a5)
[00e1e8d4] 60ca                      bra.s     $00E1E8A0
[00e1e8d6] 6100 fae4                 bsr       $00E1E3BC
[00e1e8da] 60c4                      bra.s     $00E1E8A0
[00e1e8dc] 486d 0004                 pea.l     4(a5)
[00e1e8e0] 4855                      pea.l     (a5)
[00e1e8e2] 6100 2d2a                 bsr       $00E2160E
[00e1e8e6] 4e71                      nop
[00e1e8e8] 508f                      addq.l    #8,a7
[00e1e8ea] 60ee                      bra.s     $00E1E8DA
[00e1e8ec] 2af9 0000 770c            move.l    $0000770C,(a5)+
[00e1e8f2] 3ab9 0000 7a8e            move.w    $00007A8E,(a5)
[00e1e8f8] 60e0                      bra.s     $00E1E8DA
[00e1e8fa] 1214                      move.b    (a4),d1
[00e1e8fc] 3a81                      move.w    d1,(a5)
[00e1e8fe] 60da                      bra.s     $00E1E8DA
[00e1e900] 7000                      moveq.l   #0,d0
[00e1e902] 6100 efb6                 bsr       $00E1D8BA
[00e1e906] 4a40                      tst.w     d0
[00e1e908] 6602                      bne.s     $00E1E90C
[00e1e90a] 70ff                      moveq.l   #-1,d0
[00e1e90c] 3a80                      move.w    d0,(a5)
[00e1e90e] 60ca                      bra.s     $00E1E8DA
[00e1e910] 6100 ff02                 bsr       $00E1E814
[00e1e914] 3ac2                      move.w    d2,(a5)+
[00e1e916] 3ac1                      move.w    d1,(a5)+
[00e1e918] 2a80                      move.l    d0,(a5)
[00e1e91a] 60be                      bra.s     $00E1E8DA
[00e1e91c] 41ec 0042                 lea.l     66(a4),a0
[00e1e920] 6004                      bra.s     $00E1E926
[00e1e922] 41eb 5558                 lea.l     21848(a3),a0
[00e1e926] 226e 000e                 movea.l   14(a6),a1
[00e1e92a] 3029 0004                 move.w    4(a1),d0
[00e1e92e] 3ac0                      move.w    d0,(a5)+
[00e1e930] d040                      add.w     d0,d0
[00e1e932] d0c0                      adda.w    d0,a0
[00e1e934] 3ad0                      move.w    (a0),(a5)+
[00e1e936] 3aa8 002e                 move.w    46(a0),(a5)
[00e1e93a] 609e                      bra.s     $00E1E8DA
[00e1e93c] 41f9 0000 94ac            lea.l     $000094AC,a0
[00e1e942] 2ad0                      move.l    (a0),(a5)+
[00e1e944] 2aa8 000e                 move.l    14(a0),(a5)
[00e1e948] 3c05                      move.w    d5,d6
[00e1e94a] 608e                      bra.s     $00E1E8DA
[00e1e94c] 3afc 1fff                 move.w    #$1FFF,(a5)+
[00e1e950] 6100 ee7e                 bsr       $00E1D7D0
[00e1e954] 3ac0                      move.w    d0,(a5)+
[00e1e956] 3afc 0fff                 move.w    #$0FFF,(a5)+
[00e1e95a] 6100 ee7c                 bsr       $00E1D7D8
[00e1e95e] 3ac0                      move.w    d0,(a5)+
[00e1e960] 60e6                      bra.s     $00E1E948
[00e1e962] 41eb 5550                 lea.l     21840(a3),a0
[00e1e966] 2ad8                      move.l    (a0)+,(a5)+
[00e1e968] 2ad8                      move.l    (a0)+,(a5)+
[00e1e96a] 60dc                      bra.s     $00E1E948
[00e1e96c] 3afc 007f                 move.w    #$007F,(a5)+
[00e1e970] 588d                      addq.l    #4,a5
[00e1e972] 525d                      addq.w    #1,(a5)+
[00e1e974] 60c4                      bra.s     $00E1E93A
[00e1e976] 7209                      moveq.l   #9,d1
[00e1e978] 6100 ee56                 bsr       $00E1D7D0
[00e1e97c] 6704                      beq.s     $00E1E982
[00e1e97e] 0055 0040                 ori.w     #$0040,(a5)
[00e1e982] 7202                      moveq.l   #2,d1
[00e1e984] 6100 ee52                 bsr       $00E1D7D8
[00e1e988] 6704                      beq.s     $00E1E98E
[00e1e98a] 0055 0006                 ori.w     #$0006,(a5)
[00e1e98e] 60aa                      bra.s     $00E1E93A
[00e1e990] 2679 0000 94b2            movea.l   $000094B2,a3
[00e1e996] 7019                      moveq.l   #25,d0
[00e1e998] ba40                      cmp.w     d0,d5
[00e1e99a] 621c                      bhi.s     $00E1E9B8
[00e1e99c] 0b01                      btst      d5,d1
[00e1e99e] 670a                      beq.s     $00E1E9AA
[00e1e9a0] 3007                      move.w    d7,d0
[00e1e9a2] 6100 fbe4                 bsr       $00E1E588
[00e1e9a6] 670e                      beq.s     $00E1E9B6
[00e1e9a8] 2848                      movea.l   a0,a4
[00e1e9aa] 3005                      move.w    d5,d0
[00e1e9ac] 7200                      moveq.l   #0,d1
[00e1e9ae] 1031 00e0                 move.b    -32(a1,d0.w),d0
[00e1e9b2] 4ef1 0000                 jmp       0(a1,d0.w)
[00e1e9b6] 4ed1                      jmp       (a1)
[00e1e9b8] 3005                      move.w    d5,d0
[00e1e9ba] 907c 5758                 sub.w     #$5758,d0
[00e1e9be] b07c 0004                 cmp.w     #$0004,d0
[00e1e9c2] 6206                      bhi.s     $00E1E9CA
[00e1e9c4] d07c 001a                 add.w     #$001A,d0
[00e1e9c8] 60e2                      bra.s     $00E1E9AC
[00e1e9ca] 7000                      moveq.l   #0,d0
[00e1e9cc] 60de                      bra.s     $00E1E9AC
[00e1e9ce] 4e56 fff8                 link      a6,#$FFF8
[00e1e9d2] 48e7 4d1c                 movem.l   d1/d4-d5/d7/a3-a5,-(a7)
[00e1e9d6] 3e2e 0008                 move.w    8(a6),d7
[00e1e9da] 3a2e 000a                 move.w    10(a6),d5
[00e1e9de] 2a6e 000c                 movea.l   12(a6),a5
[00e1e9e2] 6100 0182                 bsr       $00E1EB66
[00e1e9e6] 223c 0305 872c            move.l    #$0305872C,d1
[00e1e9ec] 43fa 0024                 lea.l     $00E1EA12(pc),a1
[00e1e9f0] 609e                      bra.s     $00E1E990
[00e1e9f2] 0606 1620                 addi.b    #$20,d6
[00e1e9f6] 0658 0606                 addi.w    #$0606,(a0)+
[00e1e9fa] 766c                      moveq.l   #108,d3
[00e1e9fc] 4406                      neg.b     d6
[00e1e9fe] 06a0 b074 6a06            addi.l    #$B0746A06,-(a0)
[00e1ea04] d4de                      adda.w    (a6)+,a2
[00e1ea06] 0606 0606                 addi.b    #$06,d6
[00e1ea0a] ce46                      and.w     d6,d7
[00e1ea0c] 0606 e606                 addi.b    #$06,d6
[00e1ea10] 0600 7469                 addi.b    #$69,d0
[00e1ea14] 6100 fb5c                 bsr       $00E1E572
[00e1ea18] 7c00                      moveq.l   #0,d6
[00e1ea1a] 6100 014e                 bsr       $00E1EB6A
[00e1ea1e] 4cdf 38b2                 movem.l   (a7)+,d1/d4-d5/d7/a3-a5
[00e1ea22] 4e5e                      unlk      a6
[00e1ea24] 3006                      move.w    d6,d0
[00e1ea26] 4e75                      rts
[00e1ea28] 7803                      moveq.l   #3,d4
[00e1ea2a] 2955 0008                 move.l    (a5),8(a4)
[00e1ea2e] 7400                      moveq.l   #0,d2
[00e1ea30] 6008                      bra.s     $00E1EA3A
[00e1ea32] 7805                      moveq.l   #5,d4
[00e1ea34] 2955 000c                 move.l    (a5),12(a4)
[00e1ea38] 7404                      moveq.l   #4,d2
[00e1ea3a] 302c 0006                 move.w    6(a4),d0
[00e1ea3e] 0500                      btst      d2,d0
[00e1ea40] 67d8                      beq.s     $00E1EA1A
[00e1ea42] 3014                      move.w    (a4),d0
[00e1ea44] 0800 0003                 btst      #3,d0
[00e1ea48] 67d0                      beq.s     $00E1EA1A
[00e1ea4a] 91c8                      suba.l    a0,a0
[00e1ea4c] 3204                      move.w    d4,d1
[00e1ea4e] 3007                      move.w    d7,d0
[00e1ea50] 6100 f420                 bsr       $00E1DE72
[00e1ea54] 60c4                      bra.s     $00E1EA1A
[00e1ea56] 72ff                      moveq.l   #-1,d1
[00e1ea58] 4a47                      tst.w     d7
[00e1ea5a] 67b6                      beq.s     $00E1EA12
[00e1ea5c] 082c 0003 0001            btst      #3,1(a4)
[00e1ea62] 67ae                      beq.s     $00E1EA12
[00e1ea64] 6100 f7b6                 bsr       $00E1E21C
[00e1ea68] 60b0                      bra.s     $00E1EA1A
[00e1ea6a] 4a47                      tst.w     d7
[00e1ea6c] 67a4                      beq.s     $00E1EA12
[00e1ea6e] 082c 0003 0001            btst      #3,1(a4)
[00e1ea74] 679c                      beq.s     $00E1EA12
[00e1ea76] 6100 f6d2                 bsr       $00E1E14A
[00e1ea7a] 609e                      bra.s     $00E1EA1A
[00e1ea7c] 5841                      addq.w    #4,d1
[00e1ea7e] 5441                      addq.w    #2,d1
[00e1ea80] 780c                      moveq.l   #12,d4
[00e1ea82] 7408                      moveq.l   #8,d2
[00e1ea84] 6006                      bra.s     $00E1EA8C
[00e1ea86] 5841                      addq.w    #4,d1
[00e1ea88] 7811                      moveq.l   #17,d4
[00e1ea8a] 740b                      moveq.l   #11,d2
[00e1ea8c] 41f4 103a                 lea.l     58(a4,d1.w),a0
[00e1ea90] 3015                      move.w    (a5),d0
[00e1ea92] b07c ffff                 cmp.w     #$FFFF,d0
[00e1ea96] 6c02                      bge.s     $00E1EA9A
[00e1ea98] 70ff                      moveq.l   #-1,d0
[00e1ea9a] b07c 03e8                 cmp.w     #$03E8,d0
[00e1ea9e] 6f04                      ble.s     $00E1EAA4
[00e1eaa0] 303c 03e8                 move.w    #$03E8,d0
[00e1eaa4] b050                      cmp.w     (a0),d0
[00e1eaa6] 67d2                      beq.s     $00E1EA7A
[00e1eaa8] 3080                      move.w    d0,(a0)
[00e1eaaa] 6100 ee98                 bsr       $00E1D944
[00e1eaae] 668a                      bne.s     $00E1EA3A
[00e1eab0] 60c8                      bra.s     $00E1EA7A
[00e1eab2] 33c7 0000 76f0            move.w    d7,$000076F0
[00e1eab8] 66c0                      bne.s     $00E1EA7A
[00e1eaba] 2e8d                      move.l    a5,(a7)
[00e1eabc] 6100 f300                 bsr       $00E1DDBE
[00e1eac0] 60b8                      bra.s     $00E1EA7A
[00e1eac2] 322d 0004                 move.w    4(a5),d1
[00e1eac6] 2015                      move.l    (a5),d0
[00e1eac8] 6608                      bne.s     $00E1EAD2
[00e1eaca] 7200                      moveq.l   #0,d1
[00e1eacc] 2039 0000 7504            move.l    $00007504,d0
[00e1ead2] 23c0 0000 770c            move.l    d0,$0000770C
[00e1ead8] 33c1 0000 7a8e            move.w    d1,$00007A8E
[00e1eade] 609a                      bra.s     $00E1EA7A
[00e1eae0] 3015                      move.w    (a5),d0
[00e1eae2] 1880                      move.b    d0,(a4)
[00e1eae4] 60f8                      bra.s     $00E1EADE
[00e1eae6] 41ec 0042                 lea.l     66(a4),a0
[00e1eaea] 6116                      bsr.s     $00E1EB02
[00e1eaec] 6000 ff54                 bra       $00E1EA42
[00e1eaf0] 41eb 5558                 lea.l     21848(a3),a0
[00e1eaf4] 610c                      bsr.s     $00E1EB02
[00e1eaf6] 60e6                      bra.s     $00E1EADE
[00e1eaf8] 41eb 5550                 lea.l     21840(a3),a0
[00e1eafc] 20dd                      move.l    (a5)+,(a0)+
[00e1eafe] 20dd                      move.l    (a5)+,(a0)+
[00e1eb00] 60dc                      bra.s     $00E1EADE
[00e1eb02] 381d                      move.w    (a5)+,d4
[00e1eb04] 3004                      move.w    d4,d0
[00e1eb06] d040                      add.w     d0,d0
[00e1eb08] 7401                      moveq.l   #1,d2
[00e1eb0a] 321d                      move.w    (a5)+,d1
[00e1eb0c] 0c41 ffff                 cmpi.w    #$FFFF,d1
[00e1eb10] 6704                      beq.s     $00E1EB16
[00e1eb12] 3181 0000                 move.w    d1,0(a0,d0.w)
[00e1eb16] 3168 0014 0026            move.w    20(a0),38(a0)
[00e1eb1c] 3168 0016 0028            move.w    22(a0),40(a0)
[00e1eb22] 3168 001e 002a            move.w    30(a0),42(a0)
[00e1eb28] 3168 0020 002c            move.w    32(a0),44(a0)
[00e1eb2e] d0fc 002e                 adda.w    #$002E,a0
[00e1eb32] 51ca ffd6                 dbf       d2,$00E1EB0A
[00e1eb36] 4e75                      rts
[00e1eb38] 2f0d                      move.l    a5,-(a7)
[00e1eb3a] 6100 ec7e                 bsr       $00E1D7BA
[00e1eb3e] 2a48                      movea.l   a0,a5
[00e1eb40] 2a6d 0036                 movea.l   54(a5),a5
[00e1eb44] 41ed 0028                 lea.l     40(a5),a0
[00e1eb48] 202f 0008                 move.l    8(a7),d0
[00e1eb4c] 6100 ed5e                 bsr       $00E1D8AC
[00e1eb50] 4a40                      tst.w     d0
[00e1eb52] 6706                      beq.s     $00E1EB5A
[00e1eb54] 302d 0030                 move.w    48(a5),d0
[00e1eb58] 6008                      bra.s     $00E1EB62
[00e1eb5a] 4a6d 0030                 tst.w     48(a5)
[00e1eb5e] 66e0                      bne.s     $00E1EB40
[00e1eb60] 70ff                      moveq.l   #-1,d0
[00e1eb62] 2a5f                      movea.l   (a7)+,a5
[00e1eb64] 4e75                      rts
[00e1eb66] 7001                      moveq.l   #1,d0
[00e1eb68] 6002                      bra.s     $00E1EB6C
[00e1eb6a] 7000                      moveq.l   #0,d0
[00e1eb6c] 6004                      bra.s     $00E1EB72
[00e1eb6e] 302f 0004                 move.w    4(a7),d0
[00e1eb72] 2079 0000 7534            movea.l   $00007534,a0
[00e1eb78] 2228 0002                 move.l    2(a0),d1
[00e1eb7c] b2b9 0000 cb00            cmp.l     $0000CB00,d1
[00e1eb82] 57c1                      seq       d1
[00e1eb84] 0880 0008                 bclr      #8,d0
[00e1eb88] 670c                      beq.s     $00E1EB96
[00e1eb8a] 4a50                      tst.w     (a0)
[00e1eb8c] 6708                      beq.s     $00E1EB96
[00e1eb8e] 4a01                      tst.b     d1
[00e1eb90] 6604                      bne.s     $00E1EB96
[00e1eb92] 4246                      clr.w     d6
[00e1eb94] 6046                      bra.s     $00E1EBDC
[00e1eb96] 5540                      subq.w    #2,d0
[00e1eb98] 6c30                      bge.s     $00E1EBCA
[00e1eb9a] 5440                      addq.w    #2,d0
[00e1eb9c] 6720                      beq.s     $00E1EBBE
[00e1eb9e] 2f08                      move.l    a0,-(a7)
[00e1eba0] 6100 9c58                 bsr       $00E187FA
[00e1eba4] 4e71                      nop
[00e1eba6] 588f                      addq.l    #4,a7
[00e1eba8] 4a40                      tst.w     d0
[00e1ebaa] 6630                      bne.s     $00E1EBDC
[00e1ebac] 2f39 0000 7534            move.l    $00007534,-(a7)
[00e1ebb2] 3f3c 0004                 move.w    #$0004,-(a7)
[00e1ebb6] 6100 96f4                 bsr       $00E182AC
[00e1ebba] 5c8f                      addq.l    #6,a7
[00e1ebbc] 601e                      bra.s     $00E1EBDC
[00e1ebbe] 4a50                      tst.w     (a0)
[00e1ebc0] 6f14                      ble.s     $00E1EBD6
[00e1ebc2] 4a01                      tst.b     d1
[00e1ebc4] 6710                      beq.s     $00E1EBD6
[00e1ebc6] 6118                      bsr.s     $00E1EBE0
[00e1ebc8] 6012                      bra.s     $00E1EBDC
[00e1ebca] 3f00                      move.w    d0,-(a7)
[00e1ebcc] 4eb9 00e1 7de0            jsr       $00E17DE0
[00e1ebd2] 548f                      addq.l    #2,a7
[00e1ebd4] 6006                      bra.s     $00E1EBDC
[00e1ebd6] 746b                      moveq.l   #107,d2
[00e1ebd8] 6100 f998                 bsr       $00E1E572
[00e1ebdc] 3006                      move.w    d6,d0
[00e1ebde] 4e75                      rts
[00e1ebe0] 2f08                      move.l    a0,-(a7)
[00e1ebe2] 6100 9c8c                 bsr       $00E18870
[00e1ebe6] 4e71                      nop
[00e1ebe8] 588f                      addq.l    #4,a7
[00e1ebea] 4e75                      rts
[00e1ebec] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e1ebf2] 48e7 1f20                 movem.l   d3-d7/a2,-(a7)
[00e1ebf6] 3c2f 001e                 move.w    30(a7),d6
[00e1ebfa] 7801                      moveq.l   #1,d4
[00e1ebfc] 7e01                      moveq.l   #1,d7
[00e1ebfe] 7601                      moveq.l   #1,d3
[00e1ec00] 7a01                      moveq.l   #1,d5
[00e1ec02] 3439 0000 94be            move.w    $000094BE,d2
[00e1ec08] 5342                      subq.w    #1,d2
[00e1ec0a] 3239 0000 94c0            move.w    $000094C0,d1
[00e1ec10] 5341                      subq.w    #1,d1
[00e1ec12] 3006                      move.w    d6,d0
[00e1ec14] c07c 0007                 and.w     #$0007,d0
[00e1ec18] 6702                      beq.s     $00E1EC1C
[00e1ec1a] de41                      add.w     d1,d7
[00e1ec1c] 0806 0004                 btst      #4,d6
[00e1ec20] 6702                      beq.s     $00E1EC24
[00e1ec22] de41                      add.w     d1,d7
[00e1ec24] 613e                      bsr.s     $00E1EC64
[00e1ec26] 4a40                      tst.w     d0
[00e1ec28] 6702                      beq.s     $00E1EC2C
[00e1ec2a] d642                      add.w     d2,d3
[00e1ec2c] 4840                      swap      d0
[00e1ec2e] 4a40                      tst.w     d0
[00e1ec30] 6702                      beq.s     $00E1EC34
[00e1ec32] da41                      add.w     d1,d5
[00e1ec34] 4a6f 001c                 tst.w     28(a7)
[00e1ec38] 6608                      bne.s     $00E1EC42
[00e1ec3a] 4444                      neg.w     d4
[00e1ec3c] 4447                      neg.w     d7
[00e1ec3e] 4443                      neg.w     d3
[00e1ec40] 4445                      neg.w     d5
[00e1ec42] 41ef 0020                 lea.l     32(a7),a0
[00e1ec46] 246f 0028                 movea.l   40(a7),a2
[00e1ec4a] 3498                      move.w    (a0)+,(a2)
[00e1ec4c] d95a                      add.w     d4,(a2)+
[00e1ec4e] 3498                      move.w    (a0)+,(a2)
[00e1ec50] df5a                      add.w     d7,(a2)+
[00e1ec52] 3498                      move.w    (a0)+,(a2)
[00e1ec54] 9952                      sub.w     d4,(a2)
[00e1ec56] 975a                      sub.w     d3,(a2)+
[00e1ec58] 3490                      move.w    (a0),(a2)
[00e1ec5a] 9f52                      sub.w     d7,(a2)
[00e1ec5c] 9b52                      sub.w     d5,(a2)
[00e1ec5e] 4cdf 04f8                 movem.l   (a7)+,d3-d7/a2
[00e1ec62] 4e75                      rts
[00e1ec64] 3f01                      move.w    d1,-(a7)
[00e1ec66] 6100 eb80                 bsr       $00E1D7E8
[00e1ec6a] 3200                      move.w    d0,d1
[00e1ec6c] 3006                      move.w    d6,d0
[00e1ec6e] 4840                      swap      d0
[00e1ec70] 3006                      move.w    d6,d0
[00e1ec72] c0bc 0e00 01c0            and.l     #$0E0001C0,d0
[00e1ec78] 0806 0005                 btst      #5,d6
[00e1ec7c] 6710                      beq.s     $00E1EC8E
[00e1ec7e] 0801 0001                 btst      #1,d1
[00e1ec82] 6604                      bne.s     $00E1EC88
[00e1ec84] 4680                      not.l     d0
[00e1ec86] 6006                      bra.s     $00E1EC8E
[00e1ec88] 4a80                      tst.l     d0
[00e1ec8a] 6602                      bne.s     $00E1EC8E
[00e1ec8c] 4640                      not.w     d0
[00e1ec8e] 321f                      move.w    (a7)+,d1
[00e1ec90] 4e75                      rts
[00e1ec92] 0c6e 0001 0008            cmpi.w    #$0001,8(a6)
[00e1ec98] 670e                      beq.s     $00E1ECA8
[00e1ec9a] 7201                      moveq.l   #1,d1
[00e1ec9c] 6100 eb32                 bsr       $00E1D7D0
[00e1eca0] 6708                      beq.s     $00E1ECAA
[00e1eca2] 0c6e 0002 0008            cmpi.w    #$0002,8(a6)
[00e1eca8] 4e75                      rts
[00e1ecaa] 7001                      moveq.l   #1,d0
[00e1ecac] 4e75                      rts
[00e1ecae] 205f                      movea.l   (a7)+,a0
[00e1ecb0] 7001                      moveq.l   #1,d0
[00e1ecb2] e188                      lsl.l     #8,d0
[00e1ecb4] 1039 0000 94c7            move.b    $000094C7,d0
[00e1ecba] 5200                      addq.b    #1,d0
[00e1ecbc] e188                      lsl.l     #8,d0
[00e1ecbe] 2f00                      move.l    d0,-(a7)
[00e1ecc0] 4ed0                      jmp       (a0)
[00e1ecc2] 48e7 460c                 movem.l   d1/d5-d6/a4-a5,-(a7)
[00e1ecc6] 2a48                      movea.l   a0,a5
[00e1ecc8] 2849                      movea.l   a1,a4
[00e1ecca] 3ebc 0001                 move.w    #$0001,(a7)
[00e1ecce] 6100 2714                 bsr       $00E213E4
[00e1ecd2] 4e71                      nop
[00e1ecd4] 2a39 0000 7366            move.l    $00007366,d5
[00e1ecda] 4845                      swap      d5
[00e1ecdc] 9a6d 0004                 sub.w     4(a5),d5
[00e1ece0] 4845                      swap      d5
[00e1ece2] 9a6d 0006                 sub.w     6(a5),d5
[00e1ece6] 2039 0000 7366            move.l    $00007366,d0
[00e1ecec] 2200                      move.l    d0,d1
[00e1ecee] 7401                      moveq.l   #1,d2
[00e1ecf0] 4840                      swap      d0
[00e1ecf2] 4846                      swap      d6
[00e1ecf4] 4845                      swap      d5
[00e1ecf6] 9045                      sub.w     d5,d0
[00e1ecf8] b046                      cmp.w     d6,d0
[00e1ecfa] 6c02                      bge.s     $00E1ECFE
[00e1ecfc] 3006                      move.w    d6,d0
[00e1ecfe] 51ca fff0                 dbf       d2,$00E1ECF0
[00e1ed02] 2b40 0004                 move.l    d0,4(a5)
[00e1ed06] 2e81                      move.l    d1,(a7)
[00e1ed08] 2f0c                      move.l    a4,-(a7)
[00e1ed0a] 2f0d                      move.l    a5,-(a7)
[00e1ed0c] 6100 2696                 bsr       $00E213A4
[00e1ed10] 4e71                      nop
[00e1ed12] 508f                      addq.l    #8,a7
[00e1ed14] 4a40                      tst.w     d0
[00e1ed16] 66ce                      bne.s     $00E1ECE6
[00e1ed18] 4cdf 3062                 movem.l   (a7)+,d1/d5-d6/a4-a5
[00e1ed1c] 4e75                      rts
[00e1ed1e] 6100 fe46                 bsr       $00E1EB66
[00e1ed22] 6100 fa26                 bsr       $00E1E74A
[00e1ed26] 42b9 0000 7abc            clr.l     $00007ABC
[00e1ed2c] 600a                      bra.s     $00E1ED38
[00e1ed2e] 4279 0000 7a7c            clr.w     $00007A7C
[00e1ed34] 6100 feaa                 bsr       $00E1EBE0
[00e1ed38] 2279 0000 cb00            movea.l   $0000CB00,a1
[00e1ed3e] 2079 0000 7534            movea.l   $00007534,a0
[00e1ed44] 4a50                      tst.w     (a0)
[00e1ed46] 6706                      beq.s     $00E1ED4E
[00e1ed48] b3e8 0002                 cmpa.l    2(a0),a1
[00e1ed4c] 67e0                      beq.s     $00E1ED2E
[00e1ed4e] 23c9 0000 7ac0            move.l    a1,$00007AC0
[00e1ed54] 7001                      moveq.l   #1,d0
[00e1ed56] 4e75                      rts
[00e1ed58] 4eb9 00e2 2894            jsr       $00E22894
[00e1ed5e] 2a00                      move.l    d0,d5
[00e1ed60] dab9 0000 7188            add.l     $00007188,d5
[00e1ed66] 4e75                      rts
[00e1ed68] 70ff                      moveq.l   #-1,d0
[00e1ed6a] 4eb9 00e1 7a70            jsr       $00E17A70
[00e1ed70] 4a40                      tst.w     d0
[00e1ed72] 4e75                      rts
[00e1ed74] 7208                      moveq.l   #8,d1
[00e1ed76] 6100 ea68                 bsr       $00E1D7E0
[00e1ed7a] 6606                      bne.s     $00E1ED82
[00e1ed7c] 377c 01f4 0006            move.w    #$01F4,6(a3)
[00e1ed82] 4e75                      rts
[00e1ed84] 6100 0124                 bsr       $00E1EEAA
[00e1ed88] ffec                      dc.w      $FFEC ; illegal
[00e1ed8a] 7800                      moveq.l   #0,d4
[00e1ed8c] 4e56 ffee                 link      a6,#$FFEE
[00e1ed90] 48e7 5e1c                 movem.l   d1/d3-d6/a3-a5,-(a7)
[00e1ed94] b67c 0013                 cmp.w     #$0013,d3
[00e1ed98] 6c20                      bge.s     $00E1EDBA
[00e1ed9a] 7400                      moveq.l   #0,d2
[00e1ed9c] 6100 f8c4                 bsr       $00E1E662
[00e1eda0] b67c 000f                 cmp.w     #$000F,d3
[00e1eda4] 6d02                      blt.s     $00E1EDA8
[00e1eda6] 5743                      subq.w    #3,d3
[00e1eda8] d67c 0009                 add.w     #$0009,d3
[00e1edac] 7208                      moveq.l   #8,d1
[00e1edae] 6100 ea30                 bsr       $00E1D7E0
[00e1edb2] 6606                      bne.s     $00E1EDBA
[00e1edb4] 5243                      addq.w    #1,d3
[00e1edb6] c67c fffe                 and.w     #$FFFE,d3
[00e1edba] 7401                      moveq.l   #1,d2
[00e1edbc] 6100 f8a4                 bsr       $00E1E662
[00e1edc0] 43ee fff0                 lea.l     -16(a6),a1
[00e1edc4] 3003                      move.w    d3,d0
[00e1edc6] 204c                      movea.l   a4,a0
[00e1edc8] 6100 eb5e                 bsr       $00E1D928
[00e1edcc] 6100 03f4                 bsr       $00E1F1C2
[00e1edd0] 51ee ffee                 sf        -18(a6)
[00e1edd4] 42ae fffc                 clr.l     -4(a6)
[00e1edd8] 7a00                      moveq.l   #0,d5
[00e1edda] 2639 0000 7366            move.l    $00007366,d3
[00e1ede0] 2839 0000 94be            move.l    $000094BE,d4
[00e1ede6] 41ee fff0                 lea.l     -16(a6),a0
[00e1edea] 7c02                      moveq.l   #2,d6
[00e1edec] 7206                      moveq.l   #6,d1
[00e1edee] 7402                      moveq.l   #2,d2
[00e1edf0] 4843                      swap      d3
[00e1edf2] 4844                      swap      d4
[00e1edf4] 3003                      move.w    d3,d0
[00e1edf6] 9050                      sub.w     (a0),d0
[00e1edf8] 6d0a                      blt.s     $00E1EE04
[00e1edfa] 9068 0004                 sub.w     4(a0),d0
[00e1edfe] 6f1c                      ble.s     $00E1EE1C
[00e1ee00] 5281                      addq.l    #1,d1
[00e1ee02] 4440                      neg.w     d0
[00e1ee04] 48c0                      ext.l     d0
[00e1ee06] 81c4                      divs.w    d4,d0
[00e1ee08] 5340                      subq.w    #1,d0
[00e1ee0a] b268 000c                 cmp.w     12(a0),d1
[00e1ee0e] 6702                      beq.s     $00E1EE12
[00e1ee10] 7c00                      moveq.l   #0,d6
[00e1ee12] 3141 0008                 move.w    d1,8(a0)
[00e1ee16] 4841                      swap      d1
[00e1ee18] 3200                      move.w    d0,d1
[00e1ee1a] 6006                      bra.s     $00E1EE22
[00e1ee1c] 7200                      moveq.l   #0,d1
[00e1ee1e] 4268 0008                 clr.w     8(a0)
[00e1ee22] c342                      exg       d1,d2
[00e1ee24] 5488                      addq.l    #2,a0
[00e1ee26] 4244                      clr.w     d4
[00e1ee28] 4a84                      tst.l     d4
[00e1ee2a] 66c4                      bne.s     $00E1EDF0
[00e1ee2c] 4a81                      tst.l     d1
[00e1ee2e] 660a                      bne.s     $00E1EE3A
[00e1ee30] 4a82                      tst.l     d2
[00e1ee32] 6606                      bne.s     $00E1EE3A
[00e1ee34] 42ae fffc                 clr.l     -4(a6)
[00e1ee38] 6034                      bra.s     $00E1EE6E
[00e1ee3a] 36bc 0018                 move.w    #$0018,(a3)
[00e1ee3e] 4a81                      tst.l     d1
[00e1ee40] 670a                      beq.s     $00E1EE4C
[00e1ee42] 4a82                      tst.l     d2
[00e1ee44] 6708                      beq.s     $00E1EE4E
[00e1ee46] 4a2e ffee                 tst.b     -18(a6)
[00e1ee4a] 6702                      beq.s     $00E1EE4E
[00e1ee4c] c342                      exg       d1,d2
[00e1ee4e] 2741 0006                 move.l    d1,6(a3)
[00e1ee52] 2742 000a                 move.l    d2,10(a3)
[00e1ee56] 7001                      moveq.l   #1,d0
[00e1ee58] 6100 ff10                 bsr       $00E1ED6A
[00e1ee5c] 6710                      beq.s     $00E1EE6E
[00e1ee5e] 462e ffee                 not.b     -18(a6)
[00e1ee62] 2d6e fff8 fffc            move.l    -8(a6),-4(a6)
[00e1ee68] 2006                      move.l    d6,d0
[00e1ee6a] 6100 feec                 bsr       $00E1ED58
[00e1ee6e] 6100 039a                 bsr       $00E1F20A
[00e1ee72] 670c                      beq.s     $00E1EE80
[00e1ee74] bab9 0000 7188            cmp.l     $00007188,d5
[00e1ee7a] 64f2                      bcc.s     $00E1EE6E
[00e1ee7c] 6000 ff5c                 bra       $00E1EDDA
[00e1ee80] 6100 0346                 bsr       $00E1F1C8
[00e1ee84] 4cdf 387a                 movem.l   (a7)+,d1/d3-d6/a3-a5
[00e1ee88] 4e5e                      unlk      a6
[00e1ee8a] 4e75                      rts
[00e1ee8c] 74ff                      moveq.l   #-1,d2
[00e1ee8e] 6006                      bra.s     $00E1EE96
[00e1ee90] 0805 0002                 btst      #2,d5
[00e1ee94] 57c2                      seq       d2
[00e1ee96] 245f                      movea.l   (a7)+,a2
[00e1ee98] b42e ffe8                 cmp.b     -24(a6),d2
[00e1ee9c] 6704                      beq.s     $00E1EEA2
[00e1ee9e] 5497                      addq.l    #2,(a7)
[00e1eea0] 4e75                      rts
[00e1eea2] 2257                      movea.l   (a7),a1
[00e1eea4] d2d1                      adda.w    (a1),a1
[00e1eea6] 2e89                      move.l    a1,(a7)
[00e1eea8] 4ed2                      jmp       (a2)
[00e1eeaa] 61e0                      bsr.s     $00E1EE8C
[00e1eeac] 7200                      moveq.l   #0,d1
[00e1eeae] 183b 3008                 move.b    $00E1EEB8(pc,d3.w),d4
[00e1eeb2] 6c06                      bge.s     $00E1EEBA
[00e1eeb4] 4404                      neg.b     d4
[00e1eeb6] 323c 03e8                 move.w    #$03E8,d1
[00e1eeba] 4884                      ext.w     d4
[00e1eebc] 3741 0006                 move.w    d1,6(a3)
[00e1eec0] 4e75                      rts
[00e1eec2] 1ae6                      move.b    -(a6),(a5)+
[00e1eec4] 0000                      dc.w      $0000
[00e1eec6] 0019 e700                 ori.b     #$00,(a1)+
[00e1eeca] 00e6                      dc.w      $00E6 ; illegal
[00e1eecc] 1ae7                      move.b    -(a7),(a5)+
[00e1eece] 1900                      move.b    d0,-(a4)
[00e1eed0] 7000                      moveq.l   #0,d0
[00e1eed2] 6002                      bra.s     $00E1EED6
[00e1eed4] 7001                      moveq.l   #1,d0
[00e1eed6] 61b4                      bsr.s     $00E1EE8C
[00e1eed8] 9640                      sub.w     d0,d3
[00e1eeda] 4eb9 00e1 7a44            jsr       $00E17A44
[00e1eee0] 5243                      addq.w    #1,d3
[00e1eee2] 41ee fff8                 lea.l     -8(a6),a0
[00e1eee6] 611a                      bsr.s     $00E1EF02
[00e1eee8] 4841                      swap      d1
[00e1eeea] e249                      lsr.w     #1,d1
[00e1eeec] d350                      add.w     d1,(a0)
[00e1eeee] 4841                      swap      d1
[00e1eef0] e249                      lsr.w     #1,d1
[00e1eef2] d368 0002                 add.w     d1,2(a0)
[00e1eef6] 6138                      bsr.s     $00E1EF30
[00e1eef8] 3801                      move.w    d1,d4
[00e1eefa] 3740 0006                 move.w    d0,6(a3)
[00e1eefe] c9c5                      muls.w    d5,d4
[00e1ef00] 4e75                      rts
[00e1ef02] 2f08                      move.l    a0,-(a7)
[00e1ef04] 2248                      movea.l   a0,a1
[00e1ef06] 3003                      move.w    d3,d0
[00e1ef08] 5340                      subq.w    #1,d0
[00e1ef0a] 204c                      movea.l   a4,a0
[00e1ef0c] 6100 ea1a                 bsr       $00E1D928
[00e1ef10] 518f                      subq.l    #8,a7
[00e1ef12] 224f                      movea.l   a7,a1
[00e1ef14] 3003                      move.w    d3,d0
[00e1ef16] 204c                      movea.l   a4,a0
[00e1ef18] 6100 ea0e                 bsr       $00E1D928
[00e1ef1c] 201f                      move.l    (a7)+,d0
[00e1ef1e] 221f                      move.l    (a7)+,d1
[00e1ef20] 205f                      movea.l   (a7)+,a0
[00e1ef22] 4841                      swap      d1
[00e1ef24] 9368 0004                 sub.w     d1,4(a0)
[00e1ef28] 4841                      swap      d1
[00e1ef2a] 9368 0006                 sub.w     d1,6(a0)
[00e1ef2e] 4e75                      rts
[00e1ef30] 2039 0000 7366            move.l    $00007366,d0
[00e1ef36] b67c 0012                 cmp.w     #$0012,d3
[00e1ef3a] 6606                      bne.s     $00E1EF42
[00e1ef3c] 7219                      moveq.l   #25,d1
[00e1ef3e] 4840                      swap      d0
[00e1ef40] 6004                      bra.s     $00E1EF46
[00e1ef42] 721a                      moveq.l   #26,d1
[00e1ef44] 5488                      addq.l    #2,a0
[00e1ef46] 3f01                      move.w    d1,-(a7)
[00e1ef48] 3428 0004                 move.w    4(a0),d2
[00e1ef4c] 9050                      sub.w     (a0),d0
[00e1ef4e] 6c02                      bge.s     $00E1EF52
[00e1ef50] 7000                      moveq.l   #0,d0
[00e1ef52] b042                      cmp.w     d2,d0
[00e1ef54] 6f02                      ble.s     $00E1EF58
[00e1ef56] 3002                      move.w    d2,d0
[00e1ef58] 323c 03e8                 move.w    #$03E8,d1
[00e1ef5c] 6100 ee46                 bsr       $00E1DDA4
[00e1ef60] 321f                      move.w    (a7)+,d1
[00e1ef62] 4e75                      rts
[00e1ef64] 6100 ff2a                 bsr       $00E1EE90
[00e1ef68] 48e7 7e1c                 movem.l   d1-d6/a3-a5,-(a7)
[00e1ef6c] 204f                      movea.l   a7,a0
[00e1ef6e] 6192                      bsr.s     $00E1EF02
[00e1ef70] 2439 0000 7366            move.l    $00007366,d2
[00e1ef76] 7a01                      moveq.l   #1,d5
[00e1ef78] 4840                      swap      d0
[00e1ef7a] 4841                      swap      d1
[00e1ef7c] 4842                      swap      d2
[00e1ef7e] 9440                      sub.w     d0,d2
[00e1ef80] 6c02                      bge.s     $00E1EF84
[00e1ef82] 4242                      clr.w     d2
[00e1ef84] b441                      cmp.w     d1,d2
[00e1ef86] 6f02                      ble.s     $00E1EF8A
[00e1ef88] 3401                      move.w    d1,d2
[00e1ef8a] d558                      add.w     d2,(a0)+
[00e1ef8c] 51cd ffea                 dbf       d5,$00E1EF78
[00e1ef90] 204f                      movea.l   a7,a0
[00e1ef92] 619c                      bsr.s     $00E1EF30
[00e1ef94] 3c00                      move.w    d0,d6
[00e1ef96] 6100 022a                 bsr       $00E1F1C2
[00e1ef9a] 204f                      movea.l   a7,a0
[00e1ef9c] 6192                      bsr.s     $00E1EF30
[00e1ef9e] bc40                      cmp.w     d0,d6
[00e1efa0] 6710                      beq.s     $00E1EFB2
[00e1efa2] 3681                      move.w    d1,(a3)
[00e1efa4] 3740 0006                 move.w    d0,6(a3)
[00e1efa8] 6100 fdbe                 bsr       $00E1ED68
[00e1efac] 6704                      beq.s     $00E1EFB2
[00e1efae] 3c2b 0006                 move.w    6(a3),d6
[00e1efb2] 6100 0256                 bsr       $00E1F20A
[00e1efb6] 66e2                      bne.s     $00E1EF9A
[00e1efb8] 6100 020e                 bsr       $00E1F1C8
[00e1efbc] 4cdf 387e                 movem.l   (a7)+,d1-d6/a3-a5
[00e1efc0] 4e75                      rts
[00e1efc2] 4e56 fff8                 link      a6,#$FFF8
[00e1efc6] 48e7 1810                 movem.l   d3-d4/a3,-(a7)
[00e1efca] 3600                      move.w    d0,d3
[00e1efcc] 3801                      move.w    d1,d4
[00e1efce] 3007                      move.w    d7,d0
[00e1efd0] 6100 e7f0                 bsr       $00E1D7C2
[00e1efd4] 47e8 0018                 lea.l     24(a0),a3
[00e1efd8] 7205                      moveq.l   #5,d1
[00e1efda] 6100 e808                 bsr       $00E1D7E4
[00e1efde] 6756                      beq.s     $00E1F036
[00e1efe0] 4a43                      tst.w     d3
[00e1efe2] 6f2c                      ble.s     $00E1F010
[00e1efe4] 41ee fff8                 lea.l     -8(a6),a0
[00e1efe8] 3013                      move.w    (a3),d0
[00e1efea] d06b 0004                 add.w     4(a3),d0
[00e1efee] 9043                      sub.w     d3,d0
[00e1eff0] 3080                      move.w    d0,(a0)
[00e1eff2] 316b 0002 0002            move.w    2(a3),2(a0)
[00e1eff8] 3143 0004                 move.w    d3,4(a0)
[00e1effc] 316b 0006 0006            move.w    6(a3),6(a0)
[00e1f002] 4a44                      tst.w     d4
[00e1f004] 6f04                      ble.s     $00E1F00A
[00e1f006] 9968 0006                 sub.w     d4,6(a0)
[00e1f00a] 3007                      move.w    d7,d0
[00e1f00c] 6100 f2d8                 bsr       $00E1E2E6
[00e1f010] 4a44                      tst.w     d4
[00e1f012] 6f20                      ble.s     $00E1F034
[00e1f014] 41ee fff8                 lea.l     -8(a6),a0
[00e1f018] 2093                      move.l    (a3),(a0)
[00e1f01a] 302b 0006                 move.w    6(a3),d0
[00e1f01e] 9044                      sub.w     d4,d0
[00e1f020] d168 0002                 add.w     d0,2(a0)
[00e1f024] 316b 0004 0004            move.w    4(a3),4(a0)
[00e1f02a] 3144 0006                 move.w    d4,6(a0)
[00e1f02e] 3007                      move.w    d7,d0
[00e1f030] 6100 f2b4                 bsr       $00E1E2E6
[00e1f034] 7c01                      moveq.l   #1,d6
[00e1f036] 4cdf 0818                 movem.l   (a7)+,d3-d4/a3
[00e1f03a] 4e5e                      unlk      a6
[00e1f03c] 4e75                      rts
[00e1f03e] 7207                      moveq.l   #7,d1
[00e1f040] 6100 e796                 bsr       $00E1D7D8
[00e1f044] 673a                      beq.s     $00E1F080
[00e1f046] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e1f04c] 4267                      clr.w     -(a7)
[00e1f04e] 2f28 0010                 move.l    16(a0),-(a7)
[00e1f052] 2f28 000c                 move.l    12(a0),-(a7)
[00e1f056] 3f02                      move.w    d2,-(a7)
[00e1f058] 486f 0002                 pea.l     2(a7)
[00e1f05c] 2f0f                      move.l    a7,-(a7)
[00e1f05e] 41f9 00e3 5f20            lea.l     $00E35F20,a0
[00e1f064] 4868 00c8                 pea.l     200(a0)
[00e1f068] 4879 0000 b2b4            pea.l     $0000B2B4
[00e1f06e] 6100 3582                 bsr       $00E225F2
[00e1f072] 4e71                      nop
[00e1f074] 3f3c 0001                 move.w    #$0001,-(a7)
[00e1f078] 6100 9be8                 bsr       $00E18C62
[00e1f07c] defc 001e                 adda.w    #$001E,a7
[00e1f080] 7c00                      moveq.l   #0,d6
[00e1f082] 4e75                      rts
[00e1f084] 48e7 5004                 movem.l   d1/d3/a5,-(a7)
[00e1f088] 2a4c                      movea.l   a4,a5
[00e1f08a] 7206                      moveq.l   #6,d1
[00e1f08c] 302c 0006                 move.w    6(a4),d0
[00e1f090] 6100 e750                 bsr       $00E1D7E2
[00e1f094] 673e                      beq.s     $00E1F0D4
[00e1f096] 6010                      bra.s     $00E1F0A8
[00e1f098] 0c55 0014                 cmpi.w    #$0014,(a5)
[00e1f09c] 660a                      bne.s     $00E1F0A8
[00e1f09e] 302b 0006                 move.w    6(a3),d0
[00e1f0a2] b06d 0006                 cmp.w     6(a5),d0
[00e1f0a6] 6712                      beq.s     $00E1F0BA
[00e1f0a8] 7010                      moveq.l   #16,d0
[00e1f0aa] 322d 0004                 move.w    4(a5),d1
[00e1f0ae] 6f02                      ble.s     $00E1F0B2
[00e1f0b0] d041                      add.w     d1,d0
[00e1f0b2] dac0                      adda.w    d0,a5
[00e1f0b4] b7cd                      cmpa.l    a5,a3
[00e1f0b6] 6ee0                      bgt.s     $00E1F098
[00e1f0b8] 602c                      bra.s     $00E1F0E6
[00e1f0ba] 224c                      movea.l   a4,a1
[00e1f0bc] 204b                      movea.l   a3,a0
[00e1f0be] 6144                      bsr.s     $00E1F104
[00e1f0c0] 671e                      beq.s     $00E1F0E0
[00e1f0c2] 2600                      move.l    d0,d3
[00e1f0c4] 224d                      movea.l   a5,a1
[00e1f0c6] 204b                      movea.l   a3,a0
[00e1f0c8] 613a                      bsr.s     $00E1F104
[00e1f0ca] 6714                      beq.s     $00E1F0E0
[00e1f0cc] 43ed 0008                 lea.l     8(a5),a1
[00e1f0d0] b680                      cmp.l     d0,d3
[00e1f0d2] 6e04                      bgt.s     $00E1F0D8
[00e1f0d4] 43ec 0008                 lea.l     8(a4),a1
[00e1f0d8] 41eb 0008                 lea.l     8(a3),a0
[00e1f0dc] 6100 e7c0                 bsr       $00E1D89E
[00e1f0e0] 7e00                      moveq.l   #0,d7
[00e1f0e2] bbcc                      cmpa.l    a4,a5
[00e1f0e4] 6718                      beq.s     $00E1F0FE
[00e1f0e6] 224c                      movea.l   a4,a1
[00e1f0e8] 204d                      movea.l   a5,a0
[00e1f0ea] 6118                      bsr.s     $00E1F104
[00e1f0ec] 6610                      bne.s     $00E1F0FE
[00e1f0ee] 9e7c 0010                 sub.w     #$0010,d7
[00e1f0f2] 41ed 0010                 lea.l     16(a5),a0
[00e1f0f6] 6002                      bra.s     $00E1F0FA
[00e1f0f8] 3ad8                      move.w    (a0)+,(a5)+
[00e1f0fa] b7cd                      cmpa.l    a5,a3
[00e1f0fc] 66fa                      bne.s     $00E1F0F8
[00e1f0fe] 4cdf 200a                 movem.l   (a7)+,d1/d3/a5
[00e1f102] 4e75                      rts
[00e1f104] 48e7 4018                 movem.l   d1/a3-a4,-(a7)
[00e1f108] 49e9 0008                 lea.l     8(a1),a4
[00e1f10c] 47e8 0008                 lea.l     8(a0),a3
[00e1f110] 2f2c 0004                 move.l    4(a4),-(a7)
[00e1f114] 2f14                      move.l    (a4),-(a7)
[00e1f116] 224f                      movea.l   a7,a1
[00e1f118] 204b                      movea.l   a3,a0
[00e1f11a] 6100 e782                 bsr       $00E1D89E
[00e1f11e] 302c 0004                 move.w    4(a4),d0
[00e1f122] c1ec 0006                 muls.w    6(a4),d0
[00e1f126] 322b 0004                 move.w    4(a3),d1
[00e1f12a] c3eb 0006                 muls.w    6(a3),d1
[00e1f12e] d280                      add.l     d0,d1
[00e1f130] 2001                      move.l    d1,d0
[00e1f132] e488                      lsr.l     #2,d0
[00e1f134] d280                      add.l     d0,d1
[00e1f136] 302f 0004                 move.w    4(a7),d0
[00e1f13a] c1ef 0006                 muls.w    6(a7),d0
[00e1f13e] b081                      cmp.l     d1,d0
[00e1f140] 6e0a                      bgt.s     $00E1F14C
[00e1f142] 2897                      move.l    (a7),(a4)
[00e1f144] 296f 0004 0004            move.l    4(a7),4(a4)
[00e1f14a] 7000                      moveq.l   #0,d0
[00e1f14c] 508f                      addq.l    #8,a7
[00e1f14e] 4cdf 1802                 movem.l   (a7)+,d1/a3-a4
[00e1f152] 4e75                      rts
[00e1f154] 2415                      move.l    (a5),d2
[00e1f156] b494                      cmp.l     (a4),d2
[00e1f158] 6742                      beq.s     $00E1F19C
[00e1f15a] 3215                      move.w    (a5),d1
[00e1f15c] 9254                      sub.w     (a4),d1
[00e1f15e] 946c 0002                 sub.w     2(a4),d2
[00e1f162] 3007                      move.w    d7,d0
[00e1f164] 6100 e65c                 bsr       $00E1D7C2
[00e1f168] 2068 0002                 movea.l   2(a0),a0
[00e1f16c] 41e8 0090                 lea.l     144(a0),a0
[00e1f170] 2250                      movea.l   (a0),a1
[00e1f172] d2e8 0004                 adda.w    4(a0),a1
[00e1f176] 2050                      movea.l   (a0),a0
[00e1f178] 6000 001e                 bra.w     $00E1F198
[00e1f17c] 0c50 0014                 cmpi.w    #$0014,(a0)
[00e1f180] 660e                      bne.s     $00E1F190
[00e1f182] b068 0006                 cmp.w     6(a0),d0
[00e1f186] 6608                      bne.s     $00E1F190
[00e1f188] d368 0008                 add.w     d1,8(a0)
[00e1f18c] d568 000a                 add.w     d2,10(a0)
[00e1f190] d0e8 0004                 adda.w    4(a0),a0
[00e1f194] d0fc 0010                 adda.w    #$0010,a0
[00e1f198] b3c8                      cmpa.l    a0,a1
[00e1f19a] 6ee0                      bgt.s     $00E1F17C
[00e1f19c] 4e75                      rts
[00e1f19e] 4a39 0000 94c6            tst.b     $000094C6
[00e1f1a4] 670c                      beq.s     $00E1F1B2
[00e1f1a6] 302e 0008                 move.w    8(a6),d0
[00e1f1aa] 5340                      subq.w    #1,d0
[00e1f1ac] 6d0a                      blt.s     $00E1F1B8
[00e1f1ae] 670c                      beq.s     $00E1F1BC
[00e1f1b0] 611c                      bsr.s     $00E1F1CE
[00e1f1b2] 4a6e 0008                 tst.w     8(a6)
[00e1f1b6] 4e75                      rts
[00e1f1b8] 6120                      bsr.s     $00E1F1DA
[00e1f1ba] 6002                      bra.s     $00E1F1BE
[00e1f1bc] 6134                      bsr.s     $00E1F1F2
[00e1f1be] 4e5e                      unlk      a6
[00e1f1c0] 4e75                      rts
[00e1f1c2] 6128                      bsr.s     $00E1F1EC
[00e1f1c4] 6000 f9a4                 bra       $00E1EB6A
[00e1f1c8] 6104                      bsr.s     $00E1F1CE
[00e1f1ca] 6000 f99a                 bra       $00E1EB66
[00e1f1ce] 41f9 0000 94c6            lea.l     $000094C6,a0
[00e1f1d4] 4a10                      tst.b     (a0)
[00e1f1d6] 67e8                      beq.s     $00E1F1C0
[00e1f1d8] 51d0                      sf        (a0)
[00e1f1da] 6100 270e                 bsr       $00E218EA
[00e1f1de] 4e71                      nop
[00e1f1e0] 6100 3706                 bsr       $00E228E8
[00e1f1e4] 4e71                      nop
[00e1f1e6] 6000 271e                 bra       $00E21906
[00e1f1ea] 4e71                      nop
[00e1f1ec] 50f9 0000 94c6            st        $000094C6
[00e1f1f2] 6100 36d6                 bsr       $00E228CA
[00e1f1f6] 4e71                      nop
[00e1f1f8] 41f9 00e3 5f20            lea.l     $00E35F20,a0
[00e1f1fe] 4868 007e                 pea.l     126(a0)
[00e1f202] 6100 b908                 bsr       $00E1AB0C
[00e1f206] 588f                      addq.l    #4,a7
[00e1f208] 4e75                      rts
[00e1f20a] 6114                      bsr.s     $00E1F220
[00e1f20c] 6710                      beq.s     $00E1F21E
[00e1f20e] 082d 0004 0001            btst      #4,1(a5)
[00e1f214] 67f4                      beq.s     $00E1F20A
[00e1f216] 4a39 0000 94c6            tst.b     $000094C6
[00e1f21c] 67ec                      beq.s     $00E1F20A
[00e1f21e] 4e75                      rts
[00e1f220] 4ef9 00e1 777e            jmp       $00E1777E
[00e1f226] 6100 fc68                 bsr       $00E1EE90
[00e1f22a] 48e7 5e1c                 movem.l   d1/d3-d6/a3-a5,-(a7)
[00e1f22e] 6100 ff92                 bsr.w     $00E1F1C2
[00e1f232] 3684                      move.w    d4,(a3)
[00e1f234] 7000                      moveq.l   #0,d0
[00e1f236] 6100 fb32                 bsr       $00E1ED6A
[00e1f23a] 670a                      beq.s     $00E1F246
[00e1f23c] 70ec                      moveq.l   #-20,d0
[00e1f23e] d084                      add.l     d4,d0
[00e1f240] d080                      add.l     d0,d0
[00e1f242] 6100 fb14                 bsr       $00E1ED58
[00e1f246] 6100 ffc2                 bsr.w     $00E1F20A
[00e1f24a] 670c                      beq.s     $00E1F258
[00e1f24c] bab9 0000 7188            cmp.l     $00007188,d5
[00e1f252] 64f2                      bcc.s     $00E1F246
[00e1f254] 70ff                      moveq.l   #-1,d0
[00e1f256] 60de                      bra.s     $00E1F236
[00e1f258] 6100 ff6e                 bsr       $00E1F1C8
[00e1f25c] 4cdf 387a                 movem.l   (a7)+,d1/d3-d6/a3-a5
[00e1f260] 4e75                      rts
[00e1f262] 6100 fc2c                 bsr       $00E1EE90
[00e1f266] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e1f26a] 2e2b 000a                 move.l    10(a3),d7
[00e1f26e] 2039 0000 7366            move.l    $00007366,d0
[00e1f274] 4840                      swap      d0
[00e1f276] 906b 000a                 sub.w     10(a3),d0
[00e1f27a] 4840                      swap      d0
[00e1f27c] 9047                      sub.w     d7,d0
[00e1f27e] 2a00                      move.l    d0,d5
[00e1f280] 6100 ff40                 bsr       $00E1F1C2
[00e1f284] 2039 0000 7366            move.l    $00007366,d0
[00e1f28a] 7401                      moveq.l   #1,d2
[00e1f28c] 4840                      swap      d0
[00e1f28e] 4846                      swap      d6
[00e1f290] 4845                      swap      d5
[00e1f292] 9045                      sub.w     d5,d0
[00e1f294] b046                      cmp.w     d6,d0
[00e1f296] 6c02                      bge.s     $00E1F29A
[00e1f298] 3006                      move.w    d6,d0
[00e1f29a] 51ca fff0                 dbf       d2,$00E1F28C
[00e1f29e] be80                      cmp.l     d0,d7
[00e1f2a0] 6712                      beq.s     $00E1F2B4
[00e1f2a2] 36bc 001b                 move.w    #$001B,(a3)
[00e1f2a6] 2740 000a                 move.l    d0,10(a3)
[00e1f2aa] 6100 fabc                 bsr       $00E1ED68
[00e1f2ae] 6704                      beq.s     $00E1F2B4
[00e1f2b0] 2e2b 000a                 move.l    10(a3),d7
[00e1f2b4] 6100 ff54                 bsr       $00E1F20A
[00e1f2b8] 66ca                      bne.s     $00E1F284
[00e1f2ba] 6100 ff0c                 bsr       $00E1F1C8
[00e1f2be] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e1f2c2] 4e75                      rts
[00e1f2c4] 6100 fbca                 bsr       $00E1EE90
[00e1f2c8] 45ee fff8                 lea.l     -8(a6),a2
[00e1f2cc] 48e7 471c                 movem.l   d1/d5-d7/a3-a5,-(a7)
[00e1f2d0] 284a                      movea.l   a2,a4
[00e1f2d2] 2c2b 0006                 move.l    6(a3),d6
[00e1f2d6] 2e2b 000a                 move.l    10(a3),d7
[00e1f2da] 2039 0000 7366            move.l    $00007366,d0
[00e1f2e0] 4840                      swap      d0
[00e1f2e2] 906b 0006                 sub.w     6(a3),d0
[00e1f2e6] 4840                      swap      d0
[00e1f2e8] 9046                      sub.w     d6,d0
[00e1f2ea] 2a00                      move.l    d0,d5
[00e1f2ec] 6100 fed4                 bsr       $00E1F1C2
[00e1f2f0] 2039 0000 7366            move.l    $00007366,d0
[00e1f2f6] 41ec 0000                 lea.l     0(a4),a0
[00e1f2fa] 7401                      moveq.l   #1,d2
[00e1f2fc] 4840                      swap      d0
[00e1f2fe] 4845                      swap      d5
[00e1f300] 4847                      swap      d7
[00e1f302] 9045                      sub.w     d5,d0
[00e1f304] 3210                      move.w    (a0),d1
[00e1f306] b041                      cmp.w     d1,d0
[00e1f308] 6c02                      bge.s     $00E1F30C
[00e1f30a] 3001                      move.w    d1,d0
[00e1f30c] d268 0004                 add.w     4(a0),d1
[00e1f310] 9247                      sub.w     d7,d1
[00e1f312] b041                      cmp.w     d1,d0
[00e1f314] 6f02                      ble.s     $00E1F318
[00e1f316] 3001                      move.w    d1,d0
[00e1f318] 5488                      addq.l    #2,a0
[00e1f31a] 51ca ffe0                 dbf       d2,$00E1F2FC
[00e1f31e] bc80                      cmp.l     d0,d6
[00e1f320] 6712                      beq.s     $00E1F334
[00e1f322] 36bc 001c                 move.w    #$001C,(a3)
[00e1f326] 2740 0006                 move.l    d0,6(a3)
[00e1f32a] 6100 fa3c                 bsr       $00E1ED68
[00e1f32e] 6704                      beq.s     $00E1F334
[00e1f330] 2c2b 0006                 move.l    6(a3),d6
[00e1f334] 6100 fed4                 bsr       $00E1F20A
[00e1f338] 66b6                      bne.s     $00E1F2F0
[00e1f33a] 6100 fe8c                 bsr       $00E1F1C8
[00e1f33e] 4cdf 38e2                 movem.l   (a7)+,d1/d5-d7/a3-a5
[00e1f342] 4e75                      rts
[00e1f344] 2079 0000 cb08            movea.l   $0000CB08,a0
[00e1f34a] 2008                      move.l    a0,d0
[00e1f34c] 67fe                      beq.s     $00E1F34C
[00e1f34e] 23d0 0000 cb08            move.l    (a0),$0000CB08
[00e1f354] 4e75                      rts
[00e1f356] c1fc 0122                 muls.w    #$0122,d0
[00e1f35a] 2079 0000 94b2            movea.l   $000094B2,a0
[00e1f360] d1c0                      adda.l    d0,a0
[00e1f362] 4e75                      rts
[00e1f364] 2248                      movea.l   a0,a1
[00e1f366] 6002                      bra.s     $00E1F36A
[00e1f368] 2251                      movea.l   (a1),a1
[00e1f36a] 2011                      move.l    (a1),d0
[00e1f36c] 66fa                      bne.s     $00E1F368
[00e1f36e] 22b9 0000 cb08            move.l    $0000CB08,(a1)
[00e1f374] 23d0 0000 cb08            move.l    (a0),$0000CB08
[00e1f37a] 4290                      clr.l     (a0)
[00e1f37c] 4e75                      rts
[00e1f37e] 2050                      movea.l   (a0),a0
[00e1f380] 603e                      bra.s     $00E1F3C0
[00e1f382] 2248                      movea.l   a0,a1
[00e1f384] 6032                      bra.s     $00E1F3B8
[00e1f386] 2028 0004                 move.l    4(a0),d0
[00e1f38a] 2228 0008                 move.l    8(a0),d1
[00e1f38e] d041                      add.w     d1,d0
[00e1f390] b0aa 0004                 cmp.l     4(a2),d0
[00e1f394] 6620                      bne.s     $00E1F3B6
[00e1f396] 4841                      swap      d1
[00e1f398] b26a 0008                 cmp.w     8(a2),d1
[00e1f39c] 6618                      bne.s     $00E1F3B6
[00e1f39e] 302a 000a                 move.w    10(a2),d0
[00e1f3a2] d168 000a                 add.w     d0,10(a0)
[00e1f3a6] 2292                      move.l    (a2),(a1)
[00e1f3a8] 24b9 0000 cb08            move.l    $0000CB08,(a2)
[00e1f3ae] 23ca 0000 cb08            move.l    a2,$0000CB08
[00e1f3b4] 6002                      bra.s     $00E1F3B8
[00e1f3b6] 224a                      movea.l   a2,a1
[00e1f3b8] 2451                      movea.l   (a1),a2
[00e1f3ba] 200a                      move.l    a2,d0
[00e1f3bc] 66c8                      bne.s     $00E1F386
[00e1f3be] 2050                      movea.l   (a0),a0
[00e1f3c0] 2008                      move.l    a0,d0
[00e1f3c2] 66be                      bne.s     $00E1F382
[00e1f3c4] 4e75                      rts
[00e1f3c6] 48e7 181e                 movem.l   d3-d4/a3-a6,-(a7)
[00e1f3ca] 2f0a                      move.l    a2,-(a7)
[00e1f3cc] 514f                      subq.w    #8,a7
[00e1f3ce] 2648                      movea.l   a0,a3
[00e1f3d0] 2c49                      movea.l   a1,a6
[00e1f3d2] 7600                      moveq.l   #0,d3
[00e1f3d4] 2879 0000 cb08            movea.l   $0000CB08,a4
[00e1f3da] 6000 00ae                 bra       $00E1F48A
[00e1f3de] 41ed 0004                 lea.l     4(a5),a0
[00e1f3e2] 43d7                      lea.l     (a7),a1
[00e1f3e4] 22d8                      move.l    (a0)+,(a1)+
[00e1f3e6] 22d8                      move.l    (a0)+,(a1)+
[00e1f3e8] 43d7                      lea.l     (a7),a1
[00e1f3ea] 206f 0008                 movea.l   8(a7),a0
[00e1f3ee] 6100 e46c                 bsr       $00E1D85C
[00e1f3f2] 4a40                      tst.w     d0
[00e1f3f4] 6606                      bne.s     $00E1F3FC
[00e1f3f6] 264d                      movea.l   a5,a3
[00e1f3f8] 6000 0090                 bra       $00E1F48A
[00e1f3fc] 7601                      moveq.l   #1,d3
[00e1f3fe] 302f 0002                 move.w    2(a7),d0
[00e1f402] 906d 0006                 sub.w     6(a5),d0
[00e1f406] 6f0e                      ble.s     $00E1F416
[00e1f408] 6100 009e                 bsr       $00E1F4A8
[00e1f40c] 20ed 0004                 move.l    4(a5),(a0)+
[00e1f410] 30ed 0008                 move.w    8(a5),(a0)+
[00e1f414] 3080                      move.w    d0,(a0)
[00e1f416] 3217                      move.w    (a7),d1
[00e1f418] 926d 0004                 sub.w     4(a5),d1
[00e1f41c] 6f12                      ble.s     $00E1F430
[00e1f41e] 6100 0088                 bsr       $00E1F4A8
[00e1f422] 30ed 0004                 move.w    4(a5),(a0)+
[00e1f426] 30ef 0002                 move.w    2(a7),(a0)+
[00e1f42a] 30c1                      move.w    d1,(a0)+
[00e1f42c] 30af 0006                 move.w    6(a7),(a0)
[00e1f430] 3417                      move.w    (a7),d2
[00e1f432] d46f 0004                 add.w     4(a7),d2
[00e1f436] 322d 0004                 move.w    4(a5),d1
[00e1f43a] d26d 0008                 add.w     8(a5),d1
[00e1f43e] 9242                      sub.w     d2,d1
[00e1f440] 6f0e                      ble.s     $00E1F450
[00e1f442] 6164                      bsr.s     $00E1F4A8
[00e1f444] 30c2                      move.w    d2,(a0)+
[00e1f446] 30ef 0002                 move.w    2(a7),(a0)+
[00e1f44a] 30c1                      move.w    d1,(a0)+
[00e1f44c] 30af 0006                 move.w    6(a7),(a0)
[00e1f450] 342f 0002                 move.w    2(a7),d2
[00e1f454] d46f 0006                 add.w     6(a7),d2
[00e1f458] 302d 0006                 move.w    6(a5),d0
[00e1f45c] d06d 000a                 add.w     10(a5),d0
[00e1f460] 9042                      sub.w     d2,d0
[00e1f462] 6f0e                      ble.s     $00E1F472
[00e1f464] 6142                      bsr.s     $00E1F4A8
[00e1f466] 30ed 0004                 move.w    4(a5),(a0)+
[00e1f46a] 30c2                      move.w    d2,(a0)+
[00e1f46c] 30ed 0008                 move.w    8(a5),(a0)+
[00e1f470] 3080                      move.w    d0,(a0)
[00e1f472] 2695                      move.l    (a5),(a3)
[00e1f474] 200e                      move.l    a6,d0
[00e1f476] 670e                      beq.s     $00E1F486
[00e1f478] 2c8d                      move.l    a5,(a6)
[00e1f47a] 2c4d                      movea.l   a5,a6
[00e1f47c] 41d7                      lea.l     (a7),a0
[00e1f47e] 429d                      clr.l     (a5)+
[00e1f480] 2ad8                      move.l    (a0)+,(a5)+
[00e1f482] 2ad8                      move.l    (a0)+,(a5)+
[00e1f484] 6004                      bra.s     $00E1F48A
[00e1f486] 2a8c                      move.l    a4,(a5)
[00e1f488] 284d                      movea.l   a5,a4
[00e1f48a] 2a53                      movea.l   (a3),a5
[00e1f48c] 200d                      move.l    a5,d0
[00e1f48e] 6600 ff4e                 bne       $00E1F3DE
[00e1f492] 23cc 0000 cb08            move.l    a4,$0000CB08
[00e1f498] 200e                      move.l    a6,d0
[00e1f49a] 6602                      bne.s     $00E1F49E
[00e1f49c] 2003                      move.l    d3,d0
[00e1f49e] defc 000c                 adda.w    #$000C,a7
[00e1f4a2] 4cdf 7818                 movem.l   (a7)+,d3-d4/a3-a6
[00e1f4a6] 4e75                      rts
[00e1f4a8] 204c                      movea.l   a4,a0
[00e1f4aa] 2688                      move.l    a0,(a3)
[00e1f4ac] 67fe                      beq.s     $00E1F4AC
[00e1f4ae] 2648                      movea.l   a0,a3
[00e1f4b0] 2858                      movea.l   (a0)+,a4
[00e1f4b2] 4e75                      rts
[00e1f4b4] 48e7 180c                 movem.l   d3-d4/a4-a5,-(a7)
[00e1f4b8] 3600                      move.w    d0,d3
[00e1f4ba] 3003                      move.w    d3,d0
[00e1f4bc] 4eba fe98                 jsr       $00E1F356(pc)
[00e1f4c0] 2848                      movea.l   a0,a4
[00e1f4c2] 41ec 009e                 lea.l     158(a4),a0
[00e1f4c6] 4eba fe9c                 jsr       $00E1F364(pc)
[00e1f4ca] 3014                      move.w    (a4),d0
[00e1f4cc] 0254 fff9                 andi.w    #$FFF9,(a4)
[00e1f4d0] 0800 0001                 btst      #1,d0
[00e1f4d4] 6704                      beq.s     $00E1F4DA
[00e1f4d6] 0054 0004                 ori.w     #$0004,(a4)
[00e1f4da] 0800 0003                 btst      #3,d0
[00e1f4de] 6756                      beq.s     $00E1F536
[00e1f4e0] 486c 0028                 pea.l     40(a4)
[00e1f4e4] 4eba fe5e                 jsr       $00E1F344(pc)
[00e1f4e8] 2948 009e                 move.l    a0,158(a4)
[00e1f4ec] 4298                      clr.l     (a0)+
[00e1f4ee] 2a48                      movea.l   a0,a5
[00e1f4f0] 225f                      movea.l   (a7)+,a1
[00e1f4f2] 20d9                      move.l    (a1)+,(a0)+
[00e1f4f4] 2099                      move.l    (a1)+,(a0)
[00e1f4f6] 5458                      addq.w    #2,(a0)+
[00e1f4f8] 5450                      addq.w    #2,(a0)
[00e1f4fa] 7000                      moveq.l   #0,d0
[00e1f4fc] 6100 fe58                 bsr       $00E1F356
[00e1f500] 2848                      movea.l   a0,a4
[00e1f502] 2068 0036                 movea.l   54(a0),a0
[00e1f506] 3828 0030                 move.w    48(a0),d4
[00e1f50a] 6024                      bra.s     $00E1F530
[00e1f50c] 244d                      movea.l   a5,a2
[00e1f50e] 93c9                      suba.l    a1,a1
[00e1f510] 41ec 009e                 lea.l     158(a4),a0
[00e1f514] 4eba feb0                 jsr       $00E1F3C6(pc)
[00e1f518] 4a80                      tst.l     d0
[00e1f51a] 6704                      beq.s     $00E1F520
[00e1f51c] 0054 0002                 ori.w     #$0002,(a4)
[00e1f520] b644                      cmp.w     d4,d3
[00e1f522] 6608                      bne.s     $00E1F52C
[00e1f524] 41ec 009e                 lea.l     158(a4),a0
[00e1f528] 4eba fe54                 jsr       $00E1F37E(pc)
[00e1f52c] 286c 0032                 movea.l   50(a4),a4
[00e1f530] b66c 0030                 cmp.w     48(a4),d3
[00e1f534] 66d6                      bne.s     $00E1F50C
[00e1f536] 4cdf 3018                 movem.l   (a7)+,d3-d4/a4-a5
[00e1f53a] 4e75                      rts
[00e1f53c] 48e7 1c00                 movem.l   d3-d5,-(a7)
[00e1f540] 3429 0006                 move.w    6(a1),d2
[00e1f544] 3a28 0006                 move.w    6(a0),d5
[00e1f548] da68 000a                 add.w     10(a0),d5
[00e1f54c] b445                      cmp.w     d5,d2
[00e1f54e] 5cc5                      sge       d5
[00e1f550] d469 000a                 add.w     10(a1),d2
[00e1f554] b468 0006                 cmp.w     6(a0),d2
[00e1f558] 5fc4                      sle       d4
[00e1f55a] 4a41                      tst.w     d1
[00e1f55c] 6e02                      bgt.s     $00E1F560
[00e1f55e] c945                      exg       d4,d5
[00e1f560] 4a05                      tst.b     d5
[00e1f562] 6620                      bne.s     $00E1F584
[00e1f564] 3429 0004                 move.w    4(a1),d2
[00e1f568] 3628 0004                 move.w    4(a0),d3
[00e1f56c] 4a40                      tst.w     d0
[00e1f56e] 6e08                      bgt.s     $00E1F578
[00e1f570] d469 0008                 add.w     8(a1),d2
[00e1f574] b642                      cmp.w     d2,d3
[00e1f576] 6006                      bra.s     $00E1F57E
[00e1f578] d668 0008                 add.w     8(a0),d3
[00e1f57c] b443                      cmp.w     d3,d2
[00e1f57e] 6d08                      blt.s     $00E1F588
[00e1f580] 4a04                      tst.b     d4
[00e1f582] 6604                      bne.s     $00E1F588
[00e1f584] 7001                      moveq.l   #1,d0
[00e1f586] 6002                      bra.s     $00E1F58A
[00e1f588] 4240                      clr.w     d0
[00e1f58a] 4cdf 0038                 movem.l   (a7)+,d3-d5
[00e1f58e] 4e75                      rts
[00e1f590] 48e7 181e                 movem.l   d3-d4/a3-a6,-(a7)
[00e1f594] 2648                      movea.l   a0,a3
[00e1f596] 3800                      move.w    d0,d4
[00e1f598] 3601                      move.w    d1,d3
[00e1f59a] 2850                      movea.l   (a0),a4
[00e1f59c] 240c                      move.l    a4,d2
[00e1f59e] 673e                      beq.s     $00E1F5DE
[00e1f5a0] 6036                      bra.s     $00E1F5D8
[00e1f5a2] 3203                      move.w    d3,d1
[00e1f5a4] 3004                      move.w    d4,d0
[00e1f5a6] 224d                      movea.l   a5,a1
[00e1f5a8] 204c                      movea.l   a4,a0
[00e1f5aa] 4eba ff90                 jsr       $00E1F53C(pc)
[00e1f5ae] 4a40                      tst.w     d0
[00e1f5b0] 6724                      beq.s     $00E1F5D6
[00e1f5b2] 2c4b                      movea.l   a3,a6
[00e1f5b4] 6012                      bra.s     $00E1F5C8
[00e1f5b6] 3203                      move.w    d3,d1
[00e1f5b8] 3004                      move.w    d4,d0
[00e1f5ba] 224d                      movea.l   a5,a1
[00e1f5bc] 2056                      movea.l   (a6),a0
[00e1f5be] 4eba ff7c                 jsr       $00E1F53C(pc)
[00e1f5c2] 4a40                      tst.w     d0
[00e1f5c4] 6606                      bne.s     $00E1F5CC
[00e1f5c6] 2c56                      movea.l   (a6),a6
[00e1f5c8] b9d6                      cmpa.l    (a6),a4
[00e1f5ca] 66ea                      bne.s     $00E1F5B6
[00e1f5cc] 2054                      movea.l   (a4),a0
[00e1f5ce] 2890                      move.l    (a0),(a4)
[00e1f5d0] 2096                      move.l    (a6),(a0)
[00e1f5d2] 2c88                      move.l    a0,(a6)
[00e1f5d4] 6002                      bra.s     $00E1F5D8
[00e1f5d6] 284d                      movea.l   a5,a4
[00e1f5d8] 2a54                      movea.l   (a4),a5
[00e1f5da] 200d                      move.l    a5,d0
[00e1f5dc] 66c4                      bne.s     $00E1F5A2
[00e1f5de] 4cdf 7818                 movem.l   (a7)+,d3-d4/a3-a6
[00e1f5e2] 4e75                      rts
[00e1f5e4] 45f9 0000 cb08            lea.l     $0000CB08,a2
[00e1f5ea] 2092                      move.l    (a2),(a0)
[00e1f5ec] 6014                      bra.s     $00E1F602
[00e1f5ee] 2010                      move.l    (a0),d0
[00e1f5f0] 67fe                      beq.s     $00E1F5F0
[00e1f5f2] 2040                      movea.l   d0,a0
[00e1f5f4] 2169 0004 0004            move.l    4(a1),4(a0)
[00e1f5fa] 2169 0008 0008            move.l    8(a1),8(a0)
[00e1f600] 2251                      movea.l   (a1),a1
[00e1f602] 2009                      move.l    a1,d0
[00e1f604] 66e8                      bne.s     $00E1F5EE
[00e1f606] 2490                      move.l    (a0),(a2)
[00e1f608] 4290                      clr.l     (a0)
[00e1f60a] 4e75                      rts
[00e1f60c] 4afc                      illegal
[00e1f60e] 4afc                      illegal
[00e1f610] 4afc                      illegal
[00e1f612] 4afc                      illegal
[00e1f614] 4afc                      illegal
[00e1f616] 4afc                      illegal
[00e1f618] 4afc                      illegal
[00e1f61a] 4afc                      illegal
[00e1f61c] 4afc                      illegal
[00e1f61e] 4afc                      illegal
[00e1f620] 4afc                      illegal
[00e1f622] 4afc                      illegal
[00e1f624] 4afc                      illegal
[00e1f626] 4afc                      illegal
[00e1f628] 4afc                      illegal
[00e1f62a] 4afc                      illegal
[00e1f62c] 4afc                      illegal
[00e1f62e] 4afc                      illegal
[00e1f630] 4afc                      illegal
[00e1f632] 4afc                      illegal
[00e1f634] 4afc                      illegal
[00e1f636] 4afc                      illegal
[00e1f638] 4afc                      illegal
[00e1f63a] 4afc                      illegal
[00e1f63c] 4afc                      illegal
