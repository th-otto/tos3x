_bufl equ $4b2
pun_ptr equ $516
_bootdev equ $446

     movea.l   4(a7),a6
     clr.l     -(a7)
     move.w    #$0020,-(a7) ; Super
     trap      #1
     addq.w    #6,a7
     move.l    d0,-(a7)
     move.w    #$0020,-(a7)
     lea.l     128(a6),a6
     tst.b     (a6)
     beq.s     noargs
     moveq.l   #1,d3
     moveq.l   #$46,d1
scancmd:
     move.l    d0,d5
     movea.l   a6,a3
     move.b    (a3)+,d0
     ext.w     d0
     subq.w    #1,d0
findC:
     move.b    (a3)+,d2
     and.w     #$005F,d2
     cmp.w     d1,d2
     dbeq      d0,findC
nextC:
     tst.b     (a3)
     beq.s     foundC
     cmpi.b    #$20,(a3)
     bne.s     foundC
     addq.w    #1,a3
     bra.s     nextC
foundC:
     bsr.s     atoi
     moveq.l   #$44,d1
     dbf       d3,scancmd
     tst.w     d0
     bne       install
     tst.w     d5
     bne       install
     movea.l   a6,a3
     addq.w    #1,a3
     bsr.s     atoi
     move.l    d0,d5
     bra.w     install

atoi:
     moveq.l   #2,d2
     moveq.l   #0,d0
atoi1:
     move.b    (a3)+,d1
     beq.s     atoi2
     sub.b     #$30,d1
     blt.s     atoi2
     cmp.b     #$09,d1
     bgt.s     atoi2
     mulu.w    #$000A,d0
     ext.w     d1
     add.w     d1,d0
     dbf       d2,atoi1
atoi2:
     rts

noargs:
     clr.w     d3
     move.w    #$002F,-(a7) ; Fgetdta
     trap      #1
     addq.w    #2,a7
     movea.l   d0,a3
     lea.l     35(a3),a3
     move.w    #0,-(a7)
     pea.l     fname(pc)
     move.w    #$004E,-(a7) ; Fsfirst
     trap      #1
     addq.w    #8,a7
     tst.w     d0
     beq.s     found
*     move.w    _bootdev.w,d0
* workaround for buggy short address form of as68
     dc.w $3038,_bootdev
     add.w     #$0041,d0
     move.b    d0,pname
     move.w    #0,-(a7)
     pea.l     pname(pc)
     move.w    #$004E,-(a7) ; Fsfirst
     trap      #1
     addq.w    #8,a7
     tst.w     d0
     beq.s     found
     pea.l     notfoundmsg(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     moveq.l   #-33,d5
     bra       failexit
found:
     bsr.s     atoi
     move.l    d0,d5

install:
     move.l    d0,d4
     bgt.s     instok
     tst.w     d5
     bgt.s     instok
     pea.l     usage(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     bra       failexit
instok:
* now: D4 = Data buffers, D5 = Fat buffers
     move.w    #$0200,d7 ; default sector size
*     movea.l   pun_ptr.w,a0
* workaround for buggy short address form of as68
     dc.w $2078,pun_ptr
     lea.l     82(a0),a1
     cmpi.l    #$41484449,(a1) ; 'AHDI'
     bne.s     noahdi
     cmpa.l    4(a1),a1
     bne.s     noahdi
     move.w    92(a0),d7
noahdi:
     add.w     #20,d7 ; sizeof(BCB)
*
* calculate resident size
*
     move.w    d4,d6
     add.w     d5,d6
     mulu.w    d7,d6
     addq.w    #4,d6
     move.l    a6,d0
     addq.l    #3,d0
     and.b     #$FC,d0
     movea.l   d0,a6
     add.l     d6,d0
     add.l     #$00040000,d0
     cmp.l     a7,d0
     bgt.s     oom
     add.l     #128,d6
     pea.l     crsp(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     move.l    d5,d0
     bsr       prtdec
     pea.l     fatroot(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     move.l    d4,d0
     bsr.s     prtdec
     pea.l     datadir(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     move.l    d6,d0
     moveq.l   #10,d1
     lsr.l     d1,d0
     bsr.s     prtdec
     pea.l     kdot(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     pea.l     copyright(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
*
* copy resident part to basepage
*
     lea.l     resprg(pc),a0
     lea.l     20(a6),a1
     moveq.l   #(resprgend-resprg)/4,d0
copyloop:
     move.l    (a0)+,(a1)+
     dbf       d0,copyloop
     jmp       20(a6)

oom:
     pea.l     nomem(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     move.l    d6,d0
     moveq.l   #10,d1
     lsr.l     d1,d0
     bsr.s     prtdec
     pea.l     kdotdot(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     moveq.l   #-39,d5

failexit:
     pea.l     failmsg(pc)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     bsr.s     wait
     trap      #1
     addq.w    #6,a7
     move.w    d5,-(a7)
     move.w    #$004C,-(a7)
     trap      #1
     illegal

prtdec:
     lea.l     digits+5(pc),a0
prtdec1:
     divu.w    #$000A,d0
     swap      d0
     add.b     #$30,d0
     move.b    d0,-(a0)
     clr.w     d0
     swap      d0
     bne.s     prtdec1
     pea.l     (a0)
     move.w    #$0009,-(a7)
     trap      #1
     addq.w    #6,a7
     rts
digits:                              dc.b '00000',0

wait:
     moveq.l   #90,d3
wait1:
     move.w    #$0025,-(a7) ; Vsync
     trap      #14
     addq.w    #2,a7
     dbf       d3,wait1
     rts

*
* Registers:
* D4 = Data buffers
* D5 = Fat buffers
* D6 = resident program size
* D7 = size of single buffer (sectorsize + 20)
* A6 = start of resident memory
* 
resprg:
*     lea.l     _bufl.w,a3
* workaround for buggy short address form of as68
     dc.w $47f8,_bufl
     moveq.l   #1,d3   ; loop counter for 2 lists
     move.w    d5,d0   ; count of fat buffers
nextbufl:
     movea.l   (a3),a1 ; get BCB
     move.l    a6,(a3)+ ; install new BCB
nextbcb:
     move.w    #-1,4(a6) ; invalidate b_bufdrv
     lea.l     20(a6),a0
     move.l    a0,16(a6)    ; install b_bufr
     subq.w    #1,d0
     beq.s     endlist
     lea.l     0(a6,d7.w),a0
     move.l    a0,(a6)      ; install b_link
     movea.l   a0,a6
     bra.s     nextbcb
endlist:
     move.l    a1,(a6)      ; set b_link of last node to original list
     adda.l    d7,a6
     move.w    d4,d0        ; next loop for count of root buffers
     dbf       d3,nextbufl
     trap      #1
     addq.w    #6,a7
     move.w    #0,-(a7)
     move.l    d6,-(a7)
     move.w    #$0031,-(a7)
     trap      #1
     addq.w    #8,a7
resprgend:

notfoundmsg:                         dc.b 13,10,'Couldn',39,'t find CACHE*.PRG or '
pname:                               dc.b 'X:',$5c,'AUTO',$5c
fname:                               dc.b 'CACHE*.PRG',0
crsp:                                dc.b 13,10,' ',0
fatroot:                             dc.b ' FAT/root, ',13,10,' ',0
datadir:                             dc.b ' data/dir cache buffers in ',0
kdot:                                dc.b 'K.',13,10,0
nomem:                               dc.b 13,10,'Not enough memory for ',0
kdotdot:                             dc.b 'K...',0

failmsg:                             dc.b 13,10,'No cache buffers added.',13,10
copyright:                           dc.b 27,'p',' CACHEnnn v901121                     ',27,'q',13,10
                                     dc.b 'Copyright ',189,' 1989-90, Atari Corporation',13,10,0
usage:                               dc.b 13,10,'Usage:',9,'cache [D]nnn [Fnnn] (nnn = 1 to 999)',13,10
                                     dc.b 9,'D=Data/Dir buffers',13,10
                                     dc.b 9,'F=FAT/Root Dir buffers',13,10
                                     dc.b 9,'no letter=nnn of each',13,10
                                     dc.b '`cache d10 F 20',39,' adds 10 D, 20 F buffers.',13,10,10
                                     dc.b 'Or, rename to CACHEnnn.PRG and run (equivalent to `cache nnn',39,').',13,10,0
