top_srcdir=..
subdir=link68

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../common -I $(INC) -DGEMDOS
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = link68$(EXEEXT) relmod$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

link68$(EXEEXT): $(LINK_OBJS)
	$(AM_V_LD)$(LD) $(LINK_OBJS) $(LDFLAGS) -o $@

relmod$(EXEEXT): $(RELMOD_OBJS)
	$(AM_V_LD)$(LD) $(RELMOD_OBJS) $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
