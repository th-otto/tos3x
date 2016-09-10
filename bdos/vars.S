OPNFILES equ 81
NCURDIR equ 40
    
	bss

    ds.b $4322
    
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

	ds.b $1c66

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

    ds.b $8f6
    
	xdef _dirtbl
_dirtbl: ds.l NCURDIR

    ds.b 4
    
	xdef _errbuf
_errbuf: ds.l 3

    ds.b $32
    
	xdef _diruse
_diruse: ds.b NCURDIR

    ds.b $40
    
	xdef _drvsel
_drvsel: ds.w 1

    ds.b $16
    
	xdef _errdrv
_errdrv: ds.w 1

    ds.b $2a
    
	xdef _ospd
_ospd: ds.b 256

    ds.b $10
    
	xdef _date
_date: ds.w 1
