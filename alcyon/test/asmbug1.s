* SYSTEM TABLE PROGRAM **
*
TASKNO EQU 16
IRTTW0 EQU 0
IRTTW1 EQU 1
IRTCR1 EQU 0
IRTCR1 EQU 0   CRT WRITE
*
*
TAMAX DC.B TASKNO TASK MAX
ECMAX DC.B 10 ECB MAX
STMAX DC.B 10 SOFT TIMER MAX
REMAX DC.B 10
IOMAX DC.B 10
TNUM DC.B 10
TIBTT DC.L   TIB0
INITT DC.L   INTBUF
TIBIXAT DC.L  TIBIX
ECBAT   DC.L  ECB0
DATBAT  DC.L  DTTBA
RCBAT   DC.L  QARE
UCBIXTT DC.L  UCBL
IOTOAT  DC.L  TIMOUT
INTIXTT DC.L  IRTQ
IODTT   DC.L  IOAR
TRCBT   DC.L  TRBUF
TRCBL   DC.L  TRBUF+32
URS     DC.L  USER  USER'S PROGRAM LOC
UES     DC.L  0
UMTT    DC.L  0
MAXPLNO DC.B  1
        DC.B  0  YOBI
PIBADRR DC.L  PIB
DEVTBL  DC.L  DVCTBL0   FPK
USRVET DC.L  UTCB    I/O UTCB LOC
*
*
*
TIB0   EQU *      DBUG
      DC  $0101       ( 1-1 )
      DC  $0100
      DC  $0
      DC.L  $FD400
      DC.L   TCB0
      DC.W   $0
      DC.L   $0
      DC.L   $0
      DC.W   $0
      DC.L   $0
*
TIB1   EQU  *        FHS  ( A-1 )
      DC   $0A01
      DC   $0100
      DC   $0
      DC.L   $3E200
      DC.L   TCB1
      DC     $500
      DC.L   FHSLQ
      DC.L   $3E204
      DC   $4018      MONITA TASK
    DC.L   $0
*
TIB2   EQU *    IOS
     DC     $0B01
     DC     $0100
     DC     $0
     DC.L   $35D00
     DC.L   TCB2
     DC     $500
     DC.L   IOSQ
     DC.L   $3D504
     DC.W   $4018
    DC.L    $0
*
TIB3    EQU    *
      DC   $0C01
      DC   $0101
      DC   $0
      DC.L  $3A000
      DC.L  TCB3
      DC    $500
      DC.L   DMTQ
      DC.L  $3A004
      DC    $4018
      DC.L  $0
*
TIB4    EQU   *   IOED ( D-1 )
      DC   $0D01
     DC    $0100
     DC    $0
     DC.L  $3F700
     DC.L  TCB4
     DC    $500
     DC.L  IOEDQ
     DC.L  $3F704
     DC.W  $4018
     DC.L  $0
*
TIB5   EQU   *    FDC ( E-1 )
    DC   $0E01
    DC   $0100
    DC   $0
    DC.L  $3EF00
   DC.L   TCB5
    DC     $500
    DC.L   FDCQ
     DC.L  $3EF04
    DC.W   $4018
    DC.L   $0
*
*
TIB6 EQU *  TW0-IRT
     DC   $0000
     DC   $0000
     DC   $0500
     DC.L IRTTW0
     DC.L  TCB50
     DC.W  $0000
     DC.L  $0
     DC.L  $0
     DC.W  $0
     DC.L   STACK6     STACK LOC
*
*
TIB7  EQU   *   TW1 -IRT1
     DC     $0
     DC.W   $0
     DC.W   $600
     DC.L   IRTTW1
     DC.L   TCB60
     DC.W   $0
     DC.L   $0
     DC.L   $0
     DC.W   $0
     DC.L   STACK7    STACK LOC
*
TIB8 EQU *
     DC.W   $0
     DC.W   $0
     DC.W   $0700   LAVE
     DC.L   $0      ENTRY
     DC.L   TCB70
     DC.W   $0
     DC.L   $0
     DC.L   $0
     DC.W   $0
     DC.L   STACK8      STACK
*
*
TIB9    EQU   *
     DC.W    $0
     DC.W    $0
     DC.W    $0    LAVEL
     DC.L    $0    ENTRY
     DC.L    TCB80
     DC.W    $0
     DC.L    0
     DC.L    0
    DC.W      $0
     DC.L    STACK9    STACK
*
*
TIB10    EQU   *
    DC.W     $0
    DC.W     0
    DC.W     $0    LAVEL
    DC.L     $0    ENTRY
    DC.L     TCB90
    DC.W     0
    DC.L     0
    DC.L     0
    DC.W     0
    DC.L     STACK10       STACK
*
*
TIB11    EQU   *
    DC.W     $0
    DC.W     0
    DC.W     $0       LAVEL
    DC.L     $0       ENTRY
    DC.L     TCB100
    DC.W     0
    DC.L     0
    DC.L     0
    DC.W     0
    DC.L     STACK11        STACK
*
*
TIB12   EQU *
     DC.W    $0
     DC.W    $0
     DC.W    $0      LAVEL
     DC.L    $0      ENTRY
     DC.L    TCB110
     DC.W    $0
     DC.L    $0
     DC.L    $0
     DC.W    $0
     DC.L    STACK12       STACK
*
*
TIB13   EQU  *
     DC.W     $0
     DC.W     $0
     DC.W     $0      LAVEL
     DC.L     $0      ENTRY
     DC.L     TCB120
     DC.W     $0
     DC.L     $0
     DC.L     $0
     DC.W     $0
     DC.L     STACK13     STACK
*
*
TIB14     EQU    *
     DC.W      $0
     DC.W      $0
     DC.W      $0
      DC.W     $0      LAVEL
      DC.L     $0      ENTRY
      DC.L     TCB130
      DC.W     $0
      DC.L     $0
      DC.L     $0
      DC.W     $0
      DC.L     STACK14       STACK
*
TIB15     EQU    *
       DC.W     $0
       DC.W     $0
       DC.W     $0    LAVEL
       DC.L     $0    ENTRY
       DC.L     TCB140
       DC.W     $0
       DC.L     $0
       DC.L     $0
       DC.W     $0
       DC.L     STACK15      STACK
*
*
TIB16    EQU  *
      DC.W      $0     YOBI
      DC.W      $0
      DC.W      $0     LAVEL
      DC.L      $0    ENTRY
      DC.L      TCB150
      DC.W      $0
      DC.L      $0
      DC.L      $0
      DC.W      $0
      DC.L      STACK16
*
*
TIB17  EQU  *
     DC.W    $0
     DC.W    $0
     DC.W    $0
     DC.L    $0    ENTRY
     DC.L    TCB160
     DC.W    $0
     DC.L    $0
     DC.L    $0
     DC.W    $0
     DC.L    STACK17
*
      DS.B 100   YOBI
*
INTBUF EQU *
     DC.B     $3    3 KO
     DC.L     DATA1    
     DC.L     DATA1+1999
     DC.L     DATA2
     DC.L     DATA2+1999
     DC.L     DATA3
     DC.L     DATA3+1999
     DS.W    20
*
UCBL   EQU *
     DC.L    UCB0 1
     DC.L    UCB4 2
     DC.L    UCB5 3
     DC.L    UCB6 4
    DC.L   UCB1 1
    DC.L   UCB2 2
    DC.L   UCB3 3
     DC.L    UCB7
     DC.L    UCB8
     DC.L    UCB9
*
UCB0 EQU *    FPK 0
    DC.B $4,$2   2 LEVEL
    DC    0
    DC    0
    DC.L  BCT10
    DC.L  $FF0101    SHARED RAM LOC
    DC    0
    DC.L  $3FB00     ERROR EXIT
*
UCB1   DS  30 0
UCB2   DS  30 0
UCB3   DS  30 3
*
*
UCB4   EQU  *    CRT( R/W)
      DC.B   $0,$6   6 LEVEL
      DC     0
      DC     0
      DC.L   BCT4
      DC.L   $FEE015   AICA LOC
      DC.B   $11
      DC.B   0
      DC.B  $0A
      DC.L  $0    CODE A
      DC.B    0      ATTINTION TASK A
      DC.B   $0A
      DC.B    0      ATT TASK B
      DC.L   $0   CODE B
*
*
UCB5   EQU  *
       DC.B   $01,$05   TW0 ( IRT5)
       DC     0
       DC    $5     5 SEC
       DC.L    BCT5
       DC.L   $FEE009   LOC
       DC.L    $0
*
*
UCB6   EQU   *     TW1
       DC.B    $1,$6    IRT6
       DC      $0
       DC      $5   5 SEC
       DC.L   BCT6
       DC.L    $FEE001  LOC
       DC.L    0
*
UCB7   DS  30 7
UCB8   DS  30 8
UCB9   DS  30 9
*
IRTQ EQU *
     DC.L   0  1 LEVEL
     DC.L   IRT2  2
     DC.L   0  3
     DC.L   0  4
     DC.L   IRT5  5
     DC.L   IRT6  6
     DC.L   0  7
*
IRT2 EQU *   FDC
      DC.L    $0
      DC.L    $FF0101
      DC.W    $0500
      DC.W    $0000
      DC.W    $1
*
IRT21    EQU      *   YOBI
      DC.L     $0  NEXT LINK
      DC.L     $0  
      DC.W     $400
      DC.W     $0
      DC.L     0     EXIT(ENTRY)
      DC.L     0
*
*
IRT22    EQU   *    YOBI
      DC.L     $0   NEXT LINK
      DC.L     $0
      DC.W     $400  A
      DC.W     $0    B,C
      DC.L     $0    ENTRY
      DC.L     $0
*
*
IRT23     EQU   *   YOBI
      DC.L      $0
      DC.L      $0
      DC.W      $400
      DC.W      $0     B,C
      DC.L      $0
      DC.L      $0
*
*
IRT5     EQU   *   TW0
      DC.L     $0
      DC.L     $FEE009   DEVIC NO.
      DC.W     $100
      DC.W     $0
      DC.W     $6
*
*
IRT51    EQU   *    YOBI
      DC.L     $0    NEXT LINK
      DC.L     $0    DEVIC  NO.
      DC.W     $300
      DC.W     $0
      DC.W     $0   E1,F1
      DC.L     $0    ENTRY
      DC.W     $0    E1,F2
      DC.L     $0    D0
*
IRT52    EQU    *    YOBI
      DC.L     $0    NEXT LINK
      DC.L     $0    DEVIC NO.
      DC.W     $300
      DC.W     $0
      DC.W     $0   E1,F1
      DC.L     $0    D0
      DC.W     $0   E2,F2
      DC.L     $0    D0
*
*
IRT53    EQU    *   YOBI
      DC.L      $0
      DC.L      $0   DEVIC NO.
      DC.W      $300
      DC.W      $0
      DC.W      $0   E1,F1
      DC.L      $0   D0
      DC.W      $0   E2,F2
      DC.L      $0   D0
*
*
IRT6   EQU    *
      DC.L     IRT61     CRT
      DC.L     $FEE1005
      DC.W     $0
      DC.W     $0
      DC.W     $5
*
IRT61   EQU    *       TW1
      DC.L     0
      DC.L     $FEE001
      DC.W    $100
      DC.W    $0
      DC.W    $7
*
IRT62     EQU   *     YOBI
      DC.L     $0
      DC.L     $0
      DC.W     $300
      DC.W     $0
      DC.W     $0
      DC.W     $0
      DC.W     $0
      DC.L     $0
*
IRT63      EQU   *   YOBI
      DC.L     $0
      DC.L     $0
      DC.W     $300
      DC.W     $0
      DC.W     $0
      DC.W     $0
      DC.W     $0
      DC.L     $0
*
IRT64     EQU    *   YOBI
      DC.L     $0
      DC.L     $0
      DC.W     $300
      DC.W     $0
      DC.W     $0
      DC.W     $0
      DC.W     $0
      DC.L     $0
*
*
*
IOAR EQU *
      DC.W  $0500   CRT
      DC.W  $0000
      DC.W  $0600   TW0
      DC.W  $0000
      DC.W  $0700   TW1
      DC.W  $0000
      DC.W  $0000
      DC.W  $0000
*
*
*
DVCTBL0 EQU *
  DC.B   0
  DC.B   4    MAX LOGICAL NO.
  DC     3     FILE TASK
  DC     2
  DC     2
  DC.B   0
  DC.B   0
*
*   FDC = 0
  DC    'FD00'
  DC    $1F
  DC   256
  DC.B   $33
  DC.B   0    UCB NO.
    DC.B  $0   UINT NO.
  DC.B   $0E
*
*
*
   DC     'FD01'
   DC     $1F
   DC    256  BYTE
   DC.B   $33
   DC.B   0   UCB NO.
    DC.B   1     UNIT NO.
   DC.B   $0E
*
*
*
   DC     'FD02'
   DC     $1F
   DC    256
   DC.B    $33
   DC.B    0    UCB NO.
   DC.B   2   UNIT NO.
   DC.B    $0E
*
*
*
*
   DC      'FD03'
   DC      $1F
   DC    256
   DC.B    $33
   DC.B    0   UCB NO.
   DC.B  3  UNIT NO.	
   DC.B    $0E
*
    DC.B   $FF,$FF,$FF,$FF,$FF   END
*
*
PIB    EQU   *
      DC.L    POOL1S
      DC.L    POOL1E
*
*
TIBIX   EQU *
   DS.B  4*TASKNO    16*4
*
ECB0   EQU  *
    DS.B  20  1
    DS.B  20   2
    DS.B  20   3
    DS.B  20   4
    DS.B  20   5
    DS.B  20    6
    DS.B  20   7
    DS.B  20  8
    DS.B  20   9
    DS.B  20   10
*
DTTBA   EQU *
    DS.B   24  1
    DS.B   24  2
    DS.B   24  3
    DS.B   24  4
    DS.B   24  5
    DS.B   24  6
    DS.B    24  7
    DS.B   24  8
    DS.B   24  9
    DS.B   24  10
*
QARE  EQU   *
     DS.B  8*10
*
BCT0   DS.B 16
BCT1   DS.B 16
BCT2   DS.B  16
BCT3   DS.B  16
BCT4   DS.B  16  CRT
BCT5   DS.B  16  TW0
BCT6   DS.B  16  TW1
BCT7   DS.B  16
BCT8   DS.B  16
BCT9   DS.B  16
BCT10    DS.B   30     FPK BCT
*
TIMOUT  EQU *     TIMER OUT TABLE
      DS.B   10*6
*
TCB0 DS.B 130
TCB1 DS.B 128   Q BUFFER
TCB2 DS.B 128
TCB3 DS.B 128
TCB4 DS.B 128
TCB5 DS.B 128
TCB50   DS.B 128   TW-IRT0
TCB60   DS.B 128   TW-IRT1
TCB70   DS.B  128  CRT-IRT0 ( READ )
TCB80   DS.B  128  CRT-IRT1 (WRITE)
TCB90   DS.B  128  DENSO -IRT0 ( READ )
TCB100  DS.B  128  DENSO -IRT1 (WRITE )
TCB110  DS.B  128
TCB120  DS.B  128
TCB130  DS.B  128
TCB140  DS.B  128
TCB150  DS.B  128
TCB160   DS.B   128
*
*
FHSLQ  DS.B  $500
IOSQ   DS.B  $500
DMTQ   DS.B  $500
IOEDQ  DS.B  $500
FDCQ   DS.B  $500
*
TRBUF   EQU *
     DS.B  64    TRACE BUFFER
*
*
POOL1S   EQU   *
         DS.B     15000
         DS.B     512     YOBI
POOL1E   EQU   *     POLL BUF END
*
*
STACK0   DS.W   30
STACK1   DS.W   30
STACK2   DS.W   30
STACK3   DS.W   30
STACK4   DS.W   30
STACK5   DS.W   30
STACK6   DS.W   30
STACK7   DS.W   30
STACK8   DS.W   30
STACK9   DS.W   30
STACK10  DS.W   30
STACK11  DS.W   30
STACK12  DS.W   30
STACK13  DS.W   30
STACK14  DS.W   30
STACK15  DS.W   30
STACK16  DS.W   30
STACK17  DS.W   30
*
*
UTCB  EQU  *
      DC.L      TW0UCB  0
      DC.L      TW1UCB  1
      DC.L      CRTUCB  2
      DC.L      DENUCB  3
      DC.L       0     YOBI
      DC.L       0
      DC.L       0
*
*
TW0UCB   EQU   *     TW0
      DC.L    0     DVCENO.
      DC.L    TW0BUFA
      DC.W    0     BUFCT
TW0BUFA   DS.B 50   TW0-BUFFER
*
*
TW1UCB   EQU    *    TW0
      DC.L    0      DVCEN.
     DC.L     TW1BUFA
      DC.W    0      BUFCT
TW1BUFA   DS.B 50   TW1-BUFFER
*
*
CRTUCB   EQU   *
       DC.L    0   DVCENO.
       DC.L    CRTWBF
       DC.L    CRTRBF
       DC.W    0    FLAG
       DC.W    0    W-BUFCT
       DC.W    0    R-BUFCT
CRTWBF  DS.B 128
CRTRBF  DS.B 128
*
*
DENUCB   EQU   *
       DC.L     0    DVCENO.
       DC.L   DENBUF  DENSO-BUF
       DC.W     0    FLAG
       DC.W     0    DATA
DENBUF   DS.B 128
*
*
DATA1   DS.B   2000
DATA2   DS.B   2000
DATA3   DS.B   2000
*
*
*  USER'S EXTI (  )
USER     EQU   *
         LEA    USTBL,A0
*
JUS1    EQU   *
      MOVEA.L   (A0),A1    USER-TBL LOC
      BEQ       JUS9
      MOVE.B    1(A1),D0
      MOVE.B    2(A1),D1
*
      MOVE.B    D0,(A1)
      MOVE.B    D1,2(A1)
      ADDA.L    #6,A0
      BRA       JUS1
*
JUS9   EQU     *
       MOVEA.L   #$ABC,A1    DENSO DIVEC NO.
       MOVE.B     #3,D0
       MOVE.B     #$91,D1
       MOVE.B    D0,(A1)
       MOVE.B    D1,(A1)
*
        MOVEA.L   #$DCF,A1    DENSO-2 ADDRESS
       MOVE.B     D0,(A1)
       MOVE.B     D1,(A1)
       RTS
*
USTBL   EQU   *
       DC.L      $0     ADDRESS
       DC.B      $0,$0
       DC.L      $0
       DC.B      $0,0
       DC.L      $0
       DC.B      $0,0
       DC.L      $0
       DC.B      0,0
       DC.L      $0
       DC.B      0,0
       END
