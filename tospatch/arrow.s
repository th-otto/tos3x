/* in iasync: */
[00e165da] 4eb9 00e1 83c4            jsr       $00E183C4 /* ev_mchk */
[00e165e0] 508f                      addq.l    #8,a7
[00e165e2] 4a40                      tst.w     d0
[00e165e4] 4e71                      nop
[00e165e6] 4e71                      nop
[00e165e8] 4e71                      nop
[00e165ea] 4e71                      nop
[00e165ec] 4e71                      nop
[00e165ee] 4e71                      nop
[00e165f0] 4e71                      nop
[00e165f2] 4e71                      nop
[00e165f4] 4e71                      nop
[00e165f6] 4e71                      nop

/* in hctl_window: */
[00e17a0e] 7600                      moveq.l   #0,d3
[00e17a10] 1639 0000 74de            move.b    $000074DE,d3 /* gl_dcindex */
[00e17a16] 5383                      subq.l    #1,d3
[00e17a18] ef83                      asl.l     #7,d3
[00e17a1a] 87f9 0000 75da            divs.w    $000075DA,d3 /* gl_ticktime */
[00e17a20] 48c3                      ext.l     d3
[00e17a22] d6b9 0000 7188            add.l     $00007188,d3 /* TICKS */
[00e17a28] 600a                      bra.s     $00E17A34
[00e17a2a] 206d 0002                 movea.l   2(a5),a0
[00e17a2e] 4a68 0094                 tst.w     148(a0)
[00e17a32] 6e52                      bgt.s     $00E17A86
[00e17a34] 206d 0002                 movea.l   2(a5),a0
[00e17a38] 43e8 0026                 lea.l     38(a0),a1
[00e17a3c] 0828 0001 001f            btst      #1,31(a0)
[00e17a42] 6724                      beq.s     $00E17A68
[00e17a44] 4251                      clr.w     (a1)
[00e17a46] 4267                      clr.w     -(a7)
[00e17a48] 42a7                      clr.l     -(a7)
[00e17a4a] 3f2e ffee                 move.w    -18(a6),-(a7)
[00e17a4e] 3f07                      move.w    d7,-(a7)
[00e17a50] 3f28 001c                 move.w    28(a0),-(a7)
[00e17a54] 3f04                      move.w    d4,-(a7)
[00e17a56] 4879 0000 7a6c            pea.l     $00007A6C
[00e17a5c] 4eb9 00e1 e344            jsr       $00E1E344
[00e17a62] defc 0012                 adda.w    #$0012,a7
[00e17a66] 601e                      bra.s     $00E17A86
[00e17a68] 4a51                      tst.w     (a1)
[00e17a6a] 661a                      bne.s     $00E17A86
[00e17a6c] 5259                      addq.w    #1,(a1)+
[00e17a6e] 32c4                      move.w    d4,(a1)+
[00e17a70] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e17a76] 32e8 001c                 move.w    28(a0),(a1)+
[00e17a7a] 4259                      clr.w     (a1)+
[00e17a7c] 32c7                      move.w    d7,(a1)+
[00e17a7e] 32ee ffee                 move.w    -18(a6),(a1)+
[00e17a82] 4299                      clr.l     (a1)+
[00e17a84] 4251                      clr.w     (a1)
[00e17a86] 4eb9 00e2 1e44            jsr       $00E21E44
[00e17a8c] 0839 0000 0000 ccb1       btst      #0,$0000CCB1
[00e17a94] 670a                      beq.s     $00E17AA0
[00e17a96] b6b9 0000 7188            cmp.l     $00007188,d3 /* TICKS */
[00e17a9c] 64e8                      bcc.s     $00E17A86
[00e17a9e] 608a                      bra.s     $00E17A2A
[00e17aa0] 4e71                      nop
[00e17aa2] 4e71                      nop
[00e17aa4] 4e71                      nop
[00e17aa6] 4e71                      nop
[00e17aa8] 4e71                      nop
[00e17aaa] 4e71                      nop
[00e17aac] 4e71                      nop
[00e17aae] 4e71                      nop
[00e17ab0] 4e71                      nop
[00e17ab2] 4e71                      nop
[00e17ab4] 4e71                      nop
[00e17ab6] 4e71                      nop
[00e17ab8] 4e71                      nop
[00e17aba] 4e71                      nop
[00e17abc] 4e71                      nop
[00e17abe] 4e71                      nop
[00e17ac0] 4e71                      nop
[00e17ac2] 4e71                      nop
[00e17ac4] 4e71                      nop
[00e17ac6] 4e71                      nop
[00e17ac8] 4e71                      nop
[00e17aca] 4e71                      nop
[00e17acc] 4e71                      nop
[00e17ace] 4e71                      nop
[00e17ad0] 4e71                      nop
[00e17ad2] 4e71                      nop
[00e17ad4] 4e71                      nop
[00e17ad6] 4e71                      nop
[00e17ad8] 4e71                      nop
[00e17ada] 4e71                      nop

/* ev_mesag: */
[00e18362] 2f2f 0004                 move.l    4(a7),-(a7)
[00e18366] 3f3c 0010                 move.w    #$0010,-(a7)
[00e1836a] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e18370] 3f28 001c                 move.w    28(a0),-(a7)
[00e18374] 3f3c 0001                 move.w    #$0001,-(a7)
[00e18378] 4a68 0094                 tst.w     148(a0)
[00e1837c] 6e0e                      bgt.s     $00E1838C
[00e1837e] 2f2f 000e                 move.l    14(a7),-(a7)
[00e18382] 6100 dcd2                 bsr       $00E16056
[00e18386] 588f                      addq.l    #4,a7
[00e18388] 4a40                      tst.w     d0
[00e1838a] 6604                      bne.s     $00E18390
[00e1838c] 6100 dd0a                 bsr       $00E16098
[00e18390] defc 000a                 adda.w    #$000A,a7
[00e18394] 4e75                      rts

/* ev_dclick: */
[00e18752] 41f9 0000 74df            lea.l     $000074DF,a0 /* gl_dcindex+1 */
[00e18758] 4240                      clr.w     d0
[00e1875a] 1010                      move.b    (a0),d0
[00e1875c] 43ef 0006                 lea.l     6(a7),a1
[00e18760] 4a51                      tst.w     (a1)
[00e18762] 6726                      beq.s     $00E1878A
[00e18764] 1021                      move.b    -(a1),d0
[00e18766] 1080                      move.b    d0,(a0)
[00e18768] 4eb9 00e2 2894            jsr       $00E22894
[00e1876e] 7200                      moveq.l   #0,d1
[00e18770] 1200                      move.b    d0,d1
[00e18772] 43f9 00e3 5c6a            lea.l     $00E35C6A,a1
[00e18778] d2c1                      adda.w    d1,a1
[00e1877a] d2c1                      adda.w    d1,a1
[00e1877c] 3211                      move.w    (a1),d1
[00e1877e] 83f9 0000 75da            divs.w    $000075DA,d1 /* gl_ticktime */
[00e18784] 33c1 0000 7490            move.w    d1,$00007490
[00e1878a] 4e75                      rts
[00e1878c] 4e75                      rts

/* in gem_main: */
1A5B4 $.TIME+1.B,03

/* in downorup: patches call in iasync */
[00e1aed2] 102f 0009                 move.b    9(a7),d0
[00e1aed6] 122f 0005                 move.b    5(a7),d1
[00e1aeda] b300                      eor.b     d1,d0
[00e1aedc] c02f 0008                 and.b     8(a7),d0
[00e1aee0] 57c0                      seq       d0
[00e1aee2] c03c 0001                 and.b     #$01,d0
[00e1aee6] b02f 0006                 cmp.b     6(a7),d0
[00e1aeea] 56c0                      sne       d0
[00e1aeec] c07c 0001                 and.w     #$0001,d0
[00e1aef0] 671a                      beq.s     $00E1AF0C
[00e1aef2] 2057                      movea.l   (a7),a0
[00e1aef4] 0c68 2e87 fff2            cmpi.w    #$2E87,-14(a0)
[00e1aefa] 6610                      bne.s     $00E1AF0C
[00e1aefc] 2239 0000 cb00            move.l    $0000CB00,d1
[00e1af02] b2b9 0000 7ac0            cmp.l     $00007AC0,d1
[00e1af08] 6702                      beq.s     $00E1AF0C
[00e1af0a] 7000                      moveq.l   #0,d0
[00e1af0c] 4e75                      rts

/* in far_bchange: */
22752 $4E,71,4E,71,4E,71,4E,71

/* delay overwritten; not used anymore */
[00e22894] 5389                      subq.l    #1,a1
[00e22896] 5388                      subq.l    #1,a0
[00e22898] 1211                      move.b    (a1),d1
[00e2289a] 6706                      beq.s     $00E228A2
[00e2289c] 6b02                      bmi.s     $00E228A0
[00e2289e] 1081                      move.b    d1,(a0)
[00e228a0] 3010                      move.w    (a0),d0
[00e228a2] 4e75                      rts

/* in desk/do_scroll: */
[00e2a9e8] 48e7 400c                 movem.l   d1/a4-a5,-(a7)
[00e2a9ec] 286f 0010                 movea.l   16(a7),a4
[00e2a9f0] 3eac 0006                 move.w    6(a4),(a7)
[00e2a9f4] 4eb9 00e3 1db0            jsr       $00E31DB0
[00e2a9fa] 2a40                      movea.l   d0,a5
[00e2a9fc] 200d                      move.l    a5,d0
[00e2a9fe] 6700 008e                 beq       $00E2AA8E
[00e2aa02] 302c 0008                 move.w    8(a4),d0
[00e2aa06] b07c 0007                 cmp.w     #$0007,d0
[00e2aa0a] 6200 0082                 bhi       $00E2AA8E
[00e2aa0e] d040                      add.w     d0,d0
[00e2aa10] 303b 0006                 move.w    $00E2AA18(pc,d0.w),d0
[00e2aa14] 4efb 0002                 jmp       $00E2AA18(pc,d0.w)
J1:
[00e2aa18] 0010                      dc.w $0010   ; $00e2aa28-$00e2aa18
[00e2aa1a] 0014                      dc.w $0014   ; $00e2aa2c-$00e2aa18
[00e2aa1c] 001c                      dc.w $001c   ; $00e2aa34-$00e2aa18
[00e2aa1e] 0020                      dc.w $0020   ; $00e2aa38-$00e2aa18
[00e2aa20] 0034                      dc.w $0034   ; $00e2aa4c-$00e2aa18
[00e2aa22] 0038                      dc.w $0038   ; $00e2aa50-$00e2aa18
[00e2aa24] 0040                      dc.w $0040   ; $00e2aa58-$00e2aa18
[00e2aa26] 0044                      dc.w $0044   ; $00e2aa5c-$00e2aa18
[00e2aa28] 7200                      moveq.l   #0,d1
[00e2aa2a] 6002                      bra.s     $00E2AA2E
[00e2aa2c] 7201                      moveq.l   #1,d1
[00e2aa2e] 302d 008c                 move.w    140(a5),d0
[00e2aa32] 6008                      bra.s     $00E2AA3C
[00e2aa34] 7200                      moveq.l   #0,d1
[00e2aa36] 6002                      bra.s     $00E2AA3A
[00e2aa38] 7201                      moveq.l   #1,d1
[00e2aa3a] 7001                      moveq.l   #1,d0
[00e2aa3c] 3f01                      move.w    d1,-(a7)
[00e2aa3e] 3f00                      move.w    d0,-(a7)
[00e2aa40] 2f0d                      move.l    a5,-(a7)
[00e2aa42] 4eb9 00e3 1098            jsr       $00E31098
[00e2aa48] 508f                      addq.l    #8,a7
[00e2aa4a] 6022                      bra.s     $00E2AA6E
[00e2aa4c] 7203                      moveq.l   #3,d1
[00e2aa4e] 6002                      bra.s     $00E2AA52
[00e2aa50] 7202                      moveq.l   #2,d1
[00e2aa52] 302d 008a                 move.w    138(a5),d0
[00e2aa56] 6008                      bra.s     $00E2AA60
[00e2aa58] 7203                      moveq.l   #3,d1
[00e2aa5a] 6002                      bra.s     $00E2AA5E
[00e2aa5c] 7202                      moveq.l   #2,d1
[00e2aa5e] 7001                      moveq.l   #1,d0
[00e2aa60] 3f01                      move.w    d1,-(a7)
[00e2aa62] 3f00                      move.w    d0,-(a7)
[00e2aa64] 2f0d                      move.l    a5,-(a7)
[00e2aa66] 4eb9 00e3 1140            jsr       $00E31140
[00e2aa6c] 508f                      addq.l    #8,a7
[00e2aa6e] 6124                      bsr.s     $00E2AA94
[00e2aa70] 661c                      bne.s     $00E2AA8E
[00e2aa72] 4257                      clr.w     (a7)
[00e2aa74] 4eb9 00e3 2504            jsr       $00E32504
[00e2aa7a] 5257                      addq.w    #1,(a7)
[00e2aa7c] 4eb9 00e1 f1be            jsr       $00E1F1BE
[00e2aa82] 2e8c                      move.l    a4,(a7)
[00e2aa84] 4eb9 00e1 8362            jsr       $00E18362
[00e2aa8a] 6000 ff64                 bra       $00E2A9F0
[00e2aa8e] 4cdf 3002                 movem.l   (a7)+,d1/a4-a5
[00e2aa92] 4e75                      rts
[00e2aa94] 2079 0000 cb00            movea.l   $0000CB00,a0
[00e2aa9a] 2268 0090                 movea.l   144(a0),a1
[00e2aa9e] 4a68 0094                 tst.w     148(a0)
[00e2aaa2] 6608                      bne.s     $00E2AAAC
[00e2aaa4] 43e8 0026                 lea.l     38(a0),a1
[00e2aaa8] 4a59                      tst.w     (a1)+
[00e2aaaa] 6706                      beq.s     $00E2AAB2
[00e2aaac] 0c51 0018                 cmpi.w    #$0018,(a1)
[00e2aab0] 4e75                      rts
[00e2aab2] 7001                      moveq.l   #1,d0
[00e2aab4] 4e75                      rts
