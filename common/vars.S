/*
 * common variables
 *
 * This file is used when linking a single module,
 * to get the same addresses as when linking the whole ROM.
 *
 * It is now also used when linking the final ROM, when
 * you want to create an exact binary image of an official
 * ROM version. This is because, due to the way the linker works,
 * it is impossible to get the same order of symbols in the bss section.
 *
 * This file is for version 2.x/3.x only. For 1.x, see vars1x.S.
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

	ds.b 64         /* used by ST-Book TOS by the cartridge ROM */

#if STBOOK
	V(padmagic,l,1)		/* $01241960 for STPAD touch driver */
	V(padstuff,l,40)		/* other pad variables (total 160 bytes) */
#endif

	V(gem_env,b,12)

/* save area for trap13/14. */
	V(save_beg,b,238)
	V(_trap_save_area,b,0)         /* End of Save storage */

	V(oshdr,b,6+64)

#if TOSVERSION >= 0x106
	V(cookies,l,NCOOKIES*2)
#endif

#if TOSVERSION >= 0x200
	INT(_bootpref)					/* boot pref for _dmaboot */
#endif

#if (TOSVERSION >= 0x400)
	V(dspin,b,1)		/* spinup delay before _dmaboot */
	V(iscan,b,1)		/* IDE scan mask for _dmaboot */
	V(sscan,b,1)		/* SCSI scan mask for _dmaboot */
	V(ascan,b,1)		/* ACSI scan mask for _dmaboot */
#endif

#if (TOSVERSION >= 0x300) | TP_50
	V(_altmd,l,4)
#endif

#if (TOSVERSION >= 0x200)
	BYTE(_has_dmasound)
	BYTE(STEFlag)
#endif

	xdef __sbss
__sbss:	ds.b 0 /* end of "base" BSS; start of the BSS area that is zeroed by BIOS */

#if (TOSVERSION >= 0x200)
	V(privinst,l,2)
#endif

	PTR(savepc)
	PTR(execpath)
	PTR(execname)
	V(execdta,b,44)
	V(execfname,b,32)

	V(_pbdef,b,30)

	LONG(lastacstm)

#if (TOSVERSION >= 0x200)
	V(scsicmd,b,10)

	V(dmarwflg,b,1)
	even
	
	V(idebuf,b,SECTOR_SIZE)
#endif

	ds.l 1 /* unused */

#if MSTECLK
	V(rtcbufa,b,13)
	V(rtcbufb,b,13)
	LONG(waketimeAlarm)
#endif

#if TOSVERSION >= 0x200
	V(rs232ibuf,b,SERIAL_BUFSIZE)
	V(rs232obuf,b,SERIAL_BUFSIZE)
	V(rs232iorec,b,2*14+8)
#endif

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

#if TOSVERSION >= 0x200
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
#endif

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

#if TOSVERSION < 0x200
	V(rs232ibuf,b,SERIAL_BUFSIZE)
	V(rs232obuf,b,SERIAL_BUFSIZE)
	V(rs232iorec,b,2*14+8)
#endif

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

	V(fstack,w,1034+2)
	PTR(old_tik)
	PTR(old_2)

	LONG(_rseed)

	BYTE(_prtbdev)
	even

	BYTE(_prtboch)
	even

	INT(_msec)

	INT(_prtbbval)

	ds.b 1024 /* unused? $00004b58 */
    
	V(_supstk,w,SUPSIZ)

	BYTE(_prtbflg)
	even
	
	BYTE(_prtbdmask)
	even
	
	INT(_time)

	V(_prtboval,w,4)

	V(_root,l,MAXQUICK)

	ds.b 60 /* unused? $0000577A */

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
	V(_bcbx,b,4*20) /* 4*sizeof(BCB) */
	BYTE(_prtbbol)
	even
	V(_beptr,l,3)

/* This label is referenced by the OSHEADER */
	xdef _endvdibss
_endvdibss: ds.b 0

/* start AES+DESKTOP */

	PTR(retsava)
	INT(_gl_restype)
	INT(_gl_rschange)
	V(ustak,l,33)
	INT(_diskin)
	INT(dummycmd)
	INT(execmode)
	LONG(execflags)
	
	ds.b 2 /* unused? $000083ac */
	
	PTR(_ad_rso)
	PTR(_ml_mnhold)
	GRECT(_ml_ctrl)
	PTR(_ml_pmown)
	PTR(_ml_pkown)
	V(_fsname,b,40)
	V(_fcopy,b,40)
	PTR(_pathcopy)
	INT(_defdrv)
	V(_aautopath,b,CMDLEN)
	V(_wtcolor,w,MAXOBJ)
	V(_wbcolor,w,MAXOBJ)
	V(pblock,l,5)
	LONG(_crit_err)
	LONG(_dk_err)
	LONG(savestk)
	LONG(_SAV2TRP)
	LONG(_savsup)
	LONG(drawsp)
	LONG(_aesldivr)
	LONG(errsave)
	
#if TOSVERSION >= 0x400
	V(errstk,l,401)
#else
	V(errstk,l,257)
#endif
	
	V(savea0,l,1)
	V(srsave,w,1)
	V(savsr,w,1)
	V(psetsr,w,1)
	V(dspstk,l,161)

	LONG(retaddr)
	LONG(tr_retsave)
	PTR(_drwaddr)
	PTR(_tikaddr)
	PTR(gstksave)
	PTR(tstksave)
	PTR(_tiksav)
	LONG(_NUM_TICK)
	LONG(_CMP_TICK)
	LONG(_TICKS)
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
	INT(_w_inc)
	INT(_bar_max)
	PTR(saveret)
	GRECT(_fobj)
	V(_intout,w,10)
	INT(_xprint)
	V(_ptsout,w,10)
	INT(_m_st)
	GRECT(_full)
	GRECT(_ctrl)
	V(_temp,b,50) /* unused */
	INT(_gl_dabase)
	V(_gl_adacc,l,MAX_ACCS)
	V(_winp,w,3)
	V(_rets,w,6) /* unused */
	INT(_xrat)
	INT(_yrat)
	INT(_s_cache)
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
	V(_g_buffer,b,160)
	INT(_s_defdir)
	PTR(_ad_fsel)
	INT(_gl_dclick)
	INT(_cdele_save)
	INT(_gl_height)
	TEDINFO(_gl_aname)
	PTR(_ad_pfile) /* unused */
	PTR(_ad_shcmd)
	PTR(_ad_fpath)
	INT(_gl_apid)
	TEDINFO(_gl_ainfo)
	INT(_gl_bdesired)
	INT(_gl_dcindex)
	INT(_gl_recd)
	INT(_q_change)
	PTR(_gh_buffer)
	PTR(_ad_path)
	V(_chxcache,l,2) /* USERBLK */
	PTR(_iconaddr)
	PTR(old_bcode)
	INT(_gl_hclip)
	INT(_gl_bvhard)
	PTR(_ad_shtail)
	PTR(_ad_stdesk)
	INT(_used_acc)
	INT(_cbit_save)
	INT(_back_up)
	INT(_gl_wchar)
	PTR(_ad_fsdta)
	V(_dr,b,32)
	PTR(_ad_windspb)
	INT(_gl_mode)
	INT(_gl_bpend)
	V(_gl_cmform,w,37) /* MFORM */
	INT(_gl_bvdisk)
	INT(_f_rename)
	PTR(_ad_intin)
	INT(_gl_hschar)
	PTR(_background)
	PTR(_menu_addr)
	INT(_gl_moff)
	GRECT(_gl_rcenter)
	INT(_sh_iscart)
	V(_hdr_buff,w,18)   /* RSHDR */
	INT(_d_maxcolor)
	INT(_d_level)
	INT(_gl_graphic)
	INT(_gl_lcolor)
	INT(_f_abort)
	INT(_f_level)
	PTR(old_mcode)
	INT(_gl_nplanes)
	INT(_gl_ticktime)
	V(_path1,b,PATHLEN)
	GRECT(_r_dicon)
	INT(_DOS_AX)
	LONG(_gl_mlen)
	INT(_gl_mask)
	INT(_gl_ncols)
	V(_path2,b,PATHLEN)
	INT(_d_display)
	INT(_w_walkflag)
	PTR(_ad_envrn)
	V(_desk_pid,w,MAX_ACCS)
	PTR(_winhead)
	PTR(_path3)
	INT(_pref_save)
	PTR(_gl_newdesk)
	INT(_wasclr)
	GRECT(_gl_rmnactv)
	V(_pglobal,w,15)
	V(_inf_path,b,PATHLEN)
	INT(_gl_wclip)
	PTR(_cart_dta)
	V(_gl_mkrect,b,12)  /* ORECT */
	PTR(_gl_awind)
	V(_g_autoboot,b,CMDLEN)
	PTR(_ad_title)
	V(_gl_ctwait,w,5) /* MOBLK */
	PTR(_gl_rbuf)
	INT(_p_timedate)
	INT(_gl_xclip)
	INT(_sh_isgem)
	PTR(_appfree)
	V(_mentable,b,MAXMENU)
	even
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
	PTR(_appnode)
	INT(_gl_wschar)
	PTR(_iconmem)
	PTR(_rs_global)
	LONG(_elinkoff)
	INT(_gl_play)
	V(_comtail,b,PATHLEN)
	INT(_gl_font)
	PTR(_gl_cowner)
	INT(_font_save)
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
	PTR(_newbuff)
	V(_restable,b,6)
	PTR(_cart_ptr)
	INT(_fs_count)
	INT(_gl_newroot)
	LONG(_fs_fnum)
	INT(_maxicon)
	INT(_x_index)
	INT(_gl_btrue)
	INT(_ccopy_save)
	LONG(_numfiles)
	INT(_gl_hsptschar)
	INT(_gl_hptschar)
	V(_wind_msg,w,8)
	PTR(_gl_kowner)
	INT(_crt_error)
	INT(_gl_wbox)
	PTR(_gl_mntree)
	PTR(_gl_mowner)
	V(_autofile,b,PATHLEN)
	PTR(_gl_wtree)
	INT(_gemsize)
	V(_wind_spb,b,10)    /* sizeof(SPB) */
	PTR(_lp_start)
	INT(_s_fullpath)
	GRECT(_gl_rzero)
	INT(_gl_mouse)
	INT(_s_stofit)
	INT(_gl_nrows)
	INT(_infsize)
	INT(_numicon)
	INT(_sh_up) /* unused */
	V(_filestr,b,NAMELEN)
	INT(_tmpmoff)
	INT(_windspec)
	PTR(_gl_store)
#if AESVERSION >= 0x200
	INT(_ctldown)
#endif
	INT(_autoexec)
	INT(_fs_topptr)
	INT(_d_nrows)
	INT(_gl_wtop)
	INT(_write_save)
	PTR(_applist)
	INT(_desksize)
	PTR(_deskptr)
	INT(_o_status)
	INT(_gl_wsptschar)
	INT(_gl_wptschar)
	INT(_pr_xrat)
	INT(_pr_button)
	PTR(_drawstk)
#if TP_47 /* SHBUF */
	PTR(_afile)
#else
	V(_afile,b,SIZE_AFILE)
#endif
	INT(_pr_yrat)
	INT(_d_dir)
	LONG(_numdirs)
	TEDINFO(_edblk)
	V(_pxyarray,w,10) /* unused */
	INT(_x_status)
	LONG(_tolsize)
	GRECT(_dicon)
	INT(_tbutton)
	INT(_m_cpu)
	INT(_nogem)
	V(_deskp,w,3)
	INT(_gl_mx)
	INT(_gl_my)
	INT(_fpcnt)
	PTR(_o_win)
	PTR(_cpbox)
	V(_gl_ws,w,57)   /* workout */
	INT(_i_status) /* unused */
	INT(_x_cur)
	V(_winpd,b,142*MAXWIN) /* DESKWIN[MAXWIN] */
	V(_intin,w,128)
	PTR(_x_win)
	PTR(_cda)
	INT(_wwait)
 	V(_W_ACTIVE,b,24*MAXOBJ)  /* OBJECT[MAXOBJ] */
	INT(_DOS_ERR)
	V(_ptsin,w,20)
	PTR(_dpd)
	INT(_fph)
	PTR(_dlr)
	PTR(_drl)
	
	V(_D,b,13982) /* sizeof(THEGLO) */
#if TP_WINX
	xdef _winxvars
_winxvars equ _D+$50  /* near the bottom of the SCRENMGR supervisor stack */
#endif

	PTR(_eul)
	INT(_fpt)
	PTR(_nrl)
	PTR(_rlr)
	PTR(_slr)
	PTR(_rul)
	V(_evx,b,28) /* sizeof(EVB) */
	PTR(_zlr)
	PTR(_backid)
	INT(_g_flag)
	PTR(_q_addr)
	PTR(_d_path)
	PTR(_lp_end)
	INT(_sh_gem)
	INT(_mclick)
	INT(_gl_fis)
	PTR(_lp_mid)
	INT(_no_aes)
	V(_dtabuf,b,44+4)
	PTR(_ctl_pd)
	INT(_rename)
	INT(_opcode)
	MFDB(_gl_src)
	V(_q_path,b,40) /* unused */
	INT(_o_item)
	INT(_d_exit)
	MFDB(_gl_dst)
	GRECT(_gl_rfs)
	PTR(_rs_hdr)
	MFDB(_gl_tmp)
	INT(_nodesk)
	INT(_srcbuf)
	INT(_er_num)
	INT(_srclen)
	INT(_curpid)
	BYTE(_indisp)
	even
	INT(_dstbuf)
	BYTE(_infork)
	even
	INT(_dstlen)
	INT(_apsize)
	INT(_kstate)
	INT(_s_view)
	PTR(_fixsrc)
	PTR(_gemptr)
	INT(_o_type)
	PTR(_fixdst)
	V(_contrl,w,12)
	V(_d_xywh,w,18)
	PTR(_infptr)
	V(_f_xywh,w,18)
	INT(_mtrans)
	PTR(_pxpath)
	V(_warray,l,MAXWIN)
	INT(_x_type)
	INT(_s_sort)
	PTR(_ww_win)
	INT(_tmpmon)
	INT(_button)
	
/* end AES+DESKTOP */

/* This label is referenced by the GEM_MUPB structure */
	xdef _endgembss
_endgembss:	ds.b 0

/* This label is referenced by the BIOS initialization */
	xdef __ebss
__ebss:	ds.b 0
	ds.b 0
