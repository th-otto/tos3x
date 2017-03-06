top_srcdir=..

include $(top_srcdir)/Makefile.cmn
include $(top_srcdir)/Makefile.sil
include optimize.mak

all: $(top_srcdir)/common/config.h aes.a aes.o

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

#.s.o:
#	$(AM_V_AS)oldmas -U -S $*.s

CPPFLAGS +=

STARTUP =	gemjstrt.o

aes.a: $(STARTUP) $(AES_OBJS) ${MAKEFILE}
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(STARTUP) $(AES_OBJS1)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS2)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS3)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS4)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS5)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS6)
	$(AR) $(ARFLAGS) $@ $(AES_OBJS7)

aes.o: vars.o $(STARTUP) $(AES_OBJS) aesext.o $(top_srcdir)/common/sections.mak ${MAKEFILE}
	# sections.mak might have just been generated, make sure it is reread
	$(MAKE) TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY) single

single:
	$(LD) -u -s -l -o aes.o -a $(aes_tbase) $(aes_dbase) 0 vars.o $(STARTUP) $(AES_OBJS) aesext.o

#
# need to repeat the default suffix rule here,
# because on a FAT filesystem, .S cannot be
# distinguished from .s
#
gemjstrt.o: gemjstrt.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

apgrlib.o: apgrlib.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

apgsxif.o: apgsxif.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

gemgrlib.o: gemgrlib.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

gemgsxif.o: gemgsxif.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

gsx2.o: gsx2.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

jbind.o: jbind.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

jdispa.o: jdispa.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

jdos.o: jdos.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

large.o: large.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

optimize.o: optimize.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

ratrbp.o: ratrbp.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

trap14.o: trap14.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i



vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

aesext.o: aesext.S $(top_srcdir)/common/sections.inc $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

check::

clean:
	$(RM) *.o *.a *.ndx *.i *.1 *.2 *.rel *.mod $(PROGRAMS) $(top_srcdir)/common/config.h

distclean: clean
	$(RM) $(top_srcdir)/common/sections.mak
