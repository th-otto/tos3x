include ../Makefile.common
include ../Makefile.silent
include optimize.mak

include bdos.lng

all: vars.o bdos.a bdos.o

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS3)

bdos.o: vars.o $(BDOS_OBJS) ${MAKEFILE}
	$(LD) -s -u -o $@ -a $(gemdos_tbase) $(gemdos_dbase) 0 ../bios/tosvars.o vars.o $(BDOS_OBJS)

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
