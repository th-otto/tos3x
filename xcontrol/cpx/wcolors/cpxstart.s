; cpxstart.s modified for wcolors window colors cpx
; ================================================================
; 900503 kbad Created
;
        xdef    cpxstart
        xdef    savecolors
        xdef    _savecolors
        xdef    defcolors
        xdef    _defcolors
        xdef    altcolors
        xdef    _altcolors

        xref    cpx_init
        xref    _cpx_init

        text

cpxstart:
        jmp cpx_init

        data

savecolors:
_savecolors:
;       Number of Colors ( MONOCHROME )
;               name        info        sizer
        dc.w    $11a1,$1100,$1100,$1100,$1101,$1101
;               closer      fuller      vbar
        dc.w    $1101,$1101,$1101,$1101,$1101
;               up          dn          vslid       velev
        dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
;               hbar    
        dc.w    $1101
;               lf          rt          hslid       helev
        dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
        dc.w    0,0,0,0,0,0


;       Number of Colors > 2
;               name        info        sizer
        dc.w    $11a1,$1100,$1100,$1100,$1101,$1101
;               closer      fuller      vbar
        dc.w    $1101,$1101,$1101,$1101,$1101
;               up          dn          vslid       velev
        dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
;               hbar    
        dc.w    $1101
;               lf          rt          hslid       helev
        dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
        dc.w    0,0,0,0,0,0


;       Number of Colors > 4
;               name        info        sizer
        dc.w    $9178,$9078,$9948,$9078,$9178,$9078
;               closer      fuller      vbar
        dc.w    $9178,$9078,$9178,$9078,$8068
;               up          dn          vslid       velev
        dc.w    $9178,$9078,$9178,$9078,$9179,$9079,$9178,$9078
;               hbar    
        dc.w    $8068   
;               lf          rt          hslid       helev
        dc.w    $9178,$9078,$9178,$9078,$9179,$9079,$9178,$9078
        dc.w    0,0,0,0,0,0


;               name        info        sizer
        dc.w    $11a1,$1100,$1100,$1100,$1101,$1101
;               closer      fuller      vbar
        dc.w    $1101,$1101,$1101,$1101,$1101
;               up          dn          vslid       velev
        dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
;               hbar    
        dc.w    $1101
;               lf          rt          hslid       helev
        dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
        dc.w    0,0,0,0,0,0



defcolors:
_defcolors:
;               name        info        sizer
        dc.w    $9178,$9078,$9948,$9078,$9178,$9078
;               closer      fuller      vbar
        dc.w    $9178,$9078,$9178,$9078,$8068
;               up          dn          vslid       velev
        dc.w    $9178,$9078,$9178,$9078,$9179,$9079,$9178,$9078
;               hbar    
        dc.w    $8068   
;               lf          rt          hslid       helev
        dc.w    $9178,$9078,$9178,$9078,$9179,$9079,$9178,$9078
        dc.w    0,0,0,0,0,0


; PREVIOUS DEFAULT - PLAIN WHITE
;               name        info        sizer
;       dc.w    $11a1,$1100,$1100,$1100,$1101,$1101
;               closer      fuller      vbar
;       dc.w    $1101,$1101,$1101,$1101,$1101
;               up          dn          vslid       velev
;       dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
;               hbar    
;       dc.w    $1101   
;               lf          rt          hslid       helev
;       dc.w    $1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
;       dc.w    0,0,0,0,0,0


altcolors:
_altcolors:
; F1 plain - no color
        dc.w $11a1,$1100,$1100,$1100,$1101,$1101,$1101,$1101,$1101,$1101
        dc.w $1101,$1101,$1101,$1101,$1101,$1111,$1111,$1101,$1101
        dc.w $1101,$1101,$1101,$1101,$1101
        dc.w $1111,$1111,$1101,$1101,$0000,$0000,$0000,$0000,$0000,$0000
; F2 greens
        dc.w $837b,$804b,$914b,$804b,$837b,$804b,$837b,$804b,$837b,$804b
        dc.w $884b,$837b,$804b,$837b,$804b,$836b,$806b,$837b,$804b
        dc.w $884b,$837b,$804b,$837b,$804b
        dc.w $836b,$806b,$837b,$804b,$0000,$0000,$0000,$0000,$0000,$0000
; F3 
        dc.w $9574,$906d,$c475,$906d,$9574,$906d,$9574,$906d,$9574,$906d
        dc.w $c17d,$9574,$906d,$9574,$906d,$915c,$915c,$9574,$906d
        dc.w $c17d,$9574,$906d,$9574,$906d
        dc.w $915c,$915c,$9574,$906d,$0000,$0000,$0000,$0000,$0000,$0000
; F4 
        dc.w $9477,$887f,$914a,$887f,$9477,$887f,$9477,$887f,$9477,$887f
        dc.w $c47f,$9477,$887f,$9477,$887f,$916a,$c46f,$9477,$887f
        dc.w $c47f,$9477,$887f,$9477,$887f
        dc.w $916a,$c46f,$9477,$887f,$0000,$0000,$0000,$0000,$0000,$0000

; F5 
        dc.w $4674,$402c,$446e,$402c,$467c,$402c,$467c,$402c,$467c,$402c
        dc.w $8048,$4674,$402c,$4674,$402c,$462c,$402c,$467c,$462c
        dc.w $8048,$4674,$402c,$4674,$402c
        dc.w $4624,$402c,$467c,$402c,$0000,$0000,$0000,$0000,$0000,$0000

; F6 Aqua
        dc.w $957D,$904D,$914d,$904d,$957d,$904d,$957d,$904d,$957d,$904d
        dc.w $8068,$957d,$904d,$957d,$904d,$9178,$9078,$957d,$904d
        dc.w $8068,$957d,$904d,$957d,$904d
        dc.w $9178,$9078,$957d,$904d,$0000,$0000,$0000,$0000,$0000,$0000

; F7 greys
        dc.w $8178,$8968,$9948,$8968,$9078,$8068,$8078,$8068,$8078,$8068
        dc.w $8068,$9078,$8068,$9078,$8068,$9169,$8068,$9178,$8068
        dc.w $8068,$9078,$8068,$9078,$8068
        dc.w $9169,$8068,$9178,$8068,$0000,$0000,$0000,$0000,$0000,$0000
; F8 black
        dc.w $1071,$1061,$1061,$1041,$1071,$1041,$1071,$1041,$1071,$1041
        dc.w $1041,$1071,$1041,$1071,$1041,$1161,$1041,$0171,$1041
        dc.w $1041,$1071,$1041,$1071,$1041
        dc.w $1161,$1041,$0171,$1041,$0000,$0000,$0000,$0000,$0000,$0000
; F9 greens
        dc.w $C574,$4075,$4475,$4075,$C574,$4075,$C574,$4075,$C574,$4075
        dc.w $C17D,$C574,$4075,$C574,$4075,$C14C,$C14C,$C174,$4075
        dc.w $C17D,$C574,$4075,$C574,$4075
        dc.w $C14C,$4075,$C574,$C14C,$0000,$0000,$0000,$0000,$0000,$0000

; F10 Default
        dc.w $9178,$9078,$9948,$9078,$9178,$9078,$9178,$9078,$9178,$9078
        dc.w $8068,$9178,$9078,$9178,$9078,$9179,$9079,$9178,$9078
        dc.w $8068,$9178,$9078,$9178,$9078
        dc.w $9179,$9079,$9178,$9078,$0000,$0000,$0000,$0000,$0000,$0000

        end



; Original Default - Plain White
;               name            info            sizer
;       dc.w    $11a1, $1100,   $1100, $1100,   $1101, $1101
;               closer          fuller          vbar
;       dc.w    $1101, $1101,   $1101, $1101,   $1101
;               up              dn              vslid           velev
;       dc.w    $1101, $1101,   $1101, $1101,   $1111, $1111,   $1101, $1101
;               hbar    
;       dc.w    $1101
;               lf              rt              hslid           helev
;       dc.w    $1101, $1101,   $1101, $1101,   $1111, $1111,   $1101, $1101
;       dcb.w   6,0
