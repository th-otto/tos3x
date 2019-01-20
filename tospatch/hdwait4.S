hd_wait:
            movem.l   d0-d7/a0-a6,-(a7)
            pea.l     hd_w7(pc)
            move.w    #9,-(a7)
            trap      #1
            addq.l    #6,a7
hd_w1:
            move.w    #16,d6
hd_w2:
            move.w    #-1,-(a7)
            move.w    #11,-(a7)
            trap      #13 /* Kbshift */
            addq.l    #4,a7
            btst      #3,d0
            bne.w     hd_w5
            move.w    d6,-(a7)
            move.l    (_dskbufp).w,-(a7)
            move.w    #1,-(a7)
            move.l    #0,-(a7)
            move.w    #$002A,-(a7) /* DMAread */
            trap      #14
            lea.l     14(a7),a7
            tst.l     d0
            bne.s     hd_w4
            movea.l   (_dskbufp).w,a0
            move.w    #$00FF,d0
            moveq.l   #0,d1
hd_w3:
            add.w     (a0)+,d1
            dbf       d0,hd_w3
            cmp.w     #$1234,d1
            bne.s     hd_w4
            pea.l     hd_w8(pc)
            move.w    #9,-(a7) /* Cconws */
            trap      #1
            addq.l    #6,a7
            bra.w     hd_w6
hd_w4:
            dbf       d6,hd_w2
            bra.s     hd_w1
hd_w5:
            pea.l     hd_w9(pc)
            move.w    #9,-(a7) /* Cconws */
            trap      #1
            addq.l    #6,a7
hd_w6:
            movem.l   (a7)+,d0-d7/a0-a6
            rts

#if COUNTRY == CTRY_DE
hd_w7:      dc.b 'Suche nach Festplatte'
            dc.b 0
hd_w9:      dc.b ' -> Abbruch'
            dc.b 0
#else
hd_w7:      dc.b 'Searching for harddisk'
            dc.b 0
hd_w9:      dc.b ' -> Cancel'
            dc.b 0
#endif
hd_w8:      dc.b ' -> OK'
            dc.b 0
            even
