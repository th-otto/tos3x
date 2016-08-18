include ../Makefile.common
include ../Makefile.silent

all::

include SRCFILES
include optimize.mak

all:: ${LIBC_NDX} ${LIBM_NDX} ${OPTIONOBJS} ${CRT0} srcfiles.sh

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
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MOBJS2)

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${LIBC} ${LIBC_NDX} ${CRT0} $(LIB)


clean:
	$(RM) *.o *.a *.ndx

echo:
	echo ${MAKEFILE}
