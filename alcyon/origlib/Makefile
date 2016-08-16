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
channels_OPTIMIZE=0
writeasc_OPTIMIZE=0
writebin_OPTIMIZE=0


libc.a: $(OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS1)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(AOBJS2)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS3)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS4)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS5)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS6)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS7)
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(COBJS8)

options.a: $(OPTIONOBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ $(OPTIONOBJS)

clean:
	$(RM) *.o *.a

echo:
	echo $(COBJS:.o=.c)
