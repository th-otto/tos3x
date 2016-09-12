OPNFILES equ 81
NCURDIR equ 40
KBBUFSZ equ 80
    
	bss

    ds.b $3b22
    
biosav: ds.l 1

     ds.l 1 ; unused

savesr: ds.w 1

fstack: ds.w 1034
fstrt: ds.l 1
old_tik: ds.l 1
old_2: ds.l 1

    ds.b $c10
    
	xdef _time
_time: ds.w 1

	ds.b $18

	xdef _ofdfree
_ofdfree: ds.l 1

	ds.b $3c

	xdef _glbkbchar
_glbkbchar: ds.l 3*KBBUFSZ

	xdef _ofdlist
_ofdlist: ds.l 1

 	ds.l 1

	xdef _glbcolumn
_glbcolumn: ds.w 3

	ds.b $1e8

	xdef _ldivr
_ldivr: ds.l 1

	xdef _ofdbuf
_ofdbuf: ds.b 56*100

	ds.b $74

	xdef _rwerr
_rwerr: ds.l 1

    ds.w 1

	xdef _pmd
_pmd: ds.l 3

	xdef _sft
_sft: ds.b 10*OPNFILES

	ds.b 30
	
	xdef _run
_run: ds.l 1

    ds.b $78
    
	xdef _fill
_fill: ds.b 4

    ds.b $54
    
	xdef _secbuf
_secbuf: ds.b 4*512

    ds.b $26
    
	xdef _dirtbl
_dirtbl: ds.l NCURDIR

    ds.b 4
    
	xdef _errbuf
_errbuf: ds.l 3

    ds.b $32
    
	xdef _diruse
_diruse: ds.b NCURDIR

	xdef _drvtbl
_drvtbl: ds.l 16

	xdef _drvsel
_drvsel: ds.w 1

    ds.b $16
    
	xdef _errdrv
_errdrv: ds.w 1

    ds.b $2a
    
	xdef _ospd
_ospd: ds.b 256

    xdef _buptr
_buptr: ds.l 3

    ds.l 1
    
	xdef _date
_date: ds.w 1

	xdef _bcbx
_bcbx: ds.b 4*20

    ds.w 1

    xdef _beptr
_beptr: ds.l 3
