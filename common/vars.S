/*
 * common variables
 *
 * This file is only used when linking a single module,
 * to get the same addresses as when linking the whole ROM
 */

#include "config.h"
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

#define BYTE(name) V(name,b,1)
#define INT(name) V(name,w,1)
#define LONG(name) V(name,l,1)
#define PTR(name) V(name,l,1)

#define GRECT(name) V(name,w,4)
#define TEDINFO(name) V(name,b,28)
#define MFDB(name) V(name,b,20)


	bss

	ds.b 2048       /* the system variables go here */

	ds.b 64 /* seems to be unused */

#if STBOOK
	ds.b 164 /* seems to be unused */
#endif

	V(gem_env,b,12)

/* save area for trap13/14. */
	V(save_beg,b,238)
	V(_trap_save_area,b,0)         /* End of Save storage */

	V(oshdr,b,6+64)

	V(cookies,l,NCOOKIES*2)

	INT(_bootpref)

#if (TOSVERSION >= 0x300)
	V(_altmd,l,4)
#endif

	BYTE(_has_dmasound)
	BYTE(STEFlag)

	V(__sbss,b,0)	/* start of the BSS area that is zeroed by BIOS */

	V(privinst,l,2)

	PTR(savepc)
	PTR(execpath)
	PTR(execname)
	V(execdta,b,44)
	V(execfname,b,32)

	V(_pbdef,b,30)

	PTR(lastacstm)

	V(scsicmd,b,10)

	V(dmarwflg,b,2)

	V(idebuf,b,SECTOR_SIZE)

	ds.l 1 /* unused */

#if MSTECLK
	V(rtcbufa,b,13)
	V(rtcbufb,b,13)
	LONG(waketimeAlarm)
#endif

	V(rs232ibuf,b,SERIAL_BUFSIZE)
	V(rs232obuf,b,SERIAL_BUFSIZE)
	V(rs232iorec,b,2*14+8)

	V(ikbdiorec,b,14)
	V(ikbdbuf,b,SERIAL_BUFSIZE)

	V(midiiorec,b,14)
	V(midibuf,b,128)

	V(_kbdvecs,b,42)

	INT(kb_altnum)
	
	V(kbdbuf,b,7)
	V(mousebuf,b,5)
	V(relmbuf,b,3)
	V(clockbuf,b,6)
	V(joybuf,b,3)
    even

	LONG(iclkrtime)

	LONG(iclkwtime)
	
	V(iclkbuf,b,6)      /* buffer for writing time packet to IKBD */

	V(buttonbuf,b,3)
	
	BYTE(_shifty)

	V(_curkeytbl,l,3)

	V(initmbuf,b,17)

	BYTE(kb_key_last)
	BYTE(kb_ticks)
	BYTE(kb_delay)

	BYTE(kb_initial)
	BYTE(kb_repeat)

	LONG(lst_timeout)

	INT(_tim_c_sieve)

	PTR(_sndtable)

	BYTE(_snddelay)

	BYTE(_sndtmp)

	INT(_prtconfig)

	INT(iclk_ready)

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

	INT(fd_retry)
	V(_fd_wp,b,NUMFLOPPIES)
	V(_fd_latch,b,NUMFLOPPIES)
	V(_fd_lastacc,l,NUMFLOPPIES)
	LONG(fd_timeout)
	INT(fd_motoron)
	INT(fd_desel)
	INT(fd_dev)
	INT(fd_track)
	INT(fd_sect)
	INT(fd_side)
	INT(fd_scount)
	PTR(fd_buffer)
	INT(fd_spt)
	INT(fd_interlv)
	INT(fd_virgin)
	PTR(fd_secmap)
	INT(fd_deferr)
	INT(fd_curerr)

	V(flopsregs,l,9)

	V(dsb,b,8*NUMFLOPPIES)
	
	INT(fd_rtryseek)
	PTR(fd_frb)
	PTR(fd_copyp)
	INT(fd_cscnt)

	PTR(memtnext)
	PTR(memtaddr)
	PTR(memtbot)
	PTR(memtend)
	PTR(memttop)
	PTR(memtzone)
	LONG(memtsize)
	
	INT(memterr)

/*
 * disk_buffer plus pad for equated arrays
 * BEWARE: portions also used by VDI (the "overlay area")
 */
#include "../vdi/vdivar.S"

	ds.w 1 /* unused */

	PTR(biosav)

	ds.l 1 /* unused */

	INT(savesr)

	V(fstack,w,1034)
	PTR(fstrt)
	PTR(old_tik)
	PTR(old_2)

	LONG(_rseed)

	BYTE(_prtbdev)
	even

	BYTE(_prtboch)
	even

	INT(_msec)

	INT(_prtbbval)

	ds.b 1024
    
	V(_supstk,w,SUPSIZ)
	V(_supstkend,b,0)

	BYTE(_prtbflg)
	even
	
	BYTE(_prtbdmask)
	even
	
	INT(_time)

	V(_prtboval,w,4)

	V(_root,l,MAXQUICK)

	ds.b 60

	V(_glbkbchar,l,3*KBBUFSZ)

	PTR(_ofdlist)

	LONG(_fd_mediach)
	V(_glbcolumn,w,3)
	INT(_prtbwidth)
	
	V(_drivechange,b,NUMFLOPPIES)
	INT(_rrecno)
	INT(_prtbbytes)

	BYTE(_prtbilow)
	even
	BYTE(_prtbihigh)
	even

	INT(_rwrtflg)
	INT(_curflop)

	INT(_prtbwords)
	PTR(_prtbstrt)
	PTR(_prtbp2)
	INT(_prtbchunk)

	V(_virt_work,b,180+MU_PLANES*16*2)

	PTR(_prtbp1)
	PTR(_rdm)
	INT(_prtbgval)
	PTR(_prtbend)
	INT(_prtbpmsk)
	INT(_prtbidx)
	INT(_dirlock)
	INT(_prtbinvert)
	INT(_prtbink)

#if STBOOK
	V(_ofdbuf,b,70*80)
	LONG(_ldivr)
#else
	LONG(_ldivr)
	V(_ofdbuf,b,70*80)
#endif

	V(_bpbsums,w,BPBSECT*NUMFLOPPIES)
	INT(_oscnt)

	V(_ram8x8,b,90)

	LONG(_rwerr)
	INT(_prtbrval)
	V(_pmd,l,3)
	V(_sft,b,OPNFILES*10-6*10)

#if TOSVERSION >= 0x300
	V(_ram16x32,b,90)
#endif
	
	PTR(_run)

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
	BYTE(_osuser)
	PTR(_dirrec)
	V(_prtbval,w,16)
	V(_dirtbl,l,NCURDIR)
	INT(_prtbplanes)
	INT(_prtboffset)
	V(_errbuf,l,3)
	INT(_prtbtcol)

	BYTE(_prtbacol)
	even
	V(_prtbmaxval,w,16)
	BYTE(_prtbeps)
	even
	V(_prtbobuf,b,8)
	V(_fd_err,w,NUMFLOPPIES)
	V(_diruse,b,NCURDIR)
	V(_drvtbl,l,BLKDEVNUM)
	INT(_drvsel)
	INT(_prtbheight)
	BYTE(_prtbmono)
	even
	V(_prtbmval,w,4)
	BYTE(_prtbmed)
	even
	BYTE(_prtbempty)
	even
	LONG(_uptime)
	INT(_prtblines)
	INT(_errdrv)

	BYTE(_prtbodd)
	even

	INT(_prtbdots)
	INT(_prtbomask)
	INT(_prtbbits)
	BYTE(_prtbprez)
	even
	V(_prtbminval,w,16)
	V(_ospd,b,256)
	V(_buptr,l,3)

	BYTE(_prtblow)
	even
	BYTE(_prtbamon)
	even
    
	INT(_date)
	V(_bcbx,b,4*20)
	BYTE(_prtbbol)
	even
	V(_beptr,l,3)

/* This label is referenced by the OSHEADER */
	V(_endvdibss,b,0)

/* start AES+DESKTOP */

	PTR(retsava)
	INT(_gl_restype)
	INT(_gl_rschange)
	V(ustak,l,33)
	INT(_diskin)
	INT(dummycmd)
	INT(execmode)
	LONG(execflags)
	
	ds.b 2
	
	PTR(_ad_rso)
	PTR(_ml_mnhold)
	GRECT(_ml_ctrl)
	PTR(_ml_pmown)
	PTR(_ml_pkown)
	V(_fsname,b,40)
	V(_fcopy,b,40)
	PTR(_pathcopy)
	INT(_defdrv)
	V(_autopath,b,CMDLEN)
	V(_wtcolor,w,MAXOBJ)
	V(_wbcolor,w,MAXOBJ)
	V(pblock,l,5)
	V(_crit_err,l,8) /* _crit_err,_dk_err,savestk,_SAV2TRP,_savsup,drawsp,_ldivr,errsave */
	
#if TOSVERSION >= 0x400
	V(errstk,l,401)
#else
	V(errstk,l,257)
#endif
	
	V(savea0,b,654) /* savea0,srsave,savsr,psetsr,dspstk */
	LONG(retaddr)
	LONG(tr_retsave)
	PTR(_drwaddr)
	PTR(_tikaddr)
	PTR(gstksave)
	PTR(tstksave)
	PTR(_tiksav)
	LONG(_NUM_TICK)
	LONG(_CMP_TICK)
	LONG(TICKS)
	V(gstack,l,24)
	V(tstack,l,24)
	V(fspec,b,8)
	INT(mydev)
	PTR(oldgetbpb)
	PTR(oldmediach)
	PTR(oldrwabs)
	ds.l 1 /* unused */
	PTR(_maddr)
	V(mform,w,37)
	PTR(trp13aret)
	PTR(trp14aret)
	
	ds.b $10
	
	V(_intout,w,10)
	
	ds.w 1
	
	V(_ptsout,w,10)
	
	ds.b 10
	
	GRECT(_ctrl)
	
	ds.b $32

	INT(_gl_dabase)
	V(_gl_adacc,l,MAX_ACCS)

	ds.b $12
	
	INT(_xrat)
	INT(_yrat)
	
	ds.b 2
	
	INT(_gl_hchar)
	INT(_gl_naccs)
	INT(_gl_dabox)
	INT(_gl_dacnt)
	INT(_gl_bdelay)
	PTR(_ad_hgmice)
	V(_gl_pacc,l,MAX_ACCS)
	V(_desk_acc,l,MAX_ACCS)
	V(_ib,b,34)      /* ICONBLK */
	V(_bi,b,14)      /* BITBLK */
	INT(_gl_bclick)
	PTR(_ad_fsnames)
	INT(_gl_handle)
	PTR(_ad_armice)
	PTR(_ad_select)
	
	ds.b $A2
	
	PTR(_ad_fsel)
	INT(_gl_dclick)
	
	ds.b 2
	
	INT(_gl_height)
	TEDINFO(_gl_aname)
	
	ds.b 4
	
	PTR(_ad_shcmd)
	PTR(_ad_fpath)
	
	ds.b 2
	
	TEDINFO(_gl_ainfo)
	INT(_gl_bdesired)
	INT(_gl_dcindex)
	INT(_gl_recd)
	
	ds.b 6
	
	PTR(_ad_path)
	
	ds.b 12
	
	PTR(old_bcode)
	INT(_gl_hclip)
	INT(_gl_bvhard)
	PTR(_ad_shtail)
	PTR(_ad_stdesk)
	INT(_used_acc)
	
	ds.b 4
	
	INT(_gl_wchar)
	PTR(_ad_fsdta)
	
	ds.b 32

	PTR(_ad_windspb)
	INT(_gl_mode)
	INT(_gl_bpend)
	V(_gl_cmform,w,37) /* MFORM */
	INT(_gl_bvdisk)
	
	ds.b 2
	
	PTR(_ad_intin)
	INT(_gl_hschar)
	
	ds.b 8
	
	INT(_gl_moff)
	GRECT(_gl_rcenter)
	INT(_sh_iscart)
	V(_hdr_buf,w,18)   /* RSHDR */
	
	ds.b 4
	
	INT(_gl_graphic)
	INT(_gl_lcolor)
	
	ds.b 4
	
	PTR(old_mcode)
	INT(_gl_nplanes)
	INT(_gl_ticktime)
	
	ds.b $88
	
	INT(_DOS_AX)
	LONG(_gl_mlen)
	INT(_gl_mask)
	INT(_gl_ncol)
	
	ds.b $82
	
	INT(_w_walkflag)
	PTR(_ad_envrn)
	V(_desk_pid,w,MAX_ACCS)
	
	ds.b 10
	
	PTR(_gl_newdesk)
	INT(_wasclr)
	GRECT(_gl_rmnactv)
	
	ds.b $9e
	
	INT(_gl_wclip)
	PTR(_cart_dta)
	V(_gl_mkrect,b,12)  /* ORECT */
	PTR(_gl_awind)
	V(_g_autoboot,b,CMDLEN)
	PTR(_ad_title)
	V(_gl_ctwait,w,5) /* MOBLK */
	PTR(_gl_rbuf)
	
	ds.w 1
	
	INT(_gl_xclip)
	INT(_sh_isgem)
	
	ds.b $22
	
	INT(_gl_yclip)
	INT(_gl_hbox)
	PTR(_ad_fstree)
	V(_gl_omform,w,37) /* MFORM */
	INT(_gl_rlen)
	GRECT(_gl_rmenu)
	INT(_gl_tcolor)
	PTR(_sys_adacc)
 	V(_W_TREE,b,24*NUM_WIN)  /* OBJECT[NUM_WIN] */
	LONG(_gl_mnpid) /* BUG: should be int */
	INT(_pr_mclick)
	
	ds.b 4
	
	INT(_gl_wschar)
	
	ds.b 4
	
	PTR(_rs_global)
	LONG(_elinkoff)
	INT(_gl_play)
	
	ds.b $80
	
	INT(_gl_font)
	PTR(_gl_cowner)
	
	ds.b 2
	
	INT(_gl_width)
	INT(_sh_doexec)
	PTR(_gl_pglue)
	PTR(_ad_sysglo)
	GRECT(_gl_rfull)
	INT(_gl_patt)
	GRECT(_gl_rscreen)
	INT(_fs_first)
	V(_appl_msg,w,8)
	INT(_ml_ocnt)
	
	ds.b 10
	
	PTR(_cart_ptr)
	INT(_fs_count)
	INT(_gl_newroot)
	LONG(_fs_fnum)
	
	ds.b 4
	
	INT(_gl_btrue)
	
	ds.b 6
	
	INT(_gl_hsptschar)
	INT(_gl_hptschar)
	V(_wind_msg,w,8)
	PTR(_gl_kowner)
	INT(_crt_error)
	INT(_gl_wbox)
	PTR(_gl_mntree)
	PTR(_gl_mowner)
	
	ds.b $80
	
	PTR(_gl_wtree)
	INT(_gemsize)
	V(_wind_spb,b,10)    /* sizeof(SPB) */
	
	ds.b 6
	
	GRECT(_gl_rzero)
	INT(_gl_mouse)
	
	ds.b 2
	
	INT(_gl_nrow)
	INT(_infsize)
	
	ds.b $12
	
	INT(_tmpmoff)
	
	ds.b 2
	
	PTR(_gl_store)
	INT(_ctldown)
	INT(_autoexec)
	INT(_fs_topptr)

	ds.b 2

	INT(_gl_wtop)
	
	ds.b 6
	
	INT(_desksiz)
	PTR(_deskptr)
	
	ds.b 2
	
	INT(_gl_wsptschar)
	INT(_gl_wptschar)
	INT(_pr_xrat)
	INT(_pr_button)
	PTR(_drawstk)
	
	ds.b $1060
	
	INT(_pr_yrat)
	
	ds.b 6
	
	TEDINFO(_edblk)
	
	ds.b $22
	
	INT(_tbutton)
	
	ds.b 2
	
	INT(_nogem)
	
	ds.b 6
	
	INT(_gl_mx)
	INT(_gl_my)
	INT(_fpcnt)
	
	ds.b 8
	
	V(_gl_ws,w,57)   /* workout */
	
	ds.b $474
	
	V(_intin,w,128)
	
	ds.b 4
	
	PTR(_cda)
	INT(_wwait)
 	V(_W_ACTIVE,b,24*NUM_WIN)  /* OBJECT[MAXOBJ] */
	
	ds.b $108
	
	INT(_DOS_ERR)
	V(_ptsin,w,20)
	PTR(_dpd)
	INT(_fph)
	PTR(_dlr)
	PTR(_drl)
	
	V(_D,b,$369E) /* sizeof(THEGLO) */
	
	PTR(_eul)
	INT(_fpt)
	PTR(_nrl)
	PTR(_rlr)
	PTR(_slr)
	PTR(_rul)
	V(_evx,b,28) /* sizeof(EVB) */
	PTR(_zlr)
	
	ds.l 1
	
	INT(_g_flag)
	
	ds.b 12
	
	INT(_sh_gem)
	INT(_mclick)
	INT(_gl_fis)
	
	ds.b 4
	
	INT(_no_aes)
	
	ds.b $30
	
	PTR(_ctl_pd)
	
	ds.b 4
	
	MFDB(_gl_src)
	
	ds.b $2c
	
	MFDB(_gl_dst)
	GRECT(_gl_rfs)
	PTR(_rs_hdr)
	PTR(_gl_tmp)
	
	ds.b 16
	
	INT(_nodesk)
	
	ds.b 2
	
	INT(_er_num)
	
	ds.b 2
	
	INT(_curpid)
	BYTE(_indisp)
	even
	
	ds.b 2
	BYTE(_infork)
	even
	ds.b 2
	ds.b 2
	
	INT(_kstate)
	
	ds.b 6
	
	PTR(_gemptr)
	
	ds.b 6
	
	V(_contrl,w,12)
	
	ds.b $24
	
	PTR(_infptr)
	
	ds.b $24
	
	INT(_mtrans)
	PTR(_pxpath)
	
	ds.b $28
	
	INT(_tmpmon)
	INT(_button)
	
/* end AES+DESKTOP */

/* This label is referenced by the GEM_MUPB structure */
	V(_endgembss,b,0)

/* This label is referenced by the BIOS initialization */
	V(__ebss,b,0)
