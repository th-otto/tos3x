# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = c068$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

c068$(EXEEXT): $(C068_OBJS)
	$(AM_V_LD)$(LD) $(C068_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

clean:
	$(RM) *.o *.a $(PROGRAMS)
