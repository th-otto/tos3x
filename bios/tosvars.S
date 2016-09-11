*
* tosvars.S - TOS System variables
*
* Copyright (c) 2005-2013 by the EmuTOS development team.
*
* This file is distributed under the GPL, version 2 or at your
* option any later version.  See doc/license.txt for details.
*
*

        xdef  _proc_lives
        xdef  _proc_dregs
        xdef  _proc_aregs
        xdef  _proc_enum
        xdef  _proc_usp
        xdef  _proc_stk

        xdef  _etv_timer
        xdef  _etv_critic
        xdef  _etv_term
        xdef  _etv_xtra
        xdef  _memvalid
        xdef  memctrl
        xdef  resvalid
        xdef  resvector
        xdef  _phystop
        xdef  _membot
        xdef  _memtop
        xdef  _memval2
        xdef  memval2
        xdef  _flock
        xdef  _seekrate
        xdef  _timer_ms
        xdef  _fverify
        xdef  _bootdev
        xdef  _palmode
        xdef  _defshiftmod
        xdef  _sshiftmod
        xdef  _v_bas_ad
        xdef  _vblsem
        xdef  _nvbls
        xdef  _vblqueue
        xdef  _colorptr
        xdef  _screenpt
        xdef  _vbclock
        xdef  _frclock
        xdef  _hdv_init
        xdef  _swv_vec
        xdef  _hdv_bpb
        xdef  _hdv_rw
        xdef  _hdv_boot
        xdef  _hdv_mediach
        xdef  _cmdload
        xdef  _conterm
        xdef  _themd
        xdef  ____md
        xdef  _savptr
        xdef  _nflops
        xdef  con_state
        xdef  _save_row
        xdef  sav_context
        xdef  _bufl
        xdef  _hz_200
        xdef  the_env
        xdef  _drvbits
        xdef  _dskbufp
        xdef  _autopath
        xdef  _vbl_list
        xdef  _dumpflg
        xdef  _sysbase
        xdef  _shell_p
        xdef  _end_os
        xdef  _exec_os
        xdef  _scr_dmp
        xdef  _prt_stat
        xdef  _prt_vec
        xdef  _aux_stat
        xdef  _aux_vec
        xdef  _pun_ptr
        xdef  _memval3
        xdef  memval3
        xdef  _bconstat_vec
        xdef  _bconin_vec
        xdef  _bcostat_vec
        xdef  _bconout_vec
        xdef  _longframe
        xdef  _p_cookies
        xdef  _ramtop
        xdef  _ramvalid
        xdef  _bell_hook
        xdef  _kcl_hook

	    xdef _shifty
	    
* ===========================================================================
* ==== BSS segment ==========================================================
* ===========================================================================


* ==== Beginning of RAM (used by OS) ========================================
        bss
* org 0         * start of RAM
   
* ==== Start of Exception related variables =================================
        ds.b    $380
_proc_lives:    ds.l   1       * validates system crash page, if 0x12345678
_proc_dregs:    ds.l   8       * saved registers d0-d7
_proc_aregs:    ds.l   8       * saved registers a0-a7
_proc_enum:     ds.l   1       * vector number of crash exception
_proc_usp:      ds.l   1       * saved user stackpointer
_proc_stk:      ds.w   16      * 16 words from exception stack
        ds.b 20
        
* ==== Start of System variables ============================================
_etv_timer:     ds.l   1       * GEM event timer vector
_etv_critic:    ds.l   1       * GEM critical error handler
_etv_term:      ds.l   1       * GEM program termination vector
_etv_xtra:      ds.l   5       * GEM additional vectors (unused)
_memvalid:      ds.l   1       * memory conf valid, if 0x752019f3
memctrl:        ds.b   1       * copy of contents of 0xffff8001
                ds.b 1
resvalid:       ds.l   1       * validates resvector, if 0x31415926
resvector:      ds.l   1       * reset vector
_phystop:       ds.l   1       * physical top of RAM
_membot:        ds.l   1       * start of TPA (user memory)
_memtop:        ds.l   1       * end of TPA (user memory)
_memval2:
memval2:        ds.l   1       * validates memcntrl/memconf, if 0x237698aa
_flock:         ds.w   1       * if != 0, VBL floppy routine is disabled
_seekrate:      ds.w   1       * floppy seek rate
_timer_ms:      ds.w   1       * time between timer calls in ms
_fverify:       ds.w   1       * if != 0, verify floppy writes
_bootdev:       ds.w   1       * default boot drive
_palmode:       ds.w   1       * 0 = NTSC, else PAL
_defshiftmod:   ds.w   1       * default video resolution
_sshiftmod:     ds.b   1       * copy of contents of 0xffff8260
                ds.b 1
_v_bas_ad:      ds.l   1       * screen base address
_vblsem:        ds.w   1       * if > 0, the VBL routine is executed
_nvbls:         ds.w   1       * number of VBL routines
_vblqueue:      ds.l   1       * pointer to list of VBL routines
_colorptr:      ds.l   1       * pointer to color palette to be loaded
_screenpt:      ds.l   1       * pointer to video RAM for next VBL
_vbclock:       ds.l   1       * number of VBL routines executed
_frclock:       ds.l   1       * number of VBL routines processed
_hdv_init:      ds.l   1       * vector for hard disk initialization
_swv_vec:       ds.l   1       * vector for resolution change
_hdv_bpb:       ds.l   1       * vector for getbpb for harddisk
_hdv_rw:        ds.l   1       * vector for read/write for harddisk
_hdv_boot:      ds.l   1       * vector for hard disk boot
_hdv_mediach:   ds.l   1       * vector for hard disk media change
_cmdload:       ds.w   1       * if not 0, load command.prg after boot
_conterm:       ds.b   1       * attribute vector for console output
                ds.b   1
trp14ret:       ds.l   1       * return address for trap #14 (unused)
criticret:      ds.l   1       * return address for critical error hndlr
_themd:         ds.l   4       * first memory descriptor block
____md:         ds.l   1       * space for additional memory descriptor
_savptr:        ds.l   1       * pointer to BIOS save register block
_nflops:        ds.w   1       * number of connected floppy drives
con_state:      ds.l   1       * vector for screen output
_save_row:      ds.w   1       * temporary storage for cursor line pos.
sav_context:    ds.l   1       * ptr to save area for exception processing
_bufl:          ds.l   2       * pointers to buffer ctrl blocks for GEMDOS
_hz_200:        ds.l   1       * counter for 200 Hz system clock
the_env:        ds.l   1       * pointer to default environment string
_drvbits:       ds.l   1       * bit allocation for physical drives
_dskbufp:       ds.l   1       * pointer to disk buffer
_autopath:      ds.l   1       * pointer to auto-execute path
_vbl_list:      ds.l   8       * pointers to 8 VBl routines
_dumpflg:       ds.w   1       * flag for screen dump (unsused)
_prtabt:        ds.w   1       * printer abort flag
_sysbase:       ds.l   1       * pointer to start of OS
_shell_p:       ds.l   1       * pointer to shell
_end_os:        ds.l   1       * pointer to end of OS
_exec_os:       ds.l   1       * pointer to entry point of OS

_scr_dmp:       ds.l   1       * pointer to screen dump routine
_prt_stat:      ds.l   1       * pointer to prv_lsto
_prt_vec:       ds.l   1       * pointer to prv_lst
_aux_stat:      ds.l   1       * pointer to prv_auxo
_aux_vec:       ds.l   1       * pointer to prv_aux
_pun_ptr:       ds.l   1       * if AHDI, pointer to pun_info
_memval3:
memval3:        ds.l   1       * memory conf valid if = $5555aaaa
_bconstat_vec:  ds.l   8       * 8 pointers to input-status routines
_bconin_vec:    ds.l   8       * 8 pointers to input routines
_bcostat_vec:   ds.l   8       * 8 pointers to output-status routines
_bconout_vec:   ds.l   8       * 8 pointers to output routines
_longframe:     ds.w   1       * if not 0, then not 68000 - use long stack frames
_p_cookies:     ds.l   1       * pointer to cookie jar
_ramtop:        ds.l   1       * pointer to end of FastRam
_ramvalid:      ds.l   1       * validates ramtop if $1357BD13
_bell_hook:     ds.l   1       * pointer to routine for system bell
_kcl_hook:      ds.l   1       * pointer to routine for system keyclick

*        org 0x5b4

        ds.b $24c
        
*        org 0x800


* _shifty:        ds.w   1       * reflects the status up/down of mode keys

* ===========================================================================
* ==== End ==================================================================
* ===========================================================================

