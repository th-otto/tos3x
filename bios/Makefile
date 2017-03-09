top_srcdir=..

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil
include optimize.mak

all: $(top_srcdir)/common/config.h bios.a

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

CPPFLAGS +=

bios.a: tosvars.o $(BIOS_OBJS) ${MAKEFILE}
	$(AR) $(ARFLAGS) $@ tosvars.o $(BIOS_OBJS)

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
startup.o: startup.S $(top_srcdir)/common/config.h header.h scrdmp.inc mmu030.inc romcrc.inc dma.inc nvram.inc ikbdclock.inc floppy.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

prt.o: prt.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

memtest.o: memtest.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

bios.o: tosvars.o vars.o $(BIOS_OBJS) biosext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	# sections.mak might have just been generated, make sure it is reread
	$(MAKE) TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY) single

blkdev.o: blkdev.c
	$(AM_V_CC)${BIN}cp68${EXEEXT} ${CPPFLAGS} $*.c $*.i
	$(AM_V_at)${BIN}c068${EXEEXT} $*.i $*.1 $*.2 $*.3 ${CFLAGS}
	$(AM_V_at)${BIN}c168${EXEEXT} $*.1 $*.2 $*.s
	$(AM_V_at)${RUN_OPTIMIZE_${$*_OPTIMIZE}}
	$(AM_V_at)$(SUBSTLMUL)
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.s
	$(AM_V_at)${RM} $*.s $*.i $*.1 $*.2 $*.3



single:
	$(LD) -u -s -l -o bios.o -a $(bios_tbase) $(bios_dbase) 0 tosvars.o vars.o $(BIOS_OBJS) biosext.o

biosext.o: biosext.S $(top_srcdir)/common/config.h $(top_srcdir)/common/sections.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

keytbl.o: $(top_srcdir)/common/config.h

prtkeys$(EXEEXT): prtkeys.o
	$(AM_V_LD)${LD} ${LDFLAGS} -o $@ prtkeys.o

check:: bios.o

clean:
	$(RM) *.o *.a *.ndx *.i *.1 *.2 *.rel *.mod $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
