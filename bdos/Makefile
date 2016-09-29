include ../Makefile.common
include ../Makefile.silent
include optimize.mak

all: vars.o bdos.a bdos.o

include ../config.mak
include ../language.mak

include SRCFILES

bdos.a: checklang $(BDOS_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS3)

bdos.o: checklang vars.o vdidsp$(COUNTRY).o $(BDOS_OBJS) ${MAKEFILE}
	$(LD) -s -o $@ -a $(gemdos_tbase) $(gemdos_dbase) 0 ../bios/tosvars.o vars.o vdidsp$(COUNTRY).o $(BDOS_OBJS)

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
