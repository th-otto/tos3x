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
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS3)

vdidsp.o: vdidsp.S $(top_srcdir)/common/config.h $(top_srcdir)/common/sections.inc
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

bdos.o: $(top_srcdir)/common/config.h tosvars.o vars.o vdidsp.o $(top_srcdir)/common/sections.mak $(BDOS_OBJS) ${MAKEFILE}
	$(LD) -s -o $@ -a $(bdos_tbase) $(bdos_dbase) 0 tosvars.o vars.o vdidsp.o $(BDOS_OBJS)

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
	$(RM) *.o *.a *.i *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) $(top_srcdir)/common/config.h
