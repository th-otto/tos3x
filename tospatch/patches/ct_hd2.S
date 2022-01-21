dodmawrite:     moveq   #-1,d0          /* write function has been left out */
                rts                     /* return error */

/*   ***   Programmcode to read bootsektor *** */

read_boot:      lea     $ffff8604.w,a3
                lea     $ffff8606.w,a2

                move.w  #$00c8,d2
                move.w  #$00ca,d3

                st      (_flock).w      /* lock FIFO */

                move.w  d3,(a2)
                move.w  #$0080,(a3)

                bsr.s   ctwait

                clr.w   (a3)
                move.w  d2,(a2)

                bsr.s   ctwait

                bsr.s   ctibus

                lea     ctcmd(pc),a1    /* get at-bus command */
                moveq   #5,d1           /* (6 bytes lang) */

sendct1:        bsr.s   ctgbyte
                bne.s   ctend2
                moveq   #0,d0
                move.b  (a1)+,d0        /* fetch byte */
                move.w  d0,(a3)         /* and send it */
                dbra    d1,sendct1

                move.l  #30*200,d0      /* 30s */
                bsr.s   ctgbyte1
                movea.l a0,a1
                move.w  #$01ff,d1       /* 512 byte */

ctread:         bsr.s   ctgbyte
                bne.s   ctend2
                move.w  (a3),d0
                move.b  d0,(a1)+
                dbra    d1,ctread
                bsr.s   ctgbyte
                move.w  (a3),d0
                and.w   #$00ff,d0
ctend2:         move.w  #$0080,(a2)

                sf      (_flock).w

                rts

/* *** subroutines *** */

ctwait:         moveq   #4,d0
                add.l   (_hz_200).w,d0
ctwait1:        cmp.l   (_hz_200).w,d0
                bhi.s   ctwait1
                rts

ctgbyte:        moveq   #100,d0         /* 0,5s */

ctgbyte1:       move.w  d3,(a2)
                move.w  #1,(a3)
                move.w  d2,(a2)

                add.l   (_hz_200).w,d0
                move.l  d0,$00000a82.w

ctgbyte2:       move.w  (a3),d0         /* error */
                and.w   #1,d0
                bne.s   ctgbyte3
                move.l  $00000a82.w,d0
                cmp.l   (_hz_200).w,d0
                bhi.s   ctgbyte2

                bsr.s   ctibus1
                moveq   #-1,d0
                rts

ctgbyte3:       bsr.s   ctibus1
                moveq   #0,d0
                rts

ctibus:         move.w  d3,(a2)
                move.w  #2,(a3)
                move.w  d2,(a2)
                clr.w   (a3)

ctibus1:        move.w  d3,(a2)
                clr.w   (a3)
                move.w  d2,(a2)
                rts

ctcmd:          dc.b $08,$00,$00,$00,$01,$01
