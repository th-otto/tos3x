include ../Makefile.common
include ../Makefile.silent

all::

include SRCFILES

all:: ${LIBC} ${LIBC_NDX} ${LIBM} ${LIBM_NDX} ${OPTIONOBJS} ${CRT0}

${LIBC}: $(OBJS) ${OPTIONOBJS} ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS3)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS4)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS5)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS6)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS7)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS8)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS9)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS10)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS11)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS12)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS13)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS14)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS15)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS16)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS17)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS2)

${LIBC_NDX}: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

${LIBM}: $(MOBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MCOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MCOBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MAOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MAOBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MAOBJS3)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MAOBJS4)

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${LIBC} ${LIBC_NDX} ${CRT0} $(LIB)


clean:
	$(RM) *.o *.a *.ndx
