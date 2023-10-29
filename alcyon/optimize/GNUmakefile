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
LDFLAGS += -s

all: $(PROGRAMS)

include SRCFILES

optimize$(EXEEXT): $(OPTIMIZE_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(OPTIMIZE_OBJS) ${LIBS} $(LDFLAGS) $(GLIBC_SO) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
