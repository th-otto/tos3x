include ../Makefile.cmn
include ../Makefile.sil

all::

include SRCFILES

all:: ${LIBC} ${LIBC_NDX} ${LIBM} ${LIBM_NDX} ${OPTIONOBJS} ${CRT0}

${LIBC}: $(OBJS) ${OPTIONOBJS} ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS3)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS4)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS5)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS6)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS7)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS8)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS9)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS10)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS11)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS12)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS13)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS14)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS15)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS16)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS17)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(COBJS18)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS2)

${LIBC_NDX}: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

${LIBM}: $(MOBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MCOBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MCOBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS1)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS2)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS3)
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MAOBJS4)

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${LIBC} ${LIBC_NDX} ${CRT0} nofloat.o $(LIB)


clean:
	$(RM) *.o *.a *.ndx

strtol.ttp: strtol.o libc.ndx
	aln -u -o $@ crt0.o strtol.o libc.a
