include ../Makefile.common
include ../Makefile.silent
include optimize.mak

all: bdos.a bdos.o

include ../config.mak
include ../language.mak

include SRCFILES

bdos.a: checklang config.h $(BDOS_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS3)

vdidsp.o: vdidsp.S config.h
	$(AM_V_AS)${BIN}cp68${EXEEXT} ${CPPFLAGS} -P $*.S $*.i
	$(AM_V_at)${BIN}as68${EXEEXT} ${ASFLAGS} ${$*_ASFLAGS} $*.i
	$(AM_V_at)${RM} $*.i

bdos.o: checklang config.h vars.o vdidsp.o $(BDOS_OBJS) ${MAKEFILE}
	$(LD) -s -o $@ -a $(gemdos_tbase) $(gemdos_dbase) 0 ../bios/tosvars.o vars.o vdidsp.o $(BDOS_OBJS)

$(BDOS_OBJS) vars.o: config.h

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)

distclean: clean
	$(RM) config.h
