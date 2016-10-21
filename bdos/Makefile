top_srcdir=..

include $(top_srcdir)/Makefile.common
include $(top_srcdir)/Makefile.silent
include optimize.mak

all: bdos.a bdos.o

include $(top_srcdir)/config.mak
-include $(top_srcdir)/common/sections.mak

include SRCFILES

bdos.a: $(top_srcdir)/common/config.h $(BDOS_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_$(TOSVERSION)_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_$(TOSVERSION)_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_$(TOSVERSION)_OBJS3)

vdidsp.o: vdidsp.S $(top_srcdir)/common/config.h $(top_srcdir)/common/sections.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

osinit.o: osinit.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

lmul.o: lmul.S
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

bdos.o: $(top_srcdir)/common/config.h tosvars.o vars.o vdidsp.o $(top_srcdir)/common/sections.mak $(BDOS_OBJS) ${MAKEFILE}
	# sections.mak might have just been generated, make sure it is reread
	$(MAKE) TOSVERSION=$(TOSVERSION) COUNTRY=$(COUNTRY) single

single:
	$(LD) -s -l -o bdos.o -a $(bdos_tbase) $(bdos_dbase) 0 tosvars.o vars.o vdidsp.o $(BDOS_OBJS)

vars.o: $(top_srcdir)/common/vars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

tosvars.o: $(top_srcdir)/common/tosvars.S $(top_srcdir)/common/config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $< $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i $@
	$(AM_V_at)${RM} $*.i

check::

clean:
	$(RM) *.o *.a *.i *.i *.1 *.2 *.rel *.mod $(PROGRAMS) $(top_srcdir)/common/config.h

maps:
	$(MAKE) clean
	$(MAKE) TOSVERSION=306 COUNTRY=de
	cnm -g bdos.o > tos306de.map
	$(MAKE) clean
	$(MAKE) TOSVERSION=306 COUNTRY=us
	cnm -g bdos.o > tos306us.map
	$(MAKE) clean

distclean: clean
