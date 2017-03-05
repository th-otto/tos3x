; ph_branch = 0x601a
; ph_tlen = 0x000045fc
; ph_dlen = 0x000008aa
; ph_blen = 0x00000e26
; ph_slen = 0x00000000
; ph_res1 = 0x00000000
; ph_prgflags = 0x00000000
; ph_absflag = 0x0000
; first relocation = 0x00000008
; relocation bytes = 0x00000326

[00010000] 2a4f                      movea.l   a7,a5
[00010002] 2a6d 0004                 movea.l   4(a5),a5
[00010006] 23cd 0001 4ea6            move.l    a5,$00014EA6
[0001000c] 202d 000c                 move.l    12(a5),d0
[00010010] d0ad 0014                 add.l     20(a5),d0
[00010014] d0ad 001c                 add.l     28(a5),d0
[00010018] d0bc 0000 4000            add.l     #$00004000,d0
[0001001e] 2200                      move.l    d0,d1
[00010020] d28d                      add.l     a5,d1
[00010022] c2bc ffff fffe            and.l     #$FFFFFFFE,d1
[00010028] 2e41                      movea.l   d1,a7
[0001002a] 2f00                      move.l    d0,-(a7)
[0001002c] 2f0d                      move.l    a5,-(a7)
[0001002e] 4267                      clr.w     -(a7)
[00010030] 3f3c 004a                 move.w    #$004A,-(a7)
[00010034] 4e41                      trap      #1
[00010036] dffc 0000 000c            adda.l    #$0000000C,a7
[0001003c] 2079 0001 4ea6            movea.l   $00014EA6,a0
[00010042] 2268 0018                 movea.l   24(a0),a1
[00010046] d3e8 001c                 adda.l    28(a0),a1
[0001004a] 23c9 0001 4eaa            move.l    a1,$00014EAA
[00010050] 45e8 0080                 lea.l     128(a0),a2
[00010054] 101a                      move.b    (a2)+,d0
[00010056] 0280 0000 00ff            andi.l    #$000000FF,d0
[0001005c] 3f00                      move.w    d0,-(a7)
[0001005e] 2f0a                      move.l    a2,-(a7)
[00010060] 9dce                      suba.l    a6,a6
[00010062] 4eb9 0001 1cc8            jsr       __main
[00010068] 4ef9 0001 00ba            jmp       __exit
_crystal:
[0001006e] 222f 0004                 move.l    4(a7),d1
[00010072] 303c 00c8                 move.w    #$00C8,d0
[00010076] 4e42                      trap      #2
[00010078] 4e75                      rts
_brk:
[0001007a] 206f 0004                 movea.l   4(a7),a0
[0001007e] 2008                      move.l    a0,d0
[00010080] 41e8 0100                 lea.l     256(a0),a0
[00010084] bfc8                      cmpa.l    a0,a7
[00010086] 6404                      bcc.s     $0001008C
[00010088] 70ff                      moveq.l   #-1,d0
[0001008a] 4e75                      rts
[0001008c] 23c0 0001 4eaa            move.l    d0,$00014EAA
[00010092] 4280                      clr.l     d0
[00010094] 4e75                      rts
___BDOS:
[00010096] 4e56 0000                 link      a6,#$0000
[0001009a] 302f 0008                 move.w    8(a7),d0
[0001009e] 222f 000a                 move.l    10(a7),d1
[000100a2] 4e42                      trap      #2
[000100a4] bff9 0001 4eaa            cmpa.l    $00014EAA,a7
[000100aa] 6414                      bcc.s     $000100C0
[000100ac] 303c 0009                 move.w    #$0009,d0
[000100b0] 41f9 0001 4758            lea.l     $00014758,a0
[000100b6] 2208                      move.l    a0,d1
[000100b8] 4e42                      trap      #2
__exit:
[000100ba] 303c 0000                 move.w    #$0000,d0
[000100be] 4e42                      trap      #2
[000100c0] 4e5e                      unlk      a6
[000100c2] 4e75                      rts
_blkfill:
_memset:
[000100c4] 206f 0004                 movea.l   4(a7),a0
[000100c8] 322f 0008                 move.w    8(a7),d1
[000100cc] 302f 000a                 move.w    10(a7),d0
[000100d0] 48c0                      ext.l     d0
[000100d2] 5380                      subq.l    #1,d0
[000100d4] 6f06                      ble.s     $000100DC
[000100d6] 10c1                      move.b    d1,(a0)+
[000100d8] 51c8 fffc                 dbf       d0,$000100D6
[000100dc] 4280                      clr.l     d0
[000100de] 4e75                      rts
_strchr:
[000100e0] 206f 0004                 movea.l   4(a7),a0
[000100e4] 302f 0008                 move.w    8(a7),d0
[000100e8] 4a10                      tst.b     (a0)
[000100ea] 6604                      bne.s     $000100F0
[000100ec] 4280                      clr.l     d0
[000100ee] 4e75                      rts
[000100f0] b018                      cmp.b     (a0)+,d0
[000100f2] 66f4                      bne.s     $000100E8
[000100f4] 2008                      move.l    a0,d0
[000100f6] 5380                      subq.l    #1,d0
[000100f8] 4e75                      rts

_main:
[000100fa] 4e56 fffc                 link      a6,#$FFFC
[000100fe] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[00010102] 2ebc 0001 4861            move.l    #$00014861,(a7)
[00010108] 4eb9 0001 1d6c            jsr       _printf
[0001010e] 3ebc 0002                 move.w    #$0002,(a7)
[00010112] 4eb9 0001 266c            jsr       _close
[00010118] 4257                      clr.w     (a7)
[0001011a] 4eb9 0001 266c            jsr       _close
[00010120] 206e 000a                 movea.l   10(a6),a0
[00010124] 23d0 0001 5144            move.l    (a0),_myname
[0001012a] 4279 0001 50f2            clr.w     _exitstat
[00010130] 0c6e 0003 0008            cmpi.w    #$0003,8(a6)
[00010136] 6c18                      bge.s     $00010150
[00010138] 2eb9 0001 5144            move.l    _myname,(a7)
[0001013e] 2f3c 0001 488c            move.l    #$0001488C,-(a7)
[00010144] 4eb9 0001 1d6c            jsr       _printf
[0001014a] 588f                      addq.l    #4,a7
[0001014c] 6100 090e                 bsr       _endit
[00010150] 2ebc 0001 0a5c            move.l    #_endit,(a7)
[00010156] 3f3c 0001                 move.w    #$0001,-(a7)
[0001015a] 4eb9 0001 3b38            jsr       _signal
[00010160] 548f                      addq.l    #2,a7
[00010162] 2ebc 0001 0a5c            move.l    #_endit,(a7)
[00010168] 3f3c 0002                 move.w    #$0002,-(a7)
[0001016c] 4eb9 0001 3b38            jsr       _signal
[00010172] 548f                      addq.l    #2,a7
[00010174] 266e 000a                 movea.l   10(a6),a3
[00010178] 588b                      addq.l    #4,a3
[0001017a] 2a5b                      movea.l   (a3)+,a5
[0001017c] 3e2e 0008                 move.w    8(a6),d7
[00010180] 5747                      subq.w    #3,d7
[00010182] 6000 00cc                 bra       $00010250
[00010186] 101d                      move.b    (a5)+,d0
[00010188] 4880                      ext.w     d0
[0001018a] 6000 00ae                 bra       $0001023A
case r
[0001018e] 5279 0001 50dc            addq.w    #1,_rflg
[00010194] 23fc 0001 073e 0001 51a6  move.l    #_replace,_docom
[0001019e] 6000 00b0                 bra       $00010250
case a
[000101a2] 5279 0001 51aa            addq.w    #1,_aflg
[000101a8] 23db 0001 510e            move.l    (a3)+,_psname
[000101ae] 5347                      subq.w    #1,d7
[000101b0] 6000 009e                 bra       $00010250
case b
case i
[000101b4] 5279 0001 51ac            addq.w    #1,_bflg
[000101ba] 5347                      subq.w    #1,d7
[000101bc] 23db 0001 510e            move.l    (a3)+,_psname
[000101c2] 6000 008c                 bra       $00010250
case d
[000101c6] 5279 0001 5cc8            addq.w    #1,_dflg
[000101cc] 23fc 0001 092c 0001 51a6  move.l    #_delete,_docom
[000101d6] 6000 0078                 bra       $00010250
case x
[000101da] 5279 0001 50e4            addq.w    #1,_xflg
[000101e0] 23fc 0001 0992 0001 51a6  move.l    #_extract,_docom
[000101ea] 6064                      bra.s     $00010250
case t
[000101ec] 5279 0001 50de            addq.w    #1,_tflg
[000101f2] 23fc 0001 0658 0001 51a6  move.l    #_xtell,_docom
[000101fc] 6052                      bra.s     $00010250
case p
[000101fe] 5279 0001 50d6            addq.w    #1,_pflg
[00010204] 23fc 0001 0a22 0001 51a6  move.l    #_print,_docom
[0001020e] 6040                      bra.s     $00010250
case v
[00010210] 5279 0001 50e2            addq.w    #1,_vflg
[00010216] 6038                      bra.s     $00010250
case -
[00010218] 6036                      bra.s     $00010250
default
[0001021a] 1025                      move.b    -(a5),d0
[0001021c] 4880                      ext.w     d0
[0001021e] 3e80                      move.w    d0,(a7)
[00010220] 2f39 0001 5144            move.l    _myname,-(a7)
[00010226] 2f3c 0001 48c1            move.l    #$000148C1,-(a7)
[0001022c] 4eb9 0001 1d6c            jsr       _printf
[00010232] 508f                      addq.l    #8,a7
[00010234] 6100 0826                 bsr       _endit
[00010238] 6016                      bra.s     $00010250
[0001023a] 48c0                      ext.l     d0
[0001023c] 207c 0001 4786            movea.l   #$00014786,a0
[00010242] 720a                      moveq.l   #10,d1
[00010244] b098                      cmp.l     (a0)+,d0
[00010246] 57c9 fffc                 dbeq      d1,$00010244
[0001024a] 2068 0028                 movea.l   40(a0),a0
[0001024e] 4ed0                      jmp       (a0)
[00010250] 4a15                      tst.b     (a5)
[00010252] 6600 ff32                 bne       $00010186

[00010256] 3039 0001 50dc            move.w    _rflg,d0
[0001025c] d079 0001 5cc8            add.w     _dflg,d0
[00010262] 33c0 0001 50e0            move.w    d0,_uflg
[00010268] 3039 0001 50e0            move.w    _uflg,d0
[0001026e] d079 0001 50e4            add.w     _xflg,d0
[00010274] d079 0001 50de            add.w     _tflg,d0
[0001027a] d079 0001 50d6            add.w     _pflg,d0
[00010280] b07c 0001                 cmp.w     #$0001,d0
[00010284] 6718                      beq.s     $0001029E
[00010286] 2eb9 0001 5144            move.l    _myname,(a7)
[0001028c] 2f3c 0001 48de            move.l    #$000148DE,-(a7)
[00010292] 4eb9 0001 1d6c            jsr       _printf
[00010298] 588f                      addq.l    #4,a7
[0001029a] 6100 07c0                 bsr       _endit
[0001029e] 3039 0001 51aa            move.w    _aflg,d0
[000102a4] d079 0001 51ac            add.w     _bflg,d0
[000102aa] 33c0 0001 51ae            move.w    d0,_psflg
[000102b0] 0c79 0001 0001 51ae       cmpi.w    #$0001,_psflg
[000102b8] 6f18                      ble.s     $000102D2
[000102ba] 2eb9 0001 5144            move.l    _myname,(a7)
[000102c0] 2f3c 0001 490d            move.l    #$0001490D,-(a7)
[000102c6] 4eb9 0001 1d6c            jsr       _printf
[000102cc] 588f                      addq.l    #4,a7
[000102ce] 6100 078c                 bsr       _endit
[000102d2] 4a79 0001 51ae            tst.w     _psflg
[000102d8] 6722                      beq.s     $000102FC
[000102da] 0c79 0001 0001 50dc       cmpi.w    #$0001,_rflg
[000102e2] 6718                      beq.s     $000102FC
[000102e4] 2eb9 0001 5144            move.l    _myname,(a7)
[000102ea] 2f3c 0001 4932            move.l    #$00014932,-(a7)
[000102f0] 4eb9 0001 1d6c            jsr       _printf
[000102f6] 588f                      addq.l    #4,a7
[000102f8] 6100 0762                 bsr       _endit
[000102fc] 23db 0001 50ea            move.l    (a3)+,_arname
[00010302] 3eb9 0001 50dc            move.w    _rflg,(a7)
[00010308] 2f39 0001 50ea            move.l    _arname,-(a7)
[0001030e] 6100 02c2                 bsr       _openar
[00010312] 588f                      addq.l    #4,a7
[00010314] 23c0 0001 50d8            move.l    d0,_arfp
[0001031a] 663e                      bne.s     $0001035A
[0001031c] 3039 0001 50de            move.w    _tflg,d0
[00010322] d079 0001 50e4            add.w     _xflg,d0
[00010328] d079 0001 51ac            add.w     _bflg,d0
[0001032e] d079 0001 51aa            add.w     _aflg,d0
[00010334] d079 0001 5cc8            add.w     _dflg,d0
[0001033a] 671e                      beq.s     $0001035A
[0001033c] 2eb9 0001 50ea            move.l    _arname,(a7)
[00010342] 2f39 0001 5144            move.l    _myname,-(a7)
[00010348] 2f3c 0001 4962            move.l    #$00014962,-(a7)
[0001034e] 4eb9 0001 1d6c            jsr       _printf
[00010354] 508f                      addq.l    #8,a7
[00010356] 6100 0704                 bsr       _endit
[0001035a] 4a47                      tst.w     d7
[0001035c] 6610                      bne.s     $0001036E
[0001035e] 4a79 0001 50de            tst.w     _tflg
[00010364] 6708                      beq.s     $0001036E
[00010366] 6100 072e                 bsr       _listall
[0001036a] 6100 0702                 bsr       _cleanup
[0001036e] 4a47                      tst.w     d7
[00010370] 6610                      bne.s     $00010382
[00010372] 4a79 0001 50e4            tst.w     _xflg
[00010378] 6708                      beq.s     $00010382
[0001037a] 6100 0b90                 bsr       _exall
[0001037e] 6100 06ee                 bsr       _cleanup
[00010382] 4a47                      tst.w     d7
[00010384] 6f00 fdb2                 ble       $00010138
[00010388] 4a79 0001 50e0            tst.w     _uflg
[0001038e] 6604                      bne.s     $00010394
[00010390] 4240                      clr.w     d0
[00010392] 6002                      bra.s     $00010396
[00010394] 7001                      moveq.l   #1,d0
[00010396] b079 0001 4784            cmp.w     _libmagi,d0
[0001039c] 6614                      bne.s     $000103B2
[0001039e] 2eb9 0001 5144            move.l    _myname,(a7)
[000103a4] 2f3c 0001 4979            move.l    #$00014979,-(a7)
[000103aa] 4eb9 0001 1d6c            jsr       _printf
[000103b0] 588f                      addq.l    #4,a7
[000103b2] 4a79 0001 50d6            tst.w     _pflg
[000103b8] 6600 0082                 bne       $0001043C
[000103bc] 4a79 0001 50de            tst.w     _tflg
[000103c2] 6600 0078                 bne       $0001043C
[000103c6] 2ebc 0001 49a8            move.l    #$000149A8,(a7)
[000103cc] 6100 07ce                 bsr       _mktemp
[000103d0] 23c0 0001 50e6            move.l    d0,_tempnam
[000103d6] 2ebc 0001 49b1            move.l    #$000149B1,(a7)
[000103dc] 2f39 0001 50e6            move.l    _tempnam,-(a7)
[000103e2] 4eb9 0001 1c10            jsr       _fopenb
[000103e8] 588f                      addq.l    #4,a7
[000103ea] 23c0 0001 50ee            move.l    d0,_tempfd
[000103f0] 661e                      bne.s     $00010410
[000103f2] 2eb9 0001 50e6            move.l    _tempnam,(a7)
[000103f8] 2f39 0001 5144            move.l    _myname,-(a7)
[000103fe] 2f3c 0001 49b3            move.l    #$000149B3,-(a7)
[00010404] 4eb9 0001 1d6c            jsr       _printf
[0001040a] 508f                      addq.l    #8,a7
[0001040c] 6100 064e                 bsr       _endit
[00010410] 2eb9 0001 50ee            move.l    _tempfd,(a7)
[00010416] 2f3c 0001 4784            move.l    #_libmagi,-(a7)
[0001041c] 4eb9 0001 154c            jsr       _lputw
[00010422] 588f                      addq.l    #4,a7
[00010424] 4a40                      tst.w     d0
[00010426] 6714                      beq.s     $0001043C
[00010428] 2eb9 0001 5144            move.l    _myname,(a7)
[0001042e] 2f3c 0001 49c8            move.l    #$000149C8,-(a7)
[00010434] 4eb9 0001 1d6c            jsr       _printf
[0001043a] 588f                      addq.l    #4,a7
[0001043c] 6000 00ec                 bra       $0001052A

[00010440] 3d7c 0001 fffc            move.w    #$0001,-4(a6)
[00010446] 4a79 0001 51ae            tst.w     _psflg
[0001044c] 6600 0080                 bne       $000104CE
[00010450] 426e fffe                 clr.w     -2(a6)
[00010454] 6070                      bra.s     $000104C6
[00010456] 2eb9 0001 4780            move.l    _lp,(a7)
[0001045c] 2f0b                      move.l    a3,-(a7)
[0001045e] 306e fffe                 movea.w   -2(a6),a0
[00010462] d1c8                      adda.l    a0,a0
[00010464] d1c8                      adda.l    a0,a0
[00010466] 265f                      movea.l   (a7)+,a3
[00010468] 2f33 8800                 move.l    0(a3,a0.l),-(a7)
[0001046c] 6100 0c08                 bsr       _fnonly
[00010470] 588f                      addq.l    #4,a7
[00010472] 2f00                      move.l    d0,-(a7)
[00010474] 6100 076c                 bsr       _streq
[00010478] 588f                      addq.l    #4,a7
[0001047a] 4a40                      tst.w     d0
[0001047c] 6744                      beq.s     $000104C2
[0001047e] 426e fffc                 clr.w     -4(a6)
[00010482] 2f0b                      move.l    a3,-(a7)
[00010484] 306e fffe                 movea.w   -2(a6),a0
[00010488] d1c8                      adda.l    a0,a0
[0001048a] d1c8                      adda.l    a0,a0
[0001048c] 265f                      movea.l   (a7)+,a3
[0001048e] 2f33 8800                 move.l    0(a3,a0.l),-(a7)
[00010492] 2079 0001 51a6            movea.l   _docom,a0
[00010498] 4e90                      jsr       (a0)
[0001049a] 588f                      addq.l    #4,a7
[0001049c] 5347                      subq.w    #1,d7
[0001049e] 601c                      bra.s     $000104BC
[000104a0] 306e fffe                 movea.w   -2(a6),a0
[000104a4] d1c8                      adda.l    a0,a0
[000104a6] d1c8                      adda.l    a0,a0
[000104a8] d1cb                      adda.l    a3,a0
[000104aa] 326e fffe                 movea.w   -2(a6),a1
[000104ae] 5249                      addq.w    #1,a1
[000104b0] d3c9                      adda.l    a1,a1
[000104b2] d3c9                      adda.l    a1,a1
[000104b4] d3cb                      adda.l    a3,a1
[000104b6] 2091                      move.l    (a1),(a0)
[000104b8] 526e fffe                 addq.w    #1,-2(a6)
[000104bc] be6e fffe                 cmp.w     -2(a6),d7
[000104c0] 6ede                      bgt.s     $000104A0
[000104c2] 526e fffe                 addq.w    #1,-2(a6)
[000104c6] be6e fffe                 cmp.w     -2(a6),d7
[000104ca] 6e8a                      bgt.s     $00010456
[000104cc] 604c                      bra.s     $0001051A
[000104ce] 2eb9 0001 4780            move.l    _lp,(a7)
[000104d4] 2f39 0001 510e            move.l    _psname,-(a7)
[000104da] 6100 0706                 bsr       _streq
[000104de] 588f                      addq.l    #4,a7
[000104e0] 4a40                      tst.w     d0
[000104e2] 6736                      beq.s     $0001051A
[000104e4] 426e fffc                 clr.w     -4(a6)
[000104e8] 426e fffe                 clr.w     -2(a6)
[000104ec] 601e                      bra.s     $0001050C
[000104ee] 2f0b                      move.l    a3,-(a7)
[000104f0] 306e fffe                 movea.w   -2(a6),a0
[000104f4] d1c8                      adda.l    a0,a0
[000104f6] d1c8                      adda.l    a0,a0
[000104f8] 265f                      movea.l   (a7)+,a3
[000104fa] 2f33 8800                 move.l    0(a3,a0.l),-(a7)
[000104fe] 2079 0001 51a6            movea.l   _docom,a0
[00010504] 4e90                      jsr       (a0)
[00010506] 588f                      addq.l    #4,a7
[00010508] 526e fffe                 addq.w    #1,-2(a6)
[0001050c] be6e fffe                 cmp.w     -2(a6),d7
[00010510] 6edc                      bgt.s     $000104EE
[00010512] 4247                      clr.w     d7
[00010514] 4279 0001 51ae            clr.w     _psflg
[0001051a] 4a6e fffc                 tst.w     -4(a6)
[0001051e] 670a                      beq.s     $0001052A
[00010520] 3eb9 0001 50e0            move.w    _uflg,(a7)
[00010526] 6100 05de                 bsr       _skcopy
[0001052a] 4a79 0001 5cca            tst.w     _matchflg
[00010530] 660a                      bne.s     $0001053C
[00010532] 6100 057c                 bsr       _nextar
[00010536] 4a40                      tst.w     d0
[00010538] 6600 ff06                 bne       $00010440
[0001053c] 4a47                      tst.w     d7
[0001053e] 6f5a                      ble.s     $0001059A
[00010540] 426e fffe                 clr.w     -2(a6)
[00010544] 604e                      bra.s     $00010594
[00010546] 4a79 0001 50dc            tst.w     _rflg
[0001054c] 671c                      beq.s     $0001056A
[0001054e] 2f0b                      move.l    a3,-(a7)
[00010550] 306e fffe                 movea.w   -2(a6),a0
[00010554] d1c8                      adda.l    a0,a0
[00010556] d1c8                      adda.l    a0,a0
[00010558] 265f                      movea.l   (a7)+,a3
[0001055a] 2f33 8800                 move.l    0(a3,a0.l),-(a7)
[0001055e] 2079 0001 51a6            movea.l   _docom,a0
[00010564] 4e90                      jsr       (a0)
[00010566] 588f                      addq.l    #4,a7
[00010568] 6026                      bra.s     $00010590
[0001056a] 2f0b                      move.l    a3,-(a7)
[0001056c] 306e fffe                 movea.w   -2(a6),a0
[00010570] d1c8                      adda.l    a0,a0
[00010572] d1c8                      adda.l    a0,a0
[00010574] 265f                      movea.l   (a7)+,a3
[00010576] 2eb3 8800                 move.l    0(a3,a0.l),(a7)
[0001057a] 2f3c 0001 49e8            move.l    #$000149E8,-(a7)
[00010580] 4eb9 0001 1d6c            jsr       _printf
[00010586] 588f                      addq.l    #4,a7
[00010588] 33fc 0001 0001 50f2       move.w    #$0001,_exitstat
[00010590] 526e fffe                 addq.w    #1,-2(a6)
[00010594] be6e fffe                 cmp.w     -2(a6),d7
[00010598] 6eac                      bgt.s     $00010546
[0001059a] 42a7                      clr.l     -(a7)
[0001059c] 2079 0001 51a6            movea.l   _docom,a0
[000105a2] 4e90                      jsr       (a0)
[000105a4] 588f                      addq.l    #4,a7
[000105a6] 4a79 0001 50de            tst.w     _tflg
[000105ac] 6608                      bne.s     $000105B6
[000105ae] 4a79 0001 50d6            tst.w     _pflg
[000105b4] 6704                      beq.s     $000105BA
[000105b6] 6100 04b6                 bsr       _cleanup
[000105ba] 6100 09ae                 bsr       _tmp2ar
[000105be] 4279 0001 50f2            clr.w     _exitstat
[000105c4] 6100 04a8                 bsr       _cleanup
[000105c8] 4a9f                      tst.l     (a7)+
[000105ca] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[000105ce] 4e5e                      unlk      a6
[000105d0] 4e75                      rts
_openar:
[000105d2] 4e56 fffe                 link      a6,#$FFFE
[000105d6] 48e7 0104                 movem.l   d7/a5,-(a7)
[000105da] 2ebc 0001 49f6            move.l    #$000149F6,(a7)
[000105e0] 2f2e 0008                 move.l    8(a6),-(a7)
[000105e4] 4eb9 0001 1c10            jsr       _fopenb
[000105ea] 588f                      addq.l    #4,a7
[000105ec] 2a40                      movea.l   d0,a5
[000105ee] 200d                      move.l    a5,d0
[000105f0] 660c                      bne.s     $000105FE
[000105f2] 33fc 0001 0001 5148       move.w    #$0001,_areof
[000105fa] 4280                      clr.l     d0
[000105fc] 6050                      bra.s     $0001064E
[000105fe] 2e8d                      move.l    a5,(a7)
[00010600] 2f0e                      move.l    a6,-(a7)
[00010602] 5597                      subq.l    #2,(a7)
[00010604] 4eb9 0001 120c            jsr       _lgetw
[0001060a] 588f                      addq.l    #4,a7
[0001060c] 4a40                      tst.w     d0
[0001060e] 6610                      bne.s     $00010620
[00010610] 0c6e ff65 fffe            cmpi.w    #$FF65,-2(a6)
[00010616] 672c                      beq.s     $00010644
[00010618] 0c6e ff66 fffe            cmpi.w    #$FF66,-2(a6)
[0001061e] 6724                      beq.s     $00010644
[00010620] 3eae fffe                 move.w    -2(a6),(a7)
[00010624] 2f2e 0008                 move.l    8(a6),-(a7)
[00010628] 2f39 0001 5144            move.l    _myname,-(a7)
[0001062e] 2f3c 0001 49f8            move.l    #$000149F8,-(a7)
[00010634] 4eb9 0001 1d6c            jsr       _printf
[0001063a] dffc 0000 000c            adda.l    #$0000000C,a7
[00010640] 6100 041a                 bsr       _endit
[00010644] 33ee fffe 0001 4784       move.w    -2(a6),_libmagi
[0001064c] 200d                      move.l    a5,d0
[0001064e] 4a9f                      tst.l     (a7)+
[00010650] 4cdf 2000                 movem.l   (a7)+,a5
[00010654] 4e5e                      unlk      a6
[00010656] 4e75                      rts
xtell:
[00010658] 4e56 0000                 link      a6,#$0000
[0001065c] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00010660] 0cae ffff ffff 0008       cmpi.l    #$FFFFFFFF,8(a6)
[00010668] 670c                      beq.s     $00010676
[0001066a] 2eae 0008                 move.l    8(a6),(a7)
[0001066e] 6100 085c                 bsr       _ckafile
[00010672] 4a40                      tst.w     d0
[00010674] 6672                      bne.s     $000106E8
[00010676] 4a79 0001 50e2            tst.w     _vflg
[0001067c] 6762                      beq.s     $000106E0
[0001067e] 2079 0001 4780            movea.l   _lp,a0
[00010684] 3ea8 0014                 move.w    20(a0),(a7)
[00010688] 6100 0586                 bsr       _pmode
[0001068c] 2079 0001 4780            movea.l   _lp,a0
[00010692] 1028 0013                 move.b    19(a0),d0
[00010696] 4880                      ext.w     d0
[00010698] 3e80                      move.w    d0,(a7)
[0001069a] 2079 0001 4780            movea.l   _lp,a0
[000106a0] 1028 0012                 move.b    18(a0),d0
[000106a4] 4880                      ext.w     d0
[000106a6] 3f00                      move.w    d0,-(a7)
[000106a8] 2f3c 0001 4a17            move.l    #$00014A17,-(a7)
[000106ae] 4eb9 0001 1d6c            jsr       _printf
[000106b4] 5c8f                      addq.l    #6,a7
[000106b6] 2079 0001 4780            movea.l   _lp,a0
[000106bc] 2ea8 0016                 move.l    22(a0),(a7)
[000106c0] 2f3c 0001 4a1f            move.l    #$00014A1F,-(a7)
[000106c6] 4eb9 0001 1d6c            jsr       _printf
[000106cc] 588f                      addq.l    #4,a7
[000106ce] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[000106d4] 3f3c 0020                 move.w    #$0020,-(a7)
[000106d8] 4eb9 0001 2ee8            jsr       _fputc
[000106de] 548f                      addq.l    #2,a7
[000106e0] 6110                      bsr.s     _pfname
[000106e2] 4257                      clr.w     (a7)
[000106e4] 6100 0420                 bsr       _skcopy
[000106e8] 4a9f                      tst.l     (a7)+
[000106ea] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000106ee] 4e5e                      unlk      a6
[000106f0] 4e75                      rts
_pfname:
[000106f2] 4e56 0000                 link      a6,#$0000
[000106f6] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000106fa] 2a79 0001 4780            movea.l   _lp,a5
[00010700] 7e0e                      moveq.l   #14,d7
[00010702] 6016                      bra.s     $0001071A
[00010704] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[0001070a] 101d                      move.b    (a5)+,d0
[0001070c] 4880                      ext.w     d0
[0001070e] 3f00                      move.w    d0,-(a7)
[00010710] 4eb9 0001 2ee8            jsr       _fputc
[00010716] 548f                      addq.l    #2,a7
[00010718] 5347                      subq.w    #1,d7
[0001071a] 4a15                      tst.b     (a5)
[0001071c] 6704                      beq.s     $00010722
[0001071e] 4a47                      tst.w     d7
[00010720] 66e2                      bne.s     $00010704
[00010722] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[00010728] 3f3c 000a                 move.w    #$000A,-(a7)
[0001072c] 4eb9 0001 2ee8            jsr       _fputc
[00010732] 548f                      addq.l    #2,a7
[00010734] 4a9f                      tst.l     (a7)+
[00010736] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0001073a] 4e5e                      unlk      a6
[0001073c] 4e75                      rts
_replace:
[0001073e] 4e56 ffe0                 link      a6,#$FFE0
[00010742] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00010746] 4aae 0008                 tst.l     8(a6)
[0001074a] 664a                      bne.s     $00010796
[0001074c] 4a79 0001 51ac            tst.w     _bflg
[00010752] 673a                      beq.s     $0001078E
[00010754] 4a79 0001 5148            tst.w     _areof
[0001075a] 6632                      bne.s     $0001078E
[0001075c] 3ebc 0001                 move.w    #$0001,(a7)
[00010760] 2f3c ffff ffe6            move.l    #$FFFFFFE6,-(a7)
[00010766] 2f39 0001 50d8            move.l    _arfp,-(a7)
[0001076c] 4eb9 0001 17e2            jsr       _efseek
[00010772] 508f                      addq.l    #8,a7
[00010774] b07c ffff                 cmp.w     #$FFFF,d0
[00010778] 6614                      bne.s     $0001078E
[0001077a] 2eb9 0001 5144            move.l    _myname,(a7)
[00010780] 2f3c 0001 4a24            move.l    #$00014A24,-(a7)
[00010786] 4eb9 0001 1d6c            jsr       _printf
[0001078c] 588f                      addq.l    #4,a7
[0001078e] 6100 0570                 bsr       _cprest
[00010792] 6000 018e                 bra       $00010922
[00010796] 3ebc 0020                 move.w    #$0020,(a7)
[0001079a] 2f0e                      move.l    a6,-(a7)
[0001079c] 0697 ffff ffe0            addi.l    #$FFFFFFE0,(a7)
[000107a2] 2f2e 0008                 move.l    8(a6),-(a7)
[000107a6] 6100 050a                 bsr       _copystr
[000107aa] 508f                      addq.l    #8,a7
[000107ac] 2ebc 0001 4a35            move.l    #$00014A35,(a7)
[000107b2] 2f0e                      move.l    a6,-(a7)
[000107b4] 0697 ffff ffe0            addi.l    #$FFFFFFE0,(a7)
[000107ba] 4eb9 0001 1c10            jsr       _fopenb
[000107c0] 588f                      addq.l    #4,a7
[000107c2] 2a40                      movea.l   d0,a5
[000107c4] 200d                      move.l    a5,d0
[000107c6] 661c                      bne.s     $000107E4
[000107c8] 2eae 0008                 move.l    8(a6),(a7)
[000107cc] 2f39 0001 5144            move.l    _myname,-(a7)
[000107d2] 2f3c 0001 4a37            move.l    #$00014A37,-(a7)
[000107d8] 4eb9 0001 1d6c            jsr       _printf
[000107de] 508f                      addq.l    #8,a7
[000107e0] 6100 027a                 bsr       _endit
[000107e4] 2ebc 0001 518a            move.l    #_couthd,(a7)
[000107ea] 2f0d                      move.l    a5,-(a7)
[000107ec] 4eb9 0001 1276            jsr       _getchd
[000107f2] 588f                      addq.l    #4,a7
[000107f4] 4a40                      tst.w     d0
[000107f6] 6c1c                      bge.s     $00010814
[000107f8] 2eae 0008                 move.l    8(a6),(a7)
[000107fc] 2f39 0001 5144            move.l    _myname,-(a7)
[00010802] 2f3c 0001 4a4a            move.l    #$00014A4A,-(a7)
[00010808] 4eb9 0001 1d6c            jsr       _printf
[0001080e] 508f                      addq.l    #8,a7
[00010810] 6100 024a                 bsr       _endit
[00010814] 4a79 0001 5148            tst.w     _areof
[0001081a] 6726                      beq.s     $00010842
[0001081c] 4a79 0001 51ae            tst.w     _psflg
[00010822] 671e                      beq.s     $00010842
[00010824] 2eb9 0001 510e            move.l    _psname,(a7)
[0001082a] 2f39 0001 5144            move.l    _myname,-(a7)
[00010830] 2f3c 0001 4a5d            move.l    #$00014A5D,-(a7)
[00010836] 4eb9 0001 1d6c            jsr       _printf
[0001083c] 508f                      addq.l    #8,a7
[0001083e] 6100 021c                 bsr       _endit
[00010842] 3039 0001 51ac            move.w    _bflg,d0
[00010848] 8079 0001 51aa            or.w      _aflg,d0
[0001084e] 671e                      beq.s     $0001086E
[00010850] 4a79 0001 5cca            tst.w     _matchflg
[00010856] 6616                      bne.s     $0001086E
[00010858] 4a79 0001 51aa            tst.w     _aflg
[0001085e] 6708                      beq.s     $00010868
[00010860] 3ebc 0001                 move.w    #$0001,(a7)
[00010864] 6100 02a0                 bsr       _skcopy
[00010868] 5279 0001 5cca            addq.w    #1,_matchflg
[0001086e] 3ebc 000e                 move.w    #$000E,(a7)
[00010872] 2f39 0001 4780            move.l    _lp,-(a7)
[00010878] 2f2e 0008                 move.l    8(a6),-(a7)
[0001087c] 6100 0434                 bsr       _copystr
[00010880] 508f                      addq.l    #8,a7
[00010882] 3039 0001 5148            move.w    _areof,d0
[00010888] 8079 0001 51aa            or.w      _aflg,d0
[0001088e] 670a                      beq.s     $0001089A
[00010890] 3ebc 0061                 move.w    #$0061,(a7)
[00010894] 6100 03e0                 bsr       _inform
[00010898] 6020                      bra.s     $000108BA
[0001089a] 4a79 0001 51ac            tst.w     _bflg
[000108a0] 670a                      beq.s     $000108AC
[000108a2] 3ebc 0069                 move.w    #$0069,(a7)
[000108a6] 6100 03ce                 bsr       _inform
[000108aa] 600e                      bra.s     $000108BA
[000108ac] 3ebc 0072                 move.w    #$0072,(a7)
[000108b0] 6100 03c4                 bsr       _inform
[000108b4] 4257                      clr.w     (a7)
[000108b6] 6100 024e                 bsr       _skcopy
[000108ba] 2039 0001 518c            move.l    $0001518C,d0
[000108c0] d0b9 0001 5190            add.l     $00015190,d0
[000108c6] d0b9 0001 5198            add.l     $00015198,d0
[000108cc] d0bc 0000 001c            add.l     #$0000001C,d0
[000108d2] 2279 0001 4780            movea.l   _lp,a1
[000108d8] 2340 0016                 move.l    d0,22(a1)
[000108dc] 4a79 0001 51a4            tst.w     $000151A4
[000108e2] 6616                      bne.s     $000108FA
[000108e4] 2039 0001 518c            move.l    $0001518C,d0
[000108ea] d0b9 0001 5190            add.l     $00015190,d0
[000108f0] 2279 0001 4780            movea.l   _lp,a1
[000108f6] d1a9 0016                 add.l     d0,22(a1)
[000108fa] 2eb9 0001 50e6            move.l    _tempnam,(a7)
[00010900] 2f2e 0008                 move.l    8(a6),-(a7)
[00010904] 3f3c 000e                 move.w    #$000E,-(a7)
[00010908] 2f39 0001 50ee            move.l    _tempfd,-(a7)
[0001090e] 2f0d                      move.l    a5,-(a7)
[00010910] 6100 0408                 bsr       _cp1file
[00010914] dffc 0000 000e            adda.l    #$0000000E,a7
[0001091a] 2e8d                      move.l    a5,(a7)
[0001091c] 4eb9 0001 2614            jsr       _fclose
[00010922] 4a9f                      tst.l     (a7)+
[00010924] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00010928] 4e5e                      unlk      a6
[0001092a] 4e75                      rts
_delete:
[0001092c] 4e56 fffc                 link      a6,#$FFFC
[00010930] 4aae 0008                 tst.l     8(a6)
[00010934] 662c                      bne.s     $00010962
[00010936] 6100 03c8                 bsr       _cprest
[0001093a] 4257                      clr.w     (a7)
[0001093c] 42a7                      clr.l     -(a7)
[0001093e] 2f39 0001 50ee            move.l    _tempfd,-(a7)
[00010944] 4eb9 0001 17e2            jsr       _efseek
[0001094a] 508f                      addq.l    #8,a7
[0001094c] 2eb9 0001 50ee            move.l    _tempfd,(a7)
[00010952] 2f3c 0001 4784            move.l    #_libmagi,-(a7)
[00010958] 4eb9 0001 154c            jsr       _lputw
[0001095e] 588f                      addq.l    #4,a7
[00010960] 602c                      bra.s     $0001098E
[00010962] 2eae 0008                 move.l    8(a6),(a7)
[00010966] 2f3c 0001 4a74            move.l    #$00014A74,-(a7)
[0001096c] 4eb9 0001 398a            jsr       _strcmp
[00010972] 588f                      addq.l    #4,a7
[00010974] 4a40                      tst.w     d0
[00010976] 6608                      bne.s     $00010980
[00010978] 33fc ff65 0001 4784       move.w    #$FF65,_libmagi
[00010980] 3ebc 0064                 move.w    #$0064,(a7)
[00010984] 6100 02f0                 bsr       _inform
[00010988] 4257                      clr.w     (a7)
[0001098a] 6100 017a                 bsr       _skcopy
[0001098e] 4e5e                      unlk      a6
[00010990] 4e75                      rts
_extract:
[00010992] 4e56 0000                 link      a6,#$0000
[00010996] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[0001099a] 2eae 0008                 move.l    8(a6),(a7)
[0001099e] 6100 052c                 bsr       _ckafile
[000109a2] 4a40                      tst.w     d0
[000109a4] 6672                      bne.s     $00010A18
[000109a6] 3ebc 01a4                 move.w    #$01A4,(a7)
[000109aa] 2f2e 0008                 move.l    8(a6),-(a7)
[000109ae] 4eb9 0001 25b0            jsr       _creatb
[000109b4] 588f                      addq.l    #4,a7
[000109b6] 3e00                      move.w    d0,d7
[000109b8] 6c1c                      bge.s     $000109D6
[000109ba] 2eae 0008                 move.l    8(a6),(a7)
[000109be] 2f39 0001 5144            move.l    _myname,-(a7)
[000109c4] 2f3c 0001 4a7d            move.l    #$00014A7D,-(a7)
[000109ca] 4eb9 0001 1d6c            jsr       _printf
[000109d0] 508f                      addq.l    #8,a7
[000109d2] 6100 0088                 bsr       _endit
[000109d6] 2ebc 0001 4a92            move.l    #$00014A92,(a7)
[000109dc] 3f07                      move.w    d7,-(a7)
[000109de] 4eb9 0001 1a14            jsr       _fdopen
[000109e4] 548f                      addq.l    #2,a7
[000109e6] 2a40                      movea.l   d0,a5
[000109e8] 3ebc 0078                 move.w    #$0078,(a7)
[000109ec] 6100 0288                 bsr       _inform
[000109f0] 2eae 0008                 move.l    8(a6),(a7)
[000109f4] 2f39 0001 50ea            move.l    _arname,-(a7)
[000109fa] 3f3c 0001                 move.w    #$0001,-(a7)
[000109fe] 2f0d                      move.l    a5,-(a7)
[00010a00] 2f39 0001 50d8            move.l    _arfp,-(a7)
[00010a06] 6100 0312                 bsr       _cp1file
[00010a0a] dffc 0000 000e            adda.l    #$0000000E,a7
[00010a10] 2e8d                      move.l    a5,(a7)
[00010a12] 4eb9 0001 2614            jsr       _fclose
[00010a18] 4a9f                      tst.l     (a7)+
[00010a1a] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00010a1e] 4e5e                      unlk      a6
[00010a20] 4e75                      rts
_print:
[00010a22] 4e56 fffc                 link      a6,#$FFFC
[00010a26] 2eae 0008                 move.l    8(a6),(a7)
[00010a2a] 6100 04a0                 bsr       _ckafile
[00010a2e] 4a40                      tst.w     d0
[00010a30] 6626                      bne.s     $00010A58
[00010a32] 2ebc 0001 4a94            move.l    #$00014A94,(a7)
[00010a38] 2f39 0001 50ea            move.l    _arname,-(a7)
[00010a3e] 3f3c 0001                 move.w    #$0001,-(a7)
[00010a42] 2f3c 0001 4bd8            move.l    #$00014BD8,-(a7)
[00010a48] 2f39 0001 50d8            move.l    _arfp,-(a7)
[00010a4e] 6100 02ca                 bsr       _cp1file
[00010a52] dffc 0000 000e            adda.l    #$0000000E,a7
[00010a58] 4e5e                      unlk      a6
[00010a5a] 4e75                      rts
_endit:
[00010a5c] 4e56 fffc                 link      a6,#$FFFC
[00010a60] 33fc 0001 0001 50f2       move.w    #$0001,_exitstat
[00010a68] 6104                      bsr.s     _cleanup
[00010a6a] 4e5e                      unlk      a6
[00010a6c] 4e75                      rts
_cleanup:
[00010a6e] 4e56 fffc                 link      a6,#$FFFC
[00010a72] 4ab9 0001 50e6            tst.l     _tempnam
[00010a78] 670c                      beq.s     $00010A86
[00010a7a] 2eb9 0001 50e6            move.l    _tempnam,(a7)
[00010a80] 4eb9 0001 20f4            jsr       _unlink
[00010a86] 3eb9 0001 50f2            move.w    _exitstat,(a7)
[00010a8c] 4eb9 0001 25ca            jsr       _exit
[00010a92] 4e5e                      unlk      a6
[00010a94] 4e75                      rts
_listall:
[00010a96] 4e56 fffc                 link      a6,#$FFFC
[00010a9a] 600a                      bra.s     $00010AA6
[00010a9c] 2ebc ffff ffff            move.l    #$FFFFFFFF,(a7)
[00010aa2] 6100 fbb4                 bsr       _xtell
[00010aa6] 6108                      bsr.s     _nextar
[00010aa8] 4a40                      tst.w     d0
[00010aaa] 66f0                      bne.s     $00010A9C
[00010aac] 4e5e                      unlk      a6
[00010aae] 4e75                      rts
_nextar:
[00010ab0] 4e56 fffc                 link      a6,#$FFFC
[00010ab4] 4a79 0001 5148            tst.w     _areof
[00010aba] 6628                      bne.s     $00010AE4
[00010abc] 2ebc 0001 50f4            move.l    #_libhd,(a7)
[00010ac2] 2f39 0001 50d8            move.l    _arfp,-(a7)
[00010ac8] 4eb9 0001 16e8            jsr       _getarhd
[00010ace] 588f                      addq.l    #4,a7
[00010ad0] b07c ffff                 cmp.w     #$FFFF,d0
[00010ad4] 670e                      beq.s     $00010AE4
[00010ad6] 2079 0001 50d8            movea.l   _arfp,a0
[00010adc] 0828 0005 0003            btst      #5,3(a0)
[00010ae2] 670a                      beq.s     $00010AEE
[00010ae4] 5279 0001 5148            addq.w    #1,_areof
[00010aea] 4240                      clr.w     d0
[00010aec] 6014                      bra.s     $00010B02
[00010aee] 4a39 0001 50f4            tst.b     _libhd
[00010af4] 660a                      bne.s     $00010B00
[00010af6] 5279 0001 5148            addq.w    #1,_areof
[00010afc] 4240                      clr.w     d0
[00010afe] 6002                      bra.s     $00010B02
[00010b00] 7001                      moveq.l   #1,d0
[00010b02] 4e5e                      unlk      a6
[00010b04] 4e75                      rts
_skcopy:
[00010b06] 4e56 fe00                 link      a6,#$FE00
[00010b0a] 48e7 0700                 movem.l   d5-d7,-(a7)
[00010b0e] 4a79 0001 5148            tst.w     _areof
[00010b14] 6600 007c                 bne       $00010B92
[00010b18] 2079 0001 4780            movea.l   _lp,a0
[00010b1e] 2e28 0016                 move.l    22(a0),d7
[00010b22] 0807 0000                 btst      #0,d7
[00010b26] 6702                      beq.s     $00010B2A
[00010b28] 5287                      addq.l    #1,d7
[00010b2a] 4a6e 0008                 tst.w     8(a6)
[00010b2e] 6730                      beq.s     $00010B60
[00010b30] 3ebc 0063                 move.w    #$0063,(a7)
[00010b34] 6100 0140                 bsr       _inform
[00010b38] 2eb9 0001 50e6            move.l    _tempnam,(a7)
[00010b3e] 2f39 0001 50ea            move.l    _arname,-(a7)
[00010b44] 3f3c 0007                 move.w    #$0007,-(a7)
[00010b48] 2f39 0001 50ee            move.l    _tempfd,-(a7)
[00010b4e] 2f39 0001 50d8            move.l    _arfp,-(a7)
[00010b54] 6100 01c4                 bsr       _cp1file
[00010b58] dffc 0000 000e            adda.l    #$0000000E,a7
[00010b5e] 6032                      bra.s     $00010B92
[00010b60] 3ebc 0001                 move.w    #$0001,(a7)
[00010b64] 2f07                      move.l    d7,-(a7)
[00010b66] 2f39 0001 50d8            move.l    _arfp,-(a7)
[00010b6c] 4eb9 0001 17e2            jsr       _efseek
[00010b72] 508f                      addq.l    #8,a7
[00010b74] b07c ffff                 cmp.w     #$FFFF,d0
[00010b78] 6618                      bne.s     $00010B92
[00010b7a] 2eb9 0001 5144            move.l    _myname,(a7)
[00010b80] 2f3c 0001 4a9f            move.l    #$00014A9F,-(a7)
[00010b86] 4eb9 0001 1d6c            jsr       _printf
[00010b8c] 588f                      addq.l    #4,a7
[00010b8e] 6100 fecc                 bsr       _endit
[00010b92] 4a9f                      tst.l     (a7)+
[00010b94] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[00010b98] 4e5e                      unlk      a6
[00010b9a] 4e75                      rts
_mktemp:
[00010b9c] 4e56 0000                 link      a6,#$0000
[00010ba0] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00010ba4] 4eb9 0001 1d60            jsr       _getpid
[00010baa] 3e00                      move.w    d0,d7
[00010bac] 2a6e 0008                 movea.l   8(a6),a5
[00010bb0] 6002                      bra.s     $00010BB4
[00010bb2] 528d                      addq.l    #1,a5
[00010bb4] 4a15                      tst.b     (a5)
[00010bb6] 66fa                      bne.s     $00010BB2
[00010bb8] 4246                      clr.w     d6
[00010bba] 6012                      bra.s     $00010BCE
[00010bbc] 3007                      move.w    d7,d0
[00010bbe] c07c 0007                 and.w     #$0007,d0
[00010bc2] d07c 0030                 add.w     #$0030,d0
[00010bc6] 4880                      ext.w     d0
[00010bc8] 1b00                      move.b    d0,-(a5)
[00010bca] e647                      asr.w     #3,d7
[00010bcc] 5246                      addq.w    #1,d6
[00010bce] bc7c 0005                 cmp.w     #$0005,d6
[00010bd2] 6de8                      blt.s     $00010BBC
[00010bd4] 202e 0008                 move.l    8(a6),d0
[00010bd8] 4a9f                      tst.l     (a7)+
[00010bda] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00010bde] 4e5e                      unlk      a6
[00010be0] 4e75                      rts
_streq:
[00010be2] 4e56 0000                 link      a6,#$0000
[00010be6] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00010bea] 2a6e 0008                 movea.l   8(a6),a5
[00010bee] 286e 000c                 movea.l   12(a6),a4
[00010bf2] 6008                      bra.s     $00010BFC
[00010bf4] 4a1c                      tst.b     (a4)+
[00010bf6] 6604                      bne.s     $00010BFC
[00010bf8] 7001                      moveq.l   #1,d0
[00010bfa] 600a                      bra.s     $00010C06
[00010bfc] 101d                      move.b    (a5)+,d0
[00010bfe] 4880                      ext.w     d0
[00010c00] b014                      cmp.b     (a4),d0
[00010c02] 67f0                      beq.s     $00010BF4
[00010c04] 4240                      clr.w     d0
[00010c06] 4a9f                      tst.l     (a7)+
[00010c08] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00010c0c] 4e5e                      unlk      a6
[00010c0e] 4e75                      rts
_pmode:
[00010c10] 4e56 0000                 link      a6,#$0000
[00010c14] 48e7 0104                 movem.l   d7/a5,-(a7)
[00010c18] 2a7c 0001 4826            movea.l   #$00014826,a5
[00010c1e] 600a                      bra.s     $00010C2A
[00010c20] 3eae 0008                 move.w    8(a6),(a7)
[00010c24] 2f1d                      move.l    (a5)+,-(a7)
[00010c26] 6114                      bsr.s     _select
[00010c28] 588f                      addq.l    #4,a7
[00010c2a] bbfc 0001 484a            cmpa.l    #$0001484A,a5
[00010c30] 65ee                      bcs.s     $00010C20
[00010c32] 4a9f                      tst.l     (a7)+
[00010c34] 4cdf 2000                 movem.l   (a7)+,a5
[00010c38] 4e5e                      unlk      a6
[00010c3a] 4e75                      rts
_select:
[00010c3c] 4e56 0000                 link      a6,#$0000
[00010c40] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00010c44] 2a6e 0008                 movea.l   8(a6),a5
[00010c48] 3c2e 000c                 move.w    12(a6),d6
[00010c4c] 3e1d                      move.w    (a5)+,d7
[00010c4e] 6002                      bra.s     $00010C52
[00010c50] 548d                      addq.l    #2,a5
[00010c52] 5347                      subq.w    #1,d7
[00010c54] 6d06                      blt.s     $00010C5C
[00010c56] 301d                      move.w    (a5)+,d0
[00010c58] c046                      and.w     d6,d0
[00010c5a] 67f4                      beq.s     $00010C50
[00010c5c] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[00010c62] 3f15                      move.w    (a5),-(a7)
[00010c64] 4eb9 0001 2ee8            jsr       _fputc
[00010c6a] 548f                      addq.l    #2,a7
[00010c6c] 4a9f                      tst.l     (a7)+
[00010c6e] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00010c72] 4e5e                      unlk      a6
[00010c74] 4e75                      rts
_inform:
[00010c76] 4e56 fffc                 link      a6,#$FFFC
[00010c7a] 4a79 0001 50e2            tst.w     _vflg
[00010c80] 672c                      beq.s     $00010CAE
[00010c82] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[00010c88] 102e 0009                 move.b    9(a6),d0
[00010c8c] 4880                      ext.w     d0
[00010c8e] 3f00                      move.w    d0,-(a7)
[00010c90] 4eb9 0001 2ee8            jsr       _fputc
[00010c96] 548f                      addq.l    #2,a7
[00010c98] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[00010c9e] 3f3c 0020                 move.w    #$0020,-(a7)
[00010ca2] 4eb9 0001 2ee8            jsr       _fputc
[00010ca8] 548f                      addq.l    #2,a7
[00010caa] 6100 fa46                 bsr       $000106F2
[00010cae] 4e5e                      unlk      a6
[00010cb0] 4e75                      rts
_copystr:
[00010cb2] 4e56 0000                 link      a6,#$0000
[00010cb6] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00010cba] 99cc                      suba.l    a4,a4
[00010cbc] 2a6e 0008                 movea.l   8(a6),a5
[00010cc0] 6008                      bra.s     $00010CCA
[00010cc2] 0c1d 003a                 cmpi.b    #$3A,(a5)+
[00010cc6] 6602                      bne.s     $00010CCA
[00010cc8] 284d                      movea.l   a5,a4
[00010cca] 4a15                      tst.b     (a5)
[00010ccc] 66f4                      bne.s     $00010CC2
[00010cce] 200c                      move.l    a4,d0
[00010cd0] 6704                      beq.s     $00010CD6
[00010cd2] 2a4c                      movea.l   a4,a5
[00010cd4] 6004                      bra.s     $00010CDA
[00010cd6] 2a6e 0008                 movea.l   8(a6),a5
[00010cda] 286e 000c                 movea.l   12(a6),a4
[00010cde] 3e2e 0010                 move.w    16(a6),d7
[00010ce2] 6006                      bra.s     $00010CEA
[00010ce4] 18dd                      move.b    (a5)+,(a4)+
[00010ce6] 6706                      beq.s     $00010CEE
[00010ce8] 5347                      subq.w    #1,d7
[00010cea] 4a47                      tst.w     d7
[00010cec] 66f6                      bne.s     $00010CE4
[00010cee] 6002                      bra.s     $00010CF2
[00010cf0] 421c                      clr.b     (a4)+
[00010cf2] 5347                      subq.w    #1,d7
[00010cf4] 6efa                      bgt.s     $00010CF0
[00010cf6] 4a9f                      tst.l     (a7)+
[00010cf8] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00010cfc] 4e5e                      unlk      a6
[00010cfe] 4e75                      rts
_cprest:
[00010d00] 4e56 fffc                 link      a6,#$FFFC
[00010d04] 6008                      bra.s     $00010D0E
[00010d06] 3ebc 0001                 move.w    #$0001,(a7)
[00010d0a] 6100 fdfa                 bsr       _skcopy
[00010d0e] 6100 fda0                 bsr       _nextar
[00010d12] 4a40                      tst.w     d0
[00010d14] 66f0                      bne.s     $00010D06
[00010d16] 4e5e                      unlk      a6
[00010d18] 4e75                      rts
_cp1file:
[00010d1a] 4e56 ffce                 link      a6,#$FFCE
[00010d1e] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00010d22] 3a2e 0010                 move.w    16(a6),d5
[00010d26] 0805 0002                 btst      #2,d5
[00010d2a] 6754                      beq.s     $00010D80
[00010d2c] 2ebc 0001 50f4            move.l    #_libhd,(a7)
[00010d32] 2f2e 000c                 move.l    12(a6),-(a7)
[00010d36] 4eb9 0001 13ca            jsr       _putarhd
[00010d3c] 588f                      addq.l    #4,a7
[00010d3e] 4a40                      tst.w     d0
[00010d40] 673e                      beq.s     $00010D80
[00010d42] 2eae 0016                 move.l    22(a6),(a7)
[00010d46] 2f39 0001 5144            move.l    _myname,-(a7)
[00010d4c] 2f3c 0001 4aba            move.l    #$00014ABA,-(a7)
[00010d52] 2f0e                      move.l    a6,-(a7)
[00010d54] 0697 ffff ffce            addi.l    #$FFFFFFCE,(a7)
[00010d5a] 4eb9 0001 28da            jsr       _sprintf
[00010d60] dffc 0000 000c            adda.l    #$0000000C,a7
[00010d66] 2e8e                      move.l    a6,(a7)
[00010d68] 0697 ffff ffce            addi.l    #$FFFFFFCE,(a7)
[00010d6e] 2f3c 0001 4ad0            move.l    #$00014AD0,-(a7)
[00010d74] 4eb9 0001 1d6c            jsr       _printf
[00010d7a] 588f                      addq.l    #4,a7
[00010d7c] 6100 fcde                 bsr       _endit
[00010d80] 0805 0003                 btst      #3,d5
[00010d84] 6766                      beq.s     $00010DEC
[00010d86] 2ebc 0001 518a            move.l    #_couthd,(a7)
[00010d8c] 2f2e 000c                 move.l    12(a6),-(a7)
[00010d90] 4eb9 0001 1594            jsr       _putchd
[00010d96] 588f                      addq.l    #4,a7
[00010d98] 4a40                      tst.w     d0
[00010d9a] 673e                      beq.s     $00010DDA
[00010d9c] 2eae 0016                 move.l    22(a6),(a7)
[00010da0] 2f39 0001 5144            move.l    _myname,-(a7)
[00010da6] 2f3c 0001 4ad4            move.l    #$00014AD4,-(a7)
[00010dac] 2f0e                      move.l    a6,-(a7)
[00010dae] 0697 ffff ffce            addi.l    #$FFFFFFCE,(a7)
[00010db4] 4eb9 0001 28da            jsr       _sprintf
[00010dba] dffc 0000 000c            adda.l    #$0000000C,a7
[00010dc0] 2e8e                      move.l    a6,(a7)
[00010dc2] 0697 ffff ffce            addi.l    #$FFFFFFCE,(a7)
[00010dc8] 2f3c 0001 4aef            move.l    #$00014AEF,-(a7)
[00010dce] 4eb9 0001 1d6c            jsr       _printf
[00010dd4] 588f                      addq.l    #4,a7
[00010dd6] 6100 fc84                 bsr       _endit
[00010dda] 2079 0001 4780            movea.l   _lp,a0
[00010de0] 2c28 0016                 move.l    22(a0),d6
[00010de4] dcbc ffff ffe4            add.l     #$FFFFFFE4,d6
[00010dea] 600a                      bra.s     $00010DF6
[00010dec] 2079 0001 4780            movea.l   _lp,a0
[00010df2] 2c28 0016                 move.l    22(a0),d6
[00010df6] 606a                      bra.s     $00010E62
[00010df8] bcbc 0000 0200            cmp.l     #$00000200,d6
[00010dfe] 6c04                      bge.s     $00010E04
[00010e00] 3806                      move.w    d6,d4
[00010e02] 6004                      bra.s     $00010E08
[00010e04] 383c 0200                 move.w    #$0200,d4
[00010e08] 2eae 0008                 move.l    8(a6),(a7)
[00010e0c] 3f04                      move.w    d4,-(a7)
[00010e0e] 3f3c 0001                 move.w    #$0001,-(a7)
[00010e12] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[00010e18] 4eb9 0001 1c2a            jsr       _fread
[00010e1e] 508f                      addq.l    #8,a7
[00010e20] 3e00                      move.w    d0,d7
[00010e22] 4a47                      tst.w     d7
[00010e24] 6618                      bne.s     $00010E3E
[00010e26] 2eb9 0001 5144            move.l    _myname,(a7)
[00010e2c] 2f3c 0001 4af3            move.l    #$00014AF3,-(a7)
[00010e32] 4eb9 0001 1d6c            jsr       _printf
[00010e38] 588f                      addq.l    #4,a7
[00010e3a] 6100 fc20                 bsr       _endit
[00010e3e] 2eae 000c                 move.l    12(a6),(a7)
[00010e42] 3f07                      move.w    d7,-(a7)
[00010e44] 3f3c 0001                 move.w    #$0001,-(a7)
[00010e48] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[00010e4e] 4eb9 0001 1c78            jsr       _fwrite
[00010e54] 508f                      addq.l    #8,a7
[00010e56] b047                      cmp.w     d7,d0
[00010e58] 6600 fee8                 bne       $00010D42
[00010e5c] 3007                      move.w    d7,d0
[00010e5e] 48c0                      ext.l     d0
[00010e60] 9c80                      sub.l     d0,d6
[00010e62] 4a86                      tst.l     d6
[00010e64] 6692                      bne.s     $00010DF8
[00010e66] 0805 0001                 btst      #1,d5
[00010e6a] 6728                      beq.s     $00010E94
[00010e6c] 2079 0001 4780            movea.l   _lp,a0
[00010e72] 0828 0000 0019            btst      #0,25(a0)
[00010e78] 671a                      beq.s     $00010E94
[00010e7a] 2eae 000c                 move.l    12(a6),(a7)
[00010e7e] 3f3c 0001                 move.w    #$0001,-(a7)
[00010e82] 3f3c 0001                 move.w    #$0001,-(a7)
[00010e86] 2f3c 0001 4b03            move.l    #$00014B03,-(a7)
[00010e8c] 4eb9 0001 1c78            jsr       _fwrite
[00010e92] 508f                      addq.l    #8,a7
[00010e94] 0805 0000                 btst      #0,d5
[00010e98] 6728                      beq.s     $00010EC2
[00010e9a] 2079 0001 4780            movea.l   _lp,a0
[00010ea0] 0828 0000 0019            btst      #0,25(a0)
[00010ea6] 671a                      beq.s     $00010EC2
[00010ea8] 2eae 0008                 move.l    8(a6),(a7)
[00010eac] 3f3c 0001                 move.w    #$0001,-(a7)
[00010eb0] 3f3c 0001                 move.w    #$0001,-(a7)
[00010eb4] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[00010eba] 4eb9 0001 1c2a            jsr       _fread
[00010ec0] 508f                      addq.l    #8,a7
[00010ec2] 4a9f                      tst.l     (a7)+
[00010ec4] 4cdf 00f0                 movem.l   (a7)+,d4-d7
[00010ec8] 4e5e                      unlk      a6
[00010eca] 4e75                      rts
_ckafile:
[00010ecc] 4e56 fffc                 link      a6,#$FFFC
[00010ed0] 4aae 0008                 tst.l     8(a6)
[00010ed4] 6708                      beq.s     $00010EDE
[00010ed6] 206e 0008                 movea.l   8(a6),a0
[00010eda] 4a10                      tst.b     (a0)
[00010edc] 6604                      bne.s     $00010EE2
[00010ede] 6100 fb8e                 bsr       _cleanup
[00010ee2] 4a79 0001 5148            tst.w     _areof
[00010ee8] 671c                      beq.s     $00010F06
[00010eea] 2eae 0008                 move.l    8(a6),(a7)
[00010eee] 2f39 0001 5144            move.l    _myname,-(a7)
[00010ef4] 2f3c 0001 4b04            move.l    #$00014B04,-(a7)
[00010efa] 4eb9 0001 1d6c            jsr       _printf
[00010f00] 508f                      addq.l    #8,a7
[00010f02] 7001                      moveq.l   #1,d0
[00010f04] 6002                      bra.s     $00010F08
[00010f06] 4240                      clr.w     d0
[00010f08] 4e5e                      unlk      a6
[00010f0a] 4e75                      rts
_exall:
[00010f0c] 4e56 fffc                 link      a6,#$FFFC
[00010f10] 6100 fb9e                 bsr       _nextar
[00010f14] 4a40                      tst.w     d0
[00010f16] 673c                      beq.s     $00010F54
[00010f18] 2ebc 0001 4b20            move.l    #$00014B20,(a7)
[00010f1e] 2f39 0001 4780            move.l    _lp,-(a7)
[00010f24] 4eb9 0001 398a            jsr       _strcmp
[00010f2a] 588f                      addq.l    #4,a7
[00010f2c] 4a40                      tst.w     d0
[00010f2e] 6624                      bne.s     $00010F54
[00010f30] 3ebc 0001                 move.w    #$0001,(a7)
[00010f34] 2079 0001 4780            movea.l   _lp,a0
[00010f3a] 2f28 0016                 move.l    22(a0),-(a7)
[00010f3e] 2f39 0001 50d8            move.l    _arfp,-(a7)
[00010f44] 4eb9 0001 17e2            jsr       _efseek
[00010f4a] 508f                      addq.l    #8,a7
[00010f4c] 6100 fb62                 bsr       _nextar
[00010f50] 4a40                      tst.w     d0
[00010f52] 6712                      beq.s     $00010F66
[00010f54] 2eb9 0001 4780            move.l    _lp,(a7)
[00010f5a] 6100 fa36                 bsr       _extract
[00010f5e] 6100 fb50                 bsr       _nextar
[00010f62] 4a40                      tst.w     d0
[00010f64] 66ee                      bne.s     $00010F54
[00010f66] 4e5e                      unlk      a6
[00010f68] 4e75                      rts
_tmp2ar:
[00010f6a] 4e56 0000                 link      a6,#$0000
[00010f6e] 48e7 0f00                 movem.l   d4-d7,-(a7)
[00010f72] 3ebc 01b6                 move.w    #$01B6,(a7)
[00010f76] 2f39 0001 50ea            move.l    _arname,-(a7)
[00010f7c] 4eb9 0001 25b0            jsr       _creatb
[00010f82] 588f                      addq.l    #4,a7
[00010f84] 3a00                      move.w    d0,d5
[00010f86] 6c2e                      bge.s     $00010FB6
[00010f88] 2eb9 0001 50e6            move.l    _tempnam,(a7)
[00010f8e] 2f39 0001 50ea            move.l    _arname,-(a7)
[00010f94] 2f39 0001 5144            move.l    _myname,-(a7)
[00010f9a] 2f3c 0001 4b29            move.l    #$00014B29,-(a7)
[00010fa0] 4eb9 0001 1d6c            jsr       _printf
[00010fa6] dffc 0000 000c            adda.l    #$0000000C,a7
[00010fac] 42b9 0001 50e6            clr.l     _tempnam
[00010fb2] 6000 00b8                 bra       $0001106C
[00010fb6] 2eb9 0001 50ee            move.l    _tempfd,(a7)
[00010fbc] 4eb9 0001 26fc            jsr       _fflush
[00010fc2] 2eb9 0001 50ee            move.l    _tempfd,(a7)
[00010fc8] 4eb9 0001 2614            jsr       _fclose
[00010fce] 4257                      clr.w     (a7)
[00010fd0] 2f39 0001 50e6            move.l    _tempnam,-(a7)
[00010fd6] 4eb9 0001 28c0            jsr       _openb
[00010fdc] 588f                      addq.l    #4,a7
[00010fde] 3c00                      move.w    d0,d6
[00010fe0] 6c1c                      bge.s     $00010FFE
[00010fe2] 2eb9 0001 50e6            move.l    _tempnam,(a7)
[00010fe8] 2f39 0001 5144            move.l    _myname,-(a7)
[00010fee] 2f3c 0001 4b51            move.l    #$00014B51,-(a7)
[00010ff4] 4eb9 0001 1d6c            jsr       _printf
[00010ffa] 508f                      addq.l    #8,a7
[00010ffc] 606e                      bra.s     $0001106C
[00010ffe] 6012                      bra.s     $00011012
[00011000] 3e87                      move.w    d7,(a7)
[00011002] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[00011008] 3f05                      move.w    d5,-(a7)
[0001100a] 4eb9 0001 3614            jsr       _write
[00011010] 5c8f                      addq.l    #6,a7
[00011012] 3ebc 0200                 move.w    #$0200,(a7)
[00011016] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[0001101c] 3f06                      move.w    d6,-(a7)
[0001101e] 4eb9 0001 1edc            jsr       _read
[00011024] 5c8f                      addq.l    #6,a7
[00011026] 3e00                      move.w    d0,d7
[00011028] 6ed6                      bgt.s     $00011000
[0001102a] 4239 0001 4ed6            clr.b     $00014ED6
[00011030] 4239 0001 4ed7            clr.b     $00014ED7
[00011036] 4239 0001 4ed8            clr.b     $00014ED8
[0001103c] 4239 0001 4ed9            clr.b     $00014ED9
[00011042] 3ebc 0004                 move.w    #$0004,(a7)
[00011046] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[0001104c] 3f05                      move.w    d5,-(a7)
[0001104e] 4eb9 0001 3614            jsr       _write
[00011054] 5c8f                      addq.l    #6,a7
[00011056] 2ebc 0001 4b68            move.l    #$00014B68,(a7)
[0001105c] 3f06                      move.w    d6,-(a7)
[0001105e] 4eb9 0001 1a14            jsr       _fdopen
[00011064] 548f                      addq.l    #2,a7
[00011066] 23c0 0001 50ee            move.l    d0,_tempfd
[0001106c] 4a9f                      tst.l     (a7)+
[0001106e] 4cdf 00e0                 movem.l   (a7)+,d5-d7
[00011072] 4e5e                      unlk      a6
[00011074] 4e75                      rts
_fnonly:
[00011076] 4e56 0000                 link      a6,#$0000
[0001107a] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0001107e] 286e 0008                 movea.l   8(a6),a4
[00011082] 2e8c                      move.l    a4,(a7)
[00011084] 2f3c 0001 5112            move.l    #$00015112,-(a7)
[0001108a] 4eb9 0001 39cc            jsr       _strcpy
[00011090] 588f                      addq.l    #4,a7
[00011092] 9bcd                      suba.l    a5,a5
[00011094] 600a                      bra.s     $000110A0
[00011096] 0c14 003a                 cmpi.b    #$3A,(a4)
[0001109a] 6602                      bne.s     $0001109E
[0001109c] 2a4c                      movea.l   a4,a5
[0001109e] 528c                      addq.l    #1,a4
[000110a0] 4a14                      tst.b     (a4)
[000110a2] 66f2                      bne.s     $00011096
[000110a4] 200d                      move.l    a5,d0
[000110a6] 670e                      beq.s     $000110B6
[000110a8] 528d                      addq.l    #1,a5
[000110aa] 4a15                      tst.b     (a5)
[000110ac] 6606                      bne.s     $000110B4
[000110ae] 2a7c 0001 5112            movea.l   #$00015112,a5
[000110b4] 6006                      bra.s     $000110BC
[000110b6] 2a7c 0001 5112            movea.l   #$00015112,a5
[000110bc] 200d                      move.l    a5,d0
[000110be] 4a9f                      tst.l     (a7)+
[000110c0] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000110c4] 4e5e                      unlk      a6
[000110c6] 4e75                      rts
_copy:
[000110c8] 4e56 0000                 link      a6,#$0000
[000110cc] 48e7 0f00                 movem.l   d4-d7,-(a7)
[000110d0] 3ebc 0001                 move.w    #$0001,(a7)
[000110d4] 2f2e 000c                 move.l    12(a6),-(a7)
[000110d8] 4eb9 0001 2890            jsr       _open
[000110de] 588f                      addq.l    #4,a7
[000110e0] 3c00                      move.w    d0,d6
[000110e2] bc7c ffff                 cmp.w     #$FFFF,d6
[000110e6] 661c                      bne.s     $00011104
[000110e8] 3ebc 01b6                 move.w    #$01B6,(a7)
[000110ec] 2f2e 000c                 move.l    12(a6),-(a7)
[000110f0] 4eb9 0001 2580            jsr       _creat
[000110f6] 588f                      addq.l    #4,a7
[000110f8] 3c00                      move.w    d0,d6
[000110fa] bc7c ffff                 cmp.w     #$FFFF,d6
[000110fe] 6604                      bne.s     $00011104
[00011100] 70ff                      moveq.l   #-1,d0
[00011102] 6068                      bra.s     $0001116C
[00011104] 4257                      clr.w     (a7)
[00011106] 2f2e 0008                 move.l    8(a6),-(a7)
[0001110a] 4eb9 0001 2890            jsr       _open
[00011110] 588f                      addq.l    #4,a7
[00011112] 3e00                      move.w    d0,d7
[00011114] be7c ffff                 cmp.w     #$FFFF,d7
[00011118] 6616                      bne.s     $00011130
[0001111a] 3e86                      move.w    d6,(a7)
[0001111c] 4eb9 0001 266c            jsr       _close
[00011122] 2eae 000c                 move.l    12(a6),(a7)
[00011126] 4eb9 0001 20f4            jsr       _unlink
[0001112c] 70ff                      moveq.l   #-1,d0
[0001112e] 603c                      bra.s     $0001116C
[00011130] 6012                      bra.s     $00011144
[00011132] 3e85                      move.w    d5,(a7)
[00011134] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[0001113a] 3f06                      move.w    d6,-(a7)
[0001113c] 4eb9 0001 3614            jsr       _write
[00011142] 5c8f                      addq.l    #6,a7
[00011144] 3ebc 0200                 move.w    #$0200,(a7)
[00011148] 2f3c 0001 4ed6            move.l    #$00014ED6,-(a7)
[0001114e] 3f07                      move.w    d7,-(a7)
[00011150] 4eb9 0001 1edc            jsr       _read
[00011156] 5c8f                      addq.l    #6,a7
[00011158] 3a00                      move.w    d0,d5
[0001115a] 6ed6                      bgt.s     $00011132
[0001115c] 3e87                      move.w    d7,(a7)
[0001115e] 4eb9 0001 266c            jsr       _close
[00011164] 3e86                      move.w    d6,(a7)
[00011166] 4eb9 0001 266c            jsr       _close
[0001116c] 4a9f                      tst.l     (a7)+
[0001116e] 4cdf 00e0                 movem.l   (a7)+,d5-d7
[00011172] 4e5e                      unlk      a6
[00011174] 4e75                      rts
_lgetl:
[00011176] 4e56 0000                 link      a6,#$0000
[0001117a] 48e7 0300                 movem.l   d6-d7,-(a7)
[0001117e] 2f2e 0008                 move.l    8(a6),-(a7)
[00011182] 2f2e 000c                 move.l    12(a6),-(a7)
[00011186] 4eb9 0001 1dae            jsr       _fgetc
[0001118c] 588f                      addq.l    #4,a7
[0001118e] c07c 00ff                 and.w     #$00FF,d0
[00011192] 205f                      movea.l   (a7)+,a0
[00011194] 1080                      move.b    d0,(a0)
[00011196] 2f2e 0008                 move.l    8(a6),-(a7)
[0001119a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001119e] 4eb9 0001 1dae            jsr       _fgetc
[000111a4] 588f                      addq.l    #4,a7
[000111a6] c07c 00ff                 and.w     #$00FF,d0
[000111aa] 205f                      movea.l   (a7)+,a0
[000111ac] 1140 0001                 move.b    d0,1(a0)
[000111b0] 2f2e 0008                 move.l    8(a6),-(a7)
[000111b4] 2f2e 000c                 move.l    12(a6),-(a7)
[000111b8] 4eb9 0001 1dae            jsr       _fgetc
[000111be] 588f                      addq.l    #4,a7
[000111c0] c07c 00ff                 and.w     #$00FF,d0
[000111c4] 205f                      movea.l   (a7)+,a0
[000111c6] 1140 0002                 move.b    d0,2(a0)
[000111ca] 2f2e 0008                 move.l    8(a6),-(a7)
[000111ce] 2f2e 000c                 move.l    12(a6),-(a7)
[000111d2] 4eb9 0001 1dae            jsr       _fgetc
[000111d8] 588f                      addq.l    #4,a7
[000111da] c07c 00ff                 and.w     #$00FF,d0
[000111de] 205f                      movea.l   (a7)+,a0
[000111e0] 1140 0003                 move.b    d0,3(a0)
[000111e4] 206e 000c                 movea.l   12(a6),a0
[000111e8] 0828 0004 0003            btst      #4,3(a0)
[000111ee] 660c                      bne.s     $000111FC
[000111f0] 206e 000c                 movea.l   12(a6),a0
[000111f4] 0828 0005 0003            btst      #5,3(a0)
[000111fa] 6704                      beq.s     $00011200
[000111fc] 70ff                      moveq.l   #-1,d0
[000111fe] 6002                      bra.s     $00011202
[00011200] 4240                      clr.w     d0
[00011202] 4a9f                      tst.l     (a7)+
[00011204] 4cdf 0080                 movem.l   (a7)+,d7
[00011208] 4e5e                      unlk      a6
[0001120a] 4e75                      rts
_lgetw:
[0001120c] 4e56 0000                 link      a6,#$0000
[00011210] 48e7 0300                 movem.l   d6-d7,-(a7)
[00011214] 2f2e 0008                 move.l    8(a6),-(a7)
[00011218] 2f2e 000c                 move.l    12(a6),-(a7)
[0001121c] 4eb9 0001 1dae            jsr       _fgetc
[00011222] 588f                      addq.l    #4,a7
[00011224] c07c 00ff                 and.w     #$00FF,d0
[00011228] 205f                      movea.l   (a7)+,a0
[0001122a] 1080                      move.b    d0,(a0)
[0001122c] 2f2e 0008                 move.l    8(a6),-(a7)
[00011230] 2f2e 000c                 move.l    12(a6),-(a7)
[00011234] 4eb9 0001 1dae            jsr       _fgetc
[0001123a] 588f                      addq.l    #4,a7
[0001123c] c07c 00ff                 and.w     #$00FF,d0
[00011240] 205f                      movea.l   (a7)+,a0
[00011242] 1140 0001                 move.b    d0,1(a0)
[00011246] 206e 0008                 movea.l   8(a6),a0
[0001124a] 0250 ffff                 andi.w    #$FFFF,(a0)
[0001124e] 206e 000c                 movea.l   12(a6),a0
[00011252] 0828 0004 0003            btst      #4,3(a0)
[00011258] 660c                      bne.s     $00011266
[0001125a] 206e 000c                 movea.l   12(a6),a0
[0001125e] 0828 0005 0003            btst      #5,3(a0)
[00011264] 6704                      beq.s     $0001126A
[00011266] 70ff                      moveq.l   #-1,d0
[00011268] 6002                      bra.s     $0001126C
[0001126a] 4240                      clr.w     d0
[0001126c] 4a9f                      tst.l     (a7)+
[0001126e] 4cdf 0080                 movem.l   (a7)+,d7
[00011272] 4e5e                      unlk      a6
[00011274] 4e75                      rts
_getchd:
[00011276] 4e56 fffc                 link      a6,#$FFFC
[0001127a] 2eae 0008                 move.l    8(a6),(a7)
[0001127e] 2f2e 000c                 move.l    12(a6),-(a7)
[00011282] 4eb9 0001 120c            jsr       _lgetw
[00011288] 588f                      addq.l    #4,a7
[0001128a] b07c ffff                 cmp.w     #$FFFF,d0
[0001128e] 6606                      bne.s     $00011296
[00011290] 70ff                      moveq.l   #-1,d0
[00011292] 6000 0132                 bra       $000113C6
[00011296] 2eae 0008                 move.l    8(a6),(a7)
[0001129a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001129e] 5497                      addq.l    #2,(a7)
[000112a0] 4eb9 0001 1176            jsr       _lgetl
[000112a6] 588f                      addq.l    #4,a7
[000112a8] b07c ffff                 cmp.w     #$FFFF,d0
[000112ac] 6606                      bne.s     $000112B4
[000112ae] 70ff                      moveq.l   #-1,d0
[000112b0] 6000 0114                 bra       $000113C6
[000112b4] 2eae 0008                 move.l    8(a6),(a7)
[000112b8] 2f2e 000c                 move.l    12(a6),-(a7)
[000112bc] 5c97                      addq.l    #6,(a7)
[000112be] 4eb9 0001 1176            jsr       _lgetl
[000112c4] 588f                      addq.l    #4,a7
[000112c6] b07c ffff                 cmp.w     #$FFFF,d0
[000112ca] 6606                      bne.s     $000112D2
[000112cc] 70ff                      moveq.l   #-1,d0
[000112ce] 6000 00f6                 bra       $000113C6
[000112d2] 2eae 0008                 move.l    8(a6),(a7)
[000112d6] 2f2e 000c                 move.l    12(a6),-(a7)
[000112da] 0697 0000 000a            addi.l    #$0000000A,(a7)
[000112e0] 4eb9 0001 1176            jsr       _lgetl
[000112e6] 588f                      addq.l    #4,a7
[000112e8] b07c ffff                 cmp.w     #$FFFF,d0
[000112ec] 6606                      bne.s     $000112F4
[000112ee] 70ff                      moveq.l   #-1,d0
[000112f0] 6000 00d4                 bra       $000113C6
[000112f4] 2eae 0008                 move.l    8(a6),(a7)
[000112f8] 2f2e 000c                 move.l    12(a6),-(a7)
[000112fc] 0697 0000 000e            addi.l    #$0000000E,(a7)
[00011302] 4eb9 0001 1176            jsr       _lgetl
[00011308] 588f                      addq.l    #4,a7
[0001130a] b07c ffff                 cmp.w     #$FFFF,d0
[0001130e] 6606                      bne.s     $00011316
[00011310] 70ff                      moveq.l   #-1,d0
[00011312] 6000 00b2                 bra       $000113C6
[00011316] 2eae 0008                 move.l    8(a6),(a7)
[0001131a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001131e] 0697 0000 0012            addi.l    #$00000012,(a7)
[00011324] 4eb9 0001 1176            jsr       _lgetl
[0001132a] 588f                      addq.l    #4,a7
[0001132c] b07c ffff                 cmp.w     #$FFFF,d0
[00011330] 6606                      bne.s     $00011338
[00011332] 70ff                      moveq.l   #-1,d0
[00011334] 6000 0090                 bra       $000113C6
[00011338] 2eae 0008                 move.l    8(a6),(a7)
[0001133c] 2f2e 000c                 move.l    12(a6),-(a7)
[00011340] 0697 0000 0016            addi.l    #$00000016,(a7)
[00011346] 4eb9 0001 1176            jsr       _lgetl
[0001134c] 588f                      addq.l    #4,a7
[0001134e] b07c ffff                 cmp.w     #$FFFF,d0
[00011352] 6606                      bne.s     $0001135A
[00011354] 70ff                      moveq.l   #-1,d0
[00011356] 6000 006e                 bra       $000113C6
[0001135a] 2eae 0008                 move.l    8(a6),(a7)
[0001135e] 2f2e 000c                 move.l    12(a6),-(a7)
[00011362] 0697 0000 001a            addi.l    #$0000001A,(a7)
[00011368] 4eb9 0001 120c            jsr       _lgetw
[0001136e] 588f                      addq.l    #4,a7
[00011370] b07c ffff                 cmp.w     #$FFFF,d0
[00011374] 6604                      bne.s     $0001137A
[00011376] 70ff                      moveq.l   #-1,d0
[00011378] 604c                      bra.s     $000113C6
[0001137a] 206e 000c                 movea.l   12(a6),a0
[0001137e] 0c50 601b                 cmpi.w    #$601B,(a0)
[00011382] 6640                      bne.s     $000113C4
[00011384] 2eae 0008                 move.l    8(a6),(a7)
[00011388] 2f2e 000c                 move.l    12(a6),-(a7)
[0001138c] 0697 0000 001c            addi.l    #$0000001C,(a7)
[00011392] 4eb9 0001 1176            jsr       _lgetl
[00011398] 588f                      addq.l    #4,a7
[0001139a] b07c ffff                 cmp.w     #$FFFF,d0
[0001139e] 6604                      bne.s     $000113A4
[000113a0] 70ff                      moveq.l   #-1,d0
[000113a2] 6022                      bra.s     $000113C6
[000113a4] 2eae 0008                 move.l    8(a6),(a7)
[000113a8] 2f2e 000c                 move.l    12(a6),-(a7)
[000113ac] 0697 0000 0020            addi.l    #$00000020,(a7)
[000113b2] 4eb9 0001 1176            jsr       _lgetl
[000113b8] 588f                      addq.l    #4,a7
[000113ba] b07c ffff                 cmp.w     #$FFFF,d0
[000113be] 6604                      bne.s     $000113C4
[000113c0] 70ff                      moveq.l   #-1,d0
[000113c2] 6002                      bra.s     $000113C6
[000113c4] 4240                      clr.w     d0
[000113c6] 4e5e                      unlk      a6
[000113c8] 4e75                      rts
_putarhd:
[000113ca] 4e56 fffe                 link      a6,#$FFFE
[000113ce] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000113d2] 426e fffe                 clr.w     -2(a6)
[000113d6] 4247                      clr.w     d7
[000113d8] 286e 000c                 movea.l   12(a6),a4
[000113dc] 6022                      bra.s     $00011400
[000113de] 2eae 0008                 move.l    8(a6),(a7)
[000113e2] 1014                      move.b    (a4),d0
[000113e4] 4880                      ext.w     d0
[000113e6] 3f00                      move.w    d0,-(a7)
[000113e8] 4eb9 0001 2ee8            jsr       _fputc
[000113ee] 548f                      addq.l    #2,a7
[000113f0] b07c ffff                 cmp.w     #$FFFF,d0
[000113f4] 6606                      bne.s     $000113FC
[000113f6] 70ff                      moveq.l   #-1,d0
[000113f8] 6000 00d0                 bra       $000114CA
[000113fc] 5247                      addq.w    #1,d7
[000113fe] 528c                      addq.l    #1,a4
[00011400] be7c 000e                 cmp.w     #$000E,d7
[00011404] 6dd8                      blt.s     $000113DE
[00011406] 2eae 0008                 move.l    8(a6),(a7)
[0001140a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001140e] 0697 0000 000e            addi.l    #$0000000E,(a7)
[00011414] 4eb9 0001 14d4            jsr       _lputl
[0001141a] 588f                      addq.l    #4,a7
[0001141c] b07c ffff                 cmp.w     #$FFFF,d0
[00011420] 6606                      bne.s     $00011428
[00011422] 70ff                      moveq.l   #-1,d0
[00011424] 6000 00a4                 bra       $000114CA
[00011428] 2eae 0008                 move.l    8(a6),(a7)
[0001142c] 206e 000c                 movea.l   12(a6),a0
[00011430] 1028 0012                 move.b    18(a0),d0
[00011434] 4880                      ext.w     d0
[00011436] 3f00                      move.w    d0,-(a7)
[00011438] 4eb9 0001 2ee8            jsr       _fputc
[0001143e] 548f                      addq.l    #2,a7
[00011440] b07c ffff                 cmp.w     #$FFFF,d0
[00011444] 6606                      bne.s     $0001144C
[00011446] 70ff                      moveq.l   #-1,d0
[00011448] 6000 0080                 bra       $000114CA
[0001144c] 2eae 0008                 move.l    8(a6),(a7)
[00011450] 206e 000c                 movea.l   12(a6),a0
[00011454] 1028 0013                 move.b    19(a0),d0
[00011458] 4880                      ext.w     d0
[0001145a] 3f00                      move.w    d0,-(a7)
[0001145c] 4eb9 0001 2ee8            jsr       _fputc
[00011462] 548f                      addq.l    #2,a7
[00011464] b07c ffff                 cmp.w     #$FFFF,d0
[00011468] 6604                      bne.s     $0001146E
[0001146a] 70ff                      moveq.l   #-1,d0
[0001146c] 605c                      bra.s     $000114CA
[0001146e] 2eae 0008                 move.l    8(a6),(a7)
[00011472] 2f2e 000c                 move.l    12(a6),-(a7)
[00011476] 0697 0000 0014            addi.l    #$00000014,(a7)
[0001147c] 4eb9 0001 154c            jsr       _lputw
[00011482] 588f                      addq.l    #4,a7
[00011484] b07c ffff                 cmp.w     #$FFFF,d0
[00011488] 6604                      bne.s     $0001148E
[0001148a] 70ff                      moveq.l   #-1,d0
[0001148c] 603c                      bra.s     $000114CA
[0001148e] 2eae 0008                 move.l    8(a6),(a7)
[00011492] 2f2e 000c                 move.l    12(a6),-(a7)
[00011496] 0697 0000 0016            addi.l    #$00000016,(a7)
[0001149c] 4eb9 0001 14d4            jsr       _lputl
[000114a2] 588f                      addq.l    #4,a7
[000114a4] b07c ffff                 cmp.w     #$FFFF,d0
[000114a8] 6604                      bne.s     $000114AE
[000114aa] 70ff                      moveq.l   #-1,d0
[000114ac] 601c                      bra.s     $000114CA
[000114ae] 2eae 0008                 move.l    8(a6),(a7)
[000114b2] 2f0e                      move.l    a6,-(a7)
[000114b4] 5597                      subq.l    #2,(a7)
[000114b6] 4eb9 0001 154c            jsr       _lputw
[000114bc] 588f                      addq.l    #4,a7
[000114be] b07c ffff                 cmp.w     #$FFFF,d0
[000114c2] 6604                      bne.s     $000114C8
[000114c4] 70ff                      moveq.l   #-1,d0
[000114c6] 6002                      bra.s     $000114CA
[000114c8] 4240                      clr.w     d0
[000114ca] 4a9f                      tst.l     (a7)+
[000114cc] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000114d0] 4e5e                      unlk      a6
[000114d2] 4e75                      rts
_lputl:
[000114d4] 4e56 fffc                 link      a6,#$FFFC
[000114d8] 2eae 000c                 move.l    12(a6),(a7)
[000114dc] 206e 0008                 movea.l   8(a6),a0
[000114e0] 1010                      move.b    (a0),d0
[000114e2] 4880                      ext.w     d0
[000114e4] 3f00                      move.w    d0,-(a7)
[000114e6] 4eb9 0001 2ee8            jsr       _fputc
[000114ec] 548f                      addq.l    #2,a7
[000114ee] 2eae 000c                 move.l    12(a6),(a7)
[000114f2] 206e 0008                 movea.l   8(a6),a0
[000114f6] 1028 0001                 move.b    1(a0),d0
[000114fa] 4880                      ext.w     d0
[000114fc] 3f00                      move.w    d0,-(a7)
[000114fe] 4eb9 0001 2ee8            jsr       _fputc
[00011504] 548f                      addq.l    #2,a7
[00011506] 2eae 000c                 move.l    12(a6),(a7)
[0001150a] 206e 0008                 movea.l   8(a6),a0
[0001150e] 1028 0002                 move.b    2(a0),d0
[00011512] 4880                      ext.w     d0
[00011514] 3f00                      move.w    d0,-(a7)
[00011516] 4eb9 0001 2ee8            jsr       _fputc
[0001151c] 548f                      addq.l    #2,a7
[0001151e] 2eae 000c                 move.l    12(a6),(a7)
[00011522] 206e 0008                 movea.l   8(a6),a0
[00011526] 1028 0003                 move.b    3(a0),d0
[0001152a] 4880                      ext.w     d0
[0001152c] 3f00                      move.w    d0,-(a7)
[0001152e] 4eb9 0001 2ee8            jsr       _fputc
[00011534] 548f                      addq.l    #2,a7
[00011536] 206e 000c                 movea.l   12(a6),a0
[0001153a] 0828 0004 0003            btst      #4,3(a0)
[00011540] 6704                      beq.s     $00011546
[00011542] 70ff                      moveq.l   #-1,d0
[00011544] 6002                      bra.s     $00011548
[00011546] 4240                      clr.w     d0
[00011548] 4e5e                      unlk      a6
[0001154a] 4e75                      rts
_lputw:
[0001154c] 4e56 fffc                 link      a6,#$FFFC
[00011550] 2eae 000c                 move.l    12(a6),(a7)
[00011554] 206e 0008                 movea.l   8(a6),a0
[00011558] 1010                      move.b    (a0),d0
[0001155a] 4880                      ext.w     d0
[0001155c] 3f00                      move.w    d0,-(a7)
[0001155e] 4eb9 0001 2ee8            jsr       _fputc
[00011564] 548f                      addq.l    #2,a7
[00011566] 2eae 000c                 move.l    12(a6),(a7)
[0001156a] 206e 0008                 movea.l   8(a6),a0
[0001156e] 1028 0001                 move.b    1(a0),d0
[00011572] 4880                      ext.w     d0
[00011574] 3f00                      move.w    d0,-(a7)
[00011576] 4eb9 0001 2ee8            jsr       _fputc
[0001157c] 548f                      addq.l    #2,a7
[0001157e] 206e 000c                 movea.l   12(a6),a0
[00011582] 0828 0004 0003            btst      #4,3(a0)
[00011588] 6704                      beq.s     $0001158E
[0001158a] 70ff                      moveq.l   #-1,d0
[0001158c] 6002                      bra.s     $00011590
[0001158e] 4240                      clr.w     d0
[00011590] 4e5e                      unlk      a6
[00011592] 4e75                      rts
_putchd:
[00011594] 4e56 fffc                 link      a6,#$FFFC
[00011598] 2eae 0008                 move.l    8(a6),(a7)
[0001159c] 2f2e 000c                 move.l    12(a6),-(a7)
[000115a0] 4eb9 0001 154c            jsr       _lputw
[000115a6] 588f                      addq.l    #4,a7
[000115a8] b07c ffff                 cmp.w     #$FFFF,d0
[000115ac] 6606                      bne.s     $000115B4
[000115ae] 70ff                      moveq.l   #-1,d0
[000115b0] 6000 0132                 bra       $000116E4
[000115b4] 2eae 0008                 move.l    8(a6),(a7)
[000115b8] 2f2e 000c                 move.l    12(a6),-(a7)
[000115bc] 5497                      addq.l    #2,(a7)
[000115be] 4eb9 0001 14d4            jsr       _lputl
[000115c4] 588f                      addq.l    #4,a7
[000115c6] b07c ffff                 cmp.w     #$FFFF,d0
[000115ca] 6606                      bne.s     $000115D2
[000115cc] 70ff                      moveq.l   #-1,d0
[000115ce] 6000 0114                 bra       $000116E4
[000115d2] 2eae 0008                 move.l    8(a6),(a7)
[000115d6] 2f2e 000c                 move.l    12(a6),-(a7)
[000115da] 5c97                      addq.l    #6,(a7)
[000115dc] 4eb9 0001 14d4            jsr       _lputl
[000115e2] 588f                      addq.l    #4,a7
[000115e4] b07c ffff                 cmp.w     #$FFFF,d0
[000115e8] 6606                      bne.s     $000115F0
[000115ea] 70ff                      moveq.l   #-1,d0
[000115ec] 6000 00f6                 bra       $000116E4
[000115f0] 2eae 0008                 move.l    8(a6),(a7)
[000115f4] 2f2e 000c                 move.l    12(a6),-(a7)
[000115f8] 0697 0000 000a            addi.l    #$0000000A,(a7)
[000115fe] 4eb9 0001 14d4            jsr       _lputl
[00011604] 588f                      addq.l    #4,a7
[00011606] b07c ffff                 cmp.w     #$FFFF,d0
[0001160a] 6606                      bne.s     $00011612
[0001160c] 70ff                      moveq.l   #-1,d0
[0001160e] 6000 00d4                 bra       $000116E4
[00011612] 2eae 0008                 move.l    8(a6),(a7)
[00011616] 2f2e 000c                 move.l    12(a6),-(a7)
[0001161a] 0697 0000 000e            addi.l    #$0000000E,(a7)
[00011620] 4eb9 0001 14d4            jsr       _lputl
[00011626] 588f                      addq.l    #4,a7
[00011628] b07c ffff                 cmp.w     #$FFFF,d0
[0001162c] 6606                      bne.s     $00011634
[0001162e] 70ff                      moveq.l   #-1,d0
[00011630] 6000 00b2                 bra       $000116E4
[00011634] 2eae 0008                 move.l    8(a6),(a7)
[00011638] 2f2e 000c                 move.l    12(a6),-(a7)
[0001163c] 0697 0000 0012            addi.l    #$00000012,(a7)
[00011642] 4eb9 0001 14d4            jsr       _lputl
[00011648] 588f                      addq.l    #4,a7
[0001164a] b07c ffff                 cmp.w     #$FFFF,d0
[0001164e] 6606                      bne.s     $00011656
[00011650] 70ff                      moveq.l   #-1,d0
[00011652] 6000 0090                 bra       $000116E4
[00011656] 2eae 0008                 move.l    8(a6),(a7)
[0001165a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001165e] 0697 0000 0016            addi.l    #$00000016,(a7)
[00011664] 4eb9 0001 14d4            jsr       _lputl
[0001166a] 588f                      addq.l    #4,a7
[0001166c] b07c ffff                 cmp.w     #$FFFF,d0
[00011670] 6606                      bne.s     $00011678
[00011672] 70ff                      moveq.l   #-1,d0
[00011674] 6000 006e                 bra       $000116E4
[00011678] 2eae 0008                 move.l    8(a6),(a7)
[0001167c] 2f2e 000c                 move.l    12(a6),-(a7)
[00011680] 0697 0000 001a            addi.l    #$0000001A,(a7)
[00011686] 4eb9 0001 154c            jsr       _lputw
[0001168c] 588f                      addq.l    #4,a7
[0001168e] b07c ffff                 cmp.w     #$FFFF,d0
[00011692] 6604                      bne.s     $00011698
[00011694] 70ff                      moveq.l   #-1,d0
[00011696] 604c                      bra.s     $000116E4
[00011698] 206e 000c                 movea.l   12(a6),a0
[0001169c] 0c50 601b                 cmpi.w    #$601B,(a0)
[000116a0] 6640                      bne.s     $000116E2
[000116a2] 2eae 0008                 move.l    8(a6),(a7)
[000116a6] 2f2e 000c                 move.l    12(a6),-(a7)
[000116aa] 0697 0000 001c            addi.l    #$0000001C,(a7)
[000116b0] 4eb9 0001 14d4            jsr       _lputl
[000116b6] 588f                      addq.l    #4,a7
[000116b8] b07c ffff                 cmp.w     #$FFFF,d0
[000116bc] 6604                      bne.s     $000116C2
[000116be] 70ff                      moveq.l   #-1,d0
[000116c0] 6022                      bra.s     $000116E4
[000116c2] 2eae 0008                 move.l    8(a6),(a7)
[000116c6] 2f2e 000c                 move.l    12(a6),-(a7)
[000116ca] 0697 0000 0020            addi.l    #$00000020,(a7)
[000116d0] 4eb9 0001 14d4            jsr       _lputl
[000116d6] 588f                      addq.l    #4,a7
[000116d8] b07c ffff                 cmp.w     #$FFFF,d0
[000116dc] 6604                      bne.s     $000116E2
[000116de] 70ff                      moveq.l   #-1,d0
[000116e0] 6002                      bra.s     $000116E4
[000116e2] 4240                      clr.w     d0
[000116e4] 4e5e                      unlk      a6
[000116e6] 4e75                      rts
_getarhd:
[000116e8] 4e56 fffe                 link      a6,#$FFFE
[000116ec] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000116f0] 4247                      clr.w     d7
[000116f2] 286e 000c                 movea.l   12(a6),a4
[000116f6] 601a                      bra.s     $00011712
[000116f8] 2eae 0008                 move.l    8(a6),(a7)
[000116fc] 4eb9 0001 1dae            jsr       _fgetc
[00011702] 18c0                      move.b    d0,(a4)+
[00011704] b07c ffff                 cmp.w     #$FFFF,d0
[00011708] 6606                      bne.s     $00011710
[0001170a] 70ff                      moveq.l   #-1,d0
[0001170c] 6000 00ca                 bra       $000117D8
[00011710] 5247                      addq.w    #1,d7
[00011712] be7c 000e                 cmp.w     #$000E,d7
[00011716] 6de0                      blt.s     $000116F8
[00011718] 2eae 0008                 move.l    8(a6),(a7)
[0001171c] 2f2e 000c                 move.l    12(a6),-(a7)
[00011720] 0697 0000 000e            addi.l    #$0000000E,(a7)
[00011726] 4eb9 0001 1176            jsr       _lgetl
[0001172c] 588f                      addq.l    #4,a7
[0001172e] b07c ffff                 cmp.w     #$FFFF,d0
[00011732] 6606                      bne.s     $0001173A
[00011734] 70ff                      moveq.l   #-1,d0
[00011736] 6000 00a0                 bra       $000117D8
[0001173a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001173e] 2f2e 0008                 move.l    8(a6),-(a7)
[00011742] 4eb9 0001 1dae            jsr       _fgetc
[00011748] 588f                      addq.l    #4,a7
[0001174a] 205f                      movea.l   (a7)+,a0
[0001174c] 1140 0012                 move.b    d0,18(a0)
[00011750] b07c ffff                 cmp.w     #$FFFF,d0
[00011754] 6606                      bne.s     $0001175C
[00011756] 70ff                      moveq.l   #-1,d0
[00011758] 6000 007e                 bra       $000117D8
[0001175c] 2f2e 000c                 move.l    12(a6),-(a7)
[00011760] 2f2e 0008                 move.l    8(a6),-(a7)
[00011764] 4eb9 0001 1dae            jsr       _fgetc
[0001176a] 588f                      addq.l    #4,a7
[0001176c] 205f                      movea.l   (a7)+,a0
[0001176e] 1140 0013                 move.b    d0,19(a0)
[00011772] b07c ffff                 cmp.w     #$FFFF,d0
[00011776] 6604                      bne.s     $0001177C
[00011778] 70ff                      moveq.l   #-1,d0
[0001177a] 605c                      bra.s     $000117D8
[0001177c] 2eae 0008                 move.l    8(a6),(a7)
[00011780] 2f2e 000c                 move.l    12(a6),-(a7)
[00011784] 0697 0000 0014            addi.l    #$00000014,(a7)
[0001178a] 4eb9 0001 120c            jsr       _lgetw
[00011790] 588f                      addq.l    #4,a7
[00011792] b07c ffff                 cmp.w     #$FFFF,d0
[00011796] 6604                      bne.s     $0001179C
[00011798] 70ff                      moveq.l   #-1,d0
[0001179a] 603c                      bra.s     $000117D8
[0001179c] 2eae 0008                 move.l    8(a6),(a7)
[000117a0] 2f2e 000c                 move.l    12(a6),-(a7)
[000117a4] 0697 0000 0016            addi.l    #$00000016,(a7)
[000117aa] 4eb9 0001 1176            jsr       _lgetl
[000117b0] 588f                      addq.l    #4,a7
[000117b2] b07c ffff                 cmp.w     #$FFFF,d0
[000117b6] 6604                      bne.s     $000117BC
[000117b8] 70ff                      moveq.l   #-1,d0
[000117ba] 601c                      bra.s     $000117D8
[000117bc] 2eae 0008                 move.l    8(a6),(a7)
[000117c0] 2f0e                      move.l    a6,-(a7)
[000117c2] 5597                      subq.l    #2,(a7)
[000117c4] 4eb9 0001 120c            jsr       _lgetw
[000117ca] 588f                      addq.l    #4,a7
[000117cc] b07c ffff                 cmp.w     #$FFFF,d0
[000117d0] 6604                      bne.s     $000117D6
[000117d2] 70ff                      moveq.l   #-1,d0
[000117d4] 6002                      bra.s     $000117D8
[000117d6] 4240                      clr.w     d0
[000117d8] 4a9f                      tst.l     (a7)+
[000117da] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000117de] 4e5e                      unlk      a6
[000117e0] 4e75                      rts
_efseek:
[000117e2] 4e56 fffc                 link      a6,#$FFFC
[000117e6] 206e 0008                 movea.l   8(a6),a0
[000117ea] 0828 0001 0003            btst      #1,3(a0)
[000117f0] 670a                      beq.s     $000117FC
[000117f2] 2eae 0008                 move.l    8(a6),(a7)
[000117f6] 4eb9 0001 26fc            jsr       _fflush
[000117fc] 082e 0000 0011            btst      #0,17(a6)
[00011802] 670e                      beq.s     $00011812
[00011804] 206e 0008                 movea.l   8(a6),a0
[00011808] 3028 000c                 move.w    12(a0),d0
[0001180c] 48c0                      ext.l     d0
[0001180e] 91ae 000c                 sub.l     d0,12(a6)
[00011812] 206e 0008                 movea.l   8(a6),a0
[00011816] 4268 000c                 clr.w     12(a0)
[0001181a] 206e 0008                 movea.l   8(a6),a0
[0001181e] 226e 0008                 movea.l   8(a6),a1
[00011822] 2169 0004 0008            move.l    4(a1),8(a0)
[00011828] 3eae 0010                 move.w    16(a6),(a7)
[0001182c] 2f2e 000c                 move.l    12(a6),-(a7)
[00011830] 206e 0008                 movea.l   8(a6),a0
[00011834] 3f10                      move.w    (a0),-(a7)
[00011836] 4eb9 0001 1842            jsr       _lseek
[0001183c] 5c8f                      addq.l    #6,a7
[0001183e] 4e5e                      unlk      a6
[00011840] 4e75                      rts
_lseek:
[00011842] 4e56 0000                 link      a6,#$0000
[00011846] 48e7 0104                 movem.l   d7/a5,-(a7)
[0001184a] 3eae 0008                 move.w    8(a6),(a7)
[0001184e] 4eb9 0001 19b2            jsr       __chkc
[00011854] 2a40                      movea.l   d0,a5
[00011856] 200d                      move.l    a5,d0
[00011858] 6616                      bne.s     $00011870
[0001185a] 33fc 0009 0001 4db0       move.w    #$0009,$00014DB0
[00011862] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[0001186c] 70ff                      moveq.l   #-1,d0
[0001186e] 6024                      bra.s     $00011894
[00011870] 3eae 000e                 move.w    14(a6),(a7)
[00011874] 3f2d 0004                 move.w    4(a5),-(a7)
[00011878] 2f2e 000a                 move.l    10(a6),-(a7)
[0001187c] 3f3c 0042                 move.w    #$0042,-(a7)
[00011880] 4eb9 0001 3d54            jsr       _trap
[00011886] 508f                      addq.l    #8,a7
[00011888] 2b40 0006                 move.l    d0,6(a5)
[0001188c] 0255 ffdf                 andi.w    #$FFDF,(a5)
[00011890] 202d 0006                 move.l    6(a5),d0
[00011894] 4a9f                      tst.l     (a7)+
[00011896] 4cdf 2000                 movem.l   (a7)+,a5
[0001189a] 4e5e                      unlk      a6
[0001189c] 4e75                      rts
_tell:
[0001189e] 4e56 fffc                 link      a6,#$FFFC
[000118a2] 3ebc 0001                 move.w    #$0001,(a7)
[000118a6] 42a7                      clr.l     -(a7)
[000118a8] 3f2e 0008                 move.w    8(a6),-(a7)
[000118ac] 6194                      bsr.s     _lseek
[000118ae] 5c8f                      addq.l    #6,a7
[000118b0] 4e5e                      unlk      a6
[000118b2] 4e75                      rts
[000118b4] 4e56 fffc                 link      a6,#$FFFC
[000118b8] 4e5e                      unlk      a6
[000118ba] 4e75                      rts
__allocc:
[000118bc] 4e56 0000                 link      a6,#$0000
[000118c0] 48e7 0700                 movem.l   d5-d7,-(a7)
[000118c4] 7c01                      moveq.l   #1,d6
[000118c6] 4247                      clr.w     d7
[000118c8] 6018                      bra.s     $000118E2
[000118ca] 2006                      move.l    d6,d0
[000118cc] c0b9 0001 4b6a            and.l     $00014B6A,d0
[000118d2] 660a                      bne.s     $000118DE
[000118d4] 8db9 0001 4b6a            or.l      d6,$00014B6A
[000118da] 3007                      move.w    d7,d0
[000118dc] 601e                      bra.s     $000118FC
[000118de] e386                      asl.l     #1,d6
[000118e0] 5247                      addq.w    #1,d7
[000118e2] be7c 0005                 cmp.w     #$0005,d7
[000118e6] 6de2                      blt.s     $000118CA
[000118e8] 33fc 0018 0001 4db0       move.w    #$0018,$00014DB0
[000118f0] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[000118fa] 70ff                      moveq.l   #-1,d0
[000118fc] 4a9f                      tst.l     (a7)+
[000118fe] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[00011902] 4e5e                      unlk      a6
[00011904] 4e75                      rts
__freec:
[00011906] 4e56 fffc                 link      a6,#$FFFC
[0001190a] 7001                      moveq.l   #1,d0
[0001190c] 322e 0008                 move.w    8(a6),d1
[00011910] e360                      asl.w     d1,d0
[00011912] 4640                      not.w     d0
[00011914] 48c0                      ext.l     d0
[00011916] c1b9 0001 4b6a            and.l     d0,$00014B6A
[0001191c] 4240                      clr.w     d0
[0001191e] 4e5e                      unlk      a6
[00011920] 4e75                      rts
__freec:
[00011922] 4e56 0000                 link      a6,#$0000
[00011926] 48e7 0300                 movem.l   d6-d7,-(a7)
[0001192a] 4247                      clr.w     d7
[0001192c] 6006                      bra.s     $00011934
[0001192e] 3e87                      move.w    d7,(a7)
[00011930] 6112                      bsr.s     ___chini
[00011932] 5247                      addq.w    #1,d7
[00011934] be7c 0005                 cmp.w     #$0005,d7
[00011938] 6df4                      blt.s     $0001192E
[0001193a] 4a9f                      tst.l     (a7)+
[0001193c] 4cdf 0080                 movem.l   (a7)+,d7
[00011940] 4e5e                      unlk      a6
[00011942] 4e75                      rts
___chini:
[00011944] 4e56 0000                 link      a6,#$0000
[00011948] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0001194c] 302e 0008                 move.w    8(a6),d0
[00011950] c1fc 0238                 muls.w    #$0238,d0
[00011954] 2a40                      movea.l   d0,a5
[00011956] dbfc 0001 51b0            adda.l    #$000151B0,a5
[0001195c] 302e 0008                 move.w    8(a6),d0
[00011960] 1b40 0003                 move.b    d0,3(a5)
[00011964] 4255                      clr.w     (a5)
[00011966] 422d 0002                 clr.b     2(a5)
[0001196a] 2b7c ffff ffff 000a       move.l    #$FFFFFFFF,10(a5)
[00011972] 42ad 0006                 clr.l     6(a5)
[00011976] 42ad 000e                 clr.l     14(a5)
[0001197a] 3ebc 0020                 move.w    #$0020,(a7)
[0001197e] 4267                      clr.w     -(a7)
[00011980] 2f0d                      move.l    a5,-(a7)
[00011982] 0697 0000 0012            addi.l    #$00000012,(a7)
[00011988] 4eb9 0001 00c4            jsr       _memset
[0001198e] 5c8f                      addq.l    #6,a7
[00011990] 3ebc 000b                 move.w    #$000B,(a7)
[00011994] 3f3c 0020                 move.w    #$0020,-(a7)
[00011998] 2f0d                      move.l    a5,-(a7)
[0001199a] 0697 0000 0013            addi.l    #$00000013,(a7)
[000119a0] 4eb9 0001 00c4            jsr       _memset
[000119a6] 5c8f                      addq.l    #6,a7
[000119a8] 4a9f                      tst.l     (a7)+
[000119aa] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000119ae] 4e5e                      unlk      a6
[000119b0] 4e75                      rts
__chkc:
[000119b2] 4e56 0000                 link      a6,#$0000
[000119b6] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000119ba] 3e2e 0008                 move.w    8(a6),d7
[000119be] be7c 0005                 cmp.w     #$0005,d7
[000119c2] 6516                      bcs.s     $000119DA
[000119c4] 33fc 0009 0001 4db0       move.w    #$0009,$00014DB0
[000119cc] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[000119d6] 4280                      clr.l     d0
[000119d8] 6030                      bra.s     $00011A0A
[000119da] 4240                      clr.w     d0
[000119dc] 3007                      move.w    d7,d0
[000119de] c0fc 0238                 mulu.w    #$0238,d0
[000119e2] 2a40                      movea.l   d0,a5
[000119e4] dbfc 0001 51b0            adda.l    #$000151B0,a5
[000119ea] 082d 0000 0001            btst      #0,1(a5)
[000119f0] 6616                      bne.s     $00011A08
[000119f2] 33fc 0009 0001 4db0       move.w    #$0009,$00014DB0
[000119fa] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[00011a04] 4280                      clr.l     d0
[00011a06] 6002                      bra.s     $00011A0A
[00011a08] 200d                      move.l    a5,d0
[00011a0a] 4a9f                      tst.l     (a7)+
[00011a0c] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00011a10] 4e5e                      unlk      a6
[00011a12] 4e75                      rts
_fdopen:
[00011a14] 4e56 0000                 link      a6,#$0000
[00011a18] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011a1c] 3e2e 0008                 move.w    8(a6),d7
[00011a20] 2a6e 000a                 movea.l   10(a6),a5
[00011a24] 4a47                      tst.w     d7
[00011a26] 6d16                      blt.s     $00011A3E
[00011a28] 3ebc 0001                 move.w    #$0001,(a7)
[00011a2c] 42a7                      clr.l     -(a7)
[00011a2e] 3f07                      move.w    d7,-(a7)
[00011a30] 4eb9 0001 1842            jsr       _lseek
[00011a36] 5c8f                      addq.l    #6,a7
[00011a38] b07c ffff                 cmp.w     #$FFFF,d0
[00011a3c] 6606                      bne.s     $00011A44
[00011a3e] 4280                      clr.l     d0
[00011a40] 6000 0078                 bra       $00011ABA
[00011a44] 4246                      clr.w     d6
[00011a46] 6008                      bra.s     $00011A50
[00011a48] bc7c 0010                 cmp.w     #$0010,d6
[00011a4c] 6c1a                      bge.s     $00011A68
[00011a4e] 5246                      addq.w    #1,d6
[00011a50] 3006                      move.w    d6,d0
[00011a52] c1fc 000e                 muls.w    #$000E,d0
[00011a56] 2840                      movea.l   d0,a4
[00011a58] d9fc 0001 4bca            adda.l    #$00014BCA,a4
[00011a5e] 302c 0002                 move.w    2(a4),d0
[00011a62] c07c 0003                 and.w     #$0003,d0
[00011a66] 66e0                      bne.s     $00011A48
[00011a68] bc7c 0010                 cmp.w     #$0010,d6
[00011a6c] 6d04                      blt.s     $00011A72
[00011a6e] 4280                      clr.l     d0
[00011a70] 6048                      bra.s     $00011ABA
[00011a72] 0c15 0072                 cmpi.b    #$72,(a5)
[00011a76] 672a                      beq.s     $00011AA2
[00011a78] 0c15 0052                 cmpi.b    #$52,(a5)
[00011a7c] 6724                      beq.s     $00011AA2
[00011a7e] 006c 0002 0002            ori.w     #$0002,2(a4)
[00011a84] 0c15 0061                 cmpi.b    #$61,(a5)
[00011a88] 6706                      beq.s     $00011A90
[00011a8a] 0c15 0041                 cmpi.b    #$41,(a5)
[00011a8e] 6610                      bne.s     $00011AA0
[00011a90] 3ebc 0002                 move.w    #$0002,(a7)
[00011a94] 42a7                      clr.l     -(a7)
[00011a96] 3f07                      move.w    d7,-(a7)
[00011a98] 4eb9 0001 1842            jsr       _lseek
[00011a9e] 5c8f                      addq.l    #6,a7
[00011aa0] 6006                      bra.s     $00011AA8
[00011aa2] 006c 0001 0002            ori.w     #$0001,2(a4)
[00011aa8] 426c 000c                 clr.w     12(a4)
[00011aac] 3887                      move.w    d7,(a4)
[00011aae] 4280                      clr.l     d0
[00011ab0] 2940 0008                 move.l    d0,8(a4)
[00011ab4] 2940 0004                 move.l    d0,4(a4)
[00011ab8] 200c                      move.l    a4,d0
[00011aba] 4a9f                      tst.l     (a7)+
[00011abc] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00011ac0] 4e5e                      unlk      a6
[00011ac2] 4e75                      rts
__fopen:
[00011ac4] 4e56 0000                 link      a6,#$0000
[00011ac8] 48e7 071c                 movem.l   d5-d7/a3-a5,-(a7)
[00011acc] 2a6e 0008                 movea.l   8(a6),a5
[00011ad0] 286e 000c                 movea.l   12(a6),a4
[00011ad4] 4247                      clr.w     d7
[00011ad6] 6008                      bra.s     $00011AE0
[00011ad8] be7c 0010                 cmp.w     #$0010,d7
[00011adc] 6c1a                      bge.s     $00011AF8
[00011ade] 5247                      addq.w    #1,d7
[00011ae0] 3007                      move.w    d7,d0
[00011ae2] c1fc 000e                 muls.w    #$000E,d0
[00011ae6] 2640                      movea.l   d0,a3
[00011ae8] d7fc 0001 4bca            adda.l    #$00014BCA,a3
[00011aee] 302b 0002                 move.w    2(a3),d0
[00011af2] c07c 0003                 and.w     #$0003,d0
[00011af6] 66e0                      bne.s     $00011AD8
[00011af8] be7c 0010                 cmp.w     #$0010,d7
[00011afc] 6d06                      blt.s     $00011B04
[00011afe] 4280                      clr.l     d0
[00011b00] 6000 00d4                 bra       $00011BD6
[00011b04] 0c14 0077                 cmpi.b    #$77,(a4)
[00011b08] 6706                      beq.s     $00011B10
[00011b0a] 0c14 0057                 cmpi.b    #$57,(a4)
[00011b0e] 6618                      bne.s     $00011B28
[00011b10] 3eae 0010                 move.w    16(a6),(a7)
[00011b14] 3f3c 0001                 move.w    #$0001,-(a7)
[00011b18] 2f0d                      move.l    a5,-(a7)
[00011b1a] 4eb9 0001 24ca            jsr       __creat
[00011b20] 5c8f                      addq.l    #6,a7
[00011b22] 3c00                      move.w    d0,d6
[00011b24] 6000 0070                 bra       $00011B96
[00011b28] 0c14 0061                 cmpi.b    #$61,(a4)
[00011b2c] 6706                      beq.s     $00011B34
[00011b2e] 0c14 0041                 cmpi.b    #$41,(a4)
[00011b32] 663e                      bne.s     $00011B72
[00011b34] 3eae 0010                 move.w    16(a6),(a7)
[00011b38] 3f3c 0001                 move.w    #$0001,-(a7)
[00011b3c] 2f0d                      move.l    a5,-(a7)
[00011b3e] 4eb9 0001 2796            jsr       __open
[00011b44] 5c8f                      addq.l    #6,a7
[00011b46] 3c00                      move.w    d0,d6
[00011b48] 6c16                      bge.s     $00011B60
[00011b4a] 3eae 0010                 move.w    16(a6),(a7)
[00011b4e] 3f3c 0001                 move.w    #$0001,-(a7)
[00011b52] 2f0d                      move.l    a5,-(a7)
[00011b54] 4eb9 0001 24ca            jsr       __creat
[00011b5a] 5c8f                      addq.l    #6,a7
[00011b5c] 3c00                      move.w    d0,d6
[00011b5e] 6010                      bra.s     $00011B70
[00011b60] 3ebc 0002                 move.w    #$0002,(a7)
[00011b64] 42a7                      clr.l     -(a7)
[00011b66] 3f06                      move.w    d6,-(a7)
[00011b68] 4eb9 0001 1842            jsr       _lseek
[00011b6e] 5c8f                      addq.l    #6,a7
[00011b70] 6024                      bra.s     $00011B96
[00011b72] 0c14 0072                 cmpi.b    #$72,(a4)
[00011b76] 6706                      beq.s     $00011B7E
[00011b78] 0c14 0052                 cmpi.b    #$52,(a4)
[00011b7c] 6614                      bne.s     $00011B92
[00011b7e] 3eae 0010                 move.w    16(a6),(a7)
[00011b82] 4267                      clr.w     -(a7)
[00011b84] 2f0d                      move.l    a5,-(a7)
[00011b86] 4eb9 0001 2796            jsr       __open
[00011b8c] 5c8f                      addq.l    #6,a7
[00011b8e] 3c00                      move.w    d0,d6
[00011b90] 6004                      bra.s     $00011B96
[00011b92] 4280                      clr.l     d0
[00011b94] 6040                      bra.s     $00011BD6
[00011b96] 4a46                      tst.w     d6
[00011b98] 6c04                      bge.s     $00011B9E
[00011b9a] 4280                      clr.l     d0
[00011b9c] 6038                      bra.s     $00011BD6
[00011b9e] 426b 000c                 clr.w     12(a3)
[00011ba2] 3686                      move.w    d6,(a3)
[00011ba4] 4280                      clr.l     d0
[00011ba6] 2740 0008                 move.l    d0,8(a3)
[00011baa] 2740 0004                 move.l    d0,4(a3)
[00011bae] 0c14 0072                 cmpi.b    #$72,(a4)
[00011bb2] 6706                      beq.s     $00011BBA
[00011bb4] 0c14 0052                 cmpi.b    #$52,(a4)
[00011bb8] 6608                      bne.s     $00011BC2
[00011bba] 377c 0001 0002            move.w    #$0001,2(a3)
[00011bc0] 6006                      bra.s     $00011BC8
[00011bc2] 377c 0002 0002            move.w    #$0002,2(a3)
[00011bc8] 4a6e 0010                 tst.w     16(a6)
[00011bcc] 6606                      bne.s     $00011BD4
[00011bce] 006b 0100 0002            ori.w     #$0100,2(a3)
[00011bd4] 200b                      move.l    a3,d0
[00011bd6] 4a9f                      tst.l     (a7)+
[00011bd8] 4cdf 38c0                 movem.l   (a7)+,d6-d7/a3-a5
[00011bdc] 4e5e                      unlk      a6
[00011bde] 4e75                      rts
_fopen:
[00011be0] 4e56 fffc                 link      a6,#$FFFC
[00011be4] 4257                      clr.w     (a7)
[00011be6] 2f2e 000c                 move.l    12(a6),-(a7)
[00011bea] 2f2e 0008                 move.l    8(a6),-(a7)
[00011bee] 6100 fed4                 bsr       __fopen
[00011bf2] 508f                      addq.l    #8,a7
[00011bf4] 4e5e                      unlk      a6
[00011bf6] 4e75                      rts
_fopena:
[00011bf8] 4e56 fffc                 link      a6,#$FFFC
[00011bfc] 4257                      clr.w     (a7)
[00011bfe] 2f2e 000c                 move.l    12(a6),-(a7)
[00011c02] 2f2e 0008                 move.l    8(a6),-(a7)
[00011c06] 6100 febc                 bsr       __fopen
[00011c0a] 508f                      addq.l    #8,a7
[00011c0c] 4e5e                      unlk      a6
[00011c0e] 4e75                      rts
_fopenb:
[00011c10] 4e56 fffc                 link      a6,#$FFFC
[00011c14] 3ebc 0001                 move.w    #$0001,(a7)
[00011c18] 2f2e 000c                 move.l    12(a6),-(a7)
[00011c1c] 2f2e 0008                 move.l    8(a6),-(a7)
[00011c20] 6100 fea2                 bsr       __fopen
[00011c24] 508f                      addq.l    #8,a7
[00011c26] 4e5e                      unlk      a6
[00011c28] 4e75                      rts
_fread:
[00011c2a] 4e56 0000                 link      a6,#$0000
[00011c2e] 48e7 0f0c                 movem.l   d4-d7/a4-a5,-(a7)
[00011c32] 2a6e 0008                 movea.l   8(a6),a5
[00011c36] 286e 0010                 movea.l   16(a6),a4
[00011c3a] 4247                      clr.w     d7
[00011c3c] 6026                      bra.s     $00011C64
[00011c3e] 4246                      clr.w     d6
[00011c40] 601a                      bra.s     $00011C5C
[00011c42] 2e8c                      move.l    a4,(a7)
[00011c44] 4eb9 0001 1dae            jsr       _fgetc
[00011c4a] 3a00                      move.w    d0,d5
[00011c4c] ba7c ffff                 cmp.w     #$FFFF,d5
[00011c50] 6606                      bne.s     $00011C58
[00011c52] 3007                      move.w    d7,d0
[00011c54] 6018                      bra.s     $00011C6E
[00011c56] 6002                      bra.s     $00011C5A
[00011c58] 1ac5                      move.b    d5,(a5)+
[00011c5a] 5246                      addq.w    #1,d6
[00011c5c] bc6e 000c                 cmp.w     12(a6),d6
[00011c60] 6de0                      blt.s     $00011C42
[00011c62] 5247                      addq.w    #1,d7
[00011c64] be6e 000e                 cmp.w     14(a6),d7
[00011c68] 6dd4                      blt.s     $00011C3E
[00011c6a] 302e 000e                 move.w    14(a6),d0
[00011c6e] 4a9f                      tst.l     (a7)+
[00011c70] 4cdf 30e0                 movem.l   (a7)+,d5-d7/a4-a5
[00011c74] 4e5e                      unlk      a6
[00011c76] 4e75                      rts
_fwrite:
[00011c78] 4e56 0000                 link      a6,#$0000
[00011c7c] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00011c80] 2a6e 0008                 movea.l   8(a6),a5
[00011c84] 286e 0010                 movea.l   16(a6),a4
[00011c88] 4247                      clr.w     d7
[00011c8a] 6028                      bra.s     $00011CB4
[00011c8c] 4246                      clr.w     d6
[00011c8e] 601c                      bra.s     $00011CAC
[00011c90] 2e8c                      move.l    a4,(a7)
[00011c92] 101d                      move.b    (a5)+,d0
[00011c94] 4880                      ext.w     d0
[00011c96] 3f00                      move.w    d0,-(a7)
[00011c98] 4eb9 0001 2ee8            jsr       _fputc
[00011c9e] 548f                      addq.l    #2,a7
[00011ca0] b07c ffff                 cmp.w     #$FFFF,d0
[00011ca4] 6604                      bne.s     $00011CAA
[00011ca6] 4240                      clr.w     d0
[00011ca8] 6014                      bra.s     $00011CBE
[00011caa] 5246                      addq.w    #1,d6
[00011cac] bc6e 000c                 cmp.w     12(a6),d6
[00011cb0] 6dde                      blt.s     $00011C90
[00011cb2] 5247                      addq.w    #1,d7
[00011cb4] be6e 000e                 cmp.w     14(a6),d7
[00011cb8] 6dd2                      blt.s     $00011C8C
[00011cba] 302e 000e                 move.w    14(a6),d0
[00011cbe] 4a9f                      tst.l     (a7)+
[00011cc0] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00011cc4] 4e5e                      unlk      a6
[00011cc6] 4e75                      rts
__main:
[00011cc8] 4e56 0000                 link      a6,#$0000
[00011ccc] 48e7 0104                 movem.l   d7/a5,-(a7)
[00011cd0] 4eb9 0001 1922            jsr       __freec
[00011cd6] 4257                      clr.w     (a7)
[00011cd8] 2f3c 0001 4771            move.l    #$00014771,-(a7)
[00011cde] 4eb9 0001 2890            jsr       _open
[00011ce4] 588f                      addq.l    #4,a7
[00011ce6] 3ebc 0001                 move.w    #$0001,(a7)
[00011cea] 2f3c 0001 4771            move.l    #$00014771,-(a7)
[00011cf0] 4eb9 0001 2890            jsr       _open
[00011cf6] 588f                      addq.l    #4,a7
[00011cf8] 3ebc 0001                 move.w    #$0001,(a7)
[00011cfc] 2f3c 0001 4771            move.l    #$00014771,-(a7)
[00011d02] 4eb9 0001 2890            jsr       _open
[00011d08] 588f                      addq.l    #4,a7
[00011d0a] 206e 0008                 movea.l   8(a6),a0
[00011d0e] 326e 000c                 movea.w   12(a6),a1
[00011d12] d1c9                      adda.l    a1,a0
[00011d14] 4210                      clr.b     (a0)
[00011d16] 2a6e 0008                 movea.l   8(a6),a5
[00011d1a] 6026                      bra.s     $00011D42
[00011d1c] 1015                      move.b    (a5),d0
[00011d1e] 4880                      ext.w     d0
[00011d20] 48c0                      ext.l     d0
[00011d22] d0bc 0001 4db4            add.l     #$00014DB4,d0
[00011d28] 2040                      movea.l   d0,a0
[00011d2a] 0810 0003                 btst      #3,(a0)
[00011d2e] 670a                      beq.s     $00011D3A
[00011d30] 1015                      move.b    (a5),d0
[00011d32] 4880                      ext.w     d0
[00011d34] d07c 0020                 add.w     #$0020,d0
[00011d38] 6004                      bra.s     $00011D3E
[00011d3a] 1015                      move.b    (a5),d0
[00011d3c] 4880                      ext.w     d0
[00011d3e] 1a80                      move.b    d0,(a5)
[00011d40] 528d                      addq.l    #1,a5
[00011d42] 4a15                      tst.b     (a5)
[00011d44] 66d6                      bne.s     $00011D1C
[00011d46] 3eae 000c                 move.w    12(a6),(a7)
[00011d4a] 2f2e 0008                 move.l    8(a6),-(a7)
[00011d4e] 4eb9 0001 213a            jsr       ___main
[00011d54] 588f                      addq.l    #4,a7
[00011d56] 4a9f                      tst.l     (a7)+
[00011d58] 4cdf 2000                 movem.l   (a7)+,a5
[00011d5c] 4e5e                      unlk      a6
[00011d5e] 4e75                      rts
_getpid:
[00011d60] 4e56 fffc                 link      a6,#$FFFC
[00011d64] 303c 00de                 move.w    #$00DE,d0
[00011d68] 4e5e                      unlk      a6
[00011d6a] 4e75                      rts
_printf:
[00011d6c] 4e56 fffc                 link      a6,#$FFFC
[00011d70] 2e8e                      move.l    a6,(a7)
[00011d72] 0697 0000 000c            addi.l    #$0000000C,(a7)
[00011d78] 2f2e 0008                 move.l    8(a6),-(a7)
[00011d7c] 2f3c 0001 4bd8            move.l    #$00014BD8,-(a7)
[00011d82] 4eb9 0001 2932            jsr       __doprt
[00011d88] 508f                      addq.l    #8,a7
[00011d8a] 4e5e                      unlk      a6
[00011d8c] 4e75                      rts
_fprintf:
[00011d8e] 4e56 fffc                 link      a6,#$FFFC
[00011d92] 2e8e                      move.l    a6,(a7)
[00011d94] 0697 0000 0010            addi.l    #$00000010,(a7)
[00011d9a] 2f2e 000c                 move.l    12(a6),-(a7)
[00011d9e] 2f2e 0008                 move.l    8(a6),-(a7)
[00011da2] 4eb9 0001 2932            jsr       __doprt
[00011da8] 508f                      addq.l    #8,a7
[00011daa] 4e5e                      unlk      a6
[00011dac] 4e75                      rts
_fgetc:
[00011dae] 4e56 0000                 link      a6,#$0000
[00011db2] 48e7 0104                 movem.l   d7/a5,-(a7)
[00011db6] 2a6e 0008                 movea.l   8(a6),a5
[00011dba] 536d 000c                 subq.w    #1,12(a5)
[00011dbe] 6d14                      blt.s     $00011DD4
[00011dc0] 206d 0008                 movea.l   8(a5),a0
[00011dc4] 1010                      move.b    (a0),d0
[00011dc6] 4880                      ext.w     d0
[00011dc8] c07c 00ff                 and.w     #$00FF,d0
[00011dcc] 52ad 0008                 addq.l    #1,8(a5)
[00011dd0] 600a                      bra.s     $00011DDC
[00011dd2] 6008                      bra.s     $00011DDC
[00011dd4] 2e8d                      move.l    a5,(a7)
[00011dd6] 4eb9 0001 1de6            jsr       __filbuf
[00011ddc] 4a9f                      tst.l     (a7)+
[00011dde] 4cdf 2000                 movem.l   (a7)+,a5
[00011de2] 4e5e                      unlk      a6
[00011de4] 4e75                      rts
__filbuf:
[00011de6] 4e56 0000                 link      a6,#$0000
[00011dea] 48e7 0104                 movem.l   d7/a5,-(a7)
[00011dee] 2a6e 0008                 movea.l   8(a6),a5
[00011df2] 082d 0000 0003            btst      #0,3(a5)
[00011df8] 6606                      bne.s     $00011E00
[00011dfa] 70ff                      moveq.l   #-1,d0
[00011dfc] 6000 00d4                 bra       $00011ED2
[00011e00] 082d 0007 0003            btst      #7,3(a5)
[00011e06] 670c                      beq.s     $00011E14
[00011e08] 006d 0020 0002            ori.w     #$0020,2(a5)
[00011e0e] 70ff                      moveq.l   #-1,d0
[00011e10] 6000 00c0                 bra       $00011ED2
[00011e14] 4aad 0004                 tst.l     4(a5)
[00011e18] 6626                      bne.s     $00011E40
[00011e1a] 082d 0003 0003            btst      #3,3(a5)
[00011e20] 6610                      bne.s     $00011E32
[00011e22] 3ebc 0200                 move.w    #$0200,(a7)
[00011e26] 4eb9 0001 30f8            jsr       _malloc
[00011e2c] 2b40 0004                 move.l    d0,4(a5)
[00011e30] 6608                      bne.s     $00011E3A
[00011e32] 006d 0008 0002            ori.w     #$0008,2(a5)
[00011e38] 6006                      bra.s     $00011E40
[00011e3a] 006d 0004 0002            ori.w     #$0004,2(a5)
[00011e40] 082d 0003 0003            btst      #3,3(a5)
[00011e46] 670e                      beq.s     $00011E56
[00011e48] 3015                      move.w    (a5),d0
[00011e4a] 48c0                      ext.l     d0
[00011e4c] d0bc 0001 4eb0            add.l     #$00014EB0,d0
[00011e52] 2b40 0004                 move.l    d0,4(a5)
[00011e56] bbfc 0001 4bca            cmpa.l    #$00014BCA,a5
[00011e5c] 6616                      bne.s     $00011E74
[00011e5e] 0839 0006 0001 4bdb       btst      #6,$00014BDB
[00011e66] 670c                      beq.s     $00011E74
[00011e68] 2ebc 0001 4bd8            move.l    #$00014BD8,(a7)
[00011e6e] 4eb9 0001 26fc            jsr       _fflush
[00011e74] 082d 0003 0003            btst      #3,3(a5)
[00011e7a] 6706                      beq.s     $00011E82
[00011e7c] 3ebc 0001                 move.w    #$0001,(a7)
[00011e80] 6004                      bra.s     $00011E86
[00011e82] 3ebc 0200                 move.w    #$0200,(a7)
[00011e86] 2f2d 0004                 move.l    4(a5),-(a7)
[00011e8a] 3f15                      move.w    (a5),-(a7)
[00011e8c] 4eb9 0001 1edc            jsr       _read
[00011e92] 5c8f                      addq.l    #6,a7
[00011e94] 3b40 000c                 move.w    d0,12(a5)
[00011e98] 4a6d 000c                 tst.w     12(a5)
[00011e9c] 6e1a                      bgt.s     $00011EB8
[00011e9e] 0c6d ffff 000c            cmpi.w    #$FFFF,12(a5)
[00011ea4] 6608                      bne.s     $00011EAE
[00011ea6] 006d 0030 0002            ori.w     #$0030,2(a5)
[00011eac] 6006                      bra.s     $00011EB4
[00011eae] 006d 0020 0002            ori.w     #$0020,2(a5)
[00011eb4] 70ff                      moveq.l   #-1,d0
[00011eb6] 601a                      bra.s     $00011ED2
[00011eb8] 536d 000c                 subq.w    #1,12(a5)
[00011ebc] 2b6d 0004 0008            move.l    4(a5),8(a5)
[00011ec2] 206d 0008                 movea.l   8(a5),a0
[00011ec6] 1010                      move.b    (a0),d0
[00011ec8] 4880                      ext.w     d0
[00011eca] c07c 00ff                 and.w     #$00FF,d0
[00011ece] 52ad 0008                 addq.l    #1,8(a5)
[00011ed2] 4a9f                      tst.l     (a7)+
[00011ed4] 4cdf 2000                 movem.l   (a7)+,a5
[00011ed8] 4e5e                      unlk      a6
[00011eda] 4e75                      rts
_read:
[00011edc] 4e56 0000                 link      a6,#$0000
[00011ee0] 48e7 0104                 movem.l   d7/a5,-(a7)
[00011ee4] 3eae 0008                 move.w    8(a6),(a7)
[00011ee8] 4eb9 0001 19b2            jsr       __chkc
[00011eee] 2a40                      movea.l   d0,a5
[00011ef0] 200d                      move.l    a5,d0
[00011ef2] 6604                      bne.s     $00011EF8
[00011ef4] 70ff                      moveq.l   #-1,d0
[00011ef6] 6062                      bra.s     $00011F5A
[00011ef8] 082d 0005 0001            btst      #5,1(a5)
[00011efe] 6704                      beq.s     $00011F04
[00011f00] 4240                      clr.w     d0
[00011f02] 6056                      bra.s     $00011F5A
[00011f04] 082d 0002 0001            btst      #2,1(a5)
[00011f0a] 6716                      beq.s     $00011F22
[00011f0c] 33fc 0009 0001 4db0       move.w    #$0009,$00014DB0
[00011f14] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[00011f1e] 70ff                      moveq.l   #-1,d0
[00011f20] 6038                      bra.s     $00011F5A
[00011f22] 082d 0004 0001            btst      #4,1(a5)
[00011f28] 671a                      beq.s     $00011F44
[00011f2a] 4280                      clr.l     d0
[00011f2c] 302e 000e                 move.w    14(a6),d0
[00011f30] 2e80                      move.l    d0,(a7)
[00011f32] 2f2e 000a                 move.l    10(a6),-(a7)
[00011f36] 2f0d                      move.l    a5,-(a7)
[00011f38] 4eb9 0001 1f64            jsr       __rdasc
[00011f3e] 508f                      addq.l    #8,a7
[00011f40] 6018                      bra.s     $00011F5A
[00011f42] 6016                      bra.s     $00011F5A
[00011f44] 4280                      clr.l     d0
[00011f46] 302e 000e                 move.w    14(a6),d0
[00011f4a] 2e80                      move.l    d0,(a7)
[00011f4c] 2f2e 000a                 move.l    10(a6),-(a7)
[00011f50] 2f0d                      move.l    a5,-(a7)
[00011f52] 4eb9 0001 2086            jsr       __rdbin
[00011f58] 508f                      addq.l    #8,a7
[00011f5a] 4a9f                      tst.l     (a7)+
[00011f5c] 4cdf 2000                 movem.l   (a7)+,a5
[00011f60] 4e5e                      unlk      a6
[00011f62] 4e75                      rts
__rdasc:
[00011f64] 4e56 fff8                 link      a6,#$FFF8
[00011f68] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00011f6c] 2a6e 0008                 movea.l   8(a6),a5
[00011f70] 286e 000c                 movea.l   12(a6),a4
[00011f74] 2e2e 0010                 move.l    16(a6),d7
[00011f78] 2d47 fffc                 move.l    d7,-4(a6)
[00011f7c] 6000 0096                 bra       $00012014
[00011f80] 3ebc 0001                 move.w    #$0001,(a7)
[00011f84] 2007                      move.l    d7,d0
[00011f86] 3f00                      move.w    d0,-(a7)
[00011f88] 2f0c                      move.l    a4,-(a7)
[00011f8a] 2f2d 0006                 move.l    6(a5),-(a7)
[00011f8e] 2f0d                      move.l    a5,-(a7)
[00011f90] 0697 0000 0012            addi.l    #$00000012,(a7)
[00011f96] 4eb9 0001 3cdc            jsr       __pc_rea
[00011f9c] dffc 0000 000e            adda.l    #$0000000E,a7
[00011fa2] 3d40 fffa                 move.w    d0,-6(a6)
[00011fa6] 426e fff8                 clr.w     -8(a6)
[00011faa] 6014                      bra.s     $00011FC0
[00011fac] 204c                      movea.l   a4,a0
[00011fae] 93c9                      suba.l    a1,a1
[00011fb0] 326e fff8                 movea.w   -8(a6),a1
[00011fb4] d1c9                      adda.l    a1,a0
[00011fb6] 0c10 001a                 cmpi.b    #$1A,(a0)
[00011fba] 6710                      beq.s     $00011FCC
[00011fbc] 526e fff8                 addq.w    #1,-8(a6)
[00011fc0] 4240                      clr.w     d0
[00011fc2] 302e fff8                 move.w    -8(a6),d0
[00011fc6] b06e fffa                 cmp.w     -6(a6),d0
[00011fca] 65e0                      bcs.s     $00011FAC
[00011fcc] 4a6e fffa                 tst.w     -6(a6)
[00011fd0] 6710                      beq.s     $00011FE2
[00011fd2] 204c                      movea.l   a4,a0
[00011fd4] 93c9                      suba.l    a1,a1
[00011fd6] 326e fff8                 movea.w   -8(a6),a1
[00011fda] d1c9                      adda.l    a1,a0
[00011fdc] 0c10 001a                 cmpi.b    #$1A,(a0)
[00011fe0] 660a                      bne.s     $00011FEC
[00011fe2] 3d6e fff8 fffa            move.w    -8(a6),-6(a6)
[00011fe8] 0055 0020                 ori.w     #$0020,(a5)
[00011fec] 3eae fffa                 move.w    -6(a6),(a7)
[00011ff0] 2f0c                      move.l    a4,-(a7)
[00011ff2] 6150                      bsr.s     $00012044
[00011ff4] 588f                      addq.l    #4,a7
[00011ff6] 3d40 fff8                 move.w    d0,-8(a6)
[00011ffa] 4280                      clr.l     d0
[00011ffc] 302e fffa                 move.w    -6(a6),d0
[00012000] d1ad 0006                 add.l     d0,6(a5)
[00012004] 4280                      clr.l     d0
[00012006] 302e fff8                 move.w    -8(a6),d0
[0001200a] d9c0                      adda.l    d0,a4
[0001200c] 4280                      clr.l     d0
[0001200e] 302e fff8                 move.w    -8(a6),d0
[00012012] 9e80                      sub.l     d0,d7
[00012014] 4a87                      tst.l     d7
[00012016] 6f0a                      ble.s     $00012022
[00012018] 082d 0005 0001            btst      #5,1(a5)
[0001201e] 6700 ff60                 beq       $00011F80
[00012022] 9fae fffc                 sub.l     d7,-4(a6)
[00012026] 202d 0006                 move.l    6(a5),d0
[0001202a] b0ad 000e                 cmp.l     14(a5),d0
[0001202e] 6f06                      ble.s     $00012036
[00012030] 2b6d 0006 000e            move.l    6(a5),14(a5)
[00012036] 202e fffc                 move.l    -4(a6),d0
[0001203a] 4a9f                      tst.l     (a7)+
[0001203c] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00012040] 4e5e                      unlk      a6
[00012042] 4e75                      rts
[00012044] 4e56 fff8                 link      a6,#$FFF8
[00012048] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0001204c] 2a6e 0008                 movea.l   8(a6),a5
[00012050] 2d4d fffc                 move.l    a5,-4(a6)
[00012054] 200d                      move.l    a5,d0
[00012056] 4281                      clr.l     d1
[00012058] 322e 000c                 move.w    12(a6),d1
[0001205c] d081                      add.l     d1,d0
[0001205e] 2d40 fff8                 move.l    d0,-8(a6)
[00012062] 284d                      movea.l   a5,a4
[00012064] 600a                      bra.s     $00012070
[00012066] 0c14 000d                 cmpi.b    #$0D,(a4)
[0001206a] 6702                      beq.s     $0001206E
[0001206c] 1ad4                      move.b    (a4),(a5)+
[0001206e] 528c                      addq.l    #1,a4
[00012070] b9ee fff8                 cmpa.l    -8(a6),a4
[00012074] 65f0                      bcs.s     $00012066
[00012076] 200d                      move.l    a5,d0
[00012078] 90ae fffc                 sub.l     -4(a6),d0
[0001207c] 4a9f                      tst.l     (a7)+
[0001207e] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00012082] 4e5e                      unlk      a6
[00012084] 4e75                      rts
__rdbin:
[00012086] 4e56 fffe                 link      a6,#$FFFE
[0001208a] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001208e] 2a6e 0008                 movea.l   8(a6),a5
[00012092] 286e 000c                 movea.l   12(a6),a4
[00012096] 2e2e 0010                 move.l    16(a6),d7
[0001209a] 3ebc 0001                 move.w    #$0001,(a7)
[0001209e] 2007                      move.l    d7,d0
[000120a0] 3f00                      move.w    d0,-(a7)
[000120a2] 2f0c                      move.l    a4,-(a7)
[000120a4] 2f2d 0006                 move.l    6(a5),-(a7)
[000120a8] 2f0d                      move.l    a5,-(a7)
[000120aa] 0697 0000 0012            addi.l    #$00000012,(a7)
[000120b0] 4eb9 0001 3cdc            jsr       __pc_rea
[000120b6] dffc 0000 000e            adda.l    #$0000000E,a7
[000120bc] 3d40 fffe                 move.w    d0,-2(a6)
[000120c0] 4a6e fffe                 tst.w     -2(a6)
[000120c4] 6604                      bne.s     $000120CA
[000120c6] 0055 0020                 ori.w     #$0020,(a5)
[000120ca] 4280                      clr.l     d0
[000120cc] 302e fffe                 move.w    -2(a6),d0
[000120d0] d1ad 0006                 add.l     d0,6(a5)
[000120d4] 202d 0006                 move.l    6(a5),d0
[000120d8] b0ad 000e                 cmp.l     14(a5),d0
[000120dc] 6f06                      ble.s     $000120E4
[000120de] 2b6d 0006 000e            move.l    6(a5),14(a5)
[000120e4] 4240                      clr.w     d0
[000120e6] 302e fffe                 move.w    -2(a6),d0
[000120ea] 4a9f                      tst.l     (a7)+
[000120ec] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[000120f0] 4e5e                      unlk      a6
[000120f2] 4e75                      rts
_unlink:
[000120f4] 4e56 0000                 link      a6,#$0000
[000120f8] 48e7 0700                 movem.l   d5-d7,-(a7)
[000120fc] 4eb9 0001 18bc            jsr       __allocc
[00012102] 3e00                      move.w    d0,d7
[00012104] 6c04                      bge.s     $0001210A
[00012106] 70ff                      moveq.l   #-1,d0
[00012108] 6026                      bra.s     $00012130
[0001210a] 3e87                      move.w    d7,(a7)
[0001210c] 4eb9 0001 1944            jsr       ___chini
[00012112] 3ebc 0013                 move.w    #$0013,(a7)
[00012116] 2f2e 0008                 move.l    8(a6),-(a7)
[0001211a] 3f07                      move.w    d7,-(a7)
[0001211c] 4eb9 0001 381c            jsr       ___open
[00012122] 5c8f                      addq.l    #6,a7
[00012124] 3c00                      move.w    d0,d6
[00012126] 3e87                      move.w    d7,(a7)
[00012128] 4eb9 0001 1906            jsr       __freec
[0001212e] 3006                      move.w    d6,d0
[00012130] 4a9f                      tst.l     (a7)+
[00012132] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[00012136] 4e5e                      unlk      a6
[00012138] 4e75                      rts
___main:
[0001213a] 4e56 ffde                 link      a6,#$FFDE
[0001213e] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00012142] 4257                      clr.w     (a7)
[00012144] 4eb9 0001 35aa            jsr       _sbrk
[0001214a] 23c0 0001 4ec2            move.l    d0,$00014EC2
[00012150] 23c0 0001 4ec6            move.l    d0,$00014EC6
[00012156] 4279 0001 4ec0            clr.w     $00014EC0
[0001215c] 2ebc 0001 4767            move.l    #$00014767,(a7)
[00012162] 6100 0316                 bsr       $0001247A
[00012166] 2a6e 0008                 movea.l   8(a6),a5
[0001216a] 6000 024e                 bra       $000123BA
[0001216e] 6002                      bra.s     $00012172
[00012170] 528d                      addq.l    #1,a5
[00012172] 4a15                      tst.b     (a5)
[00012174] 6714                      beq.s     $0001218A
[00012176] 1015                      move.b    (a5),d0
[00012178] 4880                      ext.w     d0
[0001217a] 48c0                      ext.l     d0
[0001217c] d0bc 0001 4db4            add.l     #$00014DB4,d0
[00012182] 2040                      movea.l   d0,a0
[00012184] 0810 0005                 btst      #5,(a0)
[00012188] 66e6                      bne.s     $00012170
[0001218a] 4a15                      tst.b     (a5)
[0001218c] 6700 0232                 beq       $000123C0
[00012190] 0c15 0022                 cmpi.b    #$22,(a5)
[00012194] 6706                      beq.s     $0001219C
[00012196] 0c15 0027                 cmpi.b    #$27,(a5)
[0001219a] 6646                      bne.s     $000121E2
[0001219c] 1c15                      move.b    (a5),d6
[0001219e] 1006                      move.b    d6,d0
[000121a0] 4880                      ext.w     d0
[000121a2] 3e80                      move.w    d0,(a7)
[000121a4] 2f0d                      move.l    a5,-(a7)
[000121a6] 5297                      addq.l    #1,(a7)
[000121a8] 4eb9 0001 00e0            jsr       _strchr
[000121ae] 588f                      addq.l    #4,a7
[000121b0] 2840                      movea.l   d0,a4
[000121b2] 200c                      move.l    a4,d0
[000121b4] 660e                      bne.s     $000121C4
[000121b6] 2ebc 0001 4b6e            move.l    #$00014B6E,(a7)
[000121bc] 2f0d                      move.l    a5,-(a7)
[000121be] 6100 0256                 bsr       $00012416
[000121c2] 588f                      addq.l    #4,a7
[000121c4] 200c                      move.l    a4,d0
[000121c6] 48c0                      ext.l     d0
[000121c8] 908d                      sub.l     a5,d0
[000121ca] 3e00                      move.w    d0,d7
[000121cc] 204d                      movea.l   a5,a0
[000121ce] 3247                      movea.w   d7,a1
[000121d0] d1c9                      adda.l    a1,a0
[000121d2] 4210                      clr.b     (a0)
[000121d4] 5247                      addq.w    #1,d7
[000121d6] 2e8d                      move.l    a5,(a7)
[000121d8] 5297                      addq.l    #1,(a7)
[000121da] 6100 029e                 bsr       $0001247A
[000121de] 6000 01d8                 bra       $000123B8
[000121e2] 4247                      clr.w     d7
[000121e4] 6002                      bra.s     $000121E8
[000121e6] 5247                      addq.w    #1,d7
[000121e8] 204d                      movea.l   a5,a0
[000121ea] 3247                      movea.w   d7,a1
[000121ec] d1c9                      adda.l    a1,a0
[000121ee] 4a10                      tst.b     (a0)
[000121f0] 6716                      beq.s     $00012208
[000121f2] 1035 7000                 move.b    0(a5,d7.w),d0
[000121f6] 4880                      ext.w     d0
[000121f8] 48c0                      ext.l     d0
[000121fa] d0bc 0001 4db4            add.l     #$00014DB4,d0
[00012200] 2040                      movea.l   d0,a0
[00012202] 0810 0005                 btst      #5,(a0)
[00012206] 67de                      beq.s     $000121E6
[00012208] 4a35 7000                 tst.b     0(a5,d7.w)
[0001220c] 670a                      beq.s     $00012218
[0001220e] 204d                      movea.l   a5,a0
[00012210] 3247                      movea.w   d7,a1
[00012212] d1c9                      adda.l    a1,a0
[00012214] 4210                      clr.b     (a0)
[00012216] 5247                      addq.w    #1,d7
[00012218] 1015                      move.b    (a5),d0
[0001221a] 4880                      ext.w     d0
[0001221c] 6000 0186                 bra       $000123A4
[00012220] 4257                      clr.w     (a7)
[00012222] 4eb9 0001 266c            jsr       _close
[00012228] 4257                      clr.w     (a7)
[0001222a] 2f0d                      move.l    a5,-(a7)
[0001222c] 5297                      addq.l    #1,(a7)
[0001222e] 4eb9 0001 28a8            jsr       _opena
[00012234] 588f                      addq.l    #4,a7
[00012236] 4a40                      tst.w     d0
[00012238] 6710                      beq.s     $0001224A
[0001223a] 2e8d                      move.l    a5,(a7)
[0001223c] 5297                      addq.l    #1,(a7)
[0001223e] 2f3c 0001 4b80            move.l    #$00014B80,-(a7)
[00012244] 6100 01d0                 bsr       $00012416
[00012248] 588f                      addq.l    #4,a7
[0001224a] 6000 016c                 bra       $000123B8
[0001224e] 3ebc 0001                 move.w    #$0001,(a7)
[00012252] 4eb9 0001 266c            jsr       _close
[00012258] 0c2d 003e 0001            cmpi.b    #$3E,1(a5)
[0001225e] 6640                      bne.s     $000122A0
[00012260] 3ebc 0001                 move.w    #$0001,(a7)
[00012264] 2f0d                      move.l    a5,-(a7)
[00012266] 5497                      addq.l    #2,(a7)
[00012268] 4eb9 0001 28a8            jsr       _opena
[0001226e] 588f                      addq.l    #4,a7
[00012270] b07c 0001                 cmp.w     #$0001,d0
[00012274] 6618                      bne.s     $0001228E
[00012276] 3ebc 0002                 move.w    #$0002,(a7)
[0001227a] 42a7                      clr.l     -(a7)
[0001227c] 3f3c 0001                 move.w    #$0001,-(a7)
[00012280] 4eb9 0001 1842            jsr       _lseek
[00012286] 5c8f                      addq.l    #6,a7
[00012288] b07c ffff                 cmp.w     #$FFFF,d0
[0001228c] 6610                      bne.s     $0001229E
[0001228e] 2e8d                      move.l    a5,(a7)
[00012290] 5297                      addq.l    #1,(a7)
[00012292] 2f3c 0001 4b8d            move.l    #$00014B8D,-(a7)
[00012298] 6100 017c                 bsr       $00012416
[0001229c] 588f                      addq.l    #4,a7
[0001229e] 6024                      bra.s     $000122C4
[000122a0] 4257                      clr.w     (a7)
[000122a2] 2f0d                      move.l    a5,-(a7)
[000122a4] 5297                      addq.l    #1,(a7)
[000122a6] 4eb9 0001 2598            jsr       _creata
[000122ac] 588f                      addq.l    #4,a7
[000122ae] b07c 0001                 cmp.w     #$0001,d0
[000122b2] 6710                      beq.s     $000122C4
[000122b4] 2e8d                      move.l    a5,(a7)
[000122b6] 5297                      addq.l    #1,(a7)
[000122b8] 2f3c 0001 4b9c            move.l    #$00014B9C,-(a7)
[000122be] 6100 0156                 bsr       $00012416
[000122c2] 588f                      addq.l    #4,a7
[000122c4] 6000 00f2                 bra       $000123B8
[000122c8] 3ebc 003f                 move.w    #$003F,(a7)
[000122cc] 2f0d                      move.l    a5,-(a7)
[000122ce] 4eb9 0001 00e0            jsr       _strchr
[000122d4] 588f                      addq.l    #4,a7
[000122d6] 4a80                      tst.l     d0
[000122d8] 6614                      bne.s     $000122EE
[000122da] 3ebc 002a                 move.w    #$002A,(a7)
[000122de] 2f0d                      move.l    a5,-(a7)
[000122e0] 4eb9 0001 00e0            jsr       _strchr
[000122e6] 588f                      addq.l    #4,a7
[000122e8] 4a80                      tst.l     d0
[000122ea] 6700 00b0                 beq       $0001239C
[000122ee] 2d7c 0001 5858 fffc       move.l    #$00015858,-4(a6)
[000122f6] 2eae fffc                 move.l    -4(a6),(a7)
[000122fa] 0697 0000 0038            addi.l    #$00000038,(a7)
[00012300] 3f3c 001a                 move.w    #$001A,-(a7)
[00012304] 4eb9 0001 0096            jsr       ___BDOS
[0001230a] 548f                      addq.l    #2,a7
[0001230c] 3ebc 0011                 move.w    #$0011,(a7)
[00012310] 2f0d                      move.l    a5,-(a7)
[00012312] 3f3c 0003                 move.w    #$0003,-(a7)
[00012316] 4eb9 0001 381c            jsr       ___open
[0001231c] 5c8f                      addq.l    #6,a7
[0001231e] 1c00                      move.b    d0,d6
[00012320] bc3c 00ff                 cmp.b     #$FF,d6
[00012324] 660e                      bne.s     $00012334
[00012326] 2ebc 0001 4bab            move.l    #$00014BAB,(a7)
[0001232c] 2f0d                      move.l    a5,-(a7)
[0001232e] 6100 00e6                 bsr       $00012416
[00012332] 588f                      addq.l    #4,a7
[00012334] 605e                      bra.s     $00012394
[00012336] 2e8e                      move.l    a6,(a7)
[00012338] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[0001233e] 1006                      move.b    d6,d0
[00012340] 4880                      ext.w     d0
[00012342] 3f00                      move.w    d0,-(a7)
[00012344] 2f2e fffc                 move.l    -4(a6),-(a7)
[00012348] 6100 015a                 bsr       $000124A4
[0001234c] 5c8f                      addq.l    #6,a7
[0001234e] 2e8e                      move.l    a6,(a7)
[00012350] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[00012356] 4eb9 0001 39ee            jsr       _strlen
[0001235c] 3e80                      move.w    d0,(a7)
[0001235e] 5257                      addq.w    #1,(a7)
[00012360] 4eb9 0001 3a8e            jsr       __salloc
[00012366] 2840                      movea.l   d0,a4
[00012368] 2e8e                      move.l    a6,(a7)
[0001236a] 0697 ffff ffde            addi.l    #$FFFFFFDE,(a7)
[00012370] 2f0c                      move.l    a4,-(a7)
[00012372] 4eb9 0001 39cc            jsr       _strcpy
[00012378] 588f                      addq.l    #4,a7
[0001237a] 2e8c                      move.l    a4,(a7)
[0001237c] 6100 00fc                 bsr       $0001247A
[00012380] 3ebc 0012                 move.w    #$0012,(a7)
[00012384] 2f0d                      move.l    a5,-(a7)
[00012386] 3f3c 0003                 move.w    #$0003,-(a7)
[0001238a] 4eb9 0001 381c            jsr       ___open
[00012390] 5c8f                      addq.l    #6,a7
[00012392] 1c00                      move.b    d0,d6
[00012394] bc3c 00ff                 cmp.b     #$FF,d6
[00012398] 669c                      bne.s     $00012336
[0001239a] 6006                      bra.s     $000123A2
[0001239c] 2e8d                      move.l    a5,(a7)
[0001239e] 6100 00da                 bsr       $0001247A
[000123a2] 6014                      bra.s     $000123B8
[000123a4] b07c 003c                 cmp.w     #$003C,d0
[000123a8] 6700 fe76                 beq       $00012220
[000123ac] b07c 003e                 cmp.w     #$003E,d0
[000123b0] 6700 fe9c                 beq       $0001224E
[000123b4] 6000 ff12                 bra       $000122C8
[000123b8] dac7                      adda.w    d7,a5
[000123ba] 4a15                      tst.b     (a5)
[000123bc] 6600 fdb0                 bne       $0001216E
[000123c0] 4297                      clr.l     (a7)
[000123c2] 6100 00b6                 bsr       $0001247A
[000123c6] 5379 0001 4ec0            subq.w    #1,$00014EC0
[000123cc] 2eb9 0001 4ec6            move.l    $00014EC6,(a7)
[000123d2] 4eb9 0001 007a            jsr       _brk
[000123d8] b07c ffff                 cmp.w     #$FFFF,d0
[000123dc] 6610                      bne.s     $000123EE
[000123de] 2ebc 0001 4bc5            move.l    #$00014BC5,(a7)
[000123e4] 2f3c 0001 4bb6            move.l    #$00014BB6,-(a7)
[000123ea] 612a                      bsr.s     $00012416
[000123ec] 588f                      addq.l    #4,a7
[000123ee] 4297                      clr.l     (a7)
[000123f0] 2f39 0001 4ec2            move.l    $00014EC2,-(a7)
[000123f6] 3f39 0001 4ec0            move.w    $00014EC0,-(a7)
[000123fc] 4eb9 0001 00fa            jsr       _main
[00012402] 5c8f                      addq.l    #6,a7
[00012404] 3e80                      move.w    d0,(a7)
[00012406] 4eb9 0001 25ca            jsr       _exit
[0001240c] 4a9f                      tst.l     (a7)+
[0001240e] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00012412] 4e5e                      unlk      a6
[00012414] 4e75                      rts
[00012416] 4e56 ff7c                 link      a6,#$FF7C
[0001241a] 2eae 0008                 move.l    8(a6),(a7)
[0001241e] 2f0e                      move.l    a6,-(a7)
[00012420] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[00012426] 4eb9 0001 39cc            jsr       _strcpy
[0001242c] 588f                      addq.l    #4,a7
[0001242e] 2eae 000c                 move.l    12(a6),(a7)
[00012432] 2f0e                      move.l    a6,-(a7)
[00012434] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[0001243a] 4eb9 0001 3960            jsr       _strcat
[00012440] 588f                      addq.l    #4,a7
[00012442] 2ebc 0001 4bc6            move.l    #$00014BC6,(a7)
[00012448] 2f0e                      move.l    a6,-(a7)
[0001244a] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[00012450] 4eb9 0001 3960            jsr       _strcat
[00012456] 588f                      addq.l    #4,a7
[00012458] 2e8e                      move.l    a6,(a7)
[0001245a] 0697 ffff ff80            addi.l    #$FFFFFF80,(a7)
[00012460] 3f3c 0009                 move.w    #$0009,-(a7)
[00012464] 4eb9 0001 0096            jsr       ___BDOS
[0001246a] 548f                      addq.l    #2,a7
[0001246c] 3ebc ffff                 move.w    #$FFFF,(a7)
[00012470] 4eb9 0001 25ca            jsr       _exit
[00012476] 4e5e                      unlk      a6
[00012478] 4e75                      rts
[0001247a] 4e56 0000                 link      a6,#$0000
[0001247e] 48e7 0104                 movem.l   d7/a5,-(a7)
[00012482] 2a6e 0008                 movea.l   8(a6),a5
[00012486] 2079 0001 4ec6            movea.l   $00014EC6,a0
[0001248c] 208d                      move.l    a5,(a0)
[0001248e] 58b9 0001 4ec6            addq.l    #4,$00014EC6
[00012494] 5279 0001 4ec0            addq.w    #1,$00014EC0
[0001249a] 4a9f                      tst.l     (a7)+
[0001249c] 4cdf 2000                 movem.l   (a7)+,a5
[000124a0] 4e5e                      unlk      a6
[000124a2] 4e75                      rts
[000124a4] 4e56 0000                 link      a6,#$0000
[000124a8] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[000124ac] 2a6e 0008                 movea.l   8(a6),a5
[000124b0] 1e2e 000d                 move.b    13(a6),d7
[000124b4] 286e 000e                 movea.l   14(a6),a4
[000124b8] 47ed 0056                 lea.l     86(a5),a3
[000124bc] 18db                      move.b    (a3)+,(a4)+
[000124be] 66fc                      bne.s     $000124BC
[000124c0] 4a9f                      tst.l     (a7)+
[000124c2] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[000124c6] 4e5e                      unlk      a6
[000124c8] 4e75                      rts
__creat:
[000124ca] 4e56 0000                 link      a6,#$0000
[000124ce] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000124d2] 4eb9 0001 18bc            jsr       __allocc
[000124d8] 3e00                      move.w    d0,d7
[000124da] be7c ffff                 cmp.w     #$FFFF,d7
[000124de] 6606                      bne.s     $000124E6
[000124e0] 70ff                      moveq.l   #-1,d0
[000124e2] 6000 0092                 bra       $00012576
[000124e6] 3e87                      move.w    d7,(a7)
[000124e8] 4eb9 0001 1944            jsr       ___chini
[000124ee] 3007                      move.w    d7,d0
[000124f0] c1fc 0238                 muls.w    #$0238,d0
[000124f4] 2a40                      movea.l   d0,a5
[000124f6] dbfc 0001 51b0            adda.l    #$000151B0,a5
[000124fc] 4a6e 000e                 tst.w     14(a6)
[00012500] 6604                      bne.s     $00012506
[00012502] 0055 0010                 ori.w     #$0010,(a5)
[00012506] 2ebc 0001 4771            move.l    #$00014771,(a7)
[0001250c] 2f2e 0008                 move.l    8(a6),-(a7)
[00012510] 4eb9 0001 3a1c            jsr       __strcmp
[00012516] 588f                      addq.l    #4,a7
[00012518] 4a40                      tst.w     d0
[0001251a] 660a                      bne.s     $00012526
[0001251c] 0055 0003                 ori.w     #$0003,(a5)
[00012520] 3007                      move.w    d7,d0
[00012522] 6052                      bra.s     $00012576
[00012524] 601e                      bra.s     $00012544
[00012526] 2ebc 0001 4776            move.l    #$00014776,(a7)
[0001252c] 2f2e 0008                 move.l    8(a6),-(a7)
[00012530] 4eb9 0001 3a1c            jsr       __strcmp
[00012536] 588f                      addq.l    #4,a7
[00012538] 4a40                      tst.w     d0
[0001253a] 6608                      bne.s     $00012544
[0001253c] 0055 0005                 ori.w     #$0005,(a5)
[00012540] 3007                      move.w    d7,d0
[00012542] 6032                      bra.s     $00012576
[00012544] 3ebc 0016                 move.w    #$0016,(a7)
[00012548] 2f2e 0008                 move.l    8(a6),-(a7)
[0001254c] 3f07                      move.w    d7,-(a7)
[0001254e] 4eb9 0001 381c            jsr       ___open
[00012554] 5c8f                      addq.l    #6,a7
[00012556] 4a40                      tst.w     d0
[00012558] 6716                      beq.s     $00012570
[0001255a] 33fc 0023 0001 4db0       move.w    #$0023,$00014DB0
[00012562] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[0001256c] 70ff                      moveq.l   #-1,d0
[0001256e] 6006                      bra.s     $00012576
[00012570] 0055 0001                 ori.w     #$0001,(a5)
[00012574] 3007                      move.w    d7,d0
[00012576] 4a9f                      tst.l     (a7)+
[00012578] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0001257c] 4e5e                      unlk      a6
[0001257e] 4e75                      rts
_creat:
[00012580] 4e56 fffc                 link      a6,#$FFFC
[00012584] 4257                      clr.w     (a7)
[00012586] 3f2e 000c                 move.w    12(a6),-(a7)
[0001258a] 2f2e 0008                 move.l    8(a6),-(a7)
[0001258e] 6100 ff3a                 bsr       __creat
[00012592] 5c8f                      addq.l    #6,a7
[00012594] 4e5e                      unlk      a6
[00012596] 4e75                      rts
_creata:
[00012598] 4e56 fffc                 link      a6,#$FFFC
[0001259c] 4257                      clr.w     (a7)
[0001259e] 3f2e 000c                 move.w    12(a6),-(a7)
[000125a2] 2f2e 0008                 move.l    8(a6),-(a7)
[000125a6] 6100 ff22                 bsr       __creat
[000125aa] 5c8f                      addq.l    #6,a7
[000125ac] 4e5e                      unlk      a6
[000125ae] 4e75                      rts
_creatb:
[000125b0] 4e56 fffc                 link      a6,#$FFFC
[000125b4] 3ebc 0001                 move.w    #$0001,(a7)
[000125b8] 3f2e 000c                 move.w    12(a6),-(a7)
[000125bc] 2f2e 0008                 move.l    8(a6),-(a7)
[000125c0] 6100 ff08                 bsr       __creat
[000125c4] 5c8f                      addq.l    #6,a7
[000125c6] 4e5e                      unlk      a6
[000125c8] 4e75                      rts
_exit:
[000125ca] 4e56 fffc                 link      a6,#$FFFC
[000125ce] 4eb9 0001 25e2            jsr       __cleanu
[000125d4] 3eae 0008                 move.w    8(a6),(a7)
[000125d8] 4eb9 0001 00ba            jsr       __exit
[000125de] 4e5e                      unlk      a6
[000125e0] 4e75                      rts
__cleanu:
[000125e2] 4e56 0000                 link      a6,#$0000
[000125e6] 48e7 0300                 movem.l   d6-d7,-(a7)
[000125ea] 4247                      clr.w     d7
[000125ec] 6016                      bra.s     $00012604
[000125ee] 3007                      move.w    d7,d0
[000125f0] c1fc 000e                 muls.w    #$000E,d0
[000125f4] d0bc 0001 4bca            add.l     #$00014BCA,d0
[000125fa] 2e80                      move.l    d0,(a7)
[000125fc] 4eb9 0001 2614            jsr       _fclose
[00012602] 5247                      addq.w    #1,d7
[00012604] be7c 0010                 cmp.w     #$0010,d7
[00012608] 6de4                      blt.s     $000125EE
[0001260a] 4a9f                      tst.l     (a7)+
[0001260c] 4cdf 0080                 movem.l   (a7)+,d7
[00012610] 4e5e                      unlk      a6
[00012612] 4e75                      rts
_fclose:
[00012614] 4e56 0000                 link      a6,#$0000
[00012618] 48e7 0104                 movem.l   d7/a5,-(a7)
[0001261c] 2a6e 0008                 movea.l   8(a6),a5
[00012620] 302d 0002                 move.w    2(a5),d0
[00012624] c07c 0003                 and.w     #$0003,d0
[00012628] 672a                      beq.s     $00012654
[0001262a] 2e8d                      move.l    a5,(a7)
[0001262c] 4eb9 0001 26fc            jsr       _fflush
[00012632] 082d 0002 0003            btst      #2,3(a5)
[00012638] 670a                      beq.s     $00012644
[0001263a] 2ead 0004                 move.l    4(a5),(a7)
[0001263e] 4eb9 0001 3286            jsr       _free
[00012644] 4240                      clr.w     d0
[00012646] 48c0                      ext.l     d0
[00012648] 2b40 0008                 move.l    d0,8(a5)
[0001264c] 2b40 0004                 move.l    d0,4(a5)
[00012650] 426d 000c                 clr.w     12(a5)
[00012654] 026d ff80 0002            andi.w    #$FF80,2(a5)
[0001265a] 3e95                      move.w    (a5),(a7)
[0001265c] 4eb9 0001 266c            jsr       _close
[00012662] 4a9f                      tst.l     (a7)+
[00012664] 4cdf 2000                 movem.l   (a7)+,a5
[00012668] 4e5e                      unlk      a6
[0001266a] 4e75                      rts
_close:
[0001266c] 4e56 0000                 link      a6,#$0000
[00012670] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00012674] 3e2e 0008                 move.w    8(a6),d7
[00012678] 3e87                      move.w    d7,(a7)
[0001267a] 4eb9 0001 19b2            jsr       __chkc
[00012680] 2a40                      movea.l   d0,a5
[00012682] 200d                      move.l    a5,d0
[00012684] 6616                      bne.s     $0001269C
[00012686] 33fc 0009 0001 4db0       move.w    #$0009,$00014DB0
[0001268e] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[00012698] 70ff                      moveq.l   #-1,d0
[0001269a] 604e                      bra.s     $000126EA
[0001269c] 4246                      clr.w     d6
[0001269e] 3015                      move.w    (a5),d0
[000126a0] c07c 0006                 and.w     #$0006,d0
[000126a4] 6616                      bne.s     $000126BC
[000126a6] 3ead 0004                 move.w    4(a5),(a7)
[000126aa] 3f3c 003e                 move.w    #$003E,-(a7)
[000126ae] 4eb9 0001 3d54            jsr       _trap
[000126b4] 548f                      addq.l    #2,a7
[000126b6] 3c00                      move.w    d0,d6
[000126b8] 6c02                      bge.s     $000126BC
[000126ba] 7cff                      moveq.l   #-1,d6
[000126bc] 3e87                      move.w    d7,(a7)
[000126be] 4eb9 0001 1944            jsr       ___chini
[000126c4] 3e87                      move.w    d7,(a7)
[000126c6] 4eb9 0001 1906            jsr       __freec
[000126cc] 4a46                      tst.w     d6
[000126ce] 6606                      bne.s     $000126D6
[000126d0] 3006                      move.w    d6,d0
[000126d2] 6016                      bra.s     $000126EA
[000126d4] 6014                      bra.s     $000126EA
[000126d6] 33fc 0005 0001 4db0       move.w    #$0005,$00014DB0
[000126de] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[000126e8] 70ff                      moveq.l   #-1,d0
[000126ea] 4a9f                      tst.l     (a7)+
[000126ec] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[000126f0] 4e5e                      unlk      a6
[000126f2] 4e75                      rts
[000126f4] 4e56 fffc                 link      a6,#$FFFC
[000126f8] 4e5e                      unlk      a6
[000126fa] 4e75                      rts
_fflush:
[000126fc] 4e56 0000                 link      a6,#$0000
[00012700] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00012704] 2a6e 0008                 movea.l   8(a6),a5
[00012708] 302d 0002                 move.w    2(a5),d0
[0001270c] c07c 000a                 and.w     #$000A,d0
[00012710] b07c 0002                 cmp.w     #$0002,d0
[00012714] 662c                      bne.s     $00012742
[00012716] 202d 0008                 move.l    8(a5),d0
[0001271a] 90ad 0004                 sub.l     4(a5),d0
[0001271e] 3c00                      move.w    d0,d6
[00012720] 6f20                      ble.s     $00012742
[00012722] 3e86                      move.w    d6,(a7)
[00012724] 2f2d 0004                 move.l    4(a5),-(a7)
[00012728] 3f15                      move.w    (a5),-(a7)
[0001272a] 4eb9 0001 3614            jsr       _write
[00012730] 5c8f                      addq.l    #6,a7
[00012732] 3e00                      move.w    d0,d7
[00012734] bc47                      cmp.w     d7,d6
[00012736] 670a                      beq.s     $00012742
[00012738] 006d 0010 0002            ori.w     #$0010,2(a5)
[0001273e] 70ff                      moveq.l   #-1,d0
[00012740] 604a                      bra.s     $0001278C
[00012742] 082d 0001 0003            btst      #1,3(a5)
[00012748] 671e                      beq.s     $00012768
[0001274a] 4aad 0004                 tst.l     4(a5)
[0001274e] 6716                      beq.s     $00012766
[00012750] 082d 0003 0003            btst      #3,3(a5)
[00012756] 6708                      beq.s     $00012760
[00012758] 3b7c 0001 000c            move.w    #$0001,12(a5)
[0001275e] 6006                      bra.s     $00012766
[00012760] 3b7c 01ff 000c            move.w    #$01FF,12(a5)
[00012766] 601c                      bra.s     $00012784
[00012768] 3ebc 0001                 move.w    #$0001,(a7)
[0001276c] 302d 000c                 move.w    12(a5),d0
[00012770] 4440                      neg.w     d0
[00012772] 48c0                      ext.l     d0
[00012774] 2f00                      move.l    d0,-(a7)
[00012776] 3f15                      move.w    (a5),-(a7)
[00012778] 4eb9 0001 1842            jsr       _lseek
[0001277e] 5c8f                      addq.l    #6,a7
[00012780] 426d 000c                 clr.w     12(a5)
[00012784] 2b6d 0004 0008            move.l    4(a5),8(a5)
[0001278a] 4240                      clr.w     d0
[0001278c] 4a9f                      tst.l     (a7)+
[0001278e] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00012792] 4e5e                      unlk      a6
[00012794] 4e75                      rts
__open:
[00012796] 4e56 0000                 link      a6,#$0000
[0001279a] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[0001279e] 4eb9 0001 18bc            jsr       __allocc
[000127a4] 3e00                      move.w    d0,d7
[000127a6] be7c ffff                 cmp.w     #$FFFF,d7
[000127aa] 6606                      bne.s     $000127B2
[000127ac] 70ff                      moveq.l   #-1,d0
[000127ae] 6000 00d6                 bra       $00012886
[000127b2] 3e87                      move.w    d7,(a7)
[000127b4] 4eb9 0001 1944            jsr       ___chini
[000127ba] 3007                      move.w    d7,d0
[000127bc] c1fc 0238                 muls.w    #$0238,d0
[000127c0] 2a40                      movea.l   d0,a5
[000127c2] dbfc 0001 51b0            adda.l    #$000151B0,a5
[000127c8] 4a6e 000c                 tst.w     12(a6)
[000127cc] 6604                      bne.s     $000127D2
[000127ce] 0055 0008                 ori.w     #$0008,(a5)
[000127d2] 4a6e 000e                 tst.w     14(a6)
[000127d6] 6604                      bne.s     $000127DC
[000127d8] 0055 0010                 ori.w     #$0010,(a5)
[000127dc] 2ebc 0001 4771            move.l    #$00014771,(a7)
[000127e2] 2f2e 0008                 move.l    8(a6),-(a7)
[000127e6] 4eb9 0001 3a1c            jsr       __strcmp
[000127ec] 588f                      addq.l    #4,a7
[000127ee] 4a40                      tst.w     d0
[000127f0] 6612                      bne.s     $00012804
[000127f2] 0055 0003                 ori.w     #$0003,(a5)
[000127f6] 3b6e 000c 0004            move.w    12(a6),4(a5)
[000127fc] 3007                      move.w    d7,d0
[000127fe] 6000 0086                 bra       $00012886
[00012802] 601e                      bra.s     $00012822
[00012804] 2ebc 0001 4776            move.l    #$00014776,(a7)
[0001280a] 2f2e 0008                 move.l    8(a6),-(a7)
[0001280e] 4eb9 0001 3a1c            jsr       __strcmp
[00012814] 588f                      addq.l    #4,a7
[00012816] 4a40                      tst.w     d0
[00012818] 6608                      bne.s     $00012822
[0001281a] 0055 0005                 ori.w     #$0005,(a5)
[0001281e] 3007                      move.w    d7,d0
[00012820] 6064                      bra.s     $00012886
[00012822] 3ebc 000f                 move.w    #$000F,(a7)
[00012826] 2f2e 0008                 move.l    8(a6),-(a7)
[0001282a] 3f07                      move.w    d7,-(a7)
[0001282c] 4eb9 0001 381c            jsr       ___open
[00012832] 5c8f                      addq.l    #6,a7
[00012834] 4a40                      tst.w     d0
[00012836] 671e                      beq.s     $00012856
[00012838] 3e87                      move.w    d7,(a7)
[0001283a] 4eb9 0001 1906            jsr       __freec
[00012840] 33fc 0002 0001 4db0       move.w    #$0002,$00014DB0
[00012848] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[00012852] 70ff                      moveq.l   #-1,d0
[00012854] 6030                      bra.s     $00012886
[00012856] 0055 0001                 ori.w     #$0001,(a5)
[0001285a] 3ebc 0002                 move.w    #$0002,(a7)
[0001285e] 42a7                      clr.l     -(a7)
[00012860] 102d 0003                 move.b    3(a5),d0
[00012864] 4880                      ext.w     d0
[00012866] 3f00                      move.w    d0,-(a7)
[00012868] 4eb9 0001 1842            jsr       _lseek
[0001286e] 5c8f                      addq.l    #6,a7
[00012870] 4257                      clr.w     (a7)
[00012872] 42a7                      clr.l     -(a7)
[00012874] 102d 0003                 move.b    3(a5),d0
[00012878] 4880                      ext.w     d0
[0001287a] 3f00                      move.w    d0,-(a7)
[0001287c] 4eb9 0001 1842            jsr       _lseek
[00012882] 5c8f                      addq.l    #6,a7
[00012884] 3007                      move.w    d7,d0
[00012886] 4a9f                      tst.l     (a7)+
[00012888] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0001288c] 4e5e                      unlk      a6
[0001288e] 4e75                      rts
_open:
[00012890] 4e56 fffc                 link      a6,#$FFFC
[00012894] 4257                      clr.w     (a7)
[00012896] 3f2e 000c                 move.w    12(a6),-(a7)
[0001289a] 2f2e 0008                 move.l    8(a6),-(a7)
[0001289e] 6100 fef6                 bsr       __open
[000128a2] 5c8f                      addq.l    #6,a7
[000128a4] 4e5e                      unlk      a6
[000128a6] 4e75                      rts
_opena:
[000128a8] 4e56 fffc                 link      a6,#$FFFC
[000128ac] 4257                      clr.w     (a7)
[000128ae] 3f2e 000c                 move.w    12(a6),-(a7)
[000128b2] 2f2e 0008                 move.l    8(a6),-(a7)
[000128b6] 6100 fede                 bsr       __open
[000128ba] 5c8f                      addq.l    #6,a7
[000128bc] 4e5e                      unlk      a6
[000128be] 4e75                      rts
_openb:
[000128c0] 4e56 fffc                 link      a6,#$FFFC
[000128c4] 3ebc 0001                 move.w    #$0001,(a7)
[000128c8] 3f2e 000c                 move.w    12(a6),-(a7)
[000128cc] 2f2e 0008                 move.l    8(a6),-(a7)
[000128d0] 6100 fec4                 bsr       __open
[000128d4] 5c8f                      addq.l    #6,a7
[000128d6] 4e5e                      unlk      a6
[000128d8] 4e75                      rts
_sprintf:
[000128da] 4e56 fff2                 link      a6,#$FFF2
[000128de] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[000128e2] 4bee fff2                 lea.l     -14(a6),a5
[000128e6] 3b7c 7fff 000c            move.w    #$7FFF,12(a5)
[000128ec] 202e 0008                 move.l    8(a6),d0
[000128f0] 2b40 0004                 move.l    d0,4(a5)
[000128f4] 2b40 0008                 move.l    d0,8(a5)
[000128f8] 3b7c 0082 0002            move.w    #$0082,2(a5)
[000128fe] 3abc ffff                 move.w    #$FFFF,(a5)
[00012902] 2e8e                      move.l    a6,(a7)
[00012904] 0697 0000 0010            addi.l    #$00000010,(a7)
[0001290a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001290e] 2f0d                      move.l    a5,-(a7)
[00012910] 4eb9 0001 2932            jsr       __doprt
[00012916] 508f                      addq.l    #8,a7
[00012918] 3e00                      move.w    d0,d7
[0001291a] 2e8d                      move.l    a5,(a7)
[0001291c] 4267                      clr.w     -(a7)
[0001291e] 4eb9 0001 2ee8            jsr       _fputc
[00012924] 548f                      addq.l    #2,a7
[00012926] 3007                      move.w    d7,d0
[00012928] 4a9f                      tst.l     (a7)+
[0001292a] 4cdf 2080                 movem.l   (a7)+,d7/a5
[0001292e] 4e5e                      unlk      a6
[00012930] 4e75                      rts
__doprt:
[00012932] 4e56 fde4                 link      a6,#$FDE4
[00012936] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[0001293a] 2a6e 0010                 movea.l   16(a6),a5
[0001293e] 426e fdec                 clr.w     -532(a6)
[00012942] 4aae 000c                 tst.l     12(a6)
[00012946] 6700 0468                 beq       $00012DB0
[0001294a] 6000 0460                 bra       $00012DAC
[0001294e] 4245                      clr.w     d5
[00012950] 2d6e 000c fff2            move.l    12(a6),-14(a6)
[00012956] 6006                      bra.s     $0001295E
[00012958] 52ae fff2                 addq.l    #1,-14(a6)
[0001295c] 5245                      addq.w    #1,d5
[0001295e] 206e fff2                 movea.l   -14(a6),a0
[00012962] 4a10                      tst.b     (a0)
[00012964] 670a                      beq.s     $00012970
[00012966] 206e fff2                 movea.l   -14(a6),a0
[0001296a] 0c10 0025                 cmpi.b    #$25,(a0)
[0001296e] 66e8                      bne.s     $00012958
[00012970] 4a45                      tst.w     d5
[00012972] 6f1c                      ble.s     $00012990
[00012974] 2eae 0008                 move.l    8(a6),(a7)
[00012978] 3f05                      move.w    d5,-(a7)
[0001297a] 2f2e 000c                 move.l    12(a6),-(a7)
[0001297e] 4eb9 0001 2e78            jsr       _fputn
[00012984] 5c8f                      addq.l    #6,a7
[00012986] 2d6e fff2 000c            move.l    -14(a6),12(a6)
[0001298c] db6e fdec                 add.w     d5,-532(a6)
[00012990] 206e 000c                 movea.l   12(a6),a0
[00012994] 0c10 0025                 cmpi.b    #$25,(a0)
[00012998] 40c0                      move.w    sr,d0
[0001299a] 52ae 000c                 addq.l    #1,12(a6)
[0001299e] 44c0                      move.b    d0,ccr
[000129a0] 6600 040e                 bne       $00012DB0
[000129a4] 426e fdea                 clr.w     -534(a6)
[000129a8] 206e 000c                 movea.l   12(a6),a0
[000129ac] 1010                      move.b    (a0),d0
[000129ae] 4880                      ext.w     d0
[000129b0] 1e00                      move.b    d0,d7
[000129b2] b07c 002d                 cmp.w     #$002D,d0
[000129b6] 40c0                      move.w    sr,d0
[000129b8] 52ae 000c                 addq.l    #1,12(a6)
[000129bc] 44c0                      move.b    d0,ccr
[000129be] 660e                      bne.s     $000129CE
[000129c0] 206e 000c                 movea.l   12(a6),a0
[000129c4] 1e10                      move.b    (a0),d7
[000129c6] 52ae 000c                 addq.l    #1,12(a6)
[000129ca] 526e fdea                 addq.w    #1,-534(a6)
[000129ce] 1d7c 0020 fff6            move.b    #$20,-10(a6)
[000129d4] be3c 0030                 cmp.b     #$30,d7
[000129d8] 660e                      bne.s     $000129E8
[000129da] 1d47 fff6                 move.b    d7,-10(a6)
[000129de] 206e 000c                 movea.l   12(a6),a0
[000129e2] 1e10                      move.b    (a0),d7
[000129e4] 52ae 000c                 addq.l    #1,12(a6)
[000129e8] 3d7c ffff fdf0            move.w    #$FFFF,-528(a6)
[000129ee] be3c 002a                 cmp.b     #$2A,d7
[000129f2] 661a                      bne.s     $00012A0E
[000129f4] 2d4d fff8                 move.l    a5,-8(a6)
[000129f8] 206e fff8                 movea.l   -8(a6),a0
[000129fc] 3d50 fdf0                 move.w    (a0),-528(a6)
[00012a00] 548d                      addq.l    #2,a5
[00012a02] 206e 000c                 movea.l   12(a6),a0
[00012a06] 1e10                      move.b    (a0),d7
[00012a08] 52ae 000c                 addq.l    #1,12(a6)
[00012a0c] 6038                      bra.s     $00012A46
[00012a0e] 602a                      bra.s     $00012A3A
[00012a10] 4a6e fdf0                 tst.w     -528(a6)
[00012a14] 6c04                      bge.s     $00012A1A
[00012a16] 426e fdf0                 clr.w     -528(a6)
[00012a1a] 1007                      move.b    d7,d0
[00012a1c] 4880                      ext.w     d0
[00012a1e] 322e fdf0                 move.w    -528(a6),d1
[00012a22] c3fc 000a                 muls.w    #$000A,d1
[00012a26] d041                      add.w     d1,d0
[00012a28] d07c ffd0                 add.w     #$FFD0,d0
[00012a2c] 3d40 fdf0                 move.w    d0,-528(a6)
[00012a30] 206e 000c                 movea.l   12(a6),a0
[00012a34] 1e10                      move.b    (a0),d7
[00012a36] 52ae 000c                 addq.l    #1,12(a6)
[00012a3a] be3c 0030                 cmp.b     #$30,d7
[00012a3e] 6d06                      blt.s     $00012A46
[00012a40] be3c 0039                 cmp.b     #$39,d7
[00012a44] 6fca                      ble.s     $00012A10
[00012a46] 7cff                      moveq.l   #-1,d6
[00012a48] be3c 002e                 cmp.b     #$2E,d7
[00012a4c] 660c                      bne.s     $00012A5A
[00012a4e] 4246                      clr.w     d6
[00012a50] 206e 000c                 movea.l   12(a6),a0
[00012a54] 1e10                      move.b    (a0),d7
[00012a56] 52ae 000c                 addq.l    #1,12(a6)
[00012a5a] be3c 002a                 cmp.b     #$2A,d7
[00012a5e] 6618                      bne.s     $00012A78
[00012a60] 2d4d fff8                 move.l    a5,-8(a6)
[00012a64] 206e fff8                 movea.l   -8(a6),a0
[00012a68] 3c10                      move.w    (a0),d6
[00012a6a] 548d                      addq.l    #2,a5
[00012a6c] 206e 000c                 movea.l   12(a6),a0
[00012a70] 1e10                      move.b    (a0),d7
[00012a72] 52ae 000c                 addq.l    #1,12(a6)
[00012a76] 602a                      bra.s     $00012AA2
[00012a78] 601c                      bra.s     $00012A96
[00012a7a] 1007                      move.b    d7,d0
[00012a7c] 4880                      ext.w     d0
[00012a7e] 3206                      move.w    d6,d1
[00012a80] c3fc 000a                 muls.w    #$000A,d1
[00012a84] d041                      add.w     d1,d0
[00012a86] 3c00                      move.w    d0,d6
[00012a88] dc7c ffd0                 add.w     #$FFD0,d6
[00012a8c] 206e 000c                 movea.l   12(a6),a0
[00012a90] 1e10                      move.b    (a0),d7
[00012a92] 52ae 000c                 addq.l    #1,12(a6)
[00012a96] be3c 0030                 cmp.b     #$30,d7
[00012a9a] 6d06                      blt.s     $00012AA2
[00012a9c] be3c 0039                 cmp.b     #$39,d7
[00012aa0] 6fd8                      ble.s     $00012A7A
[00012aa2] 426e fde8                 clr.w     -536(a6)
[00012aa6] be3c 006c                 cmp.b     #$6C,d7
[00012aaa] 6706                      beq.s     $00012AB2
[00012aac] be3c 004c                 cmp.b     #$4C,d7
[00012ab0] 660e                      bne.s     $00012AC0
[00012ab2] 526e fde8                 addq.w    #1,-536(a6)
[00012ab6] 206e 000c                 movea.l   12(a6),a0
[00012aba] 1e10                      move.b    (a0),d7
[00012abc] 52ae 000c                 addq.l    #1,12(a6)
[00012ac0] 41ee fdf2                 lea.l     -526(a6),a0
[00012ac4] 2d48 fff2                 move.l    a0,-14(a6)
[00012ac8] 1007                      move.b    d7,d0
[00012aca] 4880                      ext.w     d0
[00012acc] 6000 020c                 bra       $00012CDA
[00012ad0] 526e fde8                 addq.w    #1,-536(a6)
[00012ad4] 4a6e fde8                 tst.w     -536(a6)
[00012ad8] 6708                      beq.s     $00012AE2
[00012ada] 203c 0001 3486            move.l    #$00013486,d0
[00012ae0] 6006                      bra.s     $00012AE8
[00012ae2] 203c 0001 353c            move.l    #$0001353C,d0
[00012ae8] 23c0 0001 4eca            move.l    d0,$00014ECA
[00012aee] 2eb9 0001 4eca            move.l    $00014ECA,(a7)
[00012af4] 3f3c 0001                 move.w    #$0001,-(a7)
[00012af8] 3f3c 000a                 move.w    #$000A,-(a7)
[00012afc] 2f0e                      move.l    a6,-(a7)
[00012afe] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012b04] 2f0d                      move.l    a5,-(a7)
[00012b06] 4eb9 0001 3414            jsr       $00013414
[00012b0c] dffc 0000 000c            adda.l    #$0000000C,a7
[00012b12] 4a6e fde8                 tst.w     -536(a6)
[00012b16] 6704                      beq.s     $00012B1C
[00012b18] 7004                      moveq.l   #4,d0
[00012b1a] 6002                      bra.s     $00012B1E
[00012b1c] 7002                      moveq.l   #2,d0
[00012b1e] 48c0                      ext.l     d0
[00012b20] dbc0                      adda.l    d0,a5
[00012b22] 6000 01ce                 bra       $00012CF2
[00012b26] 526e fde8                 addq.w    #1,-536(a6)
[00012b2a] 4a6e fde8                 tst.w     -536(a6)
[00012b2e] 6708                      beq.s     $00012B38
[00012b30] 203c 0001 3486            move.l    #$00013486,d0
[00012b36] 6006                      bra.s     $00012B3E
[00012b38] 203c 0001 353c            move.l    #$0001353C,d0
[00012b3e] 23c0 0001 4eca            move.l    d0,$00014ECA
[00012b44] 2eb9 0001 4eca            move.l    $00014ECA,(a7)
[00012b4a] 4267                      clr.w     -(a7)
[00012b4c] 3f3c 000a                 move.w    #$000A,-(a7)
[00012b50] 2f0e                      move.l    a6,-(a7)
[00012b52] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012b58] 2f0d                      move.l    a5,-(a7)
[00012b5a] 4eb9 0001 3414            jsr       $00013414
[00012b60] dffc 0000 000c            adda.l    #$0000000C,a7
[00012b66] 4a6e fde8                 tst.w     -536(a6)
[00012b6a] 6704                      beq.s     $00012B70
[00012b6c] 7004                      moveq.l   #4,d0
[00012b6e] 6002                      bra.s     $00012B72
[00012b70] 7002                      moveq.l   #2,d0
[00012b72] 48c0                      ext.l     d0
[00012b74] dbc0                      adda.l    d0,a5
[00012b76] 6000 017a                 bra       $00012CF2
[00012b7a] 526e fde8                 addq.w    #1,-536(a6)
[00012b7e] 4a6e fde8                 tst.w     -536(a6)
[00012b82] 6708                      beq.s     $00012B8C
[00012b84] 203c 0001 3486            move.l    #$00013486,d0
[00012b8a] 6006                      bra.s     $00012B92
[00012b8c] 203c 0001 353c            move.l    #$0001353C,d0
[00012b92] 23c0 0001 4eca            move.l    d0,$00014ECA
[00012b98] 2eb9 0001 4eca            move.l    $00014ECA,(a7)
[00012b9e] 4267                      clr.w     -(a7)
[00012ba0] 3f3c 0008                 move.w    #$0008,-(a7)
[00012ba4] 2f0e                      move.l    a6,-(a7)
[00012ba6] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012bac] 2f0d                      move.l    a5,-(a7)
[00012bae] 4eb9 0001 3414            jsr       $00013414
[00012bb4] dffc 0000 000c            adda.l    #$0000000C,a7
[00012bba] 4a6e fde8                 tst.w     -536(a6)
[00012bbe] 6704                      beq.s     $00012BC4
[00012bc0] 7004                      moveq.l   #4,d0
[00012bc2] 6002                      bra.s     $00012BC6
[00012bc4] 7002                      moveq.l   #2,d0
[00012bc6] 48c0                      ext.l     d0
[00012bc8] dbc0                      adda.l    d0,a5
[00012bca] 6000 0126                 bra       $00012CF2
[00012bce] 526e fde8                 addq.w    #1,-536(a6)
[00012bd2] 4a6e fde8                 tst.w     -536(a6)
[00012bd6] 6708                      beq.s     $00012BE0
[00012bd8] 203c 0001 3486            move.l    #$00013486,d0
[00012bde] 6006                      bra.s     $00012BE6
[00012be0] 203c 0001 353c            move.l    #$0001353C,d0
[00012be6] 23c0 0001 4eca            move.l    d0,$00014ECA
[00012bec] 2eb9 0001 4eca            move.l    $00014ECA,(a7)
[00012bf2] 4267                      clr.w     -(a7)
[00012bf4] 3f3c 0010                 move.w    #$0010,-(a7)
[00012bf8] 2f0e                      move.l    a6,-(a7)
[00012bfa] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012c00] 2f0d                      move.l    a5,-(a7)
[00012c02] 4eb9 0001 3414            jsr       $00013414
[00012c08] dffc 0000 000c            adda.l    #$0000000C,a7
[00012c0e] 4a6e fde8                 tst.w     -536(a6)
[00012c12] 6704                      beq.s     $00012C18
[00012c14] 7004                      moveq.l   #4,d0
[00012c16] 6002                      bra.s     $00012C1A
[00012c18] 7002                      moveq.l   #2,d0
[00012c1a] 48c0                      ext.l     d0
[00012c1c] dbc0                      adda.l    d0,a5
[00012c1e] 6000 00d2                 bra       $00012CF2
[00012c22] 2d4d fffc                 move.l    a5,-4(a6)
[00012c26] 206e fffc                 movea.l   -4(a6),a0
[00012c2a] 2d50 fff2                 move.l    (a0),-14(a6)
[00012c2e] 588d                      addq.l    #4,a5
[00012c30] 6000 00c0                 bra       $00012CF2
[00012c34] 2d4d fff8                 move.l    a5,-8(a6)
[00012c38] 206e fff8                 movea.l   -8(a6),a0
[00012c3c] 3010                      move.w    (a0),d0
[00012c3e] c07c 00ff                 and.w     #$00FF,d0
[00012c42] 1d40 fdf2                 move.b    d0,-526(a6)
[00012c46] 422e fdf3                 clr.b     -525(a6)
[00012c4a] 548d                      addq.l    #2,a5
[00012c4c] 6000 00a4                 bra       $00012CF2
[00012c50] 1007                      move.b    d7,d0
[00012c52] 4880                      ext.w     d0
[00012c54] 3e80                      move.w    d0,(a7)
[00012c56] 3f06                      move.w    d6,-(a7)
[00012c58] 2f0e                      move.l    a6,-(a7)
[00012c5a] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012c60] 2f0d                      move.l    a5,-(a7)
[00012c62] 4eb9 0001 2df6            jsr       __petoa
[00012c68] dffc 0000 000a            adda.l    #$0000000A,a7
[00012c6e] 588d                      addq.l    #4,a5
[00012c70] 7cff                      moveq.l   #-1,d6
[00012c72] 6000 007e                 bra       $00012CF2
[00012c76] 1007                      move.b    d7,d0
[00012c78] 4880                      ext.w     d0
[00012c7a] 3e80                      move.w    d0,(a7)
[00012c7c] 3f06                      move.w    d6,-(a7)
[00012c7e] 2f0e                      move.l    a6,-(a7)
[00012c80] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012c86] 2f0d                      move.l    a5,-(a7)
[00012c88] 4eb9 0001 2dbe            jsr       __pftoa
[00012c8e] dffc 0000 000a            adda.l    #$0000000A,a7
[00012c94] 588d                      addq.l    #4,a5
[00012c96] 7cff                      moveq.l   #-1,d6
[00012c98] 6058                      bra.s     $00012CF2
[00012c9a] 1007                      move.b    d7,d0
[00012c9c] 4880                      ext.w     d0
[00012c9e] 3e80                      move.w    d0,(a7)
[00012ca0] 3f06                      move.w    d6,-(a7)
[00012ca2] 2f0e                      move.l    a6,-(a7)
[00012ca4] 0697 ffff fdf2            addi.l    #$FFFFFDF2,(a7)
[00012caa] 2f0d                      move.l    a5,-(a7)
[00012cac] 4eb9 0001 2e2e            jsr       __pgtoa
[00012cb2] dffc 0000 000a            adda.l    #$0000000A,a7
[00012cb8] 588d                      addq.l    #4,a5
[00012cba] 7cff                      moveq.l   #-1,d6
[00012cbc] 6034                      bra.s     $00012CF2
[00012cbe] 2eae 0008                 move.l    8(a6),(a7)
[00012cc2] 1007                      move.b    d7,d0
[00012cc4] 4880                      ext.w     d0
[00012cc6] 3f00                      move.w    d0,-(a7)
[00012cc8] 4eb9 0001 2ee8            jsr       _fputc
[00012cce] 548f                      addq.l    #2,a7
[00012cd0] 526e fdec                 addq.w    #1,-532(a6)
[00012cd4] 6000 00d6                 bra       $00012DAC
[00012cd8] 6018                      bra.s     $00012CF2
[00012cda] 907c 0043                 sub.w     #$0043,d0
[00012cde] b07c 0035                 cmp.w     #$0035,d0
[00012ce2] 62da                      bhi.s     $00012CBE
[00012ce4] e540                      asl.w     #2,d0
[00012ce6] 3040                      movea.w   d0,a0
[00012ce8] d1fc 0001 4caa            adda.l    #$00014CAA,a0
[00012cee] 2050                      movea.l   (a0),a0
[00012cf0] 4ed0                      jmp       (a0)
[00012cf2] 2eae fff2                 move.l    -14(a6),(a7)
[00012cf6] 4eb9 0001 39ee            jsr       _strlen
[00012cfc] 3a00                      move.w    d0,d5
[00012cfe] bc45                      cmp.w     d5,d6
[00012d00] 6c06                      bge.s     $00012D08
[00012d02] 4a46                      tst.w     d6
[00012d04] 6d02                      blt.s     $00012D08
[00012d06] 3a06                      move.w    d6,d5
[00012d08] 302e fdf0                 move.w    -528(a6),d0
[00012d0c] 9045                      sub.w     d5,d0
[00012d0e] 3d40 fdee                 move.w    d0,-530(a6)
[00012d12] 4a6e fdea                 tst.w     -534(a6)
[00012d16] 6658                      bne.s     $00012D70
[00012d18] 0c2e 0030 fff6            cmpi.b    #$30,-10(a6)
[00012d1e] 662a                      bne.s     $00012D4A
[00012d20] 206e fff2                 movea.l   -14(a6),a0
[00012d24] 0c10 002d                 cmpi.b    #$2D,(a0)
[00012d28] 6620                      bne.s     $00012D4A
[00012d2a] 5345                      subq.w    #1,d5
[00012d2c] 2eae 0008                 move.l    8(a6),(a7)
[00012d30] 206e fff2                 movea.l   -14(a6),a0
[00012d34] 1010                      move.b    (a0),d0
[00012d36] 4880                      ext.w     d0
[00012d38] 3f00                      move.w    d0,-(a7)
[00012d3a] 4eb9 0001 2ee8            jsr       _fputc
[00012d40] 548f                      addq.l    #2,a7
[00012d42] 52ae fff2                 addq.l    #1,-14(a6)
[00012d46] 526e fdec                 addq.w    #1,-532(a6)
[00012d4a] 6018                      bra.s     $00012D64
[00012d4c] 2eae 0008                 move.l    8(a6),(a7)
[00012d50] 102e fff6                 move.b    -10(a6),d0
[00012d54] 4880                      ext.w     d0
[00012d56] 3f00                      move.w    d0,-(a7)
[00012d58] 4eb9 0001 2ee8            jsr       _fputc
[00012d5e] 548f                      addq.l    #2,a7
[00012d60] 526e fdec                 addq.w    #1,-532(a6)
[00012d64] 302e fdee                 move.w    -530(a6),d0
[00012d68] 536e fdee                 subq.w    #1,-530(a6)
[00012d6c] 4a40                      tst.w     d0
[00012d6e] 6edc                      bgt.s     $00012D4C
[00012d70] 2eae 0008                 move.l    8(a6),(a7)
[00012d74] 3f05                      move.w    d5,-(a7)
[00012d76] 2f2e fff2                 move.l    -14(a6),-(a7)
[00012d7a] 4eb9 0001 2e78            jsr       _fputn
[00012d80] 5c8f                      addq.l    #6,a7
[00012d82] db6e fdec                 add.w     d5,-532(a6)
[00012d86] 6018                      bra.s     $00012DA0
[00012d88] 2eae 0008                 move.l    8(a6),(a7)
[00012d8c] 102e fff6                 move.b    -10(a6),d0
[00012d90] 4880                      ext.w     d0
[00012d92] 3f00                      move.w    d0,-(a7)
[00012d94] 4eb9 0001 2ee8            jsr       _fputc
[00012d9a] 548f                      addq.l    #2,a7
[00012d9c] 526e fdec                 addq.w    #1,-532(a6)
[00012da0] 302e fdee                 move.w    -530(a6),d0
[00012da4] 536e fdee                 subq.w    #1,-530(a6)
[00012da8] 4a40                      tst.w     d0
[00012daa] 6edc                      bgt.s     $00012D88
[00012dac] 6000 fba0                 bra       $0001294E
[00012db0] 302e fdec                 move.w    -532(a6),d0
[00012db4] 4a9f                      tst.l     (a7)+
[00012db6] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00012dba] 4e5e                      unlk      a6
[00012dbc] 4e75                      rts
__pftoa:
[00012dbe] 4e56 fff8                 link      a6,#$FFF8
[00012dc2] 4a6e 0010                 tst.w     16(a6)
[00012dc6] 6c04                      bge.s     $00012DCC
[00012dc8] 7006                      moveq.l   #6,d0
[00012dca] 6004                      bra.s     $00012DD0
[00012dcc] 302e 0010                 move.w    16(a6),d0
[00012dd0] 3d40 0010                 move.w    d0,16(a6)
[00012dd4] 206e 0008                 movea.l   8(a6),a0
[00012dd8] 2010                      move.l    (a0),d0
[00012dda] 2d40 fffc                 move.l    d0,-4(a6)
[00012dde] 3eae 0010                 move.w    16(a6),(a7)
[00012de2] 2f2e 000c                 move.l    12(a6),-(a7)
[00012de6] 2f2e fffc                 move.l    -4(a6),-(a7)
[00012dea] 4eb9 0001 3db8            jsr       $00013DB8
[00012df0] 508f                      addq.l    #8,a7
[00012df2] 4e5e                      unlk      a6
[00012df4] 4e75                      rts
__petoa:
[00012df6] 4e56 fff8                 link      a6,#$FFF8
[00012dfa] 4a6e 0010                 tst.w     16(a6)
[00012dfe] 6c04                      bge.s     $00012E04
[00012e00] 7006                      moveq.l   #6,d0
[00012e02] 6004                      bra.s     $00012E08
[00012e04] 302e 0010                 move.w    16(a6),d0
[00012e08] 3d40 0010                 move.w    d0,16(a6)
[00012e0c] 206e 0008                 movea.l   8(a6),a0
[00012e10] 2010                      move.l    (a0),d0
[00012e12] 2d40 fffc                 move.l    d0,-4(a6)
[00012e16] 3eae 0010                 move.w    16(a6),(a7)
[00012e1a] 2f2e 000c                 move.l    12(a6),-(a7)
[00012e1e] 2f2e fffc                 move.l    -4(a6),-(a7)
[00012e22] 4eb9 0001 3fb6            jsr       $00013FB6
[00012e28] 508f                      addq.l    #8,a7
[00012e2a] 4e5e                      unlk      a6
[00012e2c] 4e75                      rts
__pgtoa:
[00012e2e] 4e56 fff8                 link      a6,#$FFF8
[00012e32] 3eae 0010                 move.w    16(a6),(a7)
[00012e36] 2f2e 000c                 move.l    12(a6),-(a7)
[00012e3a] 2f2e 0008                 move.l    8(a6),-(a7)
[00012e3e] 6100 ff7e                 bsr       __pftoa
[00012e42] 508f                      addq.l    #8,a7
[00012e44] 2d40 fffc                 move.l    d0,-4(a6)
[00012e48] 2eae 000c                 move.l    12(a6),(a7)
[00012e4c] 4eb9 0001 39ee            jsr       _strlen
[00012e52] 322e 0010                 move.w    16(a6),d1
[00012e56] 5e41                      addq.w    #7,d1
[00012e58] b041                      cmp.w     d1,d0
[00012e5a] 6f14                      ble.s     $00012E70
[00012e5c] 3eae 0010                 move.w    16(a6),(a7)
[00012e60] 2f2e 000c                 move.l    12(a6),-(a7)
[00012e64] 2f2e 0008                 move.l    8(a6),-(a7)
[00012e68] 618c                      bsr.s     __petoa
[00012e6a] 508f                      addq.l    #8,a7
[00012e6c] 2d40 fffc                 move.l    d0,-4(a6)
[00012e70] 202e fffc                 move.l    -4(a6),d0
[00012e74] 4e5e                      unlk      a6
[00012e76] 4e75                      rts
_fputn:
[00012e78] 4e56 0000                 link      a6,#$0000
[00012e7c] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[00012e80] 2a6e 0008                 movea.l   8(a6),a5
[00012e84] 3e2e 000c                 move.w    12(a6),d7
[00012e88] 286e 000e                 movea.l   14(a6),a4
[00012e8c] 082c 0003 0003            btst      #3,3(a4)
[00012e92] 6724                      beq.s     $00012EB8
[00012e94] 426c 000c                 clr.w     12(a4)
[00012e98] 3e87                      move.w    d7,(a7)
[00012e9a] 2f0d                      move.l    a5,-(a7)
[00012e9c] 3f14                      move.w    (a4),-(a7)
[00012e9e] 4eb9 0001 3614            jsr       _write
[00012ea4] 5c8f                      addq.l    #6,a7
[00012ea6] b047                      cmp.w     d7,d0
[00012ea8] 670a                      beq.s     $00012EB4
[00012eaa] 006c 0010 0002            ori.w     #$0010,2(a4)
[00012eb0] 70ff                      moveq.l   #-1,d0
[00012eb2] 602a                      bra.s     $00012EDE
[00012eb4] 4240                      clr.w     d0
[00012eb6] 6026                      bra.s     $00012EDE
[00012eb8] 601a                      bra.s     $00012ED4
[00012eba] 2e8c                      move.l    a4,(a7)
[00012ebc] 101d                      move.b    (a5)+,d0
[00012ebe] 4880                      ext.w     d0
[00012ec0] 3f00                      move.w    d0,-(a7)
[00012ec2] 4eb9 0001 2ee8            jsr       _fputc
[00012ec8] 548f                      addq.l    #2,a7
[00012eca] b07c ffff                 cmp.w     #$FFFF,d0
[00012ece] 6604                      bne.s     $00012ED4
[00012ed0] 70ff                      moveq.l   #-1,d0
[00012ed2] 600a                      bra.s     $00012EDE
[00012ed4] 3007                      move.w    d7,d0
[00012ed6] 5347                      subq.w    #1,d7
[00012ed8] 4a40                      tst.w     d0
[00012eda] 66de                      bne.s     $00012EBA
[00012edc] 4240                      clr.w     d0
[00012ede] 4a9f                      tst.l     (a7)+
[00012ee0] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00012ee4] 4e5e                      unlk      a6
[00012ee6] 4e75                      rts
_fputc:
[00012ee8] 4e56 0000                 link      a6,#$0000
[00012eec] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00012ef0] 1e2e 0009                 move.b    9(a6),d7
[00012ef4] 2a6e 000a                 movea.l   10(a6),a5
[00012ef8] 536d 000c                 subq.w    #1,12(a5)
[00012efc] 6d16                      blt.s     $00012F14
[00012efe] 1007                      move.b    d7,d0
[00012f00] 4880                      ext.w     d0
[00012f02] 226d 0008                 movea.l   8(a5),a1
[00012f06] 1280                      move.b    d0,(a1)
[00012f08] c07c 00ff                 and.w     #$00FF,d0
[00012f0c] 52ad 0008                 addq.l    #1,8(a5)
[00012f10] 6012                      bra.s     $00012F24
[00012f12] 6010                      bra.s     $00012F24
[00012f14] 2e8d                      move.l    a5,(a7)
[00012f16] 1007                      move.b    d7,d0
[00012f18] 4880                      ext.w     d0
[00012f1a] 3f00                      move.w    d0,-(a7)
[00012f1c] 4eb9 0001 2f2e            jsr       __flsbuf
[00012f22] 548f                      addq.l    #2,a7
[00012f24] 4a9f                      tst.l     (a7)+
[00012f26] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00012f2a] 4e5e                      unlk      a6
[00012f2c] 4e75                      rts
__flsbuf:
[00012f2e] 4e56 fffe                 link      a6,#$FFFE
[00012f32] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[00012f36] 1e2e 0009                 move.b    9(a6),d7
[00012f3a] 2a6e 000a                 movea.l   10(a6),a5
[00012f3e] 4246                      clr.w     d6
[00012f40] 3a06                      move.w    d6,d5
[00012f42] 082d 0001 0003            btst      #1,3(a5)
[00012f48] 6606                      bne.s     $00012F50
[00012f4a] 70ff                      moveq.l   #-1,d0
[00012f4c] 6000 0124                 bra       $00013072
[00012f50] 4aad 0004                 tst.l     4(a5)
[00012f54] 6656                      bne.s     $00012FAC
[00012f56] 082d 0003 0003            btst      #3,3(a5)
[00012f5c] 664e                      bne.s     $00012FAC
[00012f5e] 3ebc 0200                 move.w    #$0200,(a7)
[00012f62] 4eb9 0001 30f8            jsr       _malloc
[00012f68] 2b40 0004                 move.l    d0,4(a5)
[00012f6c] 2b40 0008                 move.l    d0,8(a5)
[00012f70] 6608                      bne.s     $00012F7A
[00012f72] 006d 0008 0002            ori.w     #$0008,2(a5)
[00012f78] 6032                      bra.s     $00012FAC
[00012f7a] 006d 0004 0002            ori.w     #$0004,2(a5)
[00012f80] 3e95                      move.w    (a5),(a7)
[00012f82] 4eb9 0001 307c            jsr       _isatty
[00012f88] 4a40                      tst.w     d0
[00012f8a] 6708                      beq.s     $00012F94
[00012f8c] 006d 0040 0002            ori.w     #$0040,2(a5)
[00012f92] 6018                      bra.s     $00012FAC
[00012f94] 3b7c 01fe 000c            move.w    #$01FE,12(a5)
[00012f9a] 1007                      move.b    d7,d0
[00012f9c] 4880                      ext.w     d0
[00012f9e] 226d 0008                 movea.l   8(a5),a1
[00012fa2] 1280                      move.b    d0,(a1)
[00012fa4] 52ad 0008                 addq.l    #1,8(a5)
[00012fa8] 6000 00c8                 bra       $00013072
[00012fac] 082d 0003 0003            btst      #3,3(a5)
[00012fb2] 670e                      beq.s     $00012FC2
[00012fb4] 41ee fffe                 lea.l     -2(a6),a0
[00012fb8] 2b48 0004                 move.l    a0,4(a5)
[00012fbc] 2008                      move.l    a0,d0
[00012fbe] 2b40 0008                 move.l    d0,8(a5)
[00012fc2] 206d 0008                 movea.l   8(a5),a0
[00012fc6] 1087                      move.b    d7,(a0)
[00012fc8] 52ad 0008                 addq.l    #1,8(a5)
[00012fcc] 082d 0003 0003            btst      #3,3(a5)
[00012fd2] 671a                      beq.s     $00012FEE
[00012fd4] 7a01                      moveq.l   #1,d5
[00012fd6] 3e85                      move.w    d5,(a7)
[00012fd8] 2f2d 0004                 move.l    4(a5),-(a7)
[00012fdc] 3f15                      move.w    (a5),-(a7)
[00012fde] 4eb9 0001 3614            jsr       _write
[00012fe4] 5c8f                      addq.l    #6,a7
[00012fe6] 3c00                      move.w    d0,d6
[00012fe8] 426d 000c                 clr.w     12(a5)
[00012fec] 606e                      bra.s     $0001305C
[00012fee] 082d 0006 0003            btst      #6,3(a5)
[00012ff4] 673e                      beq.s     $00013034
[00012ff6] be3c 000a                 cmp.b     #$0A,d7
[00012ffa] 6710                      beq.s     $0001300C
[00012ffc] 202d 0004                 move.l    4(a5),d0
[00013000] d0bc 0000 0200            add.l     #$00000200,d0
[00013006] b0ad 0008                 cmp.l     8(a5),d0
[0001300a] 6222                      bhi.s     $0001302E
[0001300c] 202d 0008                 move.l    8(a5),d0
[00013010] 90ad 0004                 sub.l     4(a5),d0
[00013014] 3a00                      move.w    d0,d5
[00013016] 3e85                      move.w    d5,(a7)
[00013018] 2f2d 0004                 move.l    4(a5),-(a7)
[0001301c] 3f15                      move.w    (a5),-(a7)
[0001301e] 4eb9 0001 3614            jsr       _write
[00013024] 5c8f                      addq.l    #6,a7
[00013026] 3c00                      move.w    d0,d6
[00013028] 2b6d 0004 0008            move.l    4(a5),8(a5)
[0001302e] 426d 000c                 clr.w     12(a5)
[00013032] 6028                      bra.s     $0001305C
[00013034] 202d 0008                 move.l    8(a5),d0
[00013038] 90ad 0004                 sub.l     4(a5),d0
[0001303c] 3a00                      move.w    d0,d5
[0001303e] 3e85                      move.w    d5,(a7)
[00013040] 2f2d 0004                 move.l    4(a5),-(a7)
[00013044] 3f15                      move.w    (a5),-(a7)
[00013046] 4eb9 0001 3614            jsr       _write
[0001304c] 5c8f                      addq.l    #6,a7
[0001304e] 3c00                      move.w    d0,d6
[00013050] 3b7c 01ff 000c            move.w    #$01FF,12(a5)
[00013056] 2b6d 0004 0008            move.l    4(a5),8(a5)
[0001305c] ba46                      cmp.w     d6,d5
[0001305e] 670a                      beq.s     $0001306A
[00013060] 006d 0010 0002            ori.w     #$0010,2(a5)
[00013066] 70ff                      moveq.l   #-1,d0
[00013068] 6008                      bra.s     $00013072
[0001306a] 1007                      move.b    d7,d0
[0001306c] 4880                      ext.w     d0
[0001306e] c07c 00ff                 and.w     #$00FF,d0
[00013072] 4a9f                      tst.l     (a7)+
[00013074] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00013078] 4e5e                      unlk      a6
[0001307a] 4e75                      rts
_isatty:
[0001307c] 4e56 0000                 link      a6,#$0000
[00013080] 48e7 0104                 movem.l   d7/a5,-(a7)
[00013084] 3eae 0008                 move.w    8(a6),(a7)
[00013088] 4eb9 0001 19b2            jsr       __chkc
[0001308e] 2a40                      movea.l   d0,a5
[00013090] 200d                      move.l    a5,d0
[00013092] 6604                      bne.s     $00013098
[00013094] 4240                      clr.w     d0
[00013096] 600e                      bra.s     $000130A6
[00013098] 082d 0001 0001            btst      #1,1(a5)
[0001309e] 6604                      bne.s     $000130A4
[000130a0] 4240                      clr.w     d0
[000130a2] 6002                      bra.s     $000130A6
[000130a4] 7001                      moveq.l   #1,d0
[000130a6] 4a9f                      tst.l     (a7)+
[000130a8] 4cdf 2000                 movem.l   (a7)+,a5
[000130ac] 4e5e                      unlk      a6
[000130ae] 4e75                      rts
[000130b0] 4e56 0000                 link      a6,#$0000
[000130b4] 48e7 0104                 movem.l   d7/a5,-(a7)
[000130b8] 3eae 0008                 move.w    8(a6),(a7)
[000130bc] 4eb9 0001 19b2            jsr       __chkc
[000130c2] 2a40                      movea.l   d0,a5
[000130c4] 200d                      move.l    a5,d0
[000130c6] 6604                      bne.s     $000130CC
[000130c8] 4240                      clr.w     d0
[000130ca] 6006                      bra.s     $000130D2
[000130cc] 3015                      move.w    (a5),d0
[000130ce] c07c 0106                 and.w     #$0106,d0
[000130d2] 4a9f                      tst.l     (a7)+
[000130d4] 4cdf 2000                 movem.l   (a7)+,a5
[000130d8] 4e5e                      unlk      a6
[000130da] 4e75                      rts
[000130dc] 4e56 fffc                 link      a6,#$FFFC
[000130e0] 3eae 0008                 move.w    8(a6),(a7)
[000130e4] 6196                      bsr.s     _isatty
[000130e6] 4a40                      tst.w     d0
[000130e8] 6708                      beq.s     $000130F2
[000130ea] 203c 0001 4771            move.l    #$00014771,d0
[000130f0] 6002                      bra.s     $000130F4
[000130f2] 4280                      clr.l     d0
[000130f4] 4e5e                      unlk      a6
[000130f6] 4e75                      rts
_malloc:
[000130f8] 4e56 0000                 link      a6,#$0000
[000130fc] 48e7 0304                 movem.l   d6-d7/a5,-(a7)
[00013100] 3e2e 0008                 move.w    8(a6),d7
[00013104] 5e47                      addq.w    #7,d7
[00013106] e64f                      lsr.w     #3,d7
[00013108] 5247                      addq.w    #1,d7
[0001310a] 3e87                      move.w    d7,(a7)
[0001310c] 611e                      bsr.s     $0001312C
[0001310e] 2a40                      movea.l   d0,a5
[00013110] 200d                      move.l    a5,d0
[00013112] 6604                      bne.s     $00013118
[00013114] 4280                      clr.l     d0
[00013116] 600a                      bra.s     $00013122
[00013118] 3e87                      move.w    d7,(a7)
[0001311a] 2f0d                      move.l    a5,-(a7)
[0001311c] 6100 0086                 bsr       $000131A4
[00013120] 588f                      addq.l    #4,a7
[00013122] 4a9f                      tst.l     (a7)+
[00013124] 4cdf 2080                 movem.l   (a7)+,d7/a5
[00013128] 4e5e                      unlk      a6
[0001312a] 4e75                      rts
[0001312c] 4e56 0000                 link      a6,#$0000
[00013130] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00013134] 2879 0001 4d8a            movea.l   $00014D8A,a4
[0001313a] 2a54                      movea.l   (a4),a5
[0001313c] 605a                      bra.s     $00013198
[0001313e] 4240                      clr.w     d0
[00013140] 302d 0004                 move.w    4(a5),d0
[00013144] 4241                      clr.w     d1
[00013146] 322d 0006                 move.w    6(a5),d1
[0001314a] b340                      eor.w     d1,d0
[0001314c] 4640                      not.w     d0
[0001314e] 4a40                      tst.w     d0
[00013150] 670e                      beq.s     $00013160
[00013152] 3ebc 0001                 move.w    #$0001,(a7)
[00013156] 4eb9 0001 340c            jsr       $0001340C
[0001315c] 4280                      clr.l     d0
[0001315e] 603a                      bra.s     $0001319A
[00013160] 4240                      clr.w     d0
[00013162] 302d 0004                 move.w    4(a5),d0
[00013166] b06e 0008                 cmp.w     8(a6),d0
[0001316a] 6504                      bcs.s     $00013170
[0001316c] 200c                      move.l    a4,d0
[0001316e] 602a                      bra.s     $0001319A
[00013170] bbf9 0001 4d8a            cmpa.l    $00014D8A,a5
[00013176] 661c                      bne.s     $00013194
[00013178] 3eae 0008                 move.w    8(a6),(a7)
[0001317c] 6100 00a6                 bsr       $00013224
[00013180] 2a40                      movea.l   d0,a5
[00013182] 200d                      move.l    a5,d0
[00013184] 660e                      bne.s     $00013194
[00013186] 3ebc 0002                 move.w    #$0002,(a7)
[0001318a] 4eb9 0001 340c            jsr       $0001340C
[00013190] 4280                      clr.l     d0
[00013192] 6006                      bra.s     $0001319A
[00013194] 284d                      movea.l   a5,a4
[00013196] 2a55                      movea.l   (a5),a5
[00013198] 60a4                      bra.s     $0001313E
[0001319a] 4a9f                      tst.l     (a7)+
[0001319c] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000131a0] 4e5e                      unlk      a6
[000131a2] 4e75                      rts
[000131a4] 4e56 0000                 link      a6,#$0000
[000131a8] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[000131ac] 206e 0008                 movea.l   8(a6),a0
[000131b0] 2a50                      movea.l   (a0),a5
[000131b2] 4240                      clr.w     d0
[000131b4] 302e 000c                 move.w    12(a6),d0
[000131b8] 5840                      addq.w    #4,d0
[000131ba] b06d 0004                 cmp.w     4(a5),d0
[000131be] 6508                      bcs.s     $000131C8
[000131c0] 206e 0008                 movea.l   8(a6),a0
[000131c4] 2095                      move.l    (a5),(a0)
[000131c6] 6046                      bra.s     $0001320E
[000131c8] 284d                      movea.l   a5,a4
[000131ca] 4240                      clr.w     d0
[000131cc] 302e 000c                 move.w    12(a6),d0
[000131d0] e748                      lsl.w     #3,d0
[000131d2] 4840                      swap      d0
[000131d4] 4240                      clr.w     d0
[000131d6] 4840                      swap      d0
[000131d8] d9c0                      adda.l    d0,a4
[000131da] 4240                      clr.w     d0
[000131dc] 302d 0004                 move.w    4(a5),d0
[000131e0] 906e 000c                 sub.w     12(a6),d0
[000131e4] 3940 0004                 move.w    d0,4(a4)
[000131e8] 4240                      clr.w     d0
[000131ea] 302c 0004                 move.w    4(a4),d0
[000131ee] 4640                      not.w     d0
[000131f0] 3940 0006                 move.w    d0,6(a4)
[000131f4] 2895                      move.l    (a5),(a4)
[000131f6] 206e 0008                 movea.l   8(a6),a0
[000131fa] 208c                      move.l    a4,(a0)
[000131fc] 3b6e 000c 0004            move.w    12(a6),4(a5)
[00013202] 4240                      clr.w     d0
[00013204] 302d 0004                 move.w    4(a5),d0
[00013208] 4640                      not.w     d0
[0001320a] 3b40 0006                 move.w    d0,6(a5)
[0001320e] 23ee 0008 0001 4d8a       move.l    8(a6),$00014D8A
[00013216] 200d                      move.l    a5,d0
[00013218] 5080                      addq.l    #8,d0
[0001321a] 4a9f                      tst.l     (a7)+
[0001321c] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00013220] 4e5e                      unlk      a6
[00013222] 4e75                      rts
[00013224] 4e56 0000                 link      a6,#$0000
[00013228] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001322c] 3e2e 0008                 move.w    8(a6),d7
[00013230] de7c 003f                 add.w     #$003F,d7
[00013234] ec47                      asr.w     #6,d7
[00013236] ed47                      asl.w     #6,d7
[00013238] 3007                      move.w    d7,d0
[0001323a] e740                      asl.w     #3,d0
[0001323c] 3e80                      move.w    d0,(a7)
[0001323e] 4eb9 0001 35aa            jsr       _sbrk
[00013244] 2a40                      movea.l   d0,a5
[00013246] bbfc ffff ffff            cmpa.l    #$FFFFFFFF,a5
[0001324c] 6604                      bne.s     $00013252
[0001324e] 4280                      clr.l     d0
[00013250] 602a                      bra.s     $0001327C
[00013252] 200d                      move.l    a5,d0
[00013254] 5280                      addq.l    #1,d0
[00013256] c0bc ffff fffe            and.l     #$FFFFFFFE,d0
[0001325c] 2a40                      movea.l   d0,a5
[0001325e] 284d                      movea.l   a5,a4
[00013260] 3947 0004                 move.w    d7,4(a4)
[00013264] 4240                      clr.w     d0
[00013266] 302c 0004                 move.w    4(a4),d0
[0001326a] 4640                      not.w     d0
[0001326c] 3940 0006                 move.w    d0,6(a4)
[00013270] 2e8c                      move.l    a4,(a7)
[00013272] 5097                      addq.l    #8,(a7)
[00013274] 6110                      bsr.s     _free
[00013276] 2039 0001 4d8a            move.l    $00014D8A,d0
[0001327c] 4a9f                      tst.l     (a7)+
[0001327e] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00013282] 4e5e                      unlk      a6
[00013284] 4e75                      rts
_free:
[00013286] 4e56 0000                 link      a6,#$0000
[0001328a] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[0001328e] 2a6e 0008                 movea.l   8(a6),a5
[00013292] 518d                      subq.l    #8,a5
[00013294] 4240                      clr.w     d0
[00013296] 302d 0004                 move.w    4(a5),d0
[0001329a] 4241                      clr.w     d1
[0001329c] 322d 0006                 move.w    6(a5),d1
[000132a0] b340                      eor.w     d1,d0
[000132a2] 4640                      not.w     d0
[000132a4] 4a40                      tst.w     d0
[000132a6] 6710                      beq.s     $000132B8
[000132a8] 3ebc 0003                 move.w    #$0003,(a7)
[000132ac] 4eb9 0001 340c            jsr       $0001340C
[000132b2] 70ff                      moveq.l   #-1,d0
[000132b4] 6000 00c4                 bra       $0001337A
[000132b8] 2879 0001 4d8a            movea.l   $00014D8A,a4
[000132be] bbcc                      cmpa.l    a4,a5
[000132c0] 6504                      bcs.s     $000132C6
[000132c2] bbd4                      cmpa.l    (a4),a5
[000132c4] 6510                      bcs.s     $000132D6
[000132c6] b9d4                      cmpa.l    (a4),a4
[000132c8] 6508                      bcs.s     $000132D2
[000132ca] bbd4                      cmpa.l    (a4),a5
[000132cc] 6308                      bls.s     $000132D6
[000132ce] bbcc                      cmpa.l    a4,a5
[000132d0] 6404                      bcc.s     $000132D6
[000132d2] 2854                      movea.l   (a4),a4
[000132d4] 60e8                      bra.s     $000132BE
[000132d6] bbcc                      cmpa.l    a4,a5
[000132d8] 6532                      bcs.s     $0001330C
[000132da] 200d                      move.l    a5,d0
[000132dc] 4241                      clr.w     d1
[000132de] 322d 0004                 move.w    4(a5),d1
[000132e2] e749                      lsl.w     #3,d1
[000132e4] 4841                      swap      d1
[000132e6] 4241                      clr.w     d1
[000132e8] 4841                      swap      d1
[000132ea] d081                      add.l     d1,d0
[000132ec] 220c                      move.l    a4,d1
[000132ee] 4242                      clr.w     d2
[000132f0] 342c 0004                 move.w    4(a4),d2
[000132f4] e74a                      lsl.w     #3,d2
[000132f6] 4842                      swap      d2
[000132f8] 4242                      clr.w     d2
[000132fa] 4842                      swap      d2
[000132fc] d282                      add.l     d2,d1
[000132fe] b081                      cmp.l     d1,d0
[00013300] 620a                      bhi.s     $0001330C
[00013302] 23cc 0001 4d8a            move.l    a4,$00014D8A
[00013308] 4240                      clr.w     d0
[0001330a] 606e                      bra.s     $0001337A
[0001330c] 200d                      move.l    a5,d0
[0001330e] 4241                      clr.w     d1
[00013310] 322d 0004                 move.w    4(a5),d1
[00013314] e749                      lsl.w     #3,d1
[00013316] 4841                      swap      d1
[00013318] 4241                      clr.w     d1
[0001331a] 4841                      swap      d1
[0001331c] d081                      add.l     d1,d0
[0001331e] b094                      cmp.l     (a4),d0
[00013320] 661c                      bne.s     $0001333E
[00013322] 2054                      movea.l   (a4),a0
[00013324] 3028 0004                 move.w    4(a0),d0
[00013328] d16d 0004                 add.w     d0,4(a5)
[0001332c] 4240                      clr.w     d0
[0001332e] 302d 0004                 move.w    4(a5),d0
[00013332] 4640                      not.w     d0
[00013334] 3b40 0006                 move.w    d0,6(a5)
[00013338] 2054                      movea.l   (a4),a0
[0001333a] 2a90                      move.l    (a0),(a5)
[0001333c] 6002                      bra.s     $00013340
[0001333e] 2a94                      move.l    (a4),(a5)
[00013340] 200c                      move.l    a4,d0
[00013342] 4241                      clr.w     d1
[00013344] 322c 0004                 move.w    4(a4),d1
[00013348] e749                      lsl.w     #3,d1
[0001334a] 4841                      swap      d1
[0001334c] 4241                      clr.w     d1
[0001334e] 4841                      swap      d1
[00013350] d081                      add.l     d1,d0
[00013352] bbc0                      cmpa.l    d0,a5
[00013354] 661a                      bne.s     $00013370
[00013356] 4240                      clr.w     d0
[00013358] 302d 0004                 move.w    4(a5),d0
[0001335c] d16c 0004                 add.w     d0,4(a4)
[00013360] 4240                      clr.w     d0
[00013362] 302c 0004                 move.w    4(a4),d0
[00013366] 4640                      not.w     d0
[00013368] 3940 0006                 move.w    d0,6(a4)
[0001336c] 2895                      move.l    (a5),(a4)
[0001336e] 6002                      bra.s     $00013372
[00013370] 288d                      move.l    a5,(a4)
[00013372] 23cc 0001 4d8a            move.l    a4,$00014D8A
[00013378] 4240                      clr.w     d0
[0001337a] 4a9f                      tst.l     (a7)+
[0001337c] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00013380] 4e5e                      unlk      a6
[00013382] 4e75                      rts
_realloc:
[00013384] 4e56 fffc                 link      a6,#$FFFC
[00013388] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001338c] 2a6e 0008                 movea.l   8(a6),a5
[00013390] 2e8d                      move.l    a5,(a7)
[00013392] 6100 fef2                 bsr       _free
[00013396] 3e2e 000c                 move.w    12(a6),d7
[0001339a] 5e47                      addq.w    #7,d7
[0001339c] e64f                      lsr.w     #3,d7
[0001339e] 5247                      addq.w    #1,d7
[000133a0] 3e87                      move.w    d7,(a7)
[000133a2] 6100 fd88                 bsr       $0001312C
[000133a6] 2d40 fffc                 move.l    d0,-4(a6)
[000133aa] 6604                      bne.s     $000133B0
[000133ac] 4280                      clr.l     d0
[000133ae] 604a                      bra.s     $000133FA
[000133b0] 206e fffc                 movea.l   -4(a6),a0
[000133b4] 2850                      movea.l   (a0),a4
[000133b6] 508c                      addq.l    #8,a4
[000133b8] bbcc                      cmpa.l    a4,a5
[000133ba] 6732                      beq.s     $000133EE
[000133bc] b9cd                      cmpa.l    a5,a4
[000133be] 6410                      bcc.s     $000133D0
[000133c0] 6006                      bra.s     $000133C8
[000133c2] 18dd                      move.b    (a5)+,(a4)+
[000133c4] 536e 000c                 subq.w    #1,12(a6)
[000133c8] 4a6e 000c                 tst.w     12(a6)
[000133cc] 66f4                      bne.s     $000133C2
[000133ce] 601e                      bra.s     $000133EE
[000133d0] 4280                      clr.l     d0
[000133d2] 302e 000c                 move.w    12(a6),d0
[000133d6] d9c0                      adda.l    d0,a4
[000133d8] 4280                      clr.l     d0
[000133da] 302e 000c                 move.w    12(a6),d0
[000133de] dbc0                      adda.l    d0,a5
[000133e0] 6006                      bra.s     $000133E8
[000133e2] 1925                      move.b    -(a5),-(a4)
[000133e4] 536e 000c                 subq.w    #1,12(a6)
[000133e8] 4a6e 000c                 tst.w     12(a6)
[000133ec] 66f4                      bne.s     $000133E2
[000133ee] 3e87                      move.w    d7,(a7)
[000133f0] 2f2e fffc                 move.l    -4(a6),-(a7)
[000133f4] 6100 fdae                 bsr       $000131A4
[000133f8] 588f                      addq.l    #4,a7
[000133fa] 4a9f                      tst.l     (a7)+
[000133fc] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00013400] 4e5e                      unlk      a6
[00013402] 4e75                      rts
[00013404] 4e56 fffc                 link      a6,#$FFFC
[00013408] 4e5e                      unlk      a6
[0001340a] 4e75                      rts
[0001340c] 4e56 fffc                 link      a6,#$FFFC
[00013410] 4e5e                      unlk      a6
[00013412] 4e75                      rts
[00013414] 4e56 fff0                 link      a6,#$FFF0
[00013418] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001341c] 2f0e                      move.l    a6,-(a7)
[0001341e] 0697 ffff fff0            addi.l    #$FFFFFFF0,(a7)
[00013424] 3f2e 0012                 move.w    18(a6),-(a7)
[00013428] 3f2e 0010                 move.w    16(a6),-(a7)
[0001342c] 2f0e                      move.l    a6,-(a7)
[0001342e] 0697 0000 000c            addi.l    #$0000000C,(a7)
[00013434] 2f2e 0008                 move.l    8(a6),-(a7)
[00013438] 206e 0014                 movea.l   20(a6),a0
[0001343c] 4e90                      jsr       (a0)
[0001343e] dffc 0000 0010            adda.l    #$00000010,a7
[00013444] 2a40                      movea.l   d0,a5
[00013446] 200e                      move.l    a6,d0
[00013448] d0bc ffff fff0            add.l     #$FFFFFFF0,d0
[0001344e] bbc0                      cmpa.l    d0,a5
[00013450] 6602                      bne.s     $00013454
[00013452] 421d                      clr.b     (a5)+
[00013454] 286e 000c                 movea.l   12(a6),a4
[00013458] 6012                      bra.s     $0001346C
[0001345a] 1e25                      move.b    -(a5),d7
[0001345c] 4887                      ext.w     d7
[0001345e] de7c 0030                 add.w     #$0030,d7
[00013462] be7c 0039                 cmp.w     #$0039,d7
[00013466] 6f02                      ble.s     $0001346A
[00013468] 5e47                      addq.w    #7,d7
[0001346a] 18c7                      move.b    d7,(a4)+
[0001346c] 200e                      move.l    a6,d0
[0001346e] d0bc ffff fff0            add.l     #$FFFFFFF0,d0
[00013474] bbc0                      cmpa.l    d0,a5
[00013476] 66e2                      bne.s     $0001345A
[00013478] 4214                      clr.b     (a4)
[0001347a] 200c                      move.l    a4,d0
[0001347c] 4a9f                      tst.l     (a7)+
[0001347e] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00013482] 4e5e                      unlk      a6
[00013484] 4e75                      rts
[00013486] 4e56 fffc                 link      a6,#$FFFC
[0001348a] 48e7 0f04                 movem.l   d4-d7/a5,-(a7)
[0001348e] 2d7c 0001 4e34 fffc       move.l    #$00014E34,-4(a6)
[00013496] 2a6e 0014                 movea.l   20(a6),a5
[0001349a] 3c2e 0010                 move.w    16(a6),d6
[0001349e] 48c6                      ext.l     d6
[000134a0] 206e 0008                 movea.l   8(a6),a0
[000134a4] 2e10                      move.l    (a0),d7
[000134a6] 0c6e 0010 0010            cmpi.w    #$0010,16(a6)
[000134ac] 661a                      bne.s     $000134C8
[000134ae] 7a08                      moveq.l   #8,d5
[000134b0] 600c                      bra.s     $000134BE
[000134b2] 2007                      move.l    d7,d0
[000134b4] c07c 000f                 and.w     #$000F,d0
[000134b8] 1ac0                      move.b    d0,(a5)+
[000134ba] e887                      asr.l     #4,d7
[000134bc] 5345                      subq.w    #1,d5
[000134be] 4a87                      tst.l     d7
[000134c0] 6704                      beq.s     $000134C6
[000134c2] 4a45                      tst.w     d5
[000134c4] 66ec                      bne.s     $000134B2
[000134c6] 6068                      bra.s     $00013530
[000134c8] 0c6e 0008 0010            cmpi.w    #$0008,16(a6)
[000134ce] 6624                      bne.s     $000134F4
[000134d0] 7a0b                      moveq.l   #11,d5
[000134d2] 600c                      bra.s     $000134E0
[000134d4] 2007                      move.l    d7,d0
[000134d6] c07c 0007                 and.w     #$0007,d0
[000134da] 1ac0                      move.b    d0,(a5)+
[000134dc] e687                      asr.l     #3,d7
[000134de] 5345                      subq.w    #1,d5
[000134e0] 4a87                      tst.l     d7
[000134e2] 6704                      beq.s     $000134E8
[000134e4] 4a45                      tst.w     d5
[000134e6] 66ec                      bne.s     $000134D4
[000134e8] 4a45                      tst.w     d5
[000134ea] 6606                      bne.s     $000134F2
[000134ec] 022d 0003 ffff            andi.b    #$03,-1(a5)
[000134f2] 603c                      bra.s     $00013530
[000134f4] 4a6e 0012                 tst.w     18(a6)
[000134f8] 671a                      beq.s     $00013514
[000134fa] 4a87                      tst.l     d7
[000134fc] 6c16                      bge.s     $00013514
[000134fe] 206e 000c                 movea.l   12(a6),a0
[00013502] 2050                      movea.l   (a0),a0
[00013504] 10bc 002d                 move.b    #$2D,(a0)
[00013508] 226e 000c                 movea.l   12(a6),a1
[0001350c] 5291                      addq.l    #1,(a1)
[0001350e] 2007                      move.l    d7,d0
[00013510] 4480                      neg.l     d0
[00013512] 2e00                      move.l    d0,d7
[00013514] 6016                      bra.s     $0001352C
[00013516] 2e86                      move.l    d6,(a7)
[00013518] 2f07                      move.l    d7,-(a7)
[0001351a] 4eb9 0001 3abc            jsr       _uldiv
[00013520] 588f                      addq.l    #4,a7
[00013522] 2e00                      move.l    d0,d7
[00013524] 206e fffc                 movea.l   -4(a6),a0
[00013528] 1ae8 0003                 move.b    3(a0),(a5)+
[0001352c] 4a87                      tst.l     d7
[0001352e] 66e6                      bne.s     $00013516
[00013530] 200d                      move.l    a5,d0
[00013532] 4a9f                      tst.l     (a7)+
[00013534] 4cdf 20e0                 movem.l   (a7)+,d5-d7/a5
[00013538] 4e5e                      unlk      a6
[0001353a] 4e75                      rts
[0001353c] 4e56 fffc                 link      a6,#$FFFC
[00013540] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00013544] 206e 0008                 movea.l   8(a6),a0
[00013548] 3010                      move.w    (a0),d0
[0001354a] 48c0                      ext.l     d0
[0001354c] 2d40 fffc                 move.l    d0,-4(a6)
[00013550] 202e fffc                 move.l    -4(a6),d0
[00013554] c0bc 0000 ffff            and.l     #$0000FFFF,d0
[0001355a] 3e00                      move.w    d0,d7
[0001355c] 2a6e 0014                 movea.l   20(a6),a5
[00013560] 3c2e 0010                 move.w    16(a6),d6
[00013564] 4a6e 0012                 tst.w     18(a6)
[00013568] 671c                      beq.s     $00013586
[0001356a] 4a47                      tst.w     d7
[0001356c] 6c18                      bge.s     $00013586
[0001356e] 4240                      clr.w     d0
[00013570] 3007                      move.w    d7,d0
[00013572] 4440                      neg.w     d0
[00013574] 3e00                      move.w    d0,d7
[00013576] 206e 000c                 movea.l   12(a6),a0
[0001357a] 2050                      movea.l   (a0),a0
[0001357c] 10bc 002d                 move.b    #$2D,(a0)
[00013580] 226e 000c                 movea.l   12(a6),a1
[00013584] 5291                      addq.l    #1,(a1)
[00013586] 6012                      bra.s     $0001359A
[00013588] 4280                      clr.l     d0
[0001358a] 3007                      move.w    d7,d0
[0001358c] 80c6                      divu.w    d6,d0
[0001358e] 4840                      swap      d0
[00013590] 1ac0                      move.b    d0,(a5)+
[00013592] 4280                      clr.l     d0
[00013594] 3007                      move.w    d7,d0
[00013596] 80c6                      divu.w    d6,d0
[00013598] 3e00                      move.w    d0,d7
[0001359a] 4a47                      tst.w     d7
[0001359c] 66ea                      bne.s     $00013588
[0001359e] 200d                      move.l    a5,d0
[000135a0] 4a9f                      tst.l     (a7)+
[000135a2] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[000135a6] 4e5e                      unlk      a6
[000135a8] 4e75                      rts
_sbrk:
[000135aa] 4e56 0000                 link      a6,#$0000
[000135ae] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[000135b2] 3e2e 0008                 move.w    8(a6),d7
[000135b6] 48c7                      ext.l     d7
[000135b8] cebc 0000 ffff            and.l     #$0000FFFF,d7
[000135be] 0807 0000                 btst      #0,d7
[000135c2] 6702                      beq.s     $000135C6
[000135c4] 5287                      addq.l    #1,d7
[000135c6] 2a79 0001 4eaa            movea.l   $00014EAA,a5
[000135cc] 2847                      movea.l   d7,a4
[000135ce] d9f9 0001 4eaa            adda.l    $00014EAA,a4
[000135d4] 2e8c                      move.l    a4,(a7)
[000135d6] 4eb9 0001 007a            jsr       _brk
[000135dc] b07c ffff                 cmp.w     #$FFFF,d0
[000135e0] 6616                      bne.s     $000135F8
[000135e2] 33fc 000c 0001 4db0       move.w    #$000C,$00014DB0
[000135ea] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[000135f4] 70ff                      moveq.l   #-1,d0
[000135f6] 6012                      bra.s     $0001360A
[000135f8] 3eae 0008                 move.w    8(a6),(a7)
[000135fc] 4267                      clr.w     -(a7)
[000135fe] 2f0d                      move.l    a5,-(a7)
[00013600] 4eb9 0001 00c4            jsr       _memset
[00013606] 5c8f                      addq.l    #6,a7
[00013608] 200d                      move.l    a5,d0
[0001360a] 4a9f                      tst.l     (a7)+
[0001360c] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[00013610] 4e5e                      unlk      a6
[00013612] 4e75                      rts
_write:
[00013614] 4e56 0000                 link      a6,#$0000
[00013618] 48e7 0104                 movem.l   d7/a5,-(a7)
[0001361c] 3eae 0008                 move.w    8(a6),(a7)
[00013620] 4eb9 0001 19b2            jsr       __chkc
[00013626] 2a40                      movea.l   d0,a5
[00013628] 200d                      move.l    a5,d0
[0001362a] 6604                      bne.s     $00013630
[0001362c] 70ff                      moveq.l   #-1,d0
[0001362e] 6058                      bra.s     $00013688
[00013630] 4a6e 000e                 tst.w     14(a6)
[00013634] 6604                      bne.s     $0001363A
[00013636] 4240                      clr.w     d0
[00013638] 604e                      bra.s     $00013688
[0001363a] 082d 0003 0001            btst      #3,1(a5)
[00013640] 6716                      beq.s     $00013658
[00013642] 33fc 0009 0001 4db0       move.w    #$0009,$00014DB0
[0001364a] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[00013654] 70ff                      moveq.l   #-1,d0
[00013656] 6030                      bra.s     $00013688
[00013658] 082d 0004 0001            btst      #4,1(a5)
[0001365e] 6716                      beq.s     $00013676
[00013660] 3eae 000e                 move.w    14(a6),(a7)
[00013664] 2f2e 000a                 move.l    10(a6),-(a7)
[00013668] 2f0d                      move.l    a5,-(a7)
[0001366a] 4eb9 0001 3692            jsr       $00013692
[00013670] 508f                      addq.l    #8,a7
[00013672] 6014                      bra.s     $00013688
[00013674] 6012                      bra.s     $00013688
[00013676] 3eae 000e                 move.w    14(a6),(a7)
[0001367a] 2f2e 000a                 move.l    10(a6),-(a7)
[0001367e] 2f0d                      move.l    a5,-(a7)
[00013680] 4eb9 0001 37a2            jsr       $000137A2
[00013686] 508f                      addq.l    #8,a7
[00013688] 4a9f                      tst.l     (a7)+
[0001368a] 4cdf 2000                 movem.l   (a7)+,a5
[0001368e] 4e5e                      unlk      a6
[00013690] 4e75                      rts
[00013692] 4e56 fffa                 link      a6,#$FFFA
[00013696] 48e7 030c                 movem.l   d6-d7/a4-a5,-(a7)
[0001369a] 2a6e 0008                 movea.l   8(a6),a5
[0001369e] 286e 000c                 movea.l   12(a6),a4
[000136a2] 3e2e 0010                 move.w    16(a6),d7
[000136a6] 4240                      clr.w     d0
[000136a8] 3d40 fffa                 move.w    d0,-6(a6)
[000136ac] 3d40 fffc                 move.w    d0,-4(a6)
[000136b0] 6000 00c8                 bra       $0001377A
[000136b4] 6004                      bra.s     $000136BA
[000136b6] 526e fffa                 addq.w    #1,-6(a6)
[000136ba] be6e fffa                 cmp.w     -6(a6),d7
[000136be] 6310                      bls.s     $000136D0
[000136c0] 204c                      movea.l   a4,a0
[000136c2] 93c9                      suba.l    a1,a1
[000136c4] 326e fffa                 movea.w   -6(a6),a1
[000136c8] d1c9                      adda.l    a1,a0
[000136ca] 0c10 000a                 cmpi.b    #$0A,(a0)
[000136ce] 66e6                      bne.s     $000136B6
[000136d0] 4240                      clr.w     d0
[000136d2] 302e fffa                 move.w    -6(a6),d0
[000136d6] 906e fffc                 sub.w     -4(a6),d0
[000136da] 6366                      bls.s     $00013742
[000136dc] 3ebc 0001                 move.w    #$0001,(a7)
[000136e0] 3f2e fffa                 move.w    -6(a6),-(a7)
[000136e4] 4240                      clr.w     d0
[000136e6] 302e fffc                 move.w    -4(a6),d0
[000136ea] 9157                      sub.w     d0,(a7)
[000136ec] 200c                      move.l    a4,d0
[000136ee] 4281                      clr.l     d1
[000136f0] 322e fffc                 move.w    -4(a6),d1
[000136f4] d081                      add.l     d1,d0
[000136f6] 2f00                      move.l    d0,-(a7)
[000136f8] 2f2d 0006                 move.l    6(a5),-(a7)
[000136fc] 2f0d                      move.l    a5,-(a7)
[000136fe] 0697 0000 0012            addi.l    #$00000012,(a7)
[00013704] 4eb9 0001 3d18            jsr       __pc_wri
[0001370a] dffc 0000 000e            adda.l    #$0000000E,a7
[00013710] 3d40 fffe                 move.w    d0,-2(a6)
[00013714] 4280                      clr.l     d0
[00013716] 302e fffe                 move.w    -2(a6),d0
[0001371a] d1ad 0006                 add.l     d0,6(a5)
[0001371e] 4a6e fffe                 tst.w     -2(a6)
[00013722] 6616                      bne.s     $0001373A
[00013724] 33fc 0005 0001 4db0       move.w    #$0005,$00014DB0
[0001372c] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[00013736] 70ff                      moveq.l   #-1,d0
[00013738] 605e                      bra.s     $00013798
[0001373a] 3d6e fffa fffc            move.w    -6(a6),-4(a6)
[00013740] 6038                      bra.s     $0001377A
[00013742] 526e fffa                 addq.w    #1,-6(a6)
[00013746] 3ebc 0001                 move.w    #$0001,(a7)
[0001374a] 3f3c 0001                 move.w    #$0001,-(a7)
[0001374e] 2f3c 0001 4d8e            move.l    #$00014D8E,-(a7)
[00013754] 2f2d 0006                 move.l    6(a5),-(a7)
[00013758] 2f0d                      move.l    a5,-(a7)
[0001375a] 0697 0000 0012            addi.l    #$00000012,(a7)
[00013760] 4eb9 0001 3d18            jsr       __pc_wri
[00013766] dffc 0000 000e            adda.l    #$0000000E,a7
[0001376c] 3d40 fffe                 move.w    d0,-2(a6)
[00013770] 4280                      clr.l     d0
[00013772] 302e fffe                 move.w    -2(a6),d0
[00013776] d1ad 0006                 add.l     d0,6(a5)
[0001377a] be6e fffc                 cmp.w     -4(a6),d7
[0001377e] 6200 ff34                 bhi       $000136B4
[00013782] 202d 0006                 move.l    6(a5),d0
[00013786] b0ad 000e                 cmp.l     14(a5),d0
[0001378a] 6f06                      ble.s     $00013792
[0001378c] 2b6d 0006 000e            move.l    6(a5),14(a5)
[00013792] 4240                      clr.w     d0
[00013794] 302e fffc                 move.w    -4(a6),d0
[00013798] 4a9f                      tst.l     (a7)+
[0001379a] 4cdf 3080                 movem.l   (a7)+,d7/a4-a5
[0001379e] 4e5e                      unlk      a6
[000137a0] 4e75                      rts
[000137a2] 4e56 fffe                 link      a6,#$FFFE
[000137a6] 48e7 0104                 movem.l   d7/a5,-(a7)
[000137aa] 2a6e 0008                 movea.l   8(a6),a5
[000137ae] 3ebc 0001                 move.w    #$0001,(a7)
[000137b2] 3f2e 0010                 move.w    16(a6),-(a7)
[000137b6] 2f2e 000c                 move.l    12(a6),-(a7)
[000137ba] 2f2d 0006                 move.l    6(a5),-(a7)
[000137be] 2f0d                      move.l    a5,-(a7)
[000137c0] 0697 0000 0012            addi.l    #$00000012,(a7)
[000137c6] 4eb9 0001 3d18            jsr       __pc_wri
[000137cc] dffc 0000 000e            adda.l    #$0000000E,a7
[000137d2] 3d40 fffe                 move.w    d0,-2(a6)
[000137d6] 4a6e fffe                 tst.w     -2(a6)
[000137da] 6616                      bne.s     $000137F2
[000137dc] 33fc 0005 0001 4db0       move.w    #$0005,$00014DB0
[000137e4] 33f9 0001 4eae 0001 4db2  move.w    $00014EAE,$00014DB2
[000137ee] 70ff                      moveq.l   #-1,d0
[000137f0] 6020                      bra.s     $00013812
[000137f2] 4280                      clr.l     d0
[000137f4] 302e fffe                 move.w    -2(a6),d0
[000137f8] d1ad 0006                 add.l     d0,6(a5)
[000137fc] 202d 0006                 move.l    6(a5),d0
[00013800] b0ad 000e                 cmp.l     14(a5),d0
[00013804] 6f06                      ble.s     $0001380C
[00013806] 2b6d 0006 000e            move.l    6(a5),14(a5)
[0001380c] 4240                      clr.w     d0
[0001380e] 302e fffe                 move.w    -2(a6),d0
[00013812] 4a9f                      tst.l     (a7)+
[00013814] 4cdf 2000                 movem.l   (a7)+,a5
[00013818] 4e5e                      unlk      a6
[0001381a] 4e75                      rts
___open:
[0001381c] 4e56 fffa                 link      a6,#$FFFA
[00013820] 48e7 031c                 movem.l   d6-d7/a3-a5,-(a7)
[00013824] 2a6e 000a                 movea.l   10(a6),a5
[00013828] 302e 0008                 move.w    8(a6),d0
[0001382c] c1fc 0238                 muls.w    #$0238,d0
[00013830] d0bc 0001 51b0            add.l     #$000151B0,d0
[00013836] 2d40 fffc                 move.l    d0,-4(a6)
[0001383a] 7eff                      moveq.l   #-1,d7
[0001383c] 2e8d                      move.l    a5,(a7)
[0001383e] 6100 00ec                 bsr       $0001392C
[00013842] 264d                      movea.l   a5,a3
[00013844] 6002                      bra.s     $00013848
[00013846] 528b                      addq.l    #1,a3
[00013848] 4a13                      tst.b     (a3)
[0001384a] 6706                      beq.s     $00013852
[0001384c] 0c13 003a                 cmpi.b    #$3A,(a3)
[00013850] 66f4                      bne.s     $00013846
[00013852] 4a13                      tst.b     (a3)
[00013854] 671c                      beq.s     $00013872
[00013856] 1d63 fffa                 move.b    -(a3),-6(a6)
[0001385a] 0c2e 0041 fffa            cmpi.b    #$41,-6(a6)
[00013860] 6d0c                      blt.s     $0001386E
[00013862] 0c2e 004f fffa            cmpi.b    #$4F,-6(a6)
[00013868] 6e04                      bgt.s     $0001386E
[0001386a] 2a4b                      movea.l   a3,a5
[0001386c] 6004                      bra.s     $00013872
[0001386e] 4beb 0002                 lea.l     2(a3),a5
[00013872] 302e 000e                 move.w    14(a6),d0
[00013876] 6000 0090                 bra       $00013908
[0001387a] 4257                      clr.w     (a7)
[0001387c] 2f0d                      move.l    a5,-(a7)
[0001387e] 3f3c 003c                 move.w    #$003C,-(a7)
[00013882] 4eb9 0001 3d54            jsr       _trap
[00013888] 5c8f                      addq.l    #6,a7
[0001388a] 3e00                      move.w    d0,d7
[0001388c] 6f0a                      ble.s     $00013898
[0001388e] 206e fffc                 movea.l   -4(a6),a0
[00013892] 3147 0004                 move.w    d7,4(a0)
[00013896] 4247                      clr.w     d7
[00013898] 6000 0086                 bra       $00013920
[0001389c] 4257                      clr.w     (a7)
[0001389e] 2f0d                      move.l    a5,-(a7)
[000138a0] 3f3c 004e                 move.w    #$004E,-(a7)
[000138a4] 4eb9 0001 3d54            jsr       _trap
[000138aa] 5c8f                      addq.l    #6,a7
[000138ac] 4a80                      tst.l     d0
[000138ae] 6704                      beq.s     $000138B4
[000138b0] 4240                      clr.w     d0
[000138b2] 6004                      bra.s     $000138B8
[000138b4] 303c 00ff                 move.w    #$00FF,d0
[000138b8] 3e00                      move.w    d0,d7
[000138ba] 6064                      bra.s     $00013920
[000138bc] 3ebc 004f                 move.w    #$004F,(a7)
[000138c0] 4eb9 0001 3d54            jsr       _trap
[000138c6] 4a80                      tst.l     d0
[000138c8] 6704                      beq.s     $000138CE
[000138ca] 4240                      clr.w     d0
[000138cc] 6004                      bra.s     $000138D2
[000138ce] 303c 00ff                 move.w    #$00FF,d0
[000138d2] 3e00                      move.w    d0,d7
[000138d4] 604a                      bra.s     $00013920
[000138d6] 2e8d                      move.l    a5,(a7)
[000138d8] 3f3c 003d                 move.w    #$003D,-(a7)
[000138dc] 4eb9 0001 3d54            jsr       _trap
[000138e2] 548f                      addq.l    #2,a7
[000138e4] 3e00                      move.w    d0,d7
[000138e6] 6f0a                      ble.s     $000138F2
[000138e8] 206e fffc                 movea.l   -4(a6),a0
[000138ec] 3147 0004                 move.w    d7,4(a0)
[000138f0] 4247                      clr.w     d7
[000138f2] 602c                      bra.s     $00013920
[000138f4] 2e8d                      move.l    a5,(a7)
[000138f6] 3f3c 0041                 move.w    #$0041,-(a7)
[000138fa] 4eb9 0001 3d54            jsr       _trap
[00013900] 548f                      addq.l    #2,a7
[00013902] 3e00                      move.w    d0,d7
[00013904] 601a                      bra.s     $00013920
[00013906] 6018                      bra.s     $00013920
[00013908] 907c 000f                 sub.w     #$000F,d0
[0001390c] b07c 0007                 cmp.w     #$0007,d0
[00013910] 620e                      bhi.s     $00013920
[00013912] e540                      asl.w     #2,d0
[00013914] 3040                      movea.w   d0,a0
[00013916] d1fc 0001 4d90            adda.l    #$00014D90,a0
[0001391c] 2050                      movea.l   (a0),a0
[0001391e] 4ed0                      jmp       (a0)
[00013920] 3007                      move.w    d7,d0
[00013922] 4a9f                      tst.l     (a7)+
[00013924] 4cdf 3880                 movem.l   (a7)+,d7/a3-a5
[00013928] 4e5e                      unlk      a6
[0001392a] 4e75                      rts
[0001392c] 4e56 fffc                 link      a6,#$FFFC
[00013930] 206e 0008                 movea.l   8(a6),a0
[00013934] 0c10 0061                 cmpi.b    #$61,(a0)
[00013938] 6d12                      blt.s     $0001394C
[0001393a] 206e 0008                 movea.l   8(a6),a0
[0001393e] 0c10 007a                 cmpi.b    #$7A,(a0)
[00013942] 6e08                      bgt.s     $0001394C
[00013944] 206e 0008                 movea.l   8(a6),a0
[00013948] 0410 0020                 subi.b    #$20,(a0)
[0001394c] 206e 0008                 movea.l   8(a6),a0
[00013950] 1010                      move.b    (a0),d0
[00013952] 4880                      ext.w     d0
[00013954] 52ae 0008                 addq.l    #1,8(a6)
[00013958] 4a00                      tst.b     d0
[0001395a] 66d4                      bne.s     $00013930
[0001395c] 4e5e                      unlk      a6
[0001395e] 4e75                      rts
_strcat:
[00013960] 4e56 0000                 link      a6,#$0000
[00013964] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[00013968] 2a6e 000c                 movea.l   12(a6),a5
[0001396c] 286e 0008                 movea.l   8(a6),a4
[00013970] 6002                      bra.s     $00013974
[00013972] 528c                      addq.l    #1,a4
[00013974] 4a14                      tst.b     (a4)
[00013976] 66fa                      bne.s     $00013972
[00013978] 18dd                      move.b    (a5)+,(a4)+
[0001397a] 66fc                      bne.s     $00013978
[0001397c] 202e 0008                 move.l    8(a6),d0
[00013980] 4a9f                      tst.l     (a7)+
[00013982] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00013986] 4e5e                      unlk      a6
[00013988] 4e75                      rts
_strcmp:
[0001398a] 4e56 0000                 link      a6,#$0000
[0001398e] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00013992] 2a6e 0008                 movea.l   8(a6),a5
[00013996] 286e 000c                 movea.l   12(a6),a4
[0001399a] 6014                      bra.s     $000139B0
[0001399c] 1e1d                      move.b    (a5)+,d7
[0001399e] 1c1c                      move.b    (a4)+,d6
[000139a0] be06                      cmp.b     d6,d7
[000139a2] 6f04                      ble.s     $000139A8
[000139a4] 7001                      moveq.l   #1,d0
[000139a6] 601a                      bra.s     $000139C2
[000139a8] be06                      cmp.b     d6,d7
[000139aa] 6c04                      bge.s     $000139B0
[000139ac] 70ff                      moveq.l   #-1,d0
[000139ae] 6012                      bra.s     $000139C2
[000139b0] 4a15                      tst.b     (a5)
[000139b2] 6704                      beq.s     $000139B8
[000139b4] 4a14                      tst.b     (a4)
[000139b6] 66e4                      bne.s     $0001399C
[000139b8] 1015                      move.b    (a5),d0
[000139ba] 4880                      ext.w     d0
[000139bc] 1214                      move.b    (a4),d1
[000139be] 4881                      ext.w     d1
[000139c0] 9041                      sub.w     d1,d0
[000139c2] 4a9f                      tst.l     (a7)+
[000139c4] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[000139c8] 4e5e                      unlk      a6
[000139ca] 4e75                      rts
_strcpy:
[000139cc] 4e56 0000                 link      a6,#$0000
[000139d0] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[000139d4] 2a6e 000c                 movea.l   12(a6),a5
[000139d8] 286e 0008                 movea.l   8(a6),a4
[000139dc] 18dd                      move.b    (a5)+,(a4)+
[000139de] 66fc                      bne.s     $000139DC
[000139e0] 202e 0008                 move.l    8(a6),d0
[000139e4] 4a9f                      tst.l     (a7)+
[000139e6] 4cdf 3000                 movem.l   (a7)+,a4-a5
[000139ea] 4e5e                      unlk      a6
[000139ec] 4e75                      rts
_strlen:
[000139ee] 4e56 0000                 link      a6,#$0000
[000139f2] 48e7 010c                 movem.l   d7/a4-a5,-(a7)
[000139f6] 2a6e 0008                 movea.l   8(a6),a5
[000139fa] 284d                      movea.l   a5,a4
[000139fc] 6002                      bra.s     $00013A00
[000139fe] 528c                      addq.l    #1,a4
[00013a00] 4a14                      tst.b     (a4)
[00013a02] 66fa                      bne.s     $000139FE
[00013a04] 200c                      move.l    a4,d0
[00013a06] 48c0                      ext.l     d0
[00013a08] 908d                      sub.l     a5,d0
[00013a0a] 4a9f                      tst.l     (a7)+
[00013a0c] 4cdf 3000                 movem.l   (a7)+,a4-a5
[00013a10] 4e5e                      unlk      a6
[00013a12] 4e75                      rts
[00013a14] 4e56 fffc                 link      a6,#$FFFC
[00013a18] 4e5e                      unlk      a6
[00013a1a] 4e75                      rts
__strcmp:
[00013a1c] 4e56 0000                 link      a6,#$0000
[00013a20] 48e7 070c                 movem.l   d5-d7/a4-a5,-(a7)
[00013a24] 2a6e 0008                 movea.l   8(a6),a5
[00013a28] 286e 000c                 movea.l   12(a6),a4
[00013a2c] 6024                      bra.s     $00013A52
[00013a2e] 101d                      move.b    (a5)+,d0
[00013a30] 4880                      ext.w     d0
[00013a32] 3e80                      move.w    d0,(a7)
[00013a34] 6130                      bsr.s     $00013A66
[00013a36] 1e00                      move.b    d0,d7
[00013a38] 101c                      move.b    (a4)+,d0
[00013a3a] 4880                      ext.w     d0
[00013a3c] 3e80                      move.w    d0,(a7)
[00013a3e] 6126                      bsr.s     $00013A66
[00013a40] 1c00                      move.b    d0,d6
[00013a42] be06                      cmp.b     d6,d7
[00013a44] 6f04                      ble.s     $00013A4A
[00013a46] 7001                      moveq.l   #1,d0
[00013a48] 6012                      bra.s     $00013A5C
[00013a4a] be06                      cmp.b     d6,d7
[00013a4c] 6c04                      bge.s     $00013A52
[00013a4e] 70ff                      moveq.l   #-1,d0
[00013a50] 600a                      bra.s     $00013A5C
[00013a52] 4a15                      tst.b     (a5)
[00013a54] 66d8                      bne.s     $00013A2E
[00013a56] 4a14                      tst.b     (a4)
[00013a58] 66d4                      bne.s     $00013A2E
[00013a5a] 4240                      clr.w     d0
[00013a5c] 4a9f                      tst.l     (a7)+
[00013a5e] 4cdf 30c0                 movem.l   (a7)+,d6-d7/a4-a5
[00013a62] 4e5e                      unlk      a6
[00013a64] 4e75                      rts
[00013a66] 4e56 0000                 link      a6,#$0000
[00013a6a] 48e7 0300                 movem.l   d6-d7,-(a7)
[00013a6e] 3e2e 0008                 move.w    8(a6),d7
[00013a72] be7c 0061                 cmp.w     #$0061,d7
[00013a76] 6d0a                      blt.s     $00013A82
[00013a78] be7c 007a                 cmp.w     #$007A,d7
[00013a7c] 6e04                      bgt.s     $00013A82
[00013a7e] de7c ffe0                 add.w     #$FFE0,d7
[00013a82] 3007                      move.w    d7,d0
[00013a84] 4a9f                      tst.l     (a7)+
[00013a86] 4cdf 0080                 movem.l   (a7)+,d7
[00013a8a] 4e5e                      unlk      a6
[00013a8c] 4e75                      rts
__salloc:
[00013a8e] 205f                      movea.l   (a7)+,a0
[00013a90] 4280                      clr.l     d0
[00013a92] 301f                      move.w    (a7)+,d0
[00013a94] 5a80                      addq.l    #5,d0
[00013a96] 0880 0000                 bclr      #0,d0
[00013a9a] 2279 0001 4eaa            movea.l   $00014EAA,a1
[00013aa0] 43e9 0100                 lea.l     256(a1),a1
[00013aa4] 43f1 0800                 lea.l     0(a1,d0.l),a1
[00013aa8] bfc9                      cmpa.l    a1,a7
[00013aaa] 6206                      bhi.s     $00013AB2
[00013aac] 4ef9 0001 00ac            jmp       $000100AC
[00013ab2] 9fc0                      suba.l    d0,a7
[00013ab4] 43ef 0004                 lea.l     4(a7),a1
[00013ab8] 2009                      move.l    a1,d0
[00013aba] 4ed0                      jmp       (a0)
_uldiv:
[00013abc] 4e56 0000                 link      a6,#$0000
[00013ac0] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00013ac4] 2e2e 0008                 move.l    8(a6),d7
[00013ac8] 2c2e 000c                 move.l    12(a6),d6
[00013acc] 4a86                      tst.l     d6
[00013ace] 6616                      bne.s     $00013AE6
[00013ad0] 23fc 8000 0000 0001 4e34  move.l    #$80000000,$00014E34
[00013ada] 203c 8000 0000            move.l    #$80000000,d0
[00013ae0] 80fc 0000                 divu.w    #$0000,d0
[00013ae4] 6048                      bra.s     $00013B2E
[00013ae6] bc87                      cmp.l     d7,d6
[00013ae8] 630a                      bls.s     $00013AF4
[00013aea] 23c7 0001 4e34            move.l    d7,$00014E34
[00013af0] 4280                      clr.l     d0
[00013af2] 603a                      bra.s     $00013B2E
[00013af4] be86                      cmp.l     d6,d7
[00013af6] 6606                      bne.s     $00013AFE
[00013af8] 7a01                      moveq.l   #1,d5
[00013afa] 4287                      clr.l     d7
[00013afc] 6028                      bra.s     $00013B26
[00013afe] 7801                      moveq.l   #1,d4
[00013b00] be86                      cmp.l     d6,d7
[00013b02] 650e                      bcs.s     $00013B12
[00013b04] 2006                      move.l    d6,d0
[00013b06] e380                      asl.l     #1,d0
[00013b08] bc80                      cmp.l     d0,d6
[00013b0a] 6206                      bhi.s     $00013B12
[00013b0c] e386                      asl.l     #1,d6
[00013b0e] e384                      asl.l     #1,d4
[00013b10] 60ee                      bra.s     $00013B00
[00013b12] 4285                      clr.l     d5
[00013b14] 4a84                      tst.l     d4
[00013b16] 670e                      beq.s     $00013B26
[00013b18] be86                      cmp.l     d6,d7
[00013b1a] 6504                      bcs.s     $00013B20
[00013b1c] 8a84                      or.l      d4,d5
[00013b1e] 9e86                      sub.l     d6,d7
[00013b20] e28c                      lsr.l     #1,d4
[00013b22] e28e                      lsr.l     #1,d6
[00013b24] 60ee                      bra.s     $00013B14
[00013b26] 23c7 0001 4e34            move.l    d7,$00014E34
[00013b2c] 2005                      move.l    d5,d0
[00013b2e] 4a9f                      tst.l     (a7)+
[00013b30] 4cdf 00f0                 movem.l   (a7)+,d4-d7
[00013b34] 4e5e                      unlk      a6
[00013b36] 4e75                      rts
_signal:
[00013b38] 4e56 0000                 link      a6,#$0000
[00013b3c] 48e7 0704                 movem.l   d5-d7/a5,-(a7)
[00013b40] 3e2e 0008                 move.w    8(a6),d7
[00013b44] 2a6e 000a                 movea.l   10(a6),a5
[00013b48] be7c 0010                 cmp.w     #$0010,d7
[00013b4c] 6c04                      bge.s     $00013B52
[00013b4e] 4a47                      tst.w     d7
[00013b50] 6c06                      bge.s     $00013B58
[00013b52] 70ff                      moveq.l   #-1,d0
[00013b54] 6000 00ec                 bra       $00013C42
[00013b58] 3047                      movea.w   d7,a0
[00013b5a] d1c8                      adda.l    a0,a0
[00013b5c] d1c8                      adda.l    a0,a0
[00013b5e] d1fc 0001 514a            adda.l    #$0001514A,a0
[00013b64] 208d                      move.l    a5,(a0)
[00013b66] 3007                      move.w    d7,d0
[00013b68] 6000 00c0                 bra       $00013C2A
[00013b6c] 2ebc 0001 3c78            move.l    #$00013C78,(a7)
[00013b72] 3f3c 0004                 move.w    #$0004,-(a7)
[00013b76] 6100 00d4                 bsr       $00013C4C
[00013b7a] 548f                      addq.l    #2,a7
[00013b7c] 2ebc 0001 3c78            move.l    #$00013C78,(a7)
[00013b82] 3f3c 0008                 move.w    #$0008,-(a7)
[00013b86] 6100 00c4                 bsr       $00013C4C
[00013b8a] 548f                      addq.l    #2,a7
[00013b8c] 2ebc 0001 3c78            move.l    #$00013C78,(a7)
[00013b92] 3f3c 000a                 move.w    #$000A,-(a7)
[00013b96] 6100 00b4                 bsr       $00013C4C
[00013b9a] 548f                      addq.l    #2,a7
[00013b9c] 2ebc 0001 3c78            move.l    #$00013C78,(a7)
[00013ba2] 3f3c 000b                 move.w    #$000B,-(a7)
[00013ba6] 6100 00a4                 bsr       $00013C4C
[00013baa] 548f                      addq.l    #2,a7
[00013bac] 6000 0092                 bra       $00013C40
[00013bb0] 2ebc 0001 3c88            move.l    #$00013C88,(a7)
[00013bb6] 3f3c 0009                 move.w    #$0009,-(a7)
[00013bba] 6100 0090                 bsr       $00013C4C
[00013bbe] 548f                      addq.l    #2,a7
[00013bc0] 6000 007e                 bra       $00013C40
[00013bc4] 2ebc 0001 3c94            move.l    #$00013C94,(a7)
[00013bca] 3f3c 0020                 move.w    #$0020,-(a7)
[00013bce] 6100 007c                 bsr       $00013C4C
[00013bd2] 548f                      addq.l    #2,a7
[00013bd4] 7c23                      moveq.l   #35,d6
[00013bd6] 6010                      bra.s     $00013BE8
[00013bd8] 2ebc 0001 3c94            move.l    #$00013C94,(a7)
[00013bde] 3f06                      move.w    d6,-(a7)
[00013be0] 6100 006a                 bsr       $00013C4C
[00013be4] 548f                      addq.l    #2,a7
[00013be6] 5246                      addq.w    #1,d6
[00013be8] bc7c 0030                 cmp.w     #$0030,d6
[00013bec] 6dea                      blt.s     $00013BD8
[00013bee] 6050                      bra.s     $00013C40
[00013bf0] 2ebc 0001 3ca0            move.l    #$00013CA0,(a7)
[00013bf6] 3f3c 0002                 move.w    #$0002,-(a7)
[00013bfa] 6150                      bsr.s     $00013C4C
[00013bfc] 548f                      addq.l    #2,a7
[00013bfe] 2ebc 0001 3ca0            move.l    #$00013CA0,(a7)
[00013c04] 3f3c 0003                 move.w    #$0003,-(a7)
[00013c08] 6142                      bsr.s     $00013C4C
[00013c0a] 548f                      addq.l    #2,a7
[00013c0c] 6032                      bra.s     $00013C40
[00013c0e] 7c05                      moveq.l   #5,d6
[00013c10] 600e                      bra.s     $00013C20
[00013c12] 2ebc 0001 3cae            move.l    #$00013CAE,(a7)
[00013c18] 3f06                      move.w    d6,-(a7)
[00013c1a] 6130                      bsr.s     $00013C4C
[00013c1c] 548f                      addq.l    #2,a7
[00013c1e] 5246                      addq.w    #1,d6
[00013c20] bc7c 0008                 cmp.w     #$0008,d6
[00013c24] 6dec                      blt.s     $00013C12
[00013c26] 6018                      bra.s     $00013C40
[00013c28] 6016                      bra.s     $00013C40
[00013c2a] 5940                      subq.w    #4,d0
[00013c2c] b07c 0006                 cmp.w     #$0006,d0
[00013c30] 620e                      bhi.s     $00013C40
[00013c32] e540                      asl.w     #2,d0
[00013c34] 3040                      movea.w   d0,a0
[00013c36] d1fc 0001 4e38            adda.l    #$00014E38,a0
[00013c3c] 2050                      movea.l   (a0),a0
[00013c3e] 4ed0                      jmp       (a0)
[00013c40] 4280                      clr.l     d0
[00013c42] 4a9f                      tst.l     (a7)+
[00013c44] 4cdf 20c0                 movem.l   (a7)+,d6-d7/a5
[00013c48] 4e5e                      unlk      a6
[00013c4a] 4e75                      rts
[00013c4c] 4e56 fff2                 link      a6,#$FFF2
[00013c50] 3d6e 0008 fff6            move.w    8(a6),-10(a6)
[00013c56] 2d6e 000a fff8            move.l    10(a6),-8(a6)
[00013c5c] 42ae fffc                 clr.l     -4(a6)
[00013c60] 2e8e                      move.l    a6,(a7)
[00013c62] 0697 ffff fff6            addi.l    #$FFFFFFF6,(a7)
[00013c68] 3f3c 003d                 move.w    #$003D,-(a7)
[00013c6c] 4eb9 0001 0096            jsr       ___BDOS
[00013c72] 548f                      addq.l    #2,a7
[00013c74] 4e5e                      unlk      a6
[00013c76] 4e75                      rts
[00013c78] 23f9 0001 515a 0001 4ece  move.l    $0001515A,$00014ECE
[00013c82] 54af 0002                 addq.l    #2,2(a7)
[00013c86] 6030                      bra.s     $00013CB8
[00013c88] 23f9 0001 515e 0001 4ece  move.l    $0001515E,$00014ECE
[00013c92] 6024                      bra.s     $00013CB8
[00013c94] 23f9 0001 5162 0001 4ece  move.l    $00015162,$00014ECE
[00013c9e] 6018                      bra.s     $00013CB8
[00013ca0] bf8f                      cmpm.l    (a7)+,(a7)+
[00013ca2] 23f9 0001 5172 0001 4ece  move.l    $00015172,$00014ECE
[00013cac] 600a                      bra.s     $00013CB8
[00013cae] 23f9 0001 516a 0001 4ece  move.l    $0001516A,$00014ECE
[00013cb8] 48e7 fffe                 movem.l   d0-d7/a0-a6,-(a7)
[00013cbc] 2079 0001 4ece            movea.l   $00014ECE,a0
[00013cc2] 9dce                      suba.l    a6,a6
[00013cc4] 4e90                      jsr       (a0)
[00013cc6] 4cdf 7fff                 movem.l   (a7)+,d0-d7/a0-a6
[00013cca] 4e77                      rtr
[00013ccc] 4a85                      tst.l     d5
[00013cce] 670a                      beq.s     $00013CDA
[00013cd0] 2ebc ffff ffff            move.l    #$FFFFFFFF,(a7)
[00013cd6] 8bfc 0000                 divs.w    #$0000,d5
[00013cda] 4e75                      rts
__pc_rea:
[00013cdc] 4e56 fffa                 link      a6,#$FFFA
[00013ce0] 206e 0008                 movea.l   8(a6),a0
[00013ce4] 3d68 fff2 fffe            move.w    -14(a0),-2(a6)
[00013cea] 2eae 0010                 move.l    16(a6),(a7)
[00013cee] 306e 0016                 movea.w   22(a6),a0
[00013cf2] 2f08                      move.l    a0,-(a7)
[00013cf4] 306e 0014                 movea.w   20(a6),a0
[00013cf8] 2f08                      move.l    a0,-(a7)
[00013cfa] 4eb9 0001 3d64            jsr       lmul
[00013d00] 508f                      addq.l    #8,a7
[00013d02] 2f00                      move.l    d0,-(a7)
[00013d04] 3f2e fffe                 move.w    -2(a6),-(a7)
[00013d08] 3f3c 003f                 move.w    #$003F,-(a7)
[00013d0c] 4eb9 0001 3d54            jsr       _trap
[00013d12] 508f                      addq.l    #8,a7
[00013d14] 4e5e                      unlk      a6
[00013d16] 4e75                      rts
__pc_wri:
[00013d18] 4e56 fffa                 link      a6,#$FFFA
[00013d1c] 206e 0008                 movea.l   8(a6),a0
[00013d20] 3d68 fff2 fffe            move.w    -14(a0),-2(a6)
[00013d26] 2eae 0010                 move.l    16(a6),(a7)
[00013d2a] 306e 0016                 movea.w   22(a6),a0
[00013d2e] 2f08                      move.l    a0,-(a7)
[00013d30] 306e 0014                 movea.w   20(a6),a0
[00013d34] 2f08                      move.l    a0,-(a7)
[00013d36] 4eb9 0001 3d64            jsr       lmul
[00013d3c] 508f                      addq.l    #8,a7
[00013d3e] 2f00                      move.l    d0,-(a7)
[00013d40] 3f2e fffe                 move.w    -2(a6),-(a7)
[00013d44] 3f3c 0040                 move.w    #$0040,-(a7)
[00013d48] 4eb9 0001 3d54            jsr       _trap
[00013d4e] 508f                      addq.l    #8,a7
[00013d50] 4e5e                      unlk      a6
[00013d52] 4e75                      rts
_trap:
[00013d54] 23df 0001 4ed2            move.l    (a7)+,$00014ED2
[00013d5a] 4e41                      trap      #1
[00013d5c] 2f39 0001 4ed2            move.l    $00014ED2,-(a7)
[00013d62] 4e75                      rts
lmul:
[00013d64] 4e56 fffc                 link      a6,#$FFFC
[00013d68] 4242                      clr.w     d2
[00013d6a] 4aae 0008                 tst.l     8(a6)
[00013d6e] 6c06                      bge.s     $00013D76
[00013d70] 44ae 0008                 neg.l     8(a6)
[00013d74] 5242                      addq.w    #1,d2
[00013d76] 4aae 000c                 tst.l     12(a6)
[00013d7a] 6c06                      bge.s     $00013D82
[00013d7c] 44ae 000c                 neg.l     12(a6)
[00013d80] 5242                      addq.w    #1,d2
[00013d82] 302e 000a                 move.w    10(a6),d0
[00013d86] c0ee 000e                 mulu.w    14(a6),d0
[00013d8a] 2d40 fffc                 move.l    d0,-4(a6)
[00013d8e] 302e 0008                 move.w    8(a6),d0
[00013d92] c0ee 000e                 mulu.w    14(a6),d0
[00013d96] 322e 000c                 move.w    12(a6),d1
[00013d9a] c2ee 000a                 mulu.w    10(a6),d1
[00013d9e] d041                      add.w     d1,d0
[00013da0] d06e fffc                 add.w     -4(a6),d0
[00013da4] 3d40 fffc                 move.w    d0,-4(a6)
[00013da8] 202e fffc                 move.l    -4(a6),d0
[00013dac] 0802 0000                 btst      #0,d2
[00013db0] 6702                      beq.s     $00013DB4
[00013db2] 4480                      neg.l     d0
[00013db4] 4e5e                      unlk      a6
[00013db6] 4e75                      rts
_ftoa:
[00013db8] 4e56 fff8                 link      a6,#$FFF8
[00013dbc] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00013dc0] 2d6e 000c fffc            move.l    12(a6),-4(a6)
[00013dc6] 4a6e 0010                 tst.w     16(a6)
[00013dca] 6e04                      bgt.s     $00013DD0
[00013dcc] 7001                      moveq.l   #1,d0
[00013dce] 6012                      bra.s     $00013DE2
[00013dd0] 0c6e 0016 0010            cmpi.w    #$0016,16(a6)
[00013dd6] 6f04                      ble.s     $00013DDC
[00013dd8] 7017                      moveq.l   #23,d0
[00013dda] 6006                      bra.s     $00013DE2
[00013ddc] 302e 0010                 move.w    16(a6),d0
[00013de0] 5240                      addq.w    #1,d0
[00013de2] 3800                      move.w    d0,d4
[00013de4] 4247                      clr.w     d7
[00013de6] 42a7                      clr.l     -(a7)
[00013de8] 2f2e 0008                 move.l    8(a6),-(a7)
[00013dec] 4eb9 0001 41ea            jsr       $000141EA
[00013df2] 508f                      addq.l    #8,a7
[00013df4] 6c1c                      bge.s     $00013E12
[00013df6] 206e 000c                 movea.l   12(a6),a0
[00013dfa] 10bc 002d                 move.b    #$2D,(a0)
[00013dfe] 52ae 000c                 addq.l    #1,12(a6)
[00013e02] 2f2e 0008                 move.l    8(a6),-(a7)
[00013e06] 4eb9 0001 4360            jsr       $00014360
[00013e0c] 588f                      addq.l    #4,a7
[00013e0e] 2d40 0008                 move.l    d0,8(a6)
[00013e12] 42a7                      clr.l     -(a7)
[00013e14] 2f2e 0008                 move.l    8(a6),-(a7)
[00013e18] 4eb9 0001 41ea            jsr       $000141EA
[00013e1e] 508f                      addq.l    #8,a7
[00013e20] 6f2e                      ble.s     $00013E50
[00013e22] 6018                      bra.s     $00013E3C
[00013e24] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00013e2a] 2f2e 0008                 move.l    8(a6),-(a7)
[00013e2e] 4eb9 0001 4340            jsr       $00014340
[00013e34] 508f                      addq.l    #8,a7
[00013e36] 2d40 0008                 move.l    d0,8(a6)
[00013e3a] 5347                      subq.w    #1,d7
[00013e3c] 2f3c 8000 0041            move.l    #$80000041,-(a7)
[00013e42] 2f2e 0008                 move.l    8(a6),-(a7)
[00013e46] 4eb9 0001 41ea            jsr       $000141EA
[00013e4c] 508f                      addq.l    #8,a7
[00013e4e] 6dd4                      blt.s     $00013E24
[00013e50] 6018                      bra.s     $00013E6A
[00013e52] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00013e58] 2f2e 0008                 move.l    8(a6),-(a7)
[00013e5c] 4eb9 0001 4208            jsr       $00014208
[00013e62] 508f                      addq.l    #8,a7
[00013e64] 2d40 0008                 move.l    d0,8(a6)
[00013e68] 5247                      addq.w    #1,d7
[00013e6a] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00013e70] 2f2e 0008                 move.l    8(a6),-(a7)
[00013e74] 4eb9 0001 41ea            jsr       $000141EA
[00013e7a] 508f                      addq.l    #8,a7
[00013e7c] 6cd4                      bge.s     $00013E52
[00013e7e] d847                      add.w     d7,d4
[00013e80] 7c01                      moveq.l   #1,d6
[00013e82] 3006                      move.w    d6,d0
[00013e84] 48c0                      ext.l     d0
[00013e86] 2f00                      move.l    d0,-(a7)
[00013e88] 4eb9 0001 4228            jsr       $00014228
[00013e8e] 588f                      addq.l    #4,a7
[00013e90] 2d40 fff8                 move.l    d0,-8(a6)
[00013e94] 6018                      bra.s     $00013EAE
[00013e96] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00013e9c] 2f2e fff8                 move.l    -8(a6),-(a7)
[00013ea0] 4eb9 0001 4208            jsr       $00014208
[00013ea6] 508f                      addq.l    #8,a7
[00013ea8] 2d40 fff8                 move.l    d0,-8(a6)
[00013eac] 5246                      addq.w    #1,d6
[00013eae] bc44                      cmp.w     d4,d6
[00013eb0] 6de4                      blt.s     $00013E96
[00013eb2] 2f3c 8000 0042            move.l    #$80000042,-(a7)
[00013eb8] 2f2e fff8                 move.l    -8(a6),-(a7)
[00013ebc] 4eb9 0001 4208            jsr       $00014208
[00013ec2] 508f                      addq.l    #8,a7
[00013ec4] 2f00                      move.l    d0,-(a7)
[00013ec6] 2f2e 0008                 move.l    8(a6),-(a7)
[00013eca] 4eb9 0001 41ca            jsr       $000141CA
[00013ed0] 508f                      addq.l    #8,a7
[00013ed2] 2d40 0008                 move.l    d0,8(a6)
[00013ed6] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00013edc] 2f2e 0008                 move.l    8(a6),-(a7)
[00013ee0] 4eb9 0001 41ea            jsr       $000141EA
[00013ee6] 508f                      addq.l    #8,a7
[00013ee8] 6d0a                      blt.s     $00013EF4
[00013eea] 2d7c 8000 0041 0008       move.l    #$80000041,8(a6)
[00013ef2] 5247                      addq.w    #1,d7
[00013ef4] 4a47                      tst.w     d7
[00013ef6] 6c34                      bge.s     $00013F2C
[00013ef8] 206e 000c                 movea.l   12(a6),a0
[00013efc] 10bc 0030                 move.b    #$30,(a0)
[00013f00] 52ae 000c                 addq.l    #1,12(a6)
[00013f04] 206e 000c                 movea.l   12(a6),a0
[00013f08] 10bc 002e                 move.b    #$2E,(a0)
[00013f0c] 52ae 000c                 addq.l    #1,12(a6)
[00013f10] 4a44                      tst.w     d4
[00013f12] 6c02                      bge.s     $00013F16
[00013f14] 9e44                      sub.w     d4,d7
[00013f16] 7cff                      moveq.l   #-1,d6
[00013f18] 600e                      bra.s     $00013F28
[00013f1a] 206e 000c                 movea.l   12(a6),a0
[00013f1e] 10bc 0030                 move.b    #$30,(a0)
[00013f22] 52ae 000c                 addq.l    #1,12(a6)
[00013f26] 5346                      subq.w    #1,d6
[00013f28] bc47                      cmp.w     d7,d6
[00013f2a] 6eee                      bgt.s     $00013F1A
[00013f2c] 4246                      clr.w     d6
[00013f2e] 606a                      bra.s     $00013F9A
[00013f30] 2f2e 0008                 move.l    8(a6),-(a7)
[00013f34] 4eb9 0001 42b8            jsr       $000142B8
[00013f3a] 588f                      addq.l    #4,a7
[00013f3c] 3a00                      move.w    d0,d5
[00013f3e] 3005                      move.w    d5,d0
[00013f40] d07c 0030                 add.w     #$0030,d0
[00013f44] 226e 000c                 movea.l   12(a6),a1
[00013f48] 1280                      move.b    d0,(a1)
[00013f4a] 52ae 000c                 addq.l    #1,12(a6)
[00013f4e] bc47                      cmp.w     d7,d6
[00013f50] 660c                      bne.s     $00013F5E
[00013f52] 206e 000c                 movea.l   12(a6),a0
[00013f56] 10bc 002e                 move.b    #$2E,(a0)
[00013f5a] 52ae 000c                 addq.l    #1,12(a6)
[00013f5e] 3005                      move.w    d5,d0
[00013f60] 48c0                      ext.l     d0
[00013f62] 2f00                      move.l    d0,-(a7)
[00013f64] 4eb9 0001 4228            jsr       $00014228
[00013f6a] 588f                      addq.l    #4,a7
[00013f6c] 2d40 fff8                 move.l    d0,-8(a6)
[00013f70] 2f00                      move.l    d0,-(a7)
[00013f72] 2f2e 0008                 move.l    8(a6),-(a7)
[00013f76] 4eb9 0001 437c            jsr       $0001437C
[00013f7c] 508f                      addq.l    #8,a7
[00013f7e] 2d40 0008                 move.l    d0,8(a6)
[00013f82] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00013f88] 2f2e 0008                 move.l    8(a6),-(a7)
[00013f8c] 4eb9 0001 4340            jsr       $00014340
[00013f92] 508f                      addq.l    #8,a7
[00013f94] 2d40 0008                 move.l    d0,8(a6)
[00013f98] 5246                      addq.w    #1,d6
[00013f9a] bc44                      cmp.w     d4,d6
[00013f9c] 6d92                      blt.s     $00013F30
[00013f9e] 206e 000c                 movea.l   12(a6),a0
[00013fa2] 4210                      clr.b     (a0)
[00013fa4] 52ae 000c                 addq.l    #1,12(a6)
[00013fa8] 202e fffc                 move.l    -4(a6),d0
[00013fac] 4a9f                      tst.l     (a7)+
[00013fae] 4cdf 00f0                 movem.l   (a7)+,d4-d7
[00013fb2] 4e5e                      unlk      a6
[00013fb4] 4e75                      rts
[00013fb6] 4e56 fff8                 link      a6,#$FFF8
[00013fba] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00013fbe] 2d6e 000c fffc            move.l    12(a6),-4(a6)
[00013fc4] 4a6e 0010                 tst.w     16(a6)
[00013fc8] 6e04                      bgt.s     $00013FCE
[00013fca] 7001                      moveq.l   #1,d0
[00013fcc] 6012                      bra.s     $00013FE0
[00013fce] 0c6e 0016 0010            cmpi.w    #$0016,16(a6)
[00013fd4] 6f04                      ble.s     $00013FDA
[00013fd6] 7017                      moveq.l   #23,d0
[00013fd8] 6006                      bra.s     $00013FE0
[00013fda] 302e 0010                 move.w    16(a6),d0
[00013fde] 5240                      addq.w    #1,d0
[00013fe0] 3800                      move.w    d0,d4
[00013fe2] 4247                      clr.w     d7
[00013fe4] 42a7                      clr.l     -(a7)
[00013fe6] 2f2e 0008                 move.l    8(a6),-(a7)
[00013fea] 4eb9 0001 41ea            jsr       $000141EA
[00013ff0] 508f                      addq.l    #8,a7
[00013ff2] 6c1c                      bge.s     $00014010
[00013ff4] 206e 000c                 movea.l   12(a6),a0
[00013ff8] 10bc 002d                 move.b    #$2D,(a0)
[00013ffc] 52ae 000c                 addq.l    #1,12(a6)
[00014000] 2f2e 0008                 move.l    8(a6),-(a7)
[00014004] 4eb9 0001 4360            jsr       $00014360
[0001400a] 588f                      addq.l    #4,a7
[0001400c] 2d40 0008                 move.l    d0,8(a6)
[00014010] 42a7                      clr.l     -(a7)
[00014012] 2f2e 0008                 move.l    8(a6),-(a7)
[00014016] 4eb9 0001 41ea            jsr       $000141EA
[0001401c] 508f                      addq.l    #8,a7
[0001401e] 6f2e                      ble.s     $0001404E
[00014020] 6018                      bra.s     $0001403A
[00014022] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00014028] 2f2e 0008                 move.l    8(a6),-(a7)
[0001402c] 4eb9 0001 4340            jsr       $00014340
[00014032] 508f                      addq.l    #8,a7
[00014034] 2d40 0008                 move.l    d0,8(a6)
[00014038] 5347                      subq.w    #1,d7
[0001403a] 2f3c 8000 0041            move.l    #$80000041,-(a7)
[00014040] 2f2e 0008                 move.l    8(a6),-(a7)
[00014044] 4eb9 0001 41ea            jsr       $000141EA
[0001404a] 508f                      addq.l    #8,a7
[0001404c] 6dd4                      blt.s     $00014022
[0001404e] 6018                      bra.s     $00014068
[00014050] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00014056] 2f2e 0008                 move.l    8(a6),-(a7)
[0001405a] 4eb9 0001 4208            jsr       $00014208
[00014060] 508f                      addq.l    #8,a7
[00014062] 2d40 0008                 move.l    d0,8(a6)
[00014066] 5247                      addq.w    #1,d7
[00014068] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[0001406e] 2f2e 0008                 move.l    8(a6),-(a7)
[00014072] 4eb9 0001 41ea            jsr       $000141EA
[00014078] 508f                      addq.l    #8,a7
[0001407a] 6cd4                      bge.s     $00014050
[0001407c] 7c01                      moveq.l   #1,d6
[0001407e] 3006                      move.w    d6,d0
[00014080] 48c0                      ext.l     d0
[00014082] 2f00                      move.l    d0,-(a7)
[00014084] 4eb9 0001 4228            jsr       $00014228
[0001408a] 588f                      addq.l    #4,a7
[0001408c] 2d40 fff8                 move.l    d0,-8(a6)
[00014090] 6018                      bra.s     $000140AA
[00014092] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[00014098] 2f2e fff8                 move.l    -8(a6),-(a7)
[0001409c] 4eb9 0001 4208            jsr       $00014208
[000140a2] 508f                      addq.l    #8,a7
[000140a4] 2d40 fff8                 move.l    d0,-8(a6)
[000140a8] 5246                      addq.w    #1,d6
[000140aa] bc44                      cmp.w     d4,d6
[000140ac] 6de4                      blt.s     $00014092
[000140ae] 2f3c 8000 0042            move.l    #$80000042,-(a7)
[000140b4] 2f2e fff8                 move.l    -8(a6),-(a7)
[000140b8] 4eb9 0001 4208            jsr       $00014208
[000140be] 508f                      addq.l    #8,a7
[000140c0] 2f00                      move.l    d0,-(a7)
[000140c2] 2f2e 0008                 move.l    8(a6),-(a7)
[000140c6] 4eb9 0001 41ca            jsr       $000141CA
[000140cc] 508f                      addq.l    #8,a7
[000140ce] 2d40 0008                 move.l    d0,8(a6)
[000140d2] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[000140d8] 2f2e 0008                 move.l    8(a6),-(a7)
[000140dc] 4eb9 0001 41ea            jsr       $000141EA
[000140e2] 508f                      addq.l    #8,a7
[000140e4] 6d0a                      blt.s     $000140F0
[000140e6] 2d7c 8000 0041 0008       move.l    #$80000041,8(a6)
[000140ee] 5247                      addq.w    #1,d7
[000140f0] 4246                      clr.w     d6
[000140f2] 606a                      bra.s     $0001415E
[000140f4] 2f2e 0008                 move.l    8(a6),-(a7)
[000140f8] 4eb9 0001 42b8            jsr       $000142B8
[000140fe] 588f                      addq.l    #4,a7
[00014100] 3a00                      move.w    d0,d5
[00014102] 3005                      move.w    d5,d0
[00014104] d07c 0030                 add.w     #$0030,d0
[00014108] 226e 000c                 movea.l   12(a6),a1
[0001410c] 1280                      move.b    d0,(a1)
[0001410e] 52ae 000c                 addq.l    #1,12(a6)
[00014112] 4a46                      tst.w     d6
[00014114] 660c                      bne.s     $00014122
[00014116] 206e 000c                 movea.l   12(a6),a0
[0001411a] 10bc 002e                 move.b    #$2E,(a0)
[0001411e] 52ae 000c                 addq.l    #1,12(a6)
[00014122] 3005                      move.w    d5,d0
[00014124] 48c0                      ext.l     d0
[00014126] 2f00                      move.l    d0,-(a7)
[00014128] 4eb9 0001 4228            jsr       $00014228
[0001412e] 588f                      addq.l    #4,a7
[00014130] 2d40 fff8                 move.l    d0,-8(a6)
[00014134] 2f00                      move.l    d0,-(a7)
[00014136] 2f2e 0008                 move.l    8(a6),-(a7)
[0001413a] 4eb9 0001 437c            jsr       $0001437C
[00014140] 508f                      addq.l    #8,a7
[00014142] 2d40 0008                 move.l    d0,8(a6)
[00014146] 2f3c a000 0044            move.l    #$A0000044,-(a7)
[0001414c] 2f2e 0008                 move.l    8(a6),-(a7)
[00014150] 4eb9 0001 4340            jsr       $00014340
[00014156] 508f                      addq.l    #8,a7
[00014158] 2d40 0008                 move.l    d0,8(a6)
[0001415c] 5246                      addq.w    #1,d6
[0001415e] bc44                      cmp.w     d4,d6
[00014160] 6d92                      blt.s     $000140F4
[00014162] 206e 000c                 movea.l   12(a6),a0
[00014166] 10bc 0045                 move.b    #$45,(a0)
[0001416a] 52ae 000c                 addq.l    #1,12(a6)
[0001416e] 4a47                      tst.w     d7
[00014170] 6c12                      bge.s     $00014184
[00014172] 3007                      move.w    d7,d0
[00014174] 4440                      neg.w     d0
[00014176] 3e00                      move.w    d0,d7
[00014178] 206e 000c                 movea.l   12(a6),a0
[0001417c] 10bc 002d                 move.b    #$2D,(a0)
[00014180] 52ae 000c                 addq.l    #1,12(a6)
[00014184] 3007                      move.w    d7,d0
[00014186] 48c0                      ext.l     d0
[00014188] 81fc 000a                 divs.w    #$000A,d0
[0001418c] d07c 0030                 add.w     #$0030,d0
[00014190] 226e 000c                 movea.l   12(a6),a1
[00014194] 1280                      move.b    d0,(a1)
[00014196] 52ae 000c                 addq.l    #1,12(a6)
[0001419a] 3007                      move.w    d7,d0
[0001419c] 48c0                      ext.l     d0
[0001419e] 81fc 000a                 divs.w    #$000A,d0
[000141a2] 4840                      swap      d0
[000141a4] d07c 0030                 add.w     #$0030,d0
[000141a8] 226e 000c                 movea.l   12(a6),a1
[000141ac] 1280                      move.b    d0,(a1)
[000141ae] 52ae 000c                 addq.l    #1,12(a6)
[000141b2] 206e 000c                 movea.l   12(a6),a0
[000141b6] 4210                      clr.b     (a0)
[000141b8] 52ae 000c                 addq.l    #1,12(a6)
[000141bc] 202e fffc                 move.l    -4(a6),d0
[000141c0] 4a9f                      tst.l     (a7)+
[000141c2] 4cdf 00f0                 movem.l   (a7)+,d4-d7
[000141c6] 4e5e                      unlk      a6
[000141c8] 4e75                      rts
[000141ca] 4e56 fffc                 link      a6,#$FFFC
[000141ce] 48e7 1f00                 movem.l   d3-d7,-(a7)
[000141d2] 2e2e 0008                 move.l    8(a6),d7
[000141d6] 2c2e 000c                 move.l    12(a6),d6
[000141da] 4eb9 0001 43be            jsr       $000143BE
[000141e0] 2007                      move.l    d7,d0
[000141e2] 4cdf 00f8                 movem.l   (a7)+,d3-d7
[000141e6] 4e5e                      unlk      a6
[000141e8] 4e75                      rts
[000141ea] 4e56 fffc                 link      a6,#$FFFC
[000141ee] 48e7 1f00                 movem.l   d3-d7,-(a7)
[000141f2] 2e2e 0008                 move.l    8(a6),d7
[000141f6] 2c2e 000c                 move.l    12(a6),d6
[000141fa] 4eb9 0001 4490            jsr       $00014490
[00014200] 4cdf 00f8                 movem.l   (a7)+,d3-d7
[00014204] 4e5e                      unlk      a6
[00014206] 4e75                      rts
[00014208] 4e56 fffc                 link      a6,#$FFFC
[0001420c] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00014210] 2e2e 0008                 move.l    8(a6),d7
[00014214] 2c2e 000c                 move.l    12(a6),d6
[00014218] 4eb9 0001 44c0            jsr       $000144C0
[0001421e] 2007                      move.l    d7,d0
[00014220] 4cdf 00f8                 movem.l   (a7)+,d3-d7
[00014224] 4e5e                      unlk      a6
[00014226] 4e75                      rts
[00014228] 4e56 0000                 link      a6,#$0000
[0001422c] 48e7 0700                 movem.l   d5-d7,-(a7)
[00014230] 4aae 0008                 tst.l     8(a6)
[00014234] 6c0e                      bge.s     $00014244
[00014236] 7c01                      moveq.l   #1,d6
[00014238] 202e 0008                 move.l    8(a6),d0
[0001423c] 4480                      neg.l     d0
[0001423e] 2d40 0008                 move.l    d0,8(a6)
[00014242] 6002                      bra.s     $00014246
[00014244] 4246                      clr.w     d6
[00014246] 4aae 0008                 tst.l     8(a6)
[0001424a] 6604                      bne.s     $00014250
[0001424c] 4280                      clr.l     d0
[0001424e] 605e                      bra.s     $000142AE
[00014250] 7e18                      moveq.l   #24,d7
[00014252] 600c                      bra.s     $00014260
[00014254] 202e 0008                 move.l    8(a6),d0
[00014258] e280                      asr.l     #1,d0
[0001425a] 2d40 0008                 move.l    d0,8(a6)
[0001425e] 5287                      addq.l    #1,d7
[00014260] 202e 0008                 move.l    8(a6),d0
[00014264] c0bc 7f00 0000            and.l     #$7F000000,d0
[0001426a] 66e8                      bne.s     $00014254
[0001426c] 600c                      bra.s     $0001427A
[0001426e] 202e 0008                 move.l    8(a6),d0
[00014272] e380                      asl.l     #1,d0
[00014274] 2d40 0008                 move.l    d0,8(a6)
[00014278] 5387                      subq.l    #1,d7
[0001427a] 082e 0007 0009            btst      #7,9(a6)
[00014280] 67ec                      beq.s     $0001426E
[00014282] 202e 0008                 move.l    8(a6),d0
[00014286] e180                      asl.l     #8,d0
[00014288] 2d40 0008                 move.l    d0,8(a6)
[0001428c] debc 0000 0040            add.l     #$00000040,d7
[00014292] 2007                      move.l    d7,d0
[00014294] c0bc 0000 007f            and.l     #$0000007F,d0
[0001429a] 81ae 0008                 or.l      d0,8(a6)
[0001429e] 4a46                      tst.w     d6
[000142a0] 6708                      beq.s     $000142AA
[000142a2] 00ae 0000 0080 0008       ori.l     #$00000080,8(a6)
[000142aa] 202e 0008                 move.l    8(a6),d0
[000142ae] 4a9f                      tst.l     (a7)+
[000142b0] 4cdf 00c0                 movem.l   (a7)+,d6-d7
[000142b4] 4e5e                      unlk      a6
[000142b6] 4e75                      rts
[000142b8] 4e56 0000                 link      a6,#$0000
[000142bc] 48e7 0f00                 movem.l   d4-d7,-(a7)
[000142c0] 202e 0008                 move.l    8(a6),d0
[000142c4] c0bc 0000 007f            and.l     #$0000007F,d0
[000142ca] d0bc ffff ffc0            add.l     #$FFFFFFC0,d0
[000142d0] 3c00                      move.w    d0,d6
[000142d2] 4aae 0008                 tst.l     8(a6)
[000142d6] 6704                      beq.s     $000142DC
[000142d8] 4a46                      tst.w     d6
[000142da] 6c04                      bge.s     $000142E0
[000142dc] 4280                      clr.l     d0
[000142de] 6056                      bra.s     $00014336
[000142e0] 202e 0008                 move.l    8(a6),d0
[000142e4] c0bc 0000 0080            and.l     #$00000080,d0
[000142ea] 3a00                      move.w    d0,d5
[000142ec] bc7c 001f                 cmp.w     #$001F,d6
[000142f0] 6f14                      ble.s     $00014306
[000142f2] 4a45                      tst.w     d5
[000142f4] 6708                      beq.s     $000142FE
[000142f6] 203c 8000 0000            move.l    #$80000000,d0
[000142fc] 6006                      bra.s     $00014304
[000142fe] 203c 7fff ffff            move.l    #$7FFFFFFF,d0
[00014304] 6030                      bra.s     $00014336
[00014306] 2e2e 0008                 move.l    8(a6),d7
[0001430a] e087                      asr.l     #8,d7
[0001430c] cebc 00ff ffff            and.l     #$00FFFFFF,d7
[00014312] 9c7c 0018                 sub.w     #$0018,d6
[00014316] 6004                      bra.s     $0001431C
[00014318] e287                      asr.l     #1,d7
[0001431a] 5246                      addq.w    #1,d6
[0001431c] 4a46                      tst.w     d6
[0001431e] 6df8                      blt.s     $00014318
[00014320] 6004                      bra.s     $00014326
[00014322] e387                      asl.l     #1,d7
[00014324] 5346                      subq.w    #1,d6
[00014326] 4a46                      tst.w     d6
[00014328] 6ef8                      bgt.s     $00014322
[0001432a] 4a45                      tst.w     d5
[0001432c] 6706                      beq.s     $00014334
[0001432e] 2007                      move.l    d7,d0
[00014330] 4480                      neg.l     d0
[00014332] 2e00                      move.l    d0,d7
[00014334] 2007                      move.l    d7,d0
[00014336] 4a9f                      tst.l     (a7)+
[00014338] 4cdf 00e0                 movem.l   (a7)+,d5-d7
[0001433c] 4e5e                      unlk      a6
[0001433e] 4e75                      rts
[00014340] 4e56 fffc                 link      a6,#$FFFC
[00014344] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00014348] 2e2e 0008                 move.l    8(a6),d7
[0001434c] 2c2e 000c                 move.l    12(a6),d6
[00014350] 4eb9 0001 4532            jsr       $00014532
[00014356] 2007                      move.l    d7,d0
[00014358] 4cdf 00f8                 movem.l   (a7)+,d3-d7
[0001435c] 4e5e                      unlk      a6
[0001435e] 4e75                      rts
[00014360] 4e56 fffc                 link      a6,#$FFFC
[00014364] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00014368] 2e2e 0008                 move.l    8(a6),d7
[0001436c] 4eb9 0001 43a2            jsr       $000143A2
[00014372] 2007                      move.l    d7,d0
[00014374] 4cdf 00f8                 movem.l   (a7)+,d3-d7
[00014378] 4e5e                      unlk      a6
[0001437a] 4e75                      rts
[0001437c] 4e56 fffc                 link      a6,#$FFFC
[00014380] 48e7 1f00                 movem.l   d3-d7,-(a7)
[00014384] 2e2e 0008                 move.l    8(a6),d7
[00014388] 2c2e 000c                 move.l    12(a6),d6
[0001438c] 4eb9 0001 43ac            jsr       $000143AC
[00014392] 2007                      move.l    d7,d0
[00014394] 4cdf 00f8                 movem.l   (a7)+,d3-d7
[00014398] 4e5e                      unlk      a6
[0001439a] 4e75                      rts
[0001439c] ce3c 007f                 and.b     #$7F,d7
[000143a0] 4e75                      rts
[000143a2] 4a07                      tst.b     d7
[000143a4] 6704                      beq.s     $000143AA
[000143a6] 0a07 0080                 eori.b    #$80,d7
[000143aa] 4e75                      rts
[000143ac] 1806                      move.b    d6,d4
[000143ae] 6752                      beq.s     $00014402
[000143b0] 0a04 0080                 eori.b    #$80,d4
[000143b4] 6b6a                      bmi.s     $00014420
[000143b6] 1a07                      move.b    d7,d5
[000143b8] 6b6c                      bmi.s     $00014426
[000143ba] 660e                      bne.s     $000143CA
[000143bc] 603e                      bra.s     $000143FC
[000143be] 1806                      move.b    d6,d4
[000143c0] 6b5e                      bmi.s     $00014420
[000143c2] 673e                      beq.s     $00014402
[000143c4] 1a07                      move.b    d7,d5
[000143c6] 6b5e                      bmi.s     $00014426
[000143c8] 6732                      beq.s     $000143FC
[000143ca] 9a04                      sub.b     d4,d5
[000143cc] 6b38                      bmi.s     $00014406
[000143ce] 1807                      move.b    d7,d4
[000143d0] ba3c 0018                 cmp.b     #$18,d5
[000143d4] 642c                      bcc.s     $00014402
[000143d6] 2606                      move.l    d6,d3
[000143d8] 4203                      clr.b     d3
[000143da] eaab                      lsr.l     d5,d3
[000143dc] 1e3c 0080                 move.b    #$80,d7
[000143e0] de83                      add.l     d3,d7
[000143e2] 6504                      bcs.s     $000143E8
[000143e4] 1e04                      move.b    d4,d7
[000143e6] 4e75                      rts
[000143e8] e297                      roxr.l    #1,d7
[000143ea] 5204                      addq.b    #1,d4
[000143ec] 6902                      bvs.s     $000143F0
[000143ee] 64f4                      bcc.s     $000143E4
[000143f0] 7eff                      moveq.l   #-1,d7
[000143f2] 5304                      subq.b    #1,d4
[000143f4] 1e04                      move.b    d4,d7
[000143f6] 003c 0002                 ori.b     #$02,ccr
[000143fa] 4e75                      rts
[000143fc] 2e06                      move.l    d6,d7
[000143fe] 1e04                      move.b    d4,d7
[00014400] 4e75                      rts
[00014402] 4a07                      tst.b     d7
[00014404] 4e75                      rts
[00014406] ba3c ffe8                 cmp.b     #$E8,d5
[0001440a] 6ff0                      ble.s     $000143FC
[0001440c] 4405                      neg.b     d5
[0001440e] 2606                      move.l    d6,d3
[00014410] 4207                      clr.b     d7
[00014412] eaaf                      lsr.l     d5,d7
[00014414] 163c 0080                 move.b    #$80,d3
[00014418] de83                      add.l     d3,d7
[0001441a] 65cc                      bcs.s     $000143E8
[0001441c] 1e04                      move.b    d4,d7
[0001441e] 4e75                      rts
[00014420] 1a07                      move.b    d7,d5
[00014422] 6ba6                      bmi.s     $000143CA
[00014424] 67d6                      beq.s     $000143FC
[00014426] 7680                      moveq.l   #-128,d3
[00014428] b705                      eor.b     d3,d5
[0001442a] 9a04                      sub.b     d4,d5
[0001442c] 6750                      beq.s     $0001447E
[0001442e] 6b3c                      bmi.s     $0001446C
[00014430] ba3c 0018                 cmp.b     #$18,d5
[00014434] 64cc                      bcc.s     $00014402
[00014436] 1807                      move.b    d7,d4
[00014438] 1e03                      move.b    d3,d7
[0001443a] 2606                      move.l    d6,d3
[0001443c] 4203                      clr.b     d3
[0001443e] eaab                      lsr.l     d5,d3
[00014440] 9e83                      sub.l     d3,d7
[00014442] 6ba0                      bmi.s     $000143E4
[00014444] 1a04                      move.b    d4,d5
[00014446] 4207                      clr.b     d7
[00014448] 5304                      subq.b    #1,d4
[0001444a] bebc 0000 7fff            cmp.l     #$00007FFF,d7
[00014450] 6206                      bhi.s     $00014458
[00014452] 4847                      swap      d7
[00014454] 983c 0010                 sub.b     #$10,d4
[00014458] de87                      add.l     d7,d7
[0001445a] 5bcc fffc                 dbmi      d4,$00014458
[0001445e] b905                      eor.b     d4,d5
[00014460] 6b06                      bmi.s     $00014468
[00014462] 1e04                      move.b    d4,d7
[00014464] 6702                      beq.s     $00014468
[00014466] 4e75                      rts
[00014468] 7e00                      moveq.l   #0,d7
[0001446a] 4e75                      rts
[0001446c] ba3c ffe8                 cmp.b     #$E8,d5
[00014470] 6f8a                      ble.s     $000143FC
[00014472] 4405                      neg.b     d5
[00014474] 2607                      move.l    d7,d3
[00014476] 2e06                      move.l    d6,d7
[00014478] 1e3c 0080                 move.b    #$80,d7
[0001447c] 60be                      bra.s     $0001443C
[0001447e] 1a07                      move.b    d7,d5
[00014480] cb44                      exg       d5,d4
[00014482] 1e06                      move.b    d6,d7
[00014484] 9e86                      sub.l     d6,d7
[00014486] 67e0                      beq.s     $00014468
[00014488] 6aba                      bpl.s     $00014444
[0001448a] 4487                      neg.l     d7
[0001448c] 1805                      move.b    d5,d4
[0001448e] 60b6                      bra.s     $00014446
[00014490] be06                      cmp.b     d6,d7
[00014492] 6602                      bne.s     $00014496
[00014494] be86                      cmp.l     d6,d7
[00014496] 4e75                      rts
[00014498] 4a07                      tst.b     d7
[0001449a] 4e75                      rts
[0001449c] 8efc 0000                 divu.w    #$0000,d7
[000144a0] 4a86                      tst.l     d6
[000144a2] 661c                      bne.s     $000144C0
[000144a4] 8ebc ffff ff7f            or.l      #$FFFFFF7F,d7
[000144aa] 4a07                      tst.b     d7
[000144ac] 003c 0002                 ori.b     #$02,ccr
[000144b0] 4e75                      rts
[000144b2] 4846                      swap      d6
[000144b4] 4847                      swap      d7
[000144b6] bd07                      eor.b     d6,d7
[000144b8] 60ea                      bra.s     $000144A4
[000144ba] 6bfa                      bmi.s     $000144B6
[000144bc] 7e00                      moveq.l   #0,d7
[000144be] 4e75                      rts
[000144c0] 1a06                      move.b    d6,d5
[000144c2] 67d8                      beq.s     $0001449C
[000144c4] 2807                      move.l    d7,d4
[000144c6] 67e8                      beq.s     $000144B0
[000144c8] 7680                      moveq.l   #-128,d3
[000144ca] da45                      add.w     d5,d5
[000144cc] d844                      add.w     d4,d4
[000144ce] b705                      eor.b     d3,d5
[000144d0] b704                      eor.b     d3,d4
[000144d2] 9805                      sub.b     d5,d4
[000144d4] 69e4                      bvs.s     $000144BA
[000144d6] 4207                      clr.b     d7
[000144d8] 4847                      swap      d7
[000144da] 4846                      swap      d6
[000144dc] be46                      cmp.w     d6,d7
[000144de] 6b06                      bmi.s     $000144E6
[000144e0] 5404                      addq.b    #2,d4
[000144e2] 69ce                      bvs.s     $000144B2
[000144e4] e29f                      ror.l     #1,d7
[000144e6] 4847                      swap      d7
[000144e8] 1a03                      move.b    d3,d5
[000144ea] bb44                      eor.w     d5,d4
[000144ec] e24c                      lsr.w     #1,d4
[000144ee] 2607                      move.l    d7,d3
[000144f0] 86c6                      divu.w    d6,d3
[000144f2] 3a03                      move.w    d3,d5
[000144f4] c6c6                      mulu.w    d6,d3
[000144f6] 9e83                      sub.l     d3,d7
[000144f8] 4847                      swap      d7
[000144fa] 4846                      swap      d6
[000144fc] 3606                      move.w    d6,d3
[000144fe] 4203                      clr.b     d3
[00014500] c6c5                      mulu.w    d5,d3
[00014502] 9e83                      sub.l     d3,d7
[00014504] 6408                      bcc.s     $0001450E
[00014506] 2606                      move.l    d6,d3
[00014508] 4203                      clr.b     d3
[0001450a] de83                      add.l     d3,d7
[0001450c] 5345                      subq.w    #1,d5
[0001450e] 2606                      move.l    d6,d3
[00014510] 4843                      swap      d3
[00014512] 4247                      clr.w     d7
[00014514] 8ec3                      divu.w    d3,d7
[00014516] 4845                      swap      d5
[00014518] 6b08                      bmi.s     $00014522
[0001451a] 3a07                      move.w    d7,d5
[0001451c] da85                      add.l     d5,d5
[0001451e] 5304                      subq.b    #1,d4
[00014520] 3e05                      move.w    d5,d7
[00014522] 3a07                      move.w    d7,d5
[00014524] dabc 0000 0080            add.l     #$00000080,d5
[0001452a] 2e05                      move.l    d5,d7
[0001452c] 1e04                      move.b    d4,d7
[0001452e] 678c                      beq.s     $000144BC
[00014530] 4e75                      rts
[00014532] 1a07                      move.b    d7,d5
[00014534] 6752                      beq.s     $00014588
[00014536] 1806                      move.b    d6,d4
[00014538] 6768                      beq.s     $000145A2
[0001453a] da45                      add.w     d5,d5
[0001453c] d844                      add.w     d4,d4
[0001453e] 7680                      moveq.l   #-128,d3
[00014540] b704                      eor.b     d3,d4
[00014542] b705                      eor.b     d3,d5
[00014544] da04                      add.b     d4,d5
[00014546] 695e                      bvs.s     $000145A6
[00014548] 1803                      move.b    d3,d4
[0001454a] b945                      eor.w     d4,d5
[0001454c] e25d                      ror.w     #1,d5
[0001454e] 4845                      swap      d5
[00014550] 3a06                      move.w    d6,d5
[00014552] 4207                      clr.b     d7
[00014554] 4205                      clr.b     d5
[00014556] 3805                      move.w    d5,d4
[00014558] c8c7                      mulu.w    d7,d4
[0001455a] 4844                      swap      d4
[0001455c] 2607                      move.l    d7,d3
[0001455e] 4843                      swap      d3
[00014560] c6c5                      mulu.w    d5,d3
[00014562] d883                      add.l     d3,d4
[00014564] 4846                      swap      d6
[00014566] 2606                      move.l    d6,d3
[00014568] c6c7                      mulu.w    d7,d3
[0001456a] d883                      add.l     d3,d4
[0001456c] 4244                      clr.w     d4
[0001456e] d904                      addx.b    d4,d4
[00014570] 4844                      swap      d4
[00014572] 4847                      swap      d7
[00014574] cec6                      mulu.w    d6,d7
[00014576] 4846                      swap      d6
[00014578] 4845                      swap      d5
[0001457a] de84                      add.l     d4,d7
[0001457c] 6a0c                      bpl.s     $0001458A
[0001457e] debc 0000 0080            add.l     #$00000080,d7
[00014584] 1e05                      move.b    d5,d7
[00014586] 671a                      beq.s     $000145A2
[00014588] 4e75                      rts
[0001458a] 5305                      subq.b    #1,d5
[0001458c] 6914                      bvs.s     $000145A2
[0001458e] 6512                      bcs.s     $000145A2
[00014590] 7840                      moveq.l   #64,d4
[00014592] de84                      add.l     d4,d7
[00014594] de87                      add.l     d7,d7
[00014596] 6404                      bcc.s     $0001459C
[00014598] e297                      roxr.l    #1,d7
[0001459a] 5205                      addq.b    #1,d5
[0001459c] 1e05                      move.b    d5,d7
[0001459e] 6702                      beq.s     $000145A2
[000145a0] 4e75                      rts
[000145a2] 7e00                      moveq.l   #0,d7
[000145a4] 4e75                      rts
[000145a6] 6afa                      bpl.s     $000145A2
[000145a8] bd07                      eor.b     d6,d7
[000145aa] 8ebc ffff ff7f            or.l      #$FFFFFF7F,d7
[000145b0] 4a07                      tst.b     d7
[000145b2] 003c 0002                 ori.b     #$02,ccr
[000145b6] 4e75                      rts
[000145b8] 6d63                      blt.s     $0001461D
[000145ba] 3638 3334                 move.w    ($00003334).w,d3
[000145be] 3320                      move.w    -(a0),-(a1)
[000145c0] 666c                      bne.s     $0001462E
[000145c2] 6f61                      ble.s     $00014625
[000145c4] 7469                      moveq.l   #105,d2
[000145c6] 6e67                      bgt.s     $0001462F
[000145c8] 2070 6f69 6e74            movea.l   ([$6E74,a0,zd6.l*8]),a0 ; 68020+ only
[000145ce] 2066                      movea.l   -(a6),a0
[000145d0] 6972                      bvs.s     $00014644
[000145d2] 6d77                      blt.s     $0001464B
[000145d4] 6172                      bsr.s     $00014648
[000145d6] 6520                      bcs.s     $000145F8
[000145d8] 2863                      movea.l   -(a3),a4
[000145da] 2920                      move.l    -(a0),-(a4)
[000145dc] 636f                      bls.s     $0001464D
[000145de] 7079                      moveq.l   #121,d0
[000145e0] 7269                      moveq.l   #105,d1
[000145e2] 6768                      beq.s     $0001464C
[000145e4] 7420                      moveq.l   #32,d2
[000145e6] 3139 3831 2062            move.w    $38312062,-(a0)
[000145ec] 7920                      mvs.b     -(a0),d4 ; ColdFire only
[000145ee] 6d6f                      blt.s     $0001465F
[000145f0] 746f                      moveq.l   #111,d2
[000145f2] 726f                      moveq.l   #111,d1
[000145f4] 6c61                      bge.s     $00014657
[000145f6] 2069 6e63                 movea.l   28259(a1),a0
[000145fa] 2e00                      move.l    d0,d7

data:
[000145fc]                           dc.w $0001
[000145fe]                           dc.w $0002
[00014600]                           dc.w $0101
[00014602]                           dc.w $0201
[00014604]                           dc.w $0100
[00014606]                           dc.w $0101
[00014608]                           dc.w $0201
[0001460a]                           dc.w $0101
[0001460c]                           dc.w $0101
[0001460e]                           dc.w $0000
[00014610]                           dc.w $0000
[00014612]                           dc.w $0000
[00014614]                           dc.w $0000
[00014616]                           dc.w $0000
[00014618]                           dc.w $0100
[0001461a]                           dc.w $0001
[0001461c]                           dc.w $0003
[0001461e]                           dc.w $0500
[00014620]                           dc.w $0505
[00014622]                           dc.w $0000
[00014624]                           dc.w $0101
[00014626]                           dc.w $0201
[00014628]                           dc.w $0010
[0001462a]                           dc.w $0701
[0001462c]                           dc.w $0201
[0001462e]                           dc.w $0000
[00014630]                           dc.w $0000
[00014632]                           dc.w $0000
[00014634]                           dc.w $0000
[00014636]                           dc.w $0000
[00014638]                           dc.w $0101
[0001463a]                           dc.w $0102
[0001463c]                           dc.w $0101
[0001463e]                           dc.w $0201
[00014640]                           dc.w $0102
[00014642]                           dc.w $0101
[00014644]                           dc.w $0101
[00014646]                           dc.w $0201
[00014648]                           dc.w $0101
[0001464a]                           dc.w $0000
[0001464c]                           dc.w $0000
[0001464e]                           dc.w $0000
[00014650]                           dc.w $0000
[00014652]                           dc.w $0000
[00014654]                           dc.w $0000
[00014656]                           dc.w $0201
[00014658]                           dc.w $0101
[0001465a]                           dc.w $0101
[0001465c]                           dc.w $0601
[0001465e]                           dc.w $0104
[00014660]                           dc.w $0101
[00014662]                           dc.w $0103
[00014664]                           dc.w $0102
[00014666]                           dc.w $0101
[00014668]                           dc.w $0402
[0001466a]                           dc.w $0108
[0001466c]                           dc.w $0101
[0001466e]                           dc.w $0000
[00014670]                           dc.w $0000
[00014672]                           dc.w $0000
[00014674]                           dc.w $0101
[00014676]                           dc.w $0109
[00014678]                           dc.w $0101
[0001467a]                           dc.w $0101
[0001467c]                           dc.w $0101
[0001467e]                           dc.w $0100
[00014680]                           dc.w $0005
[00014682]                           dc.w $0100
[00014684]                           dc.w $0000
[00014686]                           dc.w $0000
[00014688]                           dc.w $0000
[0001468a]                           dc.w $0000
[0001468c]                           dc.w $0000
[0001468e]                           dc.w $0000
[00014690]                           dc.w $0000
[00014692]                           dc.w $0000
[00014694]                           dc.w $0000
[00014696]                           dc.w $0000
[00014698]                           dc.w $0000
[0001469a]                           dc.w $0000
[0001469c]                           dc.w $0000
[0001469e]                           dc.w $0000
[000146a0]                           dc.w $0000
[000146a2]                           dc.w $0000
[000146a4]                           dc.w $0000
[000146a6]                           dc.w $0000
[000146a8]                           dc.w $0000
[000146aa]                           dc.w $0000
[000146ac]                           dc.w $0000
[000146ae]                           dc.w $0000
[000146b0]                           dc.w $0403
[000146b2]                           dc.w $0008
[000146b4]                           dc.w $0300
[000146b6]                           dc.w $0601
[000146b8]                           dc.w $0008
[000146ba]                           dc.w $0100
[000146bc]                           dc.w $0801
[000146be]                           dc.w $0004
[000146c0]                           dc.w $0101
[000146c2]                           dc.w $0301
[000146c4]                           dc.w $0100
[000146c6]                           dc.w $0500
[000146c8]                           dc.w $0101
[000146ca]                           dc.w $0100
[000146cc]                           dc.w $0500
[000146ce]                           dc.w $0001
[000146d0]                           dc.w $0100
[000146d2]                           dc.w $0101
[000146d4]                           dc.w $0000
[000146d6]                           dc.w $0000
[000146d8]                           dc.w $0000
[000146da]                           dc.w $0000
[000146dc]                           dc.w $0000
[000146de]                           dc.w $0000
[000146e0]                           dc.w $0000
[000146e2]                           dc.w $0000
[000146e4]                           dc.w $0000
[000146e6]                           dc.w $0000
[000146e8]                           dc.w $0000
[000146ea]                           dc.w $0000
[000146ec]                           dc.w $0002
[000146ee]                           dc.w $0200
[000146f0]                           dc.w $0000
[000146f2]                           dc.w $0000
[000146f4]                           dc.w $0000
[000146f6]                           dc.w $0000
[000146f8]                           dc.w $0000
[000146fa]                           dc.w $0000
[000146fc]                           dc.w $0000
[000146fe]                           dc.w $0000
[00014700]                           dc.w $0000
[00014702]                           dc.w $0000
[00014704]                           dc.w $0000
[00014706]                           dc.w $0000
[00014708]                           dc.w $0000
[0001470a]                           dc.w $0501
[0001470c]                           dc.w $0005
[0001470e]                           dc.w $0100
[00014710]                           dc.w $0101
[00014712]                           dc.w $0001
[00014714]                           dc.w $0100
[00014716]                           dc.w $0205
[00014718]                           dc.w $0006
[0001471a]                           dc.w $0100
[0001471c]                           dc.w $0201
[0001471e]                           dc.w $0001
[00014720]                           dc.w $0100
[00014722]                           dc.w $0605
[00014724]                           dc.w $0000
[00014726]                           dc.w $0000
[00014728]                           dc.w $0001
[0001472a]                           dc.w $0100
[0001472c]                           dc.w $0100
[0001472e]                           dc.w $0201
[00014730]                           dc.w $0002
[00014732]                           dc.w $0101
[00014734]                           dc.w $0101
[00014736]                           dc.w $0100
[00014738]                           dc.w $0000
[0001473a]                           dc.w $0000
[0001473c]                           dc.w $0000
[0001473e]                           dc.w $0000
[00014740]                           dc.w $0000
[00014742]                           dc.w $0000
[00014744]                           dc.w $0000
[00014746]                           dc.w $0001
[00014748]                           dc.w $0203
[0001474a]                           dc.w $0102
[0001474c]                           dc.w $0101
[0001474e]                           dc.w $0101
[00014750]                           dc.w $0101
[00014752]                           dc.w $0001
[00014754]                           dc.w $0100
[00014756]                           dc.w $0102
[00014758]                           dc.b 'Stack Overflow$C runtime',0
[00014771]                           dc.b 'CON:',0
[00014776]                           dc.b 'LST:',0
[0001477b]                           dc.b $1a
[0001477c]                           dc.w $0001
[0001477e]                           dc.w $484a
[00014780]                           dc.w $0001
[00014782]                           dc.w $50f4
[00014784]                           dc.w $ff65
[00014786]                           dc.w $0000
[00014788]                           dc.w $002d
[0001478a]                           dc.w $0000
[0001478c]                           dc.w $0061
[0001478e]                           dc.w $0000
[00014790]                           dc.w $0062
[00014792]                           dc.w $0000
[00014794]                           dc.w $0064
[00014796]                           dc.w $0000
[00014798]                           dc.w $0069
[0001479a]                           dc.w $0000
[0001479c]                           dc.w $0070
[0001479e]                           dc.w $0000
[000147a0]                           dc.w $0072
[000147a2]                           dc.w $0000
[000147a4]                           dc.w $0074
[000147a6]                           dc.w $0000
[000147a8]                           dc.w $0076
[000147aa]                           dc.w $0000
[000147ac]                           dc.w $0078
[000147ae]                           dc.w $0000
[000147b0]                           dc.w $0000
[000147b2]                           dc.w $0001
[000147b4]                           dc.w $0218
[000147b6]                           dc.w $0001
[000147b8]                           dc.w $01a2
[000147ba]                           dc.w $0001
[000147bc]                           dc.w $01b4
[000147be]                           dc.w $0001
[000147c0]                           dc.w $01c6
[000147c2]                           dc.w $0001
[000147c4]                           dc.w $01b4
[000147c6]                           dc.w $0001
[000147c8]                           dc.w $01fe
[000147ca]                           dc.w $0001
[000147cc]                           dc.w $018e
[000147ce]                           dc.w $0001
[000147d0]                           dc.w $01ec
[000147d2]                           dc.w $0001
[000147d4]                           dc.w $0210
[000147d6]                           dc.w $0001
[000147d8]                           dc.w $01da
[000147da]                           dc.w $0001
[000147dc]                           dc.w $021a
[000147de]                           dc.w $0001
[000147e0]                           dc.w $0100
[000147e2]                           dc.w $0072
[000147e4]                           dc.w $002d
[000147e6]                           dc.w $0001
[000147e8]                           dc.w $0080
[000147ea]                           dc.w $0077
[000147ec]                           dc.w $002d
[000147ee]                           dc.w $0001
[000147f0]                           dc.w $0040
[000147f2]                           dc.w $0078
[000147f4]                           dc.w $002d
[000147f6]                           dc.w $0001
[000147f8]                           dc.w $0020
[000147fa]                           dc.w $0072
[000147fc]                           dc.w $002d
[000147fe]                           dc.w $0001
[00014800]                           dc.w $0010
[00014802]                           dc.w $0077
[00014804]                           dc.w $002d
[00014806]                           dc.w $0001
[00014808]                           dc.w $0008
[0001480a]                           dc.w $0078
[0001480c]                           dc.w $002d
[0001480e]                           dc.w $0001
[00014810]                           dc.w $0004
[00014812]                           dc.w $0072
[00014814]                           dc.w $002d
[00014816]                           dc.w $0001
[00014818]                           dc.w $0002
[0001481a]                           dc.w $0077
[0001481c]                           dc.w $002d
[0001481e]                           dc.w $0001
[00014820]                           dc.w $0001
[00014822]                           dc.w $0078
[00014824]                           dc.w $002d
[00014826]                           dc.w $0001
[00014828]                           dc.w $47de
[0001482a]                           dc.w $0001
[0001482c]                           dc.w $47e6
[0001482e]                           dc.w $0001
[00014830]                           dc.w $47ee
[00014832]                           dc.w $0001
[00014834]                           dc.w $47f6
[00014836]                           dc.w $0001
[00014838]                           dc.w $47fe
[0001483a]                           dc.w $0001
[0001483c]                           dc.w $4806
[0001483e]                           dc.w $0001
[00014840]                           dc.w $480e
[00014842]                           dc.w $0001
[00014844]                           dc.w $4816
[00014846]                           dc.w $0001
[00014848]                           dc.w $481e
[0001484a]                           dc.b '@(#)ar68.c',$09,'2.1',$09,'4/26/85',0
[00014861]                           dc.b 'AR68 Archiver (c) 1985 Alcyon Corporation',$0a,0
[0001488c]                           dc.b 'usage: %s [rdxtpvabi] [pos] archive file [file ...]',$0a,0
[000148c1]                           dc.b '%s: invalid option flag: %c',$0a,0
[000148de]                           dc.b '%s: one and only one of 'rdxt' flags required',$0a,0
[0001490d]                           dc.b '%s: only one of 'abi' flags allowed',$0a,0
[00014932]                           dc.b '%s: 'abi' flags can only be used with 'r' flag',$0a,0
[00014962]                           dc.b '%s: no such file '%s'',$0a,0
[00014979]                           dc.b '%s: warning, changing a random access library',$0a,0
[000149a8]                           dc.b 'art?????',0
[000149b1]                           dc.b $77
[000149b2]                           dc.w $0025
[000149b4]                           dc.b 's: can't create %s',$0a,0
[000149c8]                           dc.b '%s: write error on magic number',0
[000149e8]                           dc.b '%s not found',$0a,0
[000149f6]                           dc.w $7200
[000149f8]                           dc.b '%s: not archive format: %s %x',$0a,0
[00014a17]                           dc.b ' %d/%d ',0
[00014a1f]                           dc.b '%6ld',0
[00014a24]                           dc.b '%s: fseek error',$0a,0
[00014a35]                           dc.b $72
[00014a36]                           dc.w $0025
[00014a38]                           dc.b 's: can't open %s',$0a,0
[00014a4a]                           dc.b '%s: can't read %s',$0a,0
[00014a5d]                           dc.b '%s: %s not in library',$0a,0
[00014a74]                           dc.b '._SYMDEF',0
[00014a7d]                           dc.b '%s: can't create %s',$0a,0
[00014a92]                           dc.w $7700
[00014a94]                           dc.b 'std output',0
[00014a9f]                           dc.b '%s: seek error on library',$0a,0
[00014aba]                           dc.b '%s: write error on %s',0
[00014ad0]                           dc.b '%s',$0a,0
[00014ad4]                           dc.b '%s: write error on CHDR %s',0
[00014aef]                           dc.b '%s',$0a,0
[00014af3]                           dc.b '%s: read error',$0a,0
[00014b03]                           dc.b $00
[00014b04]                           dc.b '%s: %s not in archive file',$0a,0
[00014b20]                           dc.b '._SYMDEF',0
[00014b29]                           dc.b '%s: can't create %s',$0a,'archive left in %s',$0a,0
[00014b51]                           dc.b '%s: failed to open %s',$0a,0
[00014b68]                           dc.w $7200
[00014b6a]                           dc.w $0000
[00014b6c]                           dc.w $0000
[00014b6e]                           dc.b ': unmatched quote',0
[00014b80]                           dc.b 'Cannot open ',0
[00014b8d]                           dc.b 'Cannot append ',0
[00014b9c]                           dc.b 'Cannot create ',0
[00014bab]                           dc.b ': No match',0
[00014bb6]                           dc.b 'Stack Overflow',0
[00014bc5]                           dc.b $00
[00014bc6]                           dc.b $0d,$0a,'$',0
[00014bca]                           dc.w $0000
[00014bcc]                           dc.w $0001
[00014bce]                           dc.w $0000
[00014bd0]                           dc.w $0000
[00014bd2]                           dc.w $0000
[00014bd4]                           dc.w $0000
[00014bd6]                           dc.w $0000
[00014bd8]                           dc.w $0001
[00014bda]                           dc.w $000a
[00014bdc]                           dc.w $0000
[00014bde]                           dc.w $0000
[00014be0]                           dc.w $0000
[00014be2]                           dc.w $0000
[00014be4]                           dc.w $0000
[00014be6]                           dc.w $0002
[00014be8]                           dc.w $000a
[00014bea]                           dc.w $0000
[00014bec]                           dc.w $0000
[00014bee]                           dc.w $0000
[00014bf0]                           dc.w $0000
[00014bf2]                           dc.w $0000
[00014bf4]                           dc.w $0003
[00014bf6]                           dc.w $0000
[00014bf8]                           dc.w $0000
[00014bfa]                           dc.w $0000
[00014bfc]                           dc.w $0000
[00014bfe]                           dc.w $0000
[00014c00]                           dc.w $0000
[00014c02]                           dc.w $0004
[00014c04]                           dc.w $0000
[00014c06]                           dc.w $0000
[00014c08]                           dc.w $0000
[00014c0a]                           dc.w $0000
[00014c0c]                           dc.w $0000
[00014c0e]                           dc.w $0000
[00014c10]                           dc.w $0005
[00014c12]                           dc.w $0000
[00014c14]                           dc.w $0000
[00014c16]                           dc.w $0000
[00014c18]                           dc.w $0000
[00014c1a]                           dc.w $0000
[00014c1c]                           dc.w $0000
[00014c1e]                           dc.w $0006
[00014c20]                           dc.w $0000
[00014c22]                           dc.w $0000
[00014c24]                           dc.w $0000
[00014c26]                           dc.w $0000
[00014c28]                           dc.w $0000
[00014c2a]                           dc.w $0000
[00014c2c]                           dc.w $0007
[00014c2e]                           dc.w $0000
[00014c30]                           dc.w $0000
[00014c32]                           dc.w $0000
[00014c34]                           dc.w $0000
[00014c36]                           dc.w $0000
[00014c38]                           dc.w $0000
[00014c3a]                           dc.w $0008
[00014c3c]                           dc.w $0000
[00014c3e]                           dc.w $0000
[00014c40]                           dc.w $0000
[00014c42]                           dc.w $0000
[00014c44]                           dc.w $0000
[00014c46]                           dc.w $0000
[00014c48]                           dc.w $0009
[00014c4a]                           dc.w $0000
[00014c4c]                           dc.w $0000
[00014c4e]                           dc.w $0000
[00014c50]                           dc.w $0000
[00014c52]                           dc.w $0000
[00014c54]                           dc.w $0000
[00014c56]                           dc.w $000a
[00014c58]                           dc.w $0000
[00014c5a]                           dc.w $0000
[00014c5c]                           dc.w $0000
[00014c5e]                           dc.w $0000
[00014c60]                           dc.w $0000
[00014c62]                           dc.w $0000
[00014c64]                           dc.w $000b
[00014c66]                           dc.w $0000
[00014c68]                           dc.w $0000
[00014c6a]                           dc.w $0000
[00014c6c]                           dc.w $0000
[00014c6e]                           dc.w $0000
[00014c70]                           dc.w $0000
[00014c72]                           dc.w $000c
[00014c74]                           dc.w $0000
[00014c76]                           dc.w $0000
[00014c78]                           dc.w $0000
[00014c7a]                           dc.w $0000
[00014c7c]                           dc.w $0000
[00014c7e]                           dc.w $0000
[00014c80]                           dc.w $000d
[00014c82]                           dc.w $0000
[00014c84]                           dc.w $0000
[00014c86]                           dc.w $0000
[00014c88]                           dc.w $0000
[00014c8a]                           dc.w $0000
[00014c8c]                           dc.w $0000
[00014c8e]                           dc.w $000e
[00014c90]                           dc.w $0000
[00014c92]                           dc.w $0000
[00014c94]                           dc.w $0000
[00014c96]                           dc.w $0000
[00014c98]                           dc.w $0000
[00014c9a]                           dc.w $0000
[00014c9c]                           dc.w $000f
[00014c9e]                           dc.w $0000
[00014ca0]                           dc.w $0000
[00014ca2]                           dc.w $0000
[00014ca4]                           dc.w $0000
[00014ca6]                           dc.w $0000
[00014ca8]                           dc.w $0000
[00014caa]                           dc.w $0001
[00014cac]                           dc.w $2c34
[00014cae]                           dc.w $0001
[00014cb0]                           dc.w $2ad0
[00014cb2]                           dc.w $0001
[00014cb4]                           dc.w $2c50
[00014cb6]                           dc.w $0001
[00014cb8]                           dc.w $2c76
[00014cba]                           dc.w $0001
[00014cbc]                           dc.w $2c9a
[00014cbe]                           dc.w $0001
[00014cc0]                           dc.w $2cbe
[00014cc2]                           dc.w $0001
[00014cc4]                           dc.w $2cbe
[00014cc6]                           dc.w $0001
[00014cc8]                           dc.w $2cbe
[00014cca]                           dc.w $0001
[00014ccc]                           dc.w $2cbe
[00014cce]                           dc.w $0001
[00014cd0]                           dc.w $2cbe
[00014cd2]                           dc.w $0001
[00014cd4]                           dc.w $2cbe
[00014cd6]                           dc.w $0001
[00014cd8]                           dc.w $2cbe
[00014cda]                           dc.w $0001
[00014cdc]                           dc.w $2b7a
[00014cde]                           dc.w $0001
[00014ce0]                           dc.w $2cbe
[00014ce2]                           dc.w $0001
[00014ce4]                           dc.w $2cbe
[00014ce6]                           dc.w $0001
[00014ce8]                           dc.w $2cbe
[00014cea]                           dc.w $0001
[00014cec]                           dc.w $2c22
[00014cee]                           dc.w $0001
[00014cf0]                           dc.w $2cbe
[00014cf2]                           dc.w $0001
[00014cf4]                           dc.w $2b26
[00014cf6]                           dc.w $0001
[00014cf8]                           dc.w $2cbe
[00014cfa]                           dc.w $0001
[00014cfc]                           dc.w $2cbe
[00014cfe]                           dc.w $0001
[00014d00]                           dc.w $2bce
[00014d02]                           dc.w $0001
[00014d04]                           dc.w $2cbe
[00014d06]                           dc.w $0001
[00014d08]                           dc.w $2cbe
[00014d0a]                           dc.w $0001
[00014d0c]                           dc.w $2cbe
[00014d0e]                           dc.w $0001
[00014d10]                           dc.w $2cbe
[00014d12]                           dc.w $0001
[00014d14]                           dc.w $2cbe
[00014d16]                           dc.w $0001
[00014d18]                           dc.w $2cbe
[00014d1a]                           dc.w $0001
[00014d1c]                           dc.w $2cbe
[00014d1e]                           dc.w $0001
[00014d20]                           dc.w $2cbe
[00014d22]                           dc.w $0001
[00014d24]                           dc.w $2cbe
[00014d26]                           dc.w $0001
[00014d28]                           dc.w $2cbe
[00014d2a]                           dc.w $0001
[00014d2c]                           dc.w $2c34
[00014d2e]                           dc.w $0001
[00014d30]                           dc.w $2ad4
[00014d32]                           dc.w $0001
[00014d34]                           dc.w $2c50
[00014d36]                           dc.w $0001
[00014d38]                           dc.w $2c76
[00014d3a]                           dc.w $0001
[00014d3c]                           dc.w $2c9a
[00014d3e]                           dc.w $0001
[00014d40]                           dc.w $2cbe
[00014d42]                           dc.w $0001
[00014d44]                           dc.w $2cbe
[00014d46]                           dc.w $0001
[00014d48]                           dc.w $2cbe
[00014d4a]                           dc.w $0001
[00014d4c]                           dc.w $2cbe
[00014d4e]                           dc.w $0001
[00014d50]                           dc.w $2cbe
[00014d52]                           dc.w $0001
[00014d54]                           dc.w $2cbe
[00014d56]                           dc.w $0001
[00014d58]                           dc.w $2cbe
[00014d5a]                           dc.w $0001
[00014d5c]                           dc.w $2b7e
[00014d5e]                           dc.w $0001
[00014d60]                           dc.w $2cbe
[00014d62]                           dc.w $0001
[00014d64]                           dc.w $2cbe
[00014d66]                           dc.w $0001
[00014d68]                           dc.w $2cbe
[00014d6a]                           dc.w $0001
[00014d6c]                           dc.w $2c22
[00014d6e]                           dc.w $0001
[00014d70]                           dc.w $2cbe
[00014d72]                           dc.w $0001
[00014d74]                           dc.w $2b2a
[00014d76]                           dc.w $0001
[00014d78]                           dc.w $2cbe
[00014d7a]                           dc.w $0001
[00014d7c]                           dc.w $2cbe
[00014d7e]                           dc.w $0001
[00014d80]                           dc.w $2bd2
[00014d82]                           dc.w $0001
[00014d84]                           dc.w $4d82
[00014d86]                           dc.w $0000
[00014d88]                           dc.w $ffff
[00014d8a]                           dc.w $0001
[00014d8c]                           dc.w $4d82
[00014d8e]                           dc.w $0d00
[00014d90]                           dc.w $0001
[00014d92]                           dc.w $38d6
[00014d94]                           dc.w $0001
[00014d96]                           dc.w $3920
[00014d98]                           dc.w $0001
[00014d9a]                           dc.w $389c
[00014d9c]                           dc.w $0001
[00014d9e]                           dc.w $38bc
[00014da0]                           dc.w $0001
[00014da2]                           dc.w $38f4
[00014da4]                           dc.w $0001
[00014da6]                           dc.w $3920
[00014da8]                           dc.w $0001
[00014daa]                           dc.w $3920
[00014dac]                           dc.w $0001
[00014dae]                           dc.w $387a
[00014db0]                           dc.w $0000
[00014db2]                           dc.w $0000
[00014db4]                           dc.w $0101
[00014db6]                           dc.w $0101
[00014db8]                           dc.w $0101
[00014dba]                           dc.w $0101
[00014dbc]                           dc.w $0121
[00014dbe]                           dc.w $2101
[00014dc0]                           dc.w $2121
[00014dc2]                           dc.w $0101
[00014dc4]                           dc.w $0101
[00014dc6]                           dc.w $0101
[00014dc8]                           dc.w $0101
[00014dca]                           dc.w $0101
[00014dcc]                           dc.w $0101
[00014dce]                           dc.w $0101
[00014dd0]                           dc.w $0101
[00014dd2]                           dc.w $0101
[00014dd4]                           dc.w $2202
[00014dd6]                           dc.w $0202
[00014dd8]                           dc.w $0202
[00014dda]                           dc.w $0202
[00014ddc]                           dc.w $0202
[00014dde]                           dc.w $0202
[00014de0]                           dc.w $0202
[00014de2]                           dc.w $0202
[00014de4]                           dc.w $0404
[00014de6]                           dc.w $0404
[00014de8]                           dc.w $0404
[00014dea]                           dc.w $0404
[00014dec]                           dc.w $0404
[00014dee]                           dc.w $0202
[00014df0]                           dc.w $0202
[00014df2]                           dc.w $0202
[00014df4]                           dc.w $0208
[00014df6]                           dc.w $0808
[00014df8]                           dc.w $0808
[00014dfa]                           dc.w $0808
[00014dfc]                           dc.w $0808
[00014dfe]                           dc.w $0808
[00014e00]                           dc.w $0808
[00014e02]                           dc.w $0808
[00014e04]                           dc.w $0808
[00014e06]                           dc.w $0808
[00014e08]                           dc.w $0808
[00014e0a]                           dc.w $0808
[00014e0c]                           dc.w $0808
[00014e0e]                           dc.w $0802
[00014e10]                           dc.w $0202
[00014e12]                           dc.w $0202
[00014e14]                           dc.w $0210
[00014e16]                           dc.w $1010
[00014e18]                           dc.w $1010
[00014e1a]                           dc.w $1010
[00014e1c]                           dc.w $1010
[00014e1e]                           dc.w $1010
[00014e20]                           dc.w $1010
[00014e22]                           dc.w $1010
[00014e24]                           dc.w $1010
[00014e26]                           dc.w $1010
[00014e28]                           dc.w $1010
[00014e2a]                           dc.w $1010
[00014e2c]                           dc.w $1010
[00014e2e]                           dc.w $1002
[00014e30]                           dc.w $0202
[00014e32]                           dc.w $0201
[00014e34]                           dc.w $0000
[00014e36]                           dc.w $0000
[00014e38]                           dc.w $0001
[00014e3a]                           dc.w $3b6c
[00014e3c]                           dc.w $0001
[00014e3e]                           dc.w $3bb0
[00014e40]                           dc.w $0001
[00014e42]                           dc.w $3bc4
[00014e44]                           dc.w $0001
[00014e46]                           dc.w $3c40
[00014e48]                           dc.w $0001
[00014e4a]                           dc.w $3c0e
[00014e4c]                           dc.w $0001
[00014e4e]                           dc.w $3c40
[00014e50]                           dc.w $0001
[00014e52]                           dc.w $3bf0
[00014e54]                           dc.b 'CP/M-68K(tm), Version 1.2, Copyright (c) 1983, Digital Research XXXX-0000-654321',0
[00014ea5]                           dc.b $00


0x000100fa - 0x00011842 <unknown>
0x00011842 - 0x000118b4 lseek.o
             0x00011842 _lseek
             0x0001189e _tell
0x000118b4 - 0x00011a14 channel5.o
             0x000118b4 _maxfile
             0x000118bc __allocc
             0x00011906 __freec
             0x00011922 __chinit
             0x00011944 ___chini
             0x000119b2 __chkc
0x00011a14 - 0x00011ac4 fdopen.o
0x00011ac4 - 0x00011c2a fopen.o
             0x00011ac4 __fopen
             0x00011be0 _fopen
             0x00011bf8 _fopena
             0x00011c10 _fopenb
0x00011c2a - 0x00011c78 fread.o
             0x00011c2a _fread
0x00011c78 - 0x00011cc8 fwrite.o
             0x00011c78 _fwrite
0x00011cc8 - 0x00011d60 xmain.o
             0x00011cc8 __main
0x00011d60 - 0x00011d6c getpid.o
             0x00011d60 _getpid
0x00011d6c - 0x00011dae printf.o
             0x00011d6c _printf
             0x00011d8e _fprintf
0x00011dae - 0x00011de6 fgetc.o
             0x00011dae _fgetc
0x00011de6 - 0x00011edc filbuf.o
             0x00011de6 __filbuf
0x00011edc - 0x00011f64 read.o
             0x00011edc _read
0x00011f64 - 0x00012086 readasc.o
             0x00011f64 __rdasc
             0x00012044 __cr_col
0x00012086 - 0x000120f4 readbin.o
             0x00012086 __rdbin
0x000120f4 - 0x0001213a unlink.o
             0x000120f4 _unlink
0x0001213a - 0x000124ca xwmain.o
             0x0001213a ___main
0x000124ca - 0x000125ca creat.o
             0x000124ca __creat
             0x00012580 _creat
             0x00012598 _creata
             0x000125b0 _creatb
0x000125ca - 0x000125e2 exit.o
             0x000125ca _exit
0x000125e2 - 0x00012614 cleanup.o
             0x000125e2 __cleanu
0x00012614 - 0x0001266c fclose.o
             0x00012614 _fclose
0x0001266c - 0x000126f4 close.o
             0x0001266c _close
0x000126f4 - 0x000126fc fdecls.o
             0x000126f4 ___fdecl
0x000126fc - 0x00012796 fflush.o
             0x000126fc _fflush
0x00012796 - 0x000128da open.o
             0x00012796 __open
             0x00012890 _open
             0x000128a8 _opena
             0x000128c0 _openb
0x000128da - 0x00012932 sprintf.o
             0x000128da _sprintf
0x00012932 - 0x00012dbe doprt.o
             0x00012932 __doprt
0x00012dbe - 0x00012e78 xdoprtfp.o
             0x00012dbe __pftoa
             0x00012df6 __petoa
             0x00012e2e __pgtoa
0x00012e78 - 0x00012ee8 fputn.o
             0x00012e78 _fputn
0x00012ee8 - 0x00012f2e fputc.o
             0x00012ee8 _fputc
0x00012f2e - 0x0001307c flsbuf.o
             0x00012f2e __flsbuf
0x0001307c - 0x000130f8 isatty.o
             0x0001307c _isatty
             0x000130b0 _isdev
             0x000130dc _ttyname
0x000130f8 - 0x00013404 malloc.o
             0x000130f8 _malloc
             0x00013286 _free
             0x00013384 _realloc
0x00013404 - 0x00013414 mallocnd.o
             0x00013404 _malloc_
             0x0001340c __errmal
0x00013414 - 0x00013486 prtint.o
             0x00013414 ___prtin
0x00013486 - 0x0001353c prtld.o
             0x00013486 ___prtld
0x0001353c - 0x000135aa prtshort.o
             0x0001353c ___prtsh
0x000135aa - 0x00013614 sbrk.o
             0x000135aa _sbrk
0x00013614 - 0x00013692 write.o
             0x00013614 _write
0x00013692 - 0x000137a2 writeasc.o
             0x00013692 __wrtasc
0x000137a2 - 0x0001381c writebin.o
             0x000137a2 __wrtbin
0x0001381c - 0x00013960 xopen.o
             0x0001381c ___open
             0x0001392c _ucase
0x00013960 - 0x0001398a strcat.o
             0x00013960 _strcat
0x0001398a - 0x000139cc strcmp.o
             0x0001398a _strcmp
0x000139cc - 0x000139ee strcpy.o
             0x000139cc _strcpy
0x000139ee - 0x00013a14 strlen.o
             0x000139ee _strlen
0x00013a14 - 0x00013a1c ctype.o
             0x00013a14 ____atab
0x00013a1c - 0x00013a8e stricmp.o
             0x00013a1c __strcmp
0x00013a8e - 0x00013abc salloc.o
             0x00013a8e __salloc
0x00013abc - 0x00013b38 uldiv.o
             0x00013abc _uldiv
0x00013b38 - 0x00013c78 signal.o
             0x00013b38 _signal
             0x00013c4c __setvec
0x00013c78 - 0x00013ccc xsignal.o
             0x00013c78 __illins
             0x00013c88 __trace
             0x00013c94 __trap
             0x00013ca0 __buserr
             0x00013cae __arith
0x00013ccc - 0x00013cdc blivot.o
             0x00013ccc _sw_
0x00013cdc - 0x00013d18 xread.o
             0x00013cdc __pc_rea
0x00013d18 - 0x00013d54 xwrite.o
             0x00013d18 __pc_wri
0x00013d54 - 0x00013d64 trap.o
             0x00013d54 _trap
0x00013d64 - 0x00013db8 lmul.o
             0x00013d64 lmul
0x00013db8 - 0x00013fb6 ftoa.o
             0x00013db8 _ftoa
0x00013fb6 - 0x000141ca etoa.o
             0x00013fb6 _etoa
0x000141ca - 0x000141ea fpadd.o
             0x000141ca fpadd
             0x000141ca _fpadd
0x000141ea - 0x00014208 fpcmp.o
             0x000141ea fpcmp
             0x000141ea _fpcmp
0x00014208 - 0x00014228 fpdiv.o
             0x00014208 fpdiv
             0x00014208 _fpdiv
0x00014228 - 0x000142b8 ltof.o
             0x00014228 _fpltof
0x000142b8 - 0x00014340 ftol.o
             0x000142b8 _fpftol
0x00014340 - 0x00014360 fpmul.o
             0x00014340 fpmul
             0x00014340 _fpmul
             0x00014340 fpmult
             0x00014340 _fpmult
0x00014360 - 0x0001437c fpneg.o
             0x00014360 fpneg
             0x00014360 _fpneg
0x0001437c - 0x0001439c fpsub.o
             0x0001437c fpsub
             0x0001437c _fpsub
0x0001439c - 0x000143ac ffpabs.o
             0x0001439c ffpabs
             0x000143a2 ffpneg
0x000143ac - 0x00014490 ffpadd.o
             0x000143be ffpadd
             0x000143ac ffpsub
0x00014490 - 0x0001449c <unknown>
0x0001449c - 0x00014532 ffpdiv.o
             0x000144c0 ffpdiv
0x00014532 - 0x000145b8 ffpmul2.o
             0x00014532 ffpmul2
0x000145b8 - 0x000145fc ffpcpyrt.o
             0x000145b8 ffpcpyrt


00014780 _lp
00014784 _libmagi
00014EA6 __base
00014EAA __break
00014ED6 _buff
000150D6 _pflg
000150D8 _arfp
000150DC _rflg
000150DE _tflg
000150E0 _uflg
000150E2 _vflg
000150E4 _xflg
000150E6 _tempnam
000150EA _arname
000150EE _tempfd
000150f2 _exitstat
000150f4 _libhd
0001510E _psname
00015144 _myname
00015148 _areof
0001518A _couthd
000151A6 _docom
000151AA _aflg
000151AC _bflg
000151AE _psflg
00015CC8 _dflg
00015CCA _matchflg
