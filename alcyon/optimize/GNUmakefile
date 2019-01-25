top_srcdir=..
subdir=optimize

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

PROGRAMS = optimize$(EXEEXT)

LIBS =

CPPFLAGS = -I ../common
#CPPFLAGS += -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all: $(PROGRAMS)

include SRCFILES

optimize$(EXEEXT): $(OPTIMIZE_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(OPTIMIZE_OBJS) ${LIBS} $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

install64: all
	$(CP) $(PROGRAMS) ../../bin64

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
