include ../Makefile.cmn
include ../Makefile.sil

all::

include SRCFILES

all:: ${LIBC} ${LIBC_NDX} ${LIBM} ${LIBM_NDX} ${OPTIONOBJS} ${CRT0}

${LIBC}: $(OBJS) ${OPTIONOBJS} ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS1) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS2) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS3) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS4) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS5) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS6) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS7) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS8) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS9) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS10) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS11) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS12) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS13) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS14) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS15) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS16) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS17) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS18) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS1) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS2) > NUL:

${LIBC_NDX}: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

${LIBM}: $(MOBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MCOBJS1) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MCOBJS2) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS1) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS2) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS3) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS4) > NUL:

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${LIBC} ${LIBC_NDX} ${CRT0} nofloat.o $(LIB)


clean:
	$(RM) *.o *.a *.ndx

strtol.ttp: strtol.o libc.ndx
	aln -u -o $@ crt0.o strtol.o libc.a
