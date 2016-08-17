include ../Makefile.common
include ../Makefile.silent

PROGRAMS = size68$(EXEEXT)
LIBS = $(LIB)options.a $(LIB)libc.a

all: size68

include SRCFILES

size68: $(SIZE_OBJS)
	$(AM_V_LD)$(LD) $(LDFLAGS) -o $@$(EXEEXT) $(CRT0) $(SIZE_OBJS) $(LIBS)

clean:
	$(RM) *.o *.a *.s *.i *.1 *.2 $(PROGRAMS)
