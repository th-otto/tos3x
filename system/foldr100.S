_sysbase equ $4f2

MDBLKSIZE equ 66

    text

    movea.l   4(a7),a4 ; get basepage
    move.l    4(a4),d6 ; get hitpa
    sub.l     a4,d6
    lea.l     130(a4),a4
    lea.l     4(a4),a0
    lea.l     resprg(pc),a1
*
* copy resident part to commandline area
*
    moveq.l   #(resprgend-resprg-1),d0
copyloop:
    move.b    (a1)+,(a0)+
    dbf       d0,copyloop
    clr.l     -(a7)
    move.w    #$0020,-(a7) ; Super
    trap      #1
    addq.w    #6,a7
    move.l    d0,-(a7)
    move.w    #$0020,-(a7)
    bsr       checktos
    trap      #1 ; return to user mode
    addq.w    #6,a7
    bne.s     failexit
    bsr       findprg
*
* calculate resident size
*
    move.w    numfold,d7
    mulu.w    #MDBLKSIZE,d7
    add.l     #$00000080,d7
    cmp.l     d7,d6
    ble.s     oom
*
* push arguments, then jump to resident part
*
    clr.w     -(a7)
    move.l    d7,-(a7)
    move.w    #$0031,-(a7) ; Ptermres
    movea.l   a4,a0
    move.w    numfold,d0
    subq.w    #1,d0
    lea.l     16(a5),a5 ; install new list into _root[4]
    jmp       4(a4)
oom:
    pea.l     nomem(pc)
    move.w    #$0009,-(a7) ; Cconws
    trap      #1
    addq.w    #6,a7
failexit:
    movea.l   #hitkey,a0
    move.l    a0,-(a7)
    move.w    #$0009,-(a7) ; Cconws
    trap      #1
    addq.w    #6,a7
    move.w    #$0002,-(a7)
    move.w    #$0002,-(a7)
    trap      #13 ; Bconin(2)
    addq.w    #4,a7
    move.w    #$0001,-(a7)
    move.w    #$004C,-(a7) ; Pterm
    trap      #1
    illegal

nomem:
    dc.b 'Not enough memory to add folders.',13,10,0

*
* get p_root pointer from OS header,
* or from known TOS versions
*
checktos:
    * movea.l   (_sysbase).w,a3 ; get pointer to OSHDR
* workaround for buggy short address form of as68
    dc.w $2678,_sysbase
    movea.l   32(a3),a5 ; get p_root
    cmpi.w    #$0E4A,30(a3) ; check dosdate
    bge.s     tosok
    lea.l     tostable(pc),a0
searchtos:
    move.l    (a0)+,d1
    beq.w     wrongtos
    movea.l   (a0)+,a5
    cmp.l     24(a3),d1
    bne.s     searchtos
tosok:
    moveq.l   #0,d0
    rts

tostable:
    dc.l $11201985,$000056fa
    dc.l $02061986,$000056fa
    dc.l $04241986,$000056fa
    dc.l $06011986,$00007e0a
    dc.l 0,0
    dc.l 0,0
    dc.l 0,0
    dc.l 0
                                     
wrongtos:
    pea.l     tosmsg(pc)
    move.w    #$0009,-(a7)
    trap      #1
    addq.w    #6,a7
    moveq.l   #1,d0
    rts

tosmsg:       dc.b 'Can',$27,'t add folders to this version of TOS',13,10,0
              even

*
* Find program in autofolder, and
* convert number in name.
* Returns number of folders in global numfold
*
findprg:
    move.l    #dta,-(a7)
    move.w    #$001A,-(a7) ; Fsetdta
    trap      #1
    addq.w    #6,a7
    move.w    #$0000,-(a7)
    move.l    #pname,-(a7)
    move.w    #$004E,-(a7) ; Fsfirst
    trap      #1
    addq.w    #8,a7
    tst.w     d0
    bmi       findprg4
    moveq.l   #0,d7
    moveq.l   #0,d0
    lea.l     dta+35,a0
findprg1:
    move.b    (a0)+,d0
    beq.s     findprg2
    cmp.b     #$2E,d0
    beq.s     findprg2
    sub.b     #$30,d0
    bmi.s     findprg4
    cmp.b     #$0A,d0
    bge.s     findprg4
    mulu.w    #$000A,d7
    add.w     d0,d7
    bra.s     findprg1
findprg2:
    move.w    d7,numfold
    beq.s     findprg3
    add.w     d7,numfold
    moveq.l   #0,d0
    move.w    d7,d0
    move.l    d0,-(a7)
    bsr       prtdec
    addq.w    #4,a7
    movea.l   #extramsg,a0
    move.l    a0,-(a7)
    move.w    #$0009,-(a7)
    trap      #1
    addq.w    #6,a7
    mulu.w    #$0084,d7
    move.l    d7,-(a7)
    bsr       prtdec
    addq.w    #4,a7
    movea.l   #bytesmsg,a0
    move.l    a0,-(a7)
    move.w    #$0009,-(a7) ; Cconws
    trap      #1
    addq.w    #6,a7
    rts
findprg3:
    move.l    #badnum,-(a7)
    move.w    #$0009,-(a7) ; Cconws
    trap      #1
    addq.w    #6,a7
    bra       failexit
findprg4:
    move.l    #notfoundmsg,-(a7)
    move.w    #$0009,-(a7)
    trap      #1
    addq.w    #6,a7
    bra       failexit

pname:                               dc.b $5c,'AUTO',$5c
fname:                               dc.b 'FOLDR*.PRG',0
notfoundmsg:                         dc.b 'Cannot find ',$5c,'AUTO',$5c,'FOLDRnnn.PRG',10,0
badnum:                              dc.b 'Bad number of folders specified.',10,0
                                     even

*
* Registers:
* D0 = Number of folders - 1
* A0 = start of resident memory + 6
* A4 = start of resident memory + 2
* A5 = p_root from os header
* 
resprg:
    lea.l     MDBLKSIZE(a0),a1
    move.l    a1,(a0)
    move.w    #$0004,-2(a0)
    movea.l   a1,a0
    dbf       d0,resprg
    move.l    (a5),-MDBLKSIZE(a0)
    move.l    a4,(a5)
    trap      #1
    illegal
resprgend:


prtdec:
    lea.l     digits,a0
    move.l    4(a7),d0
prtdec1:
    divu.w    #$000A,d0
    swap      d0
    add.b     #$30,d0
    move.b    d0,(a0)+
    swap      d0
    and.l     #$0000FFFF,d0
    bne.s     prtdec1
    lea.l     digits+10,a1
prtdec2:
    move.b    -(a0),(a1)+
    cmpa.l    #digits,a0
    bne.s     prtdec2
    clr.b     (a1)+
    move.l    #digits+10,-(a7)
    move.w    #$0009,-(a7)
    trap      #1
    addq.w    #6,a7
    rts

            data

hitkey:     dc.b $0d,$0a,'Hit any key to exit...',$0d,$0a,0
extramsg:   dc.b ' extra folders in ',0
bytesmsg    dc.b ' bytes',$0d,$0a,0

		    bss

dta:        ds.b 44
numfold:    ds.w 1
digits:     ds.b 2*10
            ds.w 1
