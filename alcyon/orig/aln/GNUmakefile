top_srcdir=..
subdir=aln

# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = doindex$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

doindex$(EXEEXT): $(DOINDEX_OBJS)
	$(AM_V_LD)$(LD) $(DOINDEX_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
