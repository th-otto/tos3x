                clr.w   (_bootpref).w

                clr.w   -(sp)
                movea.l (_dskbufp).w,a0
                pea     $0200(a0)
                move.w  #1,-(sp)
                clr.l   -(sp)

                bsr     _dmaread             /* read first sector of this device */
                lea     12(sp),sp

                tst.l   d0
                bne.s   ctend

                movea.l (_dskbufp).w,a0
                lea     $0200(a0),a0
                movea.l a0,a1
                moveq   #0,d0
                move.w  #$01ff,d1
ctcheck1:       add.w   (a1)+,d0
                dbra    d1,ctcheck1
                cmpi.w  #$1234,d0
                bne.s   ctend

                jmp     (a0)            /* execute */

ctend:          rts
