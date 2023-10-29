top_srcdir=..
subdir=cgen

# 
# cross compiling
#
include ../GNUmakefile.cmn
include ../Makefile.sil

PROGRAMS = c168$(EXEEXT)

LIBS =

CPPFLAGS = -I../common -I$(INC)
CFLAGS = $(OPTS) $(WARN)
LDFLAGS += -s

all: $(PROGRAMS)

include SRCFILES

c168$(EXEEXT): $(C168_OBJS)
	$(AM_V_LD)$(CC) ${CFLAGS} $(C168_OBJS) ${LIBS} $(LDFLAGS) $(GLIBC_SO) -o $@

install: all
	$(CP) $(PROGRAMS) $(BIN)

check::

dist::
	$(CP) -a $(SRCS) $(EXTRA_DIST1) $(top_srcdir)/$(DISTDIR2)/$(subdir)
	$(CP) -a $(EXTRA_DIST2) $(top_srcdir)/$(DISTDIR2)/$(subdir)

clean:
	$(RM) *.o *.a $(PROGRAMS)
