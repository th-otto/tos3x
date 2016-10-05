.globl _dr
.globl _p_timeda
.globl _ww_win
.globl _d_nrows
.globl _d_level
.globl _d_path
.globl _gl_hchar
.globl _gl_wchar
.globl _gl_hbox
.globl _gl_wbox
.globl _gl_nrows
.globl _gl_ncols
.globl _pxyarray
.globl _d_xywh
.globl _f_xywh
.globl _menu_add
.globl _backgrou
.globl _dicon
.globl _r_dicon
.globl _iconaddr
.globl _maxicon
.globl _i_status
.globl _gh_buffe
.globl _backid
.globl _appnode
.globl _appfree
.globl _applist
.globl _path1
.globl _path2
.globl _path3
.globl _inf_path
.globl _dtabuf
.globl _warray
.globl _s_sort
.globl _s_view
.globl _ccopy_sa
.globl _cdele_sa
.globl _write_sa
.globl _cbit_sav
.globl _pref_sav
.globl _s_cache
.globl _s_stofit
.globl _windspec
.globl _autofile
.globl _path2
.globl _g_buffer
.globl _comtail
.globl _winpd
.globl _winhead
.globl _full
.globl _fobj
.globl _deskp
.globl _winp
.globl _wildext
.globl _infdata
.globl _infpath
.globl _wilds
.globl _icndata
.globl _noext
.globl _getall
.globl _bckslsh
.globl _curall
.globl _baklvl
.globl _Nextline
.globl _Nostr
.globl _d_displa
.globl _f_rename
.globl _o_win
.globl _d_dir
.globl _x_win
.globl _x_type
.globl _gl_resty
.globl _gl_rscha
.globl _d_exit
.globl _gl_vdo
.globl _pref_sav
.globl _m_cpu
.globl _s_defdir
.globl _s_fullpa
.globl _numicon
.globl _x_status
.globl _x_type
.globl _mkeys
.globl _restable
.globl _ftab
.globl _font_sav
.globl _m_st
.globl _tb3
.globl _mentable
.globl _d_rezwor
.globl _Menu
.comm _Menu,12
.globl _MData
.comm _MData,12
.globl _mtree
.comm _mtree,744
.globl _ChkTouch
.text
_ChkTouch:
~~ChkTouch:
~tree=8
~button=12
link R14,#-24
~mk=-8
~dummy=-10
~obj=-12
~rect=-20
*line 135
move #3,(sp)
jsr _wind_upd
*line 136
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _XSelect
addq.l #4,sp
*line 138
pea -2(R14)
pea -4(R14)
pea -6(R14)
pea -8(R14)
jsr _graf_mks
adda.w #16,sp
*line 139
bra L4
L3:
*line 140
*line 141
move.l -8(R14),-(sp)
move #8,-(sp)
clr -(sp)
move.l 8(R14),-(sp)
jsr _objc_fin
adda.w #12,sp
move R0,-12(R14)
*line 142
*line 143
move -12(R14),R0
cmp 12(R14),R0
beq L5
*line 144
*line 145
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
btst #0,11(R8)
beq L6
*line 145
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _XDeselec
addq.l #4,sp
L6:
*line 146
bra L7
L5:
*line 147
*line 148
*line 149
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
btst #0,11(R8)
bne L8
*line 149
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _XSelect
addq.l #4,sp
L8:L7:
*line 150
*line 151
pea -2(R14)
pea -4(R14)
pea -6(R14)
pea -8(R14)
jsr _graf_mks
adda.w #16,sp
L4:
*line 152
tst -4(R14)
bne L3
L2:
*line 153
pea -10(R14)
pea -10(R14)
pea -10(R14)
pea -10(R14)
clr -(sp)
move.l #$00010001,-(sp)
jsr _evnt_but
adda.w #22,sp
*line 154
move.l -8(R14),-(sp)
move #8,-(sp)
clr -(sp)
move.l 8(R14),-(sp)
jsr _objc_fin
adda.w #12,sp
move R0,-12(R14)
*line 155
*line 156
move -12(R14),R0
cmp 12(R14),R0
beq L9
*line 156
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _XDeselec
addq.l #4,sp
*line 157
L9:
*line 158
move #2,(sp)
jsr _wind_upd
*line 159
move -12(R14),R0
cmp 12(R14),R0
beq L10000
clr R0
bra L10001
L10000:move #1,R0
L10001:bra L1
L1:unlk R14
rts
.globl _mv_desk
.text
_mv_desk:
~~mv_desk:
link R14,#-90
movem.l R3-R7/R12-R13,-(sp)
~obj=R13
~f=R7
~fi=R6
~i=R5
~which=R4
~menui=-2
~app=R12
~len=-4
~xapp=-8
~str=-12
~temp=-42
~obj1=-46
~buf=-48
~dtree=-52
~dstart=-54
~istart=-56
~brect=-64
~flag=-66
~trect=-74
~dcolor=-76
~mk=-84
~skip_fla=-86
~old_f=-90
*line 193
move #1,-86(R14)
*line 194
move #26,(sp)
jsr _get_tree
move.l R0,R13
*line 195
move.l _menu_add,-46(R14)
*line 197
move #25,(sp)
jsr _get_tree
move.l R0,-52(R14)
*line 200
clr R5
bra L13
L14:
*line 201
move R5,R8
add.l R14,R8
move R5,R9
add.l #_mentable,R9
move.b (R9),-42(R8)
L12:
*line 200
add #1,R5
L13:
*line 200
cmp #29,R5
blt L14
L11:
*line 204
move.l 420(R13),R8
move _tb3,R1
muls #24,R1
add.l -46(R14),R1
move.l R1,R9
move.l 12(R9),(R8)
*line 205
clr.b -47(R14)
*line 206
move.b _mentable,-48(R14)
*line 207
pea -48(R14)
move #20,-(sp)
move.l R13,-(sp)
jsr _inf_sset
add.w #10,sp
*line 208
clr -2(R14)
*line 211
clr (sp)
move #29,-(sp)
move.l -52(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 212
clr (sp)
move #30,-(sp)
move.l -52(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 213
clr (sp)
move #32,-(sp)
move.l -52(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 214
clr (sp)
move #33,-(sp)
move.l -52(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 216
move.l -52(R14),R8
clr 706(R8)
*line 217
move.l -52(R14),R8
clr 730(R8)
*line 218
move.l -52(R14),R8
clr 778(R8)
*line 219
move.l -52(R14),R8
clr 802(R8)
*line 222
*line 223
tst _s_defdir
beq L15
*line 224
move #29,-54(R14)
*line 225
move #28,(sp)
jsr _get_fstr
move.l R0,84(R13)
*line 226
bra L16
L15:
*line 227
*line 228
move #30,-54(R14)
*line 229
move #29,(sp)
jsr _get_fstr
move.l R0,84(R13)
L16:
*line 231
move #1,(sp)
move -54(R14),-(sp)
move.l -52(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 235
*line 236
tst _s_fullpa
beq L17
*line 237
move #32,-56(R14)
*line 238
move #30,(sp)
jsr _get_fstr
move.l R0,132(R13)
*line 239
bra L18
L17:
*line 240
*line 241
move #33,-56(R14)
*line 242
move #31,(sp)
jsr _get_fstr
move.l R0,132(R13)
L18:
*line 244
move #1,(sp)
move -56(R14),-(sp)
move.l -52(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 248
jsr _av_mem
move.l R0,(sp)
move #22,-(sp)
move.l R13,-(sp)
jsr _f_str
addq.l #6,sp
*line 249
move.l 276(R13),R8
move.l (R8),R8
clr.b (R8)
*line 252
move.l 228(R13),R8
move.l (R8),-12(R14)
*line 253
move.l -12(R14),(sp)
jsr _strlen
move R0,-4(R14)
*line 254
move.l -12(R14),R8
clr.b (R8)
*line 256
clr.l R12
*line 257
move #13,R4
*line 259
clr.l R7
move.l R7,-90(R14)
*line 260
clr (sp)
move #26,-(sp)
jsr _fm_draw
addq.l #2,sp
*line 263
move.l #$1,R7
bra L21
L22:
*line 264
*line 265
*line 266
move.l R7,R8
add.l R8,R8
move.l #_ftab,R9
move 0(R8,R9.l),(sp)
jsr _app_key
tst.l R0
beq L23
*line 267
move.l R7,-90(R14)
bra L19
*line 268
*line 269
L23:L20:
*line 263
add.l #$1,R7
L21:
*line 263
cmp.l #$14,R7
ble L22
L19:
*line 271
clr.l R7
bra L24
*line 276
bra L27
L26:
*line 277
L28:
*line 278
move #20,(sp)
move.l R13,-(sp)
jsr _form_do
addq.l #4,sp
move R0,R4
and #32767,R4
*line 279
move.l R13,R8
move R4,R1
muls #24,R1
add.l R1,R8
and #-2,10(R8)
*line 280
*line 281
*line 282
cmp #18,R4
beq L10002
cmp #19,R4
beq L10002
cmp #23,R4
bne L29
L10002:*line 283
pea -48(R14)
move #20,-(sp)
move.l R13,-(sp)
jsr _inf_sget
add.w #10,sp
*line 284
move.b -48(R14),R0
ext.w R0
move R0,(sp) 
jsr _toupper
move.b R0,-48(R14)
*line 285
*line 286
tst.b -48(R14)
beq L30
*line 287
clr R5
bra L33
L34:
*line 288
*line 289
*line 290
cmp -2(R14),R5
beq L32
*line 291
*line 292
*line 293
move.l #_mentable,R8
move.b 0(R8,R5),R0
ext.w R0
cmp.b -48(R14),R0
bne L35
*line 294
*line 295
move #116,(sp)
jsr _do1_aler
cmp #1,R0
bne L36
*line 296
move R5,R8
add.l #_mentable,R8
clr.b (R8)
bra L32
*line 297
*line 298
*line 299
L36:
*line 300
*line 301
cmp #23,R4
bne L37
*line 301
move #23,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
*line 302
L37:bra L38
*line 303
*line 304
L35:L32:
*line 287
add #1,R5
L33:
*line 287
cmp #29,R5
blt L34
L31:
*line 305
*line 306
*line 307
L30:
*line 308
move.l #_mentable,R8
move -2(R14),R9
add.l R9,R8
move.b -48(R14),(R8)
*line 309
*line 310
L29:L24:
*line 311
move R4,R0
bra L40
*line 312
L41:
*line 313
*line 314
*line 315
pea -2(R14)
move #17,-(sp)
move.l R13,-(sp)
jsr _DoMKItem
add.w #10,sp
tst R0
bne L42
bra L39
*line 316
*line 317
L43:
*line 318
*line 319
*line 320
move #21,(sp)
move.l R13,-(sp)
jsr _ChkTouch
addq.l #4,sp
tst R0
beq L44
*line 321
*line 322
clr R5
bra L47
L48:
*line 323
move R5,R8
add.l #_mentable,R8
clr.b (R8)
L46:
*line 322
add #1,R5
L47:
*line 322
cmp #29,R5
blt L48
L45:
*line 323
*line 324
*line 325
jsr _put_keys
*line 326
move #21,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L42
*line 327
*line 328
L44:bra L39
*line 329
*line 330
L49:
*line 331
*line 332
L50:
*line 333
*line 334
*line 335
tst -2(R14)
beq L51
*line 336
move R4,(sp)
move.l R13,-(sp)
jsr _XSelect
addq.l #4,sp
*line 337
sub #1,-2(R14)
bra L42
*line 338
*line 339
L51:
*line 340
jsr _wait_up
*line 341
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L39
*line 342
*line 343
L52:
*line 344
*line 345
L53:
*line 346
*line 347
*line 348
move -2(R14),R0
add #1,R0
cmp #29,R0
bge L54
*line 349
move R4,(sp)
move.l R13,-(sp)
jsr _XSelect
addq.l #4,sp
*line 350
add #1,-2(R14)
L42:
*line 351
*line 352
move -2(R14),(sp)
jsr _ch_key
*line 353
move.l 420(R13),R8
move -2(R14),R9
add.l R9,R9
add.l #_tb3,R9
move (R9),R1
muls #24,R1
add.l -46(R14),R1
move.l R1,R9
move.l 12(R9),(R8)
*line 354
move #17,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
L38:
*line 355
*line 356
clr.b -47(R14)
*line 357
move.l #_mentable,R8
move -2(R14),R9
add.l R9,R8
move.b (R8),-48(R14)
*line 358
pea -48(R14)
move #20,-(sp)
move.l R13,-(sp)
jsr _inf_sset
add.w #10,sp
*line 359
move #20,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
*line 360
jsr _cl_delay
*line 361
*line 362
*line 363
pea -78(R14)
pea -80(R14)
pea -82(R14)
pea -84(R14)
jsr _graf_mks
adda.w #16,sp
*line 364
*line 365
tst -80(R14)
beq L55
*line 366
*line 367
*line 368
cmp #18,R4
beq L50
*line 369
*line 370
*line 371
cmp #19,R4
beq L53
*line 372
L55:
*line 373
L54:
*line 374
jsr _wait_up
*line 375
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L39
*line 376
*line 377
L56:
*line 378
*line 379
clr (sp)
move.l #$001D001C,-(sp)
clr -(sp)
pea -54(R14)
move -54(R14),-(sp)
move #28,-(sp)
move.l -52(R14),-(sp)
move.l #$00030002,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L39
*line 380
*line 381
L57:
*line 382
*line 383
clr (sp)
move.l #$0020001E,-(sp)
clr -(sp)
pea -56(R14)
move -56(R14),-(sp)
move #31,-(sp)
move.l -52(R14),-(sp)
move.l #$00050004,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L39
*line 384
*line 385
L58:
*line 386
*line 387
clr R5
bra L61
L62:
*line 388
move R5,R8
add.l #_mentable,R8
move R5,R9
add.l R14,R9
move.b -42(R9),(R8)
L60:
*line 387
add #1,R5
L61:
*line 387
cmp #29,R5
blt L62
L59:
*line 388
*line 389
bra L63
*line 390
*line 391
L64:
*line 392
*line 393
cmp #29,-54(R14)
bne L10003
move #1,R0
bra L10005
L10003:clr R0
L10005:move R0,_s_defdir
*line 394
cmp #32,-56(R14)
bne L10006
move #1,R0
bra L10008
L10006:clr R0
L10008:move R0,_s_fullpa
bra L63
*line 395
*line 396
L65:
*line 397
*line 398
L66:
*line 399
*line 400
sub.l #$1,R7
*line 401
*line 402
*line 403
tst.l R7
bne L67
*line 404
*line 405
*line 406
*line 407
tst.l -90(R14)
beq L68
*line 407
move.l -90(R14),R7
*line 408
bra L69
L68:
*line 409
add.l #$1,R7
L69:
*line 410
*line 411
jsr _wait_up
*line 412
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L39
*line 413
*line 414
L70:
*line 415
*line 416
L71:
*line 417
*line 418
add.l #$1,R7
*line 419
*line 420
*line 421
cmp.l #$14,R7
bgt L72
*line 422
L67:
*line 423
*line 424
move.l R7,R8
add.l R8,R8
move.l #_ftab,R9
move 0(R8,R9.l),(sp)
jsr _app_key
move.l R0,-8(R14)
bne L73
bra L24
*line 425
*line 426
bra L74
L73:
*line 427
*line 428
*line 429
*line 430
tst -86(R14)
bne L75
*line 431
move.l R7,-90(R14)
*line 432
move R4,(sp)
move.l R13,-(sp)
jsr _XSelect
addq.l #4,sp
*line 433
L75:
*line 434
move.l -8(R14),R12
*line 435
clr R6
*line 436
move.l 228(R13),R8
move.l 6(R12),(R8)
*line 437
move #9,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
*line 438
move.l R7,(sp)
move #11,-(sp)
move.l R13,-(sp)
jsr _f_str
addq.l #6,sp
*line 439
move #11,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
*line 440
*line 441
*line 442
tst -86(R14)
bne L76
*line 443
jsr _cl_delay
*line 444
*line 445
pea -78(R14)
pea -80(R14)
pea -82(R14)
pea -84(R14)
jsr _graf_mks
adda.w #16,sp
*line 446
*line 447
tst -80(R14)
beq L77
*line 448
*line 449
cmp #12,R4
beq L66
*line 450
*line 451
cmp #13,R4
beq L71
*line 452
L77:
*line 453
L76:L74:
*line 454
*line 455
bra L78
L72:
*line 456
*line 457
*line 458
tst.l -90(R14)
beq L79
*line 458
move.l -90(R14),R7
*line 459
bra L80
L79:
*line 460
sub.l #$1,R7
L80:L78:
*line 461
*line 462
*line 463
*line 464
*line 465
tst -86(R14)
bne L81
*line 466
jsr _wait_up
*line 467
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
*line 468
L81:
*line 469
clr -86(R14)
bra L39
*line 470
*line 471
L82:
*line 472
L83:
*line 473
*line 474
L84:
*line 475
*line 476
*line 477
*line 478
move.l R12,R0
bne L85
*line 479
jsr _wait_up
*line 480
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L27
*line 481
*line 482
*line 483
L85:
*line 484
*line 485
cmp #8,R4
bne L86
*line 486
*line 487
tst R6
ble L87
*line 488
sub #1,R6
*line 489
bra L88
L87:
*line 490
*line 491
jsr _wait_up
*line 492
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L27
*line 493
L88:
*line 494
*line 495
bra L89
L86:
*line 496
*line 497
add #1,R6
*line 498
*line 499
move R6,R0
ext.l R0
add.l 6(R12),R0
move.l R0,(sp)
jsr _strlen
cmp -4(R14),R0
bge L90
*line 500
sub #1,R6
*line 501
jsr _wait_up
*line 502
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L27
*line 503
*line 504
L90:L89:
*line 505
L91:
*line 506
*line 507
move R4,(sp)
move.l R13,-(sp)
jsr _XSelect
addq.l #4,sp
*line 508
move R6,R0
ext.l R0
add.l 6(R12),R0
move.l 228(R13),R9
move.l R0,(R9)
*line 509
move #9,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
*line 510
*line 511
*line 512
pea -78(R14)
pea -80(R14)
pea -82(R14)
pea -84(R14)
jsr _graf_mks
adda.w #16,sp
*line 513
*line 514
tst -80(R14)
bne L84
*line 515
*line 516
jsr _wait_up
*line 517
move R4,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L28
*line 518
*line 519
bra L39
L40:sub #3,R0
cmp #21,R0
bhi L39
asl #2,R0
move R0,R8
add.l #L92,R8
move.l (R8),R8
jmp (R8)
.data
L92:.dc.l L56
.dc.l L39
.dc.l L57
.dc.l L39
.dc.l L39
.dc.l L82
.dc.l L39
.dc.l L83
.dc.l L39
.dc.l L65
.dc.l L70
.dc.l L39
.dc.l L39
.dc.l L39
.dc.l L41
.dc.l L49
.dc.l L52
.dc.l L39
.dc.l L43
.dc.l L39
.dc.l L64
.dc.l L58
.text
L39:
*line 520
*line 521
*line 522
jsr _cl_delay
L27:
*line 523
bra L26
L25:L63:
*line 525
*line 526
jsr _put_keys
*line 527
move.l 228(R13),R8
move.l -12(R14),(R8)
*line 528
move.l -12(R14),R8
move.b #1,(R8)
*line 529
move #26,(sp)
jsr _do_finis
L10:tst.l (sp)+
movem.l (sp)+,R4-R7/R12-R13
unlk R14
rts
.globl _mins_app
.text
_mins_app:
~~mins_app:
link R14,#-86
movem.l R7-R7/R12-R13,-(sp)
~obj=R13
~app=R12
~sapp=-4
~install=-6
~ret=-8
~newapp=-10
~cont=-12
~setdir=-14
~dofull=-16
~where=-18
~type=-20
~sret=-22
~icon=-24
~graphic=-26
~str=-30
~buffer=-38
~l=-42
~rect=-50
~dtree=-54
~brect=-62
~trect=-70
~flag=-72
~istart=-74
~bstart=-76
~fstart=-78
~dstart=-80
~pstart=-82
~i=-84
~done=-86
*line 564
move #1,-12(R14)
*line 565
clr.l R13
*line 566
move #25,(sp)
jsr _get_tree
move.l R0,-54(R14)
*line 568
pea -20(R14)
pea -30(R14)
jsr _x_first
addq.l #8,sp
move R0,-12(R14)
*line 570
bra L96
L95:
*line 571
*line 572
*line 573
cmp #1,-20(R14)
beq L10009
cmp #8,-20(R14)
bne L97
L10009:*line 574
*line 575
pea -6(R14)
move.l -30(R14),-(sp)
jsr _app_xtyp
addq.l #8,sp
move.l R0,R12
*line 576
*line 577
*line 578
tst -6(R14)
bne L97
*line 579
*line 580
move #21,(sp)
jsr _get_tree
move.l R0,R13
*line 581
*line 582
*line 583
move 2(R12),-24(R14)
*line 584
*line 585
*line 586
move #42,(sp)
move.l 6(R12),-(sp)
jsr _scasb
addq.l #4,sp
move.l R0,R8
cmp.b #42,(R8)
bne L98
*line 586
move #1,-10(R14)
*line 587
bra L99
L98:
*line 588
clr -10(R14)
L99:
*line 589
*line 590
*line 591
tst.b _autofile
bne L100
*line 591
clr -8(R14)
bra L101
L100:
*line 592
*line 593
move.l #3+_autofile,(sp)
move.l -30(R14),-(sp)
jsr _strcmp
addq.l #4,sp
move R0,-8(R14)
L101:
*line 594
*line 595
*line 596
tst -8(R14)
beq L10010
move #1,R0
bra L10012
L10010:clr R0
L10012:move R0,-22(R14)
*line 597
move.l -54(R14),R8
clr 850(R8)
*line 598
move.l -54(R14),R8
clr 874(R8)
*line 599
clr (sp)
move #35,-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 600
clr (sp)
move #36,-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 601
*line 602
tst -8(R14)
beq L102
*line 603
move #35,-76(R14)
*line 604
move #41,(sp)
jsr _get_fstr
move.l R0,204(R13)
*line 605
bra L103
L102:
*line 606
*line 607
move #36,-76(R14)
*line 608
move #42,(sp)
jsr _get_fstr
move.l R0,204(R13)
L103:
*line 609
*line 610
move #1,(sp)
move -76(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 611
*line 612
*line 613
*line 614
move #2,-84(R14)
bra L106
L107:
*line 615
*line 616
move -84(R14),R0
muls #24,R0
add.l -54(R14),R0
move.l R0,R8
clr 10(R8)
*line 617
clr (sp)
move -84(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
L105:
*line 614
add #1,-84(R14)
L106:
*line 614
cmp #22,-84(R14)
ble L107
L104:
*line 618
*line 619
*line 620
*line 621
*line 622
move.l 6(R12),(sp)
move.l -30(R14),-(sp)
jsr _strcmp
addq.l #4,sp
tst R0
beq L108
*line 623
btst #1,25(R12)
beq L10013
move #1,R0
bra L10015
L10013:clr R0
L10015:move R0,-16(R14)
*line 624
btst #0,25(R12)
beq L10016
move #1,R0
bra L10018
L10016:clr R0
L10018:move R0,-14(R14)
*line 625
bra L109
L108:
*line 626
*line 627
move _s_fullpa,-16(R14)
*line 628
move _s_defdir,-14(R14)
L109:
*line 629
*line 630
*line 631
*line 632
*line 633
move.l -54(R14),R8
clr 706(R8)
*line 634
move.l -54(R14),R8
clr 730(R8)
*line 635
clr (sp)
move #29,-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 636
clr (sp)
move #30,-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 637
*line 638
tst -14(R14)
beq L110
*line 639
move #29,-80(R14)
*line 640
move #28,(sp)
jsr _get_fstr
move.l R0,300(R13)
*line 641
bra L111
L110:
*line 642
*line 643
move #30,-80(R14)
*line 644
move #29,(sp)
jsr _get_fstr
move.l R0,300(R13)
L111:
*line 645
*line 646
move #1,(sp)
move -80(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 647
*line 648
*line 649
*line 650
*line 651
move.l -54(R14),R8
clr 778(R8)
*line 652
move.l -54(R14),R8
clr 802(R8)
*line 653
clr (sp)
move #32,-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 654
clr (sp)
move #33,-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 655
*line 656
tst -16(R14)
beq L112
*line 657
move #32,-82(R14)
*line 658
move #30,(sp)
jsr _get_fstr
move.l R0,348(R13)
*line 659
bra L113
L112:
*line 660
*line 661
move #33,-82(R14)
*line 662
move #31,(sp)
jsr _get_fstr
move.l R0,348(R13)
L113:
*line 663
*line 664
move #1,(sp)
move -82(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 665
*line 666
*line 667
*line 668
tst -10(R14)
beq L10019
move.l #_Nostr,(sp)
bra L10021
L10019:pea 12(R12)
L10021:move #4,-(sp)
move.l R13,-(sp)
jsr _inf_sset
add.w #10,sp
*line 669
*line 670
*line 671
move.l -30(R14),(sp)
jsr _g_name
move.l R0,(sp)
move #2,-(sp)
move.l R13,-(sp)
jsr _xinf_sse
addq.l #6,sp
*line 672
*line 673
*line 674
tst -10(R14)
beq L10022
move.l #_Nostr,(sp)
bra L10024
L10022:pea 28(R12)
L10024:move #3,-(sp)
move.l R13,-(sp)
jsr _inf_sset
add.w #10,sp
*line 675
*line 676
*line 677
*line 678
*line 679
move #24,-84(R14)
bra L116
L117:
*line 680
*line 681
move -84(R14),R0
muls #24,R0
add.l -54(R14),R0
move.l R0,R8
clr 10(R8)
*line 682
clr (sp)
move -84(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
L115:
*line 679
add #1,-84(R14)
L116:
*line 679
cmp #27,-84(R14)
ble L117
L114:
*line 683
*line 684
move (R12),R0
bra L119
*line 685
L120:
*line 686
L121:
*line 687
*line 688
move #26,R0
move R0,-78(R14)
move R0,-8(R14)
*line 689
move #26,(sp)
jsr _get_fstr
move.l R0,252(R13)
bra L118
*line 690
*line 691
L122:
*line 692
*line 693
move #25,R0
move R0,-78(R14)
move R0,-8(R14)
*line 694
move #25,(sp)
jsr _get_fstr
move.l R0,252(R13)
bra L118
*line 695
*line 696
L123:
*line 697
*line 698
move #24,R0
move R0,-78(R14)
move R0,-8(R14)
*line 699
move #24,(sp)
jsr _get_fstr
move.l R0,252(R13)
bra L118
*line 700
*line 701
L124:
*line 702
*line 703
move #27,R0
move R0,-78(R14)
move R0,-8(R14)
*line 704
move #27,(sp)
jsr _get_fstr
move.l R0,252(R13)
bra L118
*line 705
*line 706
L125:
*line 707
L126:
*line 708
*line 709
move #97,(sp)
move.l -30(R14),-(sp)
jsr _fill_str
addq.l #4,sp
bra L97
*line 710
bra L118
L119:sub #1,R0
cmp #9,R0
bhi L125
asl #2,R0
move R0,R8
add.l #L127,R8
move.l (R8),R8
jmp (R8)
.data
L127:.dc.l L120
.dc.l L125
.dc.l L122
.dc.l L123
.dc.l L125
.dc.l L125
.dc.l L121
.dc.l L125
.dc.l L125
.dc.l L124
.text
L118:
*line 711
*line 712
move #1,(sp)
move -8(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 713
*line 714
*line 715
*line 716
tst 26(R12)
beq L128
*line 717
*line 718
cmp #1,26(R12)
bcc L129
*line 718
move #1,26(R12)
*line 719
L129:
*line 720
*line 721
cmp #20,26(R12)
bls L130
*line 721
move #20,26(R12)
*line 722
L130:
*line 723
pea -38(R14)
clr.l R0
move 26(R12),R0
move.l R0,-(sp)
jsr _lbintoas
addq.l #8,sp
*line 724
move 26(R12),R0
add #2,R0
move R0,-74(R14)
*line 725
*line 726
move 26(R12),(sp)
add #45,(sp)
jsr _get_fstr
move.l R0,156(R13)
*line 727
bra L131
L128:
*line 728
*line 729
move #2,-74(R14)
*line 730
clr.b -38(R14)
*line 731
move #45,(sp)
jsr _get_fstr
move.l R0,156(R13)
L131:
*line 732
*line 733
move #1,(sp)
move -74(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 734
*line 735
clr (sp)
move #21,-(sp)
jsr _fm_draw
addq.l #2,sp
*line 736
clr -18(R14)
L132:
*line 737
*line 738
clr -86(R14)
*line 739
bra L135
L134:
*line 740
*line 741
move -18(R14),(sp)
move.l R13,-(sp)
jsr _form_do
addq.l #4,sp
and #32767,R0
move R0,-8(R14)
*line 742
move.l R13,R8
move -8(R14),R1
muls #24,R1
add.l R1,R8
and #-2,10(R8)
*line 743
*line 744
move -8(R14),R0
bra L137
*line 745
L138:
*line 746
*line 747
move #1,-86(R14)
bra L136
*line 748
*line 749
L139:
*line 750
*line 751
clr -12(R14)
*line 752
move #1,-86(R14)
bra L136
*line 753
*line 754
L140:
*line 755
*line 756
move #1,(sp)
move.l #$0002002D,-(sp)
clr -(sp)
pea -74(R14)
move.l #$00010002,-(sp)
move.l -54(R14),-(sp)
move.l #$00060005,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L136
*line 757
*line 758
L141:
*line 759
*line 760
clr (sp)
move.l #$00230029,-(sp)
clr -(sp)
pea -76(R14)
move -76(R14),-(sp)
move #34,-(sp)
move.l -54(R14),-(sp)
move.l #$00080007,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L136
*line 761
*line 762
*line 763
L142:
*line 764
*line 765
clr (sp)
move.l #$00180018,-(sp)
clr -(sp)
pea -78(R14)
move -78(R14),-(sp)
move #23,-(sp)
move.l -54(R14),-(sp)
move.l #$000A0009,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L136
*line 766
*line 767
*line 768
L143:
*line 769
*line 770
clr (sp)
move.l #$001D001C,-(sp)
clr -(sp)
pea -80(R14)
move -80(R14),-(sp)
move #28,-(sp)
move.l -54(R14),-(sp)
move.l #$000C000B,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L136
*line 771
*line 772
L144:
*line 773
*line 774
clr (sp)
move.l #$0020001E,-(sp)
clr -(sp)
pea -82(R14)
move -82(R14),-(sp)
move #31,-(sp)
move.l -54(R14),-(sp)
move.l #$000E000D,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L136
*line 775
*line 776
L145:
*line 777
*line 778
move #1,-86(R14)
*line 779
move -74(R14),R0
sub #2,R0
swap R0
clr R0
swap R0
move.l R0,-42(R14)
*line 780
*line 781
cmp #3,-74(R14)
blt L146
cmp #22,-74(R14)
bgt L146
*line 782
move.l _applist,-4(R14)
*line 783
*line 784
bra L149
L148:
*line 785
*line 786
*line 787
tst -10(R14)
bne L150
*line 788
*line 789
cmp.l -4(R14),R12
beq L151
*line 790
*line 791
L150:
*line 792
*line 793
move.l -4(R14),R8
move 26(R8),R0
swap R0
clr R0
swap R0
move.l -42(R14),R1
cmp.l R1,R0
bne L152
*line 794
*line 795
move #116,(sp)
jsr _do1_aler
cmp #2,R0
bne L153
*line 796
*line 797
move #2,-84(R14)
bra L156
L157:
*line 798
*line 799
move -84(R14),R0
muls #24,R0
add.l -54(R14),R0
move.l R0,R8
clr 10(R8)
*line 800
clr (sp)
move -84(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
L155:
*line 797
add #1,-84(R14)
L156:
*line 797
cmp #22,-84(R14)
ble L157
L154:
*line 801
*line 802
*line 803
*line 804
*line 805
tst 26(R12)
beq L158
*line 806
pea -38(R14)
clr.l R0
move 26(R12),R0
move.l R0,-(sp)
jsr _lbintoas
addq.l #8,sp
*line 807
move 26(R12),R0
add #2,R0
move R0,-74(R14)
*line 808
move 26(R12),(sp)
add #45,(sp)
jsr _get_fstr
move.l R0,156(R13)
*line 809
bra L159
L158:
*line 810
*line 811
move #2,-74(R14)
*line 812
clr.b -38(R14)
*line 813
move #45,(sp)
jsr _get_fstr
move.l R0,156(R13)
L159:
*line 814
*line 815
move #1,(sp)
move -74(R14),-(sp)
move.l -54(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 816
*line 817
move #6,(sp)
move.l R13,-(sp)
jsr _draw_fld
addq.l #4,sp
*line 818
*line 819
*line 820
or #1,370(R13)
*line 821
move #15,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
*line 822
clr -18(R14)
bra L132
*line 823
*line 824
bra L160
L153:
*line 825
move.l -4(R14),R8
clr 26(R8)
L160:
*line 826
*line 827
L152:L151:
*line 828
move.l -4(R14),R8
move.l 40(R8),-4(R14)
L149:
*line 829
tst.l -4(R14)
bne L148
L147:
*line 830
*line 831
L146:
*line 832
*line 833
tst -10(R14)
beq L161
*line 834
jsr _app_allo
move.l R0,R12
*line 835
*line 836
move.l R12,R0
bne L162
*line 837
move #83,(sp)
jsr _do1_aler
bra L163
*line 838
*line 839
L162:bra L164
L161:
*line 840
*line 841
clr.b 10(R12)
L164:
*line 842
*line 843
*line 844
pea -38(R14)
move.l #L165,-(sp)
jsr _strcpy
addq.l #8,sp
*line 845
pea -36(R14)
move #4,-(sp)
move.l R13,-(sp)
jsr _inf_sget
add.w #10,sp
*line 846
pea 4(R12)
move #-1,-(sp)
pea -38(R14)
jsr _app_icon
add.w #10,sp
*line 847
*line 848
pea 10(R12)
pea -38(R14)
jsr _strcpy
addq.l #8,sp
*line 849
pea 28(R12)
move #3,-(sp)
move.l R13,-(sp)
jsr _inf_sget
add.w #10,sp
*line 850
move #1,-26(R14)
*line 851
*line 852
move -78(R14),R0
bra L167
*line 853
L168:
*line 854
*line 855
move #1,-20(R14)
bra L166
*line 856
*line 857
L169:
*line 858
*line 859
move #10,-20(R14)
bra L166
*line 860
*line 861
L170:
*line 862
*line 863
clr -26(R14)
*line 864
move #4,-20(R14)
bra L166
*line 865
*line 866
L171:
*line 867
*line 868
clr -26(R14)
*line 869
move #3,-20(R14)
bra L166
L167:cmp #24,R0
beq L170
cmp #25,R0
beq L171
cmp #26,R0
beq L168
cmp #27,R0
beq L169
bra L166
L166:
*line 871
*line 872
*line 873
move -20(R14),(R12)
*line 874
move -24(R14),2(R12)
*line 875
clr 24(R12)
*line 876
*line 877
*line 878
move.l -54(R14),R8
btst #2,707(R8)
beq L172
*line 878
move #1,24(R12)
*line 879
L172:
*line 880
*line 881
move.l -54(R14),R8
btst #2,779(R8)
beq L173
*line 881
add #2,24(R12)
*line 882
L173:
*line 883
*line 884
move.l -54(R14),R8
btst #2,851(R8)
beq L174
*line 885
*line 886
move.l -30(R14),R8
tst.b (R8)
beq L175
*line 887
*line 888
move.l -30(R14),(sp)
jsr _strlen
cmp #124,R0
bge L176
*line 889
move -26(R14),(sp)
move.l #_autofile,-(sp)
jsr _save_2
addq.l #4,sp
*line 890
move.b #32,2+_autofile
*line 891
move.l #3+_autofile,(sp)
move.l -30(R14),-(sp)
jsr _strcpy
addq.l #4,sp
*line 892
bra L177
L176:
*line 893
move #99,(sp)
jsr _do1_aler
L177:
*line 894
L175:
*line 895
bra L178
L174:
*line 896
*line 897
*line 898
tst -22(R14)
beq L179
*line 898
move.l #_autofile,(sp)
move.l #_Nostr,-(sp)
jsr _strcpy
addq.l #4,sp
L179:L178:
*line 899
*line 900
*line 901
move.l -42(R14),R0
move R0,26(R12)
*line 902
*line 903
pea 6(R12)
move.l -30(R14),-(sp)
jsr _lp_fill
addq.l #8,sp
*line 904
jsr _lp_colle
bra L136
*line 905
*line 906
L180:
*line 907
*line 908
move #1,-86(R14)
*line 909
*line 910
move.l -30(R14),R8
tst.b (R8)
beq L181
*line 910
move.l #_autofile,(sp)
move.l #_Nostr,-(sp)
jsr _strcpy
addq.l #4,sp
*line 911
L181:
*line 912
*line 913
tst -10(R14)
bne L182
*line 913
move.l R12,(sp)
jsr _app_free
L182:bra L136
*line 914
*line 915
L183:
*line 916
bra L136
L137:sub #6,R0
cmp #12,R0
bhi L183
asl #2,R0
move R0,R8
add.l #L184,R8
move.l (R8),R8
jmp (R8)
.data
L184:.dc.l L140
.dc.l L183
.dc.l L141
.dc.l L183
.dc.l L142
.dc.l L183
.dc.l L143
.dc.l L183
.dc.l L144
.dc.l L145
.dc.l L180
.dc.l L138
.dc.l L139
.text
L136:
*line 919
L135:
*line 920
tst -86(R14)
beq L134
L133:L97:
*line 921
*line 922
*line 923
tst -12(R14)
beq L94
*line 924
*line 925
pea -20(R14)
pea -30(R14)
jsr _x_next
addq.l #8,sp
move R0,-12(R14)
*line 926
L96:
*line 927
tst -12(R14)
bne L95
L94:L163:
*line 929
*line 930
move.l R13,R0
beq L185
*line 930
move #21,(sp)
jsr _do_finis
*line 931
bra L186
L185:
*line 932
move #104,(sp)
jsr _do1_aler
L186:L93:tst.l (sp)+
movem.l (sp)+,R12-R13
unlk R14
rts
.globl _mdesk_pr
.text
_mdesk_pr:
~~mdesk_pr:
link R14,#-16
movem.l R4-R7/R13-R13,-(sp)
~obj=R13
~cyes=R7
~i=R6
~flag=R5
~overwrit=-2
~font=-4
~dtree=-8
~dstart=-10
~cstart=-12
~ostart=-14
~button=-16
*line 954
move #22,(sp)
jsr _get_tree
move.l R0,R13
*line 955
move #25,(sp)
jsr _get_tree
move.l R0,-8(R14)
*line 957
*line 958
tst _cdele_sa
beq L188
*line 959
move #38,-10(R14)
*line 960
move #34,(sp)
jsr _get_fstr
move.l R0,108(R13)
*line 961
bra L189
L188:
*line 962
*line 963
move #39,-10(R14)
*line 964
move #35,(sp)
jsr _get_fstr
move.l R0,108(R13)
L189:
*line 967
*line 968
tst _write_sa
bne L190
*line 969
move #38,-14(R14)
*line 970
move #34,(sp)
jsr _get_fstr
move.l R0,204(R13)
*line 971
bra L191
L190:
*line 972
*line 973
move #39,-14(R14)
*line 974
move #35,(sp)
jsr _get_fstr
move.l R0,204(R13)
L191:
*line 977
*line 978
tst _ccopy_sa
beq L192
*line 979
move #38,-12(R14)
*line 980
move #34,(sp)
jsr _get_fstr
move.l R0,156(R13)
*line 981
bra L193
L192:
*line 982
*line 983
move #39,-12(R14)
*line 984
move #35,(sp)
jsr _get_fstr
move.l R0,156(R13)
L193:
*line 987
clr (sp)
move #22,-(sp)
jsr _fm_draw
addq.l #2,sp
*line 988
clr -16(R14)
*line 990
L196:
*line 991
clr (sp)
move.l R13,-(sp)
jsr _form_do
addq.l #4,sp
and #32767,R0
move R0,-16(R14)
*line 992
move.l R13,R8
move -16(R14),R1
muls #24,R1
add.l R1,R8
and #-2,10(R8)
*line 993
*line 994
move.l -8(R14),R8
clr 922(R8)
*line 995
move.l -8(R14),R8
clr 946(R8)
*line 996
*line 997
move -16(R14),R0
bra L198
*line 998
L199:
*line 999
*line 1000
cmp #38,-10(R14)
beq L10025
clr R0
bra L10026
L10025:move #1,R0
L10026:move R0,_cdele_sa
*line 1001
cmp #38,-12(R14)
beq L10027
clr R0
bra L10028
L10027:move #1,R0
L10028:move R0,_ccopy_sa
*line 1002
cmp #38,-14(R14)
bne L10029
clr R0
bra L10030
L10029:move #1,R0
L10030:move R0,_write_sa
*line 1003
L200:
*line 1004
bra L197
*line 1005
*line 1006
L201:
*line 1007
*line 1008
move #1,(sp)
move -10(R14),-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1009
clr (sp)
move.l #$00260022,-(sp)
clr -(sp)
pea -10(R14)
move -10(R14),-(sp)
move #37,-(sp)
move.l -8(R14),-(sp)
move.l #$00040003,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L197
*line 1010
*line 1011
L202:
*line 1012
*line 1013
move #1,(sp)
move -12(R14),-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1014
clr (sp)
move.l #$00260022,-(sp)
clr -(sp)
pea -12(R14)
move -12(R14),-(sp)
move #37,-(sp)
move.l -8(R14),-(sp)
move.l #$00060005,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L197
*line 1015
*line 1016
L203:
*line 1017
*line 1018
move #1,(sp)
move -14(R14),-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1019
clr (sp)
move.l #$00260022,-(sp)
clr -(sp)
pea -14(R14)
move -14(R14),-(sp)
move #37,-(sp)
move.l -8(R14),-(sp)
move.l #$00080007,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L197
*line 1020
bra L197
L198:sub #4,R0
cmp #6,R0
bhi L197
asl #2,R0
move R0,R8
add.l #L204,R8
move.l (R8),R8
jmp (R8)
.data
L204:.dc.l L201
.dc.l L197
.dc.l L202
.dc.l L197
.dc.l L203
.dc.l L199
.dc.l L200
.text
L197:
*line 1021
L195:
*line 1022
cmp #9,-16(R14)
beq L10031
cmp #10,-16(R14)
bne L196
L10031:L194:
*line 1024
move #22,(sp)
jsr _do_finis
L187:tst.l (sp)+
movem.l (sp)+,R5-R7/R13-R13
unlk R14
rts
.globl _set_vide
.text
_set_vide:
~~set_vide:
link R14,#-66
movem.l R4-R7/R13-R13,-(sp)
~obj=R13
~cyes=R7
~i=R6
~flag=R5
~overwrit=-2
~font=-4
~dtree=-8
~button=-10
~cstart=-12
~old_csta=-14
~lstart=-16
~old_lsta=-18
~dstart=-20
~old_dsta=-22
~vtree=-26
~output=-28
~done=-30
~monitor_=-32
~mode_cod=-34
~old_mode=-36
~color_fl=-38
~column_f=-40
~vga_flag=-42
~pal_flag=-44
~overscan=-46
~st_flag=-48
~vertical=-50
~double_f=-52
~p_value=-56
~buffer=-66
*line 1079
move #23,(sp)
jsr _get_tree
move.l R0,R13
*line 1080
move #25,(sp)
jsr _get_tree
move.l R0,-8(R14)
*line 1092
move #89,(sp)
jsr _xbios
move R0,-32(R14)
*line 1094
and #-9,82(R13)
*line 1095
and #-9,130(R13)
*line 1096
and #-9,178(R13)
*line 1098
or #64,80(R13)
*line 1099
or #64,128(R13)
*line 1100
or #64,176(R13)
*line 1104
*line 1105
tst -32(R14)
bne L206
*line 1106
or #8,82(R13)
*line 1107
or #8,130(R13)
*line 1108
or #8,178(R13)
*line 1109
*line 1110
and #-65,80(R13)
*line 1111
and #-65,128(R13)
*line 1112
and #-65,176(R13)
*line 1113
L206:
*line 1114
move #-1,(sp)
move #88,-(sp)
jsr _xbios
addq.l #2,sp
move R0,-34(R14)
move R0,-36(R14)
*line 1115
move -34(R14),R0
and #7,R0
move R0,-38(R14)
*line 1116
move -34(R14),R0
and #8,R0
move R0,-40(R14)
*line 1118
move -34(R14),R0
and #16,R0
move R0,-42(R14)
*line 1119
move -34(R14),R0
and #32,R0
move R0,-44(R14)
*line 1120
move -34(R14),R0
and #64,R0
move R0,-46(R14)
*line 1121
move -34(R14),R0
and #128,R0
move R0,-48(R14)
*line 1122
move -34(R14),R0
and #256,R0
move R0,-50(R14)
*line 1128
cmp #16,-42(R14)
bne L10032
move #1,R0
bra L10034
L10032:clr R0
L10034:move R0,-52(R14)
*line 1132
move #41,R6
bra L209
L210:
*line 1133
clr (sp)
move R6,-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
L208:
*line 1132
add #1,R6
L209:
*line 1132
cmp #45,R6
ble L210
L207:
*line 1136
move -38(R14),R0
add #41,R0
move R0,-12(R14)
*line 1138
move -38(R14),(sp)
add #36,(sp)
jsr _get_fstr
move.l R0,84(R13)
*line 1139
move #1,(sp)
move -12(R14),-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1143
clr (sp)
move #50,-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1144
clr (sp)
move #51,-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1146
*line 1147
cmp #8,-40(R14)
bne L211
*line 1148
move #51,-16(R14)
*line 1149
move #33,(sp)
jsr _get_fstr
move.l R0,132(R13)
*line 1150
bra L212
L211:
*line 1151
*line 1152
move #50,-16(R14)
*line 1153
move #32,(sp)
jsr _get_fstr
move.l R0,132(R13)
L212:
*line 1155
move #1,(sp)
move -16(R14),-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1162
*line 1163
tst -52(R14)
beq L213
*line 1163
move #66,(sp)
jsr _get_fstr
move.l R0,-(sp)
move.l 156(R13),R8
move.l (sp)+,(R8)
*line 1164
bra L214
L213:
*line 1165
move #67,(sp)
jsr _get_fstr
move.l R0,-(sp)
move.l 156(R13),R8
move.l (sp)+,(R8)
L214:
*line 1168
clr (sp)
move #47,-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1169
clr (sp)
move #48,-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1171
*line 1172
tst -50(R14)
beq L215
*line 1173
move #47,-20(R14)
*line 1174
move #43,(sp)
jsr _get_fstr
move.l R0,180(R13)
*line 1175
bra L216
L215:
*line 1176
*line 1177
move #48,-20(R14)
*line 1178
move #44,(sp)
jsr _get_fstr
move.l R0,180(R13)
L216:
*line 1180
move #1,(sp)
move -20(R14),-(sp)
move.l -8(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1182
clr -10(R14)
*line 1183
clr -28(R14)
*line 1184
clr -30(R14)
*line 1186
move -12(R14),-14(R14)
*line 1187
move -16(R14),-18(R14)
*line 1188
move -20(R14),-22(R14)
*line 1191
clr (sp)
move #23,-(sp)
jsr _fm_draw
addq.l #2,sp
*line 1193
L219:
*line 1194
clr (sp)
move.l R13,-(sp)
jsr _form_do
addq.l #4,sp
and #32767,R0
move R0,-10(R14)
*line 1195
move.l R13,R8
move -10(R14),R1
muls #24,R1
add.l R1,R8
and #-2,10(R8)
*line 1196
*line 1197
move -10(R14),R0
bra L221
*line 1198
*line 1199
*line 1200
*line 1201
*line 1202
*line 1203
*line 1204
L222:
*line 1205
*line 1206
*line 1207
*line 1208
move -12(R14),R0
cmp -14(R14),R0
bne L10035
move -16(R14),R0
cmp -18(R14),R0
bne L10035
move -20(R14),R0
cmp -22(R14),R0
bne L10035
btst #7,-35(R14)
beq L223
L10035:tst -32(R14)
beq L223
*line 1209
clr -48(R14)
*line 1210
*line 1211
*line 1212
move -12(R14),R0
add #-41,R0
cmp #51,-16(R14)
bne L10036
move #8,R1
bra L10038
L10036:clr R1
L10038:or R1,R0
cmp #47,-20(R14)
bne L10039
move #256,R1
bra L10041
L10039:clr R1
L10041:or R1,R0
or -44(R14),R0
or -46(R14),R0
or -48(R14),R0
or -42(R14),R0
move R0,-34(R14)
*line 1213
*line 1214
*line 1215
*line 1216
*line 1217
*line 1218
move -34(R14),(sp)
jsr _get_that
tst R0
bne L224
*line 1219
move #10,(sp)
move.l R13,-(sp)
jsr _XDeselec
addq.l #4,sp
bra L218
*line 1220
*line 1221
L224:
*line 1222
bra L225
L223:
*line 1223
move #11,-10(R14)
L225:
*line 1224
L226:
*line 1225
*line 1226
move #1,-30(R14)
*line 1227
move #23,(sp)
jsr _do_finis
bra L220
*line 1228
*line 1229
L227:
*line 1230
*line 1231
move #23,(sp)
jsr _do_finis
L228:
*line 1232
*line 1233
move #24,(sp)
jsr _get_tree
move.l R0,-26(R14)
*line 1234
jsr _init_vtr
*line 1235
clr (sp)
move #24,-(sp)
jsr _fmdodraw
addq.l #2,sp
move R0,-28(R14)
*line 1236
move -28(R14),R0
muls #24,R0
add.l -26(R14),R0
move.l R0,R8
and #-2,10(R8)
*line 1237
*line 1238
*line 1239
*line 1240
cmp #6,-28(R14)
bne L229
*line 1241
move #11,-10(R14)
*line 1242
*line 1243
tst -32(R14)
beq L230
*line 1244
*line 1245
clr -48(R14)
*line 1246
move -42(R14),R0
or -44(R14),R0
or -46(R14),R0
move R0,-34(R14)
*line 1247
*line 1248
move #24,(sp)
jsr _get_tree
move.l R0,-26(R14)
*line 1249
move #3,(sp)
move #3,-(sp)
move.l -26(R14),-(sp)
jsr _inf_gind
addq.l #6,sp
move R0,R5
*line 1250
*line 1251
cmp #-1,R5
beq L231
*line 1252
move R5,R0
bra L233
*line 1253
L234:
*line 1254
*line 1255
move #128,-48(R14)
*line 1256
cmp #16,-42(R14)
bne L10042
move #256,R0
bra L10044
L10042:clr R0
L10044:move R0,-50(R14)
*line 1257
move -50(R14),R0
or #130,R0
or R0,-34(R14)
bra L232
*line 1258
*line 1259
L235:
*line 1260
*line 1261
move #128,-48(R14)
*line 1262
cmp #16,-42(R14)
bne L10045
move #256,R0
bra L10047
L10045:clr R0
L10047:move R0,-50(R14)
*line 1263
move -50(R14),R0
or #137,R0
or R0,-34(R14)
bra L232
*line 1264
*line 1265
L236:
*line 1266
*line 1267
move #128,-48(R14)
*line 1268
cmp #16,-42(R14)
bne L10048
clr R0
bra L10050
L10048:move #256,R0
L10050:move R0,-50(R14)
*line 1269
move -50(R14),R0
or #136,R0
or R0,-34(R14)
bra L232
*line 1270
*line 1281
*line 1282
bra L232
L233:tst R0
beq L234
cmp #1,R0
beq L235
cmp #2,R0
beq L236
bra L232
L232:
*line 1283
*line 1284
*line 1285
*line 1286
*line 1287
*line 1288
*line 1289
add #1,R5
*line 1290
*line 1291
btst #7,-33(R14)
beq L237
*line 1292
move R5,_gl_resty
*line 1293
move R5,_pref_sav
*line 1294
move #1,_gl_rscha
*line 1295
move #2,_d_exit
*line 1296
*line 1297
*line 1298
L237:
*line 1299
*line 1300
move -34(R14),R0
move -36(R14),R1
eor R1,R0
beq L238
*line 1301
*line 1302
move -34(R14),(sp)
jsr _get_that
tst R0
beq L239
*line 1302
move #6,-10(R14)
*line 1303
bra L240
L239:
*line 1304
*line 1305
move #6,(sp)
move.l -26(R14),-(sp)
jsr _XDeselec
addq.l #4,sp
bra L228
*line 1306
L240:
*line 1307
*line 1308
*line 1309
*line 1310
L238:
*line 1311
*line 1312
L231:
*line 1313
L230:
*line 1314
move #1,-30(R14)
*line 1315
bra L241
L229:
*line 1316
*line 1317
clr -48(R14)
*line 1318
clr (sp)
move #23,-(sp)
jsr _fm_draw
addq.l #2,sp
L241:
*line 1319
bra L220
*line 1320
*line 1321
L242:
*line 1322
*line 1323
*line 1324
*line 1325
*line 1326
cmp #16,-42(R14)
bne L243
cmp #51,-16(R14)
bne L243
*line 1326
move.l -8(R14),R8
or #8,1090(R8)
*line 1327
bra L244
L243:
*line 1328
move.l -8(R14),R8
and #-9,1090(R8)
L244:
*line 1329
*line 1330
*line 1331
*line 1332
cmp #50,-16(R14)
bne L245
*line 1332
move.l -8(R14),R8
or #8,994(R8)
*line 1333
bra L246
L245:
*line 1334
move.l -8(R14),R8
and #-9,994(R8)
L246:
*line 1335
*line 1336
clr (sp)
move.l #$00290024,-(sp)
clr -(sp)
pea -12(R14)
move -12(R14),-(sp)
move #40,-(sp)
move.l -8(R14),-(sp)
move.l #$00030002,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L220
*line 1337
*line 1338
L247:
*line 1339
*line 1340
*line 1341
*line 1342
*line 1343
cmp #16,-42(R14)
bne L248
cmp #45,-12(R14)
bne L248
*line 1343
move.l -8(R14),R8
or #8,1234(R8)
*line 1344
bra L249
L248:
*line 1345
move.l -8(R14),R8
and #-9,1234(R8)
L249:
*line 1346
*line 1347
*line 1348
*line 1349
cmp #41,-12(R14)
bne L250
*line 1349
move.l -8(R14),R8
or #8,1210(R8)
*line 1350
bra L251
L250:
*line 1351
move.l -8(R14),R8
and #-9,1210(R8)
L251:
*line 1352
clr (sp)
move.l #$00320020,-(sp)
clr -(sp)
pea -16(R14)
move -16(R14),-(sp)
move #49,-(sp)
move.l -8(R14),-(sp)
move.l #$00050004,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L220
*line 1353
*line 1354
L252:
*line 1355
*line 1356
clr (sp)
move.l #$002F002B,-(sp)
clr -(sp)
pea -20(R14)
move -20(R14),-(sp)
move #46,-(sp)
move.l -8(R14),-(sp)
move.l #$00070006,-(sp)
move.l R13,-(sp)
jsr _DoPopup
adda.w #26,sp
bra L220
*line 1357
bra L220
L221:sub #3,R0
cmp #8,R0
bhi L220
asl #2,R0
move R0,R8
add.l #L253,R8
move.l (R8),R8
jmp (R8)
.data
L253:.dc.l L242
.dc.l L220
.dc.l L247
.dc.l L220
.dc.l L252
.dc.l L227
.dc.l L220
.dc.l L222
.dc.l L226
.text
L220:
*line 1358
*line 1359
L218:
*line 1360
tst -30(R14)
beq L219
L217:
*line 1364
*line 1365
cmp #11,-10(R14)
beq L254
*line 1366
*line 1367
move.l _gl_vdo,R0
and.l #$30000,R0
cmp.l #$30000,R0
bne L255
*line 1367
move -34(R14),(sp)
move #95,-(sp)
jsr _trap14
addq.l #2,sp
move R0,-34(R14)
*line 1368
L255:
*line 1369
move -34(R14),_d_rezwor
*line 1370
move #1,R0
bra L205
*line 1371
*line 1372
L254:
*line 1373
clr R0
bra L205
L205:tst.l (sp)+
movem.l (sp)+,R5-R7/R13-R13
unlk R14
rts
.globl _XSelect
.text
_XSelect:
~~XSelect:
~tree=8
~obj=12
link R14,#-12
~trect=-8
*line 1389
*line 1390
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
btst #0,11(R8)
bne L256
*line 1391
*line 1392
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
or #1,10(R8)
*line 1393
move 12(R14),R1
muls #24,R1
add.l 8(R14),R1
add.l #16,R1
move.l R14,R8
sub.l #8,R8
move.l r1,r9
move #3,r0
L10051:move (r9)+,(r8)+
dbra r0,L10051
*line 1394
pea -6(R14)
pea -8(R14)
move 12(R14),-(sp)
move.l 8(R14),-(sp)
jsr _objc_off
adda.w #14,sp
*line 1395
sub #2,-8(R14)
*line 1396
sub #2,-6(R14)
*line 1397
add #4,-4(R14)
*line 1398
add #4,-2(R14)
*line 1399
move.l -4(R14),-(sp)
move.l -8(R14),-(sp)
move #8,-(sp)
move 12(R14),-(sp)
move.l 8(R14),-(sp)
jsr _objc_dra
adda.w #16,sp
L256:unlk R14
rts
.globl _XDeselec
.text
_XDeselec:
~~XDeselec:
~tree=8
~obj=12
link R14,#-12
~trect=-8
*line 1414
*line 1415
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
btst #0,11(R8)
beq L258
*line 1416
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
and #-2,10(R8)
*line 1417
move 12(R14),R1
muls #24,R1
add.l 8(R14),R1
add.l #16,R1
move.l R14,R8
sub.l #8,R8
move.l r1,r9
move #3,r0
L10052:move (r9)+,(r8)+
dbra r0,L10052
*line 1418
pea -6(R14)
pea -8(R14)
move 12(R14),-(sp)
move.l 8(R14),-(sp)
jsr _objc_off
adda.w #14,sp
*line 1419
sub #2,-8(R14)
*line 1420
sub #2,-6(R14)
*line 1421
add #4,-4(R14)
*line 1422
add #4,-2(R14)
*line 1423
move.l -4(R14),-(sp)
move.l -8(R14),-(sp)
move #8,-(sp)
move 12(R14),-(sp)
move.l 8(R14),-(sp)
jsr _objc_dra
adda.w #16,sp
*line 1424
L258:L257:unlk R14
rts
.globl _DoPopup
.text
_DoPopup:
~~DoPopup:
~tree=8
~button=12
~title=14
~Mtree=16
~Mmenu=20
~Mfirst=22
~Mstart=24
~Mscroll=28
~FirstMen=30
~FirstTex=32
~Skip=34
link R14,#-14
~brect=-8
~flag=-10
*line 1451
*line 1452
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _ChkTouch
addq.l #4,sp
tst R0
beq L259
*line 1453
*line 1454
*line 1455
move 14(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
or #1,10(R8)
*line 1456
move 14(R14),(sp)
move.l 8(R14),-(sp)
jsr _draw_fld
addq.l #4,sp
*line 1459
pea -6(R14)
pea -8(R14)
move 12(R14),-(sp)
move.l 8(R14),-(sp)
jsr _objc_off
adda.w #14,sp
*line 1461
move.l 16(R14),_Menu
*line 1462
move 20(R14),4+_Menu
*line 1463
move 22(R14),6+_Menu
*line 1464
move 28(R14),8+_Menu
*line 1465
move.l #_MData,(sp)
move.l -8(R14),-(sp)
move.l #_Menu,-(sp)
jsr _menu_pop
addq.l #8,sp
move R0,-10(R14)
*line 1466
*line 1467
tst -10(R14)
beq L260
*line 1468
*line 1469
move.l _MData,R0
cmp.l 16(R14),R0
bne L261
move 4+_MData,R0
cmp 20(R14),R0
bne L261
*line 1470
clr (sp)
move.l 24(R14),R8
move (R8),-(sp)
move.l 16(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1471
move #1,(sp)
move 6+_MData,-(sp)
move.l 16(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1472
move.l 24(R14),R8
move 6+_MData,(R8)
*line 1473
*line 1474
*line 1475
tst 34(R14)
bne L262
*line 1475
move.l 24(R14),R8
move (R8),(sp)
move 20(R14),-(sp)
move.l 16(R14),-(sp)
move #1,-(sp)
jsr _menu_ist
addq.l #8,sp
*line 1476
L262:
*line 1477
move.l 24(R14),R8
move (R8),(sp)
move 30(R14),R0
sub R0,(sp)
move 32(R14),R0
add R0,(sp)
jsr _get_fstr
move.l R0,-(sp)
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
move.l (sp)+,12(R8)
*line 1478
L261:
*line 1479
*line 1480
L260:
*line 1481
move 14(R14),(sp)
move.l 8(R14),-(sp)
jsr _XDeselec
addq.l #4,sp
*line 1482
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _XDeselec
addq.l #4,sp
L259:unlk R14
rts
.globl _init_vtr
.text
_init_vtr:
~~init_vtr:
link R14,#-2
movem.l R4-R7/R13-R13,-(sp)
~vtree=R13
~cyes=R7
~i=R6
~flag=R5
~monitor_=-2
*line 1497
move #24,(sp)
jsr _get_tree
move.l R0,R13
*line 1500
move #3,R6
bra L266
L267:
*line 1501
move.l R13,R8
move R6,R1
muls #24,R1
add.l R1,R8
clr 10(R8)
L265:
*line 1500
add #1,R6
L266:
*line 1500
cmp #5,R6
ble L267
L264:
*line 1504
move #89,(sp)
jsr _xbios
move R0,-2(R14)
*line 1505
*line 1506
tst -2(R14)
bne L268
*line 1507
move #3,R6
bra L271
L272:
*line 1508
move.l R13,R8
move R6,R1
muls #24,R1
add.l R1,R8
move #8,10(R8)
L270:
*line 1507
add #1,R6
L271:
*line 1507
cmp #5,R6
ble L272
L269:
*line 1508
*line 1509
*line 1519
L268:L263:tst.l (sp)+
movem.l (sp)+,R5-R7/R13-R13
unlk R14
rts
.globl _DoMKItem
.text
_DoMKItem:
~~DoMKItem:
~dtree=8
~button=12
~item=14
link R14,#-32
~obj1=-4
~index=-6
~tree=-10
~flag=-12
~brect=-20
~start=-22
~old_item=-24
~length=-26
~temp=-28
*line 1547
move.l _menu_add,-4(R14)
*line 1548
move.l 14(R14),R8
move (R8),R0
add #1,R0
move R0,-22(R14)
*line 1551
clr -26(R14)
*line 1552
clr -28(R14)
*line 1554
move #1,-6(R14)
bra L276
L277:
*line 1555
*line 1556
move.l -4(R14),R8
move -6(R14),R9
sub #1,R9
add.l R9,R9
add.l #_tb3,R9
move (R9),R1
muls #24,R1
move.l 12(R8,R1.l),(sp)
jsr _strlen
move R0,-28(R14)
*line 1557
*line 1558
move -28(R14),R0
cmp -26(R14),R0
ble L278
*line 1558
move -28(R14),-26(R14)
L278:L275:
*line 1554
add #1,-6(R14)
L276:
*line 1554
cmp #29,-6(R14)
ble L277
L274:
*line 1562
*line 1563
cmp #30,-26(R14)
bge L279
*line 1563
move #30,-26(R14)
*line 1564
*line 1565
L279:
*line 1566
move #-1,_mtree
*line 1567
move #-1,R0
move R0,4+_mtree
move R0,2+_mtree
*line 1568
move #20,6+_mtree
*line 1569
move #32,8+_mtree
*line 1570
move #32,10+_mtree
*line 1571
move.l #$ff1100,12+_mtree
*line 1572
clr R0
move R0,18+_mtree
move R0,16+_mtree
*line 1573
move -26(R14),20+_mtree
*line 1574
move #29,22+_mtree
*line 1576
move #1,-6(R14)
bra L282
L283:
*line 1577
*line 1578
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move #-1,(R8)
*line 1579
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move #-1,R0
move R0,4(R8)
move -6(R14),R1
muls #24,R1
add.l #_mtree,R1
move.l R1,R9
move R0,2(R9)
*line 1580
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move #28,6(R8)
*line 1581
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
clr 8(R8)
*line 1582
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
clr 10(R8)
*line 1583
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move -6(R14),R9
sub #1,R9
add.l R9,R9
add.l #_tb3,R9
move (R9),R1
muls #24,R1
add.l -4(R14),R1
move.l R1,R9
move.l 12(R9),12(R8)
*line 1584
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
clr 16(R8)
*line 1585
move -6(R14),R0
sub #1,R0
move -6(R14),R1
muls #24,R1
add.l #_mtree,R1
move.l R1,R9
move R0,18(R9)
*line 1586
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move -26(R14),20(R8)
*line 1587
move -6(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move #1,22(R8)
*line 1588
move -6(R14),(sp)
clr -(sp)
move.l #_mtree,-(sp)
jsr _objc_add
addq.l #6,sp
L281:
*line 1576
add #1,-6(R14)
L282:
*line 1576
cmp #29,-6(R14)
ble L283
L280:
*line 1591
clr -6(R14)
bra L286
L287:
*line 1592
move -6(R14),(sp)
move.l #_mtree,-(sp)
jsr _rsrc_obf
addq.l #4,sp
L285:
*line 1591
add #1,-6(R14)
L286:
*line 1591
cmp #29,-6(R14)
ble L287
L284:
*line 1594
or #32,704+_mtree
*line 1595
move.l #_mtree,-10(R14)
*line 1596
move #1,(sp)
move -22(R14),-(sp)
move.l -10(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1600
*line 1601
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _ChkTouch
addq.l #4,sp
tst R0
bne L288
*line 1601
clr R0
bra L273
*line 1602
L288:
*line 1603
move.l 14(R14),R8
move (R8),(sp)
jsr _DoKeyChe
move R0,-24(R14)
*line 1604
*line 1605
cmp #-1,-24(R14)
beq L289
*line 1605
move -24(R14),R0
add #1,R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move -24(R14),R9
add.l R9,R9
add.l #_tb3,R9
move (R9),R1
muls #24,R1
add.l -4(R14),R1
move.l R1,R9
move.l 12(R9),12(R8)
L289:
*line 1606
move -22(R14),R0
muls #24,R0
add.l #_mtree,R0
move.l R0,R8
move.l 14(R14),R9
move (R9),R9
add.l R9,R9
add.l #_tb3,R9
move (R9),R1
muls #24,R1
add.l -4(R14),R1
move.l R1,R9
move.l 12(R9),12(R8)
*line 1608
pea -18(R14)
pea -20(R14)
move 12(R14),-(sp)
move.l 8(R14),-(sp)
jsr _objc_off
adda.w #14,sp
*line 1610
move.l -10(R14),_Menu
*line 1611
clr 4+_Menu
*line 1612
move -22(R14),6+_Menu
*line 1613
move #1,8+_Menu
*line 1614
move.l #_MData,(sp)
move.l -20(R14),-(sp)
move.l #_Menu,-(sp)
jsr _menu_pop
addq.l #8,sp
move R0,-12(R14)
*line 1615
*line 1616
tst -12(R14)
beq L290
*line 1617
*line 1618
move.l _MData,R0
cmp.l -10(R14),R0
bne L291
*line 1619
clr (sp)
move -22(R14),-(sp)
move.l -10(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1620
move #1,(sp)
move 6+_MData,-(sp)
move.l -10(R14),-(sp)
jsr _menu_ich
addq.l #6,sp
*line 1621
move 6+_MData,R0
sub #1,R0
move.l 14(R14),R9
move R0,(R9)
*line 1622
move 12(R14),R0
muls #24,R0
add.l 8(R14),R0
move.l R0,R8
and #-2,10(R8)
*line 1623
move #1,R0
bra L273
*line 1624
L291:
*line 1625
L290:
*line 1626
move 12(R14),(sp)
move.l 8(R14),-(sp)
jsr _XDeselec
addq.l #4,sp
*line 1627
clr R0
bra L273
L273:unlk R14
rts
.globl _DoKeyChe
.text
_DoKeyChe:
~~DoKeyChe:
~item=8
link R14,#-14
~obj=-4
~i=-6
~buf=-8
~flag=-10
*line 1644
move #-1,-10(R14)
*line 1646
move #26,(sp)
jsr _get_tree
move.l R0,-4(R14)
*line 1648
pea -8(R14)
move #20,-(sp)
move.l -4(R14),-(sp)
jsr _inf_sget
add.w #10,sp
*line 1649
move.b -8(R14),R0
ext.w R0
move R0,(sp) 
jsr _toupper
move.b R0,-8(R14)
*line 1650
*line 1651
tst.b -8(R14)
beq L293
*line 1652
clr -6(R14)
bra L296
L297:
*line 1653
*line 1654
*line 1655
move -6(R14),R0
cmp 8(R14),R0
beq L295
*line 1656
*line 1657
*line 1658
move -6(R14),R8
move.l #_mentable,R9
move.b 0(R8,R9.l),R0
ext.w R0
cmp.b -8(R14),R0
bne L298
*line 1659
*line 1660
move #116,(sp)
jsr _do1_aler
cmp #1,R0
bne L299
*line 1661
move.l #_mentable,R8
move -6(R14),R9
add.l R9,R8
clr.b (R8)
*line 1662
move -6(R14),(sp)
jsr _ch_key
*line 1663
move -6(R14),-10(R14)
bra L295
*line 1664
*line 1665
L299:
*line 1666
clr.b -7(R14)
*line 1667
move.l #_mentable,R8
move 8(R14),R9
add.l R9,R8
move.b (R8),-8(R14)
*line 1668
L298:L295:
*line 1652
add #1,-6(R14)
L296:
*line 1652
cmp #29,-6(R14)
blt L297
L294:
*line 1669
*line 1670
L293:
*line 1671
move.l #_mentable,R8
move 8(R14),R9
add.l R9,R8
move.b -8(R14),(R8)
*line 1672
pea -8(R14)
move #20,-(sp)
move.l -4(R14),-(sp)
jsr _inf_sset
add.w #10,sp
*line 1673
move #20,(sp)
move.l -4(R14),-(sp)
jsr _draw_fld
addq.l #4,sp
*line 1674
move 8(R14),(sp)
jsr _ch_key
*line 1675
move -10(R14),R0
bra L292
L292:unlk R14
rts
.globl _get_that
.text
_get_that:
~~get_that:
~newmode=8
link R14,#-28
~avail=-4
~size=-8
~needed=-12
~char_byt=-16
~IsVGA=-18
~IsVertFl=-20
~Result=-22
~Bits=-24
*line 1691
move.l #$ffffffff,(sp)
move #21,-(sp)
jsr _gemdos
addq.l #2,sp
move.l R0,-4(R14)
*line 1692
move 8(R14),(sp)
move #91,-(sp)
jsr _xbios
addq.l #2,sp
move.l R0,-8(R14)
*line 1693
btst #4,9(R14)
bne L10053
clr R0
bra L10054
L10053:move #1,R0
L10054:move R0,-18(R14)
*line 1694
btst #0,8(R14)
bne L10055
clr R0
bra L10056
L10055:move #1,R0
L10056:move R0,-20(R14)
*line 1695
move #2,R0
move -18(R14),R1
move -20(R14),R2
eor R2,R1
beq L10057
clr R1
bra L10058
L10057:move #1,R1
L10058:sub R1,R0
move R0,-22(R14)
*line 1697
move #1,R0
move 8(R14),R1
asl R1,R0
and #7,R0
muls -22(R14),R0
asl #3,R0
ext.l R0
move.l R0,-16(R14)
*line 1698
move.l #$3e8,-(sp)
move.l -16(R14),-(sp)
jsr lmul
addq.l #8,sp
move.l R0,-12(R14)
*line 1700
*line 1701
move.l -4(R14),R0
sub.l -8(R14),R0
cmp.l -12(R14),R0
bge L301
*line 1702
move #75,(sp)
jsr _do1_aler
*line 1703
clr R0
bra L300
*line 1704
L301:
*line 1705
move #1,R0
bra L300
L300:unlk R14
rts
.globl _wait_up
.text
_wait_up:
~~wait_up:
link R14,#-12
~mk=-8
*line 1715
L305:
*line 1716
pea -2(R14)
pea -4(R14)
pea -6(R14)
pea -8(R14)
jsr _graf_mks
adda.w #16,sp
L304:
*line 1717
tst -4(R14)
bne L305
L303:L302:unlk R14
rts
.data
L165:.dc.b $2A,$2E,$0
