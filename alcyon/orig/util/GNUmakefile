# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = size68$(EXEEXT) nm68$(EXEEXT) ar68$(EXEEXT)

all: $(PROGRAMS) getarhd.o

include SRCFILES

size68$(EXEEXT): $(SIZE_OBJS)
	$(AM_V_LD)$(LD) $(SIZE_OBJS) $(LDFLAGS) -o $@

nm68$(EXEEXT): $(NM_OBJS)
	$(AM_V_LD)$(LD) $(NM_OBJS) $(LDFLAGS) -o $@

ar68$(EXEEXT): $(AR_OBJS)
	$(AM_V_LD)$(LD) $(AR_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

clean:
	$(RM) *.o *.a $(PROGRAMS)
