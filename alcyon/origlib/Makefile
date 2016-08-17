include ../Makefile.common
include ../Makefile.silent

all::

include SRCFILES
include optimize.mak

all:: ${LIBC} ${OPTIONOBJS} ${CRT0} srcfiles.sh

${LIBC}: $(OBJS) ${OPTIONOBJS}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS3)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS4)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS5)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS6)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS7)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS8) nofloat.o

${LIBC_NDX}: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

install: ${LIBC_NDX} ${OPTIONOBJS} ${CRT0}
	$(CP) ${LIBC} ${LIBC_NDX} ${OPTIONOBJS} ${CRT0} $(LIB)


clean:
	$(RM) *.o *.a *.ndx

echo:
	echo ${MAKEFILE}
