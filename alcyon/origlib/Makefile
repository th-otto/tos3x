include ../Makefile.common
include ../Makefile.silent

all: libc.a options.a crt0.o

include SRCFILES

optoff_OPTIMIZE=0
printf_OPTIMIZE=0
fprintf_OPTIMIZE=0
doprt_OPTIMIZE=0
prtint_OPTIMIZE=0
readasc_OPTIMIZE=0
readbin_OPTIMIZE=0
xwmain_OPTIMIZE=0
malloc_OPTIMIZE=0


libc.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS3)

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

clean:
	$(RM) *.o *.a

echo:
	echo $(COBJS:.o=.c)
