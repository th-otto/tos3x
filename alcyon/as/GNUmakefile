top_srcdir=..
subdir=as

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

PROGRAMS = as68$(EXEEXT)

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)
LDFLAGS += -s

all: $(PROGRAMS)

include SRCFILES

as68$(EXEEXT): $(AS68_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(AS68_OBJS) ${LIBS} $(LDFLAGS) $(GLIBC_SO) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
