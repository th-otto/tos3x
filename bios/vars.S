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
SERIAL_BUFSIZE equ 256
NUMFLOPPPIES equ 2
    
	bss

*	ds.b $800       ; the system variables go here

	ds.b $40

	xdef gem_env
gem_env: ds.b 12

* save area for trap13/14.
    xdef save_beg
save_beg:  ds.b   238
    xdef _trap_save_area
_trap_save_area: ds.b 0         * End of Save storage

	xdef oshdr
oshdr: ds.b 6+64

	xdef cookies
cookies: ds.l 16*2

	xdef _bootpref
_bootpref: ds.w 1

	xdef _altmd
_altmd: ds.l 4

	xdef _has_dmasound
_has_dmasound: ds.b 1
	xdef _has_microwire
_has_microwire: ds.b 1
	xdef __sbss
__sbss: ds.b 0

    xdef privinst
privinst: ds.l 2

    xdef savepc
savepc: ds.l 1
    xdef execpath
execpath: ds.l 1
    xdef execname
execname: ds.l 1
    xdef execdta
execdta: ds.b 44
    xdef execfname
execfname: ds.b 32

	xdef _pbdef
_pbdef: ds.b 30

	xdef nextacsi
nextacsi: ds.l 1

	xdef scsicmd
scsicmd: ds.b 10

	xdef dmarwflg
dmarwflg: ds.b 2

	xdef idebuf
idebuf: ds.b 512

	ds.l 1 ; unused

	xdef rs232ibuf
rs232ibuf: ds.b SERIAL_BUFSIZE
    xdef rs232obuf
rs232obuf: ds.b SERIAL_BUFSIZE
	xdef rs232iorec
rs232iorec: ds.b 2*14+8

	xdef ikbdiorec
ikbdiorec: ds.b 14
	xdef ikbdbuf
ikbdbuf: ds.b SERIAL_BUFSIZE

	xdef midiiorec
midiiorec: ds.b 14
	xdef midibuf
midibuf: ds.b 128

	xdef _kbdvecs
_kbdvecs: ds.b 42

	xdef kb_altnum
kb_altnum:  ds.w 1
	
	xdef kbdbuf
kbdbuf: ds.b 7
	xdef mousebuf
mousebuf: ds.b 5
	xdef relmbuf
relmbuf: ds.b 3
	xdef clockbuf
clockbuf: ds.b 6
	xdef joybuf
joybuf: ds.b 3
    even

	xdef iclkrtime
iclkrtime: ds.l 1

	xdef iclkwtime
iclkwtime: ds.l 1
	
	xdef iclkbuf
iclkbuf: ds.b 6      ; buffer for writing time packet to IKBD

	xdef buttonbuf
buttonbuf: ds.b 3
	
	xdef _shifty
_shifty: ds.b 1

	xdef _curkeytbl
_curkeytbl: ds.l 3

	xdef initmbuf
initmbuf: ds.b 17

	xdef kb_key_last
kb_key_last: ds.b 1
	xdef kb_ticks
kb_ticks: ds.b 1
	xdef kb_delay
kb_delay: ds.b 1

    xdef kb_initial
kb_initial: ds.b 1
    xdef kb_repeat
kb_repeat: ds.b 1

	xdef lst_timeout
lst_timeout: ds.l 1

	xdef _tim_c_sieve
_tim_c_sieve: ds.w 1

	xdef _sndtable
_sndtable: ds.l 1

	xdef _snddelay
_snddelay: ds.b 1

	xdef _sndtmp
_sndtmp: ds.b 1

	xdef _prtconfig
_prtconfig: ds.w 1

	xdef iclk_ready
iclk_ready: ds.w 1

	xdef bcmap_root
bcmap_root: ds.b 16

	xdef bcmap_devs
bcmap_devs: ds.l 4*6

	xdef mdm3ibuf
mdm3ibuf: ds.b SERIAL_BUFSIZE
	xdef mdm3obuf
mdm3obuf: ds.b SERIAL_BUFSIZE
	xdef mdm3iorec
mdm3iorec: ds.b 2*14+8

	xdef mdm4ibuf
mdm4ibuf: ds.b SERIAL_BUFSIZE
	xdef mdm4obuf
mdm4obuf: ds.b SERIAL_BUFSIZE
	xdef mdm4iorec
mdm4iorec: ds.b 2*14+8

	xdef mdm2ibuf
mdm2ibuf: ds.b SERIAL_BUFSIZE
	xdef mdm2obuf
mdm2obuf: ds.b SERIAL_BUFSIZE
	xdef mdm2iorec
mdm2iorec: ds.b 2*14+8

	xdef fd_retry
fd_retry: ds.w 1
	xdef _fd_wp
_fd_wp: ds.b NUMFLOPPIES
	xdef _fd_latch
_fd_latch: ds.b NUMFLOPPIES
	xdef _fd_lastacc
_fd_lastacc: ds.l NUMFLOPPIES
	xdef fd_timeout
fd_timeout: ds.l 1
	xdef fd_motoron
fd_motoron: ds.w 1
	xdef fd_desel
fd_desel: ds.w 1
	xdef fd_dev
fd_dev: ds.w 1
	xdef fd_track
fd_track: ds.w 1
	xdef fd_sect
fd_sect: ds.w 1
	xdef fd_side
fd_side: ds.w 1
	xdef fd_scount
fd_scount: ds.w 1
	xdef fd_buffer
fd_buffer: ds.l 1
	xdef fd_spt
fd_spt: ds.w 1
	xdef fd_interlv
fd_interlv: ds.w 1
	xdef fd_virgin
fd_virgin: ds.w 1
	xdef fd_secmap
fd_secmap: ds.l 1
	xdef fd_deferr
fd_deferr: ds.w 1
	xdef fd_curerr
fd_curerr: ds.w 1

	xdef flopsregs
flopsregs: ds.l 9

	xdef dsb
dsb: ds.b 8*NUMFLOPPPIES
	
	xdef fd_rtryseek
fd_rtryseek: ds.w 1
	xdef fd_frb
fd_frb: ds.l 1
	xdef fd_copyp
fd_copyp: ds.l 1
	xdef fd_cscnt
fd_cscnt: ds.w 1

	ds.l 1
	ds.l 1
	ds.l 1
	ds.l 1
	ds.l 1
	ds.l 1
	ds.l 1
	ds.w 1

	xdef _dskbuf
_dskbuf: ds.b 0

	ds.b $1E32

#include "linea.inc"

	ds.b $85A

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
	
	xdef _drivechange
_drivechange: ds.b NUMFLOPPPIES

	xdef _rrecno
_rrecno: ds.l 1

	ds.b 2
	ds.b 2

	xdef _rwrtflg
_rwrtflg: ds.w 1
	xdef _curflop
_curflop: ds.w 1

	ds.b $1c4

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

	xdef _endgembss
_endgembss: ds.b 0
	xdef __ebss
__ebss: ds.b 0
