include ../Makefile.common
include ../Makefile.silent

all: libf.a libf.ndx options.a options.ndx crt0.o

include SRCFILES

libf.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

libf.ndx: libf.a
	$(AM_V_RANLIB)$(RANLIB) $<

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

options.ndx: options.a
	$(AM_V_RANLIB)$(RANLIB) $<

clean:
	$(RM) *.o *.a *.ndx

echo:
	echo $(COBJS:.o=.c)
