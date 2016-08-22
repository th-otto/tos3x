# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = optimize$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

optimize$(EXEEXT): $(OPTIMIZE_OBJS)
	$(AM_V_LD)$(LD) $(OPTIMIZE_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

clean:
	$(RM) *.o *.a $(PROGRAMS)
