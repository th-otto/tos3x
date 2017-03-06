top_srcdir=..
subdir=cp68

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = cp68$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

cp68$(EXEEXT): $(CP_OBJS)
	$(AM_V_LD)$(LD) $(CP_OBJS) $(LDFLAGS) -o $@

check::
	$(AM_V_at)for i in $(PROGRAMS); do cmp $$i.ttp ../bin/$$i.ttp; done

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
