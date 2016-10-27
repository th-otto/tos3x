/*
 * common variables
 *
 * This file is only used when linking a single module,
 * to get the same addresses as when linking the whole ROM
 */

#include "../common/config.h"
#include "../bdos/bdosdefs.h"
#include "../bios/biosdefs.h"
#include "../aes/aesdefs.h"
#include "../vdi/vdidefs.h"
#include "../vdi/lineaequ.inc"
#include "../desk/deskdefs.h"

#define V(name,t,size) \
	xdef name \
name: ds.t size
#define S(t,size) ds.t size

	bss

	ds.b $800       /* the system variables go here */

	ds.b $40 /* seems to be unused */

#if STBOOK
	ds.b 164 /* seems to be unused */
#endif

	V(gem_env,b,12)

/* save area for trap13/14. */
	V(save_beg,b,238)
	V(_trap_save_area,b,0)         /* End of Save storage */

	V(oshdr,b,6+64)

	V(cookies,l,NCOOKIES*2)

	V(_bootpref,w,1)

#if (TOSVERSION >= 0x300)
	V(_altmd,l,4)
#endif

	V(_has_dmasound,b,1)
	V(STEFlag,b,1)

	V(__sbss,b,0)	/* start of the BSS area that is zeroed by BIOS */

	V(privinst,l,2)

	V(savepc,l,1)
	V(execpath,l,1)
	V(execname,l,1)
	V(execdta,b,44)
	V(execfname,b,32)

	V(_pbdef,b,30)

	V(lastacstm,l,1)

	V(scsicmd,b,10)

	V(dmarwflg,b,2)

	V(idebuf,b,SECTOR_SIZE)

	ds.l 1 /* unused */

#if MSTECLK
	V(rtcbufa,b,13)
	V(rtcbufb,b,13)
	V(waketimeAlarm,l,1)
#endif

	V(rs232ibuf,b,SERIAL_BUFSIZE)
	V(rs232obuf,b,SERIAL_BUFSIZE)
	V(rs232iorec,b,2*14+8)

	V(ikbdiorec,b,14)
	V(ikbdbuf,b,SERIAL_BUFSIZE)

	V(midiiorec,b,14)
	V(midibuf,b,128)

	V(_kbdvecs,b,42)

	V(kb_altnum,w,1)
	
	V(kbdbuf,b,7)
	V(mousebuf,b,5)
	V(relmbuf,b,3)
	V(clockbuf,b,6)
	V(joybuf,b,3)
    even

	V(iclkrtime,l,1)

	V(iclkwtime,l,1)
	
	V(iclkbuf,b,6)      /* buffer for writing time packet to IKBD */

	V(buttonbuf,b,3)
	
	V(_shifty,b,1)

	V(_curkeytbl,l,3)

	V(initmbuf,b,17)

	V(kb_key_last,b,1)
	V(kb_ticks,b,1)
	V(kb_delay,b,1)

	V(kb_initial,b,1)
	V(kb_repeat,b,1)

	V(lst_timeout,l,1)

	V(_tim_c_sieve,w,1)

	V(_sndtable,l,1)

	V(_snddelay,b,1)

	V(_sndtmp,b,1)

	V(_prtconfig,w,1)

	V(iclk_ready,w,1)

	V(bcmap_root,b,16)

	V(bcmap_devs,l,4*6) /* should be 3 for 2.06 but isnt */

#if TTMFP_SUPPORT
	V(mdm3ibuf,b,SERIAL_BUFSIZE)
	V(mdm3obuf,b,SERIAL_BUFSIZE)
	V(mdm3iorec,b,2*14+8)
#endif

	V(mdm4ibuf,b,SERIAL_BUFSIZE)
	V(mdm4obuf,b,SERIAL_BUFSIZE)
	V(mdm4iorec,b,2*14+8)

	V(mdm2ibuf,b,SERIAL_BUFSIZE)
	V(mdm2obuf,b,SERIAL_BUFSIZE)
	V(mdm2iorec,b,2*14+8)

	V(fd_retry,w,1)
	V(_fd_wp,b,NUMFLOPPIES)
	V(_fd_latch,b,NUMFLOPPIES)
	V(_fd_lastacc,l,NUMFLOPPIES)
	V(fd_timeout,l,1)
	V(fd_motoron,w,1)
	V(fd_desel,w,1)
	V(fd_dev,w,1)
	V(fd_track,w,1)
	V(fd_sect,w,1)
	V(fd_side,w,1)
	V(fd_scount,w,1)
	V(fd_buffer,l,1)
	V(fd_spt,w,1)
	V(fd_interlv,w,1)
	V(fd_virgin,w,1)
	V(fd_secmap,l,1)
	V(fd_deferr,w,1)
	V(fd_curerr,w,1)

	V(flopsregs,l,9)

	V(dsb,b,8*NUMFLOPPIES)
	
	V(fd_rtryseek,w,1)
	V(fd_frb,l,1)
	V(fd_copyp,l,1)
	V(fd_cscnt,w,1)

	V(memtnext,l,1)
	V(memtaddr,l,1)
	V(memtbot,l,1)
	V(memtend,l,1)
	V(memttop,l,1)
	V(memtzone,l,1)
	V(memtsize,l,1)
	
	V(memterr,w,1)

/*
 * disk_buffer plus pad for equated arrays
 * BEWARE: portions also used by VDI (the "overlay area")
 */
#include "../vdi/vdivar.S"

	ds.w 1 /* unused */

	V(biosav,l,1)

	ds.l 1 /* unused */

	V(savesr,w,1)

	V(fstack,w,1034)
	V(fstrt,l,1)
	V(old_tik,l,1)
	V(old_2,l,1)

	V(_rseed,l,1)

	V(_prtbdev,b,1)
	even

	V(_prtboch,b,1)
	even

	V(_msec,w,1)

	V(_prtbbval,w,1)

	ds.b $400
    
	V(_supstk,w,SUPSIZ)
	V(_supstkend,b,0)

	V(_prtbflg,b,1)
	even
	
	V(_prtbdmask,b,1)
	even
	
	V(_time,w,1)

	V(_prtboval,w,4)

	V(_root,l,MAXQUICK)

	ds.b 60

	V(_glbkbchar,l,3*KBBUFSZ)

	V(_ofdlist,l,1)

	V(_fd_mediach,l,1)
	V(_glbcolumn,w,3)
	V(_prtbwidth,w,1)
	
	V(_drivechange,b,NUMFLOPPIES)
	V(_rrecno,w,1)
	V(_prtbbytes,w,1)

	V(_prtbilow,b,1)
	even
	V(_prtbihigh,b,1)
	even

	V(_rwrtflg,w,1)
	V(_curflop,w,1)

	V(_prtbwords,w,1)
	V(_prtbstrt,l,1)
	V(_prtbp2,l,1)
	V(_prtbchunk,w,1)

	V(_virt_work,b,180+MU_PLANES*16*2)

	V(_prtbp1,l,1)
	V(_rdm,l,1)
	V(_prtbgval,w,1)
	V(_prtbend,l,1)
	V(_prtbpmsk,w,1)
	V(_prtbidx,w,1)
	V(_dirlock,w,1)
	V(_prtbinvert,w,1)
	V(_prtbink,w,1)

#if STBOOK
	V(_ofdbuf,b,70*80)
	V(_ldivr,l,1)
#else
	V(_ldivr,l,1)
	V(_ofdbuf,b,70*80)
#endif

	V(_bpbsums,w,BPBSECT*NUMFLOPPIES)
	V(_oscnt,w,1)

	V(_ram8x8,b,90)

	V(_rwerr,l,1)
	V(_prtbrval,w,1)
	V(_pmd,l,3)
	V(_sft,b,OPNFILES*10-6*10)

#if TOSVERSION >= 0x300
	V(_ram16x32,b,90)
#endif
	
	V(_run,l,1)

	V(_ram8x16,b,90)
    
	V(_pbpar,b,30)

#if !STBOOK
	V(_fill,b,3)
	even
#endif

	V(_bakbuf,l,3)

#if STBOOK
	V(_fill,b,3)
	even
#endif

	V(_blkdev,b,36*NUMFLOPPIES)
	V(_secbuf,b,4*SECTOR_SIZE)
	V(_osuser,b,1)
	V(_dirrec,l,1)
	V(_prtbval,w,16)
	V(_dirtbl,l,NCURDIR)
	V(_prtbplanes,w,1)
	V(_prtboffset,w,1)
	V(_errbuf,l,3)
	V(_prtbtcol,w,1)

	V(_prtbacol,b,1)
	even
	V(_prtbmaxval,w,16)
	V(_prtbeps,b,1)
	even
	V(_prtbobuf,b,8)
	V(_fd_err,w,NUMFLOPPIES)
	V(_diruse,b,NCURDIR)
	V(_drvtbl,l,BLKDEVNUM)
	V(_drvsel,w,1)
	V(_prtbheight,w,1)
	V(_prtbmono,b,1)
	even
	V(_prtbmval,w,4)
	V(_prtbmed,b,1)
	even
	V(_prtbempty,b,1)
	even
	V(_uptime,l,1)
	V(_prtblines,w,1)
	V(_errdrv,w,1)

	V(_prtbodd,b,1)
	even

	V(_prtbdots,w,1)
	V(_prtbomask,w,1)
	V(_prtbbits,w,1)
	V(_prtbprez,b,1)
	even
	V(_prtbminval,w,16)
	V(_ospd,b,256)
	V(_buptr,l,3)

	V(_prtblow,b,1)
	even
	V(_prtbamon,b,1)
	even
    
	V(_date,w,1)
	V(_bcbx,b,4*20)
	V(_prtbbol,b,1)
	even
	V(_beptr,l,3)

/* This label is referenced by the OSHEADER */
	V(_endvdibss,b,0)

/* start AES */

	V(retsava,l,1)
	V(_gl_restype,w,1)
	V(_gl_rschange,w,1)
	V(ustak,l,33)
	V(_diskin,w,1)
	V(dummycmd,w,1)
	V(execmode,w,1)
	V(execflags,l,1)
	
	ds.b 2
	
	V(_ad_rso,l,1)
	
	ds.b $136

	V(pblock,l,5)
	V(_crit_err,l,1)
	
	ds.b $420

	V(savea0,l,1)
	V(srsave,w,1)
	V(savsr,w,1)
	V(psetsr,w,1)

	ds.b $28C

	V(_drwaddr,l,1)
	V(tikaddr,l,1)
	ds.l 1
	V(save_sp,l,1)
	V(tiksav,l,1)
	V(_NUM_TICK,l,1)
	V(_CMP_TICK,l,1)

	ds.b $C4
	
	V(fspec,b,8)
	V(mydev,w,1)
	V(oldgetbpb,l,1)
	V(oldmediach,l,1)
	V(oldrwabs,l,1)
	ds.l 1 /* unused */
	V(_maddr,l,1)
	V(mform,w,37)
	V(trp13aret,l,1)
	V(trp14aret,l,1)
	
	ds.b $10
	
	V(_intout,w,10)
	
	ds.w 1
	
	V(_ptsout,w,10)
	
	ds.b $46

	V(_gl_adacc,l,MAX_ACCS)

	ds.b $12
	
	V(_xrat,w,1)
	V(_yrat,w,1)
	
	ds.b 2
	
	V(_gl_hchar,w,1)
	V(_gl_naccs,w,1)
	
	ds.b 4
	
	V(_gl_bdelay,w,1)
	
	ds.b 4
	
	V(_gl_pacc,l,MAX_ACCS)
	
	ds.b $48
	
	V(_gl_bclick,w,1)
	
	ds.b 4
	
	V(_gl_handle,w,1)
	
	ds.b $AE
	
	V(_gl_dclick,w,1)
	
	ds.b $4A
	
	V(_gl_bdesired,w,1)
	V(_gl_dcindex,w,1)
	V(_gl_recd,w,1)
	
	ds.b $16
	
	V(old_bcode,l,1)
	
	ds.b 6
	
	V(_gl_bvhard,w,1)
	
	ds.b 10
	
	V(_gl_wchar,w,1)
	
	ds.b $28
	
	V(_gl_bpend,w,1)
	
	ds.b $4c
	
	V(_gl_bvdisk,w,1)
	
	ds.b $10
	
	V(_gl_moff,w,1)
	
	ds.b 8
	
	V(_sh_iscart,w,1)
	
	ds.b $30
	
	V(old_mcode,l,1)
	V(_gl_nplanes,w,1)
	V(_gl_ticktime,w,1)
	
	ds.b $88
	
	V(_DOS_AX,w,1)
	
	ds.b $8C
	
	V(_ad_envrn,l,1)
	
	ds.b $166
	
	V(_gl_rbuf,l,1)
	
	ds.l 1
	
	V(_sh_isgem,w,1)
	
	ds.b $24
	
	V(_gl_hbox,w,1)
	
	ds.b $4e
	
	V(_gl_rlen,w,1)
	
	ds.b 10
	
	V(_sys_adacc,l,1)
	
	ds.b $C0
	
	V(_gl_mnpid,l,1) /* BUG: should be int */
	V(_pr_mclick,l,1)
	
	ds.b 12
	
	V(_elinkoff,l,1)
	V(_gl_play,w,1)
	
	ds.b $82
	
	V(_gl_cowner,l,1)
	
	ds.b 4
	
	V(_sh_doexec,w,1)
	
	ds.b 8
	
	V(_gl_rfull,w,4)
	
	ds.b $38
	
	V(_gl_btrue,w,1)
	
	ds.b $1A
	
	V(_gl_kowner,l,1)
	V(_crt_error,w,1)
	V(_gl_wbox,w,1)
	
	ds.b 4
	
	V(_gl_bowner,l,1)
	
	ds.b $86
	
	V(_wind_spb,b,10)    /* sizeof(SPB) */
	
	ds.b $E
	
	V(_gl_mvisible,w,1)
	
	ds.b $22
	
	V(_autoexec,w,1)
	
	ds.b $18
	
	V(_pr_xrat,w,1)
	V(_pr_button,w,1)
	
	ds.b $1064
	
	V(_pr_yrat,w,1)
	
	ds.b $44
	
	V(_tbutton,w,1)
	
	ds.b 14
	
	V(_fpcnt,w,1)
	
	ds.b $4EE
	
	V(_intin,w,128)
	
	ds.b 4
	
	V(_cda,l,1)
	V(_wwait,w,1)
	
	ds.b $1C8
	
	V(_DOS_ERR,w,1)
	V(_ptsin,w,20)
	V(_dpd,l,1)
	V(_fph,w,1)
	V(_dlr,l,1)
	V(_drl,l,1)
	
	V(_D,b,$369E) /* sizeof(THEGLO) _D */
	
	V(_eul,l,1)
	V(_fpt,w,1)
	V(_nrl,l,1)
	V(_rlr,l,1)
	
	ds.b 8
	
	V(_evx,b,28) /* sizeof(EVB) */
	V(_zlr,l,1)
	
	ds.b $12
	
	V(_sh_gem,w,1)
	V(_mclick,w,1)
	
	ds.b $BE
	
	V(_indisp,b,1)
	even
	
	ds.b 2
	V(_infork,b,1)
	even
	ds.b 2
	ds.b 2
	
	V(_kstate,w,1)
	
	ds.b 16
	
	V(_contrl,w,12)
	
	ds.b $4C
	
	V(_mtrans,w,1)
	
	ds.b $2E
	
	V(_button,w,1)
	
/* end AES */

/* This label is referenced by the GEM_MUPB structure */
	V(_endgembss,b,0)

/* This label is referenced by the BIOS initialization */
	V(__ebss,b,0)
