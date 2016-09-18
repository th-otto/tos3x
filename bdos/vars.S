*
* common variables
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*

OPNFILES equ 81
NCURDIR equ 40
KBBUFSZ equ 80
SUPSIZ equ 1024
MAXQUICK equ 4
    
	bss

*	ds.b $800       ; the BIOS vars go here
	
	ds.b $3b22

	xdef biosav
biosav: ds.l 1

    ds.l 1 ; unused

	xdef savesr
savesr: ds.w 1

	xdef fstack
fstack: ds.w 1034
	xdef fstrt
fstrt: ds.l 1
	xdef old_tik
old_tik: ds.l 1
	xdef old_2
old_2: ds.l 1

    ds.l 1
    ds.l 1
    
    xdef _msec
_msec: ds.w 1

    ds.b $402
    
    xdef _supstk
_supstk: ds.w SUPSIZ

	ds.b 1
	even
	
	ds.b 1
	even
	
	xdef _time
_time: ds.w 1

	ds.b 8

	xdef _root
_root: ds.l MAXQUICK

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

	ds.w 1
	ds.w 1

	xdef _rrecno
_rrecno: ds.l 1

	ds.b 2
	ds.b 2

	xdef _rwrtflg
_rwrtflg: ds.w 1

	ds.b $1c6

	xdef _rdm
_rdm: ds.l 1

	ds.w 1
	ds.l 1
	ds.w 1
	ds.w 1

	xdef _dirlock
_dirlock: ds.w 1

	ds.w 1
	ds.w 1

	xdef _ldivr
_ldivr: ds.l 1

	xdef _ofdbuf
_ofdbuf: ds.b 56*100

	ds.b $18

	xdef _oscnt
_oscnt: ds.w 1

	ds.b $5a

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

	xdef _bakbuf
_bakbuf: ds.l 3

    ds.b $48
    
	xdef _secbuf
_secbuf: ds.b 4*512

	xdef _osuser
_osuser: ds.b 1

	xdef _dirrec
_dirrec: ds.l 1

    ds.b $20
    
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

    ds.b $10
    
    xdef _uptime
_uptime: ds.l 1

    ds.w 1
    
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


*
* These 2 functions are referenced by GEMDOS,
* but are defined elsewhere
*
	xdef _zeromem
_zeromem equ $00e01680
