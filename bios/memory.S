#include "asmdefs.h"
#include "tosvars.inc"

		text

		xdef initmmu
initmmu:
        lea.l     $00000700,a0
        lea.l     mmu_root_table,a1
        move.w    #$003F,d0
initmmu1:
        move.l    (a1)+,(a0)+
        dbf       d0,initmmu1
        TO_CRP(mmu_crp)
        TO_TC(mmu_tc)
        TO_TTR0(mmu_tt0)
        TO_TTR1(mmu_tt1)
        rts

	    data
mmu_root_table:
        dc.l      $00000742
        dc.l      $10000001
        dc.l      $20000001
        dc.l      $30000001
        dc.l      $40000001
        dc.l      $50000001
        dc.l      $60000001
        dc.l      $70000001
        dc.l      $80000041
        dc.l      $90000041
        dc.l      $a0000041
        dc.l      $b0000041
        dc.l      $c0000041
        dc.l      $d0000041
        dc.l      $e0000041
        dc.l      $00000782
        dc.l      $000007c2
        dc.l      $01000001
        dc.l      $02000001
        dc.l      $03000001
        dc.l      $04000001
        dc.l      $05000001
        dc.l      $06000001
        dc.l      $07000001
        dc.l      $08000001
        dc.l      $09000001
        dc.l      $0a000001
        dc.l      $0b000001
        dc.l      $0c000001
        dc.l      $0d000001
        dc.l      $0e000001
        dc.l      $0f000001
        dc.l      $00000041
        dc.l      $01000041
        dc.l      $02000041
        dc.l      $03000041
        dc.l      $04000041
        dc.l      $05000041
        dc.l      $06000041
        dc.l      $07000041
        dc.l      $08000041
        dc.l      $09000041
        dc.l      $0a000041
        dc.l      $0b000041
        dc.l      $0c000041
        dc.l      $0d000041
        dc.l      $0e000041
        dc.l      $000007c2
        dc.l      $00000001
        dc.l      $00100001
        dc.l      $00200001
        dc.l      $00300001
        dc.l      $00400001
        dc.l      $00500001
        dc.l      $00600001
        dc.l      $00700001
        dc.l      $00800001
        dc.l      $00900001
        dc.l      $00a00001
        dc.l      $00b00001
        dc.l      $00c00001
        dc.l      $00d00001
        dc.l      $00e00001
        dc.l      $00f00041
mmu_crp:
        dc.l                      $80000002,$00000700; no limit, short desc, addr $700
mmu_tc:
        dc.l                      $80f04445 ; enable, pagesize 32k
mmu_tt0:
        dc.l                      $017e8107
mmu_tt1:
        dc.l                      $807e8507
