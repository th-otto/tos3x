include ../Makefile.common
include ../Makefile.silent

all: libf.a options.a crt0.o

include SRCFILES

libf.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

clean:
	$(RM) *.o *.a

echo:
	echo $(COBJS:.o=.c)
