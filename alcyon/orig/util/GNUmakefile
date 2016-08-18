# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = size68$(EXEEXT) nm68$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS)
	$(AM_V_LD)$(LD) $(SIZE_OBJS) $(LDFLAGS) -o $@

nm68$(EXEEXT): $(NM_OBJS)
	$(AM_V_LD)$(LD) $(NM_OBJS) $(LDFLAGS) -o $@

clean:
	$(RM) *.o *.a $(PROGRAMS)
