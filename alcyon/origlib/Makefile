include ../Makefile.common
include ../Makefile.silent

CRT0O=crt0.o
LIBC=libc.a
LIBO=options.a

all: ${LIBC} ${LIBO} ${CRT0O} srcfiles.sh

include SRCFILES
include optimize.mak

libc.a: $(OBJS)
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
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS8)

libc.ndx: ${LIBC}
	$(AM_V_RANLIB)$(RANLIB) $<

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

options.ndx: ${LIBO}
	$(AM_V_RANLIB)$(RANLIB) $<


srcfiles.sh: SRCFILES ${MAKEFILE}
	$(AM_V_GEN)
	@echo "#!mupfel"            > $@
	@echo AOBJS1=\"${AOBJS1}\" >> $@
	@echo AOBJS2=\"${AOBJS2}\" >> $@
	@echo COBJS1=\"${COBJS1}\" >> $@
	@echo COBJS2=\"${COBJS2}\" >> $@
	@echo COBJS3=\"${COBJS3}\" >> $@
	@echo COBJS4=\"${COBJS4}\" >> $@
	@echo COBJS5=\"${COBJS5}\" >> $@
	@echo COBJS6=\"${COBJS6}\" >> $@
	@echo COBJS7=\"${COBJS7}\" >> $@
	@echo COBJS8=\"${COBJS8}\" >> $@
	@echo OPTIONOBJS=\"${OPTIONOBJS}\" >> $@
	@echo CRT0O=\"${CRT0O}\" >> $@
	@echo LIBC=\"${LIBC}\" >> $@
	@echo LIBO=\"${LIBO}\" >> $@

install: options.ndx libc.ndx ${CRT0O}
	$(CP) ${LIBC} libc.ndx ${LIBO} options.ndx ${CRT0O} $(LIB)


clean:
	$(RM) *.o *.a *.ndx

echo:
	echo ${MAKEFILE}
