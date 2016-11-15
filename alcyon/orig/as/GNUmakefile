top_srcdir=..
subdir=as

# 
# cross compiling
#
include ../GNUmakefile.common
include ../Makefile.silent

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = as68$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

as68$(EXEEXT): $(AS68_OBJS)
	$(AM_V_LD)$(LD) $(AS68_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
