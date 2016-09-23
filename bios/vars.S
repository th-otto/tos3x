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
MAXQUICK equ 5
    
	bss

*	ds.b $800       ; the BIOS vars go here

* save area for trap13/14.
    xdef save_beg
save_beg:  ds.w   157
    xdef _trap_save_area
_trap_save_area: ds.b 0         * End of Save storage

	xdef oshdr
oshdr: ds.b 6+64

	xdef cookies
cookies: ds.l 16*2

	ds.b $12

	xdef _has_dmasound
_has_dmasound: ds.b 1
	xdef _has_microwire
_has_microwire: ds.b 1
	xdef __sbss
__sbss: ds.b 0

    xdef privinst
privinst: ds.l 2

	ds.b $58

	xdef _pbdef
_pbdef: ds.b 30

	ds.b $438

	xdef midiiorec
midiiorec: ds.b 14
	xdef midibuf
midibuf: ds.b 256

	xdef ikbdiorec
ikbdiorec: ds.b 14
	xdef ikbdbuf
ikbdbuf: ds.b 128

	xdef _kbdvecs
_kbdvecs: ds.b 42

	ds.b $2b

	xdef _shifty
_shifty: ds.b 1

	ds.b $24

	xdef _tim_c_sieve
_tim_c_sieve: ds.w 1

	ds.b $0a

	xdef iclk_ready
iclk_ready: ds.w 1

	xdef bcmap_root
bcmap_root: ds.b 16

	ds.b $75c

	xdef _dskbuf
_dskbuf: ds.b 0

	ds.b $2AC8

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
    xdef _supstkend
_supstkend:

	ds.b 1
	even
	
	ds.b 1
	even
	
	xdef _time
_time: ds.w 1

	ds.b 8

	xdef _root
_root: ds.l MAXQUICK

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

	xdef _endvdibss
_endvdibss: ds.b 0

    ds.b $63E6

	xdef __ebss
	xdef _endgembss
__ebss: ds.b 0
_endgembss: ds.b 0

