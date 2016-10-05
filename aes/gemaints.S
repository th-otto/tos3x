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
.globl _signal
.text
_signal:
~~signal:
~e=8
link R14,#0
movem.l R7-R7/R11-R13,-(sp)
~p=R13
~p1=R12
~q1=R11
*line 48
move.l 8(R14),R8
move.l 12(R8),R13
*line 49
move.l 8(R14),R8
move 22(R8),R0
or R0,36(R13)
*line 52
move.l #_nrl,R11
move.l (R11),R12
bra L4
L5:L3:
*line 52
move.l R12,R11
move.l (R11),R12
L4:
*line 52
cmp.l R13,R12
beq L10000
move.l R12,R0
bne L5
L10000:L2:
*line 53
*line 54
cmp.l _rlr,R13
beq L6
*line 55
*line 56
clr R0
move 36(R13),R0
and 34(R13),R0
beq L7
*line 57
*line 58
move.l R12,R0
beq L8
*line 59
or #1,30(R12)
*line 60
*line 61
move.l (R12),(R11)
*line 62
move.l _drl,(R12)
*line 63
move.l R12,_drl
*line 64
L8:
*line 65
L7:
*line 66
L6:L1:tst.l (sp)+
movem.l (sp)+,R11-R13
unlk R14
rts
.globl _zombie
.text
_zombie:
~~zombie:
~e=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
*line 75
move.l 8(R14),R13
*line 75
move.l _zlr,4(R13)
*line 76
*line 77
tst.l _zlr
beq L10
*line 77
move.l _zlr,R8
move.l R13,8(R8)
L10:
*line 78
move.l #_zlr,R0
sub.l _elinkoff,R0
move.l R0,8(R13)
*line 79
move.l R13,_zlr
*line 80
move #2,20(R13)
*line 81
move.l R13,(sp)
jsr _signal
L9:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.data
