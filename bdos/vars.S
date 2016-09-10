OPNFILES equ 81
NCURDIR equ 40
    
	bss

    ds.b $575c
    
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

    ds.b $58
    
	xdef _errdrv
_errdrv: ds.w 1

    ds.b $2a
    
	xdef _ospd
_ospd: ds.b 256

    ds.b $10
    
	xdef _date
_date: ds.w 1
