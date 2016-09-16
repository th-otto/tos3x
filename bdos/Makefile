include ../Makefile.common
include ../Makefile.silent
include optimize.mak

LIBS = ${LIBC}

LANG=de

gemdos_tbase_de=e134e4
gemdos_dbase_de=e3d8da

gemdos_tbase=$(gemdos_tbase_$(LANG))
gemdos_dbase=$(gemdos_dbase_$(LANG))

all: bdos.a bdos.o

include SRCFILES

bdos.a: $(BDOS_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(BDOS_OBJS3)

bdos.o: $(BDOS_OBJS) ${MAKEFILE}
	$(LD) -s -u -o $@ -a $(gemdos_tbase) x 0 ../bios/tosvars.o $(BDOS_OBJS)

check::

clean:
	$(RM) *.o *.a *.i *.1 *.2 *.rel *.mod $(PROGRAMS)
