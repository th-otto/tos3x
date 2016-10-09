*
* common variables
*
* This file is only used when linking a single module,
* to get the same addresses as when linking the whole ROM
*

#include "../common/config.h"
#include "../bdos/bdosdefs.h"
#include "../bios/biosdefs.h"
#include "../aes/aesdefs.h"
#include "../vdi/vdidefs.h"
#include "../vdi/lineaequ.inc"

	bss

*	ds.b $800       ; the system variables go here

	ds.b $40 ; seems to be unused

	xdef gem_env
gem_env: ds.b 12

* save area for trap13/14.
    xdef save_beg
save_beg: ds.b   238
    xdef _trap_save_area
_trap_save_area: ds.b 0         * End of Save storage

	xdef oshdr
oshdr: ds.b 6+64

	xdef cookies
cookies: ds.l NCOOKIES*2

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
idebuf: ds.b SECTOR_SIZE

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
kb_altnum: ds.w 1
	
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
dsb: ds.b 8*NUMFLOPPIES
	
	xdef fd_rtryseek
fd_rtryseek: ds.w 1
	xdef fd_frb
fd_frb: ds.l 1
	xdef fd_copyp
fd_copyp: ds.l 1
	xdef fd_cscnt
fd_cscnt: ds.w 1

	xdef memtnext
memtnext: ds.l 1
	xdef memtaddr
memtaddr: ds.l 1
	xdef memtbot
memtbot: ds.l 1
	xdef memtend
memtend: ds.l 1
	xdef memttop
memttop: ds.l 1
	xdef memtzone
memtzone: ds.l 1
	xdef memtsize
memtsize: ds.l 1
	
	xdef memterr
memterr: ds.w 1

* disk_buffer plus pad for equated arrays
* BEWARE: portions also used by VDI
	xdef _dskbuf
_dskbuf: ds.b 7730

	xdef _width
_width: equ _dskbuf+44
	xdef _height
_height: equ _dskbuf+46

	xdef _vdivars
_vdivars: ds.b 910 ; space for negative linea vars
    xdef lineavars
lineavars:
#if TOSVERSION >= 0x400
	ds.b 4018
#else
	ds.b 2310
#endif
	xdef _reqCol
_reqCol equ lineavars+(_REQ_COL)
	xdef _reqXCol
_reqXCol equ lineavars+(_REQ_X_COL)

	ds.w 1 ; unused

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

	xdef _rseed
_rseed: ds.l 1

	xdef _prtbdev
_prtbdev: ds.b 1
	even

	xdef _prtboch
_prtboch: ds.b 1
	even

    xdef _msec
_msec: ds.w 1

	xdef _prtbbval
_prtbbval: ds.w 1

	ds.b $400
    
    xdef _supstk
_supstk: ds.w SUPSIZ
    xdef _supstkend
_supstkend: ds.b 0

	xdef _prtbflg
_prtbflg: ds.b 1
	even
	
	xdef _prtbdmask
_prtbdmask: ds.b 1
	even
	
	xdef _time
_time: ds.w 1

	xdef _prtboval
_prtboval: ds.w 4

	xdef _root
_root: ds.l MAXQUICK

	ds.b 60

	xdef _glbkbchar
_glbkbchar: ds.l 3*KBBUFSZ

	xdef _ofdlist
_ofdlist: ds.l 1

	xdef _fd_mediach
_fd_mediach: ds.l 1

	xdef _glbcolumn
_glbcolumn: ds.w 3

	xdef _prtbwidth
_prtbwidth: ds.w 1
	
	xdef _drivechange
_drivechange: ds.b NUMFLOPPIES

	xdef _rrecno
_rrecno: ds.w 1

	xdef _prtbbytes
_prtbbytes: ds.w 1

	xdef _prtbilow
_prtbilow: ds.b 1
	even
	xdef _prtbihigh
_prtbihigh: ds.b 1
	even

	xdef _rwrtflg
_rwrtflg: ds.w 1
	xdef _curflop
_curflop: ds.w 1

	xdef _prtbwords
_prtbwords: ds.w 1

	xdef _prtbstrt
_prtbstrt: ds.l 1

	xdef _prtbp2
_prtbp2: ds.l 1

	xdef _prtbchunk
_prtbchunk: ds.w 1

	xdef _virt_work
_virt_work: ds.b 436

	xdef _prtbp1
_prtbp1: ds.l 1

	xdef _rdm
_rdm: ds.l 1

	xdef _prtbgval
_prtbgval: ds.w 1

	xdef _prtbend
_prtbend: ds.l 1

	xdef _prtbpmsk
_prtbpmsk: ds.w 1

	xdef _prtbidx
_prtbidx: ds.w 1

	xdef _dirlock
_dirlock: ds.w 1

	xdef _prtbinvert
_prtbinvert: ds.w 1

	xdef _prtbink
_prtbink: ds.w 1

	xdef _ldivr
_ldivr: ds.l 1

	xdef _ofdbuf
_ofdbuf: ds.b 70*80

	xdef _bpbsums
_bpbsums: ds.w BPBSECT*NUMFLOPPIES

	xdef _oscnt
_oscnt: ds.w 1

	xdef _ram8x8
_ram8x8: ds.b 90

	xdef _rwerr
_rwerr: ds.l 1

	xdef _prtbrval
_prtbrval: ds.w 1

	xdef _pmd
_pmd: ds.l 3

	xdef _sft
_sft: ds.b 10*(OPNFILES-6)

	xdef _ram16x32
_ram16x32: ds.b 90
	
	xdef _run
_run: ds.l 1

	xdef _ram8x16
_ram8x16: ds.b 90
    
    xdef _pbpar
_pbpar: ds.b 30

	xdef _fill
_fill: ds.b 3
	even

	xdef _bakbuf
_bakbuf: ds.l 3

	xdef _blkdev
_blkdev: ds.b 36*NUMFLOPPIES
    
	xdef _secbuf
_secbuf: ds.b 4*SECTOR_SIZE

	xdef _osuser
_osuser: ds.b 1

	xdef _dirrec
_dirrec: ds.l 1

	xdef _prtbval
_prtbval: ds.w 16
    
	xdef _dirtbl
_dirtbl: ds.l NCURDIR

	xdef _prtbplanes
_prtbplanes: ds.w 1

	xdef _prtboffset
_prtboffset: ds.w 1
    
	xdef _errbuf
_errbuf: ds.l 3

	xdef _prtbtcol
_prtbtcol: ds.w 1

	xdef _prtbacol
_prtbacol: ds.b 1
	even

	xdef _prtbmaxval
_prtbmaxval: ds.w 16

	xdef _prtbeps
_prtbeps: ds.b 1
	even
	
	xdef _prtbobuf
_prtbobuf: ds.b 8

	xdef _fd_err
_fd_err: ds.w NUMFLOPPIES
    
	xdef _diruse
_diruse: ds.b NCURDIR

	xdef _drvtbl
_drvtbl: ds.l 16

	xdef _drvsel
_drvsel: ds.w 1

	xdef _prtbheight
_prtbheight: ds.w 1
    
    xdef _prtbmono
_prtbmono: ds.b 1
	even
	
	xdef _prtbmval
_prtbmval: ds.w 4

    xdef _prtbmed
_prtbmed: ds.b 1
	even
	
	xdef _prtbempty
_prtbempty: ds.b 1
	even
	
    xdef _uptime
_uptime: ds.l 1

	xdef _prtblines
_prtblines: ds.w 1
    
	xdef _errdrv
_errdrv: ds.w 1

	xdef _prtbodd
_prtbodd: ds.b 1
	even

	xdef _prtbdots
_prtbdots: ds.w 1

	xdef _prtbomask
_prtbomask ds.w 1
	
	xdef _prtbbits
_prtbbits: ds.w 1
    
	xdef _prtbprez
_prtbprez: ds.b 1
	even

	xdef _prtbminval
_prtbminval: ds.w 16
    
	xdef _ospd
_ospd: ds.b 256

    xdef _buptr
_buptr: ds.l 3

	xdef _prtblow
_prtblow: ds.b 1
	even
	
	xdef _prtbamon
_prtbamon: ds.b 1
	even
    
	xdef _date
_date: ds.w 1

	xdef _bcbx
_bcbx: ds.b 4*20

	xdef _prtbbol
_prtbbol: ds.b 1
	even

    xdef _beptr
_beptr: ds.l 3

* This label is referenced by the OSHEADER
	xdef _endvdibss
_endvdibss: ds.b 0

	ds.b $63E6

* This label is referenced by the GEM_MUPB structure
	xdef _endgembss
_endgembss: ds.b 0

	xdef __ebss
__ebss: ds.b 0
