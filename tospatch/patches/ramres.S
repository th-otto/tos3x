                lea     reset(PC),A6
                bra     memchk

/*   ---------------------------------------- */

newkres:        move    #$2700,SR       /* disable interrupts */

                clr.l   (_memvalid).w   /* make memory configuration invalid */

                movea.l 4.w,A0
                lea     (_buserror).w,A1
                moveq   #(($0100-$0008)/4)-1,D0 /* clear $8-$100 */
newkres1:       move.l  A0,(A1)+
                dbra    D0,newkres1
                bra     _main

resethandler:   cmpa.l  #resetvec,A6
                beq.s   noreset

                lea     _main(pc),a6

crc:            lea     _os_entry(PC),A3     /* start address */
                lea     rom_crc_table,A1
                moveq   #numbanks-1,D7

crc0:           move.l  #banksize,D2    /* bytes to check per loop */
                moveq   #0,D0           /* set starting values */
                moveq   #0,D5
                movea.l A3,A0

crc1:           move.w  (A0),D3
                addq.l  #numbanks,A0

                move.w  D5,D6
                lsl.w   #8,D5
                lsr.w   #8,D6
                eor.b   D3,D6
                add.w   D6,D6
                move.w  0(A1,D6.w),D4
                eor.w   D4,D5

                move.w  D0,D1
                lsl.w   #8,D0
                eor.w   D3,D1
                lsr.w   #8,D1
                add.w   D1,D1
                move.w  0(A1,D1.w),D4
                eor.w   D4,D0

                subq.l  #1,D2           /* already done? */
                bne.s   crc1            /* no => */

                move.w  (A0),D1
                move.b  D1,D6
                addq.l  #numbanks,A0
                move.b  (A0)+,D1
                cmp.w   D1,D0
                bne.s   oldkeyresetj

                lsl.w   #8,D6
                move.b  (A0),D6
                cmp.w   D6,D5
oldkeyresetj:   bne     coldboot

                addq.l  #2,A3           /* 2 passes */
                subq.l  #2,D7           /* again? */
                bpl.s   crc0            /* yes */

                jmp     (A6)

/* ---------------------------------------- */

reset:          beq     go_on

                lea     checkmem(PC),A6
                bra.s   crc

checkmem:       lea     ($00007FFE).w,SP

                movea.l (_buserror).w,A4          /* save buserror vector */
                move.l  #coldboot,(_buserror).w /* set new vector */
                lea     _os_entry(PC),A2     /* set end address */
                move.l  #$00020000,D7   /* distance of probes */
                movea.l D7,A0           /* set start address */
                move.w  #$FB55,D3       /* value to add */

loop1:          movea.l A0,A1
                move.w  D0,D2
                moveq   #$2A,D1
loop2:          move.w  D2,-(A1)
                add.w   D3,D2
                dbra    D1,loop2
                movea.l A0,A1
                moveq   #$2A,D1
loop3:          eor.w   D0,-(A1)
                bne.s   oldkeyresetj
                add.w   D3,D0
                dbra    D1,loop3
                adda.l  D7,A0
                cmpa.l  A2,A0
                bls.s   loop1
                movea.l A2,A0
                move.l  A4,(_buserror).w

                movem.l (A1),D0-D7/A1/A3-A6 /* get zeroes */
clrmem:         movem.l D0-D7/A1/A3-A6,-(A0) /* clear page */
                movem.l D0-D7/A1/A3-A6,-(A0)
                movem.l D0-D7/A1/A3-A6,-(A0)
                movem.l D0-D7/A1/A3-A6,-(A0)
                movem.l D0-D7/A1/A4-A6,-(A0)
                cmpa.w  #$0100,A0       /* clear memory to $100 */
                bhi.s   clrmem

                move.l  A2,(_phystop).w  /* highest address as phystop */

                move.l  #$752019F3,(_memvalid).w
                move.l  #$237698AA,(_memval2).w
                move.l  #$5555AAAA,(_memval3).w
                clr.l   (_ramtop).w
                move.l  #$1357BD13,(_ramvalid).w

                move.b  (memctrl).w,(memconf).w

go_on:          move.l  #$31415926,(resvalid).w
                move.l  #resethandler,(resvector).w

