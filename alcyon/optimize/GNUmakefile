top_srcdir=..
subdir=optimize

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

PROGRAMS = optimize$(EXEEXT)

LIBS =

CPPFLAGS = -I ../common -I $(INC)
CFLAGS = $(OPTS) $(WARN)

all: $(PROGRAMS)

include SRCFILES

optimize$(EXEEXT): $(OPTIMIZE_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(OPTIMIZE_OBJS) ${LIBS} $(LDFLAGS) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST) $(top_srcdir)/$(DISTDIR)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
