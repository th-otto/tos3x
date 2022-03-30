top_srcdir=..
subdir=link68

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

CPPFLAGS = -I ../common -DGEMDOS
#CPPFLAGS += -I $(INC)
CFLAGS = $(OPTS) $(WARN)

PROGRAMS = link68$(EXEEXT) relmod$(EXEEXT) reldump$(EXEEXT)

all: $(PROGRAMS)

include SRCFILES

link68$(EXEEXT): $(LINK_OBJS)
	$(AM_V_LD)$(LD) $(LINK_OBJS) $(LDFLAGS) -o $@

relmod$(EXEEXT): $(RELMOD_OBJS)
	$(AM_V_LD)$(LD) $(RELMOD_OBJS) $(LDFLAGS) -o $@

reldump$(EXEEXT): $(RELDUMP_OBJS)
	$(AM_V_LD)$(LD) $(RELDUMP_OBJS) $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
