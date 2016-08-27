# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = link68$(EXEEXT) relmod$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

link68$(EXEEXT): $(LINK_OBJS)
	$(AM_V_LD)$(LD) $(LINK_OBJS) $(LDFLAGS) -o $@

relmod$(EXEEXT): $(RELMOD_OBJS)
	$(AM_V_LD)$(LD) $(RELMOD_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

clean:
	$(RM) *.o *.a $(PROGRAMS)
