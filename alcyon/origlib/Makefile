include ../Makefile.common
include ../Makefile.silent

all: klib.a crt0.o

include SRCFILES

optoff_OPTIMIZE=0

klib.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ blivot.o

clean:
	$(RM) *.o *.a

echo:
	echo $(COBJS:.o=.c)
