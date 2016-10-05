################################  makevdi  ####################################
#
# $Revision: 3.0 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/makevdi,v $
# =============================================================================
# $Author: lozben $	$Date: 91/08/06 21:31:04 $     $Locker:  $
# =============================================================================
#
# $Log:	makevdi,v $
# Revision 3.0  91/08/06  21:31:04  lozben
# New generation VDI
# 
# Revision 2.11  91/07/26  12:08:28  lozben
# Made sbblit.s dependent on vdiincld.s.
# 
# Revision 2.10  91/07/08  16:37:19  lozben
# Added new files - sbscolog.s, sbsctlog.s sbsctran.s sbscopaq.s
# 
# Revision 2.9  90/06/13  18:06:32  lozben
# Added the -3 (68030 option) flag for the assembler.
# 
# Revision 2.8  90/03/01  12:38:02  lozben
# We now grep for comment "@check@" in relevant files before
# finishing the target. Also added "vdiincld.s" as one of the
# dependencies for some of the files.
# 
# Revision 2.7  90/02/28  11:41:19  lozben
# Fixed some typo errors.
# 
# Revision 2.6  90/02/27  16:20:53  lozben
# Made changes so that the makefile works for the ST and TT vdi.
# 
# Revision 2.5  90/02/20  12:33:18  lozben
# Added fnt16x32.c to the list of files.
# 
# Revision 2.4  90/01/29  18:40:21  lozben
# Added sbscrn.s to the list of .s files.
# 
# Revision 2.3  90/01/19  12:14:01  lozben
# Deleted sbtxt32.s from make file. It's a duplicate os sbtxtblt.s
# 
# Revision 2.2  90/01/19  11:57:15  lozben
# *** Initial Revision ***
# 
###############################################################################

include ../Makefile.common
include ../Makefile.silent
include optimize.mak

all: vdi.a vdi.o

include ../config.mak
include ../language.mak

include SRCFILES

#.s.o:
#	$(AM_V_AS)oldmas -U -S $*.s

CPPFLAGS +=

vdi.a: tosvars.o config.h $(VDI_OBJS) ${MAKEFILE}
	$(RM) $@
	$(AR) $(ARFLAGS) $@ tosvars.o $(VDI_OBJS1)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS2)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS3)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS4)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS5)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS6)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS7)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS8)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS9)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS10)
	$(AR) $(ARFLAGS) $@ $(VDI_OBJS11)

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
vars.o: vars.S config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

vdi.o: checklang tosvars.o vars.o $(STARTUP) $(VDI_OBJS) vdiext.o ${MAKEFILE}
	$(LD) -u -s -l -o $@ -a $(vdi_tbase) $(vdi_dbase) 0 tosvars.o vars.o $(VDI_OBJS) vdiext.o

vdiext.o: vdiext.S config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) config.h
