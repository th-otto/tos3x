.globl _rlr
.globl _drl
.globl _nrl
.globl _eul
.globl _dlr
.globl _zlr
.globl _elinkoff
.globl _indisp
.globl _infork
.globl _fpt
.globl _fph
.globl _fpcnt
.globl _wind_spb
.globl _cda
.globl _curpid
.globl _gl_tickt
.globl _drwaddr
.globl _old_mcod
.globl _xrat
.globl _yrat
.globl _intin
.globl _ptsin
.globl _gl_nplan
.globl _D
.globl _gl_bvdis
.comm _gl_bvdis,2
.globl _gl_bvhar
.comm _gl_bvhar,2
.globl _gl_recd
.comm _gl_recd,2
.globl _gl_rlen
.comm _gl_rlen,2
.globl _gl_rbuf
.comm _gl_rbuf,4
.globl _gl_play
.comm _gl_play,2
.globl _gl_store
.comm _gl_store,4
.globl _gl_mx
.comm _gl_mx,2
.globl _gl_my
.comm _gl_my,2
.globl _ap_init
.text
_ap_init:
~~ap_init:
~pglobal=8
link R14,#-4
*line 118
move.l 8(R14),R8
move.l #$3400001,(R8)
*line 119
move.l 8(R14),R8
add.l #$4,R8
move.l _rlr,R9
move 28(R9),(R8)
*line 120
move.l 8(R14),R8
add.l #$14,R8
move _gl_nplan,(R8)
*line 121
move.l 8(R14),R8
add.l #$16,R8
move.l #_D,(R8)
*line 122
move.l 8(R14),R8
add.l #$1a,R8
move _gl_bvdis,(R8)
*line 123
move.l 8(R14),R8
add.l #$1c,R8
move _gl_bvhar,(R8)
*line 124
move.l _rlr,R8
clr 38(R8)
*line 125
move.l _rlr,R8
move 28(R8),R0
bra L1
L1:unlk R14
rts
.globl _ap_exit
.text
_ap_exit:
~~ap_exit:
link R14,#-4
*line 133
jsr _mn_clsda
*line 134
*line 135
move.l _rlr,R8
tst 148(R8)
beq L3
*line 135
move.l #8898+_D,(sp)
move.l _rlr,R8
move 148(R8),-(sp)
move.l _rlr,R8
move 28(R8),-(sp)
move #1,-(sp)
jsr _ap_rdwr
addq.l #6,sp
*line 136
L3:
*line 137
jsr _all_run
*line 138
jsr _release
*line 139
move.l _rlr,R8
move 28(R8),(sp)
jsr _mn_free
*line 140
move #1,R0
bra L2
L2:unlk R14
rts
.globl _rd_mymsg
.text
_rd_mymsg:
~~rd_mymsg:
~buffer=8
link R14,#-4
*line 149
*line 150
move.l _rlr,R8
tst 38(R8)
beq L5
*line 151
move #16,(sp)
move.l _rlr,-(sp)
add.l #40,(sp)
move.l 8(R14),-(sp)
jsr _LBCOPY
addq.l #8,sp
*line 152
move.l _rlr,R8
clr 38(R8)
*line 153
move #1,R0
bra L4
*line 154
bra L6
L5:
*line 155
clr R0
bra L4
L6:L4:unlk R14
rts
.globl _ap_rdwr
.text
_ap_rdwr:
~~ap_rdwr:
~code=8
~id=10
~length=12
~pbuff=14
link R14,#-4
*line 173
move.l R14,(sp)
add.l #10,(sp)
move 8(R14),-(sp)
jsr _ev_block
addq.l #2,sp
L7:unlk R14
rts
.globl _ap_find
.text
_ap_find:
~~ap_find:
~pname=8
link R14,#-10
movem.l R7-R7/R13-R13,-(sp)
~p=R13
~temp=-10
*line 188
move.l 8(R14),(sp)
move.l R14,-(sp)
add.l #-10,(sp)
jsr _LSTCPY
addq.l #4,sp
*line 190
clr (sp)
move.l R14,-(sp)
add.l #-10,(sp)
jsr _fpdnm
addq.l #4,sp
move.l R0,R13
*line 191
move.l R13,R0
beq L10000
move 28(R13),R0
bra L10002
L10000:move #-1,R0
L10002:bra L8
L8:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _ap_tplay
.text
_ap_tplay:
~~ap_tplay:
~pbuff=R7
~length=12
~scale=14
link R14,#-12
movem.l R5-R7,-(sp)
~i=R6
~f=-8
~ad_f=-12
*line 213
move.l 8(R14),R7
*line 213
lea -8(R14),R8
move.l R8,-12(R14)
*line 214
jsr _dsptch
*line 215
clr _gl_play
*line 216
move _xrat,_gl_mx
*line 217
move _yrat,_gl_my
*line 219
clr R6
bra L12
L13:
*line 220
*line 221
*line 222
move #8,(sp)
move.l R7,-(sp)
move.l -12(R14),-(sp)
jsr _LBCOPY
addq.l #8,sp
*line 223
add.l #$8,R7
*line 224
*line 225
*line 226
*line 227
*line 228
move.l -8(R14),R0
bra L15
*line 229
L16:
*line 230
*line 231
move 14(R14),R0
ext.l R0
move.l R0,-(sp)
move.l #$64,-(sp)
move.l -4(R14),-(sp)
jsr lmul
addq.l #8,sp
move.l R0,-(sp)
jsr ldiv
addq.l #8,sp
move.l R0,(sp)
jsr _ev_timer
*line 232
clr.l -8(R14)
bra L14
*line 233
L17:
*line 234
*line 235
*line 236
tst _gl_play
bne L18
*line 237
clr (sp)
move.l #_justretf,-(sp)
jsr _i_lptr1
addq.l #4,sp
*line 238
clr.l (sp)
move #127,-(sp)
jsr _gsx_ncod
addq.l #2,sp
*line 239
move.l #_drwaddr,(sp)
jsr _m_lptr2
*line 240
clr (sp)
move.l #_justretf,-(sp)
jsr _i_lptr1
addq.l #4,sp
*line 241
clr.l (sp)
move #126,-(sp)
jsr _gsx_ncod
addq.l #2,sp
*line 242
move.l #_gl_store,(sp)
jsr _m_lptr2
*line 243
L18:
*line 244
move.l #_mchange,-8(R14)
*line 245
move #1,_gl_play
bra L14
*line 246
L19:
*line 247
*line 248
move.l #_bchange,-8(R14)
bra L14
*line 249
L20:
*line 250
*line 251
move.l #_kchange,-8(R14)
bra L14
*line 252
bra L14
L15:tst R0
beq L16
cmp #1,R0
beq L19
cmp #2,R0
beq L17
cmp #3,R0
beq L20
bra L14
L14:
*line 253
*line 254
*line 255
*line 256
tst.l -8(R14)
beq L21
*line 256
move.l -4(R14),(sp)
move.l -8(R14),-(sp)
jsr _forkq
addq.l #4,sp
*line 257
*line 258
*line 259
L21:
*line 260
jsr _dsptch
L11:
*line 219
add #1,R6
L12:
*line 219
cmp 12(R14),R6
blt L13
L10:
*line 263
*line 264
tst _gl_play
beq L22
*line 273
*line 274
move.l _drwaddr,(sp)
jsr _i_lptr1
*line 275
clr.l (sp)
move #127,-(sp)
jsr _gsx_ncod
addq.l #2,sp
*line 276
clr (sp)
move.l _gl_store,-(sp)
jsr _i_lptr1
addq.l #4,sp
*line 277
clr.l (sp)
move #126,-(sp)
jsr _gsx_ncod
addq.l #2,sp
*line 278
*line 282
*line 283
*line 284
clr _gl_play
*line 285
L22:L9:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _ap_trecd
.text
_ap_trecd:
~~ap_trecd:
~pbuff=R7
~length=R6
link R14,#-4
movem.l R3-R7,-(sp)
~i=R5
~code=R4
~trash=-4
*line 305
move.l 8(R14),R7
*line 305
move 12(R14),R6
*line 305
jsr _cli
*line 306
move #1,_gl_recd
*line 307
move R6,_gl_rlen
*line 308
move.l R7,_gl_rbuf
*line 309
jsr _sti
*line 312
bra L26
L25:
*line 313
move.l #$64,(sp)
jsr _ev_timer
L26:
*line 313
tst _gl_recd
bne L25
L24:
*line 316
jsr _cli
*line 317
clr _gl_recd
*line 318
sub _gl_rlen,R6
*line 319
clr _gl_rlen
*line 321
clr.l _gl_rbuf
*line 322
jsr _sti
*line 326
clr R5
bra L29
L30:
*line 327
*line 328
clr.l R4
*line 329
move.l R7,R8
move.l (R8),-4(R14)
*line 330
*line 331
move.l -4(R14),R0
cmp.l #_tchange,R0
bne L31
*line 332
clr.l R4
*line 333
*line 334
*line 335
L31:
*line 336
*line 337
move.l -4(R14),R0
cmp.l #_mchange,R0
bne L32
*line 337
move.l #$2,R4
L32:
*line 338
*line 339
move.l -4(R14),R0
cmp.l #_kchange,R0
bne L33
*line 339
move.l #$3,R4
L33:
*line 340
*line 341
move.l -4(R14),R0
cmp.l #_bchange,R0
bne L34
*line 341
move.l #$1,R4
L34:
*line 342
move.l R7,R8
move.l R4,(R8)
*line 343
add.l #$8,R7
L28:
*line 326
add #1,R5
L29:
*line 326
cmp R6,R5
blt L30
L27:
*line 345
move R6,R0
bra L23
L23:tst.l (sp)+
movem.l (sp)+,R4-R7
unlk R14
rts
.data
