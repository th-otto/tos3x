top_srcdir=..
subdir=command

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = command.tos

all: $(PROGRAMS)

include SRCFILES

command.tos: $(COMA_OBJS)
	$(AM_V_LD)$(LD) $(COMA_OBJS) $(LDFLAGS) -o $@

coma.o: coma.S
	../../../bin/as68 -u -l $<

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i ../bin/$$i; done

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
