; ph_branch = 0x601a
; ph_tlen = 0x000058d6
; ph_dlen = 0x000006f0
; ph_blen = 0x00002552
; ph_slen = 0x00000896
; ph_res1 = 0x00000000
; ph_prgflags = 0x00000007
; ph_absflag = 0x0000
; first relocation = 0x00000008
; relocation bytes = 0x0000061b

[00000000] __start:
[00000000] 224f                      movea.l   a7,a1
[00000002] 206f 0004                 movea.l   4(a7),a0
[00000006] 23c8 0000 5fca            move.l    a0,__base
[0000000c] 2028 0018                 move.l    24(a0),d0
[00000010] d0a8 001c                 add.l     28(a0),d0
[00000014] 23c0 0000 5fce            move.l    d0,__break
[0000001a] 2e7c 0000 1000            movea.l   #$00001000,a7
[00000020] dfc0                      adda.l    d0,a7
[00000022] 2200                      move.l    d0,d1
[00000024] d2bc 0000 1000            add.l     #$00001000,d1
[0000002a] bfc1                      cmpa.l    d1,a7
[0000002c] 6200 0004                 bhi       $00000032
[00000030] 2e41                      movea.l   d1,a7
[00000032] 2428 0004                 move.l    4(a0),d2
[00000036] 94bc 0000 0200            sub.l     #$00000200,d2
[0000003c] bfc2                      cmpa.l    d2,a7
[0000003e] 6500 000a                 bcs       $0000004A
[00000042] 2e42                      movea.l   d2,a7
[00000044] b481                      cmp.l     d1,d2
[00000046] 6500 004a                 bcs       $00000092
[0000004a] 220f                      move.l    a7,d1
[0000004c] c2bc ffff fffe            and.l     #$FFFFFFFE,d1
[00000052] 2e41                      movea.l   d1,a7
[00000054] 9288                      sub.l     a0,d1
[00000056] 2f01                      move.l    d1,-(a7)
[00000058] 2f08                      move.l    a0,-(a7)
[0000005a] 4267                      clr.w     -(a7)
[0000005c] 3f3c 004a                 move.w    #$004A,-(a7)
[00000060] 4e41                      trap      #1
[00000062] dffc 0000 000c            adda.l    #$0000000C,a7
[00000068] 2079 0000 5fca            movea.l   __base,a0
[0000006e] 45e8 0080                 lea.l     128(a0),a2
[00000072] 101a                      move.b    (a2)+,d0
[00000074] 4880                      ext.w     d0
[00000076] 3f00                      move.w    d0,-(a7)
[00000078] 2f0a                      move.l    a2,-(a7)
[0000007a] 9dce                      suba.l    a6,a6
[0000007c] 4eb9 0000 583e            jsr       __main
[00000082] 3f00                      move.w    d0,-(a7)
[00000084] 4eb9 0000 008a            jsr       __exit
[0000008a] __exit:
[0000008a] 4a9f                      tst.l     (a7)+
[0000008c] 3f3c 004c                 move.w    #$004C,-(a7)
[00000090] 4e41                      trap      #1
[00000092] 2e49                      movea.l   a1,a7
[00000094] 4879 0000 58fa            pea.l     $000058FA
[0000009a] 3f3c 0009                 move.w    #$0009,-(a7)
[0000009e] 4e41                      trap      #1
[000000a0] 5c8f                      addq.l    #6,a7
[000000a2] 3f3c ffd9                 move.w    #$FFD9,-(a7)
[000000a6] 4eb9 0000 008a            jsr       __exit
[000000ac] _brk:
[000000ac] bff9 0000 5fce            cmpa.l    __break,a7
[000000b2] 6500 007a                 bcs       __sovf
[000000b6] 206f 0004                 movea.l   4(a7),a0
[000000ba] 2008                      move.l    a0,d0
[000000bc] d1fc 0000 0100            adda.l    #$00000100,a0
[000000c2] bfc8                      cmpa.l    a0,a7
[000000c4] 6500 000c                 bcs       $000000D2
[000000c8] 23c0 0000 5fce            move.l    d0,__break
[000000ce] 4280                      clr.l     d0
[000000d0] 4e75                      rts       
[000000d2] 203c ffff ffff            move.l    #$FFFFFFFF,d0
[000000d8] 4e75                      rts       
[000000da] ___BDOS:
[000000da] 4e56 fffc                 link      a6,#$FFFC
[000000de] 302e 0008                 move.w    8(a6),d0
[000000e2] 0c40 0009                 cmpi.w    #$0009,d0
[000000e6] 6700 0012                 beq       $000000FA
[000000ea] 0c40 0002                 cmpi.w    #$0002,d0
[000000ee] 6500 000a                 bcs       $000000FA
[000000f2] 0c40 001a                 cmpi.w    #$001A,d0
[000000f6] 6600 0012                 bne       $0000010A
[000000fa] 2eae 000a                 move.l    10(a6),(a7)
[000000fe] 3f00                      move.w    d0,-(a7)
[00000100] 6100 001c                 bsr       $0000011E
[00000104] 548f                      addq.l    #2,a7
[00000106] 6000 0012                 bra       $0000011A
[0000010a] 222e 000a                 move.l    10(a6),d1
[0000010e] 4e42                      trap      #2
[00000110] bff9 0000 5fce            cmpa.l    __break,a7
[00000116] 6500 0016                 bcs       __sovf
[0000011a] 4e5e                      unlk      a6
[0000011c] 4e75                      rts       
[0000011e] 23df 0000 5fc6            move.l    (a7)+,$00005FC6
[00000124] 4e41                      trap      #1
[00000126] 2f39 0000 5fc6            move.l    $00005FC6,-(a7)
[0000012c] 4e75                      rts       
[0000012e] __sovf:
[0000012e] 2f3c 0000 58d6            move.l    #$000058D6,-(a7)
[00000134] 3f3c 0009                 move.w    #$0009,-(a7)
[00000138] 4e41                      trap      #1
[0000013a] 3f3c 0001                 move.w    #$0001,-(a7)
[0000013e] 303c 004c                 move.w    #$004C,d0
[00000142] 4e41                      trap      #1
[00000144] _blkfill:
[00000144] 206f 0004                 movea.l   4(a7),a0
[00000148] 322f 0008                 move.w    8(a7),d1
[0000014c] 302f 000a                 move.w    10(a7),d0
[00000150] 48c0                      ext.l     d0
[00000152] 5380                      subq.l    #1,d0
[00000154] 6f00 0008                 ble       $0000015E
[00000158] 10c1                      move.b    d1,(a0)+
[0000015a] 51c8 fffc                 dbf       d0,$00000158
[0000015e] 4280                      clr.l     d0
[00000160] 4e75                      rts       
[00000162] _strchr:
[00000162] 206f 0004                 movea.l   4(a7),a0
[00000166] 302f 0008                 move.w    8(a7),d0
[0000016a] 4a10                      tst.b     (a0)
[0000016c] 6600 0006                 bne       $00000174
[00000170] 4280                      clr.l     d0
[00000172] 4e75                      rts       
[00000174] b018                      cmp.b     (a0)+,d0
[00000176] 66f2                      bne.s     $0000016A
[00000178] 2008                      move.l    a0,d0
[0000017a] 5380                      subq.l    #1,d0
[0000017c] 4e75                      rts       
[0000017e] _crystal:
[0000017e] 222f 0004                 move.l    4(a7),d1
[00000182] 303c 00c8                 move.w    #$00C8,d0
[00000186] 4e42                      trap      #2
[00000188] 4e75                      rts       
[0000018a] _filesiz:
[0000018a] 4e56 fffa                 link      a6,#$FFFA
[0000018e] 48e7 0104                 movem.l   d7/a5,-(a7)
[00000192] 2a6e 0008                 movea.l   8(a6),a5
[00000196] 3ebc 002f                 move.w    #$002F,(a7)
[0000019a] 4eb9 0000 3a00            jsr       _gemdos
[000001a0] 2d40 fffa                 move.l    d0,-6(a6)
[000001a4] 2ebc 0000 6004            move.l    #_dta_buf,(a7)
[000001aa] 3f3c 001a                 move.w    #$001A,-(a7)
[000001ae] 4eb9 0000 3a00            jsr       _gemdos
[000001b4] 548f                      addq.l    #2,a7
[000001b6] 4257                      clr.w     (a7)
[000001b8] 2f0d                      move.l    a5,-(a7)
[000001ba] 3f3c 004e                 move.w    #$004E,-(a7)
[000001be] 4eb9 0000 3a00            jsr       _gemdos
[000001c4] 5c8f                      addq.l    #6,a7
[000001c6] 3d40 fffe                 move.w    d0,-2(a6)
[000001ca] 2eae fffa                 move.l    -6(a6),(a7)
[000001ce] 3f3c 001a                 move.w    #$001A,-(a7)
[000001d2] 4eb9 0000 3a00            jsr       _gemdos
[000001d8] 548f                      addq.l    #2,a7
[000001da] 4a6e fffe                 tst.w     -2(a6)
[000001de] 6600 0010                 bne       $000001F0
[000001e2] 2039 0000 601e            move.l    $0000601E,d0
[000001e8] 6000 000c                 bra       $000001F6
[000001ec] 6000 0008                 bra       $000001F6
[000001f0] 4280                      clr.l     d0
[000001f2] 6000 0002                 bra       $000001F6
[000001f6] 4a9f                      tst.l     (a7)+
[000001f8] 4cdf 2000                 movem.l   (a7)+,a5
[000001fc] 4e5e                      unlk      a6
[000001fe] 4e75                      rts       
[00000200] _init_op:
[00000200] 4e56 fffc                 link      a6,#$FFFC
[00000204] 2ebc ffff ffff            move.l    #$FFFFFFFF,(a7)
[0000020a] 3f3c 0048                 move.w    #$0048,-(a7)
[0000020e] 4eb9 0000 3a00            jsr       _gemdos
[00000214] 548f                      addq.l    #2,a7
[00000216] 23c0 0000 606a            move.l    d0,_memsize
[0000021c] 4ab9 0000 606a            tst.l     _memsize
[00000222] 6600 0008                 bne       $0000022C
[00000226] 4240                      clr.w     d0
[00000228] 6000 0082                 bra       $000002AC
[0000022c] 2eb9 0000 606a            move.l    _memsize,(a7)
[00000232] 3f3c 0048                 move.w    #$0048,-(a7)
[00000236] 4eb9 0000 3a00            jsr       _gemdos
[0000023c] 548f                      addq.l    #2,a7
[0000023e] 23c0 0000 617c            move.l    d0,_begmem
[00000244] 4ab9 0000 617c            tst.l     _begmem
[0000024a] 6600 0008                 bne       $00000254
[0000024e] 4240                      clr.w     d0
[00000250] 6000 005a                 bra       $000002AC
[00000254] 2039 0000 617c            move.l    _begmem,d0
[0000025a] d0b9 0000 606a            add.l     _memsize,d0
[00000260] 23c0 0000 6184            move.l    d0,_endmem
[00000266] 2039 0000 617c            move.l    _begmem,d0
[0000026c] d0bc 0000 fa00            add.l     #$0000FA00,d0
[00000272] 23c0 0000 6052            move.l    d0,_freemem
[00000278] 4240                      clr.w     d0
[0000027a] 33c0 0000 605a            move.w    d0,_bra_tot
[00000280] 33c0 0000 6068            move.w    d0,_clr_tot
[00000286] 33c0 0000 5ffa            move.w    d0,_imov1_t
[0000028c] 33c0 0000 5ff4            move.w    d0,_clr1_to
[00000292] 33c0 0000 5ffc            move.w    d0,_mov1_to
[00000298] 33c0 0000 6072            move.w    d0,_imov_to
[0000029e] 33c0 0000 6074            move.w    d0,_mov_tot
[000002a4] 303c 0001                 move.w    #$0001,d0
[000002a8] 6000 0002                 bra       $000002AC
[000002ac] 4e5e                      unlk      a6
[000002ae] 4e75                      rts       
[000002b0] _open_fi:
[000002b0] 4e56 fffc                 link      a6,#$FFFC
[000002b4] 2eae 0008                 move.l    8(a6),(a7)
[000002b8] 4eb9 0000 018a            jsr       _filesiz
[000002be] 23c0 0000 5ff6            move.l    d0,_size
[000002c4] 4257                      clr.w     (a7)
[000002c6] 2f2e 0008                 move.l    8(a6),-(a7)
[000002ca] 3f3c 003d                 move.w    #$003D,-(a7)
[000002ce] 4eb9 0000 3a00            jsr       _gemdos
[000002d4] 5c8f                      addq.l    #6,a7
[000002d6] 33c0 0000 6050            move.w    d0,_in_hndl
[000002dc] 4ab9 0000 5ff6            tst.l     _size
[000002e2] 6700 000c                 beq       $000002F0
[000002e6] 4a79 0000 6050            tst.w     _in_hndl
[000002ec] 6c00 001a                 bge       $00000308
[000002f0] 2eae 0008                 move.l    8(a6),(a7)
[000002f4] 2f3c 0000 5a70            move.l    #$00005A70,-(a7)
[000002fa] 4eb9 0000 57fc            jsr       _printf
[00000300] 588f                      addq.l    #4,a7
[00000302] 4240                      clr.w     d0
[00000304] 6000 00a0                 bra       $000003A6
[00000308] 2039 0000 5ff6            move.l    _size,d0
[0000030e] e380                      asl.l     #1,d0
[00000310] d0bc 0000 0010            add.l     #$00000010,d0
[00000316] 2239 0000 6184            move.l    _endmem,d1
[0000031c] 92b9 0000 6052            sub.l     _freemem,d1
[00000322] b081                      cmp.l     d1,d0
[00000324] 6f00 0026                 ble       $0000034C
[00000328] 2eae 0008                 move.l    8(a6),(a7)
[0000032c] 2f3c 0000 5a97            move.l    #$00005A97,-(a7)
[00000332] 4eb9 0000 57fc            jsr       _printf
[00000338] 588f                      addq.l    #4,a7
[0000033a] 2ebc 0000 5abd            move.l    #$00005ABD,(a7)
[00000340] 4eb9 0000 57fc            jsr       _printf
[00000346] 4240                      clr.w     d0
[00000348] 6000 005c                 bra       $000003A6
[0000034c] 2eb9 0000 602e            move.l    _in_buff,(a7)
[00000352] 2f39 0000 5ff6            move.l    _size,-(a7)
[00000358] 3f39 0000 6050            move.w    _in_hndl,-(a7)
[0000035e] 3f3c 003f                 move.w    #$003F,-(a7)
[00000362] 4eb9 0000 3a00            jsr       _gemdos
[00000368] 508f                      addq.l    #8,a7
[0000036a] b0b9 0000 5ff6            cmp.l     _size,d0
[00000370] 6700 001a                 beq       $0000038C
[00000374] 2eae 0008                 move.l    8(a6),(a7)
[00000378] 2f3c 0000 5ac7            move.l    #$00005AC7,-(a7)
[0000037e] 4eb9 0000 57fc            jsr       _printf
[00000384] 588f                      addq.l    #4,a7
[00000386] 4240                      clr.w     d0
[00000388] 6000 001c                 bra       $000003A6
[0000038c] 3eb9 0000 6050            move.w    _in_hndl,(a7)
[00000392] 3f3c 003e                 move.w    #$003E,-(a7)
[00000396] 4eb9 0000 3a00            jsr       _gemdos
[0000039c] 548f                      addq.l    #2,a7
[0000039e] 303c 0001                 move.w    #$0001,d0
[000003a2] 6000 0002                 bra       $000003A6
[000003a6] 4e5e                      unlk      a6
[000003a8] 4e75                      rts       
[000003aa] _close_f:
[000003aa] 4e56 fefc                 link      a6,#$FEFC
[000003ae] 2ebc 0000 6076            move.l    #_opath,(a7)
[000003b4] 2f0e                      move.l    a6,-(a7)
[000003b6] 0697 ffff ff00            addi.l    #$FFFFFF00,(a7)
[000003bc] 4eb9 0000 3c10            jsr       _strcpy
[000003c2] 588f                      addq.l    #4,a7
[000003c4] 2eae 0008                 move.l    8(a6),(a7)
[000003c8] 2f0e                      move.l    a6,-(a7)
[000003ca] 0697 ffff ff00            addi.l    #$FFFFFF00,(a7)
[000003d0] 4eb9 0000 3c7a            jsr       _strcat
[000003d6] 588f                      addq.l    #4,a7
[000003d8] 4257                      clr.w     (a7)
[000003da] 2f0e                      move.l    a6,-(a7)
[000003dc] 0697 ffff ff00            addi.l    #$FFFFFF00,(a7)
[000003e2] 3f3c 003c                 move.w    #$003C,-(a7)
[000003e6] 4eb9 0000 3a00            jsr       _gemdos
[000003ec] 5c8f                      addq.l    #6,a7
[000003ee] 33c0 0000 605e            move.w    d0,_out_hnd
[000003f4] 6c00 002a                 bge       $00000420
[000003f8] 2e8e                      move.l    a6,(a7)
[000003fa] 0697 ffff ff00            addi.l    #$FFFFFF00,(a7)
[00000400] 2f3c 0000 5ae2            move.l    #$00005AE2,-(a7)
[00000406] 4eb9 0000 57fc            jsr       _printf
[0000040c] 588f                      addq.l    #4,a7
[0000040e] 2ebc 0000 5af4            move.l    #$00005AF4,(a7)
[00000414] 4eb9 0000 57fc            jsr       _printf
[0000041a] 4240                      clr.w     d0
[0000041c] 6000 006e                 bra       $0000048C
[00000420] 2eb9 0000 6064            move.l    _out_buf,(a7)
[00000426] 2f39 0000 6060            move.l    _out_ind,-(a7)
[0000042c] 3f39 0000 605e            move.w    _out_hnd,-(a7)
[00000432] 3f3c 0040                 move.w    #$0040,-(a7)
[00000436] 4eb9 0000 3a00            jsr       _gemdos
[0000043c] 508f                      addq.l    #8,a7
[0000043e] b0b9 0000 6060            cmp.l     _out_ind,d0
[00000444] 6700 002c                 beq       $00000472
[00000448] 2eae 0008                 move.l    8(a6),(a7)
[0000044c] 2f3c 0000 6076            move.l    #_opath,-(a7)
[00000452] 2f3c 0000 5b0a            move.l    #$00005B0A,-(a7)
[00000458] 4eb9 0000 57fc            jsr       _printf
[0000045e] 508f                      addq.l    #8,a7
[00000460] 2ebc 0000 5b1d            move.l    #$00005B1D,(a7)
[00000466] 4eb9 0000 57fc            jsr       _printf
[0000046c] 4240                      clr.w     d0
[0000046e] 6000 001c                 bra       $0000048C
[00000472] 3eb9 0000 605e            move.w    _out_hnd,(a7)
[00000478] 3f3c 003e                 move.w    #$003E,-(a7)
[0000047c] 4eb9 0000 3a00            jsr       _gemdos
[00000482] 548f                      addq.l    #2,a7
[00000484] 303c 0001                 move.w    #$0001,d0
[00000488] 6000 0002                 bra       $0000048C
[0000048c] 4e5e                      unlk      a6
[0000048e] 4e75                      rts       
[00000490] _Error:
[00000490] 4e56 fffc                 link      a6,#$FFFC
[00000494] 2eae 0008                 move.l    8(a6),(a7)
[00000498] 2f3c 0000 5b27            move.l    #$00005B27,-(a7)
[0000049e] 2f3c 0000 5ecc            move.l    #$00005ECC,-(a7)
[000004a4] 4eb9 0000 581e            jsr       _fprintf
[000004aa] 508f                      addq.l    #8,a7
[000004ac] 3ebc ffff                 move.w    #$FFFF,(a7)
[000004b0] 4eb9 0000 5416            jsr       _exit
[000004b6] 4e5e                      unlk      a6
[000004b8] 4e75                      rts       
[000004ba] _usage:
[000004ba] 4e56 fffc                 link      a6,#$FFFC
[000004be] 2ebc 0000 5b2e            move.l    #$00005B2E,(a7)
[000004c4] 4eb9 0000 0490            jsr       _Error
[000004ca] 4e5e                      unlk      a6
[000004cc] 4e75                      rts       
[000004ce] _main:
[000004ce] 4e56 fffa                 link      a6,#$FFFA
[000004d2] 0c6e 0002 0008            cmpi.w    #$0002,8(a6)
[000004d8] 6c00 0008                 bge       $000004E2
[000004dc] 4eb9 0000 04ba            jsr       _usage
[000004e2] 206e 000a                 movea.l   10(a6),a0
[000004e6] 2068 0004                 movea.l   4(a0),a0
[000004ea] 0c10 002d                 cmpi.b    #$2D,(a0)
[000004ee] 6600 00a6                 bne       $00000596
[000004f2] 206e 000a                 movea.l   10(a6),a0
[000004f6] 2068 0004                 movea.l   4(a0),a0
[000004fa] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00000500] 6700 0014                 beq       $00000516
[00000504] 206e 000a                 movea.l   10(a6),a0
[00000508] 2068 0004                 movea.l   4(a0),a0
[0000050c] 0c28 004f 0001            cmpi.b    #$4F,1(a0)
[00000512] 6600 0078                 bne       $0000058C
[00000516] 206e 000a                 movea.l   10(a6),a0
[0000051a] 2068 0004                 movea.l   4(a0),a0
[0000051e] 4a28 0002                 tst.b     2(a0)
[00000522] 6600 0032                 bne       $00000556
[00000526] 0c6e 0003 0008            cmpi.w    #$0003,8(a6)
[0000052c] 6c00 0008                 bge       $00000536
[00000530] 4eb9 0000 04ba            jsr       _usage
[00000536] 206e 000a                 movea.l   10(a6),a0
[0000053a] 2ea8 0008                 move.l    8(a0),(a7)
[0000053e] 2f3c 0000 6076            move.l    #_opath,-(a7)
[00000544] 4eb9 0000 3c10            jsr       _strcpy
[0000054a] 588f                      addq.l    #4,a7
[0000054c] 3d7c 0003 fffe            move.w    #$0003,-2(a6)
[00000552] 6000 0034                 bra       $00000588
[00000556] 206e 000a                 movea.l   10(a6),a0
[0000055a] 2ea8 0004                 move.l    4(a0),(a7)
[0000055e] 0697 0000 0002            addi.l    #$00000002,(a7)
[00000564] 2f3c 0000 6076            move.l    #_opath,-(a7)
[0000056a] 4eb9 0000 3c10            jsr       _strcpy
[00000570] 588f                      addq.l    #4,a7
[00000572] 0c6e 0002 0008            cmpi.w    #$0002,8(a6)
[00000578] 6600 0008                 bne       $00000582
[0000057c] 4eb9 0000 04ba            jsr       _usage
[00000582] 3d7c 0002 fffe            move.w    #$0002,-2(a6)
[00000588] 6000 0008                 bra       $00000592
[0000058c] 4eb9 0000 04ba            jsr       _usage
[00000592] 6000 0008                 bra       $0000059C
[00000596] 3d7c 0001 fffe            move.w    #$0001,-2(a6)
[0000059c] 4eb9 0000 0200            jsr       _init_op
[000005a2] 4a40                      tst.w     d0
[000005a4] 6600 000e                 bne       $000005B4
[000005a8] 2ebc 0000 5b57            move.l    #$00005B57,(a7)
[000005ae] 4eb9 0000 0490            jsr       _Error
[000005b4] 6000 0090                 bra       $00000646
[000005b8] 23f9 0000 6052 0000 602e  move.l    _freemem,_in_buff
[000005c2] 206e 000a                 movea.l   10(a6),a0
[000005c6] 322e fffe                 move.w    -2(a6),d1
[000005ca] 48c1                      ext.l     d1
[000005cc] e581                      asl.l     #2,d1
[000005ce] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[000005d2] 4eb9 0000 02b0            jsr       _open_fi
[000005d8] 4a40                      tst.w     d0
[000005da] 6700 0064                 beq       $00000640
[000005de] 206e 000a                 movea.l   10(a6),a0
[000005e2] 322e fffe                 move.w    -2(a6),d1
[000005e6] 48c1                      ext.l     d1
[000005e8] e581                      asl.l     #2,d1
[000005ea] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[000005ee] 3f2e fffe                 move.w    -2(a6),-(a7)
[000005f2] 2f3c 0000 5b72            move.l    #$00005B72,-(a7)
[000005f8] 4eb9 0000 57fc            jsr       _printf
[000005fe] 5c8f                      addq.l    #6,a7
[00000600] 2039 0000 602e            move.l    _in_buff,d0
[00000606] d0b9 0000 5ff6            add.l     _size,d0
[0000060c] d0bc 0000 0002            add.l     #$00000002,d0
[00000612] c0bc ffff fffe            and.l     #$FFFFFFFE,d0
[00000618] 23c0 0000 6064            move.l    d0,_out_buf
[0000061e] 4eb9 0000 0726            jsr       _do_opti
[00000624] 206e 000a                 movea.l   10(a6),a0
[00000628] 322e fffe                 move.w    -2(a6),d1
[0000062c] 48c1                      ext.l     d1
[0000062e] e581                      asl.l     #2,d1
[00000630] 2eb0 1800                 move.l    0(a0,d1.l),(a7)
[00000634] 4eb9 0000 03aa            jsr       _close_f
[0000063a] 4a40                      tst.w     d0
[0000063c] 6700 0002                 beq       $00000640
[00000640] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000646] 302e fffe                 move.w    -2(a6),d0
[0000064a] b06e 0008                 cmp.w     8(a6),d0
[0000064e] 6d00 ff68                 blt       $000005B8
[00000652] 3eb9 0000 6068            move.w    _clr_tot,(a7)
[00000658] 2f3c 0000 5b80            move.l    #$00005B80,-(a7)
[0000065e] 4eb9 0000 57fc            jsr       _printf
[00000664] 588f                      addq.l    #4,a7
[00000666] 3eb9 0000 5ff4            move.w    _clr1_to,(a7)
[0000066c] 2f3c 0000 5b9a            move.l    #$00005B9A,-(a7)
[00000672] 4eb9 0000 57fc            jsr       _printf
[00000678] 588f                      addq.l    #4,a7
[0000067a] 3eb9 0000 6074            move.w    _mov_tot,(a7)
[00000680] 2f3c 0000 5bc0            move.l    #$00005BC0,-(a7)
[00000686] 4eb9 0000 57fc            jsr       _printf
[0000068c] 588f                      addq.l    #4,a7
[0000068e] 3eb9 0000 5ffc            move.w    _mov1_to,(a7)
[00000694] 2f3c 0000 5bdc            move.l    #$00005BDC,-(a7)
[0000069a] 4eb9 0000 57fc            jsr       _printf
[000006a0] 588f                      addq.l    #4,a7
[000006a2] 3eb9 0000 6072            move.w    _imov_to,(a7)
[000006a8] 2f3c 0000 5c02            move.l    #$00005C02,-(a7)
[000006ae] 4eb9 0000 57fc            jsr       _printf
[000006b4] 588f                      addq.l    #4,a7
[000006b6] 3eb9 0000 5ffa            move.w    _imov1_t,(a7)
[000006bc] 2f3c 0000 5c27            move.l    #$00005C27,-(a7)
[000006c2] 4eb9 0000 57fc            jsr       _printf
[000006c8] 588f                      addq.l    #4,a7
[000006ca] 3eb9 0000 605a            move.w    _bra_tot,(a7)
[000006d0] 2f3c 0000 5c52            move.l    #$00005C52,-(a7)
[000006d6] 4eb9 0000 57fc            jsr       _printf
[000006dc] 588f                      addq.l    #4,a7
[000006de] 3eb9 0000 5ffe            move.w    _mv1_tot,(a7)
[000006e4] 2f3c 0000 5c76            move.l    #$00005C76,-(a7)
[000006ea] 4eb9 0000 57fc            jsr       _printf
[000006f0] 588f                      addq.l    #4,a7
[000006f2] 3eb9 0000 6000            move.w    _mv2_tot,(a7)
[000006f8] 2f3c 0000 5c97            move.l    #$00005C97,-(a7)
[000006fe] 4eb9 0000 57fc            jsr       _printf
[00000704] 588f                      addq.l    #4,a7
[00000706] 3eb9 0000 6002            move.w    _adda_to,(a7)
[0000070c] 2f3c 0000 5cc9            move.l    #$00005CC9,-(a7)
[00000712] 4eb9 0000 57fc            jsr       _printf
[00000718] 588f                      addq.l    #4,a7
[0000071a] 4257                      clr.w     (a7)
[0000071c] 4eb9 0000 5416            jsr       _exit
[00000722] 4e5e                      unlk      a6
[00000724] 4e75                      rts       
[00000726] _do_opti:
[00000726] 4e56 fff8                 link      a6,#$FFF8
[0000072a] 4eb9 0000 1702            jsr       _clrwtol
[00000730] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[00000738] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[00000742] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[0000074a] 2039 0000 6060            move.l    _out_ind,d0
[00000750] 90bc 0000 0001            sub.l     #$00000001,d0
[00000756] 23c0 0000 5ff6            move.l    d0,_size
[0000075c] 4eb9 0000 0c9e            jsr       _clr2wto
[00000762] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[0000076a] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[00000774] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[0000077c] 2039 0000 6060            move.l    _out_ind,d0
[00000782] 90bc 0000 0001            sub.l     #$00000001,d0
[00000788] 23c0 0000 5ff6            move.l    d0,_size
[0000078e] 4eb9 0000 1306            jsr       _mvwtol
[00000794] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[0000079c] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[000007a6] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[000007ae] 2039 0000 6060            move.l    _out_ind,d0
[000007b4] 90bc 0000 0001            sub.l     #$00000001,d0
[000007ba] 23c0 0000 5ff6            move.l    d0,_size
[000007c0] 4eb9 0000 0f2c            jsr       _mv2wtol
[000007c6] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[000007ce] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[000007d8] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[000007e0] 2039 0000 6060            move.l    _out_ind,d0
[000007e6] 90bc 0000 0001            sub.l     #$00000001,d0
[000007ec] 23c0 0000 5ff6            move.l    d0,_size
[000007f2] 4eb9 0000 199c            jsr       _imoveop
[000007f8] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[00000800] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[0000080a] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[00000812] 2039 0000 6060            move.l    _out_ind,d0
[00000818] 90bc 0000 0001            sub.l     #$00000001,d0
[0000081e] 23c0 0000 5ff6            move.l    d0,_size
[00000824] 4eb9 0000 234e            jsr       _clrop
[0000082a] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[00000832] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[0000083c] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[00000844] 2039 0000 6060            move.l    _out_ind,d0
[0000084a] 90bc 0000 0001            sub.l     #$00000001,d0
[00000850] 23c0 0000 5ff6            move.l    d0,_size
[00000856] 4eb9 0000 276e            jsr       _mv2pea1
[0000085c] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[00000864] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[0000086e] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[00000876] 23f9 0000 6060 0000 5ff6  move.l    _out_ind,_size
[00000880] 4eb9 0000 2c2c            jsr       _mv2pea2
[00000886] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[0000088e] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[00000898] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[000008a0] 23f9 0000 6060 0000 5ff6  move.l    _out_ind,_size
[000008aa] 4eb9 0000 30f8            jsr       _adda
[000008b0] 2d79 0000 6064 fffc       move.l    _out_buf,-4(a6)
[000008b8] 23f9 0000 602e 0000 6064  move.l    _in_buff,_out_buf
[000008c2] 23ee fffc 0000 602e       move.l    -4(a6),_in_buff
[000008ca] 23f9 0000 6060 0000 5ff6  move.l    _out_ind,_size
[000008d4] 4eb9 0000 257e            jsr       _samebra
[000008da] 04b9 0000 0001 0000 6060  subi.l    #$00000001,_out_ind
[000008e4] 4e5e                      unlk      a6
[000008e6] 4e75                      rts       
[000008e8] _reverse:
[000008e8] 4e56 fff6                 link      a6,#$FFF6
[000008ec] 426e fffc                 clr.w     -4(a6)
[000008f0] 2eae 0008                 move.l    8(a6),(a7)
[000008f4] 4eb9 0000 0954            jsr       _strlen
[000008fa] 907c 0001                 sub.w     #$0001,d0
[000008fe] 3d40 fffa                 move.w    d0,-6(a6)
[00000902] 6000 0042                 bra       $00000946
[00000906] 206e 0008                 movea.l   8(a6),a0
[0000090a] 322e fffc                 move.w    -4(a6),d1
[0000090e] 48c1                      ext.l     d1
[00000910] 1030 1800                 move.b    0(a0,d1.l),d0
[00000914] 4880                      ext.w     d0
[00000916] 3d40 fffe                 move.w    d0,-2(a6)
[0000091a] 306e fffc                 movea.w   -4(a6),a0
[0000091e] d1ee 0008                 adda.l    8(a6),a0
[00000922] 326e fffa                 movea.w   -6(a6),a1
[00000926] d3ee 0008                 adda.l    8(a6),a1
[0000092a] 1091                      move.b    (a1),(a0)
[0000092c] 302e fffe                 move.w    -2(a6),d0
[00000930] 326e fffa                 movea.w   -6(a6),a1
[00000934] d3ee 0008                 adda.l    8(a6),a1
[00000938] 1280                      move.b    d0,(a1)
[0000093a] 066e 0001 fffc            addi.w    #$0001,-4(a6)
[00000940] 046e 0001 fffa            subi.w    #$0001,-6(a6)
[00000946] 302e fffc                 move.w    -4(a6),d0
[0000094a] b06e fffa                 cmp.w     -6(a6),d0
[0000094e] 6db6                      blt.s     $00000906
[00000950] 4e5e                      unlk      a6
[00000952] 4e75                      rts       
[00000954] _strlen:
[00000954] 4e56 fff8                 link      a6,#$FFF8
[00000958] 2d6e 0008 fffc            move.l    8(a6),-4(a6)
[0000095e] 6000 000a                 bra       $0000096A
[00000962] 06ae 0000 0001 fffc       addi.l    #$00000001,-4(a6)
[0000096a] 206e fffc                 movea.l   -4(a6),a0
[0000096e] 4a10                      tst.b     (a0)
[00000970] 66f0                      bne.s     $00000962
[00000972] 202e fffc                 move.l    -4(a6),d0
[00000976] 90ae 0008                 sub.l     8(a6),d0
[0000097a] 6000 0002                 bra       $0000097E
[0000097e] 4e5e                      unlk      a6
[00000980] 4e75                      rts       
[00000982] _htoa:
[00000982] 4e56 fffa                 link      a6,#$FFFA
[00000986] 3d7c 0003 fffe            move.w    #$0003,-2(a6)
[0000098c] 6000 007c                 bra       $00000A0A
[00000990] 302e 000c                 move.w    12(a6),d0
[00000994] 326e fffe                 movea.w   -2(a6),a1
[00000998] d3ee 0008                 adda.l    8(a6),a1
[0000099c] 1280                      move.b    d0,(a1)
[0000099e] 306e fffe                 movea.w   -2(a6),a0
[000009a2] d1ee 0008                 adda.l    8(a6),a0
[000009a6] 0210 000f                 andi.b    #$0F,(a0)
[000009aa] 306e fffe                 movea.w   -2(a6),a0
[000009ae] d1ee 0008                 adda.l    8(a6),a0
[000009b2] 0c10 000a                 cmpi.b    #$0A,(a0)
[000009b6] 6d00 0024                 blt       $000009DC
[000009ba] 206e 0008                 movea.l   8(a6),a0
[000009be] 322e fffe                 move.w    -2(a6),d1
[000009c2] 48c1                      ext.l     d1
[000009c4] 1030 1800                 move.b    0(a0,d1.l),d0
[000009c8] 4880                      ext.w     d0
[000009ca] d07c 0037                 add.w     #$0037,d0
[000009ce] 326e fffe                 movea.w   -2(a6),a1
[000009d2] d3ee 0008                 adda.l    8(a6),a1
[000009d6] 1280                      move.b    d0,(a1)
[000009d8] 6000 0020                 bra       $000009FA
[000009dc] 206e 0008                 movea.l   8(a6),a0
[000009e0] 322e fffe                 move.w    -2(a6),d1
[000009e4] 48c1                      ext.l     d1
[000009e6] 1030 1800                 move.b    0(a0,d1.l),d0
[000009ea] 4880                      ext.w     d0
[000009ec] d07c 0030                 add.w     #$0030,d0
[000009f0] 326e fffe                 movea.w   -2(a6),a1
[000009f4] d3ee 0008                 adda.l    8(a6),a1
[000009f8] 1280                      move.b    d0,(a1)
[000009fa] 046e 0001 fffe            subi.w    #$0001,-2(a6)
[00000a00] 302e 000c                 move.w    12(a6),d0
[00000a04] e840                      asr.w     #4,d0
[00000a06] 3d40 000c                 move.w    d0,12(a6)
[00000a0a] 4a6e fffe                 tst.w     -2(a6)
[00000a0e] 6c80                      bge.s     $00000990
[00000a10] 4e5e                      unlk      a6
[00000a12] 4e75                      rts       
[00000a14] _itoa:
[00000a14] 4e56 fff8                 link      a6,#$FFF8
[00000a18] 3d6e 0008 fffc            move.w    8(a6),-4(a6)
[00000a1e] 6c00 000c                 bge       $00000A2C
[00000a22] 302e 0008                 move.w    8(a6),d0
[00000a26] 4440                      neg.w     d0
[00000a28] 3d40 0008                 move.w    d0,8(a6)
[00000a2c] 426e fffe                 clr.w     -2(a6)
[00000a30] 302e 0008                 move.w    8(a6),d0
[00000a34] 48c0                      ext.l     d0
[00000a36] 81fc 000a                 divs.w    #$000A,d0
[00000a3a] 4840                      swap      d0
[00000a3c] d07c 0030                 add.w     #$0030,d0
[00000a40] 326e fffe                 movea.w   -2(a6),a1
[00000a44] d3ee 000a                 adda.l    10(a6),a1
[00000a48] 1280                      move.b    d0,(a1)
[00000a4a] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000a50] 302e 0008                 move.w    8(a6),d0
[00000a54] 48c0                      ext.l     d0
[00000a56] 81fc 000a                 divs.w    #$000A,d0
[00000a5a] 3d40 0008                 move.w    d0,8(a6)
[00000a5e] 6ed0                      bgt.s     $00000A30
[00000a60] 4a6e fffc                 tst.w     -4(a6)
[00000a64] 6c00 0014                 bge       $00000A7A
[00000a68] 306e fffe                 movea.w   -2(a6),a0
[00000a6c] d1ee 000a                 adda.l    10(a6),a0
[00000a70] 10bc 002d                 move.b    #$2D,(a0)
[00000a74] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000a7a] 306e fffe                 movea.w   -2(a6),a0
[00000a7e] d1ee 000a                 adda.l    10(a6),a0
[00000a82] 4210                      clr.b     (a0)
[00000a84] 2eae 000a                 move.l    10(a6),(a7)
[00000a88] 4eb9 0000 08e8            jsr       _reverse
[00000a8e] 4e5e                      unlk      a6
[00000a90] 4e75                      rts       
[00000a92] _atoi:
[00000a92] 4e56 fff4                 link      a6,#$FFF4
[00000a96] 426e fffe                 clr.w     -2(a6)
[00000a9a] 3d7c 0001 fffc            move.w    #$0001,-4(a6)
[00000aa0] 306e fffe                 movea.w   -2(a6),a0
[00000aa4] d1ee 0008                 adda.l    8(a6),a0
[00000aa8] 0c10 002d                 cmpi.b    #$2D,(a0)
[00000aac] 6600 000e                 bne       $00000ABC
[00000ab0] 3d7c ffff fffc            move.w    #$FFFF,-4(a6)
[00000ab6] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000abc] 42ae fff8                 clr.l     -8(a6)
[00000ac0] 6000 004c                 bra       $00000B0E
[00000ac4] 2f3c 0000 000a            move.l    #$0000000A,-(a7)
[00000aca] 2f2e fff8                 move.l    -8(a6),-(a7)
[00000ace] 4eb9 0000 3a10            jsr       lmul
[00000ad4] 508f                      addq.l    #8,a7
[00000ad6] 326e fffe                 movea.w   -2(a6),a1
[00000ada] d3ee 0008                 adda.l    8(a6),a1
[00000ade] 1211                      move.b    (a1),d1
[00000ae0] 4881                      ext.w     d1
[00000ae2] 48c1                      ext.l     d1
[00000ae4] d081                      add.l     d1,d0
[00000ae6] d0bc ffff ffd0            add.l     #$FFFFFFD0,d0
[00000aec] 2d40 fff8                 move.l    d0,-8(a6)
[00000af0] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000af6] 0cae 0000 7fff fff8       cmpi.l    #$00007FFF,-8(a6)
[00000afe] 6f00 000e                 ble       $00000B0E
[00000b02] 2ebc 0000 5cf2            move.l    #$00005CF2,(a7)
[00000b08] 4eb9 0000 3300            jsr       _panic
[00000b0e] 306e fffe                 movea.w   -2(a6),a0
[00000b12] d1ee 0008                 adda.l    8(a6),a0
[00000b16] 0c10 0030                 cmpi.b    #$30,(a0)
[00000b1a] 6d00 0010                 blt       $00000B2C
[00000b1e] 306e fffe                 movea.w   -2(a6),a0
[00000b22] d1ee 0008                 adda.l    8(a6),a0
[00000b26] 0c10 0039                 cmpi.b    #$39,(a0)
[00000b2a] 6f98                      ble.s     $00000AC4
[00000b2c] 2f2e fff8                 move.l    -8(a6),-(a7)
[00000b30] 306e fffc                 movea.w   -4(a6),a0
[00000b34] 2f08                      move.l    a0,-(a7)
[00000b36] 4eb9 0000 3a10            jsr       lmul
[00000b3c] 508f                      addq.l    #8,a7
[00000b3e] 6000 0002                 bra       $00000B42
[00000b42] 4e5e                      unlk      a6
[00000b44] 4e75                      rts       
[00000b46] _getnum:
[00000b46] 4e56 fffa                 link      a6,#$FFFA
[00000b4a] 426e fffe                 clr.w     -2(a6)
[00000b4e] 2079 0000 602e            movea.l   _in_buff,a0
[00000b54] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000b5a] 0c10 0028                 cmpi.b    #$28,(a0)
[00000b5e] 6600 002c                 bne       $00000B8C
[00000b62] 306e fffe                 movea.w   -2(a6),a0
[00000b66] d1ee 0008                 adda.l    8(a6),a0
[00000b6a] 10bc 0030                 move.b    #$30,(a0)
[00000b6e] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000b74] 306e fffe                 movea.w   -2(a6),a0
[00000b78] d1ee 0008                 adda.l    8(a6),a0
[00000b7c] 4210                      clr.b     (a0)
[00000b7e] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000b84] 303c 0001                 move.w    #$0001,d0
[00000b88] 6000 0110                 bra       $00000C9A
[00000b8c] 2079 0000 602e            movea.l   _in_buff,a0
[00000b92] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000b98] 0c10 0030                 cmpi.b    #$30,(a0)
[00000b9c] 6d00 0016                 blt       $00000BB4
[00000ba0] 2079 0000 602e            movea.l   _in_buff,a0
[00000ba6] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000bac] 0c10 0039                 cmpi.b    #$39,(a0)
[00000bb0] 6f00 0042                 ble       $00000BF4
[00000bb4] 2079 0000 602e            movea.l   _in_buff,a0
[00000bba] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000bc0] 0c10 002d                 cmpi.b    #$2D,(a0)
[00000bc4] 6600 00ce                 bne       $00000C94
[00000bc8] 2079 0000 602e            movea.l   _in_buff,a0
[00000bce] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000bd4] 0c28 0030 0001            cmpi.b    #$30,1(a0)
[00000bda] 6d00 00b8                 blt       $00000C94
[00000bde] 2079 0000 602e            movea.l   _in_buff,a0
[00000be4] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000bea] 0c28 0039 0001            cmpi.b    #$39,1(a0)
[00000bf0] 6e00 00a2                 bgt       $00000C94
[00000bf4] 2079 0000 602e            movea.l   _in_buff,a0
[00000bfa] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000c00] 0c10 002d                 cmpi.b    #$2D,(a0)
[00000c04] 6600 0028                 bne       $00000C2E
[00000c08] 306e fffe                 movea.w   -2(a6),a0
[00000c0c] d1ee 0008                 adda.l    8(a6),a0
[00000c10] 2279 0000 6056            movea.l   _tmp_ind,a1
[00000c16] d3f9 0000 602e            adda.l    _in_buff,a1
[00000c1c] 1091                      move.b    (a1),(a0)
[00000c1e] 06b9 0000 0001 0000 6056  addi.l    #$00000001,_tmp_ind
[00000c28] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000c2e] 6000 0028                 bra       $00000C58
[00000c32] 306e fffe                 movea.w   -2(a6),a0
[00000c36] d1ee 0008                 adda.l    8(a6),a0
[00000c3a] 2279 0000 6056            movea.l   _tmp_ind,a1
[00000c40] d3f9 0000 602e            adda.l    _in_buff,a1
[00000c46] 1091                      move.b    (a1),(a0)
[00000c48] 06b9 0000 0001 0000 6056  addi.l    #$00000001,_tmp_ind
[00000c52] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00000c58] 2079 0000 602e            movea.l   _in_buff,a0
[00000c5e] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000c64] 0c10 0030                 cmpi.b    #$30,(a0)
[00000c68] 6d00 0014                 blt       $00000C7E
[00000c6c] 2079 0000 602e            movea.l   _in_buff,a0
[00000c72] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00000c78] 0c10 0039                 cmpi.b    #$39,(a0)
[00000c7c] 6fb4                      ble.s     $00000C32
[00000c7e] 306e fffe                 movea.w   -2(a6),a0
[00000c82] d1ee 0008                 adda.l    8(a6),a0
[00000c86] 4210                      clr.b     (a0)
[00000c88] 303c 0001                 move.w    #$0001,d0
[00000c8c] 6000 000c                 bra       $00000C9A
[00000c90] 6000 0008                 bra       $00000C9A
[00000c94] 4240                      clr.w     d0
[00000c96] 6000 0002                 bra       $00000C9A
[00000c9a] 4e5e                      unlk      a6
[00000c9c] 4e75                      rts       
[00000c9e] _clr2wto:
[00000c9e] 4e56 fffc                 link      a6,#$FFFC
[00000ca2] 42b9 0000 6180            clr.l     _in_ind
[00000ca8] 42b9 0000 6060            clr.l     _out_ind
[00000cae] 6000 0268                 bra       $00000F18
[00000cb2] 6000 0030                 bra       $00000CE4
[00000cb6] 2079 0000 6060            movea.l   _out_ind,a0
[00000cbc] d1f9 0000 6064            adda.l    _out_buf,a0
[00000cc2] 2279 0000 6180            movea.l   _in_ind,a1
[00000cc8] d3f9 0000 602e            adda.l    _in_buff,a1
[00000cce] 1091                      move.b    (a1),(a0)
[00000cd0] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00000cda] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000ce4] 2079 0000 602e            movea.l   _in_buff,a0
[00000cea] d1f9 0000 6180            adda.l    _in_ind,a0
[00000cf0] 0c10 0063                 cmpi.b    #$63,(a0)
[00000cf4] 6700 0010                 beq       $00000D06
[00000cf8] 2039 0000 6180            move.l    _in_ind,d0
[00000cfe] b0b9 0000 5ff6            cmp.l     _size,d0
[00000d04] 6db0                      blt.s     $00000CB6
[00000d06] 2079 0000 602e            movea.l   _in_buff,a0
[00000d0c] d1f9 0000 6180            adda.l    _in_ind,a0
[00000d12] 0c10 0063                 cmpi.b    #$63,(a0)
[00000d16] 6600 01d2                 bne       $00000EEA
[00000d1a] 2079 0000 602e            movea.l   _in_buff,a0
[00000d20] d1f9 0000 6180            adda.l    _in_ind,a0
[00000d26] 0c28 006c 0001            cmpi.b    #$6C,1(a0)
[00000d2c] 6600 01bc                 bne       $00000EEA
[00000d30] 2079 0000 602e            movea.l   _in_buff,a0
[00000d36] d1f9 0000 6180            adda.l    _in_ind,a0
[00000d3c] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[00000d42] 6600 01a6                 bne       $00000EEA
[00000d46] 2079 0000 602e            movea.l   _in_buff,a0
[00000d4c] d1f9 0000 6180            adda.l    _in_ind,a0
[00000d52] 0c28 002d 0004            cmpi.b    #$2D,4(a0)
[00000d58] 6600 0190                 bne       $00000EEA
[00000d5c] 2079 0000 602e            movea.l   _in_buff,a0
[00000d62] d1f9 0000 6180            adda.l    _in_ind,a0
[00000d68] 0c28 0028 0005            cmpi.b    #$28,5(a0)
[00000d6e] 6600 017a                 bne       $00000EEA
[00000d72] 2079 0000 602e            movea.l   _in_buff,a0
[00000d78] d1f9 0000 6180            adda.l    _in_ind,a0
[00000d7e] 0c28 0073 0006            cmpi.b    #$73,6(a0)
[00000d84] 6600 0164                 bne       $00000EEA
[00000d88] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[00000d92] 4eb9 0000 22c4            jsr       _nextlin
[00000d98] 2079 0000 602e            movea.l   _in_buff,a0
[00000d9e] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00000da4] 0c10 0063                 cmpi.b    #$63,(a0)
[00000da8] 6600 0140                 bne       $00000EEA
[00000dac] 2079 0000 602e            movea.l   _in_buff,a0
[00000db2] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00000db8] 0c28 006c 0001            cmpi.b    #$6C,1(a0)
[00000dbe] 6600 012a                 bne       $00000EEA
[00000dc2] 2079 0000 602e            movea.l   _in_buff,a0
[00000dc8] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00000dce] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[00000dd4] 6600 0114                 bne       $00000EEA
[00000dd8] 2079 0000 602e            movea.l   _in_buff,a0
[00000dde] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00000de4] 0c28 002d 0004            cmpi.b    #$2D,4(a0)
[00000dea] 6600 00fe                 bne       $00000EEA
[00000dee] 2079 0000 602e            movea.l   _in_buff,a0
[00000df4] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00000dfa] 0c28 0028 0005            cmpi.b    #$28,5(a0)
[00000e00] 6600 00e8                 bne       $00000EEA
[00000e04] 2079 0000 602e            movea.l   _in_buff,a0
[00000e0a] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00000e10] 0c28 0073 0006            cmpi.b    #$73,6(a0)
[00000e16] 6600 00d2                 bne       $00000EEA
[00000e1a] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[00000e24] 2079 0000 6060            movea.l   _out_ind,a0
[00000e2a] d1f9 0000 6064            adda.l    _out_buf,a0
[00000e30] 2279 0000 6180            movea.l   _in_ind,a1
[00000e36] d3f9 0000 602e            adda.l    _in_buff,a1
[00000e3c] 1091                      move.b    (a1),(a0)
[00000e3e] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00000e48] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000e52] 2079 0000 6060            movea.l   _out_ind,a0
[00000e58] d1f9 0000 6064            adda.l    _out_buf,a0
[00000e5e] 2279 0000 6180            movea.l   _in_ind,a1
[00000e64] d3f9 0000 602e            adda.l    _in_buff,a1
[00000e6a] 1091                      move.b    (a1),(a0)
[00000e6c] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00000e76] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000e80] 2079 0000 6060            movea.l   _out_ind,a0
[00000e86] d1f9 0000 6064            adda.l    _out_buf,a0
[00000e8c] 2279 0000 6180            movea.l   _in_ind,a1
[00000e92] d3f9 0000 602e            adda.l    _in_buff,a1
[00000e98] 1091                      move.b    (a1),(a0)
[00000e9a] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00000ea4] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000eae] 2079 0000 6060            movea.l   _out_ind,a0
[00000eb4] d1f9 0000 6064            adda.l    _out_buf,a0
[00000eba] 10bc 002e                 move.b    #$2E,(a0)
[00000ebe] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000ec8] 2079 0000 6060            movea.l   _out_ind,a0
[00000ece] d1f9 0000 6064            adda.l    _out_buf,a0
[00000ed4] 10bc 006c                 move.b    #$6C,(a0)
[00000ed8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000ee2] 0679 0001 0000 6068       addi.w    #$0001,_clr_tot
[00000eea] 2079 0000 6060            movea.l   _out_ind,a0
[00000ef0] d1f9 0000 6064            adda.l    _out_buf,a0
[00000ef6] 2279 0000 6180            movea.l   _in_ind,a1
[00000efc] d3f9 0000 602e            adda.l    _in_buff,a1
[00000f02] 1091                      move.b    (a1),(a0)
[00000f04] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00000f0e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000f18] 2039 0000 6180            move.l    _in_ind,d0
[00000f1e] b0b9 0000 5ff6            cmp.l     _size,d0
[00000f24] 6d00 fd8c                 blt       $00000CB2
[00000f28] 4e5e                      unlk      a6
[00000f2a] 4e75                      rts       
[00000f2c] _mv2wtol:
[00000f2c] 4e56 fffc                 link      a6,#$FFFC
[00000f30] 42b9 0000 6180            clr.l     _in_ind
[00000f36] 42b9 0000 6060            clr.l     _out_ind
[00000f3c] 6000 03b4                 bra       $000012F2
[00000f40] 6000 0030                 bra       $00000F72
[00000f44] 2079 0000 6060            movea.l   _out_ind,a0
[00000f4a] d1f9 0000 6064            adda.l    _out_buf,a0
[00000f50] 2279 0000 6180            movea.l   _in_ind,a1
[00000f56] d3f9 0000 602e            adda.l    _in_buff,a1
[00000f5c] 1091                      move.b    (a1),(a0)
[00000f5e] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00000f68] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00000f72] 2079 0000 602e            movea.l   _in_buff,a0
[00000f78] d1f9 0000 6180            adda.l    _in_ind,a0
[00000f7e] 0c10 006d                 cmpi.b    #$6D,(a0)
[00000f82] 6700 0010                 beq       $00000F94
[00000f86] 2039 0000 6180            move.l    _in_ind,d0
[00000f8c] b0b9 0000 5ff6            cmp.l     _size,d0
[00000f92] 6db0                      blt.s     $00000F44
[00000f94] 2079 0000 602e            movea.l   _in_buff,a0
[00000f9a] d1f9 0000 6180            adda.l    _in_ind,a0
[00000fa0] 0c10 006d                 cmpi.b    #$6D,(a0)
[00000fa4] 6600 031e                 bne       $000012C4
[00000fa8] 2079 0000 602e            movea.l   _in_buff,a0
[00000fae] d1f9 0000 6180            adda.l    _in_ind,a0
[00000fb4] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00000fba] 6600 0308                 bne       $000012C4
[00000fbe] 2079 0000 602e            movea.l   _in_buff,a0
[00000fc4] d1f9 0000 6180            adda.l    _in_ind,a0
[00000fca] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[00000fd0] 6600 02f2                 bne       $000012C4
[00000fd4] 2039 0000 6180            move.l    _in_ind,d0
[00000fda] d0bc 0000 0005            add.l     #$00000005,d0
[00000fe0] 23c0 0000 6056            move.l    d0,_tmp_ind
[00000fe6] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[00000fec] 4eb9 0000 0b46            jsr       _getnum
[00000ff2] 4a40                      tst.w     d0
[00000ff4] 6700 02ce                 beq       $000012C4
[00000ff8] 2079 0000 602e            movea.l   _in_buff,a0
[00000ffe] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001004] 0c10 0028                 cmpi.b    #$28,(a0)
[00001008] 6600 02ba                 bne       $000012C4
[0000100c] 2079 0000 602e            movea.l   _in_buff,a0
[00001012] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001018] 0c28 0052 0001            cmpi.b    #$52,1(a0)
[0000101e] 6600 02a4                 bne       $000012C4
[00001022] 2079 0000 602e            movea.l   _in_buff,a0
[00001028] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000102e] 0c28 0031 0002            cmpi.b    #$31,2(a0)
[00001034] 6600 028e                 bne       $000012C4
[00001038] 2079 0000 602e            movea.l   _in_buff,a0
[0000103e] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001044] 0c28 0029 0004            cmpi.b    #$29,4(a0)
[0000104a] 6600 0278                 bne       $000012C4
[0000104e] 2079 0000 602e            movea.l   _in_buff,a0
[00001054] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000105a] 0c28 002d 0006            cmpi.b    #$2D,6(a0)
[00001060] 6600 0262                 bne       $000012C4
[00001064] 2079 0000 602e            movea.l   _in_buff,a0
[0000106a] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001070] 0c28 0073 0008            cmpi.b    #$73,8(a0)
[00001076] 6600 024c                 bne       $000012C4
[0000107a] 2079 0000 602e            movea.l   _in_buff,a0
[00001080] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001086] 13e8 0003 0000 618c       move.b    3(a0),_regnum
[0000108e] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[00001098] 4eb9 0000 22c4            jsr       _nextlin
[0000109e] 2079 0000 602e            movea.l   _in_buff,a0
[000010a4] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000010aa] 0c10 006d                 cmpi.b    #$6D,(a0)
[000010ae] 6600 0214                 bne       $000012C4
[000010b2] 2079 0000 602e            movea.l   _in_buff,a0
[000010b8] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000010be] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[000010c4] 6600 01fe                 bne       $000012C4
[000010c8] 2079 0000 602e            movea.l   _in_buff,a0
[000010ce] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000010d4] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[000010da] 6600 01e8                 bne       $000012C4
[000010de] 2039 0000 606e            move.l    _nxt_lin,d0
[000010e4] d0bc 0000 0005            add.l     #$00000005,d0
[000010ea] 23c0 0000 6056            move.l    d0,_tmp_ind
[000010f0] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[000010f6] 4eb9 0000 0b46            jsr       _getnum
[000010fc] 4a40                      tst.w     d0
[000010fe] 6700 01c4                 beq       $000012C4
[00001102] 2079 0000 602e            movea.l   _in_buff,a0
[00001108] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000110e] 0c10 0028                 cmpi.b    #$28,(a0)
[00001112] 6600 01b0                 bne       $000012C4
[00001116] 2079 0000 602e            movea.l   _in_buff,a0
[0000111c] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001122] 0c28 0052 0001            cmpi.b    #$52,1(a0)
[00001128] 6600 019a                 bne       $000012C4
[0000112c] 2079 0000 602e            movea.l   _in_buff,a0
[00001132] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001138] 0c28 0031 0002            cmpi.b    #$31,2(a0)
[0000113e] 6600 0184                 bne       $000012C4
[00001142] 2079 0000 602e            movea.l   _in_buff,a0
[00001148] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000114e] 0c28 0029 0004            cmpi.b    #$29,4(a0)
[00001154] 6600 016e                 bne       $000012C4
[00001158] 2079 0000 602e            movea.l   _in_buff,a0
[0000115e] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001164] 0c28 002d 0006            cmpi.b    #$2D,6(a0)
[0000116a] 6600 0158                 bne       $000012C4
[0000116e] 2079 0000 602e            movea.l   _in_buff,a0
[00001174] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000117a] 0c28 0073 0008            cmpi.b    #$73,8(a0)
[00001180] 6600 0142                 bne       $000012C4
[00001184] 2079 0000 6056            movea.l   _tmp_ind,a0
[0000118a] 2239 0000 602e            move.l    _in_buff,d1
[00001190] 1030 1803                 move.b    3(a0,d1.l),d0
[00001194] 4880                      ext.w     d0
[00001196] b039 0000 618c            cmp.b     _regnum,d0
[0000119c] 6600 0126                 bne       $000012C4
[000011a0] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[000011a6] 4eb9 0000 0a92            jsr       _atoi
[000011ac] 3f00                      move.w    d0,-(a7)
[000011ae] 2f3c 0000 5fea            move.l    #_numstr,-(a7)
[000011b4] 4eb9 0000 0a92            jsr       _atoi
[000011ba] 588f                      addq.l    #4,a7
[000011bc] 905f                      sub.w     (a7)+,d0
[000011be] b07c 0002                 cmp.w     #$0002,d0
[000011c2] 6600 0100                 bne       $000012C4
[000011c6] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[000011d0] 2079 0000 6060            movea.l   _out_ind,a0
[000011d6] d1f9 0000 6064            adda.l    _out_buf,a0
[000011dc] 2279 0000 6180            movea.l   _in_ind,a1
[000011e2] d3f9 0000 602e            adda.l    _in_buff,a1
[000011e8] 1091                      move.b    (a1),(a0)
[000011ea] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000011f4] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000011fe] 2079 0000 6060            movea.l   _out_ind,a0
[00001204] d1f9 0000 6064            adda.l    _out_buf,a0
[0000120a] 2279 0000 6180            movea.l   _in_ind,a1
[00001210] d3f9 0000 602e            adda.l    _in_buff,a1
[00001216] 1091                      move.b    (a1),(a0)
[00001218] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001222] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000122c] 2079 0000 6060            movea.l   _out_ind,a0
[00001232] d1f9 0000 6064            adda.l    _out_buf,a0
[00001238] 2279 0000 6180            movea.l   _in_ind,a1
[0000123e] d3f9 0000 602e            adda.l    _in_buff,a1
[00001244] 1091                      move.b    (a1),(a0)
[00001246] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001250] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000125a] 2079 0000 6060            movea.l   _out_ind,a0
[00001260] d1f9 0000 6064            adda.l    _out_buf,a0
[00001266] 2279 0000 6180            movea.l   _in_ind,a1
[0000126c] d3f9 0000 602e            adda.l    _in_buff,a1
[00001272] 1091                      move.b    (a1),(a0)
[00001274] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000127e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001288] 2079 0000 6060            movea.l   _out_ind,a0
[0000128e] d1f9 0000 6064            adda.l    _out_buf,a0
[00001294] 10bc 002e                 move.b    #$2E,(a0)
[00001298] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000012a2] 2079 0000 6060            movea.l   _out_ind,a0
[000012a8] d1f9 0000 6064            adda.l    _out_buf,a0
[000012ae] 10bc 006c                 move.b    #$6C,(a0)
[000012b2] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000012bc] 0679 0001 0000 6074       addi.w    #$0001,_mov_tot
[000012c4] 2079 0000 6060            movea.l   _out_ind,a0
[000012ca] d1f9 0000 6064            adda.l    _out_buf,a0
[000012d0] 2279 0000 6180            movea.l   _in_ind,a1
[000012d6] d3f9 0000 602e            adda.l    _in_buff,a1
[000012dc] 1091                      move.b    (a1),(a0)
[000012de] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000012e8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000012f2] 2039 0000 6180            move.l    _in_ind,d0
[000012f8] b0b9 0000 5ff6            cmp.l     _size,d0
[000012fe] 6d00 fc40                 blt       $00000F40
[00001302] 4e5e                      unlk      a6
[00001304] 4e75                      rts       
[00001306] _mvwtol:
[00001306] 4e56 fffc                 link      a6,#$FFFC
[0000130a] 42b9 0000 6180            clr.l     _in_ind
[00001310] 42b9 0000 6060            clr.l     _out_ind
[00001316] 6000 03d6                 bra       $000016EE
[0000131a] 6000 0030                 bra       $0000134C
[0000131e] 2079 0000 6060            movea.l   _out_ind,a0
[00001324] d1f9 0000 6064            adda.l    _out_buf,a0
[0000132a] 2279 0000 6180            movea.l   _in_ind,a1
[00001330] d3f9 0000 602e            adda.l    _in_buff,a1
[00001336] 1091                      move.b    (a1),(a0)
[00001338] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001342] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000134c] 2079 0000 602e            movea.l   _in_buff,a0
[00001352] d1f9 0000 6180            adda.l    _in_ind,a0
[00001358] 0c10 006d                 cmpi.b    #$6D,(a0)
[0000135c] 6700 0010                 beq       $0000136E
[00001360] 2039 0000 6180            move.l    _in_ind,d0
[00001366] b0b9 0000 5ff6            cmp.l     _size,d0
[0000136c] 6db0                      blt.s     $0000131E
[0000136e] 2079 0000 602e            movea.l   _in_buff,a0
[00001374] d1f9 0000 6180            adda.l    _in_ind,a0
[0000137a] 0c10 006d                 cmpi.b    #$6D,(a0)
[0000137e] 6600 0340                 bne       $000016C0
[00001382] 2079 0000 602e            movea.l   _in_buff,a0
[00001388] d1f9 0000 6180            adda.l    _in_ind,a0
[0000138e] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00001394] 6600 032a                 bne       $000016C0
[00001398] 2079 0000 602e            movea.l   _in_buff,a0
[0000139e] d1f9 0000 6180            adda.l    _in_ind,a0
[000013a4] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[000013aa] 6600 0314                 bne       $000016C0
[000013ae] 2039 0000 6180            move.l    _in_ind,d0
[000013b4] d0bc 0000 0005            add.l     #$00000005,d0
[000013ba] 23c0 0000 6056            move.l    d0,_tmp_ind
[000013c0] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[000013c6] 4eb9 0000 0b46            jsr       _getnum
[000013cc] 4a40                      tst.w     d0
[000013ce] 6700 02f0                 beq       $000016C0
[000013d2] 2079 0000 602e            movea.l   _in_buff,a0
[000013d8] d1f9 0000 6056            adda.l    _tmp_ind,a0
[000013de] 0c10 0028                 cmpi.b    #$28,(a0)
[000013e2] 6600 02dc                 bne       $000016C0
[000013e6] 2079 0000 602e            movea.l   _in_buff,a0
[000013ec] d1f9 0000 6056            adda.l    _tmp_ind,a0
[000013f2] 0c28 0052 0001            cmpi.b    #$52,1(a0)
[000013f8] 6600 02c6                 bne       $000016C0
[000013fc] 2079 0000 602e            movea.l   _in_buff,a0
[00001402] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001408] 0c28 0031 0002            cmpi.b    #$31,2(a0)
[0000140e] 6600 02b0                 bne       $000016C0
[00001412] 2079 0000 602e            movea.l   _in_buff,a0
[00001418] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000141e] 0c28 0029 0004            cmpi.b    #$29,4(a0)
[00001424] 6600 029a                 bne       $000016C0
[00001428] 2079 0000 602e            movea.l   _in_buff,a0
[0000142e] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001434] 0c28 0028 0006            cmpi.b    #$28,6(a0)
[0000143a] 6600 0284                 bne       $000016C0
[0000143e] 2079 0000 602e            movea.l   _in_buff,a0
[00001444] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000144a] 0c28 0073 0007            cmpi.b    #$73,7(a0)
[00001450] 6600 026e                 bne       $000016C0
[00001454] 2079 0000 602e            movea.l   _in_buff,a0
[0000145a] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001460] 0c28 000d 000a            cmpi.b    #$0D,10(a0)
[00001466] 6600 0258                 bne       $000016C0
[0000146a] 2079 0000 602e            movea.l   _in_buff,a0
[00001470] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001476] 13e8 0003 0000 618c       move.b    3(a0),_regnum
[0000147e] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[00001488] 4eb9 0000 22c4            jsr       _nextlin
[0000148e] 2079 0000 602e            movea.l   _in_buff,a0
[00001494] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000149a] 0c10 006d                 cmpi.b    #$6D,(a0)
[0000149e] 6600 0220                 bne       $000016C0
[000014a2] 2079 0000 602e            movea.l   _in_buff,a0
[000014a8] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000014ae] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[000014b4] 6600 020a                 bne       $000016C0
[000014b8] 2079 0000 602e            movea.l   _in_buff,a0
[000014be] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000014c4] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[000014ca] 6600 01f4                 bne       $000016C0
[000014ce] 2039 0000 606e            move.l    _nxt_lin,d0
[000014d4] d0bc 0000 0005            add.l     #$00000005,d0
[000014da] 23c0 0000 6056            move.l    d0,_tmp_ind
[000014e0] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[000014e6] 4eb9 0000 0b46            jsr       _getnum
[000014ec] 4a40                      tst.w     d0
[000014ee] 6700 01d0                 beq       $000016C0
[000014f2] 2079 0000 602e            movea.l   _in_buff,a0
[000014f8] d1f9 0000 6056            adda.l    _tmp_ind,a0
[000014fe] 0c10 0028                 cmpi.b    #$28,(a0)
[00001502] 6600 01bc                 bne       $000016C0
[00001506] 2079 0000 602e            movea.l   _in_buff,a0
[0000150c] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001512] 0c28 0052 0001            cmpi.b    #$52,1(a0)
[00001518] 6600 01a6                 bne       $000016C0
[0000151c] 2079 0000 602e            movea.l   _in_buff,a0
[00001522] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001528] 0c28 0031 0002            cmpi.b    #$31,2(a0)
[0000152e] 6600 0190                 bne       $000016C0
[00001532] 2079 0000 602e            movea.l   _in_buff,a0
[00001538] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000153e] 0c28 0029 0004            cmpi.b    #$29,4(a0)
[00001544] 6600 017a                 bne       $000016C0
[00001548] 2079 0000 602e            movea.l   _in_buff,a0
[0000154e] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001554] 0c28 002d 0006            cmpi.b    #$2D,6(a0)
[0000155a] 6600 0164                 bne       $000016C0
[0000155e] 2079 0000 602e            movea.l   _in_buff,a0
[00001564] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000156a] 0c28 0073 0008            cmpi.b    #$73,8(a0)
[00001570] 6600 014e                 bne       $000016C0
[00001574] 2079 0000 6056            movea.l   _tmp_ind,a0
[0000157a] 2239 0000 602e            move.l    _in_buff,d1
[00001580] 1030 1803                 move.b    3(a0,d1.l),d0
[00001584] 4880                      ext.w     d0
[00001586] b039 0000 618c            cmp.b     _regnum,d0
[0000158c] 6600 0132                 bne       $000016C0
[00001590] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00001596] 4eb9 0000 0a92            jsr       _atoi
[0000159c] 3f00                      move.w    d0,-(a7)
[0000159e] 2f3c 0000 5fea            move.l    #_numstr,-(a7)
[000015a4] 4eb9 0000 0a92            jsr       _atoi
[000015aa] 588f                      addq.l    #4,a7
[000015ac] 905f                      sub.w     (a7)+,d0
[000015ae] b07c 0002                 cmp.w     #$0002,d0
[000015b2] 6600 010c                 bne       $000016C0
[000015b6] 4eb9 0000 36a0            jsr       _addinra
[000015bc] 4a40                      tst.w     d0
[000015be] 6700 0100                 beq       $000016C0
[000015c2] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[000015cc] 2079 0000 6060            movea.l   _out_ind,a0
[000015d2] d1f9 0000 6064            adda.l    _out_buf,a0
[000015d8] 2279 0000 6180            movea.l   _in_ind,a1
[000015de] d3f9 0000 602e            adda.l    _in_buff,a1
[000015e4] 1091                      move.b    (a1),(a0)
[000015e6] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000015f0] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000015fa] 2079 0000 6060            movea.l   _out_ind,a0
[00001600] d1f9 0000 6064            adda.l    _out_buf,a0
[00001606] 2279 0000 6180            movea.l   _in_ind,a1
[0000160c] d3f9 0000 602e            adda.l    _in_buff,a1
[00001612] 1091                      move.b    (a1),(a0)
[00001614] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000161e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001628] 2079 0000 6060            movea.l   _out_ind,a0
[0000162e] d1f9 0000 6064            adda.l    _out_buf,a0
[00001634] 2279 0000 6180            movea.l   _in_ind,a1
[0000163a] d3f9 0000 602e            adda.l    _in_buff,a1
[00001640] 1091                      move.b    (a1),(a0)
[00001642] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000164c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001656] 2079 0000 6060            movea.l   _out_ind,a0
[0000165c] d1f9 0000 6064            adda.l    _out_buf,a0
[00001662] 2279 0000 6180            movea.l   _in_ind,a1
[00001668] d3f9 0000 602e            adda.l    _in_buff,a1
[0000166e] 1091                      move.b    (a1),(a0)
[00001670] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000167a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001684] 2079 0000 6060            movea.l   _out_ind,a0
[0000168a] d1f9 0000 6064            adda.l    _out_buf,a0
[00001690] 10bc 002e                 move.b    #$2E,(a0)
[00001694] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000169e] 2079 0000 6060            movea.l   _out_ind,a0
[000016a4] d1f9 0000 6064            adda.l    _out_buf,a0
[000016aa] 10bc 006c                 move.b    #$6C,(a0)
[000016ae] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000016b8] 0679 0001 0000 5ffc       addi.w    #$0001,_mov1_to
[000016c0] 2079 0000 6060            movea.l   _out_ind,a0
[000016c6] d1f9 0000 6064            adda.l    _out_buf,a0
[000016cc] 2279 0000 6180            movea.l   _in_ind,a1
[000016d2] d3f9 0000 602e            adda.l    _in_buff,a1
[000016d8] 1091                      move.b    (a1),(a0)
[000016da] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000016e4] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000016ee] 2039 0000 6180            move.l    _in_ind,d0
[000016f4] b0b9 0000 5ff6            cmp.l     _size,d0
[000016fa] 6d00 fc1e                 blt       $0000131A
[000016fe] 4e5e                      unlk      a6
[00001700] 4e75                      rts       
[00001702] _clrwtol:
[00001702] 4e56 fffc                 link      a6,#$FFFC
[00001706] 42b9 0000 6180            clr.l     _in_ind
[0000170c] 42b9 0000 6060            clr.l     _out_ind
[00001712] 6000 0274                 bra       $00001988
[00001716] 6000 0030                 bra       $00001748
[0000171a] 2079 0000 6060            movea.l   _out_ind,a0
[00001720] d1f9 0000 6064            adda.l    _out_buf,a0
[00001726] 2279 0000 6180            movea.l   _in_ind,a1
[0000172c] d3f9 0000 602e            adda.l    _in_buff,a1
[00001732] 1091                      move.b    (a1),(a0)
[00001734] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000173e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001748] 2079 0000 602e            movea.l   _in_buff,a0
[0000174e] d1f9 0000 6180            adda.l    _in_ind,a0
[00001754] 0c10 0063                 cmpi.b    #$63,(a0)
[00001758] 6700 0010                 beq       $0000176A
[0000175c] 2039 0000 6180            move.l    _in_ind,d0
[00001762] b0b9 0000 5ff6            cmp.l     _size,d0
[00001768] 6db0                      blt.s     $0000171A
[0000176a] 2079 0000 602e            movea.l   _in_buff,a0
[00001770] d1f9 0000 6180            adda.l    _in_ind,a0
[00001776] 0c10 0063                 cmpi.b    #$63,(a0)
[0000177a] 6600 01de                 bne       $0000195A
[0000177e] 2079 0000 602e            movea.l   _in_buff,a0
[00001784] d1f9 0000 6180            adda.l    _in_ind,a0
[0000178a] 0c28 006c 0001            cmpi.b    #$6C,1(a0)
[00001790] 6600 01c8                 bne       $0000195A
[00001794] 2079 0000 602e            movea.l   _in_buff,a0
[0000179a] d1f9 0000 6180            adda.l    _in_ind,a0
[000017a0] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[000017a6] 6600 01b2                 bne       $0000195A
[000017aa] 2079 0000 602e            movea.l   _in_buff,a0
[000017b0] d1f9 0000 6180            adda.l    _in_ind,a0
[000017b6] 0c28 0028 0004            cmpi.b    #$28,4(a0)
[000017bc] 6600 019c                 bne       $0000195A
[000017c0] 2079 0000 602e            movea.l   _in_buff,a0
[000017c6] d1f9 0000 6180            adda.l    _in_ind,a0
[000017cc] 0c28 0073 0005            cmpi.b    #$73,5(a0)
[000017d2] 6600 0186                 bne       $0000195A
[000017d6] 2079 0000 602e            movea.l   _in_buff,a0
[000017dc] d1f9 0000 6180            adda.l    _in_ind,a0
[000017e2] 0c28 000d 0008            cmpi.b    #$0D,8(a0)
[000017e8] 6600 0170                 bne       $0000195A
[000017ec] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[000017f6] 4eb9 0000 22c4            jsr       _nextlin
[000017fc] 2079 0000 602e            movea.l   _in_buff,a0
[00001802] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001808] 0c10 0063                 cmpi.b    #$63,(a0)
[0000180c] 6600 014c                 bne       $0000195A
[00001810] 2079 0000 602e            movea.l   _in_buff,a0
[00001816] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000181c] 0c28 006c 0001            cmpi.b    #$6C,1(a0)
[00001822] 6600 0136                 bne       $0000195A
[00001826] 2079 0000 602e            movea.l   _in_buff,a0
[0000182c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001832] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[00001838] 6600 0120                 bne       $0000195A
[0000183c] 2079 0000 602e            movea.l   _in_buff,a0
[00001842] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001848] 0c28 002d 0004            cmpi.b    #$2D,4(a0)
[0000184e] 6600 010a                 bne       $0000195A
[00001852] 2079 0000 602e            movea.l   _in_buff,a0
[00001858] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000185e] 0c28 0028 0005            cmpi.b    #$28,5(a0)
[00001864] 6600 00f4                 bne       $0000195A
[00001868] 2079 0000 602e            movea.l   _in_buff,a0
[0000186e] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001874] 0c28 0073 0006            cmpi.b    #$73,6(a0)
[0000187a] 6600 00de                 bne       $0000195A
[0000187e] 4eb9 0000 36a0            jsr       _addinra
[00001884] 4a40                      tst.w     d0
[00001886] 6700 00d2                 beq       $0000195A
[0000188a] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[00001894] 2079 0000 6060            movea.l   _out_ind,a0
[0000189a] d1f9 0000 6064            adda.l    _out_buf,a0
[000018a0] 2279 0000 6180            movea.l   _in_ind,a1
[000018a6] d3f9 0000 602e            adda.l    _in_buff,a1
[000018ac] 1091                      move.b    (a1),(a0)
[000018ae] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000018b8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000018c2] 2079 0000 6060            movea.l   _out_ind,a0
[000018c8] d1f9 0000 6064            adda.l    _out_buf,a0
[000018ce] 2279 0000 6180            movea.l   _in_ind,a1
[000018d4] d3f9 0000 602e            adda.l    _in_buff,a1
[000018da] 1091                      move.b    (a1),(a0)
[000018dc] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000018e6] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000018f0] 2079 0000 6060            movea.l   _out_ind,a0
[000018f6] d1f9 0000 6064            adda.l    _out_buf,a0
[000018fc] 2279 0000 6180            movea.l   _in_ind,a1
[00001902] d3f9 0000 602e            adda.l    _in_buff,a1
[00001908] 1091                      move.b    (a1),(a0)
[0000190a] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001914] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000191e] 2079 0000 6060            movea.l   _out_ind,a0
[00001924] d1f9 0000 6064            adda.l    _out_buf,a0
[0000192a] 10bc 002e                 move.b    #$2E,(a0)
[0000192e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001938] 2079 0000 6060            movea.l   _out_ind,a0
[0000193e] d1f9 0000 6064            adda.l    _out_buf,a0
[00001944] 10bc 006c                 move.b    #$6C,(a0)
[00001948] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001952] 0679 0001 0000 5ff4       addi.w    #$0001,_clr1_to
[0000195a] 2079 0000 6060            movea.l   _out_ind,a0
[00001960] d1f9 0000 6064            adda.l    _out_buf,a0
[00001966] 2279 0000 6180            movea.l   _in_ind,a1
[0000196c] d3f9 0000 602e            adda.l    _in_buff,a1
[00001972] 1091                      move.b    (a1),(a0)
[00001974] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000197e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001988] 2039 0000 6180            move.l    _in_ind,d0
[0000198e] b0b9 0000 5ff6            cmp.l     _size,d0
[00001994] 6d00 fd80                 blt       $00001716
[00001998] 4e5e                      unlk      a6
[0000199a] 4e75                      rts       
[0000199c] _imoveop:
[0000199c] 4e56 fffa                 link      a6,#$FFFA
[000019a0] 42b9 0000 6180            clr.l     _in_ind
[000019a6] 42b9 0000 6060            clr.l     _out_ind
[000019ac] 6000 044c                 bra       $00001DFA
[000019b0] 6000 0030                 bra       $000019E2
[000019b4] 2079 0000 6060            movea.l   _out_ind,a0
[000019ba] d1f9 0000 6064            adda.l    _out_buf,a0
[000019c0] 2279 0000 6180            movea.l   _in_ind,a1
[000019c6] d3f9 0000 602e            adda.l    _in_buff,a1
[000019cc] 1091                      move.b    (a1),(a0)
[000019ce] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000019d8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000019e2] 2079 0000 602e            movea.l   _in_buff,a0
[000019e8] d1f9 0000 6180            adda.l    _in_ind,a0
[000019ee] 0c10 006d                 cmpi.b    #$6D,(a0)
[000019f2] 6700 0010                 beq       $00001A04
[000019f6] 2039 0000 6180            move.l    _in_ind,d0
[000019fc] b0b9 0000 5ff6            cmp.l     _size,d0
[00001a02] 6db0                      blt.s     $000019B4
[00001a04] 2079 0000 602e            movea.l   _in_buff,a0
[00001a0a] d1f9 0000 6180            adda.l    _in_ind,a0
[00001a10] 0c10 006d                 cmpi.b    #$6D,(a0)
[00001a14] 6600 03b6                 bne       $00001DCC
[00001a18] 2079 0000 602e            movea.l   _in_buff,a0
[00001a1e] d1f9 0000 6180            adda.l    _in_ind,a0
[00001a24] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00001a2a] 6600 03a0                 bne       $00001DCC
[00001a2e] 2079 0000 602e            movea.l   _in_buff,a0
[00001a34] d1f9 0000 6180            adda.l    _in_ind,a0
[00001a3a] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[00001a40] 6600 038a                 bne       $00001DCC
[00001a44] 2079 0000 602e            movea.l   _in_buff,a0
[00001a4a] d1f9 0000 6180            adda.l    _in_ind,a0
[00001a50] 0c28 0023 0005            cmpi.b    #$23,5(a0)
[00001a56] 6600 0374                 bne       $00001DCC
[00001a5a] 2039 0000 6180            move.l    _in_ind,d0
[00001a60] d0bc 0000 0006            add.l     #$00000006,d0
[00001a66] 23c0 0000 6056            move.l    d0,_tmp_ind
[00001a6c] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[00001a72] 4eb9 0000 0b46            jsr       _getnum
[00001a78] 4a40                      tst.w     d0
[00001a7a] 6700 0350                 beq       $00001DCC
[00001a7e] 2079 0000 602e            movea.l   _in_buff,a0
[00001a84] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001a8a] 0c10 002c                 cmpi.b    #$2C,(a0)
[00001a8e] 6600 033c                 bne       $00001DCC
[00001a92] 2079 0000 602e            movea.l   _in_buff,a0
[00001a98] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001a9e] 0c28 002d 0001            cmpi.b    #$2D,1(a0)
[00001aa4] 6600 0326                 bne       $00001DCC
[00001aa8] 2079 0000 602e            movea.l   _in_buff,a0
[00001aae] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001ab4] 0c28 0028 0002            cmpi.b    #$28,2(a0)
[00001aba] 6600 0310                 bne       $00001DCC
[00001abe] 2079 0000 602e            movea.l   _in_buff,a0
[00001ac4] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001aca] 0c28 0073 0003            cmpi.b    #$73,3(a0)
[00001ad0] 6600 02fa                 bne       $00001DCC
[00001ad4] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[00001ade] 4eb9 0000 22c4            jsr       _nextlin
[00001ae4] 2079 0000 602e            movea.l   _in_buff,a0
[00001aea] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001af0] 0c10 006d                 cmpi.b    #$6D,(a0)
[00001af4] 6600 02d6                 bne       $00001DCC
[00001af8] 2079 0000 602e            movea.l   _in_buff,a0
[00001afe] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001b04] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00001b0a] 6600 02c0                 bne       $00001DCC
[00001b0e] 2079 0000 602e            movea.l   _in_buff,a0
[00001b14] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001b1a] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[00001b20] 6600 02aa                 bne       $00001DCC
[00001b24] 2079 0000 602e            movea.l   _in_buff,a0
[00001b2a] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001b30] 0c28 0023 0005            cmpi.b    #$23,5(a0)
[00001b36] 6600 0294                 bne       $00001DCC
[00001b3a] 2039 0000 606e            move.l    _nxt_lin,d0
[00001b40] d0bc 0000 0006            add.l     #$00000006,d0
[00001b46] 23c0 0000 6056            move.l    d0,_tmp_ind
[00001b4c] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00001b52] 4eb9 0000 0b46            jsr       _getnum
[00001b58] 4a40                      tst.w     d0
[00001b5a] 6700 0270                 beq       $00001DCC
[00001b5e] 2079 0000 602e            movea.l   _in_buff,a0
[00001b64] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001b6a] 0c10 002c                 cmpi.b    #$2C,(a0)
[00001b6e] 6600 025c                 bne       $00001DCC
[00001b72] 2079 0000 602e            movea.l   _in_buff,a0
[00001b78] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001b7e] 0c28 002d 0001            cmpi.b    #$2D,1(a0)
[00001b84] 6600 0246                 bne       $00001DCC
[00001b88] 2079 0000 602e            movea.l   _in_buff,a0
[00001b8e] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001b94] 0c28 0028 0002            cmpi.b    #$28,2(a0)
[00001b9a] 6600 0230                 bne       $00001DCC
[00001b9e] 2079 0000 602e            movea.l   _in_buff,a0
[00001ba4] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001baa] 0c28 0073 0003            cmpi.b    #$73,3(a0)
[00001bb0] 6600 021a                 bne       $00001DCC
[00001bb4] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[00001bba] 4eb9 0000 0a92            jsr       _atoi
[00001bc0] 33c0 0000 6176            move.w    d0,_num
[00001bc6] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00001bcc] 4eb9 0000 0a92            jsr       _atoi
[00001bd2] 33c0 0000 6188            move.w    d0,_num1
[00001bd8] 3eb9 0000 6188            move.w    _num1,(a7)
[00001bde] 2f3c 0000 850e            move.l    #_hexstr,-(a7)
[00001be4] 4eb9 0000 0982            jsr       _htoa
[00001bea] 588f                      addq.l    #4,a7
[00001bec] 3eb9 0000 6176            move.w    _num,(a7)
[00001bf2] 2f3c 0000 8512            move.l    #$00008512,-(a7)
[00001bf8] 4eb9 0000 0982            jsr       _htoa
[00001bfe] 588f                      addq.l    #4,a7
[00001c00] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[00001c0a] 2079 0000 6060            movea.l   _out_ind,a0
[00001c10] d1f9 0000 6064            adda.l    _out_buf,a0
[00001c16] 2279 0000 6180            movea.l   _in_ind,a1
[00001c1c] d3f9 0000 602e            adda.l    _in_buff,a1
[00001c22] 1091                      move.b    (a1),(a0)
[00001c24] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001c2e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001c38] 2079 0000 6060            movea.l   _out_ind,a0
[00001c3e] d1f9 0000 6064            adda.l    _out_buf,a0
[00001c44] 2279 0000 6180            movea.l   _in_ind,a1
[00001c4a] d3f9 0000 602e            adda.l    _in_buff,a1
[00001c50] 1091                      move.b    (a1),(a0)
[00001c52] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001c5c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001c66] 2079 0000 6060            movea.l   _out_ind,a0
[00001c6c] d1f9 0000 6064            adda.l    _out_buf,a0
[00001c72] 2279 0000 6180            movea.l   _in_ind,a1
[00001c78] d3f9 0000 602e            adda.l    _in_buff,a1
[00001c7e] 1091                      move.b    (a1),(a0)
[00001c80] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001c8a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001c94] 2079 0000 6060            movea.l   _out_ind,a0
[00001c9a] d1f9 0000 6064            adda.l    _out_buf,a0
[00001ca0] 2279 0000 6180            movea.l   _in_ind,a1
[00001ca6] d3f9 0000 602e            adda.l    _in_buff,a1
[00001cac] 1091                      move.b    (a1),(a0)
[00001cae] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001cb8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001cc2] 2079 0000 6060            movea.l   _out_ind,a0
[00001cc8] d1f9 0000 6064            adda.l    _out_buf,a0
[00001cce] 10bc 002e                 move.b    #$2E,(a0)
[00001cd2] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001cdc] 2079 0000 6060            movea.l   _out_ind,a0
[00001ce2] d1f9 0000 6064            adda.l    _out_buf,a0
[00001ce8] 10bc 006c                 move.b    #$6C,(a0)
[00001cec] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001cf6] 2079 0000 6060            movea.l   _out_ind,a0
[00001cfc] d1f9 0000 6064            adda.l    _out_buf,a0
[00001d02] 2279 0000 6180            movea.l   _in_ind,a1
[00001d08] d3f9 0000 602e            adda.l    _in_buff,a1
[00001d0e] 1091                      move.b    (a1),(a0)
[00001d10] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001d1a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001d24] 2079 0000 6060            movea.l   _out_ind,a0
[00001d2a] d1f9 0000 6064            adda.l    _out_buf,a0
[00001d30] 2279 0000 6180            movea.l   _in_ind,a1
[00001d36] d3f9 0000 602e            adda.l    _in_buff,a1
[00001d3c] 1091                      move.b    (a1),(a0)
[00001d3e] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001d48] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001d52] 2079 0000 6060            movea.l   _out_ind,a0
[00001d58] d1f9 0000 6064            adda.l    _out_buf,a0
[00001d5e] 10bc 0024                 move.b    #$24,(a0)
[00001d62] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001d6c] 426e fffe                 clr.w     -2(a6)
[00001d70] 6000 002a                 bra       $00001D9C
[00001d74] 2079 0000 6060            movea.l   _out_ind,a0
[00001d7a] d1f9 0000 6064            adda.l    _out_buf,a0
[00001d80] 326e fffe                 movea.w   -2(a6),a1
[00001d84] d3fc 0000 850e            adda.l    #_hexstr,a1
[00001d8a] 1091                      move.b    (a1),(a0)
[00001d8c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001d96] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[00001d9c] 0c6e 0008 fffe            cmpi.w    #$0008,-2(a6)
[00001da2] 6dd0                      blt.s     $00001D74
[00001da4] 6000 000c                 bra       $00001DB2
[00001da8] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001db2] 2079 0000 602e            movea.l   _in_buff,a0
[00001db8] d1f9 0000 6180            adda.l    _in_ind,a0
[00001dbe] 0c10 002c                 cmpi.b    #$2C,(a0)
[00001dc2] 66e4                      bne.s     $00001DA8
[00001dc4] 0679 0001 0000 6072       addi.w    #$0001,_imov_to
[00001dcc] 2079 0000 6060            movea.l   _out_ind,a0
[00001dd2] d1f9 0000 6064            adda.l    _out_buf,a0
[00001dd8] 2279 0000 6180            movea.l   _in_ind,a1
[00001dde] d3f9 0000 602e            adda.l    _in_buff,a1
[00001de4] 1091                      move.b    (a1),(a0)
[00001de6] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001df0] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001dfa] 2039 0000 6180            move.l    _in_ind,d0
[00001e00] b0b9 0000 5ff6            cmp.l     _size,d0
[00001e06] 6d00 fba8                 blt       $000019B0
[00001e0a] 4e5e                      unlk      a6
[00001e0c] 4e75                      rts       
[00001e0e] _imov1_o:
[00001e0e] 4e56 fffa                 link      a6,#$FFFA
[00001e12] 42b9 0000 6180            clr.l     _in_ind
[00001e18] 42b9 0000 6060            clr.l     _out_ind
[00001e1e] 6000 0458                 bra       $00002278
[00001e22] 6000 0030                 bra       $00001E54
[00001e26] 2079 0000 6060            movea.l   _out_ind,a0
[00001e2c] d1f9 0000 6064            adda.l    _out_buf,a0
[00001e32] 2279 0000 6180            movea.l   _in_ind,a1
[00001e38] d3f9 0000 602e            adda.l    _in_buff,a1
[00001e3e] 1091                      move.b    (a1),(a0)
[00001e40] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00001e4a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00001e54] 2079 0000 602e            movea.l   _in_buff,a0
[00001e5a] d1f9 0000 6180            adda.l    _in_ind,a0
[00001e60] 0c10 006d                 cmpi.b    #$6D,(a0)
[00001e64] 6700 0010                 beq       $00001E76
[00001e68] 2039 0000 6180            move.l    _in_ind,d0
[00001e6e] b0b9 0000 5ff6            cmp.l     _size,d0
[00001e74] 6db0                      blt.s     $00001E26
[00001e76] 2079 0000 602e            movea.l   _in_buff,a0
[00001e7c] d1f9 0000 6180            adda.l    _in_ind,a0
[00001e82] 0c10 006d                 cmpi.b    #$6D,(a0)
[00001e86] 6600 03c2                 bne       $0000224A
[00001e8a] 2079 0000 602e            movea.l   _in_buff,a0
[00001e90] d1f9 0000 6180            adda.l    _in_ind,a0
[00001e96] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00001e9c] 6600 03ac                 bne       $0000224A
[00001ea0] 2079 0000 602e            movea.l   _in_buff,a0
[00001ea6] d1f9 0000 6180            adda.l    _in_ind,a0
[00001eac] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[00001eb2] 6600 0396                 bne       $0000224A
[00001eb6] 2079 0000 602e            movea.l   _in_buff,a0
[00001ebc] d1f9 0000 6180            adda.l    _in_ind,a0
[00001ec2] 0c28 0023 0005            cmpi.b    #$23,5(a0)
[00001ec8] 6600 0380                 bne       $0000224A
[00001ecc] 2039 0000 6180            move.l    _in_ind,d0
[00001ed2] d0bc 0000 0006            add.l     #$00000006,d0
[00001ed8] 23c0 0000 6056            move.l    d0,_tmp_ind
[00001ede] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[00001ee4] 4eb9 0000 0b46            jsr       _getnum
[00001eea] 4a40                      tst.w     d0
[00001eec] 6700 035c                 beq       $0000224A
[00001ef0] 2079 0000 602e            movea.l   _in_buff,a0
[00001ef6] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001efc] 0c10 002c                 cmpi.b    #$2C,(a0)
[00001f00] 6600 0348                 bne       $0000224A
[00001f04] 2079 0000 602e            movea.l   _in_buff,a0
[00001f0a] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001f10] 0c28 0028 0001            cmpi.b    #$28,1(a0)
[00001f16] 6600 0332                 bne       $0000224A
[00001f1a] 2079 0000 602e            movea.l   _in_buff,a0
[00001f20] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001f26] 0c28 0073 0002            cmpi.b    #$73,2(a0)
[00001f2c] 6600 031c                 bne       $0000224A
[00001f30] 2079 0000 602e            movea.l   _in_buff,a0
[00001f36] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001f3c] 0c28 000d 0005            cmpi.b    #$0D,5(a0)
[00001f42] 6600 0306                 bne       $0000224A
[00001f46] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[00001f50] 4eb9 0000 22c4            jsr       _nextlin
[00001f56] 2079 0000 602e            movea.l   _in_buff,a0
[00001f5c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001f62] 0c10 006d                 cmpi.b    #$6D,(a0)
[00001f66] 6600 02e2                 bne       $0000224A
[00001f6a] 2079 0000 602e            movea.l   _in_buff,a0
[00001f70] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001f76] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00001f7c] 6600 02cc                 bne       $0000224A
[00001f80] 2079 0000 602e            movea.l   _in_buff,a0
[00001f86] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001f8c] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[00001f92] 6600 02b6                 bne       $0000224A
[00001f96] 2079 0000 602e            movea.l   _in_buff,a0
[00001f9c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00001fa2] 0c28 0023 0005            cmpi.b    #$23,5(a0)
[00001fa8] 6600 02a0                 bne       $0000224A
[00001fac] 2039 0000 606e            move.l    _nxt_lin,d0
[00001fb2] d0bc 0000 0006            add.l     #$00000006,d0
[00001fb8] 23c0 0000 6056            move.l    d0,_tmp_ind
[00001fbe] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00001fc4] 4eb9 0000 0b46            jsr       _getnum
[00001fca] 4a40                      tst.w     d0
[00001fcc] 6700 027c                 beq       $0000224A
[00001fd0] 2079 0000 602e            movea.l   _in_buff,a0
[00001fd6] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001fdc] 0c10 002c                 cmpi.b    #$2C,(a0)
[00001fe0] 6600 0268                 bne       $0000224A
[00001fe4] 2079 0000 602e            movea.l   _in_buff,a0
[00001fea] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00001ff0] 0c28 002d 0001            cmpi.b    #$2D,1(a0)
[00001ff6] 6600 0252                 bne       $0000224A
[00001ffa] 2079 0000 602e            movea.l   _in_buff,a0
[00002000] d1f9 0000 6056            adda.l    _tmp_ind,a0
[00002006] 0c28 0028 0002            cmpi.b    #$28,2(a0)
[0000200c] 6600 023c                 bne       $0000224A
[00002010] 2079 0000 602e            movea.l   _in_buff,a0
[00002016] d1f9 0000 6056            adda.l    _tmp_ind,a0
[0000201c] 0c28 0073 0003            cmpi.b    #$73,3(a0)
[00002022] 6600 0226                 bne       $0000224A
[00002026] 4eb9 0000 36a0            jsr       _addinra
[0000202c] 4a40                      tst.w     d0
[0000202e] 6700 021a                 beq       $0000224A
[00002032] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[00002038] 4eb9 0000 0a92            jsr       _atoi
[0000203e] 33c0 0000 6176            move.w    d0,_num
[00002044] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[0000204a] 4eb9 0000 0a92            jsr       _atoi
[00002050] 33c0 0000 6188            move.w    d0,_num1
[00002056] 3eb9 0000 6188            move.w    _num1,(a7)
[0000205c] 2f3c 0000 850e            move.l    #_hexstr,-(a7)
[00002062] 4eb9 0000 0982            jsr       _htoa
[00002068] 588f                      addq.l    #4,a7
[0000206a] 3eb9 0000 6176            move.w    _num,(a7)
[00002070] 2f3c 0000 8512            move.l    #$00008512,-(a7)
[00002076] 4eb9 0000 0982            jsr       _htoa
[0000207c] 588f                      addq.l    #4,a7
[0000207e] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[00002088] 2079 0000 6060            movea.l   _out_ind,a0
[0000208e] d1f9 0000 6064            adda.l    _out_buf,a0
[00002094] 2279 0000 6180            movea.l   _in_ind,a1
[0000209a] d3f9 0000 602e            adda.l    _in_buff,a1
[000020a0] 1091                      move.b    (a1),(a0)
[000020a2] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000020ac] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000020b6] 2079 0000 6060            movea.l   _out_ind,a0
[000020bc] d1f9 0000 6064            adda.l    _out_buf,a0
[000020c2] 2279 0000 6180            movea.l   _in_ind,a1
[000020c8] d3f9 0000 602e            adda.l    _in_buff,a1
[000020ce] 1091                      move.b    (a1),(a0)
[000020d0] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000020da] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000020e4] 2079 0000 6060            movea.l   _out_ind,a0
[000020ea] d1f9 0000 6064            adda.l    _out_buf,a0
[000020f0] 2279 0000 6180            movea.l   _in_ind,a1
[000020f6] d3f9 0000 602e            adda.l    _in_buff,a1
[000020fc] 1091                      move.b    (a1),(a0)
[000020fe] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00002108] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002112] 2079 0000 6060            movea.l   _out_ind,a0
[00002118] d1f9 0000 6064            adda.l    _out_buf,a0
[0000211e] 2279 0000 6180            movea.l   _in_ind,a1
[00002124] d3f9 0000 602e            adda.l    _in_buff,a1
[0000212a] 1091                      move.b    (a1),(a0)
[0000212c] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00002136] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002140] 2079 0000 6060            movea.l   _out_ind,a0
[00002146] d1f9 0000 6064            adda.l    _out_buf,a0
[0000214c] 10bc 002e                 move.b    #$2E,(a0)
[00002150] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000215a] 2079 0000 6060            movea.l   _out_ind,a0
[00002160] d1f9 0000 6064            adda.l    _out_buf,a0
[00002166] 10bc 006c                 move.b    #$6C,(a0)
[0000216a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002174] 2079 0000 6060            movea.l   _out_ind,a0
[0000217a] d1f9 0000 6064            adda.l    _out_buf,a0
[00002180] 2279 0000 6180            movea.l   _in_ind,a1
[00002186] d3f9 0000 602e            adda.l    _in_buff,a1
[0000218c] 1091                      move.b    (a1),(a0)
[0000218e] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00002198] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000021a2] 2079 0000 6060            movea.l   _out_ind,a0
[000021a8] d1f9 0000 6064            adda.l    _out_buf,a0
[000021ae] 2279 0000 6180            movea.l   _in_ind,a1
[000021b4] d3f9 0000 602e            adda.l    _in_buff,a1
[000021ba] 1091                      move.b    (a1),(a0)
[000021bc] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000021c6] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000021d0] 2079 0000 6060            movea.l   _out_ind,a0
[000021d6] d1f9 0000 6064            adda.l    _out_buf,a0
[000021dc] 10bc 0024                 move.b    #$24,(a0)
[000021e0] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000021ea] 426e fffe                 clr.w     -2(a6)
[000021ee] 6000 002a                 bra       $0000221A
[000021f2] 2079 0000 6060            movea.l   _out_ind,a0
[000021f8] d1f9 0000 6064            adda.l    _out_buf,a0
[000021fe] 326e fffe                 movea.w   -2(a6),a1
[00002202] d3fc 0000 850e            adda.l    #_hexstr,a1
[00002208] 1091                      move.b    (a1),(a0)
[0000220a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002214] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[0000221a] 0c6e 0008 fffe            cmpi.w    #$0008,-2(a6)
[00002220] 6dd0                      blt.s     $000021F2
[00002222] 6000 000c                 bra       $00002230
[00002226] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00002230] 2079 0000 602e            movea.l   _in_buff,a0
[00002236] d1f9 0000 6180            adda.l    _in_ind,a0
[0000223c] 0c10 002c                 cmpi.b    #$2C,(a0)
[00002240] 66e4                      bne.s     $00002226
[00002242] 0679 0001 0000 5ffa       addi.w    #$0001,_imov1_t
[0000224a] 2079 0000 6060            movea.l   _out_ind,a0
[00002250] d1f9 0000 6064            adda.l    _out_buf,a0
[00002256] 2279 0000 6180            movea.l   _in_ind,a1
[0000225c] d3f9 0000 602e            adda.l    _in_buff,a1
[00002262] 1091                      move.b    (a1),(a0)
[00002264] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000226e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002278] 2039 0000 6180            move.l    _in_ind,d0
[0000227e] b0b9 0000 5ff6            cmp.l     _size,d0
[00002284] 6d00 fb9c                 blt       $00001E22
[00002288] 4e5e                      unlk      a6
[0000228a] 4e75                      rts       
[0000228c] _puthex:
[0000228c] 4e56 fffc                 link      a6,#$FFFC
[00002290] 0c6e 000a 0008            cmpi.w    #$000A,8(a6)
[00002296] 6c00 0014                 bge       $000022AC
[0000229a] 302e 0008                 move.w    8(a6),d0
[0000229e] d07c 0030                 add.w     #$0030,d0
[000022a2] 226e 000a                 movea.l   10(a6),a1
[000022a6] 1280                      move.b    d0,(a1)
[000022a8] 6000 0016                 bra       $000022C0
[000022ac] 046e 000a 0008            subi.w    #$000A,8(a6)
[000022b2] 302e 0008                 move.w    8(a6),d0
[000022b6] d07c 0061                 add.w     #$0061,d0
[000022ba] 226e 000a                 movea.l   10(a6),a1
[000022be] 1280                      move.b    d0,(a1)
[000022c0] 4e5e                      unlk      a6
[000022c2] 4e75                      rts       
[000022c4] _nextlin:
[000022c4] 4e56 fffc                 link      a6,#$FFFC
[000022c8] 6000 000c                 bra       $000022D6
[000022cc] 06b9 0000 0001 0000 606e  addi.l    #$00000001,_nxt_lin
[000022d6] 2079 0000 602e            movea.l   _in_buff,a0
[000022dc] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000022e2] 0c10 000a                 cmpi.b    #$0A,(a0)
[000022e6] 6700 0010                 beq       $000022F8
[000022ea] 2039 0000 606e            move.l    _nxt_lin,d0
[000022f0] b0b9 0000 5ff6            cmp.l     _size,d0
[000022f6] 6dd4                      blt.s     $000022CC
[000022f8] 2079 0000 602e            movea.l   _in_buff,a0
[000022fe] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002304] 0c10 000a                 cmpi.b    #$0A,(a0)
[00002308] 6600 000c                 bne       $00002316
[0000230c] 06b9 0000 0001 0000 606e  addi.l    #$00000001,_nxt_lin
[00002316] 2079 0000 602e            movea.l   _in_buff,a0
[0000231c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002322] 0c10 002a                 cmpi.b    #$2A,(a0)
[00002326] 6600 0008                 bne       $00002330
[0000232a] 4eb9 0000 22c4            jsr       _nextlin
[00002330] 2079 0000 602e            movea.l   _in_buff,a0
[00002336] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000233c] 0c10 003b                 cmpi.b    #$3B,(a0)
[00002340] 6600 0008                 bne       $0000234A
[00002344] 4eb9 0000 22c4            jsr       _nextlin
[0000234a] 4e5e                      unlk      a6
[0000234c] 4e75                      rts       
[0000234e] _clrop:
[0000234e] 4e56 fffa                 link      a6,#$FFFA
[00002352] 42b9 0000 6180            clr.l     _in_ind
[00002358] 42b9 0000 6060            clr.l     _out_ind
[0000235e] 6000 020a                 bra       $0000256A
[00002362] 6000 0030                 bra       $00002394
[00002366] 2079 0000 6060            movea.l   _out_ind,a0
[0000236c] d1f9 0000 6064            adda.l    _out_buf,a0
[00002372] 2279 0000 6180            movea.l   _in_ind,a1
[00002378] d3f9 0000 602e            adda.l    _in_buff,a1
[0000237e] 1091                      move.b    (a1),(a0)
[00002380] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[0000238a] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002394] 2079 0000 602e            movea.l   _in_buff,a0
[0000239a] d1f9 0000 6180            adda.l    _in_ind,a0
[000023a0] 0c10 0063                 cmpi.b    #$63,(a0)
[000023a4] 6700 0010                 beq       $000023B6
[000023a8] 2039 0000 6180            move.l    _in_ind,d0
[000023ae] b0b9 0000 5ff6            cmp.l     _size,d0
[000023b4] 6db0                      blt.s     $00002366
[000023b6] 2079 0000 602e            movea.l   _in_buff,a0
[000023bc] d1f9 0000 6180            adda.l    _in_ind,a0
[000023c2] 0c10 0063                 cmpi.b    #$63,(a0)
[000023c6] 6600 0174                 bne       $0000253C
[000023ca] 2079 0000 602e            movea.l   _in_buff,a0
[000023d0] d1f9 0000 6180            adda.l    _in_ind,a0
[000023d6] 0c28 006c 0001            cmpi.b    #$6C,1(a0)
[000023dc] 6600 015e                 bne       $0000253C
[000023e0] 2079 0000 602e            movea.l   _in_buff,a0
[000023e6] d1f9 0000 6180            adda.l    _in_ind,a0
[000023ec] 0c28 0072 0002            cmpi.b    #$72,2(a0)
[000023f2] 6600 0148                 bne       $0000253C
[000023f6] 2079 0000 602e            movea.l   _in_buff,a0
[000023fc] d1f9 0000 6180            adda.l    _in_ind,a0
[00002402] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[00002408] 6600 0132                 bne       $0000253C
[0000240c] 2079 0000 602e            movea.l   _in_buff,a0
[00002412] d1f9 0000 6180            adda.l    _in_ind,a0
[00002418] 0c28 0052 0004            cmpi.b    #$52,4(a0)
[0000241e] 6600 011c                 bne       $0000253C
[00002422] 2079 0000 602e            movea.l   _in_buff,a0
[00002428] d1f9 0000 6180            adda.l    _in_ind,a0
[0000242e] 0c28 000a 0007            cmpi.b    #$0A,7(a0)
[00002434] 6600 0106                 bne       $0000253C
[00002438] 2079 0000 602e            movea.l   _in_buff,a0
[0000243e] d1f9 0000 6180            adda.l    _in_ind,a0
[00002444] 13e8 0005 0000 605c       move.b    5(a0),_savereg
[0000244c] 426e fffe                 clr.w     -2(a6)
[00002450] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[0000245a] 4eb9 0000 22c4            jsr       _nextlin
[00002460] 2079 0000 602e            movea.l   _in_buff,a0
[00002466] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000246c] 0c10 006d                 cmpi.b    #$6D,(a0)
[00002470] 6600 00ca                 bne       $0000253C
[00002474] 2079 0000 602e            movea.l   _in_buff,a0
[0000247a] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002480] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00002486] 6600 00b4                 bne       $0000253C
[0000248a] 2079 0000 602e            movea.l   _in_buff,a0
[00002490] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002496] 0c28 0076 0002            cmpi.b    #$76,2(a0)
[0000249c] 6600 009e                 bne       $0000253C
[000024a0] 2079 0000 602e            movea.l   _in_buff,a0
[000024a6] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000024ac] 0c28 0065 0003            cmpi.b    #$65,3(a0)
[000024b2] 6600 0088                 bne       $0000253C
[000024b6] 2079 0000 602e            movea.l   _in_buff,a0
[000024bc] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000024c2] 0c28 0020 0004            cmpi.b    #$20,4(a0)
[000024c8] 6600 0072                 bne       $0000253C
[000024cc] 6000 0002                 bra       $000024D0
[000024d0] 306e fffe                 movea.w   -2(a6),a0
[000024d4] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000024da] d1f9 0000 602e            adda.l    _in_buff,a0
[000024e0] 0c10 002c                 cmpi.b    #$2C,(a0)
[000024e4] 40c0                      move.w    sr,d0
[000024e6] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[000024ec] 44c0                      move.b    d0,ccr
[000024ee] 66e0                      bne.s     $000024D0
[000024f0] 306e fffe                 movea.w   -2(a6),a0
[000024f4] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000024fa] d1f9 0000 602e            adda.l    _in_buff,a0
[00002500] 0c10 0052                 cmpi.b    #$52,(a0)
[00002504] 6600 0036                 bne       $0000253C
[00002508] 2079 0000 602e            movea.l   _in_buff,a0
[0000250e] 322e fffe                 move.w    -2(a6),d1
[00002512] 48c1                      ext.l     d1
[00002514] d2b9 0000 606e            add.l     _nxt_lin,d1
[0000251a] 1030 1801                 move.b    1(a0,d1.l),d0
[0000251e] 4880                      ext.w     d0
[00002520] b039 0000 605c            cmp.b     _savereg,d0
[00002526] 6600 0014                 bne       $0000253C
[0000252a] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[00002534] 0679 0001 0000 6068       addi.w    #$0001,_clr_tot
[0000253c] 2079 0000 6060            movea.l   _out_ind,a0
[00002542] d1f9 0000 6064            adda.l    _out_buf,a0
[00002548] 2279 0000 6180            movea.l   _in_ind,a1
[0000254e] d3f9 0000 602e            adda.l    _in_buff,a1
[00002554] 1091                      move.b    (a1),(a0)
[00002556] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00002560] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000256a] 2039 0000 6180            move.l    _in_ind,d0
[00002570] b0b9 0000 5ff6            cmp.l     _size,d0
[00002576] 6d00 fdea                 blt       $00002362
[0000257a] 4e5e                      unlk      a6
[0000257c] 4e75                      rts       
[0000257e] _samebra:
[0000257e] 4e56 fff8                 link      a6,#$FFF8
[00002582] 42b9 0000 6180            clr.l     _in_ind
[00002588] 42b9 0000 6060            clr.l     _out_ind
[0000258e] 6000 01ca                 bra       $0000275A
[00002592] 6000 0030                 bra       $000025C4
[00002596] 2079 0000 6060            movea.l   _out_ind,a0
[0000259c] d1f9 0000 6064            adda.l    _out_buf,a0
[000025a2] 2279 0000 6180            movea.l   _in_ind,a1
[000025a8] d3f9 0000 602e            adda.l    _in_buff,a1
[000025ae] 1091                      move.b    (a1),(a0)
[000025b0] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[000025ba] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000025c4] 2079 0000 602e            movea.l   _in_buff,a0
[000025ca] d1f9 0000 6180            adda.l    _in_ind,a0
[000025d0] 0c10 0062                 cmpi.b    #$62,(a0)
[000025d4] 6700 0010                 beq       $000025E6
[000025d8] 2039 0000 6180            move.l    _in_ind,d0
[000025de] b0b9 0000 5ff6            cmp.l     _size,d0
[000025e4] 6db0                      blt.s     $00002596
[000025e6] 2079 0000 602e            movea.l   _in_buff,a0
[000025ec] d1f9 0000 6180            adda.l    _in_ind,a0
[000025f2] 0c10 0062                 cmpi.b    #$62,(a0)
[000025f6] 6600 0134                 bne       $0000272C
[000025fa] 2079 0000 602e            movea.l   _in_buff,a0
[00002600] d1f9 0000 6180            adda.l    _in_ind,a0
[00002606] 0c28 0072 0001            cmpi.b    #$72,1(a0)
[0000260c] 6600 011e                 bne       $0000272C
[00002610] 2079 0000 602e            movea.l   _in_buff,a0
[00002616] d1f9 0000 6180            adda.l    _in_ind,a0
[0000261c] 0c28 0061 0002            cmpi.b    #$61,2(a0)
[00002622] 6600 0108                 bne       $0000272C
[00002626] 2079 0000 602e            movea.l   _in_buff,a0
[0000262c] d1f9 0000 6180            adda.l    _in_ind,a0
[00002632] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[00002638] 6600 00f2                 bne       $0000272C
[0000263c] 23f9 0000 6180 0000 606e  move.l    _in_ind,_nxt_lin
[00002646] 4eb9 0000 22c4            jsr       _nextlin
[0000264c] 2079 0000 602e            movea.l   _in_buff,a0
[00002652] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002658] 0c10 0062                 cmpi.b    #$62,(a0)
[0000265c] 6600 00ce                 bne       $0000272C
[00002660] 2079 0000 602e            movea.l   _in_buff,a0
[00002666] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000266c] 0c28 0072 0001            cmpi.b    #$72,1(a0)
[00002672] 6600 00b8                 bne       $0000272C
[00002676] 2079 0000 602e            movea.l   _in_buff,a0
[0000267c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002682] 0c28 0061 0002            cmpi.b    #$61,2(a0)
[00002688] 6600 00a2                 bne       $0000272C
[0000268c] 2079 0000 602e            movea.l   _in_buff,a0
[00002692] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00002698] 0c28 0020 0003            cmpi.b    #$20,3(a0)
[0000269e] 6600 008c                 bne       $0000272C
[000026a2] 2039 0000 6180            move.l    _in_ind,d0
[000026a8] d0bc 0000 0005            add.l     #$00000005,d0
[000026ae] 3d40 fffe                 move.w    d0,-2(a6)
[000026b2] 2039 0000 606e            move.l    _nxt_lin,d0
[000026b8] d0bc 0000 0005            add.l     #$00000005,d0
[000026be] 3d40 fffc                 move.w    d0,-4(a6)
[000026c2] 6000 0034                 bra       $000026F8
[000026c6] 2079 0000 602e            movea.l   _in_buff,a0
[000026cc] 322e fffe                 move.w    -2(a6),d1
[000026d0] 48c1                      ext.l     d1
[000026d2] 1030 1800                 move.b    0(a0,d1.l),d0
[000026d6] 4880                      ext.w     d0
[000026d8] 326e fffc                 movea.w   -4(a6),a1
[000026dc] d3f9 0000 602e            adda.l    _in_buff,a1
[000026e2] 1211                      move.b    (a1),d1
[000026e4] 4881                      ext.w     d1
[000026e6] b041                      cmp.w     d1,d0
[000026e8] 6600 001e                 bne       $00002708
[000026ec] 066e 0001 fffe            addi.w    #$0001,-2(a6)
[000026f2] 066e 0001 fffc            addi.w    #$0001,-4(a6)
[000026f8] 306e fffe                 movea.w   -2(a6),a0
[000026fc] d1f9 0000 602e            adda.l    _in_buff,a0
[00002702] 0c10 000a                 cmpi.b    #$0A,(a0)
[00002706] 66be                      bne.s     $000026C6
[00002708] 306e fffe                 movea.w   -2(a6),a0
[0000270c] d1f9 0000 602e            adda.l    _in_buff,a0
[00002712] 0c10 000a                 cmpi.b    #$0A,(a0)
[00002716] 6600 0014                 bne       $0000272C
[0000271a] 0679 0001 0000 605a       addi.w    #$0001,_bra_tot
[00002722] 23f9 0000 606e 0000 6180  move.l    _nxt_lin,_in_ind
[0000272c] 2079 0000 6060            movea.l   _out_ind,a0
[00002732] d1f9 0000 6064            adda.l    _out_buf,a0
[00002738] 2279 0000 6180            movea.l   _in_ind,a1
[0000273e] d3f9 0000 602e            adda.l    _in_buff,a1
[00002744] 1091                      move.b    (a1),(a0)
[00002746] 06b9 0000 0001 0000 6180  addi.l    #$00000001,_in_ind
[00002750] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000275a] 2039 0000 6180            move.l    _in_ind,d0
[00002760] b0b9 0000 5ff6            cmp.l     _size,d0
[00002766] 6d00 fe2a                 blt       $00002592
[0000276a] 4e5e                      unlk      a6
[0000276c] 4e75                      rts       
[0000276e] _mv2pea1:
[0000276e] 4e56 ffec                 link      a6,#$FFEC
[00002772] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00002776] 4280                      clr.l     d0
[00002778] 23c0 0000 6060            move.l    d0,_out_ind
[0000277e] 2e00                      move.l    d0,d7
[00002780] 2a79 0000 602e            movea.l   _in_buff,a5
[00002786] 6000 0490                 bra       $00002C18
[0000278a] 2047                      movea.l   d7,a0
[0000278c] d1cd                      adda.l    a5,a0
[0000278e] 0c10 006d                 cmpi.b    #$6D,(a0)
[00002792] 6600 0462                 bne       $00002BF6
[00002796] 2047                      movea.l   d7,a0
[00002798] d1cd                      adda.l    a5,a0
[0000279a] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[000027a0] 6600 0454                 bne       $00002BF6
[000027a4] 2047                      movea.l   d7,a0
[000027a6] d1cd                      adda.l    a5,a0
[000027a8] 0c28 002e 0004            cmpi.b    #$2E,4(a0)
[000027ae] 6600 0446                 bne       $00002BF6
[000027b2] 2047                      movea.l   d7,a0
[000027b4] d1cd                      adda.l    a5,a0
[000027b6] 0c28 006c 0005            cmpi.b    #$6C,5(a0)
[000027bc] 6600 0438                 bne       $00002BF6
[000027c0] 2047                      movea.l   d7,a0
[000027c2] d1cd                      adda.l    a5,a0
[000027c4] 0c28 0052 0007            cmpi.b    #$52,7(a0)
[000027ca] 6600 042a                 bne       $00002BF6
[000027ce] 2007                      move.l    d7,d0
[000027d0] d0bc 0000 0008            add.l     #$00000008,d0
[000027d6] 2d40 fff4                 move.l    d0,-12(a6)
[000027da] 206e fff4                 movea.l   -12(a6),a0
[000027de] d1cd                      adda.l    a5,a0
[000027e0] 0c10 0031                 cmpi.b    #$31,(a0)
[000027e4] 6600 0048                 bne       $0000282E
[000027e8] 206e fff4                 movea.l   -12(a6),a0
[000027ec] d1cd                      adda.l    a5,a0
[000027ee] 0c28 0030 0001            cmpi.b    #$30,1(a0)
[000027f4] 6d00 0030                 blt       $00002826
[000027f8] 206e fff4                 movea.l   -12(a6),a0
[000027fc] d1cd                      adda.l    a5,a0
[000027fe] 0c28 0035 0001            cmpi.b    #$35,1(a0)
[00002804] 6e00 0020                 bgt       $00002826
[00002808] 202e fff4                 move.l    -12(a6),d0
[0000280c] 1035 0801                 move.b    1(a5,d0.l),d0
[00002810] 4880                      ext.w     d0
[00002812] d07c ffda                 add.w     #$FFDA,d0
[00002816] 3d40 fffe                 move.w    d0,-2(a6)
[0000281a] 06ae 0000 0002 fff4       addi.l    #$00000002,-12(a6)
[00002822] 6000 0006                 bra       $0000282A
[00002826] 6000 03ce                 bra       $00002BF6
[0000282a] 6000 0032                 bra       $0000285E
[0000282e] 206e fff4                 movea.l   -12(a6),a0
[00002832] d1cd                      adda.l    a5,a0
[00002834] 0c10 0037                 cmpi.b    #$37,(a0)
[00002838] 6f00 0020                 ble       $0000285A
[0000283c] 202e fff4                 move.l    -12(a6),d0
[00002840] 1035 0800                 move.b    0(a5,d0.l),d0
[00002844] 4880                      ext.w     d0
[00002846] d07c ffd0                 add.w     #$FFD0,d0
[0000284a] 3d40 fffe                 move.w    d0,-2(a6)
[0000284e] 06ae 0000 0001 fff4       addi.l    #$00000001,-12(a6)
[00002856] 6000 0006                 bra       $0000285E
[0000285a] 6000 039a                 bra       $00002BF6
[0000285e] 206e fff4                 movea.l   -12(a6),a0
[00002862] d1cd                      adda.l    a5,a0
[00002864] 0c28 002d 0001            cmpi.b    #$2D,1(a0)
[0000286a] 6600 038a                 bne       $00002BF6
[0000286e] 206e fff4                 movea.l   -12(a6),a0
[00002872] d1cd                      adda.l    a5,a0
[00002874] 0c28 0073 0003            cmpi.b    #$73,3(a0)
[0000287a] 6600 037a                 bne       $00002BF6
[0000287e] 23c7 0000 606e            move.l    d7,_nxt_lin
[00002884] 4eb9 0000 22c4            jsr       _nextlin
[0000288a] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002890] d1cd                      adda.l    a5,a0
[00002892] 0c10 0061                 cmpi.b    #$61,(a0)
[00002896] 6600 0026                 bne       $000028BE
[0000289a] 2079 0000 606e            movea.l   _nxt_lin,a0
[000028a0] d1cd                      adda.l    a5,a0
[000028a2] 0c28 0064 0001            cmpi.b    #$64,1(a0)
[000028a8] 6600 0014                 bne       $000028BE
[000028ac] 2079 0000 606e            movea.l   _nxt_lin,a0
[000028b2] d1cd                      adda.l    a5,a0
[000028b4] 0c28 0064 0002            cmpi.b    #$64,2(a0)
[000028ba] 6700 0036                 beq       $000028F2
[000028be] 2079 0000 606e            movea.l   _nxt_lin,a0
[000028c4] d1cd                      adda.l    a5,a0
[000028c6] 0c10 0073                 cmpi.b    #$73,(a0)
[000028ca] 6600 032a                 bne       $00002BF6
[000028ce] 2079 0000 606e            movea.l   _nxt_lin,a0
[000028d4] d1cd                      adda.l    a5,a0
[000028d6] 0c28 0075 0001            cmpi.b    #$75,1(a0)
[000028dc] 6600 0318                 bne       $00002BF6
[000028e0] 2079 0000 606e            movea.l   _nxt_lin,a0
[000028e6] d1cd                      adda.l    a5,a0
[000028e8] 0c28 0062 0002            cmpi.b    #$62,2(a0)
[000028ee] 6600 0306                 bne       $00002BF6
[000028f2] 2079 0000 606e            movea.l   _nxt_lin,a0
[000028f8] d1cd                      adda.l    a5,a0
[000028fa] 0c28 002e 0003            cmpi.b    #$2E,3(a0)
[00002900] 6600 02f4                 bne       $00002BF6
[00002904] 2079 0000 606e            movea.l   _nxt_lin,a0
[0000290a] d1cd                      adda.l    a5,a0
[0000290c] 0c28 006c 0004            cmpi.b    #$6C,4(a0)
[00002912] 6600 02e2                 bne       $00002BF6
[00002916] 2079 0000 606e            movea.l   _nxt_lin,a0
[0000291c] d1cd                      adda.l    a5,a0
[0000291e] 0c28 0020 0005            cmpi.b    #$20,5(a0)
[00002924] 6600 02d0                 bne       $00002BF6
[00002928] 2079 0000 606e            movea.l   _nxt_lin,a0
[0000292e] d1cd                      adda.l    a5,a0
[00002930] 0c28 0023 0006            cmpi.b    #$23,6(a0)
[00002936] 6600 02be                 bne       $00002BF6
[0000293a] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002940] d1cd                      adda.l    a5,a0
[00002942] 0c28 002d 0007            cmpi.b    #$2D,7(a0)
[00002948] 6700 0026                 beq       $00002970
[0000294c] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002952] d1cd                      adda.l    a5,a0
[00002954] 0c28 0030 0007            cmpi.b    #$30,7(a0)
[0000295a] 6d00 029a                 blt       $00002BF6
[0000295e] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002964] d1cd                      adda.l    a5,a0
[00002966] 0c28 0039 0007            cmpi.b    #$39,7(a0)
[0000296c] 6e00 0288                 bgt       $00002BF6
[00002970] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002976] d1cd                      adda.l    a5,a0
[00002978] 0c10 0061                 cmpi.b    #$61,(a0)
[0000297c] 6700 0008                 beq       $00002986
[00002980] 4240                      clr.w     d0
[00002982] 6000 0006                 bra       $0000298A
[00002986] 303c 0001                 move.w    #$0001,d0
[0000298a] 3d40 fffc                 move.w    d0,-4(a6)
[0000298e] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002994] d1cd                      adda.l    a5,a0
[00002996] 0c28 002d 0007            cmpi.b    #$2D,7(a0)
[0000299c] 6600 002c                 bne       $000029CA
[000029a0] 2039 0000 606e            move.l    _nxt_lin,d0
[000029a6] d0bc 0000 0008            add.l     #$00000008,d0
[000029ac] 2d40 fff0                 move.l    d0,-16(a6)
[000029b0] 4a6e fffc                 tst.w     -4(a6)
[000029b4] 6700 0008                 beq       $000029BE
[000029b8] 4240                      clr.w     d0
[000029ba] 6000 0006                 bra       $000029C2
[000029be] 303c 0001                 move.w    #$0001,d0
[000029c2] 3d40 fffc                 move.w    d0,-4(a6)
[000029c6] 6000 0012                 bra       $000029DA
[000029ca] 2039 0000 606e            move.l    _nxt_lin,d0
[000029d0] d0bc 0000 0007            add.l     #$00000007,d0
[000029d6] 2d40 fff0                 move.l    d0,-16(a6)
[000029da] 2d6e fff0 ffec            move.l    -16(a6),-20(a6)
[000029e0] 6000 000a                 bra       $000029EC
[000029e4] 06ae 0000 0001 fff0       addi.l    #$00000001,-16(a6)
[000029ec] 206e fff0                 movea.l   -16(a6),a0
[000029f0] d1cd                      adda.l    a5,a0
[000029f2] 0c10 0030                 cmpi.b    #$30,(a0)
[000029f6] 6d00 000e                 blt       $00002A06
[000029fa] 206e fff0                 movea.l   -16(a6),a0
[000029fe] d1cd                      adda.l    a5,a0
[00002a00] 0c10 0039                 cmpi.b    #$39,(a0)
[00002a04] 6fde                      ble.s     $000029E4
[00002a06] 206e fff0                 movea.l   -16(a6),a0
[00002a0a] d1cd                      adda.l    a5,a0
[00002a0c] 0c28 0028 0001            cmpi.b    #$28,1(a0)
[00002a12] 6600 01e2                 bne       $00002BF6
[00002a16] 206e fff0                 movea.l   -16(a6),a0
[00002a1a] d1cd                      adda.l    a5,a0
[00002a1c] 0c28 0073 0002            cmpi.b    #$73,2(a0)
[00002a22] 6600 01d2                 bne       $00002BF6
[00002a26] 42ae fff8                 clr.l     -8(a6)
[00002a2a] 2d6e ffec fff4            move.l    -20(a6),-12(a6)
[00002a30] 6000 0034                 bra       $00002A66
[00002a34] 2f3c 0000 000a            move.l    #$0000000A,-(a7)
[00002a3a] 2f2e fff8                 move.l    -8(a6),-(a7)
[00002a3e] 4eb9 0000 3a10            jsr       lmul
[00002a44] 508f                      addq.l    #8,a7
[00002a46] 2d40 fff8                 move.l    d0,-8(a6)
[00002a4a] 202e fff4                 move.l    -12(a6),d0
[00002a4e] 1035 0800                 move.b    0(a5,d0.l),d0
[00002a52] 4880                      ext.w     d0
[00002a54] d07c ffd0                 add.w     #$FFD0,d0
[00002a58] 48c0                      ext.l     d0
[00002a5a] d1ae fff8                 add.l     d0,-8(a6)
[00002a5e] 06ae 0000 0001 fff4       addi.l    #$00000001,-12(a6)
[00002a66] 202e fff4                 move.l    -12(a6),d0
[00002a6a] b0ae fff0                 cmp.l     -16(a6),d0
[00002a6e] 6dc4                      blt.s     $00002A34
[00002a70] 0cae 0000 7fff fff8       cmpi.l    #$00007FFF,-8(a6)
[00002a78] 6e00 017c                 bgt       $00002BF6
[00002a7c] 2079 0000 6060            movea.l   _out_ind,a0
[00002a82] d1f9 0000 6064            adda.l    _out_buf,a0
[00002a88] 10bc 0070                 move.b    #$70,(a0)
[00002a8c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002a96] 2079 0000 6060            movea.l   _out_ind,a0
[00002a9c] d1f9 0000 6064            adda.l    _out_buf,a0
[00002aa2] 10bc 0065                 move.b    #$65,(a0)
[00002aa6] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002ab0] 2079 0000 6060            movea.l   _out_ind,a0
[00002ab6] d1f9 0000 6064            adda.l    _out_buf,a0
[00002abc] 10bc 0061                 move.b    #$61,(a0)
[00002ac0] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002aca] 2079 0000 6060            movea.l   _out_ind,a0
[00002ad0] d1f9 0000 6064            adda.l    _out_buf,a0
[00002ad6] 10bc 0020                 move.b    #$20,(a0)
[00002ada] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002ae4] 4a6e fffc                 tst.w     -4(a6)
[00002ae8] 6600 001c                 bne       $00002B06
[00002aec] 2079 0000 6060            movea.l   _out_ind,a0
[00002af2] d1f9 0000 6064            adda.l    _out_buf,a0
[00002af8] 10bc 002d                 move.b    #$2D,(a0)
[00002afc] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002b06] 2039 0000 6064            move.l    _out_buf,d0
[00002b0c] d0b9 0000 6060            add.l     _out_ind,d0
[00002b12] 2e80                      move.l    d0,(a7)
[00002b14] 202e fff8                 move.l    -8(a6),d0
[00002b18] 3f00                      move.w    d0,-(a7)
[00002b1a] 4eb9 0000 0a14            jsr       _itoa
[00002b20] 548f                      addq.l    #2,a7
[00002b22] 6000 000c                 bra       $00002B30
[00002b26] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002b30] 2079 0000 6060            movea.l   _out_ind,a0
[00002b36] 2239 0000 6064            move.l    _out_buf,d1
[00002b3c] 4a30 1800                 tst.b     0(a0,d1.l)
[00002b40] 66e4                      bne.s     $00002B26
[00002b42] 2079 0000 6060            movea.l   _out_ind,a0
[00002b48] d1f9 0000 6064            adda.l    _out_buf,a0
[00002b4e] 10bc 0028                 move.b    #$28,(a0)
[00002b52] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002b5c] 2079 0000 6060            movea.l   _out_ind,a0
[00002b62] d1f9 0000 6064            adda.l    _out_buf,a0
[00002b68] 10bc 0052                 move.b    #$52,(a0)
[00002b6c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002b76] 0c6e 0009 fffe            cmpi.w    #$0009,-2(a6)
[00002b7c] 6f00 001c                 ble       $00002B9A
[00002b80] 2079 0000 6060            movea.l   _out_ind,a0
[00002b86] d1f9 0000 6064            adda.l    _out_buf,a0
[00002b8c] 10bc 0031                 move.b    #$31,(a0)
[00002b90] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002b9a] 302e fffe                 move.w    -2(a6),d0
[00002b9e] 48c0                      ext.l     d0
[00002ba0] 81fc 000a                 divs.w    #$000A,d0
[00002ba4] 4840                      swap      d0
[00002ba6] d07c 0030                 add.w     #$0030,d0
[00002baa] 2279 0000 6060            movea.l   _out_ind,a1
[00002bb0] d3f9 0000 6064            adda.l    _out_buf,a1
[00002bb6] 1280                      move.b    d0,(a1)
[00002bb8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002bc2] 2079 0000 6060            movea.l   _out_ind,a0
[00002bc8] d1f9 0000 6064            adda.l    _out_buf,a0
[00002bce] 10bc 0029                 move.b    #$29,(a0)
[00002bd2] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002bdc] 4eb9 0000 22c4            jsr       _nextlin
[00002be2] 2e39 0000 606e            move.l    _nxt_lin,d7
[00002be8] 9ebc 0000 0002            sub.l     #$00000002,d7
[00002bee] 0679 0001 0000 5ffe       addi.w    #$0001,_mv1_tot
[00002bf6] 2079 0000 6060            movea.l   _out_ind,a0
[00002bfc] d1f9 0000 6064            adda.l    _out_buf,a0
[00002c02] 2247                      movea.l   d7,a1
[00002c04] d3cd                      adda.l    a5,a1
[00002c06] 1091                      move.b    (a1),(a0)
[00002c08] debc 0000 0001            add.l     #$00000001,d7
[00002c0e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002c18] beb9 0000 5ff6            cmp.l     _size,d7
[00002c1e] 6d00 fb6a                 blt       $0000278A
[00002c22] 4a9f                      tst.l     (a7)+
[00002c24] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00002c28] 4e5e                      unlk      a6
[00002c2a] 4e75                      rts       
[00002c2c] _mv2pea2:
[00002c2c] 4e56 ffec                 link      a6,#$FFEC
[00002c30] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00002c34] 4280                      clr.l     d0
[00002c36] 23c0 0000 6060            move.l    d0,_out_ind
[00002c3c] 2e00                      move.l    d0,d7
[00002c3e] 2a79 0000 602e            movea.l   _in_buff,a5
[00002c44] 6000 049e                 bra       $000030E4
[00002c48] 2047                      movea.l   d7,a0
[00002c4a] d1cd                      adda.l    a5,a0
[00002c4c] 0c10 006d                 cmpi.b    #$6D,(a0)
[00002c50] 6600 0470                 bne       $000030C2
[00002c54] 2047                      movea.l   d7,a0
[00002c56] d1cd                      adda.l    a5,a0
[00002c58] 0c28 006f 0001            cmpi.b    #$6F,1(a0)
[00002c5e] 6600 0462                 bne       $000030C2
[00002c62] 2047                      movea.l   d7,a0
[00002c64] d1cd                      adda.l    a5,a0
[00002c66] 0c28 002e 0004            cmpi.b    #$2E,4(a0)
[00002c6c] 6600 0454                 bne       $000030C2
[00002c70] 2047                      movea.l   d7,a0
[00002c72] d1cd                      adda.l    a5,a0
[00002c74] 0c28 006c 0005            cmpi.b    #$6C,5(a0)
[00002c7a] 6600 0446                 bne       $000030C2
[00002c7e] 2047                      movea.l   d7,a0
[00002c80] d1cd                      adda.l    a5,a0
[00002c82] 0c28 0052 0007            cmpi.b    #$52,7(a0)
[00002c88] 6600 0438                 bne       $000030C2
[00002c8c] 2007                      move.l    d7,d0
[00002c8e] d0bc 0000 0008            add.l     #$00000008,d0
[00002c94] 2d40 fff4                 move.l    d0,-12(a6)
[00002c98] 206e fff4                 movea.l   -12(a6),a0
[00002c9c] d1cd                      adda.l    a5,a0
[00002c9e] 0c10 0031                 cmpi.b    #$31,(a0)
[00002ca2] 6600 0048                 bne       $00002CEC
[00002ca6] 206e fff4                 movea.l   -12(a6),a0
[00002caa] d1cd                      adda.l    a5,a0
[00002cac] 0c28 0030 0001            cmpi.b    #$30,1(a0)
[00002cb2] 6d00 0030                 blt       $00002CE4
[00002cb6] 206e fff4                 movea.l   -12(a6),a0
[00002cba] d1cd                      adda.l    a5,a0
[00002cbc] 0c28 0035 0001            cmpi.b    #$35,1(a0)
[00002cc2] 6e00 0020                 bgt       $00002CE4
[00002cc6] 202e fff4                 move.l    -12(a6),d0
[00002cca] 1035 0801                 move.b    1(a5,d0.l),d0
[00002cce] 4880                      ext.w     d0
[00002cd0] d07c ffda                 add.w     #$FFDA,d0
[00002cd4] 3d40 fffe                 move.w    d0,-2(a6)
[00002cd8] 06ae 0000 0002 fff4       addi.l    #$00000002,-12(a6)
[00002ce0] 6000 0006                 bra       $00002CE8
[00002ce4] 6000 03dc                 bra       $000030C2
[00002ce8] 6000 0032                 bra       $00002D1C
[00002cec] 206e fff4                 movea.l   -12(a6),a0
[00002cf0] d1cd                      adda.l    a5,a0
[00002cf2] 0c10 0037                 cmpi.b    #$37,(a0)
[00002cf6] 6f00 0020                 ble       $00002D18
[00002cfa] 202e fff4                 move.l    -12(a6),d0
[00002cfe] 1035 0800                 move.b    0(a5,d0.l),d0
[00002d02] 4880                      ext.w     d0
[00002d04] d07c ffd0                 add.w     #$FFD0,d0
[00002d08] 3d40 fffe                 move.w    d0,-2(a6)
[00002d0c] 06ae 0000 0001 fff4       addi.l    #$00000001,-12(a6)
[00002d14] 6000 0006                 bra       $00002D1C
[00002d18] 6000 03a8                 bra       $000030C2
[00002d1c] 206e fff4                 movea.l   -12(a6),a0
[00002d20] d1cd                      adda.l    a5,a0
[00002d22] 0c28 0028 0001            cmpi.b    #$28,1(a0)
[00002d28] 6600 0398                 bne       $000030C2
[00002d2c] 206e fff4                 movea.l   -12(a6),a0
[00002d30] d1cd                      adda.l    a5,a0
[00002d32] 0c28 0073 0002            cmpi.b    #$73,2(a0)
[00002d38] 6600 0388                 bne       $000030C2
[00002d3c] 23c7 0000 606e            move.l    d7,_nxt_lin
[00002d42] 4eb9 0000 22c4            jsr       _nextlin
[00002d48] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002d4e] d1cd                      adda.l    a5,a0
[00002d50] 0c10 0061                 cmpi.b    #$61,(a0)
[00002d54] 6600 0026                 bne       $00002D7C
[00002d58] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002d5e] d1cd                      adda.l    a5,a0
[00002d60] 0c28 0064 0001            cmpi.b    #$64,1(a0)
[00002d66] 6600 0014                 bne       $00002D7C
[00002d6a] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002d70] d1cd                      adda.l    a5,a0
[00002d72] 0c28 0064 0002            cmpi.b    #$64,2(a0)
[00002d78] 6700 0036                 beq       $00002DB0
[00002d7c] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002d82] d1cd                      adda.l    a5,a0
[00002d84] 0c10 0073                 cmpi.b    #$73,(a0)
[00002d88] 6600 0338                 bne       $000030C2
[00002d8c] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002d92] d1cd                      adda.l    a5,a0
[00002d94] 0c28 0075 0001            cmpi.b    #$75,1(a0)
[00002d9a] 6600 0326                 bne       $000030C2
[00002d9e] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002da4] d1cd                      adda.l    a5,a0
[00002da6] 0c28 0062 0002            cmpi.b    #$62,2(a0)
[00002dac] 6600 0314                 bne       $000030C2
[00002db0] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002db6] d1cd                      adda.l    a5,a0
[00002db8] 0c28 002e 0003            cmpi.b    #$2E,3(a0)
[00002dbe] 6600 0302                 bne       $000030C2
[00002dc2] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002dc8] d1cd                      adda.l    a5,a0
[00002dca] 0c28 006c 0004            cmpi.b    #$6C,4(a0)
[00002dd0] 6600 02f0                 bne       $000030C2
[00002dd4] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002dda] d1cd                      adda.l    a5,a0
[00002ddc] 0c28 0020 0005            cmpi.b    #$20,5(a0)
[00002de2] 6600 02de                 bne       $000030C2
[00002de6] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002dec] d1cd                      adda.l    a5,a0
[00002dee] 0c28 0023 0006            cmpi.b    #$23,6(a0)
[00002df4] 6600 02cc                 bne       $000030C2
[00002df8] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002dfe] d1cd                      adda.l    a5,a0
[00002e00] 0c28 002d 0007            cmpi.b    #$2D,7(a0)
[00002e06] 6700 0026                 beq       $00002E2E
[00002e0a] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002e10] d1cd                      adda.l    a5,a0
[00002e12] 0c28 0030 0007            cmpi.b    #$30,7(a0)
[00002e18] 6d00 02a8                 blt       $000030C2
[00002e1c] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002e22] d1cd                      adda.l    a5,a0
[00002e24] 0c28 0039 0007            cmpi.b    #$39,7(a0)
[00002e2a] 6e00 0296                 bgt       $000030C2
[00002e2e] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002e34] d1cd                      adda.l    a5,a0
[00002e36] 0c10 0061                 cmpi.b    #$61,(a0)
[00002e3a] 6700 0008                 beq       $00002E44
[00002e3e] 4240                      clr.w     d0
[00002e40] 6000 0006                 bra       $00002E48
[00002e44] 303c 0001                 move.w    #$0001,d0
[00002e48] 3d40 fffc                 move.w    d0,-4(a6)
[00002e4c] 2079 0000 606e            movea.l   _nxt_lin,a0
[00002e52] d1cd                      adda.l    a5,a0
[00002e54] 0c28 002d 0007            cmpi.b    #$2D,7(a0)
[00002e5a] 6600 002c                 bne       $00002E88
[00002e5e] 2039 0000 606e            move.l    _nxt_lin,d0
[00002e64] d0bc 0000 0008            add.l     #$00000008,d0
[00002e6a] 2d40 fff0                 move.l    d0,-16(a6)
[00002e6e] 4a6e fffc                 tst.w     -4(a6)
[00002e72] 6700 0008                 beq       $00002E7C
[00002e76] 4240                      clr.w     d0
[00002e78] 6000 0006                 bra       $00002E80
[00002e7c] 303c 0001                 move.w    #$0001,d0
[00002e80] 3d40 fffc                 move.w    d0,-4(a6)
[00002e84] 6000 0012                 bra       $00002E98
[00002e88] 2039 0000 606e            move.l    _nxt_lin,d0
[00002e8e] d0bc 0000 0007            add.l     #$00000007,d0
[00002e94] 2d40 fff0                 move.l    d0,-16(a6)
[00002e98] 2d6e fff0 ffec            move.l    -16(a6),-20(a6)
[00002e9e] 6000 000a                 bra       $00002EAA
[00002ea2] 06ae 0000 0001 fff0       addi.l    #$00000001,-16(a6)
[00002eaa] 206e fff0                 movea.l   -16(a6),a0
[00002eae] d1cd                      adda.l    a5,a0
[00002eb0] 0c10 0030                 cmpi.b    #$30,(a0)
[00002eb4] 6d00 000e                 blt       $00002EC4
[00002eb8] 206e fff0                 movea.l   -16(a6),a0
[00002ebc] d1cd                      adda.l    a5,a0
[00002ebe] 0c10 0039                 cmpi.b    #$39,(a0)
[00002ec2] 6fde                      ble.s     $00002EA2
[00002ec4] 206e fff0                 movea.l   -16(a6),a0
[00002ec8] d1cd                      adda.l    a5,a0
[00002eca] 0c28 0028 0001            cmpi.b    #$28,1(a0)
[00002ed0] 6600 01f0                 bne       $000030C2
[00002ed4] 206e fff0                 movea.l   -16(a6),a0
[00002ed8] d1cd                      adda.l    a5,a0
[00002eda] 0c28 0073 0002            cmpi.b    #$73,2(a0)
[00002ee0] 6600 01e0                 bne       $000030C2
[00002ee4] 42ae fff8                 clr.l     -8(a6)
[00002ee8] 2d6e ffec fff4            move.l    -20(a6),-12(a6)
[00002eee] 6000 0034                 bra       $00002F24
[00002ef2] 2f3c 0000 000a            move.l    #$0000000A,-(a7)
[00002ef8] 2f2e fff8                 move.l    -8(a6),-(a7)
[00002efc] 4eb9 0000 3a10            jsr       lmul
[00002f02] 508f                      addq.l    #8,a7
[00002f04] 2d40 fff8                 move.l    d0,-8(a6)
[00002f08] 202e fff4                 move.l    -12(a6),d0
[00002f0c] 1035 0800                 move.b    0(a5,d0.l),d0
[00002f10] 4880                      ext.w     d0
[00002f12] d07c ffd0                 add.w     #$FFD0,d0
[00002f16] 48c0                      ext.l     d0
[00002f18] d1ae fff8                 add.l     d0,-8(a6)
[00002f1c] 06ae 0000 0001 fff4       addi.l    #$00000001,-12(a6)
[00002f24] 202e fff4                 move.l    -12(a6),d0
[00002f28] b0ae fff0                 cmp.l     -16(a6),d0
[00002f2c] 6dc4                      blt.s     $00002EF2
[00002f2e] 202e fff8                 move.l    -8(a6),d0
[00002f32] b0bc 0000 8000            cmp.l     #$00008000,d0
[00002f38] 6c00 0188                 bge       $000030C2
[00002f3c] 4eb9 0000 3352            jsr       _add4mun
[00002f42] 4a40                      tst.w     d0
[00002f44] 6700 017c                 beq       $000030C2
[00002f48] 2079 0000 6060            movea.l   _out_ind,a0
[00002f4e] d1f9 0000 6064            adda.l    _out_buf,a0
[00002f54] 10bc 0070                 move.b    #$70,(a0)
[00002f58] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002f62] 2079 0000 6060            movea.l   _out_ind,a0
[00002f68] d1f9 0000 6064            adda.l    _out_buf,a0
[00002f6e] 10bc 0065                 move.b    #$65,(a0)
[00002f72] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002f7c] 2079 0000 6060            movea.l   _out_ind,a0
[00002f82] d1f9 0000 6064            adda.l    _out_buf,a0
[00002f88] 10bc 0061                 move.b    #$61,(a0)
[00002f8c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002f96] 2079 0000 6060            movea.l   _out_ind,a0
[00002f9c] d1f9 0000 6064            adda.l    _out_buf,a0
[00002fa2] 10bc 0020                 move.b    #$20,(a0)
[00002fa6] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002fb0] 4a6e fffc                 tst.w     -4(a6)
[00002fb4] 6600 001c                 bne       $00002FD2
[00002fb8] 2079 0000 6060            movea.l   _out_ind,a0
[00002fbe] d1f9 0000 6064            adda.l    _out_buf,a0
[00002fc4] 10bc 002d                 move.b    #$2D,(a0)
[00002fc8] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002fd2] 2039 0000 6064            move.l    _out_buf,d0
[00002fd8] d0b9 0000 6060            add.l     _out_ind,d0
[00002fde] 2e80                      move.l    d0,(a7)
[00002fe0] 202e fff8                 move.l    -8(a6),d0
[00002fe4] 3f00                      move.w    d0,-(a7)
[00002fe6] 4eb9 0000 0a14            jsr       _itoa
[00002fec] 548f                      addq.l    #2,a7
[00002fee] 6000 000c                 bra       $00002FFC
[00002ff2] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00002ffc] 2079 0000 6060            movea.l   _out_ind,a0
[00003002] 2239 0000 6064            move.l    _out_buf,d1
[00003008] 4a30 1800                 tst.b     0(a0,d1.l)
[0000300c] 66e4                      bne.s     $00002FF2
[0000300e] 2079 0000 6060            movea.l   _out_ind,a0
[00003014] d1f9 0000 6064            adda.l    _out_buf,a0
[0000301a] 10bc 0028                 move.b    #$28,(a0)
[0000301e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00003028] 2079 0000 6060            movea.l   _out_ind,a0
[0000302e] d1f9 0000 6064            adda.l    _out_buf,a0
[00003034] 10bc 0052                 move.b    #$52,(a0)
[00003038] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00003042] 0c6e 0009 fffe            cmpi.w    #$0009,-2(a6)
[00003048] 6f00 001c                 ble       $00003066
[0000304c] 2079 0000 6060            movea.l   _out_ind,a0
[00003052] d1f9 0000 6064            adda.l    _out_buf,a0
[00003058] 10bc 0031                 move.b    #$31,(a0)
[0000305c] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[00003066] 302e fffe                 move.w    -2(a6),d0
[0000306a] 48c0                      ext.l     d0
[0000306c] 81fc 000a                 divs.w    #$000A,d0
[00003070] 4840                      swap      d0
[00003072] d07c 0030                 add.w     #$0030,d0
[00003076] 2279 0000 6060            movea.l   _out_ind,a1
[0000307c] d3f9 0000 6064            adda.l    _out_buf,a1
[00003082] 1280                      move.b    d0,(a1)
[00003084] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[0000308e] 2079 0000 6060            movea.l   _out_ind,a0
[00003094] d1f9 0000 6064            adda.l    _out_buf,a0
[0000309a] 10bc 0029                 move.b    #$29,(a0)
[0000309e] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000030a8] 4eb9 0000 22c4            jsr       _nextlin
[000030ae] 2e39 0000 606e            move.l    _nxt_lin,d7
[000030b4] 9ebc 0000 0002            sub.l     #$00000002,d7
[000030ba] 0679 0001 0000 6000       addi.w    #$0001,_mv2_tot
[000030c2] 2079 0000 6060            movea.l   _out_ind,a0
[000030c8] d1f9 0000 6064            adda.l    _out_buf,a0
[000030ce] 2247                      movea.l   d7,a1
[000030d0] d3cd                      adda.l    a5,a1
[000030d2] 1091                      move.b    (a1),(a0)
[000030d4] debc 0000 0001            add.l     #$00000001,d7
[000030da] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000030e4] beb9 0000 5ff6            cmp.l     _size,d7
[000030ea] 6d00 fb5c                 blt       $00002C48
[000030ee] 4a9f                      tst.l     (a7)+
[000030f0] 4cdf 2080                 movem.l   (a7)+,d7/a5
[000030f4] 4e5e                      unlk      a6
[000030f6] 4e75                      rts       
[000030f8] _adda:
[000030f8] 4e56 fff4                 link      a6,#$FFF4
[000030fc] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00003100] 4280                      clr.l     d0
[00003102] 23c0 0000 6060            move.l    d0,_out_ind
[00003108] 2e00                      move.l    d0,d7
[0000310a] 2a79 0000 602e            movea.l   _in_buff,a5
[00003110] 6000 01da                 bra       $000032EC
[00003114] 2047                      movea.l   d7,a0
[00003116] d1cd                      adda.l    a5,a0
[00003118] 0c10 0061                 cmpi.b    #$61,(a0)
[0000311c] 6600 001e                 bne       $0000313C
[00003120] 2047                      movea.l   d7,a0
[00003122] d1cd                      adda.l    a5,a0
[00003124] 0c28 0064 0001            cmpi.b    #$64,1(a0)
[0000312a] 6600 0010                 bne       $0000313C
[0000312e] 2047                      movea.l   d7,a0
[00003130] d1cd                      adda.l    a5,a0
[00003132] 0c28 0064 0002            cmpi.b    #$64,2(a0)
[00003138] 6700 002a                 beq       $00003164
[0000313c] 2047                      movea.l   d7,a0
[0000313e] d1cd                      adda.l    a5,a0
[00003140] 0c10 0073                 cmpi.b    #$73,(a0)
[00003144] 6600 0184                 bne       $000032CA
[00003148] 2047                      movea.l   d7,a0
[0000314a] d1cd                      adda.l    a5,a0
[0000314c] 0c28 0075 0001            cmpi.b    #$75,1(a0)
[00003152] 6600 0176                 bne       $000032CA
[00003156] 2047                      movea.l   d7,a0
[00003158] d1cd                      adda.l    a5,a0
[0000315a] 0c28 0062 0002            cmpi.b    #$62,2(a0)
[00003160] 6600 0168                 bne       $000032CA
[00003164] 2007                      move.l    d7,d0
[00003166] d0bc 0000 0003            add.l     #$00000003,d0
[0000316c] 2d40 fff8                 move.l    d0,-8(a6)
[00003170] 206e fff8                 movea.l   -8(a6),a0
[00003174] d1cd                      adda.l    a5,a0
[00003176] 0c10 0061                 cmpi.b    #$61,(a0)
[0000317a] 6600 000a                 bne       $00003186
[0000317e] 06ae 0000 0001 fff8       addi.l    #$00000001,-8(a6)
[00003186] 206e fff8                 movea.l   -8(a6),a0
[0000318a] d1cd                      adda.l    a5,a0
[0000318c] 0c10 0020                 cmpi.b    #$20,(a0)
[00003190] 6700 0138                 beq       $000032CA
[00003194] 06ae 0000 0001 fff8       addi.l    #$00000001,-8(a6)
[0000319c] 206e fff8                 movea.l   -8(a6),a0
[000031a0] d1cd                      adda.l    a5,a0
[000031a2] 0c10 006c                 cmpi.b    #$6C,(a0)
[000031a6] 6600 0122                 bne       $000032CA
[000031aa] 2d6e fff8 fff4            move.l    -8(a6),-12(a6)
[000031b0] 06ae 0000 0002 fff8       addi.l    #$00000002,-8(a6)
[000031b8] 206e fff8                 movea.l   -8(a6),a0
[000031bc] d1cd                      adda.l    a5,a0
[000031be] 0c10 0023                 cmpi.b    #$23,(a0)
[000031c2] 6600 0022                 bne       $000031E6
[000031c6] 206e fff8                 movea.l   -8(a6),a0
[000031ca] d1cd                      adda.l    a5,a0
[000031cc] 0c28 0039 0001            cmpi.b    #$39,1(a0)
[000031d2] 6e00 0012                 bgt       $000031E6
[000031d6] 206e fff8                 movea.l   -8(a6),a0
[000031da] d1cd                      adda.l    a5,a0
[000031dc] 0c28 0030 0001            cmpi.b    #$30,1(a0)
[000031e2] 6c00 0006                 bge       $000031EA
[000031e6] 6000 00e2                 bra       $000032CA
[000031ea] 42ae fffc                 clr.l     -4(a6)
[000031ee] 6000 0034                 bra       $00003224
[000031f2] 2f3c 0000 000a            move.l    #$0000000A,-(a7)
[000031f8] 2f2e fffc                 move.l    -4(a6),-(a7)
[000031fc] 4eb9 0000 3a10            jsr       lmul
[00003202] 508f                      addq.l    #8,a7
[00003204] 2d40 fffc                 move.l    d0,-4(a6)
[00003208] 202e fff8                 move.l    -8(a6),d0
[0000320c] 1035 0800                 move.b    0(a5,d0.l),d0
[00003210] 4880                      ext.w     d0
[00003212] d07c ffd0                 add.w     #$FFD0,d0
[00003216] 48c0                      ext.l     d0
[00003218] d1ae fffc                 add.l     d0,-4(a6)
[0000321c] 06ae 0000 0001 fff8       addi.l    #$00000001,-8(a6)
[00003224] 206e fff8                 movea.l   -8(a6),a0
[00003228] d1cd                      adda.l    a5,a0
[0000322a] 0c10 0030                 cmpi.b    #$30,(a0)
[0000322e] 6d00 000e                 blt       $0000323E
[00003232] 206e fff8                 movea.l   -8(a6),a0
[00003236] d1cd                      adda.l    a5,a0
[00003238] 0c10 0039                 cmpi.b    #$39,(a0)
[0000323c] 6fb4                      ble.s     $000031F2
[0000323e] 0cae 0000 7fff fffc       cmpi.l    #$00007FFF,-4(a6)
[00003246] 6e00 0082                 bgt       $000032CA
[0000324a] 2047                      movea.l   d7,a0
[0000324c] d1cd                      adda.l    a5,a0
[0000324e] 0c28 0061 0003            cmpi.b    #$61,3(a0)
[00003254] 6700 0062                 beq       $000032B8
[00003258] 206e fff8                 movea.l   -8(a6),a0
[0000325c] d1cd                      adda.l    a5,a0
[0000325e] 0c28 0052 0001            cmpi.b    #$52,1(a0)
[00003264] 6600 0064                 bne       $000032CA
[00003268] 206e fff8                 movea.l   -8(a6),a0
[0000326c] d1cd                      adda.l    a5,a0
[0000326e] 0c28 0031 0002            cmpi.b    #$31,2(a0)
[00003274] 6600 0012                 bne       $00003288
[00003278] 206e fff8                 movea.l   -8(a6),a0
[0000327c] d1cd                      adda.l    a5,a0
[0000327e] 0c28 0030 0003            cmpi.b    #$30,3(a0)
[00003284] 6d00 0044                 blt       $000032CA
[00003288] 206e fff8                 movea.l   -8(a6),a0
[0000328c] d1cd                      adda.l    a5,a0
[0000328e] 0c28 0035 0003            cmpi.b    #$35,3(a0)
[00003294] 6e00 0034                 bgt       $000032CA
[00003298] 206e fff8                 movea.l   -8(a6),a0
[0000329c] d1cd                      adda.l    a5,a0
[0000329e] 0c28 0038 0002            cmpi.b    #$38,2(a0)
[000032a4] 6d00 0024                 blt       $000032CA
[000032a8] 206e fff8                 movea.l   -8(a6),a0
[000032ac] d1cd                      adda.l    a5,a0
[000032ae] 0c28 0039 0002            cmpi.b    #$39,2(a0)
[000032b4] 6e00 0014                 bgt       $000032CA
[000032b8] 206e fff4                 movea.l   -12(a6),a0
[000032bc] d1cd                      adda.l    a5,a0
[000032be] 10bc 0077                 move.b    #$77,(a0)
[000032c2] 0679 0001 0000 6002       addi.w    #$0001,_adda_to
[000032ca] 2079 0000 6060            movea.l   _out_ind,a0
[000032d0] d1f9 0000 6064            adda.l    _out_buf,a0
[000032d6] 2247                      movea.l   d7,a1
[000032d8] d3cd                      adda.l    a5,a1
[000032da] 1091                      move.b    (a1),(a0)
[000032dc] debc 0000 0001            add.l     #$00000001,d7
[000032e2] 06b9 0000 0001 0000 6060  addi.l    #$00000001,_out_ind
[000032ec] beb9 0000 5ff6            cmp.l     _size,d7
[000032f2] 6d00 fe20                 blt       $00003114
[000032f6] 4a9f                      tst.l     (a7)+
[000032f8] 4cdf 2080                 movem.l   (a7)+,d7/a5
[000032fc] 4e5e                      unlk      a6
[000032fe] 4e75                      rts       
[00003300] _panic:
[00003300] 4e56 fffc                 link      a6,#$FFFC
[00003304] 2eae 0008                 move.l    8(a6),(a7)
[00003308] 2f3c 0000 5d00            move.l    #$00005D00,-(a7)
[0000330e] 4eb9 0000 57fc            jsr       _printf
[00003314] 588f                      addq.l    #4,a7
[00003316] 2ebc 0000 5d0a            move.l    #$00005D0A,(a7)
[0000331c] 4eb9 0000 57fc            jsr       _printf
[00003322] 6000 0002                 bra       $00003326
[00003326] 3ebc 0001                 move.w    #$0001,(a7)
[0000332a] 4eb9 0000 3a00            jsr       _gemdos
[00003330] c0bc 0000 00ff            and.l     #$000000FF,d0
[00003336] b0bc 0000 0003            cmp.l     #$00000003,d0
[0000333c] 66e8                      bne.s     $00003326
[0000333e] 3ebc ffff                 move.w    #$FFFF,(a7)
[00003342] 3f3c 004c                 move.w    #$004C,-(a7)
[00003346] 4eb9 0000 3a00            jsr       _gemdos
[0000334c] 548f                      addq.l    #2,a7
[0000334e] 4e5e                      unlk      a6
[00003350] 4e75                      rts       
[00003352] _add4mun:
[00003352] 4e56 ffe8                 link      a6,#$FFE8
[00003356] 426e ffee                 clr.w     -18(a6)
[0000335a] 2d79 0000 606e fff4       move.l    _nxt_lin,-12(a6)
[00003362] 6000 00c6                 bra       $0000342A
[00003366] 2039 0000 606e            move.l    _nxt_lin,d0
[0000336c] b0b9 0000 5ff6            cmp.l     _size,d0
[00003372] 6700 0318                 beq       $0000368C
[00003376] 4eb9 0000 22c4            jsr       _nextlin
[0000337c] 42ae fff8                 clr.l     -8(a6)
[00003380] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[00003388] 6000 0022                 bra       $000033AC
[0000338c] 2079 0000 602e            movea.l   _in_buff,a0
[00003392] d1ee fffc                 adda.l    -4(a6),a0
[00003396] 0c10 003a                 cmpi.b    #$3A,(a0)
[0000339a] 6600 0008                 bne       $000033A4
[0000339e] 2d6e fffc fff8            move.l    -4(a6),-8(a6)
[000033a4] 06ae 0000 0001 fffc       addi.l    #$00000001,-4(a6)
[000033ac] 2079 0000 602e            movea.l   _in_buff,a0
[000033b2] d1ee fffc                 adda.l    -4(a6),a0
[000033b6] 0c10 000a                 cmpi.b    #$0A,(a0)
[000033ba] 66d0                      bne.s     $0000338C
[000033bc] 4aae fff8                 tst.l     -8(a6)
[000033c0] 6700 0014                 beq       $000033D6
[000033c4] 202e fff8                 move.l    -8(a6),d0
[000033c8] d0bc 0000 0001            add.l     #$00000001,d0
[000033ce] 2d40 fffc                 move.l    d0,-4(a6)
[000033d2] 6000 000a                 bra       $000033DE
[000033d6] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[000033de] 2079 0000 602e            movea.l   _in_buff,a0
[000033e4] d1ee fffc                 adda.l    -4(a6),a0
[000033e8] 0c10 006a                 cmpi.b    #$6A,(a0)
[000033ec] 6700 0014                 beq       $00003402
[000033f0] 2079 0000 602e            movea.l   _in_buff,a0
[000033f6] d1ee fffc                 adda.l    -4(a6),a0
[000033fa] 0c10 0062                 cmpi.b    #$62,(a0)
[000033fe] 6600 002a                 bne       $0000342A
[00003402] 2079 0000 602e            movea.l   _in_buff,a0
[00003408] d1ee fffc                 adda.l    -4(a6),a0
[0000340c] 0c28 0073 0001            cmpi.b    #$73,1(a0)
[00003412] 6600 0016                 bne       $0000342A
[00003416] 2079 0000 602e            movea.l   _in_buff,a0
[0000341c] d1ee fffc                 adda.l    -4(a6),a0
[00003420] 0c28 0072 0002            cmpi.b    #$72,2(a0)
[00003426] 6700 0006                 beq       $0000342E
[0000342a] 6000 ff3a                 bra       $00003366
[0000342e] 4eb9 0000 22c4            jsr       _nextlin
[00003434] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[0000343c] 6000 000a                 bra       $00003448
[00003440] 06ae 0000 0001 fffc       addi.l    #$00000001,-4(a6)
[00003448] 2079 0000 602e            movea.l   _in_buff,a0
[0000344e] d1ee fffc                 adda.l    -4(a6),a0
[00003452] 0c10 000a                 cmpi.b    #$0A,(a0)
[00003456] 66e8                      bne.s     $00003440
[00003458] 206e fffc                 movea.l   -4(a6),a0
[0000345c] d1f9 0000 602e            adda.l    _in_buff,a0
[00003462] 0c28 000d ffff            cmpi.b    #$0D,-1(a0)
[00003468] 6600 000a                 bne       $00003474
[0000346c] 04ae 0000 0001 fffc       subi.l    #$00000001,-4(a6)
[00003474] 206e fffc                 movea.l   -4(a6),a0
[00003478] d1f9 0000 602e            adda.l    _in_buff,a0
[0000347e] 0c28 002c fffd            cmpi.b    #$2C,-3(a0)
[00003484] 6600 0206                 bne       $0000368C
[00003488] 206e fffc                 movea.l   -4(a6),a0
[0000348c] d1f9 0000 602e            adda.l    _in_buff,a0
[00003492] 0c28 0073 fffe            cmpi.b    #$73,-2(a0)
[00003498] 6600 01f2                 bne       $0000368C
[0000349c] 206e fffc                 movea.l   -4(a6),a0
[000034a0] d1f9 0000 602e            adda.l    _in_buff,a0
[000034a6] 0c28 0070 ffff            cmpi.b    #$70,-1(a0)
[000034ac] 6600 01de                 bne       $0000368C
[000034b0] 2079 0000 602e            movea.l   _in_buff,a0
[000034b6] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000034bc] 0c10 0061                 cmpi.b    #$61,(a0)
[000034c0] 6600 01c0                 bne       $00003682
[000034c4] 2079 0000 602e            movea.l   _in_buff,a0
[000034ca] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000034d0] 0c28 0064 0001            cmpi.b    #$64,1(a0)
[000034d6] 6600 01aa                 bne       $00003682
[000034da] 2079 0000 602e            movea.l   _in_buff,a0
[000034e0] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000034e6] 0c28 0071 0003            cmpi.b    #$71,3(a0)
[000034ec] 6600 00b4                 bne       $000035A2
[000034f0] 2079 0000 602e            movea.l   _in_buff,a0
[000034f6] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000034fc] 0c28 0038 0008            cmpi.b    #$38,8(a0)
[00003502] 6700 0018                 beq       $0000351C
[00003506] 2079 0000 602e            movea.l   _in_buff,a0
[0000350c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003512] 0c28 0036 0008            cmpi.b    #$36,8(a0)
[00003518] 6600 0072                 bne       $0000358C
[0000351c] 2079 0000 602e            movea.l   _in_buff,a0
[00003522] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003528] 117c 002e 0003            move.b    #$2E,3(a0)
[0000352e] 2079 0000 602e            movea.l   _in_buff,a0
[00003534] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000353a] 117c 0077 0004            move.b    #$77,4(a0)
[00003540] 2079 0000 602e            movea.l   _in_buff,a0
[00003546] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000354c] 117c 0020 0005            move.b    #$20,5(a0)
[00003552] 2079 0000 602e            movea.l   _in_buff,a0
[00003558] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000355e] 117c 0023 0006            move.b    #$23,6(a0)
[00003564] 2079 0000 602e            movea.l   _in_buff,a0
[0000356a] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003570] 117c 0031 0007            move.b    #$31,7(a0)
[00003576] 2079 0000 602e            movea.l   _in_buff,a0
[0000357c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003582] 0428 0006 0008            subi.b    #$06,8(a0)
[00003588] 6000 0014                 bra       $0000359E
[0000358c] 2079 0000 602e            movea.l   _in_buff,a0
[00003592] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003598] 0628 0004 0008            addi.b    #$04,8(a0)
[0000359e] 6000 00da                 bra       $0000367A
[000035a2] 2079 0000 602e            movea.l   _in_buff,a0
[000035a8] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000035ae] 0c28 0061 0003            cmpi.b    #$61,3(a0)
[000035b4] 6600 0016                 bne       $000035CC
[000035b8] 2039 0000 606e            move.l    _nxt_lin,d0
[000035be] d0bc 0000 0008            add.l     #$00000008,d0
[000035c4] 2d40 fffc                 move.l    d0,-4(a6)
[000035c8] 6000 0012                 bra       $000035DC
[000035cc] 2039 0000 606e            move.l    _nxt_lin,d0
[000035d2] d0bc 0000 0007            add.l     #$00000007,d0
[000035d8] 2d40 fffc                 move.l    d0,-4(a6)
[000035dc] 2039 0000 602e            move.l    _in_buff,d0
[000035e2] d0ae fffc                 add.l     -4(a6),d0
[000035e6] 2e80                      move.l    d0,(a7)
[000035e8] 4eb9 0000 0a92            jsr       _atoi
[000035ee] 3d40 ffec                 move.w    d0,-20(a6)
[000035f2] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[000035f8] 3f2e ffec                 move.w    -20(a6),-(a7)
[000035fc] 4eb9 0000 0a14            jsr       _itoa
[00003602] 548f                      addq.l    #2,a7
[00003604] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[0000360a] 3f2e ffec                 move.w    -20(a6),-(a7)
[0000360e] 0657 0004                 addi.w    #$0004,(a7)
[00003612] 4eb9 0000 0a14            jsr       _itoa
[00003618] 548f                      addq.l    #2,a7
[0000361a] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00003620] 4eb9 0000 0954            jsr       _strlen
[00003626] 3f00                      move.w    d0,-(a7)
[00003628] 2f3c 0000 5fea            move.l    #_numstr,-(a7)
[0000362e] 4eb9 0000 0954            jsr       _strlen
[00003634] 588f                      addq.l    #4,a7
[00003636] b05f                      cmp.w     (a7)+,d0
[00003638] 6600 0052                 bne       $0000368C
[0000363c] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00003642] 4eb9 0000 0954            jsr       _strlen
[00003648] 907c 0001                 sub.w     #$0001,d0
[0000364c] 3d40 fff2                 move.w    d0,-14(a6)
[00003650] 6000 0022                 bra       $00003674
[00003654] 306e fff2                 movea.w   -14(a6),a0
[00003658] d1ee fffc                 adda.l    -4(a6),a0
[0000365c] d1f9 0000 602e            adda.l    _in_buff,a0
[00003662] 326e fff2                 movea.w   -14(a6),a1
[00003666] d3fc 0000 6032            adda.l    #_numstr1,a1
[0000366c] 1091                      move.b    (a1),(a0)
[0000366e] 046e 0001 fff2            subi.w    #$0001,-14(a6)
[00003674] 4a6e fff2                 tst.w     -14(a6)
[00003678] 6cda                      bge.s     $00003654
[0000367a] 6000 000a                 bra       $00003686
[0000367e] 6000 0006                 bra       $00003686
[00003682] 6000 0008                 bra       $0000368C
[00003686] 3d7c 0001 ffee            move.w    #$0001,-18(a6)
[0000368c] 23ee fff4 0000 606e       move.l    -12(a6),_nxt_lin
[00003694] 302e ffee                 move.w    -18(a6),d0
[00003698] 6000 0002                 bra       $0000369C
[0000369c] 4e5e                      unlk      a6
[0000369e] 4e75                      rts       
[000036a0] _addinra:
[000036a0] 4e56 ffe8                 link      a6,#$FFE8
[000036a4] 426e ffee                 clr.w     -18(a6)
[000036a8] 2d79 0000 606e fff4       move.l    _nxt_lin,-12(a6)
[000036b0] 6000 00c6                 bra       $00003778
[000036b4] 2039 0000 606e            move.l    _nxt_lin,d0
[000036ba] b0b9 0000 5ff6            cmp.l     _size,d0
[000036c0] 6700 030a                 beq       $000039CC
[000036c4] 4eb9 0000 22c4            jsr       _nextlin
[000036ca] 42ae fff8                 clr.l     -8(a6)
[000036ce] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[000036d6] 6000 0022                 bra       $000036FA
[000036da] 2079 0000 602e            movea.l   _in_buff,a0
[000036e0] d1ee fffc                 adda.l    -4(a6),a0
[000036e4] 0c10 003a                 cmpi.b    #$3A,(a0)
[000036e8] 6600 0008                 bne       $000036F2
[000036ec] 2d6e fffc fff8            move.l    -4(a6),-8(a6)
[000036f2] 06ae 0000 0001 fffc       addi.l    #$00000001,-4(a6)
[000036fa] 2079 0000 602e            movea.l   _in_buff,a0
[00003700] d1ee fffc                 adda.l    -4(a6),a0
[00003704] 0c10 000a                 cmpi.b    #$0A,(a0)
[00003708] 66d0                      bne.s     $000036DA
[0000370a] 4aae fff8                 tst.l     -8(a6)
[0000370e] 6700 0014                 beq       $00003724
[00003712] 202e fff8                 move.l    -8(a6),d0
[00003716] d0bc 0000 0001            add.l     #$00000001,d0
[0000371c] 2d40 fffc                 move.l    d0,-4(a6)
[00003720] 6000 000a                 bra       $0000372C
[00003724] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[0000372c] 2079 0000 602e            movea.l   _in_buff,a0
[00003732] d1ee fffc                 adda.l    -4(a6),a0
[00003736] 0c10 006a                 cmpi.b    #$6A,(a0)
[0000373a] 6700 0014                 beq       $00003750
[0000373e] 2079 0000 602e            movea.l   _in_buff,a0
[00003744] d1ee fffc                 adda.l    -4(a6),a0
[00003748] 0c10 0062                 cmpi.b    #$62,(a0)
[0000374c] 6600 002a                 bne       $00003778
[00003750] 2079 0000 602e            movea.l   _in_buff,a0
[00003756] d1ee fffc                 adda.l    -4(a6),a0
[0000375a] 0c28 0073 0001            cmpi.b    #$73,1(a0)
[00003760] 6600 0016                 bne       $00003778
[00003764] 2079 0000 602e            movea.l   _in_buff,a0
[0000376a] d1ee fffc                 adda.l    -4(a6),a0
[0000376e] 0c28 0072 0002            cmpi.b    #$72,2(a0)
[00003774] 6700 0006                 beq       $0000377C
[00003778] 6000 ff3a                 bra       $000036B4
[0000377c] 4eb9 0000 22c4            jsr       _nextlin
[00003782] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[0000378a] 2d79 0000 606e fffc       move.l    _nxt_lin,-4(a6)
[00003792] 6000 000a                 bra       $0000379E
[00003796] 06ae 0000 0001 fffc       addi.l    #$00000001,-4(a6)
[0000379e] 2079 0000 602e            movea.l   _in_buff,a0
[000037a4] d1ee fffc                 adda.l    -4(a6),a0
[000037a8] 0c10 000a                 cmpi.b    #$0A,(a0)
[000037ac] 66e8                      bne.s     $00003796
[000037ae] 206e fffc                 movea.l   -4(a6),a0
[000037b2] d1f9 0000 602e            adda.l    _in_buff,a0
[000037b8] 0c28 000d ffff            cmpi.b    #$0D,-1(a0)
[000037be] 6600 000a                 bne       $000037CA
[000037c2] 04ae 0000 0001 fffc       subi.l    #$00000001,-4(a6)
[000037ca] 206e fffc                 movea.l   -4(a6),a0
[000037ce] d1f9 0000 602e            adda.l    _in_buff,a0
[000037d4] 0c28 002c fffd            cmpi.b    #$2C,-3(a0)
[000037da] 6600 01f0                 bne       $000039CC
[000037de] 206e fffc                 movea.l   -4(a6),a0
[000037e2] d1f9 0000 602e            adda.l    _in_buff,a0
[000037e8] 0c28 0073 fffe            cmpi.b    #$73,-2(a0)
[000037ee] 6600 01dc                 bne       $000039CC
[000037f2] 206e fffc                 movea.l   -4(a6),a0
[000037f6] d1f9 0000 602e            adda.l    _in_buff,a0
[000037fc] 0c28 0070 ffff            cmpi.b    #$70,-1(a0)
[00003802] 6600 01c8                 bne       $000039CC
[00003806] 2079 0000 602e            movea.l   _in_buff,a0
[0000380c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003812] 0c10 0061                 cmpi.b    #$61,(a0)
[00003816] 6600 01aa                 bne       $000039C2
[0000381a] 2079 0000 602e            movea.l   _in_buff,a0
[00003820] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003826] 0c28 0064 0001            cmpi.b    #$64,1(a0)
[0000382c] 6600 0194                 bne       $000039C2
[00003830] 2079 0000 602e            movea.l   _in_buff,a0
[00003836] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000383c] 0c28 0071 0003            cmpi.b    #$71,3(a0)
[00003842] 6600 009e                 bne       $000038E2
[00003846] 2079 0000 602e            movea.l   _in_buff,a0
[0000384c] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003852] 0c28 0038 0008            cmpi.b    #$38,8(a0)
[00003858] 6600 0072                 bne       $000038CC
[0000385c] 2079 0000 602e            movea.l   _in_buff,a0
[00003862] d1f9 0000 606e            adda.l    _nxt_lin,a0
[00003868] 117c 002e 0003            move.b    #$2E,3(a0)
[0000386e] 2079 0000 602e            movea.l   _in_buff,a0
[00003874] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000387a] 117c 0077 0004            move.b    #$77,4(a0)
[00003880] 2079 0000 602e            movea.l   _in_buff,a0
[00003886] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000388c] 117c 0020 0005            move.b    #$20,5(a0)
[00003892] 2079 0000 602e            movea.l   _in_buff,a0
[00003898] d1f9 0000 606e            adda.l    _nxt_lin,a0
[0000389e] 117c 0023 0006            move.b    #$23,6(a0)
[000038a4] 2079 0000 602e            movea.l   _in_buff,a0
[000038aa] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000038b0] 117c 0031 0007            move.b    #$31,7(a0)
[000038b6] 2079 0000 602e            movea.l   _in_buff,a0
[000038bc] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000038c2] 117c 0030 0008            move.b    #$30,8(a0)
[000038c8] 6000 0014                 bra       $000038DE
[000038cc] 2079 0000 602e            movea.l   _in_buff,a0
[000038d2] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000038d8] 0628 0002 0008            addi.b    #$02,8(a0)
[000038de] 6000 00da                 bra       $000039BA
[000038e2] 2079 0000 602e            movea.l   _in_buff,a0
[000038e8] d1f9 0000 606e            adda.l    _nxt_lin,a0
[000038ee] 0c28 0061 0003            cmpi.b    #$61,3(a0)
[000038f4] 6600 0016                 bne       $0000390C
[000038f8] 2039 0000 606e            move.l    _nxt_lin,d0
[000038fe] d0bc 0000 0008            add.l     #$00000008,d0
[00003904] 2d40 fffc                 move.l    d0,-4(a6)
[00003908] 6000 0012                 bra       $0000391C
[0000390c] 2039 0000 606e            move.l    _nxt_lin,d0
[00003912] d0bc 0000 0007            add.l     #$00000007,d0
[00003918] 2d40 fffc                 move.l    d0,-4(a6)
[0000391c] 2039 0000 602e            move.l    _in_buff,d0
[00003922] d0ae fffc                 add.l     -4(a6),d0
[00003926] 2e80                      move.l    d0,(a7)
[00003928] 4eb9 0000 0a92            jsr       _atoi
[0000392e] 3d40 ffec                 move.w    d0,-20(a6)
[00003932] 2ebc 0000 5fea            move.l    #_numstr,(a7)
[00003938] 3f2e ffec                 move.w    -20(a6),-(a7)
[0000393c] 4eb9 0000 0a14            jsr       _itoa
[00003942] 548f                      addq.l    #2,a7
[00003944] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[0000394a] 3f2e ffec                 move.w    -20(a6),-(a7)
[0000394e] 0657 0002                 addi.w    #$0002,(a7)
[00003952] 4eb9 0000 0a14            jsr       _itoa
[00003958] 548f                      addq.l    #2,a7
[0000395a] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00003960] 4eb9 0000 0954            jsr       _strlen
[00003966] 3f00                      move.w    d0,-(a7)
[00003968] 2f3c 0000 5fea            move.l    #_numstr,-(a7)
[0000396e] 4eb9 0000 0954            jsr       _strlen
[00003974] 588f                      addq.l    #4,a7
[00003976] b05f                      cmp.w     (a7)+,d0
[00003978] 6600 0052                 bne       $000039CC
[0000397c] 2ebc 0000 6032            move.l    #_numstr1,(a7)
[00003982] 4eb9 0000 0954            jsr       _strlen
[00003988] 907c 0001                 sub.w     #$0001,d0
[0000398c] 3d40 fff2                 move.w    d0,-14(a6)
[00003990] 6000 0022                 bra       $000039B4
[00003994] 306e fff2                 movea.w   -14(a6),a0
[00003998] d1ee fffc                 adda.l    -4(a6),a0
[0000399c] d1f9 0000 602e            adda.l    _in_buff,a0
[000039a2] 326e fff2                 movea.w   -14(a6),a1
[000039a6] d3fc 0000 6032            adda.l    #_numstr1,a1
[000039ac] 1091                      move.b    (a1),(a0)
[000039ae] 046e 0001 fff2            subi.w    #$0001,-14(a6)
[000039b4] 4a6e fff2                 tst.w     -14(a6)
[000039b8] 6cda                      bge.s     $00003994
[000039ba] 6000 000a                 bra       $000039C6
[000039be] 6000 0006                 bra       $000039C6
[000039c2] 6000 0008                 bra       $000039CC
[000039c6] 3d7c 0001 ffee            move.w    #$0001,-18(a6)
[000039cc] 23ee fff4 0000 606e       move.l    -12(a6),_nxt_lin
[000039d4] 302e ffee                 move.w    -18(a6),d0
[000039d8] 6000 0002                 bra       $000039DC
[000039dc] 4e5e                      unlk      a6
[000039de] 4e75                      rts       
[000039e0] _xbios:
[000039e0] 23df 0000 5fd4            move.l    (a7)+,$00005FD4
[000039e6] 4e4e                      trap      #14
[000039e8] 2f39 0000 5fd4            move.l    $00005FD4,-(a7)
[000039ee] 4e75                      rts       
[000039f0] _bios:
[000039f0] 23df 0000 5fd4            move.l    (a7)+,$00005FD4
[000039f6] 4e4d                      trap      #13
[000039f8] 2f39 0000 5fd4            move.l    $00005FD4,-(a7)
[000039fe] 4e75                      rts       
[00003a00] _gemdos:
[00003a00] 23df 0000 5fd4            move.l    (a7)+,$00005FD4
[00003a06] 4e41                      trap      #1
[00003a08] 2f39 0000 5fd4            move.l    $00005FD4,-(a7)
[00003a0e] 4e75                      rts       
[00003a10] lmul:
[00003a10] 4e56 fffc                 link      a6,#$FFFC
[00003a14] 4242                      clr.w     d2
[00003a16] 4aae 0008                 tst.l     8(a6)
[00003a1a] 6c06                      bge.s     $00003A22
[00003a1c] 44ae 0008                 neg.l     8(a6)
[00003a20] 5242                      addq.w    #1,d2
[00003a22] 4aae 000c                 tst.l     12(a6)
[00003a26] 6c06                      bge.s     $00003A2E
[00003a28] 44ae 000c                 neg.l     12(a6)
[00003a2c] 5242                      addq.w    #1,d2
[00003a2e] 302e 000a                 move.w    10(a6),d0
[00003a32] c0ee 000e                 mulu.w    14(a6),d0
[00003a36] 2d40 fffc                 move.l    d0,-4(a6)
[00003a3a] 302e 0008                 move.w    8(a6),d0
[00003a3e] c0ee 000e                 mulu.w    14(a6),d0
[00003a42] 322e 000c                 move.w    12(a6),d1
[00003a46] c2ee 000a                 mulu.w    10(a6),d1
[00003a4a] d041                      add.w     d1,d0
[00003a4c] d06e fffc                 add.w     -4(a6),d0
[00003a50] 3d40 fffc                 move.w    d0,-4(a6)
[00003a54] 202e fffc                 move.l    -4(a6),d0
[00003a58] 0802 0000                 btst      #0,d2
[00003a5c] 6702                      beq.s     $00003A60
[00003a5e] 4480                      neg.l     d0
[00003a60] 4e5e                      unlk      a6
[00003a62] 4e75                      rts       
[00003a64] _trap:
[00003a64] 23df 0000 5fd8            move.l    (a7)+,$00005FD8
[00003a6a] 4e41                      trap      #1
[00003a6c] 2f39 0000 5fd8            move.l    $00005FD8,-(a7)
[00003a72] 4e75                      rts       
[00003a74] __pc_rea:
[00003a74] 4e56 fffa                 link      a6,#$FFFA
[00003a78] 206e 0008                 movea.l   8(a6),a0
[00003a7c] 3d68 fff2 fffe            move.w    -14(a0),-2(a6)
[00003a82] 2eae 0010                 move.l    16(a6),(a7)
[00003a86] 306e 0016                 movea.w   22(a6),a0
[00003a8a] 2f08                      move.l    a0,-(a7)
[00003a8c] 306e 0014                 movea.w   20(a6),a0
[00003a90] 2f08                      move.l    a0,-(a7)
[00003a92] 4eb9 0000 3a10            jsr       lmul
[00003a98] 508f                      addq.l    #8,a7
[00003a9a] 2f00                      move.l    d0,-(a7)
[00003a9c] 3f2e fffe                 move.w    -2(a6),-(a7)
[00003aa0] 3f3c 003f                 move.w    #$003F,-(a7)
[00003aa4] 4eb9 0000 3a64            jsr       _trap
[00003aaa] 508f                      addq.l    #8,a7
[00003aac] 4e5e                      unlk      a6
[00003aae] 4e75                      rts       
[00003ab0] __pc_wri:
[00003ab0] 4e56 fffa                 link      a6,#$FFFA
[00003ab4] 206e 0008                 movea.l   8(a6),a0
[00003ab8] 3d68 fff2 fffe            move.w    -14(a0),-2(a6)
[00003abe] 2eae 0010                 move.l    16(a6),(a7)
[00003ac2] 306e 0016                 movea.w   22(a6),a0
[00003ac6] 2f08                      move.l    a0,-(a7)
[00003ac8] 306e 0014                 movea.w   20(a6),a0
[00003acc] 2f08                      move.l    a0,-(a7)
[00003ace] 4eb9 0000 3a10            jsr       lmul
[00003ad4] 508f                      addq.l    #8,a7
[00003ad6] 2f00                      move.l    d0,-(a7)
[00003ad8] 3f2e fffe                 move.w    -2(a6),-(a7)
[00003adc] 3f3c 0040                 move.w    #$0040,-(a7)
[00003ae0] 4eb9 0000 3a64            jsr       _trap
[00003ae6] 508f                      addq.l    #8,a7
[00003ae8] 4e5e                      unlk      a6
[00003aea] 4e75                      rts       
[00003aec] _uldiv:
[00003aec] 4e56 0000                 link      a6,#$0000
[00003af0] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00003af4] 2e2e 0008                 move.l    8(a6),d7
[00003af8] 2c2e 000c                 move.l    12(a6),d6
[00003afc] 4a86                      tst.l     d6
[00003afe] 6616                      bne.s     $00003B16
[00003b00] 23fc 8000 0000 0000 5d1e  move.l    #$80000000,_uldivr
[00003b0a] 203c 8000 0000            move.l    #$80000000,d0
[00003b10] 80fc 0000                 divu.w    #$0000,d0
[00003b14] 6048                      bra.s     $00003B5E
[00003b16] bc87                      cmp.l     d7,d6
[00003b18] 630a                      bls.s     $00003B24
[00003b1a] 23c7 0000 5d1e            move.l    d7,_uldivr
[00003b20] 4280                      clr.l     d0
[00003b22] 603a                      bra.s     $00003B5E
[00003b24] be86                      cmp.l     d6,d7
[00003b26] 6606                      bne.s     $00003B2E
[00003b28] 7a01                      moveq.l   #1,d5
[00003b2a] 4287                      clr.l     d7
[00003b2c] 6028                      bra.s     $00003B56
[00003b2e] 7801                      moveq.l   #1,d4
[00003b30] be86                      cmp.l     d6,d7
[00003b32] 650e                      bcs.s     $00003B42
[00003b34] 2006                      move.l    d6,d0
[00003b36] e380                      asl.l     #1,d0
[00003b38] bc80                      cmp.l     d0,d6
[00003b3a] 6206                      bhi.s     $00003B42
[00003b3c] e386                      asl.l     #1,d6
[00003b3e] e384                      asl.l     #1,d4
[00003b40] 60ee                      bra.s     $00003B30
[00003b42] 4285                      clr.l     d5
[00003b44] 4a84                      tst.l     d4
[00003b46] 670e                      beq.s     $00003B56
[00003b48] be86                      cmp.l     d6,d7
[00003b4a] 6504                      bcs.s     $00003B50
[00003b4c] 8a84                      or.l      d4,d5
[00003b4e] 9e86                      sub.l     d6,d7
[00003b50] e28c                      lsr.l     #1,d4
[00003b52] e28e                      lsr.l     #1,d6
[00003b54] 60ee                      bra.s     $00003B44
[00003b56] 23c7 0000 5d1e            move.l    d7,_uldivr
[00003b5c] 2005                      move.l    d5,d0
[00003b5e] 4a9f                      tst.l     (a7)+
[00003b60] 4cdf 00f0                 movem.l   (a7)+,d4-d7
[00003b64] 4e5e                      unlk      a6
[00003b66] 4e75                      rts       
[00003b68] __salloc:
[00003b68] 205f                      movea.l   (a7)+,a0
[00003b6a] 4280                      clr.l     d0
[00003b6c] 301f                      move.w    (a7)+,d0
[00003b6e] 5a80                      addq.l    #5,d0
[00003b70] 0880 0000                 bclr      #0,d0
[00003b74] 2279 0000 5fce            movea.l   __break,a1
[00003b7a] 43e9 0100                 lea.l     256(a1),a1
[00003b7e] 43f1 0800                 lea.l     0(a1,d0.l),a1
[00003b82] bfc9                      cmpa.l    a1,a7
[00003b84] 6206                      bhi.s     $00003B8C
[00003b86] 4ef9 0000 012e            jmp       __sovf
[00003b8c] 9fc0                      suba.l    d0,a7
[00003b8e] 43ef 0004                 lea.l     4(a7),a1
[00003b92] 2009                      move.l    a1,d0
[00003b94] 4ed0                      jmp       (a0)
[00003b96] __strcmp:
[00003b96] 4e56 0000                 link      a6,#$0000
[00003b9a] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00003b9e] 2a6e 0008                 movea.l   8(a6),a5
[00003ba2] 286e 000c                 movea.l   12(a6),a4
[00003ba6] 6024                      bra.s     $00003BCC
[00003ba8] 101d                      move.b    (a5)+,d0
[00003baa] 4880                      ext.w     d0
[00003bac] 3e80                      move.w    d0,(a7)
[00003bae] 6130                      bsr.s     $00003BE0
[00003bb0] 1e00                      move.b    d0,d7
[00003bb2] 101c                      move.b    (a4)+,d0
[00003bb4] 4880                      ext.w     d0
[00003bb6] 3e80                      move.w    d0,(a7)
[00003bb8] 6126                      bsr.s     $00003BE0
[00003bba] 1c00                      move.b    d0,d6
[00003bbc] be06                      cmp.b     d6,d7
[00003bbe] 6f04                      ble.s     $00003BC4
[00003bc0] 7001                      moveq.l   #1,d0
[00003bc2] 6012                      bra.s     $00003BD6
[00003bc4] be06                      cmp.b     d6,d7
[00003bc6] 6c04                      bge.s     $00003BCC
[00003bc8] 70ff                      moveq.l   #-1,d0
[00003bca] 600a                      bra.s     $00003BD6
[00003bcc] 4a15                      tst.b     (a5)
[00003bce] 66d8                      bne.s     $00003BA8
[00003bd0] 4a14                      tst.b     (a4)
[00003bd2] 66d4                      bne.s     $00003BA8
[00003bd4] 4240                      clr.w     d0
[00003bd6] 4a9f                      tst.l     (a7)+
[00003bd8] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00003bdc] 4e5e                      unlk      a6
[00003bde] 4e75                      rts       
[00003be0] 4e56 0000                 link      a6,#$0000
[00003be4] 48e7 0300                 movem.l   d6-d7,-(a7)
[00003be8] 3e2e 0008                 move.w    8(a6),d7
[00003bec] be7c 0061                 cmp.w     #$0061,d7
[00003bf0] 6d0a                      blt.s     $00003BFC
[00003bf2] be7c 007a                 cmp.w     #$007A,d7
[00003bf6] 6e04                      bgt.s     $00003BFC
[00003bf8] de7c ffe0                 add.w     #$FFE0,d7
[00003bfc] 3007                      move.w    d7,d0
[00003bfe] 4a9f                      tst.l     (a7)+
[00003c00] 4cdf 0080                 movem.l   (a7)+,d7
[00003c04] 4e5e                      unlk      a6
[00003c06] 4e75                      rts       
[00003c08] ____atab:
[00003c08] 4e56 fffc                 link      a6,#$FFFC
[00003c0c] 4e5e                      unlk      a6
[00003c0e] 4e75                      rts       
[00003c10] _strcpy:
[00003c10] 4e56 0000                 link      a6,#$0000
[00003c14] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00003c18] 2a6e 000c                 movea.l   12(a6),a5
[00003c1c] 286e 0008                 movea.l   8(a6),a4
[00003c20] 18dd                      move.b    (a5)+,(a4)+
[00003c22] 66fc                      bne.s     $00003C20
[00003c24] 202e 0008                 move.l    8(a6),d0
[00003c28] 4a9f                      tst.l     (a7)+
[00003c2a] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00003c2e] 4e5e                      unlk      a6
[00003c30] 4e75                      rts       
[00003c32] _strncpy:
[00003c32] 4e56 0000                 link      a6,#$0000
[00003c36] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00003c3a] 2a6e 000c                 movea.l   12(a6),a5
[00003c3e] 286e 0008                 movea.l   8(a6),a4
[00003c42] 6004                      bra.s     $00003C48
[00003c44] 528c                      addq.l    #1,a4
[00003c46] 528d                      addq.l    #1,a5
[00003c48] 302e 0010                 move.w    16(a6),d0
[00003c4c] 536e 0010                 subq.w    #1,16(a6)
[00003c50] 4a40                      tst.w     d0
[00003c52] 6f04                      ble.s     $00003C58
[00003c54] 1895                      move.b    (a5),(a4)
[00003c56] 66ec                      bne.s     $00003C44
[00003c58] 526e 0010                 addq.w    #1,16(a6)
[00003c5c] 6002                      bra.s     $00003C60
[00003c5e] 421c                      clr.b     (a4)+
[00003c60] 302e 0010                 move.w    16(a6),d0
[00003c64] 536e 0010                 subq.w    #1,16(a6)
[00003c68] 4a40                      tst.w     d0
[00003c6a] 66f2                      bne.s     $00003C5E
[00003c6c] 202e 0008                 move.l    8(a6),d0
[00003c70] 4a9f                      tst.l     (a7)+
[00003c72] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00003c76] 4e5e                      unlk      a6
[00003c78] 4e75                      rts       
[00003c7a] _strcat:
[00003c7a] 4e56 0000                 link      a6,#$0000
[00003c7e] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00003c82] 2a6e 000c                 movea.l   12(a6),a5
[00003c86] 286e 0008                 movea.l   8(a6),a4
[00003c8a] 6002                      bra.s     $00003C8E
[00003c8c] 528c                      addq.l    #1,a4
[00003c8e] 4a14                      tst.b     (a4)
[00003c90] 66fa                      bne.s     $00003C8C
[00003c92] 18dd                      move.b    (a5)+,(a4)+
[00003c94] 66fc                      bne.s     $00003C92
[00003c96] 202e 0008                 move.l    8(a6),d0
[00003c9a] 4a9f                      tst.l     (a7)+
[00003c9c] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00003ca0] 4e5e                      unlk      a6
[00003ca2] 4e75                      rts       
[00003ca4] ___open:
[00003ca4] 4e56 fffa                 link      a6,#$FFFA
[00003ca8] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[00003cac] 2a6e 000a                 movea.l   10(a6),a5
[00003cb0] 302e 0008                 move.w    8(a6),d0
[00003cb4] c1fc 0238                 muls.w    #$0238,d0
[00003cb8] d0bc 0000 618e            add.l     #__fds,d0
[00003cbe] 2d40 fffc                 move.l    d0,-4(a6)
[00003cc2] 7eff                      moveq.l   #-1,d7
[00003cc4] 2e8d                      move.l    a5,(a7)
[00003cc6] 6100 00ec                 bsr       _ucase
[00003cca] 264d                      movea.l   a5,a3
[00003ccc] 6002                      bra.s     $00003CD0
[00003cce] 528b                      addq.l    #1,a3
[00003cd0] 4a13                      tst.b     (a3)
[00003cd2] 6706                      beq.s     $00003CDA
[00003cd4] 0c13 003a                 cmpi.b    #$3A,(a3)
[00003cd8] 66f4                      bne.s     $00003CCE
[00003cda] 4a13                      tst.b     (a3)
[00003cdc] 671c                      beq.s     $00003CFA
[00003cde] 1d63 fffa                 move.b    -(a3),-6(a6)
[00003ce2] 0c2e 0041 fffa            cmpi.b    #$41,-6(a6)
[00003ce8] 6d0c                      blt.s     $00003CF6
[00003cea] 0c2e 004f fffa            cmpi.b    #$4F,-6(a6)
[00003cf0] 6e04                      bgt.s     $00003CF6
[00003cf2] 2a4b                      movea.l   a3,a5
[00003cf4] 6004                      bra.s     $00003CFA
[00003cf6] 4beb 0002                 lea.l     2(a3),a5
[00003cfa] 302e 000e                 move.w    14(a6),d0
[00003cfe] 6000 0090                 bra       $00003D90
[00003d02] 4257                      clr.w     (a7)
[00003d04] 2f0d                      move.l    a5,-(a7)
[00003d06] 3f3c 003c                 move.w    #$003C,-(a7)
[00003d0a] 4eb9 0000 3a64            jsr       _trap
[00003d10] 5c8f                      addq.l    #6,a7
[00003d12] 3e00                      move.w    d0,d7
[00003d14] 6f0a                      ble.s     $00003D20
[00003d16] 206e fffc                 movea.l   -4(a6),a0
[00003d1a] 3147 0004                 move.w    d7,4(a0)
[00003d1e] 4247                      clr.w     d7
[00003d20] 6000 0086                 bra       $00003DA8
[00003d24] 4257                      clr.w     (a7)
[00003d26] 2f0d                      move.l    a5,-(a7)
[00003d28] 3f3c 004e                 move.w    #$004E,-(a7)
[00003d2c] 4eb9 0000 3a64            jsr       _trap
[00003d32] 5c8f                      addq.l    #6,a7
[00003d34] 4a80                      tst.l     d0
[00003d36] 6604                      bne.s     $00003D3C
[00003d38] 4240                      clr.w     d0
[00003d3a] 6004                      bra.s     $00003D40
[00003d3c] 303c 00ff                 move.w    #$00FF,d0
[00003d40] 3e00                      move.w    d0,d7
[00003d42] 6064                      bra.s     $00003DA8
[00003d44] 3ebc 004f                 move.w    #$004F,(a7)
[00003d48] 4eb9 0000 3a64            jsr       _trap
[00003d4e] 4a80                      tst.l     d0
[00003d50] 6604                      bne.s     $00003D56
[00003d52] 4240                      clr.w     d0
[00003d54] 6004                      bra.s     $00003D5A
[00003d56] 303c 00ff                 move.w    #$00FF,d0
[00003d5a] 3e00                      move.w    d0,d7
[00003d5c] 604a                      bra.s     $00003DA8
[00003d5e] 2e8d                      move.l    a5,(a7)
[00003d60] 3f3c 003d                 move.w    #$003D,-(a7)
[00003d64] 4eb9 0000 3a64            jsr       _trap
[00003d6a] 548f                      addq.l    #2,a7
[00003d6c] 3e00                      move.w    d0,d7
[00003d6e] 6f0a                      ble.s     $00003D7A
[00003d70] 206e fffc                 movea.l   -4(a6),a0
[00003d74] 3147 0004                 move.w    d7,4(a0)
[00003d78] 4247                      clr.w     d7
[00003d7a] 602c                      bra.s     $00003DA8
[00003d7c] 2e8d                      move.l    a5,(a7)
[00003d7e] 3f3c 0041                 move.w    #$0041,-(a7)
[00003d82] 4eb9 0000 3a64            jsr       _trap
[00003d88] 548f                      addq.l    #2,a7
[00003d8a] 3e00                      move.w    d0,d7
[00003d8c] 601a                      bra.s     $00003DA8
[00003d8e] 6018                      bra.s     $00003DA8
[00003d90] 907c 000f                 sub.w     #$000F,d0
[00003d94] b07c 0007                 cmp.w     #$0007,d0
[00003d98] 620e                      bhi.s     $00003DA8
[00003d9a] e540                      asl.w     #2,d0
[00003d9c] 3040                      movea.w   d0,a0
[00003d9e] d1fc 0000 5da6            adda.l    #$00005DA6,a0
[00003da4] 2050                      movea.l   (a0),a0
[00003da6] 4ed0                      jmp       (a0)
[00003da8] 3007                      move.w    d7,d0
[00003daa] 4a9f                      tst.l     (a7)+
[00003dac] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[00003db0] 4e5e                      unlk      a6
[00003db2] 4e75                      rts       
[00003db4] _ucase:
[00003db4] 4e56 fffc                 link      a6,#$FFFC
[00003db8] 206e 0008                 movea.l   8(a6),a0
[00003dbc] 0c10 0061                 cmpi.b    #$61,(a0)
[00003dc0] 6d12                      blt.s     $00003DD4
[00003dc2] 206e 0008                 movea.l   8(a6),a0
[00003dc6] 0c10 007a                 cmpi.b    #$7A,(a0)
[00003dca] 6e08                      bgt.s     $00003DD4
[00003dcc] 206e 0008                 movea.l   8(a6),a0
[00003dd0] 0410 0020                 subi.b    #$20,(a0)
[00003dd4] 206e 0008                 movea.l   8(a6),a0
[00003dd8] 1010                      move.b    (a0),d0
[00003dda] 4880                      ext.w     d0
[00003ddc] 52ae 0008                 addq.l    #1,8(a6)
[00003de0] 4a00                      tst.b     d0
[00003de2] 66d4                      bne.s     $00003DB8
[00003de4] 4e5e                      unlk      a6
[00003de6] 4e75                      rts       
[00003de8] __ttyout:
[00003de8] 4e56 fffa                 link      a6,#$FFFA
[00003dec] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00003df0] 2a6e 0008                 movea.l   8(a6),a5
[00003df4] 101d                      move.b    (a5)+,d0
[00003df6] 4880                      ext.w     d0
[00003df8] 3d40 fffe                 move.w    d0,-2(a6)
[00003dfc] 204d                      movea.l   a5,a0
[00003dfe] 326e fffe                 movea.w   -2(a6),a1
[00003e02] d1c9                      adda.l    a1,a0
[00003e04] 10bc 0024                 move.b    #$24,(a0)
[00003e08] 4247                      clr.w     d7
[00003e0a] 2d4d fffa                 move.l    a5,-6(a6)
[00003e0e] 6048                      bra.s     $00003E58
[00003e10] 204d                      movea.l   a5,a0
[00003e12] 3247                      movea.w   d7,a1
[00003e14] d1c9                      adda.l    a1,a0
[00003e16] 0c10 0024                 cmpi.b    #$24,(a0)
[00003e1a] 663a                      bne.s     $00003E56
[00003e1c] 206e fffa                 movea.l   -6(a6),a0
[00003e20] 0c10 0024                 cmpi.b    #$24,(a0)
[00003e24] 6710                      beq.s     $00003E36
[00003e26] 2eae fffa                 move.l    -6(a6),(a7)
[00003e2a] 3f3c 0009                 move.w    #$0009,-(a7)
[00003e2e] 4eb9 0000 00da            jsr       ___BDOS
[00003e34] 548f                      addq.l    #2,a7
[00003e36] 2ebc 0000 0024            move.l    #$00000024,(a7)
[00003e3c] 3f3c 0002                 move.w    #$0002,-(a7)
[00003e40] 4eb9 0000 00da            jsr       ___BDOS
[00003e46] 548f                      addq.l    #2,a7
[00003e48] 200d                      move.l    a5,d0
[00003e4a] 3207                      move.w    d7,d1
[00003e4c] 48c1                      ext.l     d1
[00003e4e] d081                      add.l     d1,d0
[00003e50] 5280                      addq.l    #1,d0
[00003e52] 2d40 fffa                 move.l    d0,-6(a6)
[00003e56] 5247                      addq.w    #1,d7
[00003e58] be6e fffe                 cmp.w     -2(a6),d7
[00003e5c] 6db2                      blt.s     $00003E10
[00003e5e] 200d                      move.l    a5,d0
[00003e60] 3207                      move.w    d7,d1
[00003e62] 48c1                      ext.l     d1
[00003e64] d081                      add.l     d1,d0
[00003e66] b0ae fffa                 cmp.l     -6(a6),d0
[00003e6a] 6710                      beq.s     $00003E7C
[00003e6c] 2eae fffa                 move.l    -6(a6),(a7)
[00003e70] 3f3c 0009                 move.w    #$0009,-(a7)
[00003e74] 4eb9 0000 00da            jsr       ___BDOS
[00003e7a] 548f                      addq.l    #2,a7
[00003e7c] 302e fffe                 move.w    -2(a6),d0
[00003e80] 4a9f                      tst.l     (a7)+
[00003e82] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00003e86] 4e5e                      unlk      a6
[00003e88] 4e75                      rts       
[00003e8a] __lstout:
[00003e8a] 4e56 fffe                 link      a6,#$FFFE
[00003e8e] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00003e92] 2a6e 0008                 movea.l   8(a6),a5
[00003e96] 1e1d                      move.b    (a5)+,d7
[00003e98] 4887                      ext.w     d7
[00003e9a] ce7c 00ff                 and.w     #$00FF,d7
[00003e9e] 3d47 fffe                 move.w    d7,-2(a6)
[00003ea2] 6010                      bra.s     $00003EB4
[00003ea4] 101d                      move.b    (a5)+,d0
[00003ea6] 4880                      ext.w     d0
[00003ea8] 3e80                      move.w    d0,(a7)
[00003eaa] 4267                      clr.w     -(a7)
[00003eac] 3f3c 0003                 move.w    #$0003,-(a7)
[00003eb0] 611c                      bsr.s     $00003ECE
[00003eb2] 588f                      addq.l    #4,a7
[00003eb4] 3007                      move.w    d7,d0
[00003eb6] 5347                      subq.w    #1,d7
[00003eb8] 4a40                      tst.w     d0
[00003eba] 6ee8                      bgt.s     $00003EA4
[00003ebc] 302e fffe                 move.w    -2(a6),d0
[00003ec0] 4a9f                      tst.l     (a7)+
[00003ec2] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00003ec6] 4e5e                      unlk      a6
[00003ec8] 4e75                      rts       
[00003eca] _j30june:
[00003eca] 4e56 fffc                 link      a6,#$FFFC
[00003ece] 23df 0000 6178            move.l    (a7)+,_xyzzx
[00003ed4] 4e4d                      trap      #13
[00003ed6] 2f39 0000 6178            move.l    _xyzzx,-(a7)
[00003edc] 4e75                      rts       
[00003ede] 4e5e                      unlk      a6
[00003ee0] 4e75                      rts       
[00003ee2] _wrtchr:
[00003ee2] 4e56 fde8                 link      a6,#$FDE8
[00003ee6] 3d7c 0001 fdf2            move.w    #$0001,-526(a6)
[00003eec] 426e fdf0                 clr.w     -528(a6)
[00003ef0] 206e 0008                 movea.l   8(a6),a0
[00003ef4] 0828 0001 0001            btst      #1,1(a0)
[00003efa] 670a                      beq.s     $00003F06
[00003efc] 2d7c 0000 3de8 fdf4       move.l    #__ttyout,-524(a6)
[00003f04] 6014                      bra.s     $00003F1A
[00003f06] 206e 0008                 movea.l   8(a6),a0
[00003f0a] 0828 0002 0001            btst      #2,1(a0)
[00003f10] 6708                      beq.s     $00003F1A
[00003f12] 2d7c 0000 3e8a fdf4       move.l    #__lstout,-524(a6)
[00003f1a] 206e 0008                 movea.l   8(a6),a0
[00003f1e] 0828 0007 0001            btst      #7,1(a0)
[00003f24] 671e                      beq.s     $00003F44
[00003f26] 202e 0008                 move.l    8(a6),d0
[00003f2a] d0bc 0000 0012            add.l     #$00000012,d0
[00003f30] 2d40 fdec                 move.l    d0,-532(a6)
[00003f34] 206e fdec                 movea.l   -532(a6),a0
[00003f38] 3028 0004                 move.w    4(a0),d0
[00003f3c] c07c 0c00                 and.w     #$0C00,d0
[00003f40] 3d40 fdf0                 move.w    d0,-528(a6)
[00003f44] 426e fdfa                 clr.w     -518(a6)
[00003f48] 3d6e 0010 fffe            move.w    16(a6),-2(a6)
[00003f4e] 6000 00ea                 bra       $0000403A
[00003f52] 3d7c 0001 fffc            move.w    #$0001,-4(a6)
[00003f58] 6000 00b6                 bra       $00004010
[00003f5c] 206e 000c                 movea.l   12(a6),a0
[00003f60] 0c10 000a                 cmpi.b    #$0A,(a0)
[00003f64] 662e                      bne.s     $00003F94
[00003f66] 3d7c ffff fdfa            move.w    #$FFFF,-518(a6)
[00003f6c] 4a6e fdf2                 tst.w     -526(a6)
[00003f70] 6720                      beq.s     $00003F92
[00003f72] 302e fffc                 move.w    -4(a6),d0
[00003f76] 5240                      addq.w    #1,d0
[00003f78] b07c 0200                 cmp.w     #$0200,d0
[00003f7c] 6c00 00a2                 bge       $00004020
[00003f80] 204e                      movea.l   a6,a0
[00003f82] 326e fffc                 movea.w   -4(a6),a1
[00003f86] d1c9                      adda.l    a1,a0
[00003f88] 117c 000d fdfc            move.b    #$0D,-516(a0)
[00003f8e] 526e fffc                 addq.w    #1,-4(a6)
[00003f92] 605c                      bra.s     $00003FF0
[00003f94] 206e 000c                 movea.l   12(a6),a0
[00003f98] 0c10 0009                 cmpi.b    #$09,(a0)
[00003f9c] 6652                      bne.s     $00003FF0
[00003f9e] 4a6e fdf0                 tst.w     -528(a6)
[00003fa2] 674c                      beq.s     $00003FF0
[00003fa4] 7008                      moveq.l   #8,d0
[00003fa6] 322e fdfa                 move.w    -518(a6),d1
[00003faa] c27c 0007                 and.w     #$0007,d1
[00003fae] 9041                      sub.w     d1,d0
[00003fb0] 3d40 fdf8                 move.w    d0,-520(a6)
[00003fb4] 302e fffc                 move.w    -4(a6),d0
[00003fb8] d06e fdf8                 add.w     -520(a6),d0
[00003fbc] b07c 0200                 cmp.w     #$0200,d0
[00003fc0] 6c5e                      bge.s     $00004020
[00003fc2] 302e fdf8                 move.w    -520(a6),d0
[00003fc6] d16e fdfa                 add.w     d0,-518(a6)
[00003fca] 6012                      bra.s     $00003FDE
[00003fcc] 204e                      movea.l   a6,a0
[00003fce] 326e fffc                 movea.w   -4(a6),a1
[00003fd2] d1c9                      adda.l    a1,a0
[00003fd4] 117c 0020 fdfc            move.b    #$20,-516(a0)
[00003fda] 526e fffc                 addq.w    #1,-4(a6)
[00003fde] 302e fdf8                 move.w    -520(a6),d0
[00003fe2] 536e fdf8                 subq.w    #1,-520(a6)
[00003fe6] 4a40                      tst.w     d0
[00003fe8] 66e2                      bne.s     $00003FCC
[00003fea] 52ae 000c                 addq.l    #1,12(a6)
[00003fee] 6020                      bra.s     $00004010
[00003ff0] 204e                      movea.l   a6,a0
[00003ff2] 326e fffc                 movea.w   -4(a6),a1
[00003ff6] d1c9                      adda.l    a1,a0
[00003ff8] 226e 000c                 movea.l   12(a6),a1
[00003ffc] 1151 fdfc                 move.b    (a1),-516(a0)
[00004000] 52ae 000c                 addq.l    #1,12(a6)
[00004004] 526e fffc                 addq.w    #1,-4(a6)
[00004008] 536e fffe                 subq.w    #1,-2(a6)
[0000400c] 526e fdfa                 addq.w    #1,-518(a6)
[00004010] 0c6e 0200 fffc            cmpi.w    #$0200,-4(a6)
[00004016] 6c08                      bge.s     $00004020
[00004018] 4a6e fffe                 tst.w     -2(a6)
[0000401c] 6600 ff3e                 bne       $00003F5C
[00004020] 302e fffc                 move.w    -4(a6),d0
[00004024] 5340                      subq.w    #1,d0
[00004026] 1d40 fdfc                 move.b    d0,-516(a6)
[0000402a] 2f0e                      move.l    a6,-(a7)
[0000402c] 0697 ffff fdfc            addi.l    #$FFFFFDFC,(a7)
[00004032] 206e fdf4                 movea.l   -524(a6),a0
[00004036] 4e90                      jsr       (a0)
[00004038] 588f                      addq.l    #4,a7
[0000403a] 4a6e fffe                 tst.w     -2(a6)
[0000403e] 6600 ff12                 bne       $00003F52
[00004042] 4240                      clr.w     d0
[00004044] 302e 0010                 move.w    16(a6),d0
[00004048] 4e5e                      unlk      a6
[0000404a] 4e75                      rts       
[0000404c] __wrtbin:
[0000404c] 4e56 fffe                 link      a6,#$FFFE
[00004050] 48e7 0104                 movem.l   d7/a5,-(a7)
[00004054] 2a6e 0008                 movea.l   8(a6),a5
[00004058] 3ebc 0001                 move.w    #$0001,(a7)
[0000405c] 3f2e 0010                 move.w    16(a6),-(a7)
[00004060] 2f2e 000c                 move.l    12(a6),-(a7)
[00004064] 2f2d 0006                 move.l    6(a5),-(a7)
[00004068] 2f0d                      move.l    a5,-(a7)
[0000406a] 0697 0000 0012            addi.l    #$00000012,(a7)
[00004070] 4eb9 0000 3ab0            jsr       __pc_wri
[00004076] dffc 0000 000e            adda.l    #$0000000E,a7
[0000407c] 3d40 fffe                 move.w    d0,-2(a6)
[00004080] 4a6e fffe                 tst.w     -2(a6)
[00004084] 6616                      bne.s     $0000409C
[00004086] 33fc 0005 0000 5da2       move.w    #$0005,_errno
[0000408e] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[00004098] 70ff                      moveq.l   #-1,d0
[0000409a] 6020                      bra.s     $000040BC
[0000409c] 4280                      clr.l     d0
[0000409e] 302e fffe                 move.w    -2(a6),d0
[000040a2] d1ad 0006                 add.l     d0,6(a5)
[000040a6] 202d 0006                 move.l    6(a5),d0
[000040aa] b0ad 000e                 cmp.l     14(a5),d0
[000040ae] 6f06                      ble.s     $000040B6
[000040b0] 2b6d 0006 000e            move.l    6(a5),14(a5)
[000040b6] 4240                      clr.w     d0
[000040b8] 302e fffe                 move.w    -2(a6),d0
[000040bc] 4a9f                      tst.l     (a7)+
[000040be] 4cdf 2000                 movem.l   (a7)+,a5
[000040c2] 4e5e                      unlk      a6
[000040c4] 4e75                      rts       
[000040c6] __wrtasc:
[000040c6] 4e56 fffa                 link      a6,#$FFFA
[000040ca] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000040ce] 2a6e 0008                 movea.l   8(a6),a5
[000040d2] 286e 000c                 movea.l   12(a6),a4
[000040d6] 3e2e 0010                 move.w    16(a6),d7
[000040da] 4240                      clr.w     d0
[000040dc] 3d40 fffa                 move.w    d0,-6(a6)
[000040e0] 3d40 fffc                 move.w    d0,-4(a6)
[000040e4] 6000 00c8                 bra       $000041AE
[000040e8] 6004                      bra.s     $000040EE
[000040ea] 526e fffa                 addq.w    #1,-6(a6)
[000040ee] be6e fffa                 cmp.w     -6(a6),d7
[000040f2] 6310                      bls.s     $00004104
[000040f4] 204c                      movea.l   a4,a0
[000040f6] 93c9                      suba.l    a1,a1
[000040f8] 326e fffa                 movea.w   -6(a6),a1
[000040fc] d1c9                      adda.l    a1,a0
[000040fe] 0c10 000a                 cmpi.b    #$0A,(a0)
[00004102] 66e6                      bne.s     $000040EA
[00004104] 4240                      clr.w     d0
[00004106] 302e fffa                 move.w    -6(a6),d0
[0000410a] 906e fffc                 sub.w     -4(a6),d0
[0000410e] 6366                      bls.s     $00004176
[00004110] 3ebc 0001                 move.w    #$0001,(a7)
[00004114] 3f2e fffa                 move.w    -6(a6),-(a7)
[00004118] 4240                      clr.w     d0
[0000411a] 302e fffc                 move.w    -4(a6),d0
[0000411e] 9157                      sub.w     d0,(a7)
[00004120] 200c                      move.l    a4,d0
[00004122] 4281                      clr.l     d1
[00004124] 322e fffc                 move.w    -4(a6),d1
[00004128] d081                      add.l     d1,d0
[0000412a] 2f00                      move.l    d0,-(a7)
[0000412c] 2f2d 0006                 move.l    6(a5),-(a7)
[00004130] 2f0d                      move.l    a5,-(a7)
[00004132] 0697 0000 0012            addi.l    #$00000012,(a7)
[00004138] 4eb9 0000 3ab0            jsr       __pc_wri
[0000413e] dffc 0000 000e            adda.l    #$0000000E,a7
[00004144] 3d40 fffe                 move.w    d0,-2(a6)
[00004148] 4280                      clr.l     d0
[0000414a] 302e fffe                 move.w    -2(a6),d0
[0000414e] d1ad 0006                 add.l     d0,6(a5)
[00004152] 4a6e fffe                 tst.w     -2(a6)
[00004156] 6616                      bne.s     $0000416E
[00004158] 33fc 0005 0000 5da2       move.w    #$0005,_errno
[00004160] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[0000416a] 70ff                      moveq.l   #-1,d0
[0000416c] 605e                      bra.s     $000041CC
[0000416e] 3d6e fffa fffc            move.w    -6(a6),-4(a6)
[00004174] 6038                      bra.s     $000041AE
[00004176] 526e fffa                 addq.w    #1,-6(a6)
[0000417a] 3ebc 0001                 move.w    #$0001,(a7)
[0000417e] 3f3c 0001                 move.w    #$0001,-(a7)
[00004182] 2f3c 0000 5dc6            move.l    #$00005DC6,-(a7)
[00004188] 2f2d 0006                 move.l    6(a5),-(a7)
[0000418c] 2f0d                      move.l    a5,-(a7)
[0000418e] 0697 0000 0012            addi.l    #$00000012,(a7)
[00004194] 4eb9 0000 3ab0            jsr       __pc_wri
[0000419a] dffc 0000 000e            adda.l    #$0000000E,a7
[000041a0] 3d40 fffe                 move.w    d0,-2(a6)
[000041a4] 4280                      clr.l     d0
[000041a6] 302e fffe                 move.w    -2(a6),d0
[000041aa] d1ad 0006                 add.l     d0,6(a5)
[000041ae] be6e fffc                 cmp.w     -4(a6),d7
[000041b2] 6200 ff34                 bhi       $000040E8
[000041b6] 202d 0006                 move.l    6(a5),d0
[000041ba] b0ad 000e                 cmp.l     14(a5),d0
[000041be] 6f06                      ble.s     $000041C6
[000041c0] 2b6d 0006 000e            move.l    6(a5),14(a5)
[000041c6] 4240                      clr.w     d0
[000041c8] 302e fffc                 move.w    -4(a6),d0
[000041cc] 4a9f                      tst.l     (a7)+
[000041ce] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000041d2] 4e5e                      unlk      a6
[000041d4] 4e75                      rts       
[000041d6] __allocc:
[000041d6] 4e56 0000                 link      a6,#$0000
[000041da] 48e7 0700                 movem.l   d5-d7,-(a7)
[000041de] 7c01                      moveq.l   #1,d6
[000041e0] 4247                      clr.w     d7
[000041e2] 6018                      bra.s     $000041FC
[000041e4] 2006                      move.l    d6,d0
[000041e6] c0b9 0000 5dc8            and.l     __chvec,d0
[000041ec] 660a                      bne.s     $000041F8
[000041ee] 8db9 0000 5dc8            or.l      d6,__chvec
[000041f4] 3007                      move.w    d7,d0
[000041f6] 601e                      bra.s     $00004216
[000041f8] e386                      asl.l     #1,d6
[000041fa] 5247                      addq.w    #1,d7
[000041fc] be7c 0010                 cmp.w     #$0010,d7
[00004200] 6de2                      blt.s     $000041E4
[00004202] 33fc 0018 0000 5da2       move.w    #$0018,_errno
[0000420a] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[00004214] 70ff                      moveq.l   #-1,d0
[00004216] 4a9f                      tst.l     (a7)+
[00004218] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[0000421c] 4e5e                      unlk      a6
[0000421e] 4e75                      rts       
[00004220] __freec:
[00004220] 4e56 fffc                 link      a6,#$FFFC
[00004224] 7001                      moveq.l   #1,d0
[00004226] 322e 0008                 move.w    8(a6),d1
[0000422a] e360                      asl.w     d1,d0
[0000422c] 4640                      not.w     d0
[0000422e] 48c0                      ext.l     d0
[00004230] c1b9 0000 5dc8            and.l     d0,__chvec
[00004236] 4240                      clr.w     d0
[00004238] 4e5e                      unlk      a6
[0000423a] 4e75                      rts       
[0000423c] __chinit:
[0000423c] 4e56 0000                 link      a6,#$0000
[00004240] 48e7 0300                 movem.l   d6-d7,-(a7)
[00004244] 4247                      clr.w     d7
[00004246] 6006                      bra.s     $0000424E
[00004248] 3e87                      move.w    d7,(a7)
[0000424a] 6112                      bsr.s     ___chini
[0000424c] 5247                      addq.w    #1,d7
[0000424e] be7c 0010                 cmp.w     #$0010,d7
[00004252] 6df4                      blt.s     $00004248
[00004254] 4a9f                      tst.l     (a7)+
[00004256] 4cdf 0080                 movem.l   (a7)+,d7
[0000425a] 4e5e                      unlk      a6
[0000425c] 4e75                      rts       
[0000425e] ___chini:
[0000425e] 4e56 0000                 link      a6,#$0000
[00004262] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00004266] 302e 0008                 move.w    8(a6),d0
[0000426a] c1fc 0238                 muls.w    #$0238,d0
[0000426e] 2a40                      movea.l   d0,a5
[00004270] dbfc 0000 618e            adda.l    #__fds,a5
[00004276] 302e 0008                 move.w    8(a6),d0
[0000427a] 1b40 0003                 move.b    d0,3(a5)
[0000427e] 4255                      clr.w     (a5)
[00004280] 422d 0002                 clr.b     2(a5)
[00004284] 2b7c ffff ffff 000a       move.l    #$FFFFFFFF,10(a5)
[0000428c] 42ad 0006                 clr.l     6(a5)
[00004290] 42ad 000e                 clr.l     14(a5)
[00004294] 3ebc 0020                 move.w    #$0020,(a7)
[00004298] 4267                      clr.w     -(a7)
[0000429a] 2f0d                      move.l    a5,-(a7)
[0000429c] 0697 0000 0012            addi.l    #$00000012,(a7)
[000042a2] 4eb9 0000 0144            jsr       _blkfill
[000042a8] 5c8f                      addq.l    #6,a7
[000042aa] 3ebc 000b                 move.w    #$000B,(a7)
[000042ae] 3f3c 0020                 move.w    #$0020,-(a7)
[000042b2] 2f0d                      move.l    a5,-(a7)
[000042b4] 0697 0000 0013            addi.l    #$00000013,(a7)
[000042ba] 4eb9 0000 0144            jsr       _blkfill
[000042c0] 5c8f                      addq.l    #6,a7
[000042c2] 4a9f                      tst.l     (a7)+
[000042c4] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000042c8] 4e5e                      unlk      a6
[000042ca] 4e75                      rts       
[000042cc] __chkc:
[000042cc] 4e56 0000                 link      a6,#$0000
[000042d0] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000042d4] 3e2e 0008                 move.w    8(a6),d7
[000042d8] be7c 0010                 cmp.w     #$0010,d7
[000042dc] 6516                      bcs.s     $000042F4
[000042de] 33fc 0009 0000 5da2       move.w    #$0009,_errno
[000042e6] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[000042f0] 4280                      clr.l     d0
[000042f2] 6030                      bra.s     $00004324
[000042f4] 4240                      clr.w     d0
[000042f6] 3007                      move.w    d7,d0
[000042f8] c0fc 0238                 mulu.w    #$0238,d0
[000042fc] 2a40                      movea.l   d0,a5
[000042fe] dbfc 0000 618e            adda.l    #__fds,a5
[00004304] 082d 0000 0001            btst      #0,1(a5)
[0000430a] 6616                      bne.s     $00004322
[0000430c] 33fc 0009 0000 5da2       move.w    #$0009,_errno
[00004314] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[0000431e] 4280                      clr.l     d0
[00004320] 6002                      bra.s     $00004324
[00004322] 200d                      move.l    a5,d0
[00004324] 4a9f                      tst.l     (a7)+
[00004326] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0000432a] 4e5e                      unlk      a6
[0000432c] 4e75                      rts       
[0000432e] _write:
[0000432e] 4e56 0000                 link      a6,#$0000
[00004332] 48e7 0104                 movem.l   d7/a5,-(a7)
[00004336] 3eae 0008                 move.w    8(a6),(a7)
[0000433a] 4eb9 0000 42cc            jsr       __chkc
[00004340] 2a40                      movea.l   d0,a5
[00004342] 200d                      move.l    a5,d0
[00004344] 6606                      bne.s     $0000434C
[00004346] 70ff                      moveq.l   #-1,d0
[00004348] 6000 0076                 bra       $000043C0
[0000434c] 4a6e 000e                 tst.w     14(a6)
[00004350] 6604                      bne.s     $00004356
[00004352] 4240                      clr.w     d0
[00004354] 606a                      bra.s     $000043C0
[00004356] 082d 0003 0001            btst      #3,1(a5)
[0000435c] 6716                      beq.s     $00004374
[0000435e] 33fc 0009 0000 5da2       move.w    #$0009,_errno
[00004366] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[00004370] 70ff                      moveq.l   #-1,d0
[00004372] 604c                      bra.s     $000043C0
[00004374] 3015                      move.w    (a5),d0
[00004376] c07c 0204                 and.w     #$0204,d0
[0000437a] 6714                      beq.s     $00004390
[0000437c] 3eae 000e                 move.w    14(a6),(a7)
[00004380] 2f2e 000a                 move.l    10(a6),-(a7)
[00004384] 2f0d                      move.l    a5,-(a7)
[00004386] 4eb9 0000 3ee2            jsr       __wrtchr
[0000438c] 508f                      addq.l    #8,a7
[0000438e] 6030                      bra.s     $000043C0
[00004390] 082d 0004 0001            btst      #4,1(a5)
[00004396] 6716                      beq.s     $000043AE
[00004398] 3eae 000e                 move.w    14(a6),(a7)
[0000439c] 2f2e 000a                 move.l    10(a6),-(a7)
[000043a0] 2f0d                      move.l    a5,-(a7)
[000043a2] 4eb9 0000 40c6            jsr       __wrtasc
[000043a8] 508f                      addq.l    #8,a7
[000043aa] 6014                      bra.s     $000043C0
[000043ac] 6012                      bra.s     $000043C0
[000043ae] 3eae 000e                 move.w    14(a6),(a7)
[000043b2] 2f2e 000a                 move.l    10(a6),-(a7)
[000043b6] 2f0d                      move.l    a5,-(a7)
[000043b8] 4eb9 0000 404c            jsr       __wrtbin
[000043be] 508f                      addq.l    #8,a7
[000043c0] 4a9f                      tst.l     (a7)+
[000043c2] 4cdf 2000                 movem.l   (a7)+,a5
[000043c6] 4e5e                      unlk      a6
[000043c8] 4e75                      rts       
[000043ca] _sbrk:
[000043ca] 4e56 0000                 link      a6,#$0000
[000043ce] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000043d2] 3e2e 0008                 move.w    8(a6),d7
[000043d6] 48c7                      ext.l     d7
[000043d8] cebc 0000 ffff            and.l     #$0000FFFF,d7
[000043de] 0807 0000                 btst      #0,d7
[000043e2] 6702                      beq.s     $000043E6
[000043e4] 5287                      addq.l    #1,d7
[000043e6] 2a79 0000 5fce            movea.l   __break,a5
[000043ec] 2847                      movea.l   d7,a4
[000043ee] d9f9 0000 5fce            adda.l    __break,a4
[000043f4] 2e8c                      move.l    a4,(a7)
[000043f6] 4eb9 0000 00ac            jsr       _brk
[000043fc] b07c ffff                 cmp.w     #$FFFF,d0
[00004400] 6616                      bne.s     $00004418
[00004402] 33fc 000c 0000 5da2       move.w    #$000C,_errno
[0000440a] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[00004414] 70ff                      moveq.l   #-1,d0
[00004416] 6012                      bra.s     $0000442A
[00004418] 3eae 0008                 move.w    8(a6),(a7)
[0000441c] 4267                      clr.w     -(a7)
[0000441e] 2f0d                      move.l    a5,-(a7)
[00004420] 4eb9 0000 0144            jsr       _blkfill
[00004426] 5c8f                      addq.l    #6,a7
[00004428] 200d                      move.l    a5,d0
[0000442a] 4a9f                      tst.l     (a7)+
[0000442c] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00004430] 4e5e                      unlk      a6
[00004432] 4e75                      rts       
[00004434] ___prtsh:
[00004434] 4e56 fffc                 link      a6,#$FFFC
[00004438] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[0000443c] 206e 0008                 movea.l   8(a6),a0
[00004440] 3010                      move.w    (a0),d0
[00004442] 48c0                      ext.l     d0
[00004444] 2d40 fffc                 move.l    d0,-4(a6)
[00004448] 202e fffc                 move.l    -4(a6),d0
[0000444c] c0bc 0000 ffff            and.l     #$0000FFFF,d0
[00004452] 3e00                      move.w    d0,d7
[00004454] 2a6e 0014                 movea.l   20(a6),a5
[00004458] 3c2e 0010                 move.w    16(a6),d6
[0000445c] 4a6e 0012                 tst.w     18(a6)
[00004460] 671c                      beq.s     $0000447E
[00004462] 4a47                      tst.w     d7
[00004464] 6c18                      bge.s     $0000447E
[00004466] 4240                      clr.w     d0
[00004468] 3007                      move.w    d7,d0
[0000446a] 4440                      neg.w     d0
[0000446c] 3e00                      move.w    d0,d7
[0000446e] 206e 000c                 movea.l   12(a6),a0
[00004472] 2050                      movea.l   (a0),a0
[00004474] 10bc 002d                 move.b    #$2D,(a0)
[00004478] 226e 000c                 movea.l   12(a6),a1
[0000447c] 5291                      addq.l    #1,(a1)
[0000447e] 6012                      bra.s     $00004492
[00004480] 4280                      clr.l     d0
[00004482] 3007                      move.w    d7,d0
[00004484] 80c6                      divu.w    d6,d0
[00004486] 4840                      swap      d0
[00004488] 1ac0                      move.b    d0,(a5)+
[0000448a] 4280                      clr.l     d0
[0000448c] 3007                      move.w    d7,d0
[0000448e] 80c6                      divu.w    d6,d0
[00004490] 3e00                      move.w    d0,d7
[00004492] 4a47                      tst.w     d7
[00004494] 66ea                      bne.s     $00004480
[00004496] 200d                      move.l    a5,d0
[00004498] 4a9f                      tst.l     (a7)+
[0000449a] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[0000449e] 4e5e                      unlk      a6
[000044a0] 4e75                      rts       
[000044a2] ___prtld:
[000044a2] 4e56 fffc                 link      a6,#$FFFC
[000044a6] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[000044aa] 2d7c 0000 5d1e fffc       move.l    #_uldivr,-4(a6)
[000044b2] 2a6e 0014                 movea.l   20(a6),a5
[000044b6] 3c2e 0010                 move.w    16(a6),d6
[000044ba] 48c6                      ext.l     d6
[000044bc] 206e 0008                 movea.l   8(a6),a0
[000044c0] 2e10                      move.l    (a0),d7
[000044c2] 0c6e 0010 0010            cmpi.w    #$0010,16(a6)
[000044c8] 661a                      bne.s     $000044E4
[000044ca] 7a08                      moveq.l   #8,d5
[000044cc] 600c                      bra.s     $000044DA
[000044ce] 2007                      move.l    d7,d0
[000044d0] c07c 000f                 and.w     #$000F,d0
[000044d4] 1ac0                      move.b    d0,(a5)+
[000044d6] e887                      asr.l     #4,d7
[000044d8] 5345                      subq.w    #1,d5
[000044da] 4a87                      tst.l     d7
[000044dc] 6704                      beq.s     $000044E2
[000044de] 4a45                      tst.w     d5
[000044e0] 66ec                      bne.s     $000044CE
[000044e2] 6068                      bra.s     $0000454C
[000044e4] 0c6e 0008 0010            cmpi.w    #$0008,16(a6)
[000044ea] 6624                      bne.s     $00004510
[000044ec] 7a0b                      moveq.l   #11,d5
[000044ee] 600c                      bra.s     $000044FC
[000044f0] 2007                      move.l    d7,d0
[000044f2] c07c 0007                 and.w     #$0007,d0
[000044f6] 1ac0                      move.b    d0,(a5)+
[000044f8] e687                      asr.l     #3,d7
[000044fa] 5345                      subq.w    #1,d5
[000044fc] 4a87                      tst.l     d7
[000044fe] 6704                      beq.s     $00004504
[00004500] 4a45                      tst.w     d5
[00004502] 66ec                      bne.s     $000044F0
[00004504] 4a45                      tst.w     d5
[00004506] 6606                      bne.s     $0000450E
[00004508] 022d 0003 ffff            andi.b    #$03,-1(a5)
[0000450e] 603c                      bra.s     $0000454C
[00004510] 4a6e 0012                 tst.w     18(a6)
[00004514] 671a                      beq.s     $00004530
[00004516] 4a87                      tst.l     d7
[00004518] 6c16                      bge.s     $00004530
[0000451a] 206e 000c                 movea.l   12(a6),a0
[0000451e] 2050                      movea.l   (a0),a0
[00004520] 10bc 002d                 move.b    #$2D,(a0)
[00004524] 226e 000c                 movea.l   12(a6),a1
[00004528] 5291                      addq.l    #1,(a1)
[0000452a] 2007                      move.l    d7,d0
[0000452c] 4480                      neg.l     d0
[0000452e] 2e00                      move.l    d0,d7
[00004530] 6016                      bra.s     $00004548
[00004532] 2e86                      move.l    d6,(a7)
[00004534] 2f07                      move.l    d7,-(a7)
[00004536] 4eb9 0000 3aec            jsr       _uldiv
[0000453c] 588f                      addq.l    #4,a7
[0000453e] 2e00                      move.l    d0,d7
[00004540] 206e fffc                 movea.l   -4(a6),a0
[00004544] 1ae8 0003                 move.b    3(a0),(a5)+
[00004548] 4a87                      tst.l     d7
[0000454a] 66e6                      bne.s     $00004532
[0000454c] 200d                      move.l    a5,d0
[0000454e] 4a9f                      tst.l     (a7)+
[00004550] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00004554] 4e5e                      unlk      a6
[00004556] 4e75                      rts       
[00004558] ___prtin:
[00004558] 4e56 fff0                 link      a6,#$FFF0
[0000455c] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00004560] 2f0e                      move.l    a6,-(a7)
[00004562] 0697 ffff fff0            addi.l    #$FFFFFFF0,(a7)
[00004568] 3f2e 0012                 move.w    18(a6),-(a7)
[0000456c] 3f2e 0010                 move.w    16(a6),-(a7)
[00004570] 2f0e                      move.l    a6,-(a7)
[00004572] 0697 0000 000c            addi.l    #$0000000C,(a7)
[00004578] 2f2e 0008                 move.l    8(a6),-(a7)
[0000457c] 206e 0014                 movea.l   20(a6),a0
[00004580] 4e90                      jsr       (a0)
[00004582] dffc 0000 0010            adda.l    #$00000010,a7
[00004588] 2a40                      movea.l   d0,a5
[0000458a] 200e                      move.l    a6,d0
[0000458c] d0bc ffff fff0            add.l     #$FFFFFFF0,d0
[00004592] bbc0                      cmpa.l    d0,a5
[00004594] 6602                      bne.s     $00004598
[00004596] 421d                      clr.b     (a5)+
[00004598] 286e 000c                 movea.l   12(a6),a4
[0000459c] 6012                      bra.s     $000045B0
[0000459e] 1e25                      move.b    -(a5),d7
[000045a0] 4887                      ext.w     d7
[000045a2] de7c 0030                 add.w     #$0030,d7
[000045a6] be7c 0039                 cmp.w     #$0039,d7
[000045aa] 6f02                      ble.s     $000045AE
[000045ac] 5e47                      addq.w    #7,d7
[000045ae] 18c7                      move.b    d7,(a4)+
[000045b0] 200e                      move.l    a6,d0
[000045b2] d0bc ffff fff0            add.l     #$FFFFFFF0,d0
[000045b8] bbc0                      cmpa.l    d0,a5
[000045ba] 66e2                      bne.s     $0000459E
[000045bc] 4214                      clr.b     (a4)
[000045be] 200c                      move.l    a4,d0
[000045c0] 4a9f                      tst.l     (a7)+
[000045c2] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000045c6] 4e5e                      unlk      a6
[000045c8] 4e75                      rts       
[000045ca] _malloc_:
[000045ca] 4e56 fffc                 link      a6,#$FFFC
[000045ce] 4e5e                      unlk      a6
[000045d0] 4e75                      rts       
[000045d2] __errmal:
[000045d2] 4e56 fffc                 link      a6,#$FFFC
[000045d6] 4e5e                      unlk      a6
[000045d8] 4e75                      rts       
[000045da] _malloc:
[000045da] 4e56 0000                 link      a6,#$0000
[000045de] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000045e2] 3e2e 0008                 move.w    8(a6),d7
[000045e6] 5e47                      addq.w    #7,d7
[000045e8] e64f                      lsr.w     #3,d7
[000045ea] 5247                      addq.w    #1,d7
[000045ec] 3e87                      move.w    d7,(a7)
[000045ee] 611e                      bsr.s     $0000460E
[000045f0] 2a40                      movea.l   d0,a5
[000045f2] 200d                      move.l    a5,d0
[000045f4] 6604                      bne.s     $000045FA
[000045f6] 4280                      clr.l     d0
[000045f8] 600a                      bra.s     $00004604
[000045fa] 3e87                      move.w    d7,(a7)
[000045fc] 2f0d                      move.l    a5,-(a7)
[000045fe] 6100 0086                 bsr       $00004686
[00004602] 588f                      addq.l    #4,a7
[00004604] 4a9f                      tst.l     (a7)+
[00004606] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0000460a] 4e5e                      unlk      a6
[0000460c] 4e75                      rts       
[0000460e] 4e56 0000                 link      a6,#$0000
[00004612] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00004616] 2879 0000 5dd4            movea.l   __aflist,a4
[0000461c] 2a54                      movea.l   (a4),a5
[0000461e] 605a                      bra.s     $0000467A
[00004620] 4240                      clr.w     d0
[00004622] 302d 0004                 move.w    4(a5),d0
[00004626] 4241                      clr.w     d1
[00004628] 322d 0006                 move.w    6(a5),d1
[0000462c] b340                      eor.w     d1,d0
[0000462e] 4640                      not.w     d0
[00004630] 4a40                      tst.w     d0
[00004632] 670e                      beq.s     $00004642
[00004634] 3ebc 0001                 move.w    #$0001,(a7)
[00004638] 4eb9 0000 45d2            jsr       __errmal
[0000463e] 4280                      clr.l     d0
[00004640] 603a                      bra.s     $0000467C
[00004642] 4240                      clr.w     d0
[00004644] 302d 0004                 move.w    4(a5),d0
[00004648] b06e 0008                 cmp.w     8(a6),d0
[0000464c] 6504                      bcs.s     $00004652
[0000464e] 200c                      move.l    a4,d0
[00004650] 602a                      bra.s     $0000467C
[00004652] bbf9 0000 5dd4            cmpa.l    __aflist,a5
[00004658] 661c                      bne.s     $00004676
[0000465a] 3eae 0008                 move.w    8(a6),(a7)
[0000465e] 6100 00a6                 bsr       $00004706
[00004662] 2a40                      movea.l   d0,a5
[00004664] 200d                      move.l    a5,d0
[00004666] 660e                      bne.s     $00004676
[00004668] 3ebc 0002                 move.w    #$0002,(a7)
[0000466c] 4eb9 0000 45d2            jsr       __errmal
[00004672] 4280                      clr.l     d0
[00004674] 6006                      bra.s     $0000467C
[00004676] 284d                      movea.l   a5,a4
[00004678] 2a55                      movea.l   (a5),a5
[0000467a] 60a4                      bra.s     $00004620
[0000467c] 4a9f                      tst.l     (a7)+
[0000467e] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00004682] 4e5e                      unlk      a6
[00004684] 4e75                      rts       
[00004686] 4e56 0000                 link      a6,#$0000
[0000468a] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0000468e] 206e 0008                 movea.l   8(a6),a0
[00004692] 2a50                      movea.l   (a0),a5
[00004694] 4240                      clr.w     d0
[00004696] 302e 000c                 move.w    12(a6),d0
[0000469a] 5840                      addq.w    #4,d0
[0000469c] b06d 0004                 cmp.w     4(a5),d0
[000046a0] 6508                      bcs.s     $000046AA
[000046a2] 206e 0008                 movea.l   8(a6),a0
[000046a6] 2095                      move.l    (a5),(a0)
[000046a8] 6046                      bra.s     $000046F0
[000046aa] 284d                      movea.l   a5,a4
[000046ac] 4240                      clr.w     d0
[000046ae] 302e 000c                 move.w    12(a6),d0
[000046b2] e748                      lsl.w     #3,d0
[000046b4] 4840                      swap      d0
[000046b6] 4240                      clr.w     d0
[000046b8] 4840                      swap      d0
[000046ba] d9c0                      adda.l    d0,a4
[000046bc] 4240                      clr.w     d0
[000046be] 302d 0004                 move.w    4(a5),d0
[000046c2] 906e 000c                 sub.w     12(a6),d0
[000046c6] 3940 0004                 move.w    d0,4(a4)
[000046ca] 4240                      clr.w     d0
[000046cc] 302c 0004                 move.w    4(a4),d0
[000046d0] 4640                      not.w     d0
[000046d2] 3940 0006                 move.w    d0,6(a4)
[000046d6] 2895                      move.l    (a5),(a4)
[000046d8] 206e 0008                 movea.l   8(a6),a0
[000046dc] 208c                      move.l    a4,(a0)
[000046de] 3b6e 000c 0004            move.w    12(a6),4(a5)
[000046e4] 4240                      clr.w     d0
[000046e6] 302d 0004                 move.w    4(a5),d0
[000046ea] 4640                      not.w     d0
[000046ec] 3b40 0006                 move.w    d0,6(a5)
[000046f0] 23ee 0008 0000 5dd4       move.l    8(a6),__aflist
[000046f8] 200d                      move.l    a5,d0
[000046fa] 5080                      addq.l    #8,d0
[000046fc] 4a9f                      tst.l     (a7)+
[000046fe] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00004702] 4e5e                      unlk      a6
[00004704] 4e75                      rts       
[00004706] 4e56 0000                 link      a6,#$0000
[0000470a] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0000470e] 3e2e 0008                 move.w    8(a6),d7
[00004712] de7c 003f                 add.w     #$003F,d7
[00004716] ec47                      asr.w     #6,d7
[00004718] ed47                      asl.w     #6,d7
[0000471a] 3007                      move.w    d7,d0
[0000471c] e740                      asl.w     #3,d0
[0000471e] 3e80                      move.w    d0,(a7)
[00004720] 4eb9 0000 43ca            jsr       _sbrk
[00004726] 2a40                      movea.l   d0,a5
[00004728] bbfc ffff ffff            cmpa.l    #$FFFFFFFF,a5
[0000472e] 6604                      bne.s     $00004734
[00004730] 4280                      clr.l     d0
[00004732] 602a                      bra.s     $0000475E
[00004734] 200d                      move.l    a5,d0
[00004736] 5280                      addq.l    #1,d0
[00004738] c0bc ffff fffe            and.l     #$FFFFFFFE,d0
[0000473e] 2a40                      movea.l   d0,a5
[00004740] 284d                      movea.l   a5,a4
[00004742] 3947 0004                 move.w    d7,4(a4)
[00004746] 4240                      clr.w     d0
[00004748] 302c 0004                 move.w    4(a4),d0
[0000474c] 4640                      not.w     d0
[0000474e] 3940 0006                 move.w    d0,6(a4)
[00004752] 2e8c                      move.l    a4,(a7)
[00004754] 5097                      addq.l    #8,(a7)
[00004756] 6110                      bsr.s     _free
[00004758] 2039 0000 5dd4            move.l    __aflist,d0
[0000475e] 4a9f                      tst.l     (a7)+
[00004760] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00004764] 4e5e                      unlk      a6
[00004766] 4e75                      rts       
[00004768] _free:
[00004768] 4e56 0000                 link      a6,#$0000
[0000476c] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00004770] 2a6e 0008                 movea.l   8(a6),a5
[00004774] 518d                      subq.l    #8,a5
[00004776] 4240                      clr.w     d0
[00004778] 302d 0004                 move.w    4(a5),d0
[0000477c] 4241                      clr.w     d1
[0000477e] 322d 0006                 move.w    6(a5),d1
[00004782] b340                      eor.w     d1,d0
[00004784] 4640                      not.w     d0
[00004786] 4a40                      tst.w     d0
[00004788] 6710                      beq.s     $0000479A
[0000478a] 3ebc 0003                 move.w    #$0003,(a7)
[0000478e] 4eb9 0000 45d2            jsr       __errmal
[00004794] 70ff                      moveq.l   #-1,d0
[00004796] 6000 00c4                 bra       $0000485C
[0000479a] 2879 0000 5dd4            movea.l   __aflist,a4
[000047a0] bbcc                      cmpa.l    a4,a5
[000047a2] 6504                      bcs.s     $000047A8
[000047a4] bbd4                      cmpa.l    (a4),a5
[000047a6] 6510                      bcs.s     $000047B8
[000047a8] b9d4                      cmpa.l    (a4),a4
[000047aa] 6508                      bcs.s     $000047B4
[000047ac] bbd4                      cmpa.l    (a4),a5
[000047ae] 6308                      bls.s     $000047B8
[000047b0] bbcc                      cmpa.l    a4,a5
[000047b2] 6404                      bcc.s     $000047B8
[000047b4] 2854                      movea.l   (a4),a4
[000047b6] 60e8                      bra.s     $000047A0
[000047b8] bbcc                      cmpa.l    a4,a5
[000047ba] 6532                      bcs.s     $000047EE
[000047bc] 200d                      move.l    a5,d0
[000047be] 4241                      clr.w     d1
[000047c0] 322d 0004                 move.w    4(a5),d1
[000047c4] e749                      lsl.w     #3,d1
[000047c6] 4841                      swap      d1
[000047c8] 4241                      clr.w     d1
[000047ca] 4841                      swap      d1
[000047cc] d081                      add.l     d1,d0
[000047ce] 220c                      move.l    a4,d1
[000047d0] 4242                      clr.w     d2
[000047d2] 342c 0004                 move.w    4(a4),d2
[000047d6] e74a                      lsl.w     #3,d2
[000047d8] 4842                      swap      d2
[000047da] 4242                      clr.w     d2
[000047dc] 4842                      swap      d2
[000047de] d282                      add.l     d2,d1
[000047e0] b081                      cmp.l     d1,d0
[000047e2] 620a                      bhi.s     $000047EE
[000047e4] 23cc 0000 5dd4            move.l    a4,__aflist
[000047ea] 4240                      clr.w     d0
[000047ec] 606e                      bra.s     $0000485C
[000047ee] 200d                      move.l    a5,d0
[000047f0] 4241                      clr.w     d1
[000047f2] 322d 0004                 move.w    4(a5),d1
[000047f6] e749                      lsl.w     #3,d1
[000047f8] 4841                      swap      d1
[000047fa] 4241                      clr.w     d1
[000047fc] 4841                      swap      d1
[000047fe] d081                      add.l     d1,d0
[00004800] b094                      cmp.l     (a4),d0
[00004802] 661c                      bne.s     $00004820
[00004804] 2054                      movea.l   (a4),a0
[00004806] 3028 0004                 move.w    4(a0),d0
[0000480a] d16d 0004                 add.w     d0,4(a5)
[0000480e] 4240                      clr.w     d0
[00004810] 302d 0004                 move.w    4(a5),d0
[00004814] 4640                      not.w     d0
[00004816] 3b40 0006                 move.w    d0,6(a5)
[0000481a] 2054                      movea.l   (a4),a0
[0000481c] 2a90                      move.l    (a0),(a5)
[0000481e] 6002                      bra.s     $00004822
[00004820] 2a94                      move.l    (a4),(a5)
[00004822] 200c                      move.l    a4,d0
[00004824] 4241                      clr.w     d1
[00004826] 322c 0004                 move.w    4(a4),d1
[0000482a] e749                      lsl.w     #3,d1
[0000482c] 4841                      swap      d1
[0000482e] 4241                      clr.w     d1
[00004830] 4841                      swap      d1
[00004832] d081                      add.l     d1,d0
[00004834] bbc0                      cmpa.l    d0,a5
[00004836] 661a                      bne.s     $00004852
[00004838] 4240                      clr.w     d0
[0000483a] 302d 0004                 move.w    4(a5),d0
[0000483e] d16c 0004                 add.w     d0,4(a4)
[00004842] 4240                      clr.w     d0
[00004844] 302c 0004                 move.w    4(a4),d0
[00004848] 4640                      not.w     d0
[0000484a] 3940 0006                 move.w    d0,6(a4)
[0000484e] 2895                      move.l    (a5),(a4)
[00004850] 6002                      bra.s     $00004854
[00004852] 288d                      move.l    a5,(a4)
[00004854] 23cc 0000 5dd4            move.l    a4,__aflist
[0000485a] 4240                      clr.w     d0
[0000485c] 4a9f                      tst.l     (a7)+
[0000485e] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00004862] 4e5e                      unlk      a6
[00004864] 4e75                      rts       
[00004866] _realloc:
[00004866] 4e56 fffc                 link      a6,#$FFFC
[0000486a] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0000486e] 2a6e 0008                 movea.l   8(a6),a5
[00004872] 2e8d                      move.l    a5,(a7)
[00004874] 6100 fef2                 bsr       _free
[00004878] 3e2e 000c                 move.w    12(a6),d7
[0000487c] 5e47                      addq.w    #7,d7
[0000487e] e64f                      lsr.w     #3,d7
[00004880] 5247                      addq.w    #1,d7
[00004882] 3e87                      move.w    d7,(a7)
[00004884] 6100 fd88                 bsr       $0000460E
[00004888] 2d40 fffc                 move.l    d0,-4(a6)
[0000488c] 6604                      bne.s     $00004892
[0000488e] 4280                      clr.l     d0
[00004890] 604a                      bra.s     $000048DC
[00004892] 206e fffc                 movea.l   -4(a6),a0
[00004896] 2850                      movea.l   (a0),a4
[00004898] 508c                      addq.l    #8,a4
[0000489a] bbcc                      cmpa.l    a4,a5
[0000489c] 6732                      beq.s     $000048D0
[0000489e] b9cd                      cmpa.l    a5,a4
[000048a0] 6410                      bcc.s     $000048B2
[000048a2] 6006                      bra.s     $000048AA
[000048a4] 18dd                      move.b    (a5)+,(a4)+
[000048a6] 536e 000c                 subq.w    #1,12(a6)
[000048aa] 4a6e 000c                 tst.w     12(a6)
[000048ae] 66f4                      bne.s     $000048A4
[000048b0] 601e                      bra.s     $000048D0
[000048b2] 4280                      clr.l     d0
[000048b4] 302e 000c                 move.w    12(a6),d0
[000048b8] d9c0                      adda.l    d0,a4
[000048ba] 4280                      clr.l     d0
[000048bc] 302e 000c                 move.w    12(a6),d0
[000048c0] dbc0                      adda.l    d0,a5
[000048c2] 6006                      bra.s     $000048CA
[000048c4] 1925                      move.b    -(a5),-(a4)
[000048c6] 536e 000c                 subq.w    #1,12(a6)
[000048ca] 4a6e 000c                 tst.w     12(a6)
[000048ce] 66f4                      bne.s     $000048C4
[000048d0] 3e87                      move.w    d7,(a7)
[000048d2] 2f2e fffc                 move.l    -4(a6),-(a7)
[000048d6] 6100 fdae                 bsr       $00004686
[000048da] 588f                      addq.l    #4,a7
[000048dc] 4a9f                      tst.l     (a7)+
[000048de] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000048e2] 4e5e                      unlk      a6
[000048e4] 4e75                      rts       
[000048e6] _isatty:
[000048e6] 4e56 0000                 link      a6,#$0000
[000048ea] 48e7 0104                 movem.l   d7/a5,-(a7)
[000048ee] 3eae 0008                 move.w    8(a6),(a7)
[000048f2] 4eb9 0000 42cc            jsr       __chkc
[000048f8] 2a40                      movea.l   d0,a5
[000048fa] 200d                      move.l    a5,d0
[000048fc] 6604                      bne.s     $00004902
[000048fe] 4240                      clr.w     d0
[00004900] 600e                      bra.s     $00004910
[00004902] 082d 0001 0001            btst      #1,1(a5)
[00004908] 6604                      bne.s     $0000490E
[0000490a] 4240                      clr.w     d0
[0000490c] 6002                      bra.s     $00004910
[0000490e] 7001                      moveq.l   #1,d0
[00004910] 4a9f                      tst.l     (a7)+
[00004912] 4cdf 2000                 movem.l   (a7)+,a5
[00004916] 4e5e                      unlk      a6
[00004918] 4e75                      rts       
[0000491a] _isdev:
[0000491a] 4e56 0000                 link      a6,#$0000
[0000491e] 48e7 0104                 movem.l   d7/a5,-(a7)
[00004922] 3eae 0008                 move.w    8(a6),(a7)
[00004926] 4eb9 0000 42cc            jsr       __chkc
[0000492c] 2a40                      movea.l   d0,a5
[0000492e] 200d                      move.l    a5,d0
[00004930] 6604                      bne.s     $00004936
[00004932] 4240                      clr.w     d0
[00004934] 6006                      bra.s     $0000493C
[00004936] 3015                      move.w    (a5),d0
[00004938] c07c 0106                 and.w     #$0106,d0
[0000493c] 4a9f                      tst.l     (a7)+
[0000493e] 4cdf 2000                 movem.l   (a7)+,a5
[00004942] 4e5e                      unlk      a6
[00004944] 4e75                      rts       
[00004946] _ttyname:
[00004946] 4e56 fffc                 link      a6,#$FFFC
[0000494a] 3eae 0008                 move.w    8(a6),(a7)
[0000494e] 6196                      bsr.s     _isatty
[00004950] 4a40                      tst.w     d0
[00004952] 6708                      beq.s     $0000495C
[00004954] 203c 0000 58ef            move.l    #___tname,d0
[0000495a] 6002                      bra.s     $0000495E
[0000495c] 4280                      clr.l     d0
[0000495e] 4e5e                      unlk      a6
[00004960] 4e75                      rts       
[00004962] __flsbuf:
[00004962] 4e56 fffe                 link      a6,#$FFFE
[00004966] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[0000496a] 1e2e 0009                 move.b    9(a6),d7
[0000496e] 2a6e 000a                 movea.l   10(a6),a5
[00004972] 4246                      clr.w     d6
[00004974] 3a06                      move.w    d6,d5
[00004976] 082d 0001 0003            btst      #1,3(a5)
[0000497c] 6606                      bne.s     $00004984
[0000497e] 70ff                      moveq.l   #-1,d0
[00004980] 6000 0124                 bra       $00004AA6
[00004984] 4aad 0004                 tst.l     4(a5)
[00004988] 6656                      bne.s     $000049E0
[0000498a] 082d 0003 0003            btst      #3,3(a5)
[00004990] 664e                      bne.s     $000049E0
[00004992] 3ebc 0200                 move.w    #$0200,(a7)
[00004996] 4eb9 0000 45da            jsr       _malloc
[0000499c] 2b40 0004                 move.l    d0,4(a5)
[000049a0] 2b40 0008                 move.l    d0,8(a5)
[000049a4] 6608                      bne.s     $000049AE
[000049a6] 006d 0008 0002            ori.w     #$0008,2(a5)
[000049ac] 6032                      bra.s     $000049E0
[000049ae] 006d 0004 0002            ori.w     #$0004,2(a5)
[000049b4] 3e95                      move.w    (a5),(a7)
[000049b6] 4eb9 0000 48e6            jsr       _isatty
[000049bc] 4a40                      tst.w     d0
[000049be] 6708                      beq.s     $000049C8
[000049c0] 006d 0040 0002            ori.w     #$0040,2(a5)
[000049c6] 6018                      bra.s     $000049E0
[000049c8] 3b7c 01fe 000c            move.w    #$01FE,12(a5)
[000049ce] 1007                      move.b    d7,d0
[000049d0] 4880                      ext.w     d0
[000049d2] 226d 0008                 movea.l   8(a5),a1
[000049d6] 1280                      move.b    d0,(a1)
[000049d8] 52ad 0008                 addq.l    #1,8(a5)
[000049dc] 6000 00c8                 bra       $00004AA6
[000049e0] 082d 0003 0003            btst      #3,3(a5)
[000049e6] 670e                      beq.s     $000049F6
[000049e8] 41ee fffe                 lea.l     -2(a6),a0
[000049ec] 2b48 0004                 move.l    a0,4(a5)
[000049f0] 2008                      move.l    a0,d0
[000049f2] 2b40 0008                 move.l    d0,8(a5)
[000049f6] 206d 0008                 movea.l   8(a5),a0
[000049fa] 1087                      move.b    d7,(a0)
[000049fc] 52ad 0008                 addq.l    #1,8(a5)
[00004a00] 082d 0003 0003            btst      #3,3(a5)
[00004a06] 671a                      beq.s     $00004A22
[00004a08] 7a01                      moveq.l   #1,d5
[00004a0a] 3e85                      move.w    d5,(a7)
[00004a0c] 2f2d 0004                 move.l    4(a5),-(a7)
[00004a10] 3f15                      move.w    (a5),-(a7)
[00004a12] 4eb9 0000 432e            jsr       _write
[00004a18] 5c8f                      addq.l    #6,a7
[00004a1a] 3c00                      move.w    d0,d6
[00004a1c] 426d 000c                 clr.w     12(a5)
[00004a20] 606e                      bra.s     $00004A90
[00004a22] 082d 0006 0003            btst      #6,3(a5)
[00004a28] 673e                      beq.s     $00004A68
[00004a2a] be3c 000a                 cmp.b     #$0A,d7
[00004a2e] 6710                      beq.s     $00004A40
[00004a30] 202d 0004                 move.l    4(a5),d0
[00004a34] d0bc 0000 0200            add.l     #$00000200,d0
[00004a3a] b0ad 0008                 cmp.l     8(a5),d0
[00004a3e] 6222                      bhi.s     $00004A62
[00004a40] 202d 0008                 move.l    8(a5),d0
[00004a44] 90ad 0004                 sub.l     4(a5),d0
[00004a48] 3a00                      move.w    d0,d5
[00004a4a] 3e85                      move.w    d5,(a7)
[00004a4c] 2f2d 0004                 move.l    4(a5),-(a7)
[00004a50] 3f15                      move.w    (a5),-(a7)
[00004a52] 4eb9 0000 432e            jsr       _write
[00004a58] 5c8f                      addq.l    #6,a7
[00004a5a] 3c00                      move.w    d0,d6
[00004a5c] 2b6d 0004 0008            move.l    4(a5),8(a5)
[00004a62] 426d 000c                 clr.w     12(a5)
[00004a66] 6028                      bra.s     $00004A90
[00004a68] 202d 0008                 move.l    8(a5),d0
[00004a6c] 90ad 0004                 sub.l     4(a5),d0
[00004a70] 3a00                      move.w    d0,d5
[00004a72] 3e85                      move.w    d5,(a7)
[00004a74] 2f2d 0004                 move.l    4(a5),-(a7)
[00004a78] 3f15                      move.w    (a5),-(a7)
[00004a7a] 4eb9 0000 432e            jsr       _write
[00004a80] 5c8f                      addq.l    #6,a7
[00004a82] 3c00                      move.w    d0,d6
[00004a84] 3b7c 01ff 000c            move.w    #$01FF,12(a5)
[00004a8a] 2b6d 0004 0008            move.l    4(a5),8(a5)
[00004a90] ba46                      cmp.w     d6,d5
[00004a92] 670a                      beq.s     $00004A9E
[00004a94] 006d 0010 0002            ori.w     #$0010,2(a5)
[00004a9a] 70ff                      moveq.l   #-1,d0
[00004a9c] 6008                      bra.s     $00004AA6
[00004a9e] 1007                      move.b    d7,d0
[00004aa0] 4880                      ext.w     d0
[00004aa2] c07c 00ff                 and.w     #$00FF,d0
[00004aa6] 4a9f                      tst.l     (a7)+
[00004aa8] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00004aac] 4e5e                      unlk      a6
[00004aae] 4e75                      rts       
[00004ab0] _fputc:
[00004ab0] 4e56 0000                 link      a6,#$0000
[00004ab4] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00004ab8] 1e2e 0009                 move.b    9(a6),d7
[00004abc] 2a6e 000a                 movea.l   10(a6),a5
[00004ac0] 536d 000c                 subq.w    #1,12(a5)
[00004ac4] 6d16                      blt.s     $00004ADC
[00004ac6] 1007                      move.b    d7,d0
[00004ac8] 4880                      ext.w     d0
[00004aca] 226d 0008                 movea.l   8(a5),a1
[00004ace] 1280                      move.b    d0,(a1)
[00004ad0] c07c 00ff                 and.w     #$00FF,d0
[00004ad4] 52ad 0008                 addq.l    #1,8(a5)
[00004ad8] 6012                      bra.s     $00004AEC
[00004ada] 6010                      bra.s     $00004AEC
[00004adc] 2e8d                      move.l    a5,(a7)
[00004ade] 1007                      move.b    d7,d0
[00004ae0] 4880                      ext.w     d0
[00004ae2] 3f00                      move.w    d0,-(a7)
[00004ae4] 4eb9 0000 4962            jsr       __flsbuf
[00004aea] 548f                      addq.l    #2,a7
[00004aec] 4a9f                      tst.l     (a7)+
[00004aee] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00004af2] 4e5e                      unlk      a6
[00004af4] 4e75                      rts       
[00004af6] _fputn:
[00004af6] 4e56 0000                 link      a6,#$0000
[00004afa] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00004afe] 2a6e 0008                 movea.l   8(a6),a5
[00004b02] 3e2e 000c                 move.w    12(a6),d7
[00004b06] 286e 000e                 movea.l   14(a6),a4
[00004b0a] 082c 0003 0003            btst      #3,3(a4)
[00004b10] 6724                      beq.s     $00004B36
[00004b12] 426c 000c                 clr.w     12(a4)
[00004b16] 3e87                      move.w    d7,(a7)
[00004b18] 2f0d                      move.l    a5,-(a7)
[00004b1a] 3f14                      move.w    (a4),-(a7)
[00004b1c] 4eb9 0000 432e            jsr       _write
[00004b22] 5c8f                      addq.l    #6,a7
[00004b24] b047                      cmp.w     d7,d0
[00004b26] 670a                      beq.s     $00004B32
[00004b28] 006c 0010 0002            ori.w     #$0010,2(a4)
[00004b2e] 70ff                      moveq.l   #-1,d0
[00004b30] 602a                      bra.s     $00004B5C
[00004b32] 4240                      clr.w     d0
[00004b34] 6026                      bra.s     $00004B5C
[00004b36] 601a                      bra.s     $00004B52
[00004b38] 2e8c                      move.l    a4,(a7)
[00004b3a] 101d                      move.b    (a5)+,d0
[00004b3c] 4880                      ext.w     d0
[00004b3e] 3f00                      move.w    d0,-(a7)
[00004b40] 4eb9 0000 4ab0            jsr       _fputc
[00004b46] 548f                      addq.l    #2,a7
[00004b48] b07c ffff                 cmp.w     #$FFFF,d0
[00004b4c] 6604                      bne.s     $00004B52
[00004b4e] 70ff                      moveq.l   #-1,d0
[00004b50] 600a                      bra.s     $00004B5C
[00004b52] 3007                      move.w    d7,d0
[00004b54] 5347                      subq.w    #1,d7
[00004b56] 4a40                      tst.w     d0
[00004b58] 66de                      bne.s     $00004B38
[00004b5a] 4240                      clr.w     d0
[00004b5c] 4a9f                      tst.l     (a7)+
[00004b5e] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00004b62] 4e5e                      unlk      a6
[00004b64] 4e75                      rts       
[00004b66] __pftoa:
[00004b66] 4e56 fff8                 link      a6,#$FFF8
[00004b6a] 4a6e 0010                 tst.w     16(a6)
[00004b6e] 6c04                      bge.s     $00004B74
[00004b70] 7006                      moveq.l   #6,d0
[00004b72] 6004                      bra.s     $00004B78
[00004b74] 302e 0010                 move.w    16(a6),d0
[00004b78] 3d40 0010                 move.w    d0,16(a6)
[00004b7c] 206e 0008                 movea.l   8(a6),a0
[00004b80] 2010                      move.l    (a0),d0
[00004b82] 2d40 fffc                 move.l    d0,-4(a6)
[00004b86] 3eae 0010                 move.w    16(a6),(a7)
[00004b8a] 2f2e 000c                 move.l    12(a6),-(a7)
[00004b8e] 2f2e fffc                 move.l    -4(a6),-(a7)
[00004b92] 4eb9 0000 0000            jsr       __start
[00004b98] 508f                      addq.l    #8,a7
[00004b9a] 4e5e                      unlk      a6
[00004b9c] 4e75                      rts       
[00004b9e] __petoa:
[00004b9e] 4e56 fff8                 link      a6,#$FFF8
[00004ba2] 4a6e 0010                 tst.w     16(a6)
[00004ba6] 6c04                      bge.s     $00004BAC
[00004ba8] 7006                      moveq.l   #6,d0
[00004baa] 6004                      bra.s     $00004BB0
[00004bac] 302e 0010                 move.w    16(a6),d0
[00004bb0] 3d40 0010                 move.w    d0,16(a6)
[00004bb4] 206e 0008                 movea.l   8(a6),a0
[00004bb8] 2010                      move.l    (a0),d0
[00004bba] 2d40 fffc                 move.l    d0,-4(a6)
[00004bbe] 3eae 0010                 move.w    16(a6),(a7)
[00004bc2] 2f2e 000c                 move.l    12(a6),-(a7)
[00004bc6] 2f2e fffc                 move.l    -4(a6),-(a7)
[00004bca] 4eb9 0000 0000            jsr       __start
[00004bd0] 508f                      addq.l    #8,a7
[00004bd2] 4e5e                      unlk      a6
[00004bd4] 4e75                      rts       
[00004bd6] __pgtoa:
[00004bd6] 4e56 fff8                 link      a6,#$FFF8
[00004bda] 3eae 0010                 move.w    16(a6),(a7)
[00004bde] 2f2e 000c                 move.l    12(a6),-(a7)
[00004be2] 2f2e 0008                 move.l    8(a6),-(a7)
[00004be6] 6100 ff7e                 bsr       __pftoa
[00004bea] 508f                      addq.l    #8,a7
[00004bec] 2d40 fffc                 move.l    d0,-4(a6)
[00004bf0] 2eae 000c                 move.l    12(a6),(a7)
[00004bf4] 4eb9 0000 0954            jsr       _strlen
[00004bfa] 322e 0010                 move.w    16(a6),d1
[00004bfe] 5e41                      addq.w    #7,d1
[00004c00] b041                      cmp.w     d1,d0
[00004c02] 6f14                      ble.s     $00004C18
[00004c04] 3eae 0010                 move.w    16(a6),(a7)
[00004c08] 2f2e 000c                 move.l    12(a6),-(a7)
[00004c0c] 2f2e 0008                 move.l    8(a6),-(a7)
[00004c10] 618c                      bsr.s     __petoa
[00004c12] 508f                      addq.l    #8,a7
[00004c14] 2d40 fffc                 move.l    d0,-4(a6)
[00004c18] 202e fffc                 move.l    -4(a6),d0
[00004c1c] 4e5e                      unlk      a6
[00004c1e] 4e75                      rts       
[00004c20] __doprt:
[00004c20] 4e56 fde4                 link      a6,#$FDE4
[00004c24] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00004c28] 2a6e 0010                 movea.l   16(a6),a5
[00004c2c] 426e fdec                 clr.w     -532(a6)
[00004c30] 4aae 000c                 tst.l     12(a6)
[00004c34] 6700 0468                 beq       $0000509E
[00004c38] 6000 0460                 bra       $0000509A
[00004c3c] 4245                      clr.w     d5
[00004c3e] 2d6e 000c fff2            move.l    12(a6),-14(a6)
[00004c44] 6006                      bra.s     $00004C4C
[00004c46] 52ae fff2                 addq.l    #1,-14(a6)
[00004c4a] 5245                      addq.w    #1,d5
[00004c4c] 206e fff2                 movea.l   -14(a6),a0
[00004c50] 4a10                      tst.b     (a0)
[00004c52] 670a                      beq.s     $00004C5E
[00004c54] 206e fff2                 movea.l   -14(a6),a0
[00004c58] 0c10 0025                 cmpi.b    #$25,(a0)
[00004c5c] 66e8                      bne.s     $00004C46
[00004c5e] 4a45                      tst.w     d5
[00004c60] 6f1c                      ble.s     $00004C7E
[00004c62] 2eae 0008                 move.l    8(a6),(a7)
[00004c66] 3f05                      move.w    d5,-(a7)
[00004c68] 2f2e 000c                 move.l    12(a6),-(a7)
[00004c6c] 4eb9 0000 4af6            jsr       _fputn
[00004c72] 5c8f                      addq.l    #6,a7
[00004c74] 2d6e fff2 000c            move.l    -14(a6),12(a6)
[00004c7a] db6e fdec                 add.w     d5,-532(a6)
[00004c7e] 206e 000c                 movea.l   12(a6),a0
[00004c82] 0c10 0025                 cmpi.b    #$25,(a0)
[00004c86] 40c0                      move.w    sr,d0
[00004c88] 52ae 000c                 addq.l    #1,12(a6)
[00004c8c] 44c0                      move.b    d0,ccr
[00004c8e] 6600 040e                 bne       $0000509E
[00004c92] 426e fdea                 clr.w     -534(a6)
[00004c96] 206e 000c                 movea.l   12(a6),a0
[00004c9a] 1010                      move.b    (a0),d0
[00004c9c] 4880                      ext.w     d0
[00004c9e] 1e00                      move.b    d0,d7
[00004ca0] b07c 002d                 cmp.w     #$002D,d0
[00004ca4] 40c0                      move.w    sr,d0
[00004ca6] 52ae 000c                 addq.l    #1,12(a6)
[00004caa] 44c0                      move.b    d0,ccr
[00004cac] 660e                      bne.s     $00004CBC
[00004cae] 206e 000c                 movea.l   12(a6),a0
[00004cb2] 1e10                      move.b    (a0),d7
[00004cb4] 52ae 000c                 addq.l    #1,12(a6)
[00004cb8] 526e fdea                 addq.w    #1,-534(a6)
[00004cbc] 1d7c 0020 fff6            move.b    #$20,-10(a6)
[00004cc2] be3c 0030                 cmp.b     #$30,d7
[00004cc6] 660e                      bne.s     $00004CD6
[00004cc8] 1d47 fff6                 move.b    d7,-10(a6)
[00004ccc] 206e 000c                 movea.l   12(a6),a0
[00004cd0] 1e10                      move.b    (a0),d7
[00004cd2] 52ae 000c                 addq.l    #1,12(a6)
[00004cd6] 3d7c ffff fdf0            move.w    #$FFFF,-528(a6)
[00004cdc] be3c 002a                 cmp.b     #$2A,d7
[00004ce0] 661a                      bne.s     $00004CFC
[00004ce2] 2d4d fff8                 move.l    a5,-8(a6)
[00004ce6] 206e fff8                 movea.l   -8(a6),a0
[00004cea] 3d50 fdf0                 move.w    (a0),-528(a6)
[00004cee] 548d                      addq.l    #2,a5
[00004cf0] 206e 000c                 movea.l   12(a6),a0
[00004cf4] 1e10                      move.b    (a0),d7
[00004cf6] 52ae 000c                 addq.l    #1,12(a6)
[00004cfa] 6038                      bra.s     $00004D34
[00004cfc] 602a                      bra.s     $00004D28
[00004cfe] 4a6e fdf0                 tst.w     -528(a6)
[00004d02] 6c04                      bge.s     $00004D08
[00004d04] 426e fdf0                 clr.w     -528(a6)
[00004d08] 1007                      move.b    d7,d0
[00004d0a] 4880                      ext.w     d0
[00004d0c] 322e fdf0                 move.w    -528(a6),d1
[00004d10] c3fc 000a                 muls.w    #$000A,d1
[00004d14] d041                      add.w     d1,d0
[00004d16] d07c ffd0                 add.w     #$FFD0,d0
[00004d1a] 3d40 fdf0                 move.w    d0,-528(a6)
[00004d1e] 206e 000c                 movea.l   12(a6),a0
[00004d22] 1e10                      move.b    (a0),d7
[00004d24] 52ae 000c                 addq.l    #1,12(a6)
[00004d28] be3c 0030                 cmp.b     #$30,d7
[00004d2c] 6d06                      blt.s     $00004D34
[00004d2e] be3c 0039                 cmp.b     #$39,d7
[00004d32] 6fca                      ble.s     $00004CFE
[00004d34] 7cff                      moveq.l   #-1,d6
[00004d36] be3c 002e                 cmp.b     #$2E,d7
[00004d3a] 660c                      bne.s     $00004D48
[00004d3c] 4246                      clr.w     d6
[00004d3e] 206e 000c                 movea.l   12(a6),a0
[00004d42] 1e10                      move.b    (a0),d7
[00004d44] 52ae 000c                 addq.l    #1,12(a6)
[00004d48] be3c 002a                 cmp.b     #$2A,d7
[00004d4c] 6618                      bne.s     $00004D66
[00004d4e] 2d4d fff8                 move.l    a5,-8(a6)
[00004d52] 206e fff8                 movea.l   -8(a6),a0
[00004d56] 3c10                      move.w    (a0),d6
[00004d58] 548d                      addq.l    #2,a5
[00004d5a] 206e 000c                 movea.l   12(a6),a0
[00004d5e] 1e10                      move.b    (a0),d7
[00004d60] 52ae 000c                 addq.l    #1,12(a6)
[00004d64] 602a                      bra.s     $00004D90
[00004d66] 601c                      bra.s     $00004D84
[00004d68] 1007                      move.b    d7,d0
[00004d6a] 4880                      ext.w     d0
[00004d6c] 3206                      move.w    d6,d1
[00004d6e] c3fc 000a                 muls.w    #$000A,d1
[00004d72] d041                      add.w     d1,d0
[00004d74] 3c00                      move.w    d0,d6
[00004d76] dc7c ffd0                 add.w     #$FFD0,d6
[00004d7a] 206e 000c                 movea.l   12(a6),a0
[00004d7e] 1e10                      move.b    (a0),d7
[00004d80] 52ae 000c                 addq.l    #1,12(a6)
[00004d84] be3c 0030                 cmp.b     #$30,d7
[00004d88] 6d06                      blt.s     $00004D90
[00004d8a] be3c 0039                 cmp.b     #$39,d7
[00004d8e] 6fd8                      ble.s     $00004D68
[00004d90] 426e fde8                 clr.w     -536(a6)
[00004d94] be3c 006c                 cmp.b     #$6C,d7
[00004d98] 6706                      beq.s     $00004DA0
[00004d9a] be3c 004c                 cmp.b     #$4C,d7
[00004d9e] 660e                      bne.s     $00004DAE
[00004da0] 526e fde8                 addq.w    #1,-536(a6)
[00004da4] 206e 000c                 movea.l   12(a6),a0
[00004da8] 1e10                      move.b    (a0),d7
[00004daa] 52ae 000c                 addq.l    #1,12(a6)
[00004dae] 41ee fdf2                 lea.l     -526(a6),a0
[00004db2] 2d48 fff2                 move.l    a0,-14(a6)
[00004db6] 1007                      move.b    d7,d0
[00004db8] 4880                      ext.w     d0
[00004dba] 6000 020c                 bra       $00004FC8
[00004dbe] 526e fde8                 addq.w    #1,-536(a6)
[00004dc2] 4a6e fde8                 tst.w     -536(a6)
[00004dc6] 6708                      beq.s     $00004DD0
[00004dc8] 203c 0000 44a2            move.l    #___prtld,d0
[00004dce] 6006                      bra.s     $00004DD6
[00004dd0] 203c 0000 4434            move.l    #___prtsh,d0
[00004dd6] 23c0 0000 5fdc            move.l    d0,$00005FDC
[00004ddc] 2eb9 0000 5fdc            move.l    $00005FDC,(a7)
[00004de2] 3f3c 0001                 move.w    #$0001,-(a7)
[00004de6] 3f3c 000a                 move.w    #$000A,-(a7)
[00004dea] 2f0e                      move.l    a6,-(a7)
[00004dec] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004df2] 2f0d                      move.l    a5,-(a7)
[00004df4] 4eb9 0000 4558            jsr       ___prtin
[00004dfa] dffc 0000 000c            adda.l    #$0000000C,a7
[00004e00] 4a6e fde8                 tst.w     -536(a6)
[00004e04] 6704                      beq.s     $00004E0A
[00004e06] 7004                      moveq.l   #4,d0
[00004e08] 6002                      bra.s     $00004E0C
[00004e0a] 7002                      moveq.l   #2,d0
[00004e0c] 48c0                      ext.l     d0
[00004e0e] dbc0                      adda.l    d0,a5
[00004e10] 6000 01ce                 bra       $00004FE0
[00004e14] 526e fde8                 addq.w    #1,-536(a6)
[00004e18] 4a6e fde8                 tst.w     -536(a6)
[00004e1c] 6708                      beq.s     $00004E26
[00004e1e] 203c 0000 44a2            move.l    #___prtld,d0
[00004e24] 6006                      bra.s     $00004E2C
[00004e26] 203c 0000 4434            move.l    #___prtsh,d0
[00004e2c] 23c0 0000 5fdc            move.l    d0,$00005FDC
[00004e32] 2eb9 0000 5fdc            move.l    $00005FDC,(a7)
[00004e38] 4267                      clr.w     -(a7)
[00004e3a] 3f3c 000a                 move.w    #$000A,-(a7)
[00004e3e] 2f0e                      move.l    a6,-(a7)
[00004e40] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004e46] 2f0d                      move.l    a5,-(a7)
[00004e48] 4eb9 0000 4558            jsr       ___prtin
[00004e4e] dffc 0000 000c            adda.l    #$0000000C,a7
[00004e54] 4a6e fde8                 tst.w     -536(a6)
[00004e58] 6704                      beq.s     $00004E5E
[00004e5a] 7004                      moveq.l   #4,d0
[00004e5c] 6002                      bra.s     $00004E60
[00004e5e] 7002                      moveq.l   #2,d0
[00004e60] 48c0                      ext.l     d0
[00004e62] dbc0                      adda.l    d0,a5
[00004e64] 6000 017a                 bra       $00004FE0
[00004e68] 526e fde8                 addq.w    #1,-536(a6)
[00004e6c] 4a6e fde8                 tst.w     -536(a6)
[00004e70] 6708                      beq.s     $00004E7A
[00004e72] 203c 0000 44a2            move.l    #___prtld,d0
[00004e78] 6006                      bra.s     $00004E80
[00004e7a] 203c 0000 4434            move.l    #___prtsh,d0
[00004e80] 23c0 0000 5fdc            move.l    d0,$00005FDC
[00004e86] 2eb9 0000 5fdc            move.l    $00005FDC,(a7)
[00004e8c] 4267                      clr.w     -(a7)
[00004e8e] 3f3c 0008                 move.w    #$0008,-(a7)
[00004e92] 2f0e                      move.l    a6,-(a7)
[00004e94] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004e9a] 2f0d                      move.l    a5,-(a7)
[00004e9c] 4eb9 0000 4558            jsr       ___prtin
[00004ea2] dffc 0000 000c            adda.l    #$0000000C,a7
[00004ea8] 4a6e fde8                 tst.w     -536(a6)
[00004eac] 6704                      beq.s     $00004EB2
[00004eae] 7004                      moveq.l   #4,d0
[00004eb0] 6002                      bra.s     $00004EB4
[00004eb2] 7002                      moveq.l   #2,d0
[00004eb4] 48c0                      ext.l     d0
[00004eb6] dbc0                      adda.l    d0,a5
[00004eb8] 6000 0126                 bra       $00004FE0
[00004ebc] 526e fde8                 addq.w    #1,-536(a6)
[00004ec0] 4a6e fde8                 tst.w     -536(a6)
[00004ec4] 6708                      beq.s     $00004ECE
[00004ec6] 203c 0000 44a2            move.l    #___prtld,d0
[00004ecc] 6006                      bra.s     $00004ED4
[00004ece] 203c 0000 4434            move.l    #___prtsh,d0
[00004ed4] 23c0 0000 5fdc            move.l    d0,$00005FDC
[00004eda] 2eb9 0000 5fdc            move.l    $00005FDC,(a7)
[00004ee0] 4267                      clr.w     -(a7)
[00004ee2] 3f3c 0010                 move.w    #$0010,-(a7)
[00004ee6] 2f0e                      move.l    a6,-(a7)
[00004ee8] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004eee] 2f0d                      move.l    a5,-(a7)
[00004ef0] 4eb9 0000 4558            jsr       ___prtin
[00004ef6] dffc 0000 000c            adda.l    #$0000000C,a7
[00004efc] 4a6e fde8                 tst.w     -536(a6)
[00004f00] 6704                      beq.s     $00004F06
[00004f02] 7004                      moveq.l   #4,d0
[00004f04] 6002                      bra.s     $00004F08
[00004f06] 7002                      moveq.l   #2,d0
[00004f08] 48c0                      ext.l     d0
[00004f0a] dbc0                      adda.l    d0,a5
[00004f0c] 6000 00d2                 bra       $00004FE0
[00004f10] 2d4d fffc                 move.l    a5,-4(a6)
[00004f14] 206e fffc                 movea.l   -4(a6),a0
[00004f18] 2d50 fff2                 move.l    (a0),-14(a6)
[00004f1c] 588d                      addq.l    #4,a5
[00004f1e] 6000 00c0                 bra       $00004FE0
[00004f22] 2d4d fff8                 move.l    a5,-8(a6)
[00004f26] 206e fff8                 movea.l   -8(a6),a0
[00004f2a] 3010                      move.w    (a0),d0
[00004f2c] c07c 00ff                 and.w     #$00FF,d0
[00004f30] 1d40 fdf2                 move.b    d0,-526(a6)
[00004f34] 422e fdf3                 clr.b     -525(a6)
[00004f38] 548d                      addq.l    #2,a5
[00004f3a] 6000 00a4                 bra       $00004FE0
[00004f3e] 1007                      move.b    d7,d0
[00004f40] 4880                      ext.w     d0
[00004f42] 3e80                      move.w    d0,(a7)
[00004f44] 3f06                      move.w    d6,-(a7)
[00004f46] 2f0e                      move.l    a6,-(a7)
[00004f48] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004f4e] 2f0d                      move.l    a5,-(a7)
[00004f50] 4eb9 0000 4b9e            jsr       __petoa
[00004f56] dffc 0000 000a            adda.l    #$0000000A,a7
[00004f5c] 588d                      addq.l    #4,a5
[00004f5e] 7cff                      moveq.l   #-1,d6
[00004f60] 6000 007e                 bra       $00004FE0
[00004f64] 1007                      move.b    d7,d0
[00004f66] 4880                      ext.w     d0
[00004f68] 3e80                      move.w    d0,(a7)
[00004f6a] 3f06                      move.w    d6,-(a7)
[00004f6c] 2f0e                      move.l    a6,-(a7)
[00004f6e] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004f74] 2f0d                      move.l    a5,-(a7)
[00004f76] 4eb9 0000 4b66            jsr       __pftoa
[00004f7c] dffc 0000 000a            adda.l    #$0000000A,a7
[00004f82] 588d                      addq.l    #4,a5
[00004f84] 7cff                      moveq.l   #-1,d6
[00004f86] 6058                      bra.s     $00004FE0
[00004f88] 1007                      move.b    d7,d0
[00004f8a] 4880                      ext.w     d0
[00004f8c] 3e80                      move.w    d0,(a7)
[00004f8e] 3f06                      move.w    d6,-(a7)
[00004f90] 2f0e                      move.l    a6,-(a7)
[00004f92] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00004f98] 2f0d                      move.l    a5,-(a7)
[00004f9a] 4eb9 0000 4bd6            jsr       __pgtoa
[00004fa0] dffc 0000 000a            adda.l    #$0000000A,a7
[00004fa6] 588d                      addq.l    #4,a5
[00004fa8] 7cff                      moveq.l   #-1,d6
[00004faa] 6034                      bra.s     $00004FE0
[00004fac] 2eae 0008                 move.l    8(a6),(a7)
[00004fb0] 1007                      move.b    d7,d0
[00004fb2] 4880                      ext.w     d0
[00004fb4] 3f00                      move.w    d0,-(a7)
[00004fb6] 4eb9 0000 4ab0            jsr       _fputc
[00004fbc] 548f                      addq.l    #2,a7
[00004fbe] 526e fdec                 addq.w    #1,-532(a6)
[00004fc2] 6000 00d6                 bra       $0000509A
[00004fc6] 6018                      bra.s     $00004FE0
[00004fc8] 907c 0043                 sub.w     #$0043,d0
[00004fcc] b07c 0035                 cmp.w     #$0035,d0
[00004fd0] 62da                      bhi.s     $00004FAC
[00004fd2] e540                      asl.w     #2,d0
[00004fd4] 3040                      movea.w   d0,a0
[00004fd6] d1fc 0000 5dd8            adda.l    #$00005DD8,a0
[00004fdc] 2050                      movea.l   (a0),a0
[00004fde] 4ed0                      jmp       (a0)
[00004fe0] 2eae fff2                 move.l    -14(a6),(a7)
[00004fe4] 4eb9 0000 0954            jsr       _strlen
[00004fea] 3a00                      move.w    d0,d5
[00004fec] bc45                      cmp.w     d5,d6
[00004fee] 6c06                      bge.s     $00004FF6
[00004ff0] 4a46                      tst.w     d6
[00004ff2] 6d02                      blt.s     $00004FF6
[00004ff4] 3a06                      move.w    d6,d5
[00004ff6] 302e fdf0                 move.w    -528(a6),d0
[00004ffa] 9045                      sub.w     d5,d0
[00004ffc] 3d40 fdee                 move.w    d0,-530(a6)
[00005000] 4a6e fdea                 tst.w     -534(a6)
[00005004] 6658                      bne.s     $0000505E
[00005006] 0c2e 0030 fff6            cmpi.b    #$30,-10(a6)
[0000500c] 662a                      bne.s     $00005038
[0000500e] 206e fff2                 movea.l   -14(a6),a0
[00005012] 0c10 002d                 cmpi.b    #$2D,(a0)
[00005016] 6620                      bne.s     $00005038
[00005018] 5345                      subq.w    #1,d5
[0000501a] 2eae 0008                 move.l    8(a6),(a7)
[0000501e] 206e fff2                 movea.l   -14(a6),a0
[00005022] 1010                      move.b    (a0),d0
[00005024] 4880                      ext.w     d0
[00005026] 3f00                      move.w    d0,-(a7)
[00005028] 4eb9 0000 4ab0            jsr       _fputc
[0000502e] 548f                      addq.l    #2,a7
[00005030] 52ae fff2                 addq.l    #1,-14(a6)
[00005034] 526e fdec                 addq.w    #1,-532(a6)
[00005038] 6018                      bra.s     $00005052
[0000503a] 2eae 0008                 move.l    8(a6),(a7)
[0000503e] 102e fff6                 move.b    -10(a6),d0
[00005042] 4880                      ext.w     d0
[00005044] 3f00                      move.w    d0,-(a7)
[00005046] 4eb9 0000 4ab0            jsr       _fputc
[0000504c] 548f                      addq.l    #2,a7
[0000504e] 526e fdec                 addq.w    #1,-532(a6)
[00005052] 302e fdee                 move.w    -530(a6),d0
[00005056] 536e fdee                 subq.w    #1,-530(a6)
[0000505a] 4a40                      tst.w     d0
[0000505c] 6edc                      bgt.s     $0000503A
[0000505e] 2eae 0008                 move.l    8(a6),(a7)
[00005062] 3f05                      move.w    d5,-(a7)
[00005064] 2f2e fff2                 move.l    -14(a6),-(a7)
[00005068] 4eb9 0000 4af6            jsr       _fputn
[0000506e] 5c8f                      addq.l    #6,a7
[00005070] db6e fdec                 add.w     d5,-532(a6)
[00005074] 6018                      bra.s     $0000508E
[00005076] 2eae 0008                 move.l    8(a6),(a7)
[0000507a] 102e fff6                 move.b    -10(a6),d0
[0000507e] 4880                      ext.w     d0
[00005080] 3f00                      move.w    d0,-(a7)
[00005082] 4eb9 0000 4ab0            jsr       _fputc
[00005088] 548f                      addq.l    #2,a7
[0000508a] 526e fdec                 addq.w    #1,-532(a6)
[0000508e] 302e fdee                 move.w    -530(a6),d0
[00005092] 536e fdee                 subq.w    #1,-530(a6)
[00005096] 4a40                      tst.w     d0
[00005098] 6edc                      bgt.s     $00005076
[0000509a] 6000 fba0                 bra       $00004C3C
[0000509e] 302e fdec                 move.w    -532(a6),d0
[000050a2] 4a9f                      tst.l     (a7)+
[000050a4] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[000050a8] 4e5e                      unlk      a6
[000050aa] 4e75                      rts       
[000050ac] _lseek:
[000050ac] 4e56 0000                 link      a6,#$0000
[000050b0] 48e7 0104                 movem.l   d7/a5,-(a7)
[000050b4] 3eae 0008                 move.w    8(a6),(a7)
[000050b8] 4eb9 0000 42cc            jsr       __chkc
[000050be] 2a40                      movea.l   d0,a5
[000050c0] 200d                      move.l    a5,d0
[000050c2] 6616                      bne.s     $000050DA
[000050c4] 33fc 0009 0000 5da2       move.w    #$0009,_errno
[000050cc] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[000050d6] 70ff                      moveq.l   #-1,d0
[000050d8] 6024                      bra.s     $000050FE
[000050da] 3eae 000e                 move.w    14(a6),(a7)
[000050de] 3f2d 0004                 move.w    4(a5),-(a7)
[000050e2] 2f2e 000a                 move.l    10(a6),-(a7)
[000050e6] 3f3c 0042                 move.w    #$0042,-(a7)
[000050ea] 4eb9 0000 3a64            jsr       _trap
[000050f0] 508f                      addq.l    #8,a7
[000050f2] 2b40 0006                 move.l    d0,6(a5)
[000050f6] 0255 ffdf                 andi.w    #$FFDF,(a5)
[000050fa] 202d 0006                 move.l    6(a5),d0
[000050fe] 4a9f                      tst.l     (a7)+
[00005100] 4cdf 2000                 movem.l   (a7)+,a5
[00005104] 4e5e                      unlk      a6
[00005106] 4e75                      rts       
[00005108] _tell:
[00005108] 4e56 fffc                 link      a6,#$FFFC
[0000510c] 3ebc 0001                 move.w    #$0001,(a7)
[00005110] 42a7                      clr.l     -(a7)
[00005112] 3f2e 0008                 move.w    8(a6),-(a7)
[00005116] 6194                      bsr.s     _lseek
[00005118] 5c8f                      addq.l    #6,a7
[0000511a] 4e5e                      unlk      a6
[0000511c] 4e75                      rts       
[0000511e] __open:
[0000511e] 4e56 0000                 link      a6,#$0000
[00005122] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00005126] 4eb9 0000 41d6            jsr       __allocc
[0000512c] 3e00                      move.w    d0,d7
[0000512e] be7c ffff                 cmp.w     #$FFFF,d7
[00005132] 6606                      bne.s     $0000513A
[00005134] 70ff                      moveq.l   #-1,d0
[00005136] 6000 00d6                 bra       $0000520E
[0000513a] 3e87                      move.w    d7,(a7)
[0000513c] 4eb9 0000 425e            jsr       ___chini
[00005142] 3007                      move.w    d7,d0
[00005144] c1fc 0238                 muls.w    #$0238,d0
[00005148] 2a40                      movea.l   d0,a5
[0000514a] dbfc 0000 618e            adda.l    #__fds,a5
[00005150] 4a6e 000c                 tst.w     12(a6)
[00005154] 6604                      bne.s     $0000515A
[00005156] 0055 0008                 ori.w     #$0008,(a5)
[0000515a] 4a6e 000e                 tst.w     14(a6)
[0000515e] 6604                      bne.s     $00005164
[00005160] 0055 0010                 ori.w     #$0010,(a5)
[00005164] 2ebc 0000 58ef            move.l    #___tname,(a7)
[0000516a] 2f2e 0008                 move.l    8(a6),-(a7)
[0000516e] 4eb9 0000 3b96            jsr       __strcmp
[00005174] 588f                      addq.l    #4,a7
[00005176] 4a40                      tst.w     d0
[00005178] 6612                      bne.s     $0000518C
[0000517a] 0055 0003                 ori.w     #$0003,(a5)
[0000517e] 3b6e 000c 0004            move.w    12(a6),4(a5)
[00005184] 3007                      move.w    d7,d0
[00005186] 6000 0086                 bra       $0000520E
[0000518a] 601e                      bra.s     $000051AA
[0000518c] 2ebc 0000 58f4            move.l    #___lname,(a7)
[00005192] 2f2e 0008                 move.l    8(a6),-(a7)
[00005196] 4eb9 0000 3b96            jsr       __strcmp
[0000519c] 588f                      addq.l    #4,a7
[0000519e] 4a40                      tst.w     d0
[000051a0] 6608                      bne.s     $000051AA
[000051a2] 0055 0005                 ori.w     #$0005,(a5)
[000051a6] 3007                      move.w    d7,d0
[000051a8] 6064                      bra.s     $0000520E
[000051aa] 3ebc 000f                 move.w    #$000F,(a7)
[000051ae] 2f2e 0008                 move.l    8(a6),-(a7)
[000051b2] 3f07                      move.w    d7,-(a7)
[000051b4] 4eb9 0000 3ca4            jsr       ___open
[000051ba] 5c8f                      addq.l    #6,a7
[000051bc] 4a40                      tst.w     d0
[000051be] 671e                      beq.s     $000051DE
[000051c0] 3e87                      move.w    d7,(a7)
[000051c2] 4eb9 0000 4220            jsr       __freec
[000051c8] 33fc 0002 0000 5da2       move.w    #$0002,_errno
[000051d0] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[000051da] 70ff                      moveq.l   #-1,d0
[000051dc] 6030                      bra.s     $0000520E
[000051de] 0055 0001                 ori.w     #$0001,(a5)
[000051e2] 3ebc 0002                 move.w    #$0002,(a7)
[000051e6] 42a7                      clr.l     -(a7)
[000051e8] 102d 0003                 move.b    3(a5),d0
[000051ec] 4880                      ext.w     d0
[000051ee] 3f00                      move.w    d0,-(a7)
[000051f0] 4eb9 0000 50ac            jsr       _lseek
[000051f6] 5c8f                      addq.l    #6,a7
[000051f8] 4257                      clr.w     (a7)
[000051fa] 42a7                      clr.l     -(a7)
[000051fc] 102d 0003                 move.b    3(a5),d0
[00005200] 4880                      ext.w     d0
[00005202] 3f00                      move.w    d0,-(a7)
[00005204] 4eb9 0000 50ac            jsr       _lseek
[0000520a] 5c8f                      addq.l    #6,a7
[0000520c] 3007                      move.w    d7,d0
[0000520e] 4a9f                      tst.l     (a7)+
[00005210] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00005214] 4e5e                      unlk      a6
[00005216] 4e75                      rts       
[00005218] _open:
[00005218] 4e56 fffc                 link      a6,#$FFFC
[0000521c] 4257                      clr.w     (a7)
[0000521e] 3f2e 000c                 move.w    12(a6),-(a7)
[00005222] 2f2e 0008                 move.l    8(a6),-(a7)
[00005226] 6100 fef6                 bsr       __open
[0000522a] 5c8f                      addq.l    #6,a7
[0000522c] 4e5e                      unlk      a6
[0000522e] 4e75                      rts       
[00005230] _opena:
[00005230] 4e56 fffc                 link      a6,#$FFFC
[00005234] 4257                      clr.w     (a7)
[00005236] 3f2e 000c                 move.w    12(a6),-(a7)
[0000523a] 2f2e 0008                 move.l    8(a6),-(a7)
[0000523e] 6100 fede                 bsr       __open
[00005242] 5c8f                      addq.l    #6,a7
[00005244] 4e5e                      unlk      a6
[00005246] 4e75                      rts       
[00005248] _openb:
[00005248] 4e56 fffc                 link      a6,#$FFFC
[0000524c] 3ebc 0001                 move.w    #$0001,(a7)
[00005250] 3f2e 000c                 move.w    12(a6),-(a7)
[00005254] 2f2e 0008                 move.l    8(a6),-(a7)
[00005258] 6100 fec4                 bsr       __open
[0000525c] 5c8f                      addq.l    #6,a7
[0000525e] 4e5e                      unlk      a6
[00005260] 4e75                      rts       
[00005262] _fflush:
[00005262] 4e56 0000                 link      a6,#$0000
[00005266] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[0000526a] 2a6e 0008                 movea.l   8(a6),a5
[0000526e] 302d 0002                 move.w    2(a5),d0
[00005272] c07c 000a                 and.w     #$000A,d0
[00005276] b07c 0002                 cmp.w     #$0002,d0
[0000527a] 662c                      bne.s     $000052A8
[0000527c] 202d 0008                 move.l    8(a5),d0
[00005280] 90ad 0004                 sub.l     4(a5),d0
[00005284] 3c00                      move.w    d0,d6
[00005286] 6f20                      ble.s     $000052A8
[00005288] 3e86                      move.w    d6,(a7)
[0000528a] 2f2d 0004                 move.l    4(a5),-(a7)
[0000528e] 3f15                      move.w    (a5),-(a7)
[00005290] 4eb9 0000 432e            jsr       _write
[00005296] 5c8f                      addq.l    #6,a7
[00005298] 3e00                      move.w    d0,d7
[0000529a] bc47                      cmp.w     d7,d6
[0000529c] 670a                      beq.s     $000052A8
[0000529e] 006d 0010 0002            ori.w     #$0010,2(a5)
[000052a4] 70ff                      moveq.l   #-1,d0
[000052a6] 604a                      bra.s     $000052F2
[000052a8] 082d 0001 0003            btst      #1,3(a5)
[000052ae] 671e                      beq.s     $000052CE
[000052b0] 4aad 0004                 tst.l     4(a5)
[000052b4] 6716                      beq.s     $000052CC
[000052b6] 082d 0003 0003            btst      #3,3(a5)
[000052bc] 6708                      beq.s     $000052C6
[000052be] 3b7c 0001 000c            move.w    #$0001,12(a5)
[000052c4] 6006                      bra.s     $000052CC
[000052c6] 3b7c 01ff 000c            move.w    #$01FF,12(a5)
[000052cc] 601c                      bra.s     $000052EA
[000052ce] 3ebc 0001                 move.w    #$0001,(a7)
[000052d2] 302d 000c                 move.w    12(a5),d0
[000052d6] 4440                      neg.w     d0
[000052d8] 48c0                      ext.l     d0
[000052da] 2f00                      move.l    d0,-(a7)
[000052dc] 3f15                      move.w    (a5),-(a7)
[000052de] 4eb9 0000 50ac            jsr       _lseek
[000052e4] 5c8f                      addq.l    #6,a7
[000052e6] 426d 000c                 clr.w     12(a5)
[000052ea] 2b6d 0004 0008            move.l    4(a5),8(a5)
[000052f0] 4240                      clr.w     d0
[000052f2] 4a9f                      tst.l     (a7)+
[000052f4] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[000052f8] 4e5e                      unlk      a6
[000052fa] 4e75                      rts       
[000052fc] ___fdecl:
[000052fc] 4e56 fffc                 link      a6,#$FFFC
[00005300] 4e5e                      unlk      a6
[00005302] 4e75                      rts       
[00005304] _close:
[00005304] 4e56 0000                 link      a6,#$0000
[00005308] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[0000530c] 3e2e 0008                 move.w    8(a6),d7
[00005310] 3e87                      move.w    d7,(a7)
[00005312] 4eb9 0000 42cc            jsr       __chkc
[00005318] 2a40                      movea.l   d0,a5
[0000531a] 200d                      move.l    a5,d0
[0000531c] 6616                      bne.s     $00005334
[0000531e] 33fc 0009 0000 5da2       move.w    #$0009,_errno
[00005326] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[00005330] 70ff                      moveq.l   #-1,d0
[00005332] 604e                      bra.s     $00005382
[00005334] 4246                      clr.w     d6
[00005336] 3015                      move.w    (a5),d0
[00005338] c07c 0006                 and.w     #$0006,d0
[0000533c] 6616                      bne.s     $00005354
[0000533e] 3ead 0004                 move.w    4(a5),(a7)
[00005342] 3f3c 003e                 move.w    #$003E,-(a7)
[00005346] 4eb9 0000 3a64            jsr       _trap
[0000534c] 548f                      addq.l    #2,a7
[0000534e] 3c00                      move.w    d0,d6
[00005350] 6c02                      bge.s     $00005354
[00005352] 7cff                      moveq.l   #-1,d6
[00005354] 3e87                      move.w    d7,(a7)
[00005356] 4eb9 0000 425e            jsr       ___chini
[0000535c] 3e87                      move.w    d7,(a7)
[0000535e] 4eb9 0000 4220            jsr       __freec
[00005364] 4a46                      tst.w     d6
[00005366] 6606                      bne.s     $0000536E
[00005368] 3006                      move.w    d6,d0
[0000536a] 6016                      bra.s     $00005382
[0000536c] 6014                      bra.s     $00005382
[0000536e] 33fc 0005 0000 5da2       move.w    #$0005,_errno
[00005376] 33f9 0000 5fd2 0000 5da4  move.w    ___cpmrv,__errcpm
[00005380] 70ff                      moveq.l   #-1,d0
[00005382] 4a9f                      tst.l     (a7)+
[00005384] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00005388] 4e5e                      unlk      a6
[0000538a] 4e75                      rts       
[0000538c] _fclose:
[0000538c] 4e56 0000                 link      a6,#$0000
[00005390] 48e7 0104                 movem.l   d7/a5,-(a7)
[00005394] 2a6e 0008                 movea.l   8(a6),a5
[00005398] 302d 0002                 move.w    2(a5),d0
[0000539c] c07c 0003                 and.w     #$0003,d0
[000053a0] 672a                      beq.s     $000053CC
[000053a2] 2e8d                      move.l    a5,(a7)
[000053a4] 4eb9 0000 5262            jsr       _fflush
[000053aa] 082d 0002 0003            btst      #2,3(a5)
[000053b0] 670a                      beq.s     $000053BC
[000053b2] 2ead 0004                 move.l    4(a5),(a7)
[000053b6] 4eb9 0000 4768            jsr       _free
[000053bc] 4240                      clr.w     d0
[000053be] 48c0                      ext.l     d0
[000053c0] 2b40 0008                 move.l    d0,8(a5)
[000053c4] 2b40 0004                 move.l    d0,4(a5)
[000053c8] 426d 000c                 clr.w     12(a5)
[000053cc] 026d ff80 0002            andi.w    #$FF80,2(a5)
[000053d2] 3e95                      move.w    (a5),(a7)
[000053d4] 4eb9 0000 5304            jsr       _close
[000053da] 4a9f                      tst.l     (a7)+
[000053dc] 4cdf 2000                 movem.l   (a7)+,a5
[000053e0] 4e5e                      unlk      a6
[000053e2] 4e75                      rts       
[000053e4] __cleanu:
[000053e4] 4e56 0000                 link      a6,#$0000
[000053e8] 48e7 0300                 movem.l   d6-d7,-(a7)
[000053ec] 4247                      clr.w     d7
[000053ee] 6016                      bra.s     $00005406
[000053f0] 3007                      move.w    d7,d0
[000053f2] c1fc 000e                 muls.w    #$000E,d0
[000053f6] d0bc 0000 5eb0            add.l     #__iob,d0
[000053fc] 2e80                      move.l    d0,(a7)
[000053fe] 4eb9 0000 538c            jsr       _fclose
[00005404] 5247                      addq.w    #1,d7
[00005406] be7c 0010                 cmp.w     #$0010,d7
[0000540a] 6de4                      blt.s     $000053F0
[0000540c] 4a9f                      tst.l     (a7)+
[0000540e] 4cdf 0080                 movem.l   (a7)+,d7
[00005412] 4e5e                      unlk      a6
[00005414] 4e75                      rts       
[00005416] _exit:
[00005416] 4e56 fffc                 link      a6,#$FFFC
[0000541a] 4eb9 0000 53e4            jsr       __cleanu
[00005420] 3eae 0008                 move.w    8(a6),(a7)
[00005424] 4eb9 0000 008a            jsr       __exit
[0000542a] 4e5e                      unlk      a6
[0000542c] 4e75                      rts       
[0000542e] ___main:
[0000542e] 4e56 ffdc                 link      a6,#$FFDC
[00005432] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00005436] 4257                      clr.w     (a7)
[00005438] 4eb9 0000 43ca            jsr       _sbrk
[0000543e] 23c0 0000 5fe2            move.l    d0,$00005FE2
[00005444] 23c0 0000 5fe6            move.l    d0,$00005FE6
[0000544a] 4279 0000 5fe0            clr.w     $00005FE0
[00005450] 2ebc 0000 58e7            move.l    #___pname,(a7)
[00005456] 4eb9 0000 5764            jsr       $00005764
[0000545c] 2a6e 0008                 movea.l   8(a6),a5
[00005460] 6000 0238                 bra       $0000569A
for (s = com...
[00005464] 6000 0008                 bra       $0000546E
[00005468] dbfc 0000 0001            adda.l    #$00000001,a5
[0000546e] 4a15                      tst.b     (a5)
[00005470] 6700 0016                 beq       $00005488
[00005474] 1015                      move.b    (a5),d0
[00005476] 4880                      ext.w     d0
[00005478] 48c0                      ext.l     d0
[0000547a] d0bc 0000 5d22            add.l     #___atab,d0
[00005480] 2040                      movea.l   d0,a0
[00005482] 0810 0005                 btst      #5,(a0)
[00005486] 66e0                      bne.s     $00005468
[00005488] 1c15                      move.b    (a5),d6
[0000548a] 4a06                      tst.b     d6
[0000548c] 6700 0212                 beq       $000056A0
[00005490] 1006                      move.b    d6,d0
[00005492] 4880                      ext.w     d0
[00005494] 3e80                      move.w    d0,(a7)
[00005496] 2f3c 0000 5f90            move.l    #$00005F90,-(a7)
[0000549c] 4eb9 0000 0162            jsr       _strchr
[000054a2] 588f                      addq.l    #4,a7
[000054a4] 4a80                      tst.l     d0
[000054a6] 6700 0052                 beq       $000054FA
[000054aa] 1006                      move.b    d6,d0
[000054ac] 4880                      ext.w     d0
[000054ae] 3e80                      move.w    d0,(a7)
[000054b0] 2f0d                      move.l    a5,-(a7)
[000054b2] 0697 0000 0001            addi.l    #$00000001,(a7)
[000054b8] 4eb9 0000 0162            jsr       _strchr
[000054be] 588f                      addq.l    #4,a7
[000054c0] 2840                      movea.l   d0,a4
[000054c2] 200c                      move.l    a4,d0
[000054c4] 6600 0012                 bne       $000054D8
[000054c8] 2ebc 0000 5f93            move.l    #$00005F93,(a7)
[000054ce] 2f0d                      move.l    a5,-(a7)
[000054d0] 4eb9 0000 5700            jsr       $00005700
[000054d6] 588f                      addq.l    #4,a7
[000054d8] 200c                      move.l    a4,d0
[000054da] 908d                      sub.l     a5,d0
[000054dc] 3e00                      move.w    d0,d7
[000054de] 3047                      movea.w   d7,a0
[000054e0] d1cd                      adda.l    a5,a0
[000054e2] 4210                      clr.b     (a0)
[000054e4] de7c 0001                 add.w     #$0001,d7
[000054e8] 2e8d                      move.l    a5,(a7)
[000054ea] 0697 0000 0001            addi.l    #$00000001,(a7)
[000054f0] 4eb9 0000 5764            jsr       $00005764
[000054f6] 6000 01a0                 bra       $00005698
[000054fa] 4247                      clr.w     d7
[000054fc] 6000 0006                 bra       $00005504
[00005500] de7c 0001                 add.w     #$0001,d7
[00005504] 3007                      move.w    d7,d0
[00005506] 48c0                      ext.l     d0
[00005508] 1035 0800                 move.b    0(a5,d0.l),d0
[0000550c] 4880                      ext.w     d0
[0000550e] 48c0                      ext.l     d0
[00005510] 4a80                      tst.l     d0
[00005512] 6700 001c                 beq       $00005530
[00005516] 3007                      move.w    d7,d0
[00005518] 48c0                      ext.l     d0
[0000551a] 1035 0800                 move.b    0(a5,d0.l),d0
[0000551e] 4880                      ext.w     d0
[00005520] 48c0                      ext.l     d0
[00005522] d0bc 0000 5d22            add.l     #___atab,d0
[00005528] 2040                      movea.l   d0,a0
[0000552a] 0810 0005                 btst      #5,(a0)
[0000552e] 67d0                      beq.s     $00005500
[00005530] 3007                      move.w    d7,d0
[00005532] 48c0                      ext.l     d0
[00005534] 4a35 0800                 tst.b     0(a5,d0.l)
[00005538] 6700 000c                 beq       $00005546
[0000553c] 3047                      movea.w   d7,a0
[0000553e] d1cd                      adda.l    a5,a0
[00005540] 4210                      clr.b     (a0)
[00005542] de7c 0001                 add.w     #$0001,d7
[00005546] 3ebc 003f                 move.w    #$003F,(a7)
[0000554a] 2f0d                      move.l    a5,-(a7)
[0000554c] 4eb9 0000 0162            jsr       _strchr
[00005552] 588f                      addq.l    #4,a7
[00005554] 4a80                      tst.l     d0
[00005556] 6600 0016                 bne       $0000556E
[0000555a] 3ebc 002a                 move.w    #$002A,(a7)
[0000555e] 2f0d                      move.l    a5,-(a7)
[00005560] 4eb9 0000 0162            jsr       _strchr
[00005566] 588f                      addq.l    #4,a7
[00005568] 4a80                      tst.l     d0
[0000556a] 6700 0124                 beq       $00005690
[0000556e] 2d7c 0000 6836 fffc       move.l    #$00006836,-4(a6)
[00005576] 2eae fffc                 move.l    -4(a6),(a7)
[0000557a] 0697 0000 0038            addi.l    #$00000038,(a7)
[00005580] 3f3c 001a                 move.w    #$001A,-(a7)
[00005584] 4eb9 0000 3a64            jsr       _trap
[0000558a] 548f                      addq.l    #2,a7
[0000558c] 3ebc 0011                 move.w    #$0011,(a7)
[00005590] 2f0d                      move.l    a5,-(a7)
[00005592] 3f3c 0003                 move.w    #$0003,-(a7)
[00005596] 4eb9 0000 3ca4            jsr       ___open
[0000559c] 5c8f                      addq.l    #6,a7
[0000559e] 4880                      ext.w     d0
[000055a0] 1c00                      move.b    d0,d6
[000055a2] bc3c 00ff                 cmp.b     #$FF,d6
[000055a6] 6600 0012                 bne       $000055BA
[000055aa] 2ebc 0000 5fa5            move.l    #$00005FA5,(a7)
[000055b0] 2f0d                      move.l    a5,-(a7)
[000055b2] 4eb9 0000 5700            jsr       $00005700
[000055b8] 588f                      addq.l    #4,a7
[000055ba] 2e8d                      move.l    a5,(a7)
[000055bc] 4eb9 0000 0954            jsr       _strlen
[000055c2] 3d40 ffdc                 move.w    d0,-36(a6)
[000055c6] 6000 002e                 bra       $000055F6
[000055ca] 302e ffdc                 move.w    -36(a6),d0
[000055ce] 907c 0001                 sub.w     #$0001,d0
[000055d2] 48c0                      ext.l     d0
[000055d4] 1035 0800                 move.b    0(a5,d0.l),d0
[000055d8] 4880                      ext.w     d0
[000055da] 3e80                      move.w    d0,(a7)
[000055dc] 2f3c 0000 5fb0            move.l    #$00005FB0,-(a7)
[000055e2] 4eb9 0000 0162            jsr       _strchr
[000055e8] 588f                      addq.l    #4,a7
[000055ea] 4a80                      tst.l     d0
[000055ec] 6600 000e                 bne       $000055FC
[000055f0] 046e 0001 ffdc            subi.w    #$0001,-36(a6)
[000055f6] 4a6e ffdc                 tst.w     -36(a6)
[000055fa] 66ce                      bne.s     $000055CA
[000055fc] 6000 0086                 bra       $00005684
[00005600] 2e8e                      move.l    a6,(a7)
[00005602] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[00005608] 1006                      move.b    d6,d0
[0000560a] 4880                      ext.w     d0
[0000560c] 3f00                      move.w    d0,-(a7)
[0000560e] 2f2e fffc                 move.l    -4(a6),-(a7)
[00005612] 4eb9 0000 57d2            jsr       $000057D2
[00005618] 5c8f                      addq.l    #6,a7
[0000561a] 2e8e                      move.l    a6,(a7)
[0000561c] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[00005622] 4eb9 0000 0954            jsr       _strlen
[00005628] 3e80                      move.w    d0,(a7)
[0000562a] 302e ffdc                 move.w    -36(a6),d0
[0000562e] d157                      add.w     d0,(a7)
[00005630] 0657 0001                 addi.w    #$0001,(a7)
[00005634] 4eb9 0000 3b68            jsr       __salloc
[0000563a] 2840                      movea.l   d0,a4
[0000563c] 3eae ffdc                 move.w    -36(a6),(a7)
[00005640] 2f0d                      move.l    a5,-(a7)
[00005642] 2f0c                      move.l    a4,-(a7)
[00005644] 4eb9 0000 3c32            jsr       _strncpy
[0000564a] 508f                      addq.l    #8,a7
[0000564c] 306e ffdc                 movea.w   -36(a6),a0
[00005650] d1cc                      adda.l    a4,a0
[00005652] 4210                      clr.b     (a0)
[00005654] 2e8e                      move.l    a6,(a7)
[00005656] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[0000565c] 2f0c                      move.l    a4,-(a7)
[0000565e] 4eb9 0000 3c7a            jsr       _strcat
[00005664] 588f                      addq.l    #4,a7
[00005666] 2e8c                      move.l    a4,(a7)
[00005668] 4eb9 0000 5764            jsr       $00005764
[0000566e] 3ebc 0012                 move.w    #$0012,(a7)
[00005672] 2f0d                      move.l    a5,-(a7)
[00005674] 3f3c 0003                 move.w    #$0003,-(a7)
[00005678] 4eb9 0000 3ca4            jsr       ___open
[0000567e] 5c8f                      addq.l    #6,a7
[00005680] 4880                      ext.w     d0
[00005682] 1c00                      move.b    d0,d6
[00005684] bc3c 00ff                 cmp.b     #$FF,d6
[00005688] 6600 ff76                 bne       $00005600
[0000568c] 6000 000a                 bra       $00005698
[00005690] 2e8d                      move.l    a5,(a7)
[00005692] 4eb9 0000 5764            jsr       $00005764
[00005698] dac7                      adda.w    d7,a5
[0000569a] 4a15                      tst.b     (a5)
[0000569c] 6600 fdc6                 bne       $00005464
[000056a0] 4297                      clr.l     (a7)
[000056a2] 4eb9 0000 5764            jsr       $00005764
[000056a8] 0479 0001 0000 5fe0       subi.w    #$0001,$00005FE0
[000056b0] 2eb9 0000 5fe6            move.l    $00005FE6,(a7)
[000056b6] 4eb9 0000 00ac            jsr       _brk
[000056bc] b07c ffff                 cmp.w     #$FFFF,d0
[000056c0] 6600 0016                 bne       $000056D8
[000056c4] 2ebc 0000 5fc2            move.l    #$00005FC2,(a7)
[000056ca] 2f3c 0000 5fb3            move.l    #$00005FB3,-(a7)
[000056d0] 4eb9 0000 5700            jsr       $00005700
[000056d6] 588f                      addq.l    #4,a7
[000056d8] 4297                      clr.l     (a7)
[000056da] 2f39 0000 5fe2            move.l    $00005FE2,-(a7)
[000056e0] 3f39 0000 5fe0            move.w    $00005FE0,-(a7)
[000056e6] 4eb9 0000 04ce            jsr       _main
[000056ec] 5c8f                      addq.l    #6,a7
[000056ee] 3e80                      move.w    d0,(a7)
[000056f0] 4eb9 0000 5416            jsr       _exit
[000056f6] 4a9f                      tst.l     (a7)+
[000056f8] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[000056fc] 4e5e                      unlk      a6
[000056fe] 4e75                      rts       
[00005700] 4e56 ff7c                 link      a6,#$FF7C
[00005704] 2eae 0008                 move.l    8(a6),(a7)
[00005708] 2f0e                      move.l    a6,-(a7)
[0000570a] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[00005710] 4eb9 0000 3c10            jsr       _strcpy
[00005716] 588f                      addq.l    #4,a7
[00005718] 2eae 000c                 move.l    12(a6),(a7)
[0000571c] 2f0e                      move.l    a6,-(a7)
[0000571e] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[00005724] 4eb9 0000 3c7a            jsr       _strcat
[0000572a] 588f                      addq.l    #4,a7
[0000572c] 2ebc 0000 5fc3            move.l    #$00005FC3,(a7)
[00005732] 2f0e                      move.l    a6,-(a7)
[00005734] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[0000573a] 4eb9 0000 3c7a            jsr       _strcat
[00005740] 588f                      addq.l    #4,a7
[00005742] 2e8e                      move.l    a6,(a7)
[00005744] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[0000574a] 3f3c 0009                 move.w    #$0009,-(a7)
[0000574e] 4eb9 0000 3a64            jsr       _trap
[00005754] 548f                      addq.l    #2,a7
[00005756] 3ebc ffff                 move.w    #$FFFF,(a7)
[0000575a] 4eb9 0000 5416            jsr       _exit
[00005760] 4e5e                      unlk      a6
[00005762] 4e75                      rts       

[00005764] 4e56 0000                 link      a6,#$0000
[00005768] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0000576c] 2a6e 0008                 movea.l   8(a6),a5
[00005770] 200d                      move.l    a5,d0
[00005772] 6700 003a                 beq       $000057AE
[00005776] 284d                      movea.l   a5,a4
[00005778] 6000 0008                 bra       $00005782
[0000577c] d9fc 0000 0001            adda.l    #$00000001,a4
[00005782] 1014                      move.b    (a4),d0
[00005784] 4880                      ext.w     d0
[00005786] 48c0                      ext.l     d0
[00005788] d0bc 0000 5d22            add.l     #___atab,d0
[0000578e] 2040                      movea.l   d0,a0
[00005790] 0810 0003                 btst      #3,(a0)
[00005794] 6700 000e                 beq       $000057A4
[00005798] 1014                      move.b    (a4),d0
[0000579a] 4880                      ext.w     d0
[0000579c] d07c 0020                 add.w     #$0020,d0
[000057a0] 6000 0006                 bra       $000057A8
[000057a4] 1014                      move.b    (a4),d0
[000057a6] 4880                      ext.w     d0
[000057a8] 4880                      ext.w     d0
[000057aa] 1880                      move.b    d0,(a4)
[000057ac] 66ce                      bne.s     $0000577C
[000057ae] 2079 0000 5fe6            movea.l   $00005FE6,a0
[000057b4] 208d                      move.l    a5,(a0)
[000057b6] 06b9 0000 0004 0000 5fe6  addi.l    #$00000004,$00005FE6
[000057c0] 0679 0001 0000 5fe0       addi.w    #$0001,$00005FE0
[000057c8] 4a9f                      tst.l     (a7)+
[000057ca] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000057ce] 4e5e                      unlk      a6
[000057d0] 4e75                      rts       
[000057d2] 4e56 0000                 link      a6,#$0000
[000057d6] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[000057da] 2a6e 0008                 movea.l   8(a6),a5
[000057de] 1e2e 000d                 move.b    13(a6),d7
[000057e2] 286e 000e                 movea.l   14(a6),a4
[000057e6] 47ed 0056                 lea.l     86(a5),a3
[000057ea] 6000 0002                 bra       $000057EE
[000057ee] 18db                      move.b    (a3)+,(a4)+
[000057f0] 66fc                      bne.s     $000057EE
[000057f2] 4a9f                      tst.l     (a7)+
[000057f4] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[000057f8] 4e5e                      unlk      a6
[000057fa] 4e75                      rts       
[000057fc] _printf:
[000057fc] 4e56 fffc                 link      a6,#$FFFC
[00005800] 2e8e                      move.l    a6,(a7)
[00005802] 0697 0000 000c            addi.l    #$0000000C,(a7)
[00005808] 2f2e 0008                 move.l    8(a6),-(a7)
[0000580c] 2f3c 0000 5ebe            move.l    #$00005EBE,-(a7)
[00005812] 4eb9 0000 4c20            jsr       __doprt
[00005818] 508f                      addq.l    #8,a7
[0000581a] 4e5e                      unlk      a6
[0000581c] 4e75                      rts       
[0000581e] _fprintf:
[0000581e] 4e56 fffc                 link      a6,#$FFFC
[00005822] 2e8e                      move.l    a6,(a7)
[00005824] 0697 0000 0010            addi.l    #$00000010,(a7)
[0000582a] 2f2e 000c                 move.l    12(a6),-(a7)
[0000582e] 2f2e 0008                 move.l    8(a6),-(a7)
[00005832] 4eb9 0000 4c20            jsr       __doprt
[00005838] 508f                      addq.l    #8,a7
[0000583a] 4e5e                      unlk      a6
[0000583c] 4e75                      rts       
[0000583e] __main:
[0000583e] 4e56 0000                 link      a6,#$0000
[00005842] 48e7 0104                 movem.l   d7/a5,-(a7)
[00005846] 4eb9 0000 423c            jsr       __chinit
[0000584c] 4257                      clr.w     (a7)
[0000584e] 2f3c 0000 58ef            move.l    #___tname,-(a7)
[00005854] 4eb9 0000 5218            jsr       _open
[0000585a] 588f                      addq.l    #4,a7
[0000585c] 3ebc 0001                 move.w    #$0001,(a7)
[00005860] 2f3c 0000 58ef            move.l    #___tname,-(a7)
[00005866] 4eb9 0000 5218            jsr       _open
[0000586c] 588f                      addq.l    #4,a7
[0000586e] 3ebc 0001                 move.w    #$0001,(a7)
[00005872] 2f3c 0000 58ef            move.l    #___tname,-(a7)
[00005878] 4eb9 0000 5218            jsr       _open
[0000587e] 588f                      addq.l    #4,a7
[00005880] 206e 0008                 movea.l   8(a6),a0
[00005884] 326e 000c                 movea.w   12(a6),a1
[00005888] d1c9                      adda.l    a1,a0
[0000588a] 4210                      clr.b     (a0)
[0000588c] 2a6e 0008                 movea.l   8(a6),a5
[00005890] 6026                      bra.s     $000058B8
[00005892] 1015                      move.b    (a5),d0
[00005894] 4880                      ext.w     d0
[00005896] 48c0                      ext.l     d0
[00005898] d0bc 0000 5d22            add.l     #___atab,d0
[0000589e] 2040                      movea.l   d0,a0
[000058a0] 0810 0003                 btst      #3,(a0)
[000058a4] 670a                      beq.s     $000058B0
[000058a6] 1015                      move.b    (a5),d0
[000058a8] 4880                      ext.w     d0
[000058aa] d07c 0020                 add.w     #$0020,d0
[000058ae] 6004                      bra.s     $000058B4
[000058b0] 1015                      move.b    (a5),d0
[000058b2] 4880                      ext.w     d0
[000058b4] 1a80                      move.b    d0,(a5)
[000058b6] 528d                      addq.l    #1,a5
[000058b8] 4a15                      tst.b     (a5)
[000058ba] 66d6                      bne.s     $00005892
[000058bc] 3eae 000c                 move.w    12(a6),(a7)
[000058c0] 2f2e 0008                 move.l    8(a6),-(a7)
[000058c4] 4eb9 0000 542e            jsr       ___main
[000058ca] 588f                      addq.l    #4,a7
[000058cc] 4a9f                      tst.l     (a7)+
[000058ce] 4cdf 2000                 movem.l   (a7)+,a5
[000058d2] 4e5e                      unlk      a6
[000058d4] 4e75                      rts       

data:
[000058d6] 5374 6163 6b20 4f76 6572  subq.w    #1,([$6B20,a4],$4F766572) ; 68020+ only
[000058e0] 666c                      bne.s     $0000594E
[000058e2] 6f77                      ble.s     $0000595B
[000058e4] 0d0a 0072                 movep.w   114(a2),d6
[000058e8] 756e 7469                 mvs.w     29801(a6),d2 ; ColdFire only
[000058ec] 6d65                      blt.s     $00005953
[000058ee] 0043 4f4e                 ori.w     #$4F4E,d3
[000058f2] 3a00                      move.w    d0,d5
[000058f4] 4c53 543a                 divu.l    (a3),d2:d5 ; 68020+ only
[000058f8] 001a 4361                 ori.b     #$61,(a2)+
[000058fc] 6e6e                      bgt.s     $0000596C
[000058fe] 6f74                      ble.s     $00005974
[00005900] 2069 6e69                 movea.l   28265(a1),a0
[00005904] 7469                      moveq.l   #105,d2
[00005906] 616c                      bsr.s     $00005974
[00005908] 697a                      bvs.s     $00005984
[0000590a] 6520                      bcs.s     $0000592C
[0000590c] 7374 6163 6b0d 0a00 0001  mvs.w     ([$6B0D,a4],$0A000001),d1 ; ColdFire only
[00005916] 0002 0101                 ori.b     #$01,d2
[0000591a] 0201 0100                 andi.b    #$00,d1
[0000591e] 0101                      btst      d0,d1
[00005920] 0201 0101                 andi.b    #$01,d1
[00005924] 0101                      btst      d0,d1
[00005926] 0000 0000                 ori.b     #$00,d0
[0000592a] 0000 0000                 ori.b     #$00,d0
[0000592e] 0000 0100                 ori.b     #$00,d0
[00005932] 0001 0003                 ori.b     #$03,d1
[00005936] 0500                      btst      d2,d0
[00005938] 0505                      btst      d2,d5
[0000593a] 0000 0101                 ori.b     #$01,d0
[0000593e] 0201 0010                 andi.b    #$10,d1
[00005942] 0701                      btst      d3,d1
[00005944] 0201 0000                 andi.b    #$00,d1
[00005948] 0000 0000                 ori.b     #$00,d0
[0000594c] 0000 0000                 ori.b     #$00,d0
[00005950] 0101                      btst      d0,d1
[00005952] 0102                      btst      d0,d2
[00005954] 0101                      btst      d0,d1
[00005956] 0201 0102                 andi.b    #$02,d1
[0000595a] 0101                      btst      d0,d1
[0000595c] 0101                      btst      d0,d1
[0000595e] 0201 0101                 andi.b    #$01,d1
[00005962] 0000 0000                 ori.b     #$00,d0
[00005966] 0000 0000                 ori.b     #$00,d0
[0000596a] 0000 0000                 ori.b     #$00,d0
[0000596e] 0201 0101                 andi.b    #$01,d1
[00005972] 0101                      btst      d0,d1
[00005974] 0601 0104                 addi.b    #$04,d1
[00005978] 0101                      btst      d0,d1
[0000597a] 0103                      btst      d0,d3
[0000597c] 0102                      btst      d0,d2
[0000597e] 0101                      btst      d0,d1
[00005980] 0402 0108                 subi.b    #$08,d2
[00005984] 0101                      btst      d0,d1
[00005986] 0000 0000                 ori.b     #$00,d0
[0000598a] 0000 0101                 ori.b     #$01,d0
[0000598e] 0109 0101                 movep.w   257(a1),d0
[00005992] 0101                      btst      d0,d1
[00005994] 0101                      btst      d0,d1
[00005996] 0100                      btst      d0,d0
[00005998] 0005 0100                 ori.b     #$00,d5
[0000599c] 0000 0000                 ori.b     #$00,d0
[000059a0] 0000 0000                 ori.b     #$00,d0
[000059a4] 0000 0000                 ori.b     #$00,d0
[000059a8] 0000 0000                 ori.b     #$00,d0
[000059ac] 0000 0000                 ori.b     #$00,d0
[000059b0] 0000 0000                 ori.b     #$00,d0
[000059b4] 0000 0000                 ori.b     #$00,d0
[000059b8] 0000 0000                 ori.b     #$00,d0
[000059bc] 0000 0000                 ori.b     #$00,d0
[000059c0] 0000 0000                 ori.b     #$00,d0
[000059c4] 0000 0000                 ori.b     #$00,d0
[000059c8] 0403 0008                 subi.b    #$08,d3
[000059cc] 0300                      btst      d1,d0
[000059ce] 0601 0008                 addi.b    #$08,d1
[000059d2] 0100                      btst      d0,d0
[000059d4] 0801 0004                 btst      #4,d1
[000059d8] 0101                      btst      d0,d1
[000059da] 0301                      btst      d1,d1
[000059dc] 0100                      btst      d0,d0
[000059de] 0500                      btst      d2,d0
[000059e0] 0101                      btst      d0,d1
[000059e2] 0100                      btst      d0,d0
[000059e4] 0500                      btst      d2,d0
[000059e6] 0001 0100                 ori.b     #$00,d1
[000059ea] 0101                      btst      d0,d1
[000059ec] 0000 0000                 ori.b     #$00,d0
[000059f0] 0000 0000                 ori.b     #$00,d0
[000059f4] 0000 0000                 ori.b     #$00,d0
[000059f8] 0000 0000                 ori.b     #$00,d0
[000059fc] 0000 0000                 ori.b     #$00,d0
[00005a00] 0000 0000                 ori.b     #$00,d0
[00005a04] 0002 0200                 ori.b     #$00,d2
[00005a08] 0000 0000                 ori.b     #$00,d0
[00005a0c] 0000 0000                 ori.b     #$00,d0
[00005a10] 0000 0000                 ori.b     #$00,d0
[00005a14] 0000 0000                 ori.b     #$00,d0
[00005a18] 0000 0000                 ori.b     #$00,d0
[00005a1c] 0000 0000                 ori.b     #$00,d0
[00005a20] 0000 0501                 ori.b     #$01,d0
[00005a24] 0005 0100                 ori.b     #$00,d5
[00005a28] 0101                      btst      d0,d1
[00005a2a] 0001 0100                 ori.b     #$00,d1
[00005a2e] 0205 0006                 andi.b    #$06,d5
[00005a32] 0100                      btst      d0,d0
[00005a34] 0201 0001                 andi.b    #$01,d1
[00005a38] 0100                      btst      d0,d0
[00005a3a] 0605 0000                 addi.b    #$00,d5
[00005a3e] 0000 0001                 ori.b     #$01,d0
[00005a42] 0100                      btst      d0,d0
[00005a44] 0100                      btst      d0,d0
[00005a46] 0201 0002                 andi.b    #$02,d1
[00005a4a] 0101                      btst      d0,d1
[00005a4c] 0101                      btst      d0,d1
[00005a4e] 0100                      btst      d0,d0
[00005a50] 0000 0000                 ori.b     #$00,d0
[00005a54] 0000 0000                 ori.b     #$00,d0
[00005a58] 0000 0000                 ori.b     #$00,d0
[00005a5c] 0000 0001                 ori.b     #$01,d0
[00005a60] 0203 0102                 andi.b    #$02,d3
[00005a64] 0101                      btst      d0,d1
[00005a66] 0101                      btst      d0,d1
[00005a68] 0101                      btst      d0,d1
[00005a6a] 0001 0100                 ori.b     #$00,d1
[00005a6e] 0102                      btst      d0,d2
[00005a70] 4572 726f                 lea.l     111(a2,d7.w*2),a2 ; 68020+ only
[00005a74] 7220                      moveq.l   #32,d1
[00005a76] 6f70                      ble.s     $00005AE8
[00005a78] 656e                      bcs.s     $00005AE8
[00005a7a] 696e                      bvs.s     $00005AEA
[00005a7c] 6720                      beq.s     $00005A9E
[00005a7e] 2573 2066 6f72            move.l    102(a3,d2.w),28530(a2)
[00005a84] 2072 6561 6469            movea.l   ([$6469,a2,zd6.w*4]),a0 ; 68020+ only
[00005a8a] 6e67                      bgt.s     $00005AF3
[00005a8c] 202d 4162                 move.l    16738(a5),d0
[00005a90] 6f72                      ble.s     $00005B04
[00005a92] 742e                      moveq.l   #46,d2
[00005a94] 0d0a 004e                 movep.w   78(a2),d6
[00005a98] 6f74                      ble.s     $00005B0E
[00005a9a] 2065                      movea.l   -(a5),a0
[00005a9c] 6e6f                      bgt.s     $00005B0D
[00005a9e] 7567                      mvs.w     -(a7),d2 ; ColdFire only
[00005aa0] 6820                      bvc.s     $00005AC2
[00005aa2] 6d65                      blt.s     $00005B09
[00005aa4] 6d6f                      blt.s     $00005B15
[00005aa6] 7279                      moveq.l   #121,d1
[00005aa8] 2066                      movea.l   -(a6),a0
[00005aaa] 6f72                      ble.s     $00005B1E
[00005aac] 206f 7065                 movea.l   28773(a7),a0
[00005ab0] 7261                      moveq.l   #97,d1
[00005ab2] 7469                      moveq.l   #105,d2
[00005ab4] 6f6e                      ble.s     $00005B24
[00005ab6] 206f 6e20                 movea.l   28192(a7),a0
[00005aba] 2573 0020 2d41            move.l    32(a3,d0.w),11585(a2)
[00005ac0] 626f                      bhi.s     $00005B31
[00005ac2] 7274                      moveq.l   #116,d1
[00005ac4] 0d0a 0045                 movep.w   69(a2),d6
[00005ac8] 7272                      moveq.l   #114,d1
[00005aca] 6f72                      ble.s     $00005B3E
[00005acc] 2072 6561 6469            movea.l   ([$6469,a2,zd6.w*4]),a0 ; 68020+ only
[00005ad2] 6e67                      bgt.s     $00005B3B
[00005ad4] 2025                      move.l    -(a5),d0
[00005ad6] 732e 202d                 mvs.b     8237(a6),d1 ; ColdFire only
[00005ada] 4162                      lea.l     -(a2),a0
[00005adc] 6f72                      ble.s     $00005B50
[00005ade] 740d                      moveq.l   #13,d2
[00005ae0] 0a00 4572                 eori.b    #$72,d0
[00005ae4] 726f                      moveq.l   #111,d1
[00005ae6] 7220                      moveq.l   #32,d1
[00005ae8] 6372                      bls.s     $00005B5C
[00005aea] 6561                      bcs.s     $00005B4D
[00005aec] 7469                      moveq.l   #105,d2
[00005aee] 6e67                      bgt.s     $00005B57
[00005af0] 2025                      move.l    -(a5),d0
[00005af2] 7300                      mvs.b     d0,d1 ; ColdFire only
[00005af4] 2066                      movea.l   -(a6),a0
[00005af6] 6f72                      ble.s     $00005B6A
[00005af8] 206f 7574                 movea.l   30068(a7),a0
[00005afc] 7075                      moveq.l   #117,d0
[00005afe] 7420                      moveq.l   #32,d2
[00005b00] 2d41 626f                 move.l    d1,25199(a6)
[00005b04] 7274                      moveq.l   #116,d1
[00005b06] 2e0d                      move.l    a5,d7
[00005b08] 0a00 4572                 eori.b    #$72,d0
[00005b0c] 726f                      moveq.l   #111,d1
[00005b0e] 7220                      moveq.l   #32,d1
[00005b10] 7772 6974 696e 6720       mvs.w     ([$696E6720,a2]),d3 ; ColdFire only; reserved OD=0
[00005b18] 2573 2573 002e 202d 4162  move.l    ([$002E202D,a3,zd2.w*4],$41626F72),29742(a2) ; 68020+ only
[00005b22] 6f72 742e
[00005b26] 000d 0a25                 ori.b     #$25,a5
[00005b2a] 730d                      mvs.b     a5,d1 ; ColdFire only
[00005b2c] 0a00 5553                 eori.b    #$53,d0
[00005b30] 4147                      lea.l     d7,a0
[00005b32] 453a 206f                 chk.l     $00007BA3(pc),d2 ; 68020+ only
[00005b36] 7074                      moveq.l   #116,d0
[00005b38] 696d                      bvs.s     $00005BA7
[00005b3a] 697a                      bvs.s     $00005BB6
[00005b3c] 652e                      bcs.s     $00005B6C
[00005b3e] 7072                      moveq.l   #114,d0
[00005b40] 6720                      beq.s     $00005B62
[00005b42] 5b20                      subq.b    #5,-(a0)
[00005b44] 2d6f 5041 5448            move.l    20545(a7),21576(a6)
[00005b4a] 205d                      movea.l   (a5)+,a0
[00005b4c] 2066                      movea.l   -(a6),a0
[00005b4e] 696c                      bvs.s     $00005BBC
[00005b50] 6573                      bcs.s     $00005BC5
[00005b52] 202e 2e2e                 move.l    11822(a6),d0
[00005b56] 0049 6e69                 ori.w     #$6E69,a1
[00005b5a] 7469                      moveq.l   #105,d2
[00005b5c] 616c                      bsr.s     $00005BCA
[00005b5e] 697a                      bvs.s     $00005BDA
[00005b60] 6174                      bsr.s     $00005BD6
[00005b62] 696f                      bvs.s     $00005BD3
[00005b64] 6e20                      bgt.s     $00005B86
[00005b66] 4661                      not.w     -(a1)
[00005b68] 696c                      bvs.s     $00005BD6
[00005b6a] 7572 6520 2121            mvs.w     ($2121,a2,d6.w*4),d2 ; ColdFire only
[00005b70] 2100                      move.l    d0,-(a0)
[00005b72] 2025                      move.l    -(a5),d0
[00005b74] 642e                      bcc.s     $00005BA4
[00005b76] 2020                      move.l    -(a0),d0
[00005b78] 2020                      move.l    -(a0),d0
[00005b7a] 2573 2e0d 0a00            move.l    13(a3,d2.l*8),2560(a2) ; 68020+ only
[00005b80] 2536 6420                 move.l    32(a6,d6.w*4),-(a2) ; 68020+ only
[00005b84] 546f 7461                 addq.w    #2,29793(a7)
[00005b88] 6c20                      bge.s     $00005BAA
[00005b8a] 636c                      bls.s     $00005BF8
[00005b8c] 7227                      moveq.l   #39,d1
[00005b8e] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005b90] 7265                      moveq.l   #101,d1
[00005b92] 6d6f                      blt.s     $00005C03
[00005b94] 7665                      moveq.l   #101,d3
[00005b96] 640d                      bcc.s     $00005BA5
[00005b98] 0a00 2536                 eori.b    #$36,d0
[00005b9c] 6420                      bcc.s     $00005BBE
[00005b9e] 546f 7461                 addq.w    #2,29793(a7)
[00005ba2] 6c20                      bge.s     $00005BC4
[00005ba4] 636c                      bls.s     $00005C12
[00005ba6] 7227                      moveq.l   #39,d1
[00005ba8] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005baa] 2877 2f73 746b 206d 6f64  movea.l   ([$746B206D,a7,zd2.l*8],$6F642920),a4 ; 68020+ only
[00005bb4] 2920
[00005bb6] 7265                      moveq.l   #101,d1
[00005bb8] 6d6f                      blt.s     $00005C29
[00005bba] 7665                      moveq.l   #101,d3
[00005bbc] 640d                      bcc.s     $00005BCB
[00005bbe] 0a00 2536                 eori.b    #$36,d0
[00005bc2] 6420                      bcc.s     $00005BE4
[00005bc4] 546f 7461                 addq.w    #2,29793(a7)
[00005bc8] 6c20                      bge.s     $00005BEA
[00005bca] 6d6f                      blt.s     $00005C3B
[00005bcc] 7665                      moveq.l   #101,d3
[00005bce] 7327                      mvs.b     -(a7),d1 ; ColdFire only
[00005bd0] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005bd2] 7265                      moveq.l   #101,d1
[00005bd4] 6d6f                      blt.s     $00005C45
[00005bd6] 7665                      moveq.l   #101,d3
[00005bd8] 640d                      bcc.s     $00005BE7
[00005bda] 0a00 2536                 eori.b    #$36,d0
[00005bde] 6420                      bcc.s     $00005C00
[00005be0] 546f 7461                 addq.w    #2,29793(a7)
[00005be4] 6c20                      bge.s     $00005C06
[00005be6] 6d6f                      blt.s     $00005C57
[00005be8] 7665                      moveq.l   #101,d3
[00005bea] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005bec] 2877 2f73 746b 206d 6f64  movea.l   ([$746B206D,a7,zd2.l*8],$6F642920),a4 ; 68020+ only
[00005bf6] 2920
[00005bf8] 7265                      moveq.l   #101,d1
[00005bfa] 6d6f                      blt.s     $00005C6B
[00005bfc] 7665                      moveq.l   #101,d3
[00005bfe] 640d                      bcc.s     $00005C0D
[00005c00] 0a00 2536                 eori.b    #$36,d0
[00005c04] 6420                      bcc.s     $00005C26
[00005c06] 546f 7461                 addq.w    #2,29793(a7)
[00005c0a] 6c20                      bge.s     $00005C2C
[00005c0c] 6d6f                      blt.s     $00005C7D
[00005c0e] 7665                      moveq.l   #101,d3
[00005c10] 2069 6d6d                 movea.l   28013(a1),a0
[00005c14] 6564                      bcs.s     $00005C7A
[00005c16] 6974                      bvs.s     $00005C8C
[00005c18] 6174                      bsr.s     $00005C8E
[00005c1a] 6573                      bcs.s     $00005C8F
[00005c1c] 2072 656d 6f76            movea.l   ([$6F76,a2],zd6.w*4),a0 ; 68020+ only; reserved OD=1
[00005c22] 6564                      bcs.s     $00005C88
[00005c24] 0d0a 0025                 movep.w   37(a2),d6
[00005c28] 3664                      movea.w   -(a4),a3
[00005c2a] 2054                      movea.l   (a4),a0
[00005c2c] 6f74                      ble.s     $00005CA2
[00005c2e] 616c                      bsr.s     $00005C9C
[00005c30] 206d 6f76                 movea.l   28534(a5),a0
[00005c34] 6520                      bcs.s     $00005C56
[00005c36] 696d                      bvs.s     $00005CA5
[00005c38] 6d65                      blt.s     $00005C9F
[00005c3a] 642e                      bcc.s     $00005C6A
[00005c3c] 2877 2f73 746b 206d 6f64  movea.l   ([$746B206D,a7,zd2.l*8],$6F642920),a4 ; 68020+ only
[00005c46] 2920
[00005c48] 7265                      moveq.l   #101,d1
[00005c4a] 6d6f                      blt.s     $00005CBB
[00005c4c] 7665                      moveq.l   #101,d3
[00005c4e] 640d                      bcc.s     $00005C5D
[00005c50] 0a00 2536                 eori.b    #$36,d0
[00005c54] 6420                      bcc.s     $00005C76
[00005c56] 546f 7461                 addq.w    #2,29793(a7)
[00005c5a] 6c20                      bge.s     $00005C7C
[00005c5c] 7265                      moveq.l   #101,d1
[00005c5e] 6475                      bcc.s     $00005CD5
[00005c60] 6e64                      bgt.s     $00005CC6
[00005c62] 616e                      bsr.s     $00005CD2
[00005c64] 7420                      moveq.l   #32,d2
[00005c66] 6272                      bhi.s     $00005CDA
[00005c68] 6127                      bsr.s     $00005C91
[00005c6a] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005c6c] 7265                      moveq.l   #101,d1
[00005c6e] 6d6f                      blt.s     $00005CDF
[00005c70] 7665                      moveq.l   #101,d3
[00005c72] 640d                      bcc.s     $00005C81
[00005c74] 0a00 2536                 eori.b    #$36,d0
[00005c78] 6420                      bcc.s     $00005C9A
[00005c7a] 546f 7461                 addq.w    #2,29793(a7)
[00005c7e] 6c20                      bge.s     $00005CA0
[00005c80] 7065                      moveq.l   #101,d0
[00005c82] 6127                      bsr.s     $00005CAB
[00005c84] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005c86] 7573 6564 2028            mvs.w     ([$2028,a3],zd6.w*4),d2 ; ColdFire only; reserved OD=0
[00005c8c] 3420                      move.w    -(a0),d2
[00005c8e] 6279                      bhi.s     $00005D09
[00005c90] 7465                      moveq.l   #101,d2
[00005c92] 7329 0d0a                 mvs.b     3338(a1),d1 ; ColdFire only
[00005c96] 0025 3664                 ori.b     #$64,-(a5)
[00005c9a] 2054                      movea.l   (a4),a0
[00005c9c] 6f74                      ble.s     $00005D12
[00005c9e] 616c                      bsr.s     $00005D0C
[00005ca0] 2070 6561 2773            movea.l   ([$2773,a0,zd6.w*4]),a0 ; 68020+ only
[00005ca6] 2075 7365 6420            movea.l   ([$6420,a5],zd7.w*2),a0 ; 68020+ only; reserved OD=1
[00005cac] 2877 2f73 746b 206d 6f64  movea.l   ([$746B206D,a7,zd2.l*8],$6F642920),a4 ; 68020+ only
[00005cb6] 2920
[00005cb8] 2832 206f                 move.l    111(a2,d2.w),d4
[00005cbc] 7220                      moveq.l   #32,d1
[00005cbe] 3420                      move.w    -(a0),d2
[00005cc0] 6279                      bhi.s     $00005D3B
[00005cc2] 7465                      moveq.l   #101,d2
[00005cc4] 7329 0d0a                 mvs.b     3338(a1),d1 ; ColdFire only
[00005cc8] 0025 3664                 ori.b     #$64,-(a5)
[00005ccc] 2054                      movea.l   (a4),a0
[00005cce] 6f74                      ble.s     $00005D44
[00005cd0] 616c                      bsr.s     $00005D3E
[00005cd2] 2061                      movea.l   -(a1),a0
[00005cd4] 6464                      bcc.s     $00005D3A
[00005cd6] 612e                      bsr.s     $00005D06
[00005cd8] 6c27                      bge.s     $00005D01
[00005cda] 7320                      mvs.b     -(a0),d1 ; ColdFire only
[00005cdc] 696d                      bvs.s     $00005D4B
[00005cde] 7072                      moveq.l   #114,d0
[00005ce0] 6f76                      ble.s     $00005D58
[00005ce2] 6564                      bcs.s     $00005D48
[00005ce4] 2028 3220                 move.l    12832(a0),d0
[00005ce8] 6279                      bhi.s     $00005D63
[00005cea] 7465                      moveq.l   #101,d2
[00005cec] 7329 0d0a                 mvs.b     3338(a1),d1 ; ColdFire only
[00005cf0] 0000 6174                 ori.b     #$74,d0
[00005cf4] 6f69                      ble.s     $00005D5F
[00005cf6] 206f 7665                 movea.l   30309(a7),a0
[00005cfa] 7266                      moveq.l   #102,d1
[00005cfc] 6c6f                      bge.s     $00005D6D
[00005cfe] 7700                      mvs.b     d0,d3 ; ColdFire only
[00005d00] 5041                      addq.w    #8,d1
[00005d02] 4e49                      trap      #9
[00005d04] 433a 2025                 chk.l     $00007D2B(pc),d1 ; 68020+ only
[00005d08] 7300                      mvs.b     d0,d1 ; ColdFire only
[00005d0a] 0d0a 4849                 movep.w   18505(a2),d6
[00005d0e] 5420                      addq.b    #2,-(a0)
[00005d10] 5e43                      addq.w    #7,d3
[00005d12] 2054                      movea.l   (a4),a0
[00005d14] 4f20                      chk.l     -(a0),d7 ; 68020+ only
[00005d16] 5155                      subq.w    #8,(a5)
[00005d18] 4954                      lea.l     (a4),a4
[00005d1a] 3a20                      move.w    -(a0),d5
[00005d1c] 0000 0000                 ori.b     #$00,d0
[00005d20] 0000 0101                 ori.b     #$01,d0
[00005d24] 0101                      btst      d0,d1
[00005d26] 0101                      btst      d0,d1
[00005d28] 0101                      btst      d0,d1
[00005d2a] 0121                      btst      d0,-(a1)
[00005d2c] 2101                      move.l    d1,-(a0)
[00005d2e] 2121                      move.l    -(a1),-(a0)
[00005d30] 0101                      btst      d0,d1
[00005d32] 0101                      btst      d0,d1
[00005d34] 0101                      btst      d0,d1
[00005d36] 0101                      btst      d0,d1
[00005d38] 0101                      btst      d0,d1
[00005d3a] 0101                      btst      d0,d1
[00005d3c] 0101                      btst      d0,d1
[00005d3e] 0101                      btst      d0,d1
[00005d40] 0101                      btst      d0,d1
[00005d42] 2202                      move.l    d2,d1
[00005d44] 0202 0202                 andi.b    #$02,d2
[00005d48] 0202 0202                 andi.b    #$02,d2
[00005d4c] 0202 0202                 andi.b    #$02,d2
[00005d50] 0202 0404                 andi.b    #$04,d2
[00005d54] 0404 0404                 subi.b    #$04,d4
[00005d58] 0404 0404                 subi.b    #$04,d4
[00005d5c] 0202 0202                 andi.b    #$02,d2
[00005d60] 0202 0208                 andi.b    #$08,d2
[00005d64] 0808 0808                 btst      #2056,a0
[00005d68] 0808 0808                 btst      #2056,a0
[00005d6c] 0808 0808                 btst      #2056,a0
[00005d70] 0808 0808                 btst      #2056,a0
[00005d74] 0808 0808                 btst      #2056,a0
[00005d78] 0808 0808                 btst      #2056,a0
[00005d7c] 0802 0202                 btst      #514,d2
[00005d80] 0202 0210                 andi.b    #$10,d2
[00005d84] 1010                      move.b    (a0),d0
[00005d86] 1010                      move.b    (a0),d0
[00005d88] 1010                      move.b    (a0),d0
[00005d8a] 1010                      move.b    (a0),d0
[00005d8c] 1010                      move.b    (a0),d0
[00005d8e] 1010                      move.b    (a0),d0
[00005d90] 1010                      move.b    (a0),d0
[00005d92] 1010                      move.b    (a0),d0
[00005d94] 1010                      move.b    (a0),d0
[00005d96] 1010                      move.b    (a0),d0
[00005d98] 1010                      move.b    (a0),d0
[00005d9a] 1010                      move.b    (a0),d0
[00005d9c] 1002                      move.b    d2,d0
[00005d9e] 0202 0201                 andi.b    #$01,d2
[00005da2] 0000 0000                 ori.b     #$00,d0
[00005da6] 0000 3d5e                 ori.b     #$5E,d0
[00005daa] 0000 3da8                 ori.b     #$A8,d0
[00005dae] 0000 3d24                 ori.b     #$24,d0
[00005db2] 0000 3d44                 ori.b     #$44,d0
[00005db6] 0000 3d7c                 ori.b     #$7C,d0
[00005dba] 0000 3da8                 ori.b     #$A8,d0
[00005dbe] 0000 3da8                 ori.b     #$A8,d0
[00005dc2] 0000 3d02                 ori.b     #$02,d0
[00005dc6] 0d00                      btst      d6,d0
[00005dc8] 0000 0000                 ori.b     #$00,d0
[00005dcc] 0000 5dcc                 ori.b     #$CC,d0
[00005dd0] 0000 ffff                 ori.b     #$FF,d0
[00005dd4] 0000 5dcc                 ori.b     #$CC,d0
[00005dd8] 0000 4f22                 ori.b     #$22,d0
[00005ddc] 0000 4dbe                 ori.b     #$BE,d0
[00005de0] 0000 4f3e                 ori.b     #$3E,d0
[00005de4] 0000 4f64                 ori.b     #$64,d0
[00005de8] 0000 4f88                 ori.b     #$88,d0
[00005dec] 0000 4fac                 ori.b     #$AC,d0
[00005df0] 0000 4fac                 ori.b     #$AC,d0
[00005df4] 0000 4fac                 ori.b     #$AC,d0
[00005df8] 0000 4fac                 ori.b     #$AC,d0
[00005dfc] 0000 4fac                 ori.b     #$AC,d0
[00005e00] 0000 4fac                 ori.b     #$AC,d0
[00005e04] 0000 4fac                 ori.b     #$AC,d0
[00005e08] 0000 4e68                 ori.b     #$68,d0
[00005e0c] 0000 4fac                 ori.b     #$AC,d0
[00005e10] 0000 4fac                 ori.b     #$AC,d0
[00005e14] 0000 4fac                 ori.b     #$AC,d0
[00005e18] 0000 4f10                 ori.b     #$10,d0
[00005e1c] 0000 4fac                 ori.b     #$AC,d0
[00005e20] 0000 4e14                 ori.b     #$14,d0
[00005e24] 0000 4fac                 ori.b     #$AC,d0
[00005e28] 0000 4fac                 ori.b     #$AC,d0
[00005e2c] 0000 4ebc                 ori.b     #$BC,d0
[00005e30] 0000 4fac                 ori.b     #$AC,d0
[00005e34] 0000 4fac                 ori.b     #$AC,d0
[00005e38] 0000 4fac                 ori.b     #$AC,d0
[00005e3c] 0000 4fac                 ori.b     #$AC,d0
[00005e40] 0000 4fac                 ori.b     #$AC,d0
[00005e44] 0000 4fac                 ori.b     #$AC,d0
[00005e48] 0000 4fac                 ori.b     #$AC,d0
[00005e4c] 0000 4fac                 ori.b     #$AC,d0
[00005e50] 0000 4fac                 ori.b     #$AC,d0
[00005e54] 0000 4fac                 ori.b     #$AC,d0
[00005e58] 0000 4f22                 ori.b     #$22,d0
[00005e5c] 0000 4dc2                 ori.b     #$C2,d0
[00005e60] 0000 4f3e                 ori.b     #$3E,d0
[00005e64] 0000 4f64                 ori.b     #$64,d0
[00005e68] 0000 4f88                 ori.b     #$88,d0
[00005e6c] 0000 4fac                 ori.b     #$AC,d0
[00005e70] 0000 4fac                 ori.b     #$AC,d0
[00005e74] 0000 4fac                 ori.b     #$AC,d0
[00005e78] 0000 4fac                 ori.b     #$AC,d0
[00005e7c] 0000 4fac                 ori.b     #$AC,d0
[00005e80] 0000 4fac                 ori.b     #$AC,d0
[00005e84] 0000 4fac                 ori.b     #$AC,d0
[00005e88] 0000 4e6c                 ori.b     #$6C,d0
[00005e8c] 0000 4fac                 ori.b     #$AC,d0
[00005e90] 0000 4fac                 ori.b     #$AC,d0
[00005e94] 0000 4fac                 ori.b     #$AC,d0
[00005e98] 0000 4f10                 ori.b     #$10,d0
[00005e9c] 0000 4fac                 ori.b     #$AC,d0
[00005ea0] 0000 4e18                 ori.b     #$18,d0
[00005ea4] 0000 4fac                 ori.b     #$AC,d0
[00005ea8] 0000 4fac                 ori.b     #$AC,d0
[00005eac] 0000 4ec0                 ori.b     #$C0,d0
[00005eb0] 0000 0001                 ori.b     #$01,d0
[00005eb4] 0000 0000                 ori.b     #$00,d0
[00005eb8] 0000 0000                 ori.b     #$00,d0
[00005ebc] 0000 0001                 ori.b     #$01,d0
[00005ec0] 000a 0000                 ori.b     #$00,a2
[00005ec4] 0000 0000                 ori.b     #$00,d0
[00005ec8] 0000 0000                 ori.b     #$00,d0
[00005ecc] 0002 000a                 ori.b     #$0A,d2
[00005ed0] 0000 0000                 ori.b     #$00,d0
[00005ed4] 0000 0000                 ori.b     #$00,d0
[00005ed8] 0000 0003                 ori.b     #$03,d0
[00005edc] 0000 0000                 ori.b     #$00,d0
[00005ee0] 0000 0000                 ori.b     #$00,d0
[00005ee4] 0000 0000                 ori.b     #$00,d0
[00005ee8] 0004 0000                 ori.b     #$00,d4
[00005eec] 0000 0000                 ori.b     #$00,d0
[00005ef0] 0000 0000                 ori.b     #$00,d0
[00005ef4] 0000 0005                 ori.b     #$05,d0
[00005ef8] 0000 0000                 ori.b     #$00,d0
[00005efc] 0000 0000                 ori.b     #$00,d0
[00005f00] 0000 0000                 ori.b     #$00,d0
[00005f04] 0006 0000                 ori.b     #$00,d6
[00005f08] 0000 0000                 ori.b     #$00,d0
[00005f0c] 0000 0000                 ori.b     #$00,d0
[00005f10] 0000 0007                 ori.b     #$07,d0
[00005f14] 0000 0000                 ori.b     #$00,d0
[00005f18] 0000 0000                 ori.b     #$00,d0
[00005f1c] 0000 0000                 ori.b     #$00,d0
[00005f20] 0008 0000                 ori.b     #$00,a0
[00005f24] 0000 0000                 ori.b     #$00,d0
[00005f28] 0000 0000                 ori.b     #$00,d0
[00005f2c] 0000 0009                 ori.b     #$09,d0
[00005f30] 0000 0000                 ori.b     #$00,d0
[00005f34] 0000 0000                 ori.b     #$00,d0
[00005f38] 0000 0000                 ori.b     #$00,d0
[00005f3c] 000a 0000                 ori.b     #$00,a2
[00005f40] 0000 0000                 ori.b     #$00,d0
[00005f44] 0000 0000                 ori.b     #$00,d0
[00005f48] 0000 000b                 ori.b     #$0B,d0
[00005f4c] 0000 0000                 ori.b     #$00,d0
[00005f50] 0000 0000                 ori.b     #$00,d0
[00005f54] 0000 0000                 ori.b     #$00,d0
[00005f58] 000c 0000                 ori.b     #$00,a4
[00005f5c] 0000 0000                 ori.b     #$00,d0
[00005f60] 0000 0000                 ori.b     #$00,d0
[00005f64] 0000 000d                 ori.b     #$0D,d0
[00005f68] 0000 0000                 ori.b     #$00,d0
[00005f6c] 0000 0000                 ori.b     #$00,d0
[00005f70] 0000 0000                 ori.b     #$00,d0
[00005f74] 000e 0000                 ori.b     #$00,a6
[00005f78] 0000 0000                 ori.b     #$00,d0
[00005f7c] 0000 0000                 ori.b     #$00,d0
[00005f80] 0000 000f                 ori.b     #$0F,d0
[00005f84] 0000 0000                 ori.b     #$00,d0
[00005f88] 0000 0000                 ori.b     #$00,d0
[00005f8c] 0000 0000                 ori.b     #$00,d0
[00005f90] 2227                      move.l    -(a7),d1
[00005f92] 003a 2075 6e6d            ori.b     #$75,$0000CE01(pc)
[00005f98] 6174                      bsr.s     $0000600E
[00005f9a] 6368                      bls.s     _dta_buf
[00005f9c] 6564                      bcs.s     _adda_to
[00005f9e] 2071 756f 7465 003a 204e  movea.l   ([$7465,a1],zd7.w*4,$003A204E),a0 ; 68020+ only; reserved OD=3
[00005fa8] 6f20                      ble.s     __base
[00005faa] 6d61                      blt.s     $0000600D
[00005fac] 7463                      moveq.l   #99,d2
[00005fae] 6800 5c3a                 bvc       $0000BBEA
[00005fb2] 0053 7461                 ori.w     #$7461,(a3)
[00005fb6] 636b                      bls.s     $00006023
[00005fb8] 204f                      movea.l   a7,a0
[00005fba] 7665                      moveq.l   #101,d3
[00005fbc] 7266                      moveq.l   #102,d1
[00005fbe] 6c6f                      bge.s     $0000602F
[00005fc0] 7700                      mvs.b     d0,d3 ; ColdFire only
[00005fc2] 000d 0a00                 ori.b     #$00,a5
;
         U _etoa
         U _ftoa
00000000 T __start
0000008a T __exit
000000ac T _brk
000000da T ___BDOS
0000012e T __sovf
00000144 T _blkfill
00000162 T _index
00000162 T _strchr
0000017e T _crystal
0000018a T _filesiz
00000200 T _init_op
000002b0 T _open_fi
000003aa T _close_f
00000490 T _Error
000004ba T _usage
000004ce T _main
00000726 T _do_opti
000008e8 T _reverse
00000954 T _strlen
00000982 T _htoa
00000a14 T _itoa
00000a92 T _atoi
00000b46 T _getnum
00000c9e T _clr2wto
00000f2c T _mv2wtol
00001306 T _mvwtol
00001702 T _clrwtol
0000199c T _imoveop
00001e0e T _imov1_o
0000228c T _puthex
000022c4 T _nextlin
0000234e T _clrop
0000257e T _samebra
0000276e T _mv2pea1
00002c2c T _mv2pea2
000030f8 T _adda
00003300 T _panic
00003352 T _add4mun
000036a0 T _addinra
000039e0 T _xbios
000039f0 T _bios
00003a00 T _gemdos
00003a10 T lmul
00003a64 T _trap
00003a74 T __pc_rea
00003ab0 T __pc_wri
00003aec T _uldiv
00003b68 T __salloc
00003b96 T __strcmp
00003c08 T ____atab
00003c10 T _strcpy
00003c32 T _strncpy
00003c7a T _strcat
00003ca4 T ___open
00003db4 T _ucase
00003de8 T __ttyout
00003e8a T __lstout
00003eca T _j30june
00003ee2 T __wrtchr
0000404c T __wrtbin
000040c6 T __wrtasc
000041d6 T __allocc
00004220 T __freec
0000423c T __chinit
0000425e T ___chini
000042cc T __chkc
0000432e T _write
000043ca T _sbrk
00004434 T ___prtsh
000044a2 T ___prtld
00004558 T ___prtin
000045ca T _malloc_
000045d2 T __errmal
000045da T _malloc
00004768 T _free
00004866 T _realloc
000048e6 T _isatty
0000491a T _isdev
00004946 T _ttyname
00004962 T __flsbuf
00004ab0 T _fputc
00004af6 T _fputn
00004b66 T __pftoa
00004b9e T __petoa
00004bd6 T __pgtoa
00004c20 T __doprt
000050ac T _lseek
00005108 T _tell
0000511e T __open
00005218 T _open
00005230 T _opena
00005248 T _openb
00005262 T _fflush
000052fc T ___fdecl
00005304 T _close
0000538c T _fclose
000053e4 T __cleanu
00005416 T _exit
0000542e T ___main
000057fc T _printf
0000581e T _fprintf
0000583e T __main
000058e7 D ___pname
000058ef D ___tname
000058f4 D ___lname
000058f9 D ___xeof
00005914 D _ctrl_cn
00005d1e D _uldivr
00005d22 D ___atab
00005da2 D _errno
00005da4 D __errcpm
00005dc8 D __chvec
00005dcc D __afreeb
00005dd4 D __aflist
00005eb0 D __iob
00005fca B __base
00005fce B __break
00005fd2 B ___cpmrv
00005fea B _numstr
00005ff4 B _clr1_to
00005ff6 B _size
00005ffa B _imov1_t
00005ffc B _mov1_to
00005ffe B _mv1_tot
00006000 B _mv2_tot
00006002 B _adda_to
00006004 B _dta_buf
0000602e B _in_buff
00006032 B _numstr1
0000603c B _numstr2
00006046 B _numstr3
00006050 B _in_hndl
00006052 B _freemem
00006056 B _tmp_ind
0000605a B _bra_tot
0000605c B _savereg
0000605e B _out_hnd
00006060 B _out_ind
00006064 B _out_buf
00006068 B _clr_tot
0000606a B _memsize
0000606e B _nxt_lin
00006072 B _imov_to
00006074 B _mov_tot
00006076 B _opath
00006176 B _num
00006178 B _xyzzx
0000617c B _begmem
00006180 B _in_ind
00006184 B _endmem
00006188 B _num1
0000618a B _num2
0000618c B _regnum
0000618e B __fds
0000850e B _hexstr
;
