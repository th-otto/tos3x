include ../Makefile.common
include ../Makefile.silent

all: libf.a crt0.o

include SRCFILES

libf.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	$(RM) *.o *.a

echo:
	echo $(COBJS:.o=.c)
