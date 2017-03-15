include ../Makefile.cmn
include ../Makefile.sil

all::

include SRCFILES

all:: ${GEMLIB} ${GEMLIB_NDX} gemstart.o

${GEMLIB}: $(OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

${GEMLIB_NDX}: ${GEMLIB}
	$(AM_V_RANLIB)$(RANLIB) $<

install: all
	$(CP) ${GEMLIB} ${GEMLIB_NDX} gemstart.o $(LIB)

clean:
	$(RM) *.o *.a *.ndx
