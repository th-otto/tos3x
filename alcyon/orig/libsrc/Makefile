include ../Makefile.common
include ../Makefile.silent

all::

include SRCFILES
include optimize.mak

all:: ${LIBC} ${LIBC_NDX} ${LIBM} ${LIBM_NDX} ${OPTIONOBJS} ${CRT0} srcfiles.sh

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
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS1) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(AOBJS2) > NUL:

# library with all objects, including optional and runtime,
# for use by analyze tool
liba.a: ${LIBC} ${OPTIONOBJS} ${CRT0}
	cp $< $@
	$(AR) $(ARFLAGS) $@ ${CRT0} ${OPTIONOBJS1}
	$(AR) $(ARFLAGS) $@ ${OPTIONOBJS2}

${LIBC_NDX}: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

${LIBM}: $(MOBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(MCOBJS1) > NUL:
	$(AM_V_at)$(AR) $(ARFLAGS) $@ $(MCOBJS2) > NUL:

${LIBM_NDX}: ${LIBM}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${LIBC} ${LIBC_NDX} nofloat.o ${CRT0} $(LIB)


clean:
	$(RM) *.o *.a *.ndx
