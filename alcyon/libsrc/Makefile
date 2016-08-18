include ../Makefile.common
include ../Makefile.silent

all::

include SRCFILES

all:: ${LIBM} ${LIBM_NDX} ${OPTIONOBJS} ${CRT0}

${LIBM}: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

clean:
	$(RM) *.o *.a *.ndx
